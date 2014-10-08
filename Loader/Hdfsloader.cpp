/*
 * Hdfsloader.cpp
 *
 *  Created on: Dec 23, 2013
 *      Author: root
 */

#include "../common/Block/BlockStream.h"
#include "../Catalog/Attribute.h"
#include "Hdfsloader.h"
#include "../Debug.h"
#include <fstream>
#include <iostream>
#include "LocalDiskConnector.h"
#include "../Config.h"
#include "Hdfsconnector.h"

#define HDFS_LOAD

HdfsLoader::HdfsLoader(TableDescriptor* tableDescriptor, const char c_separator, const char r_separator, open_flag open_flag_)
:table_descriptor_(tableDescriptor), col_separator(c_separator), row_separator(r_separator), open_flag_(open_flag_), block_size(64*1024)
{
	if (open_flag_ == APPEND)
		row_id = table_descriptor_->getRowNumber();
	else
		row_id = 0;

	table_schema = table_descriptor_->getSchema();
	vector <unsigned> prj_index;
	for(int i = 0; i < table_descriptor_->getNumberOfProjection(); i++)
	{
		projection_schema.push_back(table_descriptor_->getProjectoin(i)->getSchema());
		vector<string> prj_writepath;
		prj_writepath.clear();
		for(int j=0; j<table_descriptor_->getProjectoin(i)->getPartitioner()->getNumberOfPartitions();j++){
			string path = PartitionID(table_descriptor_->getProjectoin(i)->getProjectionID(),j).getPathAndName();
			prj_writepath.push_back(path);
		}
		writepath.push_back(prj_writepath);


		Attribute partition_attribute = table_descriptor_->getProjectoin(i)->getPartitioner()->getPartitionKey();
		int hash_key_index=table_descriptor_->getProjectoin(i)->getAttributeIndex(partition_attribute);
		partition_key_index.push_back(hash_key_index);

		PartitionFunction* pf = table_descriptor_->getProjectoin(i)->getPartitioner()->getPartitionFunction();
		partition_functin_list_.push_back(pf);

		prj_index.clear();
		for (int j = 0; j < table_descriptor_->getProjectoin(i)->getAttributeList().size(); j++)
		{
			prj_index.push_back(table_descriptor_->getProjectoin(i)->getAttributeList()[j].index);
		}
		SubTuple* st = new SubTuple(table_descriptor_->getSchema(), table_descriptor_->getProjectoin(i)->getSchema(), prj_index);
		sub_tuple_generator.push_back(st);
	}

	sblock = new Block(BLOCK_SIZE);


	///////////////////////////////////assign 64k buffer to each PJ/////////////////////////////
	for(int i = 0; i < table_descriptor_->getNumberOfProjection(); i++)
	{
		vector<BlockStreamBase*> temp_v;
		vector<unsigned long> tmp_block_num;
		vector<unsigned long> tmp_tuple_count;
		for(int j = 0; j < table_descriptor_->getProjectoin(i)->getPartitioner()->getNumberOfPartitions(); j++)
		{
			temp_v.push_back(BlockStreamBase::createBlock(table_descriptor_->getProjectoin(i)->getSchema(), block_size-sizeof(unsigned)));
			if (open_flag_ == APPEND){
				tmp_tuple_count.push_back(table_descriptor_->getProjectoin(i)->getPartitioner()->getPartitionCardinality(j));
				tmp_block_num.push_back(table_descriptor_->getProjectoin(i)->getPartitioner()->getPartitionBlocks(j));
			}
			else {
				tmp_tuple_count.push_back(0);
				tmp_block_num.push_back(0);
			}
/*for testing*/			cout << "init number of partitions " << i << "\t" << j << "\t:" << table_descriptor_->getProjectoin(i)->getPartitioner()->getPartitionBlocks(j) << endl;
		}
		pj_buffer.push_back(temp_v);
		blocks_per_partition.push_back(tmp_block_num);
		tuples_per_partition.push_back(tmp_tuple_count);
	}
	///////////////////////////////////////////////////////////////////////////////////////////
}

HdfsLoader::HdfsLoader(const char c_separator,const char r_separator, std::vector<std::string> file_name, TableDescriptor* tableDescriptor, open_flag open_flag_)
:table_descriptor_(tableDescriptor), col_separator(c_separator), row_separator(r_separator), file_list(file_name), open_flag_(open_flag_), block_size(64*1024)
{
	if (open_flag_ == APPEND)
		row_id = table_descriptor_->getRowNumber();
	else
		row_id = 0;

	table_schema = table_descriptor_->getSchema();
	vector <unsigned> prj_index;
	for(int i = 0; i < table_descriptor_->getNumberOfProjection(); i++)
	{
		projection_schema.push_back(table_descriptor_->getProjectoin(i)->getSchema());
		vector<string> prj_writepath;
		prj_writepath.clear();
		for(int j=0; j<table_descriptor_->getProjectoin(i)->getPartitioner()->getNumberOfPartitions();j++){
			string path = PartitionID(table_descriptor_->getProjectoin(i)->getProjectionID(),j).getPathAndName();
			prj_writepath.push_back(path);
		}
		writepath.push_back(prj_writepath);


		Attribute partition_attribute = table_descriptor_->getProjectoin(i)->getPartitioner()->getPartitionKey();
		int hash_key_index=table_descriptor_->getProjectoin(i)->getAttributeIndex(partition_attribute);
		partition_key_index.push_back(hash_key_index);

		PartitionFunction* pf = table_descriptor_->getProjectoin(i)->getPartitioner()->getPartitionFunction();
		partition_functin_list_.push_back(pf);

		prj_index.clear();
		for (int j = 0; j < table_descriptor_->getProjectoin(i)->getAttributeList().size(); j++)
		{
			prj_index.push_back(table_descriptor_->getProjectoin(i)->getAttributeList()[j].index);
		}
		SubTuple* st = new SubTuple(table_descriptor_->getSchema(), table_descriptor_->getProjectoin(i)->getSchema(), prj_index);
		sub_tuple_generator.push_back(st);
	}

	sblock = new Block(BLOCK_SIZE);


	///////////////////////////////////assign 64k buffer to each PJ/////////////////////////////
	for(int i = 0; i < table_descriptor_->getNumberOfProjection(); i++)
	{
		vector<BlockStreamBase*> temp_v;
		vector<unsigned long> tmp_block_num;
		vector<unsigned long> tmp_tuple_count;
		for(int j = 0; j < table_descriptor_->getProjectoin(i)->getPartitioner()->getNumberOfPartitions(); j++)
		{
			temp_v.push_back(BlockStreamBase::createBlock(table_descriptor_->getProjectoin(i)->getSchema(), block_size-sizeof(unsigned)));
			if (open_flag_ == APPEND) {
				tmp_tuple_count.push_back(table_descriptor_->getProjectoin(i)->getPartitioner()->getPartitionCardinality(j));
				tmp_block_num.push_back(table_descriptor_->getProjectoin(i)->getPartitioner()->getPartitionBlocks(j));
			}
			else {
				tmp_tuple_count.push_back(0);
				tmp_block_num.push_back(0);
			}
			//ERROR: the init table_descriptor_->getProjectoin(i)->getPartitioner()->getPartitionChunks(j) is wrong!!!
/*for testing*/			cout << "init number of partitions " << i << "\t" << j << "\t:" << table_descriptor_->getProjectoin(i)->getPartitioner()->getPartitionBlocks(j) << endl;
		}
		pj_buffer.push_back(temp_v);
		blocks_per_partition.push_back(tmp_block_num);
		tuples_per_partition.push_back(tmp_tuple_count);
	}
	///////////////////////////////////////////////////////////////////////////////////////////
}

const char HdfsLoader::get_c_separator(){
	return col_separator;
}
const char HdfsLoader::get_r_separator(){
	return row_separator;
}
vector<string> HdfsLoader::get_file_list(){
	return file_list;
}


bool HdfsLoader::insertRecords(){
	//transfer the string tuple to void* according to its schema <tuple_buffer>
	if (s_record.length() == 0)
	{
		cout << "The record is null! The row id is: "<< row_id << "\n";
//		assert(false);
		return false;
	}
	void *tuple_buffer = malloc(table_schema->getTupleMaxSize());

	//add the row_id column
	column_type* tmp = new column_type(t_u_long);
	std::string tmp_str = tmp->operate->toString(&row_id);
	delete tmp;
	s_record = tmp_str + col_separator + s_record;
	table_schema->toValue(s_record, tuple_buffer, col_separator);
	row_id++;

///*for testing begin*/
//	cout << "text : ";
//	cout << s_record << endl;
//	cout << "tuple: ";
//	table_descriptor_->getSchema()->displayTuple(tuple_buffer, " | ");
///*for testing end*/

	for (int i = 0; i < table_descriptor_->getNumberOfProjection(); i++)
	{
		//extract the sub tuple according to the projection schema  <target>
		void* target = malloc(projection_schema[i]->getTupleMaxSize());
		sub_tuple_generator[i]->getSubTuple(tuple_buffer, target);

		//determine the partition to write the tuple "target"
		int part = 0;
		const int partition_key_local_index=partition_key_index[i];
		void* partition_key_addr=projection_schema[i]->getColumnAddess(partition_key_local_index,target);
		part=projection_schema[i]->getcolumn(partition_key_local_index).operate->getPartitionValue(partition_key_addr, partition_functin_list_[i]->getNumberOfPartitions());
		tuples_per_partition[i][part]++;
		void* block_tuple_addr = pj_buffer[i][part]->allocateTuple(projection_schema[i]->getTupleMaxSize());
		if(block_tuple_addr == 0)
		{

#ifdef HDFS_LOAD
			pj_buffer[i][part]->serialize(*sblock);
//			tSize num_written_every64k_bytes = hdfsWrite(connector_->get_fs(), connector_->get_writefile()[i][part], (void*)sblock->getBlock(), sblock->getsize());
			tSize num_written_every64k_bytes = connector_->flush(i, part, sblock->getBlock(), sblock->getsize());
#endif

			cout << row_id << "\t64KB has been written to HDFS!\n";

			blocks_per_partition[i][part]++;
			pj_buffer[i][part]->setEmpty();
			block_tuple_addr = pj_buffer[i][part]->allocateTuple(projection_schema[i]->getTupleMaxSize());
		}
		projection_schema[i]->copyTuple(target,block_tuple_addr);

		free(target);
	}
	free(tuple_buffer);
	return true;
}

bool HdfsLoader::load(){
#ifdef HDFS_LOAD
	if(Config::local_disk_mode) {
		connector_ = new LocalDiskConnector(writepath);
	}
	else {
		connector_ = new HdfsConnector(writepath);
	}
	connector_->openFiles(open_flag_);
#endif

	if (open_flag_ == CREATE)
		cout << "\n\n\n--------------------------Load Begin!--------------------------\n";
	else
		cout << "\n\n\n--------------------------Append Begin!--------------------------\n";

	vector<string>::iterator iter;
///*for testing*/	int t_count = 1;
	for (iter = file_list.begin(); iter != file_list.end(); iter++)
	{
		ifstream InFile((*iter).c_str());
		if(!InFile.good()){
			printf("Cannot open source file:%s , reason: %s\n",(*iter).c_str(),strerror(errno));
			return false;
		}
		while(!InFile.eof()/* for testing && t_count++ < 500 */)
		{
			s_record.clear();
			getline(InFile,s_record,row_separator);

///*for testing*/ if(row_id/t_count > 100000)
///*for testing*/		break;

			insertRecords();
		}
		InFile.close();
///*for testing*/	t_count++;
	}

	cout << "----------------------------"<<endl;

	//flush the last block which is not full of 64*1024Byte
	for(int i = 0; i < table_descriptor_->getNumberOfProjection(); i++)
	{
		for(int j = 0; j < table_descriptor_->getProjectoin(i)->getPartitioner()->getNumberOfPartitions(); j++)
		{
			if(!pj_buffer[i][j]->Empty())
			{
				pj_buffer[i][j]->serialize(*sblock);

#ifdef HDFS_LOAD
				tSize num_written_every64k_bytes = connector_->flush(i, j, sblock->getBlock(), sblock->getsize());
				//hdfsWrite(connector_->get_fs(), connector_->get_writefile()[i][j], (void*)sblock->getBlock(), sblock->getsize());
#endif
				cout << row_id << "\tThe last block has written to HDFS!\n";

				blocks_per_partition[i][j]++;
				pj_buffer[i][j]->setEmpty();
			}
		}

	}

///*for testing begin*/
//	for(int i = 0; i < table_descriptor_->getNumberOfProjection(); i++)
//	{
//		for(int j = 0; j < table_descriptor_->getProjectoin(i)->getPartitioner()->getNumberOfPartitions(); j++)
//		{
//			cout << blocks_per_partition[i][j] << " ";
//		}
//		cout << endl;
//	}
///*for testing end*/

#ifdef HDFS_LOAD
	if(connector_->closeFiles() == false)
	{
		cout << "Failed to close HDFS." << endl;
		exit(-1);
	}else
 		cout << "HDFS close successfully." << endl;
#endif

	//register the number of rows in table to catalog
	table_descriptor_->setRowNumber(row_id);
	//register the partition information to catalog
	for(int i = 0; i < table_descriptor_->getNumberOfProjection(); i++)
	{
		for(int j = 0; j < table_descriptor_->getProjectoin(i)->getPartitioner()->getNumberOfPartitions(); j++)
		{
			table_descriptor_->getProjectoin(i)->getPartitioner()->RegisterPartitionWithNumberOfBlocks(j, blocks_per_partition[i][j],tuples_per_partition[i][j]);
			if (open_flag_ == APPEND)
			{
				//TODO: update binding infos and chunk storage level by me later.
			}
			cout << "Number of blocks " << i << "\t" << j << "\t: " << blocks_per_partition[i][j] << endl;
		}
	}
	if (open_flag_ == CREATE)
		cout << "\n\n\n--------------------------Load End!--------------------------\n";
	else
		cout << "\n\n\n--------------------------Append End!--------------------------\n";
 	return true;
}

bool HdfsLoader::append(std::string tuple_string)
{
#ifdef HDFS_LOAD
	if(Config::local_disk_mode) {
		connector_ = new LocalDiskConnector(writepath);
	}
	else {
		connector_ = new HdfsConnector(writepath);
	}
	connector_->openFiles(open_flag_);
#endif

	cout << "\n\n\n--------------------------Insert Begin!--------------------------\n";

	while (tuple_string.length() != 0)
	{
		int current = 0;
		s_record.clear();
		while (current < tuple_string.length() && tuple_string[current] != '\n')
			s_record += tuple_string[current++];
		current++;
		if (current < tuple_string.length())
			tuple_string = tuple_string.substr(current, tuple_string.length()-current);
		else
			tuple_string = "\0";
		insertRecords();
	}

	//flush the last block which is not full of 64*1024Byte
	for(int i = 0; i < table_descriptor_->getNumberOfProjection(); i++)
	{
		for(int j = 0; j < table_descriptor_->getProjectoin(i)->getPartitioner()->getNumberOfPartitions(); j++)
		{
			if(!pj_buffer[i][j]->Empty())
			{
				pj_buffer[i][j]->serialize(*sblock);

#ifdef HDFS_LOAD
				tSize num_written_every64k_bytes = connector_->flush(i, j, (void*)sblock->getBlock(), sblock->getsize());
//						hdfsWrite(connector_->get_fs(), connector_->get_writefile()[i][j], (void*)sblock->getBlock(), sblock->getsize());
#endif
				cout << row_id << "The last block has written to HDFS!\n";

				blocks_per_partition[i][j]++;
				pj_buffer[i][j]->setEmpty();
			}
		}

	}

#ifdef HDFS_LOAD
	if(connector_->closeFiles() == false)
	{
		cout << "Failed to close HDFS." << endl;
		exit(-1);
	}else
 		cout << "HDFS close successfully." << endl;
#endif

	//register the number of rows in table to catalog
	table_descriptor_->setRowNumber(row_id);
	//register the partition information to catalog
	for(int i = 0; i < table_descriptor_->getNumberOfProjection(); i++)
	{
		for(int j = 0; j < table_descriptor_->getProjectoin(i)->getPartitioner()->getNumberOfPartitions(); j++)
		{
			printf("Table %d, projection %d ::Partition info:%lx\n", i, j, table_descriptor_->getProjectoin(i)->getPartitioner());
			if(table_descriptor_->getProjectoin(i)->getPartitioner()->allPartitionBound()){
				printf("[][][]After append bound!\n");
			}
			else {
				printf("[][][]After append NOT bound!\n");
			}
			table_descriptor_->getProjectoin(i)->getPartitioner()->RegisterPartitionWithNumberOfBlocks(j, blocks_per_partition[i][j],tuples_per_partition[i][j]);
			//TODO: update binding infos and chunk storage level by me later.
			table_descriptor_->getProjectoin(i)->getPartitioner()->UpdatePartitionWithNumberOfChunksToBlockManager(j, blocks_per_partition[i][j]);



			cout << "Number of blocks " << i << "\t" << j << "\t: " << blocks_per_partition[i][j] << endl;
		}
	}
	cout << "\n\n\n--------------------------Insert End!--------------------------\n";
 	return true;
}

HdfsLoader::~HdfsLoader() {
	row_id = 0;
	writepath.clear();
	file_list.clear();
	free(table_descriptor_);
	table_descriptor_ = 0;
	free(connector_);
	connector_=0;
}

