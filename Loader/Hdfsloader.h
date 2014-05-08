/*
 * Hdfsloader.h
 *
 *  Created on: Dec 23, 2013
 *      Author: root
 */

#ifndef HDFSLOADER_H_
#define HDFSLOADER_H_

#include "Hdfsconnector.h"

#include<iostream>
#include<vector>
#include<string>

#include "../Catalog/table.h"
#include "../Catalog/Catalog.h"
#include "../hash.h"
#include "../Environment.h"
#include "../Schema/TupleConvertor.h"
#include "../Catalog/ProjectionBinding.h"
#include <hdfs.h>

using namespace std;

class HdfsLoader {
public:
	HdfsLoader();
	HdfsLoader(TableDescriptor* tableDescriptor, const char c_separator = '|', const char r_separator = '\n', open_flag open_flag_=APPEND);
	HdfsLoader(const char c_separator, const char r_separator, std::vector<std::string> file_name, TableDescriptor* tableDescriptor, open_flag open_flag_=CREATE);
	virtual ~HdfsLoader();

	const char get_c_separator();
	const char get_r_separator();
	vector<string> get_file_list();

	bool insertRecords();

	bool load();

	bool append(std::string tuple_string);

public:
	TableDescriptor* table_descriptor_;

	unsigned long row_id;

	vector<PartitionFunction*> partition_functin_list_;
	vector<int> partition_key_index;


private:
	HdfsConnector* connector_;
	unsigned block_size;
	const char col_separator;
	const char row_separator;
	std::vector<std::string> file_list;
	vector<vector<string> > writepath;
	Block* sblock;
	std::string s_record;
	vector < vector<BlockStreamBase*> > pj_buffer;

	vector<vector <unsigned long> > blocks_per_partition;
	vector<SubTuple*> sub_tuple_generator;

	Schema* table_schema;
	vector<Schema*> projection_schema;

	open_flag open_flag_;

};

#endif /* HDFSLOADER_H_ */
