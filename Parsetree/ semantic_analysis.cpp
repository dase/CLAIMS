/*
3.6日
目前主要完成对表名/属性名判断是否唯一存在以及根据单个属性获得表的别名等
尚需完善：
1.函数用的格式
2.表达式是否符合正确格式
3.groupby子句运用规则
4.aggrection(没有groupby)
*/

#include <cstdio>
#include <map>
#include <new>
#include <set>
#include <string>
#include <vector>
#include "../Catalog/Attribute.h"
#include "../Catalog/Catalog.h"
#include "../Catalog/table.h"
#include "../common/Comparator.h"
#include "../LogicalQueryPlan/EqualJoin.h"
#include "../LogicalQueryPlan/Filter.h"
#include "../LogicalQueryPlan/LogicalOperator.h"
#include "../LogicalQueryPlan/Scan.h"
#include "../BlockStreamIterator/ParallelBlockStreamIterator/BlockStreamAggregationIterator.h"
#include"sql_node_struct.h"
#include "../Environment.h"
#include "../LogicalQueryPlan/Aggregation.h"
void print_analysis_error(char * str1,char *str2,char * str3)
{
	printf("!!! Semantic Analysis >> %s >> %s   %s\n",str1,str2,str3);
}
int table_has_column(char *colname,Node * node,char *&tablename)//判断是否是否有table含有column
{
	int result=0;
	for(Node *p=node;p!=NULL;)
	{
		From_list *fromlist=(From_list *)p;
		Table *table=(Table *)fromlist->args;
		if(Environment::getInstance()->getCatalog()->isAttributeExist(table->tablename,colname)==true)
		{
			//strcpy(tablename,table->astablename);//返回astablename
			tablename=table->astablename;
			result++;
		}
		p=fromlist->next;
	}
	return result;
}
bool fromlist_has_astablename(char *astablename,Node *node,char *&tablename)//根据astablename从fromlist中得到tablename
{
	for(Node *p=node;p!=NULL;)
	{
		From_list *fromlist=(From_list *)p;
		Table *table=(Table *)fromlist->args;
		if(strcmp(table->astablename,astablename)==0)
		{
			//strcpy(tablename,table->tablename);
			tablename=table->tablename;
			return true;
		}
		p=fromlist->next;
	}
	return false;
}
void add_all_table_column(Select_list *selectlist,Node * flist,Node *next)
{
	for(Node *p=flist;p!=NULL;)
	{
		From_list *fromlist=(From_list *)p;
		Table *table=(Table *)fromlist->args;
		vector<Attribute>columns=Environment::getInstance()->getCatalog()->getTable(table->tablename)->getAttributes();
		for(vector<Attribute>::iterator it=columns.begin();it!=columns.end();it++)
		{
			Node *q=newColumn(t_name_name,table->astablename,(char*)(it->attrName).c_str(),NULL);
				  q=newSelectExpr(t_select_expr,(char*)(it->attrName).c_str(),q);
				  q=newSelectList(t_select_list,0,q,NULL);
			selectlist->next=q;
			selectlist=(Select_list *)q;
		}
		p=fromlist->next;
	}
	selectlist->next=next;
}
void add_table_column(Select_list *selectlist,Query_stmt * qstmt,Node *next)
{
	Select_expr *sexpr=(Select_expr *)selectlist->args;
	Columns *col=(Columns *)sexpr->colname;
	char * tablename;
	fromlist_has_astablename(col->parameter1,qstmt->from_list,tablename);
	vector<Attribute>columns=Environment::getInstance()->getCatalog()->getTable(tablename)->getAttributes();
	for(vector<Attribute>::iterator it=columns.begin();it!=columns.end();it++)
	{
		Node *q=newColumn(t_name_name,col->parameter1,(char*)(it->attrName).c_str(),NULL);
			  q=newSelectExpr(t_select_expr,(char*)(it->attrName).c_str(),q);
			  q=newSelectList(t_select_list,0,q,NULL);
		selectlist->next=q;
		selectlist=(Select_list *)q;
	}
	selectlist->next=next;
}

bool selectlist_analysis(Query_stmt * qstmt)
{
	Node *sltree=(Node *)qstmt->select_list;
	if(sltree==NULL)
	{
		print_analysis_error("selectlist","query_stmt select_list is null","");
	}
	else
	{
		for(Node *p=sltree;p!=NULL;)
		{
			Select_list *selectlist=(Select_list *)p;
			if(selectlist->isall==0)
			{
				Select_expr *sexpr=(Select_expr *)selectlist->args;
				Node *node=(Node *)sexpr->colname;
				if(node->type==t_expr_func)//selectlist中的function
				{
					Expr_func * funcnode=(Expr_func *)node;
					if(strcmp(funcnode->funname,"FCOUNTALL")==0)
					{
						p=selectlist->next;
						continue;
					}
					else
					{
						node=(Node *)funcnode->parameter1;
					}
				}
				switch (node->type)
				{
					case t_name:
					{
						Columns *col=(Columns *)node;
						char *astablename;
						int result=table_has_column(col->parameter2,qstmt->from_list,astablename);
						if(result==1)
						{
							col->parameter1=astablename;
							col->type=t_name_name;
							if(sexpr->ascolname==NULL)
							{
								//strcpy(sexpr->ascolname,col->parameter2);
								sexpr->ascolname=col->parameter2;
							}
						}
						else if(result==0)
						{
							print_analysis_error("selectlist",col->parameter2,"can't find in tables");
							return false;
						}
						else
						{
							print_analysis_error("selectlist",col->parameter2,"is ambiguous");
							return false;
						}
					}break;
					case t_name_name:
					{
						Columns *col=(Columns *)node;
						char *tablename;
						if(fromlist_has_astablename(col->parameter1,qstmt->from_list,tablename)==false)
						{
							char * str=col->parameter1;
							//strcat(str,".");
							//strcat(str,col->parameter2);
							print_analysis_error("selectlist",str," can't find ");
							return false;
						}
						if(strcmp(col->parameter2,"*")==0)
						{
							selectlist->isall=1;
						}
						else
						{
							if(Environment::getInstance()->getCatalog()->isAttributeExist(tablename,col->parameter2)==0)
							{
								char *str=col->parameter2;
								//strcat(str,"  can't find ");
								print_analysis_error("selectlist",col->parameter1,str);
								return false;
							}
							else
							{
								if(sexpr->ascolname==NULL)
								{
									//strcpy(sexpr->ascolname,col->parameter2);
									sexpr->ascolname=col->parameter2;
								}
							}
						}
					}break;
					default:
					{

					}
				}
			}
			p=selectlist->next;
			if(selectlist->isall==1)//处理*
			{
				if(selectlist->args==NULL)//*
				{
					add_all_table_column(selectlist,qstmt->from_list,p);
				}
				else//table.*
				{
					add_table_column(selectlist,qstmt,p);
				}
			}
		}
		/*删除isall的节点*/
		for(Node *p=sltree;p!=NULL;)
		{
			Select_list *selectlist=(Select_list *)p;
			if(selectlist->isall==0)
			{
				sltree=p;
				break;
			}
			p=selectlist->next;
			free(selectlist);
		}
		qstmt->select_list=sltree;
		Node *frontnode=sltree;
		Node *rearnode;
		for(Node *p=((Select_list *)sltree)->next;p!=NULL;)
		{
			Select_list *selectlist=(Select_list *)p;
			if(selectlist->isall==1)
			{
				((Select_list *)frontnode)->next=selectlist->next;
				free(selectlist);
			}
			else
			{
				frontnode=p;
			}
			p=selectlist->next;
		}
	}
	return true;
}
bool oncondition_analysis(Node *octree)
{
	return true;
}
bool fromlist_analysis(Query_stmt *qstmt)
{
	Node *fltree=(Node *)qstmt->from_list;
	if(fltree==NULL)
	{
		print_analysis_error("fromlist","query_stmt from_list is null","");
	}
	else
	{
		for(Node *p=fltree;p!=NULL;)//判断是否可以唯一获得表
		{
			From_list *fromlist=(From_list *)p;
			Table *table=(Table *)fromlist->args;
			if(Environment::getInstance()->getCatalog()->getTable(table->tablename)==NULL)
			{
				print_analysis_error("fromlist",table->tablename," can't find ");
				return false;
			}
			else
			{
				if(table->astablename==NULL)
				{
					table->astablename=table->tablename;
				//	strcpy(table->astablename,(const char *)table->tablename);
				}
			}
			p=fromlist->next;
		}

		for(Node *p=fltree;p!=NULL;)//表名判重
		{
			From_list *fromlist=(From_list *)p;
			Table *table=(Table *)fromlist->args;
			for(Node *q=fromlist->next;q!=NULL;)
			{
				From_list *fromlist1=(From_list *)q;
				Table *table1=(Table *)fromlist1->args;
				if(strcmp(table->astablename,table1->astablename)==0)
				{
					print_analysis_error("fromlist",table->astablename,"is ambiguous");
					return false;
				}
				q=fromlist1->next;
			}
			p=fromlist->next;
		}
	}
	return true;
}
int selectlist_has_column(char *&ascolname,Node *sltree,char *&astablename)//selectlist中是否有ascolumn
{
	int result=0;
	for(Node *p=sltree;p!=NULL;)
	{
		Select_list *selectlist=(Select_list *)p;
		Select_expr *sexpr=(Select_expr *)selectlist->args;
		Node *node=(Node *)sexpr->colname;//不一定是column
		switch (node->type)
		{
			case t_name_name:
			{
				Columns *col=(Columns *)node;
				if(strcmp(sexpr->ascolname,ascolname)==0)
				{
					strcpy(ascolname,col->parameter2);
					astablename=col->parameter1;
					result++;
				}

			}break;
			default:
			{

			}
		}
		p=selectlist->next;
	}
	return result;
}
bool wherecondition_check(Query_stmt * qstmt,Node *cur)
{
	switch(cur->type)
	{
		case t_name:
		{
			Columns *col=(Columns *)cur;
			char *astablename;
			int result=table_has_column(col->parameter2,qstmt->from_list,astablename);
			if(result==1)
			{
				col->parameter1=astablename;
				col->type=t_name_name;
			}
			else if(result==0)//如果fromlist中没有，那么要在selectlisth中寻找
			{
				int result=selectlist_has_column(col->parameter2,qstmt->select_list,astablename);
				if(result==0)
				{
					print_analysis_error("wherecondition",col->parameter2,"can't find in tables and selectlist ");
					return false;
				}
				else if(result==1)
				{
					col->parameter1=astablename;
					col->type=t_name_name;
				}
				else
				{
					print_analysis_error("wherecondition",col->parameter2,"in selectlist is ambiguous");
					return false;
				}
			}
			else
			{
				print_analysis_error("wherecondition",col->parameter2,"in fromlist is ambiguous");
				return false;
			}
		}break;
		case t_name_name:
		{
			Columns *col=(Columns *)cur;
			char *tablename;
			if(fromlist_has_astablename(col->parameter1,qstmt->from_list,tablename)==false)
			{
				char str[100]="str is null";
				strcpy(str,col->parameter1);
				strcat(str,".");
				strcat(str,col->parameter2);
				print_analysis_error("wherecondition",str," can't find ");
				return false;
			}
			else
			{
				if(Environment::getInstance()->getCatalog()->isAttributeExist(tablename,col->parameter2)==0)
				{
					char str[100]="str is null";
					strcpy(str,col->parameter2);
					strcat(str,"  can't find ");
					print_analysis_error("wherecondition",col->parameter1,str);
					return false;
				}
			}

		}break;
		case t_expr_cal:
		{
			Expr_cal *node=(Expr_cal*)cur;
			if(node->lnext==NULL||node->rnext==NULL)
			return false;
			wherecondition_check(qstmt,node->lnext);
			wherecondition_check(qstmt,node->rnext);
		}break;
		case t_expr_func:
		{
			Expr_func* node=(Expr_func *)cur;
			if(node->args==NULL)
			return false;
			wherecondition_check(qstmt,node->args);
		}break;
		default:
		{

		}
	}
	return true;
}
bool wherecondition_analysis(Query_stmt * qstmt,Node *cur)
{
	if(cur==NULL)
	{
		return true;
	}
	switch (cur->type)
	{
		case t_expr_cal:
		{
			struct Expr_cal * exprnode=(struct Expr_cal *)cur;
			if(strcmp((exprnode->sign),"ANDOP")==0)
			{
				bool flag=true;
				flag=wherecondition_analysis(qstmt,exprnode->lnext);
				if(flag==false)
					return false;
				flag=wherecondition_analysis(qstmt,exprnode->rnext);
				if(flag==false)
					return false;
			}
			else
			{
				wherecondition_check(qstmt,cur);
			}
		}break;
		default:
		{
			wherecondition_check(qstmt,cur);
		}
	}
	return true;
}
bool havingcondition_analysis(Node *hctree)
{
	return true;
}
bool groupby_analysis(Query_stmt * qstmt)
{
	Groupby_list * gblist=(Groupby_list *)(qstmt->groupby_list);
	for(Node *p=(Node *)(gblist->next);p!=NULL;)
	{
		Groupby_expr *gbexpr=(Groupby_expr *)p;
		switch(gbexpr->args->type)
		{
			case t_name:
			{
				Columns *col=(Columns *)(gbexpr->args);
				char *astablename;
				int result=table_has_column(col->parameter2,qstmt->from_list,astablename);
				if(result==1)
				{
					col->parameter1=astablename;
					col->type=t_name_name;
				}
				else if(result==0)//如果fromlist中没有，那么要在selectlisth中寻找
				{
					int result=selectlist_has_column(col->parameter2,qstmt->select_list,astablename);
					if(result==0)
					{
						print_analysis_error("groupbylist",col->parameter2,"can't find in tables and selectlist ");
						return false;
					}
					else if(result==1)
					{
						col->parameter1=astablename;
						col->type=t_name_name;
					}
					else
					{
						print_analysis_error("groupbylist",col->parameter2,"in selectlist is ambiguous");
						return false;
					}
				}
				else
				{
					print_analysis_error("groupbylist",col->parameter2,"in fromlist is ambiguous");
					return false;
				}
			}break;
			case t_name_name:
			{
				Columns *col=(Columns *)(gbexpr->args);
				char *tablename;
				if(fromlist_has_astablename(col->parameter1,qstmt->from_list,tablename)==false)
				{
					char str[100]="str is null";
					strcpy(str,col->parameter1);
					strcat(str,".");
					strcat(str,col->parameter2);
					print_analysis_error("groupbylist",str," can't find ");
					return false;
				}
				else
				{
					if(Environment::getInstance()->getCatalog()->isAttributeExist(tablename,col->parameter2)==0)
					{
						char str[100]="str is null";
						strcpy(str,col->parameter2);
						strcat(str,"  can't find ");
						print_analysis_error("groupbylist",col->parameter1,str);
						return false;
					}
				}
			}break;
			default:
			{
				print_analysis_error("groupbylist","there is other type in groupby list","not the colname ");
				return false;
			}
		}
		p=gbexpr->next;
	}
	return true;
}
bool orderby_analysis(Query_stmt * qstmt)//与groupby处理相同
{
	Orderby_list * gblist=(Orderby_list *)(qstmt->orderby_list);
	for(Node *p=(Node *)(gblist->next);p!=NULL;)
	{
		Groupby_expr *gbexpr=(Groupby_expr *)p;
		switch(gbexpr->args->type)
		{
			case t_name:
			{
				Columns *col=(Columns *)(gbexpr->args);
				char *astablename;
				int result=table_has_column(col->parameter2,qstmt->from_list,astablename);
				if(result==1)
				{
					col->parameter1=astablename;
					col->type=t_name_name;
				}
				else if(result==0)//如果fromlist中没有，那么要在selectlisth中寻找
				{
					int result=selectlist_has_column(col->parameter2,qstmt->select_list,astablename);
					if(result==0)
					{
						print_analysis_error("orderbylist",col->parameter2,"can't find in tables and selectlist ");
						return false;
					}
					else if(result==1)
					{
						col->parameter1=astablename;
						col->type=t_name_name;
					}
					else
					{
						print_analysis_error("orderbylist",col->parameter2,"in selectlist is ambiguous");
						return false;
					}
				}
				else
				{
					print_analysis_error("orderbylist",col->parameter2,"in fromlist is ambiguous");
					return false;
				}
			}break;
			case t_name_name:
			{
				Columns *col=(Columns *)(gbexpr->args);
				char *tablename;
				if(fromlist_has_astablename(col->parameter1,qstmt->from_list,tablename)==false)
				{
					char str[100]="str is null";
					strcpy(str,col->parameter1);
					strcat(str,".");
					strcat(str,col->parameter2);
					print_analysis_error("orderbylist",str," can't find ");
					return false;
				}
				else
				{
					if(Environment::getInstance()->getCatalog()->isAttributeExist(tablename,col->parameter2)==0)
					{
						char str[100]="str is null";
						strcpy(str,col->parameter2);
						strcat(str,"  can't find ");
						print_analysis_error("orderbylist",col->parameter1,str);
						return false;
					}
				}
			}break;
			default:
			{
				print_analysis_error("orderbylist","there is other type in orderby list","not the colname ");
				return false;
			}
		}
		p=gbexpr->next;
	}
	return true;
}

bool semantic_analysis(Node *parsetree)
{
	if(parsetree==NULL)
	{
		print_analysis_error("","parsetree is null","");
	}
	switch (parsetree->type)
	{
		case t_query_stmt:
		{
			Query_stmt* qstmt=(Query_stmt *)parsetree;
			bool flag=true;
			if(qstmt->from_list!=NULL)
			{
				flag=fromlist_analysis(qstmt);
				if(flag==false)
					return false;
			}
			if(qstmt->select_list!=NULL)
			{
				flag=selectlist_analysis(qstmt);
				if(flag==false)
					return false;
			}
			if(qstmt->where_list!=NULL)
			{
				flag=wherecondition_analysis(qstmt,((Where_list*)qstmt->where_list)->next);
				if(flag==false)
					return false;
			}
			if(qstmt->groupby_list!=NULL)
			{
				flag=groupby_analysis(qstmt);
				if(flag==false)
					return false;
			}
			if(qstmt->orderby_list!=NULL)
			{
				flag=orderby_analysis(qstmt);
				if(flag==false)
					return false;
			}
		}break;
		default:
		{

		};
	}
	return true;
}


