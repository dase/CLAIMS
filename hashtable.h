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


#include <memory.h>
#include <iostream>
#include <malloc.h>
#include <vector>
#include <assert.h>

#include "atomics.h"
#include "lock.h"
#include "configure.h"
#include "lock.h"
typedef void (*fun)(void*, void*);

using namespace std;

#ifndef __HASHTABLE__
#define __HASHTABLE__

inline int get_aligned_space(const unsigned& bucksize)
{
	return ((bucksize+2*sizeof(void*)-1)/cacheline_size+1)*cacheline_size;
}
class BasicHashTable
{
public:
	BasicHashTable(unsigned nbuckets, unsigned bucksize, unsigned tuplesize);
	~BasicHashTable();
	inline void* allocate(const unsigned& offset)
	{
		assert(offset<nbuckets_);

		void* data=bucket_[offset];

		void** freeloc = (void**)((char*)data + buck_actual_size_);
		void* ret;
		if ((*freeloc)+tuplesize_ <= ((char*)data + buck_actual_size_))
		{
			ret = *freeloc;
			*freeloc = ((char*)(*freeloc)) + tuplesize_;
			assert(ret!=0);
			return ret;
		}
		char* cur_mother_page=mother_page_list_.back();
		if(bucksize_+cur_MP_>=pagesize_ )// the current mother page doesn't have enough space for the new buckets
		{
			cur_mother_page=(char*)memalign(PAGE_SIZE,pagesize_);
			assert(cur_mother_page);
			cur_MP_=0;
			mother_page_list_.push_back(cur_mother_page);
		}
		overflow_count_[offset]++;
		ret=cur_mother_page+cur_MP_;
		cur_MP_+=bucksize_;

		void** new_buck_nextloc = (void**)(((char*)ret) + buck_actual_size_ + sizeof(void*));
		void** new_buck_freeloc = (void**)(((char*)ret) + buck_actual_size_);
		*new_buck_freeloc = (ret)+tuplesize_ ;
		*new_buck_nextloc = data;

		bucket_[offset]=ret;
		return ret;
	}
	inline void* atomicAllocate(const unsigned& offset){
		void* ret;
		lock_list_[offset].lock();
		ret=allocate(offset);
		lock_list_[offset].unlock();
		return ret;
	}
	inline void UpdateTuple(unsigned int offset,void* loc,void* newvalue, fun func)
	{
		func(loc, newvalue);
	}
	class Iterator
	{
		friend class BasicHashTable;
		public:
		Iterator();
		Iterator(const unsigned& buck_actual_size,const unsigned& tuplesize);
		inline void* readnext()
		{
			void* ret;
			if (cur < free)
			{
				ret = cur;
				cur = ((char*)cur) + tuplesize;
				return ret;
			}
			else if (next != 0)
				{
					ret = next;
					cur = ((char*)next) + tuplesize;
					free = *(void**)((char*)next + buck_actual_size);
					next = *(void**)((char*)next + buck_actual_size + sizeof(void*));
//					return ret < free ? ret : 0;
					return ret;
				}
				else
				{
					return 0;
				}
		}

		inline void * readCurrent()const{
			void *ret;

			if(cur<free){
				ret=cur;
				return ret;
			}
			else{
				return 0;
			}
		}

		inline void increase_cur_(){
			if (cur+tuplesize < free){
				cur = ((char*)cur) + tuplesize;
			}
			else if (next != 0){
				cur=(char*)next;
				free = *(void**)((char*)next + buck_actual_size);
				next = *(void**)((char*)next + buck_actual_size + sizeof(void*));
			}
			else{
				cur =free;
			}
		}

		private:
			void* cur;
			void* free;
			void* next;
//			void* pagestart;
			int buck_actual_size;
			int tuplesize;
	};
	BasicHashTable::Iterator CreateIterator()const;
	inline bool placeIterator(Iterator& it, const unsigned& offset)
	{
		if(offset>=nbuckets_)
			return false;
		void* start=bucket_[offset];
		it.cur = start;
		it.free = *(void**)((char*)start + buck_actual_size_);
		it.next = *(void**)((char*)start + buck_actual_size_ + sizeof(void*));

		return true;
	}
	unsigned getHashTableTupleSize(){
		return tuplesize_;
	}
private:
	int nbuckets_;
	int bucksize_;
	int buck_actual_size_;
	int tuplesize_;
	void** bucket_;
	int pagesize_;
	char* t_start_;
	int cur_MP_;
	std::vector<char*> mother_page_list_;
	SpineLock* lock_list_;
	unsigned * overflow_count_;
};

//
//		{
//#ifdef DEBUG
//			assert(0 <= offset && offset<nbuckets);
//#endif
//			try
//			{
//				void* ret;
//				lock[offset]->lock();
//				ret = allocate(offset,free);
//				lock[offset]->unlock();
//				return ret;
//			}
//			catch (exception& e) {
//					cout << "failed" << endl;
//					cout << "EXCEPTION: Standard exception: " << e.what() << endl;
//					throw;}
//		}
//		inline void atomicUpdateTuple(unsigned int offset,void* loc,int newvalue,unsigned int len)
//		{
//			lock[offset]->lock();
//			*((unsigned int *)(loc)+1)+=newvalue;
//			lock[offset]->unlock();
//		}
//		inline void atomicUpdateTuple_CAS(unsigned int offset,void* loc,int newvalue,unsigned int len)
//		{
//			 int old_value;
//			 int new_value=*((int *)(loc)+1);
//			do
//			{
//				old_value=new_value;
//				new_value=old_value+newvalue;
////				if(*((int*)loc)==1)
////					cout<<"New_value:"<<new_value<<" old_value:"<<old_value<<endl;
//				new_value=atomic_compare_and_swap((int*)loc+1,old_value,new_value);
////				if(*((int*)loc)==1)
////				{
////					cout<<"------->"<<new_value<<endl;
////				cout<<*((int*)loc+1)<<endl;
////				}
//			}
//			while(new_value!=old_value);
//		}

#endif
