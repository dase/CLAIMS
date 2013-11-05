/*
 * BlockManagerId.cpp
 *
 *  Created on: 2013-10-20
 *      Author: casa
 */

#include "BlockManagerId.h"

BlockManagerId::BlockManagerId(){
	blockManagerId=getIpFromLocal();
}

BlockManagerId::BlockManagerId(string bmi){
	blockManagerId=bmi;
}

BlockManagerId::~BlockManagerId() {

}
