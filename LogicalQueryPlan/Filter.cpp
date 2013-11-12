/*
 * Filter.cpp
 *
 *  Created on: Nov 10, 2013
 *      Author: wangli
 */

#include "Filter.h"

Filter::Filter(std::vector<FilterIterator::AttributeComparator> ComparatorList,LogicalOperator* child )
:comparator_list_(ComparatorList),child_(child){

}

Filter::~Filter() {
	// TODO Auto-generated destructor stub
}

Dataflow Filter::getDataflow(){
	/** In the currently implementation, we assume that the boolean operator
	 * between each AttributeComparator is "AND".
	 */

	Dataflow child_dataflow=child_->getDataflow();
	if(child_dataflow.isHashPartitioned()){
		for(unsigned i=0;i<child_dataflow.property_.partitioner.getNumberOfPartitions();i++){
			if(true)//is filtered
			{
				child_dataflow.property_.partitioner.getPartition(i)->setFiltered();
			}
		}
	}

	return child_->getDataflow();
}

bool Filter::couldHashPruned(unsigned partition_id,Partitioner* part){
	for(unsigned i=0;i<comparator_list_.size();i++){
		if(*part->getPartitionKey()==comparator_list_[i].)
	}
}
