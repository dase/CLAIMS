/*
 * TupleConventor.cpp
 *
 *  Created on: Jan 26, 2014
 *      Author: wangli
 */

#include "TupleConvertor.h"

TupleConvertor::TupleConvertor() {
	// TODO Auto-generated constructor stub

}

TupleConvertor::~TupleConvertor() {
	// TODO Auto-generated destructor stub
}
void TupleConvertor::sub_tuple(const Schema*& src_s,const Schema*& des_s,const void* const& tuple, void* desc, std::vector<unsigned> index){
	for(unsigned i=0;i<index.size();i++){
		const void* source_column_address=src_s->getColumnAddess(index[i],tuple);
		const void* desc_column_address=des_s->getColumnAddess(i,desc);
		des_s->getcolumn(i).operate->assign(source_column_address,desc_column_address);
	}
}

SubTuple::SubTuple(Schema* srouce, Schema* target, std::vector<unsigned> index)
:source_schema_(srouce),target_schema_(target),index_(index){

}

void SubTuple::getSubTuple(void*& tuple, void*& target){
	for(unsigned i=0;i<index_.size();i++){
//		const void* source_column_address=source_schema_->getColumnAddess(index_[i],tuple);
		const void* desc_column_address=target_schema_->getColumnAddess(i,target);
		target_schema_->getcolumn(i).operate->assign(source_column_address,desc_column_address);
	}
}
