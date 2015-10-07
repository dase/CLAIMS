/*
 * RegisterDerivedClass.cpp
 *
 *  Created on: Aug 7, 2013
 *      Author: wangli
 */
#include <boost/iostreams/device/back_inserter.hpp>
#include <boost/iostreams/stream.hpp>
#include "../../IndexManager/CSBIndexBuilding.h"
#include "../../IndexManager/IndexScanIterator.h"
#include "../../physical_query_plan/BlockStreamLimit.h"
#include "../../common/Expression/qnode.h"
#include "../../physical_query_plan/BlockStreamAggregationIterator.h"
#include "../../physical_query_plan/BlockStreamCombinedIterator.h"
#include "../../physical_query_plan/BlockStreamExpander.h"
#include "../../physical_query_plan/BlockStreamInIterator.h"
#include "../../physical_query_plan/BlockStreamJoinIterator.h"
#include "../../physical_query_plan/BlockStreamNestLoopJoinIterator.h"
#include "../../physical_query_plan/BlockStreamPrint.h"
#include "../../physical_query_plan/BlockStreamPerformanceMonitorTop.h"
#include "../../physical_query_plan/BlockStreamProjectIterator.h"
#include "../../physical_query_plan/BlockStreamSortIterator.h"
#include "../../physical_query_plan/ExpandableBlockStreamBuffer.h"
#include "../../physical_query_plan/ExpandableBlockStreamExchangeEpoll.h"
#include "../../physical_query_plan/ExpandableBlockStreamExchangeLowerEfficient.h"
#include "../../physical_query_plan/ExpandableBlockStreamExchangeLowerMaterialized.h"
#include "../../physical_query_plan/ExpandableBlockStreamFilter.h"
#include "../../physical_query_plan/ExpandableBlockStreamHdfsScan.h"
#include "../../physical_query_plan/ExpandableBlockStreamProjectionScan.h"
#include "../../physical_query_plan/ExpandableBlockStreamRandomMemAccess.h"
#include "../../physical_query_plan/ExpandableBlockStreamSingleColumnScan.h"
#include "../../physical_query_plan/ExpandableBlockStreamSingleColumnScanDisk.h"
#include "../../physical_query_plan/physical_operator.h"
#pragma auto_inline
template<class Archive>
void Register_Block_Stream_Iterator(Archive & ar){
	ar.register_type(static_cast<BlockStreamExpander*>(NULL));
	ar.register_type(static_cast<ExpandableBlockStreamSingleColumnScan*>(NULL));
	ar.register_type(static_cast<ExpandableBlockStreamSingleColumnScanDisk*>(NULL));
	ar.register_type(static_cast<ExpandableBlockStreamFilter*>(NULL));
	ar.register_type(static_cast<ExpandableBlockStreamExchangeEpoll*>(NULL));
	ar.register_type(static_cast<ExpandableBlockStreamExchangeLowerEfficient*>(NULL));
	ar.register_type(static_cast<BlockStreamCombinedIterator*>(NULL));
	ar.register_type(static_cast<BlockStreamJoinIterator*>(NULL));
	ar.register_type(static_cast<ExpandableBlockStreamHdfsScan*>(NULL));
	ar.register_type(static_cast<ExpandableBlockStreamExchangeLowerMaterialized*>(NULL));
	ar.register_type(static_cast<ExpandableBlockStreamRandomMemAccess*>(NULL));
	ar.register_type(static_cast<ExpandableBlockStreamProjectionScan*>(NULL));
	ar.register_type(static_cast<BlockStreamPerformanceMonitorTop*>(NULL));
	ar.register_type(static_cast<BlockStreamPrint*>(NULL));
	ar.register_type(static_cast<BlockStreamAggregationIterator*>(NULL));
	ar.register_type(static_cast<BlockStreamNestLoopJoinIterator*>(NULL));
	ar.register_type(static_cast<BlockStreamSortIterator*>(NULL));
	ar.register_type(static_cast<ExpandableBlockStreamBuffer*>(NULL));
	ar.register_type(static_cast<BlockStreamLimit*>(NULL));
	ar.register_type(static_cast<BlockStreamProjectIterator*>(NULL));
	ar.register_type(static_cast<PhysicalOperator*>(NULL));
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
	ar.register_type(static_cast<QExpr_date_add_sub*>(NULL));


}
void cheat_the_compiler(){
    char buffer[4096*2-sizeof(unsigned)];
    boost::iostreams::basic_array_sink<char> sr(buffer, sizeof(buffer));
    boost::iostreams::stream< boost::iostreams::basic_array_sink<char> > ostr(sr);

    boost::archive::binary_oarchive oa(ostr);
    boost::archive::text_oarchive toa(ostr);
    Register_Block_Stream_Iterator<boost::archive::binary_oarchive>(oa);
    Register_Block_Stream_Iterator<boost::archive::text_oarchive>(toa);
    char a[2];
    boost::iostreams::basic_array_source<char> device(a);
    boost::iostreams::stream<boost::iostreams::basic_array_source<char> > s(device);
    boost::archive::binary_iarchive ia(s);
    boost::archive::text_iarchive tia(s);

    Register_Block_Stream_Iterator<boost::archive::binary_iarchive>(ia);
    Register_Block_Stream_Iterator<boost::archive::text_iarchive>(tia);

}
