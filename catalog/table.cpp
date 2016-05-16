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
 * /Claims/catalog/table.cpp
 *
 *  Created on: 2013-9-22
 *      Author: liyongfeng, wangli, yukai
 *       Email: yukai2014@gmail.com
 *
 * Description:
 *
 */
#include "../catalog/table.h"

#include <string>
#include <vector>

#include "../common/file_handle/file_handle_imp_factory.h"
#include "../common/Schema/SchemaFix.h"
#include "../loader/table_file_connector.h"
using claims::common::FilePlatform;
using claims::utility::LockGuard;
namespace claims {
namespace catalog {

// only for deserialization
TableDescriptor::TableDescriptor() {}

TableDescriptor::TableDescriptor(const string& name, const TableID table_id)
    : tableName(name), table_id_(table_id), row_number_(0) {
  write_connector_ = new TableFileConnector(
      Config::local_disk_mode ? FilePlatform::kDisk : FilePlatform::kHdfs, this,
      common::kAppendFile);
}

TableDescriptor::~TableDescriptor() {}

void TableDescriptor::addAttribute(Attribute attr) {
  LockGuard<SpineLock> guard(update_lock_);
  attributes.push_back(attr);
}

// make sure the format of attname is column name
bool TableDescriptor::addAttribute(string attname, data_type dt,
                                   unsigned max_length, bool unique,
                                   bool can_be_null) {
  LockGuard<SpineLock> guard(update_lock_);
  attname = tableName + '.' + attname;
  /*check for attribute rename*/

  for (unsigned i = 0; i < attributes.size(); i++) {
    if (attributes[i].attrName == attname) return false;
  }
  Attribute att(table_id_, attributes.size(), attname, dt, max_length, unique,
                can_be_null);
  attributes.push_back(att);
  return true;
}

RetCode TableDescriptor::InitFileConnector() {
  write_connector_ = new TableFileConnector(
      Config::local_disk_mode ? FilePlatform::kDisk : FilePlatform::kHdfs, this,
      common::kAppendFile);
}

RetCode TableDescriptor::createHashPartitionedProjection(
    const vector<ColumnOffset>& column_list, Attribute partition_attribute,
    unsigned number_of_partitions) {
  //  LockGuard<SpineLock> guard(update_lock_);
  if (!update_lock_.try_lock()) {
    LOG(WARNING) << "failed to lock update_lock, may someone is loading or "
                    "inserting data";
    return common::rResourceIsLocked;
  }
  ProjectionID projection_id(table_id_, projection_list_.size());
  ProjectionDescriptor* projection = new ProjectionDescriptor(projection_id);

  for (unsigned i = 0; i < column_list.size(); i++) {
    projection->addAttribute(attributes[column_list[i]]);
  }

  PartitionFunction* hash_function =
      PartitionFunctionFactory::createGeneralModuloFunction(
          number_of_partitions);
  projection->DefinePartitonier(number_of_partitions, partition_attribute,
                                hash_function);

  projection_list_.push_back(projection);
  //  AddProjectionLocks(number_of_partitions);
  //  UpdateConnectorWithNewProj(number_of_partitions);
  write_connector_->UpdateWithNewProj();
  update_lock_.release();
  return rSuccess;
}

RetCode TableDescriptor::createHashPartitionedProjection(
    const vector<Attribute>& attribute_list, Attribute partition_attr,
    unsigned number_of_partitions) {
  //  LockGuard<SpineLock> guard(update_lock_);
  if (!update_lock_.try_lock()) {
    LOG(WARNING) << "failed to lock update_lock, may someone is loading or "
                    "inserting data";
    return common::rResourceIsLocked;
  }
  ProjectionID projection_id(table_id_, projection_list_.size());
  ProjectionDescriptor* projection = new ProjectionDescriptor(projection_id);

  for (unsigned i = 0; i < attribute_list.size(); i++) {
    projection->addAttribute(attribute_list[i]);
  }

  PartitionFunction* hash_function =
      PartitionFunctionFactory::createGeneralModuloFunction(
          number_of_partitions);
  projection->DefinePartitonier(number_of_partitions, partition_attr,
                                hash_function);

  projection_list_.push_back(projection);
  //  AddProjectionLocks(number_of_partitions);
  write_connector_->UpdateWithNewProj();
  update_lock_.release();
  return rSuccess;
}

// void TableDescriptor::AddProjectionLocks(int number_of_partitions) {
//  vector<Lock> locks;
//  for (int i = 0; i < number_of_partitions; ++i) locks.push_back(Lock());
//  partitions_write_lock_.push_back(locks);
//}

bool TableDescriptor::isExist(const string& name) const {
  for (unsigned i = 0; i < attributes.size(); i++) {
    if (attributes[i].attrName == name) return true;
  }
  return false;
}

ProjectionDescriptor* TableDescriptor::getProjectoin(
    ProjectionOffset pid) const {
  if (pid >= 0 && pid < projection_list_.size()) {
    return projection_list_.at(pid);
  } else {
    LOG(WARNING) << "no projection has been created on this table" << endl;
    return NULL;
  }
}
unsigned TableDescriptor::getNumberOfProjection() const {
  return projection_list_.size();
}

Attribute TableDescriptor::getAttribute(const std::string& name) const {
  // format of name is colname, not tablename.colname
  stringstream ss;
  ss << tableName.c_str() << "." << name.c_str();
  //  cout<<"partition_name :"<<name<<endl; // for test--by yu
  for (unsigned i = 0; i < attributes.size(); i++) {
    //    cout<<attributes[i].attrName<<endl; // for test -- by yu
    //    cout<<"--"<<attributes[i].attrName<<"--"<<ss.str()<<endl;
    if (attributes[i].attrName == ss.str()) {
      return attributes[i];
    }
  }
  printf("The attribute name [%s] does not match any attribute!\n",
         ss.str().c_str());
  assert(false);
  return NULL;
}

Attribute TableDescriptor::getAttribute2(const std::string& name) const {
  // format of name is tablename.colname
  stringstream ss;
  ss << name.c_str();
  for (unsigned i = 0; i < attributes.size(); i++) {
    //    cout<<"--"<<attributes[i].attrName<<endl;
    if (attributes[i].attrName == ss.str()) {
      return attributes[i];
    }
  }
  printf("The attribute name [%s] does not match any attribute!\n",
         ss.str().c_str());
  assert(false);
  return NULL;
}

vector<vector<string>> TableDescriptor::GetAllPartitionsPath() const {
  vector<vector<string>> write_paths;
  for (int i = 0; i < getNumberOfProjection(); i++) {
    vector<string> prj_write_path;
    prj_write_path.clear();
    for (int j = 0;
         j < projection_list_[i]->getPartitioner()->getNumberOfPartitions();
         ++j) {
      prj_write_path.push_back(
          PartitionID(getProjectoin(i)->getProjectionID(), j).getPathAndName());
    }
    write_paths.push_back(prj_write_path);
  }
  LOG(INFO) << " table:" << getTableName() << " has the below partition:";
  for (auto prj : write_paths)
    for (auto part : prj) LOG(INFO) << part;
  return write_paths;
}
Schema* TableDescriptor::getSchema() const {
  const vector<Attribute> attributes = getAttributes();
  std::vector<column_type> columns;
  for (unsigned i = 0; i < attributes.size(); i++) {
    columns.push_back(*(attributes[i].attrType));
  }
  return new SchemaFix(columns);
}

} /* namespace catalog */
} /* namespace claims */
