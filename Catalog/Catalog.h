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
#include <boost/unordered_map.hpp>
#include "../Logging.h"
#include "ProjectionBinding.h"
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
	static Catalog* getInstance();
	virtual ~Catalog();
	unsigned allocate_unique_table_id();
	bool add_table(TableDescriptor* const &table);
	TableDescriptor* getTable(const TableID&) const;
	TableDescriptor* getTable(const std::string& table_name) const;
	ProjectionDescriptor* getProjection(const ProjectionID&) const;
	ProjectionBinding* getBindingModele()const;
private:
	Catalog();
	TableIDAllocator table_id_allocator;
	boost::unordered_map<std::string,TableDescriptor*> name_to_table;
	boost::unordered_map<TableID,TableDescriptor*> tableid_to_table;
	Logging* logging;
	ProjectionBinding* binding_;
	static Catalog* instance_;
};

#endif /* CATALOG_H_ */
