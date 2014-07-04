#ifndef __PARSERTREE__
#define __PARSERTREE__
#include <cstdio>

#include <map>
#include <new>
#include <set>
#include <string>
#include <vector>

#include "sql_node_struct.h"
#include "../Environment.h"

#include "../Catalog/Attribute.h"
#include "../Catalog/Catalog.h"
#include "../Catalog/table.h"

#include "../common/Comparator.h"
#include "../common/ExpressionItem.h"
#include "../common/Logging.h"
#include "../common/AttributeComparator.h"

#include "../LogicalQueryPlan/EqualJoin.h"
#include "../LogicalQueryPlan/Filter.h"
#include "../LogicalQueryPlan/LogicalOperator.h"
#include "../LogicalQueryPlan/Scan.h"
#include "../LogicalQueryPlan/Aggregation.h"
#include "../LogicalQueryPlan/Project.h"
#include "../LogicalQueryPlan/Sort.h"

#include "../BlockStreamIterator/ParallelBlockStreamIterator/BlockStreamAggregationIterator.h"

static LogicalOperator* parsetree2logicalplan(Node *parsetree);
static void getfiltercondition(Node * wcexpr,Filter::Condition &filter_condition,char * tablename,bool &hasin,LogicalOperator* loperator){
	SQLParse_log("getfiltercondition   ");
	//filter_condition.add(catalog->getTable(node->tablename)->getAttribute(4),AttributeComparator::EQ,&order_type_);
//	cout<<"wcexpr->type  "<<wcexpr->type<<endl;
	switch(wcexpr->type)
	{
		case t_expr_cal:
		{
			Expr_cal * node=(Expr_cal *)wcexpr;
			if(strcmp(node->sign,"+")==0)
			{

			}
			else if(strcmp(node->sign,"-")==0)
			{

			}
			else if(strcmp(node->sign,"*")==0)
			{

			}
			else if(strcmp(node->sign,"/")==0)
			{

			}
			else if(strcmp(node->sign,"MOD")==0)
			{

			}
			else if(strcmp(node->sign,"INS")==0)
			{
				hasin=true;
			}
			else if(strcmp(node->sign,"ANDOP")==0)
			{

			}
			else if(strcmp(node->sign,"OR")==0)
			{

			}
			else if(strcmp(node->sign,"XOR")==0)
			{

			}
			else if(strcmp(node->sign,"CMP")==0)
			{
				char * attribute;
				switch((node->lnext)->type)//获得左边的表名
				{
					case t_name:
					{
						Expr *expr=(Expr *)(node->lnext);
						attribute=expr->data;
					}break;
					case t_name_name:
					{
						Columns *col=(Columns *)(node->lnext);
						attribute=col->parameter2;
					}break;
					default:
					{

					}
				};

				switch(node->cmp)
				{
					case 1://"<"
					{
						Expr *expr=(Expr *)(node->rnext);
						char * temp=expr->data;
						filter_condition.add(loperator->getDataflow().getAttribute(attribute),AttributeComparator::L,string(temp));

					}break;
					case 2://">"
					{
						Expr *expr=(Expr *)(node->rnext);
						char * temp=expr->data;
						filter_condition.add(loperator->getDataflow().getAttribute(attribute),AttributeComparator::G,string(temp));
					}break;
					case 3://"<>"
					{
						Expr *expr=(Expr *)(node->rnext);
						char * temp=expr->data;
						filter_condition.add(loperator->getDataflow().getAttribute(attribute),AttributeComparator::NEQ,string(temp));
					}break;
					case 4://"="
					{
						Expr *expr=(Expr *)(node->rnext);
						char * temp=expr->data;
						filter_condition.add(loperator->getDataflow().getAttribute(attribute),AttributeComparator::EQ,string(temp));
					}break;
					case 5://"<="
					{
						Expr *expr=(Expr *)(node->rnext);
						char * temp=expr->data;
						filter_condition.add(loperator->getDataflow().getAttribute(attribute),AttributeComparator::LEQ,string(temp));
					}break;
					case 6://">="
					{
						Expr *expr=(Expr *)(node->rnext);
						char * temp=expr->data;
						filter_condition.add(loperator->getDataflow().getAttribute(attribute),AttributeComparator::GEQ,string(temp));
					}break;

				}
			}
		}break;
		default:
		{
			SQLParse_elog("getfiltercondition type error");
		}
	}
}
static int getjoinpairlist(Node *wcexpr,vector<EqualJoin::JoinPair> &join_pair_list,LogicalOperator *filter_1,LogicalOperator * filter_2)
{
	switch(wcexpr->type)
	{
		case t_expr_cal://目前只支持equaljoin
		{
			Expr_cal *node=(Expr_cal *)wcexpr;
			if(strcmp(node->sign,"CMP")==0)
			{
				switch (node->cmp)
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
						Columns *lnode=(Columns *)(node->lnext);
						Columns *rnode=(Columns *)(node->rnext);
					//	printf("left %s.%s   right %s.%s\n",lnode->parameter1,lnode->parameter2,rnode->parameter1,rnode->parameter2);
						if(filter_1->getDataflow().getAttribute(lnode->parameter1,lnode->parameter2)!=NULL&&filter_2->getDataflow().getAttribute(rnode->parameter1,rnode->parameter2)!=NULL)
						{
							join_pair_list.push_back(EqualJoin::JoinPair(filter_1->getDataflow().getAttribute(lnode->parameter1,lnode->parameter2),filter_2->getDataflow().getAttribute(rnode->parameter1,rnode->parameter2)));
						}
						else if(filter_1->getDataflow().getAttribute(rnode->parameter1,rnode->parameter2)!=NULL&&filter_2->getDataflow().getAttribute(lnode->parameter1,lnode->parameter2)!=NULL)
						{
							join_pair_list.push_back(EqualJoin::JoinPair(filter_1->getDataflow().getAttribute(rnode->parameter1,rnode->parameter2),filter_2->getDataflow().getAttribute(lnode->parameter1,lnode->parameter2)));
						}
						else
						{
							return 0;
						}
					}break;
					case 5://"<="
					{

					}break;
					case 6://">="
					{

					}break;
					default:
					{

					}
				}
			}
			else
			{

			}

		}break;
		default:
		{

		}
	}
	return 1;
}
static LogicalOperator *solve_insubquery(Node *exprnode,LogicalOperator * input)
{
	switch (exprnode->type)
	{
		case t_expr_cal:
		{
			Expr_cal *node=(Expr_cal *)exprnode;
			if(strcmp(node->sign,"INS")==0)
			{
				if(node->rnext->type==t_query_stmt)
				{
					LogicalOperator * sublogicalplan=parsetree2logicalplan(node->rnext);//1.获得原子查询的logicalplan
					Query_stmt * subquery=(Query_stmt *)(node->rnext);
					vector<Attribute> group_by_attributes;
					vector<Attribute> aggregation_attributes;
					for(Node *p=subquery->select_list;p!=NULL;)//2.1获得groupby的属性
					{
						Select_list *selectlist=(Select_list *)p;
						Select_expr *sexpr=(Select_expr *)selectlist->args;
						group_by_attributes.push_back(sublogicalplan->getDataflow().getAttribute(sexpr->ascolname));///????
						p=selectlist->next;
					}//2.2在1中的logicalplan上做groupby
					LogicalOperator * aggrection_sublogicalplan=new Aggregation(group_by_attributes,std::vector<Attribute>(),std::vector<BlockStreamAggregationIterator::State::aggregation>(),sublogicalplan);
					vector<EqualJoin::JoinPair> join_pair_list;
					Node *lp,*sp;
					for(lp=node->lnext,sp=((Query_stmt *)node->rnext)->select_list;lp!=NULL;)//3.1获得equaljoin的左右属性
					{
						Expr_list * lpexpr=(Expr_list *)lp;
						Columns * lcol=(Columns *)lpexpr->data;
						Select_list *spexpr=(Select_list *)sp;
						Columns *rcol=(Columns *)spexpr->args;
						join_pair_list.push_back(EqualJoin::JoinPair(input->getDataflow().getAttribute(lcol->parameter1,lcol->parameter2),sublogicalplan->getDataflow().getAttribute(rcol->parameter1,rcol->parameter2)));
						lp=lpexpr->next;
						sp=spexpr->next;
					}
					LogicalOperator* join_logicalplan=new EqualJoin(join_pair_list,input,aggrection_sublogicalplan);
					return join_logicalplan;
				}
			}
		}break;
		default:
		{

		}
	}

	return NULL;
}
static LogicalOperator* where_from2logicalplan(Node *parsetree)//实现where_from_parsetree(即将where转换到from_list后的)到logicalplan的转换
{
	if(parsetree==NULL)
	{
		return NULL;
	}
	switch(parsetree->type)
	{
		case t_table://table节点获得scan 和在该节点上condition的filter
		{
			Table * node=(Table *)parsetree;
			LogicalOperator* tablescan;
			if(node->issubquery==0)
			{
				tablescan=new LogicalScan(Environment::getInstance()->getCatalog()->getTable(std::string(node->tablename))->getProjectoin(0));
			}
			else
			{
				tablescan=parsetree2logicalplan(node->subquery);
			}
			Expr_list_header * whcdn=(Expr_list_header *)node->whcdn;
			if(whcdn->header!=NULL)
			{
				Filter::Condition filter_condition;
				Node * p;
				bool hasin=false;
				for(p=whcdn->header;p!=NULL;p=((Expr_list *)p)->next)
				{
					getfiltercondition((Node *)((Expr_list *)p)->data,filter_condition,node->tablename,hasin,tablescan);
				}
				LogicalOperator* filter=new Filter(filter_condition,tablescan);
				if(hasin==true)
				{
					for(p=whcdn->header;p!=NULL;p=((Expr_list *)p)->next)
					{
						filter=solve_insubquery(((Expr_list *)p)->data,filter);
					}
				}
				return filter;
			}
			else
			{
				return tablescan;
			}

		}break;
		case t_from_list://由from_list递归进入args/next,并获得在其上的equaljoin
		{
			From_list *node=(From_list *)parsetree;
			LogicalOperator * filter_1= where_from2logicalplan(node->args);
			LogicalOperator * filter_2= where_from2logicalplan(node->next);//maybe NULL
			if(filter_2==NULL)
			{
				return filter_1;
			}
			Expr_list_header * whcdn=(Expr_list_header *)node->whcdn;
			if(whcdn->header!=NULL)
			{
				vector<EqualJoin::JoinPair> join_pair_list;
				Node * p;
				for(p=whcdn->header;p!=NULL;p=((Expr_list *)p)->next)//应该根据getdataflow的信息确定joinpair跟filter1/2是否一致
				{
					getjoinpairlist((Node *)((Expr_list *)p)->data,join_pair_list,filter_1,filter_2);
				}
				LogicalOperator* join=new EqualJoin(join_pair_list,filter_1,filter_2);
				return join;
			}
			else//没有equaljoin的情况
			{
				SQLParse_log("fromlist no equaljoin");
				return NULL;
			}

		}break;
		case t_join:
		{
			Join *node=(Join *)parsetree;
			LogicalOperator * filter_1= where_from2logicalplan(node->lnext);
			LogicalOperator * filter_2= where_from2logicalplan(node->rnext);
			if(node->condition!=NULL)
			{
				vector<EqualJoin::JoinPair> join_pair_list;
				Node * p;
				for(p=node->condition;p!=NULL;p=((Expr_list *)p)->next)//应该根据getdataflow的信息确定joinpair跟filter1/2是否一致
				{
					getjoinpairlist((Node *)((Expr_list *)p)->data,join_pair_list,filter_1,filter_2);
				}
				LogicalOperator* join=new EqualJoin(join_pair_list,filter_1,filter_2);
				return join;
			}
			else//没有equaljoin的情况
			{
				SQLParse_log("fromlist no equaljoin");
				return NULL;
			}

		}break;
		default :
		{
			SQLParse_elog("parsetree2logicalplan type error");
			return NULL;
		}
	}
	return NULL;
}

static char * get_expr_str(Node *node)
{
	char *str="";
	switch(node->type)
	{
		case t_expr_cal:
		{
			Expr_cal * calnode=(Expr_cal *)node;
			str=calnode->str;
		}break;
		case t_expr_func:
		{
			Expr_func * funcnode=(Expr_func *)node;
			str=funcnode->str;
		}break;
		case t_name:
		case t_name_name:
		{
			Columns *col=(Columns *)node;
			str=col->parameter2;
		}break;
		case t_stringval:
		{
			Expr * exprval=(Expr *)node;
		}break;
		case t_intnum:
		{
			Expr * exprval=(Expr *)node;
		}break;
		case t_approxnum:
		{
			Expr * exprval=(Expr *)node;

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

static void get_aggregation_args(Node *selectlist, vector<Attribute> &aggregation_attributes,vector<BlockStreamAggregationIterator::State::aggregation> &aggregation_function,LogicalOperator * input)
{
	if(selectlist==NULL)
	{
	//	puts("get_aggregation_args is null");
		return ;
	}
	switch(selectlist->type)
	{
		case t_select_list:
		{
			Select_list *selectnode=(Select_list *)selectlist;
			get_aggregation_args(selectnode->args, aggregation_attributes,aggregation_function,input);
			get_aggregation_args(selectnode->next, aggregation_attributes,aggregation_function,input);
		}break;
		case t_select_expr:
		{
			Select_expr *selectnode=(Select_expr *)selectlist;
			get_aggregation_args(selectnode->colname, aggregation_attributes,aggregation_function,input);

		}break;
		case t_expr_func:
		{
			Expr_func * funcnode=(Expr_func *)selectlist;
			if(strcmp(funcnode->funname,"FCOUNTALL")==0)
			{
				aggregation_function.push_back(BlockStreamAggregationIterator::State::count);
				aggregation_attributes.push_back(Attribute(ATTRIBUTE_ANY));
			}
			else if(strcmp(funcnode->funname,"FCOUNT")==0)
			{
				aggregation_function.push_back(BlockStreamAggregationIterator::State::count);
				if(input==NULL)
				{
					Columns *funccol=(Columns *)funcnode->parameter1;
					aggregation_attributes.push_back(Environment::getInstance()->getCatalog()->getTable(funccol->parameter1)->getAttribute(funccol->parameter2));
				}
				else
				{
					aggregation_attributes.push_back(input->getDataflow().getAttribute(get_expr_str(funcnode->parameter1)));
				}
			}
			else if(strcmp(funcnode->funname,"FSUM")==0)
			{
				aggregation_function.push_back(BlockStreamAggregationIterator::State::sum);
				if(input==NULL)
				{
					Columns *funccol=(Columns *)funcnode->parameter1;
					aggregation_attributes.push_back(Environment::getInstance()->getCatalog()->getTable(funccol->parameter1)->getAttribute(funccol->parameter2));
				}
				else
				{
					aggregation_attributes.push_back(input->getDataflow().getAttribute(get_expr_str(funcnode->parameter1)));

				}
			}
			else if(strcmp(funcnode->funname,"FMIN")==0)
			{
				aggregation_function.push_back(BlockStreamAggregationIterator::State::min);
				if(input==NULL)
				{
					Columns *funccol=(Columns *)funcnode->parameter1;
					aggregation_attributes.push_back(Environment::getInstance()->getCatalog()->getTable(funccol->parameter1)->getAttribute(funccol->parameter2));
				}
				else
				{
					aggregation_attributes.push_back(input->getDataflow().getAttribute(get_expr_str(funcnode->parameter1)));
				}
			}
			else if(strcmp(funcnode->funname,"FMAX")==0)
			{
				aggregation_function.push_back(BlockStreamAggregationIterator::State::max);
				if(input==NULL)
				{
					Columns *funccol=(Columns *)funcnode->parameter1;
					aggregation_attributes.push_back(Environment::getInstance()->getCatalog()->getTable(funccol->parameter1)->getAttribute(funccol->parameter2));
				}
				else
				{
					aggregation_attributes.push_back(input->getDataflow().getAttribute(get_expr_str(funcnode->parameter1)));
				}
			}
			else if(strcmp(funcnode->funname,"FAVG")==0)///////////////////////////////////底层还未实现
			{
				aggregation_function.push_back(BlockStreamAggregationIterator::State::avg);
				if(input==NULL)
				{
					Columns *funccol=(Columns *)funcnode->parameter1;
					aggregation_attributes.push_back(Environment::getInstance()->getCatalog()->getTable(funccol->parameter1)->getAttribute(funccol->parameter2));
				}
				else
				{
					aggregation_attributes.push_back(input->getDataflow().getAttribute(get_expr_str(funcnode->parameter1)));
				}
			}
			else
			{
				if(strcmp(funcnode->funname,"FSUBSTRING0")==0||strcmp(funcnode->funname,"FSUBSTRING1")==0)
				{
					get_aggregation_args(funcnode->args, aggregation_attributes,aggregation_function,input);
				}
				else
				{
					get_aggregation_args(funcnode->parameter1, aggregation_attributes,aggregation_function,input);
				}
			}

		}break;
		case t_expr_cal:
		{
			Expr_cal * calnode=(Expr_cal *)selectlist;
			get_aggregation_args(calnode->lnext, aggregation_attributes,aggregation_function,input);
			get_aggregation_args(calnode->rnext, aggregation_attributes,aggregation_function,input);

		}break;
		case t_name:
		case t_name_name:
		{
//			Columns *colnode=(Columns *)selectlist;
//			if(input==NULL)
//			{
//				aggregation_attributes.push_back(Environment::getInstance()->getCatalog()->getTable(colnode->parameter1)->getAttribute(colnode->parameter2));
//			}
//			else
//			{
//				aggregation_attributes.push_back(input->getDataflow().getAttribute(get_expr_str(selectlist)));
//
//			}
		}break;
		case t_stringval:
		{

		}break;
		case t_intnum:
		{

		}break;
		case t_approxnum:
		{

		}break;
		case t_bool:
		{

		}break;
		default:
		{

		}
	}

}

static void get_group_by_attributes(Node *groupby_node,vector<Attribute> &group_by_attributes,LogicalOperator *input)
{
	if(groupby_node==NULL)
	{
		SQLParse_log("groupby_node is null");
		return;
	}
	switch(groupby_node->type)
	{
		case t_groupby_list:
		{
			Groupby_list *node=(Groupby_list *)groupby_node;
			get_group_by_attributes(node->next,group_by_attributes,input);
		}break;
		case t_groupby_expr:
		{
			Groupby_expr *node=(Groupby_expr *)groupby_node;
			get_group_by_attributes(node->args,group_by_attributes,input);
			get_group_by_attributes(node->next,group_by_attributes,input);
		}break;
		case t_name:
		case t_name_name:
		case t_column:
		{
			group_by_attributes.push_back(input->getDataflow().getAttribute(get_expr_str(groupby_node)));
		}break;
		case t_expr_cal:
		{
			group_by_attributes.push_back(input->getDataflow().getAttribute(get_expr_str(groupby_node)));
		}break;
		case t_expr_func:
		{
			group_by_attributes.push_back(input->getDataflow().getAttribute(get_expr_str(groupby_node)));
		}break;
		default:
		{
			SQLParse_elog("get_group_by_attributes case is null");
		}
	}

}

static void get_a_selectlist_expression_item(vector<ExpressionItem>&expr,Node *node,LogicalOperator *input)
{
	switch(node->type)
	{
		case t_expr_func:
		{
			Expr_func * funcnode=(Expr_func *)node;
			if(strcmp(funcnode->funname,"CASE3")==0)
			{
				ExpressionItem expritem0;
				expritem0.setOperator("case");
				expr.push_back(expritem0);
				get_a_selectlist_expression_item(expr,funcnode->parameter1,input);
				ExpressionItem expritem1;
				expritem1.setOperator("case");
				expr.push_back(expritem1);
			}
			else if(strcmp(funcnode->funname,"CASE4")==0)//目前只支持case [when expr then expr]* [else expr] end
			{
				ExpressionItem expritem0;
				expritem0.setOperator("case");
				expr.push_back(expritem0);
				get_a_selectlist_expression_item(expr,funcnode->parameter1,input);
				get_a_selectlist_expression_item(expr,funcnode->parameter2,input);
				ExpressionItem expritem1;
				expritem1.setOperator("else");
				expr.push_back(expritem1);
				ExpressionItem expritem2;
				expritem2.setOperator("case");
				expr.push_back(expritem2);

			}
			else if(strcmp(funcnode->funname,"WHEN1")==0)
			{
				get_a_selectlist_expression_item(expr,funcnode->parameter1,input);
				ExpressionItem expritem0;
				expritem0.setOperator("when");
				expr.push_back(expritem0);
				get_a_selectlist_expression_item(expr,funcnode->parameter2,input);
				ExpressionItem expritem1;
				expritem1.setOperator("then");
				expr.push_back(expritem1);

			}
			else if(strcmp(funcnode->funname,"WHEN2")==0)
			{
				get_a_selectlist_expression_item(expr,funcnode->parameter1,input);
				ExpressionItem expritem0;
				expritem0.setOperator("when");
				expr.push_back(expritem0);

				get_a_selectlist_expression_item(expr,funcnode->parameter2,input);
				ExpressionItem expritem1;
				expritem1.setOperator("then");
				expr.push_back(expritem1);
			}
			else if(strcmp(funcnode->funname,"FSUBSTRING0")==0)
			{
				get_a_selectlist_expression_item(expr,funcnode->args,input);
				ExpressionItem expritem0;
				expritem0.setIntValue(0);
				expr.push_back(expritem0);
				get_a_selectlist_expression_item(expr,funcnode->parameter1,input);
				ExpressionItem expritem1;
				expritem1.setOperator("substr");
				expr.push_back(expritem1);
			}
			else if(strcmp(funcnode->funname,"FSUBSTRING1")==0)
			{
				get_a_selectlist_expression_item(expr,funcnode->args,input);
				get_a_selectlist_expression_item(expr,funcnode->parameter1,input);
				get_a_selectlist_expression_item(expr,funcnode->parameter2,input);
				ExpressionItem expritem1;
				expritem1.setOperator("substr");
				expr.push_back(expritem1);
			}
			else if(strcmp(funcnode->funname,"FTRIM0")==0)
			{
				ExpressionItem expritem0;
				expritem0.setIntValue(0);
				expr.push_back(expritem0);
				get_a_selectlist_expression_item(expr,funcnode->parameter1,input);
				get_a_selectlist_expression_item(expr,funcnode->parameter2,input);
				ExpressionItem expritem1;
				expritem1.setOperator("trim");
				expr.push_back(expritem1);
			}
			else if(strcmp(funcnode->funname,"FTRIM1")==0)
			{
				ExpressionItem expritem0;
				expritem0.setIntValue(1);
				expr.push_back(expritem0);
				get_a_selectlist_expression_item(expr,funcnode->parameter1,input);
				get_a_selectlist_expression_item(expr,funcnode->parameter2,input);
				ExpressionItem expritem1;
				expritem1.setOperator("trim");
				expr.push_back(expritem1);
			}
			else if(strcmp(funcnode->funname,"FTRIM2")==0)
			{
				ExpressionItem expritem0;
				expritem0.setIntValue(2);
				expr.push_back(expritem0);
				get_a_selectlist_expression_item(expr,funcnode->parameter1,input);
				get_a_selectlist_expression_item(expr,funcnode->parameter2,input);
				ExpressionItem expritem1;
				expritem1.setOperator("trim");
				expr.push_back(expritem1);
			}
			else if(strcmp(funcnode->funname,"FTRIM3")==0)
			{
				ExpressionItem expritem0;
				expritem0.setIntValue(0);
				expr.push_back(expritem0);
				ExpressionItem expritem;
				expritem.setStringValue(" ");
				expr.push_back(expritem);
				get_a_selectlist_expression_item(expr,funcnode->parameter1,input);
				ExpressionItem expritem1;
				expritem1.setOperator("trim");
				expr.push_back(expritem1);
			}
			else if(strcmp(funcnode->funname,"FUPPER")==0)
			{
				get_a_selectlist_expression_item(expr,funcnode->parameter1,input);
				ExpressionItem expritem1;
				expritem1.setOperator("upper");
				expr.push_back(expritem1);
			}
			else if(strcmp(funcnode->funname,"FCAST")==0)
			{
				get_a_selectlist_expression_item(expr,funcnode->parameter1,input);
				get_a_selectlist_expression_item(expr,funcnode->parameter2,input);
				ExpressionItem expritem1;
				expritem1.setOperator("cast");
				expr.push_back(expritem1);
			}
			else if(strcmp(funcnode->funname,"FCOALESCE")==0)
			{
				//get_a_selectlist_expression_item(expr,funcnode->parameter1,input,sid);
			}
			else if(strcmp(funcnode->funname,"FCOUNT")==0)
			{
				if(input==NULL)
				{

				}
				else
				{
					ExpressionItem expritem;
					expritem.setVariable("count(*)");
					expr.push_back(expritem);
				}
			}
			else if(strcmp(funcnode->funname,"FCOUNT")==0)
			{
				if(input==NULL)
				{
					get_a_selectlist_expression_item(expr,funcnode->parameter1,input);
				}
				else
				{
					ExpressionItem expritem;
					expritem.setVariable((const char*)funcnode->str);
					expr.push_back(expritem);
				}
			}
			else if(strcmp(funcnode->funname,"FSUM")==0)
			{
				if(input==NULL)
				{
					get_a_selectlist_expression_item(expr,funcnode->parameter1,input);
				}
				else
				{
					ExpressionItem expritem;
					expritem.setVariable((const char*)funcnode->str);
					expr.push_back(expritem);
				}			}
			else if(strcmp(funcnode->funname,"FAVG")==0)
			{
				if(input==NULL)
				{
					get_a_selectlist_expression_item(expr,funcnode->parameter1,input);
				}
				else
				{
					ExpressionItem expritem;
					expritem.setVariable((const char*)funcnode->str);
					expr.push_back(expritem);
				}
			}
			else if(strcmp(funcnode->funname,"FMIN")==0)
			{
				if(input==NULL)
				{
					get_a_selectlist_expression_item(expr,funcnode->parameter1,input);
				}
				else
				{
					ExpressionItem expritem;
					expritem.setVariable((const char*)funcnode->str);
					expr.push_back(expritem);
				}
			}
			else if(strcmp(funcnode->funname,"FMAX")==0)
			{
				if(input==NULL)
				{
					get_a_selectlist_expression_item(expr,funcnode->parameter1,input);
				}
				else
				{
					ExpressionItem expritem;
					expritem.setVariable((const char*)funcnode->str);
					expr.push_back(expritem);
				}
			}
			else
			{
				SQLParse_elog("get_a_selectlist_expression_item: ",funcnode->funname ,"   is null");
			}
		}break;
		case t_expr_cal:
		{
			Expr_cal * calnode=(Expr_cal *)node;
			if(strcmp(calnode->sign,"--")==0)//处理负号的初步方法，处理-的时候，在前面加0，即=（0-expr)
			{
				ExpressionItem expritem0;
				expritem0.setIntValue(0);
				expr.push_back(expritem0);
				get_a_selectlist_expression_item(expr,calnode->rnext,input);
				ExpressionItem expritem;
				expritem.setOperator("-");
				expr.push_back(expritem);
			}
			else if(strcmp(calnode->sign,"++")==0)
			{
			}
			else
			{
				get_a_selectlist_expression_item(expr,calnode->lnext,input);
				get_a_selectlist_expression_item(expr,calnode->rnext,input);
				ExpressionItem expritem;
				if(strcmp(calnode->sign,"CMP")==0)
				{
					switch(calnode->cmp)
					{
						case 1://"<"
						{
							expritem.setOperator("<");
						}break;
						case 2://">"
						{
							expritem.setOperator(">");
						}break;
						case 3://"<>"
						{
							expritem.setOperator("<>");
						}break;
						case 4://"="
						{
							expritem.setOperator("=");
						}break;
						case 5://"<="
						{
							expritem.setOperator("<=");
						}break;
						case 6://">="
						{
							expritem.setOperator(">=");
						}break;
						default:
						{
							SQLParse_elog("get_a_selectlist_expression_item"," cmp error","");
						}
					}
				}
				else
				{
					expritem.setOperator(calnode->sign);
				}
				expr.push_back(expritem);
			}
		}break;
		case t_name:
		case t_name_name:
		{
			Columns *col=(Columns *)node;
			ExpressionItem expritem;
			expritem.setVariable(col->parameter2);
			expr.push_back(expritem);
		}break;
		case t_stringval:
		{
			Expr * exprval=(Expr *)node;
			ExpressionItem expritem;
			expritem.setStringValue(exprval->data);
			expr.push_back(expritem);
		}break;
		case t_intnum:
		{
			Expr * exprval=(Expr *)node;
			ExpressionItem expritem;
			expritem.setIntValue(exprval->data);
			expr.push_back(expritem);

		}break;
		case t_approxnum:
		{
			Expr * exprval=(Expr *)node;
			ExpressionItem expritem;
			expritem.setDoubleValue(exprval->data);
			expr.push_back(expritem);

		}break;
		case t_bool:
		{

		}break;
		default:
		{

		}
	}

}
/*
 * 只需要获得agg函数参数的expr并加入到allexpr
 */

static void recurse_get_item_in_expr(Node *node,vector<vector<ExpressionItem> >&allexpr)
{
	switch(node->type)
	{
		case t_expr_func:
		{
			Expr_func * funcnode=(Expr_func *)node;
			if(strcmp(funcnode->funname,"FCOUNT")==0||strcmp(funcnode->funname,"FSUM")==0||strcmp(funcnode->funname,"FAVG")==0||strcmp(funcnode->funname,"FMIN")==0||strcmp(funcnode->funname,"FMAX")==0)
			{
				vector<ExpressionItem>expr;
				get_a_selectlist_expression_item(expr,funcnode->parameter1,NULL);
				allexpr.push_back(expr);
				expr.clear();
			}
			else if(strcmp(funcnode->funname,"FCOUNTALL")==0)
			{

			}
			else
			{
				if(strcmp(funcnode->funname,"FSUBSTRING0")==0||strcmp(funcnode->funname,"FSUBSTRING1")==0)
				{
					recurse_get_item_in_expr(funcnode->args,allexpr);
				}
				else
				{
					recurse_get_item_in_expr(funcnode->parameter1,allexpr);
				}
			}
		}break;
		case t_expr_cal:
		{
			Expr_cal * calnode=(Expr_cal *)node;
			recurse_get_item_in_expr(calnode->lnext,allexpr);
			recurse_get_item_in_expr(calnode->rnext,allexpr);

		}break;
		case t_name:
		case t_name_name:
		{

		}break;
		case t_stringval:
		{

		}break;
		case t_intnum:
		{

		}break;
		case t_approxnum:
		{

		}break;
		case t_bool:
		{

		}break;
		default:
		{

		}
	}
}
static void get_all_selectlist_expression_item(Node * node,vector<vector<ExpressionItem> >&allexpr,LogicalOperator *input,int proj_type)
{
	vector<ExpressionItem>expr;
	for(Node *p=node;p!=NULL;)
	{
		Select_list *selectlist=(Select_list *)p;
		Select_expr *sexpr=(Select_expr *)selectlist->args;
		if(proj_type==0)
		{
			get_a_selectlist_expression_item(expr,sexpr->colname,NULL);
			allexpr.push_back(expr);
			expr.clear();
		}
		else if(proj_type==1)
		{
			if(selectlist->isall==-1)
			{
				recurse_get_item_in_expr(sexpr->colname,allexpr);
			}
			else if(selectlist->isall==-2)//count(*) 不能参与运算
			{
				SQLParse_log("this sql has count(*)");
			}
		}
		else if(proj_type==2)
		{
			if(selectlist->isall==-1)
			{
				get_a_selectlist_expression_item(expr,sexpr->colname,input);
				allexpr.push_back(expr);
				expr.clear();
			}
			else
			{
				ExpressionItem expritem;
				expritem.setVariable(get_expr_str(sexpr->colname));
 				expr.push_back(expritem);
 				allexpr.push_back(expr);
				expr.clear();
			}
		}
		p=selectlist->next;
	}
}
static void get_all_groupby_expression_item(Node * node,vector<vector<ExpressionItem> >&allexpr,LogicalOperator *input)
{
	vector<ExpressionItem>expr;
	for(Node *p=(Node *)(((Groupby_list*)node)->next);p!=NULL;)
	{
		Groupby_expr *gbexpr=(Groupby_expr *)p;
		get_a_selectlist_expression_item(expr,gbexpr->args,NULL);
		allexpr.push_back(expr);
		expr.clear();
		p=gbexpr->next;
	}

}
static LogicalOperator* select_where_from2logicalplan(Node *parsetree,LogicalOperator * last_logicalplan,int proj_type)
{
	if(parsetree==NULL)
	{
		return NULL;
	}
	else
	{
		Query_stmt *node=(Query_stmt *)parsetree;
		vector<vector<ExpressionItem> >allexpr;
		allexpr.clear();
		if(proj_type==0)
		{
			get_all_selectlist_expression_item(node->select_list,allexpr,NULL,0);
		}
		else if(proj_type==1)
		{
			if(node->groupby_list!=NULL)
			{
				get_all_groupby_expression_item(node->groupby_list,allexpr,NULL);
			}
			get_all_selectlist_expression_item(node->select_list,allexpr,NULL,1);
		}
		else
		{
			get_all_selectlist_expression_item(node->select_list,allexpr,last_logicalplan,2);
		}

//		cout<<"allexpr.size= "<<allexpr.size()<<endl;
//		for(unsigned i=0;i<allexpr.size();i++){
//			cout<<"allexpr "<<i<<"  size ="<<allexpr[i].size()<<endl;
//			for(unsigned j=0;j<allexpr[i].size();j++){
//				cout<<"******"<<endl;
//				allexpr[i][j].print_value();
//			}
//		}
		LogicalOperator* proj=NULL;
		if(allexpr.size()>0)
		{
			proj=new LogicalProject(last_logicalplan,allexpr);
		}
		else
		{
			SQLParse_log("allexpr.size=0");
			proj=last_logicalplan;
		}
		return proj;
	}
}
static void dfs_select_args(int flag,int &ans,Node * node)
{
	switch(node->type)
	{
		case t_expr_func:
		{
			Expr_func * funcnode=(Expr_func *)node;
			if(strcmp(funcnode->funname,"FCOUNT")==0||strcmp(funcnode->funname,"FSUM")==0||strcmp(funcnode->funname,"FAVG")==0||strcmp(funcnode->funname,"FMIN")==0||strcmp(funcnode->funname,"FMAX")==0)
			{
				if(flag==-2)
				{
					SQLParse_elog("dfs_select_args error!!!!");
				}
				if(flag==-1&&(ans&4)==0)
				{
					ans+=4;//expr has agg
				}
				if((ans&1)==0)
				{
					ans+=1;
				}
				dfs_select_args(-2,ans,funcnode->parameter1);
			}
			else if(strcmp(funcnode->funname,"FCOUNTALL")==0)
			{
				if(flag==-2)
				{
					SQLParse_elog("dfs_select_args error!!!!");
				}
				if(flag==-1&&(ans&4)==0)
				{
					ans+=4;//expr has agg
				}
				if((ans&8)==0)
				{
					ans+=8;
				}
			}
			else if(strcmp(funcnode->funname,"CASE3")==0)
			{
				if(flag==-2&&(ans&2==0))
				{
					ans+=2;//agg has expr
				}
				dfs_select_args(-1,ans,funcnode->parameter1);
			}
			else if(strcmp(funcnode->funname,"CASE4")==0)//目前只支持case when [expr then expr]* [else expr] end
			{
				if(flag==-2&&(ans&2==0))
				{
					ans+=2;//agg has expr
				}
				dfs_select_args(-1,ans,funcnode->parameter1);
			}
			else if(strcmp(funcnode->funname,"WHEN1")==0)
			{
				if(flag==-2&&(ans&2==0))
				{
					ans+=2;//agg has expr
				}
				dfs_select_args(-1,ans,funcnode->parameter1);
			}
			else if(strcmp(funcnode->funname,"WHEN2")==0)
			{
				if(flag==-2&&(ans&2==0))
				{
					ans+=2;//agg has expr
				}
				dfs_select_args(-1,ans,funcnode->parameter1);
			}
			else if(strcmp(funcnode->funname,"FSUBSTRING0")==0)
			{
				if(flag==-2&&(ans&2==0))
				{
					ans+=2;//agg has expr
				}
				dfs_select_args(-1,ans,funcnode->args);
			}
			else if(strcmp(funcnode->funname,"FSUBSTRING1")==0)
			{
				if(flag==-2&&(ans&2==0))
				{
					ans+=2;//agg has expr
				}
				dfs_select_args(-1,ans,funcnode->args);
			}
			else if(strcmp(funcnode->funname,"FTRIM0")==0)
			{
				if(flag==-2&&(ans&2==0))
				{
					ans+=2;//agg has expr
				}
				dfs_select_args(-1,ans,funcnode->parameter1);
			}
			else if(strcmp(funcnode->funname,"FTRIM1")==0)
			{
				if(flag==-2&&(ans&2==0))
				{
					ans+=2;//agg has expr
				}
				dfs_select_args(-1,ans,funcnode->parameter1);
			}
			else if(strcmp(funcnode->funname,"FTRIM2")==0)
			{
				if(flag==-2&&(ans&2==0))
				{
					ans+=2;//agg has expr
				}
				dfs_select_args(-1,ans,funcnode->parameter1);
			}
			else if(strcmp(funcnode->funname,"FTRIM3")==0)
			{
				if(flag==-2&&(ans&2==0))
				{
					ans+=2;//agg has expr
				}
				dfs_select_args(-1,ans,funcnode->parameter1);
			}
			else if(strcmp(funcnode->funname,"FUPPER")==0)
			{
				if(flag==-2&&(ans&2==0))
				{
					ans+=2;//agg has expr
				}
				dfs_select_args(-1,ans,funcnode->parameter1);
			}
			else if(strcmp(funcnode->funname,"FCAST")==0)
			{
				if(flag==-2&&(ans&2==0))
				{
					ans+=2;//agg has expr
				}
				dfs_select_args(-1,ans,funcnode->parameter1);
			}
			else if(strcmp(funcnode->funname,"FCOALESCE")==0)
			{
				if(flag==-2&&(ans&2==0))
				{
					ans+=2;//agg has expr
				}
				dfs_select_args(-1,ans,funcnode->parameter1);
			}
			else
			{

			}
		}break;
		case t_expr_cal:
		{
			Expr_cal * calnode=(Expr_cal *)node;
			if(flag==-2&&(ans&2==0))
			{
				ans+=2;//agg has expr
			}
			dfs_select_args(-1,ans,calnode->lnext);
			dfs_select_args(-1,ans,calnode->rnext);

		}break;
		case t_name:
		case t_name_name:
		{

		}break;
		case t_stringval:
		{


		}break;
		case t_intnum:
		{


		}break;
		case t_approxnum:
		{

		}break;
		case t_bool:
		{

		}break;
		default:
		{


		}
	}
	return;
}
/*判断selectlist的expression中是否存在
 * 聚集函数agg中的参数为表达式expr eg: min(a+b)
 * 或者聚集函数agg包含在表达式中 eg:min(a)+max(b)
 */
static void judge_selectlist_agg_has_or_in_expr(Node *node,int &has_agg,int &agg_has_expr,int &agg_in_expr)
{
	int ans=0;
	for(Node *p=node;p!=NULL;)
	{
		Select_list *selectlist=(Select_list *)p;
		Select_expr *sexpr=(Select_expr *)selectlist->args;
		ans=0;
		dfs_select_args(0,ans,sexpr->colname);
//		if((ans&1)==1) has_agg=1;
//		if((ans&2)==1) agg_has_expr=1;
//		if((ans&4)==1)
//		{
//			agg_in_expr=1;
//			selectlist->isall=-1;
//		}
//		SQLParse_log("judge_selectlist_agg_has_or_in_expr ans=%d",ans);
		if((ans&1)==1)
		{
			has_agg=1;
			selectlist->isall=-1;
		}
//		SQLParse_log("ans&8= %d ",ans&8);
		if((ans&8)==8)
		{
			has_agg=1;
			selectlist->isall=-2;
		}
		p=selectlist->next;
	}
}
/*
 *a||g==0 selectlist中所有表达式放入，记为proj0
 *a||g==1 groupbylist+agg函数中的参数，记为proj1
 *对于proj2 含有agg的参数用整体表示，并完成该表达式的计算；如果没有，则直接用整体
 *eg:max(a+b),c+d => a+b,c+d
 *
 *目前的groupby中可以含有表达式，但是不支持该表达式在selectexpr中参与运算
 *
 */
static LogicalOperator* groupby_select_where_from2logicalplan(Node *parsetree)//proj,agg处理合并在一起,形成p-a-p模型
{
	Query_stmt *node=(Query_stmt *)parsetree;
	LogicalOperator * where_from_logicalplan=where_from2logicalplan(node->from_list);
	vector<Attribute> group_by_attributes;
	vector<Attribute> aggregation_attributes;
	vector<BlockStreamAggregationIterator::State::aggregation> aggregation_function;
	int agg_has_expr,agg_in_expr,has_agg,sid=0;
	agg_has_expr=agg_in_expr=has_agg=0;
	judge_selectlist_agg_has_or_in_expr(node->select_list,has_agg,agg_has_expr,agg_in_expr);
	LogicalOperator *select_logicalplan=NULL;
//	SQLParse_log("has_agg= %d,agg_has_expr=%d,agg_in_expr=%d",has_agg,agg_has_expr,agg_in_expr);
	if(has_agg==0&&node->groupby_list==NULL)
	{
		select_logicalplan= select_where_from2logicalplan(parsetree,where_from_logicalplan,0);
	}
	else
	{
		select_logicalplan= select_where_from2logicalplan(parsetree,where_from_logicalplan,1);
		if(node->groupby_list!=NULL)//获取group_by_attributes
		{
			get_group_by_attributes(node->groupby_list,group_by_attributes,select_logicalplan);
//#ifdef SQL_Parser
//			cout<<"groupby size= "<<group_by_attributes.size()<<endl;
//			for(int i=0;i<group_by_attributes.size();i++)
//			{
//				cout<<"groupby_att  "<<group_by_attributes[i].attrName<<endl;
//			}
//#endif
		}
		if(has_agg==1)
		{
			get_aggregation_args(node->select_list, aggregation_attributes,aggregation_function,select_logicalplan);
//#ifdef SQL_Parser
//			cout<<"agg_att size= "<<aggregation_attributes.size()<<endl;
//			for(int i=0;i<aggregation_attributes.size();i++)
//			{
//				cout<<"agg_att  "<<aggregation_attributes[i].attrName<<endl;
//			}
//#endif
		}
		LogicalOperator* projection_or_aggregation=new Aggregation(group_by_attributes,aggregation_attributes,aggregation_function,select_logicalplan);
		select_logicalplan= select_where_from2logicalplan(parsetree,projection_or_aggregation,2);
	}
	return select_logicalplan;
}
static void get_orderby_column_from_selectlist(Node * olnode,Node *slnode,vector<LogicalSort::OrderByAttr *>&obcol)
{
	Orderby_list * gblist=(Orderby_list *)(olnode);
	for(Node *p=(Node *)(gblist->next);p!=NULL;)
	{
		Groupby_expr *gbexpr=(Groupby_expr *)p;
		switch(gbexpr->args->type)
		{
			case t_name:
			case t_column:
			case t_name_name:
			{
				Columns *col=(Columns *)(gbexpr->args);
//				obcol.push_back(col->parameter2);
				//obcol.push_back(col->parameter2);
				obcol.push_back(new LogicalSort::OrderByAttr(col->parameter2));
			}break;
			case t_intnum:
			{
				Expr * exprval=(Expr *)(gbexpr->args);
				int num=0;
				Node * q=slnode;
				for(;q!=NULL;)
				{
					Select_list *selectlist=(Select_list *)q;
					if(num==atof(exprval->data))
						break;
					num++;
					q=selectlist->next;
				}
				if(q==NULL)
				{
					//string str=(exprval->data.int_val);
					SQLParse_elog("get_orderby_column_from_selectlist order by attribute num ","","is beyond");
				}
				else
				{
					Select_list *selectlist=(Select_list *)q;
					Select_expr *sexpr=(Select_expr *)selectlist->args;
					switch(sexpr->colname->type)
					{
						case t_name:
						case t_name_name:
						case t_column:
						{
							Columns * col=(Columns *)sexpr->colname;
							obcol.push_back(new LogicalSort::OrderByAttr(col->parameter2));
						}break;
						default:
						{
							SQLParse_elog("get_orderby_column_from_selectlist","gbexpr->colname->type ","not exist");
						}
					}
				}
			}break;
			default:
			{
				SQLParse_elog("get_orderby_column_from_selectlist","gbexpr->args->type ","not exist");
			}
		}
		p=gbexpr->next;
	}
}
static LogicalOperator* orderby_select_groupby_where_from2logicalplan(Node *&parsetree)
{
	LogicalOperator* orderby_logicalplan=NULL;
	LogicalOperator* select_logicalplan= groupby_select_where_from2logicalplan(parsetree);
	Query_stmt *node=(Query_stmt *)parsetree;
	if(node->orderby_list==NULL)
	{
		orderby_logicalplan= select_logicalplan;
	}
	else
	{
//		vector<string>obstr;
		vector<LogicalSort::OrderByAttr*>obcol;
		get_orderby_column_from_selectlist(node->orderby_list,node->select_list,obcol);
//		for(int i=obstr.size()-1;i>=0;i--)
//		{
//			obcol.push_back(new LogicalSort::OrderByAttr(obstr[i].c_str()));
//		}
	//	obcol.push_back(&LogicalSort::OrderByAttr("LINEITEM.L_LINESTATUS"));
		reverse(obcol.begin(),obcol.end());
//#ifdef SQL_Parser
//		cout<<"orderby size=  "<<obcol.size()<<endl;
//		for(int i=0;i<obcol.size();i++)
//		{
//			cout<<"orderby att= "<<obcol[i]->ta_ <<endl;
//		}
//#endif
		 orderby_logicalplan=new LogicalSort(select_logicalplan,obcol);
	}
		return orderby_logicalplan;
}
static LogicalOperator* parsetree2logicalplan(Node *parsetree)//实现parsetree 到logicalplan的转换，
{
	LogicalOperator* alllogicalplan=NULL;
	switch(parsetree->type)
	{
		case t_query_stmt:
		{
			alllogicalplan=orderby_select_groupby_where_from2logicalplan(parsetree);
		}break;
		default:
		{
			SQLParse_log("parsetree type is null");
			alllogicalplan= NULL;
		}
	}
	SQLParse_log("parsetree2Logicalplan is completed!!!!!!!!!!!!!!!!!!!!!");
	return alllogicalplan;
}


#endif

