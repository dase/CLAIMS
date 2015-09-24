#include<iostream>
#include "../../Test/set_up_environment.h"
#include "../CrossJoin.h"
#include "../../LogicalQueryPlan/logical_scan.h"
#include "../../LogicalQueryPlan/LogicalQueryPlanRoot.h"
#include "../../common/ExpressionItem.h"
#include "../../LogicalQueryPlan/Filter.h"
#include "../../common/Expression/qnode.h"
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
		startup_multiple_node_environment_of_stock();

		LogicalOperator* scan_field=new LogicalScan(Environment::getInstance()->getCatalog()->getTable(std::string("a"))->getProjectoin(0));
		LogicalOperator* scan_area=new LogicalScan(Environment::getInstance()->getCatalog()->getTable(std::string("b"))->getProjectoin(0));
//		vector<QNode *>exprTree0;
//		QColcumns *cal0=new QColcumns("field","field.row_id",t_u_long,"field.row_id");
//		QExpr *qexpr0=new QExpr("3",t_string,"3");
//		QExpr_binary *qcalnode0=new QExpr_binary(cal0,qexpr0,t_u_long,oper_less,t_qexpr_cmp,"field.row_id<3");
//		exprTree0.push_back(qcalnode0);
//		LogicalOperator *filter_field=new Filter(scan_field,exprTree0);
//
//		vector<QNode *>exprTree1;
//		QColcumns *cal1=new QColcumns("area","area.row_id",t_u_long,"area.row_id");
//		QExpr *qexpr1=new QExpr("2",t_string,"2");
//		QExpr_binary *qcalnode1=new QExpr_binary(cal1,qexpr1,t_u_long,oper_less,t_qexpr_cmp,"area.row_id<2");
//		exprTree1.push_back(qcalnode1);
//		LogicalOperator *filter_area=new Filter(scan_area,exprTree1);


		LogicalOperator* cross_join=new CrossJoin(scan_field,scan_area);

		LogicalOperator* root=new LogicalQueryPlanRoot(0,cross_join,LogicalQueryPlanRoot::RESULTCOLLECTOR);
		cout<<"~~~~~~~~~logical plan~~~~~~~~~~~~~~"<<endl;
		root->print(0);
		BlockStreamIteratorBase* physical_iterator_tree=root->getIteratorTree(64*1024);
		cout<<"~~~~~~~~~physical plan~~~~~~~~~~~~~~"<<endl;
		physical_iterator_tree->print();
		cout<<"~~~~~~~~~~~~~~~~end~~~~~~~~~~~~~~~~~~~~"<<endl;

		physical_iterator_tree->open();
		while(physical_iterator_tree->next(0));
		physical_iterator_tree->close();
		ResultSet* result=physical_iterator_tree->getResultSet();
		result->print();
		delete result;
		delete Environment::getInstance(true);
	}
	return 0;
}
