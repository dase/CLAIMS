/*
 * Hdfsloader.h
 *
 *  Created on: Dec 23, 2013
 *      Author: root
 */

#ifndef HDFSLOADER_H_
#define HDFSLOADER_H_


#include<iostream>
#include<vector>
#include<string>
#include <hdfs.h>

#include "../Environment.h"
#include "../Catalog/table.h"
#include "../Catalog/Catalog.h"
#include "../Catalog/ProjectionBinding.h"
#include "../common/Schema/TupleConvertor.h"
#include "../common/hash.h"
#include "Connector.h"

using namespace std;

class HdfsLoader {
public:
	HdfsLoader();
	HdfsLoader(TableDescriptor* tableDescriptor,  open_flag open_flag_ );
	HdfsLoader(TableDescriptor* tableDescriptor, const char c_separator = '|', const char r_separator = '\n', open_flag open_flag_=APPENDD);
	HdfsLoader(const char c_separator, const char r_separator, std::vector<std::string> file_name, TableDescriptor* tableDescriptor, open_flag open_flag_=CREATEE);
	virtual ~HdfsLoader();

	const char get_c_separator();
	const char get_r_separator();
	vector<string> get_file_list();

	bool insertRecords();

	bool load(double sample_rate=1);

	bool append(std::string tuple_string);

	bool DeleteDataFilesForDropTable();

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
	vector < vector<BlockStreamBase*> > pj_buffer;

	vector<vector <unsigned long> > blocks_per_partition;
	vector<vector <unsigned long> > tuples_per_partition;
	vector<SubTuple*> sub_tuple_generator;

	Schema* table_schema;
	vector<Schema*> projection_schema;

	open_flag open_flag_;

};

#endif /* HDFSLOADER_H_ */
