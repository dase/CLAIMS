/*
 * Hdfsloader.h
 *
 *  Created on: Dec 23, 2013
 *      Author: root
 */

#ifndef HDFSLOADER_H_
#define HDFSLOADER_H_

#include <iostream>
#include <vector>
#include <string>
#include <hdfs.h>

#include "../common/file_handle/file_handle_imp.h"
#include "../Environment.h"
#include "../catalog/table.h"
#include "../catalog/catalog.h"
#include "../catalog/projection_binding.h"
#include "../common/Schema/TupleConvertor.h"
#include "../common/hash.h"
#include "Connector.h"

using namespace std;
using namespace claims::common;

class Hdfsloader {
 public:
  Hdfsloader();
  Hdfsloader(TableDescriptor* tableDescriptor, const char c_separator = '|',
             const char r_separator = '\n',
             FileOpenFlag open_flag = kAppendFile);
  Hdfsloader(const char c_separator, const char r_separator,
             std::vector<std::string> file_name,
             TableDescriptor* tableDescriptor,
             FileOpenFlag open_flag = kCreateFile);
  virtual ~Hdfsloader();

  const char Hdfsloader::get_c_separator() const { return col_separator; }
  const char Hdfsloader::get_r_separator() const { return row_separator; }
  vector<string> Hdfsloader::get_file_list() const { return file_list; }

  bool insertRecords();

  bool load(double sample_rate = 1);

  bool append(std::string tuple_string);

 public:
  TableDescriptor* table_descriptor_;

  unsigned long row_id;

  vector<PartitionFunction*> partition_functin_list_;
  vector<int> partition_key_index;

 private:
  Connector* connector_;
  unsigned block_size;
  const char col_separator;
  const char row_separator;
  std::vector<std::string> file_list;
  vector<vector<string> > writepath;
  Block* sblock;
  std::string s_record;
  vector<vector<BlockStreamBase*> > pj_buffer;

  vector<vector<unsigned long> > blocks_per_partition;
  vector<vector<unsigned long> > tuples_per_partition;
  vector<SubTuple*> sub_tuple_generator;

  Schema* table_schema;
  vector<Schema*> projection_schema;

  open_flag open_flag_;
};

#endif /* HDFSLOADER_H_ */
