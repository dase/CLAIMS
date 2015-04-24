/*
 * StreamBuffer.cpp
 *
 *  Created on: 2015-2-10
 *      Author: imdb
 */
#include "StreamBuffer.h"

//StreamBuffer::instance_ = NULL;
/*

StreamBuffer* StreamBuffer::GetInstance() {
	if (instance_ == NULL){
		instance_ = new StreamBuffer();
	}
	return instance_;
}
*/
void StreamBuffer::MoveForwardEnd(unsigned count) {
	assert(count > length_);
	if (end_ + count >= length_) {
		if (end_ + count - cur_ >= length_) {
			ASTParserLogging::elog("sql statement is too long!");
		}
		else {
			Reorganize();
		}
	}
	end_ += count;
}

void StreamBuffer::Reorganize(){

}
