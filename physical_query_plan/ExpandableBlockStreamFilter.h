/*
 * ExpandableBlockStreamFilter.h
 *
 *  Created on: Aug 28, 2013
 *      Author: wangli
 */

#ifndef EXPANDABLEBLOCKSTREAMFILTER_H_
#define EXPANDABLEBLOCKSTREAMFILTER_H_
#include <list>
#ifdef DMALLOC
#include "dmalloc.h"
#endif
#include "../physical_query_plan/BlockStreamIteratorBase.h"
#include "../physical_query_plan/physical_operator.h"
#include "../../common/Schema/Schema.h"
#include "../../common/Comparator.h"
#include "../../common/Block/BlockStream.h"
#include "../../utility/lock.h"
#include "../../common/AttributeComparator.h"
#include "../../common/ExpressionItem.h"
#include "../../common/Mapping.h"
#include "../catalog/attribute.h"
#include "../physical_query_plan/BlockStreamProjectIterator.h"
#include <map>
#include <string>
#include <boost/serialization/map.hpp>
#include "../../common/Expression/qnode.h"
#include "../../codegen/ExpressionGenerator.h"
//typedef vector<ExpressionItem> ExpressItem_List;
class ExpandableBlockStreamFilter:public PhysicalOperator {
public:
	class filter_thread_context:public ThreadContext{
	public:
		BlockStreamBase* block_for_asking_;
		BlockStreamBase* temp_block_;
		BlockStreamBase::BlockStreamTraverseIterator* block_stream_iterator_;
		vector<QNode *>thread_qual_;
		~filter_thread_context();
	};
	/* struct to hold the remaining data when the next is returned but the block from the child
	 *  iterator is not exhausted.*/
	struct remaining_block{
		remaining_block(BlockStreamBase* block,BlockStreamBase::BlockStreamTraverseIterator* iterator):block(block),iterator(iterator){};
		remaining_block():block(0),iterator(0){};
		BlockStreamBase* block;
		BlockStreamBase::BlockStreamTraverseIterator* iterator;
	};



	class State{
	public:
		friend class ExpandableBlockStreamFilter;
		State(Schema* schema, BlockStreamIteratorBase* child,vector<QNode *>qual,map<string,int>colindex,unsigned block_size );
		State(Schema* s, BlockStreamIteratorBase* child,std::vector<AttributeComparator> comparator_list,unsigned block_size );
		State(){};
	public:
		Schema* schema_;
		BlockStreamIteratorBase* child_;
		unsigned block_size_;
		vector<QNode *>qual_;
		std::vector<AttributeComparator> comparator_list_;
		vector<ExpressItem_List> v_ei_;
		map<string,int>colindex_;
	private:
		friend class boost::serialization::access;
		template<class Archive>
		void serialize(Archive & ar, const unsigned int version){
			ar & schema_ &child_ & block_size_ & qual_& comparator_list_&colindex_ ;
		}
	};


	ExpandableBlockStreamFilter(State state);
	ExpandableBlockStreamFilter();
	virtual ~ExpandableBlockStreamFilter();
	bool Open(const PartitionOffset& part_off);
	bool Next(BlockStreamBase* block);
	bool Close();
    void Print();
private:
	void process_logic(BlockStreamBase* block,filter_thread_context * tc);
private:
	ThreadContext* CreateContext();
	//ExecEvalQual(tc->thread_qual_, tuple_from_child,	state_.schema_);
	typedef void(*filter_func)(bool& ret, void* tuple,expr_func func_gen, Schema* schema,vector<QNode *> thread_qual_);
	static void computeFilter(bool& ret, void* tuple, expr_func func_gen, Schema* schema,vector<QNode *> thread_qual_);
	static void computeFilterwithGeneratedCode(bool& ret, void* tuple, expr_func func_gen, Schema* schema,vector<QNode *>);
private:
	State state_;
	map<string,int>colindex;

	unsigned long tuple_after_filter_;
//	vector<QNode *>qual_;//store the transfromed Qnode
	Lock lock_;
	filter_func ff_;
	expr_func generated_filter_function_;
	filter_process_func generated_filter_processing_fucntoin_;
	/* the following code is for boost serialization*/
private:
	friend class boost::serialization::access;
	template<class Archive>
	void serialize(Archive & ar, const unsigned int version){
		ar & boost::serialization::base_object<BlockStreamIteratorBase>(*this) &state_;
	}
};

#endif /* EXPANDABLEBLOCKSTREAMFILTER_H_ */
