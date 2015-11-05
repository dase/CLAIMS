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
 * /Claims/catalog/projection.h
 *
 *  Created on: Oct 23, 2015
 *      Author: yukai
 *		   Email: yukai2014@gmail.com
 *
 * Description:
 *
 */

#ifndef CATALOG_PROJECTION_H_
#define CATALOG_PROJECTION_H_
#include <map>
#include <set>
#include <string>
#include <vector>

#include "../catalog/column.h"
using std::map;
using std::set;
using std::vector;
using std::string;

class Schema;
namespace claims {
namespace catalog {
class Partitioner;
/**
 * Attribute describes a column in type, name, position in the table.
 */

/**
 * A projection is a combination of columns that belong to a single table and
 *will be horizontally
 * partitioned among storage instances.
 * ProjectionDescriptor mainly contains two kinds of important information:
 *
 * 1). how many attributes there are in a projection and what are they.
 *
 * 2). how the projection is partitioned (e.g., hash partition, range
 *partition). How many storage
 *    instances are involved.
 *
 */
class ProjectionDescriptor {
 public:
  friend class TableDescriptor;
  ProjectionDescriptor() {}
  explicit ProjectionDescriptor(ProjectionID);
  explicit ProjectionDescriptor(const string& name);
  virtual ~ProjectionDescriptor();
  void addAttribute(Attribute attr);
  bool hasAttribute(const Attribute& attr) const;
  void DefinePartitonier(const unsigned& number_of_partitions,
                         const Attribute& partition_key,
                         PartitionFunction* partition_functin);
  Partitioner* getPartitioner() const;
  bool isExist(const string& name) const;
  inline void setProjectionID(const ProjectionID& pid) { projection_id_ = pid; }
  inline map<string, set<string> > getFileLocations() const {
    return fileLocations;
  }
  inline ProjectionID getProjectionID() const { return projection_id_; }
  bool AllPartitionBound() const;
  std::vector<Attribute> getAttributeList() const;
  Schema* getSchema() const;
  int getAttributeIndex(const Attribute& att) const;
  int long getNumberOfTuplesOnPartition(const unsigned partition_off) const;

  /**
   * @brief Method description:
   * @return : the sum of all attribute's length in projection,
   *           as this projection's cost
   */
  unsigned int getProjectionCost() const;

 private:
  //  ProjectionOffset projection_offset_;
  ProjectionID projection_id_;
  vector<Column> column_list_;

  Partitioner* partitioner;

  /* The following is considered to be deleted, as the catalog module only has a
   * logically view
   * of a table rather than detailed physical view such as filename, etc.
   */
  map<string, set<string> > fileLocations;
  string hdfsFilePath;
  map<string, string> blkMemoryLocations;

  /* The following is deleted from version 1.2*/
  string Projection_name_;  // projection does not need a string name.

  friend class boost::serialization::access;
  template <class Archive>
  void serialize(Archive& ar, const unsigned int version) {
    /* The serialzation does not include fileLocations, hdfsFilePath and
     * blkMemoryLocations */
    ar& projection_id_& column_list_& partitioner& Projection_name_;
  }
};

} /* namespace catalog */
} /* namespace claims */

#endif  // CATALOG_PROJECTION_H_
