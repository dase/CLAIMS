//============================================================================
// Name        : a.cpp
// Author      : fzh
// Version     :
// Copyright   : Your copyright notice
// Description : Hello World in C++, Ansi-style
//============================================================================

#include"sql_node_struct.h"
set<string>st;
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
int judgepos(struct Node *args)//判断fromlist当前节点的值（args）是否是第一次涉及到st中的表，完成定位
{
	Table * tnode=(Table *)(args);
	string tname=tnode->tablename;
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
void setwcposition(struct Node *wccur,struct Node *flcur)//在fromlist中定位并把wcexpr放入
{
	if(flcur->type==t_from_list)
	{
		From_list * node=(From_list *)flcur;
		int judgeresult=judgepos(node->args);
		if(judgeresult==0)
		{
			setwcposition(wccur,node->next);
		}
		else
		{
			if(st.size()==1)
			{
				//Table * tnode=(Table *)(node->args);
			//	tnode->condition=wccur;
				//tnode->wcondition.push_back(wccur);
				setwcposition(wccur,node->args);
			}
			else if(st.size()>1)
			{
				//node->condition=wccur;
				//node->wcondition.push_back(wccur);
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
	}
	else if(flcur->type==t_table)
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
//		puts("setwcposition error!");
		return;
	}
}
void getwctable(struct Node *cur)//cur=wcexpr,获得wcexpr中涉及的表并放入set中
{
//	cout<<"wc "<<cur->type<<"  "<<(st.size())<<endl;
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
			getwctable(node->lnext);
		}
		if(node->rnext!=NULL)
		{
			getwctable(node->rnext);
		}
	}
	else if(cur->type==t_expr_func)
	{
		Expr_func* node=(Expr_func *)cur;
		if(node->args!=NULL)
		{
			getwctable(node->args);
		}
	}
	else
	{
	//	puts("getwctable error!");
	}
	return;
}
void solvewc(struct Node * wcexpr,struct Node *fromlist)//分两个过程，1.获得wcexpr中的表  2.在fromlist中寻找相应的位置并加入wcexpr
{
//	puts("0");
	st.clear();
//	puts("1");
	getwctable(wcexpr);//过程1
//	puts("2");
	setwcposition(wcexpr,fromlist);//过程2
//	puts("3");
}
void departwc(struct Node * wherecondition,struct  Node * fromlist)//对wherecondition的A范式分解，对每一个wcexpr调用solvewc把wherecondition放在fromlist相应的位置上
{   		

		Node *cur=wherecondition;
//		cout<<cur->type<<endl;
	if(cur->type==t_expr_cal)
	{
		struct Expr_cal * node=(struct Expr_cal *)cur;
//		cout<<node->sign<<endl;
		if(strcmp((node->sign),"ANDOP")==0)////////////////////////////////
		{
		//	puts("bb");
			departwc(node->rnext,fromlist);
			departwc(node->lnext,fromlist);
		//	puts("bb");
		}
		else
		{
		//	puts("cc");
			solvewc(cur,fromlist);
		//	puts("cc");
		}
	
	}
	else
	{
	//	puts("dd");
		solvewc(cur,fromlist);
	//	puts("dd");
	}


//	free(wherecondition);
	return;
}


