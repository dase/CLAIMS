/*
 * PartitoiinedTable.h
 *
 *  Created on: May 8, 2013
 *      Author: wangli
 */

#ifndef TABLEPARTITION_H_
#define TABLEPARTITION_H_
#include "Column_to_be_removed.h"
#include "HeapColumn.h"
#include "ColumnAppenderBase.h"
#include <vector>
#include "data_type.h"
using std::vector;



class TablePartition {
public:
	TablePartition(unsigned table_id);
	virtual ~TablePartition();
	bool createTablePartition(vector<data_type> dt_list);
	unsigned get_column_number();


	/**
	 * use for appending records to this partition.
	 */
	class Appender {
	public:
		Appender(TablePartition* table_partition);
		bool open();
		bool close();
		bool append(void* src, oid_t oid);
	private:
		vector<unsigned> offset_list;// the offset for each column
		TablePartition* table_partition;
		vector<ColumnAppenderBase*> column_appender_list;
	};
	Appender create_appender()
	{
		return Appender(this);
	}
public:
	unsigned record_count;
	unsigned table_id;
	vector<Column_TOBEREMOVED*> column_list;
};

#endif /* TABLEPARTITION_H_ */
