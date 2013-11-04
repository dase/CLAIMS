/*
 * SequencialDiskAccessIterator.cpp
 *
 *  Created on: 2013年8月27日
 *      Author: SCDONG
 */

#include <malloc.h>
#include <errno.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/mman.h>

#include "SequencialDiskAccessIterator.h"

SequencialDiskAccessIterator::SequencialDiskAccessIterator(SequencialDiskAccessIterator::State state)
:Iterator(), state(state), fd(NULL), length(0), cursor(0)
{

}

SequencialDiskAccessIterator::~SequencialDiskAccessIterator() {

}

bool SequencialDiskAccessIterator::open()
{
	fd = fopen(state.file_name.c_str(),"r+");
	if (NULL == fd)
	{
		printf("Cannot open file %s!\n", state.file_name.c_str());
//		printf("Cannot open file %s! Reason %s\n", state.file_name.c_str(), strerror(erron));
		return false;
	}

	fseek(fd, 0, SEEK_END);
	length = ftell(fd);
	fseek(fd, 0, SEEK_SET);
	return true;
}

bool SequencialDiskAccessIterator::close()
{
	if(0 == fclose(fd));
		return true;
	return false;
}


