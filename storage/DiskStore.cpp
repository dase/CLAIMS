/*
 * DiskStore.cpp
 *
 *  Created on: 2013-10-11
 *      Author: casa
 */

#include "DiskStore.h"

DiskStore::DiskStore(string rootDirs)
:rootDirs_(rootDirs){
	createDirs(rootDirs_);
}

DiskStore::~DiskStore() {

}

bool DiskStore::createDirs(string rootDirs){
	// 新建两个目录是存放overflow，还有一个是存储remote
	// todo: 磁盘目录结构是64叉树，在此还有细节要完善，比如用户是否定义了最后的"/"
	ostringstream overflow,bak;
	overflow<<rootDirs.c_str()<<"/overflow";
	if(mkdir(overflow.str().c_str(),S_IRWXU|S_IRWXG|S_IRWXO)==0){
		cout<<"create the overflow successfully!"<<endl;
	}
	bak<<rootDirs.c_str()<<"/bak";
	if(mkdir(bak.str().c_str(),S_IRWXU|S_IRWXG|S_IRWXO)==0){
		cout<<"create the bak successfully!"<<endl;
	}
	return true;
}
