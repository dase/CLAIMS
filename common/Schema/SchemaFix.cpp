/*
 * SchemaFix.cpp
 *
 *  Created on: Jun 8, 2013
 *      Author: wangli
 */

// this macro decides whether write DLOG message into log file.
// Open means no DLOG message.
#define NDEBUG

#include "SchemaFix.h"
#include <memory.h>
#include <glog/logging.h>
#include <string>
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
 */
/*
 * TODO(yukai, lizhifang): 检查源数据是否合法,如果来自kSQL,
 * 若出现error则直接返回,
 * 若只有warning,放入warning_columns_index,
 * 同时处理warning(字符串过长,截断;数字类型不在合法范围内设为默认值);
 * 如果来自kFile, 出现error将值设为默认值, 视为warning对待.
 *
 * 完成以上功能,可以在Operate类加入以下函数:
 * int Check(const string& raw_string), 返回0:success, 1: warning, 2: error
 * void SetDefault(string& raw_string) 设置源数据为默认值
 */
bool SchemaFix::toValue(std::string text_tuple, void* binary_tuple,
                        const char attr_separator,
                        RawDataSource raw_data_source,
                        vector<unsigned>& warning_columns_index) {
  string::size_type prev_pos = 0;
  string::size_type pos = 0;
  warning_columns_index.clear();

  for (int i = 0; i < columns.size(); ++i) {
    pos = text_tuple.find(attr_separator, prev_pos);

    int actual_column_data_length = pos - prev_pos;
    string text_column = text_tuple.substr(prev_pos, pos - prev_pos);

    // TODO(yukai, lizhifang): should be implemented by Operate.Check()
    /*
     * check the real data size of column whose type is string and choose the
     * minimum, and set the last char of string to '\0'
     */
    if (0 == strcmp(typeid(*(columns[i].operate)).name(),
                    typeid(OperateString).name())) {
      int column_max_length = columns[i].size - 1;

      if (actual_column_data_length > column_max_length) {
        LOG(WARNING) << "Data truncated for column " << i << std::endl;
        text_column = text_tuple.substr(prev_pos, column_max_length) + "\0 ";
        warning_columns_index.push_back(i);
      }
    }

    columns[i].operate->toValue(
        static_cast<char*>(binary_tuple) + accum_offsets[i],
        text_column.c_str());

    DLOG(INFO) << "Original: "
               << text_tuple.substr(prev_pos, pos - prev_pos).c_str()
               << "\t Transfer: "
               << columns[i].operate->toString(binary_tuple + accum_offsets[i])
               << endl;
    prev_pos = pos + 1;
  }
  return true;
}
void SchemaFix::addColumn(column_type ct, unsigned size) {
  accum_offsets.push_back(totalsize);
  columns.push_back(ct);
  totalsize += size;
}
