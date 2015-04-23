#include <iostream>
using namespace std;

#include "../SchemaVar.h"
#include "../../rename.h"
#include "../../../common/Block/BlockStream.h"

static int generateVariableData_test(){
	int fd=FileOpen("/home/casa/storage/file/var/new.data",O_WRONLY|O_CREAT);
	BlockStreamBase *blockstreambase=0;
	vector<column_type> column_list;
	column_list.push_back(column_type(t_int));
	column_list.push_back(column_type(t_double));
	column_list.push_back(column_type(t_string));
	Schema *schema=new SchemaVar(column_list);
	unsigned string1_number=0;
	unsigned string123_number=0;
	unsigned number=0;
	for(unsigned i=0;i<2;i++){
		if(i==0){
			Block *block=new Block(64*1024);
			blockstreambase=new BlockStreamVar(64*1024-sizeof(unsigned),schema);
			void *start=0;
			int col_1=1;
			double col_2=2;
			char* col_3="string1";
			while((start=blockstreambase->allocateTuple(24))!=0){
				void *tuple=malloc(24);		//newmalloc
				*(int *)tuple=8;
				*((int *)tuple+1)=col_1;
				*((double *)((char *)tuple+8))=col_2;
				memcpy((char *)tuple+16,col_3,8);
				reinterpret_cast<BlockStreamVar*>(blockstreambase)->insert(start,tuple,24);
				free(tuple);
	//			reinterpret_cast<BlockStreamVar*>(blockstreambase)->toDisk(blockstreambase,"/home/casa/storage/file/var/bak");
			}
			blockstreambase->serialize(*block);
			lseek(fd,64*1024*i,SEEK_SET);
			write(fd,block->getBlock(),1024*64);
		}
		if(i==1){
			Block *block=new Block(64*1024);
			blockstreambase=new BlockStreamVar(64*1024-sizeof(unsigned),schema);
			void *start=0;
			int col_1=1;
			double col_2=2;
			char* col_4="string123";
			while((start=blockstreambase->allocateTuple(26))!=0){
				void *tuple=malloc(26);		//newmalloc
				*(int *)tuple=10;
				*((int *)tuple+1)=col_1;
				*((double *)((char *)tuple+8))=col_2;
				memcpy((char *)tuple+16,col_4,10);
				reinterpret_cast<BlockStreamVar*>(blockstreambase)->insert(start,tuple,26);
				free(tuple);
	//			reinterpret_cast<BlockStreamVar*>(blockstreambase)->toDisk(blockstreambase,"/home/casa/storage/file/var/bak");
			}
			blockstreambase->serialize(*block);
			lseek(fd,64*1024*i,SEEK_SET);
			write(fd,block->getBlock(),1024*64);
		}
	}
	return 0;
}
