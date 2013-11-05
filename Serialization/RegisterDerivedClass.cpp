/*
 * RegisterDerivedClass.cpp
 *
 *  Created on: Aug 7, 2013
 *      Author: wangli
 */

#include "RegisterDerivedClass.h"
#include "../iterator/SingleColumnScanIterator.h"
#include "../iterator/PrintIterator.h"
#include "../iterator/FilterIterator.h"
#include "../iterator/JoinIterator.h"
#include "../iterator/CombinedIterator.h"
#include "../iterator/AggregationIterator.h"
#include "../iterator/ExchangeIteratorLower.h"
#include "../iterator/ExchangeIteratorLowerWithWideDependency.h"
#include "../iterator/ExchangeIteratorWithWideDependency.h"
#include "../iterator/ExchangeIteratorEager.h"
#include "../iterator/ExchangeIteratorEagerLower.h"
#include "../PerformanceMonitor/PerformanceIteratorTop.h"

#include "../BlockStreamIterator/ParallelBlockStreamIterator/ExpandableBlockStreamSingleColumnScan.h"
#include "../BlockStreamIterator/ParallelBlockStreamIterator/ExpandableBlockStreamSingleColumnScanDisk.h"
#include "../BlockStreamIterator/ParallelBlockStreamIterator/BlockStreamExpander.h"
#include "../BlockStreamIterator/ParallelBlockStreamIterator/ExpandableBlockStreamFilter.h"
#include "../BlockStreamIterator/ParallelBlockStreamIterator/ExpandableBlockStreamExchange.h"
#include "../BlockStreamIterator/ParallelBlockStreamIterator/ExpandableBlockStreamExchangeEpoll.h"
#include "../BlockStreamIterator/ParallelBlockStreamIterator/ExpandableBlockStreamExchangeLower.h"
#include "../BlockStreamIterator/ParallelBlockStreamIterator/ExpandableBlockStreamExchangeLowerEfficient.h"
#include "../BlockStreamIterator/ParallelBlockStreamIterator/ExpandableBlockStreamExchangeLowerMaterialized.h"
#include "../BlockStreamIterator/ParallelBlockStreamIterator/ExpandableBlockStreamExchangeMaterialized.h"
#include "../BlockStreamIterator/ParallelBlockStreamIterator/ExpandableBlockStreamBroadcastExchange.h"
#include "../BlockStreamIterator/ParallelBlockStreamIterator/ExpandableBlockStreamBroadcastExchangeLower.h"
#include "../BlockStreamIterator/ParallelBlockStreamIterator/BlockStreamCombinedIterator.h"
#include "../BlockStreamIterator/ParallelBlockStreamIterator/BlockStreamJoinIterator.h"
#include "../BlockStreamIterator/ParallelBlockStreamIterator/ExpandableBlockStreamHdfsScan.h"
#include "../BlockStreamIterator/BlockStreamExchangeLowerBase.h"
#include "../BlockStreamIterator/BlockStreamExchangeBase.h"

#include "../iterator/RowScanIterator.h"
#include "../iterator/RandomDiskAccessIterator.h"
#include "../iterator/SequencialDiskAccessIterator.h"
#include "../iterator/RandomMemAccessIterator.h"

#include "../BlockStreamIterator/ParallelBlockStreamIterator/ExpandableBlockStreamRandomMemAccess.h"

#pragma auto_inline
template<class Archive>
void Register_Tuple_Stream_Iterators(Archive & ar)
{
	ar.register_type(static_cast<PrintIterator*>(NULL));
	ar.register_type(static_cast<SingleColumnScanIterator*>(NULL));
	ar.register_type(static_cast<FilterIterator*>(NULL));
	ar.register_type(static_cast<CombinedIterator*>(NULL));
	ar.register_type(static_cast<AggregationIterator*>(NULL));
	ar.register_type(static_cast<ExchangeIteratorLower*>(NULL));
	ar.register_type(static_cast<JoinIterator*>(NULL));
	ar.register_type(static_cast<ExchangeIteratorLowerWithWideDependency*>(NULL));
	ar.register_type(static_cast<ExchangeIteratorWithWideDependency*>(NULL));
	ar.register_type(static_cast<ExchangeIteratorEager*>(NULL));
	ar.register_type(static_cast<ExchangeIteratorEagerLower*>(NULL));

	ar.register_type(static_cast<RowScanIterator*>(NULL));
	ar.register_type(static_cast<RandomDiskAccessIterator*>(NULL));
	ar.register_type(static_cast<SequencialDiskAccessIterator*>(NULL));
	ar.register_type(static_cast<RandomMemAccessIterator*>(NULL));

}

#pragma auto_inline
template<class Archive>
void Register_Block_Stream_Iterator(Archive & ar){
	ar.register_type(static_cast<BlockStreamExpander*>(NULL));
	ar.register_type(static_cast<ExpandableBlockStreamSingleColumnScan*>(NULL));
	ar.register_type(static_cast<ExpandableBlockStreamSingleColumnScanDisk*>(NULL));
	ar.register_type(static_cast<ExpandableBlockStreamFilter*>(NULL));
	ar.register_type(static_cast<ExpandableBlockStreamExchange*>(NULL));
	ar.register_type(static_cast<ExpandableBlockStreamExchangeEpoll*>(NULL));
	ar.register_type(static_cast<ExpandableBlockStreamExchangeLower*>(NULL));
	ar.register_type(static_cast<ExpandableBlockStreamExchangeLowerEfficient*>(NULL));
	ar.register_type(static_cast<ExpandableBlockStreamExchangeLowerMaterialized*>(NULL));
	ar.register_type(static_cast<ExpandableBlockStreamBroadcastExchange*>(NULL));
	ar.register_type(static_cast<ExpandableBlockStreamBroadcastExchangeLower*>(NULL));
	ar.register_type(static_cast<BlockStreamCombinedIterator*>(NULL));
	ar.register_type(static_cast<BlockStreamJoinIterator*>(NULL));
	ar.register_type(static_cast<ExpandableBlockStreamHdfsScan*>(NULL));
	ar.register_type(static_cast<ExpandableBlockStreamExchangeLowerMaterialized*>(NULL));
//	ar.register_type(static_cast<BlockStreamExchangeBase*>(NULL));
	ar.register_type(static_cast<ExpandableBlockStreamExchangeMaterialized*>(NULL));

	ar.register_type(static_cast<ExpandableBlockStreamRandomMemAccess*>(NULL));

}
