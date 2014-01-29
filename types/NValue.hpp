/*
 * NValue.hpp
 *
 *  Created on: 2014年1月16日
 *      Author: SCDONG
 */

#ifndef NVALUE_HPP_
#define NVALUE_HPP_

#include <cassert>
#include <stdint.h>
#include <iostream>
using namespace std;

#include <boost/functional/hash.hpp>
#include "../third_party/ttmath/ttmathint.h"
#include "ExportSerializeIo.h"
#include "types.h"
#include "value_defs.h"

namespace decimal {

#define OBJECT_NULL_BIT static_cast<char>(1 << 6)

//The int used for storage and return values
typedef ttmath::Int<2> TTInt;
//Long integer with space for multiplication and division without carry/overflow
typedef ttmath::Int<4> TTLInt;

class NValue {
public:
    static const int64_t kMaxScaleFactor = 1000000000000;

    /**
     * 16 bytes of storage for NValue data.
     */
    char m_data[16];
    ValueType m_valueType;
    bool m_sourceInlined;

    static const uint16_t kMaxDecScale = 12;

    static TTInt s_maxDecimalValue;
    static TTInt s_minDecimalValue;
public:

    NValue();

    NValue(const ValueType type) {
        ::memset( m_data, 0, 16);
        setValueType(type);
        m_sourceInlined = false;
    }

    // Function declarations for NValue.cpp definitions.
    void createDecimalFromString(const std::string &txt);
    std::string createStringFromDecimal() const;
    NValue opMultiplyDecimals(const NValue &lhs, const NValue &rhs) const;
    NValue opDivideDecimals(const NValue lhs, const NValue rhs) const;

    /* Check if the value represents SQL NULL */
    bool isNull() const;
    /* Serialize this NValue to an Export stream */
    void serializeToExport(ExportSerializeOutput&) const;

    //computing
    NValue op_add(const NValue rhs) const;
    NValue op_subtract(const NValue rhs) const;
    NValue op_multiply(const NValue rhs) const;
    NValue op_divide(const NValue rhs) const;

    /* Return a copy of MAX(this, rhs) */
    NValue op_max(const NValue rhs) const;
    /* Return a copy of MIN(this, rhs) */
    NValue op_min(const NValue rhs) const;

    bool op_equals(const NValue rhs) const;

    int compare(const NValue rhs) const;

    static NValue getDecimalValueFromString(const std::string &value) {
        NValue retval(VALUE_TYPE_DECIMAL);
        retval.createDecimalFromString(value);
        return retval;
    }

    TTInt& getDecimal() {
        assert(getValueType() == VALUE_TYPE_DECIMAL);
        void* retval = reinterpret_cast<void*>(m_data);
        return *reinterpret_cast<TTInt*>(retval);
    }

    const TTInt& getDecimal() const {
        assert(getValueType() == VALUE_TYPE_DECIMAL);
        const void* retval = reinterpret_cast<const void*>(m_data);
        return *reinterpret_cast<const TTInt*>(retval);
    }

    void setValueType(ValueType type) {
        m_valueType = type;
    }

    ValueType getValueType() const {
        return m_valueType;
    }

    NValue opAddDecimals(const NValue lhs, const NValue rhs) const {
        if ((lhs.getValueType() != VALUE_TYPE_DECIMAL) ||
            (rhs.getValueType() != VALUE_TYPE_DECIMAL))
        {
            cout << "Non-decimal NValue in decimal adder.\n";
            exit(-1);
        }

        if (lhs.isNull() || rhs.isNull()) {
            TTInt retval;
            retval.SetMin();
            return getDecimalValue(retval);
        }

        TTInt retval(lhs.getDecimal());
        if (retval.Add(rhs.getDecimal()) || retval > s_maxDecimalValue || retval < s_minDecimalValue) {
            cout << "Attempted to add " << lhs.createStringFromDecimal().c_str() << " with " << rhs.createStringFromDecimal().c_str() << " causing overflow/underflow\n";
            exit(-1);
        }
        return getDecimalValue(retval);
    }

    NValue opSubtractDecimals(const NValue lhs, const NValue rhs) const {
        if ((lhs.getValueType() != VALUE_TYPE_DECIMAL) ||
            (rhs.getValueType() != VALUE_TYPE_DECIMAL))
        {
            cout << "Non-decimal NValue in decimal subtract.\n";
            exit(-1);
        }

        if (lhs.isNull() || rhs.isNull()) {
            TTInt retval;
            retval.SetMin();
            return getDecimalValue(retval);
        }

        TTInt retval(lhs.getDecimal());
        if (retval.Sub(rhs.getDecimal()) || retval > s_maxDecimalValue || retval < s_minDecimalValue) {
            cout << "Attempted to subtract " << rhs.createStringFromDecimal().c_str() << " from " << lhs.createStringFromDecimal().c_str() << " causing overflow/underflow\n";
            exit(-1);
        }

        return getDecimalValue(retval);
    }

    static NValue getDecimalValue(TTInt value) {
        NValue retval(VALUE_TYPE_DECIMAL);
        retval.getDecimal() = value;
        return retval;
    }

    /**
     * Get the type of the value. This information is private
     * to prevent code outside of NValue from branching based on the type of a value.
     */
    std::string getValueTypeString() const {
        return getTypeName(m_valueType);
    }

    int compareDecimalValue(const NValue rhs) const {
        switch (rhs.getValueType()) {
          case VALUE_TYPE_DECIMAL:
          {
              const TTInt lhsValue = getDecimal();
              const TTInt rhsValue = rhs.getDecimal();

              if (lhsValue == rhsValue) {
                  return VALUE_COMPARE_EQUAL;
              } else if (lhsValue > rhsValue) {
                  return VALUE_COMPARE_GREATERTHAN;
              } else {
                  return VALUE_COMPARE_LESSTHAN;
              }
          }
          default:
          {
              cout << "Type " << valueToString(rhs.getValueType()).c_str() << " cannot be cast for comparison to type " << valueToString(getValueType()).c_str() << endl;
              exit(-1);
          }
       }
    }
};

inline NValue::NValue() {
    ::memset( m_data, 0, 16);
    setValueType(VALUE_TYPE_INVALID);
    m_sourceInlined = false;
}

inline bool NValue::isNull() const {
    if (getValueType() == VALUE_TYPE_DECIMAL) {
        TTInt min;
        min.SetMin();
        return getDecimal() == min;
    }
    return m_data[13] == OBJECT_NULL_BIT;
}

inline void NValue::serializeToExport(ExportSerializeOutput &io) const
{
    switch (getValueType()) {
    case VALUE_TYPE_DECIMAL:
    {
    	std::string decstr = createStringFromDecimal();
    	int32_t objectLength = (int32_t)decstr.length();
    	io.writeBinaryString(decstr.data(), objectLength);
    	return;
    }
    default:
    {
    	cout << "Invalid type in serializeToExport\n";
    	return;
    }
    }
}

inline NValue NValue::op_add(const NValue rhs) const {
	if (VALUE_TYPE_DECIMAL == getValueType() && VALUE_TYPE_DECIMAL == rhs.getValueType())
	{
		return opAddDecimals(this->getDecimalValue(this->getDecimal()), rhs);
	}
    cout << "Promotion of " << getValueTypeString().c_str() << " and " << rhs.getValueTypeString().c_str() << " failed in op_add.\n";
    exit(-1);
}

inline NValue NValue::op_subtract(const NValue rhs) const {
	if (VALUE_TYPE_DECIMAL == getValueType() && VALUE_TYPE_DECIMAL == rhs.getValueType())
	{
        return opSubtractDecimals(this->getDecimalValue(this->getDecimal()), rhs);
	}
	cout << "Promotion of " << getValueTypeString().c_str() << " and " << rhs.getValueTypeString().c_str() << " failed in op_add.\n";
	exit(-1);
}

inline NValue NValue::op_multiply(const NValue rhs) const {
    if (VALUE_TYPE_DECIMAL == getValueType() && VALUE_TYPE_DECIMAL == rhs.getValueType())
    {
        return opMultiplyDecimals(*this, rhs);
    }
    cout << "Promotion of " << getValueTypeString().c_str() << " and " << rhs.getValueTypeString().c_str() << " failed in op_multiply.\n";
    exit(-1);
}

inline NValue NValue::op_divide(const NValue rhs) const {
	if (VALUE_TYPE_DECIMAL == getValueType() && VALUE_TYPE_DECIMAL == rhs.getValueType())
	{
        return opDivideDecimals(this->getDecimalValue(this->getDecimal()), rhs);
    }
	cout << "Promotion of " << getValueTypeString().c_str() << " and " << rhs.getValueTypeString().c_str() << " failed in op_divide.\n";
	exit(-1);
}

inline NValue NValue::op_max(const NValue rhs) const {
	const int value = compare(rhs);
	if (value > 0) {
		return *this;
	} else {
		return rhs;
	}
}

inline NValue NValue::op_min(const NValue rhs) const {
	const int value = compare(rhs);
	if (value < 0) {
		return *this;
	} else {
		return rhs;
	}
}

inline bool NValue::op_equals(const NValue rhs) const {
    return compare(rhs) == 0 ? true : false;
}

inline int NValue::compare(const NValue rhs) const {
    switch (getValueType()) {
    case VALUE_TYPE_DECIMAL:
        return compareDecimalValue(rhs);
    default: {
        cout << "non comparable types lhs '" << getValueTypeString().c_str() << "' rhs '" << rhs.getValueTypeString().c_str() << "'\n";
        exit(-1);
    }
    }
}

} //end for namespace
#endif /* NVALUE_HPP_ */
