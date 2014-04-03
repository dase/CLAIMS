//============================================================================
// Name        : a.cpp
// Author      : fzh
// Version     :
// Copyright   : Your copyright notice
// Description : Hello World in C++, Ansi-style
//============================================================================

#include"sql_node_struct.h"

/*
t_none,
t_name,t_uservar,t_name_name,t_stringval,t_intnum,t_approxnum,t_bool,
t_table,t_column,t_join,t_subquery,t_condition,
t_query_stmt,
t_select_list,t_select_opts,t_select_expr,
t_expr,t_expr_list,t_expr_cal,t_expr_func,t_expr_relation,
t_from_list,t_from_expr,t_from_table,
t_where_list,
t_groupby_list,t_groupby_expr,
t_having_list,
t_orderby_list,t_orderby_expr,
t_limit_list,t_limit_expr
*/

int judgepos(struct Node *args,set<string>st)//判断fromlist当前节点的值（args）是否是第一次涉及到st中的表，完成定位
{
	Table * tnode=(Table *)(args);
	string tname=tnode->astablename;
	set<string>::iterator it;
	for(it=st.begin();it!=st.end();it++)
	{
		if(*it==tname)//string=char * 的支持
		{
			return 1;
		}
	}
	return 0;
}
void setwcposition(struct Node *wccur,struct Node *flcur,set<string>&st)//在fromlist中定位并把wcexpr放入
{
	switch(flcur->type)
	{
		case t_from_list:
		{
			From_list * node=(From_list *)flcur;
			int judgeresult=judgepos(node->args,st);
			if(judgeresult==0)
			{
				setwcposition(wccur,node->next,st);
			}
			else
			{
				if(st.size()==1)//=1定位在单个table上，>1定位在fromlist上
				{
					setwcposition(wccur,node->args,st);
				}
				else if(st.size()>1)
				{
					Node *p=newExprList(t_expr_list,wccur,NULL);
					Expr_list_header *whcdn=(Expr_list_header *)(node->whcdn);
					if(whcdn->header==NULL)
					{
						whcdn->header=p;
						whcdn->tail=p;
					}
					else
					{
						((Expr_list *)whcdn->tail)->next=p;
						whcdn->tail=p;
					}
				}
				else
				{
					return;
				}
			}
		}break;
		case t_table:
		{
			Table * node=(Table *)flcur;
			Node *p=newExprList(t_expr_list,wccur,NULL);
			Expr_list_header *whcdn=(Expr_list_header *)(node->whcdn);
			if(whcdn->header==NULL)
			{
				whcdn->header=p;
				whcdn->tail=p;
			}
			else
			{
				((Expr_list *)whcdn->tail)->next=p;
				whcdn->tail=p;
			}
		}break;
		case t_join:
		{
			Join * jnode=(Join *)flcur;
			setwcposition(wccur,jnode->rnext,st);
			setwcposition(wccur,jnode->lnext,st);
		}break;
		default:
		{
	//		puts("setwcposition error!");
			return;
		}
	}
}
void getwctable(struct Node *cur,set<string>&st)//cur=wcexpr,获得wcexpr中涉及的表并放入set中
{
	if(cur->type==t_name)
	{
		Expr *node=(Expr *)(cur);
		st.insert((node->data).string_val);//应该获得的是表名？？？
	}
	else if(cur->type==t_name_name)
	{
		Columns *node=(Columns *)(cur);
		st.insert(node->parameter1);//为了保证测试，测试的时候用A.a的格式，就可以保证获得表名
	}
	else if(cur->type==t_expr_cal)
	{
		Expr_cal *node=(Expr_cal*)cur;
		if(node->lnext!=NULL)
		{
			getwctable(node->lnext,st);
		}
		if(node->rnext!=NULL)
		{
			getwctable(node->rnext,st);
		}
	}
	else if(cur->type==t_expr_func)
	{
		Expr_func* node=(Expr_func *)cur;
		if(node->args!=NULL)
		{
			getwctable(node->args,st);
		}
	}
	else if(cur->type==t_expr_list)
	{
		Expr_list *node=(Expr_list *)cur;
		getwctable(node->data,st);
		if(node->next!=NULL)
		getwctable(node->next,st);
	}
	else
	{
		puts("getwctable error!");
	}
	return;
}
void solvewc(struct Node * wcexpr,struct Node *fromlist)//分两个过程，1.获得wcexpr中的表  2.在fromlist中寻找相应的位置并加入wcexpr
{
	set<string>st;
	st.clear();
	getwctable(wcexpr,st);//过程1
	setwcposition(wcexpr,fromlist,st);//过程2

}
void departwc(struct Node * wherecondition,struct  Node * fromlist)//对wherecondition的A范式分解，对每一个wcexpr调用solvewc把wherecondition放在fromlist相应的位置上
{   		

	Node *cur=wherecondition;
	if(cur->type==t_expr_cal)
	{
		struct Expr_cal * node=(struct Expr_cal *)cur;
		if(strcmp((node->sign),"ANDOP")==0)////////////////////////////////
		{
			departwc(node->rnext,fromlist);
			departwc(node->lnext,fromlist);
		}
		else if(strcmp(node->sign,"INS")==0)
		{
			if(node->rnext->type==t_query_stmt)
			{
				Query_stmt *qstmt=(Query_stmt *)(node->rnext);
				if(qstmt->where_list!=NULL)
				{
					Where_list * curt=(Where_list *)(qstmt->where_list);
					Node *cur=(Node *)(curt->next);
					departwc(cur,qstmt->from_list);
				}
			}
			solvewc(cur,fromlist);
		}
		else
		{
			solvewc(cur,fromlist);
		}
	
	}
	else
	{
		solvewc(cur,fromlist);
	}
	return;
}

int setocposition(struct Node *wccur,struct Node *flcur,set<string>&st,vector<Node *>&jcondition)//在fromlist中定位并把wcexpr放入
{
	int judgeresult=judgepos(flcur,st);
	if(judgeresult==0)
		return 0;
	switch(flcur->type)
	{
		case t_table:
		{
			if(st.size()==1)
			{
				Table * node=(Table *)flcur;
				Node *p=newExprList(t_expr_list,wccur,NULL);
				Expr_list_header *whcdn=(Expr_list_header *)(node->whcdn);
				if(whcdn->header==NULL)
				{
					whcdn->header=p;
					whcdn->tail=p;
				}
				else
				{
					((Expr_list *)whcdn->tail)->next=p;
					whcdn->tail=p;
				}
			}
			else
			{
				jcondition.push_back(wccur);
			}
		}break;
		default:
		{
	//		puts("setwcposition error!");
		}
	}
	return 1;
}

int set_join_condition_on_joinnode(Join * joinnode,Node *condition,vector<Node *>&jcondition)
{
	/*int flag=set_join_condition(joinnode->lnext,condition);
	if(flag==0)
	{
		flag=set_join_condition(joinnode->rnext,condition);
		if(flag==0)
		{
			return 0;
		}
	}*/
	set<string>st;
	st.clear();
	getwctable(condition,st);

	int fg=setocposition(condition,joinnode->lnext,st,jcondition);
	if(fg==0)
	{
		fg=setocposition(condition,joinnode->rnext,st,jcondition);
		return fg;
	}
	return 1;
}
int depart_join_condition(Join *joinnode,Node * condition,vector<Node *>&jcondition)
{
	Node *cur=condition;
	if(cur->type==t_expr_cal)
	{
		struct Expr_cal * node=(struct Expr_cal *)cur;
		if(strcmp((node->sign),"ANDOP")==0)////////////////////////////////
		{
			depart_join_condition(joinnode,node->lnext,jcondition);
			depart_join_condition(joinnode,node->rnext,jcondition);
		}
		else if(strcmp(node->sign,"INS")==0)
		{
			if(node->rnext->type==t_query_stmt)
			{
				Query_stmt *qstmt=(Query_stmt *)(node->rnext);
				if(qstmt->where_list!=NULL)
				{
					Where_list * curt=(Where_list *)(qstmt->where_list);
					Node *cur=(Node *)(curt->next);
					departwc(cur,qstmt->from_list);
				}
				if(qstmt->from_list!=NULL)
				{
					int fg=solve_join_condition(qstmt->from_list);
					if(fg==0)
						return 0;
				}
			}
			set_join_condition_on_joinnode(joinnode,cur,jcondition);
		}
		else
		{
			set_join_condition_on_joinnode(joinnode,cur,jcondition);
		}

	}
	else
	{
		set_join_condition_on_joinnode(joinnode,cur,jcondition);
	}
	return 1;
}

int solve_join_condition(Node * fromnode)
{
	for(Node *p=fromnode;p!=NULL;)
	{
		From_list * fnode=(From_list *)p;
		switch(fnode->args->type)
		{
			case t_join:
			{
				Join * node=(Join *)fnode->args;
				if(node->condition!=NULL)
				{
					Condition * cnode=(Condition *)node->condition;
					if(cnode->conditiontype==1)
					{
						vector<Node *>jcondition;
						jcondition.clear();
						depart_join_condition(node,cnode->args,jcondition);
						Node *p=newExprList(t_expr_list,jcondition[0],NULL);
						node->condition=p;
						for(int i=1;i<jcondition.size();i++)
						{
							Node *q=newExprList(t_expr_list,jcondition[i],NULL);
							((Expr_list *)p)->next=q;
							p=q;
						}
					}
					else
					{

					}
				}
			}break;
			default:
			{

			}
		}
		p=fnode->next;
	}
	return 1;
}

