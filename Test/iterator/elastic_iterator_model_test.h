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

#include "../../Client/Client.h"

using decimal::NValue;

/*
 * This test is designed for validating the correctness
 * of elastic iterator model.
 */


class ElasticIteratorModelTest : public ::testing::Test{
public:
	static void SetConnectionInfo(std::string host, int port){
		ip_=host;
		port_=port;
	}
	static void SetConnectionPort(int port){
		port_=port;
	}
	static void SetConnectionIp(std::string Ip){
		ip_=Ip;
	}
	Client client_;
	static std::string ip_;
	static int port_;
protected:
	void SetUp() {
		client_.connection(ip_,port_);
	}
	void TearDown() {
		client_.shutdown();
	}
private:

};

std::string ElasticIteratorModelTest::ip_;
int ElasticIteratorModelTest::port_;

TEST_F(ElasticIteratorModelTest,Scan){
	EXPECT_TRUE(client_.connected());
	ResultSet rs;
	std::string command="select count(*) from LINEITEM;";
	std::string message;
	client_.submit(command,message,rs);
	DynamicBlockBuffer::Iterator it=rs.createIterator();
	BlockStreamBase::BlockStreamTraverseIterator *b_it=it.nextBlock()->createIterator();
	EXPECT_EQ(6001215,*(long*)b_it->nextTuple());
	delete b_it;
}

TEST_F(ElasticIteratorModelTest,FilterHighSelectivity){
	ResultSet rs;
	std::string message;
	client_.submit("select count(*) from LINEITEM where row_id < 6000000;",message,rs);
	DynamicBlockBuffer::Iterator it=rs.createIterator();
	BlockStreamBase::BlockStreamTraverseIterator *b_it=it.nextBlock()->createIterator();
	EXPECT_EQ(6000000,*(long*)b_it->nextTuple());
	delete b_it;
}
TEST_F(ElasticIteratorModelTest,FilterMedianSelectivity){
	ResultSet rs;
	std::string message;
	client_.submit("select count(*) from LINEITEM where row_id < 3000000;",message,rs);
	DynamicBlockBuffer::Iterator it=rs.createIterator();
	BlockStreamBase::BlockStreamTraverseIterator *b_it=it.nextBlock()->createIterator();
	EXPECT_EQ(3000000,*(long*)b_it->nextTuple());
	delete b_it;
}
TEST_F(ElasticIteratorModelTest,FilterLowSelectivity){
	ResultSet rs;
	std::string message;
	client_.submit("select count(*) from LINEITEM where row_id < 300;",message,rs);
	DynamicBlockBuffer::Iterator it=rs.createIterator();
	BlockStreamBase::BlockStreamTraverseIterator *b_it=it.nextBlock()->createIterator();
	EXPECT_EQ(300,*(long*)b_it->nextTuple());
	delete b_it;
}
TEST_F(ElasticIteratorModelTest,ScalaAggregation){
	ResultSet rs;
	std::string message;
	client_.submit("select count(*),sum(L_QUANTITY) from LINEITEM;",message,rs);
	DynamicBlockBuffer::Iterator it = rs.createIterator();
	BlockStreamBase::BlockStreamTraverseIterator *b_it=it.nextBlock()->createIterator();
	EXPECT_EQ(6001215,*(long*)b_it->currentTuple());
	NValue v;
	v.createDecimalFromString("153078795.0000");
	EXPECT_TRUE(v.op_equals(*(NValue*)((char*)b_it->currentTuple()+sizeof(long))));
}
TEST_F(ElasticIteratorModelTest,AggregationLargeGroups){
	ResultSet rs;
	std::string message;
	client_.submit("select row_id from LINEITEM group by row_id;",message,rs);
	EXPECT_EQ(6001215,rs.getNumberOftuples());
}
TEST_F(ElasticIteratorModelTest,AggregationMedianGroups){
	ResultSet rs;
	std::string message;
	client_.submit("select L_PARTKEY,count(*) from LINEITEM group by L_PARTKEY;",message,rs);
	EXPECT_EQ(200000,rs.getNumberOftuples());
}
TEST_F(ElasticIteratorModelTest,AggregationSmallGroups){
	ResultSet rs;
	std::string message;
	client_.submit("select L_RETURNFLAG from LINEITEM group by L_RETURNFLAG;",message,rs);
	EXPECT_EQ(3,rs.getNumberOftuples());
}
TEST_F(ElasticIteratorModelTest,Join){
	ResultSet rs;
	std::string message;
	client_.submit("select count(*) from PART,LINEITEM where PART.row_id=LINEITEM.row_id;",message,rs);
	DynamicBlockBuffer::Iterator it=rs.createIterator();
	BlockStreamBase::BlockStreamTraverseIterator *b_it=it.nextBlock()->createIterator();
	EXPECT_EQ(200000,*(long*)b_it->nextTuple());
	delete b_it;
}
TEST_F(ElasticIteratorModelTest,FilteredJoin){
	ResultSet rs;
	std::string message;
	client_.submit("select count(*) from PART,LINEITEM where PART.row_id%10=1 and LINEITEM.row_id % 10 =1 and PART.row_id = LINEITEM.row_id;",message,rs);
	DynamicBlockBuffer::Iterator it=rs.createIterator();
	BlockStreamBase::BlockStreamTraverseIterator *b_it=it.nextBlock()->createIterator();
	EXPECT_EQ(20000,*(long*)b_it->nextTuple());
	delete b_it;
}





#endif /* ELASTIC_ITERATOR_MODEL_TEST_H_ */
