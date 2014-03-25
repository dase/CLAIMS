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

struct chunk_index_list
{
	chunk_index_list(Attribute attr) { attribute = attr; csb_tree_list.clear(); }
	Attribute attribute;
	std::map<ChunkID, void* > csb_tree_list;
//	ChunkID chunk_id;
//	CSBPlusTree<T>* csb_tree;
};

class IndexManager {
public:
	static IndexManager* getInstance();
	virtual ~IndexManager();

	template <typename T>
	std::map<ChunkID, CSBPlusTree<T>* > getIndexList(Attribute attribute);

	bool addIndexToList(unsigned key_indexing, map<ChunkID, void* > attr_index_list);

private:
	IndexManager();

private:
//	template<typename T>
//	map<Attribute, chunk_index_list<T> > csb_index_;
	vector<chunk_index_list> csb_index_;

	static IndexManager* instance_;
};

#endif /* INDEXMANAGER_H_ */
