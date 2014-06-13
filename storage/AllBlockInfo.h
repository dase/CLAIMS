/*
 * AllBlockInfo.h
 *
 *  Created on: 2013-10-26
 *      Author: casa
 */

#ifndef ALLBLOCKINFO_H_
#define ALLBLOCKINFO_H_

#include <iostream>
#include <string>
#include <vector>
#include <map>
#ifdef DMALLOC
#include "dmalloc.h"
#endif
#include "../utility/lock.h"
using namespace std;


/*
 * 在master端由于theron的局限性，用一个专门的单例对象来存储，供master使用,
 * 记录所有机器上的block的信息，同步的get&put都在这个类中
 * */
class AllBlockInfo {
public:
	static AllBlockInfo* getInstance(){
		if(abi_==0){
			abi_=new AllBlockInfo();
		}
		return abi_;
	}
	~AllBlockInfo();

	bool put(string blockmanagerId,string blockId);

private:
	AllBlockInfo();

private:
	static AllBlockInfo *abi_;
	vector<string> vv_;
	Lock lock_;
};

#endif /* ALLBLOCKINFO_H_ */
