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

#define CLAIMS_COMMON_DECIMAL_TTSIZE 4
#define CLAIMS_COMMON_DECIMAL_TTLSIZE 8
#define CLAIMS_COMMON_DECIMAL_MAXSCALE 30
#define CLAIMS_COMMON_DECIMAL_MAXPRCISION 72
#define CLAIMS_COMMON_DECIMAL_PSUBS \
  (CLAIMS_COMMON_DECIMAL_MAXPRCISION - CLAIMS_COMMON_DECIMAL_MAXSCALE)

#define NWORDS 1

// The int used for storage and return values
typedef ttmath::Int<CLAIMS_COMMON_DECIMAL_TTSIZE> TTInt;
// Long integer with space for multiplication and division without
// carry/overflow
typedef ttmath::Int<CLAIMS_COMMON_DECIMAL_TTLSIZE> TTLInt;

//#define DECIM_DEBUG

#ifdef DECIM_DEBUG
#define DEBUGOUT(A)    \
  do {                 \
    cout << A << endl; \
  } while (0)
#else
#define DEBUGOUT(A)
#endif

#define RETURNTODECIMAL()                                                 \
  do {                                                                    \
    string ress;                                                          \
    rett.ToString(ress);                                                  \
    if (rett.IsSign()) ress.erase(0, 1);                                  \
    while ((kMaxDecScale - (int)ress.length()) >= 0) ress.insert(0, "0"); \
    ress.insert(ress.length() - Decimal::kMaxDecScale, ".");              \
    if (rett.IsSign()) ress.insert(0, "-");                               \
    Decimal ret(CLAIMS_COMMON_DECIMAL_MAXPRCISION,                        \
                CLAIMS_COMMON_DECIMAL_MAXSCALE, ress);                    \
    return ret;                                                           \
  } while (0)

/**-----------------------------------------
 					  e/E
				      /   \
                          .      esign
				   /  \        \
                       /    \         ePower
                   whole     fractinal
                     /
                 issign
 --------------------------------------------
 		-123.456e-7
 		=>{
			 is_sign = true;
			 whole="123";
			 fractinal = "456";
			 esign = true;
			 ePower = "7";
		 }
 */
class DecimalString {
 public:
  DecimalString(bool isSign = false, string wholePart = "",
                string fractinalPart = "", bool eSign = false,
                string ePower = "")
      : is_sign_(isSign),
        whole_part_(wholePart),
        fractional_part_(fractinalPart),
        e_sign_(eSign),
        e_power_(ePower) {}

  DecimalString& operator=(const DecimalString& rhs) {
    if (this == &rhs) return *this;
    this->is_sign_ = rhs.is_sign_;
    this->whole_part_ = rhs.whole_part_;
    this->fractional_part_ = rhs.fractional_part_;
    this->e_sign_ = rhs.e_sign_;
    this->e_power_ = rhs.e_power_;
    return *this;
  }

  void PrintValue() {
    cout << setw(20) << "is_sign_:[" << is_sign_ << "]" << endl;
    cout << setw(20) << "whole_part_:[" << whole_part_ << "]" << endl;
    cout << setw(20) << "fractional_part_:[" << fractional_part_ << "]" << endl;
    cout << setw(20) << "e_sign_:[" << e_sign_ << "]" << endl;
    cout << setw(20) << "e_power_:[" << e_power_ << "]" << endl;
  }

  void clear() {
    is_sign_ = false;
    whole_part_ = "";
    fractional_part_ = "";
    e_sign_ = false;
    e_power_ = "";
  }

 public:
  bool is_sign_;
  string whole_part_;
  string fractional_part_;
  bool e_sign_;
  string e_power_;
};

/*
 *
 */
class Decimal {
 public:
  Decimal();
  Decimal(int precision, int scale, string valstr);
  virtual ~Decimal();
  static bool StringToDecimal(int p, int s, string strdec, bool* pissign = NULL,
                              string* pwhole = NULL, string* pfractinal = NULL);
  static bool StringToDecimal(string strdec, DecimalString& decstr);
  string ToString(unsigned number_of_fractinal_digits =
                      CLAIMS_COMMON_DECIMAL_MAXSCALE) const;
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
  void SetTTInt(TTInt value) { this->word[0] = value; }
  void SetTTInt(bool issign, string whole, string fractinal);
  TTInt Round(unsigned num) const;

 private:
  static const TTInt kMaxScaleFactor;
  static const int kMaxDecScale = CLAIMS_COMMON_DECIMAL_MAXSCALE;
  //  static const TTInt kMaxTTIntValue;
  //  static const TTInt kMinTTIntValue;

  const int precision_;
  const int scale_;
  TTInt word[NWORDS];
};

inline Decimal Decimal::op_add(const Decimal rhs) const {
  if ((this->isNull()) && (!rhs.isNull())) {
    return rhs;
  }
  if ((!this->isNull()) && (rhs.isNull())) {
    return *this;
  }
  if ((this->isNull()) && (rhs.isNull())) {
    return Decimal::CreateNullDecimal();
  }

  TTInt rett;
  rett = this->GetTTInt();
  rett.Add(rhs.GetTTInt());
  RETURNTODECIMAL();
}

inline Decimal Decimal::op_subtract(const Decimal rhs) const {
  if ((this->isNull()) && (!rhs.isNull())) {
    return rhs;
  }
  if ((!this->isNull()) && (rhs.isNull())) {
    return *this;
  }
  if ((this->isNull()) && (rhs.isNull())) {
    return Decimal(1, 0, "0");
  }

  TTInt rett;
  rett = this->GetTTInt();
  rett.Sub(rhs.GetTTInt());
  RETURNTODECIMAL();
}

inline Decimal Decimal::op_multiply(const Decimal rhs) const {
  if ((this->isNull()) && (!rhs.isNull())) {
    return rhs;
  }
  if ((!this->isNull()) && (rhs.isNull())) {
    return *this;
  }
  if ((this->isNull()) && (rhs.isNull())) {
    return Decimal::CreateNullDecimal();
  }
  TTLInt rett;
  rett = this->GetTTInt();
  rett *= rhs.GetTTInt();
  rett /= Decimal::kMaxScaleFactor;
  RETURNTODECIMAL();
}

inline Decimal Decimal::op_divide(const Decimal rhs) const {
  TTInt zero("0");
  if (rhs.isNull() || zero == rhs.GetTTInt() || this->isNull()) {
    return Decimal::CreateNullDecimal();
  }

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
