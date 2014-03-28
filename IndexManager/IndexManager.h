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

struct attr_index_list
{
	attr_index_list(Attribute attr) :attribute(attr), attr_index_name("\0") { csb_tree_list.clear(); }
	attr_index_list(Attribute attr, std::string index_name) :attribute(attr), attr_index_name(index_name) { csb_tree_list.clear(); }
	Attribute attribute;
	std::string attr_index_name;
	std::map<ChunkID, void* > csb_tree_list;
};

class IndexManager {
public:
	static IndexManager* getInstance();
	virtual ~IndexManager();

	bool addIndexToList(unsigned key_indexing, map<ChunkID, void* > attr_index);
	bool insertIndexToList(std::string index_name, unsigned key_indexing, map<ChunkID, void* > attr_index);

	template <typename T>
	std::map<ChunkID, CSBPlusTree<T>* > getIndexList(Attribute attribute);
	template <typename T>
	std::map<ChunkID, CSBPlusTree<T>* > getIndexList(unsigned long attr_index_id);

	std::map<ChunkID, void* > getAttrIndex(unsigned long attr_index_id);

	bool deleteIndexFromList(unsigned long index_id);

	data_type getIndexType(unsigned long index_id);

private:
	IndexManager();

private:
//	vector<attr_index_list> csb_index_;
	map<unsigned long, attr_index_list*> csb_index_;

	unsigned long attr_index_id_;
//	map<unsigned long, attr_index_list*> id_to_index_list;
	map<Attribute, unsigned long > column_attribute_to_id;
	map<unsigned long, Attribute> id_to_column_attribute;


	static IndexManager* instance_;
};

#endif /* INDEXMANAGER_H_ */
