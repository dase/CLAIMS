/*
    Copyright 2011, Spyros Blanas.

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

#include <algorithm>
#include "hashtable.h"

#include <malloc.h>
#define MAX(x, y) (x > y ? x : y)

//#define __MOTHER_PAGE__

//#define __PRE_ALLOCATED__

#define _DEBUG_

unsigned BasicHashTable::number_of_instances_ = 0;

BasicHashTable::BasicHashTable(unsigned nbuckets, unsigned bucksize,
                               unsigned tuplesize,
                               unsigned expected_number_of_visiting_thread)
    : nbuckets_(nbuckets), tuplesize_(tuplesize) {
  bucksize_ = (get_aligned_space(MAX(bucksize, tuplesize)));

#ifdef CONTENTION_REDUCTION
  expected_number_of_visiting_thread_ = expected_number_of_visiting_thread;
  grandmothers = new pool<>* [expected_number_of_visiting_thread_];
  for (unsigned i = 0; i < expected_number_of_visiting_thread_; i++) {
    grandmothers[i] = new pool<>(bucksize_);
  }
  grandmother_lock_ = new SpineLock[expected_number_of_visiting_thread_];
#else
  grandmother = new pool<>(bucksize_);
#endif

#ifdef _DEBUG_
  number_of_instances_++;
  allocate_count = 0;
#endif
  try {
#ifdef _DEBUG_
    overflow_count_ = new unsigned[nbuckets];
    memset(overflow_count_, 0, nbuckets * sizeof(unsigned));
#endif
    /** create spinlocks, each of which corresponds to a single bucket*/
    lock_list_ = new SpineLock[nbuckets];

    bucket_ = (void**)new char[sizeof(void*) * nbuckets];
    memset(bucket_, 0, sizeof(void*) * nbuckets);
    buck_actual_size_ = bucksize_ - 2 * sizeof(void*);
    //		pagesize_=(unsigned long)nbuckets*bucksize_<16*1024*(unsigned
    // long)1024?(unsigned long)nbuckets*bucksize_:16*1024*(unsigned long)1024;
    pagesize_ = 1024 * 1024 * 4;
//		printf("Page size:%d\n",pagesize_);
#ifdef __MOTHER_PAGE__
    char* cur_mother_page = (char*)memalign(PAGE_SIZE, pagesize_);
    assert(cur_mother_page);
    t_start_ = cur_mother_page;

    /*
     * It's beneficial in IBM X5 to reset the newly allocated memory.
     * TODO: check whether other servers benefit from this.
     * Finished TODO. --Dec 31th by li
     * memset should not be used in our new machines.
     */
    memset(cur_mother_page, 0, pagesize_);
    assert(cur_mother_page);
    cur_MP_ = 0;
    mother_page_list_.push_back(cur_mother_page);
#endif

#ifdef __PRE_ALLOCATED__
    for (unsigned i = 0; i < nbuckets; i++) {
#ifdef __MOTHER_PAGE__
      if (bucksize_ + cur_MP_ >= pagesize_)  // the current mother page doesn't
                                             // have enough space for the new
                                             // buckets
      {
        cur_mother_page = (char*)memalign(PAGE_SIZE, pagesize_);
        assert(cur_mother_page);

        // TODO: as mentioned above.
        memset(cur_mother_page, 0, pagesize_);

        cur_MP_ = 0;
        mother_page_list_.push_back(cur_mother_page);
      }
      bucket_[i] = (void*)(cur_mother_page + cur_MP_);
      cur_MP_ += bucksize_;
#else
//			bucket_[i]=malloc(bucksize_);
//			bucket_[i]=(char*)memalign(256,bucksize_);
//			bucket_[i]=(char*)grandmother.malloc();
//			memset(bucket_[i],0,bucksize_);
//			mother_page_lock_.lock();
//			allocate_count++;
//			allocated_buckets.insert(bucket_[i]);
//			mother_page_lock_.unlock();
#endif

      void** free_p = (void**)(((char*)bucket_[i]) + buck_actual_size_);
      void** next_p =
          (void**)(((char*)bucket_[i]) + buck_actual_size_ + sizeof(void*));
      *next_p = 0;
      *free_p = bucket_[i];
    }
#endif
  } catch (exception& e) {
    cout << "failed" << endl;
    cout << "EXCEPTION: Standard exception: " << e.what() << endl;
  }
}

BasicHashTable::~BasicHashTable() {
  number_of_instances_--;
//	printf("free hash table :%lx\n",this);
//	report_status();
#ifdef __MOTHER_PAGE__
  for (int i = 0; i < mother_page_list_.size(); i++) {
    free(mother_page_list_.at(i));
  }
#else
//	return;
//	unsigned long free_count=0;
//	for(int i=0;i<nbuckets_;i++){
//		void* next_bucket=bucket_[i];
//		while(next_bucket!=0){
//			void* cur_bucket=next_bucket;
//			next_bucket=*(void**)(((char*)next_bucket) +
// buck_actual_size_ + sizeof(void*));
//			free(cur_bucket);
//			free_count++;
//			assert(allocated_buckets.erase(cur_bucket)==1);
//		}
//	}
//	printf("%ld allocate, %ld free\n",allocate_count,free_count);
//	assert(allocated_buckets.empty());
#ifdef CONTENTION_REDUCTION
  for (unsigned i = 0; i < expected_number_of_visiting_thread_; i++) {
    grandmothers[i]->purge_memory();
    delete grandmothers[i];
  }
  delete[] grandmothers;
  delete[] grandmother_lock_;
#else
  grandmother->purge_memory();
  delete grandmother;
#endif
#endif
  delete[] bucket_;
  delete[] lock_list_;
  delete[] overflow_count_;
}

BasicHashTable::Iterator::Iterator()
    : buck_actual_size(0), tuplesize(0), cur(0), next(0), free(0) {}
BasicHashTable::Iterator::Iterator(const unsigned& buck_actual_size,
                                   const unsigned& tuplesize)
    : buck_actual_size(buck_actual_size),
      tuplesize(tuplesize),
      cur(0),
      next(0),
      free(0) {}

void* BasicHashTable::allocate(const unsigned& offset, unsigned thread_id) {
  //	assert(offset<nbuckets_);

  void* data = bucket_[offset];
  void* ret;
  if (data != 0) {
    void** freeloc = (void**)((char*)data + buck_actual_size_);

    if ((char*)(*freeloc) + tuplesize_ <= ((char*)data + buck_actual_size_)) {
      ret = *freeloc;
      *freeloc = ((char*)(*freeloc)) + tuplesize_;
      //			assert(ret!=0);
      return ret;
    }
  }
//	return 0;
#ifdef __MOTHER_PAGE__
  mother_page_lock_.acquire();
  char* cur_mother_page = mother_page_list_.back();
  if (bucksize_ + cur_MP_ >= pagesize_)  // the current mother page doesn't have
                                         // enough space for the new buckets
  {
    cur_mother_page = (char*)memalign(PAGE_SIZE, pagesize_);
    // TODO: as mentioned in .cpp.
    memset(cur_mother_page, 0, pagesize_);
    printf(
        "<><><><><><><><><><><><>New mother page!<><><><><><><><><><><><>\n");
    assert(cur_mother_page);
    cur_MP_ = 0;
    mother_page_list_.push_back(cur_mother_page);
  }
  ret = cur_mother_page + cur_MP_;
  cur_MP_ += bucksize_;
#else
//	ret=malloc(bucksize_);
//	ret=(char*)memalign(256,bucksize_);
#ifdef CONTENTION_REDUCTION
  grandmother_lock_[thread_id].acquire();
  ret = (char*)grandmothers[thread_id]->malloc();
  grandmother_lock_[thread_id].release();
#else
  mother_page_lock_.acquire();
  ret = (char*)grandmother->malloc();
  mother_page_lock_.release();
#endif
//	memset(ret,0,bucksize_);
//	allocate_count++;
//	allocated_buckets.insert(ret);
#endif
  void** new_buck_nextloc =
      (void**)(((char*)ret) + buck_actual_size_ + sizeof(void*));
  void** new_buck_freeloc = (void**)(((char*)ret) + buck_actual_size_);
  *new_buck_freeloc = (char*)(ret) + tuplesize_;
  *new_buck_nextloc = data;
  overflow_count_[offset]++;
  bucket_[offset] = ret;
#ifdef __MOTHER_PAGE__
  mother_page_lock_.release();
#endif
  return ret;
}

void BasicHashTable::report_status() {
  printf("-----------Hash table status--------------\n");
  printf("Bucket size: %d\n", bucksize_);
  printf("tuple size: %d\n", tuplesize_);
  printf("#. of buckets: %d\n", nbuckets_);
//	printf("Number of Mother pages:%d\n",mother_page_list_.size());
#ifdef _DEBUG_
  unsigned long int total_overflow_buckets = 0;
  for (unsigned i = 0; i < nbuckets_; i++) {
    total_overflow_buckets += overflow_count_[i];
  }
  //	printf("#. of overflow buckets: %ld\n",total_overflow_buckets);

  printf("#. of total buckets :%ld \n", total_overflow_buckets + nbuckets_);
#endif

  unsigned long long int tuple_count = 0;
  for (unsigned i = 0; i < nbuckets_; i++) {
    Iterator it = this->CreateIterator();
    this->placeIterator(it, i);
    while (it.readnext()) {
      tuple_count++;
    }
  }
  //	printf("------------------------------------------\n");

  vector<unsigned> tuples;
  double agv_tuples_per_bucket = tuple_count / (double)(nbuckets_);
  printf("Total tuples in hash table:%lld\n  %3.3f tuple per bucket index\n",
         tuple_count, agv_tuples_per_bucket);
  double var = 0;
  for (unsigned i = 0; i < nbuckets_; i++) {
    Iterator it = this->CreateIterator();
    int count = 0;
    this->placeIterator(it, i);
    while (it.readnext()) {
      count++;
    }
    tuples.push_back(count);
    var += (count - agv_tuples_per_bucket) * (count - agv_tuples_per_bucket);
  }
  printf("VAR: %4.4f\n", var / nbuckets_);

  sort(tuples.begin(), tuples.end(), std::greater<int>());

  int last = -1, last_index = 0;
  for (int i = 0; i < tuples.size(); i++) {
    if (last != tuples[i]) {
      if (last != -1) printf("%d repeated %d times \n", last, i - last_index);
      last = tuples[i];
      last_index = i;
    }
  }
  printf("%d repeated %d times \n", last, nbuckets_ - last_index);
  printf("\n");
}

BasicHashTable::Iterator BasicHashTable::CreateIterator() const {
  return BasicHashTable::Iterator(buck_actual_size_, tuplesize_);
}
BasicHashTable::Iterator::Iterator(const Iterator& r) {
  this->buck_actual_size = r.buck_actual_size;
  this->cur = r.cur;
  this->free = r.free;
  this->next = r.next;
  this->tuplesize = r.tuplesize;
}
BasicHashTable::Iterator::~Iterator() {
  this->buck_actual_size = 0;
  this->cur = 0;
  this->free = 0;
  this->next = 0;
  this->tuplesize = 0;
}

unsigned BasicHashTable::getNumberOfInstances() { return number_of_instances_; }
