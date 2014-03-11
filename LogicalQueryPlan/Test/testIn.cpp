/*
 * testIn.cpp
 *
 *  Created on: Mar 11, 2014
 *      Author: scdong
 */

#include <vector>
#include <iostream>
#include "../../Environment.h"
#include "../Scan.h"
#include "../LogicalQueryPlanRoot.h"
#include "../EqualJoin.h"
#include "../../Catalog/ProjectionBinding.h"
#include "../Filter.h"
#include "../Aggregation.h"
#include "../Buffer.h"
#include "../../utility/rdtsc.h"
using namespace std;
static int in_test(){
	int master;
	printf("~!OKOKO!!!!!\n");
	printf("Master(0) or Slave(others)??\n");
	scanf("%d",&master);
	if(master!=0){
		Environment::getInstance(false);
	}
	else{

		Environment::getInstance(true);

		ResourceManagerMaster *rmms=Environment::getInstance()->getResourceManagerMaster();
		Catalog* catalog=Environment::getInstance()->getCatalog();

		TableDescriptor* table_1=new TableDescriptor("cj",Environment::getInstance()->getCatalog()->allocate_unique_table_id());
		table_1->addAttribute("row_id",data_type(t_u_long),0,true);  				//0
		table_1->addAttribute("trade_date",data_type(t_int));
		table_1->addAttribute("order_no",data_type(t_u_long));
		table_1->addAttribute("sec_code",data_type(t_int));
		table_1->addAttribute("trade_dir",data_type(t_int));
		table_1->addAttribute("order_type",data_type(t_int));				//5
		table_1->addAttribute("trade_no",data_type(t_int));
		table_1->addAttribute("trade_time",data_type(t_int));
		table_1->addAttribute("trade_time_dec",data_type(t_u_long));
		table_1->addAttribute("order_time",data_type(t_int));
		table_1->addAttribute("order_time_dec",data_type(t_u_long));		//10
		table_1->addAttribute("trade_price",data_type(t_double));
		table_1->addAttribute("trade_amt",data_type(t_double));
		table_1->addAttribute("trade_vol",data_type(t_double));
		table_1->addAttribute("pbu_id",data_type(t_int));
		table_1->addAttribute("acct_id",data_type(t_int));					//15
		table_1->addAttribute("order_prtfil_code",data_type(t_int));
		table_1->addAttribute("tran_type",data_type(t_int));
		table_1->addAttribute("trade_type",data_type(t_int));
		table_1->addAttribute("proc_type",data_type(t_int));

		vector<ColumnOffset> cj_proj0_index;
		cj_proj0_index.push_back(0);
		cj_proj0_index.push_back(1);
		cj_proj0_index.push_back(2);
		cj_proj0_index.push_back(3);
		cj_proj0_index.push_back(4);
		cj_proj0_index.push_back(5);
//		const int partition_key_index_1=2;
		table_1->createHashPartitionedProjection(cj_proj0_index,"order_no",4);	//G0
//		table_1->createHashPartitionedProjection(cj_proj0_index,"row_id",4);	//G0
//		catalog->add_table(table_1);
		vector<ColumnOffset> cj_proj1_index;
		cj_proj1_index.push_back(0);
		cj_proj1_index.push_back(6);
		cj_proj1_index.push_back(7);
		cj_proj1_index.push_back(8);
		cj_proj1_index.push_back(9);
		cj_proj1_index.push_back(10);
		cj_proj1_index.push_back(11);
		cj_proj1_index.push_back(12);
		cj_proj1_index.push_back(13);
		cj_proj1_index.push_back(14);
		cj_proj1_index.push_back(15);
		cj_proj1_index.push_back(16);
		cj_proj1_index.push_back(17);
		cj_proj1_index.push_back(18);
		cj_proj1_index.push_back(18);

		table_1->createHashPartitionedProjection(cj_proj1_index,"row_id",4);	//G1

		catalog->add_table(table_1);

		////////////////////////////////////////
		/* the following codes should be triggered by Load module*/
		//////////////////ONE DAY////////////////////////////////////////////////
		//cj_table
		// 4 partitions partitioned by row_id
		for(unsigned i=0;i<table_1->getProjectoin(0)->getPartitioner()->getNumberOfPartitions();i++){

			catalog->getTable(0)->getProjectoin(0)->getPartitioner()->RegisterPartition(i,2);
		}

		for(unsigned i=0;i<table_1->getProjectoin(1)->getPartitioner()->getNumberOfPartitions();i++){

			catalog->getTable(0)->getProjectoin(1)->getPartitioner()->RegisterPartition(i,6);
		}
		/////////////////////////////////////////

		printf("ready(?)\n");
		int input;
		scanf("%d",&input);

		LogicalOperator* cj_join_key_scan=new LogicalScan(table_1->getProjectoin(0));

		LogicalOperator* cj_payload_scan=new LogicalScan(table_1->getProjectoin(1));

		//set child: row_id < 20;
		Filter::Condition filter_condition_1;
		filter_condition_1.add(table_1->getAttribute(0),FilterIterator::AttributeComparator::L,std::string("5"
				""));
		LogicalOperator* filter_1=new Filter(filter_condition_1,cj_payload_scan);

		//aggregation
		std::vector<Attribute> group_by_attributes;
		group_by_attributes.push_back(table_1->getAttribute("row_id"));
		std::vector<Attribute> aggregation_attributes;
		LogicalOperator* aggregation=new Aggregation(group_by_attributes,std::vector<Attribute>(),std::vector<BlockStreamAggregationIterator::State::aggregation>(),filter_1);

		//join
		std::vector<EqualJoin::JoinPair> join_pair_list;
		join_pair_list.push_back(EqualJoin::JoinPair(table_1->getAttribute("row_id"),aggregation->getDataflow().getAttribute("row_id")));
		LogicalOperator* sb_cj_join=new EqualJoin(join_pair_list,cj_join_key_scan,aggregation);

		const NodeID collector_node_id=0;
		LogicalOperator* root=new LogicalQueryPlanRoot(collector_node_id,sb_cj_join,LogicalQueryPlanRoot::PRINT);
		unsigned long long int timer_start=curtick();
//		root->print();

//		const NodeID collector_node_id=0;
//		LogicalOperator* root1=new LogicalQueryPlanRoot(0,sb_cj_join,LogicalQueryPlanRoot::PRINT);


//		root->getDataflow();

//		BlockStreamIteratorBase* executable_query_plan=root->getIteratorTree(1024-sizeof(unsigned));
			BlockStreamIteratorBase* executable_query_plan=root->getIteratorTree(1024*64-sizeof(unsigned));
			printf("query optimization time :%5.5f\n",getMilliSecond(timer_start));
			root->print();
			executable_query_plan->print();

		int c=1;
		while(c==1){
			timer_start=curtick();
//			IteratorExecutorMaster::getInstance()->ExecuteBlockStreamIteratorsOnSite(root1->getIteratorTree(1024*64-sizeof(unsigned)),"127.0.0.1");//						executable_query_plan->open();//			while(executable_query_plan->next(0));
			IteratorExecutorMaster::getInstance()->ExecuteBlockStreamIteratorsOnSite(executable_query_plan,"127.0.0.1");//						executable_query_plan->open();//			while(executable_query_plan->next(0));
//			executable_query_plan->close();
//
//			cout<<"Terminal(0) or continue(others)?"<<endl<<flush;
//			cin>>c;
			printf("Terminate(0) or continue(others)?\n");
//			sleep()
			scanf("%d",&c);
//			sleep(10);
//			getchar();
//			cout<<"<<<<<<<<<<<<<<<<<<<<<You input ="<<c+1<<endl<<flush;
			printf("you print=%d\n",c);
//			sleep(5);
		}
	}
	cout<<"Waiting~~~!~"<<endl;
	while(true){
		sleep(1);
	}

}


