/*
 * TableManager.h
 *
 *  Created on: May 9, 2013
 *      Author: wangli
 */

#ifndef TABLEMANAGER_H_
#define TABLEMANAGER_H_

#include <map>
#include <stdio.h>
#include "TablePartition.h"

class TableManager {
public:
	TableManager();
	virtual ~TableManager();
	bool create_table_partition(unsigned table_id, vector<data_type> list);
	bool append(unsigned table_id, void* src, oid_t oid);
private:
	map<unsigned, TablePartition*> table_partition_list;
};

#endif /* TABLEMANAGER_H_ */
