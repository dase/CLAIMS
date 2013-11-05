/*
 * BlanceMatcher.cpp
 *
 *  Created on: 2013-10-31
 *      Author: casa
 */

#include "BlanceMatcher.h"

BlanceMatcher *BlanceMatcher::blancematcher_=0;

BlanceMatcher::BlanceMatcher() {

}

BlanceMatcher::~BlanceMatcher() {

}

bool BlanceMatcher::projectsInput(string filename,list<string> project){
	projects_[filename.c_str()]=project;
	return true;
}

string BlanceMatcher::matcher(string filename,BlockManagerId bmi){
	cout<<"in the matcher: "<<filename.c_str()<<" + "<<bmi.blockManagerId.c_str()<<endl;
	if(coming_ip_[filename.c_str()].empty()){
		list<string> proj;
		proj.push_back(bmi.blockManagerId);
		coming_ip_[filename.c_str()]=proj;
		string rt=projects_[filename.c_str()].front();
		projects_[filename.c_str()].pop_front();
		return rt;
	}
	else{
		// 判断来取数据的ip是否来过
		cout<<"the vector is not empty!"<<endl;
		list<string> bmis=coming_ip_[filename.c_str()];
		bool exists=false;
		list<string>::iterator itr=bmis.begin();
		for(unsigned i=0;i<bmis.size();i++){
			if((*itr)==bmi.blockManagerId){
				exists=true;
				break;
			}
			else{
				itr++;
			}
		}
		if(!exists){
			coming_ip_[filename.c_str()].push_back(bmi.blockManagerId);
		}
		string ite=projects_[filename.c_str()].front();
		projects_[filename.c_str()].pop_front();
		return ite;
	}
}
