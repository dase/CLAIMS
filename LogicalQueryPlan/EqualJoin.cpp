/*
 * EqualJoin.cpp
 *
 *  Created on: Nov 10, 2013
 *      Author: wangli
 */

#include "EqualJoin.h"

EqualJoin::EqualJoin(std::vector<JoinPair> joinpair_list,LogicalOperator* left_input,LogicalOperator* right_input)
:joinkey_pair_list_(joinpair_list),left_child_(left_input),right_child_(right_input){

}

EqualJoin::~EqualJoin() {
	// TODO Auto-generated destructor stub
}

Dataflow EqualJoin::getDataflow(){
	/** in the current implementation, only the hash join is considered**/
	Dataflow left_dataflow=left_child_->getDataflow();
	Dataflow right_dataflow=right_child_->getDataflow();
	if(isHashOnLeftKey(left_dataflow.property_.partitoiner,))
//	left_dataflow.property_.
	return left_dataflow;
}

bool EqualJoin::isHashOnLeftKey(const Partitioner& part,const Attribute& key)const{
	if(part.getPartitionFashion()!=PartitionFunction::hash_f)
		return false;
	for(unsigned i=0;i<joinkey_pair_list_.size();i++){

	}
	return *(part.getPartitionKey())==key;
}
