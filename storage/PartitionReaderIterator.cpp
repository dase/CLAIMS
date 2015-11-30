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
 * /CLAIMS/storage/PartitionReaderIterator.cpp
 *
 *  Created on: NOV 19, 2015
 *      Author: Hanzhang,wangli
 *		   Email:
 *
 * Description:
 *
 */
#include "./PartitionReaderIterator.h"
// namespace claims {
// namespace storage {
PartitionReaderIterator::PartitionReaderIterator(
    PartitionStorage* partition_storage)
    : ps_(partition_storage), chunk_cur_(0), chunk_it_(NULL) {}

PartitionReaderIterator::~PartitionReaderIterator() {}

ChunkReaderIterator* PartitionReaderIterator::NextChunk() {
  if (chunk_cur_ < ps_->number_of_chunks_)
    return ps_->chunk_list_[chunk_cur_++]->CreateChunkReaderIterator();
  else
    return NULL;
}

AtomicPartitionReaderIterator::~AtomicPartitionReaderIterator() {}

ChunkReaderIterator* AtomicPartitionReaderIterator::NextChunk() {
  ChunkReaderIterator* ret = NULL;
  if (chunk_cur_ < ps_->number_of_chunks_)
    ret = ps_->chunk_list_[chunk_cur_++]->CreateChunkReaderIterator();
  else
    ret = NULL;
  return ret;
}

bool PartitionReaderIterator::NextBlock(BlockStreamBase*& block) {
  assert(false);
  if (chunk_it_ > 0 && chunk_it_->NextBlock(block)) {
    return true;
  } else {
    if ((chunk_it_ = NextChunk()) > 0) {
      return NextBlock(block);
    } else {
      return false;
    }
  }
}

bool AtomicPartitionReaderIterator::NextBlock(BlockStreamBase*& block) {
  lock_.acquire();
  ChunkReaderIterator::block_accessor* ba = NULL;
  if (chunk_it_ != 0 && chunk_it_->GetNextBlockAccessor(ba)) {
    lock_.release();
    ba->GetBlock(block);
    return true;
  } else {
    if ((chunk_it_ = PartitionReaderIterator::NextChunk()) > 0) {
      lock_.release();
      return NextBlock(block);
    } else {
      lock_.release();
      return false;
    }
  }
}  //这个函数是关键，我需要考虑清楚，在决定如何动，这块～～～ --han 1123
//}  // namespace storage
//}  // namespace claims
