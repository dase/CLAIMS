/*
 * BlockStreamIteratorBase.cpp
 *
 *  Created on: Aug 26, 2013
 *      Author: wangli
 */

#include <iostream>
#include <assert.h>
#include "BlockStreamIteratorBase.h"
#include "ParallelBlockStreamIterator/ExpandableBlockStreamSingleColumnScan.h"

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
ResultSet BlockStreamIteratorBase::getResultSet(){
	printf("You can get ResultSet from Root operator!\n");
	assert(false);
	return ResultSet();
}
