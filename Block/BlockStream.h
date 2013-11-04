/*
 * BlockStream.h
 *
 *  Created on: Aug 26, 2013
 *      Author: wangli
 */

#ifndef BLOCKSTREAM_H_
#define BLOCKSTREAM_H_
#include "../Schema/Schema.h"
#include "Block.h"
class BlockStreamBase{
public:

	class BlockStreamTraverseIterator{
	public:
		explicit BlockStreamTraverseIterator(BlockStreamBase* block_stream_base)
		:block_stream_base_(block_stream_base),cur(0){};
		~BlockStreamTraverseIterator(){};
		inline void* nextTuple(){
			return block_stream_base_->getTuple(cur++);
		}


		/* get the current tuple of the iterator without increasing cur_
		 * Usually, increase_cur_() is called after.
		 */
		inline void* currentTuple() const{
			return block_stream_base_->getTuple(cur);
		}


		/* This function is usually called after currentTuple()  */
		inline void increase_cur_(){
			cur++;
		}


		inline void reset(){
			cur=0;
		}

	private:
		BlockStreamBase* block_stream_base_;
		unsigned cur;
	};
	friend class BlockStreamTraverseIterator;
	virtual ~BlockStreamBase(){};

	virtual void* allocateTuple(unsigned bytes)=0;
	virtual void setEmpty()=0;
	virtual bool Empty() const =0;
	virtual void* getBlockDataAddress()=0;
	virtual void setBlockDataAddress(void* addr)=0;
	virtual unsigned getTuplesInBlock()const=0;
	/* copy a block in the storage layer into the BlockStream, the member variables (e.g., _free) are
	 * also updated according the new data.
	 */
	virtual void copyBlock(void* addr, unsigned length)=0;

	virtual bool switchBlock(BlockStreamBase &block)=0;

	/* serialize the Block Stream into the Block which can be sent through the network.*/
	virtual bool serialize(Block &block) const =0;

	/* convert the Block from the network into the content of current instance*/
	virtual bool deserialize(Block *block)=0;

	/* return the actual size to hold this stream block, including the data and some auxiliary structure.*/
	virtual unsigned getSerializedBlockSize()const=0;

	BlockStreamTraverseIterator* createIterator(){
		return new BlockStreamTraverseIterator(this);
	};
	static BlockStreamBase* createBlock(Schema* schema,unsigned block_size);
protected:
	virtual void* getTuple(unsigned offset) const =0;

};

class BlockStreamFix:public BlockStreamBase {
public:
	BlockStreamFix(unsigned block_size,unsigned tuple_size);
	virtual ~BlockStreamFix();
public:
	inline void* allocateTuple(unsigned bytes){
		if(free_+bytes<=data_+block_size_){
			void* ret=free_;
			free_+=bytes;
			return ret;
		}
		return 0;
	}
	void setEmpty();

	/* get [offset]-th tuple of the block */
	inline void* getTuple(unsigned offset) const {
		void* ret=data_+offset*tuple_size_;
		if(ret>=free_){
			return 0;
		}
		return ret;
	}
	bool Empty() const;
	void* getBlockDataAddress();
	void setBlockDataAddress(void* addr);
	bool switchBlock(BlockStreamBase& block);
	void copyBlock(void* addr, unsigned length);
	bool serialize(Block & block) const;
	bool deserialize(Block * block);
	unsigned getSerializedBlockSize()const;
	unsigned getTuplesInBlock()const;
protected:
	char* data_;
	unsigned block_size_;
	char* free_;
	unsigned tuple_size_;
public:

};



#endif /* BLOCKSTREAM_H_ */
