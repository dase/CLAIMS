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

typedef void (*fun)(void*, void*);

using namespace std;

#ifndef __HASHTABLE__
#define __HASHTABLE__

inline unsigned get_aligned_space(unsigned bucksize)
{
	return ((bucksize+2*sizeof(void*)-1)/cacheline_size+1)*cacheline_size;
}
class BasicHashTable
{
public:
	BasicHashTable(unsigned nbuckets, unsigned bucksize, unsigned tuplesize);
	~BasicHashTable();
	inline void* allocate(unsigned offset)
	{
		assert(0 <= offset && offset<nbuckets);
		void* data = (char*)t_start+get_aligned_space(bucksize)*offset;

		void** freeloc = (void**)((char*)data + bucksize);
		void* ret;
		if ((*freeloc) <= ((char*)data + bucksize - tuplesize))
		{
			ret = *freeloc;
			*freeloc = ((char*)(*freeloc)) + tuplesize;
			return ret;
		}
		char* cur_mother_page=mother_page_list.at(mother_page_list.size()-1);
		if(get_aligned_space(bucksize)>(pagesize-(cur_MP+1)) )// the current mother page doesn't have enough space for the new buckets
		{
			cur_mother_page=(char*)malloc(pagesize);
			if(cur_mother_page==0)
				cout<<"Hash table allocation error!"<<endl;
			cur_MP=0;
			memset(cur_mother_page,0,pagesize);
			mother_page_list.push_back(cur_mother_page);
		}

		ret=cur_mother_page+cur_MP;
		cur_MP+=get_aligned_space(bucksize);

		void** nextloc = (void**)(((char*)data) + bucksize + sizeof(void*));
		*nextloc = ret;
		freeloc = (void**)(((char*)ret) + bucksize);
		*freeloc = (ret) ;
		nextloc = (void**)(((char*)ret) + bucksize + sizeof(void*));

		*nextloc=0;

		return ret;
	}
	inline void UpdateTuple(unsigned int offset,void* loc,void* newvalue, fun func)
	{
		//TODO: make it support various aggregation functions and various data type.
//		*((int *)(loc))+=*(int *)newvalue;
		func(loc, newvalue);
	}
	class Iterator
	{
		friend class BasicHashTable;
		public:
		Iterator();
		Iterator(unsigned int bucksize, unsigned int tuplesize);
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
					free = *(void**)((char*)next + bucksize);
					next = *(void**)((char*)next + bucksize + sizeof(void*));
					return ret < free ? ret : 0;
				}
				else
				{
					return 0;
				}
		}

		inline void * readCurrent(){
			void *ret;
			if(cur<free){
				ret=cur;
				return ret;
			}
			else if(next!=0){
				ret = next;
				ever_next=next;
				free = *(void**)((char*)next + bucksize);
				next = *(void**)((char*)next + bucksize + sizeof(void*));
				return ret < free ? ret : 0;
			}
			else{
				return 0;
			}
		}

		inline void *increase_cur_(){
			if (cur < free){
				cur = ((char*)cur) + tuplesize;
			}
			else if (next != 0){
				cout<<"****************"<<endl;
				cur = ((char*)ever_next) + tuplesize;
			}
			else{
				return 0;
			}
		}

		inline void* nextPage()
		{
			void* ret;
			ret=pagestart;
			if(next!=0)
			{
				pagestart=next;
				cur=pagestart;
				free=*(void**)((char*)pagestart+bucksize);
				next = *(void**)((char*)pagestart + bucksize + sizeof(void*));
			}
			else
			{
				pagestart=0;
				cur=0;
			}
			return ret;
		}
		private:
			void* cur;
			void* free;
			void* ever_next;
			void* next;
			void* pagestart;
			unsigned int bucksize;
			unsigned int tuplesize;
	};
	BasicHashTable::Iterator CreateIterator();
	inline bool placeIterator(Iterator& it, unsigned int offset) //__attribute__((always_inline))
	{
		if(offset>=nbuckets)
			return false;

		void* start = (char*)t_start+offset*get_aligned_space(bucksize);
		it.cur = start;

		it.free = *(void**)((char*)start + bucksize);
		it.next = *(void**)((char*)start + bucksize + sizeof(void*));

		it.tuplesize=tuplesize;
		it.pagestart=start;
		return true;
	}
	unsigned getHashTableTupleSize(){
		return tuplesize;
	}
private:
	unsigned nbuckets;
	unsigned bucksize;
	unsigned tuplesize;
	void** bucket;
	unsigned pagesize;
	char* t_start;
	unsigned cur_MP;
	std::vector<char*> mother_page_list;
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
