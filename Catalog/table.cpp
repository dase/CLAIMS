/*
 * table.cpp
 *
 *  Created on: 2013-9-22
 *      Author: liyongfeng
 */

#include "table.h"
#include "../Schema/SchemaFix.h"
// ColumnDescriptor
ProjectionDescriptor::ProjectionDescriptor(ProjectionID pid):projection_id_(pid){
}

ProjectionDescriptor::ProjectionDescriptor(const string& name)
:Projection_name_(name){
	// Check if hdfsfile already exists
	// Meanwhile create a hsfsfile to store this column's data.
}

ProjectionDescriptor::~ProjectionDescriptor(){

}
void ProjectionDescriptor::addAttribute(Attribute attr)
{
	const ColumnID cid(projection_id_,column_list_.size());
	const Column col(attr,cid);
	column_list_.push_back(col);
}
bool ProjectionDescriptor::hasAttribute(const Attribute &attr)const{
	for(unsigned i=0;i<column_list_.size();i++){
		if(column_list_[i].index==attr.index)
			return true;
	}
	return false;
}

void ProjectionDescriptor::DefinePartitonier(const unsigned& number_of_partitions,const Attribute &partition_key,PartitionFunction* partition_functin){
	partitioner=new Partitioner(projection_id_,number_of_partitions,partition_key,partition_functin);
}
Partitioner* ProjectionDescriptor::getPartitioner()const{
	return partitioner;
}
bool ProjectionDescriptor::isExist(const string& name) const
{
	for(unsigned i=0;i<column_list_.size();i++){
		if(column_list_[i].attrName==name) return true;
	}
	return false;
}
bool ProjectionDescriptor::AllPartitionBound()const{
	return partitioner->allPartitionBound();
}
std::vector<Attribute> ProjectionDescriptor::getAttributeList()const{
	std::vector<Attribute> ret;
	for(unsigned i=0;i<this->column_list_.size();i++){
		ret.push_back((Attribute)column_list_[i]);
	}
	return ret;
}
Schema* ProjectionDescriptor::getSchema()const{
	/**
	 * Only fixed schema is supported now.
	 * TODO: support other schema.
	 */
	const vector<Attribute> attributes=getAttributeList();
	std::vector<column_type> columns;
	for(unsigned i=0;i<attributes.size();i++){
		columns.push_back(*attributes[i].attrType);
	}
	return new SchemaFix(columns);
}
int ProjectionDescriptor::getAttributeIndex(const Attribute& att)const{
	const vector<Attribute> attributes=getAttributeList();
	for(unsigned i=0;i<attributes.size();i++){
		if(attributes[i]==att){
			return i;
		}
	}
	return -1;
}

// TableDescritptor
TableDescriptor::TableDescriptor(const string& name, const TableID table_id)
: tableName(name),table_id_(table_id){
	/*
	 * Automatically add row_id as the first column.
	 */
//	addAttribute("row_id",data_type(t_u_long));
}

TableDescriptor::~TableDescriptor(){

}

void TableDescriptor::addAttribute(Attribute attr)
{
	attributes.push_back(attr);
}
bool TableDescriptor::addAttribute(string attname,data_type dt,unsigned max_length,bool unique){
	/*check for attribute rename*/
	for(unsigned i=0;i<attributes.size();i++){
		if(attributes[i].attrName==attname)
			return false;
	}
	Attribute att(table_id_,attributes.size(),attname,dt,max_length,unique);
	attributes.push_back(att);
	return true;
}

void TableDescriptor::addColumn(ProjectionDescriptor* column)
{
//	ColumnID current = (ColumnID)columns.size();
//	column->setColumnID(current);
//	columns[current] = column;
}
//void TableDescriptor::addProjection(vector<ColumnID> id_list){
//	ProjectionDescriptor* projection=new ProjectionDescriptor();
//	for(unsigned i=0;i<id_list.size();i++){
//		projection->addAttribute(attributes[id_list[i]]);
//	}
//}

bool TableDescriptor::createHashPartitionedProjection(vector<ColumnOffset> column_list,ColumnOffset partition_key_index,unsigned number_of_partitions){
	ProjectionID projection_id(table_id_,projection_list_.size());
	ProjectionDescriptor *projection=new ProjectionDescriptor(projection_id);

//	projection->projection_offset_=projection_list_.size();
	for(unsigned i=0;i<column_list.size();i++){
		projection->addAttribute(attributes[column_list[i]]);
	}

	PartitionFunction* hash_function=PartitionFunctionFactory::createGeneralModuloFunction(number_of_partitions);
//	projection->partitioner=new Partitioner(number_of_partitions,attributes[partition_key_index],hash_function);
	projection->DefinePartitonier(number_of_partitions,attributes[partition_key_index],hash_function);


	projection_list_.push_back(projection);
	return true;

}
bool TableDescriptor::createHashPartitionedProjection(vector<ColumnOffset> column_list,std::string partition_attribute_name,unsigned number_of_partitions){


	ProjectionID projection_id(table_id_,projection_list_.size());
	ProjectionDescriptor *projection=new ProjectionDescriptor(projection_id);

//	projection->projection_offset_=projection_list_.size();
//	projection->addAttribute(attributes[0]);
	for(unsigned i=0;i<column_list.size();i++){
		projection->addAttribute(attributes[column_list[i]]);
	}

	PartitionFunction* hash_function=PartitionFunctionFactory::createGeneralModuloFunction(number_of_partitions);
//	projection->partitioner=new Partitioner(number_of_partitions,attributes[partition_key_index],hash_function);
	projection->DefinePartitonier(number_of_partitions,getAttribute(partition_attribute_name),hash_function);


	projection_list_.push_back(projection);
	return true;
}
bool TableDescriptor::createHashPartitionedProjection(vector<Attribute> attribute_list,std::string partition_attribute_name,unsigned number_of_partitions){


	ProjectionID projection_id(table_id_,projection_list_.size());
	ProjectionDescriptor *projection=new ProjectionDescriptor(projection_id);

//	projection->projection_offset_=projection_list_.size();
	projection->addAttribute(attributes[0]);//add row_id
	for(unsigned i=0;i<attribute_list.size();i++){
		projection->addAttribute(attribute_list[i]);
	}

	PartitionFunction* hash_function=PartitionFunctionFactory::createGeneralModuloFunction(number_of_partitions);
//	projection->partitioner=new Partitioner(number_of_partitions,attributes[partition_key_index],hash_function);
	projection->DefinePartitonier(number_of_partitions,getAttribute(partition_attribute_name),hash_function);


	projection_list_.push_back(projection);
	return true;
}

bool TableDescriptor::isExist(const string& name) const
{
	for(unsigned i=0;i<attributes.size();i++){
		if(attributes[i].attrName==name) return true;
	}
	return false;
}

map<string, set<string> > TableDescriptor::getColumnLocations(const string& attrName) const
{
//	hashmap<string, set<string> > result;
//
//	foreachvalue(ColumnDescriptor* cd, columns)
//	{
//		if(cd->isExist(attrName))
//		{
//			result = cd->getFileLocations();
//			break;
//		}
//	}
//	return result;
}

ProjectionDescriptor* TableDescriptor::getProjectoin(ProjectionOffset pid)const{
	if(pid>=0||pid<projection_list_.size()){
		return projection_list_[pid];
	}
	return NULL;
}
unsigned TableDescriptor::getNumberOfProjection()const{
	return projection_list_.size();
}

Schema* TableDescriptor::getSchema()const
{
	const vector<Attribute> attributes=getAttributes();
	std::vector<column_type> columns;
	for(unsigned i=0;i<attributes.size();i++){
		columns.push_back(*attributes[i].attrType);
	}
	return new SchemaFix(columns);
}
