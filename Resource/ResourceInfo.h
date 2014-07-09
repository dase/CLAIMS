/*
 * ResourceInfo.h
 *
 *  Created on: Oct 31, 2013
 *      Author: wangli
 */

#ifndef RESOURCEINFO_H_
#define RESOURCEINFO_H_
#include <string>
#include <iostream>
#ifdef DMALLOC
#include "dmalloc.h"
#endif
struct memory_info{
	/* the following volumes are in Mbytes*/
	memory_info():allocated_capacity(-1),used_capacity(-1){};
	memory_info(int allo,int used):allocated_capacity(allo),used_capacity(used){};
	void initialize(int capacity){
		allocated_capacity=capacity;
		used_capacity=0;
	}
	bool take(int buget){
		if(allocated_capacity>=used_capacity+buget){
			used_capacity+=buget;
			std::cout<<"used_capacity:"<<used_capacity<<std::endl;
			return true;
		}
		return false;
	}
	void put(int size){
		used_capacity-=size;
	}
	int allocated_capacity;
	int used_capacity;
};
struct disk_info{
	/* the following volumes are in Mbytes*/
	disk_info():allocated_capacity(-1),used_capacity(-1){};
	disk_info(int allo,int used):allocated_capacity(allo),used_capacity(used){};
	bool take(int buget){
		if(allocated_capacity>=used_capacity+buget){
			used_capacity+=buget;
			return true;
		}
		return false;
	}
	void put(int size){
		used_capacity-=size;
	}
	void initialize(int capacity){
		allocated_capacity=capacity;
		used_capacity=0;
	}
	int allocated_capacity;
	int used_capacity;

};

class ResourceInfo {
public:
	friend class ResourceManagerMaster;
	ResourceInfo();
	virtual ~ResourceInfo();
protected:
	std::string slave_ip_;
	memory_info memory;
	disk_info disk;
	/*
	 * TODO: other resources, such as CPUs.
	 */

};

#endif /* RESOURCEINFO_H_ */
