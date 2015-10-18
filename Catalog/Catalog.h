/*
 * Catalog.h
 *
 *  Created on: Oct 31, 2013
 *      Author: wangli
 */

#ifndef CATALOG_H_
#define CATALOG_H_
#include <boost/unordered_map.hpp>
#include <string>

#include "./table.h"
#include "./unordered_map.hpp"
#include "./ProjectionBinding.h"
#include "../common/error_define.h"
#include "../common/Logging.h"
#include "../utility/lock.h"

struct TableIDAllocator {
  TableIDAllocator() { table_id_curosr = 0; }
  Lock lock_;
  unsigned table_id_curosr;
  unsigned allocate_unique_table_id() {  // add lock, like postgreSQL --- yukai
    lock_.acquire();
    unsigned id = table_id_curosr;
    ++table_id_curosr;
    lock_.release();
    return id;
  }

  void decrease_table_id() {
    lock_.acquire();
    --table_id_curosr;
    lock_.release();
  }

  friend class boost::serialization::access;
  template <class Archive>
  void serialize(Archive& ar, const unsigned int file_version) {
    ar& table_id_curosr;
  }
};

class Catalog {
 public:
  static Catalog* getInstance();
  virtual ~Catalog();
  unsigned allocate_unique_table_id();
  bool add_table(TableDescriptor* const& table);
  TableDescriptor* getTable(const TableID&) const;
  TableDescriptor* getTable(const std::string& table_name) const;
  ProjectionDescriptor* getProjection(const ProjectionID&) const;
  ProjectionBinding* getBindingModele() const;

  unsigned getTableCount() const { return table_id_allocator.table_id_curosr; }

  RetCode saveCatalog();     // 2014-3-20---save as a file---by Yu
  RetCode restoreCatalog();  // 2014-3-20---restore from a file---by Yu
  void outPut();

  /* whether given attribute specified by table_name and attribute_name exists*/
  bool isAttributeExist(const std::string& table_name,
                        const std::string& attribute_name) const;

  vector<PartitionID> getPartitionIDList(const std::string& table_name,
                                         const std::string& attribute_name);

 private:
  Catalog();  // avoiding generate more instance
  bool IsDataFileExist();
  bool CanFileAccessed(string file_name);
  RetCode LoadFileFromHdfs(string file_name, void*& buffer, int* read_length);
  RetCode LoadFileFromDisk(string file_name, void*& buffer, int* read_length);

  TableIDAllocator table_id_allocator;

  boost::unordered_map<std::string, TableDescriptor*> name_to_table;
  boost::unordered_map<TableID, TableDescriptor*> tableid_to_table;
  Logging* logging;
  ProjectionBinding* binding_;
  static Catalog* instance_;

  // 2014-3-20---add serialize function---by Yu
  friend class boost::serialization::access;
  template <class Archive>
  void serialize(Archive& ar, const unsigned int version) {
    ar& table_id_allocator& name_to_table& tableid_to_table;
  }
};

#endif /* CATALOG_H_ */
