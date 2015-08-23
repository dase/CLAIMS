/*
 * ast_node.cpp
 *  Created on: May 21, 2015 4:10:35 PM
 *      Author: fzh
 *       Email: fzhedu@gmail.com
 *   Copyright: Copyright (c) @ ECNU.DaSE
 * Description:
 */

#include "../ast_node/ast_node.h"
#include <iostream>
#include <iomanip>

using namespace std;
using std::cout;
using std::setw;
using std::endl;

AstNode::AstNode(AstNodeType ast_node_type)
        : ast_node_type_(ast_node_type) {

}

AstNode::~AstNode() {
}
void AstNode::Print(int level) const {
    cout << setw(level * 8) << " " << "This is an AST_NODE!" << endl;
}

AstNodeType AstNode::ast_node_type() {
    return ast_node_type_;
}

AstStmtList::AstStmtList(AstNodeType ast_node_type, AstNode* stmt,
                         AstNode* next)
        : AstNode(ast_node_type),
          stmt_(stmt),
          next_(next) {
}

AstStmtList::~AstStmtList() {
    delete stmt_;
    delete next_;
}

void AstStmtList::Print(int level) const {
    cout << setw(level * 8) << " " << "|stmt list|" << endl;
    if (stmt_ != NULL) {
        stmt_->Print(level);
    }
    if (next_ != NULL) {
        next_->Print(level++);
    }
}
