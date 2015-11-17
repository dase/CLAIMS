///*
// * LogicalIndexFilter.cpp
// *
// *  Created on: Mar 28, 2014
// *      Author: scdong
// */
//
//#include "LogicalIndexFilter.h"
//#include "IndexManager.h"
//#include "IndexScanIterator.h"
//#include "../BlockStreamIterator/ParallelBlockStreamIterator/physical_projection_scan.h"
//#include "../BlockStreamIterator/ParallelBlockStreamIterator/physical_filter.h"
//
//LogicalIndexFilter::Condition::Condition(const Condition& r){
//	for(unsigned i=0;i<r.attribute_list_.size();i++){
//		add(r.attribute_list_[i],r.comparison_list_[i],r.const_value_list_[i]);
//	}
//}
//void LogicalIndexFilter::Condition::add(const Attribute& attr,const comparison& com,const void*const & const_value){
//	attribute_list_.push_back(attr);
//	comparison_list_.push_back(com);
//	const unsigned value_length=attr.attrType->get_length();
//	void* value=malloc(value_length);		//newmalloc
//	attr.attrType->operate->assignment(const_value,value);
//	const_value_list_.push_back(value);
//}
//void LogicalIndexFilter::Condition::add(const Attribute& attr,const comparison& com,std::string str_exp){
//	attribute_list_.push_back(attr);
//	comparison_list_.push_back(com);
//	const unsigned value_length=attr.attrType->get_length();
//	void* value=malloc(value_length);		//newmalloc
//	attr.attrType->operate->toValue(value,str_exp.c_str());
//	const_value_list_.push_back(value);
//}
//void LogicalIndexFilter::Condition::print(int level)const{
//	printf("%*.sFilter:\n",level*8," ");
//        for(unsigned i=0;i<attribute_list_.size();i++){
//        	printf("%*.s",level*8,"    ");
//                printf("%s",attribute_list_[i].attrName.c_str());
//                switch(comparison_list_[i]){
//                        case FilterIterator::AttributeComparator::L :{
//                                printf("<");
//                                break;
//                        }
//                        case FilterIterator::AttributeComparator::LEQ:{
//                                        printf("<=");
//                                        break;
//                                }
//                        case FilterIterator::AttributeComparator::EQ:{
//                                        printf("=");
//                                        break;
//                                }
//                        case FilterIterator::AttributeComparator::NEQ:{
//                                        printf("!=");
//                                        break;
//                                }
//                        case FilterIterator::AttributeComparator::G:{
//                                        printf(">");
//                                        break;
//                                }
//                        case FilterIterator::AttributeComparator::GEQ:{
//                                        printf(">=");
//                                        break;
//                                }
//                        default:{
//                                printf("?");
//                        }
//                }
//                printf("%s\n",attribute_list_[i].attrType->operate->toString(const_value_list_[i]).c_str());
//        }
////        child_->print(level+1);
//}
//
//unsigned LogicalIndexFilter::Condition::getCompaisonNumber()const{
//	return attribute_list_.size();
//}
//LogicalIndexFilter::Condition::~Condition(){
//
//	for(unsigned i=0;i<const_value_list_.size();i++){
//		free(const_value_list_[i]);
//	}
//}
//
//LogicalIndexFilter::LogicalIndexFilter(const Condition &condition, LogicalOperator* child)
//: condition_(condition), child_(child){
//
//}
//
//LogicalIndexFilter::~LogicalIndexFilter() {
//	// TODO Auto-generated destructor stub
//	if (child_ > 0)
//		child_->~LogicalOperator();
//}
//
//Dataflow LogicalIndexFilter::getDataflow()
//{
//	Dataflow dataflow = child_->getDataflow();
//	if(comparator_list_.size()==0)
//		generateComparatorList(dataflow);
//	return dataflow;
//}
//
//BlockStreamIteratorBase* LogicalIndexFilter::getIteratorTree(const unsigned& blocksize)
//{
//	Dataflow dataflow = getDataflow();
//
//	if (child_->getOperatorType() == l_scan)
//	{
//		/*
//		 * scan the condition_.attribute_list_ to find whether there are attributes which are indexed by a CSB+ Tree index
//		 * ideally we should find the indexed attribute whose filter ratio is the least of all, but now we just find the first attribute which is indexed
//		 */
//		unsigned i = 0;
//		for (i = 0; i < condition_.attribute_list_.size(); i++) {
//			//found the first attribute in filter condition which is indexed
//			if (IndexManager::getInstance()->isIndexExist(condition_.attribute_list_[i]))
//				break;
//		}
//		if (i != condition_.attribute_list_.size())
//		{
//			//constructing the index_scan according to the filter condition of condition_.attribute_list_[i]
//			vector<Attribute>::iterator iter_attr = condition_.attribute_list_.begin();
//			vector<FilterIterator::AttributeComparator::comparison>::iterator iter_comp = condition_.comparison_list_.begin();
//			vector<void*>::iterator iter_value = condition_.const_value_list_.begin();
//			IndexScanIterator::State state;
////			state.projection_id_ = ((ExpandableBlockStreamProjectionScan*) child_)->state_.projection_id_;
//			state.schema_ = getSchema(dataflow.attribute_list_);
//			state.block_size_ = blocksize;
//			state.index_id_ = IndexManager::getInstance()->getIndexID(condition_.attribute_list_[i]);
//			while (iter_attr != condition_.attribute_list_.end())
//			{
//				assert(state.index_id_ != -1);
//			}
//			//use this attribute to be the child index_scan_filter, and delete the filter condition in condition_
//		}
//	}
//
//	// the child_iterator isn't scan or there isn't indexed attribute in filter condition: original LogicalFilter operator
//	BlockStreamIteratorBase* child_iterator = child_->getIteratorTree(blocksize);
//	ExpandableBlockStreamFilter::State state;
//	state.block_size_ = blocksize;
//	state.child_ = child_iterator;
//	state.comparator_list_ = comparator_list_;
//	assert(!state.comparator_list_.empty());
//	state.schema_ = getSchema(dataflow.attribute_list_);
//	BlockStreamIteratorBase* filter = new ExpandableBlockStreamFilter(state);
//	return filter;
//}
//
//bool LogicalIndexFilter::GetOptimalPhysicalPlan(Requirement requirement,PhysicalPlanDescriptor& physical_plan_descriptor, const unsigned & block_size)
//{
//	assert(false);
//	return false;
//}
//
//void LogicalIndexFilter::print(int level)const
//{
//	condition_.print(level);
//	child_->print(level+1);
//}
//
//void LogicalIndexFilter::generateComparatorList(const Dataflow& dataflow)
//{
//	for(unsigned i=0;i<condition_.getCompaisonNumber();i++){
//		int attribute_index=0;
//		for(;attribute_index<dataflow.attribute_list_.size();attribute_index++){
//			if(condition_.attribute_list_[i]==dataflow.attribute_list_[attribute_index]){
//				break;
//			}
//		}
//		if(attribute_index==dataflow.attribute_list_.size()){
//			printf("the filter condition does match any input attribute! Rechech the filter condition or the filter transformation module.\n");
//			assert(false);
//		}
//		FilterIterator::AttributeComparator filter(*dataflow.attribute_list_[attribute_index].attrType,condition_.comparison_list_[i],attribute_index,condition_.const_value_list_[i]);
//		comparator_list_.push_back(filter);
//		printf("************** pushed ***************\n");
//	}
//	printf("comparator size=%d\n",comparator_list_.size());
//	assert(condition_.comparison_list_.size()==comparator_list_.size());
//}
