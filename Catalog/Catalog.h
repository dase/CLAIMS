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

#include "unordered_map.hpp"



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

	friend class boost::serialization::access;
	template<class Archive>
	void serialize(Archive &ar, const unsigned int file_version)
	{
		ar & table_id_curosr;
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

	void saveCatalog(const char* filename = "catalogData.dat");	// 2014-3-20---save as a fileby Yu
	void restoreCatalog(const char* filename = "catalogData.dat");	// 2014-3-20---restore from a file---by Yu
	void saveCatalog(Catalog &catalog_, const char* filename = "catalogData.dat");	// 2014-3-20---save as a fileby Yu
	void restoreCatalog(Catalog &catalog_, const char* filename = "catalogData.dat");	// 2014-3-20---restore from a file---by Yu
	void outPut();


	/* whether given attribute specified by table_name and attribute_name exists*/
	bool isAttributeExist(const std::string& table_name,const std::string& attribute_name )const;

private:
	Catalog();
	TableIDAllocator table_id_allocator;
	boost::unordered_map<std::string,TableDescriptor*> name_to_table;
	boost::unordered_map<TableID,TableDescriptor*> tableid_to_table;

	Logging* logging;
	ProjectionBinding* binding_;
	static Catalog* instance_;

	// 2014-3-20---add serialize function---by Yu
	friend class boost::serialization::access;
	template<class Archive>
	void serialize(Archive &ar, const unsigned int version)
	{
		ar & table_id_allocator & name_to_table & tableid_to_table;
	}

};


#endif /* CATALOG_H_ */
