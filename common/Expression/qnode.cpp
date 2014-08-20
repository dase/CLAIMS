/*
 * QNode.cpp
 *
 *  Created on: Aug 5, 2014
 *      Author: imdb
 */
#include"qnode.h"
#include"execfunc.h"
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


