/*
 * sql_parser_test.h
 *
 *  Created on: Jul 7, 2014
 *      Author: wangli
 */

#ifndef SQL_PARSER_TEST_H_
#define SQL_PARSER_TEST_H_
#include "../../logical_query_plan/LogicalQueryPlanRoot.h"
#include "../set_up_environment.h"
#include <stdio.h>
#include "../../logical_query_plan/logical_operator.h"
#include "../../physical_query_plan/BlockStreamIteratorBase.h"

char sql[1000];
int sql_parser_test(){
	startup_single_node_environment_of_poc();

	while(true){
		printf("sql:\n");
		fflush(stdout);
		fgets(sql,1000,stdin);
		LogicalOperator* logical_tree=convert_sql_to_logical_operator_tree(sql);
		if(logical_tree!=0){
			LogicalOperator* root=new LogicalQueryPlanRoot(0,logical_tree,LogicalQueryPlanRoot::PRINT );
			BlockStreamIteratorBase* physical_plan=root->GetPhysicalPlan(64*1024);

			physical_plan->Open(0);
			physical_plan->Next(0);
			physical_plan->Close();

		}
	}

}


#endif /* SQL_PARSER_TEST_H_ */
