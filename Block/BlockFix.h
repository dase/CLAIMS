/*
 * BlockFix.h
 *
 *  Created on: 2013-8-23
 *      Author: zhanglei
 */

#ifndef BLOCKFIX_H_
#define BLOCKFIX_H_

#include <malloc.h>
#include <memory.h>
#include "../configure.h"
#include "../common/Schema/SchemaFix.h"
#include "../Block/Block.h"
class BlockFix {
public:
	BlockFix(unsigned BlockSize,SchemaFix* _schema);
	BlockFix(Block &block,SchemaFix* schema);
	virtual ~BlockFix();

	//support general block
	void *getBlock() const{
		return start;
	}

	void setBlock(void *addr){
		start=(char*)addr;
	}

	unsigned getsize() const{
		return BlockSize;
	}
	inline unsigned getFreeTuples() const {
		return BlockSize/tuple_size-tuplecount;
	}
	/* the user must make sure the empty tuples in the block by
	 * calling getFreeTuples() before allocating*/
	inline void* allocateTuples(unsigned tuples){
		char* ret=_free;
		tuplecount+=tuples;
		_free+=tuples*tuple_size;
		return ret;
	}
	/*support write into block*/

	//insert is too frequent, so inline it
	inline bool insert(void * tuple){
		if(tuplecount*tuple_size+tuple_size<=BlockSize){
			//memcpy the tuple into the _free location, return the tuple size
//			tuplesize=schema->copyTuple(tuple,_free);
			memcpy(_free,tuple,tuple_size);
//			*(int*)_free=*(int*)tuple;
			(_free)=_free+tuple_size;
			tuplecount++;
			return true;
		}
		return false;
	}
	inline void* allocateTuple(){
		if((tuplecount+1)*tuple_size<=BlockSize){
			void* ret=_free;
			_free=(char*)_free+tuple_size;
			tuplecount++;
			return ret;
		}
		return 0;
	}

//	//fold the block and the last block
//	inline void fold() const{
//		*(unsigned*)((char*)start+BlockSize-2*sizeof(tuplecount))=tuplecount;
//		*(unsigned*)((char*)start+BlockSize-sizeof(tuplecount))=1;
//	}
//
//	//fold the last block
//	inline void foldLastBlock() const{
//		*(unsigned*)((char*)start+BlockSize-2*sizeof(tuplecount))=tuplecount;
//		*(unsigned*)((char*)start+BlockSize-sizeof(tuplecount))=0;
//	}
//
//	//is the last block
//	inline bool isLastBlock() const{
//		return *(unsigned*)((char*)start+BlockSize-sizeof(tuplecount))==0;
//	}

	bool copyBlock(void *src){
		memcpy(start,src,BlockSize);
		return true;
	}

	//reset the block
	inline void reset(){
		_free=(char*)start;
		cur=(char *)start;

		tuplecount=0;
	}
	inline void cur_reset(){

		cur=(char *)start;

	}	//get tuple count
	inline unsigned getTupleCount() const{
		return tuplecount;
	}
//
//	inline unsigned getMaxTupleCount() const{
//		return (BlockSize-2*sizeof(unsigned))/schema->getTupleMaxSize();
//	}

	//used in the readable block
	inline unsigned getActualTupleCount() const{
		unsigned* tuplecount=(unsigned *)((char*)start+BlockSize-2*sizeof(unsigned));
		return *tuplecount;
	}

	inline void setEmpty(){
		_free=start;
		cur=start;

		tuplecount=0;
	}

	inline bool hasSpace() const{
		return (char*)_free+schema->getTupleMaxSize()<=(char*)start+BlockSize-2*sizeof(tuplecount);
	}

	inline void *next(){
		char* ret=cur;
		if(ret+tuple_size<=start+tuplecount*tuple_size){
			cur+=tuple_size;
			return ret;
		}
		else
			return 0;
	}
	inline void* cur_get() const{
		if(cur+tuple_size<=start+tuplecount*tuple_size){
			return cur;
		}
		else
			return 0;
	}

	char* getend() const{
		return start+tuplecount*tuple_size;
	}

private:
	unsigned BlockSize;
	SchemaFix* schema;

	char *start;
	char *_free;
	unsigned tuplecount;
	unsigned tuple_size;

	char *cur;


	//Debug
public:
	long filtered;
	long pass_filtered;

};

#endif /* BLOCKFIX_H_ */
