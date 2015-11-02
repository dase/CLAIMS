/*
 * sql_query_test.h
 *
 *  Created on: Sep 20, 2014
 *      Author: wangli
 */

#ifndef SQL_QUERY_TEST_H_
#define SQL_QUERY_TEST_H_
#include <gtest/gtest.h>
#include <iostream>
#include "../../Test/set_up_environment.h"
#include "../../Environment.h"
#include <string>
#include "../../utility/print_tool.h"
class DMLTestPOC1Partition : public ::testing::Test{
protected:

	static void SetUpTestCase() {
	   Environment::getInstance(true);
	   create_poc_data_one_partitions();
	}

	static void TearDownTestCase() {
		sleep(1);
		Environment::getInstance(true)->~Environment();
	}
};

TEST_F(DMLTestPOC1Partition,FullTableScan){
	std::string sql="select row_id from cj;";
	query_result result;
	if(query(sql,result)){
		EXPECT_EQ(3966020,result.result_set->getNumberOftuples());
	}
	else{
		printf("SQL parser error: %s\n",result.msg.c_str());
		EXPECT_EQ(0,1);
	}
}

TEST_F(DMLTestPOC1Partition,FullTableFilter){
	std::string sql="select row_id from cj where row_id=1;";
	query_result result;
	if(query(sql,result)){
		EXPECT_EQ(1,result.result_set->getNumberOftuples());
	}
	else{
		printf("SQL parser error: %s\n",result.msg.c_str());
		EXPECT_EQ(0,1);
	}
}

TEST_F(DMLTestPOC1Partition,Filter){
	std::string sql="select row_id from cj where row_id<=1000;";
	query_result result;
	if(query(sql,result)){
		EXPECT_EQ(279,result.result_set->getNumberOftuples());
	}
	else{
		printf("SQL parser error: %s\n",result.msg.c_str());
		EXPECT_EQ(0,1);
	}
}
TEST_F(DMLTestPOC1Partition,LogicalAggregation){
	std::string sql="select min(row_id), max(row_id) from cj;";
	query_result result;
	if(query(sql,result)){
		EXPECT_EQ(1,result.result_set->getNumberOftuples());
	}
	else{
		printf("SQL parser error: %s\n",result.msg.c_str());
		EXPECT_EQ(0,1);
	}
}
TEST_F(DMLTestPOC1Partition,FilteredJoin){
	std::string sql="select count(*) from cj, sb where cj.sec_code=sb.sec_code and cj.order_no = sb.order_no;";
	query_result result;
	if(query(sql,result)){
		EXPECT_EQ(1,result.result_set->getNumberOftuples());
	}
	else{
		printf("SQL parser error: %s\n",result.msg.c_str());
		EXPECT_EQ(0,1);
	}
}



#endif /* SQL_QUERY_TEST_H_ */
