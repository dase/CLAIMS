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
}

IndexManager* IndexManager::instance_ = 0;
IndexManager::IndexManager() {
	// TODO Auto-generated constructor stub
	csb_index_.clear();
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

template <typename T>
std::map<ChunkID, CSBPlusTree<T>* > IndexManager::getIndexList(Attribute attribute)
{
	map<ChunkID, CSBPlusTree<T>* > ret;
	ret.clear();
	vector<chunk_index_list>::iterator iter;
	for (vector<chunk_index_list>::iterator iter = csb_index_.begin(); iter != csb_index_.end(); iter++)
	{
		if (iter->attribute == attribute)
		{
			for (map<ChunkID, void*>::iterator iter_ = iter->csb_tree_list.begin(); iter_ != iter->csb_tree_list.end(); iter_++)
				ret[iter_->first] = (CSBPlusTree<T>*)(iter_->second);
			return ret;
		}
//			break;
	}
	if (iter == csb_index_.end())
		return ret;
//	else
//	{
//		for (map<ChunkID, void*>::iterator iter_ = iter->csb_tree_list.begin(); iter_ != iter->csb_tree_list.end(); iter_++)
//			ret[iter_->first] = (CSBPlusTree<T>*)(iter_->second);
//		return ret;
//	}
}

bool IndexManager::addIndexToList(unsigned key_indexing, map<ChunkID, void* > attr_index_list)
{
	map<ChunkID, void* >::iterator iter_add = attr_index_list.begin();
	TableID table_id = iter_add->first.partition_id.projection_id.table_id;
	Attribute attribute = ((Catalog::getInstance()->getProjection(iter_add->first.partition_id.projection_id))->getAttributeList())[key_indexing];

	vector<chunk_index_list>::iterator iter = csb_index_.begin();
	for (vector<chunk_index_list>::iterator iter = csb_index_.begin(); iter != csb_index_.end(); iter++)
	{
		if (iter->attribute == attribute)
			break;
	}

	if (iter == csb_index_.end())
	{
		chunk_index_list* new_chunk_index = new chunk_index_list(attribute);
		for (; iter_add != attr_index_list.end(); iter_add++)
			new_chunk_index->csb_tree_list[iter_add->first] = iter_add->second;
		csb_index_.push_back(*new_chunk_index);
		return true;
	}
	else
	{
		for (; iter_add != attr_index_list.end(); iter_add++)
		{
			if (iter->csb_tree_list.find(iter_add->first) == iter->csb_tree_list.end())
				iter->csb_tree_list[iter_add->first] = iter_add->second;
			else
			{
				cout << "[ERROR: IndexManager.cpp->addIndexToList]: The Chunk is already indexed!\n";
				return false;
			}
		}
		return true;
	}
}
