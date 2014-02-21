/*
 * Executing.cpp
 *
 *  Created on: Feb 20, 2014
 *      Author: wangli
 */

#include "Executing.h"
#include "../Parsetree/runparsetree.cpp"
#include "../Parsetree/parsetree2logicalplan.cpp"
#include "../LogicalQueryPlan/LogicalQueryPlanRoot.h"
Executing::Executing() {
	// TODO Auto-generated constructor stub

}

Executing::~Executing() {
	// TODO Auto-generated destructor stub
}

ResultSet* Executing::run_sql(std::string sql){
//	char * test="select row_id,count(*) from cj where cj.row_id=1 group by cj.row_id;";
	Node* node=getparsetreeroot(sql.c_str());
	LogicalOperator* plan=parsetree2logicalplan(node);

	LogicalOperator* root=new LogicalQueryPlanRoot(0,plan,LogicalQueryPlanRoot::RESULTCOLLECTOR);
//	unsigned long long int timer_start=curtick();

	BlockStreamIteratorBase* collector = root->getIteratorTree(
			1024 * 64 - sizeof(unsigned));
	collector->print();
	collector->open();
	collector->next(0);
	collector->close();
	ResultSet* resultset = collector->getResultSet();
	resultset->print();
	root->~LogicalOperator();;

}
