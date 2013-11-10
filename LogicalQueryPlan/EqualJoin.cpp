/*
 * EqualJoin.cpp
 *
 *  Created on: Nov 10, 2013
 *      Author: wangli
 */

#include "EqualJoin.h"

EqualJoin::EqualJoin(std::vector<JoinPair> joinpair_list,LogicalOperator* left_input,LogicalOperator* right_input)
:joinkey_pair_list_(joinpair_list),left_child_(left_input),right_child_(right_input),join_police_(na){
	for(unsigned i=0;i<joinpair_list.size();i++){
		left_join_key_list_.push_back(joinpair_list[i].first);
		right_join_key_list_.push_back(joinpair_list[i].second);
	}
}

EqualJoin::~EqualJoin() {
	// TODO Auto-generated destructor stub
}

Dataflow EqualJoin::getDataflow(){
	/** in the current implementation, only the hash join is considered**/
	Dataflow left_dataflow=left_child_->getDataflow();
	Dataflow right_dataflow=right_child_->getDataflow();
	Dataflow ret;

	const bool left_dataflow_key_partitioned=canLeverageHashPartition(left_join_key_list_,left_dataflow.property_.partitioner);
	const bool right_dataflow_key_partitioned=canLeverageHashPartition(right_join_key_list_,right_dataflow.property_.partitioner);

	const Attribute left_partition_key=left_dataflow.property_.partitioner.getPartitionKey();
	const Attribute right_partition_key=right_dataflow.property_.partitioner.getPartitionKey();
	if(left_dataflow_key_partitioned&&right_dataflow_key_partitioned){
		if(left_partition_key==right_partition_key){
			/** the best situation**/
			if(left_dataflow.property_.partitioner.hasSamePartitionLocation(right_dataflow.property_.partitioner)){
				join_police_=no_repartition;
				ret.attribute_list_.insert(ret.attribute_list_.end(),left_dataflow.attribute_list_.begin(),left_dataflow.attribute_list_.end());
				ret.attribute_list_.insert(ret.attribute_list_.end(),right_dataflow.attribute_list_.begin(),right_dataflow.attribute_list_.end());

				return ret;
			}
			else{
				join_police_=decideLeftOrRightRepartition(left_dataflow,right_dataflow);
			}
		}
		else{
			join_police_=decideLeftOrRightRepartition(left_dataflow,right_dataflow);
		}
	}
	else{
		if(left_dataflow_key_partitioned&&!right_dataflow_key_partitioned){
			join_police_=left_repartition;
		}
		if(!left_dataflow_key_partitioned&&right_dataflow_key_partitioned){
			join_police_=right_repartition;
		}
		if(!left_dataflow_key_partitioned&&!right_dataflow_key_partitioned)
			join_police_=complete_repartition;
	}


	/**finally, construct the output data flow according to the join police**/
	switch(join_police_){
		case no_repartition:{

			break;
		}
		case left_repartition:{
			break;
		}
		case right_repartition:{
			break;
		}
		case complete_repartition:{
			break;
		}
	}



	return left_dataflow;
}

bool EqualJoin::isHashOnLeftKey(const Partitioner& part,const Attribute& key)const{
	if(part.getPartitionFashion()!=PartitionFunction::hash_f)
		return false;
	for(unsigned i=0;i<joinkey_pair_list_.size();i++){

	}
	return *(part.getPartitionKey())==key;
}
bool EqualJoin::canLeverageHashPartition(const std::vector<Attribute> &partition_key_list,const DataflowPartitionDescriptor& partitoiner)const{
	Attribute attribute=partitoiner.getPartitionKey();
	for(unsigned i=0;i<partition_key_list.size();i++){
		if(attribute==partition_key_list[i])
			return true;
	}
	return false;
}
EqualJoin::JoinPolice EqualJoin::decideLeftOrRightRepartition(const Dataflow& left_dataflow,const Dataflow& right_dataflow)const{
	const unsigned left_data_size=left_dataflow.getAggregatedDatasize();
	const unsigned right_data_size=right_dataflow.getAggregatedDatasize();
	if(left_data_size>right_data_size){
		return right_repartition;
	}
	else{
		return left_repartition;
	}
}
