
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
#include "../common/ExpressionItem.h"
#include "../LogicalQueryPlan/Project.h"
#include "../LogicalQueryPlan/Sort.h"
static LogicalOperator* parsetree2logicalplan(Node *parsetree);
static void p2l_print_error(char *str1,char *str2,char *str3)
{
	printf("parsetree2logicalplan >> %s  %s   %s",str1,str2,str3);
}
static void getfiltercondition(Node * wcexpr,Filter::Condition &filter_condition,char * tablename,bool &hasin,LogicalOperator* loperator)
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

				switch(node->cmp)
				{
					case 1://"<"
					{
						Expr *expr=(Expr *)(node->rnext);
						switch(expr->type)//获得右边的属性
						{
							case t_intnum:
							{
								int temp=expr->data.int_val;
							//	cout<<"attribute:  "<<attribute<<"  temp   "<<temp<<endl;
								std::ostringstream str;
								str<<temp;
							//	cout<<str.str()<<endl;
								filter_condition.add(loperator->getDataflow().getAttribute(attribute),FilterIterator::AttributeComparator::L,str.str());
							}break;
							case t_approxnum:
							{
								double temp=expr->data.double_val;
								filter_condition.add(loperator->getDataflow().getAttribute(attribute),FilterIterator::AttributeComparator::L,&temp);
							}break;
							case t_name:
							case t_stringval ://////////////////////
							{
								char * temp=expr->data.string_val;
							//	cout<<"attribute:  "<<attribute<<"  temp    "<<temp<<endl;
								filter_condition.add(loperator->getDataflow().getAttribute(attribute),FilterIterator::AttributeComparator::L,temp);

							}break;
							default:
							{

							}
						}

					}break;
					case 2://">"
					{
						Expr *expr=(Expr *)(node->rnext);
						switch(expr->type)//获得右边的属性
						{
							case t_intnum:
							{
								int temp=expr->data.int_val;
							//	cout<<"attribute:  "<<attribute<<"  temp   "<<temp<<endl;
								std::ostringstream str;
								str<<temp;
							//	cout<<str.str()<<endl;
								filter_condition.add(loperator->getDataflow().getAttribute(attribute),FilterIterator::AttributeComparator::G,str.str());
							}break;
							case t_approxnum:
							{
								double temp=expr->data.double_val;
								filter_condition.add(loperator->getDataflow().getAttribute(attribute),FilterIterator::AttributeComparator::G,&temp);
							}break;
							case t_name:
							case t_stringval ://////////////////////
							{
								char * temp=expr->data.string_val;
							//	cout<<"attribute:  "<<attribute<<"  temp    "<<temp<<endl;
								filter_condition.add(loperator->getDataflow().getAttribute(attribute),FilterIterator::AttributeComparator::G,temp);

							}break;
							default:
							{

							}
						}

					}break;
					case 3://"<>"
					{
						Expr *expr=(Expr *)(node->rnext);
						switch(expr->type)//获得右边的属性
						{
							case t_intnum:
							{
								int temp=expr->data.int_val;
								std::ostringstream str;
								str<<temp;
								filter_condition.add(loperator->getDataflow().getAttribute(attribute),FilterIterator::AttributeComparator::NEQ,str.str());
							}break;
							case t_approxnum:
							{
								double temp=expr->data.double_val;
								filter_condition.add(loperator->getDataflow().getAttribute(attribute),FilterIterator::AttributeComparator::NEQ,&temp);
							}break;
							case t_name:
							case t_stringval ://////////////////////
							{
								char * temp=expr->data.string_val;
								filter_condition.add(loperator->getDataflow().getAttribute(attribute),FilterIterator::AttributeComparator::NEQ,temp);

							}break;
							default:
							{

							}
						}

					}break;
					case 4://"="
					{
						Expr *expr=(Expr *)(node->rnext);
						switch(expr->type)//获得右边的属性
						{
							case t_intnum:
							{
								int temp=expr->data.int_val;
						//		cout<<"arrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrttribute:  "<<attribute<<"  temp   "<<temp<<endl;
								std::ostringstream str;
								str<<temp;
						//		cout<<str.str()<<endl;
								filter_condition.add(loperator->getDataflow().getAttribute(attribute),FilterIterator::AttributeComparator::EQ,str.str());
							//	filter_condition.add(Environment::getInstance()->getCatalog()->getTable(tablename)->getAttribute(attribute),FilterIterator::AttributeComparator::EQ,str.str());
							}break;
							case t_approxnum:
							{
								double temp=expr->data.double_val;
								filter_condition.add(loperator->getDataflow().getAttribute(attribute),FilterIterator::AttributeComparator::EQ,&temp);
							}break;
							case t_name:
							case t_stringval ://////////////////////
							{
								char * temp=expr->data.string_val;
							//	cout<<"attribute:  "<<attribute<<"  temp    "<<temp<<endl;
								filter_condition.add(loperator->getDataflow().getAttribute(attribute),FilterIterator::AttributeComparator::EQ,temp);

							}break;
							default:
							{

							}
						}

					}break;
					case 5://"<="
					{
						Expr *expr=(Expr *)(node->rnext);
						switch(expr->type)//获得右边的属性
						{
							case t_intnum:
							{
								int temp=expr->data.int_val;
							//	cout<<"attribute:  "<<attribute<<"  temp   "<<temp<<endl;
								std::ostringstream str;
								str<<temp;
							//	cout<<str.str()<<endl;
								filter_condition.add(loperator->getDataflow().getAttribute(attribute),FilterIterator::AttributeComparator::LEQ,str.str());
							}break;
							case t_approxnum:
							{
								double temp=expr->data.double_val;
								filter_condition.add(loperator->getDataflow().getAttribute(attribute),FilterIterator::AttributeComparator::LEQ,&temp);
							}break;
							case t_name:
							case t_stringval ://////////////////////
							{
								char * temp=expr->data.string_val;
							//	cout<<"attribute:  "<<attribute<<"  temp    "<<temp<<endl;
								filter_condition.add(loperator->getDataflow().getAttribute(attribute),FilterIterator::AttributeComparator::LEQ,temp);

							}break;
							default:
							{

							}
						}

					}break;
					case 6://">="
					{
						Expr *expr=(Expr *)(node->rnext);
						switch(expr->type)//获得右边的属性
						{
							case t_intnum:
							{
								int temp=expr->data.int_val;
							//	cout<<"attribute:  "<<attribute<<"  temp   "<<temp<<endl;
								std::ostringstream str;
								str<<temp;
							//	cout<<str.str()<<endl;
								filter_condition.add(loperator->getDataflow().getAttribute(attribute),FilterIterator::AttributeComparator::GEQ,str.str());
							}break;
							case t_approxnum:
							{
								double temp=expr->data.double_val;
								filter_condition.add(loperator->getDataflow().getAttribute(attribute),FilterIterator::AttributeComparator::GEQ,&temp);
							}break;
							case t_name:
							case t_stringval ://////////////////////
							{
								char * temp=expr->data.string_val;
							//	cout<<"attribute:  "<<attribute<<"  temp    "<<temp<<endl;
								filter_condition.add(loperator->getDataflow().getAttribute(attribute),FilterIterator::AttributeComparator::GEQ,temp);

							}break;
							default:
							{

							}
						}

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
		puts("parsetree2logicalpaln error");
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
				puts("fromlist no equaljoin");
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
				puts("fromlist no equaljoin");
				return NULL;
			}

		}break;
		default :
		{
			puts("parsetree2logicalplan type error");
			return NULL;
		}
	}
	return NULL;
}
static void get_aggregation_args(int sid,int &funcnum,Node *selectlist, vector<Attribute> &aggregation_attributes,vector<BlockStreamAggregationIterator::State::aggregation> &aggregation_function,LogicalOperator * input)
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
			get_aggregation_args(sid,funcnum,selectnode->args, aggregation_attributes,aggregation_function,input);
			get_aggregation_args(sid+1,funcnum,selectnode->next, aggregation_attributes,aggregation_function,input);
		}break;
		case t_select_expr:
		{
			Select_expr *selectnode=(Select_expr *)selectlist;
			get_aggregation_args(sid,funcnum,selectnode->colname, aggregation_attributes,aggregation_function,input);

		}break;
		case t_expr_func:
		{
			Expr_func * funcnode=(Expr_func *)selectlist;
			if(strcmp(funcnode->funname,"FCOUNTALL")==0)
			{
				aggregation_function.push_back(BlockStreamAggregationIterator::State::count);
				aggregation_attributes.push_back(Attribute(ATTRIBUTE_ANY));
				funcnum++;
			}
			else if(strcmp(funcnode->funname,"FCOUNT")==0)
			{
				aggregation_function.push_back(BlockStreamAggregationIterator::State::count);
				Columns *funccol=(Columns *)funcnode->parameter1;
		//		cout<<"Fcount  "<<funccol->parameter1<<" "<<funccol->parameter2<<endl;
				//aggregation_attributes.push_back(Environment::getInstance()->getCatalog()->getTable(funccol->parameter1)->getAttribute(funccol->parameter2));
				aggregation_attributes.push_back(input->getDataflow().attribute_list_[sid]);
				funcnum++;
			}/////////////////////----2.19日
			else if(strcmp(funcnode->funname,"FSUM")==0)
			{
				aggregation_function.push_back(BlockStreamAggregationIterator::State::sum);
				Columns *funccol=(Columns *)funcnode->parameter1;
				aggregation_attributes.push_back(input->getDataflow().attribute_list_[sid]);
				funcnum++;
			}
			else if(strcmp(funcnode->funname,"FMIN")==0)
			{
				aggregation_function.push_back(BlockStreamAggregationIterator::State::min);
				Columns *funccol=(Columns *)funcnode->parameter1;
				aggregation_attributes.push_back(input->getDataflow().attribute_list_[sid]);
				funcnum++;
			}
			else if(strcmp(funcnode->funname,"FMAX")==0)
			{
				aggregation_function.push_back(BlockStreamAggregationIterator::State::max);
				Columns *funccol=(Columns *)funcnode->parameter1;
				aggregation_attributes.push_back(input->getDataflow().attribute_list_[sid]);
				funcnum++;
			}
			else if(strcmp(funcnode->funname,"FAVG")==0)///////////////////////////////////底层还未实现
			{
				aggregation_function.push_back(BlockStreamAggregationIterator::State::count);
				Columns *funccol=(Columns *)funcnode->parameter1;
				aggregation_attributes.push_back(input->getDataflow().attribute_list_[sid]);
				funcnum++;
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
static void get_a_selectlist_expression_item(vector<ExpressionItem>&expr,Node *node)
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
				get_a_selectlist_expression_item(expr,funcnode->parameter1);
				ExpressionItem expritem1;
				expritem1.setOperator("case");
				expr.push_back(expritem1);
			}
			else if(strcmp(funcnode->funname,"CASE4")==0)//目前只支持case when [expr then expr]* [else expr] end
			{
				ExpressionItem expritem0;
				expritem0.setOperator("case");
				expr.push_back(expritem0);
				get_a_selectlist_expression_item(expr,funcnode->parameter1);
				get_a_selectlist_expression_item(expr,funcnode->parameter2);
				ExpressionItem expritem1;
				expritem1.setOperator("else");
				expr.push_back(expritem1);
				ExpressionItem expritem2;
				expritem2.setOperator("case");
				expr.push_back(expritem2);

			}
			else if(strcmp(funcnode->funname,"WHEN1")==0)
			{
				get_a_selectlist_expression_item(expr,funcnode->parameter1);
				ExpressionItem expritem0;
				expritem0.setOperator("when");
				expr.push_back(expritem0);

				get_a_selectlist_expression_item(expr,funcnode->parameter2);
				ExpressionItem expritem1;
				expritem1.setOperator("then");
				expr.push_back(expritem1);

			}
			else if(strcmp(funcnode->funname,"WHEN2")==0)
			{
				get_a_selectlist_expression_item(expr,funcnode->parameter1);
				ExpressionItem expritem0;
				expritem0.setOperator("when");
				expr.push_back(expritem0);

				get_a_selectlist_expression_item(expr,funcnode->parameter2);
				ExpressionItem expritem1;
				expritem1.setOperator("then");
				expr.push_back(expritem1);
			}
			else if(strcmp(funcnode->funname,"FSUBSTRING0")==0)
			{
				get_a_selectlist_expression_item(expr,funcnode->args);
				ExpressionItem expritem0;
				expritem0.setIntValue(0);
				expr.push_back(expritem0);
				get_a_selectlist_expression_item(expr,funcnode->parameter1);
				ExpressionItem expritem1;
				expritem1.setOperator("substr");
				expr.push_back(expritem1);
			}
			else if(strcmp(funcnode->funname,"FSUBSTRING1")==0)
			{
				get_a_selectlist_expression_item(expr,funcnode->args);
				get_a_selectlist_expression_item(expr,funcnode->parameter1);
				get_a_selectlist_expression_item(expr,funcnode->parameter2);
				ExpressionItem expritem1;
				expritem1.setOperator("substr");
				expr.push_back(expritem1);
			}
			else if(strcmp(funcnode->funname,"FTRIM0")==0)
			{
				ExpressionItem expritem0;
				expritem0.setIntValue(0);
				expr.push_back(expritem0);
				get_a_selectlist_expression_item(expr,funcnode->parameter1);
				get_a_selectlist_expression_item(expr,funcnode->parameter2);
				ExpressionItem expritem1;
				expritem1.setOperator("trim");
				expr.push_back(expritem1);
			}
			else if(strcmp(funcnode->funname,"FTRIM1")==0)
			{
				ExpressionItem expritem0;
				expritem0.setIntValue(1);
				expr.push_back(expritem0);
				get_a_selectlist_expression_item(expr,funcnode->parameter1);
				get_a_selectlist_expression_item(expr,funcnode->parameter2);
				ExpressionItem expritem1;
				expritem1.setOperator("trim");
				expr.push_back(expritem1);
			}
			else if(strcmp(funcnode->funname,"FTRIM2")==0)
			{
				ExpressionItem expritem0;
				expritem0.setIntValue(2);
				expr.push_back(expritem0);
				get_a_selectlist_expression_item(expr,funcnode->parameter1);
				get_a_selectlist_expression_item(expr,funcnode->parameter2);
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
				get_a_selectlist_expression_item(expr,funcnode->parameter1);
				ExpressionItem expritem1;
				expritem1.setOperator("trim");
				expr.push_back(expritem1);
			}
			else if(strcmp(funcnode->funname,"FUPPER")==0)
			{
				get_a_selectlist_expression_item(expr,funcnode->parameter1);
				ExpressionItem expritem1;
				expritem1.setOperator("upper");
				expr.push_back(expritem1);
			}
			else if(strcmp(funcnode->funname,"FCAST")==0)
			{
				get_a_selectlist_expression_item(expr,funcnode->parameter1);
				get_a_selectlist_expression_item(expr,funcnode->parameter2);
				ExpressionItem expritem1;
				expritem1.setOperator("cast");
				expr.push_back(expritem1);
			}
			else if(strcmp(funcnode->funname,"FCOALESCE")==0)
			{
				get_a_selectlist_expression_item(expr,funcnode->parameter1);
			}
			else if(strcmp(funcnode->funname,"FCOUNT")==0)
			{
				get_a_selectlist_expression_item(expr,funcnode->parameter1);
			}
			else if(strcmp(funcnode->funname,"FSUM")==0)
			{
				get_a_selectlist_expression_item(expr,funcnode->parameter1);
			}
			else if(strcmp(funcnode->funname,"FAVG")==0)
			{
				get_a_selectlist_expression_item(expr,funcnode->parameter1);
			}
			else if(strcmp(funcnode->funname,"FMIN")==0)
			{
				get_a_selectlist_expression_item(expr,funcnode->parameter1);
			}
			else if(strcmp(funcnode->funname,"FMAX")==0)
			{
				get_a_selectlist_expression_item(expr,funcnode->parameter1);
			}
			else
			{
				p2l_print_error("get_a_selectlist_expression_item: ",funcnode->funname ,"   is null");
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
				get_a_selectlist_expression_item(expr,calnode->rnext);
				ExpressionItem expritem;
				expritem.setOperator("-");
				expr.push_back(expritem);
			}
			else
			{
				get_a_selectlist_expression_item(expr,calnode->lnext);
				get_a_selectlist_expression_item(expr,calnode->rnext);
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
							p2l_print_error("get_a_selectlist_expression_item"," cmp error","");
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
			expritem.setVariable(col->parameter1,col->parameter2);
			expr.push_back(expritem);
		}break;
		case t_stringval:
		{
			Expr * exprval=(Expr *)node;
			ExpressionItem expritem;
			expritem.setStringValue(exprval->data.string_val);
			expr.push_back(expritem);
		}break;
		case t_intnum:
		{
			Expr * exprval=(Expr *)node;
			ExpressionItem expritem;
			expritem.setIntValue(exprval->data.int_val);
			expr.push_back(expritem);

		}break;
		case t_approxnum:
		{
			Expr * exprval=(Expr *)node;
			ExpressionItem expritem;
			expritem.setDoubleValue(exprval->data.double_val);
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
static void get_all_selectlist_expression_item(Node * node,vector<vector<ExpressionItem> >&allexpr)
{
	vector<ExpressionItem>expr;
	for(Node *p=node;p!=NULL;)
	{
		Select_list *selectlist=(Select_list *)p;
		Select_expr *sexpr=(Select_expr *)selectlist->args;
		get_a_selectlist_expression_item(expr,sexpr->colname);
		allexpr.push_back(expr);
		expr.clear();
		p=selectlist->next;
	}
}
static LogicalOperator* select_where_from2logicalplan(Node *parsetree)
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
		get_all_selectlist_expression_item(node->select_list,allexpr);
		cout<<"allexpr.size= "<<allexpr.size()<<endl;
		for(unsigned i=0;i<allexpr.size();i++){
			cout<<"allexpr "<<i<<"  size ="<<allexpr[i].size()<<endl;
			for(unsigned j=0;j<allexpr[i].size();j++){
				cout<<"******"<<endl;
				allexpr[i][j].print_value();
			}
		}
		LogicalOperator * where_from_logicalplan=where_from2logicalplan(node->from_list);
		LogicalOperator* proj=new LogicalProject(where_from_logicalplan,allexpr);
		return proj;
	}
}
static LogicalOperator* groupby_select_where_from2logicalplan(Node *parsetree)//实现groupby_select_where_from2logicalplan 到logicalplan的转换
{
	Query_stmt *node=(Query_stmt *)parsetree;
	LogicalOperator * select_where_from_logicalplan=select_where_from2logicalplan(parsetree);
	int funcnum=0,sid=0;
	vector<Attribute> group_by_attributes;
	vector<Attribute> aggregation_attributes;
	vector<BlockStreamAggregationIterator::State::aggregation> aggregation_function;
	get_aggregation_args(sid,funcnum,node->select_list, aggregation_attributes,aggregation_function,select_where_from_logicalplan);
	if(funcnum==0)
	{
		return select_where_from_logicalplan;
	}
	else//获得select子句中的聚集函数及其参数以及groupby子句参数
	{
		if(node->groupby_list!=NULL)
		get_group_by_attributes(node->groupby_list,group_by_attributes);
		LogicalOperator* aggregation=new Aggregation(group_by_attributes,aggregation_attributes,aggregation_function,select_where_from_logicalplan);
		return aggregation;
	}
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
				obcol.push_back(new LogicalSort::OrderByAttr(col->parameter1,col->parameter2));
			}break;
			case t_intnum:
			{
				Expr * exprval=(Expr *)(gbexpr->args);
				int num=0;
				Node * q=slnode;
				for(;q!=NULL;)
				{
					Select_list *selectlist=(Select_list *)q;
					if(num==exprval->data.int_val)
						break;
					num++;
					q=selectlist->next;
				}
				cout<<"```````````````````````````````"<<endl;
				if(q==NULL)
				{
					//string str=(exprval->data.int_val);
					p2l_print_error("get_orderby_column_from_selectlist order by attribute num ","","is beyond");
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
							cout<<"order by attribute  "<<col->parameter1<<"  "<<col->parameter2<<endl;
							obcol.push_back(new LogicalSort::OrderByAttr(col->parameter1,col->parameter2));
						}break;
						default:
						{
							p2l_print_error("get_orderby_column_from_selectlist","gbexpr->colname->type ","not exist");
						}
					}
				}
			}break;
			default:
			{
				p2l_print_error("get_orderby_column_from_selectlist","gbexpr->args->type ","not exist");
			}
		}
		p=gbexpr->next;
	}
}
static LogicalOperator* orderby_select_groupby_where_from2logicalplan(Node *parsetree)
{
	LogicalOperator* select__logicalplan= groupby_select_where_from2logicalplan(parsetree);
	Query_stmt *node=(Query_stmt *)parsetree;
	if(node->orderby_list==NULL)
	{
		return select__logicalplan;
	}
	else
	{
		vector<LogicalSort::OrderByAttr *>obcol;
		get_orderby_column_from_selectlist(node->orderby_list,node->select_list,obcol);
		LogicalOperator* orderby__logicalplan=new LogicalSort(select__logicalplan,obcol[0]);
		return orderby__logicalplan;
	}
}
static LogicalOperator* parsetree2logicalplan(Node *parsetree)//实现parsetree 到logicalplan的转换，
{
	LogicalOperator* result=NULL;
	switch(parsetree->type)
	{
		case t_query_stmt:
		{
			result=orderby_select_groupby_where_from2logicalplan(parsetree);
		}break;
		default:
		{
			puts("parsetree type is null");
			result= NULL;
		}
	}
	cout<<"parsetree2Logicalplan is completed!!!!!!!!!!!!!!!!!!!!!"<<endl;
	return result;
}

#endif

