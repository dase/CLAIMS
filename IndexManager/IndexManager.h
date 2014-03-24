/*
 * IndexManager.h
 *
 *  Created on: 2013年12月5日
 *      Author: imdb
 */

#ifndef INDEXMANAGER_H_
#define INDEXMANAGER_H_
#include <map>
#include <vector>
#include "../ids.h"
#include "CSBPlusTree.h"
#include "../Catalog/Attribute.h"
#include "../data_type.h"

template <typename T>
struct chunk_index_list
{
	Attribute attribute;
	std::map<ChunkID, CSBPlusTree<T>* > csb_tree_list;
//	ChunkID chunk_id;
//	CSBPlusTree<T>* csb_tree;
};

class IndexManager {
public:
	static IndexManager* getInstance();
	virtual ~IndexManager();

	template <typename T>
	std::map<ChunkID, CSBPlusTree<T>* > getIndexList();

	template <typename T>
	bool addIndexToList(chunk_index_list<T> new_index);

private:
	IndexManager();

private:
//	template<typename T>
//	map<Attribute, chunk_index_list<T> > csb_index_;
	template<typename T>
	vector<chunk_index_list<T> > csb_index_;

	static IndexManager* instance_;
};

#endif /* INDEXMANAGER_H_ */
