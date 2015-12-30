/*
 * Copyright [2012-2015] DaSE@ECNU
 *
 * Licensed to the Apache Software Foundation (ASF) under one or more
 * contributor license agreements.  See the NOTICE file distributed with
 * this work for additional information regarding copyright ownership.
 * The ASF licenses this file to You under the Apache License, Version 2.0
 * (the "License"); you may not use this file except in compliance with
 * the License.  You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 *
 * /CLAIMS/common/types/decimal.h
 *
 *  Created on: Dec 23, 2015
 *      Author: SCDONG, cswang
 *	 		 Email: cs_wang@infosys.com
 *
 * Description:
 *
 */

#ifndef COMMON_TYPES_DECIMAL_H_
#define COMMON_TYPES_DECIMAL_H_
#include <stdint.h>
#include <boost/functional/hash.hpp>
#include "./ttmath/ttmathint.h"
using namespace std;

namespace claims {
namespace common {

// The int used for storage and return values
typedef ttmath::Int<4> TTInt;
// Long integer with space for multiplication and division without
// carry/overflow
typedef ttmath::Int<8> TTLInt;

#define NWORDS 1

#define RETURNTODECIMAL() do\
{\
  string result;\
  rett.ToString(result);\
  if(rett.IsSign())\
  	result.erase(0,1);\
  while((kMaxDecScale-(int)result.length())>=0)\
	result.insert(0, "0");\
  result.insert(result.length()-kMaxDecScale, ".");\
  if(rett.IsSign())\
  	result.insert(0,"-");\
  Decimal ret(65, 30, result);\
  return ret;\
}while(0)

/*
 *
 */
class Decimal {
 public:
  Decimal(int precision, int scale, string valstr);
  virtual ~Decimal();

  bool StringToDecimal(string strdec);
  string ToString(unsigned number_of_fractinal_digits=30) const;
  static Decimal CreateNullDecimal();
  bool isNull() const;

  Decimal op_add(const Decimal rhs) const;
  Decimal op_subtract(const Decimal rhs) const;
  Decimal op_multiply(const Decimal rhs) const;
  Decimal op_divide(const Decimal rhs) const;
  Decimal op_max(const Decimal rhs) const;
  Decimal op_min(const Decimal rhs) const;

  bool op_equals(const Decimal rhs) const;
  bool op_not_equals(const Decimal rhs) const;
  bool op_less(const Decimal rhs) const;
  bool op_less_equals(const Decimal rhs) const;
  bool op_great(const Decimal rhs) const;
  bool op_great_equals(const Decimal rhs) const;

  int compare(const Decimal rhs) const;

  Decimal & operator=(const Decimal &rhs);

  void PrintValue();

  const TTInt& GetTTInt() const {
    const void* retval = reinterpret_cast<const void*>(&word[0]);
    return *reinterpret_cast<const TTInt*>(retval);
  }
private:
  void SetTTInt(TTInt value) { this->word[0] = value; }

 private:
  static const TTInt kMaxScaleFactor;
  static const int kMaxDecScale = 30;
  const int precision_;
  const int scale_;
  TTInt word[NWORDS];
};

inline Decimal Decimal::op_add(const Decimal rhs) const {
  TTInt rett;
  rett = this->GetTTInt();
  rett.Add(rhs.GetTTInt());
  RETURNTODECIMAL();
}

inline Decimal Decimal::op_subtract(const Decimal rhs) const {
  TTInt rett;
  rett = this->GetTTInt();
  rett.Sub(rhs.GetTTInt());
  RETURNTODECIMAL();
}

inline Decimal Decimal::op_multiply(const Decimal rhs) const {
  TTLInt rett;
  rett = this->GetTTInt();
  rett *= rhs.GetTTInt();
  rett /= Decimal::kMaxScaleFactor;
  RETURNTODECIMAL();
}

inline Decimal Decimal::op_divide(const Decimal rhs) const {
  TTLInt rett;
  rett = this->GetTTInt();
  rett *= Decimal::kMaxScaleFactor;
  rett /= rhs.GetTTInt();
  RETURNTODECIMAL();
}

inline int Decimal::compare(const Decimal rhs) const {
  const TTInt l = this->GetTTInt();
  const TTInt r = rhs.GetTTInt();
  if (l == r)
    return 0;
  else if (l > r)
    return 1;
  else
    return -1;
}

inline Decimal Decimal::op_max(const Decimal rhs) const {
  const int value = compare(rhs);
  if (value > 0) {
    return *this;
  } else {
    return rhs;
  }
}

inline Decimal Decimal::op_min(const Decimal rhs) const {
  const int value = compare(rhs);
  if (value < 0) {
    return *this;
  } else {
    return rhs;
  }
}

inline bool Decimal::op_equals(const Decimal rhs) const {
  return compare(rhs) == 0 ? true : false;
}
inline bool Decimal::op_not_equals(const Decimal rhs) const {
  return compare(rhs) == 0 ? false : true;
}
inline bool Decimal::op_less(const Decimal rhs) const {
  return compare(rhs) < 0 ? true : false;
}
inline bool Decimal::op_less_equals(const Decimal rhs) const {
  return compare(rhs) > 0 ? false : true;
}
inline bool Decimal::op_great(const Decimal rhs) const {
  return compare(rhs) > 0 ? true : false;
}
inline bool Decimal::op_great_equals(const Decimal rhs) const {
  return compare(rhs) < 0 ? false : true;
}

}  // namespace common
}  // namespace claims
#endif /* COMMON_TYPES_DECIMAL_H_ */
