/*
 * QNode.cpp
 *
 *  Created on: Aug 5, 2014
 *      Author: imdb
 */
#include"qnode.h"
#include"execfunc.h"
QExpr_cal::QExpr_cal(QNode *l_arg,QNode *r_arg,data_type a_type,char *tsign,char *para,int tcmp)
{
	if(tsign!=NULL)
	{
		sign=string(tsign);
	}

	if(para!=NULL)
	{
		parameter=string(para);
	}
	cmp=tcmp;
	type=t_qexpr_cal;
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


