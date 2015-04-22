/*
 * Project.cpp
 *
 *  Created on: 2014-2-21
 *      Author: casa
 */

#include "Project.h"

#include "../common/ids.h"
#include "../common/data_type.h"
#include "../common/Expression/initquery.h"

LogicalProject::LogicalProject(LogicalOperator *child, std::vector<std::vector<ExpressionItem> > &exprArray)
:child_(child),exprArray_(exprArray),dataflow_(0){
//	initialize_arithmetic_type_promotion_matrix();
//	initialize_type_cast_functions();
	setOperatortype(l_project);
}
LogicalProject::LogicalProject(LogicalOperator *child, vector<QNode *>exprTree)
:child_(child),exprTree_(exprTree),dataflow_(0)
{
	setOperatortype(l_project);
//	initialize_arithmetic_type_promotion_matrix();
//	initialize_type_cast_functions();
}

LogicalProject::~LogicalProject(){
	delete dataflow_;
	delete child_;
}

Dataflow LogicalProject::getDataflow(){
	if(dataflow_!=NULL)
		return *dataflow_;
	mappings_=getMapping();
	Dataflow ret;
	const Dataflow child_dataflow=child_->getDataflow();
	ret.property_.commnication_cost=child_dataflow.property_.commnication_cost;
	ret.property_.partitioner=child_dataflow.property_.partitioner;
	std::vector<Attribute> ret_attrs;

	Schema *input_=getSchema(child_dataflow.attribute_list_);
	for(unsigned i=0;i<exprArray_.size();i++){
		int seq=-1;
		for(unsigned j=0;j<exprArray_[i].size();j++){
			if(exprArray_[i][j].type==ExpressionItem::variable_type){
				seq++;
				exprArray_[i][j].return_type=input_->getcolumn(mappings_.getMapping()[i][seq]).type;
				if(exprArray_[i][j].return_type==t_string){
					exprArray_[i][j].size=input_->getcolumn(mappings_.getMapping()[i][seq]).size;
				}
			}
		}
	}
/*above:initalize the expr node,get the return_type and get_length
 *next:construct the dataflow at this level
 *
 */
	for(unsigned i=0;i<mappings_.getMapping().size();i++){
//		cout<<"mappings_.getMappings().size(): "<<mappings_.getMapping().size()<<endl;
//		if(exprArray_[i].size()>1){
			string alias=recovereyName(exprArray_[i]);
			column_type column=ExpressionCalculator::getOutputType_(exprArray_[i]);
			//currently,0 is to table_id
			//todo: support the talbe id value
			const unsigned table_id=INTERMEIDATE_TABLEID;
			Attribute attr_alais(table_id,i,alias,column.type,column.get_length());
			ret_attrs.push_back(attr_alais);
//		}
//		else{
//			Attribute tmp=child_dataflow.attribute_list_[mappings_.getMapping()[i][0]];
//			Attribute attr(0,i,tmp.getName(),tmp.attrType->type,tmp.attrType->get_length());
//			ret_attrs.push_back(attr);
//		}
	}
/////////new exprtree////
	getcolindex(child_dataflow);
	for(int i=0;i<exprTree_.size();i++)
	{
		InitExprAtLogicalPlan(exprTree_[i],exprTree_[i]->actual_type,colindex_,input_);
	}
	ret_attrs.clear();
	for(int i=0;i<exprTree_.size();i++)
	{
		column_type *column=0;
		if(exprTree_[i]->return_type==t_string||exprTree_[i]->return_type==t_decimal)
		{
			column=new column_type(exprTree_[i]->return_type,exprTree_[i]->length);
		}
		else
		{
			column=new column_type(exprTree_[i]->return_type);
		}
		const unsigned table_id=INTERMEIDATE_TABLEID;
		Attribute attr_alais(table_id,i,exprTree_[i]->alias,column->type,column->size);
		ret_attrs.push_back(attr_alais);
	}
	ret.attribute_list_=ret_attrs;
	dataflow_=new Dataflow();
	*dataflow_=ret;
	return ret;
}
bool LogicalProject::getcolindex(Dataflow dataflow)
{
	for(int i=0;i<dataflow.attribute_list_.size();i++)
	{
		colindex_[dataflow.attribute_list_[i].attrName]=i;
	}
	return true;
}
BlockStreamIteratorBase *LogicalProject::getIteratorTree(const unsigned& blocksize){
	getDataflow();
	Dataflow child_dataflow=child_->getDataflow();
	BlockStreamIteratorBase *child=child_->getIteratorTree(blocksize);
	BlockStreamProjectIterator::State state;
	state.block_size_=blocksize;
	state.child_=child;
	state.v_ei_=exprArray_;
	state.input_=getSchema(child_dataflow.attribute_list_);
	state.map_=mappings_;
	state.output_=getOutputSchema();
	state.exprTree_=exprTree_;
	/*
	 * the output schema has the column type which has the data type and the data length
	 * like the column type string, _string and sizeof(_string)
	 * */
	return new BlockStreamProjectIterator(state);
}

Schema *LogicalProject::getOutputSchema(){
	Schema *schema=getSchema(dataflow_->attribute_list_);
	return schema;
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
	/*every time invoke a getColumnSeq, you need to new a catalog--@li: it seams that you actually get
	 * the reference to the catalog rather than creating one.
	*/
	Dataflow child_dataflow=child_->getDataflow();
	for(unsigned i=0;i<child_dataflow.attribute_list_.size();i++){
	//		TableDescriptor *table=Catalog::getInstance()->getTable(child_dataflow.attribute_list_[i].table_id_);
	//		stringstream ss;
	//		ss<<table->getTableName()<<"."<<child_dataflow.attribute_list_[i].attrName;
	//		string table_column=ss.str();
//		if((tablename.compare(ei.content.var.table_name)==0)&&(child_dataflow.attribute_list_[i].attrName.compare(ei.content.var.column_name)==0)){
//			return i;
//		}
		string table_column=child_dataflow.attribute_list_[i].attrName;
		if(table_column.compare(ei.content.var.table_column)==0)
			return i;
	}
	printf("Variable ExpressItem fails to match any attribute in the dataflow!\n");
	assert(false);
}

string LogicalProject::recovereyName(Expression ei) {
	string ret;
	/* 将后缀表达式变成中缀表达式 */
	stringstream ss;
	Exp_op_tmp exop_ini;
	for(unsigned i=0;i<ei.size();i++){
		Exp_op ini;
		ini.expr.push_back(ei[i]);
		ini.op=0;
		exop_ini.push_back(ini);
	}

	Exp_op_tmp current;
	while(exop_ini.size()>1){
//		cout<<"in the while loop！"<<endl;
	for(unsigned i=0;i<exop_ini.size();i++){
		if(exop_ini[i].op==2){
			continue;
		}
		if(exop_ini[i].op==1){
			continue;
		}
		if(exop_ini[i].op==0){
			if(exop_ini[i].expr[0].type==ExpressionItem::operator_type){
				if(exop_ini[i].expr[0].getOperatorName()=="+"||exop_ini[i].expr[0].getOperatorName()=="-"){
					//将前两个i-1和i-2加起来
					Exp_op op;
					for(unsigned m=0;m<exop_ini[i-2].expr.size();m++){
						op.expr.push_back(exop_ini[i-2].expr[m]);
					}
					op.expr.push_back(exop_ini[i].expr[0]);
					for(unsigned n=0;n<exop_ini[i-1].expr.size();n++){
						op.expr.push_back(exop_ini[i-1].expr[n]);
					}
					op.op=1;
					for(unsigned j=0;j<exop_ini.size();j++){
						if((j>=i-2)&&(j<=i)){
							if(j==i){
								current.push_back(op);
								continue;
							}
							else
								continue;
						}
						else
							current.push_back(exop_ini[j]);
					}
					exop_ini=current;
					current.clear();
					break;
				}
				else{
					//将前两个相乘，看是否有括号
					if(exop_ini[i-1].op==1||exop_ini[i-2].op==1){
						Exp_op op;
						ExpressionItem bl;
						ExpressionItem br;
						bl.item_name="(";
						br.item_name=")";
						if(exop_ini[i-2].op==1){
							op.expr.push_back(bl);
							for(unsigned m=0;m<exop_ini[i-2].expr.size();m++){
								op.expr.push_back(exop_ini[i-2].expr[m]);
							}
							op.expr.push_back(br);
						}
						else{
							for(unsigned m=0;m<exop_ini[i-2].expr.size();m++){
								op.expr.push_back(exop_ini[i-2].expr[m]);
							}
						}
						op.expr.push_back(exop_ini[i].expr[0]);
						if(exop_ini[i-1].op==1){
							op.expr.push_back(bl);
							for(unsigned n=0;n<exop_ini[i-1].expr.size();n++){
								op.expr.push_back(exop_ini[i-1].expr[n]);
							}
							op.expr.push_back(br);
						}
						else{
							for(unsigned n=0;n<exop_ini[i-1].expr.size();n++){
								op.expr.push_back(exop_ini[i-1].expr[n]);
							}
						}
						op.op=2;
						for(unsigned j=0;j<exop_ini.size();j++){
							if((j>=i-2)&&(j<=i)){
								if(j==i){
									current.push_back(op);
									continue;
								}
								else
									continue;
							}
							else
								current.push_back(exop_ini[j]);
						}
						exop_ini=current;
						current.clear();
						break;
					}
					else{
						Exp_op op;
						for(unsigned m=0;m<exop_ini[i-2].expr.size();m++){
							op.expr.push_back(exop_ini[i-2].expr[m]);
						}
						op.expr.push_back(exop_ini[i].expr[0]);
						for(unsigned n=0;n<exop_ini[i-1].expr.size();n++){
							op.expr.push_back(exop_ini[i-1].expr[n]);
						}
						op.op=2;
						for(unsigned j=0;j<exop_ini.size();j++){
							if((j>=i-2)&&(j<=i)){
								if(j==i){
									current.push_back(op);
									continue;
								}
								else
									continue;
							}
							else
								current.push_back(exop_ini[j]);
						}
						exop_ini=current;
						current.clear();
						break;
					}
				}
			}
			else{
				continue;
			}
		}
	}
	}

	for(unsigned i=0;i<exop_ini[0].expr.size();i++){
		ss<<exop_ini[0].expr[i].item_name;
	}
	ret=ss.str();
	return ret;
}

void LogicalProject::printProjSchema()const
{

}

void LogicalProject::print(int level)const{
	printf("project:\n");
	for(int i=0;i<exprTree_.size();i++)
	{
		printf("	%s\n",exprTree_[i]->alias.c_str());
	}
	child_->print(level+1);
}
