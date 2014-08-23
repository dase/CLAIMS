/*
 * QNode.cpp
 *
 *  Created on: Aug 5, 2014
 *      Author: imdb
 */
#include"qnode.h"
#include"execfunc.h"
QExpr_unary::QExpr_unary(QNode * arg,data_type a_type,oper_type op_types,qnodetype q_type,char *t_alias)
{
	op_type=op_types;
	type=q_type;
	next=arg;
	actual_type=a_type;
	alias=string(t_alias);
}
QExpr_binary::QExpr_binary(QNode *l_arg,QNode *r_arg,data_type a_type,oper_type op_types,qnodetype q_type,char *t_alias)
{
	op_type=op_types;
	type=q_type;
	lnext=l_arg;
	rnext=r_arg;
	actual_type=a_type;
//	function_call=f_call;
	FuncId=Exec_cal;
	alias=string(t_alias);
}
QExpr_ternary::QExpr_ternary(QNode *arg0,QNode *arg1,QNode *arg2,data_type a_type,oper_type op_types,qnodetype q_type,char *t_alias)
{
	op_type=op_types;
	type=q_type;
	next0=arg0;
	next1=arg1;
	next2=arg2;
	actual_type=a_type;
	alias=string(t_alias);

}
QExpr::QExpr(char *val,data_type a_type,char *t_alias)
{
	type=t_qexpr;
	const_value=string(val);
	actual_type=a_type;
	FuncId=getConst;
	alias=string(t_alias);
}
QColcumns::QColcumns(char *tbl,char *coln,data_type a_type,char *t_alias)
{
	type=t_qcolcumns;
	id=-1;
	actual_type=a_type;
	table=string(tbl);
	col=string(coln);
	FuncId=getcol;
	alias=string(t_alias);
}
QExpr_case_when::QExpr_case_when(vector<QNode *>&qual_,vector<QNode *>&ans_,string alias_)
{
	type=t_qexpr_case_when;
	qual=qual_;
	ans=ans_;
	alias=string(alias_) ;
	actual_type=ans[0]->actual_type;
}
QExpr_in::QExpr_in(vector<QNode *>&cmpnode_,vector<vector< QNode *> >&rnode_,char * alias_)
{
	cmpnode=cmpnode_;
	rnode=rnode_;
	actual_type=t_boolean;
	type=t_qexpr_in;
	alias=string(alias_);
}

