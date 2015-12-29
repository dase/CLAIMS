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
#include <list>
#include <vector>
#include <string>

#include "../common/error_define.h"
#include "../common/file_handle/file_handle_imp.h"
#include "../common/hash.h"
#include "../catalog/table.h"
#include "./validity.h"

using claims::common::FileOpenFlag;
using claims::catalog::TableDescriptor;
using std::vector;
using std::string;
class SubTuple;
class Block;
class BlockStreamBase;
class Schema;
class PartitionFunction;
class ExecutedResult;

namespace claims {
namespace loader {

class FileConnector;
class DataInjector {
 public:
  struct LoadTask {
    std::string tuple_;
    std::string file_name_;
    uint64_t row_id_in_file_ = 0;
    LoadTask(std::string tuple, std::string file_name, uint64_t row_id)
        : tuple_(tuple), file_name_(file_name), row_id_in_file_(row_id) {}
    LoadTask() = default;
  };

 public:
  //  DataInjector() {}
  /**
   * @brief Method description: get necessary info from table and init
   * connector_
   * @param table: table to load
   * @param col_separator: column separator
   * @param row_separator: row separator
   */
  DataInjector(TableDescriptor* table, const string col_separator = "|",
               const string row_separator = "\n");

  virtual ~DataInjector();

  /**
   * @brief Method description: load/append data from multiple files into table
   * @param file_names: the names of files to be read
   * @param open_flag: specify the way to write to data file:
   *        kCreateFile(meaning create or overwrite i.e., implies O_TRUNC) /
   *        kAppendFile(implies O_APPEND) /
   *        kReadFile( implies O_RDONLY)
   * @param sample_rate: the sample rate about write data into table
   * @return rSuccess if success or other on error
   */
  RetCode LoadFromFile(vector<string> input_file_names, FileOpenFlag open_flag,
                       ExecutedResult* result, double sample_rate = 1.0);

  RetCode LoadFromFileSingleThread(vector<string> input_file_names,
                                   FileOpenFlag open_flag,
                                   ExecutedResult* result, double sample_rate);

  RetCode LoadFromFileMultiThread(vector<string> input_file_names,
                                  FileOpenFlag open_flag,
                                  ExecutedResult* result, double sample_rate);

  /**
   * @brief Method description: insert tuples into table
   * @param tuples: the data to insert into tables, which may be a line or
   * multiple lines
   * @return rSuccess if success or other on error
   */
  RetCode InsertFromString(const string tuples, ExecutedResult* result);

 private:
  /**
   * @brief Method description: handle memory which store single line
   * @param tuple_buffer: single tuple memory
   * @return  rSuccess if succeed
   */
  RetCode InsertSingleTuple(void* tuple_buffer);

  /**
   * @brief Method description: check the validity of the tuple string, and
   *        store it in tuple_buffer
   * @param tuple_string: raw tuple data to be insert
   * @param tuple_buffer: the address of memory where tuple data is stored
   * @param warning_indexs: store the index of columns which have warning
   * @return true if tuple is not valid even though there are some warnings
   */
  inline RetCode CheckAndToValue(string tuple_string, void* tuple_buffer,
                                 RawDataSource raw_data_source,
                                 vector<Validity>& columns_validities);

  /**
   * @brief Method description: add row_id column value
   */
  inline RetCode AddRowIdColumn(const string& tuple_string);

  /**
   * @brief Method description: insert single tuple insert the proj_index
   * projection
   * @param proj_index: the id of projection
   * @param tuple_buffer: the memory of tuple to be write
   */
  RetCode InsertTupleIntoProjection(int proj_index, void* tuple_buffer);

  RetCode UpdateCatalog(FileOpenFlag open_flag);

  /**
   * @brief Method description: after handle all tuple, flush all block that are
   *        not full into file
   */
  RetCode FlushNotFullBlock();

  RetCode PrepareInitInfo(FileOpenFlag open_flag);

  RetCode HandleSingleLine(string tuple_record, void* tuple_buffer,
                           string data_source, uint64_t row_id_in_raw_data,
                           ExecutedResult* result);

  string GenerateDataValidityInfo(const Validity& vali, TableDescriptor* table,
                                  int line, const string& file);
  void AnnounceIAmLoading();

  static void* HandleTuple(void* ptr);

  RetCode SetTableState(FileOpenFlag open_flag, ExecutedResult* result);
  RetCode CheckFiles(vector<string> input_file_names, ExecutedResult* result);
  RetCode PrepareEverythingForLoading(vector<string> input_file_names,
                                      FileOpenFlag open_flag,
                                      ExecutedResult* result);

  RetCode FinishJobAfterLoading(FileOpenFlag open_flag);

 public:
  static istream& GetTupleTerminatedBy(ifstream& ifs, string& res,
                                       const string& terminator);

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

  string col_separator_;
  string row_separator_;
  uint64_t row_id_in_table_;

  // support multi-thread
  std::list<LoadTask> tuple_buffer_;
  Lock tuple_buffer_access_lock_;
  semaphore tuple_count_sem_in_buffer_;

  Lock row_id_lock_;
  Lock** pj_buffer_access_lock_;
  semaphore finished_thread_sem_;

  bool all_tuple_read_ = false;
  RetCode multi_thread_status_ = rSuccess;
  ExecutedResult* result_;
  /******************debug********************/
 public:
  static uint64_t total_get_substr_time_;
  static uint64_t total_check_string_time_;
  static uint64_t total_to_value_time_;
  static uint64_t total_check_and_to_value_func_time_;
  static uint64_t total_check_and_to_value_time_;
  static uint64_t total_insert_time_;
  static uint64_t total_add_time_;
};

} /* namespace loader */
} /* namespace claims */

#endif  // LOADER_DATA_INJECTOR_H_
