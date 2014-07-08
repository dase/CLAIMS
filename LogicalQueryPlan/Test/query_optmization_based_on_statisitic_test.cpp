/*
 * query_optmization_based_on_statisitic_test.cpp
 *
 *  Created on: Feb 7, 2014
 *      Author: wangli
 */

#include <unistd.h>
#include <cstdio>
#include <iostream>
#include <vector>
#include <unistd.h>
#include <cstdio>
#include <iostream>
#include <vector>

#include "../../Catalog/Attribute.h"
#include "../../Catalog/Catalog.h"
#include "../../Catalog/Partitioner.h"
#include "../../Catalog/stat/Analyzer.h"
#include "../../Catalog/stat/Estimation.h"
#include "../../Catalog/stat/Statistic.h"
#include "../../Catalog/stat/StatManager.h"
#include "../../Catalog/table.h"
#include "../../Environment.h"
#include "../../common/ids.h"
#include "../../common/data_type.h"
#include "../../common/AttributeComparator.h"

#include "../../LogicalQueryPlan/Aggregation.h"
#include "../../LogicalQueryPlan/Buffer.h"
#include "../../LogicalQueryPlan/EqualJoin.h"
#include "../../LogicalQueryPlan/Filter.h"
#include "../../LogicalQueryPlan/LogicalQueryPlanRoot.h"
#include "../../LogicalQueryPlan/Scan.h"

#include "../../Parsetree/parsetree2logicalplan.cpp"
#include "../../Parsetree/runparsetree.cpp"
#include "../../Parsetree/sql_node_struct.h"
#include "../../Parsetree/ExecuteLogicalQueryPlan.h"
#include "../../utility/rdtsc.h"
#include "../../Test/set_up_environment.h"

//#define DEBUG_TestForSerialize

using namespace std;
static int query_optimization_based_on_statistics_join(){
	int master;
	//	cout<<"Master(0) or Slave(others)"<<endl;
	//	cin>>master;
	printf("~!OKOKO!!!!!\n");
	printf("Master(0) or Slave(others)??\n");
	scanf("%d", &master);
	if (master != 0) {
		Environment::getInstance(false);
	} else {

		Environment::getInstance(true);

		ResourceManagerMaster *rmms =
				Environment::getInstance()->getResourceManagerMaster();
		Catalog* catalog = Environment::getInstance()->getCatalog();

		TableDescriptor* table_1 =
				new TableDescriptor("cj",
						Environment::getInstance()->getCatalog()->allocate_unique_table_id());
		table_1->addAttribute("row_id", data_type(t_u_long), 0, true);  	//0
		table_1->addAttribute("trade_date", data_type(t_int));
		table_1->addAttribute("order_no", data_type(t_u_long), 0, true);
		table_1->addAttribute("sec_code", data_type(t_int));
		table_1->addAttribute("trade_dir", data_type(t_int));
		table_1->addAttribute("order_type", data_type(t_int));				//5
		table_1->addAttribute("trade_no", data_type(t_int), 0, true);
		table_1->addAttribute("trade_time", data_type(t_int));
		table_1->addAttribute("trade_time_dec", data_type(t_u_long));
		table_1->addAttribute("order_time", data_type(t_int));
		table_1->addAttribute("order_time_dec", data_type(t_u_long));		//10
		table_1->addAttribute("trade_price", data_type(t_double));
		table_1->addAttribute("trade_amt", data_type(t_double));
		table_1->addAttribute("trade_vol", data_type(t_double));
		table_1->addAttribute("pbu_id", data_type(t_int));
		table_1->addAttribute("acct_id", data_type(t_int));					//15
		table_1->addAttribute("order_prtfil_code", data_type(t_int));
		table_1->addAttribute("tran_type", data_type(t_int));
		table_1->addAttribute("trade_type", data_type(t_int));
		table_1->addAttribute("proc_type", data_type(t_int));

		vector<ColumnOffset> cj_proj0_index;
		cj_proj0_index.push_back(0);
		cj_proj0_index.push_back(1);
		cj_proj0_index.push_back(2);
		cj_proj0_index.push_back(3);
		cj_proj0_index.push_back(4);
		cj_proj0_index.push_back(5);
		const int partition_key_index_1 = 2;
		//		table_1->createHashPartitionedProjection(cj_proj0_index,"order_no",4);	//G0
		table_1->createHashPartitionedProjection(cj_proj0_index, "row_id", 1);//G0
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

		table_1->createHashPartitionedProjection(cj_proj1_index, "row_id", 1);//G1

		//		table_1->createHashPartitionedProjection(cj_proj0_index,"order_no",8);	//G2
		//		table_1->createHashPartitionedProjection(cj_proj1_index,"row_id",8);	//G3
		//
		//		//1 month
		//		// 4 partitions
		//		table_1->createHashPartitionedProjection(cj_proj0_index,"order_no",4);	//G4
		//		table_1->createHashPartitionedProjection(cj_proj1_index,"row_id",4);	//G5
		//		// 18 partitions
		//		table_1->createHashPartitionedProjection(cj_proj0_index,"order_no",8);	//G6
		//		table_1->createHashPartitionedProjection(cj_proj1_index,"row_id",8);	//G7
		//
		//		// 5 days
		//		table_1->createHashPartitionedProjection(cj_proj0_index,"order_no",4);	//G8
		//		table_1->createHashPartitionedProjection(cj_proj1_index,"row_id",4);	//G9
		//
		//		// 1 month 8 partitions
		//		table_1->createHashPartitionedProjection(cj_proj0_index,"order_no",8);	//G10
		//		table_1->createHashPartitionedProjection(cj_proj1_index,"row_id",8);	//G11
		//
		//		// 5 days 8 partitions
		//		table_1->createHashPartitionedProjection(cj_proj0_index,"order_no",8);	//G12
		//		table_1->createHashPartitionedProjection(cj_proj1_index,"row_id",8);	//G13
		//
		//
		//		// 1 day 4 partitions by row_id
		//		table_1->createHashPartitionedProjection(cj_proj0_index,"row_id",4);	//G14
		catalog->add_table(table_1);

		////////////////////////////////////Create table right//////////////////////////
		TableDescriptor* table_2=new TableDescriptor("sb",Environment::getInstance()->getCatalog()->allocate_unique_table_id());
		table_2->addAttribute("row_id",data_type(t_u_long),0,true);
		table_2->addAttribute("order_no",data_type(t_u_long),0,true);
		table_2->addAttribute("entry_date",data_type(t_int));
		table_2->addAttribute("sec_code",data_type(t_int));
		table_2->addAttribute("order_type",data_type(t_int));
		table_2->addAttribute("entry_dir",data_type(t_int));			//5
		table_2->addAttribute("tran_maint_code",data_type(t_int));
		table_2->addAttribute("Last_upd_date",data_type(t_int));
		table_2->addAttribute("Last_upd_time",data_type(t_int));
		table_2->addAttribute("Last_upd_time_dec",data_type(t_u_long));
		table_2->addAttribute("entry_time",data_type(t_int));			//10
		table_2->addAttribute("entry_time_dec",data_type(t_double));
		table_2->addAttribute("order_price",data_type(t_double));
		table_2->addAttribute("order_exec_vol",data_type(t_double));
		table_2->addAttribute("order_vol",data_type(t_double));
		table_2->addAttribute("pbu_id",data_type(t_int));				//15
		table_2->addAttribute("acct_id",data_type(t_int));
		table_2->addAttribute("acct_attr",data_type(t_int));
		table_2->addAttribute("branch_id",data_type(t_int));
		table_2->addAttribute("pbu_inter_order_no",data_type(t_int));
		table_2->addAttribute("pub_inter_txt",data_type(t_int));		//20
		table_2->addAttribute("aud_type",data_type(t_int));
		table_2->addAttribute("trade_restr_type",data_type(t_int));
		table_2->addAttribute("order_star",data_type(t_int));
		table_2->addAttribute("order_restr_type",data_type(t_int));
		table_2->addAttribute("short_sell_flag",data_type(t_int));		//25





		vector<ColumnOffset> sb_proj0_index;
		sb_proj0_index.push_back(0);
		sb_proj0_index.push_back(1);
		sb_proj0_index.push_back(2);
		sb_proj0_index.push_back(3);
		sb_proj0_index.push_back(4);
		sb_proj0_index.push_back(5);

		//		table_2->createHashPartitionedProjection(sb_proj0_index,"order_no",4);	//G0
		table_2->createHashPartitionedProjection(sb_proj0_index,"row_id",1);	//G0




		vector<ColumnOffset> sb_proj1_index;
		sb_proj1_index.push_back(0);
		sb_proj1_index.push_back(6);
		sb_proj1_index.push_back(7);
		sb_proj1_index.push_back(8);
		sb_proj1_index.push_back(9);
		sb_proj1_index.push_back(10);
		sb_proj1_index.push_back(11);
		sb_proj1_index.push_back(12);
		sb_proj1_index.push_back(13);
		sb_proj1_index.push_back(14);
		sb_proj1_index.push_back(15);
		sb_proj1_index.push_back(16);
		sb_proj1_index.push_back(17);
		sb_proj1_index.push_back(18);
		sb_proj1_index.push_back(19);
		sb_proj1_index.push_back(20);
		sb_proj1_index.push_back(21);
		sb_proj1_index.push_back(22);
		sb_proj1_index.push_back(23);
		sb_proj1_index.push_back(24);
		sb_proj1_index.push_back(25);




		table_2->createHashPartitionedProjection(sb_proj1_index,"row_id",1);	//G1


		//		table_2->createHashPartitionedProjection(sb_proj0_index,"order_no",8);	//G2
		//		table_2->createHashPartitionedProjection(sb_proj1_index,"row_id",8);	//G3
		//
		//		// 1 month
		//		// 4 partitions
		//		table_2->createHashPartitionedProjection(sb_proj0_index,"order_no",4);	//G4
		//		table_2->createHashPartitionedProjection(sb_proj1_index,"row_id",4);	//G5
		//
		//		// 18 partitions
		//		table_2->createHashPartitionedProjection(sb_proj0_index,"order_no",8);	//G6
		//		table_2->createHashPartitionedProjection(sb_proj1_index,"row_id",8);	//G7
		//
		//
		//		//5 days
		//		table_2->createHashPartitionedProjection(sb_proj0_index,"order_no",4);	//G8
		//		table_2->createHashPartitionedProjection(sb_proj1_index,"row_id",4);	//G9
		//
		//		// 1 month 8 partitions
		//		table_2->createHashPartitionedProjection(sb_proj0_index,"order_no",8);	//G10
		//		table_2->createHashPartitionedProjection(sb_proj1_index,"row_id",8);	//G11
		//
		//		// 5 days 8 partitions
		//		table_2->createHashPartitionedProjection(sb_proj0_index,"order_no",8);	//G12
		//		table_2->createHashPartitionedProjection(sb_proj1_index,"row_id",8);	//G13
		//
		//		// 1 day 4 partitions by row_id
		//		table_2->createHashPartitionedProjection(sb_proj0_index,"row_id",4);	//G14

		catalog->add_table(table_2);
		///////////////////////////////////////////////////////////

		///////////////////////////////////////

		////////////////////////////////////////
		/* the following codes should be triggered by Load module*/
		//////////////////ONE DAY////////////////////////////////////////////////
		//cj_table
		// 4 partitions partitioned by order_no
		for (unsigned i = 0;
				i
				< table_1->getProjectoin(0)->getPartitioner()->getNumberOfPartitions();
				i++) {

			catalog->getTable(0)->getProjectoin(0)->getPartitioner()->RegisterPartition(
					i, 2);
		}

		for (unsigned i = 0;
				i
				< table_1->getProjectoin(1)->getPartitioner()->getNumberOfPartitions();
				i++) {

			catalog->getTable(0)->getProjectoin(1)->getPartitioner()->RegisterPartition(
					i, 6);
		}
		//partitioned by row_id
		//		for(unsigned i=0;i<table_1->getProjectoin(14)->getPartitioner()->getNumberOfPartitions();i++){
		//
		//			catalog->getTable(0)->getProjectoin(14)->getPartitioner()->RegisterPartition(i,2);
		//		}
		//
		//		// 8 partitions
		//		for(unsigned i=0;i<table_1->getProjectoin(2)->getPartitioner()->getNumberOfPartitions();i++){
		//
		//			catalog->getTable(0)->getProjectoin(2)->getPartitioner()->RegisterPartition(i,1);
		//		}
		//
		//		for(unsigned i=0;i<table_1->getProjectoin(3)->getPartitioner()->getNumberOfPartitions();i++){
		//
		//			catalog->getTable(0)->getProjectoin(3)->getPartitioner()->RegisterPartition(i,3);
		//		}

		//sb_table
		for (unsigned i = 0;
				i
				< table_2->getProjectoin(0)->getPartitioner()->getNumberOfPartitions();
				i++) {

			catalog->getTable(1)->getProjectoin(0)->getPartitioner()->RegisterPartition(
					i, 2);
		}

		for (unsigned i = 0;
				i
				< table_2->getProjectoin(1)->getPartitioner()->getNumberOfPartitions();
				i++) {

			catalog->getTable(1)->getProjectoin(1)->getPartitioner()->RegisterPartition(
					i, 6);
		}
		//		for(unsigned i=0;i<table_2->getProjectoin(2)->getPartitioner()->getNumberOfPartitions();i++){
		//
		//			catalog->getTable(1)->getProjectoin(2)->getPartitioner()->RegisterPartition(i,1);
		//		}
		//
		//		for(unsigned i=0;i<table_2->getProjectoin(3)->getPartitioner()->getNumberOfPartitions();i++){
		//
		//			catalog->getTable(1)->getProjectoin(3)->getPartitioner()->RegisterPartition(i,3);
		//		}
		//
		//		//partitioned by row_id
		//		for(unsigned i=0;i<table_2->getProjectoin(14)->getPartitioner()->getNumberOfPartitions();i++){
		//
		//			catalog->getTable(1)->getProjectoin(14)->getPartitioner()->RegisterPartition(i,2);
		//		}
		//
		//		////////////////////////////////////////
		//
		//		///////////////////ONE MONTH/////////////////////////////////////////////////////////////
		//		//CJ
		//		// 4 partition
		//		for(unsigned i=0;i<table_1->getProjectoin(4)->getPartitioner()->getNumberOfPartitions();i++){
		//
		//			catalog->getTable(0)->getProjectoin(4)->getPartitioner()->RegisterPartition(i,40);
		//		}
		//
		//		for(unsigned i=0;i<table_1->getProjectoin(5)->getPartitioner()->getNumberOfPartitions();i++){
		//
		//			catalog->getTable(0)->getProjectoin(5)->getPartitioner()->RegisterPartition(i,104);
		//		}
		//		//8 partitions
		//		for(unsigned i=0;i<table_1->getProjectoin(10)->getPartitioner()->getNumberOfPartitions();i++){
		//
		//			catalog->getTable(0)->getProjectoin(10)->getPartitioner()->RegisterPartition(i,20);
		//		}
		//
		//		for(unsigned i=0;i<table_1->getProjectoin(11)->getPartitioner()->getNumberOfPartitions();i++){
		//
		//			catalog->getTable(0)->getProjectoin(11)->getPartitioner()->RegisterPartition(i,52);
		//		}
		//		// 18 partitions
		//		for(unsigned i=0;i<table_1->getProjectoin(6)->getPartitioner()->getNumberOfPartitions();i++){
		//
		//			catalog->getTable(0)->getProjectoin(6)->getPartitioner()->RegisterPartition(i,10);
		//		}
		//
		//		for(unsigned i=0;i<table_1->getProjectoin(7)->getPartitioner()->getNumberOfPartitions();i++){
		//
		//			catalog->getTable(0)->getProjectoin(7)->getPartitioner()->RegisterPartition(i,24);
		//		}
		//
		//		//SB
		//		// 4 partition
		//		for(unsigned i=0;i<table_2->getProjectoin(4)->getPartitioner()->getNumberOfPartitions();i++){
		//
		//			catalog->getTable(1)->getProjectoin(4)->getPartitioner()->RegisterPartition(i,39);
		//		}
		//
		//		for(unsigned i=0;i<table_2->getProjectoin(5)->getPartitioner()->getNumberOfPartitions();i++){
		//
		//			catalog->getTable(1)->getProjectoin(5)->getPartitioner()->RegisterPartition(i,131);
		//		}
		//		// 8 partitions
		//		for(unsigned i=0;i<table_2->getProjectoin(10)->getPartitioner()->getNumberOfPartitions();i++){
		//
		//			catalog->getTable(1)->getProjectoin(10)->getPartitioner()->RegisterPartition(i,20);
		//		}
		//
		//		for(unsigned i=0;i<table_2->getProjectoin(11)->getPartitioner()->getNumberOfPartitions();i++){
		//
		//			catalog->getTable(1)->getProjectoin(11)->getPartitioner()->RegisterPartition(i,66);
		//		}
		//		// 18 partitions
		//		for(unsigned i=0;i<table_2->getProjectoin(6)->getPartitioner()->getNumberOfPartitions();i++){
		//
		//			catalog->getTable(1)->getProjectoin(6)->getPartitioner()->RegisterPartition(i,10);
		//		}
		//
		//		for(unsigned i=0;i<table_2->getProjectoin(7)->getPartitioner()->getNumberOfPartitions();i++){
		//
		//			catalog->getTable(1)->getProjectoin(7)->getPartitioner()->RegisterPartition(i,30);
		//		}
		//		////////////////////////////////
		//
		//
		//		///////////// FIVE DAYS //////////////////
		//		//// cj////
		//		// 4 partitions
		//		for(unsigned i=0;i<table_1->getProjectoin(8)->getPartitioner()->getNumberOfPartitions();i++){
		//
		//			catalog->getTable(0)->getProjectoin(8)->getPartitioner()->RegisterPartition(i,14);
		//		}
		//
		//		for(unsigned i=0;i<table_1->getProjectoin(9)->getPartitioner()->getNumberOfPartitions();i++){
		//
		//			catalog->getTable(0)->getProjectoin(9)->getPartitioner()->RegisterPartition(i,36);
		//		}
		//		// 8 partitions
		//		for(unsigned i=0;i<table_1->getProjectoin(12)->getPartitioner()->getNumberOfPartitions();i++){
		//
		//			catalog->getTable(0)->getProjectoin(12)->getPartitioner()->RegisterPartition(i,7);
		//		}
		//
		//		for(unsigned i=0;i<table_1->getProjectoin(13)->getPartitioner()->getNumberOfPartitions();i++){
		//
		//			catalog->getTable(0)->getProjectoin(13)->getPartitioner()->RegisterPartition(i,19);
		//		}
		//
		//		//// sb ////
		//		// 4 partitions//
		//		for(unsigned i=0;i<table_2->getProjectoin(8)->getPartitioner()->getNumberOfPartitions();i++){
		//
		//			catalog->getTable(1)->getProjectoin(8)->getPartitioner()->RegisterPartition(i,14);
		//		}
		//
		//		for(unsigned i=0;i<table_2->getProjectoin(9)->getPartitioner()->getNumberOfPartitions();i++){
		//
		//			catalog->getTable(1)->getProjectoin(9)->getPartitioner()->RegisterPartition(i,131);
		//		}
		//		// 8 partitions//
		//		for(unsigned i=0;i<table_2->getProjectoin(12)->getPartitioner()->getNumberOfPartitions();i++){
		//
		//			catalog->getTable(1)->getProjectoin(12)->getPartitioner()->RegisterPartition(i,7);
		//		}
		//
		//		for(unsigned i=0;i<table_2->getProjectoin(13)->getPartitioner()->getNumberOfPartitions();i++){
		//
		//			catalog->getTable(1)->getProjectoin(13)->getPartitioner()->RegisterPartition(i,23);
		//		}

		TableID table_id = catalog->getTable("cj")->get_table_id();
		Attribute att;
		Analyzer::analyse(table_id,Analyzer::a_l_attribute);

		table_id=catalog->getTable("sb")->get_table_id();
		Analyzer::analyse(table_id,Analyzer::a_l_attribute);


		int cont=1;
		while(cout){
			//			printf("SQL:\n");
			//			puts("aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa\n");
			//			Node* node=getparsetreeroot();
			//			if(node==0){
			//				printf("Error occurs while parsing the sql.\n");
			//				break;
			//			}
			//			output(node,0);
			//			puts("aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa\n");
			//			LogicalOperator* plan=parsetree2logicalplan(node);




			LogicalOperator* cj_join_key_scan=new LogicalScan(table_1->getProjectoin(0));
			Filter::Condition filter_condition_1;
			//			const int order_type=1;
			filter_condition_1.add(table_1->getAttribute(5),AttributeComparator::EQ,std::string("1"));
			//			const int trade_date=20101008;
			filter_condition_1.add(table_1->getAttribute(1),AttributeComparator::GEQ,std::string("20101008"));
			int sec_code;
			printf("sec_code=%?\n");
			scanf("%d",&sec_code);
			filter_condition_1.add(table_1->getAttribute(3),AttributeComparator::EQ,&sec_code);
			LogicalOperator* filter_1=new Filter(filter_condition_1,cj_join_key_scan);


			LogicalOperator* sb_join_key_scan=new LogicalScan(table_2->getProjectoin(0));
			Filter::Condition filter_condition_2;
			//			const int order_type=1;
			filter_condition_2.add(table_2->getAttribute(4),AttributeComparator::EQ,std::string("1"));
			//			const int trade_date=20101008;
			filter_condition_2.add(table_2->getAttribute(2),AttributeComparator::GEQ,std::string("20101008"));
			//			int sec_code;
			//			printf("sec_code=%?\n");
			//			scanf("%d",&sec_code);
			//			filter_condition_2.add(table_2->getAttribute(3),AttributeComparator::EQ,&sec_code);
			LogicalOperator* filter_2=new Filter(filter_condition_2,sb_join_key_scan);


			std::vector<EqualJoin::JoinPair> sb_cj_join_pair_list;
			sb_cj_join_pair_list.push_back(EqualJoin::JoinPair(table_1->getAttribute("order_no"),table_2->getAttribute("order_no")));
			sb_cj_join_pair_list.push_back(EqualJoin::JoinPair(table_1->getAttribute("trade_date"),table_2->getAttribute("entry_date")));
			sb_cj_join_pair_list.push_back(EqualJoin::JoinPair(table_1->getAttribute("trade_dir"),table_2->getAttribute("entry_dir")));
			//		sb_cj_join_pair_list.push_back(EqualJoin::JoinPair(table_1->getAttribute("row_id"),table_2->getAttribute("row_id")));
			LogicalOperator* sb_cj_join=new EqualJoin(sb_cj_join_pair_list,filter_1,filter_2);

			const NodeID collector_node_id=0;
			LogicalOperator* root=new LogicalQueryPlanRoot(0,sb_cj_join,LogicalQueryPlanRoot::PERFORMANCE);
			unsigned long long int timer_start=curtick();

			BlockStreamIteratorBase* please=root->getIteratorTree(64*1024-sizeof(unsigned));
			//			root->print();
			//
			//
			//			please->print();
			//
			IteratorExecutorMaster::getInstance()->ExecuteBlockStreamIteratorsOnSite(please,"127.0.0.1");//



			printf("Continue(1) or not (others)?\n");
			scanf("%d",&cout);
		}

		/////////////////////////////////////////

		//	sleep(1);
		//		cout<<"ready(?)"<<endl;
		printf("ready(?)\n");
		int input;
		scanf("%d", &input);
		//		cin>>input;

		//
		cout << "Waiting~~~!~" << endl;
		while (true) {
			sleep(1);
		}
	}

}
static int query_optimization_based_on_statistics_aggregation(){
	int master;
	//	cout<<"Master(0) or Slave(others)"<<endl;
	//	cin>>master;
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
		table_1->addAttribute("order_no",data_type(t_u_long),0,true);
		table_1->addAttribute("sec_code",data_type(t_int));
		table_1->addAttribute("trade_dir",data_type(t_int));
		table_1->addAttribute("order_type",data_type(t_int));				//5
		table_1->addAttribute("trade_no",data_type(t_int),0,true);
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
		const int partition_key_index_1=2;
		//		table_1->createHashPartitionedProjection(cj_proj0_index,"order_no",4);	//G0
		table_1->createHashPartitionedProjection(cj_proj0_index,"row_id",4);	//G0
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

		table_1->createHashPartitionedProjection(cj_proj0_index,"order_no",8);	//G2
		table_1->createHashPartitionedProjection(cj_proj1_index,"row_id",8);	//G3

		//1 month
		// 4 partitions
		table_1->createHashPartitionedProjection(cj_proj0_index,"order_no",4);	//G4
		table_1->createHashPartitionedProjection(cj_proj1_index,"row_id",4);	//G5
		// 18 partitions
		table_1->createHashPartitionedProjection(cj_proj0_index,"order_no",8);	//G6
		table_1->createHashPartitionedProjection(cj_proj1_index,"row_id",8);	//G7
		//
		//		// 5 days
		//		table_1->createHashPartitionedProjection(cj_proj0_index,"order_no",4);	//G8
		//		table_1->createHashPartitionedProjection(cj_proj1_index,"row_id",4);	//G9
		//
		//		// 1 month 8 partitions
		//		table_1->createHashPartitionedProjection(cj_proj0_index,"order_no",8);	//G10
		//		table_1->createHashPartitionedProjection(cj_proj1_index,"row_id",8);	//G11
		//
		//		// 5 days 8 partitions
		//		table_1->createHashPartitionedProjection(cj_proj0_index,"order_no",8);	//G12
		//		table_1->createHashPartitionedProjection(cj_proj1_index,"row_id",8);	//G13
		//
		//
		//		// 1 day 4 partitions by row_id
		//		table_1->createHashPartitionedProjection(cj_proj0_index,"row_id",4);	//G14
		catalog->add_table(table_1);

		////////////////////////////////////Create table right//////////////////////////
		TableDescriptor* table_2=new TableDescriptor("sb",Environment::getInstance()->getCatalog()->allocate_unique_table_id());
		table_2->addAttribute("row_id",data_type(t_u_long),0,true);
		table_2->addAttribute("order_no",data_type(t_u_long),0,true);
		table_2->addAttribute("entry_date",data_type(t_int));
		table_2->addAttribute("sec_code",data_type(t_int));
		table_2->addAttribute("order_type",data_type(t_int));
		table_2->addAttribute("entry_dir",data_type(t_int));			//5
		table_2->addAttribute("tran_maint_code",data_type(t_int));
		table_2->addAttribute("Last_upd_date",data_type(t_int));
		table_2->addAttribute("Last_upd_time",data_type(t_int));
		table_2->addAttribute("Last_upd_time_dec",data_type(t_u_long));
		table_2->addAttribute("entry_time",data_type(t_int));			//10
		table_2->addAttribute("entry_time_dec",data_type(t_double));
		table_2->addAttribute("order_price",data_type(t_double));
		table_2->addAttribute("order_exec_vol",data_type(t_double));
		table_2->addAttribute("order_vol",data_type(t_double));
		table_2->addAttribute("pbu_id",data_type(t_int));				//15
		table_2->addAttribute("acct_id",data_type(t_int));
		table_2->addAttribute("acct_attr",data_type(t_int));
		table_2->addAttribute("branch_id",data_type(t_int));
		table_2->addAttribute("pbu_inter_order_no",data_type(t_int));
		table_2->addAttribute("pub_inter_txt",data_type(t_int));		//20
		table_2->addAttribute("aud_type",data_type(t_int));
		table_2->addAttribute("trade_restr_type",data_type(t_int));
		table_2->addAttribute("order_star",data_type(t_int));
		table_2->addAttribute("order_restr_type",data_type(t_int));
		table_2->addAttribute("short_sell_flag",data_type(t_int));		//25















		vector<ColumnOffset> sb_proj0_index;
		sb_proj0_index.push_back(0);
		sb_proj0_index.push_back(1);
		sb_proj0_index.push_back(2);
		sb_proj0_index.push_back(3);
		sb_proj0_index.push_back(4);
		sb_proj0_index.push_back(5);

		//		table_2->createHashPartitionedProjection(sb_proj0_index,"order_no",4);	//G0
		table_2->createHashPartitionedProjection(sb_proj0_index,"row_id",4);	//G0



		vector<ColumnOffset> sb_proj1_index;
		sb_proj1_index.push_back(0);
		sb_proj1_index.push_back(6);
		sb_proj1_index.push_back(7);
		sb_proj1_index.push_back(8);
		sb_proj1_index.push_back(9);
		sb_proj1_index.push_back(10);
		sb_proj1_index.push_back(11);
		sb_proj1_index.push_back(12);
		sb_proj1_index.push_back(13);
		sb_proj1_index.push_back(14);
		sb_proj1_index.push_back(15);
		sb_proj1_index.push_back(16);
		sb_proj1_index.push_back(17);
		sb_proj1_index.push_back(18);
		sb_proj1_index.push_back(19);
		sb_proj1_index.push_back(20);
		sb_proj1_index.push_back(21);
		sb_proj1_index.push_back(22);
		sb_proj1_index.push_back(23);
		sb_proj1_index.push_back(24);
		sb_proj1_index.push_back(25);




		table_2->createHashPartitionedProjection(sb_proj1_index,"row_id",4);	//G1

		table_2->createHashPartitionedProjection(sb_proj0_index,"order_no",8);	//G2
		table_2->createHashPartitionedProjection(sb_proj1_index,"row_id",8);	//G3

		// 1 month
		// 4 partitions
		table_2->createHashPartitionedProjection(sb_proj0_index,"order_no",4);	//G4
		table_2->createHashPartitionedProjection(sb_proj1_index,"row_id",4);	//G5

		// 18 partitions
		table_2->createHashPartitionedProjection(sb_proj0_index,"order_no",8);	//G6
		table_2->createHashPartitionedProjection(sb_proj1_index,"row_id",8);	//G7
		//
		//
		//		//5 days
		//		table_2->createHashPartitionedProjection(sb_proj0_index,"order_no",4);	//G8
		//		table_2->createHashPartitionedProjection(sb_proj1_index,"row_id",4);	//G9
		//
		//		// 1 month 8 partitions
		//		table_2->createHashPartitionedProjection(sb_proj0_index,"order_no",8);	//G10
		//		table_2->createHashPartitionedProjection(sb_proj1_index,"row_id",8);	//G11
		//
		//		// 5 days 8 partitions
		//		table_2->createHashPartitionedProjection(sb_proj0_index,"order_no",8);	//G12
		//		table_2->createHashPartitionedProjection(sb_proj1_index,"row_id",8);	//G13
		//
		//		// 1 day 4 partitions by row_id
		//		table_2->createHashPartitionedProjection(sb_proj0_index,"row_id",4);	//G14


		catalog->add_table(table_2);
		///////////////////////////////////////////////////////////








		///////////////////////////////////////


		////////////////////////////////////////
		/* the following codes should be triggered by Load module*/
		//////////////////ONE DAY////////////////////////////////////////////////
		//cj_table
		// 4 partitions partitioned by order_no
		for(unsigned i=0;i<table_1->getProjectoin(0)->getPartitioner()->getNumberOfPartitions();i++){

			catalog->getTable(0)->getProjectoin(0)->getPartitioner()->RegisterPartition(i,2);
		}

		for(unsigned i=0;i<table_1->getProjectoin(1)->getPartitioner()->getNumberOfPartitions();i++){

			catalog->getTable(0)->getProjectoin(1)->getPartitioner()->RegisterPartition(i,6);
		}
		//partitioned by row_id
		//		for(unsigned i=0;i<table_1->getProjectoin(14)->getPartitioner()->getNumberOfPartitions();i++){
		//
		//			catalog->getTable(0)->getProjectoin(14)->getPartitioner()->RegisterPartition(i,2);
		//		}
		//
		//		// 8 partitions
		for(unsigned i=0;i<table_1->getProjectoin(2)->getPartitioner()->getNumberOfPartitions();i++){

			catalog->getTable(0)->getProjectoin(2)->getPartitioner()->RegisterPartition(i,1);
		}

		for(unsigned i=0;i<table_1->getProjectoin(3)->getPartitioner()->getNumberOfPartitions();i++){

			catalog->getTable(0)->getProjectoin(3)->getPartitioner()->RegisterPartition(i,3);
		}

		//sb_table
		for(unsigned i=0;i<table_2->getProjectoin(0)->getPartitioner()->getNumberOfPartitions();i++){

			catalog->getTable(1)->getProjectoin(0)->getPartitioner()->RegisterPartition(i,2);
		}

		for(unsigned i=0;i<table_2->getProjectoin(1)->getPartitioner()->getNumberOfPartitions();i++){

			catalog->getTable(1)->getProjectoin(1)->getPartitioner()->RegisterPartition(i,6);
		}
		for(unsigned i=0;i<table_2->getProjectoin(2)->getPartitioner()->getNumberOfPartitions();i++){

			catalog->getTable(1)->getProjectoin(2)->getPartitioner()->RegisterPartition(i,1);
		}

		for(unsigned i=0;i<table_2->getProjectoin(3)->getPartitioner()->getNumberOfPartitions();i++){

			catalog->getTable(1)->getProjectoin(3)->getPartitioner()->RegisterPartition(i,3);
		}
		//
		//		//partitioned by row_id
		//		for(unsigned i=0;i<table_2->getProjectoin(14)->getPartitioner()->getNumberOfPartitions();i++){
		//
		//			catalog->getTable(1)->getProjectoin(14)->getPartitioner()->RegisterPartition(i,2);
		//		}
		//
		//		////////////////////////////////////////
		//
		//		///////////////////ONE MONTH/////////////////////////////////////////////////////////////
		//		//CJ
		//		// 4 partition
		for(unsigned i=0;i<table_1->getProjectoin(4)->getPartitioner()->getNumberOfPartitions();i++){

			catalog->getTable(0)->getProjectoin(4)->getPartitioner()->RegisterPartition(i,40);
		}

		for(unsigned i=0;i<table_1->getProjectoin(5)->getPartitioner()->getNumberOfPartitions();i++){

			catalog->getTable(0)->getProjectoin(5)->getPartitioner()->RegisterPartition(i,104);
		}
		//		//8 partitions
		//		for(unsigned i=0;i<table_1->getProjectoin(10)->getPartitioner()->getNumberOfPartitions();i++){
		//
		//			catalog->getTable(0)->getProjectoin(10)->getPartitioner()->RegisterPartition(i,20);
		//		}
		//
		//		for(unsigned i=0;i<table_1->getProjectoin(11)->getPartitioner()->getNumberOfPartitions();i++){
		//
		//			catalog->getTable(0)->getProjectoin(11)->getPartitioner()->RegisterPartition(i,52);
		//		}
		//		// 18 partitions
		for(unsigned i=0;i<table_1->getProjectoin(6)->getPartitioner()->getNumberOfPartitions();i++){

			catalog->getTable(0)->getProjectoin(6)->getPartitioner()->RegisterPartition(i,10);
		}

		for(unsigned i=0;i<table_1->getProjectoin(7)->getPartitioner()->getNumberOfPartitions();i++){

			catalog->getTable(0)->getProjectoin(7)->getPartitioner()->RegisterPartition(i,24);
		}
		//
		//		//SB
		//		// 4 partition
		for(unsigned i=0;i<table_2->getProjectoin(4)->getPartitioner()->getNumberOfPartitions();i++){

			catalog->getTable(1)->getProjectoin(4)->getPartitioner()->RegisterPartition(i,39);
		}

		for(unsigned i=0;i<table_2->getProjectoin(5)->getPartitioner()->getNumberOfPartitions();i++){

			catalog->getTable(1)->getProjectoin(5)->getPartitioner()->RegisterPartition(i,131);
		}
		//		// 8 partitions
		//		for(unsigned i=0;i<table_2->getProjectoin(10)->getPartitioner()->getNumberOfPartitions();i++){
		//
		//			catalog->getTable(1)->getProjectoin(10)->getPartitioner()->RegisterPartition(i,20);
		//		}
		//
		//		for(unsigned i=0;i<table_2->getProjectoin(11)->getPartitioner()->getNumberOfPartitions();i++){
		//
		//			catalog->getTable(1)->getProjectoin(11)->getPartitioner()->RegisterPartition(i,66);
		//		}
		//		// 18 partitions
		for(unsigned i=0;i<table_2->getProjectoin(6)->getPartitioner()->getNumberOfPartitions();i++){

			catalog->getTable(1)->getProjectoin(6)->getPartitioner()->RegisterPartition(i,10);
		}

		for(unsigned i=0;i<table_2->getProjectoin(7)->getPartitioner()->getNumberOfPartitions();i++){

			catalog->getTable(1)->getProjectoin(7)->getPartitioner()->RegisterPartition(i,30);
		}
		//		////////////////////////////////
		//
		//
		//		///////////// FIVE DAYS //////////////////
		//		//// cj////
		//		// 4 partitions
		//		for(unsigned i=0;i<table_1->getProjectoin(8)->getPartitioner()->getNumberOfPartitions();i++){
		//
		//			catalog->getTable(0)->getProjectoin(8)->getPartitioner()->RegisterPartition(i,14);
		//		}
		//
		//		for(unsigned i=0;i<table_1->getProjectoin(9)->getPartitioner()->getNumberOfPartitions();i++){
		//
		//			catalog->getTable(0)->getProjectoin(9)->getPartitioner()->RegisterPartition(i,36);
		//		}
		//		// 8 partitions
		//		for(unsigned i=0;i<table_1->getProjectoin(12)->getPartitioner()->getNumberOfPartitions();i++){
		//
		//			catalog->getTable(0)->getProjectoin(12)->getPartitioner()->RegisterPartition(i,7);
		//		}
		//
		//		for(unsigned i=0;i<table_1->getProjectoin(13)->getPartitioner()->getNumberOfPartitions();i++){
		//
		//			catalog->getTable(0)->getProjectoin(13)->getPartitioner()->RegisterPartition(i,19);
		//		}
		//
		//		//// sb ////
		//		// 4 partitions//
		//		for(unsigned i=0;i<table_2->getProjectoin(8)->getPartitioner()->getNumberOfPartitions();i++){
		//
		//			catalog->getTable(1)->getProjectoin(8)->getPartitioner()->RegisterPartition(i,14);
		//		}
		//
		//		for(unsigned i=0;i<table_2->getProjectoin(9)->getPartitioner()->getNumberOfPartitions();i++){
		//
		//			catalog->getTable(1)->getProjectoin(9)->getPartitioner()->RegisterPartition(i,131);
		//		}
		//		// 8 partitions//
		//		for(unsigned i=0;i<table_2->getProjectoin(12)->getPartitioner()->getNumberOfPartitions();i++){
		//
		//			catalog->getTable(1)->getProjectoin(12)->getPartitioner()->RegisterPartition(i,7);
		//		}
		//
		//		for(unsigned i=0;i<table_2->getProjectoin(13)->getPartitioner()->getNumberOfPartitions();i++){
		//
		//			catalog->getTable(1)->getProjectoin(13)->getPartitioner()->RegisterPartition(i,23);
		//		}

		printf("continue?\n");
		scanf("%d");

		TableID table_id=catalog->getTable("cj")->get_table_id();
		Attribute att;
		//		Analyzer::analyse(table_id,Analyzer::a_l_attribute);
		Analyzer::analyse_on_project(catalog->getTable("cj")->getProjectoin(0)->getProjectionID());

		table_id=catalog->getTable("sb")->get_table_id();
		//		Analyzer::analyse(table_id,Analyzer::a_l_attribute);

		int cont=1;
		while(cout){
			//			printf("SQL:\n");
			//			puts("aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa\n");
			//			Node* node=getparsetreeroot();
			//			if(node==0){
			//				printf("Error occurs while parsing the sql.\n");
			//				break;
			//			}
			//			output(node,0);
			//			puts("aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa\n");
			//			LogicalOperator* plan=parsetree2logicalplan(node);




			LogicalOperator* cj_join_key_scan=new LogicalScan(table_1->getProjectoin(6));
			Filter::Condition filter_condition_1;
			//			const int order_type=1;
			filter_condition_1.add(table_1->getAttribute(5),AttributeComparator::EQ,std::string("1"));
			//			const int trade_date=20101008;
			filter_condition_1.add(table_1->getAttribute(1),AttributeComparator::GEQ,std::string("20101008"));
			int sec_code;
			printf("sec_code=%?\n");
			scanf("%d",&sec_code);
			filter_condition_1.add(table_1->getAttribute(3),AttributeComparator::EQ,&sec_code);
			LogicalOperator* filter_1=new Filter(filter_condition_1,cj_join_key_scan);


			LogicalOperator* sb_join_key_scan=new LogicalScan(table_2->getProjectoin(0));
			Filter::Condition filter_condition_2;
			//			const int order_type=1;
			filter_condition_2.add(table_2->getAttribute(4),AttributeComparator::EQ,std::string("1"));
			//			const int trade_date=20101008;
			filter_condition_2.add(table_2->getAttribute(2),AttributeComparator::GEQ,std::string("20101008"));
			//			int sec_code;
			//			printf("sec_code=%?\n");
			//			scanf("%d",&sec_code);
			//			filter_condition_2.add(table_2->getAttribute(3),AttributeComparator::EQ,&sec_code);
			LogicalOperator* filter_2=new Filter(filter_condition_2,sb_join_key_scan);

			std::vector<Attribute> group_by_attributes;
			group_by_attributes.push_back(table_1->getAttribute("order_no"));
			group_by_attributes.push_back(table_1->getAttribute("trade_dir"));
			group_by_attributes.push_back(table_1->getAttribute("trade_date"));

			std::vector<Attribute> aggregation_attributes;
			aggregation_attributes.push_back(Attribute(ATTRIBUTE_ANY));

			//			aggregation_attributes.push_back(table_1->getAttribute("trade_date"));


			std::vector<BlockStreamAggregationIterator::State::aggregation> aggregation_function;

			aggregation_function.push_back(BlockStreamAggregationIterator::State::count);
			LogicalOperator* aggregation=new Aggregation(group_by_attributes,aggregation_attributes,aggregation_function,filter_1);



			const NodeID collector_node_id=0;
			LogicalOperator* root=new LogicalQueryPlanRoot(0,aggregation,LogicalQueryPlanRoot::PERFORMANCE);
			unsigned long long int timer_start=curtick();

			BlockStreamIteratorBase* please=root->getIteratorTree(64*1024-sizeof(unsigned));
			//			root->print();
			//
			//
			//			please->print();
			//
			IteratorExecutorMaster::getInstance()->ExecuteBlockStreamIteratorsOnSite(please,collector_node_id);//

			printf("Continue(1) or not (others)?\n");
			scanf("%d",&cout);
		}
		/////////////////////////////////////////

		//	sleep(1);
		//		cout<<"ready(?)"<<endl;
		printf("ready(?)\n");
		int input;
		scanf("%d",&input);
		//		cin>>input;

		//
	}
	cout<<"Waiting~~~!~"<<endl;
	while(true){
		sleep(1);
	}

}




static int query_optimization_based_on_statistics(){
	int master;
	//	cout<<"Master(0) or Slave(others)"<<endl;
	//	cin>>master;
	printf("~!OKOKO!!!!!\n");
	printf("Master(0) or Slave(others)??\n");
	scanf("%d",&master);
	getchar();	// 2014-3-4---屏蔽换行符对后面的影响---by余楷
	if(master!=0){
		Environment::getInstance(false);
	}
	else{
		Environment::getInstance(true);

		ResourceManagerMaster *rmms=Environment::getInstance()->getResourceManagerMaster();
		Catalog* catalog=Environment::getInstance()->getCatalog();

		catalog->restoreCatalog();	/* restore Catalog ---Yu*/

#ifdef DEBUG_TestForSerialize
		puts("it is time to create table");
		TableDescriptor* table_1=new TableDescriptor("cj",Environment::getInstance()->getCatalog()->allocate_unique_table_id());
		table_1->addAttribute("row_id",data_type(t_u_long),0,true);  				//0
		table_1->addAttribute("trade_date",data_type(t_int));
		table_1->addAttribute("order_no",data_type(t_u_long),0,true);
		table_1->addAttribute("sec_code",data_type(t_int));
		table_1->addAttribute("trade_dir",data_type(t_int));
		table_1->addAttribute("order_type",data_type(t_int));				//5
		table_1->addAttribute("trade_no",data_type(t_int),0,true);
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
		//		cj_proj0_index.push_back(2);
		//		cj_proj0_index.push_back(3);
		//		cj_proj0_index.push_back(4);
		//		cj_proj0_index.push_back(5);
		const int partition_key_index_1=2;
		//		table_1->createHashPartitionedProjection(cj_proj0_index,"order_no",4);	//G0
		table_1->createHashPartitionedProjection(cj_proj0_index,"row_id",1);	//G0

		/*
		 * 输入sql语句为：
		std::string create_projection="CREATE PROJECTION ON cj (row_id,trade_date) [number=1] partitioned on row_id";
		number 是 partition_key_index，是createHashPartitionedProjection的第三个参数。
		std::vector<ColumnOffset> index;
		index.push_back(table_1->getAttribute(std::string("row_id")).index);
		 */

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

		table_1->createHashPartitionedProjection(cj_proj1_index,"row_id",1);	//G1

		//		table_1->createHashPartitionedProjection(cj_proj0_index,"order_no",8);	//G2
		//		table_1->createHashPartitionedProjection(cj_proj1_index,"row_id",8);	//G3
		//
		//		//1 month
		//		// 4 partitions
		//		table_1->createHashPartitionedProjection(cj_proj0_index,"order_no",4);	//G4
		//		table_1->createHashPartitionedProjection(cj_proj1_index,"row_id",4);	//G5
		//		// 18 partitions
		//		table_1->createHashPartitionedProjection(cj_proj0_index,"order_no",8);	//G6
		//		table_1->createHashPartitionedProjection(cj_proj1_index,"row_id",8);	//G7
		//
		//		// 5 days
		//		table_1->createHashPartitionedProjection(cj_proj0_index,"order_no",4);	//G8
		//		table_1->createHashPartitionedProjection(cj_proj1_index,"row_id",4);	//G9
		//
		//		// 1 month 8 partitions
		//		table_1->createHashPartitionedProjection(cj_proj0_index,"order_no",8);	//G10
		//		table_1->createHashPartitionedProjection(cj_proj1_index,"row_id",8);	//G11
		//
		//		// 5 days 8 partitions
		//		table_1->createHashPartitionedProjection(cj_proj0_index,"order_no",8);	//G12
		//		table_1->createHashPartitionedProjection(cj_proj1_index,"row_id",8);	//G13
		//
		//
		//		// 1 day 4 partitions by row_id
		//		table_1->createHashPartitionedProjection(cj_proj0_index,"row_id",4);	//G14
		catalog->add_table(table_1);

		////////////////////////////////////Create table right//////////////////////////

		TableDescriptor* table_2=new TableDescriptor("sb",Environment::getInstance()->getCatalog()->allocate_unique_table_id());
		table_2->addAttribute("row_id",data_type(t_u_long));
		table_2->addAttribute("order_no",data_type(t_u_long));
		table_2->addAttribute("entry_date",data_type(t_int));
		table_2->addAttribute("sec_code",data_type(t_int));
		table_2->addAttribute("order_type",data_type(t_int));
		table_2->addAttribute("entry_dir",data_type(t_int));			//5
		table_2->addAttribute("tran_maint_code",data_type(t_int));
		table_2->addAttribute("Last_upd_date",data_type(t_int));
		table_2->addAttribute("Last_upd_time",data_type(t_int));
		table_2->addAttribute("Last_upd_time_dec",data_type(t_u_long));
		table_2->addAttribute("entry_time",data_type(t_int));			//10
		table_2->addAttribute("entry_time_dec",data_type(t_double));
		table_2->addAttribute("order_price",data_type(t_double));
		table_2->addAttribute("order_exec_vol",data_type(t_double));
		table_2->addAttribute("order_vol",data_type(t_double));
		table_2->addAttribute("pbu_id",data_type(t_int));				//15
		table_2->addAttribute("acct_id",data_type(t_int));
		table_2->addAttribute("acct_attr",data_type(t_int));
		table_2->addAttribute("branch_id",data_type(t_int));
		table_2->addAttribute("pbu_inter_order_no",data_type(t_int));
		table_2->addAttribute("pub_inter_txt",data_type(t_int));		//20
		table_2->addAttribute("aud_type",data_type(t_int));
		table_2->addAttribute("trade_restr_type",data_type(t_int));
		table_2->addAttribute("order_star",data_type(t_int));
		table_2->addAttribute("order_restr_type",data_type(t_int));
		table_2->addAttribute("short_sell_flag",data_type(t_int));		//25


		vector<ColumnOffset> sb_proj0_index;
		sb_proj0_index.push_back(0);
		sb_proj0_index.push_back(1);
		sb_proj0_index.push_back(2);
		sb_proj0_index.push_back(3);
		sb_proj0_index.push_back(4);
		sb_proj0_index.push_back(5);

		//		table_2->createHashPartitionedProjection(sb_proj0_index,"order_no",4);	//G0
		table_2->createHashPartitionedProjection(sb_proj0_index,"row_id",4);	//G0



		vector<ColumnOffset> sb_proj1_index;
		sb_proj1_index.push_back(0);
		sb_proj1_index.push_back(6);
		sb_proj1_index.push_back(7);
		sb_proj1_index.push_back(8);
		sb_proj1_index.push_back(9);
		sb_proj1_index.push_back(10);
		sb_proj1_index.push_back(11);
		sb_proj1_index.push_back(12);
		sb_proj1_index.push_back(13);
		sb_proj1_index.push_back(14);
		sb_proj1_index.push_back(15);
		sb_proj1_index.push_back(16);
		sb_proj1_index.push_back(17);
		sb_proj1_index.push_back(18);
		sb_proj1_index.push_back(19);
		sb_proj1_index.push_back(20);
		sb_proj1_index.push_back(21);
		sb_proj1_index.push_back(22);
		sb_proj1_index.push_back(23);
		sb_proj1_index.push_back(24);
		sb_proj1_index.push_back(25);



		table_2->createHashPartitionedProjection(sb_proj1_index,"row_id",4);	//G1

		//		table_2->createHashPartitionedProjection(sb_proj0_index,"order_no",8);	//G2
		//		table_2->createHashPartitionedProjection(sb_proj1_index,"row_id",8);	//G3
		//
		//		// 1 month
		//		// 4 partitions
		//		table_2->createHashPartitionedProjection(sb_proj0_index,"order_no",4);	//G4
		//		table_2->createHashPartitionedProjection(sb_proj1_index,"row_id",4);	//G5
		//
		//		// 18 partitions
		//		table_2->createHashPartitionedProjection(sb_proj0_index,"order_no",8);	//G6
		//		table_2->createHashPartitionedProjection(sb_proj1_index,"row_id",8);	//G7
		//
		//
		//		//5 days
		//		table_2->createHashPartitionedProjection(sb_proj0_index,"order_no",4);	//G8
		//		table_2->createHashPartitionedProjection(sb_proj1_index,"row_id",4);	//G9
		//
		//		// 1 month 8 partitions
		//		table_2->createHashPartitionedProjection(sb_proj0_index,"order_no",8);	//G10
		//		table_2->createHashPartitionedProjection(sb_proj1_index,"row_id",8);	//G11
		//
		//		// 5 days 8 partitions
		//		table_2->createHashPartitionedProjection(sb_proj0_index,"order_no",8);	//G12
		//		table_2->createHashPartitionedProjection(sb_proj1_index,"row_id",8);	//G13
		//
		//		// 1 day 4 partitions by row_id
		//		table_2->createHashPartitionedProjection(sb_proj0_index,"row_id",4);	//G14


		catalog->add_table(table_2);
		///////////////////////////////////////////////////////////




		///////////////////////////////////////


		////////////////////////////////////////
		/* the following codes should be triggered by Load module*/
		//////////////////ONE DAY////////////////////////////////////////////////
		//cj_table
		// 4 partitions partitioned by order_no
		for(unsigned i=0;i<table_1->getProjectoin(0)->getPartitioner()->getNumberOfPartitions();i++){

			catalog->getTable(0)->getProjectoin(0)->getPartitioner()->RegisterPartition(i,2);
		}

		for(unsigned i=0;i<table_1->getProjectoin(1)->getPartitioner()->getNumberOfPartitions();i++){

			catalog->getTable(0)->getProjectoin(1)->getPartitioner()->RegisterPartition(i,6);
		}
		//partitioned by row_id
		//		for(unsigned i=0;i<table_1->getProjectoin(14)->getPartitioner()->getNumberOfPartitions();i++){
		//
		//			catalog->getTPartitionFunctionable(0)->getProjectoin(14)->getPartitioner()->RegisterPartition(i,2);
		//		}
		//
		//		// 8 partitions
		//		for(unsigned i=0;i<table_1->getProjectoin(2)->getPartitioner()->getNumberOfPartitions();i++){
		//
		//			catalog->getTable(0)->getProjectoin(2)->getPartitioner()->RegisterPartition(i,1);
		//		}
		//
		//		for(unsigned i=0;i<table_1->getProjectoin(3)->getPartitioner()->getNumberOfPartitions();i++){
		//
		//			catalog->getTable(0)->getProjectoin(3)->getPartitioner()->RegisterPartition(i,3);
		//		}

		//sb_table
		for(unsigned i=0;i<table_2->getProjectoin(0)->getPartitioner()->getNumberOfPartitions();i++){

			catalog->getTable(1)->getProjectoin(0)->getPartitioner()->RegisterPartition(i,2);
		}

		for(unsigned i=0;i<table_2->getProjectoin(1)->getPartitioner()->getNumberOfPartitions();i++){

			catalog->getTable(1)->getProjectoin(1)->getPartitioner()->RegisterPartition(i,6);
		}
		//		for(unsigned i=0;i<table_2->getProjectoin(2)->getPartitioner()->getNumberOfPartitions();i++){
		//
		//			catalog->getTable(1)->getProjectoin(2)->getPartitioner()->RegisterPartition(i,1);
		//		}
		//
		//		for(unsigned i=0;i<table_2->getProjectoin(3)->getPartitioner()->getNumberOfPartitions();i++){
		//
		//			catalog->getTable(1)->getProjectoin(3)->getPartitioner()->RegisterPartition(i,3);
		//		}
		//
		//		//partitioned by row_id
		//		for(unsigned i=0;i<table_2->getProjectoin(14)->getPartitioner()->getNumberOfPartitions();i++){
		//
		//			catalog->getTable(1)->getProjectoin(14)->getPartitioner()->RegisterPartition(i,2);
		//		}
		//
		//		////////////////////////////////////////
		//
		//		///////////////////ONE MONTH/////////////////////////////////////////////////////////////
		//		//CJ
		//		// 4 partition
		//		for(unsigned i=0;i<table_1->getProjectoin(4)->getPartitioner()->getNumberOfPartitions();i++){
		//
		//			catalog->getTable(0)->getProjectoin(4)->getPartitioner()->RegisterPartition(i,40);
		//		}
		//
		//		for(unsigned i=0;i<table_1->getProjectoin(5)->getPartitioner()->getNumberOfPartitions();i++){
		//
		//			catalog->getTable(0)->getProjectoin(5)->getPartitioner()->RegisterPartition(i,104);
		//		}
		//		//8 partitions
		//		for(unsigned i=0;i<table_1->getProjectoin(10)->getPartitioner()->getNumberOfPartitions();i++){
		//
		//			catalog->getTable(0)->getProjectoin(10)->getPartitioner()->RegisterPartition(i,20);
		//		}
		//
		//		for(unsigned i=0;i<table_1->getProjectoin(11)->getPartitioner()->getNumberOfPartitions();i++){
		//
		//			catalog->getTable(0)->getProjectoin(11)->getPartitioner()->RegisterPartition(i,52);
		//		}
		//		// 18 partitions
		//		for(unsigned i=0;i<table_1->getProjectoin(6)->getPartitioner()->getNumberOfPartitions();i++){
		//
		//			catalog->getTable(0)->getProjectoin(6)->getPartitioner()->RegisterPartition(i,10);
		//		}
		//
		//		for(unsigned i=0;i<table_1->getProjectoin(7)->getPartitioner()->getNumberOfPartitions();i++){
		//
		//			catalog->getTable(0)->getProjectoin(7)->getPartitioner()->RegisterPartition(i,24);
		//		}
		//
		//		//SB
		//		// 4 partition
		//		for(unsigned i=0;i<table_2->getProjectoin(4)->getPartitioner()->getNumberOfPartitions();i++){
		//
		//			catalog->getTable(1)->getProjectoin(4)->getPartitioner()->RegisterPartition(i,39);
		//		}
		//
		//		for(unsigned i=0;i<table_2->getProjectoin(5)->getPartitioner()->getNumberOfPartitions();i++){
		//
		//			catalog->getTable(1)->getProjectoin(5)->getPartitioner()->RegisterPartition(i,131);
		//		}
		//		// 8 partitions
		//		for(unsigned i=0;i<table_2->getProjectoin(10)->getPartitioner()->getNumberOfPartitions();i++){
		//
		//			catalog->getTable(1)->getProjectoin(10)->getPartitioner()->RegisterPartition(i,20);
		//		}
		//
		//		for(unsigned i=0;i<table_2->getProjectoin(11)->getPartitioner()->getNumberOfPartitions();i++){
		//
		//			catalog->getTable(1)->getProjectoin(11)->getPartitioner()->RegisterPartition(i,66);
		//		}
		//		// 18 partitions
		//		for(unsigned i=0;i<table_2->getProjectoin(6)->getPartitioner()->getNumberOfPartitions();i++){
		//
		//			catalog->getTable(1)->getProjectoin(6)->getPartitioner()->RegisterPartition(i,10);
		//		}
		//
		//		for(unsigned i=0;i<table_2->getProjectoin(7)->getPartitioner()->getNumberOfPartitions();i++){
		//
		//			catalog->getTable(1)->getProjectoin(7)->getPartitioner()->RegisterPartition(i,30);
		//		}
		//		////////////////////////////////
		//
		//
		//		///////////// FIVE DAYS //////////////////
		//		//// cj////
		//		// 4 partitions
		//		for(unsigned i=0;i<table_1->getProjectoin(8)->getPartitioner()->getNumberOfPartitions();i++){
		//
		//			catalog->getTable(0)->getProjectoin(8)->getPartitioner()->RegisterPartition(i,14);
		//		}
		//
		//		for(unsigned i=0;i<table_1->getProjectoin(9)->getPartitioner()->getNumberOfPartitions();i++){
		//
		//			catalog->getTable(0)->getProjectoin(9)->getPartitioner()->RegisterPartition(i,36);
		//		}
		//		// 8 partitions
		//		for(unsigned i=0;i<table_1->getProjectoin(12)->getPartitioner()->getNumberOfPartitions();i++){
		//
		//			catalog->getTable(0)->getProjectoin(12)->getPartitioner()->RegisterPartition(i,7);
		//		}
		//
		//		for(unsigned i=0;i<table_1->getProjectoin(13)->getPartitioner()->getNumberOfPartitions();i++){
		//
		//			catalog->getTable(0)->getProjectoin(13)->getPartitioner()->RegisterPartition(i,19);
		//		}
		//
		//		//// sb ////
		//		// 4 partitions//
		//		for(unsigned i=0;i<table_2->getProjectoin(8)->getPartitioner()->getNumberOfPartitions();i++){
		//
		//			catalog->getTable(1)->getProjectoin(8)->getPartitioner()->RegisterPartition(i,14);
		//		}
		//
		//		for(unsigned i=0;i<table_2->getProjectoin(9)->getPartitioner()->getNumberOfPartitions();i++){
		//
		//			catalog->getTable(1)->getProjectoin(9)->getPartitioner()->RegisterPartition(i,131);
		//		}
		//		// 8 partitions//
		//		for(unsigned i=0;i<table_2->getProjectoin(12)->getPartitioner()->getNumberOfPartitions();i++){
		//
		//			catalog->getTable(1)->getProjectoin(12)->getPartitioner()->RegisterPartition(i,7);
		//		}
		//
		//		for(unsigned i=0;i<table_2->getProjectoin(13)->getPartitioner()->getNumberOfPartitions();i++){
		//
		//			catalog->getTable(1)->getProjectoin(13)->getPartitioner()->RegisterPartition(i,23);
		//		}

		TableID table_id=catalog->getTable("cj")->get_table_id();
		cout<<"cj is :"<<table_id<<endl;

#endif DEBUG_TestForSerialize


		Attribute att;
		//		Analyzer::analyse(table_id,Analyzer::a_l_attribute);


		ExecuteLogicalQueryPlan();

		/////////////////////////////////////////
		catalog->saveCatalog();	// serialize the catalog---by Yu

		cout<<"Waiting~~~!~"<<endl;
		while(true){
			sleep(1);
		}
	}
	return 0;
}
static int query_optimization_outputdata()
{
	int master;
//	cout<<"Master(0) or Slave(others)"<<endl;
//	cin>>master;
	printf("~!OKOKO!!!!!\n");
	printf("Master(0) or Slave(others)??\n");
	scanf("%d",&master);
	getchar();	// 2014-3-4---屏蔽换行符对后面的影响---by余楷
	if(master!=0)
	{
		Environment::getInstance(false);
	}
	else
	{
		Environment::getInstance(true);
		startup_multiple_node_environment_of_tpch();

		Catalog* catalog=Environment::getInstance()->getCatalog();
		catalog->restoreCatalog();
//		ResourceManagerMaster *rmms=Environment::getInstance()->getResourceManagerMaster();
//		Catalog* catalog=Environment::getInstance()->getCatalog();
////		TableDescriptor* table_1=new TableDescriptor("PART",0);
////		table_1->addAttribute("row_id", data_type(t_u_long),0,true);
////		table_1->addAttribute("P_PARTKEY",data_type(t_u_long),0,true);  				//0
////		table_1->addAttribute("P_NAME",data_type(t_string),55);
////		table_1->addAttribute("P_MFGR",data_type(t_string),25);
////		table_1->addAttribute("P_BRAND",data_type(t_string),10);
////		table_1->addAttribute("P_TYPE",data_type(t_string),25);
////		table_1->addAttribute("P_SIZE",data_type(t_int));
////		table_1->addAttribute("P_CONTAINER",data_type(t_string),10);
////		table_1->addAttribute("P_RETAILPRICE",data_type(t_decimal),4);
////		table_1->addAttribute("P_COMMENT",data_type(t_string),23);
////
////		table_1->createHashPartitionedProjectionOnAllAttribute("P_PARTKEY",1);//should be 4
////
////		catalog->add_table(table_1);
////
////		for(unsigned i=0;i<table_1->getProjectoin(0)->getPartitioner()->getNumberOfPartitions();i++){
////
////			catalog->getTable(0)->getProjectoin(0)->getPartitioner()->RegisterPartition(i,3);
////		}
//
//
//		TableDescriptor* table_1=new TableDescriptor("LINEITEM",0);
//		table_1->addAttribute("row_id", data_type(t_u_long));
//		table_1->addAttribute("L_ORDERKEY",data_type(t_u_long));  				//0
//		table_1->addAttribute("L_PARTKEY",data_type(t_u_long));
//		table_1->addAttribute("L_SUPPKEY",data_type(t_u_long));
//		table_1->addAttribute("L_LINENUMBER",data_type(t_u_long));
//		table_1->addAttribute("L_QUANTITY",data_type(t_decimal));
//		table_1->addAttribute("L_EXTENDEDPRICE",data_type(t_decimal));
//		table_1->addAttribute("L_DISCOUNT",data_type(t_decimal));
//		table_1->addAttribute("L_TEX",data_type(t_decimal));
//		table_1->addAttribute("L_RETURNFLAG",data_type(t_string),1);
//		table_1->addAttribute("L_LINESTATUS",data_type(t_string),1);
//		table_1->addAttribute("L_SHIPDATE",data_type(t_date));
//		table_1->addAttribute("L_COMMITDATE",data_type(t_date));
//		table_1->addAttribute("L_RECEIPTDATE",data_type(t_date));
//		table_1->addAttribute("L_SHIPINSTRUCT",data_type(t_string),25);
//		table_1->addAttribute("L_SHIPMODE",data_type(t_string),10);
//		table_1->addAttribute("L_COMMENT",data_type(t_string),44);
//
//		table_1->createHashPartitionedProjectionOnAllAttribute("L_ORDERKEY",1);
///*
//row_id,L_ORDERKEY,L_PARTKEY,L_SUPPKEY,L_LINENUMBER,L_QUANTITY,L_EXTENDEDPRICE,L_DISCOUNT
// */
//		catalog->add_table(table_1);
//
//		for(unsigned i=0;i<table_1->getProjectoin(0)->getPartitioner()->getNumberOfPartitions();i++){
//
//			catalog->getTable(0)->getProjectoin(0)->getPartitioner()->RegisterPartition(i,5);
//		}


//		TableDescriptor* table_1=new TableDescriptor("cj",Environment::getInstance()->getCatalog()->allocate_unique_table_id());
//		/**
//		 * @li: I change the following code such that the scheme matches those in cj's first projection
//		 */
//		table_1->addAttribute("row_id",data_type(t_u_long));  				//0
//		table_1->addAttribute("trade_date",data_type(t_int));
//		table_1->addAttribute("order_no",data_type(t_u_long));
//		table_1->addAttribute("sec_code",data_type(t_int));
//		table_1->addAttribute("trade_dir",data_type(t_int));
//		table_1->addAttribute("order_type",data_type(t_int));
//
//		vector<ColumnOffset> cj_proj0;
//		cj_proj0.push_back(0);
//		cj_proj0.push_back(1);
//		cj_proj0.push_back(2);
//		cj_proj0.push_back(3);
//		cj_proj0.push_back(4);
//		cj_proj0.push_back(5);
//		const int partition_key_index_1=0;
//		table_1->createHashPartitionedProjection(cj_proj0,"row_id",1);	//G0
//		catalog->add_table(table_1);
//
//		for(unsigned i=0;i<table_1->getProjectoin(0)->getPartitioner()->getNumberOfPartitions();i++)
//		{
//
//			catalog->getTable(0)->getProjectoin(0)->getPartitioner()->RegisterPartition(i,1);
//		}



		ExecuteLogicalQueryPlan();
		printf("ready(?)\n");
		int input;
		scanf("%d",&input);

		cout<<"Waiting~~~!~"<<endl;
		while(true)
		{
			sleep(1);
		}
	}
}
