/*
 * BlockStreamPrint.cpp
 *
 *  Created on: Nov 19, 2013
 *      Author: wangli
 */

#include "../physical_query_plan/BlockStreamPrint.h"

BlockStreamPrint::BlockStreamPrint() : block_buffer_(NULL) {
  // TODO Auto-generated constructor stub
}
BlockStreamPrint::BlockStreamPrint(State state)
    : state_(state), block_buffer_(NULL) {}

BlockStreamPrint::~BlockStreamPrint() {}
bool BlockStreamPrint::Open(const PartitionOffset& offset) {
  block_buffer_ =
      BlockStreamBase::createBlock(state_.schema_, state_.block_size_);
  tuple_count_ = 0;
  return state_.child_->Open(offset);
}
bool BlockStreamPrint::Next(BlockStreamBase*) {
  printf("Query result:\n");
  printf(
      "========================================================================"
      "================\n");
  for (unsigned i = 0; i < state_.attribute_name_list_.size(); i++) {
    std::string format = "%s" + state_.spliter_;
    printf(format.c_str(), state_.attribute_name_list_[i].c_str());
  }
  printf("\n");

  //	getchar();
  //	getchar();

  unsigned block_count(0);
  while (state_.child_->Next(block_buffer_)) {
    unsigned tuple_in_block(0);
    BlockStreamBase::BlockStreamTraverseIterator* it =
        block_buffer_->createIterator();
    void* tuple;
    while (NULL != (tuple = it->nextTuple())) {
      state_.schema_->displayTuple(tuple, state_.spliter_.c_str());
      tuple_in_block++;
      tuple_count_++;
    }
    //		printf("Tuples in
    // Block[%d]=%d\n",block_count++,block_buffer_->getTuplesInBlock());
    block_buffer_->setEmpty();
    //		sleep(1);
  }
  return false;
}
bool BlockStreamPrint::Close() {
  printf("tuple count:%d\n", tuple_count_);
  block_buffer_->~BlockStreamBase();
  cout << "----------total tuples: " << tuple_count_ << "----------\n";
  // LOG(INFO)<< "----------total tuples: " << tuple_count_ << "----------\n";
  return state_.child_->Close();
}
void BlockStreamPrint::Print() {
  printf("Print:\n");
  state_.child_->Print();
}

BlockStreamPrint::State::~State() {
  delete schema_;
  if (child_ > 0) delete child_;
}
