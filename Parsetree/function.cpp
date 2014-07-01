#include <iostream>
#include <malloc.h>
#include <stdlib.h>
#include <stdarg.h>
#include <vector>
#include "sql_node_struct.h"
using namespace std;

extern Node * parsetreeroot;
extern char globalInputText[10000];
extern int globalReadOffset;
extern vector<Node*> NodePointer;	// 2014-4-2---存放节点指针的数组改为vector---by Yu

inline void insertNodePointer(Node *a)	// 2014-3-7---增加将节点指针存入指针数组的函数---by余楷
{
	NodePointer.push_back(a);	// 2014-4-2---存放节点指针的数组改为vector---by Yu
}

// 2014-3-4---增加新建语句列表函数---by余楷
struct Node *newStmt(nodetype t, Node *list, Node *newNode)
{
	struct Stmt *a= (struct Stmt *)malloc(sizeof(struct Stmt));
	if(!a)
	{
		yyerror("out of space!");
		exit(0);
	}

	a->type = t;
	a->data = newNode;
	a->next = NULL;
	a->last = (Node *)a;

	if (list != NULL)
	{
		((Stmt *)(((Stmt *)list)->last))->next = (Node *)a;
		((Stmt *)list)->last = (Node *)a;
		return (Node *)list;
	}
	//cout<<"newStmt is created"<<endl;
	insertNodePointer((Node*)a);	// 2014-3-7---将节点指针存入指针数组---by余楷
	return (struct Node *)a;
}

// 2014-4-14---the old version is newExpr(nodetype t, dataval d)---by Yu
struct Node * newExpr(nodetype t, char *d)
{
	struct Expr * a= (struct Expr *)malloc(sizeof(struct Expr));
	if(!a)
	{
		yyerror("out of space!");
		exit(0);
	}

	a->type = t;
//	cout<<d.string_val<<endl;
//    cout<<"NexExpr create begin!!!!"<<endl;
	if(t == t_stringval) // 2014-3-25---输入若为字符串，去除首位的引号
	{
//		if(d.string_val[0]=='\''||d.string_val[0]=='\"')
//		{
//			int slen=strlen(d.string_val);
//			d.string_val[slen-1]='\0';
//			for(int i=1;i<slen;i++)
//			{
//				d.string_val[i-1]=d.string_val[i];
//			}
//		}
	}
	a->data = d;
//	if(t == t_stringval)	// 2014-3-25---输入若为字符串，去除首尾的引号	//2014-4-2---将这部分工作放在词法识别阶段进行，见sql.l
//	{
//		strncpy(a->data, d+1, strlen(d)-2);
//		a->data[strlen(d)-2] = '\0';
//	}

	//cout<<"newExpr is created"<<endl;
	insertNodePointer((Node*)a);	// 2014-3-7---将节点指针存入指针数组---by余楷
	return (struct Node *)a;
}

struct Node * newExprList(nodetype t, Node * data, Node * next)
{
	struct Expr_list * a= (struct Expr_list *)malloc(sizeof(struct Expr_list));
	if(!a)
	{
		yyerror("out of space!");
		exit(0);
	}

	a->type = t;
	a->data = data;
	a->next = next;
	//cout<<"newExpr is created"<<endl;
	insertNodePointer((Node*)a);	// 2014-3-7---将节点指针存入指针数组---by余楷
	return (struct Node *)a;
}

struct Node * newColumn(nodetype t, char * parameter1, char *parameter2, Node * next)
{
	struct Columns * a= (struct Columns *)malloc(sizeof(struct Columns));
	if(!a)
	{
		yyerror("out of space!");
		exit(0);
	}

	a->type = t;
	a->parameter1 = parameter1;
	a->parameter2 = parameter2;
	a->next = next;
	//cout<<"newColumn is created"<<endl;

	insertNodePointer((Node*)a);	// 2014-3-7---将节点指针存入指针数组---by余楷
	return (struct Node *)a;
}

struct Node * newExprCal(nodetype type, char * sign, char *parameter,
	int cmp, Node *lnext, Node *rnext)
{
	struct Expr_cal * a= (struct Expr_cal *)malloc(sizeof(struct Expr_cal));
	if(!a)
	{
		yyerror("out of space!");
		exit(0);
	}
	a->str="";
	a->type = type;
	a->sign = sign;
	a->parameter = parameter;
	a->cmp = cmp;
	a->lnext = lnext;
	a->rnext = rnext;
	//cout<<"newExprCal is created"<<endl;

	insertNodePointer((Node*)a);	// 2014-3-7---将节点指针存入指针数组---by余楷
	return (struct Node *)a;
}

struct Node * newExprFunc(nodetype type, char * funname, Node *args,
	Node * parameter1, Node *parameter2, Node *next)
{
	struct Expr_func * a= (struct Expr_func *)malloc(sizeof(struct Expr_func));
	if(!a) {
		yyerror("out of space!");
		exit(0);
	}
	a->str="";
	a->type = type;
	a->funname = funname;
	a->args = args;
	a->parameter1 = parameter1;
	a->parameter2 = parameter2;
	a->next = next;
	//cout<<"newExprFunc is created"<<endl;

	insertNodePointer((Node*)a);	// 2014-3-7---将节点指针存入指针数组---by余楷
	return (struct Node *)a;
}

struct Node *newExprlistheader(nodetype type,Node * header,Node * tail)
{
	Expr_list_header *node=( Expr_list_header *)malloc(sizeof( Expr_list_header));
	node->type=t_expr_list_header;
	node->header=header;
	node->tail=tail;
	return (struct Node *)node;
}

struct Node * newTable(nodetype type, char * dbname, char *tablename,
	char *astablename, int issubquery, Node *subquery)
{
	struct Table * a= (struct Table *)malloc(sizeof(struct Table)+10);
	if(!a) {
		yyerror("out of space!");
		exit(0);
	}
	
	a->type = type;
	a->dbname = dbname;
	a->tablename = tablename;
	a->astablename = astablename;
	a->issubquery = issubquery;
	a->subquery = subquery;

	a->whcdn=newExprlistheader(t_expr_list_header,NULL,NULL);
	


	insertNodePointer((Node*)a);	// 2014-3-7---将节点指针存入指针数组---by余楷
	return (struct Node *)a;
}
struct Node * newJoin(nodetype type, int jointype,Node *lnext, Node *rnext, Node *condition)
{
	struct Join * a= (struct Join *)malloc(sizeof(struct Join));
	if(!a) {
		yyerror("out of space!");
		exit(0);
	}

	a->type = type;
	a->jointype = jointype;
	a->lnext = lnext;
	a->rnext = rnext;
	a->condition = condition;
	//cout<<"Join is created"<<endl;

	insertNodePointer((Node*)a);	// 2014-3-7---将节点指针存入指针数组---by余楷
	return (struct Node *)a;
}

struct Node * newCondition(nodetype type, int conditiontype, Node * args)
{
	struct Condition * a= (struct Condition *)malloc(sizeof(struct Condition));
	if(!a) {
		yyerror("out of space!");
		exit(0);
	}

	a->type = type;
	a->conditiontype = conditiontype;
	a->args = args;
	//cout<<"Condition is created"<<endl;

	insertNodePointer((Node*)a);	// 2014-3-7---将节点指针存入指针数组---by余楷
	return (struct Node *)a;
}

struct Node * newSubquery(nodetype type, char * querystring, Node *next)
{
	struct Subquery * a= (struct Subquery *)malloc(sizeof(struct Subquery));
	if(!a) {
		yyerror("out of space!");
		exit(0);
	}

	a->type = type;
	a->querystring = querystring;
	a->next = next;
	//cout<<"Subquery is created"<<endl;

	insertNodePointer((Node*)a);	// 2014-3-7---将节点指针存入指针数组---by余楷
	return (struct Node *)a;
}

struct Node * newQueryStmt(nodetype t, char * querystring, int select_opts,
	Node *select_list, Node *from_list, Node *where_list, Node *groupby_list,
	Node *having_list, Node *orderby_list, Node *limit_list, Node *into_list)
{
	struct Query_stmt * a= (struct Query_stmt *)malloc(sizeof(struct Query_stmt));
	if(!a)
	{
		yyerror("out of space!");
		exit(0);
	}

	a->type = t;
	a->querystring = querystring;
	a->select_opts = select_opts;
	a->select_list = select_list;
	a->from_list = from_list;
	a->where_list = where_list;
	a->groupby_list = groupby_list;
	a->having_list = having_list;
	a->orderby_list = orderby_list;
	a->limit_list = limit_list;
	a->into_list = into_list;

	//cout<<"Query_stmt is created"<<endl;

	insertNodePointer((Node*)a);	// 2014-3-7---将节点指针存入指针数组---by余楷
	return (struct Node *)a;
}

struct Node * newSelectList(nodetype type,int isall,Node * args,Node *next)
{
	struct Select_list * a= (struct Select_list *)malloc(sizeof(struct Select_list));
	if(!a)
	{
		yyerror("out of space!");
		exit(0);
	}
	a->type = type;
	a->isall = isall;
	a->args = args;
	a->next = next;

	//cout<<"Select_list is created"<<endl;

	insertNodePointer((Node*)a);	// 2014-3-7---将节点指针存入指针数组---by余楷
	return (struct Node *)a;
}

struct Node * newSelectExpr(nodetype type, char *ascolname, Node * colname)
{
	struct Select_expr * a= (struct Select_expr *)malloc(sizeof(struct Select_expr));
	if(!a)
	{
		yyerror("out of space!");
		exit(0);
	}
	a->type = type;
	a->ascolname = ascolname;
	a->colname = colname;

	//cout<<"Select_expr is created"<<endl;

	insertNodePointer((Node*)a);	// 2014-3-7---将节点指针存入指针数组---by余楷
	return (struct Node *)a;
};


struct Node *  newFromList(nodetype type, Node * args, Node *next)
{
	struct From_list * a= (struct From_list *)malloc(sizeof(struct From_list));
	if(!a)
	{
		yyerror("out of space!");
		exit(0);
	}
	a->type = type;
	a->args = args;
	a->next = next;
//	a->condition.clear();
	//a->wcondition.clear();
	a->whcdn=newExprlistheader(t_expr_list_header,NULL,NULL);
	//cout<<"From_list is created"<<endl;

	insertNodePointer((Node*)a);	// 2014-3-7---将节点指针存入指针数组---by余楷
	return (struct Node *)a;
};

struct Node * newFromExpr( nodetype type, char * astablename, Node *next)
{
	struct From_expr * a= (struct From_expr *)malloc(sizeof(struct From_expr));
	if(!a)
	{
		yyerror("out of space!");
		exit(0);
	}
	a->type = type;
	a->astablename = astablename;
	a->next = next;

	//cout<<"From_expr is created"<<endl;

	insertNodePointer((Node*)a);	// 2014-3-7---将节点指针存入指针数组---by余楷
	return (struct Node *)a;
};

struct Node * newWhereList(nodetype type, char * wherestring, Node *next)
{
	struct Where_list * a= (struct Where_list *)malloc(sizeof(struct Where_list));
	if(!a)
	{
		yyerror("out of space!");
		exit(0);
	}
	a->type = type;
	a->wherestring = wherestring;
	a->next = next;

	//cout<<"Where_list is created"<<endl;

	insertNodePointer((Node*)a);	// 2014-3-7---将节点指针存入指针数组---by余楷
	return (struct Node *)a;
};

struct Node * newGroupbyList(nodetype type, char * groupbystring, Node *next, int with_rollup)
{
	Groupby_list * a= (Groupby_list *)malloc(sizeof(Groupby_list));
	if(!a)
	{
		yyerror("out of space!");
		exit(0);
	}
	a->type = type;
	a->groupbystring = groupbystring;
	a->next = next;
	a->with_rollup = with_rollup;

	//cout<<"Groupby_list is created"<<endl;

	insertNodePointer((Node*)a);	// 2014-3-7---将节点指针存入指针数组---by余楷
	return (struct Node *)a;
};

struct Node * newGroupbyExpr(nodetype type, Node *args, int sorttype, Node *next)
{
	Groupby_expr * a= (Groupby_expr *)malloc(sizeof(Groupby_expr));
	if(!a)
	{
		yyerror("out of space!");
		exit(0);
	}
	a->type = type;
	a->args = args;
	a->sorttype = sorttype;
	a->next = next;

	//cout<<"Groupby_expr is created"<<endl;

	insertNodePointer((Node*)a);	// 2014-3-7---将节点指针存入指针数组---by余楷
	return (struct Node *)a;
};

struct Node * newHavingList(nodetype type, char * havingstring, Node *next)
{
	Having_list * a= (Having_list *)malloc(sizeof( Having_list));
	if(!a)
	{
		yyerror("out of space!");
		exit(0);
	}
	a->type = type;
	a->havingstring = havingstring;
	a->next = next;

	//cout<<"Having_list is created"<<endl;

	insertNodePointer((Node*)a);	// 2014-3-7---将节点指针存入指针数组---by余楷
	return (struct Node *)a;
};


struct Node * newOrderbyList(nodetype type,char * orderbystring, Node *next)
{
	Orderby_list * a= ( Orderby_list *)malloc(sizeof( Orderby_list));
	if(!a)
	{
		yyerror("out of space!");
		exit(0);
	}
	a->type = type;
	a->orderbystring = orderbystring;
	a->next = next;

	//cout<<"Orderby_list is created"<<endl;

	insertNodePointer((Node*)a);	// 2014-3-7---将节点指针存入指针数组---by余楷
	return (struct Node *)a;
};

struct Node * newOrderbyExpr(nodetype type, Node *args, char * sorttype, Node *next)
{
	struct Orderby_expr * a= (struct Orderby_expr *)malloc(sizeof(struct Orderby_expr));
	if(!a)
	{
		yyerror("out of space!");
		exit(0);
	}
	a->type = type;
	a->args = args;
	a->sorttype = sorttype;
	a->next = next;

	//cout<<"Orderby_expr is created"<<endl;

	insertNodePointer((Node*)a);	// 2014-3-7---将节点指针存入指针数组---by余楷
	return (struct Node *)a;
};

struct Node * newLimitExpr(nodetype type, Node * offset, Node * row_count)
{
	struct Limit_expr * a= (struct Limit_expr *)malloc(sizeof(struct Limit_expr));
	if(!a)
	{
		yyerror("out of space!");
		exit(0);
	}
	a->type = type;
	a->offset = offset;
	a->row_count = row_count;

	//cout<<"Limit_expr is created"<<endl;

	insertNodePointer((Node*)a);	// 2014-3-7---将节点指针存入指针数组---by余楷
	return (Node *)a;
};

//2014-4-16---add---by Yu
Node* newInsertStmt(int insert_opt, char *tablename, Node *col_list,
		Node *insert_val_list, Node *insert_assign_list, Node *insert_assign_list_from_set, Node *select_stmt)
{
	Insert_stmt *a= (Insert_stmt *)malloc(sizeof(Insert_stmt));
	if(!a)
	{
		yyerror("out of space!");
		exit(0);
	}
	a->type = t_insert_stmt;
	a->insert_opt = insert_opt;
	a->tablename = tablename;
	a->col_list = col_list;
	a->insert_val_list = insert_val_list;
	a->insert_assign_list = insert_assign_list;
	a->insert_assign_list_from_set = insert_assign_list_from_set;
	a->select_stmt = select_stmt;

	//cout<<"Insert_stmt is created"<<endl;

	insertNodePointer((Node*)a);	// 2014-3-7---将节点指针存入指针数组---by余楷
	return (Node*)a;
};

//2014-4-17---add---by Yu
Node* newInsertValueList(Node *insert_vals, Node *next)
{
	Insert_val_list *a= (Insert_val_list *)malloc(sizeof(Insert_val_list));
	if(!a)
	{
		yyerror("out of space!");
		exit(0);
	}
	a->type = t_insert_val_list;
	a->insert_vals = insert_vals;
	a->next = next;

	//cout<<"Insert_val_list is created"<<endl;


	insertNodePointer((Node*)a);	// 2014-3-7---将节点指针存入指针数组---by余楷
	return (Node*)a;
};

//2014-4-17---add---by Yu
Node* newInsertVals(int value_type, Node *expr, Node *next)
{
	Insert_vals *a= (Insert_vals *)malloc(sizeof(Insert_vals));
	if(!a)
	{
		yyerror("out of space!");
		exit(0);
	}
	a->type = t_insert_vals;
	a->value_type = value_type;
	a->expr = expr;
	a->next = next;

	//cout<<"Insert_vals is created"<<endl;
	insertNodePointer((Node*)a);
	return (Node*)a;
};

//2014-4-17---add---by Yu
Node* newInsertAssignList(char *col_name, int value_type, Node *expr, Node *next)
{
	Insert_assign_list *a= (Insert_assign_list *)malloc(sizeof(Insert_assign_list));
	if(!a)
	{
		yyerror("out of space!");
		exit(0);
	}
	a->type = t_insert_assign;
	a->col_name = col_name;
	a->value_type = value_type;
	a->expr = expr;
	a->next = next;

	//cout<<"Insert_assign_list is created"<<endl;
	insertNodePointer((Node*)a);
	return (Node*)a;
};


/*************************** DDL语句开始 ********************************/

	/*** 		create 语句			***/
struct Node * newCreateDatabaseStmt(nodetype type, int create_type, int check, char * name)
{
	struct Create_database_stmt * a= (Create_database_stmt *)malloc(sizeof(Create_database_stmt));
	if(!a)
	{
		yyerror("out of space!");
		exit(0);
	}
	a->type = type;
	a->create_type = create_type;
	a->check = check;
	a->name = name;

	//cout<<"Create_database_stmt is created"<<endl;

	insertNodePointer((Node*)a);	// 2014-3-7---将节点指针存入指针数组---by余楷
	return (struct Node *)a;
};

struct Node * newCreateTableStmt
( nodetype type, int create_type, int check, char * name1, char * name2, Node * list, Node * select_stmt)
{
	struct Create_table_stmt * a= (struct Create_table_stmt *)malloc(sizeof(struct Create_table_stmt));
	if(!a)
	{
		yyerror("out of space!");
		exit(0);
	}
	a->type = type;
	a->create_type = create_type;
	a->check = check;
	a->name1 = name1;
	a->name2 = name2;
	a->list = list;
	a->select_stmt = select_stmt;

	//cout<<"Create_table_stmt is created"<<endl;

	insertNodePointer((Node*)a);	// 2014-3-7---将节点指针存入指针数组---by余楷
	return (struct Node *)a;
};

struct Node * newCreateColList(nodetype type, Node * data, Node * next)
{
	struct Create_col_list * a= (struct Create_col_list *)malloc(sizeof(struct Create_col_list));
	if(!a)
	{
		yyerror("out of space!");
		exit(0);
	}
	a->type = type;
	a->data = data;
	a->next = next;

	//cout<<"Create_col_list is created"<<endl;

	insertNodePointer((Node*)a);	// 2014-3-7---将节点指针存入指针数组---by余楷
	return (struct Node *)a;
};

struct Node * newCreateDef(nodetype type, int deftype, char * name,
		Node * datatype,  Node * col_atts, Node * col_list)
{	// 2-18---增加name属性---by余楷
	struct Create_def * a= (struct Create_def *)malloc(sizeof(struct Create_def));
	if(!a)
	{
		yyerror("out of space!");
		exit(0);
	}
	a->type = type;
	a->deftype = deftype;
	a->name = name;		// 2-18---增加name属性---by余楷
	a->datatype = datatype;
	a->col_atts = col_atts;
	a->col_list = col_list;

	//cout<<"Create_def is created"<<endl;

	insertNodePointer((Node*)a);	// 2014-3-7---将节点指针存入指针数组---by余楷
	return (struct Node *)a;
};

struct Node * newColumnAtts(nodetype type, int datatype, int num1, double num2, char *s, Node * col_list)
{
	Column_atts * a= (Column_atts *)malloc(sizeof(Column_atts));
	if(!a)
	{
		yyerror("out of space!");
		exit(0);
	}
	a->type = type;
	a->datatype = datatype;
	a->num1 = num1;
	a->num2 = num2;
	a->s = s;
	a->col_list = col_list;

	//cout<<"Column_atts is created"<<endl;

	insertNodePointer((Node*)a);	// 2014-3-7---将节点指针存入指针数组---by余楷
	return (struct Node *)a;
};

struct Node * newCreateSelectStmt(nodetype type, int ignore_replace, int temporary, Node * select_stmt)
{
	Create_select_stmt * a= (Create_select_stmt *)malloc(sizeof(Create_select_stmt));
	if(!a)
	{
		yyerror("out of space!");
		exit(0);
	}
	a->type = type;
	a->ignore_replace = ignore_replace;
	a->temporary = temporary;
	a->select_stmt = select_stmt;

	//cout<<"Create_select_stmt is created"<<endl;

	insertNodePointer((Node*)a);	// 2014-3-7---将节点指针存入指针数组---by余楷
	return (struct Node *)a;
};

// 2014-2-24---增加该结构体---by余楷

struct Node *newCreateProjectionStmt(nodetype type, char *tablename, Node *column_list,
		int partition_num, char *partition_attribute_name)
{
	Create_projection_stmt * a= (Create_projection_stmt *)malloc(sizeof(Create_projection_stmt));
	if(!a)
	{
		yyerror("out of space!");
		exit(0);
	}
	a->type = type;
	a->tablename = tablename;
	a->column_list = column_list;
	a->partition_num = partition_num;
	a->partition_attribute_name = partition_attribute_name;

	//cout<<"Create_projection_stmt is created"<<endl;

	insertNodePointer((Node*)a);	// 2014-3-7---将节点指针存入指针数组---by余楷
	return (struct Node *)a;
};

	/*** 		do 语句				***/
struct Node * newDoStmt(nodetype type, Node * data)
{
	struct Do_stmt * a= (struct Do_stmt *)malloc(sizeof(struct Do_stmt));
	if(!a)
	{
		yyerror("out of space!");
		exit(0);
	}
	a->type = type;
	a->data = data;

	//cout<<"Do_stmt is created"<<endl;

	insertNodePointer((Node*)a);	// 2014-3-7---将节点指针存入指针数组---by余楷
	return (struct Node *)a;
};

	/*** 		truncate 语句			***/
struct Node * newTruncateStmt(nodetype type, char * name)
{
	struct Truncate_stmt * a= (struct Truncate_stmt *)malloc(sizeof(struct Truncate_stmt));
	if(!a)
	{
		yyerror("out of space!");
		exit(0);
	}
	a->type = type;
	a->name = name;

	//cout<<"Truncate_stmt is created"<<endl;

	insertNodePointer((Node*)a);	// 2014-3-7---将节点指针存入指针数组---by余楷
	return (struct Node *)a;
};

	/*** 		alter 语句			***/	// 2-19---把函数名改为newAlterDatabaseStmt---by余楷
struct Node * newAlterDatabaseStmt(nodetype type, int createtype, char * name, Node* opt)

{
	Alterdatabase_stmt * a= (Alterdatabase_stmt *)malloc(sizeof( Alterdatabase_stmt));
	if(!a)
	{
		yyerror("out of space!");
		exit(0);
	}
	a->type = type;
	a->createtype = createtype;
	a->name = name;
	a->opt = opt;

	//cout<<"Alterdatabase_stmt is created"<<endl;

	insertNodePointer((Node*)a);	// 2014-3-7---将节点指针存入指针数组---by余楷
	return (struct Node *)a;
};

struct Node * newOptCsc(nodetype type, int datatype, char * s1, char * s2)
{
	Opt_csc * a= (Opt_csc *)malloc(sizeof( Opt_csc));
	if(!a)
	{
		yyerror("out of space!");
		exit(0);
	}
	a->type = type;
	a->datatype = datatype;
	a->s1 = s1;
	a->s2 = s2;

	//cout<<"Opt_csc is created"<<endl;

	insertNodePointer((Node*)a);	// 2014-3-7---将节点指针存入指针数组---by余楷
	return (struct Node *)a;
};

struct Node * newAlterTableStmt(nodetype type, int isignore, char * name, Node * parameter)
{
	Altertable_stmt * a= (Altertable_stmt *)malloc(sizeof( Altertable_stmt));
	if(!a)
	{
		yyerror("out of space!");
		exit(0);
	}
	a->type = type;
	a->isignore = isignore;
	a->name = name;
	a->parameter = parameter;

	//cout<<"Altertable_stmt is created"<<endl;

	insertNodePointer((Node*)a);	// 2014-3-7---将节点指针存入指针数组---by余楷
	return (struct Node *)a;
};

struct Node * newAlterDef (nodetype type, int altertype, char * name1, char * name2, 
	int datatype, int coltype, Node * parameter, Node * next)
{
	Alter_def * a= (Alter_def *)malloc(sizeof( Alter_def));
	if(!a)
	{
		yyerror("out of space!");
		exit(0);
	}
	a->type = type;
	a->altertype = altertype;
	a->name1 = name1;
	a->name2 = name2;
	a->datatype = datatype;
	a->coltype = coltype;
	a->parameter = parameter;
	a->next = next;

	//cout<<"Alter_def is created"<<endl;

	insertNodePointer((Node*)a);	// 2014-3-7---将节点指针存入指针数组---by余楷
	return (struct Node *)a;
};

struct Node * newDatatype (nodetype type, int datatype, Node* length, int opt_uz,
	Node * opt_csc, int isbinary, Node * enum_list)
{
	Datatype * a= (Datatype *)malloc(sizeof(Datatype));
	if(!a)
	{
		yyerror("out of space!");
		exit(0);
	}
	a->type = type;
	a->datatype = datatype;
	a->length = length;
	a->opt_uz = opt_uz;
	a->opt_csc = opt_csc;
	a->isbinary = isbinary;
	a->enum_list = enum_list;

	//cout<<"Datatype is created"<<endl;

	insertNodePointer((Node*)a);	// 2014-3-7---将节点指针存入指针数组---by余楷
	return (struct Node *)a;
};

struct Node * newLength (nodetype type, int data1, int data2)
{
	Length * a= (Length *)malloc(sizeof(Length));
	if(!a)
	{
		yyerror("out of space!");
		exit(0);
	}
	a->type = type;
	a->data1 = data1;
	a->data2 = data2;

	//cout<<"Length is created"<<endl;

	insertNodePointer((Node*)a);	// 2014-3-7---将节点指针存入指针数组---by余楷
	return (struct Node *)a;
};

struct Node * newEnumList (nodetype type, char * s, Node * next)
{
	enum_list * a= (enum_list *)malloc(sizeof(enum_list));
	if(!a)
	{
		yyerror("out of space!");
		exit(0);
	}
	a->type = type;
	a->s = s;
	a->next = next;

	//cout<<"enum_list is created"<<endl;

	insertNodePointer((Node*)a);	// 2014-3-7---将节点指针存入指针数组---by余楷
	return (struct Node *)a;
};

// 2014-3-24---增加---by Yu
struct Node* newCreateIndex(nodetype type, int index_att, char* index_name, int index_type, char* table_name, Node* index_col_name)
{
	Createindex_stmt * a= (Createindex_stmt *)malloc(sizeof(Createindex_stmt));
	if(!a)
	{
		yyerror("out of space!");
		exit(0);
	}
	a->type = type;
	a->index_att = index_att;
	a->index_name = index_name;
	a->index_type = index_type;
	a->table_name = table_name;
	a->index_col_name = index_col_name;

	//cout<<"Createindex_stmt is created"<<endl;

	insertNodePointer((Node*)a);	// 2014-3-7---将节点指针存入指针数组---by余楷
	return (struct Node *)a;
}

// 2014-3-24---增加---by Yu
struct Node* newIndexColList(nodetype type, char * name, Node* length, int asc_desc, Node * next)
{
	Index_col_list * a= (Index_col_list *)malloc(sizeof(Index_col_list));
	if(!a)
	{
		yyerror("out of space!");
		exit(0);
	}
	a->type = type;
	a->name = name;
	a->length = length;
	a->asc_desc = asc_desc;
	a->next = next;

	//cout<<"Index_col_list is created"<<endl;

	insertNodePointer((Node*)a);	// 2014-3-7---将节点指针存入指针数组---by余楷
	return (struct Node *)a;
}

// 2014-3-24---增加---by Yu
struct Node* newDropIndex(nodetype type, char* index_name, char* table_name)
{
	Dropindex_stmt * a= (Dropindex_stmt *)malloc(sizeof(Dropindex_stmt));
	if(!a)
	{
		yyerror("out of space!");
		exit(0);
	}
	a->type = type;
	a->index_name = index_name;
	a->table_name = table_name;

	//cout<<"Dropindex_stmt is created"<<endl;

	insertNodePointer((Node*)a);	// 2014-3-7---将节点指针存入指针数组---by余楷
	return (struct Node *)a;
}

// 2014-3-24---增加---by Yu
struct Node* newDropDatabase(nodetype type, int drop_type, int check, char* name)
{
	Dropdatabase_stmt * a= (Dropdatabase_stmt *)malloc(sizeof(Dropdatabase_stmt));
	if(!a)
	{
		yyerror("out of space!");
		exit(0);
	}
	a->type = type;
	a->droptype = drop_type;
	a->check = check;
	a->name = name;

	//cout<<"Dropdatabase_stmt is created"<<endl;

	insertNodePointer((Node*)a);	// 2014-3-7---将节点指针存入指针数组---by余楷
	return (struct Node *)a;
}

// 2014-3-24---增加---by Yu
struct Node* newDropTable(nodetype type, int is_temp, int is_check, int opt_rc, Node * table_list)
{
	Droptable_stmt * a= (Droptable_stmt *)malloc(sizeof(Droptable_stmt));
	if(!a)
	{
		yyerror("out of space!");
		exit(0);
	}
	a->type = type;
	a->is_temp = is_temp;

	a->is_check = is_check;
	a->option_rc = opt_rc;
	a->table_list = table_list;

	//cout<<"Droptable_stmt is created"<<endl;

	insertNodePointer((Node*)a);	// 2014-3-7---将节点指针存入指针数组---by余楷
	return (struct Node *)a;
}

// 2014-3-27---增加---by Yu
struct Node* newLoadTable(nodetype type, char *table_name, Node *path, char *column_separator, char *tuple_separator)
{
	Loadtable_stmt *a= (Loadtable_stmt *)malloc(sizeof(Loadtable_stmt));
	if(!a)
	{
		yyerror("out of space!");
		exit(0);
	}
	a->type = type;
	a->table_name = table_name;
	a->path = path;
	a->column_separator = column_separator;
	a->tuple_separator = tuple_separator;
	printf("the separator are %s,%s\n", a->column_separator, a->tuple_separator);

	//cout<<"Loadtable_stmt is created"<<endl;

	insertNodePointer((Node*)a);	// 2014-3-7---将节点指针存入指针数组---by余楷
	return (Node*)a;
}

// 2014-3-24---增加---by Yu
struct Node* newTableList(nodetype type, char * name1, char * name2, Node * next)
{
	Tablelist * a= (Tablelist *)malloc(sizeof(Tablelist));
	if(!a)
	{
		yyerror("out of space!");
		exit(0);
	}
	a->type = type;
	a->name1 = name1;
	a->name2 = name2;
	a->next = next;

	//cout<<"Tablelist is created"<<endl;
	insertNodePointer((Node*)a);	// 2014-3-7---将节点指针存入指针数组---by余楷
	return (struct Node *)a;
}

Node *newShowStmt(int show_type, bool full, char *database_name, char *like_string)	//2014-5-4---add ---by Yu
{
	Show_stmt *a = (Show_stmt *)malloc(sizeof(Show_stmt));
	if(!a)
	{
		yyerror("out of space!");
		exit(0);
	}
	a->type = t_show_stmt;
	a->show_type = show_type;
	a->full = full;
	a->database_name = database_name;
	a->like_string = like_string;

	insertNodePointer((Node*)a);
	return (struct Node *)a;
}
	
/*************************** DDL语句结束 ********************************/
void outputSpace(int f)
{
	while(f--)
		cout<<"  ";
}
void output(Node * oldnode, int floor)
{
	//puts("there is output()");
	if(oldnode == NULL)
	{
		//cout<<"This is Nothing!"<<endl;
		return;
	}
	nodetype type = oldnode->type;
	switch(type)
	{
		
		case t_name_name:
		{
			Columns * node = (Columns *) oldnode;
			outputSpace(floor);
			cout<<"Columns: "<< node->parameter1<< "  "<< node->parameter2<<endl;
			break;
		}
		case t_name:	// ---3.5---
		{
					Columns * node = (Columns *) oldnode;
					outputSpace(floor);
					cout<<"Columns: "<< node->parameter2<<endl;
					break;
		}
		case t_stringval:
		{
			Expr * node = (Expr *) oldnode;
			outputSpace(floor);
			cout<<"t_stringval: "<<node->data<<endl;	// 2014-4-14---modify because of the change of struct---by Yu
			break;
		}
		case t_intnum:
		{
			Expr * node = (Expr *) oldnode;
			outputSpace(floor);
			cout<<"t_intnum: "<<node->data<<endl;	// 2014-4-14---modify because of the change of struct---by Yu
			break;
		}
		case t_approxnum:
		{
			Expr * node = (Expr *) oldnode;
			outputSpace(floor);
			cout<<"t_approxnum: "<<node->data<<endl;	// 2014-4-14---modify because of the change of struct---by Yu
			break;
		}
		case t_bool:
		{
			Expr * node = (Expr *) oldnode;
			outputSpace(floor);
			cout<<"t_bool: "<<node->data<<endl;	// 2014-4-14---modify because of the change of struct---by Yu
			break;
		}	
		case t_dateval:
		{
			Expr * node = (Expr *) oldnode;
			outputSpace(floor);
			cout<<"t_dateval: "<<node->data<<endl;	//---2014.6.22fzh---
			break;
		}
		case t_expr_list:
		{
			Expr_list * node = (Expr_list *) oldnode;
			outputSpace(floor);
			cout<<"Expr_list: ";cout<<endl;
			
			if(node->data!=NULL) output(node->data,floor+1);
			if(node->next!=NULL) output(node->next,floor);//---3.14fzh---
			
			
			
			break;
		}
		case t_column:
		/* nodetype type;char * parameter1;char *parameter2;Node * next; */
		{
			Columns * node = (Columns *) oldnode;
			outputSpace(floor);
			cout<<"Columns: ";
			if (node->parameter1)
				cout<<node->parameter1<<" ";
			if (node->parameter2)
				cout<<node->parameter2<<" ";
				
			cout<<endl;
			if (node->next)
				output(node->next, floor + 1);
			
			break;
		}
		/*
		struct Expr_cal//计算表达式,二元表达式
		{
			nodetype type;
			char * sign,*parameter;
			int cmp;
			Node *lnext,*rnext;
		};
		*/
		case t_expr_cal: 
		{
			Expr_cal * node = (Expr_cal *) oldnode;
			outputSpace(floor);//---5.23by fzh---
			cout<<"str: >>>>>>>>"<<node->str<<endl;//---5.23by fzh---
			outputSpace(floor);
			cout<<"Expr_cal: ";
			if (node->sign)
				cout<<node->sign<<" ";
			if (node->parameter)
				cout<<node->parameter<<" ";
			if (node->cmp)
				cout<<node->cmp<<" ";
				
			cout<<endl;
			output(node->lnext, floor + 1);
			output(node->rnext, floor + 1);
			break;
		}
		
		/*
		struct Expr_func //函数表达式，将is null/exist等判断抽象成函数
		{
			nodetype type;
			char * funname;
			Node *args;
			Node * parameter1,*parameter2;//函数中的参数列表，处理between...and.../case...when...then...end等
			Node *next;
		}; 
		*/
		case t_expr_func:
		{
			Expr_func * node = (Expr_func *) oldnode;
			outputSpace(floor);//---5.23by fzh---
			if(node->str!=NULL)
			cout<<"str: >>>>>>>>"<<node->str<<endl;//---5.23by fzh---
			outputSpace(floor);
			cout<<"Expr_func: ";
			if (node->funname)
				cout<<node->funname<<" ";
				
			cout<<endl;
			output(node->args, floor + 1);
			output(node->parameter1, floor + 1);
			output(node->parameter2, floor + 1);
			output(node->next, floor + 1);
			break;
		}
		
		case t_table://///////////////////////////////////////////////////
		/* nodetype type;	char * dbname,*tablename,*astablename;
		 * int issubquery;Node *subquery; Node * condition */
		{
			Table * node = (Table *) oldnode;
			
			outputSpace(floor);
			cout<<"Table: ";
			if (node->dbname)
				cout<<node->dbname<<" ";
			if (node->tablename)
				cout<<node->tablename<<" ";
			if (node->astablename)
				cout<<node->astablename<<" ";
			if (node->issubquery)
				cout<<node->issubquery<<" ";
				
			cout<<endl;
			output(node->subquery, floor + 1);
			//output(node->condition, floor + 1);
		//	set<Node*>::iterator it;
		//	vector<Node*>::iterator it;
		//	for(it=node->wcondition.begin();it!=node->wcondition.end();it++)
		//	{
		//		output((Node *)*it,floor+1);
		//	}
			Node * p;
			Expr_list_header * lists=(Expr_list_header *)node->whcdn;
			for(p=lists->header;p!=NULL;p=((Expr_list *)p)->next)
			{
				
				output(p,floor+1);
			}
			
			break;
			
		}
		
		/*************************************************/
		
		case t_query_stmt:
		/* nodetype type;char * querystring;int select_opts;Node *select_list;
			 Node *from_list; Node *where_list; Node *groupby_list;
			Node *having_list; Node *orderby_list; Node *limit_list; Node *into_list; */
		{
			Query_stmt * node = (Query_stmt *) oldnode;
			outputSpace(floor);
			cout<<"Query_stmt: ";
			if(node->querystring)
				cout<<node->querystring<<" ";
			if(node->select_opts)
				cout<<node->select_opts<<" "; 
				
			cout<<endl;
			output(node->select_list, floor + 1);
			output(node->from_list, floor + 1);
			output(node->where_list, floor + 1);
			output(node->groupby_list, floor + 1);
			output(node->having_list, floor + 1);
			output(node->orderby_list, floor + 1);
			output(node->limit_list, floor + 1);
			output(node->into_list, floor + 1);
			
			/* 可继续添加 */
			break;
		}
		case t_select_list:
		/* nodetype type; int isall; Node * args; Node *next; */
		{
			Select_list * node = (Select_list *) oldnode;
			outputSpace(floor);
			cout<<"select_list: ";
			if (node->isall) cout<<"is all ";
			
			cout<<endl;
			output(node->args, floor + 1);
			output(node->next, floor);//---3.14fzh---
			
			break;
		}
		case t_select_expr:
		/* nodetype type;	char *ascolname;	Node * colname; */
		{
			Select_expr * node = (Select_expr *) oldnode;
			outputSpace(floor);
			cout<<"Select_expr: ";
			if (node->ascolname)
				cout<<node->ascolname<< " ";
				
			cout<<endl;
			output(node->colname, floor + 1);
			
			break;
		}
		
		case t_from_list:
		/* nodetype type;	Node * args;	Node *next;  Node * condition */
		{
			From_list * node = (From_list *) oldnode;
			outputSpace(floor);
			cout<<"From_list: ";
			
			cout<<endl;
			//output(node->condition, floor + 1);////////////////////////////////
			Node * p;
			Expr_list_header * lists=(Expr_list_header *)node->whcdn;
			for(p=lists->header;p!=NULL;p=((Expr_list *)p)->next)
			{
				output(p,floor+1);
			}
			output(node->args, floor + 1);
			output(node->next, floor + 1);
			
			break;
		}
		case t_from_expr:
		/* nodetype type;	char * astablename;	Node *next; */
		{
			From_expr * node = (From_expr *) oldnode;
			outputSpace(floor);
			cout<<"From_expr: ";
			if(node->astablename)
				cout<<node->astablename;
			cout<<endl;
			output(node->next, floor+1);
			break;
		}
		case t_where_list:
		/* nodetype type;	char * wherestring;	Node *next; */
		{
			Where_list * node = (Where_list *) oldnode;
			outputSpace(floor);
			cout<<"Where_list: ";
			if(node->wherestring)
				cout<<node->wherestring;
			cout<<endl;
			output(node->next, floor+1);
			break;
		}
		case t_groupby_list:
		/* nodetype type;char * groupbystring;Node *next;int with_rollup; */
		{
			Groupby_list * node = (Groupby_list *) oldnode;
			outputSpace(floor);
			cout<<"Groupby_list: ";
			if(node->groupbystring)
				cout<<node->groupbystring<<" ";
			if (node->with_rollup)
				cout<<node->with_rollup<<" ";
			cout<<endl;
			output(node->next, floor+1);
			break;
		}
		case t_groupby_expr:
		/* nodetype type;	Node *args;	int sorttype;	Node *next; */
		{
			Groupby_expr * node = (Groupby_expr *) oldnode;
			outputSpace(floor);
			cout<<"Groupby_expr: ";
			
			if(node->sorttype)
				cout<<node->sorttype<<" ";
			cout<<endl;
			output(node->args, floor+1);
			output(node->next, floor+1);
			break;
		}
		
		case t_having_list:
		/* nodetype type;	char * havingstring;	Node *next; */
		{
			Having_list * node = (Having_list *) oldnode;
			outputSpace(floor);
			cout<<"Having_list: ";
			if(node->havingstring)
				cout<<node->havingstring<<" ";
				
			cout<<endl;
			output(node->next, floor+1);
			break;
		}
		case t_orderby_list:
		/* nodetype type;	char * orderbystring;	Node *next; */
		{
			Orderby_list * node = (Orderby_list *) oldnode;
			outputSpace(floor);
			cout<<"Orderby_list: ";
			if(node->orderbystring)
				cout<<node->orderbystring<<" ";
				
			cout<<endl;
			output(node->next, floor+1);
			break;
		}
		
		case t_orderby_expr:
		/* nodetype type;	Node *args;	char * sorttype;	Node *next; */
		{
			Orderby_expr * node = (Orderby_expr *) oldnode;
			outputSpace(floor);
			cout<<"Orderby_expr: ";
			if(node->sorttype)
				cout<<node->sorttype<<" ";
				
			cout<<endl;
			output(node->args, floor+1);
			output(node->next, floor+1);
			break;
		}
		
		case t_limit_expr:
		/* nodetype type;	Node * offset;	Node *row_count; */
		{
			Limit_expr * node = (Limit_expr *) oldnode;
			outputSpace(floor);
			cout<<"Limit_expr: ";
			
			cout<<endl;
			output(node->offset, floor+1);
			output(node->row_count, floor+1);
			break;
		}//---3.21 fzh---
		case t_join:
		{
			Join *node=(Join *)oldnode;
			outputSpace(floor);
			cout<<"Join:   jtype= "<<node->jointype<<endl;
			output(node->lnext,floor+1);
			output(node->rnext,floor+1);
			output(node->condition,floor+1);
		}break;
		case t_condition:
		{
			Condition *node=(Condition *)oldnode;
			outputSpace(floor);
			cout<<"joincondition:   ctype= "<<node->conditiontype<<endl;
			output(node->args,floor+1);
		}break;

		default:
		{
			printf("output type not exist!!!\n");
		}
	}
}

void FreeAllNode()	// 2014-3-6---增加释放所有节点的函数---by余楷	// 2014-4-2---存放节点指针的数组改为vector---by Yu
{
	int NodePointerNum = NodePointer.size();
	vector<Node*>::iterator it;
	for (it = NodePointer.begin(); it != NodePointer.end(); ++it)
	{
		free(*it);
		*it = NULL;
	}
	cout<<"All "<<NodePointerNum<<" node freed successfully"<<endl;
	NodePointer.clear();
}

// 2014-4-2----处理转义字符以及消去首尾的引号---by Yu
void GetCorrectString(char *dest, const char *src)
{
	int j = 0;
	for(int i = 1; i < strlen(src)-1; ++i)
	{
		if(src[i] == '\\')
		{
			if(src[i+1] == 'n')	// 若内部表示为“\\n”，改为“\n”
			{
				dest[j++] = '\n';
				++i;
			}
			else if(src[i+1] == 't')	// 若内部表示为“\\t”，改为“\t”
			{
				dest[j++] = '\t';
				++i;
			}
			else if(src[i+1] == '\'' && (i+1) != (strlen(src)-1))	// 若内部表示为“\\\'”且引号不为最后的引号,改为“\'”
			{
				dest[j++] = '\'';
				++i;
			}
			else if(src[i+1] == '\"' && (i+1) != (strlen(src)-1))	// 若内部表示为“\\\"”且引号不为最后的引号，改为“\"”
			{
				dest[j++] = '\"';
				++i;
			}
			else	//复制
			{
				dest[j++] = src[i];
			}
		}
		else
		{
			dest[j++] = src[i];
		}
	}
	dest[j] = '\0';
}



