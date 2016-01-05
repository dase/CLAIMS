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
 * /CLAIMS/common/types/decimal.cpp
 *
 *  Created on: Dec 23, 2015
 *      Author: SCDONG, cswang
 *	 		 Email: cs_wang@infosys.com
 *
 * Description:
 *
 */

#include "./decimal.h"

#include <stdio.h>
#include <string>
#include <ctype.h>

namespace claims {
namespace common {

inline string& ltrim(string& ss, char c) {
  while (ss.size() > 0 && ss[0] == c)
    ss.erase(0, 1);
  return ss;
}
inline string& rtrim(string& ss, char c) {
  while (ss.size() > 0 && ss[ss.size() - 1] == c)
    ss.erase(ss.size() - 1, 1);
  return ss;
}
inline string& trim(string& st, char c) {
  ltrim(rtrim(st, c), c);
  return st;
}

inline bool isAllDigit(string &ss) {
  for (int ii = 0; ii < static_cast<int>(ss.size()); ii++) {
    if (!isdigit(ss[ii])) {
      return false;
    }
  }
  return true;
}

inline bool DoSign(string & ss) {
  bool isSign = false;
  if ((isSign = (ss[0] == '-')) || (ss[0] == '+')) ss.erase(0, 1);
  return isSign;
}

// 30 '0'  1000000000000000000000000000000
#define KMAXSCALEFACTOR "1000000000000000000000000000000"

// 45+30
#define NULLTTINTSTRING "9999999999\
9999999999\
9999999999\
9999999999\
99999\
9999999999\
9999999999\
9999999999"

/*
 // 42+30
 #define MAXTTINTSTRING "9999999999\
9999999999\
9999999999\
9999999999\
99\
9999999999\
9999999999\
9999999999"

 // -, 42+30
 #define MINTTINTSTRING "-9999999999\
9999999999\
9999999999\
9999999999\
99\
9999999999\
9999999999\
9999999999"
 */

const TTInt Decimal::kMaxScaleFactor = KMAXSCALEFACTOR;
//const TTInt Decimal::kMaxTTIntValue = MAXTTINTSTRING;
//const TTInt Decimal::kMinTTIntValue = MINTTINTSTRING;

Decimal::Decimal() {
  memset(word, 0, sizeof(word));
}

Decimal::Decimal(int precision, int scale, string valuestr) {
  // TODO Auto-generated constructor stub
  memset(word, 0, sizeof(word));

  bool issign = false;
  string whole = "";
  string fractinal = "";
  if (StringToDecimal(precision, scale, valuestr, &issign, &whole, &fractinal)) SetTTInt(
      issign, whole, fractinal);
}

Decimal::~Decimal() {
  // TODO Auto-generated destructor stub
}

bool Decimal::StringToDecimal(int p, int s, string strdec, bool * pissign,
                              string * pwhole, string * pfractinal) {
  DecimalString decstr;
  if (!StringToDecimal(strdec, decstr)) {
    cout << "Invalied string during convert to decimal:\"" << strdec << "\"."
         << endl;
    return false;
  }
  DEBUGOUT("To decimal:--------------------------------------");
  //assert((p>=s)&&(p-s)<=CLAIMS_COMMON_DECIMAL_PSUBS);
  const int psubs = p - s;
  if (psubs > CLAIMS_COMMON_DECIMAL_PSUBS) {
    cout << "Invalided precision and scale:\"" << p << "\", \"" << s << "\"."
         << endl;
    return false;
  }
  string whole_part(decstr.whole_part_);
  string fractional_part(decstr.fractional_part_);

  DEBUGOUT("1 whole_part:" << whole_part);
  DEBUGOUT("2 fractional_part:" << fractional_part);

  if (decstr.e_power_ != "") {
    int e_int_power = atoi(decstr.e_power_.c_str());
    DEBUGOUT("epower:" << e_int_power);
    if (decstr.e_sign_)  // -
    {
      if ((int) whole_part.size() - e_int_power > psubs) {
        cout << "Too Large Decimal value nearly:\"" << whole_part
            << "\" and \"-" << decstr.e_power_ << "\"." << endl;
        return false;
      }
      if (e_int_power <= (int) whole_part.size()) {
        fractional_part.insert(
            0, whole_part.substr(whole_part.size() - e_int_power, e_int_power));
        whole_part.erase(whole_part.size() - e_int_power, e_int_power);
      }
      else {
        fractional_part.insert(0, whole_part);
        for (unsigned int ii = 0; ii < e_int_power - whole_part.size(); ii++) {
          fractional_part.insert(0, "0");
        }
        whole_part = "";
      }
    }
    else  //+
    {
      /*
       if((int)whole_part.size() + e_int_power > psubs)
       {
       cout << "Too Large Decimal value nearly:\"" << whole_part << "\" and \"+" << decstr.e_power_ << "\"." << endl;
       return false;
       }
       */
      if (e_int_power <= (int) fractional_part.size()) {
        whole_part.append(fractional_part.substr(0, e_int_power));
        fractional_part.erase(0, e_int_power);
      }
      else {
        whole_part.append(fractional_part);
        for (unsigned int ii = 0; ii < e_int_power - fractional_part.size();
            ii++) {
          whole_part.append("0");
        }
        fractional_part = "";
      }

    }
  }

  ltrim(whole_part, '0');

  DEBUGOUT("3 whole_part:" << whole_part);
  DEBUGOUT("4 whole_part size:" << whole_part.size());
  DEBUGOUT("5 fractional_part:" << fractional_part);
  DEBUGOUT("6 fractional_part size:" << fractional_part.size());

  if ((int) whole_part.size() > psubs) {
    cout << "Too Large Decimal value nearly:\"" << strdec << "\"." << endl;
    return false;
  }

  // here now keep the max scale  30 decimal places
  if ((int) fractional_part.size() > Decimal::kMaxDecScale) {
    fractional_part.erase(Decimal::kMaxDecScale,
                          fractional_part.size() - Decimal::kMaxDecScale);
  }
  while (fractional_part.size() < Decimal::kMaxDecScale) {
    fractional_part.push_back('0');
  }
  DEBUGOUT("7 fractional_part:" << fractional_part);

  if ((NULL != pissign) && (NULL != pwhole) && (NULL != pfractinal)) {
    *pissign = decstr.is_sign_;
    *pwhole = whole_part;
    *pfractinal = fractional_part;
  }

  return true;
}

bool Decimal::StringToDecimal(string strdec, DecimalString & decstr) {
  bool is_sign = false;
  size_t epos = string::npos;
  size_t dot_pos = string::npos;
  string numstr = "";
  string whole_part = "";
  string fractional_part = "";
  string e_str_power = "";
  bool e_sign = false;

  DEBUGOUT("enter strdec...........................................");
  DEBUGOUT("1:" + strdec);
  trim(strdec, ' ');
  DEBUGOUT("2:" + strdec);
#if 0
  for (int ii = 0; ii < static_cast<int>(strdec.size()); ii++) {
    if (!isdigit(strdec[ii]) &&
        (strdec[ii] != '-') &&
        (strdec[ii] != '.') &&
        (strdec[ii] != 'e') &&
        (strdec[ii] != 'E') &&
        (strdec[ii] != '+')) {
      cout << "Invalid characters in decimal string: " << strdec << endl;
      return false;
    }
  }
#endif
  // true is negative, false is postive
  is_sign = DoSign(strdec);
  DEBUGOUT("3:" + strdec);
  epos = ((epos = strdec.find('e')) == string::npos) ? strdec.find('E') : epos;
  DEBUGOUT("epos:" << epos);

  numstr = strdec.substr(0, epos);
  DEBUGOUT("4:" + numstr);
  dot_pos = numstr.find('.', 0);

  whole_part = numstr.substr(0, dot_pos);
  ltrim(whole_part, '0');
  if (!isAllDigit(whole_part)) {
    cout << "Invalid characters in decimal whole part:\"" << whole_part << "\"."
         << endl;
    return false;
  }
  if (dot_pos != string::npos) {
    fractional_part = numstr.substr(dot_pos + 1, numstr.size() - (dot_pos + 1));
    rtrim(fractional_part, '0');
    if (!isAllDigit(fractional_part)) {
      cout << "Invalid characters in decimal fractional part:\""
           << fractional_part << "\"." << endl;
      return false;
    }
  }

  DEBUGOUT("5:" + whole_part);
  DEBUGOUT("6:" + fractional_part);

  if (epos != string::npos) {
    e_str_power = strdec.substr(epos + 1, strdec.size() - (epos + 1));
    DEBUGOUT("e_power:" + e_str_power);
    e_sign = DoSign(e_str_power);
    ltrim(e_str_power, '0');
    if (!isAllDigit(e_str_power)) {
      cout << "Invalid ePower:\"" << e_str_power << "\"." << endl;
      return false;
    }
    DEBUGOUT("7:" + e_str_power);
  }

  if (NULL != &decstr) {
    decstr.is_sign_ = is_sign;
    decstr.whole_part_ = whole_part;
    decstr.fractional_part_ = fractional_part;
    decstr.e_sign_ = e_sign;
    decstr.e_power_ = e_str_power;
  }

  return true;
}

void Decimal::SetTTInt(bool issign, string wholestr, string fractinalstr) {
  string whole_part(wholestr);
  string fractional_part(fractinalstr);

  TTInt whole(whole_part);
  TTInt fractional(fractional_part);
  whole *= Decimal::kMaxScaleFactor;
  whole += fractional;
  if (issign) {
    whole.SetSign();
  }

  SetTTInt(whole);
}

TTInt Decimal::Round(unsigned num) const
{
	TTInt out_value = this->word[0];
	string sfrafive = "5";
	while(Decimal::kMaxDecScale - (int)num - sfrafive.size() > 0 )
		sfrafive.append("0");
	DEBUGOUT("sfrafive: " << sfrafive);
	TTInt frafive_value(sfrafive.c_str());
	if(out_value.IsSign())
     out_value -= frafive_value;
	else
	 out_value += frafive_value;
	return out_value;
}

string Decimal::ToString(unsigned number_of_fractinal_digits) const {
  if (isNull()) return "NULL";
  assert(number_of_fractinal_digits <= Decimal::kMaxDecScale);
  string ress = "";
  TTInt rest = Round(number_of_fractinal_digits);
  rest.ToString(ress);
  if (rest.IsSign()) ress.erase(0, 1);
  while ((Decimal::kMaxDecScale - (int) ress.length()) >= 0)
    ress.insert(0, "0");
  ress.insert(ress.length() - Decimal::kMaxDecScale, ".");
  ress.erase(ress.size() - Decimal::kMaxDecScale + number_of_fractinal_digits,
             Decimal::kMaxDecScale - number_of_fractinal_digits);
  if (rest.IsSign()) ress.insert(0, "-");
  return ress;
}

Decimal Decimal::CreateNullDecimal() {
  Decimal NDecimal;
  TTInt NTTInt(NULLTTINTSTRING);
  NDecimal.SetTTInt(NTTInt);
  return NDecimal;
}

bool Decimal::isNull() const {
  TTInt NTTInt(NULLTTINTSTRING);
  return NTTInt == this->GetTTInt();
}

Decimal & Decimal::operator=(const Decimal &rhs) {
  if (this == &rhs) return *this;
  this->word[0] = rhs.GetTTInt();
  return *this;
}

void Decimal::PrintValue(int ifra) {
  cout << "value : [" << ToString(ifra) << "]" << endl;
}

}  // namespace common
}  // namespace claims
