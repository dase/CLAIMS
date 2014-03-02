/*
 * Project.cpp
 *
 *  Created on: 2014-2-21
 *      Author: casa
 */

#include "Project.h"

Project::Project(LogicalOperator *child, std::vector<std::vector<ExpressionItem> > &exprArray)
:child_(child),exprArray_(exprArray){

}

Project::~Project(){

}

Dataflow Project::getDataflow(){
	return child_->getDataflow();
}

BlockStreamIteratorBase *Project::getIteratorTree(const unsigned& blocksize){
	BlockStreamIteratorBase *child=child_->getIteratorTree(blocksize);
	BlockStreamProjectIterator::State state;
	state.block_size_=blocksize;
	state.children_=child;
	state.v_ei_=exprArray_;
	Dataflow dataflow=child_->getDataflow();
	state.input_=getSchema(dataflow.attribute_list_);
	state.map_=*getMapping();
	return new BlockStreamProjectIterator(state);
}

Mapping* Project::getMapping(){
	Mapping *mappings=new Mapping();
	for(unsigned i=0;i<exprArray_.size();i++){
		ExpressionMapping em;
		for(unsigned j=0;j<exprArray_[i].size();j++){
		}
		mappings->atomicPushExpressionMapping(em);
	}
	return mappings;
}

int Project::getColumnSeq(ExpressionItem *ei){
//	if(ei->content.var.table_name)
	int rt;
	return rt;
}
