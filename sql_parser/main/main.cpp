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
 * /CLAIMS/sql_parser/ast_node/header.cpp
 *
 *  Created on: Aug 24, 2015
 *      Author:
 *       Email:
 *
 * Description:
 *
 */
#include <iostream>  //NOLINT
#include <string>
#include <stdio.h>  //NOLINT
#include "../../common/log/logging.h"
#include "../parser/parser.h"

// using claims::common;

int main(int argc, char* argv[]) {
  claims::common::Logging my_glog(argv[0]);
  int flag = 1;
  while (flag) {
    Parser* my_parser = new Parser();
    if (my_parser->GetRawAST() != NULL) {
      my_parser->GetRawAST()->Print();
    } else {
      LOG_WARNING << "the raw ast is null";
    }
    delete my_parser;
    printf("continue 1 or not 0!\n");
    scanf("%d", &flag);
  }

  return 0;
}
