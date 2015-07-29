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
 * ErrorDefine.h
 *
 *  Created on: Jul 29, 2015
 *      Author: yukai
 *		 Email: yukai2014@gmail.com
 * 
 * Description:
 *
 */

#ifndef Claims_COMMON_ERRORDEFINE_H_
#define Claims_COMMON_ERRORDEFINE_H_

#include <stdlib.h>

/*
 * add Error Message in ErrorNo.h and ErrorNo.cpp, if new ErrNo is add
 */

namespace claims {
namespace common {

const int ERROR_MAX_NUMBER = 10000;
static const char* ERROR_MESSEGE[ERROR_MAX_NUMBER];

const int C_SUCCESS = EXIT_SUCCESS;
const int C_FAILURE = EXIT_FAILURE;

/* errno begin from here   */

/* errno for common  -1 ~ -1000 */
const int C_TYPE_ERROR = -1;
const int C_NOT_INIT = -2;

/* errno for SQL parser -1001 ~ -2000  */
const int C_NO_TABLE_FOUND = -1001;

}  // end namespace common
}  // end namespace claims

#endif /* Claims_COMMON_ERRORDEFINE_H_ */
