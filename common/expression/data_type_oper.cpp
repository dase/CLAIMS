/*
 * data_type_oper.h
 *  Created on: May 29, 2015 11:28:14 AM
 *      Author: fzh
 *       Email: fzhedu@gmail.com
 *   Copyright: Copyright (c) @ ECNU.DaSE
 * Description:
 */
#include "../../common/Logging.h"
#include "./data_type_oper.h"

#include "./expr_node.h"

namespace claims {
namespace common {
DataTypeOperFunc
    DataTypeOper::data_type_oper_func_[DATA_TYPE_NUM][OPER_TYPE_NUM];
AvgDivide DataTypeOper::avg_divide_[DATA_TYPE_NUM];
GetPartitionValue DataTypeOper::partition_value_[DATA_TYPE_NUM][2];
// the avg_divide array store the AvgDivide function according to the datatype
}  // namespace common
}  // namespace claims
