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
	startup_multiple_node_environment_of_tpch();
	ResultSet *result_set;
	bool result_flag=true;
	string error_msg;
	char sql[1000];
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
		ExecuteLogicalQueryPlan(string(sql),result_set,result_flag,error_msg);
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
		printf("please input 1 to continue or 0 to end\n");
		scanf("%d\n",&count);
	}
	while(true)
	{
		sleep(1);
	}
}


