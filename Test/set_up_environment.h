/*
 * set_up_environment.h
 *
 *  Created on: May 22, 2014
 *      Author: wangli
 */

#ifndef SET_UP_ENVIRONMENT_H_
#define SET_UP_ENVIRONMENT_H_
#include "../Environment.h"

static void startup_single_node_environment_of_poc(){
//	int master;
	int master=0;
//	printf("Master(0) or Slave(others)??\n");
//	scanf("%d",&master);
	if(master!=0){
		Environment::getInstance(false);
	}
	else{

		Environment::getInstance(true);

		ResourceManagerMaster *rmms=Environment::getInstance()->getResourceManagerMaster();
		Catalog* catalog=Environment::getInstance()->getCatalog();

		TableDescriptor* table_1=new TableDescriptor("cj",Environment::getInstance()->getCatalog()->allocate_unique_table_id());
		table_1->addAttribute("row_id",data_type(t_u_long));  				//0
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
		const int partition_key_index_1=2;
		table_1->createHashPartitionedProjection(cj_proj0_index,"row_id",1);	//G0
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


		catalog->add_table(table_2);
		for(unsigned i=0;i<table_1->getProjectoin(0)->getPartitioner()->getNumberOfPartitions();i++){

			catalog->getTable(0)->getProjectoin(0)->getPartitioner()->RegisterPartition(i,2);
//			catalog->getTable(0)->getProjectoin(0)->getPartitioner()->RegisterPartition(i,0);
		}

		for(unsigned i=0;i<table_1->getProjectoin(1)->getPartitioner()->getNumberOfPartitions();i++){

			catalog->getTable(0)->getProjectoin(1)->getPartitioner()->RegisterPartition(i,6);
		}


		//sb_table
		for(unsigned i=0;i<table_2->getProjectoin(0)->getPartitioner()->getNumberOfPartitions();i++){

			catalog->getTable(1)->getProjectoin(0)->getPartitioner()->RegisterPartition(i,2);
		}

		for(unsigned i=0;i<table_2->getProjectoin(1)->getPartitioner()->getNumberOfPartitions();i++){

			catalog->getTable(1)->getProjectoin(1)->getPartitioner()->RegisterPartition(i,6);
		}
	}
}

static void startup_mulitple_node_environment_of_poc(){
//	int master;
	int master=0;
//	printf("Master(0) or Slave(others)??\n");
//	scanf("%d",&master);
	if(master!=0){
		Environment::getInstance(false);
	}
	else{

		Environment::getInstance(true);

		ResourceManagerMaster *rmms=Environment::getInstance()->getResourceManagerMaster();
		Catalog* catalog=Environment::getInstance()->getCatalog();

		TableDescriptor* table_1=new TableDescriptor("cj",Environment::getInstance()->getCatalog()->allocate_unique_table_id());
		table_1->addAttribute("row_id",data_type(t_u_long));  				//0
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
		const int partition_key_index_1=2;
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


		catalog->add_table(table_2);
		for(unsigned i=0;i<table_1->getProjectoin(0)->getPartitioner()->getNumberOfPartitions();i++){

			catalog->getTable(0)->getProjectoin(0)->getPartitioner()->RegisterPartition(i,2);
//			catalog->getTable(0)->getProjectoin(0)->getPartitioner()->RegisterPartition(i,0);
		}

		for(unsigned i=0;i<table_1->getProjectoin(1)->getPartitioner()->getNumberOfPartitions();i++){

			catalog->getTable(0)->getProjectoin(1)->getPartitioner()->RegisterPartition(i,6);
		}


		//sb_table
		for(unsigned i=0;i<table_2->getProjectoin(0)->getPartitioner()->getNumberOfPartitions();i++){

			catalog->getTable(1)->getProjectoin(0)->getPartitioner()->RegisterPartition(i,2);
		}

		for(unsigned i=0;i<table_2->getProjectoin(1)->getPartitioner()->getNumberOfPartitions();i++){

			catalog->getTable(1)->getProjectoin(1)->getPartitioner()->RegisterPartition(i,6);
		}
	}
}


static void startup_single_node_environment_of_tpch(bool master=true){
	Environment::getInstance(master);
	printf("Press any key to continue!\n");
	int input;
	scanf("%d",&input);
	ResourceManagerMaster *rmms=Environment::getInstance()->getResourceManagerMaster();
	Catalog* catalog=Environment::getInstance()->getCatalog();


	/////////////////////////////// PART TABLE //////////////////////////////////
//	TableDescriptor* table_1=new TableDescriptor("PART",Environment::getInstance()->getCatalog()->allocate_unique_table_id());
	TableDescriptor* table_1=new TableDescriptor("PART",0);
	table_1->addAttribute("row_id", data_type(t_u_long),0,true);
	table_1->addAttribute("P_PARTKEY",data_type(t_u_long),0,true);  				//0
	table_1->addAttribute("P_NAME",data_type(t_string),55);
	table_1->addAttribute("P_MFGR",data_type(t_string),25);
	table_1->addAttribute("P_BRAND",data_type(t_string),10);
	table_1->addAttribute("P_TYPE",data_type(t_string),25);
	table_1->addAttribute("P_SIZE",data_type(t_int));
	table_1->addAttribute("P_CONTAINER",data_type(t_string),10);
	table_1->addAttribute("P_RETAILPRICE",data_type(t_decimal),4);
	table_1->addAttribute("P_COMMENT",data_type(t_string),23);

	table_1->createHashPartitionedProjectionOnAllAttribute("P_PARTKEY",4);//should be 4
	///////////////////////////////////////////////////////////////////////////////


	/////////////////////////////// SUPPLIER TABLE //////////////////////////////////
	TableDescriptor* table_2=new TableDescriptor("SUPPLIER",1);
	table_2->addAttribute("row_id", data_type(t_u_long),0,true);
	table_2->addAttribute("S_SUPPKEY",data_type(t_u_long),0,true);  				//0
	table_2->addAttribute("S_NAME",data_type(t_string),55);
	table_2->addAttribute("S_ADDRESS",data_type(t_string),40);
	table_2->addAttribute("S_NATIONKEY",data_type(t_u_long));
	table_2->addAttribute("S_PHONE",data_type(t_string),15);
	table_2->addAttribute("S_ACCTBAL",data_type(t_decimal),4);
	table_2->addAttribute("S_COMMENT",data_type(t_string),101);

	table_2->createHashPartitionedProjectionOnAllAttribute("S_SUPPKEY",4);//should be 4
	///////////////////////////////////////////////////////////////////////////////


	/////////////////////////////// PARTSUPP TABLE //////////////////////////////////
	TableDescriptor* table_3=new TableDescriptor("PARTSUPP",2);
	table_3->addAttribute("row_id", data_type(t_u_long),0,true);
	table_3->addAttribute("PS_PARTKEY",data_type(t_u_long),0,true);  				//0
	table_3->addAttribute("PS_SUPPKEY",data_type(t_u_long));
	table_3->addAttribute("PS_AVAILQTY",data_type(t_int));
	table_3->addAttribute("PS_SUPPLYCOST",data_type(t_decimal),2);
	table_3->addAttribute("PS_COMMENT",data_type(t_string),199);

	table_3->createHashPartitionedProjectionOnAllAttribute("PS_PARTKEY",4);//should be 4
	///////////////////////////////////////////////////////////////////////////////


	/////////////////////////////// CUSTOM TABLE //////////////////////////////////
	TableDescriptor* table_4=new TableDescriptor("CUSTOMER",3);
	table_4->addAttribute("row_id", data_type(t_u_long),0,true);
	table_4->addAttribute("C_CUSTKEY",data_type(t_u_long),0,true);  				//0
	table_4->addAttribute("C_NAME",data_type(t_string),25);
	table_4->addAttribute("C_ADDRESS",data_type(t_string),40);
	table_4->addAttribute("C_NATIONKEY",data_type(t_u_long));
	table_4->addAttribute("C_PHONE",data_type(t_string),15);
	table_4->addAttribute("C_ACCTBAL",data_type(t_decimal),4);
	table_4->addAttribute("C_MKTSEGMENT",data_type(t_string),10);
	table_4->addAttribute("C_COMMENT",data_type(t_string),117);

	table_4->createHashPartitionedProjectionOnAllAttribute("C_CUSTKEY",4);//should be 4
	///////////////////////////////////////////////////////////////////////////////


	/////////////////////////////// ORDERS TABLE //////////////////////////////////
	TableDescriptor* table_5=new TableDescriptor("ORDERS",4);
	table_5->addAttribute("row_id", data_type(t_u_long),0,true);
	table_5->addAttribute("O_ORDERKEY",data_type(t_u_long),0,true);  				//0
	table_5->addAttribute("O_CUSTKEY",data_type(t_u_long));
	table_5->addAttribute("O_ORDERSTATUS",data_type(t_string),1);
	table_5->addAttribute("O_TOTALPRICE",data_type(t_decimal),4);
	table_5->addAttribute("O_ORDERDATE",data_type(t_date),15);
	table_5->addAttribute("O_ORDERPRIORITY",data_type(t_string),15);
	table_5->addAttribute("O_CLERK",data_type(t_string),15);
	table_5->addAttribute("O_SHIPPRIORITY",data_type(t_int));
	table_5->addAttribute("O_COMMENT",data_type(t_string),79);

	table_5->createHashPartitionedProjectionOnAllAttribute("O_ORDERKEY",4);//should be 4
	///////////////////////////////////////////////////////////////////////////////

	/////////////////////////////// LINEITEM TABLE //////////////////////////////////
	TableDescriptor* table_6=new TableDescriptor("LINEITEM",5);
	table_6->addAttribute("row_id", data_type(t_u_long),0,true);			//0
	table_6->addAttribute("L_ORDERKEY",data_type(t_u_long),0,true);
	table_6->addAttribute("L_PARTKEY",data_type(t_u_long));
	table_6->addAttribute("L_SUPPKEY",data_type(t_u_long));
	table_6->addAttribute("L_LINENUMBER",data_type(t_u_long));
	table_6->addAttribute("L_QUANTITY",data_type(t_decimal)); 		//5
	table_6->addAttribute("L_EXTENDEDPRICE",data_type(t_decimal),4);
	table_6->addAttribute("L_DISCOUNT",data_type(t_decimal));
	table_6->addAttribute("L_TEX",data_type(t_decimal),4);
	table_6->addAttribute("L_RETURNFLAG",data_type(t_string),1);
	table_6->addAttribute("L_LINESTATUS",data_type(t_string),1);	//10
	table_6->addAttribute("L_SHIPDATE",data_type(t_date));
	table_6->addAttribute("L_COMMITDATE",data_type(t_date));
	table_6->addAttribute("L_RECEIPTDATE",data_type(t_date));
	table_6->addAttribute("L_SHIPINSTRUCT",data_type(t_string),25);
	table_6->addAttribute("L_SHIPMODE",data_type(t_string),10);
	table_6->addAttribute("L_COMMENT",data_type(t_string),44);

	table_6->createHashPartitionedProjectionOnAllAttribute("L_ORDERKEY",4); // the number of partition should be 4.
	///////////////////////////////////////////////////////////////////////////////
	/////////////////////////////// NATION TABLE //////////////////////////////////
	TableDescriptor* table_7=new TableDescriptor("NATION",6);
	table_7->addAttribute("row_id", data_type(t_u_long),0,true);
	table_7->addAttribute("N_NATIONKEY",data_type(t_u_long),0,true);  				//0
	table_7->addAttribute("N_NAME",data_type(t_string),25);
	table_7->addAttribute("N_REGIONKEY",data_type(t_u_long));
	table_7->addAttribute("N_COMMENT",data_type(t_string),152);

	table_7->createHashPartitionedProjectionOnAllAttribute("N_NATIONKEY",1);
	///////////////////////////////////////////////////////////////////////////////

	/////////////////////////////// REGION TABLE //////////////////////////////////
	TableDescriptor* table_8=new TableDescriptor("REGION",7);
	table_8->addAttribute("row_id", data_type(t_u_long),0,true);
	table_8->addAttribute("R_REGIONKEY",data_type(t_u_long),0,true);  				//0
	table_8->addAttribute("R_NAME",data_type(t_string),25);
	table_8->addAttribute("R_COMMENT",data_type(t_string),152);

	table_8->createHashPartitionedProjectionOnAllAttribute("R_REGIONKEY",1);
	///////////////////////////////////////////////////////////////////////////////



	catalog->add_table(table_1);
	catalog->add_table(table_2);
	catalog->add_table(table_3);
	catalog->add_table(table_4);
	catalog->add_table(table_5);
	catalog->add_table(table_6);
	catalog->add_table(table_7);
	catalog->add_table(table_8);


	//T0
	for(unsigned i=0;i<table_1->getProjectoin(0)->getPartitioner()->getNumberOfPartitions();i++){

		catalog->getTable(0)->getProjectoin(0)->getPartitioner()->RegisterPartition(i,3);
	}

	for(unsigned i=0;i<table_2->getProjectoin(0)->getPartitioner()->getNumberOfPartitions();i++){

		catalog->getTable(1)->getProjectoin(0)->getPartitioner()->RegisterPartition(i,1);
	}

	for(unsigned i=0;i<table_3->getProjectoin(0)->getPartitioner()->getNumberOfPartitions();i++){

		catalog->getTable(2)->getProjectoin(0)->getPartitioner()->RegisterPartition(i,15);
	}
	//T3
	for(unsigned i=0;i<table_4->getProjectoin(0)->getPartitioner()->getNumberOfPartitions();i++){

		catalog->getTable(3)->getProjectoin(0)->getPartitioner()->RegisterPartition(i,3);
	}

	for(unsigned i=0;i<table_5->getProjectoin(0)->getPartitioner()->getNumberOfPartitions();i++){

		catalog->getTable(4)->getProjectoin(0)->getPartitioner()->RegisterPartition(i,19);
	}

	//T6
	for(unsigned i=0;i<table_6->getProjectoin(0)->getPartitioner()->getNumberOfPartitions();i++){

		table_6->getProjectoin(0)->getPartitioner()->RegisterPartition(i,93);
	}
	for(unsigned i=0;i<table_7->getProjectoin(0)->getPartitioner()->getNumberOfPartitions();i++){

		table_7->getProjectoin(0)->getPartitioner()->RegisterPartition(i,1);
	}
	for(unsigned i=0;i<table_8->getProjectoin(0)->getPartitioner()->getNumberOfPartitions();i++){

		table_8->getProjectoin(0)->getPartitioner()->RegisterPartition(i,1);
	}
}

static void startup_single_node_one_partition_environment_of_tpch(bool master=true){
	Environment::getInstance(master);
	printf("---TPC---\n");
	sleep(1);
//	int input;
//	scanf("%d",&input);
	ResourceManagerMaster *rmms=Environment::getInstance()->getResourceManagerMaster();
	Catalog* catalog=Environment::getInstance()->getCatalog();


	/////////////////////////////// PART TABLE //////////////////////////////////
//	TableDescriptor* table_1=new TableDescriptor("PART",Environment::getInstance()->getCatalog()->allocate_unique_table_id());
	TableDescriptor* table_1=new TableDescriptor("PART",0);
	table_1->addAttribute("row_id", data_type(t_u_long),0,true);
	table_1->addAttribute("P_PARTKEY",data_type(t_u_long),0,true);  				//0
	table_1->addAttribute("P_NAME",data_type(t_string),55);
	table_1->addAttribute("P_MFGR",data_type(t_string),25);
	table_1->addAttribute("P_BRAND",data_type(t_string),10);
	table_1->addAttribute("P_TYPE",data_type(t_string),25);
	table_1->addAttribute("P_SIZE",data_type(t_int));
	table_1->addAttribute("P_CONTAINER",data_type(t_string),10);
	table_1->addAttribute("P_RETAILPRICE",data_type(t_decimal),4);
	table_1->addAttribute("P_COMMENT",data_type(t_string),23);

	table_1->createHashPartitionedProjectionOnAllAttribute("P_PARTKEY",1);//should be 4
	///////////////////////////////////////////////////////////////////////////////


	/////////////////////////////// SUPPLIER TABLE //////////////////////////////////
	TableDescriptor* table_2=new TableDescriptor("SUPPLIER",1);
	table_2->addAttribute("row_id", data_type(t_u_long),0,true);
	table_2->addAttribute("S_SUPPKEY",data_type(t_u_long),0,true);  				//0
	table_2->addAttribute("S_NAME",data_type(t_string),55);
	table_2->addAttribute("S_ADDRESS",data_type(t_string),40);
	table_2->addAttribute("S_NATIONKEY",data_type(t_u_long));
	table_2->addAttribute("S_PHONE",data_type(t_string),15);
	table_2->addAttribute("S_ACCTBAL",data_type(t_decimal),4);
	table_2->addAttribute("S_COMMENT",data_type(t_string),101);

	table_2->createHashPartitionedProjectionOnAllAttribute("S_SUPPKEY",1);//should be 4
	///////////////////////////////////////////////////////////////////////////////


	/////////////////////////////// PARTSUPP TABLE //////////////////////////////////
	TableDescriptor* table_3=new TableDescriptor("PARTSUPP",2);
	table_3->addAttribute("row_id", data_type(t_u_long),0,true);
	table_3->addAttribute("PS_PARTKEY",data_type(t_u_long),0,true);  				//0
	table_3->addAttribute("PS_SUPPKEY",data_type(t_u_long));
	table_3->addAttribute("PS_AVAILQTY",data_type(t_int));
	table_3->addAttribute("PS_SUPPLYCOST",data_type(t_decimal),2);
	table_3->addAttribute("PS_COMMENT",data_type(t_string),199);

	table_3->createHashPartitionedProjectionOnAllAttribute("PS_PARTKEY",1);//should be 4
	///////////////////////////////////////////////////////////////////////////////


	/////////////////////////////// CUSTOM TABLE //////////////////////////////////
	TableDescriptor* table_4=new TableDescriptor("CUSTOMER",3);
	table_4->addAttribute("row_id", data_type(t_u_long),0,true);
	table_4->addAttribute("C_CUSTKEY",data_type(t_u_long),0,true);  				//0
	table_4->addAttribute("C_NAME",data_type(t_string),25);
	table_4->addAttribute("C_ADDRESS",data_type(t_string),40);
	table_4->addAttribute("C_NATIONKEY",data_type(t_u_long));
	table_4->addAttribute("C_PHONE",data_type(t_string),15);
	table_4->addAttribute("C_ACCTBAL",data_type(t_decimal),4);
	table_4->addAttribute("C_MKTSEGMENT",data_type(t_string),10);
	table_4->addAttribute("C_COMMENT",data_type(t_string),117);

	table_4->createHashPartitionedProjectionOnAllAttribute("C_CUSTKEY",1);//should be 4
	///////////////////////////////////////////////////////////////////////////////


	/////////////////////////////// ORDERS TABLE //////////////////////////////////
	TableDescriptor* table_5=new TableDescriptor("ORDERS",4);
	table_5->addAttribute("row_id", data_type(t_u_long),0,true);
	table_5->addAttribute("O_ORDERKEY",data_type(t_u_long),0,true);  				//0
	table_5->addAttribute("O_CUSTKEY",data_type(t_u_long));
	table_5->addAttribute("O_ORDERSTATUS",data_type(t_string),1);
	table_5->addAttribute("O_TOTALPRICE",data_type(t_decimal),4);
	table_5->addAttribute("O_ORDERDATE",data_type(t_date),15);
	table_5->addAttribute("O_ORDERPRIORITY",data_type(t_string),15);
	table_5->addAttribute("O_CLERK",data_type(t_string),15);
	table_5->addAttribute("O_SHIPPRIORITY",data_type(t_int));
	table_5->addAttribute("O_COMMENT",data_type(t_string),79);

	table_5->createHashPartitionedProjectionOnAllAttribute("O_ORDERKEY",1);//should be 4
	///////////////////////////////////////////////////////////////////////////////

	/////////////////////////////// LINEITEM TABLE //////////////////////////////////
	TableDescriptor* table_6=new TableDescriptor("LINEITEM",5);
	table_6->addAttribute("row_id", data_type(t_u_long),0,true);			//0
	table_6->addAttribute("L_ORDERKEY",data_type(t_u_long),0,true);
	table_6->addAttribute("L_PARTKEY",data_type(t_u_long));
	table_6->addAttribute("L_SUPPKEY",data_type(t_u_long));
	table_6->addAttribute("L_LINENUMBER",data_type(t_u_long));
	table_6->addAttribute("L_QUANTITY",data_type(t_decimal)); 		//5
	table_6->addAttribute("L_EXTENDEDPRICE",data_type(t_decimal),4);
	table_6->addAttribute("L_DISCOUNT",data_type(t_decimal));
	table_6->addAttribute("L_TEX",data_type(t_decimal),4);
	table_6->addAttribute("L_RETURNFLAG",data_type(t_string),1);
	table_6->addAttribute("L_LINESTATUS",data_type(t_string),1);	//10
	table_6->addAttribute("L_SHIPDATE",data_type(t_date));
	table_6->addAttribute("L_COMMITDATE",data_type(t_date));
	table_6->addAttribute("L_RECEIPTDATE",data_type(t_date));
	table_6->addAttribute("L_SHIPINSTRUCT",data_type(t_string),25);
	table_6->addAttribute("L_SHIPMODE",data_type(t_string),10);
	table_6->addAttribute("L_COMMENT",data_type(t_string),44);

	table_6->createHashPartitionedProjectionOnAllAttribute("L_ORDERKEY",1); // the number of partition should be 4.
	///////////////////////////////////////////////////////////////////////////////
	/////////////////////////////// NATION TABLE //////////////////////////////////
	TableDescriptor* table_7=new TableDescriptor("NATION",6);
	table_7->addAttribute("row_id", data_type(t_u_long),0,true);
	table_7->addAttribute("N_NATIONKEY",data_type(t_u_long),0,true);  				//0
	table_7->addAttribute("N_NAME",data_type(t_string),25);
	table_7->addAttribute("N_REGIONKEY",data_type(t_u_long));
	table_7->addAttribute("N_COMMENT",data_type(t_string),152);

	table_7->createHashPartitionedProjectionOnAllAttribute("N_NATIONKEY",1);
	///////////////////////////////////////////////////////////////////////////////

	/////////////////////////////// REGION TABLE //////////////////////////////////
	TableDescriptor* table_8=new TableDescriptor("REGION",7);
	table_8->addAttribute("row_id", data_type(t_u_long),0,true);
	table_8->addAttribute("R_REGIONKEY",data_type(t_u_long),0,true);  				//0
	table_8->addAttribute("R_NAME",data_type(t_string),25);
	table_8->addAttribute("R_COMMENT",data_type(t_string),152);

	table_8->createHashPartitionedProjectionOnAllAttribute("R_REGIONKEY",1);
	///////////////////////////////////////////////////////////////////////////////



	catalog->add_table(table_1);
	catalog->add_table(table_2);
	catalog->add_table(table_3);
	catalog->add_table(table_4);
	catalog->add_table(table_5);
	catalog->add_table(table_6);
	catalog->add_table(table_7);
	catalog->add_table(table_8);


	//T0
	for(unsigned i=0;i<table_1->getProjectoin(0)->getPartitioner()->getNumberOfPartitions();i++){

		catalog->getTable(0)->getProjectoin(0)->getPartitioner()->RegisterPartition(i,3);
	}

	for(unsigned i=0;i<table_2->getProjectoin(0)->getPartitioner()->getNumberOfPartitions();i++){

		catalog->getTable(1)->getProjectoin(0)->getPartitioner()->RegisterPartition(i,1);
	}

	for(unsigned i=0;i<table_3->getProjectoin(0)->getPartitioner()->getNumberOfPartitions();i++){

		catalog->getTable(2)->getProjectoin(0)->getPartitioner()->RegisterPartition(i,15);
	}
	//T3
	for(unsigned i=0;i<table_4->getProjectoin(0)->getPartitioner()->getNumberOfPartitions();i++){

		catalog->getTable(3)->getProjectoin(0)->getPartitioner()->RegisterPartition(i,3);
	}

	for(unsigned i=0;i<table_5->getProjectoin(0)->getPartitioner()->getNumberOfPartitions();i++){

		catalog->getTable(4)->getProjectoin(0)->getPartitioner()->RegisterPartition(i,19);
	}

	//T6
	for(unsigned i=0;i<table_6->getProjectoin(0)->getPartitioner()->getNumberOfPartitions();i++){

		table_6->getProjectoin(0)->getPartitioner()->RegisterPartition(i,93);
	}
	for(unsigned i=0;i<table_7->getProjectoin(0)->getPartitioner()->getNumberOfPartitions();i++){

		table_7->getProjectoin(0)->getPartitioner()->RegisterPartition(i,1);
	}
	for(unsigned i=0;i<table_8->getProjectoin(0)->getPartitioner()->getNumberOfPartitions();i++){

		table_8->getProjectoin(0)->getPartitioner()->RegisterPartition(i,1);
	}
}



static void startup_multiple_node_environment_of_tpch(bool master=true){
	Environment::getInstance(master);

	ResourceManagerMaster *rmms=Environment::getInstance()->getResourceManagerMaster();
	Catalog* catalog=Environment::getInstance()->getCatalog();


	/////////////////////////////// PART TABLE //////////////////////////////////
	TableDescriptor* table_1=new TableDescriptor("PART",Environment::getInstance()->getCatalog()->allocate_unique_table_id());
//	TableDescriptor* table_1=new TableDescriptor("PART",0);
	table_1->addAttribute("row_id", data_type(t_u_long),0,true);
	table_1->addAttribute("P_PARTKEY",data_type(t_u_long),0,true);  				//0
	table_1->addAttribute("P_NAME",data_type(t_string),55);
	table_1->addAttribute("P_MFGR",data_type(t_string),25);
	table_1->addAttribute("P_BRAND",data_type(t_string),10);
	table_1->addAttribute("P_TYPE",data_type(t_string),25);
	table_1->addAttribute("P_SIZE",data_type(t_int));
	table_1->addAttribute("P_CONTAINER",data_type(t_string),10);
	table_1->addAttribute("P_RETAILPRICE",data_type(t_decimal),4);
	table_1->addAttribute("P_COMMENT",data_type(t_string),23);

	table_1->createHashPartitionedProjectionOnAllAttribute("P_PARTKEY",4);//should be 4
	///////////////////////////////////////////////////////////////////////////////


	/////////////////////////////// SUPPLIER TABLE //////////////////////////////////
	TableDescriptor* table_2=new TableDescriptor("SUPPLIER",Environment::getInstance()->getCatalog()->allocate_unique_table_id());
//	TableDescriptor* table_2=new TableDescriptor("SUPPLIER",1);
	table_2->addAttribute("row_id", data_type(t_u_long),0,true);
	table_2->addAttribute("S_SUPPKEY",data_type(t_u_long),0,true);  				//0
	table_2->addAttribute("S_NAME",data_type(t_string),55);
	table_2->addAttribute("S_ADDRESS",data_type(t_string),40);
	table_2->addAttribute("S_NATIONKEY",data_type(t_u_long));
	table_2->addAttribute("S_PHONE",data_type(t_string),15);
	table_2->addAttribute("S_ACCTBAL",data_type(t_decimal),4);
	table_2->addAttribute("S_COMMENT",data_type(t_string),101);

	table_2->createHashPartitionedProjectionOnAllAttribute("S_SUPPKEY",4);//should be 4
	///////////////////////////////////////////////////////////////////////////////


	/////////////////////////////// PARTSUPP TABLE //////////////////////////////////
	TableDescriptor* table_3=new TableDescriptor("PARTSUPP",Environment::getInstance()->getCatalog()->allocate_unique_table_id());
//	TableDescriptor* table_3=new TableDescriptor("PARTSUPP",2);
	table_3->addAttribute("row_id", data_type(t_u_long),0,true);
	table_3->addAttribute("PS_PARTKEY",data_type(t_u_long),0,true);  				//0
	table_3->addAttribute("PS_SUPPKEY",data_type(t_u_long));
	table_3->addAttribute("PS_AVAILQTY",data_type(t_int));
	table_3->addAttribute("PS_SUPPLYCOST",data_type(t_decimal),2);
	table_3->addAttribute("PS_COMMENT",data_type(t_string),199);

	table_3->createHashPartitionedProjectionOnAllAttribute("PS_PARTKEY",4);//should be 4
	///////////////////////////////////////////////////////////////////////////////


	/////////////////////////////// CUSTOM TABLE //////////////////////////////////
	TableDescriptor* table_4=new TableDescriptor("CUSTOMER",Environment::getInstance()->getCatalog()->allocate_unique_table_id());

//	TableDescriptor* table_4=new TableDescriptor("CUSTOMER",3);
	table_4->addAttribute("row_id", data_type(t_u_long),0,true);
	table_4->addAttribute("C_CUSTKEY",data_type(t_u_long),0,true);  				//0
	table_4->addAttribute("C_NAME",data_type(t_string),25);
	table_4->addAttribute("C_ADDRESS",data_type(t_string),40);
	table_4->addAttribute("C_NATIONKEY",data_type(t_u_long));
	table_4->addAttribute("C_PHONE",data_type(t_string),15);
	table_4->addAttribute("C_ACCTBAL",data_type(t_decimal),4);
	table_4->addAttribute("C_MKTSEGMENT",data_type(t_string),10);
	table_4->addAttribute("C_COMMENT",data_type(t_string),117);

	table_4->createHashPartitionedProjectionOnAllAttribute("C_CUSTKEY",4);//should be 4
	///////////////////////////////////////////////////////////////////////////////


	/////////////////////////////// ORDERS TABLE //////////////////////////////////
	TableDescriptor* table_5=new TableDescriptor("ORDERS",Environment::getInstance()->getCatalog()->allocate_unique_table_id());

//	TableDescriptor* table_5=new TableDescriptor("ORDERS",4);
	table_5->addAttribute("row_id", data_type(t_u_long),0,true);
	table_5->addAttribute("O_ORDERKEY",data_type(t_u_long),0,true);  				//0
	table_5->addAttribute("O_CUSTKEY",data_type(t_u_long));
	table_5->addAttribute("O_ORDERSTATUS",data_type(t_string),1);
	table_5->addAttribute("O_TOTALPRICE",data_type(t_decimal),4);
	table_5->addAttribute("O_ORDERDATE",data_type(t_date),15);
	table_5->addAttribute("O_ORDERPRIORITY",data_type(t_string),15);
	table_5->addAttribute("O_CLERK",data_type(t_string),15);
	table_5->addAttribute("O_SHIPPRIORITY",data_type(t_int));
	table_5->addAttribute("O_COMMENT",data_type(t_string),79);

	table_5->createHashPartitionedProjectionOnAllAttribute("O_ORDERKEY",4);//should be 4
	///////////////////////////////////////////////////////////////////////////////

	/////////////////////////////// LINEITEM TABLE //////////////////////////////////
	TableDescriptor* table_6=new TableDescriptor("LINEITEM",Environment::getInstance()->getCatalog()->allocate_unique_table_id());

//	TableDescriptor* table_6=new TableDescriptor("LINEITEM",5);
	table_6->addAttribute("row_id", data_type(t_u_long),0,true);			//0
	table_6->addAttribute("L_ORDERKEY",data_type(t_u_long),0,true);
	table_6->addAttribute("L_PARTKEY",data_type(t_u_long));
	table_6->addAttribute("L_SUPPKEY",data_type(t_u_long));
	table_6->addAttribute("L_LINENUMBER",data_type(t_u_long));
	table_6->addAttribute("L_QUANTITY",data_type(t_decimal)); 		//5
	table_6->addAttribute("L_EXTENDEDPRICE",data_type(t_decimal),4);
	table_6->addAttribute("L_DISCOUNT",data_type(t_decimal));
	table_6->addAttribute("L_TEX",data_type(t_decimal),4);
	table_6->addAttribute("L_RETURNFLAG",data_type(t_string),1);
	table_6->addAttribute("L_LINESTATUS",data_type(t_string),1);	//10
	table_6->addAttribute("L_SHIPDATE",data_type(t_date));
	table_6->addAttribute("L_COMMITDATE",data_type(t_date));
	table_6->addAttribute("L_RECEIPTDATE",data_type(t_date));
	table_6->addAttribute("L_SHIPINSTRUCT",data_type(t_string),25);
	table_6->addAttribute("L_SHIPMODE",data_type(t_string),10);
	table_6->addAttribute("L_COMMENT",data_type(t_string),44);

	table_6->createHashPartitionedProjectionOnAllAttribute("L_ORDERKEY",4); // the number of partition should be 4.
	///////////////////////////////////////////////////////////////////////////////
	/////////////////////////////// NATION TABLE //////////////////////////////////
//	TableDescriptor* table_7=new TableDescriptor("NATION",6);
	TableDescriptor* table_7=new TableDescriptor("NATION",Environment::getInstance()->getCatalog()->allocate_unique_table_id());

	table_7->addAttribute("row_id", data_type(t_u_long),0,true);
	table_7->addAttribute("N_NATIONKEY",data_type(t_u_long),0,true);  				//0
	table_7->addAttribute("N_NAME",data_type(t_string),25);
	table_7->addAttribute("N_REGIONKEY",data_type(t_u_long));
	table_7->addAttribute("N_COMMENT",data_type(t_string),152);

	table_7->createHashPartitionedProjectionOnAllAttribute("N_NATIONKEY",1);
	///////////////////////////////////////////////////////////////////////////////

	/////////////////////////////// REGION TABLE //////////////////////////////////
//	TableDescriptor* table_8=new TableDescriptor("REGION",7);
	TableDescriptor* table_8=new TableDescriptor("REGION",Environment::getInstance()->getCatalog()->allocate_unique_table_id());

	table_8->addAttribute("row_id", data_type(t_u_long),0,true);
	table_8->addAttribute("R_REGIONKEY",data_type(t_u_long),0,true);  				//0
	table_8->addAttribute("R_NAME",data_type(t_string),25);
	table_8->addAttribute("R_COMMENT",data_type(t_string),152);

	table_8->createHashPartitionedProjectionOnAllAttribute("R_REGIONKEY",1);
	///////////////////////////////////////////////////////////////////////////////



	catalog->add_table(table_1);
	catalog->add_table(table_2);
	catalog->add_table(table_3);
	catalog->add_table(table_4);
	catalog->add_table(table_5);
	catalog->add_table(table_6);
	catalog->add_table(table_7);
	catalog->add_table(table_8);


	//T0
	for(unsigned i=0;i<table_1->getProjectoin(0)->getPartitioner()->getNumberOfPartitions();i++){

		catalog->getTable(0)->getProjectoin(0)->getPartitioner()->RegisterPartition(i,3);
	}

	for(unsigned i=0;i<table_2->getProjectoin(0)->getPartitioner()->getNumberOfPartitions();i++){

		catalog->getTable(1)->getProjectoin(0)->getPartitioner()->RegisterPartition(i,1);
	}

	for(unsigned i=0;i<table_3->getProjectoin(0)->getPartitioner()->getNumberOfPartitions();i++){

		catalog->getTable(2)->getProjectoin(0)->getPartitioner()->RegisterPartition(i,15);
	}
	//T3
	for(unsigned i=0;i<table_4->getProjectoin(0)->getPartitioner()->getNumberOfPartitions();i++){

		catalog->getTable(3)->getProjectoin(0)->getPartitioner()->RegisterPartition(i,3);
	}

	for(unsigned i=0;i<table_5->getProjectoin(0)->getPartitioner()->getNumberOfPartitions();i++){

		catalog->getTable(4)->getProjectoin(0)->getPartitioner()->RegisterPartition(i,19);
	}

	//T6
	for(unsigned i=0;i<table_6->getProjectoin(0)->getPartitioner()->getNumberOfPartitions();i++){

		table_6->getProjectoin(0)->getPartitioner()->RegisterPartition(i,93);
	}
	for(unsigned i=0;i<table_7->getProjectoin(0)->getPartitioner()->getNumberOfPartitions();i++){

		table_7->getProjectoin(0)->getPartitioner()->RegisterPartition(i,1);
	}
	for(unsigned i=0;i<table_8->getProjectoin(0)->getPartitioner()->getNumberOfPartitions();i++){

		table_8->getProjectoin(0)->getPartitioner()->RegisterPartition(i,1);
	}
}







#endif /* SET_UP_ENVIRONMENT_H_ */
