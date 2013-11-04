
#include <string>
#include <stdlib.h>
#include <stdio.h>
#include <typeinfo>
#include <vector>
#include <Theron/Theron.h>
#include <Theron/Defines.h>
#include <libconfig.h++>

#include "../TablePartition.h"
#include "../HeapColumn.h"
#include "../TableManager.h"
#include "../rdtsc.h"
#include "../TableManagerSlave.h"
#include "../TableManagerMaster.h"

using namespace std;


template <typename T>
struct record{
	oid_t oid;
	T value;
};

typedef int S;

struct int_float
{
	int i;
	int f;

};




//int main567656()
//{
//
////libconfig::Config cong;
////cong.readFile("/home/wangli/config/config");
////string stra;
////
////stra =(const char*)cong.lookup("app.user.sex");
////
////cout<<stra<<endl;
//
////	unsigned long long t;
////	startTimer(&t);
////	long t1=getMilliSecond
//
//	int a;
//	char c;
//	printf("%s",typeid(a).name());
//	printf("%s",typeid(char).name());
////   HeapColumn<int> asf(1,2);
////
////   //asf.create_empty();
////   asf.pt();
////   HeapColumn<int>::Appender a=asf.create_appender();
////   char aaa='a';
////   a.open();
////
////   record<int> r;
////   r.oid=1;
////   r.value=100;
////
////	   printf("-----%s----",typeid(r.oid).name());
////
////   a.append(&r);
////   vector<int> bbbb;
////   a.close();
//
//	vector<data_type> dt;
//	dt.push_back(t_int);
//	dt.push_back(t_float);
//
//	int choice;
//	printf("Master(0) or Slave(1) ?\n");
//	choice=1;
//	scanf("%d",&choice);
//
//	if(choice==0)
//	{
//		//Master:
////
//		TableManagerMaster* tmm=TableManagerMaster::instance();
//		printf("Press 's' to send create message to all the slaves!\n");
//		char again[256];;
//		scanf("%s",again);
//		while(strcmp(again, "exit")!=0)
//		{
//
//			tmm->create_table_on_all_slaves(std::string(again),dt);
//			scanf("%s",again);
//		}
////		printf("OK!!\n");
//
//
////		Theron::EndPoint endpoint("abc","tcp://58.198.176.121:5556");
////		if(!endpoint.Connect("tcp://58.198.176.121:5555"))
////		{
////			printf("Network faillasf~~\n");
////		}
////		Theron::Framework framework(endpoint);
////
////		sleep(1);
////		framework.Send(1,Theron::Address(),Theron::Address("58.198.176.121:5555"));
//
//	}
//	else
//	{
//		//Slave:
//		TableManagerSlave tms("10.11.1.224","5555");
//	}
//
//
//
//
//
//	printf("Go to sleep while!\n");
//	while(1)
//	{
//		sleep(1);
//	}
//
//
//
//	return 1;
//	TableManager tm;
//	tm.create_table_partition(1,dt);
//
//
//	printf("\n sizof of TableManager is %d\n",sizeof(Column));
//
//
//	unsigned long long t1;
//	startTimer(&t1);
//
//	int tupleCount=10000;
//	oid_t oid=0;
//	int_float tmp;
//	while(tupleCount>0)
//	{
//
//		tmp.i=tupleCount;
//		tmp.f=tupleCount/(float)10;
//		tm.append(1,&tmp,oid++);
//		tupleCount--;
//	}
//	double time=getMilliSecond(t1);
//	printf("Total Time: %lf ms, %f M record/s!\n",time, 8*10000/(float)1024/1024/(time/1000));
//
//}



int mainfasbxcv(){
	vector<data_type> dt;
	dt.push_back(t_int);
//	dt.push_back(t_int);
	TableManager tm;
		tm.create_table_partition(4,dt);


		printf("\n sizof of TableManager is %d\n",sizeof(Column));


		unsigned long long t1;
		startTimer(&t1);

		int tupleCount=1024*1024*256;
		oid_t oid=0;
		int_float tmp;
		while(tupleCount>0)
		{

			tmp.i=rand()%(64*1024*1024);//rand()%10;//tupleCount;
//			tmp.f=3;//rand()%10;//tupleCount/(int)10;
//			tmp.f=tupleCount/(float)10;
			oid=rand()%(64*1024*1024);
			tm.append(3,&tmp,oid++);
//			tmp.i=3;
//			tmp.f=3;
//			tm.append(1,&tmp,oid++);
			tupleCount--;
			tupleCount--;
			if(tupleCount%100000==0){
				printf("%d\n",tupleCount);
			}
		}
		double time=getMilliSecond(t1);
		printf("Total Time: %lf ms, %f M record/s!\n",time, 8*10000/(float)1024/1024/(time/1000));
	return 0;
}
