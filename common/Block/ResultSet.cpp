/*
 * ResultSet.cpp
 *
 *  Created on: Mar 30, 2014
 *      Author: wangli
 */

#include "ResultSet.h"

int utf8_strlen(const std::string& str) {
  const char* s = str.c_str();
  int i, j;
  int len = 0;
  for (i = 0; s[i] != 0; i++) {
    if (s[i] & 0x80) {
      j = 1;
      while ((s[++i] & 0x80) && !(s[i] & 0x40)) j++;
      len += 2;
      i--;
    } else {
      len++;
    }
  }
  return len;
}
ResultSet::~ResultSet() { delete schema_; }

ResultSet::ResultSet(const ResultSet& r) : DynamicBlockBuffer(r) {
  this->column_header_list_ = r.column_header_list_;
  this->schema_ = r.schema_->duplicateSchema();
  this->query_time_ = r.query_time_;
}

void ResultSet::print() const {
  std::vector<int> column_wides;
  unsigned space_per_column = 0;  // two spaces and one "|"

  for (int i = 0; i < schema_->getncolumns(); i++) {
    column_wides.push_back(column_header_list_[i].size());
  }

  //	unsigned
  //sample_budget=20>this->getNumberOftuples()?20:this->getNumberOftuples();
  unsigned sample_budget = 20;

  Iterator it = this->createIterator();
  BlockStreamBase* block;
  BlockStreamBase::BlockStreamTraverseIterator* tuple_it;
  unsigned sample_times = 0;
  while (block = it.nextBlock()) {
    bool finish = false;
    tuple_it = block->createIterator();
    void* tuple;
    while (tuple = tuple_it->nextTuple()) {
      for (unsigned i = 0; i < column_header_list_.size(); i++) {
        int wide = utf8_strlen(schema_->getcolumn(i).operate->toString(
                       schema_->getColumnAddess(i, tuple))) +
                   space_per_column;
        column_wides[i] = max(column_wides[i], wide);
      }
      sample_times++;
      if (sample_times >= sample_budget) {
        finish = true;
      }
    }
    delete tuple_it;
    if (finish) break;
  }

  /* print header*/

  printf("\n");

  /* print horizontal line */
  for (unsigned i = 0; i < column_header_list_.size(); i++) {
    printf("+");
    printNChar(column_wides[i] + 2, '-');
  }
  printf("+\n");

  for (unsigned i = 0; i < column_header_list_.size(); i++) {
    printf("| ", column_header_list_[i].c_str());

    printNChar((column_wides[i] - column_header_list_[i].size()) / 2, ' ');
    printf("%s", column_header_list_[i].c_str());
    printNChar(column_wides[i] - column_header_list_[i].size() -
                   (column_wides[i] - column_header_list_[i].size()) / 2,
               ' ');
    printf(" ");
  }
  printf("|\n");

  /* print horizontal line */
  for (unsigned i = 0; i < column_header_list_.size(); i++) {
    printf("+");
    printNChar(column_wides[i] + 2, '-');
  }
  printf("+\n");

  it = this->createIterator();
  while (block = it.nextBlock()) {
    tuple_it = block->createIterator();
    void* tuple;
    while (tuple = tuple_it->nextTuple()) {
      for (unsigned i = 0; i < column_header_list_.size(); i++) {
        int current_value_length =
            utf8_strlen(schema_->getcolumn(i).operate->toString(
                schema_->getColumnAddess(i, tuple)));
        column_wides[i] = max(column_wides[i], current_value_length);
        printf("| ");
        printNChar((column_wides[i] - current_value_length) / 2, ' ');
        printf("%s", schema_->getcolumn(i)
                         .operate->toString(schema_->getColumnAddess(i, tuple))
                         .c_str());
        printNChar(column_wides[i] - current_value_length -
                       (column_wides[i] - current_value_length) / 2,
                   ' ');
        printf(" ");
      }
      printf("|\n");
    }
    delete tuple_it;
  }

  /* print horizontal line */
  for (unsigned i = 0; i < column_header_list_.size(); i++) {
    printf("+");
    printNChar(column_wides[i] + 2, '-');
  }
  printf("+\n");

  printf("%d tuples (%6.4f seconds)\n\n", this->getNumberOftuples(),
         query_time_);
}

void ResultSet::printNChar(int n, char c) const {
  for (int i = 0; i < n; i++) {
    printf("%c", c);
  }
}
