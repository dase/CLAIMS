/*
 * RegisterDerivedClass.cpp
 *
 *  Created on: Aug 7, 2013
 *      Author: wangli
 */
#include <boost/iostreams/device/back_inserter.hpp>
#include <boost/iostreams/stream.hpp>

#include "RegisterDerivedClass.h"
#include "../ParallelBlockStreamIterator/BlockStreamExpander.h"
#include "../ParallelBlockStreamIterator/ExpandableBlockStreamSingleColumnScan.h"
#include "../ParallelBlockStreamIterator/ExpandableBlockStreamSingleColumnScanDisk.h"
#include "../ParallelBlockStreamIterator/ExpandableBlockStreamFilter.h"
#include "../ParallelBlockStreamIterator/ExpandableBlockStreamExchangeEpoll.h"
#include "../ParallelBlockStreamIterator/ExpandableBlockStreamExchangeLowerEfficient.h"
#include "../ParallelBlockStreamIterator/ExpandableBlockStreamExchangeLowerMaterialized.h"
#include "../ParallelBlockStreamIterator/BlockStreamCombinedIterator.h"
#include "../ParallelBlockStreamIterator/BlockStreamJoinIterator.h"
#include "../ParallelBlockStreamIterator/ExpandableBlockStreamHdfsScan.h"
#include "../ParallelBlockStreamIterator/ExpandableBlockStreamExchangeMaterialized.h"
#include "../ParallelBlockStreamIterator/ExpandableBlockStreamRandomMemAccess.h"
#include "../ParallelBlockStreamIterator/ExpandableBlockStreamBuffer.h"
#include "../ParallelBlockStreamIterator/BlockStreamInIterator.h"
#include "../../BlockStreamIterator/ParallelBlockStreamIterator/ExpandableBlockStreamProjectionScan.h"
#include "../../BlockStreamIterator/ParallelBlockStreamIterator/BlockStreamAggregationIterator.h"
#include "../../BlockStreamIterator/ParallelBlockStreamIterator/BlockStreamProjectIterator.h"
#include "../../BlockStreamIterator/ParallelBlockStreamIterator/BlockStreamSortIterator.h"

#include "../../BlockStreamIterator/BlockStreamPerformanceMonitorTop.h"
#include "../../BlockStreamIterator/BlockStreamPrint.h"
#include "../BlockStreamLimit.h"

#include "../ExpandableBlockStreamIteratorBase.h"

#include "../../IndexManager/CSBIndexBuilding.h"
#include "../../IndexManager/IndexScanIterator.h"
#include "../BlockStreamLimit.h"
#include "../../BlockStreamIterator/ParallelBlockStreamIterator/BlockStreamSortIterator.h"
#include "../../common/Expression/qnode.h"
#pragma auto_inline
template<class Archive>
void Register_Tuple_Stream_Iterators(Archive & ar)
{
//	ar.register_type(static_cast<FilterIterator*>(NULL));

}

#pragma auto_inline
template<class Archive>
void Register_Block_Stream_Iterator(Archive & ar){
	ar.register_type(static_cast<BlockStreamExpander*>(NULL));
	ar.register_type(static_cast<ExpandableBlockStreamSingleColumnScan*>(NULL));
	ar.register_type(static_cast<ExpandableBlockStreamSingleColumnScanDisk*>(NULL));
	ar.register_type(static_cast<ExpandableBlockStreamFilter*>(NULL));
	ar.register_type(static_cast<ExpandableBlockStreamExchangeEpoll*>(NULL));
	ar.register_type(static_cast<ExpandableBlockStreamExchangeLowerEfficient*>(NULL));
	ar.register_type(static_cast<ExpandableBlockStreamExchangeLowerMaterialized*>(NULL));
	ar.register_type(static_cast<BlockStreamCombinedIterator*>(NULL));
	ar.register_type(static_cast<BlockStreamJoinIterator*>(NULL));
	ar.register_type(static_cast<ExpandableBlockStreamHdfsScan*>(NULL));
	ar.register_type(static_cast<ExpandableBlockStreamExchangeLowerMaterialized*>(NULL));
//	ar.register_type(static_cast<BlockStreamExchangeBase*>(NULL));
	ar.register_type(static_cast<ExpandableBlockStreamExchangeMaterialized*>(NULL));

	ar.register_type(static_cast<ExpandableBlockStreamRandomMemAccess*>(NULL));
	ar.register_type(static_cast<ExpandableBlockStreamProjectionScan*>(NULL));
	ar.register_type(static_cast<BlockStreamPerformanceMonitorTop*>(NULL));
	ar.register_type(static_cast<BlockStreamPrint*>(NULL));
	ar.register_type(static_cast<BlockStreamAggregationIterator*>(NULL));

	ar.register_type(static_cast<BlockStreamSortIterator*>(NULL));

	ar.register_type(static_cast<ExpandableBlockStreamBuffer*>(NULL));
	ar.register_type(static_cast<BlockStreamLimit*>(NULL));
	ar.register_type(static_cast<BlockStreamProjectIterator*>(NULL));
	ar.register_type(static_cast<ExpandableBlockStreamIteratorBase*>(NULL));
	ar.register_type(static_cast<BlockStreamInIterator*>(NULL));
	ar.register_type(static_cast<bottomLayerCollecting*>(NULL));
	ar.register_type(static_cast<bottomLayerSorting*>(NULL));
	ar.register_type(static_cast<IndexScanIterator*>(NULL));
	ar.register_type(static_cast<QNode*>(NULL));
	ar.register_type(static_cast<QExpr_binary*>(NULL));
	ar.register_type(static_cast<QColcumns*>(NULL));
	ar.register_type(static_cast<QExpr*>(NULL));
	ar.register_type(static_cast<QExpr_unary*>(NULL));
	ar.register_type(static_cast<QExpr_ternary*>(NULL));
	ar.register_type(static_cast<QExpr_case_when*>(NULL));
	ar.register_type(static_cast<QExpr_in*>(NULL));

}
void cheat_the_compiler(){
    char buffer[4096*2-sizeof(unsigned)];
    boost::iostreams::basic_array_sink<char> sr(buffer, sizeof(buffer));
    boost::iostreams::stream< boost::iostreams::basic_array_sink<char> > ostr(sr);

    boost::archive::binary_oarchive oa(ostr);
    boost::archive::text_oarchive toa(ostr);
    Register_Tuple_Stream_Iterators<boost::archive::binary_oarchive>(oa);
    Register_Tuple_Stream_Iterators<boost::archive::text_oarchive>(toa);
    Register_Block_Stream_Iterator<boost::archive::binary_oarchive>(oa);
    Register_Block_Stream_Iterator<boost::archive::text_oarchive>(toa);
    char a[2];
    boost::iostreams::basic_array_source<char> device(a);
    boost::iostreams::stream<boost::iostreams::basic_array_source<char> > s(device);
    boost::archive::binary_iarchive ia(s);
    boost::archive::text_iarchive tia(s);
    Register_Tuple_Stream_Iterators<boost::archive::binary_iarchive>(ia);
    Register_Tuple_Stream_Iterators<boost::archive::text_iarchive>(tia);
    Register_Block_Stream_Iterator<boost::archive::binary_iarchive>(ia);
    Register_Block_Stream_Iterator<boost::archive::text_iarchive>(tia);

}

