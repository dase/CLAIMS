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
#include "../BlockStreamIterator/ParallelBlockStreamIterator/BlockStreamExpander.h"
EqualJoin::EqualJoin(std::vector<JoinPair> joinpair_list,LogicalOperator* left_input,LogicalOperator* right_input)
:joinkey_pair_list_(joinpair_list),left_child_(left_input),right_child_(right_input),join_police_(na),dataflow_(0){
	for(unsigned i=0;i<joinpair_list.size();i++){
		left_join_key_list_.push_back(joinpair_list[i].first);
		right_join_key_list_.push_back(joinpair_list[i].second);
	}
	print();
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
//				join_police_=left_repartition;

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
			join_police_=right_repartition;
		}
		if(!left_dataflow_key_partitioned&&right_dataflow_key_partitioned){
			join_police_=left_repartition;
		}
		if(!left_dataflow_key_partitioned&&!right_dataflow_key_partitioned)
			join_police_=complete_repartition;
	}


	/**finally, construct the output data flow according to the join police**/
	switch(join_police_){
		case no_repartition:{
			printf("no_repartition\n");
			ret.attribute_list_.insert(ret.attribute_list_.end(),left_dataflow.attribute_list_.begin(),left_dataflow.attribute_list_.end());
			ret.attribute_list_.insert(ret.attribute_list_.end(),right_dataflow.attribute_list_.begin(),right_dataflow.attribute_list_.end());
			/*use the left partitioner as the output dataflow partitioner.
			 * TODO: in fact, the output dataflow partitioner should contains both
			 * left partitioner and right partitioner.
			 */
//			ret.property_.partitioner=left_dataflow.property_.partitioner;
			ret.property_.partitioner.setPartitionList(left_dataflow.property_.partitioner.getPartitionList());
			ret.property_.partitioner.setPartitionFunction(left_dataflow.property_.partitioner.getPartitionFunction());
			ret.property_.partitioner.setPartitionKey(left_partition_key);
			ret.property_.partitioner.addShadowPartitionKey(right_partition_key);

			/* set the generated data size.
			 * Currently, we assume the generated data size is the sum of input data volumn.
			 * TODO: some reasonable output size estimation is needed.
			 */
			for(unsigned i=0;i<ret.property_.partitioner.getNumberOfPartitions();i++){
				const unsigned l_size=left_dataflow.property_.partitioner.getPartition(i)->getDataSize();
				const unsigned r_size=right_dataflow.property_.partitioner.getPartition(i)->getDataSize();
				ret.property_.partitioner.getPartition(i)->setDataSize(l_size+r_size);
			}

			ret.property_.commnication_cost=left_dataflow.property_.commnication_cost+right_dataflow.property_.commnication_cost;
			break;
		}
		case left_repartition:{
			printf("left_repartition\n");
			ret.attribute_list_.insert(ret.attribute_list_.end(),left_dataflow.attribute_list_.begin(),left_dataflow.attribute_list_.end());
			ret.attribute_list_.insert(ret.attribute_list_.end(),right_dataflow.attribute_list_.begin(),right_dataflow.attribute_list_.end());
//			ret.property_.partitioner=right_dataflow.property_.partitioner;

			ret.property_.partitioner.setPartitionList(right_dataflow.property_.partitioner.getPartitionList());
			ret.property_.partitioner.setPartitionFunction(right_dataflow.property_.partitioner.getPartitionFunction());
			ret.property_.partitioner.setPartitionKey(right_dataflow.property_.partitioner.getPartitionKey());
//			ret.property_.partitioner.addShadowPartitionKey(right_partition_key);
			/* set the generated data size*/
			const unsigned left_total_size=left_dataflow.property_.partitioner.getAggregatedDatasize();
			const unsigned right_partition_count=right_dataflow.property_.partitioner.getNumberOfPartitions();
			for(unsigned i=0;i<ret.property_.partitioner.getNumberOfPartitions();i++){
				const unsigned r_size=right_dataflow.property_.partitioner.getPartition(i)->getDataSize();
				ret.property_.partitioner.getPartition(i)->setDataSize(r_size+left_total_size/right_partition_count);
			}

			ret.property_.commnication_cost=left_dataflow.property_.commnication_cost+right_dataflow.property_.commnication_cost;
			ret.property_.commnication_cost+=left_dataflow.property_.partitioner.getAggregatedDatasize();
			break;
		}
		case right_repartition:{
			printf("right_repartition\n");
			ret.attribute_list_.insert(ret.attribute_list_.end(),left_dataflow.attribute_list_.begin(),left_dataflow.attribute_list_.end());
			ret.attribute_list_.insert(ret.attribute_list_.end(),right_dataflow.attribute_list_.begin(),right_dataflow.attribute_list_.end());
//			ret.property_.partitioner=left_dataflow.property_.partitioner;

			ret.property_.partitioner.setPartitionList(left_dataflow.property_.partitioner.getPartitionList());
			ret.property_.partitioner.setPartitionFunction(left_dataflow.property_.partitioner.getPartitionFunction());
			ret.property_.partitioner.setPartitionKey(left_dataflow.property_.partitioner.getPartitionKey());
//			ret.property_.partitioner.addShadowPartitionKey(right_partition_key);
			/* set the generated data size*/
			const unsigned right_total_size=right_dataflow.property_.partitioner.getAggregatedDatasize();
			const unsigned left_partition_count=left_dataflow.property_.partitioner.getNumberOfPartitions();
			for(unsigned i=0;i<ret.property_.partitioner.getNumberOfPartitions();i++){
				const unsigned l_size=left_dataflow.property_.partitioner.getPartition(i)->getDataSize();
				ret.property_.partitioner.getPartition(i)->setDataSize(l_size+right_total_size/left_partition_count);
			}
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
			ret.attribute_list_.insert(ret.attribute_list_.end(),right_dataflow.attribute_list_.begin(),right_dataflow.attribute_list_.end());
			ret.property_.commnication_cost=left_dataflow.property_.commnication_cost+right_dataflow.property_.commnication_cost;
			ret.property_.commnication_cost+=left_dataflow.property_.partitioner.getAggregatedDatasize();
			ret.property_.commnication_cost+=right_dataflow.property_.partitioner.getAggregatedDatasize();

			ret.property_.partitioner=decideOutputDataflowProperty(left_dataflow,right_dataflow);


//
//			printf("[Complete_repartition hash join] is not implemented, because I'm very lazy. -_- \n");
//			assert(false);
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
	return part.getPartitionKey()==key;
}
bool EqualJoin::canLeverageHashPartition(const std::vector<Attribute> &partition_key_list,const DataflowPartitioningDescriptor& partitoiner)const{
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

	state.ht_nbuckets=1024*1024;
	state.input_schema_left=getSchema(dataflow_left.attribute_list_);
	state.input_schema_right=getSchema(dataflow_right.attribute_list_);
	state.ht_schema=getSchema(dataflow_left.attribute_list_);
	/* the bucket size is 64-byte-aligned */
//	state.ht_bucketsize=((state.input_schema_left->getTupleMaxSize()-1)/64+1)*64;
	/*
	 * In the initial implementation, I set the bucket size to be up round to cache line
	 * size, e.g., 64Bytes. Finally, I realized that different from aggregation, the hash
	 * table bucket in the build phase of hash join is filled very quickly and hence a
	 * a relatively large bucket size could reduce the number of overflowing buckets and
	 * avoid the random memory access caused by acceesing overflowing buckets.
	 */
	state.ht_bucketsize=128;
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
			BlockStreamExpander::State expander_state;
			expander_state.block_count_in_buffer_=10;
			expander_state.block_size_=block_size;
			expander_state.thread_count_=1;
			expander_state.child_=child_iterator_left;
			expander_state.schema_=getSchema(dataflow_left.attribute_list_);
			BlockStreamIteratorBase* expander=new BlockStreamExpander(expander_state);


			NodeTracker* node_tracker=NodeTracker::getInstance();
			ExpandableBlockStreamExchangeEpoll::State exchange_state;
			exchange_state.block_size=block_size;
			exchange_state.child=expander;//child_iterator_left;
			exchange_state.exchange_id=IDsGenerator::getInstance()->generateUniqueExchangeID();

			std::vector<NodeID> upper_id_list=getInvolvedNodeID(dataflow_->property_.partitioner);
			exchange_state.upper_ip_list=convertNodeIDListToNodeIPList(upper_id_list);

			std::vector<NodeID> lower_id_list=getInvolvedNodeID(dataflow_left.property_.partitioner);
			exchange_state.lower_ip_list=convertNodeIDListToNodeIPList(lower_id_list);


			const Attribute right_partition_key=dataflow_->property_.partitioner.getPartitionKey();

			/* get the left attribute that is corresponding to the partition key.*/
			Attribute left_partition_key=joinkey_pair_list_[getIndexInRightJoinKeyList(right_partition_key)].first;

			exchange_state.partition_key_index=getIndexInAttributeList(dataflow_left.attribute_list_,left_partition_key);


//			exchange_state.schema=getSchema(dataflow_left.attribute_list_,dataflow_right.attribute_list_);
			exchange_state.schema=getSchema(dataflow_left.attribute_list_);
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


			const Attribute left_partition_key=dataflow_->property_.partitioner.getPartitionKey();

			if(exchange_state.exchange_id==0){
				printf("0\n");
			}
			/* get the right attribute that is corresponding to the partition key.*/
			Attribute right_partition_key;
			if(dataflow_->property_.partitioner.hasShadowPartitionKey()){
				right_partition_key=joinkey_pair_list_[getIndexInLeftJoinKeyList(left_partition_key,dataflow_->property_.partitioner.getShadowAttributeList())].second;
			}
			else{
				right_partition_key=joinkey_pair_list_[getIndexInLeftJoinKeyList(left_partition_key)].second;
			}


			exchange_state.partition_key_index=getIndexInAttributeList(dataflow_right.attribute_list_,right_partition_key);


			exchange_state.schema=getSchema(dataflow_left.attribute_list_);
			BlockStreamIteratorBase* exchange=new ExpandableBlockStreamExchangeEpoll(exchange_state);
			state.child_left=exchange;
			state.child_right=child_iterator_right;
			join_iterator=new BlockStreamJoinIterator(state);
			break;
		}
		case complete_repartition:{

			/* build left input*/
			ExpandableBlockStreamExchangeEpoll::State l_exchange_state;
			l_exchange_state.block_size=block_size;
			l_exchange_state.child=child_iterator_left;
			l_exchange_state.exchange_id=IDsGenerator::getInstance()->generateUniqueExchangeID();

			std::vector<NodeID> lower_id_list=getInvolvedNodeID(dataflow_left.property_.partitioner);
			l_exchange_state.lower_ip_list=convertNodeIDListToNodeIPList(lower_id_list);

			std::vector<NodeID> upper_id_list=getInvolvedNodeID(dataflow_->property_.partitioner);
			l_exchange_state.upper_ip_list=convertNodeIDListToNodeIPList(upper_id_list);

			const Attribute left_partition_key=dataflow_->property_.partitioner.getPartitionKey();
			l_exchange_state.partition_key_index=getIndexInAttributeList(dataflow_left.attribute_list_,left_partition_key);
			l_exchange_state.schema=getSchema(dataflow_left.attribute_list_);
			BlockStreamIteratorBase* l_exchange=new ExpandableBlockStreamExchangeEpoll(l_exchange_state);

			/*build right input*/
			ExpandableBlockStreamExchangeEpoll::State r_exchange_state;
			r_exchange_state.block_size=block_size;
			r_exchange_state.child=child_iterator_right;
			r_exchange_state.exchange_id=IDsGenerator::getInstance()->generateUniqueExchangeID();

			lower_id_list=getInvolvedNodeID(dataflow_right.property_.partitioner);
			r_exchange_state.lower_ip_list=convertNodeIDListToNodeIPList(lower_id_list);

			upper_id_list=getInvolvedNodeID(dataflow_->property_.partitioner);
			r_exchange_state.upper_ip_list=convertNodeIDListToNodeIPList(upper_id_list);

			const Attribute right_partition_key=joinkey_pair_list_[getIndexInLeftJoinKeyList(left_partition_key)].second;
			r_exchange_state.partition_key_index=getIndexInAttributeList(dataflow_right.attribute_list_,right_partition_key);
			r_exchange_state.schema=getSchema(dataflow_right.attribute_list_);
			BlockStreamIteratorBase* r_exchange=new ExpandableBlockStreamExchangeEpoll(r_exchange_state);

			/* finally  build the join iterator itself*/
			state.child_left=l_exchange;
			state.child_right=r_exchange;
			join_iterator=new BlockStreamJoinIterator(state);
			break;


		}
		default:{
			break;
		}
	}
	return join_iterator;
}

bool EqualJoin::GetOptimalPhysicalPlan(Requirement requirement,PhysicalPlanDescriptor& physical_plan_descriptor, const unsigned & block_size){

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
	const std::vector<unsigned> left_join_key_index_list=getLeftJoinKeyIndexList();

	for(unsigned i=0;i<dataflow.attribute_list_.size();i++){
		for(unsigned j=0;j<left_join_key_index_list.size();j++){
			if(i==left_join_key_index_list[j]){
				break;
			}

		}
		ret.push_back(i);
	}
	return ret;

//	for(unsigned i=0;i<joinkey_pair_list_.size();i++){
//		for(unsigned j=0;j<dataflow.attribute_list_.size();j++){
//			if(joinkey_pair_list_[i].first==dataflow.attribute_list_[j]){
//				break;
//			}
//			else{
//				ret.push_back(j);
//			}
//		}
//	}
//	return ret;

}

std::vector<unsigned> EqualJoin::getRightPayloadIndexList()const{
	std::vector<unsigned> ret;
	const Dataflow dataflow=right_child_->getDataflow();
	const std::vector<unsigned> right_join_key_index_list=getRightJoinKeyIndexList();

	for(unsigned i=0;i<dataflow.attribute_list_.size();i++){
		for(unsigned j=0;j<right_join_key_index_list.size();j++){
			if(i==right_join_key_index_list[j]){
				break;
			}

		}
		ret.push_back(i);
	}
	return ret;

}
int EqualJoin::getIndexInLeftJoinKeyList(const Attribute& attribute)const{
	for(unsigned i=0;i<joinkey_pair_list_.size();i++){
		if(joinkey_pair_list_[i].first==attribute){
			return i;
		}
	}
	assert(false);
	return -1;
}
int EqualJoin::getIndexInLeftJoinKeyList(const Attribute& attribute,const std::vector<Attribute> shadow_attribute_list )const{
	for(unsigned i=0;i<joinkey_pair_list_.size();i++){
		if(joinkey_pair_list_[i].first==attribute){
			return i;
		}
	}
	/* the attribute fails to match any join key.
	 * Now we try to match the shadow partition attribute(s)*/
	for(unsigned s=0;s<shadow_attribute_list.size();s++){
		for(unsigned i=0;i<joinkey_pair_list_.size();i++){
			if(joinkey_pair_list_[i].first==shadow_attribute_list[s]){
				return i;
			}
		}
	}

	/*
	 * neither the partition attribute nor the shadow partition attribute could match any join key.
	 */
	assert(false);
	return -1;

}
int EqualJoin::getIndexInRightJoinKeyList(const Attribute& attribute)const{
	for(unsigned i=0;i<joinkey_pair_list_.size();i++){
		if(joinkey_pair_list_[i].second==attribute){
			return i;
		}
	}
	assert(false);
	return -1;
}
int EqualJoin::getIndexInRightJoinKeyList(const Attribute& attribute,const std::vector<Attribute> shadow_attribute_list )const{
	for(unsigned i=0;i<joinkey_pair_list_.size();i++){
		if(joinkey_pair_list_[i].second==attribute){
			return i;
		}
	}
	/* the attribute fails to match any join key.
	 * Now we try to match the shadow partition attribute(s)*/
	for(unsigned s=0;s<shadow_attribute_list.size();s++){
		for(unsigned i=0;i<joinkey_pair_list_.size();i++){
			if(joinkey_pair_list_[i].second==shadow_attribute_list[s]){
				return i;
			}
		}
	}

	/*
	 * neither the partition attribute nor the shadow partition attribute could match any join key.
	 */
	assert(false);
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
DataflowPartitioningDescriptor EqualJoin::decideOutputDataflowProperty(const Dataflow& left_dataflow,const Dataflow& right_dataflow)const{
	DataflowPartitioningDescriptor ret;

	const unsigned l_datasize=left_dataflow.getAggregatedDatasize();
	const unsigned r_datasize=right_dataflow.getAggregatedDatasize();

	std::vector<NodeID> all_node_id_list=NodeTracker::getInstance()->getNodeIDList();
	/* In the current implementation, all the nodes are involved in the complete_repartition method.
	 * TODO decide the degree of parallelism*/
	const unsigned degree_of_parallelism=all_node_id_list.size();

	std::vector<DataflowPartition> dataflow_partition_list;
	for(unsigned i=0;i<degree_of_parallelism;i++){
		const NodeID location=all_node_id_list[i];

		/* Currently, the join output size cannot be predicted due to the absence of data statistics.
		 * We just use the magic number as following */
		const unsigned datasize=l_datasize/degree_of_parallelism+r_datasize/degree_of_parallelism;
		DataflowPartition dfp(i,datasize,location);
		dataflow_partition_list.push_back(dfp);
	}
	ret.setPartitionList(dataflow_partition_list);
	ret.setPartitionKey(joinkey_pair_list_[0].first);
	ret.addShadowPartitionKey(joinkey_pair_list_[0].second);
	PartitionFunction* partition_function=PartitionFunctionFactory::createBoostHashFunction(degree_of_parallelism);
	ret.setPartitionFunction(partition_function);
	return ret;

}
void EqualJoin::print()const{
	printf("Join--------------\n");
	for(unsigned i=0;i<this->joinkey_pair_list_.size();i++){
		printf("%s=%s\n",joinkey_pair_list_[i].first.attrName.c_str(),joinkey_pair_list_[i].second.attrName.c_str());
	}
	printf("-----------------\n");
}
