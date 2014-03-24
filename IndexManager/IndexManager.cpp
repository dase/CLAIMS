/*
 * IndexManager.cpp
 *
 *  Created on: 2013年12月5日
 *      Author: imdb
 */

#include "IndexManager.h"

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
		instance_ = new BufferManager();
	return instance_;
}

template <typename T>
std::map<ChunkID, CSBPlusTree<T>* > IndexManager::getIndexList()
{

}

template <typename T>
bool IndexManager::addIndexToList(chunk_index_list<T> new_index)
{

}
