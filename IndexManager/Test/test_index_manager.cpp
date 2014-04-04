/*
 * test_index_manager.cpp
 *
 *  Created on: Mar 24, 2014
 *      Author: scdong
 */

#include "../CSBPlusTree.h"
#include <map>
#include <vector>
#include "../../ids.h"
#include "../IndexManager.h"
#include "../../Catalog/Catalog.h"



static int test_index_manager_()
{
	map<ChunkID, CSBPlusTree<int>* > sec_code_index = IndexManager::getInstance()->getIndexList<int>(Catalog::getInstance()->getTable(0)->getAttribute(3));
//	map<ChunkID, CSBPlusTree<int>* > sec_code_index = IndexManager::getInstance()->getIndexList<int>(0);
	if (sec_code_index.size() == 0)
	{
		cout << "[ERROR: test_index_manager.cpp->test_index_manager()]: No index on attribute " << Catalog::getInstance()->getTable(0)->getAttribute(3).attrName << endl;
		return -1;
	}
	map<ChunkID, CSBPlusTree<int>* >::iterator iter = sec_code_index.begin();
	map<index_offset, vector<index_offset> >* ret = new map<index_offset, vector<index_offset> > [sec_code_index.size()];
	while (true)
	{
		int sec_code = 0;
		cout << "\nPlease input the sec_code for searching: \n";
		cin >> sec_code;

		int count = 0;
		unsigned long total_tuple = 0;
		for (iter = sec_code_index.begin(); iter != sec_code_index.end(); iter++)
		{
			ret[count].clear();
			ret[count] = iter->second->rangeQuery(sec_code, sec_code);
			total_tuple += ret[count].size();
			count++;
		}

		cout << "The total tuple number is: " << total_tuple << endl;
		cout << "tuples in each chunk: \n";
		for (count = 0; count < sec_code_index.size(); count++)
		{
			cout << "Chunk " << count << ": " << ret[count].size() << " tuples.\n";
			if (ret[count].size() != 0)
			{
				cout << "How many to print? ";
				cin >> sec_code;
				for (map<index_offset, vector<index_offset> >::iterator iter = ret[count].begin(); iter != ret[count].end(); iter++)
					for (vector<index_offset>::iterator iter_ = iter->second.begin(); iter_ != iter->second.end(); iter_++)
						cout << "<" << iter->first << ", " << *iter_ << ">\t";
//				for (int i = 0; i < ret[count].size() && i < sec_code; i++)
//					cout << "<" << ret[count][i]->_block_off << ", " << ret[count][i]->_tuple_off << ">\t";
				cout << endl;
			}
		}

//		ret.clear();
//		ret = iter->second->Search(sec_code);
//		if (ret.size() == 0)
//		{
//			cout << "The result set is NULL!\n";
//			continue;
//		}
//		cout << "The result set size is: " << ret.size() << "\nHow many to print? ";
//		cin >> sec_code;
//		for (int i = 0; i < ret.size() && i < sec_code; i++)
//			cout << "<" << ret[i]->_block_off << ", " << ret[i]->_tuple_off << ">\t";
//		cout << endl;
	}
	return 0;
}


