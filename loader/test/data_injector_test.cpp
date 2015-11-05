/*
 * Copyright [2012-2015] DaSE@ECNU
 *
 * Licensed to the Apache Software Foundation (ASF) under one or more
 * contributor license agreements.  See the NOTICE file distributed with
 * this work for additional information regarding copyright ownership.
 * The ASF licenses this file to You under the Apache License, Version 2.0
 * (the "License"); you may not use this file except in compliance with
 * the License.  You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 *
 * /Claims/loader/test/data_injector_test.cpp
 *
 *  Created on: Oct 30, 2015
 *      Author: yukai
 *		   Email: yukai2014@gmail.com
 *
 * Description:
 *
 */

#include "./data_injector_test.h"

#include <vector>
#include <string>

#include "../../catalog/table.h"
#include "../../catalog/attribute.h"
using claims::catalog::TableDescriptor;
using claims::catalog::Attribute;
using std::string;
using std::vector;

namespace claims {
namespace loader {

DataInjectorTest::DataInjectorTest() {
  // TODO Auto-generated constructor stub
}

DataInjectorTest::~DataInjectorTest() {
  // TODO Auto-generated destructor stub
}

TEST_F(DataInjectorTest, Load) {
  EXPECT_TRUE(client_.connected());
  ResultSet rs;
  string command = "";
}

TEST_F(DataInjectorTest, Append) {}

TEST_F(DataInjectorTest, Insert) {}

void DataInjectorTest::SetUpTestCase() {
  Environment::getInstance();
  int num_of_partition = 4;
  vector<string> file_name;
  file_name.push_back("/home/spark/poc/cj/CJ20101008.txt");
  string table_name = "TestDataInjector";
  TableDescriptor* TD = new TableDescriptor(
      table_name,
      Environment::getInstance()->getCatalog()->allocate_unique_table_id());

  Attribute* attr0 = new Attribute(TD->get_table_id(), 0, "row_id", t_u_long);
  Attribute* attr1 = new Attribute(TD->get_table_id(), 1, "Trade_No", t_int);
  Attribute* attr2 =
      new Attribute(TD->get_table_id(), 2, "Trade_Date", t_date);  // 2
  Attribute* attr3 = new Attribute(TD->get_table_id(), 3, "Trade_Time", t_time);
  Attribute* attr4 = new Attribute(TD->get_table_id(), 4, "Trade_Time_Dec",
                                   t_decimal, 7);  // 6); 取6有bug
  Attribute* attr5 = new Attribute(TD->get_table_id(), 5, "Order_Time", t_time);
  Attribute* attr6 = new Attribute(TD->get_table_id(), 6, "Order_Time_Dec",
                                   t_decimal, 7);  // 6);
  Attribute* attr7 = new Attribute(TD->get_table_id(), 7, "Order_No",
                                   t_u_long);  // decimal?  //7
  Attribute* attr8 =
      new Attribute(TD->get_table_id(), 8, "Trade_Price", t_decimal, 5);
  Attribute* attr9 =
      new Attribute(TD->get_table_id(), 9, "Trade_Amt", t_decimal, 5);
  Attribute* attr10 =
      new Attribute(TD->get_table_id(), 10, "Trade_Vol", t_decimal, 3);
  Attribute* attr11 =
      new Attribute(TD->get_table_id(), 11, "Sec_Code", t_string, 6);  // 11
  Attribute* attr12 =
      new Attribute(TD->get_table_id(), 12, "PBU_ID", t_string, 5);
  Attribute* attr13 =
      new Attribute(TD->get_table_id(), 13, "Acct_ID", t_string, 10);
  Attribute* attr14 =
      new Attribute(TD->get_table_id(), 14, "Trade_Dir", t_string, 1);  // 14
  Attribute* attr15 =
      new Attribute(TD->get_table_id(), 15, "Order_PrtFil_Code", t_string, 1);
  Attribute* attr16 =
      new Attribute(TD->get_table_id(), 16, "Tran_Type", t_string, 3);
  Attribute* attr17 =
      new Attribute(TD->get_table_id(), 17, "Trade_Type", t_string, 1);
  Attribute* attr18 =
      new Attribute(TD->get_table_id(), 18, "Proc_Type", t_string, 1);
  Attribute* attr19 =
      new Attribute(TD->get_table_id(), 19, "Order_Type", t_string, 2);  // 19

  TD->addAttribute(*attr0);
  TD->addAttribute(*attr1);
  TD->addAttribute(*attr2);
  TD->addAttribute(*attr3);
  TD->addAttribute(*attr4);
  TD->addAttribute(*attr5);
  TD->addAttribute(*attr6);
  TD->addAttribute(*attr7);
  TD->addAttribute(*attr8);
  TD->addAttribute(*attr9);
  TD->addAttribute(*attr10);
  TD->addAttribute(*attr11);
  TD->addAttribute(*attr12);
  TD->addAttribute(*attr13);
  TD->addAttribute(*attr14);
  TD->addAttribute(*attr15);
  TD->addAttribute(*attr16);
  TD->addAttribute(*attr17);
  TD->addAttribute(*attr18);
  TD->addAttribute(*attr19);

  vector<ColumnOffset> Col_index_PJ0;  // PJ1     row_id + col(1,2,6)
  Col_index_PJ0.push_back(0);

  Col_index_PJ0.push_back(2);
  Col_index_PJ0.push_back(7);
  Col_index_PJ0.push_back(11);
  Col_index_PJ0.push_back(14);
  Col_index_PJ0.push_back(19);

  vector<ColumnOffset> Col_index_PJ1;  // PJ2    payload
  Col_index_PJ1.push_back(0);

  Col_index_PJ1.push_back(1);
  Col_index_PJ1.push_back(3);
  Col_index_PJ1.push_back(4);
  Col_index_PJ1.push_back(5);
  Col_index_PJ1.push_back(6);
  Col_index_PJ1.push_back(8);
  Col_index_PJ1.push_back(9);
  Col_index_PJ1.push_back(10);
  Col_index_PJ1.push_back(12);
  Col_index_PJ1.push_back(13);
  Col_index_PJ1.push_back(15);
  Col_index_PJ1.push_back(16);
  Col_index_PJ1.push_back(17);
  Col_index_PJ1.push_back(18);

  vector<vector<ColumnOffset> > Col_index_PJ;
  Col_index_PJ.push_back(Col_index_PJ0);
  Col_index_PJ.push_back(Col_index_PJ1);

  TD->createHashPartitionedProjection(Col_index_PJ0, 0, num_of_partition);
  TD->createHashPartitionedProjection(Col_index_PJ1, 0, num_of_partition);

  Hdfsloader* Hl = new Hdfsloader('|', '\n', file_name, TD);

  Hl->load();

  Catalog* catalog = Catalog::getInstance();

  cout << "Importing accomplished!!\n\trow numbers: "
       << catalog->getTable("CJ")->getRowNumber() << endl;

  client_.connection(ip_, port_);
}

} /* namespace loader */
} /* namespace claims */
