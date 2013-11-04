/*
 * RandomDiskAccessIterator.cpp
 *
 *  Created on: 2013年8月26日
 *      Author: SCDONG
 */

#include <malloc.h>
#include <errno.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/mman.h>

#include "RandomDiskAccessIterator.h"

RandomDiskAccessIterator::RandomDiskAccessIterator(RandomDiskAccessIterator::State state)
:Iterator(), state(state), length(0), fd(NULL), oid(0)
{

}

RandomDiskAccessIterator::~RandomDiskAccessIterator() {

}

bool RandomDiskAccessIterator::open()
{
	state.child->open();
	fd = fopen(state.file_name.c_str(),"r+");
	if (NULL == fd)
	{
		printf("Cannot open file %s!\n", state.file_name.c_str());
//		printf("Cannot open file %s! Reason %s\n", state.file_name.c_str(), strerror(erron));
		return false;
	}

	fseek(fd, 0, SEEK_END);
	length = ftell(fd);
	return true;
}

bool RandomDiskAccessIterator::close()
{
	if(0 == fclose(fd));
		return true;
	return false;
}
