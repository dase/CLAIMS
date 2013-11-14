/*
 * EqualJoin.cpp
 *
 *  Created on: Nov 10, 2013
 *      Author: wangli
 */

#include "EqualJoin.h"
#include "../BlockStreamIterator/ParallelBlockStreamIterator/BlockStreamJoinIterator.h"
#include "../BlockStreamIterator/ParallelBlockStreamIterator/ExpandableBlockStreamExchangeEpoll.h"
#include "../IDsGenerator.h"

EqualJoin::EqualJoin(std::vector<JoinPair> joinpair_list,LogicalOperator* left_input,LogicalOperator* right_input)
:joinkey_pair_list_(joinpair_list),left_child_(left_input),right_child_(right_input),join_police_(na),dataflow_(0){
	for(unsigned i=0;i<joinpair_list.size();i++){
		left_join_key_list_.push_back(joinpair_list[i].first);
		right_join_key_list_.push_back(joinpair_list[i].second);
	}
}

EqualJoin::~EqualJoin() {
	// TODO Auto-generated destructor stub
}

Dataflow EqualJoin::getDataflow(){
	if(dataflow_!=0){
		/* the data flow has been computed*/
		return *dataflow_;
	}


	/** in the current implementation, only the hash join is considered**/
	Dataflow left_dataflow=left_child_->getDataflow();
	Dataflow right_dataflow=right_child_->getDataflow();
	Dataflow ret;

	const bool left_dataflow_key_partitioned=canLeverageHashPartition(left_join_key_list_,left_dataflow.property_.partitioner);
	const bool right_dataflow_key_partitioned=canLeverageHashPartition(right_join_key_list_,right_dataflow.property_.partitioner);

	const Attribute left_partition_key=left_dataflow.property_.partitioner.getPartitionKey();
	const Attribute right_partition_key=right_dataflow.property_.partitioner.getPartitionKey();
	if(left_dataflow_key_partitioned&&right_dataflow_key_partitioned){
		if(isEqualCondition(left_partition_key,right_partition_key)){
			/** the best situation**/
			if(left_dataflow.property_.partitioner.hasSamePartitionLocation(right_dataflow.property_.partitioner)){
				join_police_=no_repartition;

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
			printf("no_repartition\n");
			ret.attribute_list_.insert(ret.attribute_list_.end(),left_dataflow.attribute_list_.begin(),left_dataflow.attribute_list_.end());
//			ret.attribute_list_.insert(ret.attribute_list_.end(),right_dataflow.attribute_list_.begin(),right_dataflow.attribute_list_.end());
			/*use the left partitioner as the output dataflow partitioner.
			 * TODO: in fact, the output dataflow partitioner should contains both
			 * left partitioner and right partitioner.
			 */
			ret.property_.partitioner=left_dataflow.property_.partitioner;
			ret.property_.commnication_cost=left_dataflow.property_.commnication_cost+right_dataflow.property_.commnication_cost;
			break;
		}
		case left_repartition:{
			printf("left_repartition\n");
//			ret.attribute_list_.insert(ret.attribute_list_.end(),left_dataflow.attribute_list_.begin(),left_dataflow.attribute_list_.end());
			ret.attribute_list_.insert(ret.attribute_list_.end(),right_dataflow.attribute_list_.begin(),right_dataflow.attribute_list_.end());
			ret.property_.partitioner=right_dataflow.property_.partitioner;
			ret.property_.commnication_cost=left_dataflow.property_.commnication_cost+right_dataflow.property_.commnication_cost;
			ret.property_.commnication_cost+=left_dataflow.property_.partitioner.getAggregatedDatasize();
			break;
		}
		case right_repartition:{
			printf("right_repartition\n");
			ret.attribute_list_.insert(ret.attribute_list_.end(),left_dataflow.attribute_list_.begin(),left_dataflow.attribute_list_.end());
//			ret.attribute_list_.insert(ret.attribute_list_.end(),right_dataflow.attribute_list_.begin(),right_dataflow.attribute_list_.end());
			ret.property_.partitioner=left_dataflow.property_.partitioner;
			ret.property_.commnication_cost=left_dataflow.property_.commnication_cost+right_dataflow.property_.commnication_cost;
			ret.property_.commnication_cost+=right_dataflow.property_.partitioner.getAggregatedDatasize();
			break;
		}
		case complete_repartition:{
			/** the repartition strategy (e.g., the degree of parallelism and the partition function) in such case is not decided by
			 * any child data flow. Additional optimization can be made by adopting the partition strategy which benefits the remaining
			 * work.TODO.
			 */
			printf("complete_repartition\n");
			ret.attribute_list_.insert(ret.attribute_list_.end(),left_dataflow.attribute_list_.begin(),left_dataflow.attribute_list_.end());
//			ret.attribute_list_.insert(ret.attribute_list_.end(),right_dataflow.attribute_list_.begin(),right_dataflow.attribute_list_.end());
			ret.property_.commnication_cost=left_dataflow.property_.commnication_cost+right_dataflow.property_.commnication_cost;
			ret.property_.commnication_cost+=left_dataflow.property_.partitioner.getAggregatedDatasize();
			ret.property_.commnication_cost+=right_dataflow.property_.partitioner.getAggregatedDatasize();
			printf("This is not implemented, as I'm very lazy. -_- \n");
			assert(false);
			break;
		}
		default:{
			printf("The join police has not been decided!\n");
			assert(false);
			break;
		}
	}

	dataflow_=new Dataflow();
	*dataflow_=ret;

	return ret;
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

bool EqualJoin::isEqualCondition(const Attribute& a1,const Attribute& a2)const{
	for(unsigned i=0;i<joinkey_pair_list_.size();i++){
		if(a1==joinkey_pair_list_[i].first&&a2==joinkey_pair_list_[i].second){
			return true;
		}
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

BlockStreamIteratorBase* EqualJoin::getIteratorTree(const unsigned& block_size){
	BlockStreamJoinIterator* join_iterator;
	BlockStreamIteratorBase* child_iterator_left=left_child_->getIteratorTree(block_size);
	BlockStreamIteratorBase* child_iterator_right=right_child_->getIteratorTree(block_size);
	Dataflow dataflow_left=left_child_->getDataflow();
	Dataflow dataflow_right=right_child_->getDataflow();
	BlockStreamJoinIterator::State state;
	state.block_size_=block_size;
//	state.child_left=child_iterator_left;
//	state.child_right=child_iterator_right;

	state.ht_nbuckets=1024*1024;
	state.input_schema_left=getSchema(dataflow_left.attribute_list_);
	state.input_schema_right=getSchema(dataflow_right.attribute_list_);
	/* the bucket size is lager than one tuple and is 64-byte-aligned */
	state.ht_bucketsize=((state.input_schema_left->getTupleMaxSize()-1)/64+1)*64;
	state.output_schema=getSchema(dataflow_->attribute_list_);

	state.joinIndex_left=getLeftJoinKeyIndexList();
	state.joinIndex_right=getRightJoinKeyIndexList();

	state.payload_left=getLeftPayloadIndexList();
	state.payload_right=getRightPayloadIndexList();



	switch(join_police_){
	case no_repartition:{
		state.child_left=child_iterator_left;
		state.child_right=child_iterator_right;

		join_iterator=new BlockStreamJoinIterator(state);
		break;
	}
	case left_repartition:{
//		state.child_left
		NodeTracker* node_tracker=NodeTracker::getInstance();
		ExpandableBlockStreamExchangeEpoll::State exchange_state;
		exchange_state.block_size=block_size;
		exchange_state.child=child_iterator_left;
		exchange_state.exchange_id=IDsGenerator::getInstance()->generateUniqueExchangeID();

		std::vector<NodeID> upper_id_list=getInvolvedNodeID(dataflow_->property_.partitioner);
		exchange_state.upper_ip_list=convertNodeIDListToNodeIPList(upper_id_list);

		std::vector<NodeID> lower_id_list=getInvolvedNodeID(dataflow_left.property_.partitioner);
		exchange_state.lower_ip_list=convertNodeIDListToNodeIPList(lower_id_list);

//		state.partition_index=
		const Attribute right_partition_key=dataflow_->property_.partitioner.getPartitionKey();

		/* get the left attribute that is corresponding to the partition key.*/
		Attribute left_partition_key=joinkey_pair_list_[getIndexInRightJoinKeyList(right_partition_key)].first;

		exchange_state.partition_index=getIndexInAttributeList(dataflow_left.attribute_list_,left_partition_key);


		exchange_state.schema=getSchema(dataflow_left.attribute_list_,dataflow_right.attribute_list_);
		BlockStreamIteratorBase* exchange=new ExpandableBlockStreamExchangeEpoll(exchange_state);
		state.child_left=exchange;
		state.child_right=child_iterator_right;
		join_iterator=new BlockStreamJoinIterator(state);

		break;
	}
	case right_repartition:{
		NodeTracker* node_tracker=NodeTracker::getInstance();
		ExpandableBlockStreamExchangeEpoll::State exchange_state;
		exchange_state.block_size=block_size;
		exchange_state.child=child_iterator_left;
		exchange_state.exchange_id=IDsGenerator::getInstance()->generateUniqueExchangeID();

		std::vector<NodeID> upper_id_list=getInvolvedNodeID(dataflow_->property_.partitioner);
		exchange_state.upper_ip_list=convertNodeIDListToNodeIPList(upper_id_list);

		std::vector<NodeID> lower_id_list=getInvolvedNodeID(dataflow_right.property_.partitioner);
		exchange_state.lower_ip_list=convertNodeIDListToNodeIPList(lower_id_list);

//		state.partition_index=
		const Attribute left_partition_key=dataflow_->property_.partitioner.getPartitionKey();

		/* get the right attribute that is corresponding to the partition key.*/
		Attribute right_partition_key=joinkey_pair_list_[getIndexInLeftJoinKeyList(left_partition_key)].second;

		exchange_state.partition_index=getIndexInAttributeList(dataflow_right.attribute_list_,right_partition_key);


		exchange_state.schema=getSchema(dataflow_right.attribute_list_,dataflow_left.attribute_list_);
		BlockStreamIteratorBase* exchange=new ExpandableBlockStreamExchangeEpoll(exchange_state);
		state.child_left=exchange;
		state.child_right=child_iterator_right;
		join_iterator=new BlockStreamJoinIterator(state);
		break;
	}
	case complete_repartition:{
		printf("the join partiotin that needs to repartition each input data flow is NOT implemented yet!\n");
		assert(false);
		break;
	}
	default:{
		break;
	}
	}
}
std::vector<unsigned> EqualJoin::getLeftJoinKeyIndexList()const{
	std::vector<unsigned> ret;
	const Dataflow dataflow=left_child_->getDataflow();
	for(unsigned i=0;i<joinkey_pair_list_.size();i++){
		for(unsigned j=0;j<dataflow.attribute_list_.size();j++){
			if(joinkey_pair_list_[i].first==dataflow.attribute_list_[j]){
				ret.push_back(j);
			}
		}
	}
	return ret;
}

std::vector<unsigned> EqualJoin::getRightJoinKeyIndexList()const{
	std::vector<unsigned> ret;
	const Dataflow dataflow=right_child_->getDataflow();
	for(unsigned i=0;i<joinkey_pair_list_.size();i++){
		for(unsigned j=0;j<dataflow.attribute_list_.size();j++){
			if(joinkey_pair_list_[i].second==dataflow.attribute_list_[j]){
				ret.push_back(j);
			}
		}
	}
	return ret;
}
std::vector<unsigned> EqualJoin::getLeftPayloadIndexList()const{
	std::vector<unsigned> ret;
	const Dataflow dataflow=left_child_->getDataflow();
	for(unsigned i=0;i<joinkey_pair_list_.size();i++){
		for(unsigned j=0;j<dataflow.attribute_list_.size();j++){
			if(joinkey_pair_list_[i].first==dataflow.attribute_list_[j]){
				break;
			}
			else{
				ret.push_back(j);
			}
		}
	}
	return ret;

}

std::vector<unsigned> EqualJoin::getRightPayloadIndexList()const{
	std::vector<unsigned> ret;
	const Dataflow dataflow=right_child_->getDataflow();
	for(unsigned i=0;i<joinkey_pair_list_.size();i++){
		for(unsigned j=0;j<dataflow.attribute_list_.size();j++){
			if(joinkey_pair_list_[i].second==dataflow.attribute_list_[j]){
				break;
			}
			else{
				ret.push_back(j);
			}
		}
	}
	return ret;

}
int EqualJoin::getIndexInLeftJoinKeyList(const Attribute& attribute)const{
	for(unsigned i=0;i<joinkey_pair_list_.size();i++){
		if(joinkey_pair_list_[i].first==attribute){
			return i;
		}
	}
	return -1;
}
int EqualJoin::getIndexInRightJoinKeyList(const Attribute& attribute)const{
	for(unsigned i=0;i<joinkey_pair_list_.size();i++){
		if(joinkey_pair_list_[i].second==attribute){
			return i;
		}
	}
	return -1;
}
int EqualJoin::getIndexInAttributeList(const std::vector<Attribute>& attributes,const Attribute& attribute)const{
	for(unsigned i=0;i<attributes.size();i++){
		if(attributes[i]==attribute){
			return i;
		}
	}
	return -1;
}
