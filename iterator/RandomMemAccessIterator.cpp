/*
 * RandomMemAccessIterator.cpp
 *
 *  Created on: 2013年8月27日
 *      Author: SCDONG
 */

#include <malloc.h>
#include <errno.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/mman.h>

#include "RandomMemAccessIterator.h"
#include "../configure.h"
#include "../common/rename.h"
RandomMemAccessIterator::RandomMemAccessIterator(RandomMemAccessIterator::State state)
:Iterator(), state(state), base(NULL), length(0), fd(0), oid(0)
{

}

RandomMemAccessIterator::~RandomMemAccessIterator() {

}

bool RandomMemAccessIterator::open()
{
	state.child->open();
	fd = FileOpen(state.file_name.c_str(),O_RDONLY);
	if (-1 == fd)
	{
		printf("Cannot open file %s!\n", state.file_name.c_str());
//		printf("Cannot open file %s! Reason %s\n", state.file_name.c_str(), strerror(erron));
		return false;
	}

	length = lseek(fd, 0, SEEK_END);
	base = mmap(0, length, PROT_READ, MAP_PRIVATE, fd, 0);
//	data=(char*)malloc(length);
//	memcpy(data,base,length);
//	base=data;
	if (0 != base)
	{
//		cursor = (char* )base;
		return true;
	}

	return false;
}

bool RandomMemAccessIterator::close()
{
//	free(data);
//	munmap(base, length);

	if(0 == FileClose(fd));
		return true;
	return false;
}


