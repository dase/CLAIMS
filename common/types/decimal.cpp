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

namespace claims {
namespace common {

inline string& ltrim(string& ss, char c) {
	while (ss.size() > 0 && ss[0] == c) ss.erase(0, 1);
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


// 30 '0'  1000000000000000000000000000000
#define KMAXSCALEFACTOR "1000000000000000000000000000000"
#define NULLDECIMALSTRING "9999999999\
9999999999\
9999999999\
999999\
.\
9999999999\
9999999999\
9999999999"

const TTInt Decimal::kMaxScaleFactor = KMAXSCALEFACTOR;

Decimal::Decimal(int precision, int scale, string valuestr)
    : precision_(precision), scale_(scale) {
  // TODO Auto-generated constructor stub
  memset(word, 0, sizeof(word));
  StringToDecimal(valuestr);
}

Decimal::~Decimal() {
  // TODO Auto-generated destructor stub
}

bool Decimal::StringToDecimal(string strdec) {
  cout << "enter strdec.." << endl;
  cout << "1:" << strdec << endl;
  trim(strdec, ' ');

  cout << "2:" << strdec << endl;
  // true is negative, false is postive
  bool isSign = false;
  if (isSign = (strdec[0] == '-'))
    strdec.erase(0, 1);
  else if (strdec[0] == '+')
    strdec.erase(0, 1);

  cout << "3:" << strdec << endl;

  for (int ii = 0; ii < static_cast<int>(strdec.size()); ii++) {
    if ((strdec[ii] < '0' || strdec[ii] > '9') && (strdec[ii] != '.') &&
        (strdec[ii] != 'e') && (strdec[ii] != 'E') &&
        (strdec[ii]) != '-') {
      cout << "Invalid characters in decimal string: " << strdec << endl;
      return false;
    }
  }

  size_t epos = strdec.find('e');
  size_t Epos = strdec.find('E');
  size_t sciencenumpos = string::npos;
  cout << epos << "-" << Epos << "-" << string::npos << endl;
  if ((epos != string::npos) && (Epos != string::npos)) {
    cout << "illegal science number.." << endl;
    return false;
  }
  if ((epos != string::npos) && (Epos == string::npos)) {
    sciencenumpos = epos;
  } else if ((epos == string::npos) && (Epos != string::npos)) {
    sciencenumpos = Epos;
  }
  
  string numstr = strdec.substr(0, sciencenumpos);

  cout << "4:" << numstr << endl;

  size_t comma_pos = numstr.find('.', 0);
  string whole_part = "";
  string fractional_part = "";
  if (comma_pos != string::npos) {
    whole_part = numstr.substr(0, comma_pos);
    fractional_part = 
        numstr.substr(comma_pos + 1, numstr.size() - (comma_pos + 1));
  } else {
    whole_part = numstr;
  }
  cout << "5:" << whole_part << endl;
  cout << "6:" << fractional_part << endl;
  size_t child_comma_pos = fractional_part.find('.');
  if (child_comma_pos != string::npos) {
    cout << "Invalid fractional_part.." << endl;
    return false;
  }

  ltrim(whole_part, '0');
  rtrim(fractional_part, '0');

  int sic = 0;

  if (sciencenumpos != string::npos) {
    // science number like 1.23e4
    string estr =
        strdec.substr(sciencenumpos + 1, strdec.size() - (sciencenumpos + 1));
	if((estr[0] < '0' || estr[0] > '9')&&(estr[0] != '-'))
	{
		cout << "Invalid characters sign:" << estr[0] << endl;
		return false;
	}
    for (int ii = 1; ii < static_cast<int>(estr.size()); ii++) {
      if (estr[ii] < '0' || estr[ii] > '9') {
        cout << "Invalid characters:" << estr << endl;
        return false;
      }
    }
    cout << "7:" << estr << endl;
	bool eisSign = false;
	if(estr[0] == '-')
	{
		estr.erase(0, 1);
		eisSign = true;
	}
    sic = atoi(estr.c_str());
	if(eisSign)
	{
		sic = 0 - sic;
	}
	
  }

  int pvs = whole_part.size() + sic;
  cout << "13:" << sic << endl;

  if (pvs > precision_ - scale_) {
    cout << "8:" << pvs << endl;
    cout << "out of range.." << endl;
    return false;
  }
	if(sic >= 0)//1.23e4
	{
	  if (sic <= fractional_part.size()) {
	    whole_part.append(fractional_part.substr(0, sic));
	    fractional_part.erase(0, sic);
	  } else {
	    whole_part.append(fractional_part);
	    for (unsigned int ii = 0; ii < sic - fractional_part.size(); ii++) {
	      whole_part.append("0");
	    }
	    fractional_part.erase(0, fractional_part.size());
	  }
	}
	else//1.23e-4
	{
		cout << "12:" << sic << endl;
		cout << "still not support now!" << endl;
		return false;
	}
  cout << "9:" << whole_part << endl;
  cout << "10:" << fractional_part << endl;

  if (fractional_part.size() > (unsigned)scale_) {
    fractional_part.erase(scale_, fractional_part.size());
  }
  while (fractional_part.size() < kMaxDecScale) {
    fractional_part.push_back('0');
  }

  cout << "11:" << fractional_part << endl;
  TTInt whole(whole_part);
  TTInt fractional(fractional_part);

  whole *= kMaxScaleFactor;
  whole += fractional;

  if (isSign) {
	  whole.SetSign();
  }

   SetTTInt(whole);
  return true;
}

string Decimal::ToString(unsigned number_of_fractinal_digits) const{
    if(isNull())
		return "NULL";
    std::ostringstream buffer;
    TTInt scaledValue = GetTTInt();
    if (scaledValue.IsSign()) {
        buffer << '-';
    }
    TTInt whole(scaledValue);
    TTInt fractional(scaledValue);
    whole /= Decimal::kMaxScaleFactor;
    fractional %= Decimal::kMaxScaleFactor;
    if (whole.IsSign()) {
        whole.ChangeSign();
    }
    buffer << whole.ToString(10);
    buffer << '.';
    if (fractional.IsSign()) {
        fractional.ChangeSign();
    }
    std::string fractionalString = fractional.ToString(10);
    unsigned number_of_zero=0;
    for (int ii = static_cast<int>(fractionalString.size()); ii < Decimal::kMaxDecScale&&number_of_zero<number_of_fractinal_digits; ii++,number_of_zero++) {
        buffer << '0';
    }
    number_of_fractinal_digits-=number_of_zero;
    buffer << (fractionalString.size()>number_of_fractinal_digits? fractionalString.substr(0,number_of_fractinal_digits):fractionalString);
    return buffer.str();
}

Decimal Decimal::CreateNullDecimal()
{
	Decimal NDecimal(66, 30, NULLDECIMALSTRING);
	return NDecimal;
}

bool Decimal::isNull() const{
	TTInt NTTInt(NULLDECIMALSTRING);
	return NTTInt == GetTTInt();
}

Decimal & Decimal::operator=(const Decimal &rhs){
	if(this==&rhs) return *this;
	const_cast<int&>(this->precision_) = rhs.precision_;
	const_cast<int&>(this->scale_) = rhs.scale_;
	this->word[0] = rhs.GetTTInt();
	return *this;
}


void Decimal::PrintValue()
{
	cout << "value : [" <<ToString(scale_)<< "]" << endl;
}

}  // namespace common
}  // namespace claims
