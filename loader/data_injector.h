/*
 * Copyright [2012-2015] DaSE@ECNU
 *
 * Licensed to the Apache Software Foundation (ASF) under one or more
 * contributor license agreements.  See the NOTICE file distributed with
 * this work for additional information regarding copyright ownership.
 * The ASF licenses this file to You under the Apache License, Version 2.0
 * (the "License"); you may not use this file except in compliance with
 * the License.  You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 *
 * /Claims/loader/data_injector.h
 *
 *  Created on: Oct 22, 2015
 *      Author: yukai
 *		   Email: yukai2014@gmail.com
 *
 * Description: class for injecting data from files or string
 *
 */

#ifndef LOADER_DATA_INJECTOR_H_
#define LOADER_DATA_INJECTOR_H_
#include <vector>
#include <string>

#include "../common/error_define.h"
#include "../common/file_handle/file_handle_imp.h"
#include "../common/hash.h"
#include "../catalog/table.h"

class SubTuple;
class Block;
class BlockStreamBase;
class Schema;
class PartitionFunction;
class ExecutedResult;

namespace claims {
namespace loader {

using claims::common::FileOpenFlag;
using claims::catalog::TableDescriptor;
using std::vector;
using std::string;

class FileConnector;
class DataInjector {
 public:
  //  DataInjector() {}
  DataInjector(TableDescriptor* table, const char col_separator = '|',
               const char row_separator = '\n');

  virtual ~DataInjector();

  /**
   * @brief Method description: load/append data from multiple files into table
   * @param file_names: the names of files to be read
   * @param open_flag: specify the way to write to data file:
   *        kCreateFile(meaning create or overwrite i.e., implies O_TRUNC) /
   *        kAppendFile(implies O_APPEND) /
   *        kReadFile( implies O_RDONLY)
   * @param sample_rate: the sample rate about write data into table
   * @return kSuccess if success or other on error
   */
  RetCode LoadFromFile(vector<string> input_file_names, FileOpenFlag open_flag,
                       ExecutedResult* result, double sample_rate = 1.0);

  /**
   * @brief Method description: insert tuples into table
   * @param tuples: the data to insert into tables, which may be a line or
   * multiple lines
   * @return kSuccess if success or other on error
   */
  RetCode InsertFromString(const string tuples, ExecutedResult* result);

 private:
  RetCode InsertSingleTuple(void* tuple_buffer);

  /**
   * @brief Method description: check the validity of the tuple string, and
   *        store it in tuple_buffer
   * @param tuple_string: raw tuple data to be insert
   * @param tuple_buffer: the address of memory where tuple data is stored
   * @param warning_indexs: store the index of columns which have warning
   * @return true if tuple is not valid even though there are some warnings
   */
  bool CheckTupleValidity(string tuple_string, void* tuple_buffer,
                          vector<unsigned>& warning_indexs);

  RetCode AddRowIdColumn(const string& tuple_string);

  RetCode InsertSubTupleIntoProjection(int proj_index, void* tuple_buffer);

  RetCode UpdateCatalog(FileOpenFlag open_flag);

  RetCode FlushNotFullBlock();

  RetCode PrepareInitInfo(FileOpenFlag open_flag);

 private:
  TableDescriptor* table_;
  FileConnector* connector_;

  Schema* table_schema_;
  vector<Schema*> projections_schema_;
  std::vector<std::string> file_list_;
  vector<vector<string>> write_path_;

  vector<PartitionFunction*> partition_functin_list_;
  vector<int> partition_key_index;
  vector<SubTuple*> sub_tuple_generator;
  Block* sblock;

  vector<vector<size_t>> blocks_per_partition;
  vector<vector<size_t>> tuples_per_partition;
  vector<vector<BlockStreamBase*>> pj_buffer;

  char col_separator_;
  char row_separator_;
  uint64_t row_id_;
};

} /* namespace loader */
} /* namespace claims */

#endif  // LOADER_DATA_INJECTOR_H_
