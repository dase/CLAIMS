#ifndef NODES_H_
#define NODES_H_
#include <stdio.h>
#include <string.h>
#include<malloc.h>
#include<string>
#include<iostream>
#include<set>
#include<vector>
#include<algorithm>
using namespace std;
enum nodetype
{
	t_none,
	t_name,t_uservar,t_name_name,t_stringval,t_intnum,t_approxnum,t_bool,
	t_table,t_column,t_join,t_subquery,t_condition,
	t_query_stmt,t_stmt,
	t_select_list,t_select_opts,t_select_expr,
	t_expr,t_expr_list,t_expr_cal,t_expr_func,t_expr_relation,
	t_from_list,t_from_expr,t_from_table,//25
	t_where_list,t_expr_list_header,
	t_groupby_list,t_groupby_expr,
	t_having_list,
	t_orderby_list,t_orderby_expr,
	t_limit_list,t_limit_expr
};

union dataval
{
	int int_val;
	char * string_val;
	double double_val;
	bool bool_val;
};

struct Node//基本节点
{
	nodetype type;
};
struct Expr//常量表达式
{
	nodetype type;
	dataval data;
	//Node *next;
};

struct Expr_list_header
{
	nodetype type;
	Node * header;
	Node * tail;
};

struct Expr_list
{
	nodetype type;
	Node * data;
	Node *next;
};

struct Columns//列
{
	nodetype type;
	char * parameter1;
	char *parameter2;
	Node * next;
};

struct Expr_cal//计算表达式,二元表达式
{
	nodetype type;
	char * sign;
	char*parameter;
	int cmp;
	Node *lnext,*rnext;
};

struct Expr_func //函数表达式，将is null/exist等判断抽象成函数
{
	nodetype type;
	char * funname;
	Node *args;
	Node * parameter1,*parameter2;//函数中的参数列表，处理between...and.../case...when...then...end等
	Node *next;
};

struct Table //table
{
	nodetype type;
	char * dbname,*tablename,*astablename;
	int issubquery;
	Node *subquery;
	//set<Node *>condition;
	//vector<Node *>wcondition;
	Node *whcdn;
};
struct Join //join
{
	nodetype type;
	int jointype;
	Node *lnext,*rnext,*condition;
};
struct Condition
{
	nodetype type;
	int conditiontype;
	Node * args;
};
struct Subquery //subquery
{
	nodetype type;
	char * querystring;
	Node *next;
};
/*****************select子句开始*********************************************************************************************************************/
struct Query_stmt
{
	nodetype type;
	char * querystring;
	int select_opts;
	Node *select_list;
	Node *from_list;
	Node *where_list;
	Node *groupby_list;
	Node *having_list;
	Node *orderby_list;
	Node *limit_list;
	Node *into_list;
};

/*
select 子句前面有些修饰符

select 子句中每个选项的类型如下：
1.column
2.函数
3.计算式
4.关系式
*/
struct Select_list//选项列表
{
	nodetype type;
	int isall;
	Node * args;
	Node *next;
};


struct Select_expr //选项表达式
{
	nodetype type;
	char *ascolname;
	Node * colname;
};

/*
from子句选项类型：
1.表.别名
2.子查询 .别名
3.join
*/
struct From_list
{
	nodetype type;
	Node * args;
	Node * next;
//	set<Node *>condition;
//	vector<Node *>wcondition;
	Node *whcdn;
};

struct From_expr //from子句
{
	nodetype type;
	char * astablename;
	Node *next;
};

/*
 where子句类型
 1.逻辑表达式
 2.关系表达式
 用expr表示即可
 */

struct Where_list
{
	nodetype type;
	char * wherestring;
	Node *next;
};

/*
groupby子句
1.总体是否有with rollup?
2.每一项可能有asc/desc
3.选项可能是expr,如rand()

*/
struct Groupby_list
{
	nodetype type;
	char * groupbystring;
	Node *next;
	int with_rollup;
};

struct Groupby_expr
{
	nodetype type;
	Node *args;
	int sorttype;
	Node *next;
};

/*
having 子句同where子句
*/

struct Having_list
{
	nodetype type;
	char * havingstring;
	Node *next;
};

/*
orderby子句
{col_name | expr | position} [ASC | DESC] , ...
类似于groupby
*/

struct Orderby_list
{
	nodetype type;
	char * orderbystring;
	Node *next;
};

struct Orderby_expr
{
	nodetype type;
	Node *args;
	char * sorttype;
	Node *next;
};

/*
limit子句
1.num
2.num,num
*/
/*
struct Limit_list//没什么用，因为limit只有一项
{
	nodetype type;
	char * limitstring;
	Node *next;
};*/

struct Limit_expr
{
	nodetype type;
	Node * offset;
	Node *row_count;
};

/*****************select子句结束*********************************************************************************************************************/

/*
extern "C"
{
	int yywrap();
 	int yylex();
 	int yyparse();
 	void emit(char *s, ...);
 	void yyerror(char *s, ...);
 }
*/

int readInputForLexer( char *buffer, int *numBytesRead, int maxBytesToRead );
static int globalReadOffset;


void output(Node * n, int floor);

inline void outputSpace(int f);

struct Node* newExpr(nodetype t, dataval d);

struct Node * newExprList(nodetype t, Node * data, Node * next);
struct Node *newExprlistheader(nodetype type,Node * header,Node * tail);

struct Node * newColumn(nodetype t, char * parameter1, char *parameter2, Node * next);

struct Node * newExpr_cal(nodetype type, char * sign, char *parameter,
	int cmp, Node *lnext, Node *rnext);

struct Node * newExpr_func(nodetype type, char * funname, Node *args,
	Node * parameter1, Node *parameter2, Node *next);

struct Node * newTable(nodetype type, char * dbname, char *tablename,
	char *astablename, int issubquery, Node *subquery);

struct Node * newJoin(nodetype type, int jointype,Node *lnext, Node *rnext, Node *condition);

struct Node * newCondition(nodetype type, int conditiontype, Node * args);

struct Node * newSubquery(nodetype type, char * querystring, Node *next);

struct Node * newQueryStmt(nodetype t, char * querystring, int select_opts,
	Node *select_list, Node *from_list, Node *where_list, Node *groupby_list,
	Node *having_list, Node *orderby_list, Node *limit_list, Node *into_list);

struct Node * newSelectList(nodetype type,int isall,Node * args,Node *next);

struct Node * newSelectExpr(nodetype type, char *ascolname, Node * colname);

struct Node *  newFromList(nodetype type, Node * args, Node *next);

struct Node * newFromExpr( nodetype type, char * astablename, Node *next);

struct Node * newWhereList(nodetype type, char * wherestring, Node *next);

struct Node * newGroupbyList(nodetype type, char * groupbystring, Node *next, int with_rollup);

struct Node * newGroupbyExpr(nodetype type, Node *args, int sorttype, Node *next);

struct Node * newHavingList(nodetype type, char * havingstring, Node *next);

struct Node * newOrderbyList(nodetype type,char * orderbystring, Node *next);

struct Node * newOrderbyExpr(nodetype type, Node *args, char * sorttype, Node *next);

struct Node * newLimitExpr(nodetype type, Node * offset, Node * row_count);

int judgepos(struct Node *args,set<string>*st);

void setwcposition(struct Node *wccur,set<string>*st,struct Node *flcur);

void getwctable(struct Node *cur,set<string>*st);

void solvewc(struct Node * wcexpr,struct Node *fromlist);

void departwc(struct Node *  wherecondition,struct Node * fromlist);

#endif /* NODES_H_ */
