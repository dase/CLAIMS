/*
 * TableManager.cpp
 *
 *  Created on: May 9, 2013
 *      Author: wangli
 */

#include "TableManager.h"

TableManager::TableManager() {
	// TODO Auto-generated constructor stub

}

TableManager::~TableManager() {
	// TODO Auto-generated destructor stub
}

bool TableManager::create_table_partition(unsigned table_id,vector<data_type> list)
{
	if(table_partition_list.find(table_id)!=table_partition_list.end())
	{
		printf("Table(%d) has already existed!\n",table_id);
		return false;
	}

	TablePartition *tp =new TablePartition(table_id);
	if(tp->createTablePartition(list)==false)
		printf("Cannot create file for Table(%d)",table_id);
	table_partition_list[table_id]=tp;

	return true;
}

bool TableManager::append(unsigned table_id, void* src, oid_t oid)
{
	if(table_partition_list.find(table_id)==table_partition_list.end())
	{
		printf("Table(%d) does not exist!\n", table_id);
		return false;
	}

	TablePartition::Appender ta= table_partition_list[table_id]->create_appender();
	ta.open();
	bool ret=ta.append(src,oid);
	ta.close();

	return ret;
}

