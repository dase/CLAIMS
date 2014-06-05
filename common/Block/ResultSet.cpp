/*
 * ResultSet.cpp
 *
 *  Created on: Mar 30, 2014
 *      Author: wangli
 */

#include "ResultSet.h"

ResultSet::~ResultSet() {
	delete schema_;
}

ResultSet::ResultSet(const ResultSet& r)
:DynamicBlockBuffer(r){
	this->column_header_list_=r.column_header_list_;
	this->schema_=r.schema_->duplicateSchema();
	this->query_time_=r.query_time_;
}

void ResultSet::print() const {
	printf("\n");
	for(unsigned i=0;i<column_header_list_.size();i++){
		printf("%s\t",column_header_list_[i].c_str());
	}

	printf("-------------------------------------------\n");
	Iterator it=this->createIterator();
	BlockStreamBase* block;
	while(block=it.nextBlock()){
		BlockStreamBase::BlockStreamTraverseIterator* block_it=block->createIterator();
		void* tuple;
		while(tuple=block_it->nextTuple()){
//			schema_->displayTuple(tuple,"\t");
		}
	}
	printf(" Total tuples:%d  (%4.4f seconds)\n",this->getNumberOftuples(),query_time_);
}
