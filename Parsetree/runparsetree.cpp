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
extern int globalReadOffset;

static Node * getparsetreeroot()
{
	globalReadOffset = 0;

	puts("SQL!!!!!");
	int charnum=0;
    while(1)
    {
        char c=getchar();
        globalInputText[charnum++]=c;
        if(c==';')
        {
            globalInputText[charnum++]='\0';
            break;
        }
    }
	strcpy(globalInputText,"select row_id,count(*) from cj where cj.row_id=1 group by cj.row_id;");
    puts(globalInputText);

  	if(!yyparse())
  	{
  		printf("SQL parse worked\n");

		output(parsetreeroot, 1);
		if(parsetreeroot->type==t_query_stmt)
		{
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
			return parsetreeroot;
		}
		else
		{
			return parsetreeroot;
		}
    }
  	else
  	{
   	 printf("SQL parse failed\n");
   	 return NULL;
    }
    //return 0;
}
static Node * getparsetreeroot(const char* sql)
{
	globalReadOffset = 0;


	/**
	 * I highly doubt that the globally shared globalInputText will introduce troubles
	 * if getparsetreeroot() is called by multiple thread concurrently.
	 */

//	int charnum=0;
//    while(sql[charnum]!='\0')
//    {
//        char c=sql[charnum];
//        globalInputText[charnum++]=c;
//        if(c==';')
//        {
//            globalInputText[charnum++]='\0';
//            break;
//        }
//    }
	strcpy(globalInputText,sql);
    puts(globalInputText);
  	if(!yyparse())
  	{
  		printf("SQL parse worked\n");

		output(parsetreeroot, 1);
		if(parsetreeroot->type==t_query_stmt)
		{
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
			return parsetreeroot;
		}
		else
		{
			return parsetreeroot;
		}
    }
  	else
  	{
   	 printf("SQL parse failed\n");
   	 return NULL;
    }
    //return 0;
}
#endif
