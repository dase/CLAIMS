/*
 * Catalog.cpp
 *
 *  Created on: Oct 31, 2013
 *      Author: wangli
 */

#include "Catalog.h"

#include <sys/types.h>
#include <dirent.h>
#include <fcntl.h>
#include <glog/logging.h>
#include <string>
#include <vector>

#include "../common/rename.h"
#include "../Config.h"
#include "../Loader/Hdfsconnector.h"
#include "../Loader/LocalDiskConnector.h"
using std::vector;
using std::string;
using std::endl;

using namespace claims::common;
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
     * fix:add to eliminate effect on table id;	-- yu, 2015-2-8
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

  vector<vector<string>> catalog_files;
  vector<string> temp;
  temp.push_back(Config::catalog_file);
  catalog_files.push_back(temp);

  Connector* connector = NULL;
  if (0 == Config::local_disk_mode)
    connector = new HdfsConnector(catalog_files);
  else
    connector = new LocalDiskConnector(catalog_files);

  if (false == connector->openFiles(open_flag::CREATEE)) {
    LOG(ERROR) << "open catalog file failed" << endl;
  }

  if (oss.str().length() !=
      connector->flush(0, 0, static_cast<const void*>(oss.str().c_str()),
                       oss.str().length())) {
    LOG(ERROR) << "write catalog into file failed" << endl;
  }
  if (false == connector->closeFiles()) {
    LOG(ERROR) << "close catalog file failed" << endl;
  }
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

bool Catalog::CanFileAccessed(string file_name) {
  if (Config::local_disk_mode) {
    return 0 == access(file_name.c_str(), 0);
  } else {
    hdfsFS hdfsfs =
        hdfsConnect(Config::hdfs_master_ip.c_str(), Config::hdfs_master_port);
    return 0 == hdfsExists(hdfsfs, file_name.c_str());
  }
}

RetCode Catalog::LoadFileFromHdfs(string file_name, void*& buffer,
                                  int* read_length) {
  hdfsFS fs =
      hdfsConnect(Config::hdfs_master_ip.c_str(), Config::hdfs_master_port);
  hdfsFile readFile = hdfsOpenFile(fs, file_name.c_str(), O_RDONLY, 0, 0, 0);
  if (NULL == readFile) {
    PLOG(ERROR) << "Fail to open file [" << file_name << "].Reason:";
    hdfsDisconnect(fs);
    return EOpenHdfsFileFail;
  } else {
    LOG(INFO) << "file [" << file_name << "] is opened " << endl;
  }
  hdfsFileInfo* hdfsfile = hdfsGetPathInfo(fs, file_name.c_str());
  int file_length = hdfsfile->mSize;
  LOG(INFO) << "The length of file " << file_name << "is " << file_length
            << endl;

  buffer = new char[file_length];
  int read_num = hdfsRead(fs, readFile, buffer, file_length);
  hdfsCloseFile(fs, readFile);
  hdfsDisconnect(fs);
  if (read_num != file_length) {
    LOG(ERROR) << "read file [" << file_name << "] from hdfs failed" << endl;
    return EReadHdfsFileFail;
  } else {
    LOG(INFO) << "read " << read_num << " from hdfs file " << file_name << endl;
  }
  *read_length = read_num;
  return kSuccess;
}

RetCode Catalog::LoadFileFromDisk(string file_name, void*& buffer,
                                  int* read_length) {
  int fd = FileOpen(file_name.c_str(), O_RDONLY | O_CREAT, S_IRUSR | S_IWUSR);
  long int file_length = lseek(fd, 0, SEEK_END);
  if (-1 == file_length) {
    PLOG(ERROR) << "lseek called on fd to set pos to the end of file " << fd
                << " failed : ";
    return ELSeekDiskFileFail;
  }
  LOG(INFO) << "The length of file " << file_name << "is " << file_length
            << endl;
  buffer = new char[file_length];
  // reset pos to 0
  if (0 == lseek(fd, 0, SEEK_SET)) {
    PLOG(ERROR) << "lseek called on fd to reset pos to the start of file " << fd
                << " failed : ";
  }
  int read_num = read(fd, buffer, file_length);
  LOG(INFO) << "read " << read_num << " from disk file " << file_name << endl;

  FileClose(fd);
  if (read_num != file_length) {
    LOG(ERROR) << "read file [" << file_name
               << "] from disk failed, expected read " << file_length
               << " , actually read " << read_num << endl;
    return EReadDiskFileFail;
  }
  *read_length = read_num;
  return kSuccess;
}

// 2014-3-20---restore from a file---by Yu
RetCode Catalog::restoreCatalog() {
  string catalog_file = Config::catalog_file;
  // check whether there is catalog file if there are data file
  if (!CanFileAccessed(catalog_file) && IsDataFileExist()) {
    LOG(ERROR) << "The data file are existed while catalog file "
               << catalog_file << " is not existed!" << endl;
    return ECatalogNotFound;
  } else if (!CanFileAccessed(catalog_file)) {
    LOG(INFO) << "The catalog file and data file all are not existed" << endl;
    return kSuccess;
  } else if (IsDataFileExist()) {
    LOG(WARNING) << "There are no data file while catalog file exists" << endl;
    return kSuccess;
  } else {
    int file_length = 0;
    void* buffer;
    if (Config::local_disk_mode) {
      if (kSuccess != LoadFileFromDisk(catalog_file, buffer, &file_length) ||
          (buffer == NULL)) {
        LOG(ERROR) << "load catalog data from " << catalog_file << " failed"
                   << endl;
        return ECatalogRestoreInvild;
      }
    } else {
      if (kSuccess != LoadFileFromHdfs(catalog_file, buffer, &file_length) ||
          (buffer == NULL)) {
        LOG(ERROR) << "load catalog data from " << catalog_file << " failed"
                   << endl;
        return ECatalogRestoreInvild;
      }
    }

    LOG(INFO) << "Start to deserialize catalog ..." << endl;
    string temp(static_cast<char*>(buffer), file_length);
    std::istringstream iss(temp);
    boost::archive::text_iarchive ia(iss);
    ia >> *this;
    return kSuccess;
  }
}
