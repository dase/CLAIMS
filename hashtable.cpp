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

#include "hashtable.h"

#include <malloc.h>
#include "rdtsc.h"


BasicHashTable::BasicHashTable(unsigned nbuckets, unsigned bucksize, unsigned tuplesize)
:nbuckets(nbuckets), bucksize(bucksize), tuplesize(tuplesize)
{
	try
	{
		bucket = (void**)new char[sizeof(void*) * nbuckets];

		pagesize=nbuckets*get_aligned_space(bucksize);
		char* cur_mother_page=(char*)malloc(pagesize);
		t_start=cur_mother_page;
		if(cur_mother_page==0)
		{
			cout<<"Error!"<<endl;
		}
		memset(cur_mother_page,0,pagesize);
		cur_MP=0;
		mother_page_list.push_back(cur_mother_page);

		for (unsigned i=0; i<nbuckets; i++)
		{
			if(get_aligned_space(bucksize)>(pagesize-(cur_MP)) )// the current mother page doesn't have enough space for the new buckets
			{
				cur_mother_page=(char*)malloc(pagesize);
				if(cur_mother_page==0)
				{
					cout<<"Error!"<<endl;
				}
				memset(cur_mother_page,0,pagesize);
				cur_MP=0;
				mother_page_list.push_back(cur_mother_page);
			}
			bucket[i]=(void*)(cur_mother_page+cur_MP);
			cur_MP+=get_aligned_space(bucksize);

			void** free = (void**)(((char*)bucket[i]) + bucksize);
			void** next = (void**)(((char*)bucket[i]) + bucksize + sizeof(void*));
			*next = 0;
			*free = bucket[i];
		}
	}
	catch (exception& e) {
		cout << "failed" << endl;
		cout << "EXCEPTION: Standard exception: " << e.what() << endl;}
}

BasicHashTable::~BasicHashTable()
{
	for(int i=0;i<mother_page_list.size();i++)
	{
		free(mother_page_list.at(i));
	}
		delete[] bucket;
}

BasicHashTable::Iterator::Iterator() : bucksize(0), tuplesize(0), cur(0), next(0), free(0),pagestart(0) { }
BasicHashTable::Iterator::Iterator(unsigned int bucksize, unsigned int tuplesize):bucksize(bucksize),tuplesize(tuplesize),cur(0), next(0), free(0),pagestart(0){}
BasicHashTable::Iterator BasicHashTable:: CreateIterator()
{
	return BasicHashTable::Iterator(bucksize, tuplesize);
}

//
//
//void HashTable::init(unsigned int nbuckets, unsigned int bucksize, unsigned int tuplesize)
//{
//
//	init(nbuckets,bucksize,tuplesize,-1);
//
//}
//void HashTable::init(unsigned int nbuckets, unsigned int bucksize, unsigned int tuplesize,unsigned _nodeid)
//{
//	this->nodeid=_nodeid;
//
//	try
//	{
//		lock = new Lock*[nbuckets];
//
//		for (int i=0; i<nbuckets; i++) {
//			lock[i]=new Lock();//cout<<"++"<<i<<endl;
//		}
//
//		bucket = (void**)new char[sizeof(void*) * nbuckets];
//#ifdef __MULTI_NODE__
//
//
//
//		char* cur_mother_page[NNODES];
//		pagesize=nbuckets*get_aligned_space(bucksize)/NNODES;
//
//		for(unsigned i=0;i<NNODES;i++)
//		{
//			if(nodeid>=0)
//				cur_mother_page[i]=(char*)numa_alloc_onnode(pagesize,nodeid);
//			else
//				cur_mother_page[i]=(char*)numa_alloc_onnode(pagesize,i);
//			memset(cur_mother_page[i],0,pagesize);
//			mother_page_list[i].push_back(cur_mother_page[i]);
//			cur_MP[i]=0;
//			t_start[i]=cur_mother_page[i];
//		}
//
//		range=nbuckets/NNODES;
//#endif
//
//#ifdef __SINGLE_NODE__
//		pagesize=nbuckets*get_aligned_space(bucksize);
//		char* cur_mother_page=(char*)numa_alloc_onnode(pagesize,nodeid);
//		t_start=cur_mother_page;
//		if(cur_mother_page==0)
//		{
//			cout<<"Error!"<<endl;
//		}
//		memset(cur_mother_page,0,pagesize);
//		cur_MP=0;
//		mother_page_list.push_back(cur_mother_page);
//#endif
//
//		for (int i=0; i<nbuckets; ++i) {
//#ifdef __SINGLE_NODE__
//			if(get_aligned_space(bucksize)>(pagesize-(cur_MP)) )// the current mother page doesn't have enough space for the new buckets
//			{
//
//				cur_mother_page=(char*)numa_alloc_onnode(pagesize,nodeid);
//				if(cur_mother_page==0)
//				{
//					cout<<"Error!"<<endl;
//				}
//				memset(cur_mother_page,0,pagesize);
//				cur_MP=0;
//				mother_page_list.push_back(cur_mother_page);
//			}
//			bucket[i]=(void*)(cur_mother_page+cur_MP);
//			cur_MP+=get_aligned_space(bucksize);
//#endif
//#ifdef __MULTI_NODE__
//			if(get_aligned_space(bucksize)>(pagesize-(cur_MP[i/range])) )// the current mother page doesn't have enough space for the new buckets
//			{
//				if(nodeid>=0)
//					cur_mother_page[i/range]=(char*)numa_alloc_onnode(pagesize,nodeid);
//				else
//					cur_mother_page[i/range]=(char*)numa_alloc_onnode(pagesize,i/range);
//				if(cur_mother_page[i/range]==0)
//				{
//					cout<<"Error!"<<endl;
//				}
//				memset(cur_mother_page[i/range],0,pagesize);
//				cur_MP[i/range]=0;
//				mother_page_list[i/range].push_back(cur_mother_page[i/range]);
//			}
//			bucket[i]=(void*)(cur_mother_page[i/range]+cur_MP[i/range]);
//			cur_MP[i/range]+=get_aligned_space(bucksize);
//#endif
//
//
//
//
//			void** free = (void**)(((char*)bucket[i]) + bucksize);
//			void** next = (void**)(((char*)bucket[i]) + bucksize + sizeof(void*));
//			*next = 0;
//			*free = bucket[i];
//		}
//
//		this->bucksize = bucksize;
//		this->tuplesize = tuplesize;
//		this->nbuckets = nbuckets;
////	cout<<"OK!!"<<endl;
//	}
//	catch (exception& e) {
//		cout << "failed" << endl;
//		cout << "EXCEPTION: Standard exception: " << e.what() << endl;}
//}
//void HashTable::destroy()
//{
////#define __Sequential_allocate__
////#ifdef __Sequential_allocate__
//#ifdef __SINGLE_NODE__
//		for(int i=0;i<mother_page_list.size();i++)
//		{
////			free(mother_page_list.at(i));
//			numa_free(mother_page_list.at(i),pagesize);
//		}
//#endif
//#ifdef __MULTI_NODE__
//		for(unsigned j=0;j<NNODES;j++)
//		{
//			for(int i=0;i<mother_page_list[j].size();i++)
//			{
//			//			free(mother_page_list.at(i));
//				numa_free(mother_page_list[j].at(i),pagesize);
//			}
//		}
//#endif
////#else
////		for (int i=0; i<nbuckets; ++i) {
////			void* tmp = bucket[i];
////			void* cur = *(void**)(((char*)bucket[i]) + bucksize + sizeof(void*));
////		while (cur) {
////#ifdef __Aligned__
////			free(bucket[i]);
////#else
////			delete[] (char*)tmp;
////#endif
////			tmp = cur;
////			cur = *(void**)(((char*)cur) + bucksize + sizeof(void*));
////		}
////		delete[] (char*) tmp;}
////#endif
//
//
//
//	delete[] lock;
//	delete[] (char*) bucket;
//}
//
//unsigned HashTable::getNodeID()
//{
//	return nodeid;
//}
//
////void* HashTable::allocate(unsigned int offset)
////{
////#ifdef DEBUG
////	assert(0 <= offset && offset<nbuckets);
////#endif
////
////	void* data = bucket[offset];
////	void** freeloc = (void**)((char*)data + bucksize);
////	void* ret;
////	if ((*freeloc) <= ((char*)data + bucksize - tuplesize)) {
////		// Fast path: it fits!
////		//
////		ret = *freeloc;
////		*freeloc = ((char*)(*freeloc)) + tuplesize;
////		return ret;
////	}
////
////	// Allocate new page and make bucket[offset] point to it.
////	//
////	//throw PageFullException(offset);
////#ifdef __Sequential_allocate__
////
////
////	char* cur_mother_page=mother_page_list.at(mother_page_list.size()-1);
////	if(get_aligned_space(bucksize)>(pagesize-(cur_MP+1)) )// the current mother page doesn't have enough space for the new buckets
////			{
//////				cur_mother_page= (char*)memalign(pagesize,pagesize);
////		cur_mother_page=(char*)numa_alloc_onnode(pagesize,nodeid);
//////		cur_mother_page=(char*)my_NUMA_alloc_local(pagesize,pagesize);
////				if(cur_mother_page==0)
////					cout<<"Hash table allocation error!"<<endl;
////				cur_MP=0;
////				memset(cur_mother_page,0,pagesize);
////				mother_page_list.push_back(cur_mother_page);
////			}
////
////				ret=cur_mother_page+cur_MP;
////				cur_MP+=get_aligned_space(bucksize);
////#else
////
////
////	ret = new char[bucksize + 2*sizeof(void*)];
////#endif
////
////	bucket[offset] = ret;
////
////	void** nextloc = (void**)(((char*)ret) + bucksize + sizeof(void*));
////	*nextloc = data;
////
////	freeloc = (void**)(((char*)ret) + bucksize);
////	*freeloc = ((char*)ret) + tuplesize;
////
////	return ret;
////}
////void* HashTable::allocate(unsigned int offset,void * f)
////{
////#ifdef DEBUG
////	assert(0 <= offset && offset<nbuckets);
////#endif
////
////	void* data = bucket[offset];
////	void** freeloc = (void**)((char*)data + bucksize);
////	if(f!=*freeloc)
////		return 0;
////	void* ret;
////	if ((*freeloc) <= ((char*)data + bucksize - tuplesize)) {
////		// Fast path: it fits!
////		//
////		ret = *freeloc;
////		*freeloc = ((char*)(*freeloc)) + tuplesize;
////		return ret;
////	}
////
////	// Allocate new page and make bucket[offset] point to it.
////	//
////	//throw PageFullException(offset);
////#ifdef __Sequential_allocate__
////
////
////	char* cur_mother_page=mother_page_list.at(mother_page_list.size()-1);
////	if(get_aligned_space(bucksize)>(pagesize-(cur_MP+1)) )// the current mother page doesn't have enough space for the new buckets
////			{
//////				cur_mother_page= (char*)memalign(pagesize,pagesize);
////		cur_mother_page=(char*)my_NUMA_alloc_onnode(4*1024,pagesize,nodeid);
//////		cur_mother_page=(char*)my_NUMA_alloc_local(pagesize,pagesize);
////				if(cur_mother_page==0)
////					cout<<"Hash table allocation error!"<<endl;
////				cur_MP=0;
////				memset(cur_mother_page,0,pagesize);
////				mother_page_list.push_back(cur_mother_page);
////			}
////
////				ret=cur_mother_page+cur_MP;
////				cur_MP+=get_aligned_space(bucksize);
////#else
////
////
////	ret = new char[bucksize + 2*sizeof(void*)];
////#endif
////
//////	bucket[offset] = ret;
////
////	void** nextloc = (void**)(((char*)ret) + bucksize + sizeof(void*));
////	*nextloc = data;
////
////	freeloc = (void**)(((char*)ret) + bucksize);
////	*freeloc = ((char*)ret) + tuplesize;
////
////	return ret;
////}
//HashTable::Iterator HashTable::createIterator()
//{
//	return Iterator(bucksize, tuplesize);
//}
//
//HashTable::Iterator::Iterator(unsigned int bucksize, unsigned int tuplesize)
//	: bucksize(bucksize), tuplesize(tuplesize), cur(0), free(0), next(0)
//{
//}
