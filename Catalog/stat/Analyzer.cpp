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

#include "../../common/Block/BlockStream.h"
#include "../../common/Block/DynamicBlockBuffer.h"
#include "../../common/Block/ResultSet.h"
#include "../../common/data_type.h"
#include "../../logical_operator/logical_aggregation.h"
#include "../../logical_operator/logical_query_plan_root.h"
#include "../../logical_operator/logical_scan.h"
#include "../../physical_operator/physical_aggregation.h"
#include "../Catalog.h"
#include "../table.h"

#include "StatManager.h"
using namespace claims::logical_operator;
using claims::physical_operator::PhysicalAggregation;
using std::map;

typedef void* TuplePtr;

inline int getFrequency(const void* tuple, const column_type* type) {
  return *((int*)(((char*)tuple) + type->get_length()));
}

Analyzer::Analyzer() {
  // TODO(Anyone): Auto-generated constructor stub
}

Analyzer::~Analyzer() {
  // TODO(Anyone): Auto-generated destructor stub
}

int compare(const void* a, const void* b, void* arg) {
  return ((Operate*)arg)->compare(*(void**)a, *(void**)b);
}

void Analyzer::analyse(const AttributeID& attrID) {
  Catalog* catalog = Catalog::getInstance();

  TableDescriptor* table = catalog->getTable(attrID.table_id);
  ProjectionDescriptor* projection = NULL;

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

  std::vector<PhysicalAggregation::State::Aggregation> aggregation_function;
  aggregation_function.push_back(PhysicalAggregation::State::kCount);

  LogicalOperator* sb_payload_scan = new LogicalScan(projection);

  LogicalOperator* aggregation =
      new LogicalAggregation(group_by_attributes, aggregation_attributes,
                             aggregation_function, sb_payload_scan);
  const NodeID collector_node_id = 0;

  LogicalOperator* root = new LogicalQueryPlanRoot(
      collector_node_id, aggregation, LogicalQueryPlanRoot::kResultCollector);

  PhysicalOperatorBase* collector =
      root->GetPhysicalPlan(1024 * 64 - sizeof(unsigned));

  collector->Open();
  collector->Next(0);
  collector->Close();
  ResultSet* resultset = collector->GetResultSet();
  ResultSet::Iterator it = resultset->createIterator();

  BlockStreamBase* block;
  void* tuple;
  BlockStreamBase::BlockStreamTraverseIterator* block_it;

  unsigned long valueCount = resultset->getNumberOftuples();
  unsigned long tupleCount = 0;
  TuplePtr* list = new TuplePtr[valueCount];
  unsigned long i = 0;
  while (block = (BlockStreamBase*)it.atomicNextBlock()) {
    block_it = block->createIterator();
    while (tuple = block_it->nextTuple()) {
      list[i++] = tuple;
      tupleCount += getFrequency(tuple, attr.attrType);
    }
  }

  int magicNumber = 100;

  StatisticOnTable* stat = new StatisticOnTable(magicNumber);

  stat->setValueCount(valueCount);
  stat->setTupleCount(tupleCount);

  qsort_r(list, valueCount, sizeof(void*), compare,
          (void*)(attr.attrType->operate));

  mcvAnalyse(list, valueCount, attr, (Histogram*)stat);
  equiDepthAnalyse(list, valueCount, attr, (Histogram*)stat);

  //	StatManager::getInstance()->addStat(attrID, stat);
  StatManager::getInstance()->getTableStatistic(attrID.table_id);
  delete list;
  resultset->destory();
}

void Analyzer::analyse(TableID table_id, analysis_level level) {
  TableStatistic* tab_stat =
      StatManager::getInstance()->getTableStatistic(table_id);
  TableDescriptor* table = Catalog::getInstance()->getTable(table_id);
  if (tab_stat == 0) {
    //    LogicalOperator * scan = new LogicalScan(table->getProjectoin(0));
    //    std::vector<Attribute> group_by_attributes;
    //    std::vector<Attribute> aggregation_attributes;
    //
    //    aggregation_attributes.push_back(Attribute(ATTRIBUTE_ANY));
    //
    //    std::vector<BlockStreamAggregationIterator::State::aggregation>
    //    aggregation_function;
    //    aggregation_function.push_back(
    //        BlockStreamAggregationIterator::State::count);
    //    LogicalOperator* agg = new Aggregation(group_by_attributes,
    //                                           aggregation_attributes,
    //                                           aggregation_function, scan);
    //    LogicalOperator* root = new LogicalQueryPlanRoot(
    //        0, agg, LogicalQueryPlanRoot::RESULTCOLLECTOR);
    //
    //    BlockStreamIteratorBase* collector = root->getIteratorTree(
    //        1024 * 64 - sizeof(unsigned));
    //    collector->open();
    //    collector->next(0);
    //    collector->close();
    //    ResultSet* resultset = collector->getResultSet();
    //    ResultSet::Iterator it = resultset->createIterator();
    //    BlockStreamBase::BlockStreamTraverseIterator* b_it = it.nextBlock()
    //        ->createIterator();
    //    const unsigned long tuple_count = *(unsigned
    //    long*) b_it->nextTuple();
    //    BlockStreamBase* block;
    //    while (block = it.nextBlock()) {
    //      BlockStreamBase::BlockStreamTraverseIterator* b_it =
    //          block->createIterator();
    //
    //    }
    //    tab_stat = new TableStatistic();
    //    tab_stat->number_of_tuples_ = tuple_count;
    //    printf("Statistics for table %s is
    //        gathered!\n",Catalog::getInstance()->getTable(table_id)
    //    ->getTableName().c_str());
    //        tab_stat->print();
    //        StatManager::getInstance()->setTableStatistic(table_id, tab_stat);
    //        resultset->destory();
    //        root->~LogicalOperator();
    compute_table_stat(table_id);
  }

  if (level == a_l_attribute) {
    std::vector<Attribute> attribute_list = table->getAttributes();
    for (unsigned i = 0; i < attribute_list.size(); i++) {
      compute_attribute_stat(attribute_list[i].getID());
    }
  }
}

void Analyzer::analyse_on_project(ProjectionID projection_id) {
  /**
   * First make sure that the table statistic is available, and then
   * make attribute-level-analyze on each attribute within this projection.
   */
  TableStatistic* tab_stat =
      StatManager::getInstance()->getTableStatistic(projection_id.table_id);
  if (tab_stat == 0) {
    analyse(projection_id.table_id);
  }

  ProjectionDescriptor* p_des =
      Catalog::getInstance()->getProjection(projection_id);
  assert(p_des);
  std::vector<Attribute> attri_list = p_des->getAttributeList();
  for (unsigned i = 0; i < attri_list.size(); i++) {
    compute_attribute_stat(attri_list[i].getID());
  }
}

void Analyzer::compute_histogram(const AttributeID& attr_id) {}
void Analyzer::compute_attribute_stat(const AttributeID& attr_id) {
  TableStatistic* tab_stat =
      StatManager::getInstance()->getTableStatistic(attr_id.table_id);
  assert(tab_stat != 0);
  if (tab_stat->getAttributeStatistics(attr_id)) {
    printf("attribute statistics is already existed!\n");
    return;
  }
  unsigned long distinct_cardinality;
  if (Catalog::getInstance()
          ->getTable(attr_id.table_id)
          ->getAttribute(attr_id.offset)
          .isUnique()) {
    /**
     * for attribute that has UNIQUE property, gathering the number of distinct
     * values is unnecessary.
     */
    distinct_cardinality = tab_stat->number_of_tuples_;
  } else {
    distinct_cardinality = Analyzer::getDistinctCardinality(attr_id);
  }
  AttributeStatistics* attr_stat = new AttributeStatistics();
  attr_stat->setDistinctCardinality(distinct_cardinality);
  printf("Distinct values:%d\n", distinct_cardinality);
  if (Analyzer::isBeneficalFromHistrogram(distinct_cardinality,
                                          tab_stat->number_of_tuples_)) {
    //		Analyzer::analyse(attr_id);
    Histogram* his = Analyzer::computeHistogram(
        attr_id, decideNumberOfBucketsForHistogram(
                     distinct_cardinality, tab_stat->number_of_tuples_));
    his->Print(Catalog::getInstance()
                   ->getTable(attr_id.table_id)
                   ->getAttribute(attr_id.offset)
                   .attrType->type);
    attr_stat->setHistogram(his);
  }

  attr_stat->print();
  printf("----%s----\n", Catalog::getInstance()
                             ->getTable(attr_id.table_id)
                             ->getAttribute(attr_id.offset)
                             .getName()
                             .c_str());
  tab_stat->addAttributeStatistics(attr_id, attr_stat);
}

void Analyzer::compute_table_stat(const TableID& tab_id) {
  TableDescriptor* table = Catalog::getInstance()->getTable(tab_id);

  LogicalOperator* scan = new LogicalScan(table->getProjectoin(0));
  std::vector<Attribute> group_by_attributes;
  std::vector<Attribute> aggregation_attributes;

  aggregation_attributes.push_back(Attribute(ATTRIBUTE_ANY));

  std::vector<PhysicalAggregation::State::Aggregation> aggregation_function;
  aggregation_function.push_back(PhysicalAggregation::State::kCount);
  LogicalOperator* agg = new LogicalAggregation(
      group_by_attributes, aggregation_attributes, aggregation_function, scan);
  LogicalOperator* root =
      new LogicalQueryPlanRoot(0, agg, LogicalQueryPlanRoot::kResultCollector);

  PhysicalOperatorBase* collector =
      root->GetPhysicalPlan(1024 * 64 - sizeof(unsigned));
  collector->Open();
  collector->Next(0);
  collector->Close();
  ResultSet* resultset = collector->GetResultSet();
  ResultSet::Iterator it = resultset->createIterator();
  BlockStreamBase::BlockStreamTraverseIterator* b_it =
      it.nextBlock()->createIterator();
  const unsigned long tuple_count = *(unsigned long*)b_it->nextTuple();
  BlockStreamBase* block;
  while (block = it.nextBlock()) {
    BlockStreamBase::BlockStreamTraverseIterator* b_it =
        block->createIterator();
  }
  TableStatistic* tab_stat = new TableStatistic();
  tab_stat->number_of_tuples_ = tuple_count;
  printf("Statistics for table %s is gathered!\n",
         Catalog::getInstance()->getTable(tab_id)->getTableName().c_str());
  tab_stat->print();
  StatManager::getInstance()->setTableStatistic(tab_id, tab_stat);
  resultset->destory();
  root->~LogicalOperator();
}

void Analyzer::mcvAnalyse(void** list, const unsigned long size,
                          const Attribute& attr, Histogram* stat) {
  unsigned magicNumber = stat->getBucketCnt();
  unsigned i;
  unsigned bucketCnt = 0;
  TuplePtr* mcvList = new TuplePtr[magicNumber];

  for (i = 0; i < size; ++i) {
    // bubble sort
    // TODO if the number of values is less than the bucket count?
    if (bucketCnt < magicNumber ||
        getFrequency(mcvList[bucketCnt - 1], attr.attrType) <
            getFrequency(list[i], attr.attrType)) {
      unsigned j = bucketCnt < magicNumber ? bucketCnt++ : bucketCnt - 1;
      mcvList[j] = list[i];
      while (j > 0 &&
             getFrequency(mcvList[j - 1], attr.attrType) <
                 getFrequency(mcvList[j], attr.attrType)) {
        swap(mcvList[j], mcvList[j - 1]);
        --j;
      }
    }
  }

  void** valueList = new void* [magicNumber];  // new
  double* selList = new double[magicNumber];   // new
  for (i = 0; i < magicNumber; ++i) {
    valueList[i] = new char[attr.attrType->get_length()];  // new
    attr.attrType->operate->assign(mcvList[i], valueList[i]);
    selList[i] =
        (1.0 * getFrequency(mcvList[i], attr.attrType)) / stat->getTupleCount();
  }

  stat->setMostCommonValues(valueList, selList);

  delete mcvList;
}

void Analyzer::equiDepthAnalyse(void** list, const unsigned long size,
                                const Attribute& attr, Histogram* stat) {
  int magicNumber = stat->getBucketCnt();
  int bucketCnt = -1;
  unsigned long i;
  TuplePtr* boundList = new TuplePtr[magicNumber];
  unsigned long* boundCnt = new unsigned long[magicNumber];
  unsigned long cumFre = 0;
  //	unsigned long depth = stat->getTupleCount() / (magicNumber - 1);
  unsigned long depth = stat->getTupleCount() / (magicNumber);

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

  void** valueList = new void* [magicNumber];  // new
  for (i = 0; i < magicNumber; ++i) {
    valueList[i] = new char[attr.attrType->get_length()];  // new
    attr.attrType->operate->assign(boundList[i], valueList[i]);
  }

  stat->setEquithDepthBound(valueList, boundCnt);

  delete boundList;
}
unsigned long Analyzer::getDistinctCardinality(const AttributeID& attr_id) {
  LogicalOperator* scan = new LogicalScan(
      Catalog::getInstance()->getTable(attr_id.table_id)->getProjectoin(0));

  std::vector<Attribute> group_by_attributes;
  group_by_attributes.push_back(
      Catalog::getInstance()->getTable(attr_id.table_id)->getAttribute(
          attr_id.offset));

  LogicalOperator* agg = new LogicalAggregation(
      group_by_attributes, std::vector<Attribute>(),
      std::vector<PhysicalAggregation::State::Aggregation>(), scan);

  std::vector<Attribute> aggregation_attributes;
  aggregation_attributes.push_back(Attribute(ATTRIBUTE_ANY));
  std::vector<PhysicalAggregation::State::Aggregation> aggregation_function;
  aggregation_function.push_back(PhysicalAggregation::State::kCount);

  LogicalOperator* count_agg =
      new LogicalAggregation(std::vector<Attribute>(), aggregation_attributes,
                             aggregation_function, agg);

  LogicalOperator* root = new LogicalQueryPlanRoot(
      0, count_agg, LogicalQueryPlanRoot::kResultCollector);

  PhysicalOperatorBase* collector =
      root->GetPhysicalPlan(1024 * 64 - sizeof(unsigned));
  collector->Open();
  collector->Next(0);
  collector->Close();
  ResultSet* resultset = collector->GetResultSet();
  ResultSet::Iterator it = resultset->createIterator();
  BlockStreamBase::BlockStreamTraverseIterator* b_it =
      it.nextBlock()->createIterator();
  const unsigned long distinct_cardinality = *(unsigned long*)b_it->nextTuple();

  resultset->destory();
  collector->~PhysicalOperatorBase();
  root->~LogicalOperator();
  return distinct_cardinality;
}
bool Analyzer::isBeneficalFromHistrogram(unsigned distinct_values,
                                         unsigned cardinality) {
  if (distinct_values == 1) {
    return false;
  }
  const int magic_number = 3;
  return distinct_values * magic_number < cardinality;
}
Histogram* Analyzer::computeHistogram(const AttributeID& attr_id,
                                      const unsigned nbuckets) {
  printf("Compute for histogram for attribute %s (%d buckets)\n",
         Catalog::getInstance()
             ->getTable(attr_id.table_id)
             ->getAttribute(attr_id.offset)
             .attrName.c_str(),
         nbuckets);
  Catalog* catalog = Catalog::getInstance();

  TableDescriptor* table = catalog->getTable(attr_id.table_id);
  ProjectionDescriptor* projection = NULL;

  unsigned pidSize = table->getNumberOfProjection();
  const Attribute attr = table->getAttribute(attr_id.offset);

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

  std::vector<PhysicalAggregation::State::Aggregation> aggregation_function;
  aggregation_function.push_back(PhysicalAggregation::State::kCount);

  LogicalOperator* sb_payload_scan = new LogicalScan(projection);

  LogicalOperator* aggregation =
      new LogicalAggregation(group_by_attributes, aggregation_attributes,
                             aggregation_function, sb_payload_scan);
  const NodeID collector_node_id = 0;

  LogicalOperator* root = new LogicalQueryPlanRoot(
      collector_node_id, aggregation, LogicalQueryPlanRoot::kResultCollector);

  PhysicalOperatorBase* collector =
      root->GetPhysicalPlan(1024 * 64 - sizeof(unsigned));

  collector->Open();
  collector->Next(0);
  collector->Close();
  ResultSet* resultset = collector->GetResultSet();
  ResultSet::Iterator it = resultset->createIterator();

  BlockStreamBase* block;
  void* tuple;
  BlockStreamBase::BlockStreamTraverseIterator* block_it;

  unsigned long valueCount = resultset->getNumberOftuples();
  unsigned long tupleCount = 0;
  TuplePtr* list = new TuplePtr[valueCount];
  unsigned long i = 0;
  while (block = (BlockStreamBase*)it.atomicNextBlock()) {
    block_it = block->createIterator();
    while (tuple = block_it->nextTuple()) {
      list[i++] = tuple;
      tupleCount += getFrequency(tuple, attr.attrType);
    }
  }

  Histogram* stat = new Histogram(nbuckets);

  stat->setValueCount(valueCount);
  stat->setTupleCount(tupleCount);

  qsort_r(list, valueCount, sizeof(void*), compare,
          (void*)(attr.attrType->operate));

  mcvAnalyse(list, valueCount, attr, (Histogram*)stat);
  equiDepthAnalyse(list, valueCount, attr, (Histogram*)stat);

  //	StatManager::getInstance()->addStat(attrID, stat);
  //	StatManager::getInstance()->getTableStatistic(attrID.table_id)
  delete list;
  resultset->destory();
  return stat;
}
unsigned Analyzer::decideNumberOfBucketsForHistogram(
    const int distinct_cardinality, const int cardinality) {
  return 1 + (distinct_cardinality / 10 < 1000 ? distinct_cardinality / 10 + 1
                                               : 1000);
}
