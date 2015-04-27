/*
 * BlockStreamProjectIterator.h
 *
 *  Created on: 2014-2-17
 *      Author: casa
 */

#ifndef BLOCKSTREAMPROJECTITERATOR_H_
#define BLOCKSTREAMPROJECTITERATOR_H_

#include "../BlockStreamIteratorBase.h"
#include "../../common/ExpressionCalculator.h"
#include "../../common/ExpressionItem.h"
#include "../../common/Mapping.h"
#include "../../configure.h"
#include "../../common/Expression/qnode.h"
#include "../../common/Expression/initquery.h"
#include "../../common/Expression/execfunc.h"

#include <iostream>
#include <vector>
#include <map>
#include <list>

#include "../ExpandableBlockStreamIteratorBase.h"
#ifdef DMALLOC
#include "dmalloc.h"
#endif
using namespace std;

typedef vector<ExpressionItem> ExpressItem_List;

class BlockStreamProjectIterator:public ExpandableBlockStreamIteratorBase {
public:
	class project_thread_context:public thread_context{
	public:
		BlockStreamBase* block_for_asking_;
		BlockStreamBase* temp_block_;
		BlockStreamBase::BlockStreamTraverseIterator* block_stream_iterator_;
		vector<QNode *>thread_qual_;
		~project_thread_context(){
			delete block_for_asking_;
			delete temp_block_;
			delete block_stream_iterator_;
			for (int i =0 ;i<thread_qual_.size();i++){
				delete thread_qual_[i];
			}
		}
	};

	class State{
		friend class BlockStreamProjectIterator;
	public:
		State(Schema *input, Schema* output, BlockStreamIteratorBase * children, unsigned blocksize, Mapping map,vector<ExpressItem_List> v_ei,vector<QNode *>exprTree);
		State(Schema *input, Schema* output, BlockStreamIteratorBase * children, unsigned blocksize, Mapping map,vector<ExpressItem_List> v_ei);
		State(){};
	public:
		Schema *input_;
		Schema *output_;
		/* Recently, the expression is supporting the reduce the number of the input table!!!
		 * TODO: support the multi-to-multi between the input table and the select list, this expr
		 * is the result of the getIteratorTree to construct a schema. getDataflow() can generate a
		 * schema by using the SQLExpression and Expression can be computed by SQLExpression
		 * */
		vector<ExpressItem_List> v_ei_;
		Mapping map_;


		vector<QNode *>exprTree_;
		unsigned block_size_;
		BlockStreamIteratorBase *child_;

		friend class boost::serialization::access;
		template<class Archive>
		void serialize(Archive & ar, const unsigned int version){
			ar & input_ & output_ & child_ & map_ & block_size_ & v_ei_ &exprTree_;
		}
	};
	BlockStreamProjectIterator(State state);
	BlockStreamProjectIterator();
	virtual ~BlockStreamProjectIterator();

	bool open(const PartitionOffset& partition_offset=0);
	bool next(BlockStreamBase *block);
	bool close();
void print();
private:

	thread_context* createContext();

	bool copyNewValue(void *tuple,void *result,int length);

	bool copyColumn(void *&tuple,ExpressionItem &result,int length);
	void process_logic(BlockStreamBase* block, project_thread_context* tc);
private:

	State state_;

private:
	friend class boost::serialization::access;
	template<class Archive>
	void serialize(Archive & ar, const unsigned int version){
		ar & boost::serialization::base_object<BlockStreamIteratorBase>(*this) & state_;
	}
};

#endif /* BLOCKSTREAMPROJECTITERATOR_H_ */
