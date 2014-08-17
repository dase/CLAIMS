#include "qnode.h"
/*
 * queryfunc.cpp
 *
 *  Created on: Aug 6, 2014
 *      Author: imdb
 */

void int_eq(FuncCallInfo fcinfo)
{
	bool ans=1;
	if(*(int *)fcinfo->args[0]!=*(int *)fcinfo->args[1])
	{
		ans=0;
	}
	fcinfo->results=&ans;
}
void u_long_eq(FuncCallInfo fcinfo)
{
	bool ans=1;
	if(*(unsigned long *)fcinfo->args[0]!=*(unsigned long *)fcinfo->args[1])
	{
		ans=0;
	}
	fcinfo->results=&ans;
}

