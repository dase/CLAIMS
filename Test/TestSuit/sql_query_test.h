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
class DMLTest : public ::testing::Test{
protected:

	static void SetUpTestCase() {
	   Environment::getInstance(true);
	   create_poc_data_one_partitions();
	}

	static void TearDownTestCase() {
		Environment::getInstance(true)->~Environment();
	}
};

TEST_F(DMLTest,FullTableScan){
	std::string sql="select * from cj;";
	query_result result;
	if(query(sql,result)){
		EXPECT_EQ(3966020,result.result_set->getNumberOftuples());
	}
	else{
		printf("SQL parser error: %s\n",result.msg.c_str());
		EXPECT_EQ(0,1);
	}
}

TEST_F(DMLTest,FullTableFilter){
	std::string sql="select * from cj where row_id=1;";
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
