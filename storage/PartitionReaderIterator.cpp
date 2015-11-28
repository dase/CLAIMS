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
 * /Claims/storage/PartitionReaderIterator.cpp
 *
 *  Created on: 2015年11月19日
 *      Author: Han
 *		   Email:
 *
 * Description:
 *
 */
#include "./PartitionReaderIterator.h"

PartitionReaderIterator::PartitionReaderIterator(
    PartitionStorage* partition_storage)
    : ps(partition_storage), chunk_cur_(0), chunk_it_(NULL) {}

PartitionReaderIterator::~PartitionReaderIterator() {}

ChunkReaderIterator* PartitionReaderIterator::nextChunk() {
  if (chunk_cur_ < ps->number_of_chunks_)
    return ps->chunk_list_[chunk_cur_++]->createChunkReaderIterator();
  else
    return NULL;
}

AtomicPartitionReaderIterator::~AtomicPartitionReaderIterator() {}

ChunkReaderIterator* AtomicPartitionReaderIterator::nextChunk() {
  //  lock_.acquire();
  ChunkReaderIterator* ret = NULL;
  if (chunk_cur_ < ps->number_of_chunks_)
    ret = ps->chunk_list_[chunk_cur_++]->createChunkReaderIterator();
  else
    ret = NULL;
  //  lock_.release();
  return ret;
}

bool PartitionReaderIterator::nextBlock(BlockStreamBase*& block) {
  assert(false);
  if (chunk_it_ > 0 && chunk_it_->NextBlock(block)) {
    return true;
  } else {
    if ((chunk_it_ = nextChunk()) > 0) {
      return nextBlock(block);
    } else {
      return false;
    }
  }
}

bool AtomicPartitionReaderIterator::nextBlock(BlockStreamBase*& block) {
  lock_.acquire();
  ChunkReaderIterator::block_accessor* ba;
  if (chunk_it_ != 0 && chunk_it_->getNextBlockAccessor(ba)) {
    lock_.release();
    ba->GetBlock(block);
    return true;
  } else {
    if ((chunk_it_ = PartitionReaderIterator::nextChunk()) > 0) {
      lock_.release();
      return nextBlock(block);
    } else {
      lock_.release();
      return false;
    }
  }
}  //这个函数是关键，我需要考虑清楚，在决定如何动，这块～～～ --han 1123
