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

Filter::Filter(LogicalOperator *child, vector<QNode *> qual) :
  child_(child), qual_(qual) {
    setOperatortype(l_filter);
    // initialize_type_cast_functions();
}

Filter::Filter(std::vector<AttributeComparator> ComparatorList,
  LogicalOperator* child) :
  comparator_list_(ComparatorList), child_(child) {
    assert(!comparator_list_.empty());
    setOperatortype(l_filter);
}

Filter::~Filter() {
  if (child_ > 0)  delete child_;
}

//  getDataflow基本没有问题，有待couldHashPruned重写来进行优化；在改进的情况，或许会有AttributeComparater之间的操作是OR，以适应更多种类的sql语句
Dataflow Filter::getDataflow() {
	/** In the currently implementation, we assume that the boolean operator
	 * between each AttributeComparator is "AND".
	 */

	Dataflow dataflow = child_->getDataflow(); //<获取子算子的Dataflow
//	if (comparator_list_.size() == 0)
//		generateComparatorList(dataflow); //<不需要的代码
	if (dataflow.isHashPartitioned()) { //<判断拿到的dataflow是否被哈希划分过
		for (unsigned i = 0;
				i < dataflow.property_.partitioner.getNumberOfPartitions();
				i++) {
			if (couldHashPruned(i, dataflow.property_.partitioner)) //is filtered
					{ //<如果某部分已经被哈希划分（也就是被filter过），需要对每个划分设置Filtered标志，这样不需要推测基数了（HASH会将值相同的元组放在一起，只要顺序查询就可以计算出基数）
				dataflow.property_.partitioner.getPartition(i)->setFiltered();
			} else { //<如果没有被filter过，获取原有基数，乘以推测系数再将值赋回
				/*
				 * should predict the volume of data that passes the filter.
				 * TODO: a precious prediction is needed based on the statistic of
				 * the input data, which may be maintained in the catalog module.
				 */
				const unsigned before_filter_cardinality =
						dataflow.property_.partitioner.getPartition(i)->getDataCardinality();
				const unsigned after_filter_cardinality =
						before_filter_cardinality * predictSelectivity();
				dataflow.property_.partitioner.getPartition(i)->setDataCardinality(
						after_filter_cardinality);
			}
		}
	}
	getcolindex(dataflow);
	Schema *input_ = getSchema(dataflow.attribute_list_); //<获取Schema
	for (int i = 0; i < qual_.size(); i++) { //<初始化逻辑执行计划的表达式，传入qual及colindex
		InitExprAtLogicalPlan(qual_[i], t_boolean, colindex_, input_);
	}
	return dataflow; //继续将dataflow传给上层
}
BlockStreamIteratorBase* Filter::getIteratorTree(const unsigned& blocksize) {
	Dataflow dataflow = getDataflow(); //<获取Dataflow
	BlockStreamIteratorBase* child_iterator = child_->getIteratorTree(
			blocksize); //<获取子算子的IteratorTree
	ExpandableBlockStreamFilter::State state; //<准备向物理层传送数据状态
	state.block_size_ = blocksize;
	state.child_ = child_iterator;
	state.qual_ = qual_;
	state.colindex_ = colindex_;
//	assert(!comparator_list_.empty());
	state.comparator_list_ = comparator_list_;
//	assert(!state.comparator_list_.empty());
	state.schema_ = getSchema(dataflow.attribute_list_);
	BlockStreamIteratorBase* filter = new ExpandableBlockStreamFilter(state); //<向物理层传递
	return filter;
}
bool Filter::GetOptimalPhysicalPlan(Requirement requirement,
		PhysicalPlanDescriptor& physical_plan_descriptor,
		const unsigned & block_size) {
	PhysicalPlanDescriptor physical_plan;
	std::vector<PhysicalPlanDescriptor> candidate_physical_plans;

	/* no requirement to the child*/
	if (child_->GetOptimalPhysicalPlan(Requirement(), physical_plan)) {
		NetworkTransfer transfer = requirement.requireNetworkTransfer(
				physical_plan.dataflow);
		if (transfer == NONE) {
			ExpandableBlockStreamFilter::State state;
			state.block_size_ = block_size;
			state.child_ = physical_plan.plan;
			state.qual_ = qual_;
			state.colindex_ = colindex_;
			state.comparator_list_ = comparator_list_;
//			state.v_ei_ = exprArray_;
			Dataflow dataflow = getDataflow();
			state.schema_ = getSchema(dataflow.attribute_list_);
			BlockStreamIteratorBase* filter = new ExpandableBlockStreamFilter(
					state);
			physical_plan.plan = filter;
			candidate_physical_plans.push_back(physical_plan);
		} else if ((transfer == OneToOne) || (transfer == Shuffle)) {
			/* the input data flow should be transfered in the network to meet the requirement
			 * TODO: implement OneToOne Exchange
			 * */
			ExpandableBlockStreamFilter::State state_f;
			state_f.block_size_ = block_size;
			state_f.child_ = physical_plan.plan;
//			state_f.v_ei_ = exprArray_;
			state_f.qual_ = qual_;
			state_f.colindex_ = colindex_;
			state_f.comparator_list_ = comparator_list_;
			Dataflow dataflow = getDataflow();
			state_f.schema_ = getSchema(dataflow.attribute_list_);
			BlockStreamIteratorBase* filter = new ExpandableBlockStreamFilter(
					state_f);
			physical_plan.plan = filter;

			physical_plan.cost +=
					physical_plan.dataflow.getAggregatedDatasize();

			ExpandableBlockStreamExchangeEpoll::State state;
			state.block_size_ = block_size;
			state.child_ = physical_plan.plan; //child_iterator;
			state.exchange_id_ =
					IDsGenerator::getInstance()->generateUniqueExchangeID();
			state.schema_ = getSchema(physical_plan.dataflow.attribute_list_);

			std::vector<NodeID> upper_id_list;
			if (requirement.hasRequiredLocations()) {
				upper_id_list = requirement.getRequiredLocations();
			} else {
				if (requirement.hasRequiredPartitionFunction()) {
					/* partition function contains the number of partitions*/
					PartitionFunction* partitoin_function =
							requirement.getPartitionFunction();
					upper_id_list =
							std::vector<NodeID>(
									NodeTracker::getInstance()->getNodeIDList().begin(),
									NodeTracker::getInstance()->getNodeIDList().begin()
											+ partitoin_function->getNumberOfPartitions()
											- 1);
				} else {
					//TODO: decide the degree of parallelism
					upper_id_list = NodeTracker::getInstance()->getNodeIDList();
				}
			}
			state.upper_id_list_ = upper_id_list;

			assert(requirement.hasReuiredPartitionKey());

			state.partition_schema_ = partition_schema::set_hash_partition(
					this->getIndexInAttributeList(
							physical_plan.dataflow.attribute_list_,
							requirement.getPartitionKey()));
			assert(state.partition_schema_.partition_key_index >= 0);

			std::vector<NodeID> lower_id_list = getInvolvedNodeID(
					physical_plan.dataflow.property_.partitioner);

			state.lower_id_list_ = lower_id_list;

			BlockStreamIteratorBase* exchange =
					new ExpandableBlockStreamExchangeEpoll(state);

			physical_plan.plan = exchange;

		}
		candidate_physical_plans.push_back(physical_plan);
	}

	if (child_->GetOptimalPhysicalPlan(requirement, physical_plan)) {
		ExpandableBlockStreamFilter::State state;
		state.block_size_ = block_size;
		state.child_ = physical_plan.plan;
//		state.v_ei_ = exprArray_;
		state.qual_ = qual_;
		state.colindex_ = colindex_;
		state.comparator_list_ = comparator_list_;
		Dataflow dataflow = getDataflow();
		state.schema_ = getSchema(dataflow.attribute_list_);
		BlockStreamIteratorBase* filter = new ExpandableBlockStreamFilter(
				state);
		physical_plan.plan = filter;
		candidate_physical_plans.push_back(physical_plan);
	}

	physical_plan_descriptor = getBestPhysicalPlanDescriptor(
			candidate_physical_plans);

	if (requirement.passLimits(physical_plan_descriptor.cost))
		return true;
	else
		return false;

}
// getcolindex不知道为什么用的是bool类型，或许考虑改为void
bool Filter::getcolindex(Dataflow dataflow) {
	for (int i = 0; i < dataflow.attribute_list_.size(); i++) { //<在获得的属性列表中遍历，并把属性和列值存入colindex中
		colindex_[dataflow.attribute_list_[i].attrName] = i;
	}
	return true;
}
// couldHashPruned待修改,condition_已经被删除了
/* TODO(Anybody)
 * couldHashPruned存在一定价值，在getDataflow的时候可以省去对一部分代码进行基数的推测修改
 */
bool Filter::couldHashPruned(unsigned partition_id,
		const DataflowPartitioningDescriptor& part) const {
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
	// actually not used in current version. so can be ignored
/*
 	for (unsigned i = 0; i < condition_.getCompaisonNumber(); i++) {
		if (part.getPartitionKey() == condition_.attribute_list_[i]) {
			if (comparator_list_[i].getCompareType() == Comparator::EQ) {
				if (partition_id
						== part.getPartitionKey().attrType->operate->getPartitionValue(
								comparator_list_[i].get_value(),
								part.getPartitionFunction()->getNumberOfPartitions())) {

				} else {
					return true;
				}
			}
		}
	}
*/
	return false;

}
float Filter::predictSelectivity() const {
	float ret = 1;

	/*
	 * In the current version, due to the lack of statistic information, we only use a factor
	 * 0.5 for each comparison. TODO: a more precious prediction is greatly needed.
	 */

	/**
	 * TODO: Before predicting the selectivity, we should first check whether there exist contradicted
	 * conditions (such as x=1 and x=4 is contradicted to each other).
	 */

	// because condition will be abandoned, the following code is actually not used
//	for (unsigned i = 0; i < condition_.getCompaisonNumber(); i++) {
//		float selectivity = 1;
//		const Attribute attr = condition_.attribute_list_[i];
//		const TableStatistic* tab_stat =
//				StatManager::getInstance()->getTableStatistic(attr.table_id_);
//		if (tab_stat > 0) {
//			 //Table statistics is available.
//
//			unsigned long cardinality = tab_stat->number_of_tuples_;
//			const AttributeStatistics* attr_stat =
//					StatManager::getInstance()->getAttributeStatistic(attr);
//			if (attr_stat > 0) {
//				 // Attribute statistics is available.
//				if (attr_stat->getHistogram()) {
//					/**
//					 * Histogram is available.
//					 * Selectivity prediction is based on the histogram
//					 */
//					const Histogram* histogram = attr_stat->getHistogram();
//					/**
//					 * In the current implementation, only point estimation is available, and hence we assume that the
//					 * comparator is equal.
//					 * TODO:
//					 */
//					unsigned long filtered_cardinality =
//							Estimation::estEqualOper(attr.getID(),
//									condition_.const_value_list_[i]);
//					selectivity = (float) filtered_cardinality / cardinality;
//				} else {
//					/**
//					 * No histogram is available. We just use the attribute cardinality to predict the selectivity.
//					 * In such case, we assume each distinct value has the same frequency, i.e., cardinality/distinct_cardinality.
//					 */
//					const unsigned int distinct_card =
//							attr_stat->getDistinctCardinality();
//					unsigned long filtered_cardinality = (double) cardinality
//							/ distinct_card;
//					selectivity = (double) filtered_cardinality / cardinality;
//				}
//			} else {
//				/**
//				 * Only Table statistic is available. We have to use the matic number.
//				 */
//				selectivity = 0.1;
//			}
//
//		} else {
//			/**
//			 * No statistic is available, so we use the magic number.
//			 */
//			selectivity = 0.1;
//		}
//
//		ret *= selectivity;
//	}

	return ret;
}
void Filter::print(int level) const {
//	condition_.print(level);
	printf("filter:\n");
	for (int i = 0; i < qual_.size(); i++) {
		printf("	%s\n", qual_[i]->alias.c_str());
	}
	child_->print(level + 1);
}
