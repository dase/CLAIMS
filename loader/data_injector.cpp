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
 * /Claims/loader/data_injector.cpp
 *
 *  Created on: Oct 22, 2015
 *      Author: yukai
 *		   Email: yukai2014@gmail.com
 *
 * Description:
 *
 */

#include "./data_injector.h"

#include <stdlib.h>
#include <iostream>
#include <istream>
#include <string>
#include <vector>

#include "./file_connector.h"
#include "./table_file_connector.h"
#include "./single_file_connector.h"
#include "../common/file_handle/file_handle_imp.h"
#include "../common/file_handle/file_handle_imp_factory.h"
#include "../common/Schema/TupleConvertor.h"
#include "../common/Schema/Schema.h"
#include "../common/Block/Block.h"
#include "../common/Block/BlockStream.h"
#include "../common/error_define.h"
#include "../common/memory_handle.h"

#include "../catalog/projection.h"
#include "../catalog/table.h"
#include "../catalog/catalog.h"
#include "../catalog/projection_binding.h"

#include "../Config.h"
#include "../Debug.h"
#include "../Daemon/Daemon.h"
#include "../Environment.h"
#include "../utility/maths.h"
#include "../utility/ThreadPool.h"

using claims::common::FileOpenFlag;
using claims::common::FilePlatform;
using claims::common::Malloc;
using std::basic_istream;
using claims::catalog::TableDescriptor;
using claims::catalog::Attribute;
using claims::catalog::ProjectionBinding;
using claims::catalog::Partitioner;
using claims::catalog::ProjectionDescriptor;
using claims::catalog::Catalog;
using boost::lexical_cast;
using namespace claims::common;

const bool kDataInjectorDebugLog = false;

#define DLOG_DI(info) \
  DLOG_IF(INFO, (kDataInjectorDebugLog) && (kClaimsDebugLog)) << info << endl;

// open multi-thread loading
#define MULTI_THREAD_LOAD

#define LOAD_PERFORMANCE_MONITOR

// this macro decides whether really write data into data file.
// Open means no write.
#define DATA_DO_LOAD
//// this macro decides whether write DLOG message into log file.
//// Open means no DLOG message.
// #define NDEBUG

namespace claims {
namespace loader {

static uint64_t DataInjector::total_get_substr_time_ = 0;
static uint64_t DataInjector::total_check_string_time_ = 0;
static uint64_t DataInjector::total_to_value_time_ = 0;
static uint64_t DataInjector::total_check_and_to_value_func_time_ = 0;
static uint64_t DataInjector::total_check_and_to_value_time_ = 0;
static uint64_t DataInjector::total_insert_time_ = 0;
static uint64_t DataInjector::total_add_time_ = 0;

DataInjector::DataInjector(TableDescriptor* table, const string col_separator,
                           const string row_separator)
    : table_(table),
      col_separator_(col_separator),
      row_separator_(row_separator) {
  sub_tuple_generator.clear();
  table_schema_ = table_->getSchema();
  for (int i = 0; i < table_->getNumberOfProjection(); i++) {
    projections_schema_.push_back(table_->getProjectoin(i)->getSchema());
    vector<string> prj_write_path;
    prj_write_path.clear();
    for (
        int j = 0;
        j < table_->getProjectoin(i)->getPartitioner()->getNumberOfPartitions();
        ++j) {
      string path = PartitionID(table_->getProjectoin(i)->getProjectionID(), j)
                        .getPathAndName();
      prj_write_path.push_back(path);
    }
    write_path_.push_back(prj_write_path);

    Attribute partition_attribute =
        table_->getProjectoin(i)->getPartitioner()->getPartitionKey();
    int hash_key_index =
        table_->getProjectoin(i)->getAttributeIndex(partition_attribute);
    partition_key_index.push_back(hash_key_index);

    PartitionFunction* pf =
        table_->getProjectoin(i)->getPartitioner()->getPartitionFunction();
    partition_functin_list_.push_back(pf);

    vector<unsigned> prj_index;
    vector<Attribute> prj_attrs = table_->getProjectoin(i)->getAttributeList();
    prj_index.clear();
    for (int j = 0; j < prj_attrs.size(); j++) {
      prj_index.push_back(prj_attrs[j].index);
    }
    SubTuple* st = new SubTuple(
        table_->getSchema(), table_->getProjectoin(i)->getSchema(), prj_index);
    sub_tuple_generator.push_back(st);
  }

  sblock = new Block(BLOCK_SIZE);

#ifdef DATA_DO_LOAD
  connector_ = new TableFileConnector(
      Config::local_disk_mode ? FilePlatform::kDisk : FilePlatform::kHdfs,
      write_path_);
#endif
}

DataInjector::~DataInjector() {
  DELETE_PTR(table_schema_);
  DELETE_PTR(connector_);
  DELETE_PTR(sblock);
  for (auto it : pj_buffer) {
    for (auto iter : it) {
      DELETE_PTR(iter);
    }
    it.clear();
  }
  pj_buffer.clear();
  for (auto it : sub_tuple_generator) DELETE_PTR(it);
  sub_tuple_generator.clear();

  write_path_.clear();
  file_list_.clear();

  for (int i = 0; i < table_->getNumberOfProjection(); ++i)
    DELETE_ARRAY(pj_buffer_access_lock_[i]);
  DELETE_ARRAY(pj_buffer_access_lock_);
}

RetCode DataInjector::PrepareInitInfo(FileOpenFlag open_flag) {
  int ret = rSuccess;
#ifdef MULTI_THREAD_LOAD
  pj_buffer_access_lock_ = new Lock* [table_->getNumberOfProjection()];
#endif
  for (int i = 0; i < table_->getNumberOfProjection(); i++) {
    pj_buffer_access_lock_[i] = new Lock
        [table_->getProjectoin(i)->getPartitioner()->getNumberOfPartitions()];

    vector<BlockStreamBase*> temp_v;
    vector<size_t> tmp_block_num;
    vector<size_t> tmp_tuple_count;
    for (
        int j = 0;
        j < table_->getProjectoin(i)->getPartitioner()->getNumberOfPartitions();
        ++j) {
      temp_v.push_back(
          BlockStreamBase::createBlock(table_->getProjectoin(i)->getSchema(),
                                       BLOCK_SIZE - sizeof(unsigned)));
      if (kAppendFile == open_flag) {
        tmp_tuple_count.push_back(
            table_->getProjectoin(i)->getPartitioner()->getPartitionCardinality(
                j));
        tmp_block_num.push_back(
            table_->getProjectoin(i)->getPartitioner()->getPartitionBlocks(j));
      } else {
        tmp_tuple_count.push_back(0);
        tmp_block_num.push_back(0);
      }
      LOG(INFO)
          << "init number of partitions " << i << "\t" << j << "\t:"
          << table_->getProjectoin(i)->getPartitioner()->getPartitionBlocks(j)
          << endl;
    }
    pj_buffer.push_back(temp_v);
    blocks_per_partition.push_back(tmp_block_num);
    tuples_per_partition.push_back(tmp_tuple_count);
  }
  return ret;
}

RetCode DataInjector::LoadFromFileSingleThread(vector<string> input_file_names,
                                               FileOpenFlag open_flag,
                                               ExecutedResult* result,
                                               double sample_rate) {
  int ret = rSuccess;
  int file_count = 0;
  uint64_t row_id_in_file = 0;
  uint64_t inserted_tuples_in_file = 0;
  uint64_t total_tuple_count = 0;
  string tuple_record = "";
  multi_thread_status_ = rSuccess;
  all_tuple_read_ = false;
  void* tuple_buffer = Malloc(table_schema_->getTupleMaxSize());
  if (tuple_buffer == NULL) return rNoMemory;
  cout << endl;

  EXEC_AND_RETURN_ERROR(
      ret, PrepareEverythingForLoading(input_file_names, open_flag, result),
      "failed to prepare everything for loading");

  GETCURRENTTIME(start_read_time);
  // read every raw data file
  for (auto file_name : input_file_names) {
    ifstream input_file(file_name.c_str());
    DLOG_DI("Now handle file :" << file_name);

    // read every tuple
    while (GetTupleTerminatedBy(input_file, tuple_record, row_separator_) ||
           tuple_record != "") {
      if (tuple_record == "\r")
        tuple_record = "";  // eliminate the effect of '\r'
      DLOG_DI("---------------read tuple "
              << tuple_record << "tuple size is " << tuple_record.length()
              << ". input file's eof is " << input_file.eof());

      // just to tell everyone "i am alive!!!"
      if (0 == row_id_in_file % 50000) AnnounceIAmLoading();
      ++row_id_in_file;

      if (GetRandomDecimal() >= sample_rate) continue;  // sample

      GETCURRENTTIME(add_time);
      EXEC_AND_ONLY_LOG_ERROR(ret, AddRowIdColumn(tuple_record),
                              "failed to add row_id column for tuple.");
      // make sure tuple string in a uniform format(always has a column
      // separator before row separator) with format of what is get from INSERT
      tuple_record += col_separator_;
      total_add_time_ += GetElapsedTimeInUs(add_time);
      DLOG_DI("after adding row id, tuple is:" << tuple_record);

      GETCURRENTTIME(start_check_time);
      /*
       * store the validity of every column data, the extra 2 validity is used
       * for too many columns and too few columns
       */
      vector<Validity> columns_validities;
      memset(tuple_buffer, 0, table_schema_->getTupleMaxSize());
      if (rSuccess !=
          (ret = CheckAndToValue(tuple_record, tuple_buffer,
                                 RawDataSource::kFile, columns_validities))) {
        /**
         * contain error, but not include data error.
         * when loading from file, all invalid data will be treated as warning
         * and set default
         */
        ELOG(ret, "Data is in file name: " << file_name
                                           << "  Line: " << row_id_in_file);
        return ret;
      }
      // only handle data warnings, because of no data error
      for (auto it : columns_validities) {
        string validity_info =
            GenerateDataValidityInfo(it, table_, row_id_in_file, file_name);
        DLOG_DI("append warning info:" << validity_info);
        result->AppendWarning(validity_info);
      }
      total_check_and_to_value_time_ += GetElapsedTimeInUs(start_check_time);

      GETCURRENTTIME(start_insert_time);
      EXEC_AND_ONLY_LOG_ERROR(ret, InsertSingleTuple(tuple_buffer),
                              "failed to insert tuple in "
                                  << file_name << " at line " << row_id_in_file
                                  << ". ret:" << ret);
      total_insert_time_ += GetElapsedTimeInUs(start_insert_time);

      ++row_id_in_table_;
      tuple_record.clear();
    }

    DLOG_DI("--------------- input file's eof is " << input_file.eof());
    LOG(INFO) << "insert all " << row_id_in_file << " line from " << file_name
              << " into blocks" << endl;
    input_file.close();
    ++file_count;
  }
  DELETE_PTR(tuple_buffer);

  LOG(INFO) << "used " << GetElapsedTimeInUs(start_read_time) / 1000000.0
            << " time to handled " << file_count
            << " file, then flush unfilled block " << endl;
  LOG(INFO) << "  total add time: " << total_add_time_ / 1000000.0
            << "  total check string time: "
            << total_check_string_time_ / 1000000.0
            << "  total get substring and check string time: "
            << total_get_substr_time_ / 1000000.0
            << "  total to value time: " << total_to_value_time_ / 1000000.0
            << "  total check_to_value func time: "
            << total_check_and_to_value_func_time_ / 1000000.0
            << "  total check_to_value time: "
            << total_check_and_to_value_time_ / 1000000.0
            << "  total insert time: " << total_insert_time_ / 1000000.0
            << endl;

  EXEC_AND_RETURN_ERROR(ret, FinishJobAfterLoading(open_flag), "");

  return ret;
}

RetCode DataInjector::SetTableState(FileOpenFlag open_flag,
                                    ExecutedResult* result) {
  int ret = rSuccess;
  if (FileOpenFlag::kCreateFile == open_flag) {
    /*
     * Before overwriting data, it is need to unbind all projection, as well as
     * set row_id 0
     */
    row_id_in_table_ = 0;
    for (int i = 0; i < table_->getNumberOfProjection(); i++) {
      if (table_->getProjectoin(0)->getPartitioner()->allPartitionBound()) {
        bool res = Catalog::getInstance()
                       ->getBindingModele()
                       ->UnbindingEntireProjection(
                           table_->getProjectoin(i)->getPartitioner());
        if (res) {
          LOG(INFO) << "unbound entire projection " << i << " in table "
                    << table_->getTableName() << std::endl;
        } else {
          result->SetError("inner error");
          LOG(ERROR) << "failed to unbind entire projection " << i
                     << " in table " << table_->getTableName() << std::endl;
          return rFailure;
        }
      }
    }
    LOG(INFO) << "\n--------------------Load  Begin!------------------------\n";
  } else {
    row_id_in_table_ = table_->getRowNumber();
    LOG(INFO) << "\n------------------Append  Begin!-----------------------\n";
  }
  return ret;
}

RetCode DataInjector::CheckFiles(vector<string> input_file_names,
                                 ExecutedResult* result) {
  int ret = rSuccess;
  for (auto file_name : input_file_names) {
    ifstream input_file(file_name.c_str());
    if (!input_file.good()) {
      ret = rOpenDiskFileFail;
      PLOG(ERROR) << "[ " << ret << ", " << CStrError(ret) << " ]"
                  << "File name:" << file_name << ". Reason";
      result->SetError("Can't access file :" + file_name);
      return ret;
    }
  }
  return ret;
}

RetCode DataInjector::PrepareEverythingForLoading(
    vector<string> input_file_names, FileOpenFlag open_flag,
    ExecutedResult* result) {
  int ret = rSuccess;
  GETCURRENTTIME(prepare_start_time);
  EXEC_AND_RETURN_ERROR(ret, PrepareInitInfo(open_flag),
                        "failed to prepare initialization info");
  LOG(INFO) << "prepare time: "
            << GetElapsedTimeInUs(prepare_start_time) / 1000000.0 << endl;

  // open files
  GETCURRENTTIME(open_start_time);
  EXEC_AND_RETURN_ERROR(ret, connector_->Open(open_flag),
                        " failed to open connector");

  LOG(INFO) << "open connector time: "
            << GetElapsedTimeInUs(open_start_time) / 1000000.0 << endl;

  // set table initialized state
  GETCURRENTTIME(unbind_time);
  EXEC_AND_RETURN_ERROR(ret, SetTableState(open_flag, result),
                        "failed to set table state");
  LOG(INFO) << "unbind time: " << GetElapsedTimeInUs(unbind_time) << endl;

  // check files
  GETCURRENTTIME(start_check_file_time);
  EXEC_AND_RETURN_ERROR(ret, CheckFiles(input_file_names, result),
                        "some files are unaccessible");
  LOG(INFO) << "used " << GetElapsedTimeInUs(start_check_file_time) / 1000000.0
            << " time to check file " << endl;
  return ret;
}

RetCode DataInjector::FinishJobAfterLoading(FileOpenFlag open_flag) {
  int ret = rSuccess;
  EXEC_AND_LOG(ret, FlushNotFullBlock(),
               "flush all last block that are not full",
               "failed to flush all last block. ret:" << ret);

#ifdef DATA_DO_LOAD
  EXEC_AND_LOG(ret, connector_->Close(), "closed connector.",
               "Failed to close connector. ret:" << ret);
#endif

  GETCURRENTTIME(update_time);
  EXEC_AND_ONLY_LOG_ERROR(ret, UpdateCatalog(open_flag),
                          "failed to update catalog information. ret:" << ret);
  LOG(INFO) << "update time: " << GetElapsedTimeInUs(update_time) / 1000000.0
            << endl;

  LOG(INFO) << "\n-----------------------"
            << (kCreateFile == open_flag ? "Load" : "Append")
            << " End!-----------------------\n";
  return ret;
}

RetCode DataInjector::LoadFromFileMultiThread(vector<string> input_file_names,
                                              FileOpenFlag open_flag,
                                              ExecutedResult* result,
                                              double sample_rate) {
  int ret = rSuccess;
  int file_count = 0;
  uint64_t row_id_in_file = 0;
  uint64_t inserted_tuples_in_file = 0;
  uint64_t total_tuple_count = 0;

  string tuple_record = "";
  multi_thread_status_ = rSuccess;
  all_tuple_read_ = false;
  result_ = result;
  cout << endl;

  EXEC_AND_RETURN_ERROR(
      ret, PrepareEverythingForLoading(input_file_names, open_flag, result),
      "failed to prepare everything for loading");

  // create threads handling tuples
  int thread_count = sysconf(_SC_NPROCESSORS_CONF) + 2;
  for (int i = 0; i < thread_count - 1; ++i)
    Environment::getInstance()->getThreadPool()->add_task(HandleTuple, this);

  // start to read every raw data file
  GETCURRENTTIME(start_read_time);
  for (auto file_name : input_file_names) {
    ifstream input_file(file_name.c_str());
    DLOG_DI("Now handle file :" << file_name);
    // read every tuple
    while (GetTupleTerminatedBy(input_file, tuple_record, row_separator_) ||
           tuple_record != "") {
      if (tuple_record == "\r")
        tuple_record = "";  // eliminate the effect of '\r'
      DLOG_DI("---------------read tuple "
              << tuple_record << "tuple size is " << tuple_record.length()
              << ". input file's eof is " << input_file.eof());

      // just to tell everyone "i am alive!!!"
      if (0 == row_id_in_file % 50000) AnnounceIAmLoading();
      ++row_id_in_file;

      if (GetRandomDecimal() >= sample_rate) continue;  // sample

      {  // push into tuple pool
        LockGuard<Lock> guard(tuple_buffer_access_lock_);
        tuple_buffer_.push_back(
            std::move(LoadTask(tuple_record, file_name, row_id_in_file)));
        tuple_count_sem_in_buffer_.post();
      }
    }

    DLOG_DI("--------------- input file's eof is " << input_file.eof());
    LOG(INFO) << "insert all " << row_id_in_file
              << " line into tuple pool from " << file_name << " into blocks"
              << endl;
    input_file.close();
    ++file_count;
  }
  all_tuple_read_ = true;
  LOG(INFO) << "used " << GetElapsedTimeInUs(start_read_time) / 1000000.0
            << " time to read all " << file_count
            << " file and insert them into pool " << endl;

  // after read all tuple, go on handling tuple as well as child threads
  HandleTuple(this);
  if (rSuccess != (ret = multi_thread_status_)) {
    ELOG(multi_thread_status_, "failed to load using multi-thread ");
    return ret;
  }

  // waiting for all threads finishing task
  for (int i = 0; i < thread_count; ++i) finished_thread_sem_.wait();
  LOG(INFO) << " all threads finished its job " << endl;

  LOG(INFO) << "  total add time: " << total_add_time_ / 1000000.0
            << "  total check string time: "
            << total_check_string_time_ / 1000000.0
            << "  total get substring and check string time: "
            << total_get_substr_time_ / 1000000.0
            << "  total to value time: " << total_to_value_time_
            << "  total to value func time: "
            << total_check_and_to_value_func_time_
            << "  total check time: " << total_check_and_to_value_time_
            << "  total insert time: " << total_insert_time_ << endl;

  EXEC_AND_RETURN_ERROR(ret, FinishJobAfterLoading(open_flag), "");

  return ret;
}

/**
 * Phase 1: write data from file into block, then write into HDFS/disk when
 * every block is filled fully by raw data
 * Phase 2: after handing all raw data file, flush all block that are not full
 * into HDFS/disk
 */
RetCode DataInjector::LoadFromFile(vector<string> input_file_names,
                                   FileOpenFlag open_flag,
                                   ExecutedResult* result, double sample_rate) {
  total_get_substr_time_ = 0;
  total_check_string_time_ = 0;
  total_to_value_time_ = 0;
  total_check_and_to_value_func_time_ = 0;
  total_check_and_to_value_time_ = 0;
  total_insert_time_ = 0;
  total_add_time_ = 0;
#ifndef MULTI_THREAD_LOAD
  return LoadFromFileSingleThread(input_file_names, open_flag, result,
                                  sample_rate);
#else
  return LoadFromFileMultiThread(input_file_names, open_flag, result,
                                 sample_rate);
#endif
}

void* DataInjector::HandleTuple(void* ptr) {
  DataInjector* injector = static_cast<DataInjector*>(ptr);
  string tuple_to_handle = "";
  string file_name = "";
  uint64_t row_id_in_file = 0;
  DataInjector::LoadTask task;
  RetCode ret = rSuccess;
  /*
   * store the validity of every column data,
   * the extra 2 validity[(-1, rTooManyColumn), (-1, rTooFewColumn)]
   * is used for too many columns and too few columns
   */
  vector<Validity> columns_validities;
  void* tuple_buffer = Malloc(injector->table_schema_->getTupleMaxSize());
  if (NULL == tuple_buffer) {
    ret = rNoMemory;
    injector->multi_thread_status_ = ret;
    return NULL;
  }

  while (true) {
    if (injector->multi_thread_status_ != rSuccess) return NULL;
    if (injector->all_tuple_read_ == true) {
      if (injector->tuple_count_sem_in_buffer_.try_wait() == false) {
        DLOG_DI("all tuple in pool is handled ");
        injector->finished_thread_sem_.post();
        return NULL;  // success. all tuple is handled
      }
      DLOG_DI("all tuple is read ,tuple count sem is:"
              << injector->tuple_count_sem_in_buffer_.get_value());
    } else {
      DLOG_DI("tuple count sem is:"
              << injector->tuple_count_sem_in_buffer_.get_value());
      // waiting for new tuple read from file
      injector->tuple_count_sem_in_buffer_.wait();
    }
    {
      LockGuard<Lock> guard(injector->tuple_buffer_access_lock_);
      task = injector->tuple_buffer_.front();
      injector->tuple_buffer_.pop_front();
    }

    tuple_to_handle = task.tuple_;
    file_name = task.file_name_;
    row_id_in_file = task.row_id_in_file_;
    if (0 == row_id_in_file % 10000) injector->AnnounceIAmLoading();

    GETCURRENTTIME(add_time);
    EXEC_AND_ONLY_LOG_ERROR(ret, injector->AddRowIdColumn(tuple_to_handle),
                            "failed to add row_id column for tuple.");
    // make sure tuple string in a uniform format(always has a column
    // separator before row separator) with format of what is get from INSERT
    tuple_to_handle += injector->col_separator_;
    __sync_add_and_fetch(&injector->total_add_time_,
                         GetElapsedTimeInUs(add_time));

    DLOG_DI("after adding row id, tuple is:" << tuple_to_handle);

    GETCURRENTTIME(start_check_time);
    columns_validities.clear();
    memset(tuple_buffer, 0, injector->table_schema_->getTupleMaxSize());
    if (rSuccess != (ret = injector->CheckAndToValue(
                         tuple_to_handle, tuple_buffer, RawDataSource::kFile,
                         columns_validities))) {
      /**
       * contain error, but not data error. When loading from file,
       * all invalid data will be treated as warning and set default
       */
      ELOG(ret, "Data is in file name: " << file_name
                                         << "  Line: " << row_id_in_file);
      break;
    }
    // only handle data warnings, because of no data error
    for (auto it : columns_validities) {
      string validity_info = injector->GenerateDataValidityInfo(
          it, injector->table_, row_id_in_file, file_name);
      DLOG_DI("append warning info:" << validity_info);
      injector->result_->AtomicAppendWarning(validity_info);
    }
    __sync_add_and_fetch(&injector->total_check_and_to_value_time_,
                         GetElapsedTimeInUs(start_check_time));

    GETCURRENTTIME(start_insert_time);
    EXEC_AND_ONLY_LOG_ERROR(ret, injector->InsertSingleTuple(tuple_buffer),
                            "failed to insert tuple in "
                                << file_name << " at line " << row_id_in_file
                                << ".");
    __sync_add_and_fetch(&injector->total_insert_time_,
                         GetElapsedTimeInUs(start_insert_time));
  }
  if (ret != rSuccess) {  // it is not need to use lock
    injector->multi_thread_status_ = ret;
  }
  DELETE_PTR(tuple_buffer);
}

/**
 * check validity of all tuples,
 * if all OK, then insert into file and update catalog;
 * else return error to client without inserting any data
 */
RetCode DataInjector::InsertFromString(const string tuples,
                                       ExecutedResult* result) {
  int ret = rSuccess;
  LOG(INFO) << "tuples is: " << tuples << endl;

  if (0 == tuples.length()) {
    LOG(WARNING) << "tuples string length is 0" << endl;
    return ret;
  }
  if ('\n' != tuples[tuples.length() - 1]) {
    LOG(WARNING) << "tuples string ends with " << tuples[tuples.length() - 1]
                 << ", but not '\\n' " << endl;
  }
  EXEC_AND_RETURN_ERROR(ret, PrepareInitInfo(kAppendFile),
                        "failed to prepare initialization info");

  EXEC_AND_RETURN_ERROR(ret, connector_->Open(kAppendFile),
                        " failed to open connector");

  row_id_in_table_ = table_->getRowNumber();
  LOG(INFO) << "\n------------------Insert  Begin!-----------------------\n";

  string::size_type cur = 0;
  int prev_cur = 0;
  int line = 0;
  vector<void*> correct_tuple_buffer;

  while (string::npos != (cur = tuples.find('\n', prev_cur))) {
    DLOG_IF(INFO, kClaimsDebugLog == true) << "cur: " << cur
                                           << " prev_cur: " << prev_cur << endl;
    string tuple_record = tuples.substr(prev_cur, cur - prev_cur);
    LOG(INFO) << "row " << line << ": " << tuple_record << endl;

    EXEC_AND_ONLY_LOG_ERROR(ret, AddRowIdColumn(tuple_record),
                            "failed to add row_id column for tuple.");

    vector<Validity> columns_validities;
    void* tuple_buffer = Malloc(table_schema_->getTupleMaxSize());
    if (tuple_buffer == NULL) return rNoMemory;
    ostringstream oss;

    if (rSuccess !=
        (ret = CheckAndToValue(tuple_record, tuple_buffer, RawDataSource::kSQL,
                               columns_validities))) {
      // contain data error, which is stored in the end of columns_validities

      // eliminate the side effect in row_id_in_table_
      row_id_in_table_ -= correct_tuple_buffer.size();
      for (auto it : correct_tuple_buffer) DELETE_PTR(it);
      correct_tuple_buffer.clear();

      // handle error which stored in the end
      Validity err = columns_validities.back();
      columns_validities.pop_back();
      string validity_info = GenerateDataValidityInfo(err, table_, line, "");
      LOG(ERROR) << validity_info;
      result->SetError(validity_info);
    }

    // handle all warnings
    for (auto it : columns_validities) {
      string validity_info = GenerateDataValidityInfo(it, table_, line, "");
      DLOG_IF(INFO, kClaimsDebugLog == true)
          << "append warning info:" << validity_info << endl;
      result->AppendWarning(validity_info);
    }

    // if check failed, return ret
    if (rSuccess != ret) return ret;

    correct_tuple_buffer.push_back(tuple_buffer);
    ++row_id_in_table_;
    ++line;
    prev_cur = cur + 1;
  }

  for (auto it : correct_tuple_buffer) {
    EXEC_AND_ONLY_LOG_ERROR(ret, InsertSingleTuple(it),
                            "failed to insert tuple in line "
                                << line << ". ret:" << ret);
    DELETE_PTR(it);
  }
  correct_tuple_buffer.clear();
  LOG(INFO) << "totally inserted " << line << " rows data into blocks" << endl;
  EXEC_AND_LOG(ret, FlushNotFullBlock(),
               "flush all last block that are not full",
               "failed to flush all last block");
#ifdef DATA_DO_LOAD
  EXEC_AND_LOG(ret, connector_->Close(), "closed connector.",
               "Failed to close connector.");
#endif
  EXEC_AND_ONLY_LOG_ERROR(ret, UpdateCatalog(kAppendFile),
                          "failed to update catalog information");

  LOG(INFO) << "\n---------------------Insert End!---------------------\n";
  return ret;
}

// flush the last block which is not full of 64*1024Byte
RetCode DataInjector::FlushNotFullBlock() {
  int ret = rSuccess;
  for (int i = 0; i < table_->getNumberOfProjection(); i++) {
    for (
        int j = 0;
        j < table_->getProjectoin(i)->getPartitioner()->getNumberOfPartitions();
        j++) {
      if (!pj_buffer[i][j]->Empty()) {
        pj_buffer[i][j]->serialize(*sblock);

#ifdef DATA_DO_LOAD
        EXEC_AND_LOG(
            ret, connector_->Flush(i, j, sblock->getBlock(), sblock->getsize()),
            "flushed the last block from buffer(" << i << "," << j
                                                  << ") into file",
            "failed to flush the last block from buffer(" << i << "," << j
                                                          << "). ret:" << ret);
#endif
        ++blocks_per_partition[i][j];
        pj_buffer[i][j]->setEmpty();
      }
    }
  }
  return ret;
}

RetCode DataInjector::UpdateCatalog(FileOpenFlag open_flag) {
  int ret = rSuccess;
  // register the number of rows in table to catalog
  table_->setRowNumber(row_id_in_table_);
  // register the partition information to catalog
  for (int i = 0; i < table_->getNumberOfProjection(); i++) {
    for (
        int j = 0;
        j < table_->getProjectoin(i)->getPartitioner()->getNumberOfPartitions();
        j++) {
      table_->getProjectoin(i)
          ->getPartitioner()
          ->RegisterPartitionWithNumberOfBlocks(j, blocks_per_partition[i][j]);
      if (kAppendFile == open_flag) {
        table_->getProjectoin(i)
            ->getPartitioner()
            ->UpdatePartitionWithNumberOfChunksToBlockManager(
                j, blocks_per_partition[i][j]);
      }
      LOG(INFO) << "Number of blocks in " << i << "th projection, " << j
                << "th partition\t: " << blocks_per_partition[i][j] << endl;
    }
  }
  return ret;
}

inline RetCode DataInjector::AddRowIdColumn(const string& tuple_string) {
  static column_type row_id(t_u_long);

  /*
    {
      // faster than implement with lock, but with bug that some row will be
      // insert twice
      uint64_t row_id_value = __sync_fetch_and_add(&row_id_in_table_, 1);
      tuple_string =
          row_id.operate->toString(&row_id_value) + col_separator_ +
    tuple_string;
    }
    */

  LockGuard<Lock> guard(row_id_lock_);
  ++row_id_in_table_;
  tuple_string = row_id.operate->toString(&row_id_in_table_) + col_separator_ +
                 tuple_string;

  // tuple_string = lexical_cast<string>(row_id_in_table_) + col_separator_ +
  // tuple_string;

  //// it has best performance
  //  ostringstream oss;
  //  oss << row_id_in_table_ << col_separator_ << tuple_string;
  //  tuple_string = oss.str();

  //  char res[21] = {0};
  //  snprintf(res, sizeof(res), "%lu", row_id_in_table_);
  //  tuple_string = string(res) + col_separator_ + tuple_string;

  return rSuccess;
}

// TODO(ANYONE): consider multi-thread
RetCode DataInjector::InsertTupleIntoProjection(int proj_index,
                                                void* tuple_buffer) {
  int ret = rSuccess;
  if (proj_index >= table_->getNumberOfProjection()) {
    LOG(ERROR) << "projection index is " << proj_index
               << ", larger than projection number" << endl;
    return rParamInvalid;
  }

  int i = proj_index;
  unsigned tuple_max_length = projections_schema_[i]->getTupleMaxSize();
  // extract the sub tuple according to the projection schema
  void* target = Malloc(tuple_max_length);  // newmalloc
  if (target == NULL) return rNoMemory;
  sub_tuple_generator[i]->getSubTuple(tuple_buffer, target);

  // determine the partition to write the tuple "target"
  const int partition_key_local_index = partition_key_index[i];
  void* partition_key_addr = projections_schema_[i]->getColumnAddess(
      partition_key_local_index, target);
  int part = projections_schema_[i]
                 ->getcolumn(partition_key_local_index)
                 .operate->getPartitionValue(
                     partition_key_addr,
                     partition_functin_list_[i]->getNumberOfPartitions());

  DLOG_DI("insert tuple into projection: " << i << ", partition: " << part);

  __sync_add_and_fetch(&tuples_per_partition[i][part], 1);

  // copy tuple to buffer
  void* block_tuple_addr;
  {
#ifdef MULTI_THREAD_LOAD
    LockGuard<Lock> guard(pj_buffer_access_lock_[i][part]);
#endif
    block_tuple_addr = pj_buffer[i][part]->allocateTuple(tuple_max_length);
    if (NULL == block_tuple_addr) {
// if buffer is full, write buffer(64K) to HDFS/disk
#ifdef DATA_DO_LOAD
      pj_buffer[i][part]->serialize(*sblock);
      EXEC_AND_LOG(ret, connector_->Flush(i, part, sblock->getBlock(),
                                          sblock->getsize()),
                   row_id_in_table_ << "\t64KB has been written to file!",
                   "failed to write to data file. ret:" << ret);
#endif
      ++blocks_per_partition[i][part];
      pj_buffer[i][part]->setEmpty();
      block_tuple_addr = pj_buffer[i][part]->allocateTuple(tuple_max_length);
    }
  }
  int copy_size = projections_schema_[i]->copyTuple(target, block_tuple_addr);
  if (copy_size > tuple_max_length && "copy more than malloc size") {
    LOG(ERROR) << "copy " << copy_size << " length memory, more than allocated "
               << tuple_max_length << std::endl;
  }
  DELETE_PTR(target);
  return ret;
}

/**
 * for every projection, get sub tuple according to projection schema,
 * write into the partition block whose id equal to the tuple's
 * partition key
 * if the block is full, write to real data file in HDFS/disk.
 */
RetCode DataInjector::InsertSingleTuple(void* tuple_buffer) {
  int ret = rSuccess;
  for (int i = 0; i < table_->getNumberOfProjection(); i++) {
    ret = InsertTupleIntoProjection(i, tuple_buffer);
  }
  return ret;
}

inline RetCode DataInjector::CheckAndToValue(
    string tuple_string, void* tuple_buffer, RawDataSource raw_data_source,
    vector<Validity>& columns_validities) {
  //  RetCode success =
  return table_schema_->CheckAndToValue(tuple_string, tuple_buffer,
                                        col_separator_, raw_data_source,
                                        columns_validities);
}

istream& DataInjector::GetTupleTerminatedBy(ifstream& ifs, string& res,
                                            const string& terminator) {
  res.clear();
  if (1 == terminator.length()) {
    return getline(ifs, res, static_cast<char>(terminator[0]));
  }
  int c = 0;
  while (EOF != (c = ifs.get())) {
    res += c;
    if (c == terminator[0]) {
      int coincide_length = 1;
      while (EOF != (c = ifs.get())) {
        res += c;
        if (terminator[coincide_length] == c) {
          if (++coincide_length == terminator.length()) {
            // don't read terminator into string, same as getline()
            res = res.substr(0, res.length() - terminator.length());
            return ifs;
          }
        } else {
          break;
        }
      }
    }
  }
  return ifs;
}

void DataInjector::AnnounceIAmLoading() {
  static char* load_output_info[7] = {
      "Loading         \r", "Loading.\r",     "Loading..\r",    "Loading...\r",
      "Loading....\r",      "Loading.....\r", "Loading......\r"};
  static int count = 0;
  cout << load_output_info[count = (++count % 7)] << std::flush;
}

/*
const char* validity_info[9][2] = {
    {"Data larger than range value for column '%s' at line: %d\n",
     "Data larger than range value for column '%s' at line: %d in file:%s\n"},
    {},
    {},
    {},
    {},
    {},
    {},
    {},
    {}};
*/
string DataInjector::GenerateDataValidityInfo(const Validity& vali,
                                              TableDescriptor* table, int line,
                                              const string& file) {
  ostringstream oss;
  oss.clear();
  switch (vali.check_res_) {
    case rTooLargeData: {
      oss << "Data larger than range value for column '"
          << table_->getAttribute(vali.column_index_).attrName
          << "' at line: " << line;
      if ("" != file) oss << " in file: " << file;
      oss << "\n";
      break;
    }
    case rTooSmallData: {
      oss << "Data smaller than range value for column '"
          << table_->getAttribute(vali.column_index_).attrName
          << "' at line: " << line;
      if ("" != file) oss << " in file: " << file;
      oss << "\n";
      break;
    }
    case rInterruptedData: {
      oss << "Data truncated from non-digit for column '"
          << table_->getAttribute(vali.column_index_).attrName
          << "' at line: " << line;
      if ("" != file) oss << " in file: " << file;
      oss << "\n";
      break;
    }
    case rTooLongData: {
      oss << "Data truncated for column '"
          << table_->getAttribute(vali.column_index_).attrName
          << "' at line: " << line;
      if ("" != file) oss << " in file: " << file;
      oss << "\n";
      break;
    }
    case rIncorrectData: {
      oss << "Incorrect format value for column '"
          << table_->getAttribute(vali.column_index_).attrName
          << "' at line: " << line;
      if ("" != file) oss << " in file: " << file;
      oss << "\n";
      break;
    }
    case rInvalidNullData: {
      oss << "Null Data value is invalid for column '"
          << table_->getAttribute(vali.column_index_).attrName
          << "' at line: " << line;
      if ("" != file) oss << " in file: " << file;
      oss << "\n";
      break;
    }
    case rTooFewColumn: {
      oss << "Line: " << line;
      if ("" != file) oss << " in file: " << file;
      oss << " doesn't contain data for all columns\n";
      break;
    }
    case rTooManyColumn: {
      oss << "Line: " << line;
      if ("" != file) oss << " in file: " << file;
      oss << " was truncated; it contained more data than there were "
             "input columns\n";
      break;
    }
    default:
      LOG(ERROR) << "Unknown ERROR" << endl;
      oss << "Unknown ERROR\n";
      break;
  }
  return oss.str();
}

} /* namespace loader */
} /* namespace claims */
