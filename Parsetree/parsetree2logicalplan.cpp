
#ifndef __PARSERTREE__
#define __PARSERTREE__
#include <cstdio>

#include <map>
#include <new>
#include <set>
#include <string>
#include <vector>

#include "../Catalog/Attribute.h"
#include "../Catalog/Catalog.h"
#include "../Catalog/table.h"
#include "../Comparator.h"
#include "../iterator/FilterIterator.h"
#include "../LogicalQueryPlan/EqualJoin.h"
#include "../LogicalQueryPlan/Filter.h"
#include "../LogicalQueryPlan/LogicalOperator.h"
#include "../LogicalQueryPlan/Scan.h"
#include "../BlockStreamIterator/ParallelBlockStreamIterator/BlockStreamAggregationIterator.h"
#include"sql_node_struct.h"
#include "../Environment.h"
#include "../LogicalQueryPlan/Aggregation.h"
static void getfiltercondition(Node * wcexpr,Filter::Condition &filter_condition,char * tablename)
{
	printf("getfiltercondition   ");
	//filter_condition.add(catalog->getTable(node->tablename)->getAttribute(4),FilterIterator::AttributeComparator::EQ,&order_type_);
	cout<<"wcexpr->type  "<<wcexpr->type<<endl;
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
			else if(strcmp(node->sign,"--")==0)
			{

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
				switch(node->cmp)
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
						char * attribute;
						switch((node->lnext)->type)//获得左边的表名
						{
							case t_name:
							{
								Expr *expr=(Expr *)(node->lnext);
								attribute=expr->data.string_val;
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
						Expr *expr=(Expr *)(node->rnext);
						switch(expr->type)//获得右边的属性
						{
							case t_intnum:
							{
								int temp=expr->data.int_val;
								cout<<"attribute:  "<<attribute<<"  temp   "<<temp<<endl;
								std::ostringstream str;
								str<<temp;
								cout<<str.str()<<endl;
								filter_condition.add(Environment::getInstance()->getCatalog()->getTable(tablename)->getAttribute(attribute),FilterIterator::AttributeComparator::EQ,str.str());
							}break;
							case t_approxnum:
							{
								double temp=expr->data.double_val;
								filter_condition.add(Environment::getInstance()->getCatalog()->getTable(tablename)->getAttribute(attribute),FilterIterator::AttributeComparator::EQ,&temp);
							}break;
							case t_name:
							case t_stringval ://////////////////////
							{
								char * temp=expr->data.string_val;
								cout<<"attribute:  "<<attribute<<"  temp    "<<temp<<endl;
								filter_condition.add(Environment::getInstance()->getCatalog()->getTable(tablename)->getAttribute(attribute),FilterIterator::AttributeComparator::EQ,temp);

							}break;
							default:
							{

							}
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
						puts("getfiltercondition type sign cmp error");
					}
				}
			}
		}break;
		default:
		{
			puts("getfiltercondition type error");
		}
	}
}
static void getjoinpairlist(Node *wcexpr,vector<EqualJoin::JoinPair> &join_pair_list)
{
	printf("getjoinpairlist   ");
//		join_pair_list.push_back(EqualJoin::JoinPair(table_1->getAttribute("order_no"),table_2->getAttribute("order_no")));
	switch(wcexpr->type)
	{
		case t_expr_cal://目前只支持equaljoin
		{
			Expr_cal *node=(Expr_cal *)wcexpr;
			Columns *lnode=(Columns *)(node->lnext);
			Columns *rnode=(Columns *)(node->rnext);
			printf("left %s.%s   right %s.%s\n",lnode->parameter1,lnode->parameter2,rnode->parameter1,rnode->parameter2);
			join_pair_list.push_back(EqualJoin::JoinPair(Environment::getInstance()->getCatalog()->getTable(lnode->parameter1)->getAttribute(lnode->parameter2),Environment::getInstance()->getCatalog()->getTable(rnode->parameter1)->getAttribute(rnode->parameter2)));
		}break;

	}
}
static LogicalOperator* where_from2logicalplan(Node *parsetree)//实现where_from_parsetree(即将where转换到from_list后的)到logicalplan的转换
{
	if(parsetree==NULL)
	{
		puts("parsetree2logicalpaln error");
		return NULL;
	}

	//对于t_query_stmt中才可以直接进行下面的操作，所以以后要再套一层判断
	switch(parsetree->type)
	{
		case t_table://table节点获得scan 和在该节点上condition的filter
		{
			Table * node=(Table *)parsetree;
			LogicalOperator* tablescan=new LogicalScan(Environment::getInstance()->getCatalog()->getTable(std::string(node->tablename))->getProjectoin(0));

			Expr_list_header * whcdn=(Expr_list_header *)node->whcdn;
			if(whcdn->header!=NULL)
			{
				Filter::Condition filter_condition;
				Node * p;
				for(p=whcdn->header;p!=NULL;p=((Expr_list *)p)->next)
				{
					getfiltercondition((Node *)((Expr_list *)p)->data,filter_condition,node->tablename);
				}
				LogicalOperator* filter=new Filter(filter_condition,tablescan);
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
				for(p=whcdn->header;p!=NULL;p=((Expr_list *)p)->next)
				{
					getjoinpairlist((Node *)((Expr_list *)p)->data,join_pair_list);
				}
				vector<EqualJoin::JoinPair>::iterator it;
				for(it=join_pair_list.begin();it!=join_pair_list.end();it++)
				{
					cout<<it->first.attrName<<"  <<  "<<it->second.attrName<<endl;
				}
				LogicalOperator* join=new EqualJoin(join_pair_list,filter_1,filter_2);
				return join;
			}
			else//没有equaljoin的情况
			{
				puts("parsetree2logicalplan  type fromlist error");
				return NULL;
			}

		}break;
		default :
		{
			puts("parsetree2logicalplan type error");
			return NULL;
		}
	}

}
static void get_aggregation_args(Node *selectlist, vector<Attribute> &aggregation_attributes,vector<BlockStreamAggregationIterator::State::aggregation> &aggregation_function)
{
	if(selectlist==NULL)
	{
		puts("get_aggregation_args is null");
		return ;
	}
	switch(selectlist->type)
	{
		case t_select_list:
		{
			Select_list *selectnode=(Select_list *)selectlist;
			get_aggregation_args(selectnode->args, aggregation_attributes,aggregation_function);
			get_aggregation_args(selectnode->next, aggregation_attributes,aggregation_function);
		}break;
		case t_select_expr:
		{
			Select_expr *selectnode=(Select_expr *)selectlist;
			get_aggregation_args(selectnode->colname, aggregation_attributes,aggregation_function);

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
				Columns *funccol=(Columns *)funcnode->parameter1;
				cout<<"Fcount  "<<funccol->parameter1<<" "<<funccol->parameter2<<endl;
				aggregation_attributes.push_back(Environment::getInstance()->getCatalog()->getTable(funccol->parameter1)->getAttribute(funccol->parameter2));
			}/////////////////////----2.19日
			else if(strcmp(funcnode->funname,"FSUM")==0)
			{
				aggregation_function.push_back(BlockStreamAggregationIterator::State::sum);
				Columns *funccol=(Columns *)funcnode->parameter1;
				cout<<"Fsum  "<<funccol->parameter1<<" "<<funccol->parameter2<<endl;
				aggregation_attributes.push_back(Environment::getInstance()->getCatalog()->getTable(funccol->parameter1)->getAttribute(funccol->parameter2));
			}
			else if(strcmp(funcnode->funname,"FMIN")==0)
			{
				aggregation_function.push_back(BlockStreamAggregationIterator::State::min);
				Columns *funccol=(Columns *)funcnode->parameter1;
				cout<<"Fmin  "<<funccol->parameter1<<" "<<funccol->parameter2<<endl;
				aggregation_attributes.push_back(Environment::getInstance()->getCatalog()->getTable(funccol->parameter1)->getAttribute(funccol->parameter2));
			}
			else if(strcmp(funcnode->funname,"FMAX")==0)
			{
				aggregation_function.push_back(BlockStreamAggregationIterator::State::max);
				Columns *funccol=(Columns *)funcnode->parameter1;
				cout<<"Fmax  "<<funccol->parameter1<<" "<<funccol->parameter2<<endl;
				aggregation_attributes.push_back(Environment::getInstance()->getCatalog()->getTable(funccol->parameter1)->getAttribute(funccol->parameter2));
			}
			else if(strcmp(funcnode->funname,"FAVG")==0)///////////////////////////////////底层还未实现
			{
				aggregation_function.push_back(BlockStreamAggregationIterator::State::count);
				Columns *funccol=(Columns *)funcnode->parameter1;
				cout<<"Fcount  "<<funccol->parameter1<<" "<<funccol->parameter2<<endl;
				aggregation_attributes.push_back(Environment::getInstance()->getCatalog()->getTable(funccol->parameter1)->getAttribute(funccol->parameter2));
			}/////////////////////----2.19日
			else
			{
				puts("aggrection func is null");
			}

		}break;
		case t_column:
		{

		}break;
		default:
		{
			puts("selectlist->type is null");
		}
	}

}
static void get_group_by_attributes(Node *groupby_node,vector<Attribute> &group_by_attributes)
{
	if(groupby_node==NULL)
	{
		puts("groupby_node is null");
		return;
	}
	switch(groupby_node->type)
	{
		case t_groupby_list:
		{
			Groupby_list *node=(Groupby_list *)groupby_node;
			get_group_by_attributes(node->next,group_by_attributes);
		}break;
		case t_groupby_expr:
		{
			Groupby_expr *node=(Groupby_expr *)groupby_node;
			get_group_by_attributes(node->args,group_by_attributes);
			get_group_by_attributes(node->next,group_by_attributes);
		}break;
		case t_name_name:
		case t_column:
		{
			Columns *node=(Columns *)groupby_node;
			cout<<"group by "<<node->parameter1<<"  "<<node->parameter2<<endl;
			group_by_attributes.push_back(Environment::getInstance()->getCatalog()->getTable(node->parameter1)->getAttribute(node->parameter2));
		}break;
		default:
		{
			puts("get_group_by_attributes case is null");
		}
	}

}
static LogicalOperator* groupby_where_from2logicalplan(Node *parsetree)//实现groupby_where_from_parsetree 到logicalplan的转换
{
	Query_stmt *node=(Query_stmt *)parsetree;
	LogicalOperator * where_from_logicalplan=where_from2logicalplan(node->from_list);
	if(node->groupby_list==NULL)
	{
		return where_from_logicalplan;
	}
	else//获得select子句中的聚集函数及其参数以及groupby子句参数
	{
		vector<Attribute> group_by_attributes;
		vector<Attribute> aggregation_attributes;
		vector<BlockStreamAggregationIterator::State::aggregation> aggregation_function;
		if(node->select_list!=NULL)
		{
			get_aggregation_args(node->select_list, aggregation_attributes,aggregation_function);
		}
		get_group_by_attributes(node->groupby_list,group_by_attributes);
		LogicalOperator* aggregation=new Aggregation(group_by_attributes,aggregation_attributes,aggregation_function,where_from_logicalplan);
		return aggregation;
	}


}

static LogicalOperator* parsetree2logicalplan(Node *parsetree)//实现parsetree 到logicalplan的转换，
{
	return groupby_where_from2logicalplan(parsetree);
}

#endif

