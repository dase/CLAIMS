/*
 * test_indexing.cpp
 *
 *  Created on: Mar 11, 2014
 *      Author: scdong
 */

#include "../CSBPlusTree.h"

#include <iostream>
#include "../CSBPlusTree.h"
using namespace std;

static int test_csb_indexing()
{
	const unsigned data_size = 101;
	data_offset<int>* data = new data_offset<int> [data_size];
	for (unsigned i = 0; i < data_size; i++)
	{
		data[i]._key = i;
		data[i]._block_off = 1;
		data[i]._tuple_off = i;
	}

	CSBPlusTree<int>* csb_tree = new CSBPlusTree<int>();

//For testing bulkload
	csb_tree->BulkLoad(data, data_size);
//	csb_tree->printDoubleLinkedList();
	csb_tree->printTree();
	cout << "***********************************************************************\n";
	cout << "************************** Test Serialize *****************************\n";
	cout << "***********************************************************************\n";
	FILE* filename = fopen("test.dat", "wb+");
	csb_tree->serialize(filename);
	fclose(filename);

	CSBPlusTree<int>* csb_tree_ = new CSBPlusTree<int>();
	filename = fopen("test.dat", "r");
	csb_tree_->deserialize(filename);
	fclose(filename);
	csb_tree_->printTree();
	csb_tree_->printDoubleLinkedList();


//For testing search
	map<index_offset, vector<index_offset> > ret;
//	ret.clear();
//	ret = csb_tree->Search(5);
//	if (ret.size() != 0)
//	{
//		for (map<index_offset, vector<index_offset> >::iterator iter = ret.begin(); iter != ret.end(); iter++)
//			for (vector<index_offset>::iterator iter_ = iter->second.begin(); iter_ != iter->second.end(); iter_++)
//				cout << "<" << iter->first << ", " << *iter_ << ">\t";
//	}
//	else
//		cout << "No record searched!\n";
//	cout << endl;


//For testing range query
	cout << "\n\n************************** Test Serialize *****************************\n";
	ret.clear();
	ret = csb_tree_->rangeQuery(5, 10);
	if (ret.size() != 0)
	{
		for (map<index_offset, vector<index_offset> >::iterator iter = ret.begin(); iter != ret.end(); iter++)
			for (vector<index_offset>::iterator iter_ = iter->second.begin(); iter_ != iter->second.end(); iter_++)
				cout << "<" << iter->first << ", " << *iter_ << ">\t";
	}
	else
		cout << "No record searched!\n";
	cout << endl;

	cout << endl << "!!!Hello World!!!" << endl; // prints !!!Hello World!!!
	return 0;
}
