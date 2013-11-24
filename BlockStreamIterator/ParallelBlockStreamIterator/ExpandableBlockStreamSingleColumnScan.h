/*
 * ExpandableBlockStreamSingleColumnScan.h
 * In the current implementation, for simplicity, we assume that the underlying storage
 * is arranged in blocks, each of which is the same as the size of the
 * block in the parameter of the next function.
 * TODO: adjust or rewrite this iterator if the consumption above does not hold.
 *  Created on: Aug 27, 2013
 *      Author: wangli
 */

#ifndef EXPANDABLEBLOCKSTREAMSINGLECOLUMNSCAN_H_
#define EXPANDABLEBLOCKSTREAMSINGLECOLUMNSCAN_H_
#include <string>
#include <boost/archive/text_iarchive.hpp>
#include <boost/archive/text_oarchive.hpp>
#include "../BlockStreamIteratorBase.h"
#include "../../Schema/Schema.h"
#include "../../Block/synch.h"
class ExpandableBlockStreamSingleColumnScan:public BlockStreamIteratorBase {
public:
	struct allocated_block{
		char* start;
		unsigned length;
	};
	class State{
	friend class ExpandableBlockStreamSingleColumnScan;
	public:
	State(std::string file_name,Schema* schema,unsigned block_size);
		State(std::string file_name,Schema* schema,unsigned block_size, unsigned long used_length);
		State(){};
	public:
		Schema* schema_;
		std::string filename_;
		unsigned block_size_;
		unsigned long used_length_;
		bool used_size_control_;
		friend class boost::serialization::access;
		template<class Archive>
		void serialize(Archive & ar, const unsigned int version){
			ar & schema_ & filename_ & block_size_ & used_length_ & used_size_control_;
		}
	};
	ExpandableBlockStreamSingleColumnScan(State state);
	ExpandableBlockStreamSingleColumnScan();
	virtual ~ExpandableBlockStreamSingleColumnScan();
	bool open(const PartitionOffset& part_off=0);
	bool next(BlockStreamBase* block);
	bool close();
private:
	/* allocate a block from the underling storage, return false if the underly
	 * data is exhausted.
	 */
	bool atomicIncreaseCursor(unsigned bytes, allocated_block &allo_block);
private:

	State state_;

	/* the semaphore used to guarantee that only one thread does the real work
	 * in the open function the open function
	 */
	semaphore sema_open_;

	volatile bool open_finished_;
	semaphore sema_open_finished_;
	int fd_;
	unsigned long file_length_;
	char* base_;
	char* data_;
	char *cursor_;
	Lock cursor_lock_;

	/*
	 * The following code is for boost serialization.
	 */
private:
	friend class boost::serialization::access;
	template<class Archive>
	void serialize(Archive & ar, const unsigned int version){
		ar & boost::serialization::base_object<BlockStreamIteratorBase>(*this) & state_;
	}
};

#endif /* EXPANDABLEBLOCKSTREAMSINGLECOLUMNSCAN_H_ */
