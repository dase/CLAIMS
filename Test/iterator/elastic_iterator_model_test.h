/*
 * elastic_iterator_model_test.h
 *
 *  Created on: Apr 18, 2015
 *      Author: wangli
 */

#ifndef ELASTIC_ITERATOR_MODEL_TEST_H_
#define ELASTIC_ITERATOR_MODEL_TEST_H_
#include <gtest/gtest.h>
#include <iosfwd>

#include "../../common/types/NValue.hpp"
#include "../../common/types/decimal.h"

#include "../../Client/Client.h"

using decimal::NValue;

/*
 * This test is designed for validating the correctness
 * of elastic iterator model.
 */

class ElasticIteratorModelTest : public ::testing::Test {
 public:
  static void SetConnectionInfo(std::string host, int port) {
    ip_ = host;
    port_ = port;
  }
  static void SetConnectionPort(int port) { port_ = port; }
  static void SetConnectionIp(std::string Ip) { ip_ = Ip; }
  Client client_;
  static std::string ip_;
  static int port_;

 protected:
  void SetUp() { client_.connection(ip_, port_); }
  void TearDown() { client_.shutdown(); }

 private:
};

std::string ElasticIteratorModelTest::ip_;
int ElasticIteratorModelTest::port_;

TEST_F(ElasticIteratorModelTest, Scan) {
  EXPECT_TRUE(client_.connected());
  ResultSet rs;
  std::string command = "select count(*) from LINEITEM;";
  std::string message;
  client_.submit(command, message, rs);
  DynamicBlockBuffer::Iterator it = rs.createIterator();
  BlockStreamBase::BlockStreamTraverseIterator *b_it =
      it.nextBlock()->createIterator();
  EXPECT_EQ(6001215, *(long *)b_it->nextTuple());
  delete b_it;
}

TEST_F(ElasticIteratorModelTest, FilterHighSelectivity) {
  ResultSet rs;
  std::string message;
  client_.submit("select count(*) from LINEITEM where row_id < 6000000;",
                 message, rs);
  DynamicBlockBuffer::Iterator it = rs.createIterator();
  BlockStreamBase::BlockStreamTraverseIterator *b_it =
      it.nextBlock()->createIterator();
  EXPECT_EQ(6000000, *(long *)b_it->nextTuple());
  delete b_it;
}
TEST_F(ElasticIteratorModelTest, FilterMedianSelectivity) {
  ResultSet rs;
  std::string message;
  client_.submit("select count(*) from LINEITEM where row_id < 3000000;",
                 message, rs);
  DynamicBlockBuffer::Iterator it = rs.createIterator();
  BlockStreamBase::BlockStreamTraverseIterator *b_it =
      it.nextBlock()->createIterator();
  EXPECT_EQ(3000000, *(long *)b_it->nextTuple());
  delete b_it;
}
TEST_F(ElasticIteratorModelTest, FilterLowSelectivity) {
  ResultSet rs;
  std::string message;
  client_.submit("select count(*) from LINEITEM where row_id < 300;", message,
                 rs);
  DynamicBlockBuffer::Iterator it = rs.createIterator();
  BlockStreamBase::BlockStreamTraverseIterator *b_it =
      it.nextBlock()->createIterator();
  EXPECT_EQ(300, *(long *)b_it->nextTuple());
  delete b_it;
}
TEST_F(ElasticIteratorModelTest, ScalaAggregation) {
  ResultSet rs;
  std::string message;
  client_.submit("select count(*),sum(L_QUANTITY) from LINEITEM;", message, rs);
  DynamicBlockBuffer::Iterator it = rs.createIterator();
  BlockStreamBase::BlockStreamTraverseIterator *b_it =
      it.nextBlock()->createIterator();
  EXPECT_EQ(6001215, *(long *)b_it->currentTuple());
  // NValue v;
  // v.createDecimalFromString("153078795.0000");
  Decimal v(65, 30, "153078795.0000");
  EXPECT_TRUE(
      v.op_equals(*(Decimal *)((char *)b_it->currentTuple() + sizeof(long))));
}
TEST_F(ElasticIteratorModelTest, AggregationLargeGroups) {
  ResultSet rs;
  std::string message;
  client_.submit("select row_id from LINEITEM group by row_id;", message, rs);
  EXPECT_EQ(6001215, rs.getNumberOftuples());
}
TEST_F(ElasticIteratorModelTest, AggregationMedianGroups) {
  ResultSet rs;
  std::string message;
  client_.submit("select L_PARTKEY,count(*) from LINEITEM group by L_PARTKEY;",
                 message, rs);
  EXPECT_EQ(200000, rs.getNumberOftuples());
}
TEST_F(ElasticIteratorModelTest, AggregationSmallGroups) {
  ResultSet rs;
  std::string message;
  client_.submit("select L_RETURNFLAG from LINEITEM group by L_RETURNFLAG;",
                 message, rs);
  EXPECT_EQ(3, rs.getNumberOftuples());
}
TEST_F(ElasticIteratorModelTest, EqualJoin) {
  ResultSet rs;
  std::string message;
  client_.submit(
      "select count(*) from PART,LINEITEM where PART.row_id=LINEITEM.row_id;",
      message, rs);
  DynamicBlockBuffer::Iterator it = rs.createIterator();
  BlockStreamBase::BlockStreamTraverseIterator *b_it =
      it.nextBlock()->createIterator();
  EXPECT_EQ(200000, *(long *)b_it->nextTuple());
  delete b_it;
}
TEST_F(ElasticIteratorModelTest, CrossJoin) {
  ResultSet rs;
  std::string message;
  client_.submit("select count(*) from PART,REGION;", message, rs);
  DynamicBlockBuffer::Iterator it = rs.createIterator();
  BlockStreamBase::BlockStreamTraverseIterator *b_it =
      it.nextBlock()->createIterator();
  EXPECT_EQ(1000000, *(long *)b_it->nextTuple());
  delete b_it;
}

TEST_F(ElasticIteratorModelTest, CrossJoinWithSubquery) {
  ResultSet rs;
  std::string message;
  client_.submit(
      "select count(*) from (select row_id from NATION where row_id<3) as a, "
      "(select row_id from REGION where row_id=2) as b;",
      message, rs);
  DynamicBlockBuffer::Iterator it = rs.createIterator();
  BlockStreamBase::BlockStreamTraverseIterator *b_it =
      it.nextBlock()->createIterator();
  EXPECT_EQ(3, *(long *)b_it->nextTuple());
  delete b_it;
}

TEST_F(ElasticIteratorModelTest, CrossJoinWithRightNULLTable) {
  ResultSet rs;
  std::string message;
  client_.submit(
      "select count(*) from (select row_id from PART) as a, (select row_id "
      "from REGION where row_id=222) as b;",
      message, rs);
  DynamicBlockBuffer::Iterator it = rs.createIterator();
  BlockStreamBase *b_it = it.nextBlock();
  EXPECT_EQ(0, b_it);
}

TEST_F(ElasticIteratorModelTest, CrossJoinWithLeftNULLTable) {
  ResultSet rs;
  std::string message;
  client_.submit(
      "select count(*) from (select row_id from REGION where row_id>33) as a, "
      "(select row_id from PART) as b;",
      message, rs);
  DynamicBlockBuffer::Iterator it = rs.createIterator();
  BlockStreamBase *b_it = it.nextBlock();
  EXPECT_EQ(0, b_it);
}

TEST_F(ElasticIteratorModelTest, CrossJoinWithAllNULLTable) {
  ResultSet rs;
  std::string message;
  client_.submit(
      "select count(*) from (select row_id from REGION where row_id>33) as a, "
      "(select row_id from NATION where row_id>40) as b;",
      message, rs);
  DynamicBlockBuffer::Iterator it = rs.createIterator();
  BlockStreamBase *b_it = it.nextBlock();
  EXPECT_EQ(0, b_it);
}

TEST_F(ElasticIteratorModelTest, FilteredJoin) {
  ResultSet rs;
  std::string message;
  client_.submit(
      "select count(*) from PART,LINEITEM where PART.row_id%10=1 and  "
      "LINEITEM.row_id % 10 =1 and PART.row_id = LINEITEM.row_id;",
      message, rs);
  DynamicBlockBuffer::Iterator it = rs.createIterator();
  BlockStreamBase::BlockStreamTraverseIterator *b_it =
      it.nextBlock()->createIterator();
  EXPECT_EQ(20000, *(long *)b_it->nextTuple());
  delete b_it;
}

TEST_F(ElasticIteratorModelTest, OuterJoin) {
  ResultSet rs;
  std::string message;
  client_.submit(
      "SELECT COUNT(*) FROM CUSTOMER LEFT OUTER JOIN ORDERS ON "
      "C_CUSTKEY = O_CUSTKEY AND O_COMMENT NOT LIKE '%unusual%deposits%';",
      message, rs);
  DynamicBlockBuffer::Iterator it = rs.createIterator();
  BlockStreamBase::BlockStreamTraverseIterator *b_it =
      it.nextBlock()->createIterator();
  EXPECT_EQ(1533872, *(long *)b_it->nextTuple());
  delete b_it;
}

// delete data test.
TEST_F(ElasticIteratorModelTest, createTable) {
  string createtablesql =
      "create table PART2(\
P_PARTKEY bigint unsigned,\
P_NAME varchar(55),\
P_MFGR varchar(25),\
P_BRAND varchar(10),\
P_TYPE varchar(25),\
P_SIZE int,\
P_CONTAINER varchar(10),\
P_RETAILPRICE  decimal(4),\
P_COMMENT varchar(23)\
);";

  string showtablessql = "SHOW TABLES;";

  ResultSet rs;
  std::string message;
  client_.submit(createtablesql.c_str(), message, rs);
  EXPECT_STREQ("create table successfully\n", message.c_str());
  cout << message << endl;
  client_.submit(showtablessql.c_str(), message, rs);

  if ((message.find("PART2") != std::string::npos)) {
    EXPECT_TRUE(true);
  } else {
    EXPECT_TRUE(false);
  }
}

TEST_F(ElasticIteratorModelTest, createprojection) {
  string createprojectionsql =
      "create projection on PART2(\
P_PARTKEY,\
P_NAME,\
P_MFGR,\
P_BRAND,\
P_TYPE,\
P_SIZE,\
P_CONTAINER,\
P_RETAILPRICE,\
P_COMMENT\
) number = 18 partitioned on P_PARTKEY;";

  ResultSet rs;
  std::string message;
  client_.submit(createprojectionsql.c_str(), message, rs);
  EXPECT_STREQ("create projection successfully\n", message.c_str());

  cout << message << endl;
}
#if 0
TEST_F(ElasticIteratorModelTest,loaddata){

	string datapathfile = "/home/imdb/data/part.tbl";
	/* this data file should be loaded by tester SELF and the structure of the data is the same as PART. 
		just load the data of PART into PART2. 
	*/
#if 0 
	cout << "please input the your data to load:" << endl;
	cin >> datapathfile;
#endif

	string loaddataintopart2sql="load table PART2 from \""+ datapathfile +"\" with '|','\\n';";

	cout << loaddataintopart2sql << endl;
	
	ResultSet rs;
	std::string message;
	client_.submit(loaddataintopart2sql.c_str(),message,rs);
	EXPECT_STREQ("load data successfully\n", message.c_str());
	
	cout << message << endl;
}

TEST_F(ElasticIteratorModelTest,deletedata){


	string deletedatafrompart2sql="delete from PART2 where row_id < 10;";
	
	ResultSet rs;
	std::string message;
	client_.submit(deletedatafrompart2sql.c_str(),message,rs);
	//EXPECT_STREQ("load data successfully", message.c_str());
	
	cout << message << endl;
}

TEST_F(ElasticIteratorModelTest,showdeleteddatafromtableDEL){


	string showdeletedatafrompart2sql="select * from PART2_DEL order by row_id_DEL;";
	
	ResultSet rs;
	std::string message;
	client_.submit(showdeletedatafrompart2sql.c_str(),message,rs);
	DynamicBlockBuffer::Iterator it=rs.createIterator();
	BlockStreamBase::BlockStreamTraverseIterator *b_it=it.nextBlock()->createIterator();
	EXPECT_EQ(10,rs.getNumberOftuples());
	cout << message << endl;
}
#endif
TEST_F(ElasticIteratorModelTest, droptestdata) {
  string droptablepart2sql = "drop table PART2;";

  ResultSet rs;
  std::string message;
  client_.submit(droptablepart2sql.c_str(), message, rs);
  DynamicBlockBuffer::Iterator it = rs.createIterator();
  BlockStreamBase::BlockStreamTraverseIterator *b_it =
      it.nextBlock()->createIterator();
  EXPECT_EQ("drop table successfully!\n", message);
  cout << message << endl;
}

// TEST_F(ElasticIteratorModelTest, CreateTempTableForTableFileConnectorTest) {
//  string table_name = "sfdfsf";
//  string create_table_stmt =
//      "create table " + table_name + " (a int , b varchar(12));";
//  string create_prj_stmt1 = "create projection on " + table_name +
//                            " (a  , b ) number = 2 partitioned on a ;";
//  string create_prj_stmt2 = "create projection on " + table_name +
//                            " (a ) number = 3 partitioned on a ;";
//
//  ResultSet rs;
//  string message = "";
//  client_.submit(create_table_stmt.c_str(), message, rs);
//  EXPECT_EQ("create table successfully\n", message);
//  cout << message << endl;
//  client_.submit(create_prj_stmt1.c_str(), message, rs);
//  EXPECT_EQ("create projection successfully\n", message);
//  cout << message << endl;
//  client_.submit(create_prj_stmt2.c_str(), message, rs);
//  EXPECT_EQ("create projection successfully\n", message);
//  cout << message << endl;
//}

// add by cswang 19 Oct, 2015

#endif /* ELASTIC_ITERATOR_MODEL_TEST_H_ */
