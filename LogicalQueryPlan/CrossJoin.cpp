/*
 * CrossJoin.cpp
 *
 *  Created on: Jul 14, 2014
 *      Author: fzhedu, wangli1426
 */

#include "CrossJoin.h"
#include "../BlockStreamIterator/ParallelBlockStreamIterator/BlockStreamNestLoopJoinIterator.h"
#include "../BlockStreamIterator/ParallelBlockStreamIterator/ExpandableBlockStreamExchangeEpoll.h"
#include "../IDsGenerator.h"
#include "../BlockStreamIterator/ParallelBlockStreamIterator/BlockStreamExpander.h"
#include "../Config.h"

CrossJoin::CrossJoin():left_child_(0),right_child_(0),dataflow_(0)
{
	setOperatortype(l_cross_join);
}

CrossJoin::CrossJoin(LogicalOperator* left_input,LogicalOperator* right_input)
:left_child_(left_input),right_child_(right_input),dataflow_(0){
	setOperatortype(l_cross_join);
}
CrossJoin::~CrossJoin() {
	delete dataflow_;
	if(left_child_>0){
		delete left_child_;
	}
	if(right_child_>0){
		delete right_child_;
	}
}

Dataflow CrossJoin::getDataflow()
{
	if (0!=dataflow_)
	{
		/* the data flow has been computed*/
		return *dataflow_;
	}
	Dataflow left_dataflow=left_child_->getDataflow();
	Dataflow right_dataflow=right_child_->getDataflow();

	if(canLocalJoin(left_dataflow, right_dataflow)){
		join_police_=local_join;
	}
	else {
		if(left_dataflow.getAggregatedDatasize()<right_dataflow.getAggregatedDatasize()){
			/** left input is the smaller one **/
			join_police_=left_broadcast;
		}
		else {
			join_police_=right_broadcast;
		}

	}

	Dataflow ret;
	const Attribute left_partition_key=left_dataflow.property_.partitioner.getPartitionKey();
	const Attribute right_partition_key=right_dataflow.property_.partitioner.getPartitionKey();
	ret.attribute_list_.insert(ret.attribute_list_.end(),left_dataflow.attribute_list_.begin(),left_dataflow.attribute_list_.end());
	ret.attribute_list_.insert(ret.attribute_list_.end(),right_dataflow.attribute_list_.begin(),right_dataflow.attribute_list_.end());
	switch(join_police_) {
		case local_join: {
			ret.property_.partitioner.setPartitionList(left_dataflow.property_.partitioner.getPartitionList());
			ret.property_.partitioner.setPartitionFunction(left_dataflow.property_.partitioner.getPartitionFunction());
			ret.property_.partitioner.setPartitionKey(left_partition_key);
			ret.property_.partitioner.addShadowPartitionKey(right_partition_key);

			const unsigned long l_cardinality=left_dataflow.getAggregatedDataCardinality();
			const unsigned long r_cardinality=right_dataflow.getAggregatedDataCardinality();
			ret.property_.partitioner.getPartition(0)->setDataCardinality(l_cardinality*r_cardinality);

			ret.property_.commnication_cost=left_dataflow.property_.commnication_cost+right_dataflow.property_.commnication_cost;

			break;
		}
		case left_broadcast: {
			ret.property_.partitioner.setPartitionList(right_dataflow.property_.partitioner.getPartitionList());
			ret.property_.partitioner.setPartitionFunction(right_dataflow.property_.partitioner.getPartitionFunction());
			ret.property_.partitioner.setPartitionKey(right_partition_key);

			const unsigned long l_cardinality=left_dataflow.getAggregatedDataCardinality();

			for(unsigned i=0;i<right_dataflow.property_.partitioner.getNumberOfPartitions();i++){
				const unsigned r_cardinality=right_dataflow.property_.partitioner.getPartition(i)->getDataCardinality();
				ret.property_.partitioner.getPartition(i)->setDataCardinality(l_cardinality*r_cardinality);
			}
			ret.property_.commnication_cost=left_dataflow.property_.commnication_cost+right_dataflow.property_.commnication_cost;
			ret.property_.commnication_cost+=left_dataflow.getAggregatedDatasize()*right_dataflow.property_.partitioner.getNumberOfPartitions();

			break;
		}
		case right_broadcast: {
			ret.property_.partitioner.setPartitionList(left_dataflow.property_.partitioner.getPartitionList());
			ret.property_.partitioner.setPartitionFunction(left_dataflow.property_.partitioner.getPartitionFunction());
			ret.property_.partitioner.setPartitionKey(left_partition_key);

			const unsigned long r_cardinality=left_dataflow.getAggregatedDataCardinality();

			for(unsigned i=0;i<left_dataflow.property_.partitioner.getNumberOfPartitions();i++){
				const unsigned l_cardinality=left_dataflow.property_.partitioner.getPartition(i)->getDataCardinality();
				ret.property_.partitioner.getPartition(i)->setDataCardinality(l_cardinality*r_cardinality);
			}
			ret.property_.commnication_cost=right_dataflow.property_.commnication_cost+left_dataflow.property_.commnication_cost;
			ret.property_.commnication_cost+=right_dataflow.getAggregatedDatasize()*left_dataflow.property_.partitioner.getNumberOfPartitions();

			break;
		}
		default: {
			assert(false);
		}
	}


	dataflow_=new Dataflow();
	*dataflow_=ret;
	return ret;
}

BlockStreamIteratorBase* CrossJoin::getIteratorTree(const unsigned & block_size)
{
	if(dataflow_==0){
		getDataflow();

	}
	BlockStreamNestLoopJoinIterator *cross_join_iterator;
	BlockStreamIteratorBase* child_iterator_left=0;//=left_child_->getIteratorTree(block_size);
	BlockStreamIteratorBase* child_iterator_right=0;//=right_child_->getIteratorTree(block_size);
	generateChildPhysicalQueryPlan(child_iterator_left,child_iterator_right, block_size);
	Dataflow dataflow_left=left_child_->getDataflow();
	Dataflow dataflow_right=right_child_->getDataflow();
	BlockStreamNestLoopJoinIterator::State state;
	state.block_size_=block_size;
	state.input_schema_left=getSchema(dataflow_left.attribute_list_);
	state.input_schema_right=getSchema(dataflow_right.attribute_list_);
	state.output_schema=getSchema(dataflow_->attribute_list_);
	state.child_left=child_iterator_left;
	state.child_right=child_iterator_right;
	cross_join_iterator =new BlockStreamNestLoopJoinIterator(state);
	return cross_join_iterator;
}

void CrossJoin::generateChildPhysicalQueryPlan(
		BlockStreamIteratorBase*& left_child_iterator_tree,
		BlockStreamIteratorBase*& right_child_iterator_tree, const unsigned & blocksize) {

	Dataflow left_dataflow=left_child_->getDataflow();
	Dataflow right_dataflow=right_child_->getDataflow();

	switch (join_police_){
	case local_join:{
		left_child_iterator_tree=left_child_->getIteratorTree(blocksize);
		right_child_iterator_tree=right_child_->getIteratorTree(blocksize);
		return;
	}
	case left_broadcast: {

		BlockStreamExpander::State expander_state;
		expander_state.block_count_in_buffer_=EXPANDER_BUFFER_SIZE;
		expander_state.block_size_=blocksize;
		expander_state.init_thread_count_=Config::initial_degree_of_parallelism;
		expander_state.child_=left_child_->getIteratorTree(blocksize);
		expander_state.schema_=left_dataflow.getSchema();
		BlockStreamIteratorBase* expander=new BlockStreamExpander(expander_state);



		ExpandableBlockStreamExchangeEpoll::State exchange_state;
		exchange_state.block_size_=blocksize;
		exchange_state.child_=expander;
		exchange_state.exchange_id_=IDsGenerator::getInstance()->generateUniqueExchangeID();
		std::vector<NodeID> upper_id_list=getInvolvedNodeID(dataflow_->property_.partitioner);
		exchange_state.upper_ip_list_=convertNodeIDListToNodeIPList(upper_id_list);
		std::vector<NodeID> lower_id_list=getInvolvedNodeID(left_dataflow.property_.partitioner);
		exchange_state.lower_ip_list_=convertNodeIDListToNodeIPList(lower_id_list);
		exchange_state.partition_schema_=partition_schema::set_broadcast_partition();
		exchange_state.schema_=left_dataflow.getSchema();

		BlockStreamIteratorBase* exchange=new ExpandableBlockStreamExchangeEpoll(exchange_state);

		left_child_iterator_tree=exchange;
		right_child_iterator_tree=right_child_->getIteratorTree(blocksize);
		return;
	}
	case right_broadcast: {





		left_child_iterator_tree=left_child_->getIteratorTree(blocksize);

		BlockStreamExpander::State expander_state;
		expander_state.block_count_in_buffer_=EXPANDER_BUFFER_SIZE;
		expander_state.block_size_=blocksize;
		expander_state.init_thread_count_=Config::initial_degree_of_parallelism;
		expander_state.child_=right_child_->getIteratorTree(blocksize);
		expander_state.schema_=left_dataflow.getSchema();
		BlockStreamIteratorBase* expander=new BlockStreamExpander(expander_state);

		ExpandableBlockStreamExchangeEpoll::State exchange_state;
		exchange_state.block_size_=blocksize;
		exchange_state.child_=expander;
		exchange_state.exchange_id_=IDsGenerator::getInstance()->generateUniqueExchangeID();

		std::vector<NodeID> upper_id_list=getInvolvedNodeID(dataflow_->property_.partitioner);
		exchange_state.upper_ip_list_=convertNodeIDListToNodeIPList(upper_id_list);
		std::vector<NodeID> lower_id_list=getInvolvedNodeID(right_dataflow.property_.partitioner);
		exchange_state.lower_ip_list_=convertNodeIDListToNodeIPList(lower_id_list);

		exchange_state.partition_schema_=partition_schema::set_broadcast_partition();
		exchange_state.schema_=right_dataflow.getSchema();

		BlockStreamIteratorBase* exchange=new ExpandableBlockStreamExchangeEpoll(exchange_state);

		right_child_iterator_tree=exchange;
		return;
	}
	default:{
		assert(false);
		return;
	}
	}


}

void CrossJoin::print(int level) const
{
	printf("CrossJoin:\n",level*8," ");
	switch(join_police_){
	case left_broadcast:{
		printf(" left_broadcast\n");
		break;
	}
	case right_broadcast:{
		printf(" right_broadcast\n");
		break;
	}
	case local_join:{
		printf(" loca_join\n");
		break;
	}
	}
	left_child_->print(level+1);
	right_child_->print(level+1);
}

bool CrossJoin::canLocalJoin(Dataflow& left, Dataflow& right) const {
	if(left.property_.partitioner.getNumberOfPartitions()>1)
		return false;
	if(right.property_.partitioner.getNumberOfPartitions()>1)
		return false;
	return left.property_.partitioner.getPartitionList()[0].getLocation()==right.property_.partitioner.getPartitionList()[0].getLocation();
}
