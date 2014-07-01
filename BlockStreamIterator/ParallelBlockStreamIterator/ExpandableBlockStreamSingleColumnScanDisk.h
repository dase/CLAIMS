/*
 * ExpandableBlockStreamSingleColumnScanDISK.h
 * In the current implementation, for simplicity, we assume that the underlying storage
 * is arranged in blocks, each of which is the same as the size of the
 * block in the parameter of the next function.
 * TODO: adjust or rewrite this iterator if the consumption above does not hold.
 *  Created on: Sep 5, 2013
 *      Author: wangli
 */

#ifndef EXPANDABLEBLOCKSTREAMSINGLECOLUMNSCANDISK_H_
#define EXPANDABLEBLOCKSTREAMSINGLECOLUMNSCANDISK_H_
#include <string>
#include <boost/archive/text_iarchive.hpp>
#include <boost/archive/text_oarchive.hpp>
#ifdef DMALLOC
#include "dmalloc.h"
#endif
#include "../BlockStreamIteratorBase.h"
#include "../../common/Schema/Schema.h"
#include "../../utility/lock.h"
class ExpandableBlockStreamSingleColumnScanDisk:public BlockStreamIteratorBase {
public:
	class State{
	friend class ExpandableBlockStreamSingleColumnScanDisk;
	public:
		State(std::string file_name,Schema* schema,unsigned block_size);
		State(std::string file_name,Schema* schema,unsigned block_size, unsigned long used_length);
		State(){};
	private:
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
public:
	ExpandableBlockStreamSingleColumnScanDisk(State state);
	ExpandableBlockStreamSingleColumnScanDisk();
	virtual ~ExpandableBlockStreamSingleColumnScanDisk();
	bool open(const PartitionOffset& part_off=0);
	bool next(BlockStreamBase* block);
	bool close();

private:

	State state_;

	/* the semaphore used to guarantee that only one thread does the real work
	 * in the open function the open function
	 */
	semaphore sema_open_;
	BlockStreamBase* block_for_reading_;
	volatile bool open_finished_;
	semaphore sema_open_finished_;
	int fd_;
	unsigned long file_length_;
	Lock file_flength_lock_;
	char* base_;
	char* data_;

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

#endif /* EXPANDABLEBLOCKSTREAMSINGLECOLUMNSCANDISK_H_ */
