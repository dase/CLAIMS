/*
 * BlanceMatcher.h
 *
 *  Created on: 2013-10-31
 *      Author: casa
 */

#ifndef BLANCEMATCHER_H_
#define BLANCEMATCHER_H_
/*
 * 为storage提供数据，scan操作的是每个
 * */
#include "BlockManagerId.h"

#include <map>
#include <string>
#include <list>
#include <iostream>
using namespace std;

class BlanceMatcher {
public:
	static BlanceMatcher *getInstance(){
		if(blancematcher_==0){
			blancematcher_=new BlanceMatcher();
		}
		return blancematcher_;
	}
	virtual ~BlanceMatcher();

	bool projectsInput(string filename, list<string> project);
	// 返回的是projectid
	string matcher(string filename,BlockManagerId bmi);

private:
	BlanceMatcher();
private:
	static BlanceMatcher *blancematcher_;
	// 文件与project的对应关系
	map<string, list<string> > projects_;
	// 文件与来取数据的ip的对应关系
	map<string, list<string> > coming_ip_;
};

#endif /* BLANCEMATCHER_H_ */
