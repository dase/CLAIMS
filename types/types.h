/*
 * types.h
 *
 *  Created on: 2014年1月16日
 *      Author: SCDONG
 */

#ifndef TYPES_H_
#define TYPES_H_

namespace decimal {
enum ValueType {
	VALUE_TYPE_INVALID      = 0,
    VALUE_TYPE_DECIMAL      = 22,   // decimal(p,s)
};

std::string getTypeName(ValueType type);

std::string valueToString(ValueType type);

} //endfor namespace
#endif /* TYPES_H_ */
