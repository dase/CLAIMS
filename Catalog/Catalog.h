/*
 * Catalog.h
 *
 *  Created on: Oct 31, 2013
 *      Author: wangli
 */

#ifndef CATALOG_H_
#define CATALOG_H_
#include <string>
#include "table.h"
#include "../hashmap.hpp"
#include "../Logging.h"
struct TableIDAllocator{
	TableIDAllocator(){
		table_id_curosr=0;
	}
	unsigned table_id_curosr;
	unsigned allocate_unique_table_id(){
		return table_id_curosr++;
	}
};

class Catalog {

public:
	Catalog();
	virtual ~Catalog();
	unsigned allocate_unique_table_id();
	bool add_table(TableDescriptor* const &table);
	TableDescriptor* getTable(const TableOffset&) const;
private:
	TableIDAllocator table_id_allocator;
	hashmap<std::string,TableDescriptor*> name_to_table;
	hashmap<TableOffset,TableDescriptor*> tableid_to_table;
	Logging* logging;
};

#endif /* CATALOG_H_ */
