/*
 * Project.cpp
 *
 *  Created on: 2014-2-21
 *      Author: casa
 */

#include "Project.h"

LogicalProject::LogicalProject(LogicalOperator *child, std::vector<std::vector<ExpressionItem> > &exprArray)
:child_(child),exprArray_(exprArray){
	initialize_arithmetic_type_promotion_matrix();
	initialize_type_cast_functions();
}

LogicalProject::~LogicalProject(){

}

Dataflow LogicalProject::getDataflow(){
	dataflow_=child_->getDataflow();
	return dataflow_;
}

BlockStreamIteratorBase *LogicalProject::getIteratorTree(const unsigned& blocksize){
	getDataflow();
	BlockStreamIteratorBase *child=child_->getIteratorTree(blocksize);
	BlockStreamProjectIterator::State state;
	state.block_size_=blocksize;
	state.children_=child;
	state.v_ei_=exprArray_;
	state.input_=getSchema(dataflow_.attribute_list_);
	vector<column_type> column_list;
	column_list.push_back(column_type(t_int));
	state.output_=new SchemaFix(column_list);
	state.output_=getOutputSchema();
	state.map_=getMapping();
	return new BlockStreamProjectIterator(state);
}

Schema *LogicalProject::getOutputSchema(){
	//must scan the expression and get the output schema
	vector<column_type> column_list;
	for(unsigned i=0;i<exprArray_.size();i++){

		column_list.push_back();
	}
	Schema *rt_schema=new SchemaFix();
	return rt_schema;
}

Mapping LogicalProject::getMapping(){
	Mapping mappings;
	for(unsigned i=0;i<exprArray_.size();i++){
		ExpressionMapping em;
		for(unsigned j=0;j<exprArray_[i].size();j++){
			if(exprArray_[i][j].type==ExpressionItem::variable_type)
				em.push_back(getColumnSeq(exprArray_[i][j]));
		}
		mappings.atomicPushExpressionMapping(em);
	}
	return mappings;
}

int LogicalProject::getColumnSeq(ExpressionItem &ei){
	int rt;
	//every time invoke a getColumnSeq, you need to new a catalog
	for(unsigned i=0;i<dataflow_.attribute_list_.size();i++){
		TableDescriptor *table=Catalog::getInstance()->getTable(dataflow_.attribute_list_[i].table_id_);
		string tablename=table->getTableName();
		if((tablename.compare(ei.content.var.table_name)==0)&&(dataflow_.attribute_list_[i].attrName.compare(ei.content.var.column_name)==0)){
			rt=i;
			break;
		}
	}
	return rt;
}
