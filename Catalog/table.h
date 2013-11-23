/*
 * table.h
 *
 *  Created on: 2013-9-22
 *      Author: liyongfeng
 *      Modified by wangli on 2013-10-12
 */

#ifndef TABLE_H_
#define TABLE_H_

#include <vector>
#include <set>
#include <string>
#include <map>
#include "../data_type.h"
#include "Partitioner.h"
#include "Attribute.h"
#include "../ids.h"
#include "Column.h"

using namespace std;



//typedef struct{ls
//	TableID table_id;
//	size_t local_projection_id;
//} ProjectionID;




#define TEST

/**
 * Attribute describes a column in type, name, position in the table.
 */

/**
 * A projection is a combination of columns that belong to a single table and will be horizontally
 * partitioned among storage instances.
 * ProjectionDescriptor mainly contains two kinds of important information:
 *
 * 1). how many attributes there are in a projection and what are they.
 *
 * 2). how the projection is partitioned (e.g., hash partition, range partition). How many storage
 * 		instances are involved.
 *
 */
class ProjectionDescriptor
{
public:
	friend class TableDescriptor;
	ProjectionDescriptor(ProjectionID);
	ProjectionDescriptor(const string& name);
	virtual ~ProjectionDescriptor();
	void addAttribute(Attribute attr);
	bool hasAttribute(const Attribute &attr)const;
	void DefinePartitonier(const unsigned& number_of_partitions,const Attribute &partition_key,PartitionFunction* partition_functin);
	Partitioner* getPartitioner()const;
	bool isExist(const string& name) const;
	inline void setProjectionID(const ProjectionID& pid) {projection_id_ = pid;}
	inline map<string, set<string> > getFileLocations() const {return fileLocations;}
	inline ProjectionID getProjectionID()const{return projection_id_;}
	bool AllPartitionBound()const;
	std::vector<Attribute> getAttributeList()const;
private:
//	ProjectionOffset projection_offset_;
	ProjectionID projection_id_;
	vector<Column> column_list_;

	Partitioner* partitioner;


	/* The following is considered to be deleted, as the catalog module only has a logically view
	 * of a table rather than detailed physical view such as filename, etc.
	 */
	map<string, set<string> > fileLocations;
	string hdfsFilePath;
	map<string, string> blkMemoryLocations;

	/* The following is deleted from version 1.2*/
	string Projection_name_;	//projection does not need a string name.
};


class TableDescriptor {

public:
	TableDescriptor(const string& name, const TableID table_id);
	virtual ~TableDescriptor();

	void addAttribute(Attribute attr);
	bool addAttribute(string attname,data_type dt,unsigned max_length=0);

//	void addProjection(vector<ColumnOffset> id_list);
	bool createHashPartitionedProjection(vector<ColumnOffset> column_list,ColumnOffset partition_key_index,unsigned number_of_partitions);
	bool createHashPartitionedProjection(vector<ColumnOffset> column_list,std::string partition_attribute_name,unsigned number_of_partitions);
	bool isExist(const string& name) const;
	inline string getTableName() const {return tableName;}
	ColumnOffset getColumnID(const string& attrName) const;
	map<string, set<string> > getColumnLocations(const string& attrName) const;

	vector<Attribute> getAttributes()const{
		return attributes;
	}
	vector<Attribute> getAttributes(vector<unsigned> index_list)const{
		vector<Attribute> attribute_list;
		for(unsigned i=0;i<index_list.size();i++){
			assert(index_list[i]<attributes.size());
			attribute_list.push_back(attributes[index_list[i]]);
		}
		return attribute_list;
	}
	Attribute getAttribute(unsigned index)const{
		assert(index<attributes.size());
		return attributes[index];
	}
	Attribute getAttribute(const std::string& name)const{
		for(unsigned i=0;i<attributes.size();i++){
			if(attributes[i].attrName==name)
				return attributes[i];
		}
		printf("The attribute name [%s] does not match any attribute!\n",name.c_str());
		assert(false);
	}
	/* the following methods are considered to be deleted.*/
	void addColumn(ProjectionDescriptor* column);
	inline string get_table_name()const{return tableName;}
	inline TableID get_table_id()const{return table_id_;}
	ProjectionDescriptor* getProjectoin(ProjectionOffset) const;
	unsigned getNumberOfProjection()const;
protected:
	string tableName;
	vector<Attribute> attributes;
	TableID table_id_;
	vector<ProjectionDescriptor*> projection_list_;
	// delete for debugging
//	hashmap<ColumnID, ColumnDescriptor*> columns;

};






#endif /* TABLE_H_ */
