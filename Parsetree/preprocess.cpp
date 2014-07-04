#include <cstdio>
#include <map>
#include <new>
#include <set>
#include <string>
#include <vector>
#include<malloc.h>
#include<string.h>
#include "../Catalog/Attribute.h"
#include "../Catalog/Catalog.h"
#include "../Catalog/table.h"

#include "../common/Comparator.h"
#include "../common/Logging.h"

#include "../LogicalQueryPlan/Aggregation.h"
#include "../LogicalQueryPlan/EqualJoin.h"
#include "../LogicalQueryPlan/Filter.h"
#include "../LogicalQueryPlan/LogicalOperator.h"
#include "../LogicalQueryPlan/Scan.h"

#include "../BlockStreamIterator/ParallelBlockStreamIterator/BlockStreamAggregationIterator.h"

#include "sql_node_struct.h"
#include "../Environment.h"

int getlevel(Expr_cal *calnode)
{
	int level=0;
	if(strcmp(calnode->sign,"--")==0)//处理负号的初步方法，处理-的时候，在前面加0，即=（0-(expr))
	{
		level=4;
	}
	else if(strcmp(calnode->sign,"++")==0)
	{
		level=4;
	}
	else
	{
		if(strcmp(calnode->sign,"CMP")==0)
		{
			switch(calnode->cmp)
			{
				case 1://"<"
				{
					level=1;
				}break;
				case 2://">"
				{
					level=1;
				}break;
				case 3://"<>"
				{
					level=1;
				}break;
				case 4://"="
				{
					level=1;
				}break;
				case 5://"<="
				{
					level=1;
				}break;
				case 6://">="
				{
					level=1;
				}break;
				default:
				{
				}
			}
		}
		else
		{
			if(strcmp(calnode->sign,"+")==0||strcmp(calnode->sign,"-")==0)
			{
				level=2;
			}
			else if(strcmp(calnode->sign,"*")==0||strcmp(calnode->sign,"/")==0||strcmp(calnode->sign,"%")==0)
			{
				level=3;
			}
			else
			{
				level=-1;
				SQLParse_elog("level is unknown!!!!!!!!!!");
			}

		}
	}
	return level;
}
string expr_to_str(Node * node,int level)
{
	string str="";
	int thislevel=0;
	switch(node->type)
	{
		case t_expr_func:
		{
			Expr_func * funcnode=(Expr_func *)node;
			if(strcmp(funcnode->funname,"CASE3")==0)
			{
				str="case ";
				str=str+expr_to_str(funcnode->parameter1,0);
				str=str+" end";
			}
			else if(strcmp(funcnode->funname,"CASE4")==0)
			{
				str="case ";
				str=str+expr_to_str(funcnode->parameter1,0);
				str=str+" else ";
				str=str+expr_to_str(funcnode->parameter2,0);
				str=str+" end";
			}
			else if(strcmp(funcnode->funname,"WHEN1")==0)
			{
				str="when ";
				str=str+expr_to_str(funcnode->parameter1,0);
				str=str+" then ";
				str=str+expr_to_str(funcnode->parameter2,0);
			}
			else if(strcmp(funcnode->funname,"WHEN2")==0)
			{
				str="when ";
				str=str+expr_to_str(funcnode->parameter1,0);
				str=str+" then ";
				str=str+expr_to_str(funcnode->parameter2,0);
			}
			else if(strcmp(funcnode->funname,"FSUBSTRING0")==0)
			{
				str="substr(";
				str=str+expr_to_str(funcnode->args,0);
				str=str+" , ";
				str=str+expr_to_str(funcnode->parameter1,0);
				str=str+")";
			}
			else if(strcmp(funcnode->funname,"FSUBSTRING1")==0)
			{
				str="substr(";
				str=str+expr_to_str(funcnode->args,0);
				str=str+" , ";
				str=str+expr_to_str(funcnode->parameter1,0);
				str=str+" , ";
				str=str+expr_to_str(funcnode->parameter2,0);
				str=str+")";
			}
			else if(strcmp(funcnode->funname,"FTRIM0")==0)
			{
				str="trim(";
				str=str+"both ";
				str=str+expr_to_str(funcnode->parameter1,0);
				str=str+"from ";
				str=str+expr_to_str(funcnode->parameter2,0);
				str=str+")";
			}
			else if(strcmp(funcnode->funname,"FTRIM1")==0)
			{
				str="trim(";
				str=str+"trailing ";
				str=str+expr_to_str(funcnode->parameter1,0);
				str=str+"from ";
				str=str+expr_to_str(funcnode->parameter2,0);
				str=str+")";
			}
			else if(strcmp(funcnode->funname,"FTRIM2")==0)
			{
				str="trim(";
				str=str+"leading ";
				str=str+expr_to_str(funcnode->parameter1,0);
				str=str+"from ";
				str=str+expr_to_str(funcnode->parameter2,0);
				str=str+")";

			}
			else if(strcmp(funcnode->funname,"FTRIM3")==0)
			{
				str="trim(";
				str=str+expr_to_str(funcnode->parameter1,0);
				str=str+")";
			}
			else if(strcmp(funcnode->funname,"FUPPER")==0)
			{
				str="upper(";
				str=str+expr_to_str(funcnode->parameter1,0);
				str=str+")";
			}
			else if(strcmp(funcnode->funname,"FCAST")==0)
			{
				str="case(";
				str=str+expr_to_str(funcnode->parameter1,0);
				str=str+" as ";
				str=str+expr_to_str(funcnode->parameter2,0);
				str=str+")";
			}
			else if(strcmp(funcnode->funname,"FCOALESCE")==0)
			{

			}
			else if(strcmp(funcnode->funname,"FCOUNTALL")==0)
			{
				str="count(*)";
			}
			else if(strcmp(funcnode->funname,"FCOUNT")==0)
			{
				str="count(";
				str=str+expr_to_str(funcnode->parameter1,0);
				str=str+")";
			}
			else if(strcmp(funcnode->funname,"FSUM")==0)
			{
				str="sum(";
				str=str+expr_to_str(funcnode->parameter1,0);
				str=str+")";
			}
			else if(strcmp(funcnode->funname,"FAVG")==0)
			{
				str="avg(";
				str=str+expr_to_str(funcnode->parameter1,0);
				str=str+")";
			}
			else if(strcmp(funcnode->funname,"FMIN")==0)
			{
				str="min(";
				str=str+expr_to_str(funcnode->parameter1,0);
				str=str+")";
			}
			else if(strcmp(funcnode->funname,"FMAX")==0)
			{
				str="max(";
				str=str+expr_to_str(funcnode->parameter1,0);
				str=str+")";
			}
			else
			{
				SQLParse_elog("expr_to_str doesn't exist this function !!!");
			}
			funcnode->str=(char *)malloc(str.size()+1);
			strcpy(funcnode->str,str.c_str());
		}break;
		case t_expr_cal:
		{
			Expr_cal * calnode=(Expr_cal *)node;
			thislevel=getlevel(calnode);
			if(strcmp(calnode->sign,"--")==0)//处理负号的初步方法，处理-的时候，在前面加0，即=（0-(expr))
			{
				str="-";
			}
			else if(strcmp(calnode->sign,"++")==0)
			{
				str="+";
			}
			else
			{
				str=expr_to_str(calnode->lnext,thislevel);
				if(strcmp(calnode->sign,"CMP")==0)
				{
					switch(calnode->cmp)
					{
						case 1://"<"
						{
							str=str+"<";
						}break;
						case 2://">"
						{
							str=str+">";
						}break;
						case 3://"<>"
						{
							str=str+"<>";
						}break;
						case 4://"="
						{
							str=str+"=";
						}break;
						case 5://"<="
						{
							str=str+"<=";
						}break;
						case 6://">="
						{
							str=str+">=";
						}break;
						default:
						{
						}
					}
				}
				else
				{

					str=str+calnode->sign;
				}
			}

	    	str=str+expr_to_str(calnode->rnext,thislevel);
			calnode->str=(char *)malloc(str.size()+1);
			strcpy(calnode->str,str.c_str());
			if(thislevel<level)
			{
				str="("+str+")";
			}
//			memcpy(calnode->str,str.c_str(),str.size());
		}break;
		case t_name:
		case t_name_name:
		{
			Columns *col=(Columns *)node;
			if(col->parameter1==NULL)
			{
				str=str+string(col->parameter2);
			}
			else
			{
				str=str+string(col->parameter2);
			}
		}break;
		case t_stringval:
		{
			Expr * exprval=(Expr *)node;
			str=str+string(exprval->data);
		}break;
		case t_intnum:
		{
			Expr * exprval=(Expr *)node;
			str=str+string(exprval->data);
		}break;
		case t_approxnum:
		{
			Expr * exprval=(Expr *)node;
			str=str+string(exprval->data);
		}break;
		case t_bool:
		{

		}break;
		default:
		{

		}
	}
	return str;
}
void expr_to_str_for_selectlist(Node *node)
{
	if(node==NULL)
		return;
	for(Node *p=node;p!=NULL;)
	{
		Select_list *selectlist=(Select_list *)p;
		Select_expr *sexpr=(Select_expr *)selectlist->args;
		expr_to_str(sexpr->colname,0);
		p=selectlist->next;
	}
}
void expr_to_str_for_groupby(Node* node)
{
	if(node==NULL)
		return;
	Groupby_list *gplist=(Groupby_list *)node;
	for(Node *p=(Node *)(gplist->next);p!=NULL;)
	{
		Groupby_expr *gbexpr=(Groupby_expr *)p;
		expr_to_str(gbexpr->args,0);
		p=gbexpr->next;
	}
}
void expr_to_str_for_orderby(Node* node)
{
	if(node==NULL)
		return;
	Orderby_list * gblist=(Orderby_list *)node;
	for(Node *p=(Node *)(gblist->next);p!=NULL;)
	{
		Groupby_expr *obexpr=(Groupby_expr *)p;
		expr_to_str(obexpr->args,0);
		p=obexpr->next;
	}
}
void expr_to_str_test(Node *node)
{
	Query_stmt *ptree=(Query_stmt *)node;
	expr_to_str_for_selectlist(ptree->select_list);
	expr_to_str_for_groupby(ptree->groupby_list);
	expr_to_str_for_orderby(ptree->orderby_list);
}
void solve_const_value_in_wherecondition(Node *&cur)
{
	switch(cur->type)
	{
		case t_name:
		{
			Columns *col=(Columns *)cur;

		}break;
		case t_name_name:
		{
			Columns *col=(Columns *)cur;

		}break;
		case t_query_stmt:
		{

		}break;
		case t_expr_cal:
		{
			Expr_cal *node=(Expr_cal*)cur;
			solve_const_value_in_wherecondition(node->lnext);
			solve_const_value_in_wherecondition(node->rnext);
		}break;
		case t_expr_func:
		{
			Expr_func* node=(Expr_func *)cur;
			string datestr;
			date constdate(from_string("1990-10-01"));
			if(strcmp(node->funname,"FDATE_ADD")==0)
			{
				if(node->args->type==t_stringval)
				{
					datestr=string(((Expr *)node->args)->data);
					Expr_func *datefunc=(Expr_func *)node->parameter1;

					assert(((Expr *)datefunc->args)->type==t_intnum);

					if(strcmp(datefunc->funname,"INTERVAL_DAY")==0)
					{
						date_duration dd(atof(((Expr *)datefunc->args)->data));
						constdate=date(from_string(datestr))+dd;
					}
					else if(strcmp(datefunc->funname,"INTERVAL_WEEK")==0)
					{
						weeks dd(atof(((Expr *)datefunc->args)->data));
						constdate=date(from_string(datestr))+dd;
					}
					else if(strcmp(datefunc->funname,"INTERVAL_MONTH")==0)
					{
						months dd(atof(((Expr *)datefunc->args)->data));
						constdate=date(from_string(datestr))+dd;
					}
					else if(strcmp(datefunc->funname,"INTERVAL_YEAR")==0)
					{
						years dd(atof(((Expr *)datefunc->args)->data));
						constdate=date(from_string(datestr))+dd;
					}
					else if(strcmp(datefunc->funname,"INTERVAL_QUARTER")==0)
					{
						months dd(atof(((Expr *)datefunc->args)->data)*3);
						constdate=date(from_string(datestr))+dd;
					}
					datestr=to_simple_string(constdate);
					char *datechar=(char *)malloc(datestr.length()+2);
					strcpy(datechar,datestr.c_str());
					SQLParse_log("the date result after date_add, string= %s ------------------\n",datechar);
					//free(cur);
					cur=(Node *)newExpr(t_dateval,datechar);
				}

			}
			else if(strcmp(node->funname,"FDATE_SUB")==0)
			{
				if(node->args->type==t_stringval)
				{
					datestr=string(((Expr *)node->args)->data);
					Expr_func *datefunc=(Expr_func *)node->parameter1;

					assert(((Expr *)datefunc->args)->type==t_intnum);

					if(strcmp(datefunc->funname,"INTERVAL_DAY")==0)
					{
						date_duration dd(atof(((Expr *)datefunc->args)->data));
						constdate=date(from_string(datestr))-dd;
					}
					else if(strcmp(datefunc->funname,"INTERVAL_WEEK")==0)
					{
						weeks dd(atof(((Expr *)datefunc->args)->data));
						constdate=date(from_string(datestr))-dd;
					}
					else if(strcmp(datefunc->funname,"INTERVAL_MONTH")==0)
					{
						months dd(atof(((Expr *)datefunc->args)->data));
						constdate=date(from_string(datestr))-dd;
					}
					else if(strcmp(datefunc->funname,"INTERVAL_YEAR")==0)
					{
						years dd(atof(((Expr *)datefunc->args)->data));
						constdate=date(from_string(datestr))-dd;
					}
					else if(strcmp(datefunc->funname,"INTERVAL_QUARTER")==0)
					{
						months dd(atof(((Expr *)datefunc->args)->data)*3);
						constdate=date(from_string(datestr))-dd;
					}
					datestr=to_simple_string(constdate);
					char *datechar=(char *)malloc(datestr.length()+2);
					strcpy(datechar,datestr.c_str());
					SQLParse_log("the date result after date_sub, string= %s ------------------\n",datechar);
					//free(cur);
					cur=(Node *)newExpr(t_dateval,datechar);
				}
			}
			else
			{

			}
		}break;
		case t_expr_list:
		{
			Expr_list * node=(Expr_list *)cur;

		}break;
		default:
		{
				SQLParse_elog("solve_const_value_in_wherecondition can't know the type=%d\n",cur->type);
		}
	}
}
void solve_const_value(Node *node)
{
	Query_stmt *ptree=(Query_stmt *)node;
	if(((Where_list*)ptree->where_list)!=NULL)
	{
		solve_const_value_in_wherecondition(((Where_list*)ptree->where_list)->next);
	}
}
void preprocess(Node *node)
{
	expr_to_str_test(node);
	solve_const_value(node);
}
