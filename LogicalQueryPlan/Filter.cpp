/*
 * Filter.cpp
 *
 *  Created on: Nov 10, 2013
 *      Author: wangli
 */

#include "Filter.h"
#include "../BlockStreamIterator/ParallelBlockStreamIterator/ExpandableBlockStreamFilter.h"
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

	Dataflow dataflow=child_->getDataflow();
	if(dataflow.isHashPartitioned()){
		for(unsigned i=0;i<dataflow.property_.partitioner.getNumberOfPartitions();i++){
			if(couldHashPruned(i,dataflow.property_.partitioner,dataflow))//is filtered
			{
				dataflow.property_.partitioner.getPartition(i)->setFiltered();
			}
		}
	}

	return dataflow;
}
BlockStreamIteratorBase* Filter::getIteratorTree(const unsigned& blocksize){
	BlockStreamIteratorBase* child_iterator=child_->getIteratorTree(blocksize);
	ExpandableBlockStreamFilter::State state;
	state.block_size_=blocksize;
	state.child_=child_iterator;
	state.comparator_list_=comparator_list_;
	Dataflow dataflow=getDataflow();
	state.schema_=getSchema(dataflow.attribute_list_);
	BlockStreamIteratorBase* filter=new ExpandableBlockStreamFilter(state);
	return filter;
}

bool Filter::couldHashPruned(unsigned partition_id,const DataflowPartitionDescriptor& part,const Dataflow& dataflow){
	for(unsigned i=0;i<comparator_list_.size();i++){
		const unsigned comparator_attribute_index=comparator_list_[i].get_index();
		if(part.getPartitionKey()==dataflow.attribute_list_[comparator_attribute_index]){
			if(comparator_list_[i].getCompareType()==Comparator::EQ){
				if(partition_id==part.getPartitionKey().attrType->operate->getPartitionValue(comparator_list_[i].get_value(),part.getPartitionFunction())){

				}
				else{/* the data flow on this partition is filtered.*/
					return true;

				}


			}

		}
	}
	return false;
}
