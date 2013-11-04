/*
 * SingleColumnScanIterator.cpp
 *
 *  Created on: Jun 7, 2013
 *      Author: wangli
 */
#include <malloc.h>
#include <errno.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/mman.h>
#include "SingleColumnScanIterator.h"
#include "../configure.h"
#include "../rename.h"
//typedef int (f)(const char*,int,...);
//f *fileopen=open;
//typedef int (g)(int);
//g *fileclose=close;
SingleColumnScanIterator::SingleColumnScanIterator(SingleColumnScanIterator::State state)
:Iterator(),state(state),cursor(0),length(0),base(0),fd(-1)
{
	// TODO Auto-generated constructor stub
//	tuple=memalign(cacheline_size,state.output->getTupleMaxSize());
}

SingleColumnScanIterator::~SingleColumnScanIterator() {
	// TODO Auto-generated destructor stub
}

bool SingleColumnScanIterator::open()
{
	fd=FileOpen(state.file_name.c_str(),O_RDONLY);
	if(fd==-1)
	{
		printf("Cannot open file %s! Reason: %s\n",state.file_name.c_str(),strerror(errno));
		return false;
	}
	else
	{
		length=lseek(fd, 0, SEEK_END);
	}
	base=mmap(0,length,PROT_READ,MAP_PRIVATE,fd,0);
	if(base!=0)
	{
		cursor=(char*)base;
		return true;
	}
	else
		return false;
}


bool SingleColumnScanIterator::close()
{
//	munmap(base,length);

	if(FileClose(fd)==0)
		return true;
	else
		return false;
}



