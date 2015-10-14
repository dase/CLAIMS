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
 * /Claims/physical_query_plan/physical_projectionscan.cpp
 *
 *  Created on: Nov.14, 2013
 *      Author: wangli, Hanzhang
 *		   Email: wangli1426@gmail.com
 *
 * Description: Implementation of Scan operator in physical layer.
 *
 */

#include "../physical_query_plan/physical_ProjectionScan.h"

#include <memory.h>
#include <malloc.h>
#include <sys/mman.h>
#include <errno.h>
#include <limits.h>
#include "../../common/rename.h"
#include "../../storage/BlockManager.h"
#include "../../Executor/ExpanderTracker.h"
#include "../../Config.h"
#include "../../utility/warmup.h"
#include "../../storage/ChunkStorage.h"

// namespace claims{
// namespace physical_query_plan{

ExpandableBlockStreamProjectionScan::ExpandableBlockStreamProjectionScan(
    State state)
    : state_(state), partition_reader_iterator_(NULL) {
  InitExpandedStatus();
}

ExpandableBlockStreamProjectionScan::ExpandableBlockStreamProjectionScan()
    : partition_reader_iterator_(NULL) {
  InitExpandedStatus();
}

ExpandableBlockStreamProjectionScan::~ExpandableBlockStreamProjectionScan() {
  delete state_.schema_;
}

ExpandableBlockStreamProjectionScan::State::State(ProjectionID projection_id,
                                                  Schema* schema,
                                                  unsigned block_size,
                                                  float sample_rate)
    : schema_(schema),
      projection_id_(projection_id),
      block_size_(block_size),
      sample_rate_(sample_rate) {}

/**
 * @brief Method description: Initialize the operator. The logic is read
 * partition and get chunks, read a chunk and get blocks. different policy
 * decide if it generates a buffer.
 */
bool ExpandableBlockStreamProjectionScan::Open(
    const PartitionOffset& partition_offset) {
  if (TryEntryIntoSerializedSection()) {
    /* this is the first expanded thread*/
    PartitionStorage* partition_handle_;
    return_blocks_ = 0;
    if ((partition_handle_ = BlockManager::getInstance()->getPartitionHandle(
             PartitionID(state_.projection_id_, partition_offset))) == 0) {
      printf("The partition[%s] does not exists!\n",
             PartitionID(state_.projection_id_, partition_offset)
                 .getName()
                 .c_str());
      SetReturnStatus(false);
    } else {
      partition_reader_iterator_ =
          partition_handle_->createAtomicReaderIterator();
      SetReturnStatus(true);
    }

#ifdef AVOID_CONTENTION_IN_SCAN
    unsigned long long start = curtick();

    ChunkReaderIterator* chunk_reader_it;
    ChunkReaderIterator::block_accessor* ba;
    while (chunk_reader_it = partition_reader_iterator_->nextChunk()) {
      while (chunk_reader_it->getNextBlockAccessor(ba)) {
        ba->getBlockSize();
        input_dataset_.input_data_blocks.push_back(ba);
      }
    }
#endif
    ExpanderTracker::getInstance()->addNewStageEndpoint(
        pthread_self(), LocalStageEndPoint(stage_src, "Scan", 0));
    perf_info =
        ExpanderTracker::getInstance()->getPerformanceInfo(pthread_self());
    perf_info->initialize();
  }
  BarrierArrive();
  return GetReturnStatus();
}

/**
 * @brief Method description:There are two method of strategy to scan data
 * 1) make a buffer(input_data).wait for quantitative block and return it.
 * 2) get a block and return it immediately.
 * according to AVOID_CONTENTION_IN_SCAN.
 */
bool ExpandableBlockStreamProjectionScan::Next(BlockStreamBase* block) {
  unsigned long long total_start = curtick();
#ifdef AVOID_CONTENTION_IN_SCAN
  scan_thread_context* stc = (scan_thread_context*)GetContext();
  if (stc == 0) {
    stc = new scan_thread_context();
    InitContext(stc);
  }
  if (ExpanderTracker::getInstance()->isExpandedThreadCallBack(
          pthread_self())) {
    input_dataset_.atomicPut(stc->assigned_data_);
    delete stc;
    destorySelfContext();
    perf_info->report_instance_performance_in_millibytes();
    return false;
  }

  if (!stc->assigned_data_.empty()) {
    ChunkReaderIterator::block_accessor* ba = stc->assigned_data_.front();
    stc->assigned_data_.pop_front();

    ba->getBlock(block);

    // whether delete InMemeryBlockAccessor::target_block_start_address
    // is depend on whether use copy in ba->getBlock(block);
    delete ba;
    perf_info->processed_one_block();
    return true;
  } else {
    if (input_dataset_.atomicGet(stc->assigned_data_, Config::scan_batch)) {
      // case(1)
      return Next(block);
    } else {
      delete stc;
      destorySelfContext();
      return false;
    }
  }

#else

  if (ExpanderTracker::getInstance()->isExpandedThreadCallBack(
          pthread_self())) {
    return false;
  }
  perf_info->processed_one_block();
  // case(2)
  return partition_reader_iterator_->nextBlock(block);

#endif
}

bool ExpandableBlockStreamProjectionScan::Close() {
  if (NULL != partition_reader_iterator_) {
    delete partition_reader_iterator_;
    partition_reader_iterator_ = NULL;
  }
  DestoryAllContext();

  /* reset the expanded status in that open and next will be re-invoked.*/
  InitExpandedStatus();

  return true;
}

void ExpandableBlockStreamProjectionScan::Print() {
  printf("Scan (ID=%d)\n", state_.projection_id_.table_id);
}

bool ExpandableBlockStreamProjectionScan::passSample() const {
  if ((rand() / (float)RAND_MAX) < state_.sample_rate_) return true;
  return false;
}

//} // namespace physical_query_plan
//} // namespace claims
