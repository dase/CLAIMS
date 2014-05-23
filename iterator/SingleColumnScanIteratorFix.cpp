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
#include <stdio.h>
#include "SingleColumnScanIteratorFix.h"
#include "../configure.h"
#include "../common/rename.h"

SingleColumnScanIteratorFix::SingleColumnScanIteratorFix(SingleColumnScanIteratorFix::State state)
:state(state),cursor(0),length(0),base(0),fd(-1)
{
	// TODO Auto-generated constructor stub
//	tuple=memalign(cacheline_size,state.output->getTupleMaxSize());
}

SingleColumnScanIteratorFix::~SingleColumnScanIteratorFix() {
	// TODO Auto-generated destructor stub
}

bool SingleColumnScanIteratorFix::open()
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

	data=malloc(length);
	memcpy(data,base,length);




	if(data!=0)
	{
		cursor=(char*)data;
		return true;
	}
	else
		return false;
}


bool SingleColumnScanIteratorFix::close()
{
//	munmap(base,length);

	free(data);
	if(FileClose(fd)==0)
		return true;
	else
		return false;
}



