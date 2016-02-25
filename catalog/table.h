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
 * /Claims/catalog/table.h
 *
 *  Created on: 2013-9-22
 *      Author: liyongfeng, wangli, yukai
 *       Email: yukai2014@gmail.com
 *
 * Description:
 *
 */

#ifndef CATALOG_TABLE_H_
#define CATALOG_TABLE_H_

#include <boost/serialization/map.hpp>
#include <boost/serialization/set.hpp>
#include <vector>
#include <set>
#include <string>
#include <map>

#include "../catalog/attribute.h"
#include "../catalog/column.h"
#include "../catalog/partitioner.h"
#include "../catalog/projection.h"
#include "../common/ids.h"
#include "../common/Schema/Schema.h"
#include "../utility/lock.h"
#include "../utility/lock_guard.h"

using claims::catalog::ProjectionDescriptor;
using claims::utility::LockGuard;

namespace claims {

namespace loader {
class DataInjector;
};

namespace catalog {
class TableDescriptor {
 public:
  friend class claims::loader::DataInjector;

 public:
  TableDescriptor() {}
  TableDescriptor(const string& name, const TableID table_id);
  virtual ~TableDescriptor();

  void addAttribute(Attribute attr);
  bool addAttribute(string attname, data_type dt, unsigned max_length = 0,
                    bool unique = false, bool can_be_null = true);

  //  void addProjection(vector<ColumnOffset> id_list);
  bool createHashPartitionedProjection(vector<ColumnOffset> column_list,
                                       ColumnOffset partition_key_index,
                                       unsigned number_of_partitions);
  bool createHashPartitionedProjection(vector<ColumnOffset> column_list,
                                       std::string partition_attribute_name,
                                       unsigned number_of_partitions);
  bool createHashPartitionedProjection(vector<Attribute> column_list,
                                       std::string partition_attribute_name,
                                       unsigned number_of_partitions);
  bool createHashPartitionedProjectionOnAllAttribute(
      std::string partition_attribute_name, unsigned number_of_partitions);
  bool isExist(const string& name) const;
  inline string getTableName() const { return tableName; }
  ColumnOffset getColumnID(const string& attrName) const;
  map<string, set<string> > getColumnLocations(const string& attrName) const;

  vector<Attribute> getAttributes() const { return attributes; }
  vector<Attribute> getAttributes(vector<unsigned> index_list) const {
    vector<Attribute> attribute_list;
    for (unsigned i = 0; i < index_list.size(); i++) {
      assert(index_list[i] < attributes.size());
      attribute_list.push_back(attributes[index_list[i]]);
    }
    return attribute_list;
  }
  Attribute getAttribute(unsigned offset) const {
    assert(offset < attributes.size());
    return attributes[offset];
  }
  Attribute getAttribute(const std::string& name) const;
  Attribute getAttribute2(const std::string& name) const;
  /* the following methods are considered to be deleted.*/
  void addColumn(ProjectionDescriptor* column);
  inline string get_table_name() const { return tableName; }
  inline TableID get_table_id() const { return table_id_; }
  ProjectionDescriptor* getProjectoin(ProjectionOffset) const;
  unsigned getNumberOfProjection() const;
  vector<ProjectionDescriptor*>* GetProjectionList() {
    return &projection_list_;
  }

  Schema* getSchema() const;
  inline void setRowNumber(unsigned long row_number) {
    LockGuard<Lock> guard(lock_);
    row_number_ = row_number;
  }
  inline unsigned long getRowNumber() { return row_number_; }
  inline unsigned long isEmpty() { return row_number_ == 0; }
  inline unsigned int getNumberOfAttribute() {
    return attributes.size();
  }  // add by Yu
  inline bool HasDeletedTuples() { return has_deleted_tuples_; }
  inline void SetDeletedTuples(bool has_deleted_tuples) {
    has_deleted_tuples_ = has_deleted_tuples;
  }

 protected:
  string tableName;
  vector<Attribute> attributes;
  TableID table_id_;
  vector<ProjectionDescriptor*> projection_list_;
  unsigned long row_number_;
  bool has_deleted_tuples_ = false;

  Lock lock_;
  // delete for debugging
  // hashmap<ColumnID, ColumnDescriptor*> columns;

  friend class boost::serialization::access;
  template <class Archive>
  void serialize(Archive& ar, const unsigned int version) {
    ar& tableName& attributes& table_id_& projection_list_& row_number_&
        has_deleted_tuples_;
  }
};

} /* namespace catalog */
} /* namespace claims */

#endif  // CATALOG_TABLE1_H_
