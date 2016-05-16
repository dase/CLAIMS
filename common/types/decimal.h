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

#define DECIMAL_TTSIZE 4
#define DECIMAL_TTLSIZE 8
#define DECIMAL_MAXSCALE 30
#define DECIMAL_MAXPRCISION 72
#define DECIMAL_PSUBS \
  (DECIMAL_MAXPRCISION - DECIMAL_MAXSCALE)

#define NWORDS 1

#define DECIMAL_POS 	0x00
#define DECIMAL_NEG 	0x01
#define DECIMAL_NAN 	0x02
// The int used for storage and return values
typedef ttmath::Int<DECIMAL_TTSIZE> TTInt;
// Long integer with space for multiplication and division without
// carry/overflow
typedef ttmath::Int<DECIMAL_TTLSIZE> TTLInt;

//#define DECIM_DEBUG

#ifdef DECIM_DEBUG
#define DEBUGOUT(A)    \
  do {                 \
    cout << A << endl; \
  } while (0)
#else
#define DEBUGOUT(A)
#endif


/*
 *
 */
class Decimal {
 public:

  Decimal(int precision, int scale, string valstr);
  Decimal(int precision, int scale, const char * valstr);
  virtual ~Decimal();
  bool StrtoDecimal(int p, int s, const char *cp);
  string toString(unsigned number_of_fractinal_digits) const;
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

  Decimal& operator=(const Decimal& rhs);

  void PrintValue(int ifra);

  const TTInt& GetTTInt() const {
    const void* retval = reinterpret_cast<const void*>(&word[0]);
    return *reinterpret_cast<const TTInt*>(retval);
  }

 private:
  Decimal();
  void SetTTInt(TTInt value) { this->word[0] = value; }

 private:
  static const TTInt kMaxScaleFactor;
  static const int kMaxDecScale = DECIMAL_MAXSCALE;

  // Mark decimal NULL or not
  const char decimal_sign_;
  TTInt word[NWORDS];
};

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
