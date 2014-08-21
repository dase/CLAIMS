/*
 * QNode.cpp
 *
 *  Created on: Aug 5, 2014
 *      Author: imdb
 */
#include"qnode.h"
#include"execfunc.h"
QExpr_unary::QExpr_unary(QNode * arg,data_type a_type,oper_type op_types,qnodetype q_type)
{
	op_type=op_types;
	type=q_type;
	next=arg;
	actual_type=a_type;
}
QExpr_binary::QExpr_binary(QNode *l_arg,QNode *r_arg,data_type a_type,oper_type op_types,qnodetype q_type)
{
	op_type=op_types;
	type=q_type;
	lnext=l_arg;
	rnext=r_arg;
	actual_type=a_type;
//	function_call=f_call;
	FuncId=Exec_cal;
}
QExpr_ternary::QExpr_ternary(QNode *arg0,QNode *arg1,QNode *arg2,data_type a_type,oper_type op_types,qnodetype q_type)
{
	op_type=op_types;
	type=q_type;
	next0=arg0;
	next1=arg1;
	next2=arg2;
	actual_type=a_type;
}
QExpr::QExpr(char *val,data_type a_type)
{
	type=t_qexpr;
	value=string(val);
	actual_type=a_type;
	FuncId=getConst;
}
QColcumns::QColcumns(char *tbl,char *coln,data_type a_type)
{
	type=t_qcolcumns;
	id=-1;
	actual_type=a_type;
	table=string(tbl);
	col=string(coln);
	FuncId=getcol;
}


