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

#include <limits.h>
#include <ctype.h>
#include <errno.h>
#include <stdio.h>
#include <memory.h>
#include <iostream>

#include <stdio.h>
#include <string>
#include <ctype.h>
#include <glog/logging.h>

namespace claims {
namespace common {

// 30 '0'  1000000000000000000000000000000
#define KMAXSCALEFACTOR "1000000000000000000000000000000"


const TTInt Decimal::kMaxScaleFactor = KMAXSCALEFACTOR;

Decimal::Decimal()
	: decimal_sign_(DECIMAL_POS) {
  memset(word, 0, sizeof(word));
}

Decimal::Decimal(int precision, int scale, string valuestr)
	: decimal_sign_(DECIMAL_POS) {
	memset(word, 0, sizeof(word));
  	StrtoDecimal(precision, scale, valuestr.c_str());
}

Decimal::Decimal(int precision, int scale, const char * valstr)
	: decimal_sign_(DECIMAL_POS) {
	memset(word, 0, sizeof(word));
	StrtoDecimal(precision, scale, valstr);
}

Decimal::~Decimal() {
  // TODO Auto-generated destructor stub
}

bool Decimal::StrtoDecimal(int p, int s, const char *cp)
{
	bool		have_dp = false;
	int			i = 0;
    char decdigits[DECIMAL_MAXPRCISION+1];
	char num1[31];
    int dsign = DECIMAL_POS;
    int dweight = -1;
	const char *str = cp;
	
	memset(decdigits, '0', DECIMAL_MAXPRCISION+1);
	num1[0] = '1';
	memset(num1+1, '0', 30);
	
	while (isspace(*cp))
		cp++;
	//printf("source :%s\n",cp);
	switch (*cp)
	{
		case '+':
			cp++;
			break;
		case '-':
			dsign = DECIMAL_NEG;
			cp++;
			break;
	}

	while (*cp)
	{
		if (isdigit((unsigned char) *cp))
		{
			decdigits[i++] = *cp++;
			if (!have_dp)
				dweight++;
		}
		else if (*cp == '.')
		{
			if (have_dp)
			{
				printf("invalid input syntax for type numeric: \"%s\"\n", str);
				return false;
			}
			have_dp = true;
			cp++;
		}
		else
			break;
	}
	if (*cp == 'e' || *cp == 'E')
	{
		long		exponent;
		char	   *endptr;
		cp++;
		exponent = strtol(cp, &endptr, 10);
		if (endptr == cp)
		{
			printf("invalid input syntax for type numeric: \"%s\"\n", str);
			return false;
		}
		cp = endptr;
		dweight += (int) exponent;
	}
	if (dweight > p - s )
	{
		printf("invalid input syntax for type numeric: \"%s\"\n",	str);
		return false;
	}
	while (*cp)
	{
		if (isspace((unsigned char) *cp))
		{
			cp++;
		}
		else
		{
			printf("invalid input syntax for type numeric: \"%s\"\n",  str);
			return false;
		}
	}

	decdigits[DECIMAL_MAXSCALE + dweight + 1] = 0;
	char c = decdigits[s + dweight + 1];
	memset(decdigits + s + dweight + 1, '0', DECIMAL_MAXSCALE - s);
	TTInt whole(decdigits);
	
	num1[30 - s + 1] = 0;
	TTInt fra(num1);

	if (DECIMAL_NEG == dsign)
	{
		fra.SetSign();
		whole.SetSign();
	}
	SetTTInt((c>='5'?whole+fra:whole));	
	return true;
}

string Decimal::toString(unsigned number_of_fractinal_digits) const {
  if (isNull()) return "NULL";
  string ress = "";
  TTInt rest = this->word[0];
  rest.ToString(ress);
  int sign = 0;
  if (rest.IsSign()) sign = 1;
  while ((Decimal::kMaxDecScale + sign - (int) ress.length()) >= 0)
    ress.insert(sign, "0");
  if(number_of_fractinal_digits > 0)
    ress.insert(ress.length() - Decimal::kMaxDecScale, ".");
  ress.erase(ress.size() - Decimal::kMaxDecScale + number_of_fractinal_digits,
             Decimal::kMaxDecScale - number_of_fractinal_digits);
  return ress;
}

Decimal Decimal::CreateNullDecimal() {
  static Decimal NDecimal;
  const_cast<char&>(NDecimal.decimal_sign_) = DECIMAL_NAN;
  return NDecimal;
}

bool Decimal::isNull() const {
  return decimal_sign_ == DECIMAL_NAN;
}

Decimal & Decimal::operator=(const Decimal &rhs) {
  if (this == &rhs) return *this;
  this->word[0] = rhs.GetTTInt();
  const_cast<char&>(this->decimal_sign_) = rhs.decimal_sign_;
  //PrintValue(10);
  return *this;
}

void Decimal::PrintValue(int ifra) {
  printf("value:[%s][%s]\n", this->word[0].ToString().c_str(), toString(ifra).c_str());
}

/////////////////////////////////////////////////////////////////////
Decimal Decimal::op_add(const Decimal rhs) const {

  TTInt rett;
  rett = this->GetTTInt();
  rett.Add(rhs.GetTTInt());

  Decimal ret;
  ret.SetTTInt(rett);

  return ret;
}

Decimal Decimal::op_subtract(const Decimal rhs) const {

  TTInt rett;
  rett = this->GetTTInt();
  rett.Sub(rhs.GetTTInt());

  Decimal ret;
  ret.SetTTInt(rett);

  return ret;
}

Decimal Decimal::op_multiply(const Decimal rhs) const {
  
  TTLInt rett;
  rett = this->GetTTInt();
  rett *= rhs.GetTTInt();
  rett /= Decimal::kMaxScaleFactor;

  Decimal ret;
  ret.SetTTInt(rett);

  return ret;
}

Decimal Decimal::op_divide(const Decimal rhs) const {
  
  TTLInt rett;
  rett = this->GetTTInt();
  rett *= Decimal::kMaxScaleFactor;
  rett /= rhs.GetTTInt();

  Decimal ret;
  ret.SetTTInt(rett);
  
  return ret;
}


}  // namespace common
}  // namespace claims
