/*
 * OneToTwoToThree.cpp

 *
 *  Created on: Aug 4, 2013
 *      Author: zhanglei
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
#include "../iterator/FilterIterator.h"
#include "../iterator/PrintIterator.h"
#include "../iterator/SingleColumnScanIterator.h"
#include "../iterator/ExchangeIteratorWithWideDependency.h"
#include "../Environment.h"
typedef pair<string,int> leaves;

/*
 * the test is like root iterator is PrintIterator, the left child and right child is FilterIterator,
 * between the PrintIterator and FilterIterator is the ExchangeIterator, and the child of FilterIterator
 * is ScanIterator, and Between is the ExchangeIterator.
 * */
int mainasfasf(int argc,char* argv[])
{
	int choice=0;

	vector<vector<leaves> > tree;

	vector<leaves> level_0;
	vector<leaves> level_1;
	vector<leaves> level_2;

	leaves tree_node_1("tcp://10.11.1.208:6000",1);
	level_0.push_back(tree_node_1);

	leaves tree_node_2("tcp://10.11.1.204:6000",1);
	leaves tree_node_3("tcp://10.11.1.205:6000",0);
	level_1.push_back(tree_node_2);
	level_1.push_back(tree_node_3);

	leaves tree_node_4("tcp://10.11.1.206:6000",0);
	leaves tree_node_5("tcp://10.11.1.207:6000",0);
	level_2.push_back(tree_node_4);
	level_2.push_back(tree_node_5);

	tree.push_back(level_0);
	tree.push_back(level_1);
	tree.push_back(level_2);

	int block_size=2048;
	printf("Master(0) or Slave(1) ?\n");
	choice=0;
	scanf("%d",&choice);

	if(choice==0)
	{
		std::cout<<"Environment initializing!"<<std::endl;
		Environment::getInstance(true);



		///////////////////////////////////////////Scan////////////////////////////////////////|
		std::vector<column_type> column_list;
		column_list.push_back(column_type(t_int));

		Schema* input=new SchemaFix(column_list);
		Schema* output=new SchemaFix(column_list);
		//SingleColumnScanIterator::State SCstate1("/home/imdb/temp/1_0.column_copy",input, output);
		SingleColumnScanIterator::State SCstate1("/home/imdb/temp/1_0.column.400k",input,output);
		Iterator* scs1=new SingleColumnScanIterator(SCstate1);

		///////////////////////////////////////////Exchange////////////////////////////////////|
		std::vector<std::string > uppernode;
		std::vector<std::string > lowernode;
		std::string node204("10.11.1.204");
		std::string node205("10.11.1.205");
		uppernode.push_back(node204);
		uppernode.push_back(node205);

		std::string node206("10.11.1.206");
		std::string node207("10.11.1.207");
		lowernode.push_back(node206);
		lowernode.push_back(node207);
		const unsigned long long int exchange_id_1=1;
		ExchangeIteratorWithWideDependency::State EIstate(input,scs1,block_size,lowernode,uppernode,exchange_id_1);
		Iterator* ei=new ExchangeIteratorWithWideDependency(EIstate);

		///////////////////////////////////////Filter//////////////////////////////////////////|
		FilterIterator::State FIstate;
		FIstate.child=ei;

		FIstate.input=output;
		FIstate.output=output;
		int f=0;
		FilterIterator::AttributeComparator filter1(column_type(t_int),Comparator::GEQ,0,&f);
		FIstate.ComparatorList.push_back(filter1);
		Iterator* fi=new FilterIterator(FIstate);

		///////////////////////////////////////////Exchange////////////////////////////////////|
		std::vector<std::string > uppernode_;
		std::vector<std::string > lowernode_;

		std::string node208_("10.11.1.208");
		uppernode_.push_back(node208_);

		std::string node204_("10.11.1.204");
		std::string node205_("10.11.1.205");
		lowernode_.push_back(node204_);
		lowernode_.push_back(node205_);
		const unsigned long long int exchange_id_0=0;
		ExchangeIteratorWithWideDependency::State EIstate_(output,fi,block_size,lowernode_,uppernode_,exchange_id_0);
		Iterator* ei_=new ExchangeIteratorWithWideDependency(EIstate_);

		///////////////////////////////////////Print///////////////////////////////////////////|
		PrintIterator::State PIstate(output,ei_);
		Iterator *pi=new PrintIterator(PIstate);

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


