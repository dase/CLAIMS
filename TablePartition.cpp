/*
 * PartitoiinedTable.cpp
 *
 *  Created on: May 8, 2013
 *      Author: wangli
 */

#include "TablePartition.h"

TablePartition::TablePartition(unsigned table_id)
:table_id(table_id)
{
	// TODO Auto-generated constructor stub

}

TablePartition::~TablePartition() {
	// TODO Auto-generated destructor stub
}

bool TablePartition::createTablePartition(vector<data_type> dt_list)
{
	for(unsigned i=0;i<dt_list.size();i++)
	{
		switch(dt_list[i])
		{
		case t_int:
			{
				HeapColumn<int>* tmp=new HeapColumn<int>(table_id,i);
				if(!tmp->create_empty())
					return false;
				column_list.push_back(tmp);
				break;
			}
		case t_float:
			{
				HeapColumn<float>* tmp=new HeapColumn<float>(table_id,i);
				if(!tmp->create_empty())
					return false;
				column_list.push_back(tmp);
				break;
			}
		default:
			printf("the %d-th type is unknown!",i);
		}
	}
	return true;
}


TablePartition::Appender::Appender(TablePartition * table_partition)
:table_partition(table_partition)
{

	//initialize the offset_list

	for(int i=0;i<table_partition->column_list.size();i++)
	{
		if(i==0)
		{
			offset_list.push_back(0);
		}
		else
		{
			offset_list.push_back(table_partition->column_list[i]->get_type_length()+offset_list[i-1]);
		}
		column_appender_list.push_back(table_partition->column_list[i]->create_appender());

	}

//	for(int i=0;i<table_partition->column_list.size();i++)
//		{
//		printf("offset[%d]=%d\n",i,offset_list[i]);
//		}

}
bool TablePartition::Appender::open()
{
	for(int i=0;i<table_partition->column_list.size();i++)
	{
		column_appender_list[i]->open();
	}
	return false;
}

bool TablePartition::Appender::close()
{
	for(int i=0;i<table_partition->column_list.size();i++)
	{
		column_appender_list[i]->close();
	}
	return false;
}

bool TablePartition::Appender::append(void* src, oid_t oid)
{
	for(int i=0;i<table_partition->column_list.size();i++)
		{
			column_appender_list[i]->append((char*)src+offset_list[i],oid);
		}
	table_partition->record_count++;
}

unsigned TablePartition::get_column_number()
{
	return column_list.size();
}

