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
Filter::Filter(std::vector<FilterIterator::AttributeComparator> ComparatorList,LogicalOperator* child )
:comparator_list_(ComparatorList),child_(child){

}

Filter::~Filter() {
	// TODO Auto-generated destructor stub
}

Dataflow Filter::getDataflow(){
	/** In the currently implementation, we assume that the boolean operator
	 * between each AttributeComparator is "AND".
	 */

	Dataflow dataflow=child_->getDataflow();
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
				const unsigned before_filter_datasize=dataflow.property_.partitioner.getPartition(i)->getDataSize();
				const unsigned after_filter_datasize=before_filter_datasize*predictSelectivity();
				dataflow.property_.partitioner.getPartition(i)->setDataSize(after_filter_datasize);
			}
		}
	}

	return dataflow;
}
BlockStreamIteratorBase* Filter::getIteratorTree(const unsigned& blocksize){
	BlockStreamIteratorBase* child_iterator=child_->getIteratorTree(blocksize);
	ExpandableBlockStreamFilter::State state;
	state.block_size_=blocksize;
	state.child_=child_iterator;
	state.comparator_list_=comparator_list_;
	Dataflow dataflow=getDataflow();
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
			state.comparator_list_=comparator_list_;
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
			state_f.comparator_list_=comparator_list_;
			Dataflow dataflow=getDataflow();
			state_f.schema_=getSchema(dataflow.attribute_list_);
			BlockStreamIteratorBase* filter=new ExpandableBlockStreamFilter(state_f);
			physical_plan.plan=filter;


			physical_plan.cost+=physical_plan.dataflow.getAggregatedDatasize();

			ExpandableBlockStreamExchangeEpoll::State state;
			state.block_size=block_size;
			state.child=physical_plan.plan;//child_iterator;
			state.exchange_id=IDsGenerator::getInstance()->generateUniqueExchangeID();
			state.schema=getSchema(physical_plan.dataflow.attribute_list_);

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
			state.upper_ip_list=convertNodeIDListToNodeIPList(upper_id_list);

			assert(requirement.hasReuiredPartitionKey());

			state.partition_key_index=this->getIndexInAttributeList(physical_plan.dataflow.attribute_list_,requirement.getPartitionKey());
			assert(state.partition_key_index>=0);

			std::vector<NodeID> lower_id_list=getInvolvedNodeID(physical_plan.dataflow.property_.partitioner);

			state.lower_ip_list=convertNodeIDListToNodeIPList(lower_id_list);


			BlockStreamIteratorBase* exchange=new ExpandableBlockStreamExchangeEpoll(state);

			physical_plan.plan=exchange;

		}
		candidate_physical_plans.push_back(physical_plan);
	}


	if(child_->GetOptimalPhysicalPlan(requirement,physical_plan)){
		ExpandableBlockStreamFilter::State state;
		state.block_size_=block_size;
		state.child_=physical_plan.plan;
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
				if(partition_id==part.getPartitionKey().attrType->operate->getPartitionValue(comparator_list_[i].get_value(),part.getPartitionFunction())){

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
	for(unsigned i=0;i<condition_.getCompaisonNumber();i++){
		ret*=0.5;
	}
	return ret;
}
Filter::Condition::Condition(const Condition& r){
	for(unsigned i=0;i<r.attribute_list_.size();i++){
		add(r.attribute_list_[i],r.comparison_list_[i],r.const_value_list_[i]);
	}
}
void Filter::Condition::add(const Attribute& attr,const FilterIterator::AttributeComparator::comparison& com,const void*const & const_value){
	attribute_list_.push_back(attr);
	comparison_list_.push_back(com);
	const unsigned value_length=attr.attrType->get_length();
	void* value=malloc(value_length);
	attr.attrType->operate->assignment(const_value,value);
	const_value_list_.push_back(value);
}
void Filter::Condition::add(const Attribute& attr,const FilterIterator::AttributeComparator::comparison& com,std::string str_exp){
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
                        case FilterIterator::AttributeComparator::L :{
                                printf("<");
                                break;
                        }
                        case FilterIterator::AttributeComparator::LEQ:{
                                        printf("<=");
                                        break;
                                }
                        case FilterIterator::AttributeComparator::EQ:{
                                        printf("=");
                                        break;
                                }
                        case FilterIterator::AttributeComparator::NEQ:{
                                        printf("!=");
                                        break;
                                }
                        case FilterIterator::AttributeComparator::G:{
                                        printf(">");
                                        break;
                                }
                        case FilterIterator::AttributeComparator::GEQ:{
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
Filter::Filter(const Condition& condition,LogicalOperator*  child):condition_(condition),child_(child){

//	condition_.print();


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
			printf("the filter condition does match any input attribute! Rechech the filter condition or the filter transformation module.\n");
			assert(false);
		}
		FilterIterator::AttributeComparator filter(*dataflow.attribute_list_[attribute_index].attrType,condition_.comparison_list_[i],attribute_index,condition_.const_value_list_[i]);
		comparator_list_.push_back(filter);
	}
}
void Filter::print(int level)const{
	condition_.print(level);
	child_->print(level+1);
}
