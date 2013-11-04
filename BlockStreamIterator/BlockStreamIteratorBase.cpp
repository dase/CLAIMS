/*
 * BlockStreamIteratorBase.cpp
 *
 *  Created on: Aug 26, 2013
 *      Author: wangli
 */

#include "BlockStreamIteratorBase.h"
#include "ParallelBlockStreamIterator/ExpandableBlockStreamSingleColumnScan.h"

#include <iostream>
using namespace std;

BlockStreamIteratorBase::BlockStreamIteratorBase() {
	// TODO Auto-generated constructor stub

}

BlockStreamIteratorBase::~BlockStreamIteratorBase() {
	// TODO Auto-generated destructor stub
}

BlockStreamIteratorBase* BlockStreamIteratorBase::createIterator(const string &IteratorName){
	if(IteratorName=="scan"){
		cout<<"scan iterator"<<endl;
		return new ExpandableBlockStreamSingleColumnScan();
	}
}
