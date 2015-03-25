#ifndef __GENERATING_EXPR__CPP__
#define __GENERATING_EXPR__CPP__
#include "../common/Expression/qnode.h"
#include <vector>
#include "../common/data_type.h"
#include "../common/Schema/SchemaFix.h"
#include "../common/Schema/Schema.h"
#include "../common/Expression/initquery.h"
#include "ExpressionGenerator.h"
#include "CodeGenerator.h"
using namespace std;
static QNode* get_one(){
	std::vector<column_type> columns;
	columns.push_back(data_type(t_int));
	columns.push_back(data_type(t_int));
	Schema* s=new SchemaFix(columns);
	map<string,int> column_index;
	column_index["a"]=0;
	column_index["b"]=1;

	QColcumns* a=new QColcumns("T","a",t_int,"a");
	QColcumns* b=new QColcumns("T","b",t_int,"b");

	QExpr_binary* op=new QExpr_binary(a,b,t_int,oper_add,t_qexpr_cal,"result");

	InitExprAtLogicalPlan(op,t_int,column_index,s);
	CodeGenerator::getInstance();

	expr_func f=getExprFunc(op,s);

	void* tuple=malloc(s->getTupleMaxSize());
	*(int*)tuple=1;
	*((int*)tuple+1)=2;
	int ret;
	f(tuple,&ret);
	printf("Return: %d\n",ret);


}
#endif
