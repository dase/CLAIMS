/*
 * BlockStreamIteratorBase.cpp
 *
 *  Created on: Aug 26, 2013
 *      Author: wangli
 */

#include "../physical_query_plan/BlockStreamIteratorBase.h"

#include <iostream>
#include <assert.h>

#include "ExpandableBlockStreamSingleColumnScan.h"

using namespace std;

BlockStreamIteratorBase::BlockStreamIteratorBase() {
	// TODO Auto-generated constructor stub

}

BlockStreamIteratorBase::~BlockStreamIteratorBase() {
	// TODO Auto-generated destructor stub
}

BlockStreamIteratorBase* BlockStreamIteratorBase::createIterator(const string &IteratorName){
//	if(IteratorName=="scan"){
//		cout<<"scan iterator"<<endl;
//		return new ExpandableBlockStreamSingleColumnScan();
//	}
}
ResultSet* BlockStreamIteratorBase::GetResultSet(){
	printf("You cannot get ResultSet from Root operator!\n");
	assert(false);
	return 0;
}
