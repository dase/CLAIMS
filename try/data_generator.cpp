/*
 * data_generator.cpp
 *
 *  Created on: Aug 31, 2013
 *      Author: wangli
 */

#include <fcntl.h>
#include <unistd.h>
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
int maindghrtuujjrtf(int argc, const char** argv){
	unsigned long tuples=atoi(argv[1]);
	tuples=tuples*1024*1024;
	const char* file_name=argv[2];

	printf("File name:\t%s\nTuples:\t%lu ",file_name,tuples);
	printf("Continue (y) or not (n)?\n");
	char c=getchar();
	if(c!='y'){
		return 0;
	}

	FILE* fd;
	if((fd=fopen(file_name,"ab"))==NULL){
		printf("Cannot open column file %s!",file_name);
		return -1;
	}

	int record;
	const unsigned size_of_record=sizeof(record);
	while(tuples--){
		record=rand()%10000;
		if(fwrite(&record,size_of_record,1,fd)!=1){
			printf("Error occurs~!\n");
			return -1;
		}
		if(tuples%(1024*1024)==0){
			printf("%lu\n",tuples);
		}
	}
	printf("finished!\n");

}

