/*
 * initquery.cpp
 *
 *  Created on: Aug 4, 2014
 *      Author: imdb
 */
#include"initquery.h"
#include "../../common/TypeCast.h"
#include "queryfunc.h"
#include "qnode.h"
QNode * transformqual(Node *node)
{
//	memalign(cacheline_size,1024)
	if(node==NULL)
		return NULL;
	switch(node->type)
	{
		case t_expr_cal:
		{
			Expr_cal * calnode=(Expr_cal *)node;
			QNode *lnode=transformqual(calnode->lnext);
			QNode *rnode=transformqual(calnode->rnext);
			data_type a_type=TypePromotion::arith_type_promotion_map[lnode->actual_type][rnode->actual_type];
			if(strcmp(calnode->sign,"ANDOP")==0)
			{
				QExpr_binary *qcalnode=new QExpr_binary(lnode,rnode,t_boolean,oper_and,t_qexpr_cal);
				return qcalnode;
			}
			else if(strcmp(calnode->sign,"OR")==0)
			{
				QExpr_binary *qcalnode=new QExpr_binary(lnode,rnode,t_boolean,oper_or,t_qexpr_cal);
				return qcalnode;
			}
			else if(strcmp(calnode->sign,"+")==0)
			{
				QExpr_binary *qcalnode=new QExpr_binary(lnode,rnode,a_type,oper_add,t_qexpr_cal);
				return qcalnode;
			}
			else if(strcmp(calnode->sign,"-")==0)
			{
				QExpr_binary *qcalnode=new QExpr_binary(lnode,rnode,a_type,oper_minus,t_qexpr_cal);
				return qcalnode;
			}
			else if(strcmp(calnode->sign,"*")==0)
			{
				QExpr_binary *qcalnode=new QExpr_binary(lnode,rnode,a_type,oper_multi,t_qexpr_cal);
				return qcalnode;
			}
			else if(strcmp(calnode->sign,"/")==0)
			{
				QExpr_binary *qcalnode=new QExpr_binary(lnode,rnode,a_type,oper_divide,t_qexpr_cal);
				return qcalnode;
			}
			else if(strcmp(calnode->sign,"%")==0)
			{
				QExpr_binary *qcalnode=new QExpr_binary(lnode,rnode,a_type,oper_mod,t_qexpr_cal);
				return qcalnode;
			}
			else if(strcmp(calnode->sign,"LIKE")==0)
			{
				QExpr_binary *likenode=new QExpr_binary(lnode,rnode,a_type,oper_like,t_qexpr_cal);
				return likenode;
			}
			else if(strcmp(calnode->sign,"NLIKE")==0)
			{
				QExpr_binary *likenode=new QExpr_binary(lnode,rnode,a_type,oper_not_like,t_qexpr_cal);
				return likenode;
			}
			else if(strcmp(calnode->sign,"CMP")==0)
			{
				switch(calnode->cmp)
				{
					case 1://"<"
					{
						QExpr_binary *qcalnode=new QExpr_binary(lnode,rnode,a_type,oper_less,t_qexpr_cmp);
						return qcalnode;
 					}break;
					case 2://">"
					{
						QExpr_binary *qcalnode=new QExpr_binary(lnode,rnode,a_type,oper_great,t_qexpr_cmp);
						return qcalnode;
 					}break;
					case 3://"<>"
					{
						QExpr_binary *qcalnode=new QExpr_binary(lnode,rnode,a_type,oper_not_equal,t_qexpr_cmp);
						return qcalnode;
 					}break;
					case 4://"="
					{
						QExpr_binary *qcalnode=new QExpr_binary(lnode,rnode,a_type,oper_equal,t_qexpr_cmp);
						return qcalnode;
 					}break;
					case 5://"<="
					{
						QExpr_binary *qcalnode=new QExpr_binary(lnode,rnode,a_type,oper_less_equal,t_qexpr_cmp);
						return qcalnode;
 					}break;
					case 6://">="
					{
						QExpr_binary *qcalnode=new QExpr_binary(lnode,rnode,a_type,oper_great_equal,t_qexpr_cmp);
						return qcalnode;
 					}break;
					default:
					{
						SQLParse_elog("not support now\n");
					}
				}

			}
		}break;
		case t_expr_func:
		{
			Expr_func * funcnode=(Expr_func *)node;
			if(strcmp(funcnode->funname,"CASE3")==0)
			{

			}
			else if(strcmp(funcnode->funname,"CASE4")==0)//now just support |case [when expr then expr]* [else expr] end|
			{

			}
			else if(strcmp(funcnode->funname,"WHEN1")==0)
			{

			}
			else if(strcmp(funcnode->funname,"WHEN2")==0)
			{

			}
			else if(strcmp(funcnode->funname,"FSUBSTRING0")==0)
			{

			}
			else if(strcmp(funcnode->funname,"FSUBSTRING1")==0)
			{

			}
			else if(strcmp(funcnode->funname,"FTRIM0")==0)//both
			{
				QNode *lnode=transformqual(funcnode->parameter1);
				QNode *rnode=transformqual(funcnode->parameter2);
				data_type a_type=t_string;
				QExpr_binary *trimnode=new QExpr_binary(lnode,rnode,a_type,oper_both_trim,t_qexpr_cal);
				return trimnode;
			}
			else if(strcmp(funcnode->funname,"FTRIM1")==0)//leading
			{
				QNode *lnode=transformqual(funcnode->parameter1);
				QNode *rnode=transformqual(funcnode->parameter2);
				data_type a_type=t_string;
				QExpr_binary *trimnode=new QExpr_binary(lnode,rnode,a_type,oper_leading_trim,t_qexpr_cal);
				return trimnode;
			}
			else if(strcmp(funcnode->funname,"FTRIM2")==0)//trailing
			{
				QNode *lnode=transformqual(funcnode->parameter1);
				QNode *rnode=transformqual(funcnode->parameter2);
				data_type a_type=t_string;
				QExpr_binary *trimnode=new QExpr_binary(lnode,rnode,a_type,oper_trailing_trim,t_qexpr_cal);
				return trimnode;
			}
			else if(strcmp(funcnode->funname,"FTRIM3")==0)//both ' '
			{
				QNode *lnode=new QExpr(" ",t_string);//construct the ' ' const node
				QNode *rnode=transformqual(funcnode->parameter1);
				data_type a_type=t_string;
				QExpr_binary *trimnode=new QExpr_binary(lnode,rnode,a_type,oper_both_trim,t_qexpr_cal);
				return trimnode;
			}
			else if(strcmp(funcnode->funname,"FUPPER")==0)
			{

			}
			else if(strcmp(funcnode->funname,"FCAST")==0)
			{

			}
			else if(strcmp(funcnode->funname,"FCOALESCE")==0)
			{
			}
			else if(strcmp(funcnode->funname,"FCOUNT")==0)
			{

			}
			else if(strcmp(funcnode->funname,"FCOUNT")==0)
			{

			}
			else if(strcmp(funcnode->funname,"FSUM")==0)
			{

			}
			else if(strcmp(funcnode->funname,"FAVG")==0)
			{

			}
			else if(strcmp(funcnode->funname,"FMIN")==0)
			{

			}
			else if(strcmp(funcnode->funname,"FMAX")==0)
			{

			}
			else
			{
				SQLParse_elog("get_a_expression_item: ",funcnode->funname ,"   is null");
			}

		}break;
		case t_name:
		case t_name_name:
		{
			Columns *col=(Columns *)node;
			data_type a_type=Environment::getInstance()->getCatalog()->name_to_table[col->parameter1]->getAttribute2(col->parameter2).attrType->type;
			QColcumns *qcol=new QColcumns(col->parameter1,col->parameter2,a_type);
			return qcol;
		}break;
		case t_stringval:
		{
			Expr * exprval=(Expr *)node;
			QExpr *qexpr=new QExpr(exprval->data,t_string);
			return qexpr;
		}break;
		case t_intnum:
		{
			Expr * exprval=(Expr *)node;
			QExpr *qexpr=new QExpr(exprval->data,t_int);
			return qexpr;

		}break;
		case t_approxnum:
		{
			Expr * exprval=(Expr *)node;
			QExpr *qexpr=new QExpr(exprval->data,t_double);
			return qexpr;

		}break;
		case t_bool:
		{

		}break;
		default:
		{

		}
	}
	return NULL;
}
void initqual(QNode *&node,data_type r_type,map<string,int>&colindex)
{
	if(node==NULL)
		return ;
	switch(node->type)
	{
		case t_qexpr_cal://binary calculation node
		{

			QExpr_binary * calnode=(QExpr_binary *)(node);
			calnode->return_type=r_type;
			calnode->FuncId=Exec_cal;
			initqual(calnode->lnext,calnode->actual_type,colindex);
			initqual(calnode->rnext,calnode->actual_type,colindex);
			calnode->function_call=ExectorFunction::operator_function[calnode->actual_type][calnode->op_type];//TODO
		}break;
		case t_qexpr_cmp://binary comparison node
		{
			QExpr_binary * cmpnode=(QExpr_binary *)(node);
			cmpnode->return_type=r_type;
			cmpnode->FuncId=Exec_cmp;
			initqual(cmpnode->lnext,cmpnode->actual_type,colindex);
			initqual(cmpnode->rnext,cmpnode->actual_type,colindex);
			cmpnode->function_call=ExectorFunction::operator_function[cmpnode->actual_type][cmpnode->op_type];//TODO
		}break;
		case t_qexpr_func:
		{
//			QExpr_binary *funcnode=(QExpr_binary *)(node);
//			funcnode->return_type=r_type;
		}break;
		case t_qcolcumns:
		{
			QColcumns * qcol=(QColcumns *)(node);
			qcol->FuncId=getcol;
			qcol->id=colindex[qcol->col];//col=A.a or col= a?
			qcol->return_type=r_type;
		}break;
		case t_qexpr:
		{
			QExpr *qexpr=(QExpr *)(node);
			qexpr->FuncId=getConst;
			qexpr->return_type=r_type;
			strcpy(qexpr->result_store,qexpr->value.c_str());//change the storage style from string to char *,so store the value in the return_type[]
		}break;
		default:
		{

		}


	}
}
