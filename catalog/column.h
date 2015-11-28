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
 * /Claims/catalog/column.h
 *
 *  Created on: Nov 8, 2013
 *      Author: wangli
 *  Renamed on: Oct 26, 2015
 *      Author: yukai
 *		   Email: yukai2014@gmail.com
 *
 * Description:
 *
 */

#ifndef CATALOG_COLUMN_H_
#define CATALOG_COLUMN_H_
#include "../catalog/attribute.h"
#include "../common/ids.h"

using claims::catalog::Attribute;

namespace claims {
namespace catalog {

class Column : public Attribute {
 public:
  Column() {}
  Column(Attribute&, ColumnID);
  Column(const Column& c) : Attribute(c) { this->column_id_ = c.column_id_; }
  virtual ~Column();
  ColumnID column_id_;

  /* for boost::serialization*/
  friend class boost::serialization::access;
  template <class Archive>
  void serialize(Archive& ar, const unsigned int version) {
    ar& boost::serialization::base_object<Attribute>(*this) & column_id_;
  }
};

} /* namespace catalog */
} /* namespace claims */

#endif  // CATALOG_COLUMN_H_
