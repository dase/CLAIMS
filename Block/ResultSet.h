/*
 * ResultSet.h
 *
 *  Created on: Jan 20, 2014
 *      Author: wangli
 */

#ifndef RESULTSET_H_
#define RESULTSET_H_

#include "DynamicBlockBuffer.h"

//typedef DynamicBlockBuffer ResultSet;
class ResultSet :public DynamicBlockBuffer{
public:
	ResultSet():DynamicBlockBuffer(),schema_(0){};
	ResultSet(const ResultSet& r):DynamicBlockBuffer(r),schema_(0){};
	virtual ~ResultSet(){
		schema_->~Schema();
	}
	void print()const{
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
				schema_->displayTuple(tuple,"\t");
			}
		}
		printf(" Total tuples:%d  (%4.4f seconds)\n",this->getNumberOftuples(),query_time_);
	}
	Schema* schema_;
	std::vector<std::string> column_header_list_;
	double query_time_;
private:
};

#endif /* RESULTSET_H_ */
