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
 * /CLAIMS/storage/StorageLevel.h
 *
 *  Created on: Nov 16, 2013
 *  Modified on: Nov 13, 2015
 *      Author: Hanzhang, wangli
 *		   Email:
 *
 * Description: The level of storage is divided into three level. Data is aimed
 * to put into memory.
 *
 */

#ifndef STORAGE_STORAGELEVEL_H_
#define STORAGE_STORAGELEVEL_H_

typedef unsigned StorageLevel;
#define MEMORY 0
#define DISK 1
#define HDFS 2

#define DESIRIABLE_STORAGE_LEVEL MEMORY

#endif  //  STORAGE_STORAGELEVEL_H_
