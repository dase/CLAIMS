/*
 * Aggregation.cpp
 *
 *  Created on: Nov 11, 2013
 *      Author: wangli
 */

#include "Aggregation.h"

Aggregation::Aggregation(std::vector<Attribute> group_by_attribute_list,std::vector<Attribute> aggregation_attribute_list,std::vector<AggregationIterator::State::aggregation> aggregation_list,LogicalOperator* child)
:group_by_attribute_list_(group_by_attribute_list),aggregation_attribute_list_(aggregation_attribute_list),aggregation_list_(aggregation_list),dataflow_(0),child_(child){

}

Aggregation::~Aggregation() {
	// TODO Auto-generated destructor stub
}
Dataflow Aggregation::getDataflow(){
	if(dataflow_!=0)
		return *dataflow_;
	Dataflow ret;
	const Dataflow child_dataflow=child_->getDataflow();
	if(canLeverageHashPartition(child_dataflow)){
		/*
		 * the aggregated groups in each partition does not overlap, and hence no merge aggregation
		 *  is need to get the final result.
		 */
		ret.attribute_list_.insert(ret.attribute_list_.end(),group_by_attribute_list_.begin(),group_by_attribute_list_.end());
		ret.attribute_list_.insert(ret.attribute_list_.end(),aggregation_attribute_list_.begin(),aggregation_attribute_list_.end());
		ret.property_=child_dataflow.property_;
		return ret;
	}
	else{
		printf("The aggregation that cannot leverage input dataflow partition is not implemented, now!\n");
		assert(false);
		return ret;
	}

}

bool Aggregation::canLeverageHashPartition(const Dataflow& child_dataflow)const{
	if(!child_dataflow.isHashPartitioned())
		return false;
	/** the hash property of the input data flow can be leveraged in the aggragation as long
	 * as the hash attribute is one of the group-by attributes.
	 */
	for(unsigned i=0;i<group_by_attribute_list_.size();i++){
		if(group_by_attribute_list_[i]==child_dataflow.property_.partitioner.getPartitionKey())
			return true;
	}
	return false;
}
