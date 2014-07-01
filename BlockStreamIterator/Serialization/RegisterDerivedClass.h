/*
 * s.h
 *
 *  Created on: Jun 21, 2013
 *      Author: wangli
 */

#ifndef REGISTERDERIVEDCLASS_H_
#define REGISTERDERIVEDCLASS_H_

#include <boost/archive/text_iarchive.hpp>
#include <boost/archive/text_oarchive.hpp>
#include <boost/archive/binary_iarchive.hpp>
#include <boost/archive/binary_oarchive.hpp>
#ifdef DMALLOC
#include "dmalloc.h"
#endif

#include "../../common/Schema/SchemaFix.h"
template<class Archive>
void Register_Schemas(Archive & ar)
{
	ar.register_type(static_cast<SchemaFix *>(NULL));
}
//template<class Archive>
//inline void Register_Iterators_Except_For_SingleColumnScanIterator(Archive & ar)
//{
//	ar.register_type(static_cast<PrintIterator*>(NULL));
//	ar.register_type(static_cast<FilterIterator*>(NULL));
//	ar.register_type(static_cast<CombinedIterator*>(NULL));
//	ar.register_type(static_cast<AggregationIterator*>(NULL));
//}
//
//template<class Archive>
//inline void Register_Iterators_Except_For_PrintIterator(Archive & ar)
//{
//	ar.register_type(static_cast<SingleColumnScanIterator*>(NULL));
//	ar.register_type(static_cast<FilterIterator*>(NULL));
//	ar.register_type(static_cast<CombinedIterator*>(NULL));
//	ar.register_type(static_cast<AggregationIterator*>(NULL));
//}

template<class Archive>
void Register_Tuple_Stream_Iterators(Archive & ar);
//{
//	ar.register_type(static_cast<PrintIterator*>(NULL));
//	ar.register_type(static_cast<SingleColumnScanIterator*>(NULL));
//	ar.register_type(static_cast<FilterIterator*>(NULL));
//	ar.register_type(static_cast<CombinedIterator*>(NULL));
//	ar.register_type(static_cast<AggregationIterator*>(NULL));
//	ar.register_type(static_cast<ExchangeIteratorLower*>(NULL));
//	ar.register_type(static_cast<JoinIterator*>(NULL));
//	ar.register_type(static_cast<ExchangeIteratorLowerWithWideDependency*>(NULL));
//	ar.register_type(static_cast<ExchangeIteratorWithWideDependency*>(NULL));
//}

template<class Archive>
void Register_Block_Stream_Iterator(Archive & ar);
#endif /* S_H_ */

