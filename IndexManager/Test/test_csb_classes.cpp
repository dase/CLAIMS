/*
 * test_csb_classes.cpp
 *
 *  Created on: Mar 14, 2014
 *      Author: scdong
 */

#include <iostream>
#include "../CSBPlusTree.h"
using namespace std;

static int test_csb_classes ()
{
	const unsigned data_size = 10;
	data_offset<int>* data = new data_offset<int> [data_size];
	for (unsigned i = 0; i < data_size; i++)
	{
		data[i]._key = i;
		data[i]._block_off = 1;
		data[i]._tuple_off = i;
	}

//For testing begin: print original data set
	for (unsigned i = 0; i < data_size; i++)
		cout << i << ": " << "data: " << data[i]._key << " block off: " << data[i]._block_off << " tuple off: " << data[i]._tuple_off << endl;
	cout << endl;
//For testing end

	CCSBLeafNodeGroup<int>* leaf = new CCSBLeafNodeGroup<int> (1);
	for (unsigned i = 0; i < data_size; i++)
		leaf->getNode(0)->setElement(i, data[i]);
	leaf->leaf_nodes[0]->used_keys = data_size;

//For testing begin: print the leaf node group
	for (unsigned i = 0; i < leaf->used_nodes; i++)
	{
		cout << "Node group: " << i << ": ";
		cout << "used nodes: " << leaf[i].used_nodes << endl;
		for (unsigned j = 0; j < leaf[i].used_nodes; j++)
		{
			cout << "\nNode " << j << ": ";
			cout << "used datas: " << leaf[i].leaf_nodes[j]->used_keys << endl;
			for (int k = 0; k < leaf[i].leaf_nodes[j]->used_keys; k++)
			{
				cout << "node " << k << ": ";
				cout << "key: " << leaf[i].getNode(j)->getElement(k)._key << " ";
				cout << "block offset: " << leaf[i].getNode(j)->getElement(k)._block_off << " ";
				cout << "tuple offset: " << leaf[i].getNode(j)->getElement(k)._tuple_off << " ";
				cout << endl;
			}
		}
	}
	cout << endl << endl;
	delete leaf;

	CCSBInternalNodeGroup<int>** internal = new CCSBInternalNodeGroup<int>* [2];
	for (unsigned i = 0; i < 2; i++)
	{
		internal[i] = new CCSBInternalNodeGroup<int> (1);
		internal[i]->getNode(0)->setUsedKeys(5);
	}
	for (unsigned i = 0; i < 2; i++)
	{
		for (unsigned j = 0; j < 5; j++)
		{
			internal[i]->getNode(0)->setElement(j, data[i*5+j]._key);
		}
	}
	for (unsigned i = 0; i < 2; i++)
	{
		cout << "internal node group" << i << ": \n";
		cout << "node 0: \n";
		for (unsigned j = 0; j < 5; j++)
		{
			cout << "keys " << j << ": " << internal[i]->getNode(0)->getElement(j)._key << " ";
		}
		cout << endl;
	}

	delete[] data;

	cout << "!!!Hello World!!!" << endl; // prints !!!Hello World!!!
	return 0;
}
