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
#include "../Logging.h"
bool semantic_analysis(Node *parsetree,bool issubquery);
bool wherecondition_analysis(Query_stmt * qstmt,Node *cur,vector<Node *>rtable);

int subquery_has_column(char *colname,Node * subquery)
{
	int result=0;
	Query_stmt * querynode=(Query_stmt *)subquery;

	for(Node * p=querynode->select_list;p!=NULL;)
	{
		Select_list * slist=(Select_list *)p;
		Select_expr * sexpr=(Select_expr *)slist->args;
		if(strcmp(colname,sexpr->ascolname)==0)
		{
			result++;
		}
		p=slist->next;
	}
	return result;
}
int table_has_column(char *colname,vector<Node *>rtable,char *&tablename)//判断是否是否有table含有column
{
	int result=0;
	for(int i=0;i<rtable.size();i++)
//	for(Node *p=node;p!=NULL;)
	{
	//	From_list *fromlist=(From_list *)rtable[i];
		Table *table=(Table *)rtable[i];
		if(table->issubquery==0)
		{
			if(Environment::getInstance()->getCatalog()->isAttributeExist(table->tablename,colname)==true)
			{
				//strcpy(tablename,table->astablename);//返回astablename
				tablename=table->astablename;
				result++;
			}
		}
		else if(table->issubquery==1)//判断除了基表之外的子查询中是否有这个属性
		{
			int fg=subquery_has_column(colname,table->subquery);
			if(fg>0)
			{
				tablename=table->astablename;
				result+=fg;
			}
		}
	//	p=fromlist->next;
	}
	return result;
}
int fromlist_has_astablename(char *astablename,vector<Node *>rtable,char *&tablename,Node *&subnode)//根据astablename从fromlist中得到tablename,如果是subquery则返回2，基表返回1
{
	for(int i;i<rtable.size();i++)
//	for(Node *p=node;p!=NULL;)
	{
	//	From_list *fromlist=(From_list *)rtable[i];
		Table *table=(Table *)rtable[i];
		if(strcmp(table->astablename,astablename)==0)
		{
			//strcpy(tablename,table->tablename);
			if(table->issubquery==0)
			{
				tablename=table->tablename;
				return 1;
			}
			else
			{
				subnode=table->subquery;
				return 2;
			}
		}
	//	p=fromlist->next;
	}
	return 0;
}
Node * get_copy_selectlist_from_subquery(Node *subnode,Node *&tailnode)//传回第一个和最后一个指针
{
	Select_list *node,*temp;
	node=temp=(Select_list *)newSelectList(t_select_list,0,NULL,NULL);
	Query_stmt* subquery=(Query_stmt *)subnode;
	for(Node *p=subquery->select_list;p!=NULL;)//并不是复制所有，而只是将select_list复制
	{
		Select_list *slist=(Select_list *)p;
		Node *q=newSelectList(t_select_list,slist->isall,slist->args,NULL);
		temp->next=q;
		temp=(Select_list *)q;
		p=slist->next;
	}
	tailnode=(Node *)temp;
	return node->next;
}
void add_all_table_column(Select_list *selectlist,vector<Node *>rtable,Node *next)
{
	for(int i;i<rtable.size();i++)
//	for(Node *p=flist;p!=NULL;)
	{
	//	output(rtable[i],0);
	//	From_list *fromlist=(From_list *)rtable[i];
		Table *table=(Table *)rtable[i];
		if(table->issubquery==0)
		{
			vector<Attribute>columns=Environment::getInstance()->getCatalog()->getTable(table->tablename)->getAttributes();
			for(vector<Attribute>::iterator it=columns.begin();it!=columns.end();it++)
			{
				Node *q=newColumn(t_name_name,table->astablename,(char*)(it->attrName).c_str(),NULL);
					  q=newSelectExpr(t_select_expr,(char*)(it->attrName).c_str(),q);
					  q=newSelectList(t_select_list,0,q,NULL);
				selectlist->next=q;
				selectlist=(Select_list *)q;
			}
		}
		else
		{
			Node * tailnode;
			Node *q=get_copy_selectlist_from_subquery(table->subquery,tailnode);
			selectlist->next=q;
			selectlist=(Select_list *)tailnode;
		}
		//p=fromlist->next;
	}
	selectlist->next=next;
}
void add_table_column(Select_list *selectlist,Node *next,vector<Node *>rtable)//table.*
{
	Select_expr *sexpr=(Select_expr *)selectlist->args;
	Columns *col=(Columns *)sexpr->colname;
	char * tablename;
	Node *subnode;
	int fg=fromlist_has_astablename(col->parameter1,rtable,tablename,subnode);
	if(fg==1)
	{
		vector<Attribute>columns=Environment::getInstance()->getCatalog()->getTable(tablename)->getAttributes();
		for(vector<Attribute>::iterator it=columns.begin();it!=columns.end();it++)
		{
			Node *q=newColumn(t_name_name,col->parameter1,(char*)(it->attrName).c_str(),NULL);
				  q=newSelectExpr(t_select_expr,(char*)(it->attrName).c_str(),q);
				  q=newSelectList(t_select_list,0,q,NULL);
			selectlist->next=q;
			selectlist=(Select_list *)q;
		}
	}
	else if(fg==2)
	{
		Node *tailnode;
		Node *q=get_copy_selectlist_from_subquery(subnode,tailnode);
		selectlist->next=q;
		selectlist=(Select_list *)tailnode;
	}
	selectlist->next=next;
}
int selectlist_expr_analysis(Node* slnode,Query_stmt * qstmt,Node *node,vector<Node *>rtable)
{
	Select_list *selectlist=(Select_list *)slnode;
	Select_expr *sexpr=(Select_expr *)selectlist->args;

	switch(node->type)
	{
		case t_expr_func://需要判断函数使用是否正确，包括参数及参数类型
		{
			Expr_func * funcnode=(Expr_func *)node;
			if(strcmp(funcnode->funname,"FCOUNTALL")==0)
			{
				return 3;
			}
			else
			{
				int result=selectlist_expr_analysis(slnode,qstmt,funcnode->parameter1,rtable);
				if(result==3)//countall不能被套用
					return 0;
				else
					return result;
			}
		}break;
		case t_expr_cal://需要判断表达式是否正确使用，比如计算符号左右的类型，以及除数是否为零判断等
		{
			Expr_cal * calnode=(Expr_cal *)node;
			return selectlist_expr_analysis(slnode,qstmt,calnode->lnext,rtable)&selectlist_expr_analysis(slnode,qstmt,calnode->rnext,rtable);
		}break;
		case t_name:
		{
			Columns *col=(Columns *)node;
			char *astablename;
			int result=table_has_column(col->parameter2,rtable,astablename);
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
				SQLParse_elog("selectlist",col->parameter2,"can't find in tables");
				return 0;
			}
			else
			{
				SQLParse_elog("selectlist",col->parameter2,"is ambiguous");
				return 0;
			}
		}break;
		case t_name_name:
		{
			Columns *col=(Columns *)node;


			char *tablename;
			Node * subnode;
			int fg=fromlist_has_astablename(col->parameter1,rtable,tablename,subnode);
			if(fg==0)
			{
				char * str="str is null";
				strcpy(str,col->parameter1);
				strcat(str,".");
				strcat(str,col->parameter2);
				SQLParse_elog("selectlist",str," can't find ");
				return 0;
			}
			if(strcmp(col->parameter2,"*")==0)
			{
				selectlist->isall=1;
				return 1;
			}
			if(fg==1)//基表
			{
				if(Environment::getInstance()->getCatalog()->isAttributeExist(tablename,col->parameter2)==0)
				{
					char *str=col->parameter2;
					//strcat(str,"  can't find ");
					SQLParse_elog("selectlist",col->parameter1,str);
					return 0;
				}
				else
				{
					if(sexpr->ascolname==NULL)
					{
						//strcpy(sexpr->ascolname,col->parameter2);
						sexpr->ascolname=col->parameter2;
					}
					return 1;
				}
			}
			if(fg==2)//subquery
			{
				int fg=subquery_has_column(col->parameter2,subnode);
				if(fg>1)
					return 0;
				return fg;
			}
		}break;
		default:
		{

		}
	}
	return 1;
}
bool selectlist_analysis(Query_stmt * qstmt,vector<Node *>rtable)
{
	Node *sltree=(Node *)qstmt->select_list;
	if(sltree==NULL)
	{
		SQLParse_elog("selectlist","query_stmt select_list is null","");
	}
	else
	{
		for(Node *p=sltree;p!=NULL;)
		{
			Select_list *selectlist=(Select_list *)p;
			if(selectlist->isall==0)
			{
				Select_expr *sexpr=(Select_expr *)selectlist->args;
				Node *node=(Node *)sexpr->colname;//此处selectexpr包括expr_func,t_name,t_name_name,expr_cal
				int result=selectlist_expr_analysis(p,qstmt,node,rtable);
				if(result==3)//特殊判断fcountall情况
				{
					p=selectlist->next;
					continue;
				}
				else if(result==0)
				{
					return false;
				}
			}
			p=selectlist->next;
			if(selectlist->isall==1)//处理*
			{
				if(selectlist->args==NULL)//*
				{
					add_all_table_column(selectlist,rtable,p);
				}
				else//table.*
				{
					add_table_column(selectlist,p,rtable);
				}
			}
		}
		/*删除isall的节点*/
		for(Node *p=sltree;p!=NULL;)//处理第一个isall的
		{
			Select_list *selectlist=(Select_list *)p;
			if(selectlist->isall==0)
			{
				sltree=p;
				break;
			}
			p=selectlist->next;
			free(selectlist);////此处消除貌似有点不规范
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
int oncondition_check(Node *cur,vector<Node *>rtable)
{
	switch(cur->type)
	{
		case t_name:
		{
			Columns *col=(Columns *)cur;
			char *astablename;
			int result=table_has_column(col->parameter2,rtable,astablename);
			if(result==1)
			{
				col->parameter1=astablename;
				col->type=t_name_name;
			}
			else if(result==0)
			{
				SQLParse_elog("oncondition",col->parameter2,"can't find in tables and selectlist ");
				return false;
			}
			else
			{
				SQLParse_elog("oncondition",col->parameter2,"in fromlist is ambiguous");
				return false;
			}
		}break;
		case t_name_name:
		{
			Columns *col=(Columns *)cur;
			char *tablename;
			Node *subnode;
			int fg=fromlist_has_astablename(col->parameter1,rtable,tablename,subnode);
			if(fg==0)
			{
				char str[100]="str is null";
				strcpy(str,col->parameter1);
				strcat(str,".");
				strcat(str,col->parameter2);
				SQLParse_elog("oncondition",str," can't find ");
				return false;
			}
			else if(fg==1)
			{
				if(Environment::getInstance()->getCatalog()->isAttributeExist(tablename,col->parameter2)==0)
				{
					char str[100]="str is null";
					strcpy(str,col->parameter2);
					strcat(str,"  can't find ");
					SQLParse_elog("oncondition",col->parameter1,str);
					return false;
				}
			}
			else if(fg==2)
			{
				int fg=subquery_has_column(col->parameter2,subnode);
				if(fg>1||fg==0)
					return false;
			}

		}break;
		case t_query_stmt:
		{
			return semantic_analysis(cur,true);
		}break;
		case t_expr_cal:
		{
			Expr_cal *node=(Expr_cal*)cur;
			if(node->lnext==NULL||node->rnext==NULL)
			return false;
			bool flag=true;
			flag=oncondition_check(node->lnext,rtable);
			if(flag==false)
				return false;
			flag=oncondition_check(node->rnext,rtable);
			if(flag==false)
			return false;
		}break;
		case t_expr_func:
		{
			Expr_func* node=(Expr_func *)cur;
			if(node->args==NULL)
			return false;
			return oncondition_check(node->args,rtable);
		}break;
		case t_expr_list:
		{
			Expr_list * node=(Expr_list *)cur;
			bool flag=true;
			if(node->data!=NULL)
			{
				flag= oncondition_check(node->data,rtable);
				if(flag==false)
					return false;
			}
			else
			{
				return false;
			}
			if(node->next!=NULL)
			{
				return oncondition_check(node->next,rtable);
			}
		}break;
		default:
		{

		}
	}
	return true;
}
bool oncondition_analysis(Node *cur,vector<Node *>jointable)
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
				flag=oncondition_analysis(exprnode->lnext,jointable);
				if(flag==false)
					return false;
				flag=oncondition_analysis(exprnode->rnext,jointable);
				if(flag==false)
					return false;
			}
			else
			{
				oncondition_check(cur,jointable);
			}
		}break;
		default:
		{
			oncondition_check(cur,jointable);
		}
	}
	return true;
}
bool fromlist_analysis(Query_stmt * querynode,Node *qnode,vector<Node *>&rtable)
{
	if(qnode==NULL)
	{
		SQLParse_elog("fromlist","query_stmt from_list is null","");
	}
	switch(qnode->type)
	{
		case t_query_stmt:
		{
			Query_stmt *qstmt=(Query_stmt *)qnode;
			bool fg=fromlist_analysis(qstmt,qstmt->from_list,rtable);
			if(fg==false)
				return false;
		}break;
		case t_from_list:
		{
			From_list * flnode=(From_list *)qnode;
			bool fg=false;
			fg=fromlist_analysis(querynode,flnode->args,rtable);
			assert(fg);
			if(fg==false)
				return false;
			if(flnode->next!=NULL)
			{
				fg=fromlist_analysis(querynode,flnode->next,rtable);
				assert(fg);
				if(fg==false)
					return false;
			}
		}break;
		case t_table:
		{
			Table *table=(Table *)qnode;
			rtable.push_back(qnode);
			if(table->issubquery==0)//不是subquery的情况
			{
				if(Environment::getInstance()->getCatalog()->getTable(table->tablename)==NULL)
				{
					SQLParse_elog("fromlist",table->tablename," can't find ");
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
			}
			else//是subquery的情况
			{
				bool fg= semantic_analysis(table->subquery,true);
				if(fg==false)
					return false;
			}
		}break;
		case t_join:
		{
			Join * jnode=(Join *)qnode;
			bool fg=false;
			fg=fromlist_analysis(querynode,jnode->lnext,rtable);
			if(fg==false)
				return false;
			fg=fromlist_analysis(querynode,jnode->rnext,rtable);
			if(fg==false)
				return false;
			vector<Node *>jointable;
			jointable.push_back(jnode->lnext);
			jointable.push_back(jnode->rnext);
			if(jnode->condition!=NULL)/*on 1 using 2*/
			{
				Condition *cnode=(Condition *)jnode->condition;
				if(cnode->conditiontype==1)
				{
					fg=oncondition_analysis(cnode->args,rtable);//应该限定表的范围
					if(fg==false)
						return false;
				}
				else
				{
					Node *tp=newExprList(t_expr_list,NULL,NULL);
					Node *temp=tp;
					for(Node *p=cnode->args;p!=NULL;)
					{
						Columns * col=(Columns *)p;
						cout<<((Table *)(jnode->lnext))->astablename<<"dddddddddddddd "<<col->parameter1<<endl;
						Node * cal=newExprCal(t_expr_cal, "CMP", NULL, 4,newColumn(t_name_name,((Table *)(jnode->lnext))->astablename, col->parameter1, NULL),newColumn(t_name_name,((Table *)(jnode->rnext))->astablename, col->parameter1, NULL));
						Node *q=newExprList(t_expr_list,cal,NULL);
						((Expr_list *)tp)->next=q;
						tp=q;
						p=col->next;
					}
					cnode->args=((Expr_list*)temp)->next;
				}
			}
		}break;
		default:
		{

		}
	}
	return true;
}
bool fromlist_table_is_unique(vector<Node *>rtable)
{
	for(int i;i<rtable.size();i++)
//	for(Node *p=fltree;p!=NULL;)//表名判重
	{
	//	From_list *fromlist=(From_list *)rtable[i];
		Table *table=(Table *)rtable[i];
		for(int j;j<rtable.size();j++)
	//	for(Node *q=fromlist->next;q!=NULL;)
		{
		//	From_list *fromlist1=(From_list *)rtable[j];
			Table *table1=(Table *)rtable[i];
			if(strcmp(table->astablename,table1->astablename)==0)
			{
				SQLParse_elog("fromlist",table->astablename,"is ambiguous");
				return false;
			}
		//	q=fromlist1->next;
		}
	//	p=fromlist->next;
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
bool wherecondition_check(Query_stmt * qstmt,Node *cur,vector<Node *>rtable)
{
	switch(cur->type)
	{
		case t_name:
		{
			Columns *col=(Columns *)cur;
			char *astablename;
			int result=table_has_column(col->parameter2,rtable,astablename);
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
					SQLParse_elog("wherecondition",col->parameter2,"can't find in tables and selectlist ");
					return false;
				}
				else if(result==1)
				{
					col->parameter1=astablename;
					col->type=t_name_name;
				}
				else
				{
					SQLParse_elog("wherecondition",col->parameter2,"in selectlist is ambiguous");
					return false;
				}
			}
			else
			{
				SQLParse_elog("wherecondition",col->parameter2,"in fromlist is ambiguous");
				return false;
			}
		}break;
		case t_name_name:
		{
			Columns *col=(Columns *)cur;
			char *tablename;
			Node *subnode;
			int fg=fromlist_has_astablename(col->parameter1,rtable,tablename,subnode);
			if(fg==0)
			{
				char str[100]="str is null";
				strcpy(str,col->parameter1);
				strcat(str,".");
				strcat(str,col->parameter2);
				SQLParse_elog("wherecondition",str," can't find ");
				return false;
			}
			else if(fg==1)
			{
				if(Environment::getInstance()->getCatalog()->isAttributeExist(tablename,col->parameter2)==0)
				{
					char str[100]="str is null";
					strcpy(str,col->parameter2);
					strcat(str,"  can't find ");
					SQLParse_elog("wherecondition",col->parameter1,str);
					return false;
				}
			}
			else if(fg==2)
			{
				int fg=subquery_has_column(col->parameter2,subnode);
				if(fg>1||fg==0)
					return false;
			}

		}break;
		case t_query_stmt:
		{
			return semantic_analysis(cur,true);
		}break;
		case t_expr_cal:
		{
			Expr_cal *node=(Expr_cal*)cur;
			if(node->lnext==NULL||node->rnext==NULL)
			return false;
			bool flag=true;
			flag=wherecondition_check(qstmt,node->lnext,rtable);
			if(flag==false)
				return false;
			flag=wherecondition_check(qstmt,node->rnext,rtable);
			return flag;
		}break;
		case t_expr_func:
		{
			Expr_func* node=(Expr_func *)cur;
			if(node->args==NULL)
			return false;
			return wherecondition_check(qstmt,node->args,rtable);
		}break;
		case t_expr_list:
		{
			Expr_list * node=(Expr_list *)cur;
			bool flag=true;
			if(node->data!=NULL)
			{
				flag= wherecondition_check(qstmt,node->data,rtable);
				if(flag==false)
					return false;
			}
			else
			{
				return false;
			}
			if(node->next!=NULL)
			{
				return wherecondition_check(qstmt,node->next,rtable);
			}
		}break;
		default:
		{

		}
	}
	return true;
}
bool wherecondition_analysis(Query_stmt * qstmt,Node *cur,vector<Node *>rtable)
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
				flag=wherecondition_analysis(qstmt,exprnode->lnext,rtable);
				if(flag==false)
					return false;
				flag=wherecondition_analysis(qstmt,exprnode->rnext,rtable);
				if(flag==false)
					return false;
			}
			else
			{
				wherecondition_check(qstmt,cur,rtable);
			}
		}break;
		default:
		{
			wherecondition_check(qstmt,cur,rtable);
		}
	}
	return true;
}
bool havingcondition_analysis(Node *hctree)
{
	return true;
}
bool groupby_analysis(Query_stmt * qstmt,vector<Node *>rtable)
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
				int result=table_has_column(col->parameter2,rtable,astablename);
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
						SQLParse_elog("groupbylist",col->parameter2,"can't find in tables and selectlist ");
						return false;
					}
					else if(result==1)
					{
						col->parameter1=astablename;
						col->type=t_name_name;
					}
					else
					{
						SQLParse_elog("groupbylist",col->parameter2,"in selectlist is ambiguous");
						return false;
					}
				}
				else
				{
					SQLParse_elog("groupbylist",col->parameter2,"in fromlist is ambiguous");
					return false;
				}
			}break;
			case t_name_name:
			{
				Columns *col=(Columns *)(gbexpr->args);
				char *tablename;
				Node *subnode;
				int fg=fromlist_has_astablename(col->parameter1,rtable,tablename,subnode);
				if(fg==0)
				{
					char str[100]="str is null";
					strcpy(str,col->parameter1);
					strcat(str,".");
					strcat(str,col->parameter2);
					SQLParse_elog("groupbylist",str," can't find ");
					return false;
				}
				else if(fg==1)
				{
					if(Environment::getInstance()->getCatalog()->isAttributeExist(tablename,col->parameter2)==0)
					{
						char str[100]="str is null";
						strcpy(str,col->parameter2);
						strcat(str,"  can't find ");
						SQLParse_elog("groupbylist",col->parameter1,str);
						return false;
					}
				}
				else if(fg==2)
				{
					int fg=subquery_has_column(col->parameter2,subnode);
					if(fg>1||fg==0)
						return false;
				}
			}break;
			default:
			{
				SQLParse_elog("groupbylist","there is other type in groupby list","not the colname ");
				return false;
			}
		}
		p=gbexpr->next;
	}
	return true;
}
bool orderby_analysis(Query_stmt * qstmt,vector<Node *>rtable)//与groupby处理相同
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
				int result=table_has_column(col->parameter2,rtable,astablename);
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
						SQLParse_elog("orderbylist",col->parameter2,"can't find in tables and selectlist ");
						return false;
					}
					else if(result==1)
					{
						col->parameter1=astablename;
						col->type=t_name_name;
					}
					else
					{
						SQLParse_elog("orderbylist",col->parameter2,"in selectlist is ambiguous");
						return false;
					}
				}
				else
				{
					SQLParse_elog("orderbylist",col->parameter2,"in fromlist is ambiguous");
					return false;
				}
			}break;
			case t_name_name:
			{
				Columns *col=(Columns *)(gbexpr->args);
				char *tablename;
				Node *subnode;
				int fg=fromlist_has_astablename(col->parameter1,rtable,tablename,subnode);
				if(fg==0)
				{
					char str[100]="str is null";
					strcpy(str,col->parameter1);
					strcat(str,".");
					strcat(str,col->parameter2);
					SQLParse_elog("orderbylist",str," can't find ");
					return false;
				}
				else if(fg==1)
				{
					if(Environment::getInstance()->getCatalog()->isAttributeExist(tablename,col->parameter2)==0)
					{
						char str[100]="str is null";
						strcpy(str,col->parameter2);
						strcat(str,"  can't find ");
						SQLParse_elog("orderbylist",col->parameter1,str);
						return false;
					}
				}
				else if(fg==2)
				{
					int fg=subquery_has_column(col->parameter2,subnode);
					if(fg>1||fg==0)
						return false;
				}
			}break;
			default:
			{
				SQLParse_elog("orderbylist","there is other type in orderby list","not the colname ");
				return false;
			}
		}
		p=gbexpr->next;
	}
	return true;
}

bool semantic_analysis(Node *parsetree,bool issubquery)
{
	if(parsetree==NULL)
	{
		SQLParse_elog("","parsetree is null","");
	}
	switch (parsetree->type)
	{
		case t_query_stmt:
		{
			Query_stmt* qstmt=(Query_stmt *)parsetree;
			vector<Node *>rtable;
			bool flag=true;
			if(qstmt->from_list!=NULL)
			{
				flag=fromlist_analysis(qstmt,parsetree,rtable);
		//		assert(flag);
				if(flag==false)
					return false;
				if(issubquery==true&&rtable.size()>1)
				{
					flag=fromlist_table_is_unique(rtable);
					assert(flag);
					if(flag==false)
						return false;
				}
			}
			if(qstmt->select_list!=NULL)
			{
				flag=selectlist_analysis(qstmt,rtable);
				if(flag==false)
					return false;
			}
			if(qstmt->where_list!=NULL)
			{
				flag=wherecondition_analysis(qstmt,((Where_list*)qstmt->where_list)->next,rtable);
				if(flag==false)
					return false;
			}
			if(qstmt->groupby_list!=NULL)
			{
				flag=groupby_analysis(qstmt,rtable);
				if(flag==false)
					return false;
			}
			if(qstmt->orderby_list!=NULL)
			{
				flag=orderby_analysis(qstmt,rtable);
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


