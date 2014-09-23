/*
 * CrossJoin.cpp
 *
 *  Created on: Jul 14, 2014
 *      Author: imdb
 */

#include "CrossJoin.h"
#include "../BlockStreamIterator/ParallelBlockStreamIterator/BlockStreamNestLoopJoinIterator.h"

CrossJoin::CrossJoin():left_child_(0),right_child_(0),dataflow_(0)
{
	setOperatortype(l_cross_join);
}

CrossJoin::CrossJoin(LogicalOperator* left_input,LogicalOperator* right_input)
:left_child_(left_input),right_child_(right_input),dataflow_(0){
	setOperatortype(l_cross_join);
}
CrossJoin::~CrossJoin() {
	dataflow_->~Dataflow();
	if(left_child_>0){
		left_child_->~LogicalOperator();
	}
	if(right_child_>0){
		right_child_->~LogicalOperator();
	}
}

Dataflow CrossJoin::getDataflow()
{
	if(dataflow_!=0)
	{
		/* the data flow has been computed*/
		return *dataflow_;
	}
	Dataflow left_dataflow=left_child_->getDataflow();
	Dataflow right_dataflow=right_child_->getDataflow();
	Dataflow ret;
	const Attribute left_partition_key=left_dataflow.property_.partitioner.getPartitionKey();
	const Attribute right_partition_key=right_dataflow.property_.partitioner.getPartitionKey();
	ret.attribute_list_.insert(ret.attribute_list_.end(),left_dataflow.attribute_list_.begin(),left_dataflow.attribute_list_.end());
	ret.attribute_list_.insert(ret.attribute_list_.end(),right_dataflow.attribute_list_.begin(),right_dataflow.attribute_list_.end());
	ret.property_.partitioner.setPartitionList(left_dataflow.property_.partitioner.getPartitionList());
	ret.property_.partitioner.setPartitionFunction(left_dataflow.property_.partitioner.getPartitionFunction());
	ret.property_.partitioner.setPartitionKey(left_partition_key);
	ret.property_.partitioner.addShadowPartitionKey(right_partition_key);

	for(unsigned i=0;i<ret.property_.partitioner.getNumberOfPartitions();i++){
		const unsigned l_cardinality=left_dataflow.property_.partitioner.getPartition(i)->getDataCardinality();
		const unsigned r_cardinality=right_dataflow.property_.partitioner.getPartition(i)->getDataCardinality();
		ret.property_.partitioner.getPartition(i)->setDataCardinality(l_cardinality+r_cardinality);
	}

	ret.property_.commnication_cost=left_dataflow.property_.commnication_cost+right_dataflow.property_.commnication_cost;

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
	BlockStreamIteratorBase* child_iterator_left=left_child_->getIteratorTree(block_size);
	BlockStreamIteratorBase* child_iterator_right=right_child_->getIteratorTree(block_size);
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

void CrossJoin::print(int level) const
{
	printf("CrossJoin:\n",level*8," ");
	left_child_->print(level+1);
	right_child_->print(level+1);
}
