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
#include "../utility/maths.h"

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

// this macro decides whether really write data into data file.
// Open means no write.
#define DATA_DO_LOAD
//// this macro decides whether write DLOG message into log file.
//// Open means no DLOG message.
// #define NDEBUG

namespace claims {
namespace loader {

static double DataInjector::total_get_substr_time_ = 0;
static double DataInjector::total_check_string_time_ = 0;
static double DataInjector::total_to_value_time_ = 0;
static double DataInjector::total_to_value_func_time_ = 0;

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
}

RetCode DataInjector::PrepareInitInfo(FileOpenFlag open_flag) {
  int ret = rSuccess;
  for (int i = 0; i < table_->getNumberOfProjection(); i++) {
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

/**
 * Phase 1: write data from file into block, then write into HDFS/disk when
 * every block is filled fully by raw data
 * Phase 2: after handing all raw data file, flush all block that are not full
 * into HDFS/disk
 */
RetCode DataInjector::LoadFromFile(vector<string> input_file_names,
                                   FileOpenFlag open_flag,
                                   ExecutedResult* result, double sample_rate) {
  int ret = rSuccess;

  total_get_substr_time_ = 0;
  total_check_string_time_ = 0;
  total_to_value_time_ = 0;
  total_to_value_func_time_ = 0;
  GETCURRENTTIME(prepare_start_time);
  ret = PrepareInitInfo(open_flag);
  LOG(INFO) << "prepare time: " << GetElapsedTime(prepare_start_time) / 1000
            << endl;

  GETCURRENTTIME(open_start_time);
  ret = connector_->Open(open_flag);
  if (rSuccess != ret) {
    LOG(ERROR) << " failed to open connector" << endl;
    return ret;
  }
  LOG(INFO) << "open connector time: " << GetElapsedTime(open_start_time) / 1000
            << endl;

  GETCURRENTTIME(unbind_time);
  if (FileOpenFlag::kCreateFile == open_flag) {
    /*
     * Before overwriting data, it is need to unbind all projection, as well as
     * set row_id 0
     */
    row_id_ = 0;
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
          LOG(ERROR) << "failed to unbind entire projection " << i
                     << " in table " << table_->getTableName() << std::endl;
          return rFailure;
        }
      }
    }
    LOG(INFO) << "\n--------------------Load  Begin!------------------------\n";
  } else {
    row_id_ = table_->getRowNumber();
    LOG(INFO) << "\n------------------Append  Begin!-----------------------\n";
  }
  LOG(INFO) << "unbind time: " << GetElapsedTime(unbind_time) << endl;

  // read every raw data file
  int file_count = 0;
  uint64_t row_id_in_file = 0;
  uint64_t inserted_tuples_in_file = 0;
  uint64_t total_tuple_count = 0;
  double total_check_time = 0;
  double total_insert_time = 0;
  double total_add_time = 0;
  string tuple_record = "";
  static char* load_output_info[7] = {
      "Loading         \r", "Loading.\r",     "Loading..\r",    "Loading...\r",
      "Loading....\r",      "Loading.....\r", "Loading......\r"};
  cout << endl;

  /* store the validity of every column data, the extra 2 validity is used for
   * too many columns and too few columns
   */
  void* tuple_buffer = Malloc(table_schema_->getTupleMaxSize());
  if (tuple_buffer == NULL) return rNoMemory;

  GETCURRENTTIME(start_read_time);
  for (auto file_name : input_file_names) {
    ifstream input_file(file_name.c_str());
    if (!input_file.good()) {
      ret = rOpenDiskFileFail;
      PLOG(ERROR) << "[ " << ret << ", " << CStrError(ret) << " ]"
                  << "File name:" << file_name << ". Reason";
      result->SetError("Can't open file :" + file_name);
      return ret;
    }
    //    DLOG(INFO) << "Now handle file :" << file_name << endl;

    // read every tuple
    while (GetTupleTerminatedBy(input_file, tuple_record, row_separator_) &&
           !input_file.eof()) {
      // just to tell everyone "i am alive!!!"
      if (0 == row_id_in_file % 10000) {
        cout << load_output_info[(row_id_in_file / 10000) % 7] << std::flush;
      }

      ++row_id_in_file;
      // sample
      if (GetRandomDecimal() >= sample_rate) continue;

      GETCURRENTTIME(add_time);
      EXEC_AND_ONLY_LOG_ERROR(
          ret, AddRowIdColumn(tuple_record),
          "failed to add row_id column for tuple. ret:" << ret);
      // make sure tuple string in a uniform format(always has a column
      // separator before row separator) with format of what is get from INSERT
      tuple_record += col_separator_;
      total_add_time += GetElapsedTime(add_time);

      //      DLOG(INFO) << "after adding row id, tuple is:" << tuple_record <<
      //      endl;

      GETCURRENTTIME(start_check_time);
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
      ostringstream oss;
      for (auto it : columns_validities) {
        string validity_info =
            GenerateDataValidityInfo(it, table_, row_id_in_file, file_name);
        //        DLOG(INFO) << "append warning info:" << validity_info << endl;
        result->AppendWarning(validity_info);
      }
      total_check_time += GetElapsedTime(start_check_time);

      GETCURRENTTIME(start_insert_time);
      EXEC_AND_ONLY_LOG_ERROR(ret, InsertSingleTuple(tuple_buffer),
                              "failed to insert tuple in "
                                  << file_name << " at line " << row_id_in_file
                                  << ". ret:" << ret);
      total_insert_time += GetElapsedTime(start_insert_time);

      ++row_id_;
      tuple_record.clear();
    }
    LOG(INFO) << "insert all " << row_id_in_file << " line from " << file_name
              << " into blocks" << endl;
    input_file.close();
    ++file_count;
  }
  DELETE_PTR(tuple_buffer);
  LOG(INFO) << "used " << GetElapsedTime(start_read_time) / 1000
            << " time to handled " << file_count
            << " file, then flush unfilled block " << endl;

  LOG(INFO) << "  total add time: " << total_add_time / 1000.0
            << "  total get substring time: " << total_get_substr_time_ / 1000
            << "  total check string time: " << total_check_string_time_ / 1000
            << "  total to value time: " << total_to_value_time_ / 1000.0
            << "  total to value func time: "
            << total_to_value_func_time_ / 1000
            << "  total check time: " << total_check_time / 1000.0
            << "  total insert time: " << total_insert_time / 1000.0 << endl;

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
  LOG(INFO) << "update time: " << GetElapsedTime(update_time) << endl;

  LOG(INFO) << "\n-----------------------"
            << (kCreateFile == open_flag ? "Load" : "Append")
            << " End!-----------------------\n";

  return ret;
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
    LOG(WARNING) << "tuples string ends with " << *tuples.end()
                 << ", but not '\\n' " << endl;
  }
  EXEC_AND_ONLY_LOG_ERROR(ret, PrepareInitInfo(kAppendFile),
                          "failed to prepare initialization info");
  ret = connector_->Open(kAppendFile);
  if (rSuccess != ret) {
    ELOG(ret, "");
    return ret;
  }
  row_id_ = table_->getRowNumber();
  LOG(INFO) << "\n------------------Insert  Begin!-----------------------\n";

  string::size_type cur = 0;
  int prev_cur = 0;
  int line = 0;
  vector<void*> correct_tuple_buffer;

  while (string::npos != (cur = tuples.find('\n', prev_cur))) {
    string tuple_record = tuples.substr(prev_cur, cur - prev_cur);
    LOG(INFO) << "row " << line << ": " << tuple_record << endl;

    EXEC_AND_ONLY_LOG_ERROR(
        ret, AddRowIdColumn(tuple_record),
        "failed to add row_id column for tuple. ret:" << ret);

    vector<Validity> columns_validities;
    void* tuple_buffer = Malloc(table_schema_->getTupleMaxSize());
    if (tuple_buffer == NULL) return rNoMemory;
    ostringstream oss;

    if (rSuccess !=
        (ret = CheckAndToValue(tuple_record, tuple_buffer, RawDataSource::kSQL,
                               columns_validities))) {
      // contain data error, which is stored in the end of columns_validities

      // eliminate the side effect in row_id_
      row_id_ -= correct_tuple_buffer.size();
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
      //      DLOG(INFO) << "append warning info:" << validity_info << endl;
      result->AppendWarning(validity_info);
    }

    // if check failed, return ret
    if (rSuccess != ret) return ret;

    correct_tuple_buffer.push_back(tuple_buffer);
    ++row_id_;
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
  table_->setRowNumber(row_id_);
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
  tuple_string =
      row_id.operate->toString(&row_id_) + col_separator_ + tuple_string;

  // tuple_string = lexical_cast<string>(row_id_) + col_separator_ +
  // tuple_string;

  //// it has best performance
  //  ostringstream oss;
  //  oss << row_id_ << col_separator_ << tuple_string;
  //  tuple_string = oss.str();

  //  char res[21] = {0};
  //  snprintf(res, sizeof(res), "%lu", row_id_);
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

  //  DLOG(INFO) << "insert tuple into projection: " << i << ",
  //  partition: " << part << endl;

  ++tuples_per_partition[i][part];

  // copy tuple to buffer
  void* block_tuple_addr = pj_buffer[i][part]->allocateTuple(tuple_max_length);
  if (NULL == block_tuple_addr) {
// if buffer is full, write buffer(64K) to HDFS/disk
#ifdef DATA_DO_LOAD
    pj_buffer[i][part]->serialize(*sblock);
    EXEC_AND_LOG(
        ret, connector_->Flush(i, part, sblock->getBlock(), sblock->getsize()),
        row_id_ << "\t64KB has been written to file!",
        "failed to write to data file. ret:" << ret);
#endif
    ++blocks_per_partition[i][part];
    pj_buffer[i][part]->setEmpty();
    block_tuple_addr = pj_buffer[i][part]->allocateTuple(tuple_max_length);
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

  //  DLOG(INFO) << "text : " << tuple_string << endl;
  //  DLOG(INFO) << "tuple: ";
  //  table_->getSchema()->displayTuple(tuple_buffer, " | ");
  //  return success;
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
