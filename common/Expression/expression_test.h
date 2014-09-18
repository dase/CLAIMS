/*
 * expression_test.h
 *
 *  Created on: Aug 26, 2014
 *      Author: imdb
 */

#ifndef EXPRESSION_TEST_H_
#define EXPRESSION_TEST_H_
#include <gtest/gtest.h>
#include<iostream>
#include<stdio.h>
#include<algorithm>
#include<string.h>
#include<string>
#include<math.h>
#include "queryfunc.h"
#include "qnode.h"
using namespace std;
#define VAL_SIZE 256
int test_string_like(char *str,char *pat)
{
	FuncCallInfo finfo;
	char val[VAL_SIZE];
	finfo->args[0]=str;
	finfo->args[1]=pat;
	finfo->results=(void *)val;
	finfo->nargs=2;
	string_like(finfo);
	return *(bool *)finfo->results;
}
string test_string_upper(char * str)
{
	FuncCallInfo finfo;
	char val[VAL_SIZE];
	finfo->args[0]=str;
	finfo->results=(void *)val;
	finfo->nargs=1;
	string_upper(finfo);
	return string((char *)finfo->results);
}
string test_string_substring(char *str,int frompoint,int endpoint)
{
	FuncCallInfo finfo;
	char val[VAL_SIZE];
	finfo->args[0]=str;
	finfo->args[1]=&frompoint;
	finfo->args[2]=&endpoint;
	finfo->results=(void *)val;
	finfo->nargs=3;
	string_substring(finfo);
	return string((char *)finfo->results);
}

string test_string_both_trim(char *str,char *trimstr)
{
	FuncCallInfo finfo;
	char val[VAL_SIZE];
	finfo->args[0]=str;
	finfo->args[1]=trimstr;
	finfo->results=(void *)val;
	finfo->nargs=2;
	string_both_trim(finfo);
	return string((char *)finfo->results);
}
string test_string_leading_trim(char *str,char *trimstr)
{
	FuncCallInfo finfo;
	char val[VAL_SIZE];
	finfo->args[0]=str;
	finfo->args[1]=trimstr;
	finfo->results=(void *)val;
	finfo->nargs=2;
	string_leading_trim(finfo);
	return string((char *)finfo->results);
}
string test_string_trailing_trim(char *str,char *trimstr)
{
	FuncCallInfo finfo;
	char val[VAL_SIZE];
	finfo->args[0]=str;
	finfo->args[1]=trimstr;
	finfo->results=(void *)val;
	finfo->nargs=2;
	string_trailing_trim(finfo);
	return string((char *)finfo->results);
}
TEST(DATA_TYPE,STRING_LIKE)
{
	EXPECT_EQ(1,test_string_like("abcdefe","%a_c%"));
}
TEST(DATA_TYPE,STRING_UPPER)
{
	EXPECT_STREQ("  STRING_UPPER+-*/STRING ",test_string_upper("  string_upper+-*/string "));
}
TEST(DATA_TYPE,STRING_SUBSTRING)
{
	EXPECT_STREQ("string",test_string_substring(" string_substring ",1,7));
}
TEST(DATA_TYPE,STRING_BOTH_TRIM)
{
	EXPECT_STREQ("trim_blank",test_string_both_trim("  trim_blank   "," "));
}
TEST(DATA_TYPE,STRING_LEADING_TRIM)
{
	EXPECT_STREQ("trim_blank   ",test_string_leading_trim("  trim_blank   "," "));
}
TEST(DATA_TYPE,STRING_TRAILING_TRIM)
{
	EXPECT_STREQ("  trim_blank",test_string_trailing_trim("  trim_blank   "," "));
}
#endif /* EXPRESSION_TEST_H_ */
