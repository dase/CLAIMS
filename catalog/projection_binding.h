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
 * /Claims/catalog/projection_binding.h
 *
 *  Created on: Nov 2, 2013
 *      Author: wangli
 *  Renamed on: Oct 26, 2015
 *      Author: yukai
 *		   Email: yukai2014@gmail.com
 *
 * Description:
 *
 */

#ifndef CATALOG_PROJECTION_BINDING_H_
#define CATALOG_PROJECTION_BINDING_H_

#include "../catalog/partitioner.h"
#include "../storage/StorageLevel.h"
#include "../utility/lock.h"
namespace claims {
namespace catalog {

class ProjectionBinding {
 public:
  ProjectionBinding();
  virtual ~ProjectionBinding();

  /*binding an entire projection independently*/
  bool BindingEntireProjection(
      Partitioner*, const StorageLevel& desriable_storage_level = MEMORY);
  bool UnbindingEntireProjection(Partitioner* part);

 private:
  Lock lock_;
};

} /* namespace catalog */
} /* namespace claims */

#endif  // CATALOG_PROJECTION_BINDING_H_
