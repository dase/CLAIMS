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
 * /Claims/catalog/attribute.h
 *
 *  Created on: Oct 30, 2013
 *      Author: wangli, yukai
 *		   Email: yukai2014@gmail.com
 *
 * Description: rename to attribute.h by yukai
 *
 */

#ifndef CATALOG_ATTRIBUTE_H_
#define CATALOG_ATTRIBUTE_H_

#include <string>
#include "../common/ids.h"
#include "../common/data_type.h"
#include "../common/memory_handle.h"
#include "../common/error_define.h"

using claims::common::rSuccess;
using claims::common::rEmptyAttributeName;
using claims::common::rColumnNotExist;
using std::string;
namespace claims {
namespace catalog {

struct Attribute {
  enum AttributeType { kAttributeNull = -1, kAttributeAny = -2 };
  Attribute(TableID tableid, unsigned pos, const std::string& name,
            data_type type, unsigned size = 0, bool unqiue = false,
            bool can_be_null = true) {
    table_id_ = tableid;
    index = pos;
    attrName = name;
    attrType = new column_type(type, size, can_be_null);
    unique = unqiue;
  }
  Attribute(const Attribute& att) {
    table_id_ = att.table_id_;
    attrName = att.attrName;
    unique = att.unique;
    if (att.table_id_ < kAttributeAny) {
      //      cout<<"Attribute    ="<<att.table_id_<<endl;
      //      cout<<"ATTRIBUTE_ANY="<<ATTRIBUTE_ANY<<endl;
      //      if(att.table_id_==NULL)
      //      {
      //        puts("att.table_id_ is NULL");
      //      }
      //      if(att.table_id_<-2)
      //      {
      //        puts("0<-2  oh ,my god!!");
      //      }
      attrType = new column_type(*att.attrType);
      index = att.index;
    }
  }
  Attribute& operator=(const Attribute& att) {
    table_id_ = att.table_id_;
    attrName = att.attrName;
    unique = att.unique;
    if (att.table_id_ < kAttributeAny) {
      attrType = new column_type(*att.attrType);
      index = att.index;
    }
    return *this;
  }

  bool operator<(const Attribute& att) const {
    if (table_id_ < att.table_id_)
      return false;
    else if (table_id_ == att.table_id_)
      return (index < att.index);
    else
      return false;
  }

  Attribute(unsigned symbol = kAttributeNull)
      : table_id_(symbol), index(-1), attrType(0), unique(false) {
    switch (symbol) {
      case kAttributeAny: {
        attrName = "*";
        break;
      }
      case kAttributeNull: {
        attrName = "NULL";
        break;
      }
      default: { assert(false); }
    }
  }
  ~Attribute() {
    if (table_id_ < TableID(kAttributeAny)) {
      DELETE_PTR(attrType);
    }
  }
  bool operator==(const Attribute& r) const {
    return table_id_ == r.table_id_ && index == r.index;
  }
  bool operator!=(const Attribute& r) const {
    return !(table_id_ == r.table_id_ && index == r.index);
  }
  bool isNULL() const { return table_id_ == TableID(kAttributeNull); }
  bool isANY() const { return table_id_ == TableID(kAttributeAny); }
  bool isUnique() const { return unique; }
  string getName() const { return attrName; }
  AttributeID getID() const { return AttributeID(table_id_, index); }
  unsigned GetAttributeIndex() const { return index; }

  inline RetCode SetAttributeName(const string name) {
    RetCode ret = rSuccess;
    if (0 != name.size()) {
      attrName = name;
      return ret;
    } else {
      ret = rEmptyAttributeName;
      LOG(WARNING) << "[" << ret << ", " << CStrError(ret) << "] "
                   << "the attribute has empty name in table:" << table_id_
                   << "with attribute index:" << index << std::endl;
    }
  }

  inline RetCode SetAttributeIndex(const unsigned attribute_index) {
    RetCode ret = rSuccess;
    if (NULL != this) {
      index = attribute_index;
      return ret;
    } else {
      ret = rColumnNotExist;
      WLOG(ret, "the attribute is not existed when setting its index");
      return ret;
    }
  }
  inline RetCode SetTableID(const unsigned table_id) {
    RetCode ret = rSuccess;
    if (NULL != this) {
      table_id_ = table_id;
      return ret;
    } else {
      ret = rColumnNotExist;
      WLOG(ret, "the attribute is not existed when setting its table id");
      return ret;
    }
  }

 public:
  string attrName;
  column_type* attrType;
  /*the position in the table*/
  unsigned index;
  TableID table_id_;
  bool unique;

  friend class boost::serialization::access;
  template <class Archive>
  void serialize(Archive& ar, const unsigned int version) {
    ar& attrName& attrType& index& table_id_& unique;
  }
};
}  // namespace catalog

}  // namespace claims

#endif  // CATALOG_ATTRIBUTE_H_
