/*
 * MonitorableBuffer.cpp
 *
 *  Created on: Mar 27, 2014
 *      Author: wangli
 */

#include "MonitorableBuffer.h"

MonitorableBuffer::MonitorableBuffer():input_complete_(false){

}

MonitorableBuffer::~MonitorableBuffer() {

}

void MonitorableBuffer::setInputComplete(){
	input_complete_=true;
}
bool MonitorableBuffer::inputComplete()const{
	return input_complete_==true;
}
