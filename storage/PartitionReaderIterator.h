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
 * /Claims/storage/PartitionReaderIderator.h
 *
 *  Created on: Nov 19 ,2015
 *      Author: Han，wangli
 *		   Email:
 *
 * Description: It belongs to design patterns of iterator. This is a iterator of
 * PartitionStorage.
 *
 */

#ifndef STORAGE_PARTITIONREADERITERATOR_H_
#define STORAGE_PARTITIONREADERITERATOR_H_

#include "ChunkStorage.h"
#include "StorageLevel.h"
#include "PartitionReaderIterator.h"
#include "../utility/lock.h"

class PartitionReaderIterator {
 public:
  //    PartitionReaderIteaor();
  PartitionReaderIterator(PartitionStorage* partition_storage);
  virtual ~PartitionReaderIterator();
  virtual ChunkReaderIterator* nextChunk();
  virtual bool nextBlock(BlockStreamBase*& block);

 protected:
  PartitionStorage* ps;
  unsigned chunk_cur_;
  ChunkReaderIterator* chunk_it_;
};

class AtomicPartitionReaderIterator : public PartitionReaderIterator {
 public:
  //    AtomicPartitionReaderIterator();
  AtomicPartitionReaderIterator(PartitionStorage* partition_storage)
      : PartitionReaderIterator(partition_storage){};
  virtual ~AtomicPartitionReaderIterator() override;
  ChunkReaderIterator* nextChunk() override;
  virtual bool nextBlock(BlockStreamBase*& block);

 private:
  Lock lock_;
};

#endif  //  STORAGE_PARTITIONREADERITERATOR_H_
