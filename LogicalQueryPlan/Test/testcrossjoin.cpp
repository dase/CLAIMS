#include<iostream>
#include "../../Test/set_up_environment.h"
#include "../CrossJoin.h"
#include "../../LogicalQueryPlan/Scan.h"
#include "../../LogicalQueryPlan/LogicalQueryPlanRoot.h"
#include "../../common/ExpressionItem.h"
#include "../../LogicalQueryPlan/Filter.h"
using namespace std;
static int test_cross_join()
{
	int master;
//	cout<<"Master(0) or Slave(others)"<<endl;
//	cin>>master;
	printf("~!OKOKO!!!!!\n");
	printf("Master(0) or Slave(others)??\n");
	scanf("%d",&master);
	getchar();	// 2014-3-4---屏蔽换行符对后面的影响---by余楷
	if(master!=0)
	{
		Environment::getInstance(false);
	}
	else
	{
		Environment::getInstance(true);
		startup_multiple_node_environment_of_tpch();

		LogicalOperator* scan_part=new LogicalScan(Environment::getInstance()->getCatalog()->getTable(std::string("PART"))->getProjectoin(0));
		LogicalOperator* scan_supplier=new LogicalScan(Environment::getInstance()->getCatalog()->getTable(std::string("SUPPLIER"))->getProjectoin(0));
		vector<vector<ExpressionItem> >allexpr;
		vector<ExpressionItem>expr;
		ExpressionItem expritem0;
		expritem0.setVariable("PART.row_id");
		expr.push_back(expritem0);
		ExpressionItem expritem2;
		expritem2.setIntValue(3);
		expr.push_back(expritem2);
		ExpressionItem expritem1;
		expritem1.setOperator("<");
		expr.push_back(expritem1);
		allexpr.push_back(expr);
		LogicalOperator *filter_part=new Filter(scan_part,allexpr);
		vector<vector<ExpressionItem> >allexpr1;
		vector<ExpressionItem>expr1;
		ExpressionItem expritem10;
		expritem10.setVariable("SUPPLIER.row_id");
		expr1.push_back(expritem10);
		ExpressionItem expritem12;
		expritem12.setIntValue(3);
		expr1.push_back(expritem12);
		ExpressionItem expritem11;
		expritem11.setOperator("<");
		expr1.push_back(expritem11);
		allexpr1.push_back(expr1);
		LogicalOperator *filter_supplier=new Filter(scan_supplier,allexpr1);

		LogicalOperator* cross_join=new CrossJoin(filter_part,filter_supplier);
		LogicalOperator* root=new LogicalQueryPlanRoot(0,cross_join,LogicalQueryPlanRoot::PRINT);
		cout<<"~~~~~~~~~logical plan~~~~~~~~~~~~~~"<<endl;
		root->print(0);
		BlockStreamIteratorBase* physical_iterator_tree=root->getIteratorTree(64*1024);
		cout<<"~~~~~~~~~physical plan~~~~~~~~~~~~~~"<<endl;
		physical_iterator_tree->print();
		cout<<"~~~~~~~~~~~~~~~~end~~~~~~~~~~~~~~~~~~~~"<<endl;

		physical_iterator_tree->open();
		while(physical_iterator_tree->next(0));
		physical_iterator_tree->close();

	}
	return 0;
}
