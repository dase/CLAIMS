/*
 * Analyzer.cpp
 *
 *  Created on: 2014年1月20日
 *      Author: volt
 */

#include "Analyzer.h"

#include <algorithm>
#include <cstdlib>
#include <vector>

#include "../../Block/BlockStream.h"
#include "../../Block/DynamicBlockBuffer.h"
#include "../../Block/ResultSet.h"
#include "../../BlockStreamIterator/ParallelBlockStreamIterator/BlockStreamAggregationIterator.h"
#include "../../data_type.h"
#include "../../LogicalQueryPlan/Aggregation.h"
#include "../../LogicalQueryPlan/LogicalQueryPlanRoot.h"
#include "../../LogicalQueryPlan/Scan.h"
#include "../Catalog.h"
#include "../table.h"
#include "StatManager.h"

using std::map;

typedef void* TuplePtr;

inline int getFrequency(const void *tuple, const column_type *type) {

	return *((int*) (((char*) tuple) + type->get_length()));
}

Analyzer::Analyzer() {
	// TODO Auto-generated constructor stub

}

Analyzer::~Analyzer() {
	// TODO Auto-generated destructor stub
}

int compare(const void *a, const void *b, void *arg) {

	return ((Operate *) arg)->compare( *(void **)a, *(void **)b);
}

void Analyzer::analyse(const AttributeID &attrID) {

	Catalog *catalog = Catalog::getInstance();

	TableDescriptor* table = catalog->getTable(attrID.table_id);
	ProjectionDescriptor * projection = NULL;

	unsigned pidSize = table->getNumberOfProjection();
	const Attribute attr = table->getAttribute(attrID.offset);

	for (unsigned i = 0; i < pidSize; ++i) {
		if (table->getProjectoin(i)->hasAttribute(attr)) {
			projection = table->getProjectoin(i);
			break;
		}
	}

	std::vector<Attribute> group_by_attributes;
	std::vector<Attribute> aggregation_attributes;

	group_by_attributes.push_back(attr);
	aggregation_attributes.push_back(attr);

	std::vector<BlockStreamAggregationIterator::State::aggregation> aggregation_function;
	aggregation_function.push_back(
			BlockStreamAggregationIterator::State::count);

	LogicalOperator* sb_payload_scan = new LogicalScan(projection);

	LogicalOperator* aggregation = new Aggregation(group_by_attributes,
			aggregation_attributes, aggregation_function, sb_payload_scan);
	const NodeID collector_node_id = 0;

	LogicalOperator* root = new LogicalQueryPlanRoot(collector_node_id,
			aggregation, LogicalQueryPlanRoot::RESULTCOLLECTOR);

	BlockStreamIteratorBase* collector = root->getIteratorTree(
			1024 * 64 - sizeof(unsigned));

	collector->open();
	collector->next(0);
	collector->close();
	ResultSet resultset = collector->getResultSet();
	ResultSet::Iterator it = resultset.createIterator();

	BlockStreamBase* block;
	void* tuple;
	BlockStreamBase::BlockStreamTraverseIterator *block_it;

	unsigned long valueCount = resultset.getNumberOftuples();
	unsigned long tupleCount = 0;
	TuplePtr *list = new TuplePtr[valueCount];
	unsigned long i = 0;
	while (block = (BlockStreamBase*) it.atomicNextBlock()) {
		block_it = block->createIterator();
		while (tuple = block_it->nextTuple()) {

			list[i++] = tuple;
			tupleCount += getFrequency(tuple, attr.attrType);
		}
	}

	int magicNumber = 10;

	StatisticOnTable *stat = new StatisticOnTable(magicNumber);

	stat->setValueCount(valueCount);
	stat->setTupleCount(tupleCount);

	qsort_r(list, valueCount, sizeof(void *), compare,
			(void *) (attr.attrType->operate));

	mcvAnalyse(list, valueCount, attr, (Statistic *) stat);
	equiDepthAnalyse(list, valueCount, attr, (Statistic *) stat);

	StatManager::getInstance()->addStat(attrID, stat);

	delete list;
	resultset.destory();
}

void Analyzer::mcvAnalyse(void **list, const unsigned long size,
		const Attribute &attr, Statistic *stat) {

	unsigned magicNumber = stat->getBucketCnt();
	unsigned i;
	unsigned bucketCnt = 0;
	TuplePtr *mcvList = new TuplePtr[magicNumber];

	for (i = 0; i < size; ++i) {
		//bubble sort
		//TODO if the number of values is less than the bucket count?
		if (bucketCnt < magicNumber
				|| getFrequency(mcvList[bucketCnt - 1], attr.attrType)
						< getFrequency(list[i], attr.attrType)) {

			unsigned j = bucketCnt < magicNumber ? bucketCnt++ : bucketCnt - 1;
			mcvList[j] = list[i];
			while (j > 0
					&& getFrequency(mcvList[j - 1], attr.attrType)
							< getFrequency(mcvList[j], attr.attrType)) {
				swap(mcvList[j], mcvList[j - 1]);
				--j;
			}
		}
	}

	void **valueList = new void*[magicNumber];
	double *selList = new double[magicNumber];
	for (i = 0; i < magicNumber; ++i) {
		valueList[i] = new char[attr.attrType->get_length()];
		attr.attrType->operate->assign(mcvList[i], valueList[i]);
		selList[i] = (1.0 * getFrequency(mcvList[i], attr.attrType))
				/ stat->getTupleCount();
	}

	stat->setMostCommonValues(valueList, selList);

	delete mcvList;
}

void Analyzer::equiDepthAnalyse(void **list, const unsigned long size,
		const Attribute &attr, Statistic *stat) {

	int magicNumber = stat->getBucketCnt();
	int bucketCnt = -1;
	unsigned long i;
	TuplePtr *boundList = new TuplePtr[magicNumber];
	unsigned long *boundCnt = new unsigned long[magicNumber];
	unsigned long cumFre = 0;
	unsigned long depth = stat->getTupleCount() / (magicNumber - 1);

	cumFre = depth;
	for (i = 0; i < size; ++i) {

		int fre = getFrequency(list[i], attr.attrType);
		cumFre += fre;
		if (cumFre > depth && bucketCnt < magicNumber - 2) {
			boundList[++bucketCnt] = list[i];
			boundCnt[bucketCnt] = 1;
			cumFre = fre;
		} else {
			boundCnt[bucketCnt]++;
		}
	}
	boundList[++bucketCnt] = list[size - 1];
	boundCnt[bucketCnt++] = 0;
	for (; bucketCnt < magicNumber; ++bucketCnt) {
		boundList[bucketCnt] = boundList[bucketCnt - 1];
		boundCnt[bucketCnt] = 0;
	}

	void **valueList = new void*[magicNumber];
	for (i = 0; i < magicNumber; ++i) {
		valueList[i] = new char[attr.attrType->get_length()];
		attr.attrType->operate->assign(boundList[i], valueList[i]);
	}

	stat->setEquithDepthBound(valueList, boundCnt);

	delete boundList;
}
