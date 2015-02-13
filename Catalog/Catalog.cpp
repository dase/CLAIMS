/*
 * Catalog.cpp
 *
 *  Created on: Oct 31, 2013
 *      Author: wangli
 */

#include "Catalog.h"
Catalog* Catalog::instance_=0;

Catalog::Catalog() {
	logging=new CatalogLogging();
	binding_=new ProjectionBinding();

//	restoreCatalog();
}

Catalog::~Catalog() {
	for( boost::unordered_map<std::string,TableDescriptor*>::iterator it = name_to_table.begin();it!=name_to_table.end();it++){
		delete it->second;
	}
	instance_=0;
	delete logging;
	delete binding_;
}
Catalog* Catalog::getInstance(){
	if(instance_==0){
		instance_=new Catalog();
	}
	return instance_;
}
unsigned Catalog::allocate_unique_table_id(){
	return table_id_allocator.allocate_unique_table_id();
}
bool Catalog::add_table(TableDescriptor* const &table){
	std::string new_table_name=table->getTableName();
	TableID new_table_id=table->get_table_id();
	boost::unordered_map<std::string,TableDescriptor*>::iterator it_name_to_table=name_to_table.find(new_table_name);
	if(it_name_to_table!=name_to_table.cend()){
		logging->elog("the table named %s is existed!", new_table_name.c_str());
		return false;
	}
	boost::unordered_map<TableID,TableDescriptor*>::iterator it_tableid_to_table=tableid_to_table.find(new_table_id);

	if(it_tableid_to_table!=tableid_to_table.cend()){
		logging->elog("the table whose id is %d is existed!", new_table_id);
		return false;
	}
	/*The check is successful. Now we can add the new table into the catalog module.*/
	name_to_table[new_table_name]=table;
	tableid_to_table[new_table_id]=table;
	logging->log("New table \"%s\",id=%d is created!",new_table_name.c_str(),new_table_id);

	return true;
}
TableDescriptor* Catalog::getTable(const TableID &target)const{
	if(tableid_to_table.find(target)==tableid_to_table.cend())
		return NULL;

	/* at could retain const while [] doesn't.*/
	return tableid_to_table.at(target);

}
TableDescriptor* Catalog::getTable(const std::string& table_name) const{
	if(name_to_table.find(table_name)==name_to_table.cend())
		return NULL;

	/* at could retain const while [] doesn't.*/
	return name_to_table.at(table_name);
}
ProjectionDescriptor* Catalog::getProjection(const ProjectionID& pid) const{
	const TableDescriptor* td=getTable(pid.table_id);
	return td==0?0:td->getProjectoin(pid.projection_off);
}
ProjectionBinding* Catalog::getBindingModele()const{
	return binding_;
}

bool Catalog::isAttributeExist(const std::string& table_name,const std::string& attribute_name )const{
	TableDescriptor* td=getTable(table_name);
	if(td==0){
		return false;
	}
	else
		return td->isExist(attribute_name);
}


vector<PartitionID> Catalog::getPartitionIDList(const std::string& table_name, const std::string& attribute_name)
{
	vector<PartitionID> ret;
	ret.clear();
	TableDescriptor* table_descripter = this->getTable(table_name);
	ProjectionDescriptor* projection_descripter = NULL;
	unsigned projection_num = table_descripter->getNumberOfProjection();
	for (unsigned i = 0; i < projection_num; i++)
	{
		projection_descripter = table_descripter->getProjectoin(i);
		if (projection_descripter->isExist(attribute_name))
			break;
	}
	return projection_descripter->getPartitioner()->getPartitionIDList();
}

//void Catalog::outPut()
//{
//	boost::unordered_map<TableID,TableDescriptor*>::iterator it_tableid_to_table;//=tableid_to_table.find(new_table_id);
//	for(it_tableid_to_table = tableid_to_table.begin(); it_tableid_to_table != tableid_to_table.end(); ++it_tableid_to_table)
//	{
//		cout<<it_tableid_to_table->first<<"  "<<it_tableid_to_table->second->getTableName()<<"  "<<it_tableid_to_table->second->get_table_id()<<"  ";
//		cout<<it_tableid_to_table->second->getNumberOfProjection();
//	}
//	cout<<endl;
//}

// 2014-3-20---save as a file---by Yu
void Catalog::saveCatalog(const char *filename)
{
	std::ofstream ofs(filename);
	boost::archive::text_oarchive oa(ofs);

	oa<<*this;
}

// 2014-3-20---restore from a file---by Yu
void Catalog::restoreCatalog(const char *filename)
{
	if(access(filename,0) != 0)
	{
		logging->elog("the file %s is not existed!", filename);
	}
//	assert(access(filename, 0) == 0);
	else
	{
		std::ifstream ifs(filename);
		boost::archive::text_iarchive ia(ifs);
		ia>>*this;
	}

//	cout<<"================================================================"<<endl<<endl;
//	outPut();
//	cout<<"================================================================"<<endl<<endl;
}

void Catalog::saveCatalog(Catalog &catalog_, const char *filename)
{
	std::ofstream ofs(filename);
	boost::archive::text_oarchive oa(ofs);
	oa<<catalog_;
	ofs.close();
}

// 2014-3-20---restore from a file---by Yu
void Catalog::restoreCatalog(Catalog &catalog_, const char *filename)
{
	//	if(access(filename,0) != 0)
	//		logging->elog("the file %s is not existed!", filename);
	assert(access(filename, 0) == 0);

	std::ifstream ifs(filename);
	boost::archive::text_iarchive ia(ifs);
	ia>>catalog_;
	ifs.close();
	printf("<><><><><><> restore\n");
//	cout<<"================================================================"<<endl<<endl;
//	outPut();
//	cout<<"================================================================"<<endl<<endl;
}

