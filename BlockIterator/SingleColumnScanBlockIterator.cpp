/*
 * SingleColumnScanBlockIterator.cpp
 *
 *  Created on: Aug 23, 2013
 *      Author: wangli
 */
#include <malloc.h>
#include <errno.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/mman.h>
#include "SingleColumnScanBlockIterator.h"
#include "../rename.h"

SingleColumnScanBlockIteratorFix::SingleColumnScanBlockIteratorFix(State state)
:state(state){
	// TODO Auto-generated constructor stub

}

SingleColumnScanBlockIteratorFix::~SingleColumnScanBlockIteratorFix() {
	// TODO Auto-generated destructor stub
}

bool SingleColumnScanBlockIteratorFix::open(){
	printf("SingleCOlumn: open!\n");
	tuple_length=state.schema->getTupleSize();
	fd=FileOpen(state.file_name.c_str(),O_RDONLY);
	if(fd==-1){
		printf("Cannot open file %s! Reason: %s\n",state.file_name.c_str(),strerror(errno));
		return false;
	}
	length=lseek(fd,0,SEEK_END);
	printf("input length=%d bytes.\n",length);
	base=mmap(0,length,PROT_READ,MAP_PRIVATE,fd,0);
//	read(fd,base,length);
	if(base==0){
		printf("mmap errors!\n");
		return false;
	}
//	printf("begin malloc.\n");
	data=malloc(length);
//	printf("malloc is successful!\n");
	if(data==0){
		printf("malloc errors!\n");
		return false;
	}
	memcpy(data,base,length);
	if(data!=0){
		cursor=(char*)data;
		printf("Open is successful!\n");
		return true;
	}
	else
		return false;
}
bool SingleColumnScanBlockIteratorFix::close(){
	free(data);
	data=0;
//	munmap(base,length);
	if(FileClose(fd)==0)
		return true;
	else
		return false;
}
