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
	value=NULL;
}
QExpr_unary::QExpr_unary(QExpr_unary *node)
{
	this->op_type=node->op_type;
	this->type=node->type;
	this->actual_type=node->actual_type;
	this->alias=node->alias;
	this->return_type=node->return_type;
	this->isnull=node->isnull;
	this->length=node->length;
}
QExpr_binary::QExpr_binary(QNode *l_arg,QNode *r_arg,data_type a_type,oper_type op_types,qnodetype q_type,char *t_alias)
{
	op_type=op_types;
	type=q_type;
	lnext=l_arg;
	rnext=r_arg;
	actual_type=a_type;
	alias=string(t_alias);
	value=NULL;
}
QExpr_binary::QExpr_binary(QExpr_binary *node)
{
	this->op_type=node->op_type;
	this->type=node->type;
	this->actual_type=node->actual_type;
	this->alias=node->alias;
	this->return_type=node->return_type;
	this->isnull=node->isnull;
	this->length=node->length;
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
	value=NULL;
}
QExpr_ternary::QExpr_ternary(QExpr_ternary *node)
{
	this->op_type=node->op_type;
	this->type=node->type;
	this->actual_type=node->actual_type;
	this->alias=node->alias;
	this->return_type=node->return_type;
	this->isnull=node->isnull;
	this->length=node->length;
}
QExpr::QExpr(char *val,data_type a_type,char *t_alias)
{
	type=t_qexpr;
	const_value=string(val);
	actual_type=a_type;
	alias=string(t_alias);
	value=NULL;
}
QExpr::QExpr(QExpr *node)
{
	this->type=node->type;
	this->const_value=node->const_value;
	this->actual_type=node->actual_type;
	this->alias=node->alias;
	this->return_type=node->return_type;
	this->isnull=node->isnull;
	this->length=node->length;
}
QColcumns::QColcumns(const char *tbl,const char *coln,data_type a_type,const char *t_alias)
{
	type=t_qcolcumns;
	id=-1;
	tab_index=0;
	actual_type=a_type;
	table=string(tbl);
	col=string(coln);
	alias=string(t_alias);
	value=NULL;
}
QColcumns::QColcumns(QColcumns *node)
{
	this->type=node->type;
	this->id=node->id;
	this->actual_type=node->actual_type;
	this->table=node->table;
	this->col=node->col;
	this->alias=node->alias;
	this->return_type=node->return_type;
	this->isnull=node->isnull;
	this->length=node->length;
	tab_index=0;
}
QExpr_case_when::QExpr_case_when(vector<QNode *>&qual_,vector<QNode *>&ans_,string alias_)
{
	type=t_qexpr_case_when;
	qual=qual_;
	ans=ans_;
	alias=string(alias_) ;
	actual_type=ans[0]->actual_type;
	value=NULL;
}
QExpr_case_when::QExpr_case_when(QExpr_case_when *node)
{
	this->type=node->type;
	this->alias=node->alias;
	this->actual_type=node->actual_type;
	this->return_type=node->return_type;
	this->isnull=node->isnull;
	this->length=node->length;
}
QExpr_in::QExpr_in(vector<QNode *>&cmpnode_,vector<vector< QNode *> >&rnode_,char * alias_)
{
	cmpnode=cmpnode_;
	rnode=rnode_;
	actual_type=t_boolean;
	type=t_qexpr_in;
	alias=string(alias_);
	value=NULL;
}
QExpr_in::QExpr_in(QExpr_in *node)
{
	this->actual_type=node->actual_type;
	this->type=node->type;
	this->alias=node->alias;
	this->return_type=node->return_type;
	this->isnull=node->isnull;
	this->length=node->length;
}
QExpr_date_add_sub::QExpr_date_add_sub(QNode *l_arg,QNode *r_arg,data_type a_type,oper_type op_types,qnodetype q_type,data_type rr_type,char *t_alias)
{
	op_type=op_types;
	type=q_type;
	lnext=l_arg;
	rnext=r_arg;
	actual_type=a_type;
	alias=string(t_alias);
	value=NULL;
	rnext_type=rr_type;
}
QExpr_date_add_sub::QExpr_date_add_sub(QExpr_date_add_sub *node)
{
	this->op_type=node->op_type;
	this->type=node->type;
	this->actual_type=node->actual_type;
	this->alias=node->alias;
	this->rnext_type=node->rnext_type;
	this->return_type=node->return_type;
	this->isnull=node->isnull;
	this->length=node->length;
}
