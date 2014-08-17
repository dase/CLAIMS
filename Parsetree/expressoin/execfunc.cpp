/*
 * execfunc.cpp
 *
 *  Created on: Aug 6, 2014
 *      Author: imdb
 */
#include"execfunc.h"
#include "qnode.h"
#include "../../common/TypeCast.h"
bool ExecEvalQual(vector<QNode *>v_qual,void *tuple,Schema *schema)
{
	assert(tuple!=NULL);
	for(int i=0;i<v_qual.size();i++)
	{
		QNode *qual=v_qual[i];
		void * result=((*(qual->FuncId))(qual,tuple,schema));
		if(*(bool *)result!=true)
			return false;
	}
	return true;

}


void *Exec_cal(Node *cinfo,void *tuple,Schema *schema)
{
	QExpr_cal *cal=(QExpr_cal *)(cinfo);
	void *lresult=cal->lnext->FuncId(cal->lnext,tuple,schema);
	void *rresult=cal->rnext->FuncId(cal->rnext,tuple,schema);
	FuncCallInfoData finfo;
	finfo.args[0]=lresult;
	finfo.args[1]=rresult;
	finfo.nargs=2;
	finfo.results=NULL;
	cal->function_call(&finfo);
	return finfo.results;

}
void *getConst(Node *cinfo,void *tuple,Schema *schema)//TODO string=>actual_type=>return_type
{
	QExpr *qexpr=(QExpr*)(cinfo);
//	unsigned long val=strtoul(qexpr->value.c_str(),NULL,10);
//	return &val;
	return TypeCast::type_cast_func[t_string][qexpr->return_type](&(qexpr->value));
}
void *getcol(Node *cinfo,void *tuple,Schema *schema)//TODO need actual_type=>return_type
{
	QColcumns *qcol=(QColcumns *)(cinfo);
	void *result=schema->getColumnAddess(qcol->id,tuple);
	return TypeCast::type_cast_func[qcol->actual_type][qcol->return_type](result);
}
