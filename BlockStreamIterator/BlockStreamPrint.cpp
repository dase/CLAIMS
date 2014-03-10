/*
 * BlockStreamPrint.cpp
 *
 *  Created on: Nov 19, 2013
 *      Author: wangli
 */

#include "BlockStreamPrint.h"

BlockStreamPrint::BlockStreamPrint()
:block_buffer_(0){
	// TODO Auto-generated constructor stub

}
BlockStreamPrint::BlockStreamPrint(State state)
:state_(state),block_buffer_(0){

}

BlockStreamPrint::~BlockStreamPrint() {
	// TODO Auto-generated destructor stub
}
bool BlockStreamPrint::open(const PartitionOffset& offset){
	block_buffer_=BlockStreamBase::createBlock(state_.schema_,state_.block_size_);
	tuple_count_=0;
	return state_.child_->open(offset);

}
bool BlockStreamPrint::next(BlockStreamBase*){

	printf("Query result:\n");
	printf("================================\n");
	for(unsigned i=0;i<state_.attribute_name_list_.size();i++){
		std::string format="%s"+state_.spliter_;
		printf(format.c_str(),state_.attribute_name_list_[i].c_str());
	}
	printf("\n");


	unsigned block_count(0);

	while(state_.child_->next(block_buffer_)){
		unsigned tuple_in_block(0);
		BlockStreamBase::BlockStreamTraverseIterator* it=block_buffer_->createIterator();
		void* tuple;
		while((tuple=it->nextTuple())!=0){
//			cout<<"int: "<<*(int*)tuple<<endl;
			assert(*(int*)(tuple+8)==20101008);
//			cout<<"float: "<<*(float*)((char*)tuple+4)<<endl;
//			cout<<"int: "<<*(int*)((char*)tuple+8)<<endl;
//			state_.schema_->displayTuple(tuple,state_.spliter_.c_str());
//			sleep(1);
			tuple_in_block++;
			tuple_count_++;
		}
//		printf("Tuples in Block[%d]=%d\n",block_count++,block_buffer_->getTuplesInBlock());
		block_buffer_->setEmpty();
//		sleep(1);
	}
	return false;
}
bool BlockStreamPrint::close(){
	printf("tuple count:%d\n",tuple_count_);
	block_buffer_->~BlockStreamBase();
	return state_.child_->close();
}
void BlockStreamPrint::print(){
	printf("Print:\n");
	state_.child_->print();
}
