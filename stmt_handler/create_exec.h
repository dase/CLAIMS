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
 * createtable_exec.h
 *
 *  Created on: Sep 23, 2015
 *      Author: cswang
 *	 		 Email: cs_wang@infosys.com
 * 
 * Description:
 *      this file is mainly about the data defination language of "create", like "create table", "create projection"...
 *
 */

#ifndef STMT_HANDLER_CREATE_EXEC_H_
#define STMT_HANDLER_CREATE_EXEC_H_

#include "../stmt_handler/stmt_exec.h"

namespace claims {
namespace stmt_handler {
/**
 *
 */
class CreateTableExec : public StmtExec {
 public:
  CreateTableExec(AstNode* stmt);
  virtual ~CreateTableExec();
  int Execute();

 private:
  AstCreateTable * createtable_ast_;

};

} // namespace stmt_handler
}  // namespace claims 

#endif // STMT_HANDLER_CREATE_EXEC_H_ 
