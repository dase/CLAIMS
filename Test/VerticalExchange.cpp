/*
 * VerticalExchange.cpp

 *
 *  Created on: Jun 25, 2013
 *      Author: wangli
 */
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
#include "../iterator/SingleColumnScanIterator.h"
#include "../iterator/ExchangeIteratorWithWideDependency.h"
#include "../iterator/PrintIterator.h"

//int mainhcudh(int argc,char* argv[])
//{
//	int choice=0;
//	printf("Master(0) or Slave(1) ?\n");
//	choice=0;
//	scanf("%d",&choice);
//
//	if(choice==0)
//	{
//
//		///////////////////////////////////////////Scan////////////////////////////////////////|
//		std::vector<column_type> column_list;
//		column_list.push_back(column_type(t_int));
//
//		Schema* input=new SchemaFix(column_list);
//		Schema* output=new SchemaFix(column_list);
//		SingleColumnScanIterator::State SCstate1("/home/imdb/temp/1_0.column",input, output);
//		Iterator* scs1=new SingleColumnScanIterator(SCstate1);
//
//
//		///////////////////////////////////////////Exchange////////////////////////////////////|
//		printf("argc: %d\n", argc);
//		int block_size;
//
//		if(argc==2)
//		{
//			block_size=atoi(argv[1]);
//
//		}
//		else
//		{
//			block_size=64;
//		}
//		ExchangeIterator::State EIstate(input, scs1,block_size);
//		Iterator* ei=new ExchangeIterator(EIstate);
//
//
//
//
//
//		///////////////////////////////////////Print///////////////////////////////////////////|
//
//
//		PrintIterator::State PIstate(input,ei);
//		Iterator *pi=new PrintIterator(PIstate);
//		//-------------------------------------------------------------------------------------|
//
//		std::ostringstream ostr;
//		boost::archive::text_oarchive oa(ostr);
////	//	oa.register_type(static_cast<SingleColumnScanIterator *>(NULL));
////		Register_Schemas<boost::archive::text_oarchive>(oa);
////		Register_Iterators(oa);
////		oa<<pi;
////		std::cout<<"Serialization Result:"<<ostr.str()<<std::endl;
//		IteratorExecutorMaster IEM;
//		int d;
//		printf("Enter 1 to continue, other number to stop.\n");
//		scanf("%d",&d);
//
//		while(d==1)
//		{
//			pi->open();
//			while(pi->next(0));
//			pi->close();
////			IEM.ExecuteIteratorsOnSlave(pi);
////			printf("Enter 1 to continue, other number to stop.\n");
//			scanf("%d",&d);
//		}
////		IteratorMessage IM(pi);
////		Message256 message= IteratorMessage::serialize(IM);
////		std::cout<<"Serialization Result:"<<message.message<<std::endl;
////		ostringstream ostr;
////		boost::archive::text_oarchive oa(ostr);
////		Register_Iterators(oa);
////		oa<<pi;
//
////		istringstream istr(ostr.str());
////
////		printf("%s\n",istr.str());
//
//
////		IEM.ExecuteIteratorsOnSlave(pi);
//
//
//
//	}
//	else
//	{
//		IteratorExecutorSlave IES;
//
//
//
//	}
//	printf("Go to sleep while!\n");
//	while(1)
//	{
//		sleep(1);
//	}
//	return 1;
//}

int main_on_to_multi(int argc,char* argv[])
{
	int choice=0;
	printf("Master(0) or Slave(1) ?\n");
	choice=0;
	scanf("%d",&choice);

	if(choice==0)
	{

		///////////////////////////////////////////Scan////////////////////////////////////////|
		std::vector<column_type> column_list;
		column_list.push_back(column_type(t_int));

		Schema* input=new SchemaFix(column_list);
		Schema* output=new SchemaFix(column_list);
		//SingleColumnScanIterator::State SCstate1("/home/imdb/temp/1_0.column_copy",input, output);
		SingleColumnScanIterator::State SCstate1("/home/imdb/temp/1_0.column.400k",input,output);
		Iterator* scs1=new SingleColumnScanIterator(SCstate1);


		///////////////////////////////////////////Exchange////////////////////////////////////|
		printf("argc: %d\n", argc);
		int block_size;

		if(argc==2)
		{
			block_size=atoi(argv[1]);

		}
		else
		{
			block_size=64;
		}
		std::vector<std::string > uppernode;
		std::vector<std::string > lowernode;
		std::string node204("10.11.1.204");
		std::string node205("10.11.1.205");
		std::string node206("10.11.1.206");
		uppernode.push_back(node204);
		//uppernode.push_back(node205);
		lowernode.push_back(node206);

		std::string node207("10.11.1.207");
		std::string node208("10.11.1.208");
		lowernode.push_back(node207);
		lowernode.push_back(node208);

		ExchangeIteratorWithWideDependency::State EIstate(input, scs1,block_size,lowernode,uppernode);
		Iterator* ei=new ExchangeIteratorWithWideDependency(EIstate);





		///////////////////////////////////////Print///////////////////////////////////////////|


		PrintIterator::State PIstate(input,ei);
		Iterator *pi=new PrintIterator(PIstate);
		//-------------------------------------------------------------------------------------|

		std::ostringstream ostr;
		boost::archive::text_oarchive oa(ostr);
//	//	oa.register_type(static_cast<SingleColumnScanIterator *>(NULL));
//		Register_Schemas<boost::archive::text_oarchive>(oa);
//		Register_Iterators(oa);
//		oa<<pi;
//		std::cout<<"Serialization Result:"<<ostr.str()<<std::endl;
		IteratorExecutorMaster IEM;
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
//			IEM.ExecuteIteratorsOnSlave(pi);
//			printf("Enter 1 to continue, other number to stop.\n");
			scanf("%d",&d);
		}
//		IteratorMessage IM(pi);
//		Message256 message= IteratorMessage::serialize(IM);
//		std::cout<<"Serialization Result:"<<message.message<<std::endl;
//		ostringstream ostr;
//		boost::archive::text_oarchive oa(ostr);
//		Register_Iterators(oa);
//		oa<<pi;

//		istringstream istr(ostr.str());
//
//		printf("%s\n",istr.str());


//		IEM.ExecuteIteratorsOnSlave(pi);



	}
	else
	{
		IteratorExecutorSlave IES;



	}
	printf("Go to sleep while!\n");
	while(1)
	{
		sleep(1);
	}
	return 1;
}


