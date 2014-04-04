/*
 * IndexManager.cpp
 *
 *  Created on: 2013年12月5日
 *      Author: imdb
 */

#include "IndexManager.h"
#include "../Catalog/Catalog.h"
#include <stdio.h>

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
		column_attribute_to_id.erase(id_to_column_attribute.find(index_id)->second);
		id_to_column_attribute.erase(index_id);
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

bool IndexManager::isIndexExist(Attribute attr)
{
	if (column_attribute_to_id.find(attr) != column_attribute_to_id.end())
		return true;
	return false;
}

bool IndexManager::isIndexExist(unsigned long index_id)
{
	if (csb_index_.find(index_id) != csb_index_.end())
		return true;
	return false;
}

unsigned long IndexManager::getIndexID(Attribute attr)
{
	if (isIndexExist(attr))
		return column_attribute_to_id[attr];
	cout << "[ERROR: IndexManager.cpp->getIndexID()]: The index for column " << attr.attrName << " is not exist!\n";
	return -1;
}

bool IndexManager::serialize(std::string file_name)
{
	FILE* filename = fopen(file_name.c_str(), "wb+");
	if (filename == NULL)
	{
		cout << "[ERROR: IndexManager.cpp->serialize()]: Can't open file " << file_name << ", serialization failed!\n";
		return false;
	}

	/*
	 * Serialize the IndexManager: map<unsigned long, attr_index_list*> csb_index_
	 * csb_index_.size() 				(unsigned long)
	 * for each item in csb_index_:
	 * 	 map.first: index_id_ 			(unsigned long)
	 * 	 map.second: attr_index_list*
	 * 	   index_name 					(unsigned+string)
	 * 	   Attribute->unique 			(bool)
	 * 	   Attribute->table_id_ 		(TableID unsigned)
	 * 	   Attribute->index				(unsigned)
	 * 	   Attribute->attrType			(just data_type, new operator* in deserialize)
	 * 	   Attribute->attrName			(unsigned+string)
	 * 	   map<ChunkID, void*>
	 * 	     map.size()					(unsigned long)
	 * 	     for each item in the map
	 * 	       map.first: ChunkID		(struct)
	 * 	       map.second: void*		(CSBPlusTree<T>*)
	 */
	attr_index_list* new_attr_index = new attr_index_list();
	unsigned long tmp = csb_index_.size();
	fwrite((void*)(&tmp), sizeof(unsigned long), 1, filename);
	for (unsigned long i = 0; i < attr_index_id_; i++)
	{
		if (csb_index_.find(i) != csb_index_.end())
		{
			fwrite((void*)(&i), sizeof(unsigned long), 1, filename);

			new_attr_index = csb_index_.find(i)->second;

			tmp = new_attr_index->attr_index_name.length();
			fwrite((void*)(&tmp), sizeof(unsigned long), 1, filename);
			fwrite(new_attr_index->attr_index_name.c_str(), sizeof(char), new_attr_index->attr_index_name.length(), filename);

			fwrite((void*)(&new_attr_index->attribute.unique), sizeof(bool), 1, filename);
			fwrite((void*)(&new_attr_index->attribute.table_id_), sizeof(unsigned), 1, filename);
			fwrite((void*)(&new_attr_index->attribute.index), sizeof(unsigned), 1, filename);
			fwrite((void*)(&new_attr_index->attribute.attrType->type), sizeof(data_type), 1, filename);
			tmp = new_attr_index->attribute.attrName.length();
			fwrite((void*)(&tmp), sizeof(unsigned long), 1, filename);
			fwrite((void*)new_attr_index->attribute.attrName.c_str(), sizeof(char), new_attr_index->attribute.attrName.length(), filename);

			tmp = new_attr_index->csb_tree_list.size();
			fwrite((void*)(&tmp), sizeof(unsigned long), 1, filename);
			for (map<ChunkID, void*>::iterator iter = new_attr_index->csb_tree_list.begin(); iter != new_attr_index->csb_tree_list.end(); iter++)
			{
				fwrite((void*)(&iter->first), sizeof(ChunkID), 1, filename);
				switch(new_attr_index->attribute.attrType->type)
				{
				case t_int:
					((CSBPlusTree<int>*)iter->second)->serialize(filename);
					break;
				default:
					cout << "[ERROR: IndexManager->serialize()]: The data_type of index is illegal!\n";
					return false;
				}
			}

		}
	}
	fclose(filename);
	return true;
}
