/*
 * NValue.cpp
 *
 *  Created on: 2014年1月16日
 *      Author: SCDONG
 */

#include <string>
#include <iostream>
using namespace std;
#include "NValue.hpp"
namespace decimal {

TTInt NValue::s_maxDecimalValue("9999999999"   //10 digits
                                "9999999999"   //20 digits
                                "9999999999"   //30 digits
                                "99999999");    //38 digits

TTInt NValue::s_minDecimalValue("-9999999999"   //10 digits
                                 "9999999999"   //20 digits
                                 "9999999999"   //30 digits
                                 "99999999");    //38 digits

/**
 *   set a decimal value from a serialized representation
 */
void NValue::createDecimalFromString(const string &txt) {
    if (txt.length() == 0) {
        cout << "Can not create Decimal with String NULL!\n";
        exit(-1);
    }
    bool setSign = false;
    if (txt[0] == '-') {
        setSign = true;
    }

    /**
     * Check for invalid characters
     */
    bool set_points = false;
    for (int ii = (setSign ? 1 : 0); ii < static_cast<int>(txt.size()); ii++) {
    	if ((txt[ii] < '0' || txt[ii] > '9') && txt[ii] != '.') {
            cout << "Invalid characters in decimal string: " << txt.c_str() << endl;
            exit(-1);
        }
    }

    size_t separatorPos = txt.find( '.', 0);
    if (separatorPos == string::npos) {
        const string wholeString = txt.substr( setSign ? 1 : 0, txt.size());
        const size_t wholeStringSize = wholeString.size();
        if (wholeStringSize > 26) {
        	cout << "Maximum precision exceeded. Maximum of 26 digits to the left of the decimal point";
        }
        TTInt whole(wholeString);
        if (setSign) {
            whole.SetSign();
        }
        whole *= kMaxScaleFactor;
        getDecimal() = whole;
        return;
    }

    if (txt.find( '.', separatorPos + 1) != string::npos) {
        cout << "Too many decimal points\n";
        exit(-1);
    }

    const string wholeString = txt.substr( setSign ? 1 : 0, separatorPos - (setSign ? 1 : 0));
    const size_t wholeStringSize = wholeString.size();
    if (wholeStringSize > 26) {
        cout << "Maximum precision exceeded. Maximum of 26 digits to the left of the decimal point\n";
        exit(-1);
    }
    TTInt whole(wholeString);
    string fractionalString = txt.substr( separatorPos + 1, txt.size() - (separatorPos + 1));
    // remove trailing zeros
    while (fractionalString.size() > 0 && fractionalString[fractionalString.size() - 1] == '0')
        fractionalString.erase(fractionalString.size() - 1, 1);
    // check if too many decimal places
    if (fractionalString.size() > 12) {
        cout << "Maximum scale exceeded. Maximum of 12 digits to the right of the decimal point\n";
        exit(-1);
    }
    while(fractionalString.size() < NValue::kMaxDecScale) {
        fractionalString.push_back('0');
    }
    TTInt fractional(fractionalString);

    whole *= kMaxScaleFactor;
    whole += fractional;

    if (setSign) {
        whole.SetSign();
    }

    getDecimal() = whole;
}


/**
 * Serialize sign and value using radix point (no exponent).
 */
std::string NValue::createStringFromDecimal(unsigned number_of_fractinal_digits) const {
    assert(!isNull());
    std::ostringstream buffer;
    TTInt scaledValue = getDecimal();
    if (scaledValue.IsSign()) {
        buffer << '-';
    }
    TTInt whole(scaledValue);
    TTInt fractional(scaledValue);
    whole /= NValue::kMaxScaleFactor;
    fractional %= NValue::kMaxScaleFactor;
    if (whole.IsSign()) {
        whole.ChangeSign();
    }
    buffer << whole.ToString(10);
    buffer << '.';
    if (fractional.IsSign()) {
        fractional.ChangeSign();
    }
    std::string fractionalString = fractional.ToString(10);
//    for (int ii = static_cast<int>(fractionalString.size()); ii < NValue::kMaxDecScale&&ii<number_of_fractinal_digits; ii++) {
    unsigned number_of_zero=0;
    for (int ii = static_cast<int>(fractionalString.size()); ii < NValue::kMaxDecScale&&number_of_zero<number_of_fractinal_digits; ii++,number_of_zero++) {
        buffer << '0';
    }
    number_of_fractinal_digits-=number_of_zero;
    buffer << (fractionalString.size()>number_of_fractinal_digits? fractionalString.substr(0,number_of_fractinal_digits):fractionalString);
    return buffer.str();
}

NValue NValue::opMultiplyDecimals(const NValue &lhs, const NValue &rhs) const {
//    if ((lhs.getValueType() != VALUE_TYPE_DECIMAL) ||
//        (rhs.getValueType() != VALUE_TYPE_DECIMAL))
//    {
//        cout << "Non-decimal NValue in decimal multiply\n";
//        exit(-1);
//    }

    if (lhs.isNull() || rhs.isNull()) {
        TTInt retval;
        retval.SetMin();
        return getDecimalValue( retval );
    }
    TTLInt calc;
	calc.FromInt(lhs.getDecimal());
	calc *= rhs.getDecimal();
	calc /= NValue::kMaxScaleFactor;
	TTInt retval;
	if (retval.FromInt(calc)  || retval > s_maxDecimalValue || retval < s_minDecimalValue) {
		cout << "Attempted to multiply " << lhs.createStringFromDecimal(12).c_str() << " by " << rhs.createStringFromDecimal(12).c_str() << " causing overflow/underflow. Unscaled result was " << calc.ToString(10).c_str() << endl;
		exit(-1);
	}
	return getDecimalValue(retval);
}

NValue NValue::opDivideDecimals(const NValue lhs, const NValue rhs) const {
//    if ((lhs.getValueType() != VALUE_TYPE_DECIMAL) ||
//        (rhs.getValueType() != VALUE_TYPE_DECIMAL))
//    {
//    	cout << "Non-decimal NValue in decimal divide\n";
//    	exit(-1);
//    }

    if (lhs.isNull() || rhs.isNull()) {
        TTInt retval;
        retval.SetMin();
        return getDecimalValue( retval );
    }

    TTLInt calc;
    calc.FromInt(lhs.getDecimal());
    calc *= NValue::kMaxScaleFactor;
    if (calc.Div(rhs.getDecimal())) {
        char message[4096];
        cout << "Attempted to divide " << lhs.createStringFromDecimal(12).c_str() << " by " << rhs.createStringFromDecimal(12).c_str() << "causing overflow/underflow (or divide by zero)\n";
        exit(-1);
    }
    TTInt retval;
    if (retval.FromInt(calc)  || retval > s_maxDecimalValue || retval < s_minDecimalValue) {
        char message[4096];
        cout << "Attempted to divide " << lhs.createStringFromDecimal(12).c_str() << " by " << rhs.createStringFromDecimal(12).c_str() << " causing overflow. Unscaled result was " << calc.ToString(10).c_str() << endl;
        exit(-1);
    }
    return getDecimalValue(retval);
}

} //endfor namespace
