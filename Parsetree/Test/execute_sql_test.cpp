/*
 * execute_sql_test.cpp
 *
 *  Created on: Sep 11, 2014
 *      Author: imdb
 */

#include<stdio.h>
#include<iostream>
#include<algorithm>
#include<string>
#include "../../Parsetree/ExecuteLogicalQueryPlan.h"
#include "../../common/Block/ResultSet.h"
#include "../../Environment.h"
#include "../../Test/set_up_environment.h"
using namespace std;
static void execute_sql_test()
{
	Environment::getInstance(true);
//	startup_multiple_node_environment_of_stock();
	startup_single_node_environment_of_poc();

	Catalog* catalog=Environment::getInstance()->getCatalog();
//	catalog->saveCatalog();
//	catalog->restoreCatalog();	/* restore Catalog*/

	ResultSet *result_set = NULL;
	bool result_flag=true;
	string error_msg;
	string info;
	char sql[10000];
	cout<<"please input  sql:"<<endl;
	int charnum=0;
	int count=1;
	while(count)
	{
		while(1)
		{
			char c=getchar();
			sql[charnum++]=c;
			if(c==';')
			{
				sql[charnum++]='\0';
				break;
			}
		}
		result_flag=true;

		ExecuteLogicalQueryPlan(string(sql),result_set,result_flag,error_msg, info);
		if(result_flag==false)
		{
			cout<<"[ERROR] "<<error_msg<<endl;
		}
		else
		{
			if(result_set!=NULL)
				result_set->print();
		}
		memset(sql,0,sizeof(0));
		charnum=0;
	}
	while(true)
	{
		sleep(1);
	}
}


