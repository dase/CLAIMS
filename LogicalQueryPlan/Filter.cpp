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
	generateComparatorList(dataflow);


	if(dataflow.isHashPartitioned()){
		for(unsigned i=0;i<dataflow.property_.partitioner.getNumberOfPartitions();i++){
			if(couldHashPruned(i,dataflow.property_.partitioner))//is filtered
			{
				dataflow.property_.partitioner.getPartition(i)->setFiltered();
			}
			else{
				/*
				 * should predict the volume of data that passes the filter.
				 * TODO: a precious prediction is needed based on the statistic of
				 * the input data, which may be maintained in the catalog module.
				 */
				const unsigned before_filter_datasize=dataflow.property_.partitioner.getPartition(i)->getDataSize();
				const unsigned after_filter_datasize=before_filter_datasize*predictSelectivity();
				dataflow.property_.partitioner.getPartition(i)->setDataSize(after_filter_datasize);
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

bool Filter::couldHashPruned(unsigned partition_id,const DataflowPartitioningDescriptor& part)const{
//	for(unsigned i=0;i<comparator_list_.size();i++){
//		const unsigned comparator_attribute_index=comparator_list_[i].get_index();
//		if(part.getPartitionKey()==dataflow.attribute_list_[comparator_attribute_index]){
//			if(comparator_list_[i].getCompareType()==Comparator::EQ){
//				if(partition_id==part.getPartitionKey().attrType->operate->getPartitionValue(comparator_list_[i].get_value(),part.getPartitionFunction())){
//
//				}
//				else{/* the data flow on this partition is filtered.*/
//					return true;
//
//				}
//
//
//			}
//
//		}
//	}
//	return false;
	for(unsigned i=0;i<condition_.getCompaisonNumber();i++){
		if(part.getPartitionKey()==condition_.attribute_list_[i]){
			if(comparator_list_[i].getCompareType()==Comparator::EQ){
				if(partition_id==part.getPartitionKey().attrType->operate->getPartitionValue(comparator_list_[i].get_value(),part.getPartitionFunction())){

				}
				else{
					return true;
				}
			}
		}
	}
	return false;

}
float Filter::predictSelectivity()const{
	float ret=1;

	/*
	 * In the current version, due to the lack of statistic information, we only use a factor
	 * 0.5 for each comparison. TODO: a more precious prediction is greatly needed.
	 */
	for(unsigned i=0;i<condition_.getCompaisonNumber();i++){
		ret*=0.5;
	}
	return ret;
}
void Filter::Condition::add(const Attribute& attr,const FilterIterator::AttributeComparator::comparison& com,const void*const & const_value){
	attribute_list_.push_back(attr);
	comparison_list_.push_back(com);
	const unsigned value_length=attr.attrType->get_length();
	void* value=malloc(value_length);
	attr.attrType->operate->assignment(const_value,value);
	const_value_list_.push_back(value);
}
unsigned Filter::Condition::getCompaisonNumber()const{
	return attribute_list_.size();
}
Filter::Condition::~Condition(){
	for(unsigned i=0;i<const_value_list_.size();i++){
		free(const_value_list_[i]);
	}
}
Filter::Filter(const Condition& condition,LogicalOperator*  child):condition_(condition),child_(child){

}
void Filter::generateComparatorList(const Dataflow& dataflow){
	for(unsigned i=0;i<condition_.getCompaisonNumber();i++){
		int attribute_index=0;
		for(;attribute_index<dataflow.attribute_list_.size();attribute_index++){
			if(condition_.attribute_list_[i]==dataflow.attribute_list_[attribute_index]){
				break;
			}
		}
		if(attribute_index==dataflow.attribute_list_.size()){
			printf("the filter condition does match any input attribute! Rechech the filter condition or the filter transformation module.\n");
			assert(false);
		}
		FilterIterator::AttributeComparator filter(*dataflow.attribute_list_[attribute_index].attrType,condition_.comparison_list_[i],attribute_index,condition_.const_value_list_[i]);
		comparator_list_.push_back(filter);
	}
}
