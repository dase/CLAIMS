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

#include <iostream>
#include <string>
#include <vector>

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

#include "../Config.h"
#include "../Debug.h"
#include "./file_connector.h"
#include "./table_file_connector.h"
#include "./single_file_connector.h"
#include "../catalog/catalog.h"
#include "../catalog/projection_binding.h"
#include "../Daemon/Daemon.h"
#include "../utility/maths.h"

#define HDFS_LOAD

namespace claims {
namespace loader {

using claims::catalog::TableDescriptor;
using claims::catalog::Attribute;
using claims::catalog::ProjectionBinding;
using claims::catalog::Partitioner;
using claims::catalog::ProjectionDescriptor;
using claims::catalog::Catalog;
using namespace claims::common;

DataInjector::DataInjector(TableDescriptor* table, const char col_separator,
                           const char row_separator)
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

#ifdef HDFS_LOAD
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
  int ret = kSuccess;
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
  int ret = kSuccess;
  ret = PrepareInitInfo(open_flag);
  ret = connector_->Open(open_flag);
  if (kSuccess != ret) {
    LOG(ERROR) << " failed to open connector" << endl;
    return ret;
  }
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
          return kFailure;
        }
      }
    }
    LOG(INFO) << "\n--------------------Load  Begin!------------------------\n";
  } else {
    row_id_ = table_->getRowNumber();
    LOG(INFO) << "\n------------------Append  Begin!-----------------------\n";
  }

  // read every raw data file
  int file_count = 0;
  uint64_t row_id_in_file = 0;
  uint64_t inserted_tuples_in_file = 0;
  uint64_t total_tuple_count = 0;
  for (auto file_name : input_file_names) {
    ifstream input_file(file_name.c_str());
    if (!input_file.good()) {
      PLOG(ERROR) << "Cannot open source file:" << file_name << ", reason: ";
      result->SetError("Cannot open source file:" + file_name);
      return EOpenDiskFileFail;
    }
    string tuple_record;
    // read every line
    while (getline(input_file, tuple_record, row_separator_) &&
           !input_file.eof()) {
      /////////////////////////////////////////////
      ++row_id_in_file;
      // sample
      if (GetRandomDecimal() >= sample_rate) continue;

      EXEC_AND_ONLY_LOG_ERROR(AddRowIdColumn(tuple_record),
                              "failed to add row_id column for tuple");

      void* tuple_buffer = Malloc(table_schema_->getTupleMaxSize());
      if (tuple_buffer == NULL) return ENoMemory;

      vector<unsigned> warning_indexs;
      if (!CheckTupleValidity(tuple_record, tuple_buffer, warning_indexs)) {
        ostringstream oss;
        oss << "The data in " << file_name << ":" << row_id_in_file
            << " line is invalid " << std::endl;
        LOG(ERROR) << oss.str();
        result->SetError(oss.str());
      }

      for (auto it : warning_indexs) {
        ostringstream oss;
        oss << "Data truncated from " << table_->getAttribute(it).attrName
            << " at line: " << row_id_in_file << " in file: " << file_name
            << "\n";
        result->AppendWarning(oss.str());
        LOG(WARNING) << oss.str();
      }

      ret = InsertSingleTuple(tuple_buffer);
      if (kSuccess != ret)
        LOG(ERROR) << "failed to insert tuple in " << file_name << ": line "
                   << row_id_in_file << "" << std::endl;
      tuple_record.clear();
      DELETE_PTR(tuple_buffer);
      ///////////////////////////////////
    }
    input_file.close();
    ++file_count;
  }
  LOG(INFO) << "handled " << file_count << " file, then flush unfilled block "
            << endl;

  EXEC_AND_LOG(FlushNotFullBlock(), "flush all last block that are not full",
               "failed to flush all last block");

//  if (kSuccess == (ret = FlushNotFullBlock()))
//    LOG(INFO) << "flush all last block that are not full" << endl;
//  else
//    LOG(ERROR) << "failed to flush all last block" << endl;

#ifdef HDFS_LOAD
  //  if (kSuccess != (ret = connector_->Close()))
  //    LOG(ERROR) << "Failed to close connector." << endl;
  //  else
  //    LOG(INFO) << "closed connector." << endl;

  EXEC_AND_LOG(connector_->Close(), "Failed to close connector.",
               "closed connector.");
#endif

  if (kSuccess != (ret = UpdateCatalog(open_flag))) {
    LOG(ERROR) << "failed to update catalog information" << endl;
  }

  LOG(INFO) << "\n-----------------------"
            << (kCreateFile == open_flag ? "Load" : "Append")
            << " End!-----------------------\n";

  return ret;
}

RetCode DataInjector::InsertFromString(const string tuples,
                                       ExecutedResult* result) {
  int ret = kSuccess;
  LOG(INFO) << "tuples is: " << tuples << endl;

  if (0 == tuples.length()) {
    LOG(WARNING) << "tuples string length is 0" << endl;
    return ret;
  }
  if ('\n' != tuples[tuples.length() - 1]) {
    LOG(WARNING) << "tuples string ends with " << *tuples.end()
                 << ", but not '\\n' " << endl;
  }
  EXEC_AND_ONLY_LOG_ERROR(PrepareInitInfo(kAppendFile),
                          "failed to prepare initialization info");
  ret = connector_->Open(kAppendFile);
  if (kSuccess != ret) {
    LOG(ERROR) << " failed to open connector" << endl;
    return ret;
  }
  row_id_ = table_->getRowNumber();
  LOG(INFO) << "\n------------------Insert  Begin!-----------------------\n";

  string::size_type cur = 0;
  int prev_cur = 0;
  int line = 0;
  while (string::npos != (cur = tuples.find('\n', prev_cur))) {
    ++line;
    string tuple = tuples.substr(prev_cur, cur);
    LOG(INFO) << "row " << line << ": " << tuple << endl;

    EXEC_AND_ONLY_LOG_ERROR(AddRowIdColumn(tuple),
                            "failed to add row_id column for tuple");
    void* tuple_buffer = Malloc(table_schema_->getTupleMaxSize());
    if (tuple_buffer == NULL) return ENoMemory;

    vector<unsigned> warning_indexs;
    if (!CheckTupleValidity(tuple, tuple_buffer, warning_indexs)) {
      ostringstream oss;
      oss << "The data in "
          << ":" << line << " line is invalid " << std::endl;
      LOG(ERROR) << oss.str();
      result->SetError(oss.str());
      return EInvalidInsertData;
    }

    for (auto it : warning_indexs) {
      ostringstream oss;
      oss << "Data truncated from " << table_->getAttribute(it).attrName
          << " at line: " << line << "\n";
      result->AppendWarning(oss.str());
      LOG(WARNING) << oss.str();
    }

    ret = InsertSingleTuple(tuple_buffer);
    if (kSuccess != ret) {
      LOG(ERROR) << "failed to insert tuple in line " << line << ""
                 << std::endl;
      return ret;
    }
    prev_cur = cur + 1;
  }
  LOG(INFO) << "totally inserted " << line << " rows data into blocks";

  ret = FlushNotFullBlock();

#ifdef HDFS_LOAD
  EXEC_AND_LOG(connector_->Close(), "closed connector.",
               "Failed to close connector.");
#endif

  ret = UpdateCatalog(kAppendFile);
  if (kSuccess != ret) {
    LOG(ERROR) << "failed to update catalog information" << endl;
  }

  LOG(INFO) << "\n---------------------Insert End!---------------------\n";
  return ret;
}

// flush the last block which is not full of 64*1024Byte
RetCode DataInjector::FlushNotFullBlock() {
  int ret = kSuccess;
  for (int i = 0; i < table_->getNumberOfProjection(); i++) {
    for (
        int j = 0;
        j < table_->getProjectoin(i)->getPartitioner()->getNumberOfPartitions();
        j++) {
      if (!pj_buffer[i][j]->Empty()) {
        pj_buffer[i][j]->serialize(*sblock);

#ifdef HDFS_LOAD
        EXEC_AND_LOG(
            connector_->Flush(i, j, sblock->getBlock(), sblock->getsize()),
            "flushed the last block from buffer(" << i << "," << j
                                                  << ") into file",
            "failed to flush the last block from buffer(" << i << "," << j
                                                          << ")");
#endif
        blocks_per_partition[i][j]++;
        pj_buffer[i][j]->setEmpty();
      }
    }
  }
  return ret;
}

RetCode DataInjector::UpdateCatalog(FileOpenFlag open_flag) {
  int ret = kSuccess;
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
      LOG(INFO) << "Number of blocks " << i << "\t" << j
                << "\t: " << blocks_per_partition[i][j] << endl;
    }
  }
  return ret;
}

RetCode DataInjector::AddRowIdColumn(const string& tuple_string) {
  column_type* tmp = new column_type(t_u_long);
  std::string tmp_str = tmp->operate->toString(&row_id_);
  delete tmp;
  tuple_string = tmp_str + col_separator_ + tuple_string;
  return kSuccess;
}

// TODO(yukai): can be executed by multithreading
RetCode DataInjector::InsertSubTupleIntoProjection(int proj_index,
                                                   void* tuple_buffer) {
  int ret = kSuccess;
  if (proj_index >= table_->getNumberOfProjection()) {
    LOG(ERROR) << "projection index is " << proj_index
               << ", larger than projection number" << endl;
    return EParamInvalid;
  }

  int i = proj_index;
  unsigned tuple_max_length = projections_schema_[i]->getTupleMaxSize();
  // extract the sub tuple according to the projection schema
  void* target = Malloc(tuple_max_length);  // newmalloc
  if (target == NULL) return ENoMemory;
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

  LOG(INFO) << "insert tuple into partition: " << part << endl;
  tuples_per_partition[i][part]++;

  // copy tuple to buffer
  void* block_tuple_addr = pj_buffer[i][part]->allocateTuple(tuple_max_length);
  if (NULL == block_tuple_addr) {
// if buffer is full, write buffer(64K) to HDFS/disk
#ifdef HDFS_LOAD
    pj_buffer[i][part]->serialize(*sblock);
    if (kSuccess != (ret = connector_->Flush(i, part, sblock->getBlock(),
                                             sblock->getsize()))) {
      LOG(ERROR) << "failed to write to data file. ErrCode: " << ret << endl;
    } else {
      LOG(INFO) << row_id_ << "\t64KB has been written to HDFS!\n";
    }
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

  free(target);
  return ret;
}

/**
 * for every projection, get sub tuple according to projection schema,
 * write into the partition block whose id equal to the tuple's partition key
 * if the block is full, write to real data file in HDFS/disk.
 */
RetCode DataInjector::InsertSingleTuple(void* tuple_buffer) {
  int ret = kSuccess;
  for (int i = 0; i < table_->getNumberOfProjection(); i++) {
    LOG(INFO) << "begin to insert tuple into projection: " << i << endl;
    ret = InsertSubTupleIntoProjection(i, tuple_buffer);
  }
  return ret;
}

bool DataInjector::CheckTupleValidity(string tuple_string, void* tuple_buffer,
                                      vector<unsigned>& warning_indexs) {
  if (tuple_string.length() == 0) {
    LOG(ERROR) << "The tuple record is null!\n";
    return false;
  }
  warning_indexs =
      table_schema_->toValue(tuple_string, tuple_buffer, col_separator_);
  row_id_++;

  LOG(INFO) << "text : " << tuple_string << endl;
  LOG(INFO) << "tuple: ";
  table_->getSchema()->displayTuple(tuple_buffer, " | ");
  return true;
}
} /* namespace loader */
} /* namespace claims */
