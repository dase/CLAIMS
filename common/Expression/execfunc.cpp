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


void *Exec_unary(Node *cinfo,void *tuple,Schema *schema)
{
	QExpr_unary *unode=(QExpr_unary *)cinfo;
	FuncCallInfoData finfo;
	finfo.args[0]=unode->next->FuncId(unode->next,tuple,schema);
	finfo.nargs=1;
	finfo.results=unode->value;
	unode->function_call(&finfo);
	return unode->type_cast_func(finfo.results,unode->value);
}
void *Exec_cal(Node *cinfo,void *tuple,Schema *schema)
{
	QExpr_binary *cal=(QExpr_binary *)(cinfo);
	FuncCallInfoData finfo;
	finfo.args[0]=cal->lnext->FuncId(cal->lnext,tuple,schema);
	finfo.args[1]=cal->rnext->FuncId(cal->rnext,tuple,schema);
	finfo.nargs=2;
	finfo.results=cal->value;
	cal->function_call(&finfo);
	return cal->type_cast_func(finfo.results,cal->value);
}
void *Exec_cmp(Node *cinfo,void *tuple,Schema *schema)
{
	QExpr_binary *cal=(QExpr_binary *)(cinfo);
	FuncCallInfoData finfo;
	finfo.args[0]=cal->lnext->FuncId(cal->lnext,tuple,schema);
	finfo.args[1]=cal->rnext->FuncId(cal->rnext,tuple,schema);
	finfo.nargs=2;
	finfo.results=cal->value;
	cal->function_call(&finfo);
	return finfo.results;//the actual type is bool ,so it needn't change
}
void *Exec_ternary(Node *cinfo,void *tuple,Schema *schema)
{
	QExpr_ternary *tnode=(QExpr_ternary *)cinfo;
	FuncCallInfoData finfo;
	finfo.args[0]=tnode->next0->FuncId(tnode->next0,tuple,schema);
	finfo.args[1]=tnode->next1->FuncId(tnode->next1,tuple,schema);
	finfo.args[2]=tnode->next2->FuncId(tnode->next2,tuple,schema);
	finfo.nargs=3;
	finfo.results=tnode->value;
	tnode->function_call(&finfo);
	return tnode->type_cast_func(finfo.results,tnode->value);
}

void *getConst(Node *cinfo,void *tuple,Schema *schema)//TODO string=>actual_type=>return_type
{
	QExpr *qexpr=(QExpr*)(cinfo);
	return qexpr->value;
}
void *getcol(Node *cinfo,void *tuple,Schema *schema)//TODO need actual_type=>return_type
{
	QColcumns *qcol=(QColcumns *)(cinfo);
	void *result=schema->getColumnAddess(qcol->id,tuple);
	return qcol->type_cast_func(result,qcol->value);
}
void *Exec_case_when(Node *cinfo,void *tuple,Schema *schema)
{
	QExpr_case_when *cwnode=(QExpr_case_when *)cinfo;
	void *result=NULL;
	int i=0;
	for(i=0;i<cwnode->qual.size();i++)
	{
		if(*(bool *)(cwnode->qual[i]->FuncId(cwnode->qual[i],tuple,schema))==true)
		{
			result=cwnode->ans[i];
			break;
		}
	}
	if(result==NULL&&(i+1)==cwnode->ans.size())
	{
		result=cwnode->ans[i];
	}
	if(result==NULL)
	{
		assert(false);
		return NULL;
	}
	else
	{
		result=((QNode *)result)->FuncId((QNode *)result,tuple,schema);
		return cwnode->type_cast_func(result,cwnode->value);
	}

}
void *Exec_in_item(QNode *cinfo,void *tuple,Schema *schema,QNode *node)
{
	QExpr_binary *cal=(QExpr_binary *)(cinfo);
	FuncCallInfoData finfo;
	finfo.args[0]=cal->lnext->FuncId(cal->lnext,tuple,schema);
	finfo.args[1]=node->FuncId(node,tuple,schema);
	finfo.nargs=2;
	finfo.results=cal->value;
	cal->function_call(&finfo);
	return finfo.results;//the actual type is bool ,so it needn't change
}
void *Exec_in(Node *cinfo,void *tuple,Schema *schema)
{
	QExpr_in *innode=(QExpr_in *)cinfo;
	bool result=false;
	for(int i=0;i<innode->rnode.size();i++)
	{
		bool ans=true;
		for(int j=0;j<innode->rnode[i].size()&&ans;j++)
		{
			ans=(*(bool *)Exec_in_item(innode->cmpnode[j],tuple,schema,innode->rnode[i][j]));
		}
		if(ans)
		{
			result=true;
			break;
		}
	}
	return innode->type_cast_func(&result,innode->value);
}







