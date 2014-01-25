
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

#include"sql_node_struct.h"
#include "../Environment.h"
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
static LogicalOperator* parsetree2logicalplan(Node *parsetree)//实现parsetree 到logicalplan的转换
{
	if(parsetree==NULL)
	{
		puts("parsetree2logicalpaln error");
		return NULL;
	}
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
			LogicalOperator * filter_1= parsetree2logicalplan(node->args);
			LogicalOperator * filter_2= parsetree2logicalplan(node->next);//maybe NULL
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
#endif

