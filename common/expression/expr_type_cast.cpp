/*
 * expr_type_cast.cpp
 *  Created on: May 29, 2015 3:57:55 PM
 *      Author: fzh
 *       Email: fzhedu@gmail.com
 *   Copyright: Copyright (c) @ ECNU.DaSE
 * Description:
 */

#include "./expr_type_cast.h"
namespace claims {

namespace common {
TypeCastFunc ExprTypeCast::type_cast_func_[DATA_TYPE_NUMBER][DATA_TYPE_NUMBER];
ExprTypeCast::ExprTypeCast() {}

ExprTypeCast::~ExprTypeCast() {}

}  // namespace common
}  // namespace claims
