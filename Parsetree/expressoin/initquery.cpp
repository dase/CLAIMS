#include "initquery.h"
/*
 * initquery.cpp
 *
 *  Created on: Aug 4, 2014
 *      Author: imdb
 */
#include"initquery.h"
QNode * transformqual(Node *node)
{
	if(node==NULL)
		return NULL;
	switch(node->type)
	{
		case t_expr_cal:
		{
			Expr_cal * calnode=(Expr_cal *)node;
			QNode *lnode=transformqual(calnode->lnext);
			QNode *rnode=transformqual(calnode->rnext);
			if(strcmp(calnode->sign,"ANDOP")==0)
			{

			}
			else if(strcmp(calnode->sign,"OR")==0)
			{

			}
			else if(strcmp(calnode->sign,"+")==0)
			{

			}
			else if(strcmp(calnode->sign,"-")==0)
			{

			}
			else if(strcmp(calnode->sign,"*")==0)
			{

			}
			else if(strcmp(calnode->sign,"/")==0)
			{

			}
			else if(strcmp(calnode->sign,"CMP")==0)
			{
				switch(calnode->cmp)
				{
					case 1://"<"
					{

 					}break;
					case 2://">"
					{

 					}break;
					case 3://"<>"
					{

 					}break;
					case 4://"="
					{
						data_type a_type=TypePromotion::arith_type_promotion_map[lnode->actual_type][rnode->actual_type];
						QExpr_cal *qcalnode=new QExpr_cal(lnode,rnode,a_type,calnode->sign,calnode->parameter,calnode->cmp);
						return qcalnode;
 					}break;
					case 5://"<="
					{

 					}break;
					case 6://">="
					{

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
			cout<<"not support now~"<<endl;
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
		case t_qexpr_cal:
		{
			QExpr_cal * calnode=(QExpr_cal *)(node);
			calnode->return_type=r_type;
			calnode->FuncId=Exec_cal;
			initqual(calnode->lnext,calnode->actual_type,colindex);
			initqual(calnode->rnext,calnode->actual_type,colindex);
			if(strcmp(calnode->sign.c_str(),"ANDOP")==0)
			{

			}
			else if(strcmp(calnode->sign.c_str(),"OR")==0)
			{

			}
			else if(strcmp(calnode->sign.c_str(),"+")==0)
			{

			}
			else if(strcmp(calnode->sign.c_str(),"-")==0)
			{

			}
			else if(strcmp(calnode->sign.c_str(),"*")==0)
			{

			}
			else if(strcmp(calnode->sign.c_str(),"/")==0)
			{

			}
			else if(strcmp(calnode->sign.c_str(),"CMP")==0)
			{
				switch(calnode->cmp)
				{
					case 1://"<"
					{

 					}break;
					case 2://">"
					{

 					}break;
					case 3://"<>"
					{

 					}break;
					case 4://"="
					{
						calnode->function_call=u_long_eq;//TODO
 					}break;
					case 5://"<="
					{

 					}break;
					case 6://">="
					{

 					}break;
					default:
					{
						SQLParse_elog("not support now\n");
					}
				}

			}
		}break;
		case t_qexpr_func:
		{
//			Expr_func * funcnode=dynamic_cast<Expr_func *>node;
			cout<<"not support now~"<<endl;
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
		}break;
		default:
		{

		}


	}
}
