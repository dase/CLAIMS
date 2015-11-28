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
#include "../../common/Schema/SchemaFix.h"
template <class Archive>
void Register_Schemas(Archive &ar) {
  ar.register_type(static_cast<SchemaFix *>(NULL));
}
template <class Archive>
void Register_Block_Stream_Iterator(Archive &ar);
#endif /* S_H_ */
