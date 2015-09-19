/*
 * execfunc.cpp
 *
 *  Created on: Aug 6, 2014
 *      Author: imdb
 */
#include "execfunc.h"
#include "qnode.h"
#include "../../common/TypeCast.h"
bool ExecEvalQual(vector<QNode *> v_qual, void *tuple, Schema *schema) {
  assert(tuple!=NULL);

  for (int i = 0; i < v_qual.size(); i++) {
    QNode *qual = v_qual[i];
    void * result = ((*(qual->FuncId))(qual, tuple, schema));
    if (*(bool *) result != true)
      return false;
  }
  return true;

}

void *Exec_unary(Node *cinfo, void *tuple, Schema *schema) {
  QExpr_unary *unode = (QExpr_unary *) cinfo;
  FuncCallInfoData finfo;
  finfo.args[0] = unode->next->FuncId(unode->next, tuple, schema);
  finfo.nargs = 1;
  finfo.results = unode->value;
  unode->function_call(&finfo);
  return unode->type_cast_func(finfo.results, unode->value);
}
void *Exec_cal(Node *cinfo, void *tuple, Schema *schema) {
  QExpr_binary *cal = (QExpr_binary *) (cinfo);
  FuncCallInfoData finfo;
  finfo.args[0] = cal->lnext->FuncId(cal->lnext, tuple, schema);
  finfo.args[1] = cal->rnext->FuncId(cal->rnext, tuple, schema);
  finfo.nargs = 2;
  finfo.results = cal->value;
  cal->function_call(&finfo);
  return cal->type_cast_func(finfo.results, cal->value);
}
void *Exec_cmp(Node *cinfo, void *tuple, Schema *schema) {
  QExpr_binary *cal = (QExpr_binary *) (cinfo);
  FuncCallInfoData finfo;
  finfo.args[0] = cal->lnext->FuncId(cal->lnext, tuple, schema);
  finfo.args[1] = cal->rnext->FuncId(cal->rnext, tuple, schema);
  finfo.nargs = 2;
  finfo.results = cal->value;
  cal->function_call(&finfo);
  //return finfo.results;//the actual type is bool ,so it needn't change
  return cal->type_cast_func(finfo.results, cal->value);
}
void *Exec_ternary(Node *cinfo, void *tuple, Schema *schema) {
  QExpr_ternary *tnode = (QExpr_ternary *) cinfo;
  FuncCallInfoData finfo;
  finfo.args[0] = tnode->next0->FuncId(tnode->next0, tuple, schema);
  finfo.args[1] = tnode->next1->FuncId(tnode->next1, tuple, schema);
  finfo.args[2] = tnode->next2->FuncId(tnode->next2, tuple, schema);
  finfo.nargs = 3;
  finfo.results = tnode->value;
  tnode->function_call(&finfo);
  return tnode->type_cast_func(finfo.results, tnode->value);
}

void *getConst(Node *cinfo, void *tuple, Schema *schema)  //TODO string=>actual_type=>return_type
               {
  QExpr *qexpr = (QExpr*) (cinfo);
  return qexpr->value;
}
void *getcol(Node *cinfo, void *tuple, Schema *schema)  //TODO need actual_type=>return_type
             {
  QColcumns *qcol = (QColcumns *) (cinfo);
  void *result = schema->getColumnAddess(qcol->id, tuple);
  return qcol->type_cast_func(result, qcol->value);
}
void *Exec_case_when(Node *cinfo, void *tuple, Schema *schema) {
  QExpr_case_when *cwnode = (QExpr_case_when *) cinfo;
  void *result = NULL;
  int i = 0;
  for (i = 0; i < cwnode->qual.size(); i++) {
    if (*(bool *) (cwnode->qual[i]->FuncId(cwnode->qual[i], tuple, schema))
        == true) {
      result = cwnode->ans[i];
      break;
    }
  }
  if (result == NULL && (i + 1) == cwnode->ans.size())  //no condition satisfied, then result=else node
      {
    result = cwnode->ans[i];
  }
  if (result == NULL) {
    assert(false);
    return NULL;
  } else {
    result = ((QNode *) result)->FuncId((QNode *) result, tuple, schema);
    return cwnode->type_cast_func(result, cwnode->value);
  }

}
void *Exec_in_item(QNode *cinfo, void *tuple, Schema *schema, QNode *node) {
  QExpr_binary *cal = (QExpr_binary *) (cinfo);
  FuncCallInfoData finfo;
  finfo.args[0] = cal->lnext->FuncId(cal->lnext, tuple, schema);
  finfo.args[1] = node->FuncId(node, tuple, schema);
  finfo.nargs = 2;
  finfo.results = cal->value;
  cal->function_call(&finfo);
  return finfo.results;  //the actual type is bool ,so it needn't change
}
void *Exec_in(Node *cinfo, void *tuple, Schema *schema) {
  QExpr_in *innode = (QExpr_in *) cinfo;
  bool result = false;
  for (int i = 0; i < innode->rnode.size(); i++) {
    bool ans = true;
    for (int j = 0; j < innode->rnode[i].size() && ans; j++) {
      ans = (*(bool *) Exec_in_item(innode->cmpnode[j], tuple, schema,
                                    innode->rnode[i][j]));
    }
    if (ans) {
      result = true;
      break;
    }
  }
  return innode->type_cast_func(&result, innode->value);
}

void *Exec_date_add_sub(Node *cinfo, void *tuple, Schema *schema) {
  QExpr_date_add_sub *cal = (QExpr_date_add_sub *) (cinfo);
  FuncCallInfoData finfo;
  finfo.args[0] = cal->lnext->FuncId(cal->lnext, tuple, schema);
  finfo.args[1] = cal->rnext->FuncId(cal->rnext, tuple, schema);
  finfo.nargs = 2;
  finfo.results = cal->value;
  cal->function_call(&finfo);
  return cal->type_cast_func(finfo.results, cal->value);
}
// deep copy with operator new
void Expr_copy(QNode *node, QNode *&new_node) {
  switch (node->type) {
    case t_qexpr_cal:  //binary calculation node
    {
      QExpr_binary * calnode = (QExpr_binary *) (node);
      QExpr_binary *temp_node = new QExpr_binary(calnode);
      Expr_copy(calnode->lnext, temp_node->lnext);
      Expr_copy(calnode->rnext, temp_node->rnext);
      new_node = temp_node;
    }
      break;
    case t_qexpr_cmp:  //binary comparison node
    {
      QExpr_binary * cmpnode = (QExpr_binary *) (node);
      QExpr_binary *temp_node = new QExpr_binary(cmpnode);
      Expr_copy(cmpnode->lnext, temp_node->lnext);
      Expr_copy(cmpnode->rnext, temp_node->rnext);
      new_node = temp_node;
    }
      break;
    case t_qexpr_unary: {
      QExpr_unary *unode = (QExpr_unary *) node;
      QExpr_unary *temp_node = new QExpr_unary(unode);
      Expr_copy(unode->next, temp_node->next);
      new_node = temp_node;
    }
      break;
    case t_qexpr_ternary: {
      QExpr_ternary *tnode = (QExpr_ternary *) node;
      QExpr_ternary *temp_node = new QExpr_ternary(tnode);
      Expr_copy(tnode->next0, temp_node->next0);
      Expr_copy(tnode->next1, temp_node->next1);
      Expr_copy(tnode->next2, temp_node->next2);
      new_node = temp_node;
    }
      break;
    case t_qexpr_case_when: {
      QExpr_case_when *cwnode = (QExpr_case_when *) node;
      QExpr_case_when *temp_node = new QExpr_case_when(cwnode);
      temp_node->qual = cwnode->qual;
      temp_node->ans = cwnode->ans;
      for (int i = 0; i < cwnode->qual.size(); i++) {
        Expr_copy(cwnode->qual[i], temp_node->qual[i]);
      }
      for (int i = 0; i < cwnode->ans.size(); i++) {
        Expr_copy(cwnode->ans[i], temp_node->ans[i]);
      }
      new_node = temp_node;
    }
      break;
    case t_qexpr_in: {
      QExpr_in *innode = (QExpr_in *) node;
      QExpr_in *temp_node = new QExpr_in(innode);
      temp_node->cmpnode = innode->cmpnode;
      temp_node->rnode = innode->rnode;
      for (int i = 0; i < innode->cmpnode.size(); i++) {
        Expr_copy(innode->cmpnode[i], temp_node->cmpnode[i]);
      }
      for (int i = 0; i < innode->rnode.size(); i++) {
        for (int j = 0; j < innode->rnode[i].size(); j++) {
          Expr_copy(innode->rnode[i][j], temp_node->rnode[i][j]);
        }
      }
      new_node = temp_node;
    }
      break;
    case t_qexpr_date_add_sub: {
      QExpr_date_add_sub * date_node = (QExpr_date_add_sub *) (node);
      QExpr_date_add_sub *temp_node = new QExpr_date_add_sub(date_node);
      Expr_copy(date_node->lnext, temp_node->lnext);
      Expr_copy(date_node->rnext, temp_node->rnext);
      new_node = temp_node;
    }
      break;
    case t_qexpr_func: {

    }
      break;
    case t_qcolcumns: {
      QColcumns * qcol = (QColcumns *) (node);
      QColcumns * temp_node = new QColcumns(qcol);
      new_node = temp_node;
    }
      break;
    case t_qexpr: {
      QExpr *qexpr = (QExpr *) (node);
      QExpr *temp_node = new QExpr(qexpr);
      new_node = temp_node;
    }
      break;
    default: {
      cout << "there is unknown type in Expr_copy" << endl;
      assert(false);
    }

  }
}

