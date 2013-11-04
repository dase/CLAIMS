#include <stdlib.h>
#include <boost/archive/tmpdir.hpp>
#include <boost/archive/text_iarchive.hpp>
#include <boost/archive/text_oarchive.hpp>
#include <boost/serialization/base_object.hpp>
#include <boost/serialization/utility.hpp>
#include <boost/serialization/list.hpp>
#include <boost/serialization/assume_abstract.hpp>

#include "../Executor/IteratorExecutorMaster.h"
#include "../Executor/IteratorExecutorSlave.h"
#include "../iterator.hpp"
#include "../iterator/FilterIterator.h"
#include "../iterator/PrintIterator.h"
#include "../iterator/SingleColumnScanIterator.h"
#include "../iterator/ExchangeIteratorEager.h"
#include "../Environment.h"
#include "../PerformanceMonitor/PerformanceIteratorTop.h"
int mainasfasf345asf(int argc, char** argv){
	int choice=0;


	int block_size=atoi(argv[1]);
	printf("Master(0) or Slave(1) ?\n");
	choice=0;
	scanf("%d",&choice);

	if(choice==0)
	{
		std::vector<std::string> upper_ip_list;
		std::vector<std::string> lower_ip_list;

		std::cout<<"Environment initializing!"<<std::endl;
		Environment::getInstance(true);



		///////////////////////////////////////////Scan////////////////////////////////////////|
		std::vector<column_type> column_list;
		column_list.push_back(column_type(t_int));

		Schema* input=new SchemaFix(column_list);
		Schema* output=new SchemaFix(column_list);
		//SingleColumnScanIterator::State SCstate1("/home/imdb/temp/1_0.column_copy",input, output);
		SingleColumnScanIterator::State SCstate1("/home/imdb/temp/Uniform_0_99.column",input,output);
		Iterator* scs1=new SingleColumnScanIterator(SCstate1);

		///////////////////////////////////////////Exchange////////////////////////////////////|
		upper_ip_list.push_back("10.11.1.204");
		upper_ip_list.push_back("10.11.1.205");
		lower_ip_list.push_back("10.11.1.206");
		lower_ip_list.push_back("10.11.1.207");

		const unsigned long long int exchange_id_1=1;
		ExchangeIteratorEager::State EIstate(input,scs1,block_size,lower_ip_list,upper_ip_list,exchange_id_1);
		Iterator* ei=new ExchangeIteratorEager(EIstate);

		///////////////////////////////////////Filter//////////////////////////////////////////|
		FilterIterator::State FIstate;
		FIstate.child=scs1;

		FIstate.input=output;
		FIstate.output=output;
		int f=0;
		f=atoi(argv[2]);
		FilterIterator::AttributeComparator filter1(column_type(t_int),Comparator::L,0,&f);
		FIstate.ComparatorList.push_back(filter1);
		Iterator* fi=new FilterIterator(FIstate);

		///////////////////////////////////////////Exchange////////////////////////////////////|
		upper_ip_list.clear();
		lower_ip_list.clear();
		upper_ip_list.push_back("10.11.1.214");

		lower_ip_list.push_back("10.11.1.201");
		lower_ip_list.push_back("10.11.1.202");
		lower_ip_list.push_back("10.11.1.203");
		lower_ip_list.push_back("10.11.1.204");
		lower_ip_list.push_back("10.11.1.205");
		lower_ip_list.push_back("10.11.1.206");
		lower_ip_list.push_back("10.11.1.207");
		lower_ip_list.push_back("10.11.1.208");
		lower_ip_list.push_back("10.11.1.209");
		lower_ip_list.push_back("10.11.1.211");
		lower_ip_list.push_back("10.11.1.210");

		const unsigned long long int exchange_id_0=0;

		ExchangeIteratorEager::State EIstate_(output,fi,block_size,lower_ip_list,upper_ip_list,exchange_id_0);

//		ExchangeIteratorWithWideDependency::State EIstate_(output,fi,block_size,lowernode_,uppernode_,exchange_id_0);
		Iterator* ei_=new ExchangeIteratorEager(EIstate_);

		///////////////////////////////////////Performance///////////////////////////////////////////|


		PerformanceIteratorTop::State PITstate(output,ei_,1000);
		Iterator *pi=new PerformanceIteratorTop(PITstate);

		//-------------------------------------------------------------------------------------|
		//IteratorExecutorMaster IEM;
		int d;
		printf("Enter 1 to continue, other number to stop.\n");
		scanf("%d",&d);

		int printcount=0;
		while(d==1)
		{
			cout<<"in the Exchange test"<<endl;
			pi->open();
			cout<<"in the Exchange test and already pass the open func"<<endl;
			while(pi->next(0))
				printcount++;
			cout<<"total number:"<<printcount<<endl;
			pi->close();
			getchar();
			scanf("%d",&d);
		}
	}
	else
	{
		Environment::getInstance(false);
		//IteratorExecutorSlave IES;
	}

	printf("Go to sleep while!\n");
	while(1)
	{
		sleep(1);
	}
	return 1;
}
