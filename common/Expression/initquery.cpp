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
			if(strcmp(calnode->sign,"ANDOP")==0)
			{
				QNode *lnode=transformqual(calnode->lnext);
				QNode *rnode=transformqual(calnode->rnext);
				data_type a_type=TypePromotion::arith_type_promotion_map[lnode->actual_type][rnode->actual_type];
				QExpr_binary *qcalnode=new QExpr_binary(lnode,rnode,t_boolean,oper_and,t_qexpr_cal,calnode->str);
				return qcalnode;
			}
			else if(strcmp(calnode->sign,"OR")==0)
			{
				QNode *lnode=transformqual(calnode->lnext);
				QNode *rnode=transformqual(calnode->rnext);
				data_type a_type=TypePromotion::arith_type_promotion_map[lnode->actual_type][rnode->actual_type];
				QExpr_binary *qcalnode=new QExpr_binary(lnode,rnode,t_boolean,oper_or,t_qexpr_cal,calnode->str);
				return qcalnode;
			}
			else if(strcmp(calnode->sign,"+")==0)
			{
				QNode *lnode=transformqual(calnode->lnext);
				QNode *rnode=transformqual(calnode->rnext);
				data_type a_type=TypePromotion::arith_type_promotion_map[lnode->actual_type][rnode->actual_type];
				QExpr_binary *qcalnode=new QExpr_binary(lnode,rnode,a_type,oper_add,t_qexpr_cal,calnode->str);
				return qcalnode;
			}
			else if(strcmp(calnode->sign,"-")==0)
			{
				QNode *lnode=transformqual(calnode->lnext);
				QNode *rnode=transformqual(calnode->rnext);
				data_type a_type=TypePromotion::arith_type_promotion_map[lnode->actual_type][rnode->actual_type];
				QExpr_binary *qcalnode=new QExpr_binary(lnode,rnode,a_type,oper_minus,t_qexpr_cal,calnode->str);
				return qcalnode;
			}
			else if(strcmp(calnode->sign,"*")==0)
			{
				QNode *lnode=transformqual(calnode->lnext);
				QNode *rnode=transformqual(calnode->rnext);
				data_type a_type=TypePromotion::arith_type_promotion_map[lnode->actual_type][rnode->actual_type];
				QExpr_binary *qcalnode=new QExpr_binary(lnode,rnode,a_type,oper_multiply,t_qexpr_cal,calnode->str);
				return qcalnode;
			}
			else if(strcmp(calnode->sign,"/")==0)
			{
				QNode *lnode=transformqual(calnode->lnext);
				QNode *rnode=transformqual(calnode->rnext);
				data_type a_type=TypePromotion::arith_type_promotion_map[lnode->actual_type][rnode->actual_type];
				QExpr_binary *qcalnode=new QExpr_binary(lnode,rnode,a_type,oper_divide,t_qexpr_cal,calnode->str);
				return qcalnode;
			}
			else if(strcmp(calnode->sign,"%")==0)
			{
				QNode *lnode=transformqual(calnode->lnext);
				QNode *rnode=transformqual(calnode->rnext);
				data_type a_type=TypePromotion::arith_type_promotion_map[lnode->actual_type][rnode->actual_type];
				QExpr_binary *qcalnode=new QExpr_binary(lnode,rnode,a_type,oper_mod,t_qexpr_cal,calnode->str);
				return qcalnode;
			}
			else if(strcmp(calnode->sign,"LIKE")==0)
			{
				QNode *lnode=transformqual(calnode->lnext);
				QNode *rnode=transformqual(calnode->rnext);
				data_type a_type=TypePromotion::arith_type_promotion_map[lnode->actual_type][rnode->actual_type];
				QExpr_binary *likenode=new QExpr_binary(lnode,rnode,a_type,oper_like,t_qexpr_cmp,calnode->str);
				return likenode;
			}
			else if(strcmp(calnode->sign,"NLIKE")==0)
			{
				QNode *lnode=transformqual(calnode->lnext);
				QNode *rnode=transformqual(calnode->rnext);
				data_type a_type=TypePromotion::arith_type_promotion_map[lnode->actual_type][rnode->actual_type];
				QExpr_binary *likenode=new QExpr_binary(lnode,rnode,a_type,oper_not_like,t_qexpr_cmp,calnode->str);
				return likenode;
			}
			else if(strcmp(calnode->sign,"NOT")==0||strcmp(calnode->sign,"!")==0)
			{
				QNode *nnode=transformqual(calnode->rnext);
				QExpr_unary *unode=new QExpr_unary(nnode,t_boolean,oper_not,t_qexpr_unary,calnode->str);
				return unode;
			}
			else if(strcmp(calnode->sign,"--")==0)
			{
				QNode *nnode=transformqual(calnode->rnext);
				QExpr_unary *unode=new QExpr_unary(nnode,nnode->actual_type,oper_negative,t_qexpr_unary,calnode->str);
				return unode;
			}
			else if(strcmp(calnode->sign,"CMP")==0)
			{
				QNode *lnode=transformqual(calnode->lnext);
				QNode *rnode=transformqual(calnode->rnext);
				data_type a_type=TypePromotion::arith_type_promotion_map[lnode->actual_type][rnode->actual_type];
				switch(calnode->cmp)
				{
					case 1://"<"
					{
						QExpr_binary *qcalnode=new QExpr_binary(lnode,rnode,a_type,oper_less,t_qexpr_cmp,calnode->str);
						return qcalnode;
 					}break;
					case 2://">"
					{
						QExpr_binary *qcalnode=new QExpr_binary(lnode,rnode,a_type,oper_great,t_qexpr_cmp,calnode->str);
						return qcalnode;
 					}break;
					case 3://"<>"
					{
						QExpr_binary *qcalnode=new QExpr_binary(lnode,rnode,a_type,oper_not_equal,t_qexpr_cmp,calnode->str);
						return qcalnode;
 					}break;
					case 4://"="
					{
						QExpr_binary *qcalnode=new QExpr_binary(lnode,rnode,a_type,oper_equal,t_qexpr_cmp,calnode->str);
						return qcalnode;
 					}break;
					case 5://"<="
					{
						QExpr_binary *qcalnode=new QExpr_binary(lnode,rnode,a_type,oper_less_equal,t_qexpr_cmp,calnode->str);
						return qcalnode;
 					}break;
					case 6://">="
					{
						QExpr_binary *qcalnode=new QExpr_binary(lnode,rnode,a_type,oper_great_equal,t_qexpr_cmp,calnode->str);
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
				QNode *node0=transformqual(funcnode->args);
				QNode *node1=transformqual(funcnode->parameter1);
				QNode *node2=new QExpr("128",t_int,"t_const_node");//128 is the size of the qnode->value
				data_type a_type=t_string;
				QExpr_ternary *substrnode=new QExpr_ternary(node0,node1,node2,a_type,oper_substring,t_qexpr_ternary,funcnode->str);
				return substrnode;
			}
			else if(strcmp(funcnode->funname,"FSUBSTRING1")==0)
			{
				QNode *node0=transformqual(funcnode->args);
				QNode *node1=transformqual(funcnode->parameter1);
				QNode *node2=transformqual(funcnode->parameter2);
				data_type a_type=t_string;
				QExpr_ternary *substrnode=new QExpr_ternary(node0,node1,node2,a_type,oper_substring,t_qexpr_ternary,funcnode->str);
				return substrnode;
			}
			else if(strcmp(funcnode->funname,"FTRIM0")==0)//both
			{
				QNode *lnode=transformqual(funcnode->parameter1);
				QNode *rnode=transformqual(funcnode->parameter2);
				data_type a_type=t_string;
				QExpr_binary *trimnode=new QExpr_binary(lnode,rnode,a_type,oper_both_trim,t_qexpr_cal,funcnode->str);
				return trimnode;
			}
			else if(strcmp(funcnode->funname,"FTRIM1")==0)//leading
			{
				QNode *lnode=transformqual(funcnode->parameter1);
				QNode *rnode=transformqual(funcnode->parameter2);
				data_type a_type=t_string;
				QExpr_binary *trimnode=new QExpr_binary(lnode,rnode,a_type,oper_leading_trim,t_qexpr_cal,funcnode->str);
				return trimnode;
			}
			else if(strcmp(funcnode->funname,"FTRIM2")==0)//trailing
			{
				QNode *lnode=transformqual(funcnode->parameter1);
				QNode *rnode=transformqual(funcnode->parameter2);
				data_type a_type=t_string;
				QExpr_binary *trimnode=new QExpr_binary(lnode,rnode,a_type,oper_trailing_trim,t_qexpr_cal,funcnode->str);
				return trimnode;
			}
			else if(strcmp(funcnode->funname,"FTRIM3")==0)//both ' '
			{
				QNode *lnode=new QExpr(" ",t_string,"t_const_node");//construct the ' ' const node
				QNode *rnode=transformqual(funcnode->parameter1);
				data_type a_type=t_string;
				QExpr_binary *trimnode=new QExpr_binary(lnode,rnode,a_type,oper_both_trim,t_qexpr_cal,funcnode->str);
				return trimnode;
			}
			else if(strcmp(funcnode->funname,"FUPPER")==0)
			{
				QNode *nnext=transformqual(funcnode->parameter1);
				data_type a_type=t_string;
				QExpr_unary *uppernode=new QExpr_unary(nnext,a_type,oper_upper,t_qexpr_unary,funcnode->str);
				return uppernode;
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
			QColcumns *qcol=new QColcumns(col->parameter1,col->parameter2,a_type,col->parameter2);
			return qcol;
		}break;
		case t_stringval:
		{
			Expr * exprval=(Expr *)node;
			QExpr *qexpr=new QExpr(exprval->data,t_string,exprval->data);
			return qexpr;
		}break;
		case t_intnum:
		{
			Expr * exprval=(Expr *)node;
			QExpr *qexpr=new QExpr(exprval->data,t_int,exprval->data);
			return qexpr;

		}break;
		case t_approxnum:
		{
			Expr * exprval=(Expr *)node;
			QExpr *qexpr=new QExpr(exprval->data,t_double,exprval->data);
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
void InitExprAtLogicalPlan(QNode *node,data_type r_type,map<string,int>&colindex,Schema *schema)
{
	if(node==NULL)
		return ;
	switch(node->type)
	{
		case t_qexpr_cal://binary calculation node
		{
			QExpr_binary * calnode=(QExpr_binary *)(node);
			calnode->return_type=r_type;
			InitExprAtLogicalPlan(calnode->lnext,calnode->actual_type,colindex,schema);
			InitExprAtLogicalPlan(calnode->rnext,calnode->actual_type,colindex,schema);
			calnode->isnull=(calnode->lnext->isnull||calnode->rnext->isnull);
			calnode->length=max(calnode->lnext->length,calnode->rnext->length);
		}break;
		case t_qexpr_cmp://binary comparison node
		{
			QExpr_binary * cmpnode=(QExpr_binary *)(node);
			cmpnode->return_type=r_type;
			InitExprAtLogicalPlan(cmpnode->lnext,cmpnode->actual_type,colindex,schema);
			InitExprAtLogicalPlan(cmpnode->rnext,cmpnode->actual_type,colindex,schema);
			cmpnode->length=max(cmpnode->lnext->length,cmpnode->rnext->length);
			cmpnode->isnull=(cmpnode->lnext->isnull||cmpnode->rnext->isnull);
		}break;
		case t_qexpr_unary:
		{
			QExpr_unary *unode=(QExpr_unary *)node;
			unode->return_type=r_type;
			InitExprAtLogicalPlan(unode->next,unode->actual_type,colindex,schema);
			unode->length=unode->next->length;
			unode->isnull=unode->next->isnull;
		}break;
		case t_qexpr_ternary:
		{
			QExpr_ternary *tnode=(QExpr_ternary *)node;
			tnode->return_type=r_type;
			InitExprAtLogicalPlan(tnode->next0,tnode->actual_type,colindex,schema);
			InitExprAtLogicalPlan(tnode->next1,t_int,colindex,schema);
			InitExprAtLogicalPlan(tnode->next2,t_int,colindex,schema);
			tnode->length=max(tnode->next0->length,max(tnode->next1->length,tnode->next2->length));
			tnode->isnull=(tnode->next0->isnull||tnode->next1->isnull||tnode->next2->isnull);
		}break;
		case t_qexpr_func:
		{
//			QExpr_binary *funcnode=(QExpr_binary *)(node);
//			funcnode->return_type=r_type;
		}break;
		case t_qcolcumns:
		{
			QColcumns * qcol=(QColcumns *)(node);
			qcol->id=colindex[qcol->col];//col=A.a or col= a?
			qcol->return_type=r_type;
			qcol->length=schema->getcolumn(qcol->id).get_length();
			qcol->isnull=false;//TODO
		}break;
		case t_qexpr:
		{
			QExpr *qexpr=(QExpr *)(node);
			qexpr->return_type=r_type;
			qexpr->length=max((int)(qexpr->const_value.size()),BASE_SIZE);
			qexpr->isnull=false;

		}break;
		default:
		{

		}


	}
}
void InitExprAtPhysicalPlan(QNode *node)
{
	if(node==NULL)
		return ;
	switch(node->type)
	{
		case t_qexpr_cal://binary calculation node
		{

			QExpr_binary * calnode=(QExpr_binary *)(node);
			calnode->FuncId=Exec_cal;
			InitExprAtPhysicalPlan(calnode->lnext);
			InitExprAtPhysicalPlan(calnode->rnext);
			calnode->function_call=ExectorFunction::operator_function[calnode->actual_type][calnode->op_type];
			calnode->type_cast_func=TypeCast::type_cast_func[calnode->actual_type][calnode->return_type];
			calnode->value=memalign(cacheline_size,calnode->length);
		}break;
		case t_qexpr_cmp://binary comparison node
		{
			QExpr_binary * cmpnode=(QExpr_binary *)(node);
			cmpnode->FuncId=Exec_cmp;
			InitExprAtPhysicalPlan(cmpnode->lnext);
			InitExprAtPhysicalPlan(cmpnode->rnext);
			cmpnode->function_call=ExectorFunction::operator_function[cmpnode->actual_type][cmpnode->op_type];
			cmpnode->type_cast_func=TypeCast::type_cast_func[cmpnode->actual_type][cmpnode->return_type];
			cmpnode->value=memalign(cacheline_size,cmpnode->length);
		}break;
		case t_qexpr_unary:
		{
			QExpr_unary *unode=(QExpr_unary *)node;
			unode->FuncId=Exec_unary;
			InitExprAtPhysicalPlan(unode->next);
			unode->function_call=ExectorFunction::operator_function[unode->actual_type][unode->op_type];
			unode->type_cast_func=TypeCast::type_cast_func[unode->actual_type][unode->return_type];
			unode->value=memalign(cacheline_size,unode->length);
		}break;
		case t_qexpr_ternary:
		{
			QExpr_ternary *tnode=(QExpr_ternary *)node;
			tnode->FuncId=Exec_ternary;
			InitExprAtPhysicalPlan(tnode->next0);
			InitExprAtPhysicalPlan(tnode->next1);
			InitExprAtPhysicalPlan(tnode->next2);
			tnode->function_call=ExectorFunction::operator_function[tnode->actual_type][tnode->op_type];
			tnode->type_cast_func=TypeCast::type_cast_func[tnode->actual_type][tnode->return_type];
			tnode->value=memalign(cacheline_size,tnode->length);
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
			qcol->type_cast_func=TypeCast::type_cast_func[qcol->actual_type][qcol->return_type];
			qcol->value=memalign(cacheline_size,qcol->length);
		}break;
		case t_qexpr:
		{
			QExpr *qexpr=(QExpr *)(node);
			qexpr->FuncId=getConst;
			qexpr->value=memalign(cacheline_size,qexpr->length);
			strcpy((char *)qexpr->value,qexpr->const_value.c_str());//change the storage style from string to char *,so store the value in the return_type[]
			TypeCast::type_cast_func[t_string][qexpr->return_type](qexpr->value,qexpr->value);
		}break;
		default:
		{

		}


	}
}
