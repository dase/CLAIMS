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
 * /Claims/node_manager/node_main.cpp
 *
 *  Created on: Jan 4, 2016
 *      Author: fzh
 *		   Email: fzhedu@gmail.com
 *
 * Description:
 *
 */
#include <unistd.h>
#include <iostream>
#include <string>

#include "./master_node.h"
#include "./slave_node.h"
#include "../common/log/logging.h"
#include "../Config.h"
using claims::MasterNode;
using claims::SlaveNode;
int main(int argc, char** argv) {
  claims::common::Logging claims_logging(argv[0]);
  Config::getInstance();
  MasterNode* master_node = new MasterNode();
  //  usleep(1000);
  SlaveNode* slave_node1 = new SlaveNode();
  slave_node1->RegisterToMaster(true);
  SlaveNode* slave_node2 = new SlaveNode();
  slave_node2->RegisterToMaster(true);
  SlaveNode* slave_node3 = new SlaveNode();
  slave_node3->RegisterToMaster(true);

  master_node->PrintNodeList();
  //  usleep(5000);
  master_node->FinishAllNode();
  delete master_node;
  delete slave_node1;
  delete slave_node2;
  delete slave_node3;
  caf::await_all_actors_done();
  std::cout << "all actors done!" << std::endl;
  return 0;
}
