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
 * /Claims/catalog/catalog.cpp
 *
 *  Created on: Oct 31, 2013
 *      Author: wangli
 *  Renamed on: Oct 26, 2015
 *      Author: yukai
 *		   Email: yukai2014@gmail.com
 *
 * Description:
 *
 */

#include "../catalog/catalog.h"
#include <sys/types.h>
#include <dirent.h>
#include <fcntl.h>
#include <glog/logging.h>
#include <string>
#include <vector>

#include "../common/error_define.h"
#include "../common/rename.h"
#include "../Config.h"
#include "../loader/file_connector.h"
#include "../loader/single_file_connector.h"
using std::vector;
using std::string;
using std::endl;
using namespace claims::common;
using claims::loader::FileConnector;
using claims::loader::SingleFileConnector;

namespace claims {
namespace catalog {

Catalog* Catalog::instance_ = NULL;

Catalog::Catalog() {
  logging = new CatalogLogging();
  binding_ = new ProjectionBinding();
}

Catalog::~Catalog() {
  for (boost::unordered_map<std::string, TableDescriptor*>::iterator it =
           name_to_table.begin();
       it != name_to_table.end(); it++) {
    delete it->second;
  }
  instance_ = 0;
  delete logging;
  delete binding_;
}
Catalog* Catalog::getInstance() {
  if (instance_ == 0) {
    instance_ = new Catalog();
  }
  return instance_;
}
unsigned Catalog::allocate_unique_table_id() {
  return table_id_allocator.allocate_unique_table_id();
}
bool Catalog::add_table(TableDescriptor* const& table) {
  std::string new_table_name = table->getTableName();
  TableID new_table_id = table->get_table_id();
  boost::unordered_map<std::string, TableDescriptor*>::iterator
      it_name_to_table = name_to_table.find(new_table_name);
  if (it_name_to_table != name_to_table.cend()) {
    logging->elog("the table named %s is existed!", new_table_name.c_str());
    /*
     * bug:if name is duplicate, table can't be added successfully with
     * table_id_cursor increased
     * fix:add to eliminate effect on table id; -- yu, 2015-2-8
     */
    table_id_allocator.decrease_table_id();
    return false;
  }
  boost::unordered_map<TableID, TableDescriptor*>::iterator
      it_tableid_to_table = tableid_to_table.find(new_table_id);

  if (it_tableid_to_table != tableid_to_table.cend()) {
    logging->elog("the table whose id is %d is existed!", new_table_id);
    return false;
  }
  /*The check is successful. Now we can add the new table into the catalog
   * module.*/
  name_to_table[new_table_name] = table;
  tableid_to_table[new_table_id] = table;
  logging->log("New table \"%s\",id=%d is created!", new_table_name.c_str(),
               new_table_id);

  return true;
}
TableDescriptor* Catalog::getTable(const TableID& target) const {
  if (tableid_to_table.find(target) == tableid_to_table.cend()) return NULL;

  /* at could retain const while [] doesn't.*/
  return tableid_to_table.at(target);
}
TableDescriptor* Catalog::getTable(const std::string& table_name) const {
  if (name_to_table.find(table_name) == name_to_table.cend()) return NULL;

  /* at could retain const while [] doesn't.*/
  return name_to_table.at(table_name);
}
ProjectionDescriptor* Catalog::getProjection(const ProjectionID& pid) const {
  const TableDescriptor* td = getTable(pid.table_id);
  return td == 0 ? 0 : td->getProjectoin(pid.projection_off);
}
ProjectionBinding* Catalog::getBindingModele() const { return binding_; }

bool Catalog::isAttributeExist(const std::string& table_name,
                               const std::string& attribute_name) const {
  TableDescriptor* td = getTable(table_name);
  if (td == 0)
    return false;
  else
    return td->isExist(attribute_name);
}

vector<PartitionID> Catalog::getPartitionIDList(
    const std::string& table_name, const std::string& attribute_name) {
  vector<PartitionID> ret;
  ret.clear();
  TableDescriptor* table_descripter = this->getTable(table_name);
  ProjectionDescriptor* projection_descripter = NULL;
  unsigned projection_num = table_descripter->getNumberOfProjection();
  for (unsigned i = 0; i < projection_num; i++) {
    projection_descripter = table_descripter->getProjectoin(i);
    if (projection_descripter->isExist(attribute_name)) break;
  }
  return projection_descripter->getPartitioner()->getPartitionIDList();
}

void Catalog::outPut() {
  for (auto it_tableid_to_table = tableid_to_table.begin();
       it_tableid_to_table != tableid_to_table.end(); ++it_tableid_to_table) {
    cout << it_tableid_to_table->first << "  "
         << it_tableid_to_table->second->getTableName() << "  "
         << it_tableid_to_table->second->get_table_id() << "  ";
    cout << it_tableid_to_table->second->getNumberOfProjection();
  }
  cout << endl;
}

// 2014-3-20---save as a file---by Yu
RetCode Catalog::saveCatalog() {
  std::ostringstream oss;
  boost::archive::text_oarchive oa(oss);
  oa << *this;

  int ret = kSuccess;
  FileConnector* connector = new SingleFileConnector(
      Config::local_disk_mode ? kDisk : kHdfs, Config::catalog_file);

  EXEC_AND_ONLY_LOG_ERROR(ret, connector->Open(FileOpenFlag::kCreateFile),
                          "catalog file name:" << Config::catalog_file);

  EXEC_AND_ONLY_LOG_ERROR(
      ret, connector->Flush(static_cast<const void*>(oss.str().c_str()),
                            oss.str().length()),
      "catalog file name:" << Config::catalog_file);

  EXEC_AND_ONLY_LOG_ERROR(ret, connector->Close(),
                          "catalog file name:" << Config::catalog_file);
  return kSuccess;
}

bool Catalog::IsDataFileExist() {
  if (Config::local_disk_mode) {
    DIR* dir = NULL;
    struct dirent* file_ptr = NULL;

    dir = opendir(Config::data_dir.c_str());
    while ((file_ptr = readdir(dir)) != NULL) {
      if ('T' == file_ptr->d_name[0]) {
        LOG(INFO) << "The data disk file started with 'T': "
                  << file_ptr->d_name[0] << " is existed" << endl;
        return true;
      }
    }
    LOG(INFO) << "There are no data file in disk" << endl;
    return false;
  } else {
    hdfsFS hdfsfs =
        hdfsConnect(Config::hdfs_master_ip.c_str(), Config::hdfs_master_port);
    int file_num;
    hdfsFileInfo* file_list =
        hdfsListDirectory(hdfsfs, Config::data_dir.c_str(), &file_num);
    for (int cur = 0; cur < file_num; ++cur) {
      LOG(INFO) << "  " << file_list[cur].mName << "----";
      string full_file_name(file_list[cur].mName);
      int pos = full_file_name.find_last_of('/');
      string file_name = full_file_name.substr(pos + 1);
      LOG(INFO) << file_name << endl;
      if ('T' == file_name[0]) {
        LOG(INFO) << "The data HDFS file started with 'T': "
                  << file_list[cur].mName[0] << " is existed" << endl;
        hdfsFreeFileInfo(file_list, file_num);
        return true;
      }
    }
    hdfsFreeFileInfo(file_list, file_num);
    LOG(INFO) << "There are no data file in HDFS" << endl;
    return false;
  }
}

RetCode Catalog::restoreCatalog() {
  int ret = kSuccess;
  string catalog_file = Config::catalog_file;
  SingleFileConnector* connector = new SingleFileConnector(
      Config::local_disk_mode ? FilePlatform::kDisk : FilePlatform::kHdfs,
      catalog_file);

  // check whether there is catalog file if there are data file
  if (!connector->CanAccess() && IsDataFileExist()) {
    LOG(ERROR) << "The data file are existed while catalog file "
               << catalog_file << " is not existed!" << endl;
    return ECatalogNotFound;
  } else if (!connector->CanAccess()) {
    LOG(INFO) << "The catalog file and data file all are not existed" << endl;
    return kSuccess;
  } else if (!IsDataFileExist()) {
    LOG(WARNING) << "There are no data file while catalog file exists. "
                    "The catalog file will be overwrite" << endl;
    return kSuccess;
  } else {
    uint64_t file_length = 0;
    void* buffer;
    EXEC_AND_ONLY_LOG_ERROR(ret, connector->Open(kReadFile),
                            "catalog file name: " << catalog_file);
    EXEC_AND_ONLY_LOG_ERROR(ret, connector->LoadTotalFile(buffer, &file_length),
                            "catalog file name: " << catalog_file);

    LOG(INFO) << "Start to deserialize catalog ..." << endl;
    string temp(static_cast<char*>(buffer), file_length);
    std::istringstream iss(temp);
    boost::archive::text_iarchive ia(iss);
    ia >> *this;
    return kSuccess;
  }
}

} /* namespace catalog */
} /* namespace claims */
