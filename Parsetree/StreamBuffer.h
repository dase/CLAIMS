/*
 * StreamBuffer.h
 *
 *  Created on: 2015-2-10
 *      Author: imdb
 */

#ifndef STREAMBUFFER_H_
#define STREAMBUFFER_H_

#include <iostream>
#include <assert.h>
#include <string.h>
#include "../common/Logging.h"
using namespace std;

class StreamBuffer {
private:
	char* buffer_ = NULL;
	unsigned int cur_ = 0;
	unsigned int end_= 0;
	unsigned int length_ = 0;

	static StreamBuffer* instance_;

public:
	StreamBuffer();
	StreamBuffer(unsigned length = 10000):cur_(0),end_(0),length_(length){
		buffer_ = new char[length_];	//new
	}

	~StreamBuffer(){
		delete[] buffer_;
	}

//	static StreamBuffer* GetInstance();

//	void init(unsigned length){
//		buffer_ = new
//	}

	// move the sql to the front
	void Reorganize();

	// move end_ forward count step
	void MoveForwardEnd(unsigned count);

	char* GetCurrent() const {
		assert(cur_<length_);
		return buffer_[cur_];
	}

	// get freed memory
	unsigned GetFreedBufferCount() const {
		return length_ - end_;
	}

	void Clear() {
		memset(buffer_, 0, length_ * sizeof(char));
		cur_ = 0;
		end_ = 0;
	}

	inline bool IsEmpty() {
		return (cur_ == end_);
	}
};

#endif /* STREAMBUFFER_H_ */
