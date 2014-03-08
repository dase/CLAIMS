/*
 * runparsetree.cpp
 *
 *  Created on: 2014年1月21日
 *      Author: imdb
 */
#ifndef __RUNPARSER__
#define __RUNPARSER__
#include "sql_node_struct.h"
//#include "../Environment.h"
extern "C" int yylex();
extern "C" int yyparse();
//extern "C" int yyclearin();
//extern "C" int yyerrok();
extern Node * parsetreeroot;
extern char globalInputText[10000];
extern int globalReadOffset;
extern int errorNumber;		// 2014-3-6---声明errorNumber变量---by余楷
extern Node *NodePointer[10000];		// 2014-3-6---指向每个节点指针的数组---by余楷
extern int NodePointerNum;		// 2014-3-6---指向每个节点指针的数组元素个数---by余楷

static Node * getparsetreeroot()
{
	puts("SQL!!!!!");
	int charnum=0;
	globalReadOffset = 0;

	memset(NodePointer,0,sizeof(NodePointer));	// 2014-3-7---初始化---by余楷
	NodePointerNum = 0;
	errorNumber = 0;	// 2014-3-6---初始化SQL解析错误个数---by余楷
	parsetreeroot = NULL;	// 2014-3-4---增加初始化语句---by余楷
	memset(globalInputText, 0, sizeof(globalInputText));	// 2014-3-4---增加初始化语句---by余楷
	cout<<"globalInputText:"<<globalInputText;

	while(1)
	{
		char c=getchar();
		globalInputText[charnum++]=c;
		if(c==';')
		{
			globalInputText[charnum++]='\0';
			getchar();
			break;
		}
	}

	// 2014-3-4---测试数据---by余楷
//	strcpy(globalInputText,
//			"select row_id from cj;\n"
//			"create table tt(num int not null, s int, ss int primary key, c int, vc varchar(15));\n"
//			"create projection on tt(num, s, c) partitioned on num;\n"
//			"select s from tt;\n"
//			"create projection on tt(ss, c) partitioned on ss;\n"
//			"select ss from tt;\n"

//			"create table t(num int sdf, d double, f float);\n"

	//			"create table tt(num int, d double, f float);\n"
//			"create table ttt(num int, d double, f float);\n"
//			"create projection on t(num, f, d) partitioned on num;\n"
//			"select f,d from t;\n"
//			);
	cout<<"globalInputText:"<<globalInputText<<endl;

//	yyclearin;
////	yychar = YYEMPTY;
//	yyerrok;
//	cleanSQLparse();

	if(!yyparse())	// 2014-3-4---注释冗余代码---by余楷
	{
		return parsetreeroot;
	}
	else
	{
		printf("SQL parse failed\n");
		return NULL;
	}
	//return 0;
}

static Node * getparsetreeroot(const char *sql)
		{
	puts("SQL!!!!!");
	int charnum=0;
	globalReadOffset = 0;

	memset(NodePointer,0,sizeof(NodePointer));	// 2014-3-7---初始化---by余楷
	NodePointerNum = 0;
	errorNumber = 0;	// 2014-3-6---初始化SQL解析错误个数---by余楷
	parsetreeroot = NULL;	// 2014-3-4---增加初始化语句---by余楷
	memset(globalInputText, 0, sizeof(globalInputText));	// 2014-3-4---增加初始化语句---by余楷
	cout<<"globalInputText:"<<globalInputText;

	while(1)
	{
		char c=getchar();
		globalInputText[charnum++]=c;
		if(c==';')
		{
			globalInputText[charnum++]='\0';
			getchar();
			break;
		}
	}

	// 2014-3-4---测试数据---by余楷
	strcpy(globalInputText,	sql);
	cout<<"globalInputText:"<<globalInputText<<endl;

	//	yyclearin;
	////	yychar = YYEMPTY;
	//	yyerrok;
	//	cleanSQLparse();

	if(!yyparse())	// 2014-3-4---注释冗余代码---by余楷
	{
		Stmt *stmtList = (Stmt *)getparsetreeroot();
		Node *node = (Node *)stmtList->data;
		Query_stmt *querynode=(Query_stmt *)node;
		puts("select_stmt2>>>>>>>>");
		if(querynode->where_list!=NULL)
		{
			struct Where_list * curt=(struct Where_list *)(querynode->where_list);
			struct Node *cur=(struct Node *)(curt->next);
			puts("wc2tb");
			departwc(cur,querynode->from_list);
			puts("partree complete!!!");
		}
		return parsetreeroot;
	}
	else
	{
		printf("SQL parse failed\n");
		return NULL;
	}
	//return 0;
		}

#endif
