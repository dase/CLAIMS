/*
 * Filter.cpp
 *
 *  Created on: Nov 10, 2013
 *      Author: wangli
 */

#include "Filter.h"
#include "../BlockStreamIterator/ParallelBlockStreamIterator/ExpandableBlockStreamFilter.h"
#include "../BlockStreamIterator/ParallelBlockStreamIterator/ExpandableBlockStreamExchangeEpoll.h"
#include "../IDsGenerator.h"
#include "../Catalog/stat/StatManager.h"
#include "../Catalog/stat/Estimation.h"
#include "../common/AttributeComparator.h"
#include "../common/TypePromotionMap.h"
#include "../common/TypeCast.h"
#include "../common/Expression/initquery.h"
Filter::Filter(LogicalOperator *child,vector<QNode *>qual)
:child_(child),qual_(qual)
{
	setOperatortype(l_filter);
	initialize_type_cast_functions();
}
Filter::Filter(LogicalOperator *child, std::vector<std::vector<ExpressionItem> > &exprArray)
:child_(child),exprArray_(exprArray){
	initialize_arithmetic_type_promotion_matrix();
	initialize_type_cast_functions();
	setOperatortype(l_filter);
}

Filter::Filter(std::vector<AttributeComparator> ComparatorList,LogicalOperator* child )
:comparator_list_(ComparatorList),child_(child){
	assert(!comparator_list_.empty());
	setOperatortype(l_filter);
}

Filter::Filter(const Condition& condition,LogicalOperator*  child):condition_(condition),child_(child){

//	condition_.print();
	setOperatortype(l_filter);
}
Filter::~Filter() {
	// TODO Auto-generated destructor stub
	if(child_>0){
		child_->~LogicalOperator();
	}
}

Dataflow Filter::getDataflow(){
	/** In the currently implementation, we assume that the boolean operator
	 * between each AttributeComparator is "AND".
	 */

	Dataflow dataflow=child_->getDataflow();
	if(comparator_list_.size()==0)
		generateComparatorList(dataflow);
	if(dataflow.isHashPartitioned()){
		for(unsigned i=0;i<dataflow.property_.partitioner.getNumberOfPartitions();i++){
			if(couldHashPruned(i,dataflow.property_.partitioner))//is filtered
			{
				dataflow.property_.partitioner.getPartition(i)->setFiltered();
			}
			else{
				/*
				 * should predict the volume of data that passes the filter.
				 * TODO: a precious prediction is needed based on the statistic of
				 * the input data, which may be maintained in the catalog module.
				 */
				const unsigned before_filter_cardinality=dataflow.property_.partitioner.getPartition(i)->getDataCardinality();
				const unsigned after_filter_cardinality=before_filter_cardinality*predictSelectivity();
				dataflow.property_.partitioner.getPartition(i)->setDataCardinality(after_filter_cardinality);
			}
		}
	}
	getcolindex(dataflow);
	Schema *input_=getSchema(dataflow.attribute_list_);
	for(int i=0;i<qual_.size();i++)
	{
		InitExprAtLogicalPlan(qual_[i],t_boolean,colindex_,input_);
	}
	return dataflow;
}
BlockStreamIteratorBase* Filter::getIteratorTree(const unsigned& blocksize){
	Dataflow dataflow=getDataflow();
	BlockStreamIteratorBase* child_iterator=child_->getIteratorTree(blocksize);
	ExpandableBlockStreamFilter::State state;
	state.block_size_=blocksize;
	state.child_=child_iterator;
	state.v_ei_=exprArray_;
	state.qual_=qual_;
	state.colindex_=colindex_;
//	assert(!comparator_list_.empty());
	state.comparator_list_=comparator_list_;
//	assert(!state.comparator_list_.empty());
	state.schema_=getSchema(dataflow.attribute_list_);
	BlockStreamIteratorBase* filter=new ExpandableBlockStreamFilter(state);
	return filter;
}

bool Filter::GetOptimalPhysicalPlan(Requirement requirement,PhysicalPlanDescriptor& physical_plan_descriptor, const unsigned & block_size){
	PhysicalPlanDescriptor physical_plan;
	std::vector<PhysicalPlanDescriptor> candidate_physical_plans;

	/* no requirement to the child*/
	if(child_->GetOptimalPhysicalPlan(Requirement(),physical_plan)){
		NetworkTransfer transfer=requirement.requireNetworkTransfer(physical_plan.dataflow);
		if(transfer==NONE){
			ExpandableBlockStreamFilter::State state;
			state.block_size_=block_size;
			state.child_=physical_plan.plan;
			state.qual_=qual_;
			state.colindex_=colindex_;
			state.comparator_list_=comparator_list_;
			state.v_ei_=exprArray_;
			Dataflow dataflow=getDataflow();
			state.schema_=getSchema(dataflow.attribute_list_);
			BlockStreamIteratorBase* filter=new ExpandableBlockStreamFilter(state);
			physical_plan.plan=filter;
			candidate_physical_plans.push_back(physical_plan);
		}
		else if((transfer==OneToOne)||(transfer==Shuffle)){
			/* the input data flow should be transfered in the network to meet the requirement
			 * TODO: implement OneToOne Exchange
			 * */
			ExpandableBlockStreamFilter::State state_f;
			state_f.block_size_=block_size;
			state_f.child_=physical_plan.plan;
			state_f.v_ei_=exprArray_;
			state_f.qual_=qual_;
			state_f.colindex_=colindex_;
			state_f.comparator_list_=comparator_list_;
			Dataflow dataflow=getDataflow();
			state_f.schema_=getSchema(dataflow.attribute_list_);
			BlockStreamIteratorBase* filter=new ExpandableBlockStreamFilter(state_f);
			physical_plan.plan=filter;

			physical_plan.cost+=physical_plan.dataflow.getAggregatedDatasize();

			ExpandableBlockStreamExchangeEpoll::State state;
			state.block_size_=block_size;
			state.child_=physical_plan.plan;//child_iterator;
			state.exchange_id_=IDsGenerator::getInstance()->generateUniqueExchangeID();
			state.schema_=getSchema(physical_plan.dataflow.attribute_list_);

			std::vector<NodeID> upper_id_list;
			if(requirement.hasRequiredLocations()){
				upper_id_list=requirement.getRequiredLocations();
			}
			else{
				if(requirement.hasRequiredPartitionFunction()){
					/* partition function contains the number of partitions*/
					PartitionFunction* partitoin_function=requirement.getPartitionFunction();
					upper_id_list=std::vector<NodeID>(NodeTracker::getInstance()->getNodeIDList().begin(),NodeTracker::getInstance()->getNodeIDList().begin()+partitoin_function->getNumberOfPartitions()-1);
				}
				else{
					//TODO: decide the degree of parallelism
					upper_id_list=NodeTracker::getInstance()->getNodeIDList();
				}
			}
			state.upper_ip_list_=convertNodeIDListToNodeIPList(upper_id_list);

			assert(requirement.hasReuiredPartitionKey());

			state.partition_schema_=partition_schema::set_hash_partition(this->getIndexInAttributeList(physical_plan.dataflow.attribute_list_,requirement.getPartitionKey()));
			assert(state.partition_schema_.partition_key_index>=0);

			std::vector<NodeID> lower_id_list=getInvolvedNodeID(physical_plan.dataflow.property_.partitioner);

			state.lower_ip_list_=convertNodeIDListToNodeIPList(lower_id_list);


			BlockStreamIteratorBase* exchange=new ExpandableBlockStreamExchangeEpoll(state);

			physical_plan.plan=exchange;

		}
		candidate_physical_plans.push_back(physical_plan);
	}


	if(child_->GetOptimalPhysicalPlan(requirement,physical_plan)){
		ExpandableBlockStreamFilter::State state;
		state.block_size_=block_size;
		state.child_=physical_plan.plan;
		state.v_ei_=exprArray_;
		state.qual_=qual_;
		state.colindex_=colindex_;
		state.comparator_list_=comparator_list_;
		Dataflow dataflow=getDataflow();
		state.schema_=getSchema(dataflow.attribute_list_);
		BlockStreamIteratorBase* filter=new ExpandableBlockStreamFilter(state);
		physical_plan.plan=filter;
		candidate_physical_plans.push_back(physical_plan);
	}

	physical_plan_descriptor=getBestPhysicalPlanDescriptor(candidate_physical_plans);

	if(requirement.passLimits(physical_plan_descriptor.cost))
		return true;
	else
		return false;

}


bool Filter::getcolindex(Dataflow dataflow)
{
	for(int i=0;i<dataflow.attribute_list_.size();i++)
	{
		colindex_[dataflow.attribute_list_[i].attrName]=i;
	}
	return true;
}
bool Filter::couldHashPruned(unsigned partition_id,const DataflowPartitioningDescriptor& part)const{
//	for(unsigned i=0;i<comparator_list_.size();i++){
//		const unsigned comparator_attribute_index=comparator_list_[i].get_index();
//		if(part.getPartitionKey()==dataflow.attribute_list_[comparator_attribute_index]){
//			if(comparator_list_[i].getCompareType()==Comparator::EQ){
//				if(partition_id==part.getPartitionKey().attrType->operate->getPartitionValue(comparator_list_[i].get_value(),part.getPartitionFunction())){
//
//				}
//				else{/* the data flow on this partition is filtered.*/
//					return true;
//
//				}
//
//
//			}
//
//		}
//	}
//	return false;
	for(unsigned i=0;i<condition_.getCompaisonNumber();i++){
		if(part.getPartitionKey()==condition_.attribute_list_[i]){
			if(comparator_list_[i].getCompareType()==Comparator::EQ){
				if(partition_id==part.getPartitionKey().attrType->operate->getPartitionValue(comparator_list_[i].get_value(),part.getPartitionFunction()->getNumberOfPartitions())){

				}
				else{
					return true;
				}
			}
		}
	}
	return false;

}
float Filter::predictSelectivity()const{
	float ret=1;

	/*
	 * In the current version, due to the lack of statistic information, we only use a factor
	 * 0.5 for each comparison. TODO: a more precious prediction is greatly needed.
	 */


	/**
	 * TODO: Before predicting the selectivity, we should first check whether there exist contradicted
	 * conditions (such as x=1 and x=4 is contradicted to each other).
	 */
	for(unsigned i=0;i<condition_.getCompaisonNumber();i++){
		float selectivity=1;
		const Attribute attr=condition_.attribute_list_[i];
		const TableStatistic* tab_stat=StatManager::getInstance()->getTableStatistic(attr.table_id_);
		if(tab_stat>0){
			/**
			 * Table statistics is available.
			 */
			unsigned long cardinality=tab_stat->number_of_tuples_;
			const AttributeStatistics* attr_stat=StatManager::getInstance()->getAttributeStatistic(attr);
			if(attr_stat>0){
				/**
				 * Attribute statistics is available.
				 */
				if(attr_stat->getHistogram()){
					/**
					 * Histogram is available.
					 * Selectivity prediction is based on the histogram
					 */
					const Histogram* histogram=attr_stat->getHistogram();
					/**
					 * In the current implementation, only point estimation is available, and hence we assume that the
					 * comparator is equal.
					 * TODO:
					 */
					unsigned long filtered_cardinality=Estimation::estEqualOper(attr.getID(),condition_.const_value_list_[i]);
					selectivity=(float)filtered_cardinality/cardinality;
				}
				else{
					/**
					 * No histogram is available. We just use the attribute cardinality to predict the selectivity.
					 * In such case, we assume each distinct value has the same frequency, i.e., cardinality/distinct_cardinality.
					 */
					const unsigned int distinct_card=attr_stat->getDistinctCardinality();
					unsigned long filtered_cardinality=(double)cardinality/distinct_card;
					selectivity=(double)filtered_cardinality/cardinality;
				}
			}
			else{
				/**
				 * Only Table statistic is available. We have to use the matic number.
				 */
				selectivity=0.1;
			}

		}
		else{
			/**
			 * No statistic is available, so we use the magic number.
			 */
			selectivity=0.1;
		}


		ret*=selectivity;
	}
	return ret;
}
Filter::Condition::Condition(const Condition& r){
	for(unsigned i=0;i<r.attribute_list_.size();i++){
		add(r.attribute_list_[i],r.comparison_list_[i],r.const_value_list_[i]);
	}
}
void Filter::Condition::add(const Attribute& attr,const AttributeComparator::comparison& com,const void*const & const_value){
	attribute_list_.push_back(attr);
	comparison_list_.push_back(com);
	const unsigned value_length=attr.attrType->get_length();
	void* value=malloc(value_length);
	attr.attrType->operate->assignment(const_value,value);
	const_value_list_.push_back(value);
}
void Filter::Condition::add(const Attribute& attr,const AttributeComparator::comparison& com,std::string str_exp){
	attribute_list_.push_back(attr);
	comparison_list_.push_back(com);
	const unsigned value_length=attr.attrType->get_length();
	void* value=malloc(value_length);
	attr.attrType->operate->toValue(value,str_exp.c_str());
	const_value_list_.push_back(value);
}
void Filter::Condition::print(int level)const{
	printf("%*.sFilter:\n",level*8," ");
        for(unsigned i=0;i<attribute_list_.size();i++){
        	printf("%*.s",level*8,"    ");
                printf("%s",attribute_list_[i].attrName.c_str());
                switch(comparison_list_[i]){
                        case AttributeComparator::L :{
                                printf("<");
                                break;
                        }
                        case AttributeComparator::LEQ:{
                                        printf("<=");
                                        break;
                                }
                        case AttributeComparator::EQ:{
                                        printf("=");
                                        break;
                                }
                        case AttributeComparator::NEQ:{
                                        printf("!=");
                                        break;
                                }
                        case AttributeComparator::G:{
                                        printf(">");
                                        break;
                                }
                        case AttributeComparator::GEQ:{
                                        printf(">=");
                                        break;
                                }
                        default:{
                                printf("?");
                        }
                }
                printf("%s\n",attribute_list_[i].attrType->operate->toString(const_value_list_[i]).c_str());
        }
//        child_->print(level+1);
}

unsigned Filter::Condition::getCompaisonNumber()const{
	return attribute_list_.size();
}
Filter::Condition::~Condition(){

	for(unsigned i=0;i<const_value_list_.size();i++){
		free(const_value_list_[i]);
	}
}
void Filter::generateComparatorList(const Dataflow& dataflow){
	for(unsigned i=0;i<condition_.getCompaisonNumber();i++){
		int attribute_index=0;
		for(;attribute_index<dataflow.attribute_list_.size();attribute_index++){
			if(condition_.attribute_list_[i]==dataflow.attribute_list_[attribute_index]){
				break;
			}
		}
		if(attribute_index==dataflow.attribute_list_.size()){
			printf("the filter condition %s does match any input attribute! Rechech the filter condition or the filter transformation module.\n",condition_.attribute_list_[i].attrName.c_str());
			assert(false);
		}
		AttributeComparator filter(*dataflow.attribute_list_[attribute_index].attrType,condition_.comparison_list_[i],attribute_index,condition_.const_value_list_[i]);
		comparator_list_.push_back(filter);
//		printf("************** pushed ***************\n");
	}
//	printf("comparator size=%d\n",comparator_list_.size());
	assert(condition_.comparison_list_.size()==comparator_list_.size());
}
void Filter::print(int level)const{
//	condition_.print(level);
	printf("filter:\n");
	for(int i=0;i<qual_.size();i++)
	{
		printf("	%s\n",qual_[i]->alias.c_str());
	}
	child_->print(level+1);
}
