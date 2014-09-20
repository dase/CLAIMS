/*
 * ExpandableBlockStreamExchangeLower.h
 *
 *  Created on: Aug 29, 2013
 *      Author: wangli
 */

#ifndef EXPANDABLEBLOCKSTREAMEXCHANGELOWERMATERIALIZED_H_
#define EXPANDABLEBLOCKSTREAMEXCHANGELOWERMATERIALIZED_H_


#include <boost/archive/text_iarchive.hpp>
#include <boost/archive/text_oarchive.hpp>
#include <boost/serialization/vector.hpp>
#include <boost/serialization/utility.hpp>
#include <string>
#ifdef DMALLOC
#include "dmalloc.h"
#endif

#include <pthread.h>
#include <stdio.h>
#include "../../common/Schema/Schema.h"
#include "../BlockStreamExchangeLowerBase.h"
#include "../../Executor/IteratorExecutorMaster.h"
#include "../../utility/lock.h"
#include "../../common/Block/PartitionedBlockBuffer.h"
#include "../../common/Block/BlockStream.h"
#include "../../common/Block/BlockStreamBuffer.h"
#include "../../common/Logging.h"

class ExpandableBlockStreamExchangeLowerMaterialized:public BlockStreamExchangeLowerBase {
public:
	struct State{
		Schema* schema_;
		BlockStreamIteratorBase* child_;
		unsigned long long int exchange_id_;
		std::vector<std::string> upper_ip_list_;
		unsigned block_size_;
		PartitionOffset partition_offset;
		unsigned partition_key_index_;
		State(Schema *schema, BlockStreamIteratorBase* child, std::vector<std::string> upper_ip_list, unsigned block_size,
						unsigned long long int exchange_id=0,unsigned partition_key_index=0)
		:schema_(schema),child_(child),upper_ip_list_(upper_ip_list),block_size_(block_size),exchange_id_(exchange_id),partition_offset(0),partition_key_index_(partition_key_index)
		{}
		State(){};
		friend class boost::serialization::access;
		template<class Archive>
		void serialize(Archive & ar, const unsigned int version){
			ar & schema_ & child_ & exchange_id_ & upper_ip_list_ &block_size_&partition_offset&partition_key_index_;
		}
	};
	ExpandableBlockStreamExchangeLowerMaterialized(State state);
	ExpandableBlockStreamExchangeLowerMaterialized();
	virtual ~ExpandableBlockStreamExchangeLowerMaterialized();
	bool open(const PartitionOffset& part_off=0);
	bool next(BlockStreamBase* );
	bool close();
private:
	void Send();
	bool Materialize();
	static void* materialize_and_send(void* arg);
	/* the thread for outputing debug information*/
	static void* debug(void* arg);

	/** random select one block from the partitioned files and return the partition
	 *  index. Return -1 if all the partitioned files reach EOF.
	 */
	int getBlockFromFile(Block &block);
	bool createWorkerThread();
	void cancelWorkerThread();
	void closeDiskFiles();
	void deleteDiskFiles();
	std::string getPartititionedFileName(int partition_index)const;
private:
	State state_;
	unsigned nuppers_;
	int* socket_fd_upper_list_;
	PartitionedBlockBuffer* partitioned_data_buffer_;

	/* one BlockStream for each uppers, the tuples from the child
	 * iterator are fed to the cur_block_stream_list_ according to their
	 * partition key.
	 */
	BlockStreamBase** partitioned_block_stream_;
	Block* block_for_sending_;
	Block* block_for_serialization_;
	BlockStreamBase* block_stream_for_asking_;
	pthread_t sender_tid_;
	pthread_t debug_tid_;

	Logging* log_;

	int* disk_fd_list_;
//	unsigned* disk_file_length_list_;
//	unsigned* disk_file_cur_list_;
	volatile bool child_exhausted_;
private:
	friend class boost::serialization::access;
	template<class Archive>
	void serialize(Archive & ar, const unsigned int version){
		ar & boost::serialization::base_object<BlockStreamExchangeLowerBase>(*this) & state_;
	}
};




#endif /* EXPANDABLEBLOCKSTREAMEXCHANGELOWER_H_ */
