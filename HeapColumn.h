/*
 * HeapColumn.h
 *
 *  Created on: May 7, 2013
 *      Author: wangli
 */

#ifndef HeapColumn_H_
#define HeapColumn_H_


#include <fcntl.h>
#include <unistd.h>
#include <errno.h>
#include <stdio.h>

#include <string>
#include <sys/types.h>
#include <assert.h>
#include <iostream>
#include "Column.h"
#include "ColumnAppenderBase.h"

using std::string;
using namespace std;
template<typename T>
class HeapColumn:public Column{
public:


	 HeapColumn(unsigned table_id,unsigned column_id)
	:Column(table_id,column_id,sizeof(T))
	{
		 char name[256];
		 sprintf(name,"/home/imdb/temp/%d_%d.column",table_id,column_id);
		 file_name=string(name);

	};
	void prt();

	~HeapColumn(){};
	unsigned get_type_length()
	{

		return sizeof(T);
	}

class Appender:public ColumnAppenderBase{
public:
		Appender(HeapColumn* h)
		:heapColumn(h),fd(0),opened(false),record_length(sizeof(T)+sizeof(oid_t))
		{

		}
		void p()
		{
			printf("%s\n",heapColumn->file_name.c_str());
		}
		bool open()
		{
//			fd=open(heapColumn->file_name.c_str(),O_APPEND|O_RDWR,0777);
//			if(fd==-1)
//			{
//				printf("Cannot open column file %s, reason:%s",heapColumn->file_name.c_str(), errno);
//				return false;
//			}
//
//			opened=true;
//			return true;
			assert(fd==0);
			if((fd=fopen(heapColumn->file_name.c_str(),"ab"))==NULL)
			{
				printf("Cannot open column file %s!",heapColumn->file_name.c_str());
				return false;
			}
			opened=true;
			return true;

			//write(fd,&c,1);
			//close(fd);
		}
		bool append(void* src)
		{
			assert(opened==true);
			assert(record_length!=0);
			printf("oid=%d,value=%d",*(int*)src,*((int*)src+1));
			printf("length=%d\n",record_length);

			if(fwrite(src,record_length,1,fd)!=record_length)
			{
				printf("Cannot append this record!\n");
				return false;
			}
			return true;

		}

		bool append(void* src,oid_t oid)
		{
			assert(opened==true);
			assert(record_length!=0);
			//printf("oid=%d,value=%d",oid,*(T *)src);

			//cout<<"oid="<<oid<<",value="<<*(T *)src<<endl;

			//printf("length=%d\n",record_length);

			int tmp1,tmp2;
			if(((tmp1=fwrite(src,record_length-sizeof(oid_t),1,fd))!=1)||((tmp2=fwrite(&oid, sizeof(oid_t),1,fd))!= 1))
			{
				printf("Cannot append this record!\n");
				printf("Reason: %d\t%d\n",tmp1,tmp2);
				return false;
			}
			heapColumn->record_count++;
			return true;

		}

		void close()
		{
			fclose(fd);
		}
private:
		HeapColumn* heapColumn;
		FILE* fd;
		//int fd;
		bool opened;
		unsigned record_length;
	};
    ColumnAppenderBase* create_appender()
    {
    	return new Appender(this);
    }



private:
	string file_name;

public:// will be private!
	bool create_empty()
	{
		int fd;
		fd=open(file_name.c_str(),O_CREAT|O_RDWR|O_TRUNC,00777);
		//printf("fd=%d",fd);
		if(fd==-1)
		{
			printf("Cannot create file: %s\n",file_name.c_str());
			return false;
		}
		close(fd);
		//printf("fd=%d",fd);
		return true;
	}


};
//#ifndef __HeapColumn_PT_INT__
//#define __HeapColumn_PT_INT__
//template<>
//void HeapColumn<int>::prt()
//{
//	printf("INT!!!!\n");
//
//}
//#endif
#endif /* HeapColumn_H_ */
