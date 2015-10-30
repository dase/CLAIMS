/*
 * Catalog.cpp
 *
 *  Created on: Oct 31, 2013
 *      Author: wangli
 */

#include "Catalog.h"
#include "../Config.h"
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
		/*
		 * bug:if name is duplicate, table can't be added successfully with table_id_cursor increased
		 * fix:add to eliminate effect on table id;	-- yu, 2015-2-8
		 */
		table_id_allocator.decrease_table_id();
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

bool Catalog::drop_table(const std::string table_name, const TableID id)
{
    bool isdropped = false;
    bool isnamedrop = false;
    bool istableIDdrop =false;

    TableDescriptor *table_desc = NULL;
    
    if(table_name != "")
    {
        table_desc = getTable(table_name);
        if(1 == name_to_table.erase(table_name))
        {
            isnamedrop = true;
        }
        else
        {
            cout << "falied to drop table by name" << endl;
        }
    }
    if(id != -1)
    {
        table_desc = getTable(id);
        if(1 == tableid_to_table.erase(id))
        {
            istableIDdrop = true;
        }
        else
        {
            cout << "falied to drop table by tableID" << endl;
        }
    }

    if(isnamedrop&&istableIDdrop)
    {
        //table_id_allocator.decrease_table_id();
        isdropped  =true;
    }
    else
    {
        if(!isnamedrop)
        {
            tableid_to_table[id] = table_desc;
        }
        if(!istableIDdrop)
        {
            name_to_table[table_name] = table_desc;
        }
    }
    
    return isdropped;
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

/*
    Every table will follow one _DEL table strictly.
    We just do show tables :
        Get table name end with _DEL and check the name eliminate suffix _DEL in map name_to_table
        if it is, not shadow table confirmed.
*/
void Catalog::getTables(ostringstream &ostr) const{
    
    for ( auto it = name_to_table.begin(); it != name_to_table.end(); ++it )
    {
        string tbname = it->first;
        int len = tbname.length();
        if(len <= 4) continue;
        if(tbname.substr(len-4, len) == "_DEL" )
        {
            tbname = tbname.substr(0, len-4);
            if(name_to_table.find(tbname)!=name_to_table.cend())
            {
                ostr << tbname << endl;
            }
        }
    }

}


int Catalog::getTableSize() const{
    return name_to_table.size();
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
void Catalog::saveCatalog()
{
	std::ofstream ofs(Config::catalog_file.c_str());
	boost::archive::text_oarchive oa(ofs);

	oa<<*this;
}

// 2014-3-20---restore from a file---by Yu
void Catalog::restoreCatalog()
{
	if(access(Config::catalog_file.c_str(),0) != 0)
	{
		logging->elog("the file %s is not existed!", Config::catalog_file.c_str());
	}
//	assert(access(filename, 0) == 0);
	else
	{
		std::ifstream ifs(Config::catalog_file.c_str());
		boost::archive::text_iarchive ia(ifs);
		ia>>*this;
	}

//	cout<<"================================================================"<<endl<<endl;
//	outPut();
//	cout<<"================================================================"<<endl<<endl;
}


