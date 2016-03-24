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
 * gtest_main.cpp
 *
 *  Created on: Apr 18, 2015
 *      Author: wangli
 */
#include <getopt.h>
#include <gtest/gtest.h>
#include <string>
// #include "../codegen/codegen_test.h"
#include "./common/errno_test.h"
#include "../common/test/operate_test.h"
#include "../common/types/Test/data_type_test.h"
#include "../loader/test/data_injector_test.h"
#include "../common/file_handle/test/disk_file_handle_imp_test.h"
#include "../common/file_handle/test/hdfs_file_handle_imp_test.h"
#include "../loader/test/single_thread_single_file_connector_test.h"
#include "../loader/test/single_file_connector_test.h"
// #include "../loader/test/table_file_connector_test.h"
#include "./iterator/elastic_iterator_model_test.h"

#define GLOG_NO_ABBREVIATED_SEVERITIES
#include "../common/log/logging.h"

static int consumed_args = 0;

struct option long_options[] = {{"help", no_argument, 0, 'h'},
                                {"ip", required_argument, 0, 256},
                                {"port", required_argument, 0, 257},
                                {nullptr, 0, 0, 0}};

std::string help_info =
    std::string("") + "Usage:\n\t" +
    "test [test_option] [test_option] ...[gtest_option] [gtest_option]\n" +
    "\te.g. ./test --ip 127.0.0.1 --port 10001 --gtest_filter=Elastic*\n" +
    "Note: [test_option] should be given before [gtest_option].\n" +
    "The possible [test_option]s are as following:" +
    "--ip MASTER_IP\n\t\t Specify the master ip\n" + "--port MASTER_PORT\n" +
    "\t\t Specify the master port for client connection\n\n" +
    "The following are the help information from gtest:\n";

void handle_parameters(int argc, char** argv) {
  optind = 0;
  int opt;
  while ((opt = getopt_long(argc, argv, "h", long_options, NULL)) != -1) {
    switch (opt) {
      case 256:
        ElasticIteratorModelTest::ip_ = (std::string(optarg));
        consumed_args++;
        break;
      case 257:
        ElasticIteratorModelTest::port_ = (atoi(optarg));
        consumed_args++;
        break;
      case 'h':
        printf("%s", help_info.c_str());
        break;
      default:
        printf("Invalid parameters! Try -h for help\n");
        break;
    }
  }
}

int main(int argc, char** argv) {
  handle_parameters(argc, argv);
  int new_argc = argc + consumed_args;
  claims::common::Logging claims_logging(argv[0]);
  testing::InitGoogleTest(&(new_argc), argv + consumed_args);
  return RUN_ALL_TESTS();
}
