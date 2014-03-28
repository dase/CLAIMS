/*
 * IndexManager.cpp
 *
 *  Created on: 2013年12月5日
 *      Author: imdb
 */

#include "IndexManager.h"
#include "../Catalog/Catalog.h"

void cheak_com_index()
{
	Attribute attr;
	IndexManager::getInstance()->getIndexList<int>(attr);
	IndexManager::getInstance()->getIndexList<int>(0);
}

IndexManager* IndexManager::instance_ = 0;
IndexManager::IndexManager() {
	// TODO Auto-generated constructor stub
	csb_index_.clear();
	attr_index_id_ = 0;
	column_attribute_to_id.clear();
	id_to_column_attribute.clear();
}

IndexManager::~IndexManager() {
	// TODO Auto-generated destructor stub
}

IndexManager* IndexManager::getInstance()
{
	if (instance_ == 0)
		instance_ = new IndexManager();
	return instance_;
}

bool IndexManager::addIndexToList(unsigned key_indexing, map<ChunkID, void* > attr_index)
{
	map<ChunkID, void* >::iterator iter_insert = attr_index.begin();
	TableID table_id = iter_insert->first.partition_id.projection_id.table_id;
	Attribute attribute = ((Catalog::getInstance()->getProjection(iter_insert->first.partition_id.projection_id))->getAttributeList())[key_indexing];

	map<unsigned long, attr_index_list*>::iterator iter = csb_index_.begin();
	for (; iter != csb_index_.end(); iter++)
	{
		if (iter->second->attribute == attribute)
			break;
	}

	if (iter == csb_index_.end())
	{	//When arrivals here, it means that the column has never be indexed, the function called should be insertIndexToList
		attr_index_list* new_attr_index = new attr_index_list(attribute);
		for (; iter_insert != attr_index.end(); iter_insert++)
			new_attr_index->csb_tree_list[iter_insert->first] = iter_insert->second;
		csb_index_[attr_index_id_] = new_attr_index;

		column_attribute_to_id[attribute] = attr_index_id_;
		id_to_column_attribute[attr_index_id_] = attribute;
		attr_index_id_++;
		return true;
	}
	else
	{
		for (; iter_insert != attr_index.end(); iter_insert++)
		{
			if (iter->second->csb_tree_list.find(iter_insert->first) == iter->second->csb_tree_list.end())
				iter->second->csb_tree_list[iter_insert->first] = iter_insert->second;
			else
			{
				cout << "[ERROR: IndexManager.cpp->addIndexToList]: The Chunk is already indexed!\n";
				return false;
			}
		}
		return true;
	}
}

bool IndexManager::insertIndexToList(std::string index_name, unsigned key_indexing, map<ChunkID, void* > attr_index)
{
	map<ChunkID, void* >::iterator iter_insert = attr_index.begin();
	TableID table_id = iter_insert->first.partition_id.projection_id.table_id;
	Attribute attribute = ((Catalog::getInstance()->getProjection(iter_insert->first.partition_id.projection_id))->getAttributeList())[key_indexing];

	//To make sure that the column hasn't be indexed by CSB+ tree
	for (map<unsigned long, attr_index_list*>::iterator iter = csb_index_.begin(); iter != csb_index_.end(); iter++)
	{
		if (iter->second->attribute == attribute)
		{
			cout << "[ERROR: IndexManager.cpp->addIndexToList]: The column " << attribute.attrName << " is already indexed!\n";
			return false;
		}
	}



	attr_index_list* new_attr_index = new attr_index_list(attribute, index_name);
	for (; iter_insert != attr_index.end(); iter_insert++)
		new_attr_index->csb_tree_list[iter_insert->first] = iter_insert->second;
	csb_index_[attr_index_id_] = new_attr_index;

	column_attribute_to_id[attribute] = attr_index_id_;
	id_to_column_attribute[attr_index_id_] = attribute;
	attr_index_id_++;
	return true;
}

template <typename T>
std::map<ChunkID, CSBPlusTree<T>* > IndexManager::getIndexList(Attribute attribute)
{
	map<ChunkID, CSBPlusTree<T>* > ret;
	ret.clear();

	map<Attribute, unsigned long >::iterator iter = column_attribute_to_id.find(attribute);
	if (iter == column_attribute_to_id.end())
		cout << "[WARNING: IndexManager.cpp->getIndexList()]: The attribute " << attribute.attrName << "hasn't be indexed by CSB+ tree!\n";
	else
	{
		unsigned long index_id = iter->second;
		map<unsigned long, attr_index_list*>::iterator iter_index = csb_index_.find(index_id);
		for (map<ChunkID, void*>::iterator iter_ = iter_index->second->csb_tree_list.begin(); iter_ != iter_index->second->csb_tree_list.end(); iter_++)
			ret[iter_->first] = (CSBPlusTree<T>*)(iter_->second);
	}
	return ret;
}

template <typename T>
std::map<ChunkID, CSBPlusTree<T>* > IndexManager::getIndexList(unsigned long attr_index_id)
{
	map<ChunkID, CSBPlusTree<T>* > ret;
	ret.clear();
	if (csb_index_.find(attr_index_id) == csb_index_.end())
		cout << "[WARNING: IndexManager.cpp->getIndexList()]: The index id " << attr_index_id << "hasn't be used to mapping a CSB+ column index!\n";
	else
	{
		map<unsigned long, attr_index_list*>::iterator iter = csb_index_.find(attr_index_id);
		for (map<ChunkID, void*>::iterator iter_ = iter->second->csb_tree_list.begin(); iter_ != iter->second->csb_tree_list.end(); iter_++)
			ret[iter_->first] = (CSBPlusTree<T>*)(iter_->second);
	}
	return ret;
}

std::map<ChunkID, void* > IndexManager::getAttrIndex(unsigned long attr_index_id)
{
	map<ChunkID, void* > ret;
	ret.clear();
	if (csb_index_.find(attr_index_id) == csb_index_.end())
		cout << "[WARNING: IndexManager.cpp->getIndexList()]: The index id " << attr_index_id << "hasn't be used to mapping a CSB+ column index!\n";
	else
	{
		map<unsigned long, attr_index_list*>::iterator iter = csb_index_.find(attr_index_id);
		for (map<ChunkID, void*>::iterator iter_ = iter->second->csb_tree_list.begin(); iter_ != iter->second->csb_tree_list.end(); iter_++)
			ret[iter_->first] = (iter_->second);
	}
	return ret;
}

bool IndexManager::deleteIndexFromList(unsigned long index_id)
{
	if (csb_index_.find(index_id) == csb_index_.end())
		return true;
	else
	{
		/*
		 * here need to delete the index structure
		 */
		csb_index_.erase(index_id);
	}
}

data_type IndexManager::getIndexType(unsigned long index_id)
{
	if (id_to_column_attribute.find(index_id) == id_to_column_attribute.end())
	{
		cout << "[ERROR: IndexManager.cpp->getIndexType()]: The index id " << index_id << "hasn't be used to mapping a CSB+ column index!\n";
		exit(-1);
	}
	return id_to_column_attribute.find(index_id)->second.attrType->type;
}
