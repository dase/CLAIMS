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
extern "C" int yylex(); ////////////////////////
extern "C" int yyparse();////////////////////
extern Node * parsetreeroot;
extern char globalInputText[10000];
#include <iostream>
static Node * getparsetreeroot()
{
	globalReadOffset = 0;

	puts("SQL!!!!!");
	getchar();
	gets(globalInputText);
//	std::cin>>globalInputText;
//	char input[1000]="select a from B,A where B.a=A.a and A.a=0;";
//	globalInputText="select row_id from cj;";
//	strcpy(globalInputText,input);
	puts(globalInputText);

  	if(!yyparse())
  	{
  		printf("SQL parse worked\n");

		output(parsetreeroot, 1);

		Query_stmt *node=(Query_stmt *)parsetreeroot;
		puts("select_stmt2>>>>>>>>");
		if(node->where_list!=NULL)
		{
			struct Where_list * curt=(struct Where_list *)(node->where_list);
			struct Node *cur=(struct Node *)(curt->next);
			puts("wc2tb");
			departwc(cur,node->from_list);
			puts("partree complete!!!");
		}
		puts("after>>>>>>>>>>>>>");
		output(node->from_list, 1);
		return node->from_list;
    }
  	else
  	{
   	 printf("SQL parse failed\n");
   	 return NULL;
    }
    //return 0;
} 

#endif
