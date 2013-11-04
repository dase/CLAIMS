/*
 * PartitionedBlockContainer.h
 * This class contains an array of BlockContainer, one for each receiving
 * exchange upper. This class is the auxiliary structure to support non-
 * blocking sending.
 *  Created on: Sep 22, 2013
 *      Author: wangli
 */

#ifndef PARTITIONEDBLOCKCONTAINER_H_
#define PARTITIONEDBLOCKCONTAINER_H_
#include <vector>
#include "BlockContainer.h"
class PartitionedBlockContainer {
	struct status{
		bool avaiable;
		status(){
			avaiable=false;
		}
		//TODO: support more status variables.
	};
public:
	PartitionedBlockContainer(unsigned partitions,unsigned block_size);
	virtual ~PartitionedBlockContainer();
	int getBlockForSending(BlockContainer* &block);
	void insert(unsigned partition_id,BlockContainer* & block);
	void Initialized();
private:
	unsigned partition_count_;
	unsigned block_size_;
	std::vector<BlockContainer*> block_list_;
	std::vector<status> status_list_;
};

#endif /* PARTITIONEDBLOCKCONTAINER_H_ */
