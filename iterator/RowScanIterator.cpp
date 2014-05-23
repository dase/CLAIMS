/*
 * RowScanIterator.cpp
 *
 *  Created on: 2013年8月23日
 *      Author: SCDONG
 */

#include <malloc.h>
#include <errno.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/mman.h>

#include "../common/rename.h"
#include "RowScanIterator.h"

RowScanIterator::RowScanIterator(RowScanIterator::State state)
:Iterator(), state(state), cursor(0), length(0), base(0), fd(-1)
{

}

RowScanIterator::~RowScanIterator() {

}

bool RowScanIterator::open()
{
	fd = FileOpen(state.file_name.c_str(),O_RDONLY);
	if (-1 == fd)
	{
		printf("Cannot open file %s!\n", state.file_name.c_str());
//		printf("Cannot open file %s! Reason %s\n", state.file_name.c_str(), strerror(erron));
		return false;
	}

	length = lseek(fd, 0, SEEK_END);
	base = mmap(0, length, PROT_READ, MAP_PRIVATE, fd, 0);
	if (0 != base)
	{
		cursor = (char* )base;
		return true;
	}

	return false;
}

bool RowScanIterator::close()
{
//	munmap(base, length);

	if(0 == FileClose(fd));
		return true;
	return false;
}



