/*
 * types.cpp
 *
 *  Created on: 2014年1月16日
 *      Author: SCDONG
 */

#include <iostream>
using namespace std;
#include "types.h"

namespace decimal {
string getTypeName(ValueType type) {
    string ret;
    switch (type) {
        case (VALUE_TYPE_DECIMAL):
            ret = "decimal";
            break;
        default: {
            char buffer[32];
            cout << "UNKNOWN[" << type << "]" << endl;
            ret = buffer;
        }
    }
    return (ret);
}

string valueToString(ValueType type)
{
    switch (type) {
      case VALUE_TYPE_INVALID: {
          return "INVALID";
      }
      case VALUE_TYPE_DECIMAL: {
          return "DECIMAL";
      }
      default:
          return "INVALID";
    }
}

} //endfor namespace
