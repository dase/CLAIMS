/*
 * ExpandableBlockStreamHdfsScan.h
 *
 *  Created on: 2013-10-12
 *      Author: casa
 */

#ifndef EXPANDABLEBLOCKSTREAMHDFSSCAN_H_
#define EXPANDABLEBLOCKSTREAMHDFSSCAN_H_
#ifdef DMALLOC
#include "dmalloc.h"
#endif
#include "../../storage/BlockManager.h"
#include "../../storage/BlanceMatcher.h"
#include "../../common/Schema/Schema.h"
#include "../../utility/lock.h"
#include "../BlockStreamIteratorBase.h"
#include "../../Debug.h"
#include "../../configure.h"

#include <string>
#include <vector>
using namespace std;

#include <boost/archive/text_iarchive.hpp>
#include <boost/archive/text_oarchive.hpp>

class ExpandableBlockStreamHdfsScan:public BlockStreamIteratorBase{
public:
	struct current_block{
		unsigned file_index;// n-th block
		unsigned length;// 64M determined
		char *base;// base of 64M block

//		unsigned sublock_size;
//		unsigned sublock_index;//n-th 64K block
		char *cursor; //cursor in 64K block
	};
	struct allocated_block{
		char* start;
		unsigned length;
	};
	class State{
		friend class ExpandableBlockStreamHdfsScan;
	public:
		State(string partition_file_name,Schema *partition_schema,unsigned block_size);
		State(){}
	private:
		string partition_file_name_;
		Schema *partition_schema_;
		unsigned block_size_;
	friend class boost::serialization::access;
	template<class Archive>
	void serialize(Archive & ar, const unsigned int version){
		ar & partition_file_name_ & partition_schema_ & block_size_;
		}
	};

	ExpandableBlockStreamHdfsScan(State state);
	ExpandableBlockStreamHdfsScan();
	virtual ~ExpandableBlockStreamHdfsScan();

	/* 如果三个线程读一堆文件的话，
	 * 怎样才算最快？先一个一个读吧
	 * */
	bool open(const PartitionOffset& part_off);
	bool next(BlockStreamBase *block);
	bool close();
	void print();
private:
	bool atomicIncreaseCursor(unsigned bytes,allocated_block &allo_block);

private:
	State state_;
	semaphore sema_open_;
	volatile bool open_finished_;
	vector<char *> file_set_;
	current_block cb_;
	Lock lock_;

private:
	friend class boost::serialization::access;
	template<class Archive>
	void serialize(Archive & ar, const unsigned int version){
		ar & boost::serialization::base_object<BlockStreamIteratorBase>(*this) & state_;
	}
};

#endif /* EXPANDABLEBLOCKSTREAMHDFSSCAN_H_ */
