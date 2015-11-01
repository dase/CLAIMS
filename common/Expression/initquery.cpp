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
#include "../../Parsetree/sql_node_struct.h"
#include "../../logical_operator/logical_operator.h"
/*
 * the transformqual() transform the ast(the parsetree) to expression tree
 */
QNode * transformqual(Node *node,LogicalOperator* child)
{
//	memalign(cacheline_size,1024)
	if(node==NULL)
		return NULL;
	switch(node->type)
	{
		case t_expr_cal:
		{
			Expr_cal * calnode=(Expr_cal *)node;
			if(strcmp(calnode->sign,"ANDOP")==0)//now in and the two parameter should be boolean,so the a_type = t_boolean
			{
				QNode *lnode=transformqual(calnode->lnext,child);
				QNode *rnode=transformqual(calnode->rnext,child);
				QExpr_binary *qcalnode=new QExpr_binary(lnode,rnode,t_boolean,oper_and,t_qexpr_cal,calnode->str);
				return qcalnode;
			}
			else if(strcmp(calnode->sign,"OR")==0)//now in or the two parameter should be boolean,so the a_type = t_boolean
			{
				QNode *lnode=transformqual(calnode->lnext,child);
				QNode *rnode=transformqual(calnode->rnext,child);
				QExpr_binary *qcalnode=new QExpr_binary(lnode,rnode,t_boolean,oper_or,t_qexpr_cal,calnode->str);
				return qcalnode;
			}
			else if(strcmp(calnode->sign,"+")==0)
			{
				QNode *lnode=transformqual(calnode->lnext,child);
				QNode *rnode=transformqual(calnode->rnext,child);
				data_type a_type=TypePromotion::arith_type_promotion_map[lnode->actual_type][rnode->actual_type];
				QExpr_binary *qcalnode=new QExpr_binary(lnode,rnode,a_type,oper_add,t_qexpr_cal,calnode->str);
				return qcalnode;
			}
			else if(strcmp(calnode->sign,"-")==0)
			{
				QNode *lnode=transformqual(calnode->lnext,child);
				QNode *rnode=transformqual(calnode->rnext,child);
				data_type a_type=TypePromotion::arith_type_promotion_map[lnode->actual_type][rnode->actual_type];
				QExpr_binary *qcalnode=new QExpr_binary(lnode,rnode,a_type,oper_minus,t_qexpr_cal,calnode->str);
				return qcalnode;
			}
			else if(strcmp(calnode->sign,"*")==0)
			{
				QNode *lnode=transformqual(calnode->lnext,child);
				QNode *rnode=transformqual(calnode->rnext,child);
				data_type a_type=TypePromotion::arith_type_promotion_map[lnode->actual_type][rnode->actual_type];
				QExpr_binary *qcalnode=new QExpr_binary(lnode,rnode,a_type,oper_multiply,t_qexpr_cal,calnode->str);
				return qcalnode;
			}
			else if(strcmp(calnode->sign,"/")==0)
			{
				QNode *lnode=transformqual(calnode->lnext,child);
				QNode *rnode=transformqual(calnode->rnext,child);
				data_type a_type=TypePromotion::arith_type_promotion_map[lnode->actual_type][rnode->actual_type];
				QExpr_binary *qcalnode=new QExpr_binary(lnode,rnode,a_type,oper_divide,t_qexpr_cal,calnode->str);
				return qcalnode;
			}
			else if(strcmp(calnode->sign,"%")==0)
			{
				QNode *lnode=transformqual(calnode->lnext,child);
				QNode *rnode=transformqual(calnode->rnext,child);
				data_type a_type=TypePromotion::arith_type_promotion_map[lnode->actual_type][rnode->actual_type];
				QExpr_binary *qcalnode=new QExpr_binary(lnode,rnode,a_type,oper_mod,t_qexpr_cal,calnode->str);
				return qcalnode;
			}
			else if(strcmp(calnode->sign,"LIKE")==0)
			{
				QNode *lnode=transformqual(calnode->lnext,child);
				QNode *rnode=transformqual(calnode->rnext,child);
				data_type a_type=TypePromotion::arith_type_promotion_map[lnode->actual_type][rnode->actual_type];
				QExpr_binary *likenode=new QExpr_binary(lnode,rnode,a_type,oper_like,t_qexpr_cmp,calnode->str);
				return likenode;
			}
			else if(strcmp(calnode->sign,"NLIKE")==0)
			{
				QNode *lnode=transformqual(calnode->lnext,child);
				QNode *rnode=transformqual(calnode->rnext,child);
				data_type a_type=TypePromotion::arith_type_promotion_map[lnode->actual_type][rnode->actual_type];
				QExpr_binary *likenode=new QExpr_binary(lnode,rnode,a_type,oper_not_like,t_qexpr_cmp,calnode->str);
				return likenode;
			}
			else if(strcmp(calnode->sign,"NOT")==0||strcmp(calnode->sign,"!")==0)
			{
				QNode *nnode=transformqual(calnode->rnext,child);
				QExpr_unary *unode=new QExpr_unary(nnode,t_boolean,oper_not,t_qexpr_unary,calnode->str);
				return unode;
			}
			else if(strcmp(calnode->sign,"--")==0)
			{
				QNode *nnode=transformqual(calnode->rnext,child);
				QExpr_unary *unode=new QExpr_unary(nnode,nnode->actual_type,oper_negative,t_qexpr_unary,calnode->str);
				return unode;
			}
			else if(strcmp(calnode->sign,"INVS")==0)//in (2,3),one level list,one comparison parameter
			{
				vector<QNode *>lnode,tmp;
				vector< vector<QNode *> >rnode;
				lnode.push_back(transformqual(calnode->lnext,child));//only one
				int index=0;
				for(Node *tpnode=calnode->rnext;tpnode!=NULL;)
				{
					Expr_list *elnode=(Expr_list *)tpnode;
					tmp.push_back(transformqual(elnode->data,child));//only one column,more rows
					tpnode=elnode->next;
					rnode.push_back(tmp);
					tmp.clear();
				}
				vector<QNode *>cmpnode;
				for(int i=0;i<lnode.size();i++)
				{
					data_type a_type=TypePromotion::arith_type_promotion_map[lnode[i]->actual_type][rnode[0][i]->actual_type];
					QExpr_binary *qcalnode=new QExpr_binary(lnode[i],rnode[0][i],a_type,oper_equal,t_qexpr_cmp,"tempnode");
					cmpnode.push_back(qcalnode);
				}
				QNode *innode=new QExpr_in(cmpnode,rnode,calnode->str);
				return innode;
			}
			else if(strcmp(calnode->sign,"INVM")==0)//in ((2,'a'),(3,'b')),two level list,more comparison parameter
			{
				vector<QNode *>lnode,tmp;
				vector< vector<QNode *> >rnode;
				for(Node *tpnode=calnode->lnext;tpnode!=NULL;)//more columns
				{
					Expr_list *elnode=(Expr_list *)tpnode;
					lnode.push_back(transformqual(elnode->data,child));
					tpnode=elnode->next;
				}
				int index=0;
				for(Node *tpnode=calnode->rnext;tpnode!=NULL;)//more rows
				{
					Expr_list *elnode=(Expr_list *)tpnode;
					for(Node *pnode=elnode->data;pnode!=NULL;)//more columns
					{
						Expr_list *enode=(Expr_list *)pnode;
						tmp.push_back(transformqual(enode->data,child));
						pnode=enode->next;
					}
					rnode.push_back(tmp);
					tmp.clear();
					index++;
					tpnode=elnode->next;
				}
				vector<QNode *>cmpnode;
				for(int i=0;i<lnode.size();i++)//more comparisons form one list
				{
					data_type a_type=TypePromotion::arith_type_promotion_map[lnode[i]->actual_type][rnode[0][i]->actual_type];
					QExpr_binary *qcalnode=new QExpr_binary(lnode[i],rnode[0][i],a_type,oper_equal,t_qexpr_cmp,"tempnode");
					cmpnode.push_back(qcalnode);
				}
				QNode *innode=new QExpr_in(cmpnode,rnode,calnode->str);
				return innode;
			}
			else if(strcmp(calnode->sign,"CMP")==0)
			{
				QNode *lnode=transformqual(calnode->lnext,child);
				QNode *rnode=transformqual(calnode->rnext,child);
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
			if(strcmp(funcnode->funname,"CASE3")==0)//no END in CASE...WHEN
			{
				vector<QNode *>qual;
				vector<QNode *>ans;
				for(Node *tnode=funcnode->parameter1;tnode!=NULL;)
				{
					Expr_func *tfnode=(Expr_func *)tnode;
					assert(strcmp(tfnode->funname,"WHEN2")==0);
					qual.push_back(transformqual(tfnode->parameter1,child));
					ans.push_back(transformqual(tfnode->parameter2,child));
					tnode=tfnode->next;
				}
				QNode *cwnode=new QExpr_case_when(qual,ans,funcnode->str);
				return cwnode;
			}
			else if(strcmp(funcnode->funname,"CASE4")==0)//now just support |case [when expr then expr]* [else expr] end|
			{
				vector<QNode *>qual;
				vector<QNode *>ans;
				for(Node *tnode=funcnode->parameter1;tnode!=NULL;)
				{
					Expr_func *tfnode=(Expr_func *)tnode;
					assert(strcmp(tfnode->funname,"WHEN2")==0);
					qual.push_back(transformqual(tfnode->parameter1,child));
					ans.push_back(transformqual(tfnode->parameter2,child));
					tnode=tfnode->next;
				}
				ans.push_back(transformqual(funcnode->parameter2,child));
				QNode *cwnode=new QExpr_case_when(qual,ans,funcnode->str);
				return cwnode;
			}
			else if(strcmp(funcnode->funname,"WHEN1")==0)//no use
			{

			}
			else if(strcmp(funcnode->funname,"WHEN2")==0)//no use
			{

			}
			else if(strcmp(funcnode->funname,"FSUBSTRING0")==0)//TODO the max length is 128,but it may be larger in practice
			{
				QNode *node0=transformqual(funcnode->args,child);
				QNode *node1=transformqual(funcnode->parameter1,child);
				QNode *node2=new QExpr("128",t_int,"t_const_node");//128 is the size of the qnode->value
				data_type a_type=t_string;
				QExpr_ternary *substrnode=new QExpr_ternary(node0,node1,node2,a_type,oper_substring,t_qexpr_ternary,funcnode->str);
				return substrnode;
			}
			else if(strcmp(funcnode->funname,"FSUBSTRING1")==0)
			{
				QNode *node0=transformqual(funcnode->args,child);
				QNode *node1=transformqual(funcnode->parameter1,child);
				QNode *node2=transformqual(funcnode->parameter2,child);
				data_type a_type=t_string;
				QExpr_ternary *substrnode=new QExpr_ternary(node0,node1,node2,a_type,oper_substring,t_qexpr_ternary,funcnode->str);
				return substrnode;
			}
			else if(strcmp(funcnode->funname,"FTRIM0")==0)//both
			{
				QNode *lnode=transformqual(funcnode->parameter1,child);
				QNode *rnode=transformqual(funcnode->parameter2,child);
				data_type a_type=t_string;
				QExpr_binary *trimnode=new QExpr_binary(lnode,rnode,a_type,oper_both_trim,t_qexpr_cal,funcnode->str);
				return trimnode;
			}
			else if(strcmp(funcnode->funname,"FTRIM1")==0)//leading
			{
				QNode *lnode=transformqual(funcnode->parameter1,child);
				QNode *rnode=transformqual(funcnode->parameter2,child);
				data_type a_type=t_string;
				QExpr_binary *trimnode=new QExpr_binary(lnode,rnode,a_type,oper_leading_trim,t_qexpr_cal,funcnode->str);
				return trimnode;
			}
			else if(strcmp(funcnode->funname,"FTRIM2")==0)//trailing
			{
				QNode *lnode=transformqual(funcnode->parameter1,child);
				QNode *rnode=transformqual(funcnode->parameter2,child);
				data_type a_type=t_string;
				QExpr_binary *trimnode=new QExpr_binary(lnode,rnode,a_type,oper_trailing_trim,t_qexpr_cal,funcnode->str);
				return trimnode;
			}
			else if(strcmp(funcnode->funname,"FTRIM3")==0)//both ' '
			{
				QNode *lnode=new QExpr(" ",t_string,"t_const_node");//construct the ' ' const node
				QNode *rnode=transformqual(funcnode->parameter1,child);
				data_type a_type=t_string;
				QExpr_binary *trimnode=new QExpr_binary(lnode,rnode,a_type,oper_both_trim,t_qexpr_cal,funcnode->str);
				return trimnode;
			}
			else if(strcmp(funcnode->funname,"FUPPER")==0)
			{
				QNode *nnext=transformqual(funcnode->parameter1,child);
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
			else if(strcmp(funcnode->funname,"FCOUNTALL")==0)//the agg nodes are changed to QColumns node ,only to get value from tuple is ok
			{
				QNode *aggnode=new QColcumns("",funcnode->str,t_u_long,funcnode->str);
				return aggnode;
			}
			else if(strcmp(funcnode->funname,"FCOUNT")==0)
			{
				QNode *aggnode=new QColcumns("",funcnode->str,t_u_long,funcnode->str);
				return aggnode;
			}
			else if(strcmp(funcnode->funname,"FSUM")==0)
			{
//				QNode *anext=transformqual(funcnode->parameter1,child);
				data_type a_type=child->GetPlanContext().GetAttribute(string(funcnode->str)).attrType->type;
				QNode *aggnode=new QColcumns("",funcnode->str,a_type,funcnode->str);
				return aggnode;
			}
			else if(strcmp(funcnode->funname,"FAVG")==0)
			{
//				QNode *anext=transformqual(funcnode->parameter1,child);
				data_type a_type=child->GetPlanContext().GetAttribute(string(funcnode->str)).attrType->type;
				QNode *aggnode=new QColcumns("",funcnode->str,a_type,funcnode->str);
				return aggnode;
			}
			else if(strcmp(funcnode->funname,"FMIN")==0)
			{
//				QNode *anext=transformqual(funcnode->parameter1,child);
				data_type a_type=child->GetPlanContext().GetAttribute(string(funcnode->str)).attrType->type;
				QNode *aggnode=new QColcumns("",funcnode->str,a_type,funcnode->str);
				return aggnode;
			}
			else if(strcmp(funcnode->funname,"FMAX")==0)
			{
//				QNode *anext=transformqual(funcnode->parameter1,child);
				data_type a_type=child->GetPlanContext().GetAttribute(string(funcnode->str)).attrType->type;
				QNode *aggnode=new QColcumns("",funcnode->str,a_type,funcnode->str);
				return aggnode;
			}
			else if(strcmp(funcnode->funname,"BA")==0)//between...and... transform to arg>=param1 and arg<=param2
			{
				QNode *arg=transformqual(funcnode->args,child);
				QNode *param1=transformqual(funcnode->parameter1,child);
				QNode *param2=transformqual(funcnode->parameter2,child);
				data_type a_type=TypePromotion::arith_type_promotion_map[arg->actual_type][param1->actual_type];
				QNode *lnext=new QExpr_binary(arg,param1,a_type,oper_great_equal,t_qexpr_cmp,"arg>=parma1");
				QNode *rnext=new QExpr_binary(arg,param2,a_type,oper_less_equal,t_qexpr_cmp,"arg<=parma2");
				QExpr_binary *banode=new QExpr_binary(lnext,rnext,t_boolean,oper_and,t_qexpr_cal,funcnode->str);
				return banode;
			}
			/*
			 * special case for solving FDATE_ADD and FDATE_SUB
			 * FDATE_ADD(date_str,deta)
			 * the date_str,deta and the return type are all t_string
			 * so take them as  string_date_add and string_date_sub
			 * the function above is slow ,so construct special QExpr_date_add_sub node
			 */
			else if(strcmp(funcnode->funname,"FDATE_ADD")==0)
			{
				QNode *lnext=transformqual(funcnode->args,child);
				Expr_func *datefunc=(Expr_func *)funcnode->parameter1;
				QExpr_date_add_sub *date_add;
				if(strcmp(datefunc->funname,"INTERVAL_DAY")==0)
				{
					QNode *rnext=transformqual(datefunc->args,child);
					date_add=new QExpr_date_add_sub(lnext,rnext,t_date,oper_date_add_day,t_qexpr_date_add_sub,t_date_day,funcnode->str);
				}
				else if(strcmp(datefunc->funname,"INTERVAL_WEEK")==0)
				{
					QNode *rnext=transformqual(datefunc->args,child);
					date_add=new QExpr_date_add_sub(lnext,rnext,t_date,oper_date_add_week,t_qexpr_date_add_sub,t_date_week,funcnode->str);
				}
				else if(strcmp(datefunc->funname,"INTERVAL_MONTH")==0)
				{
					QNode *rnext=transformqual(datefunc->args,child);
					date_add=new QExpr_date_add_sub(lnext,rnext,t_date,oper_date_add_month,t_qexpr_date_add_sub,t_date_month,funcnode->str);
				}
				else if(strcmp(datefunc->funname,"INTERVAL_YEAR")==0)
				{
					QNode *rnext=transformqual(datefunc->args,child);
					date_add=new QExpr_date_add_sub(lnext,rnext,t_date,oper_date_add_year,t_qexpr_date_add_sub,t_date_year,funcnode->str);
				}
				else if(strcmp(datefunc->funname,"INTERVAL_QUARTER")==0)
				{
					QNode *rnext=transformqual(datefunc->args,child);
					date_add=new QExpr_date_add_sub(lnext,rnext,t_date,oper_date_add_month,t_qexpr_date_add_sub,t_date_quarter,funcnode->str);
				}
				return date_add;
			}
			else if(strcmp(funcnode->funname,"FDATE_SUB")==0)
			{
				QNode *lnext=transformqual(funcnode->args,child);
				Expr_func *datefunc=(Expr_func *)funcnode->parameter1;
				QExpr_date_add_sub *date_sub;
				if(strcmp(datefunc->funname,"INTERVAL_DAY")==0)
				{
					QNode *rnext=transformqual(datefunc->args,child);
					date_sub=new QExpr_date_add_sub(lnext,rnext,t_date,oper_date_sub_day,t_qexpr_date_add_sub,t_date_day,funcnode->str);
				}
				else if(strcmp(datefunc->funname,"INTERVAL_WEEK")==0)
				{
					QNode *rnext=transformqual(datefunc->args,child);
					date_sub=new QExpr_date_add_sub(lnext,rnext,t_date,oper_date_sub_week,t_qexpr_date_add_sub,t_date_week,funcnode->str);
				}
				else if(strcmp(datefunc->funname,"INTERVAL_MONTH")==0)
				{
					QNode *rnext=transformqual(datefunc->args,child);
					date_sub=new QExpr_date_add_sub(lnext,rnext,t_date,oper_date_sub_month,t_qexpr_date_add_sub,t_date_month,funcnode->str);
				}
				else if(strcmp(datefunc->funname,"INTERVAL_YEAR")==0)
				{
					QNode *rnext=transformqual(datefunc->args,child);
					date_sub=new QExpr_date_add_sub(lnext,rnext,t_date,oper_date_sub_year,t_qexpr_date_add_sub,t_date_year,funcnode->str);
				}
				else if(strcmp(datefunc->funname,"INTERVAL_QUARTER")==0)
				{
					QNode *rnext=transformqual(datefunc->args,child);
					date_sub=new QExpr_date_add_sub(lnext,rnext,t_date,oper_date_sub_month,t_qexpr_date_add_sub,t_date_quarter,funcnode->str);
				}
				return date_sub;
			}
			else
			{
				SQLParse_elog("get_a_expression_item: %s is null",funcnode->funname);
			}

		}break;
		case t_name:
		case t_name_name:
		{
			Columns *col=(Columns *)node;
//			data_type a_type=Environment::getInstance()->getCatalog()->name_to_table[col->parameter1]->getAttribute2(col->parameter2).attrType->type;
			data_type a_type=child->GetPlanContext().GetAttribute(string(col->parameter2)).attrType->type;
			if(col->parameter1==NULL)//for temporary variable
			{
				col->parameter1="";
			}

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
			QExpr *qexpr = NULL;
			long temp = atol(exprval->data);
			if (temp < INT_MAX) {
			  qexpr=new QExpr(exprval->data,t_int,exprval->data);
			} else {
			  qexpr=new QExpr(exprval->data, t_u_long, exprval->data);
			}
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
/*
 * the InitExprAtLogicalPlan() initialize the exprTree nodes at logical plan
 * set return type
 * get column reference id in schema
 * get isnull
 * get the storage length
 */
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
			cmpnode->return_type=r_type; // Li: I believe the return type for compare expression should be t_boolean
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
		case t_qexpr_ternary://now for substring,not for all
		{
			QExpr_ternary *tnode=(QExpr_ternary *)node;
			tnode->return_type=r_type;
			InitExprAtLogicalPlan(tnode->next0,tnode->actual_type,colindex,schema);
			InitExprAtLogicalPlan(tnode->next1,tnode->next1->actual_type,colindex,schema);//parameter return type =actual type
			InitExprAtLogicalPlan(tnode->next2,tnode->next2->actual_type,colindex,schema);//parameter return type =actual type
			tnode->length=max(tnode->next0->length,max(tnode->next1->length,tnode->next2->length));
			tnode->isnull=(tnode->next0->isnull||tnode->next1->isnull||tnode->next2->isnull);
		}break;
		case t_qexpr_case_when:
		{
			QExpr_case_when *cwnode=(QExpr_case_when *)node;
			cwnode->return_type=r_type;
			cwnode->length=BASE_SIZE;
			cwnode->isnull=true;
			for(int i=0;i<cwnode->qual.size();i++)
			{
				InitExprAtLogicalPlan(cwnode->qual[i],t_boolean,colindex,schema);
			}
			for(int i=0;i<cwnode->ans.size();i++)
			{
				InitExprAtLogicalPlan(cwnode->ans[i],cwnode->ans[i]->actual_type,colindex,schema);
				cwnode->length=max(cwnode->length,cwnode->ans[i]->length);
				cwnode->isnull=(cwnode->isnull||cwnode->ans[i]->isnull);
			}
		}break;
		case t_qexpr_in:
		{
			QExpr_in *innode=(QExpr_in *)node;
			innode->return_type=r_type;
			innode->length=BASE_SIZE;
			innode->isnull=true;
			for(int i=0;i<innode->cmpnode.size();i++)
			{
				InitExprAtLogicalPlan(innode->cmpnode[i],t_boolean,colindex,schema);
			}
			for(int i=0;i<innode->rnode.size();i++)
			{
				for(int j=0;j<innode->rnode[i].size();j++)
				{
					InitExprAtLogicalPlan(innode->rnode[i][j],innode->cmpnode[j]->actual_type,colindex,schema);
				}
			}
		}break;
		case t_qexpr_date_add_sub:
		{
			QExpr_date_add_sub * date_node=(QExpr_date_add_sub *)(node);
			date_node->return_type=r_type;
			InitExprAtLogicalPlan(date_node->lnext,date_node->actual_type,colindex,schema);
			InitExprAtLogicalPlan(date_node->rnext,date_node->rnext_type,colindex,schema);//the difference between t_qexpr_date_add_sub and t_qexpr_cal
			date_node->isnull=(date_node->lnext->isnull||date_node->rnext->isnull);
			date_node->length=max(date_node->lnext->length,date_node->rnext->length);
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
			if(qcol->return_type ==t_string)
				qcol->length=max(schema->getcolumn(qcol->id).get_length(),(unsigned int)BASE_SIZE);
			else
				qcol->length=schema->getcolumn(qcol->id).size;
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
/*
 * the InitExprAtPhysicalPlan() initialize the exprtree at physical plan
 * set node->function_call
 * set node->FuncId
 * allocate room for storing value
 */
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
//			cmpnode->actual_type=t_boolean;//
			cmpnode->type_cast_func=TypeCast::type_cast_func[t_boolean][cmpnode->return_type];
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
		case t_qexpr_case_when:
		{
			QExpr_case_when *cwnode=(QExpr_case_when *)node;
			for(int i=0;i<cwnode->qual.size();i++)
			{
				InitExprAtPhysicalPlan(cwnode->qual[i]);
			}
			for(int i=0;i<cwnode->ans.size();i++)
			{
				InitExprAtPhysicalPlan(cwnode->ans[i]);
			}
			cwnode->FuncId=Exec_case_when;
			cwnode->type_cast_func=TypeCast::type_cast_func[cwnode->actual_type][cwnode->return_type];
			cwnode->value=memalign(cacheline_size,cwnode->length);
		}break;
		case t_qexpr_in:
		{
			QExpr_in *innode=(QExpr_in *)node;
			for(int i=0;i<innode->cmpnode.size();i++)
			{
				InitExprAtPhysicalPlan(innode->cmpnode[i]);
			}
			for(int i=0;i<innode->rnode.size();i++)
			{
				for(int j=0;j<innode->rnode[i].size();j++)
				{
					InitExprAtPhysicalPlan(innode->rnode[i][j]);
				}
			}
			innode->FuncId=Exec_in;
			innode->type_cast_func=TypeCast::type_cast_func[innode->actual_type][innode->return_type];
			innode->value=memalign(cacheline_size,innode->length);
		}break;
		case t_qexpr_date_add_sub:
		{
			QExpr_date_add_sub * date_node=(QExpr_date_add_sub *)(node);
			date_node->FuncId=Exec_date_add_sub;
			InitExprAtPhysicalPlan(date_node->lnext);
			InitExprAtPhysicalPlan(date_node->rnext);
			date_node->function_call=ExectorFunction::operator_function[date_node->actual_type][date_node->op_type];
			date_node->type_cast_func=TypeCast::type_cast_func[date_node->actual_type][date_node->return_type];
			date_node->value=memalign(cacheline_size,date_node->length);
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
		case t_qexpr://copy the value from conststring to node->value,and the data type has casted
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
