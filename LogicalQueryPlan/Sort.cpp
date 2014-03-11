/*
 * Sort.cpp
 *
 *  Created on: 2014-3-1
 *      Author: casa
 */

#include "Sort.h"

LogicalSort::LogicalSort(LogicalOperator *child, OrderByAttr *oba)
:child_(child),oba_(oba){

}

LogicalSort::~LogicalSort() {

}

Dataflow LogicalSort::getDataflow(){
	dataflow_=child_->getDataflow();
	return dataflow_;
}

BlockStreamIteratorBase *LogicalSort::getIteratorTree(const unsigned& blocksize){
	getDataflow();
	BlockStreamIteratorBase *child=child_->getIteratorTree(blocksize);
	BlockStreamSortIterator::State state;
	state.block_size_=blocksize;
	state.child_=child;
	state.orderbyKey_=getOrderByKey(oba_->tbl_,oba_->attr_);
	Dataflow dataflow=child_->getDataflow();
	state.input_=getSchema(dataflow_.attribute_list_);
	return new BlockStreamSortIterator(state);
}

int LogicalSort::getOrderByKey(const char *tbl,const char *attr){
	for(unsigned i=0;i<dataflow_.attribute_list_.size();i++){
		TableDescriptor *table=Catalog::getInstance()->getTable(dataflow_.attribute_list_[i].table_id_);
		string tablename=table->getTableName();
		if((tablename.compare(tbl)==0)&&(dataflow_.attribute_list_[i].attrName.compare(attr)==0)){
			return i;
		}
	}
}
