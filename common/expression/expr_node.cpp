/*
 * expr_node.cpp
 *  Created on: May 29, 2015 11:28:14 AM
 *      Author: fzh
 *       Email: fzhedu@gmail.com
 *   Copyright: Copyright (c) @ ECNU.DaSE
 * Description:
 */

#include "./expr_node.h"

#include <algorithm>
#include <string>
#include <iostream>
ExprNode::ExprNode(ExprNodeType expr_node_type, data_type actual_type,
                   const char* alias)
        : expr_node_type_(expr_node_type),
          actual_type_(actual_type),
          alias_(string(alias)),
          TypeCastFunc_(NULL),
          return_type_(actual_type),
          get_type_(actual_type),
          value_(NULL),
          value_size_(0),
          is_null_(false) {
}
ExprNode::ExprNode(ExprNode* expr)
        : expr_node_type_(expr->expr_node_type_),
          actual_type_(expr->actual_type_),
          return_type_(expr->return_type_),
          get_type_(expr->get_type_),
          alias_(expr->alias_),
          is_null_(expr->is_null_),
          value_size_(expr->value_size_),
          TypeCastFunc_(expr->TypeCastFunc_),
          value_(expr->value_) {
}




