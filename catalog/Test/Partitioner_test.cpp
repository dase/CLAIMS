/*
 * Partitioner_test.cpp
 *
 *  Created on: Oct 30, 2013
 *      Author: wangli
 */
#include <iostream>

#include "../../catalog/catalog.h"
#include "../../catalog/ProjectionBinding.h"
#include "../../catalog/table.h"
#include "../../common/hash.h"
#include "../../Environment.h"
using namespace std;

int TableDescriptor_test() {
  Catalog* catalog = Catalog::getInstance();

  TableDescriptor* table =
      new TableDescriptor("Student", catalog->allocate_unique_table_id());
  table->addAttribute("Name", data_type(t_string), 10);
  table->addAttribute("Age", data_type(t_int));
  table->addAttribute("Gender", data_type(t_int));
  table->addAttribute("Score", data_type(t_float));

  vector<ColumnOffset> index;
  index.push_back(0);
  index.push_back(1);
  index.push_back(3);
  const int partition_key_index = 3;
  table->createHashPartitionedProjection(index, partition_key_index, 64);

  catalog->add_table(table);
}

int BindingTest() {
  Environment::getInstance(true);
  ResourceManagerMaster* rmms =
      Environment::getInstance()->getResourceManagerMaster();
  //	rmms->RegisterNewSlave("192.168.1.1");
  //	rmms->RegisterNewSlave("192.168.1.2");
  //	rmms->RegisterNewSlave("192.168.1.3");
  //	rmms->RegisterNewSlave("192.168.1.4");
  //	rmms->RegisterNewSlave("192.168.1.5");
  //	rmms->RegisterDiskBuget(0,0);
  //	rmms->RegisterDiskBuget(1,1000);
  //	rmms->RegisterDiskBuget(2,0);
  //	rmms->RegisterDiskBuget(3,10000);
  //	rmms->RegisterDiskBuget(4,0);

  ///////////////////////////////////////
  /* the following codes should be triggered by DDL module*/
  TableDescriptor* table = new TableDescriptor(
      "Student",
      Environment::getInstance()->getCatalog()->allocate_unique_table_id());
  table->addAttribute("Name", data_type(t_string), 10);
  table->addAttribute("Age", data_type(t_int));
  table->addAttribute("Gender", data_type(t_int));
  table->addAttribute("Score", data_type(t_float));

  vector<ColumnOffset> index;
  index.push_back(0);
  index.push_back(1);
  index.push_back(3);
  const int partition_key_index = 3;
  table->createHashPartitionedProjection(index, partition_key_index, 4);
  Catalog* catalog = Environment::getInstance()->getCatalog();
  catalog->add_table(table);
  ///////////////////////////////////////

  ////////////////////////////////////////
  /* the following codes should be triggered by Load module*/
  ;
  for (unsigned i = 0;
       i < table->getProjectoin(0)->getPartitioner()->getNumberOfPartitions();
       i++) {
    catalog->getTable(0)->getProjectoin(0)->getPartitioner()->RegisterPartition(
        i, 12);
  }
  ////////////////////////////////////////

  ProjectionBinding* pb = new ProjectionBinding();
  pb->BindingEntireProjection(
      catalog->getTable(0)->getProjectoin(0)->getPartitioner());
  //	pb->BindingEntireProjection(catalog->getTable(0)->getProjectoin(0)->getPartitoiner());
}

int Theron_test() {
  Theron::EndPoint* endpoint =
      new Theron::EndPoint("endpoint", "tcp://127.0.0.1:24444");
  Theron::Framework* framework = new Theron::Framework(*endpoint);
}

int main2342343333() {
  BindingTest();

  cout << "Waiting~" << endl;
  while (true) {
    sleep(1);
  }
  //	return TableDescriptor_test();
}
