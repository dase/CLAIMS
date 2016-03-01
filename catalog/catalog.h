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
 * /Claims/catalog/catalog.h
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

#ifndef CATALOG_CATALOG_H_
#define CATALOG_CATALOG_H_
#include <boost/unordered_map.hpp>
#include <string>

#include "../catalog/projection_binding.h"
#include "../catalog/table.h"
#include "../catalog/unordered_map.hpp"
#include "../common/error_define.h"
#include "../common/Logging.h"
#include "../utility/lock.h"

namespace claims {

namespace loader {
class SingleFileConnector;
};

namespace catalog {
using loader::SingleFileConnector;
class SingleFileConnector;

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
  void GetAllTables(ostringstream& ostr) const;
  ProjectionDescriptor* getProjection(const ProjectionID&) const;
  ProjectionBinding* getBindingModele() const;

  /**
   * ATTENTION: this method do not return the number of existing table,
   *            other than the next table'id
   * An example is: a table is dropped, but the return value don't change
   */
  unsigned getTableCount() const { return table_id_allocator.table_id_curosr; }

  RetCode saveCatalog();     // 2014-3-20---save as a file---by Yu
  RetCode restoreCatalog();  // 2014-3-20---restore from a file---by Yu
  void outPut();

  /* whether given attribute specified by table_name and attribute_name exists*/
  bool isAttributeExist(const std::string& table_name,
                        const std::string& attribute_name) const;

  vector<PartitionID> getPartitionIDList(const std::string& table_name,
                                         const std::string& attribute_name);

  bool DropTable(const std::string table_name, const TableID id);

 private:
  Catalog();  // avoiding generate more instance
  bool IsDataFileExist();

  TableIDAllocator table_id_allocator;

  boost::unordered_map<std::string, TableDescriptor*> name_to_table;
  boost::unordered_map<TableID, TableDescriptor*> tableid_to_table;
  Logging* logging;
  ProjectionBinding* binding_;
  static Catalog* instance_;
  SingleFileConnector* connector_ = NULL;

  // 2014-3-20---add serialize function---by Yu
  friend class boost::serialization::access;
  template <class Archive>
  void serialize(Archive& ar, const unsigned int version) {
    ar& table_id_allocator& name_to_table& tableid_to_table;
  }
};

} /* namespace catalog */
} /* namespace claims */

#endif  // CATALOG_CATALOG_H_
