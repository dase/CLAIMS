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
	const unsigned data_size = 9;
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


//For testing search
	vector<search_result*> ret;
	ret.clear();
	ret = csb_tree->Search(5);
	if (ret.size() != 0)
	{
		for (vector<search_result*>::iterator iter = ret.begin(); iter != ret.end(); iter++)
		{
			cout << "<" << (*iter)->_block_off << ", " << (*iter)->_tuple_off << ">\t";
		}
	}
	else
		cout << "No record searched!\n";


	cout << endl << "!!!Hello World!!!" << endl; // prints !!!Hello World!!!
	return 0;
}
