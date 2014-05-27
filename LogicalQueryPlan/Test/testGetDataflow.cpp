/*
 * test.cpp
 *
 *  Created on: Nov 7, 2013
 *      Author: wangli
 */

#include "../LogicalOperator.h"
#include "../../Environment.h"
#include "../../Catalog/ProjectionBinding.h"
#include "../Scan.h"
#include "../EqualJoin.h"
#include "../Filter.h"
#include "../../common/AttributeComparator.h"
int main_asdfasdf(){
	Environment::getInstance(true);

	ResourceManagerMaster *rmms=Environment::getInstance()->getResourceManagerMaster();
	Catalog* catalog=Environment::getInstance()->getCatalog();
//	rmms->RegisterNewSlave("192.168.1.1");
//	rmms->RegisterNewSlave("192.168.1.2");
//	rmms->RegisterNewSlave("192.168.1.3");
//	rmms->RegisterNewSlave("192.168.1.4");
//	rmms->RegisterNewSlave("192.168.1.5");
//	rmms->RegisterDiskBuget(0,10000);
//	rmms->RegisterDiskBuget(1,10000);
//	rmms->RegisterDiskBuget(2,10000);
//	rmms->RegisterDiskBuget(3,10000);
//	rmms->RegisterDiskBuget(4,10000);


	/////////////////////////////////////Create table left/////////////////////
	TableDescriptor* table_1=new TableDescriptor("Left",Environment::getInstance()->getCatalog()->allocate_unique_table_id());
	table_1->addAttribute("Name",data_type(t_string),3);
	table_1->addAttribute("Age",data_type(t_int));
	table_1->addAttribute("Gender",data_type(t_int));
	table_1->addAttribute("Score",data_type(t_int));

	vector<ColumnOffset> index_1;
	index_1.push_back(0);
	index_1.push_back(1);
	index_1.push_back(3);
	const int partition_key_index_1=3;
	table_1->createHashPartitionedProjection(index_1,partition_key_index_1,3);
	catalog->add_table(table_1);

	////////////////////////////////////Create table right//////////////////////////
	TableDescriptor* table_2=new TableDescriptor("right",Environment::getInstance()->getCatalog()->allocate_unique_table_id());
	table_2->addAttribute("Name",data_type(t_string),10);
	table_2->addAttribute("Age",data_type(t_int));
	table_2->addAttribute("Gender",data_type(t_int));
	table_2->addAttribute("Score",data_type(t_int));

	vector<ColumnOffset> index_2;
	index_2.push_back(0);
	index_2.push_back(1);
	index_2.push_back(3);
	const int partition_key_index_2=3;
	table_2->createHashPartitionedProjection(index_2,partition_key_index_2,3);
	catalog->add_table(table_2);
	///////////////////////////////////////////////////////////








	///////////////////////////////////////


	////////////////////////////////////////
	/* the following codes should be triggered by Load module*/

	for(unsigned i=0;i<table_1->getProjectoin(0)->getPartitioner()->getNumberOfPartitions();i++){

		catalog->getTable(0)->getProjectoin(0)->getPartitioner()->RegisterPartition(i,5);
	}


	for(unsigned i=0;i<table_2->getProjectoin(0)->getPartitioner()->getNumberOfPartitions();i++){

		catalog->getTable(1)->getProjectoin(0)->getPartitioner()->RegisterPartition(i,4);
	}

	////////////////////////////////////////



	ProjectionBinding *pb=new ProjectionBinding();
	pb->BindingEntireProjection(catalog->getTable(0)->getProjectoin(0)->getPartitioner());
	pb->BindingEntireProjection(catalog->getTable(1)->getProjectoin(0)->getPartitioner());

	////scan////////
	std::vector<unsigned> index_list_1;
	index_list_1.push_back(0);
	index_list_1.push_back(1);
	index_list_1.push_back(3);
	LogicalOperator* scan_1=new LogicalScan(table_1->getAttributes(index_list_1));

	////////filter////////
	int f=0;
	AttributeComparator filter1(column_type(t_int),Comparator::EQ,2,&f);
	std::vector<AttributeComparator> ComparatorList;
	ComparatorList.push_back(filter1);
	LogicalOperator* filter=new Filter(ComparatorList,scan_1);

	////scan/////////
	std::vector<unsigned> index_list_2;
	index_list_2.push_back(0);
	index_list_2.push_back(1);
	index_list_2.push_back(3);
	LogicalOperator* scan_2=new LogicalScan(table_2->getAttributes(index_list_2));
	//////////////////


	////Join////////
	EqualJoin::JoinPair joinpair(table_1->getAttribute(3),table_2->getAttribute(3));
	std::vector<EqualJoin::JoinPair> pair_list;
	pair_list.push_back(joinpair);

	LogicalOperator* join=new EqualJoin(pair_list,filter,scan_2);


	Dataflow final_dataflow=join->getDataflow();
	printf("Total communication cost: %d\n",final_dataflow.property_.commnication_cost);





	printf("Waiting~\n");
	while(true){
		sleep(1);

	}


}

