/*
 * SchemaFix.cpp
 *
 *  Created on: Jun 8, 2013
 *      Author: wangli
 */

#include "SchemaFix.h"

#include <glog/logging.h>
#include <string>
#include <memory.h>
#include <algorithm>
#include <vector>
SchemaFix::SchemaFix(const std::vector<column_type>& col) : Schema(col) {
  //	accum_offsets=new unsigned[columns.size()];	//new
  totalsize = 0;
  unsigned accumu = 0;
  for (unsigned i = 0; i < col.size(); i++) {
    totalsize += col[i].get_length();
    accum_offsets.push_back(accumu);
    accumu += col[i].get_length();
  }
}
SchemaFix::SchemaFix(const SchemaFix& r) : Schema(r) {
  this->accum_offsets = r.accum_offsets;
  this->totalsize = r.totalsize;
}
SchemaFix::~SchemaFix() {
  //	accum_offsets.~vector();
}

unsigned SchemaFix::getTupleMaxSize() const { return totalsize; }

void SchemaFix::getColumnValue(unsigned index, void* src, void* desc) {
  assert(index < columns.size());
  assert(src != 0 && desc != 0);

  columns[index].operate->assignment(accum_offsets[index] + (char*)src, desc);
}

unsigned SchemaFix::getColumnOffset(unsigned index) {
  return accum_offsets[index];
}
Schema* SchemaFix::getSubSchema(std::vector<unsigned> index) const {
  std::vector<column_type> col;
  for (unsigned i = 0; i < index.size(); i++) {
    col.push_back(columns[index[i]]);
  }
  return new SchemaFix(col);
}
Schema* SchemaFix::duplicateSchema() const { return new SchemaFix(columns); }

int SchemaFix::getColumnOffset(unsigned index) const {
  return accum_offsets[index];
}

/**
 * Attention: if columns with type is string that length is less than real data
 * length , data will be truncated.
 * TODO(yukai): after datatype.h modifies, whether have warning is depended on
 * Operate.toValue()
 */
vector<unsigned> SchemaFix::toValue(std::string text_tuple, void* binary_tuple,
                                    const char attr_separator) {
  string::size_type prev_pos = 0;
  string::size_type pos = 0;

  // store the index of columns that max length is less than real data length
  vector<unsigned> warning_columns_index;
  warning_columns_index.clear();

  for (int i = 0; i < columns.size(); i++) {
    for (; (attr_separator != text_tuple[pos]) && (pos < text_tuple.length());
         pos++) {
    }
    if (prev_pos <= pos) {
      int actual_column_data_length = pos - prev_pos;
      int copy_length = actual_column_data_length;
      string text_column = text_tuple.substr(prev_pos, pos - prev_pos);

      /**
       * check the real data size of column whose type is string and choose the
       * minimum, and set the last char of string to '\0'
       */
      if (0 == strcmp(typeid(*(columns[i].operate)).name(),
                      typeid(OperateString).name())) {
        int column_max_length = columns[i].size - 1;

        if (actual_column_data_length > column_max_length) {
          LOG(WARNING) << "Data truncated for column " << i << std::endl;
          copy_length = column_max_length;
          text_column = text_tuple.substr(prev_pos, copy_length) + "\0 ";
          warning_columns_index.push_back(i);
        }
      }
      columns[i].operate->toValue((char*)binary_tuple + accum_offsets[i],
                                  text_column.c_str());

      //      cout << "Original: "
      //           << text_tuple.substr(prev_pos, pos - prev_pos).c_str()
      //           << "\t Transfer: "
      //           << columns[i].operate->toString(binary_tuple +
      //           accum_offsets[i])
      //           << endl;
      pos++;
      prev_pos = pos;
    }
  }
  return warning_columns_index;
}
void SchemaFix::addColumn(column_type ct, unsigned size) {
  accum_offsets.push_back(totalsize);
  columns.push_back(ct);
  totalsize += size;
}
