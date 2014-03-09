
/* A Bison parser, made by GNU Bison 2.4.1.  */

/* Skeleton implementation for Bison GLR parsers in C
   
      Copyright (C) 2002, 2003, 2004, 2005, 2006 Free Software Foundation, Inc.
   
   This program is free software: you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation, either version 3 of the License, or
   (at your option) any later version.
   
   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.
   
   You should have received a copy of the GNU General Public License
   along with this program.  If not, see <http://www.gnu.org/licenses/>.  */

/* As a special exception, you may create a larger work that contains
   part or all of the Bison parser skeleton and distribute that work
   under terms of your choice, so long as that work isn't itself a
   parser generator using the skeleton or a modified version thereof
   as a parser skeleton.  Alternatively, if you modify or redistribute
   the parser skeleton itself, you may (at your option) remove this
   special exception, which will cause the skeleton and the resulting
   Bison output files to be licensed under the GNU General Public
   License without this special exception.
   
   This special exception was added by the Free Software Foundation in
   version 2.2 of Bison.  */

/* C GLR parser skeleton written by Paul Hilfinger.  */

/* Identify Bison output.  */
#define YYBISON 1

/* Bison version.  */
#define YYBISON_VERSION "2.4.1"

/* Skeleton name.  */
#define YYSKELETON_NAME "glr.c"

/* Pure parsers.  */
#define YYPURE 0

/* Using locations.  */
#define YYLSP_NEEDED 1




/* Copy the first part of user declarations.  */

/* Line 172 of glr.c  */
#line 24 "sql.ypp"

#include <stdlib.h>
#include <stdarg.h>
#include <string.h>
#include <iostream>
#include <stdio.h>
#include <malloc.h>
#include "sql_node_struct.h"
using namespace std;

Node * parsetreeroot = NULL;
char globalInputText[10000];
int globalReadOffset;
Node *NodePointer[10000];		// 2014-3-6---指向每个节点的指针数组---by余楷
int NodePointerNum = 0;		// 2014-3-6---指向每个节点的指针数组中元素个数---by余楷
int errorNumber = 0;			// 2014-3-6---声明SQL解析错误数---by余楷
int readInputForLexer( char *buffer, int *numBytesRead, int maxBytesToRead );
char linebuf[500];


/* Line 172 of glr.c  */
#line 80 "sql.tab.cpp"



#include "sql.tab.hpp"

/* Enabling traces.  */
#ifndef YYDEBUG
# define YYDEBUG 0
#endif

/* Enabling verbose error messages.  */
#ifdef YYERROR_VERBOSE
# undef YYERROR_VERBOSE
# define YYERROR_VERBOSE 1
#else
# define YYERROR_VERBOSE 0
#endif

/* Enabling the token table.  */
#ifndef YYTOKEN_TABLE
# define YYTOKEN_TABLE 0
#endif

/* Default (constant) value used for initialization for null
   right-hand sides.  Unlike the standard yacc.c template,
   here we set the default value of $$ to a zeroed-out value.
   Since the default value is undefined, this behavior is
   technically correct.  */
static YYSTYPE yyval_default;

/* Copy the second part of user declarations.  */


/* Line 243 of glr.c  */
#line 115 "sql.tab.cpp"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#ifndef YY_
# if YYENABLE_NLS
#  if ENABLE_NLS
#   include <libintl.h> /* INFRINGES ON USER NAME SPACE */
#   define YY_(msgid) dgettext ("bison-runtime", msgid)
#  endif
# endif
# ifndef YY_
#  define YY_(msgid) msgid
# endif
#endif

/* Suppress unused-variable warnings by "using" E.  */
#if ! defined lint || defined __GNUC__
# define YYUSE(e) ((void) (e))
#else
# define YYUSE(e) /* empty */
#endif

/* Identity function, used to suppress warnings about constant conditions.  */
#ifndef lint
# define YYID(n) (n)
#else
#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
static int
YYID (int i)
#else
static int
YYID (i)
    int i;
#endif
{
  return i;
}
#endif

#ifndef YYFREE
# define YYFREE free
#endif
#ifndef YYMALLOC
# define YYMALLOC malloc
#endif
#ifndef YYREALLOC
# define YYREALLOC realloc
#endif

#define YYSIZEMAX ((size_t) -1)

#ifdef __cplusplus
   typedef bool yybool;
#else
   typedef unsigned char yybool;
#endif
#define yytrue 1
#define yyfalse 0

#ifndef YYSETJMP
# include <setjmp.h>
# define YYJMP_BUF jmp_buf
# define YYSETJMP(env) setjmp (env)
# define YYLONGJMP(env, val) longjmp (env, val)
#endif

/*-----------------.
| GCC extensions.  |
`-----------------*/

#ifndef __attribute__
/* This feature is available in gcc versions 2.5 and later.  */
# if (! defined __GNUC__ || __GNUC__ < 2 \
      || (__GNUC__ == 2 && __GNUC_MINOR__ < 5) || __STRICT_ANSI__)
#  define __attribute__(Spec) /* empty */
# endif
#endif

#define YYOPTIONAL_LOC(Name) Name

#ifndef YYASSERT
# define YYASSERT(condition) ((void) ((condition) || (abort (), 0)))
#endif

/* YYFINAL -- State number of the termination state.  */
#define YYFINAL  240
/* YYLAST -- Last index in YYTABLE.  */
#define YYLAST   2955

/* YYNTOKENS -- Number of terminals.  */
#define YYNTOKENS  334
/* YYNNTS -- Number of nonterminals.  */
#define YYNNTS  168
/* YYNRULES -- Number of rules.  */
#define YYNRULES  595
/* YYNRULES -- Number of states.  */
#define YYNSTATES  1204
/* YYMAXRHS -- Maximum number of symbols on right-hand side of rule.  */
#define YYMAXRHS 13
/* YYMAXLEFT -- Maximum number of symbols to the left of a handle
   accessed by $0, $-1, etc., in any rule.  */
#define YYMAXLEFT 0

/* YYTRANSLATE(X) -- Bison symbol number corresponding to X.  */
#define YYUNDEFTOK  2
#define YYMAXUTOK   573

#define YYTRANSLATE(YYX)						\
  ((unsigned int) (YYX) <= YYMAXUTOK ? yytranslate[YYX] : YYUNDEFTOK)

/* YYTRANSLATE[YYLEX] -- Bison symbol number corresponding to YYLEX.  */
static const unsigned short int yytranslate[] =
{
       0,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,    17,     2,     2,     2,    28,    22,     2,
     331,   332,    26,    24,   329,    25,   330,    27,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,   328,
       2,     2,     2,   333,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,    30,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,    21,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     1,     2,     3,     4,
       5,     6,     7,     8,     9,    10,    11,    12,    13,    14,
      15,    16,    18,    19,    20,    23,    29,    31,    32,    33,
      34,    35,    36,    37,    38,    39,    40,    41,    42,    43,
      44,    45,    46,    47,    48,    49,    50,    51,    52,    53,
      54,    55,    56,    57,    58,    59,    60,    61,    62,    63,
      64,    65,    66,    67,    68,    69,    70,    71,    72,    73,
      74,    75,    76,    77,    78,    79,    80,    81,    82,    83,
      84,    85,    86,    87,    88,    89,    90,    91,    92,    93,
      94,    95,    96,    97,    98,    99,   100,   101,   102,   103,
     104,   105,   106,   107,   108,   109,   110,   111,   112,   113,
     114,   115,   116,   117,   118,   119,   120,   121,   122,   123,
     124,   125,   126,   127,   128,   129,   130,   131,   132,   133,
     134,   135,   136,   137,   138,   139,   140,   141,   142,   143,
     144,   145,   146,   147,   148,   149,   150,   151,   152,   153,
     154,   155,   156,   157,   158,   159,   160,   161,   162,   163,
     164,   165,   166,   167,   168,   169,   170,   171,   172,   173,
     174,   175,   176,   177,   178,   179,   180,   181,   182,   183,
     184,   185,   186,   187,   188,   189,   190,   191,   192,   193,
     194,   195,   196,   197,   198,   199,   200,   201,   202,   203,
     204,   205,   206,   207,   208,   209,   210,   211,   212,   213,
     214,   215,   216,   217,   218,   219,   220,   221,   222,   223,
     224,   225,   226,   227,   228,   229,   230,   231,   232,   233,
     234,   235,   236,   237,   238,   239,   240,   241,   242,   243,
     244,   245,   246,   247,   248,   249,   250,   251,   252,   253,
     254,   255,   256,   257,   258,   259,   260,   261,   262,   263,
     264,   265,   266,   267,   268,   269,   270,   271,   272,   273,
     274,   275,   276,   277,   278,   279,   280,   281,   282,   283,
     284,   285,   286,   287,   288,   289,   290,   291,   292,   293,
     294,   295,   296,   297,   298,   299,   300,   301,   302,   303,
     304,   305,   306,   307,   308,   309,   310,   311,   312,   313,
     314,   315,   316,   317,   318,   319,   320,   321,   322,   323,
     324,   325,   326,   327
};

#if YYDEBUG
/* YYPRHS[YYN] -- Index of the first RHS symbol of rule number YYN in
   YYRHS.  */
static const unsigned short int yyprhs[] =
{
       0,     0,     3,     6,    10,    13,    17,    19,    23,    35,
      36,    39,    40,    45,    48,    53,    54,    56,    58,    59,
      62,    63,    66,    67,    71,    72,    75,    80,    81,    84,
      86,    90,    91,    94,    97,   100,   103,   106,   109,   112,
     115,   117,   121,   123,   127,   130,   132,   136,   138,   140,
     143,   148,   152,   156,   157,   159,   162,   164,   165,   171,
     175,   181,   188,   194,   195,   197,   199,   200,   202,   204,
     206,   209,   212,   213,   214,   216,   219,   224,   226,   230,
     234,   236,   244,   247,   250,   253,   254,   261,   264,   269,
     270,   273,   281,   283,   292,   293,   299,   300,   303,   306,
     309,   312,   314,   315,   316,   320,   324,   330,   332,   334,
     338,   342,   350,   358,   362,   366,   372,   378,   380,   389,
     397,   405,   407,   416,   417,   420,   423,   427,   433,   439,
     447,   449,   454,   459,   460,   464,   466,   475,   486,   496,
     503,   515,   524,   528,   529,   531,   533,   534,   536,   538,
     542,   546,   552,   557,   562,   568,   574,   575,   579,   582,
     586,   590,   594,   598,   601,   605,   609,   612,   616,   622,
     624,   635,   649,   652,   654,   657,   661,   663,   668,   673,
     675,   681,   683,   687,   691,   698,   702,   706,   710,   715,
     718,   722,   726,   727,   729,   730,   732,   734,   745,   746,
     748,   750,   752,   753,   756,   758,   760,   762,   766,   770,
     772,   778,   780,   785,   790,   791,   794,   796,   803,   805,
     809,   813,   819,   820,   822,   824,   826,   830,   831,   835,
     843,   849,   859,   861,   864,   867,   869,   872,   874,   876,
     878,   881,   884,   885,   887,   892,   893,   897,   900,   901,
     904,   906,   911,   913,   918,   920,   922,   924,   927,   933,
     937,   939,   943,   945,   949,   953,   955,   958,   961,   963,
     965,   968,   970,   977,   978,   980,   982,   985,   988,   991,
     993,   995,   999,  1001,  1005,  1007,  1012,  1018,  1020,  1024,
    1026,  1030,  1032,  1042,  1044,  1048,  1050,  1053,  1055,  1057,
    1059,  1061,  1063,  1065,  1067,  1069,  1072,  1074,  1075,  1078,
    1081,  1084,  1087,  1090,  1093,  1095,  1105,  1107,  1117,  1119,
    1123,  1126,  1129,  1132,  1134,  1136,  1138,  1140,  1142,  1145,
    1147,  1156,  1158,  1168,  1170,  1180,  1182,  1186,  1190,  1196,
    1198,  1207,  1208,  1211,  1213,  1218,  1219,  1221,  1223,  1225,
    1231,  1233,  1238,  1239,  1241,  1243,  1245,  1247,  1249,  1251,
    1256,  1257,  1259,  1261,  1263,  1268,  1270,  1276,  1277,  1280,
    1283,  1286,  1288,  1294,  1296,  1301,  1305,  1313,  1318,  1323,
    1328,  1332,  1336,  1341,  1346,  1350,  1354,  1358,  1363,  1369,
    1373,  1377,  1383,  1386,  1390,  1395,  1401,  1407,  1412,  1417,
    1421,  1425,  1426,  1428,  1429,  1432,  1433,  1436,  1437,  1440,
    1441,  1443,  1445,  1451,  1453,  1457,  1459,  1463,  1464,  1466,
    1468,  1470,  1474,  1476,  1480,  1482,  1484,  1486,  1488,  1491,
    1493,  1496,  1499,  1504,  1506,  1508,  1512,  1513,  1515,  1517,
    1519,  1525,  1529,  1534,  1535,  1538,  1540,  1543,  1545,  1549,
    1551,  1554,  1556,  1558,  1563,  1567,  1571,  1575,  1576,  1579,
    1581,  1583,  1586,  1588,  1592,  1596,  1600,  1601,  1605,  1611,
    1612,  1614,  1615,  1618,  1621,  1622,  1627,  1631,  1634,  1638,
    1642,  1646,  1650,  1654,  1658,  1662,  1666,  1670,  1674,  1676,
    1678,  1680,  1682,  1684,  1688,  1694,  1697,  1702,  1704,  1706,
    1708,  1710,  1714,  1718,  1722,  1726,  1732,  1738,  1740,  1744,
    1746,  1748,  1752,  1756,  1758,  1760,  1762,  1764,  1768,  1772,
    1776,  1780,  1784,  1788,  1791,  1794,  1798,  1802,  1806,  1810,
    1816,  1823,  1830,  1837,  1841,  1845,  1849,  1853,  1856,  1859,
    1863,  1867,  1871,  1876,  1880,  1885,  1891,  1893,  1897,  1898,
    1900,  1906,  1913,  1919,  1926,  1931,  1937,  1942,  1947,  1952,
    1957,  1962,  1967,  1972,  1977,  1984,  1993,  1998,  2006,  2008,
    2010,  2012,  2019,  2026,  2030,  2034,  2038,  2042,  2046,  2050,
    2054,  2058,  2062,  2067,  2074,  2078,  2084,  2089,  2095,  2099,
    2104,  2108,  2113,  2115,  2117,  2119
};

/* YYRHS -- A `-1'-separated list of the rules' RHS.  */
static const short int yyrhs[] =
{
     335,     0,    -1,   336,   328,    -1,   335,   336,   328,    -1,
       1,   328,    -1,   335,     1,   328,    -1,   337,    -1,   189,
     348,   349,    -1,   189,   348,   349,   105,   351,   338,   339,
     343,   344,   345,   346,    -1,    -1,   239,   487,    -1,    -1,
     108,    47,   340,   342,    -1,   487,   341,    -1,   487,   341,
     329,   340,    -1,    -1,    39,    -1,    81,    -1,    -1,   241,
     185,    -1,    -1,   109,   487,    -1,    -1,   161,    47,   340,
      -1,    -1,   134,   487,    -1,   134,   487,   329,   487,    -1,
      -1,   125,   347,    -1,     3,    -1,     3,   329,   347,    -1,
      -1,   348,    33,    -1,   348,    84,    -1,   348,    85,    -1,
     348,   110,    -1,   348,   208,    -1,   348,   205,    -1,   348,
     203,    -1,   348,   204,    -1,   350,    -1,   350,   329,   349,
      -1,    26,    -1,    26,   329,   349,    -1,   487,   355,    -1,
     352,    -1,   352,   329,   351,    -1,   353,    -1,   356,    -1,
       3,   355,    -1,     3,   330,     3,   355,    -1,   364,   354,
       3,    -1,   331,   351,   332,    -1,    -1,    38,    -1,    38,
       3,    -1,     3,    -1,    -1,   352,   357,   127,   353,   361,
      -1,   352,   208,   353,    -1,   352,   208,   353,   156,   487,
      -1,   352,   359,   358,   127,   353,   362,    -1,   352,   152,
     360,   127,   353,    -1,    -1,   118,    -1,    62,    -1,    -1,
     163,    -1,   133,    -1,   184,    -1,   133,   358,    -1,   184,
     358,    -1,    -1,    -1,   362,    -1,   156,   487,    -1,   230,
     331,   347,   332,    -1,     3,    -1,   363,   329,     3,    -1,
     331,   337,   332,    -1,   365,    -1,    80,   366,   105,     3,
     338,   344,   345,    -1,   366,   144,    -1,   366,   171,    -1,
     366,   115,    -1,    -1,    80,   366,   367,   105,   351,   338,
      -1,     3,   368,    -1,   367,   329,     3,   368,    -1,    -1,
     330,    26,    -1,    80,   366,   105,   367,   230,   351,   338,
      -1,   369,    -1,   121,   371,   372,     3,   373,   234,   374,
     370,    -1,    -1,   156,   157,   128,   227,   376,    -1,    -1,
     371,   144,    -1,   371,    79,    -1,   371,   110,    -1,   371,
     115,    -1,   125,    -1,    -1,    -1,   331,   347,   332,    -1,
     331,   375,   332,    -1,   374,   329,   331,   375,   332,    -1,
     487,    -1,    78,    -1,   375,   329,   487,    -1,   375,   329,
      78,    -1,   121,   371,   372,     3,   192,   376,   370,    -1,
     121,   371,   372,     3,   373,   337,   370,    -1,     3,    20,
     487,    -1,     3,    20,    78,    -1,   376,   329,     3,    20,
     487,    -1,   376,   329,     3,    20,    78,    -1,   377,    -1,
     179,   371,   372,     3,   373,   234,   374,   370,    -1,   179,
     371,   372,     3,   192,   376,   370,    -1,   179,   371,   372,
       3,   373,   337,   370,    -1,   378,    -1,   227,   379,   351,
     192,   380,   338,   344,   345,    -1,    -1,   371,   144,    -1,
     371,   115,    -1,     3,    20,   487,    -1,     3,   330,     3,
      20,   487,    -1,   380,   329,     3,    20,   487,    -1,   380,
     329,     3,   330,     3,    20,   487,    -1,   381,    -1,    61,
      68,   382,     3,    -1,    61,   186,   382,     3,    -1,    -1,
     114,    18,    97,    -1,   383,    -1,    61,   386,   209,   382,
       3,   331,   387,   332,    -1,    61,   386,   209,   382,     3,
     330,     3,   331,   387,   332,    -1,    61,   386,   209,   382,
       3,   331,   387,   332,   384,    -1,    61,   386,   209,   382,
       3,   384,    -1,    61,   386,   209,   382,     3,   330,     3,
     331,   387,   332,   384,    -1,    61,   386,   209,   382,     3,
     330,     3,   384,    -1,   385,   354,   337,    -1,    -1,   115,
      -1,   179,    -1,    -1,   210,    -1,   388,    -1,   387,   329,
     388,    -1,     3,   495,   389,    -1,   167,   128,   331,   347,
     332,    -1,   128,   331,   347,   332,    -1,   116,   331,   347,
     332,    -1,   106,   116,   331,   347,   332,    -1,   106,   128,
     331,   347,   332,    -1,    -1,   389,    18,   154,    -1,   389,
     154,    -1,   389,    78,     4,    -1,   389,    78,     5,    -1,
     389,    78,     7,    -1,   389,    78,     6,    -1,   389,    40,
      -1,   389,   224,   128,    -1,   389,   167,   128,    -1,   389,
     128,    -1,   389,    56,     4,    -1,   389,   224,   331,   347,
     332,    -1,   390,    -1,    61,   169,   156,     3,   331,   347,
     332,   165,   156,     3,    -1,    61,   169,   156,     3,   331,
     347,   332,   155,    20,     5,   165,   156,     3,    -1,   246,
     497,    -1,   391,    -1,   252,     3,    -1,   252,   209,     3,
      -1,   392,    -1,    34,    68,     3,   494,    -1,    34,   186,
       3,   494,    -1,   393,    -1,    34,   397,   209,     3,   394,
      -1,   395,    -1,   394,   329,   395,    -1,    32,   396,   387,
      -1,    51,   396,     3,     3,   495,   389,    -1,    88,   396,
       3,    -1,    88,   167,   128,    -1,    88,   116,   347,    -1,
      88,   104,   128,   347,    -1,   177,     3,    -1,   177,   219,
       3,    -1,   161,    47,     3,    -1,    -1,    55,    -1,    -1,
     115,    -1,   398,    -1,    61,   399,   116,     3,   400,   156,
       3,   331,   402,   332,    -1,    -1,   224,    -1,   106,    -1,
     197,    -1,    -1,   230,   401,    -1,   314,    -1,   315,    -1,
     403,    -1,   402,   329,   403,    -1,     3,   491,   341,    -1,
     404,    -1,    88,   116,     3,   156,     3,    -1,   405,    -1,
      88,    68,   406,     3,    -1,    88,   186,   406,     3,    -1,
      -1,   114,    97,    -1,   407,    -1,    88,   386,   209,   406,
     408,   409,    -1,     3,    -1,     3,   330,     3,    -1,   408,
     329,     3,    -1,   408,   329,     3,   330,     3,    -1,    -1,
     181,    -1,    49,    -1,   410,    -1,   177,   209,   411,    -1,
      -1,     3,   219,     3,    -1,     3,   330,     3,   219,     3,
     330,     3,    -1,   411,   329,     3,   219,     3,    -1,   411,
     329,     3,   330,     3,   219,     3,   330,     3,    -1,   412,
      -1,    82,     3,    -1,    81,     3,    -1,   413,    -1,   229,
       3,    -1,   414,    -1,   416,    -1,   419,    -1,   264,   265,
      -1,   262,   415,    -1,    -1,   313,    -1,   263,   415,   417,
     418,    -1,    -1,    36,   266,   267,    -1,    36,   267,    -1,
      -1,   266,   176,    -1,   176,    -1,   270,   415,   417,   418,
      -1,   420,    -1,   192,   268,    20,   487,    -1,   421,    -1,
     422,    -1,   423,    -1,   269,     3,    -1,   270,   415,   219,
     269,     3,    -1,   176,   269,     3,    -1,   424,    -1,   139,
     272,   425,    -1,   426,    -1,   425,   329,   426,    -1,     3,
     355,   427,    -1,   172,    -1,   172,   271,    -1,   144,   242,
      -1,   242,    -1,   428,    -1,   225,   272,    -1,   429,    -1,
     192,   430,   265,   273,   274,   431,    -1,    -1,   275,    -1,
     276,    -1,   172,   277,    -1,   172,   278,    -1,   279,   172,
      -1,   280,    -1,   432,    -1,    61,   251,   433,    -1,   434,
      -1,   433,   329,   434,    -1,     8,    -1,     8,   281,    47,
       4,    -1,     8,   281,    47,   282,     4,    -1,   435,    -1,
      88,   251,   436,    -1,     8,    -1,   436,   329,     8,    -1,
     437,    -1,   107,   438,   156,    26,   330,    26,   219,   433,
     440,    -1,   439,    -1,   438,   329,   439,    -1,    33,    -1,
      33,   283,    -1,   189,    -1,   227,    -1,   121,    -1,    80,
      -1,    61,    -1,    88,    -1,   116,    -1,    34,    -1,   107,
     159,    -1,   175,    -1,    -1,   241,   441,    -1,   107,   159,
      -1,   247,   487,    -1,   248,   487,    -1,   249,   487,    -1,
     250,   487,    -1,   442,    -1,   107,   438,   156,     3,   330,
      26,   219,   433,   440,    -1,   443,    -1,   107,   444,   156,
       3,   330,     3,   219,   433,   440,    -1,   445,    -1,   444,
     329,   445,    -1,   189,   373,    -1,   227,   373,    -1,   121,
     373,    -1,    80,    -1,    61,    -1,    88,    -1,   116,    -1,
      34,    -1,   107,   159,    -1,   446,    -1,   183,   438,   156,
      26,   330,    26,   105,   436,    -1,   447,    -1,   183,   438,
     156,     3,   330,    26,   105,   436,   440,    -1,   448,    -1,
     183,   444,   156,     3,   330,     3,   105,   436,   440,    -1,
     449,    -1,   177,   251,   450,    -1,     8,   219,     8,    -1,
     450,   329,     8,   219,     8,    -1,   451,    -1,   192,   282,
     452,    20,   282,   331,     4,   332,    -1,    -1,   102,     8,
      -1,   453,    -1,    35,   454,   209,   408,    -1,    -1,   271,
      -1,   153,    -1,   455,    -1,   284,   209,   408,   219,     4,
      -1,   456,    -1,    53,   209,   408,   457,    -1,    -1,   171,
      -1,   253,    -1,   254,    -1,   255,    -1,   256,    -1,   458,
      -1,   285,   209,   408,   459,    -1,    -1,   171,    -1,   255,
      -1,   460,    -1,   158,   454,   209,   408,    -1,   461,    -1,
     286,   454,   209,   408,   462,    -1,    -1,   462,   171,    -1,
     462,   255,    -1,   462,   287,    -1,   463,    -1,   288,   209,
     408,   105,     4,    -1,   464,    -1,   193,   289,   192,   467,
      -1,   193,   290,   467,    -1,   193,   465,   291,   105,     3,
     466,   467,    -1,   193,    61,    68,     3,    -1,   193,    61,
     186,     3,    -1,   193,    61,   209,     3,    -1,   193,    68,
       3,    -1,   193,   186,     3,    -1,   193,   292,     3,   293,
      -1,   193,   292,     3,   294,    -1,   193,   295,   296,    -1,
     193,   297,   345,    -1,   193,   487,   297,    -1,   193,   298,
     102,     8,    -1,   193,   116,   105,     3,   466,    -1,   193,
     299,   294,    -1,   193,   469,   293,    -1,   193,   317,   272,
     466,   467,    -1,   193,   283,    -1,   193,   465,   300,    -1,
     193,   430,   294,   467,    -1,   193,   209,   294,   466,   467,
      -1,   193,   465,   272,   466,   467,    -1,   193,   301,   466,
     468,    -1,   193,   430,   302,   467,    -1,   193,   303,   345,
      -1,   193,   487,   303,    -1,    -1,   318,    -1,    -1,   105,
       3,    -1,    -1,    14,     4,    -1,    -1,    14,   487,    -1,
      -1,   316,    -1,   470,    -1,   260,   116,   471,    16,     3,
      -1,   472,    -1,   471,   329,   472,    -1,     3,    -1,     3,
     473,   363,    -1,    -1,   116,    -1,   128,    -1,   474,    -1,
     304,   454,   475,    -1,   476,    -1,   475,   329,   476,    -1,
     305,    -1,   306,    -1,   293,    -1,   283,    -1,   259,   260,
      -1,   294,    -1,   209,     3,    -1,   272,   408,    -1,   272,
     241,   172,   139,    -1,   307,    -1,   477,    -1,   130,   478,
     487,    -1,    -1,   308,    -1,   259,    -1,   479,    -1,   136,
     116,   125,   260,   471,    -1,     3,   480,   481,    -1,   473,
     331,   363,   332,    -1,    -1,   115,   257,    -1,   482,    -1,
     309,   483,    -1,   484,    -1,   483,   329,   484,    -1,   258,
      -1,   259,   260,    -1,   261,    -1,   485,    -1,   310,     3,
     105,   336,    -1,   312,     3,   486,    -1,   311,   310,     3,
      -1,    88,   310,     3,    -1,    -1,   230,   436,    -1,   333,
      -1,   488,    -1,   192,   489,    -1,   490,    -1,   489,   329,
     490,    -1,     8,    20,   487,    -1,     8,     9,   487,    -1,
      -1,   331,     5,   332,    -1,   331,     5,   329,     5,   332,
      -1,    -1,    43,    -1,    -1,   493,   226,    -1,   493,   245,
      -1,    -1,   494,    52,   192,     3,    -1,   494,    54,     3,
      -1,    44,   491,    -1,   217,   491,   493,    -1,   194,   491,
     493,    -1,   147,   491,   493,    -1,   122,   491,   493,    -1,
     123,   491,   493,    -1,    42,   491,   493,    -1,   174,   491,
     493,    -1,    87,   491,   493,    -1,   101,   491,   493,    -1,
      76,   491,   493,    -1,    70,    -1,   214,    -1,   215,    -1,
      71,    -1,   243,    -1,    52,   491,   494,    -1,   236,   331,
       5,   332,   494,    -1,    43,   491,    -1,   235,   331,     5,
     332,    -1,   216,    -1,    45,    -1,   146,    -1,   141,    -1,
     218,   492,   494,    -1,   211,   492,   494,    -1,   148,   492,
     494,    -1,   142,   492,   494,    -1,    95,   331,   496,   332,
     494,    -1,   192,   331,   496,   332,   494,    -1,     4,    -1,
     496,   329,     4,    -1,     3,    -1,     8,    -1,     3,   330,
       3,    -1,     3,   330,    26,    -1,     4,    -1,     5,    -1,
       7,    -1,     6,    -1,   487,    24,   487,    -1,   487,    25,
     487,    -1,   487,    26,   487,    -1,   487,    27,   487,    -1,
     487,    28,   487,    -1,   487,    29,   487,    -1,    25,   487,
      -1,    24,   487,    -1,   487,    12,   487,    -1,   487,    10,
     487,    -1,   487,    11,   487,    -1,   487,    20,   487,    -1,
     487,    20,   331,   337,   332,    -1,   487,    20,    37,   331,
     337,   332,    -1,   487,    20,   195,   331,   337,   332,    -1,
     487,    20,    33,   331,   337,   332,    -1,   487,    21,   487,
      -1,   487,    22,   487,    -1,   487,    30,   487,    -1,   487,
      23,   487,    -1,    18,   487,    -1,    17,   487,    -1,     8,
       9,   487,    -1,   331,   487,   332,    -1,   487,    15,   154,
      -1,   487,    15,    18,   154,    -1,   487,    15,     6,    -1,
     487,    15,    18,     6,    -1,   487,    19,   487,    36,   487,
      -1,   487,    -1,   487,   329,   497,    -1,    -1,   497,    -1,
     487,    16,   331,   497,   332,    -1,   487,    18,    16,   331,
     497,   332,    -1,   487,    16,   331,   337,   332,    -1,   487,
      18,    16,   331,   337,   332,    -1,    97,   331,   337,   332,
      -1,    18,    97,   331,   337,   332,    -1,     3,   331,   498,
     332,    -1,   323,   331,    26,   332,    -1,   323,   331,   487,
     332,    -1,   324,   331,   487,   332,    -1,   325,   331,   487,
     332,    -1,   326,   331,   487,   332,    -1,   327,   331,   487,
     332,    -1,   319,   331,   497,   332,    -1,   319,   331,   487,
     105,   487,   332,    -1,   319,   331,   487,   105,   487,   102,
     487,   332,    -1,   320,   331,   497,   332,    -1,   320,   331,
     499,   487,   105,   497,   332,    -1,   131,    -1,   220,    -1,
      46,    -1,   321,   331,   487,   329,   500,   332,    -1,   322,
     331,   487,   329,   500,   332,    -1,   124,   487,    72,    -1,
     124,   487,    73,    -1,   124,   487,    74,    -1,   124,   487,
      75,    -1,   124,   487,   244,    -1,   124,   487,   243,    -1,
     124,   487,   111,    -1,   124,   487,   112,    -1,   124,   487,
     113,    -1,    50,   487,   501,    94,    -1,    50,   487,   501,
      91,   487,    94,    -1,    50,   501,    94,    -1,    50,   501,
      91,   487,    94,    -1,   238,   487,   213,   487,    -1,   501,
     238,   487,   213,   487,    -1,   487,    14,   487,    -1,   487,
      18,    14,   487,    -1,   487,    13,   487,    -1,   487,    18,
      13,   487,    -1,    65,    -1,    63,    -1,    64,    -1,    43,
     487,    -1
};

/* YYRLINE[YYN] -- source line where rule number YYN was defined.  */
static const unsigned short int yyrline[] =
{
       0,   432,   432,   445,   458,   466,   478,   482,   487,   495,
     496,   498,   499,   502,   507,   514,   515,   516,   519,   520,
     523,   524,   531,   532,   539,   540,   541,   544,   545,   548,
     549,   552,   553,   554,   555,   556,   557,   558,   559,   560,
     563,   564,   565,   566,   569,   572,   573,   576,   577,   581,
     586,   591,   596,   599,   600,   603,   604,   605,   620,   624,
     628,   633,   637,   643,   644,   645,   648,   649,   652,   653,
     656,   657,   658,   661,   662,   669,   673,   693,   694,   697,
     702,   705,   710,   711,   712,   713,   716,   721,   722,   725,
     726,   729,   735,   738,   744,   745,   748,   749,   750,   751,
     752,   755,   755,   758,   759,   762,   763,   766,   767,   768,
     769,   772,   778,   784,   787,   790,   793,   799,   802,   808,
     814,   820,   823,   830,   831,   832,   836,   846,   857,   859,
     868,   872,   874,   878,   879,   884,   887,   891,   895,   900,
     904,   909,   915,   919,   920,   921,   924,   925,   928,   929,
     933,   934,   935,   936,   937,   938,   941,   942,   943,   944,
     945,   946,   947,   948,   949,   950,   951,   952,   953,   957,
     961,   963,   974,   978,   981,   982,   986,   990,   991,   995,
    1000,  1004,  1005,  1009,  1010,  1011,  1012,  1013,  1014,  1015,
    1016,  1017,  1020,  1021,  1025,  1026,  1030,  1034,  1037,  1038,
    1039,  1040,  1043,  1044,  1047,  1048,  1051,  1052,  1055,  1059,
    1062,  1066,  1070,  1071,  1074,  1075,  1079,  1082,  1085,  1086,
    1087,  1088,  1091,  1092,  1093,  1098,  1101,  1104,  1105,  1106,
    1107,  1108,  1112,  1115,  1116,  1120,  1123,  1127,  1128,  1129,
    1132,  1133,  1136,  1137,  1140,  1143,  1144,  1145,  1148,  1149,
    1150,  1153,  1157,  1160,  1164,  1165,  1166,  1170,  1173,  1176,
    1180,  1183,  1186,  1187,  1190,  1193,  1194,  1195,  1196,  1200,
    1203,  1207,  1211,  1214,  1215,  1216,  1220,  1221,  1222,  1223,
    1227,  1230,  1233,  1234,  1237,  1238,  1239,  1243,  1246,  1249,
    1250,  1254,  1257,  1260,  1261,  1265,  1266,  1267,  1268,  1269,
    1270,  1271,  1272,  1273,  1274,  1275,  1276,  1279,  1280,  1284,
    1285,  1286,  1287,  1288,  1292,  1295,  1299,  1302,  1305,  1306,
    1310,  1311,  1312,  1313,  1314,  1315,  1316,  1317,  1318,  1328,
    1331,  1334,  1337,  1340,  1343,  1347,  1350,  1354,  1355,  1359,
    1362,  1365,  1366,  1370,  1373,  1376,  1377,  1378,  1382,  1385,
    1389,  1392,  1395,  1396,  1397,  1398,  1399,  1400,  1404,  1407,
    1410,  1411,  1412,  1416,  1419,  1423,  1426,  1429,  1430,  1431,
    1432,  1436,  1439,  1443,  1446,  1447,  1448,  1449,  1450,  1451,
    1452,  1453,  1454,  1455,  1456,  1457,  1458,  1459,  1460,  1461,
    1462,  1463,  1464,  1465,  1466,  1467,  1468,  1469,  1470,  1471,
    1472,  1475,  1476,  1479,  1480,  1483,  1484,  1487,  1488,  1491,
    1492,  1496,  1499,  1502,  1503,  1506,  1507,  1510,  1511,  1512,
    1516,  1519,  1522,  1523,  1526,  1527,  1528,  1529,  1530,  1531,
    1532,  1533,  1534,  1535,  1539,  1542,  1545,  1546,  1547,  1551,
    1554,  1557,  1560,  1563,  1564,  1568,  1571,  1574,  1575,  1578,
    1579,  1580,  1584,  1587,  1588,  1589,  1590,  1593,  1594,  1597,
    1605,  1608,  1610,  1610,  1613,  1617,  1625,  1626,  1627,  1630,
    1631,  1634,  1635,  1636,  1639,  1640,  1642,  1647,  1648,  1649,
    1650,  1651,  1652,  1653,  1654,  1655,  1656,  1657,  1658,  1659,
    1660,  1661,  1662,  1663,  1664,  1665,  1666,  1667,  1668,  1669,
    1670,  1671,  1672,  1673,  1674,  1675,  1676,  1679,  1680,  1686,
    1687,  1688,  1689,  1690,  1691,  1692,  1693,  1696,  1697,  1698,
    1699,  1700,  1701,  1702,  1703,  1704,  1705,  1706,  1707,  1708,
    1709,  1710,  1711,  1712,  1713,  1714,  1715,  1716,  1717,  1718,
    1719,  1722,  1723,  1724,  1725,  1728,  1732,  1733,  1736,  1737,
    1740,  1741,  1742,  1743,  1744,  1745,  1748,  1752,  1753,  1755,
    1756,  1757,  1758,  1762,  1763,  1764,  1765,  1766,  1769,  1770,
    1771,  1774,  1775,  1778,  1779,  1780,  1781,  1782,  1783,  1784,
    1785,  1786,  1789,  1790,  1791,  1792,  1795,  1796,  1799,  1800,
    1803,  1804,  1807,  1808,  1809,  1812
};
#endif

#if YYDEBUG || YYERROR_VERBOSE || YYTOKEN_TABLE
/* YYTNAME[SYMBOL-NUM] -- String name of the symbol SYMBOL-NUM.
   First, the terminals, then, starting at YYNTOKENS, nonterminals.  */
static const char *const yytname[] =
{
  "$end", "error", "$undefined", "NAME", "STRING", "INTNUM", "BOOL",
  "APPROXNUM", "USERVAR", "ASSIGN", "OR", "XOR", "ANDOP", "REGEXP", "LIKE",
  "IS", "IN", "'!'", "NOT", "BETWEEN", "COMPARISON", "'|'", "'&'", "SHIFT",
  "'+'", "'-'", "'*'", "'/'", "'%'", "MOD", "'^'", "UMINUS", "ADD", "ALL",
  "ALTER", "ANALYZE", "AND", "ANY", "AS", "ASC", "AUTO_INCREMENT",
  "BEFORE", "BIGINT", "BINARY", "BIT", "BLOB", "BOTH", "BY", "CALL",
  "CASCADE", "CASE", "CHANGE", "CHAR", "CHECK", "COLLATE", "COLUMN",
  "COMMENT", "CONDITION", "CONSTRAINT", "CONTINUE", "CONVERT", "CREATE",
  "CROSS", "CURRENT_DATE", "CURRENT_TIME", "CURRENT_TIMESTAMP",
  "CURRENT_USER", "CURSOR", "DATABASE", "DATABASES", "DATE", "DATETIME",
  "DAY_HOUR", "DAY_MICROSECOND", "DAY_MINUTE", "DAY_SECOND", "DECIMAL",
  "DECLARE", "DEFAULT", "DELAYED", "DELETE", "DESC", "DESCRIBE",
  "DETERMINISTIC", "DISTINCT", "DISTINCTROW", "DIV", "DOUBLE", "DROP",
  "DUAL", "EACH", "ELSE", "ELSEIF", "ENCLOSED", "END", "ENUM", "ESCAPED",
  "EXISTS", "EXIT", "EXPLAIN", "FETCH", "FLOAT", "FOR", "FORCE", "FOREIGN",
  "FROM", "FULLTEXT", "GRANT", "GROUP", "HAVING", "HIGH_PRIORITY",
  "HOUR_MICROSECOND", "HOUR_MINUTE", "HOUR_SECOND", "IF", "IGNORE",
  "INDEX", "INFILE", "INNER", "INOUT", "INSENSITIVE", "INSERT", "INT",
  "INTEGER", "INTERVAL", "INTO", "ITERATE", "JOIN", "KEY", "KEYS", "KILL",
  "LEADING", "LEAVE", "LEFT", "LIMIT", "LINES", "LOAD", "LOCALTIME",
  "LOCALTIMESTAMP", "LOCK", "LONG", "LONGBLOB", "LONGTEXT", "LOOP",
  "LOW_PRIORITY", "MATCH", "MEDIUMBLOB", "MEDIUMINT", "MEDIUMTEXT",
  "MINUTE_MICROSECOND", "MINUTE_SECOND", "MODIFIES", "NATURAL",
  "NO_WRITE_TO_BINLOG", "NULLX", "NUMBER", "ON", "DUPLICATE", "OPTIMIZE",
  "OPTION", "OPTIONALLY", "ORDER", "OUT", "OUTER", "OUTFILE",
  "PARTITIONED", "PRECISION", "PRIMARY", "PROCEDURE", "PROJECTION",
  "PURGE", "QUICK", "READ", "READS", "REAL", "REFERENCES", "RELEASE",
  "RENAME", "REPEAT", "REPLACE", "REQUIRE", "RESTRICT", "RETURN", "REVOKE",
  "RIGHT", "ROLLUP", "SCHEMA", "SCHEMAS", "SECOND_MICROSECOND", "SELECT",
  "SENSITIVE", "SEPARATOR", "SET", "SHOW", "SMALLINT", "SOME", "SONAME",
  "SPATIAL", "SPECIFIC", "SQL", "SQLEXCEPTION", "SQLSTATE", "SQLWARNING",
  "SQL_BIG_RESULT", "SQL_CALC_FOUND_ROWS", "SQL_SMALL_RESULT", "SSL",
  "STARTING", "STRAIGHT_JOIN", "TABLE", "TEMPORARY", "TEXT", "TERMINATED",
  "THEN", "TIME", "TIMESTAMP", "TINYBLOB", "TINYINT", "TINYTEXT", "TO",
  "TRAILING", "TRIGGER", "UNDO", "UNION", "UNIQUE", "UNLOCK", "UNSIGNED",
  "UPDATE", "USAGE", "USE", "USING", "UTC_DATE", "UTC_TIME",
  "UTC_TIMESTAMP", "VALUES", "VARBINARY", "VARCHAR", "VARYING", "WHEN",
  "WHERE", "WHILE", "WITH", "WRITE", "YEAR", "YEAR_MONTH", "ZEROFILL",
  "DO", "MAX_QUERIES_PER_HOUR", "MAX_UPDATES_PER_HOUR",
  "MAX_CONNECTIONS_PER_HOUR", "MAX_USER_CONNECTIONS", "USER", "TRUNCATE",
  "FAST", "MEDIUM", "EXTENDED", "CHANGED", "LEAVES", "MASTER", "QUERY",
  "CACHE", "SLAVE", "BEGINT", "COMMIT", "START", "TRANSACTION", "NO",
  "CHAIN", "AUTOCOMMIT", "SAVEPOINT", "ROLLBACK", "LOCAL", "TABLES",
  "ISOLATION", "LEVEL", "GLOBAL", "SESSION", "UNCOMMITTED", "COMMITTED",
  "REPEATABLE", "SERIALIZABLE", "IDENTIFIED", "PASSWORD", "PRIVILEGES",
  "BACKUP", "CHECKSUM", "REPAIR", "USE_FRM", "RESTORE", "CHARACTER",
  "COLLATION", "COLUMNS", "ENGINE", "LOGS", "STATUS", "STORAGE", "ENGINES",
  "ERRORS", "GRANTS", "INNODB", "PROCESSLIST", "TRIGGERS", "VARIABLES",
  "WARNINGS", "FLUSH", "HOSTS", "DES_KEY_FILE", "USER_RESOURCES",
  "CONNECTION", "RESET", "PREPARE", "DEALLOCATE", "EXECUTE", "WORK",
  "BTREE", "HASH", "BDB", "OPEN", "FULL", "FSUBSTRING", "FTRIM",
  "FDATE_ADD", "FDATE_SUB", "FCOUNT", "FSUM", "FAVG", "FMIN", "FMAX",
  "';'", "','", "'.'", "'('", "')'", "'?'", "$accept", "stmt_list", "stmt",
  "select_stmt", "opt_where", "opt_groupby", "groupby_list",
  "opt_asc_desc", "opt_with_rollup", "opt_having", "opt_orderby",
  "opt_limit", "opt_into_list", "column_list", "select_opts",
  "select_expr_list", "select_expr", "table_references", "table_reference",
  "table_factor", "opt_as", "opt_as_alias", "join_table",
  "opt_inner_cross", "opt_outer", "left_or_right",
  "opt_left_or_right_outer", "opt_join_condition", "join_condition",
  "index_list", "table_subquery", "delete_stmt", "delete_opts",
  "delete_list", "opt_dot_star", "insert_stmt", "opt_ondupupdate",
  "insert_opts", "opt_into", "opt_col_names", "insert_vals_list",
  "insert_vals", "insert_asgn_list", "replace_stmt", "update_stmt",
  "update_opts", "update_asgn_list", "create_database_stmt",
  "opt_if_not_exists", "create_table_stmt", "create_select_statement",
  "opt_ignore_replace", "opt_temporary", "create_col_list",
  "create_definition", "column_atts", "create_projection_stmt",
  "truncate_stmt", "alter_database_stmt", "alter_table_stmt", "alter_list",
  "alter_def", "opt_col", "opt_ignore", "create_index_stmt", "index_att",
  "opt_using_type", "index_type", "index_col_list", "index_col_name",
  "drop_index_stmt", "drop_database_stmt", "opt_if_exists",
  "drop_table_stmt", "table_list", "opt_rc", "rename_table_stmt",
  "rename_list", "describe_stmt", "use_stmt", "start_transaction_stmt",
  "opt_work", "commit_stmt", "opt_chain", "opt_release", "rollback_stmt",
  "set_autocommit_stmt", "savepoint_stmt", "rollback_to_savepoint_stmt",
  "release_savepoint", "lock_table_stmt", "lock_table_list", "lock_table",
  "opt_read_write", "unlock_tables", "set_transaction_stmt",
  "opt_trans_level", "trans_att", "create_user_stmt", "create_user_list",
  "create_user", "drop_user_stmt", "user_list", "grant_all_stmt",
  "priv_database_list", "priv_database_type", "opt_with_opt", "with_opt",
  "grant_database_stmt", "grant_table_stmt", "priv_table_list",
  "priv_table_type", "revoke_all_stmt", "revoke_database_stmt",
  "revoke_table_stmt", "rename_user_stmt", "utou_list",
  "set_password_stmt", "opt_for_user", "analyze_table_stmt", "analyze_opt",
  "backup_table_stmt", "check_table_stmt", "check_opts",
  "checksum_table_stmt", "checksum_opt", "optimize_table_stmt",
  "repair_table_stmt", "repair_opts", "restore_table_stmt", "show_stmt",
  "opt_full", "opt_from", "opt_like_string", "opt_like_expr", "opt_bdb",
  "cache_index_stmt", "table_index_list", "table_index", "opt_index_key",
  "flush_stmt", "flush_opt_list", "flush_opt", "kill_stmt",
  "opt_connection_query", "load_index_stmt", "opt_index_key_list",
  "opt_ignore_leaves", "reset_stmt", "reset_opt_list", "reset_opt",
  "prepare_stmt", "opt_use_name", "expr", "set_stmt", "set_list",
  "set_expr", "opt_length", "opt_binary", "opt_uz", "opt_csc", "data_type",
  "enum_list", "expr_list", "opt_expr_list", "trim_ltb", "interval_exp",
  "case_list", 0
};
#endif

/* YYR1[YYN] -- Symbol number of symbol that rule YYN derives.  */
static const unsigned short int yyr1[] =
{
       0,   334,   335,   335,   335,   335,   336,   337,   337,   338,
     338,   339,   339,   340,   340,   341,   341,   341,   342,   342,
     343,   343,   344,   344,   345,   345,   345,   346,   346,   347,
     347,   348,   348,   348,   348,   348,   348,   348,   348,   348,
     349,   349,   349,   349,   350,   351,   351,   352,   352,   353,
     353,   353,   353,   354,   354,   355,   355,   355,   356,   356,
     356,   356,   356,   357,   357,   357,   358,   358,   359,   359,
     360,   360,   360,   361,   361,   362,   362,   363,   363,   364,
     336,   365,   366,   366,   366,   366,   365,   367,   367,   368,
     368,   365,   336,   369,   370,   370,   371,   371,   371,   371,
     371,   372,   372,   373,   373,   374,   374,   375,   375,   375,
     375,   369,   369,   376,   376,   376,   376,   336,   377,   377,
     377,   336,   378,   379,   379,   379,   380,   380,   380,   380,
     336,   381,   381,   382,   382,   336,   383,   383,   383,   383,
     383,   383,   384,   385,   385,   385,   386,   386,   387,   387,
     388,   388,   388,   388,   388,   388,   389,   389,   389,   389,
     389,   389,   389,   389,   389,   389,   389,   389,   389,   336,
     390,   390,   336,   336,   391,   391,   336,   392,   392,   336,
     393,   394,   394,   395,   395,   395,   395,   395,   395,   395,
     395,   395,   396,   396,   397,   397,   336,   398,   399,   399,
     399,   399,   400,   400,   401,   401,   402,   402,   403,   336,
     404,   336,   405,   405,   406,   406,   336,   407,   408,   408,
     408,   408,   409,   409,   409,   336,   410,   411,   411,   411,
     411,   411,   336,   412,   412,   336,   413,   336,   336,   336,
     414,   414,   415,   415,   416,   417,   417,   417,   418,   418,
     418,   419,   336,   420,   336,   336,   336,   421,   422,   423,
     336,   424,   425,   425,   426,   427,   427,   427,   427,   336,
     428,   336,   429,   430,   430,   430,   431,   431,   431,   431,
     336,   432,   433,   433,   434,   434,   434,   336,   435,   436,
     436,   336,   437,   438,   438,   439,   439,   439,   439,   439,
     439,   439,   439,   439,   439,   439,   439,   440,   440,   441,
     441,   441,   441,   441,   336,   442,   336,   443,   444,   444,
     445,   445,   445,   445,   445,   445,   445,   445,   445,   336,
     446,   336,   447,   336,   448,   336,   449,   450,   450,   336,
     451,   452,   452,   336,   453,   454,   454,   454,   336,   455,
     336,   456,   457,   457,   457,   457,   457,   457,   336,   458,
     459,   459,   459,   336,   460,   336,   461,   462,   462,   462,
     462,   336,   463,   336,   464,   464,   464,   464,   464,   464,
     464,   464,   464,   464,   464,   464,   464,   464,   464,   464,
     464,   464,   464,   464,   464,   464,   464,   464,   464,   464,
     464,   465,   465,   466,   466,   467,   467,   468,   468,   469,
     469,   336,   470,   471,   471,   472,   472,   473,   473,   473,
     336,   474,   475,   475,   476,   476,   476,   476,   476,   476,
     476,   476,   476,   476,   336,   477,   478,   478,   478,   336,
     479,   471,   480,   481,   481,   336,   482,   483,   483,   484,
     484,   484,   336,   485,   485,   485,   485,   486,   486,   487,
     336,   488,   489,   489,   490,   490,   491,   491,   491,   492,
     492,   493,   493,   493,   494,   494,   494,   495,   495,   495,
     495,   495,   495,   495,   495,   495,   495,   495,   495,   495,
     495,   495,   495,   495,   495,   495,   495,   495,   495,   495,
     495,   495,   495,   495,   495,   495,   495,   496,   496,   487,
     487,   487,   487,   487,   487,   487,   487,   487,   487,   487,
     487,   487,   487,   487,   487,   487,   487,   487,   487,   487,
     487,   487,   487,   487,   487,   487,   487,   487,   487,   487,
     487,   487,   487,   487,   487,   487,   497,   497,   498,   498,
     487,   487,   487,   487,   487,   487,   487,   487,   487,   487,
     487,   487,   487,   487,   487,   487,   487,   487,   499,   499,
     499,   487,   487,   500,   500,   500,   500,   500,   500,   500,
     500,   500,   487,   487,   487,   487,   501,   501,   487,   487,
     487,   487,   487,   487,   487,   487
};

/* YYR2[YYN] -- Number of symbols composing right hand side of rule YYN.  */
static const unsigned char yyr2[] =
{
       0,     2,     2,     3,     2,     3,     1,     3,    11,     0,
       2,     0,     4,     2,     4,     0,     1,     1,     0,     2,
       0,     2,     0,     3,     0,     2,     4,     0,     2,     1,
       3,     0,     2,     2,     2,     2,     2,     2,     2,     2,
       1,     3,     1,     3,     2,     1,     3,     1,     1,     2,
       4,     3,     3,     0,     1,     2,     1,     0,     5,     3,
       5,     6,     5,     0,     1,     1,     0,     1,     1,     1,
       2,     2,     0,     0,     1,     2,     4,     1,     3,     3,
       1,     7,     2,     2,     2,     0,     6,     2,     4,     0,
       2,     7,     1,     8,     0,     5,     0,     2,     2,     2,
       2,     1,     0,     0,     3,     3,     5,     1,     1,     3,
       3,     7,     7,     3,     3,     5,     5,     1,     8,     7,
       7,     1,     8,     0,     2,     2,     3,     5,     5,     7,
       1,     4,     4,     0,     3,     1,     8,    10,     9,     6,
      11,     8,     3,     0,     1,     1,     0,     1,     1,     3,
       3,     5,     4,     4,     5,     5,     0,     3,     2,     3,
       3,     3,     3,     2,     3,     3,     2,     3,     5,     1,
      10,    13,     2,     1,     2,     3,     1,     4,     4,     1,
       5,     1,     3,     3,     6,     3,     3,     3,     4,     2,
       3,     3,     0,     1,     0,     1,     1,    10,     0,     1,
       1,     1,     0,     2,     1,     1,     1,     3,     3,     1,
       5,     1,     4,     4,     0,     2,     1,     6,     1,     3,
       3,     5,     0,     1,     1,     1,     3,     0,     3,     7,
       5,     9,     1,     2,     2,     1,     2,     1,     1,     1,
       2,     2,     0,     1,     4,     0,     3,     2,     0,     2,
       1,     4,     1,     4,     1,     1,     1,     2,     5,     3,
       1,     3,     1,     3,     3,     1,     2,     2,     1,     1,
       2,     1,     6,     0,     1,     1,     2,     2,     2,     1,
       1,     3,     1,     3,     1,     4,     5,     1,     3,     1,
       3,     1,     9,     1,     3,     1,     2,     1,     1,     1,
       1,     1,     1,     1,     1,     2,     1,     0,     2,     2,
       2,     2,     2,     2,     1,     9,     1,     9,     1,     3,
       2,     2,     2,     1,     1,     1,     1,     1,     2,     1,
       8,     1,     9,     1,     9,     1,     3,     3,     5,     1,
       8,     0,     2,     1,     4,     0,     1,     1,     1,     5,
       1,     4,     0,     1,     1,     1,     1,     1,     1,     4,
       0,     1,     1,     1,     4,     1,     5,     0,     2,     2,
       2,     1,     5,     1,     4,     3,     7,     4,     4,     4,
       3,     3,     4,     4,     3,     3,     3,     4,     5,     3,
       3,     5,     2,     3,     4,     5,     5,     4,     4,     3,
       3,     0,     1,     0,     2,     0,     2,     0,     2,     0,
       1,     1,     5,     1,     3,     1,     3,     0,     1,     1,
       1,     3,     1,     3,     1,     1,     1,     1,     2,     1,
       2,     2,     4,     1,     1,     3,     0,     1,     1,     1,
       5,     3,     4,     0,     2,     1,     2,     1,     3,     1,
       2,     1,     1,     4,     3,     3,     3,     0,     2,     1,
       1,     2,     1,     3,     3,     3,     0,     3,     5,     0,
       1,     0,     2,     2,     0,     4,     3,     2,     3,     3,
       3,     3,     3,     3,     3,     3,     3,     3,     1,     1,
       1,     1,     1,     3,     5,     2,     4,     1,     1,     1,
       1,     3,     3,     3,     3,     5,     5,     1,     3,     1,
       1,     3,     3,     1,     1,     1,     1,     3,     3,     3,
       3,     3,     3,     2,     2,     3,     3,     3,     3,     5,
       6,     6,     6,     3,     3,     3,     3,     2,     2,     3,
       3,     3,     4,     3,     4,     5,     1,     3,     0,     1,
       5,     6,     5,     6,     4,     5,     4,     4,     4,     4,
       4,     4,     4,     4,     6,     8,     4,     7,     1,     1,
       1,     6,     6,     3,     3,     3,     3,     3,     3,     3,
       3,     3,     4,     6,     3,     5,     4,     5,     3,     4,
       3,     4,     1,     1,     1,     2
};

/* YYDPREC[RULE-NUM] -- Dynamic precedence of rule #RULE-NUM (0 if none).  */
static const unsigned char yydprec[] =
{
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     1,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     2,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0
};

/* YYMERGER[RULE-NUM] -- Index of merging function for rule #RULE-NUM.  */
static const unsigned char yymerger[] =
{
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0
};

/* YYDEFACT[S] -- default rule to reduce with in state S when YYTABLE
   doesn't specify something else to do.  Zero means the default is an
   error.  */
static const unsigned short int yydefact[] =
{
       0,     0,   194,   345,     0,   146,    85,     0,     0,   146,
       0,    96,   436,     0,     0,   345,     0,     0,    96,     0,
      31,   273,   401,     0,    96,     0,     0,     0,     0,   242,
     242,     0,     0,   242,     0,     0,   345,     0,   345,     0,
       0,     0,     0,     0,     0,     6,    80,    92,   117,   121,
     130,   135,   169,   173,   176,   179,   196,   209,   211,   216,
     225,   232,   235,   237,   238,   239,   252,   254,   255,   256,
     260,   269,   271,   280,   287,   291,   314,   316,   329,   331,
     333,   335,   339,   343,   348,   350,   358,   363,   365,   371,
     373,   411,   420,   434,   439,   445,   452,   460,     4,     0,
     195,     0,     0,   347,   346,     0,     0,   133,   200,     0,
     133,   201,   147,   199,     0,     0,     0,     0,   234,   233,
     214,     0,   214,     0,     0,     0,   295,   304,   301,   300,
     302,     0,   303,   103,   306,   103,   103,     0,   293,     0,
     318,   102,   438,   437,     0,     0,     0,     0,     0,   227,
       0,   102,     0,     0,     0,     0,     0,   274,   275,   341,
       0,   461,   462,   509,   513,   514,   516,   515,   510,     0,
       0,     0,     0,     0,     0,     0,   593,   594,   592,     0,
       0,     0,     0,     0,   392,     0,   405,     0,     0,    24,
       0,     0,   403,    24,   410,     0,   402,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   459,     0,     0,
       0,     0,   270,     0,     0,   236,   546,   172,   174,     0,
       0,   243,   241,   245,   240,   257,   245,     0,     0,     0,
       0,     0,   449,     0,   451,   446,   447,     0,     0,   457,
       1,     0,     0,     2,   474,   474,     0,     0,   218,   352,
       0,     0,     0,     0,   284,   281,   282,   133,     0,    89,
       0,    84,    82,    83,     0,     0,     0,     0,     0,   289,
     288,   456,   214,   296,   305,     0,   322,   320,   321,     0,
       0,     0,     0,    98,    99,   100,   101,    97,     0,   435,
       0,    57,   261,   262,     0,   259,     0,   226,     0,   336,
       0,     0,     0,    42,    32,    33,    34,    35,    38,    39,
      37,    36,     7,    40,    57,     0,     0,     0,     0,     0,
       0,     0,     0,   548,     0,   538,     0,   537,   524,   523,
     595,     0,     0,     0,     0,     0,     0,   380,     0,     0,
     381,   403,   405,     0,   375,     0,   384,     0,   385,     0,
     389,     0,   407,   399,   403,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   405,   405,   403,     0,   393,
     390,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   386,   400,   100,    97,    57,     0,     0,    45,    47,
      48,    53,     0,   175,   415,     0,   413,     0,   248,     0,
     248,     0,   360,     0,     0,     0,     0,     0,   427,   426,
     429,   424,   425,   433,   421,   422,   450,     0,     0,   455,
       0,   454,     5,     3,   177,   178,     0,   344,     0,   353,
     354,   355,   356,   357,     0,   351,     0,   131,     0,   132,
       0,     0,     0,   202,     0,    87,     9,     0,     0,     0,
     215,   212,     0,   213,     0,     0,    29,     0,     0,     0,
     304,   301,   300,   302,     0,   303,   299,   297,   298,   294,
       0,   327,   324,   323,   325,     0,   326,   103,   103,   103,
     319,   103,     0,    56,     0,     0,     0,   364,     0,     0,
       0,     0,     0,   103,     0,     0,     0,     0,     0,     0,
      44,   465,   464,   253,   342,     0,     0,   463,   511,   512,
     549,     0,   539,     0,     0,     0,     0,   584,     0,   377,
     378,   379,     0,   403,   405,   374,   406,   382,   383,    25,
     387,   404,     0,   397,   405,   546,     0,   570,   568,   569,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     540,   394,   398,   405,     0,   526,   527,   525,   590,   588,
     543,     0,   541,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   528,   533,   534,   536,   517,   518,   519,   520,
     521,   522,   535,     0,    49,     0,     0,     0,    65,    64,
      68,    72,    69,     0,     0,     0,    66,    54,     0,   547,
     418,   419,     0,   443,     0,     0,     0,   247,   250,     0,
     244,     0,   251,     0,   361,   362,   359,   367,     0,   430,
     428,     0,   431,     0,   448,   453,   458,     0,     0,   192,
     192,   192,     0,     0,   180,   181,   219,   220,   134,     0,
       0,   283,   143,     0,     0,    90,     0,    22,     0,     9,
      89,   210,   290,   222,     0,   104,     0,     0,   305,     0,
     328,     0,     0,   440,    55,     0,   265,   268,   264,   263,
     228,     0,     0,   337,     0,     0,     0,     0,     0,     0,
      43,     9,    41,     0,     0,   556,     0,     0,     0,   582,
       0,     0,   554,   388,   395,     0,   408,   391,     0,   563,
     566,     0,     0,     0,   557,   558,   559,   560,   561,   562,
     396,   403,   544,   542,     0,     0,   591,   589,     0,     0,
       0,     0,     0,     0,    57,    79,    52,     0,     9,    66,
      66,     0,    59,    46,     0,    67,     0,    51,    77,     0,
     416,     0,   441,   412,   415,   414,   246,   249,   258,   349,
     366,   372,     0,   423,     0,   476,   193,     0,     0,     0,
       0,     0,     0,     0,   189,     0,     0,     0,     0,   285,
       0,   144,   145,     0,     0,   139,    53,   204,   205,   203,
       0,    10,     0,    24,     9,    86,    88,   224,   223,   217,
      30,     0,     0,     0,     0,    94,     0,    94,   267,   266,
       0,     0,     0,     0,    94,     0,    94,     0,     0,     0,
      11,     0,     0,     0,   279,   272,   554,   586,     0,   585,
       0,    26,     0,     0,     0,     0,     0,   405,   552,   550,
       0,     0,   545,     0,     0,     0,   529,    50,     0,     0,
       0,    22,    70,    71,     0,     0,    73,     0,     0,     0,
     444,     0,   368,   369,   370,   432,   475,     0,     0,     0,
       0,     0,   183,   148,     0,     0,   187,   186,   185,   191,
     190,   182,   221,     0,   286,   143,     0,     0,     0,     0,
      81,    91,     0,     0,     0,     0,     0,     0,   111,     0,
      94,   112,     0,   230,     0,   338,   119,    94,   120,     0,
       0,     0,     0,    20,     0,   276,   277,   278,   583,   587,
       0,   564,     0,     0,   571,   572,   376,   553,   551,   532,
     530,   531,   126,     0,     0,    24,    62,    60,     0,     0,
      58,    74,     0,   442,    78,   466,   466,   466,   498,   466,
     488,   491,   466,   466,     0,   466,   466,   466,   500,   469,
     499,   466,   469,   466,     0,   466,   469,   489,   490,   497,
     466,   469,     0,     0,   492,   156,     0,     0,     0,     0,
       0,     0,     0,   188,     0,     0,     0,   141,   143,   142,
       0,    23,    15,   307,   307,   307,   114,   113,     0,     0,
     108,     0,   107,     0,    93,     0,     0,   118,   307,   330,
     307,     0,     0,    22,   340,     0,   567,   573,   574,   575,
     576,   579,   580,   581,   578,   577,     0,     0,     0,   122,
      75,     0,    61,     0,   471,   495,   477,   474,   471,   471,
       0,   471,   471,   471,   470,   474,   471,   474,   471,     0,
     471,   474,   471,   474,     0,     0,   150,     0,     0,     0,
       0,     0,   149,   156,     0,     0,     0,   138,   466,     0,
     206,    16,    17,    13,     0,   315,   292,   317,     0,     0,
       0,   105,     0,   229,     0,   332,   334,    18,    21,    24,
     565,   127,   128,     0,     0,     0,   483,   493,   487,   485,
     507,     0,   486,   481,   482,   504,   480,   503,   484,     0,
     479,   502,   478,   501,     0,     0,     0,   163,     0,     0,
     166,   158,     0,     0,     0,     0,   153,   152,     0,   184,
       0,   170,   143,    15,     0,   197,     0,     0,     0,     0,
       0,     0,   308,     0,   116,   115,   110,   109,     0,     0,
       0,    12,    27,     0,    76,     0,   467,   472,   473,     0,
     474,   474,   496,   474,   157,   167,   159,   160,   162,   161,
     165,   164,     0,   154,   155,   151,     0,   140,   208,   207,
      14,   309,   310,   311,   312,   313,    95,   106,   231,    19,
       0,     8,   129,     0,   508,   505,   506,   494,     0,     0,
      28,   468,   168,   171
};

/* YYPDEFGOTO[NTERM-NUM].  */
static const short int yydefgoto[] =
{
      -1,    43,    44,    45,   657,   913,   991,  1073,  1151,  1013,
     793,   348,  1191,   467,   154,   312,   313,   397,   398,   399,
     608,   495,   400,   605,   746,   606,   741,   940,   941,   750,
     401,    46,   117,   264,   455,    47,   898,   141,   288,   276,
     900,  1001,   805,    48,    49,   214,   738,    50,   251,    51,
     785,   786,   115,   872,   873,  1056,    52,    53,    54,    55,
     644,   645,   767,   102,    56,   116,   654,   789,  1069,  1070,
      57,    58,   266,    59,   249,   799,    60,   297,    61,    62,
      63,   222,    64,   408,   620,    65,    66,    67,    68,    69,
      70,   292,   293,   678,    71,    72,   160,   825,    73,   255,
     256,    74,   270,    75,   137,   138,  1075,  1142,    76,    77,
     139,   140,    78,    79,    80,    81,   299,    82,   319,    83,
     105,    84,    85,   445,    86,   626,    87,    88,   760,    89,
      90,   209,   352,   344,   543,   210,    91,   405,   406,   612,
      92,   424,   425,    93,   144,    94,   613,   752,    95,   235,
     236,    96,   431,   216,    97,   161,   162,  1034,  1045,  1096,
     434,   975,  1101,   217,   521,   551,   835,   333
};

/* YYPACT[STATE-NUM] -- Index in YYTABLE of the portion describing
   STATE-NUM.  */
#define YYPACT_NINF -941
static const short int yypact[] =
{
    2112,  -230,   121,   -68,   -65,   511,  -941,   236,   250,     1,
    1132,  -941,  -173,   197,    75,   -68,   135,  -109,  -941,  1132,
    -941,    22,   430,   172,    13,   473,  1171,    29,   366,   171,
     171,   221,   489,   171,   293,   298,   -68,   302,   -68,   286,
     506,   203,   512,  1952,   193,  -941,  -941,  -941,  -941,  -941,
    -941,  -941,  -941,  -941,  -941,  -941,  -941,  -941,  -941,  -941,
    -941,  -941,  -941,  -941,  -941,  -941,  -941,  -941,  -941,  -941,
    -941,  -941,  -941,  -941,  -941,  -941,  -941,  -941,  -941,  -941,
    -941,  -941,  -941,  -941,  -941,  -941,  -941,  -941,  -941,  -941,
    -941,  -941,  -941,  -941,  -941,  -941,  -941,  -941,  -941,   520,
    -941,   529,   342,  -941,  -941,   352,   590,   483,  -941,   459,
     483,  -941,  -941,  -941,   616,   435,   542,    76,  -941,  -941,
     549,   662,   549,   659,   668,   465,   395,  -105,   -99,   -89,
     -88,   525,   -84,  -113,  -941,  -108,  -100,   -66,  -941,   -64,
    -941,   547,  -941,  -941,  1171,   560,   686,   484,   689,   691,
     687,   547,   -62,   -30,    56,   398,   676,  -941,  -941,   596,
     439,   380,  -941,    94,  -941,  -941,  -941,  -941,   701,  1171,
    1310,  1171,  1171,  1171,   582,     8,  -941,  -941,  -941,   708,
     381,   609,   713,   421,  -941,   526,   703,   727,   438,   610,
     643,   464,   654,   610,  -941,   509,  -941,   451,   452,   454,
     456,   463,   466,   468,   470,   475,  1171,  -941,   181,   151,
     497,  1944,  -941,   418,    23,  -941,  1780,  -941,  -941,   801,
     804,  -941,  -941,   774,  -941,  -941,     3,   590,   590,   604,
     590,   394,  -941,   554,  -941,   486,  -941,   711,   814,   588,
    -941,   491,   493,  -941,  -941,  -941,   819,   590,   494,    61,
     805,   822,   844,   845,   568,   530,  -941,   483,   857,   533,
     858,  -941,  -941,  -941,   -83,   768,   863,   714,   865,  -941,
     540,  -941,   549,  -941,   -28,   868,  -941,  -941,  -941,   104,
    1968,   869,  1969,  -941,  -941,  -941,  -941,  -941,   871,  2925,
     615,    98,   570,  -941,   590,  -941,  -144,   571,   657,   581,
     880,   391,   908,   583,  -941,  -941,  -941,  -941,  -941,  -941,
    -941,  -941,   809,   591,  2904,  1171,  1171,  1171,   909,   902,
     650,   925,   423,  1171,  1171,  1251,   603,  1251,  -941,  -941,
    -941,  1171,  1996,    89,   932,   934,   942,  -941,   757,   944,
    -941,   654,   703,   945,  -941,   237,  -941,  1171,  -941,   940,
    -941,   947,   937,  -941,   654,  1171,   453,  1171,  1171,   605,
    1171,  1171,  1171,  1171,   149,   703,   703,   654,   847,  -941,
    -941,  1171,  1171,  1171,  1171,  1171,    59,   624,   564,  1171,
     531,  1171,  1171,  1171,  1171,  1171,  1171,  1171,  1171,  1171,
    1171,  -941,  -941,    24,    38,    55,    12,   764,    92,  -941,
    -941,   919,  1171,  -941,    17,    -5,  -941,   300,   -60,   690,
     -60,  -141,    64,   590,   -74,   955,   704,    50,  -941,  -941,
    -941,  -941,  -941,  -941,   634,  -941,  -941,   286,  2250,  -941,
     659,  -941,  -941,  -941,    51,    51,   365,   636,   963,  -941,
    -941,  -941,  -941,  -941,   964,  -941,   872,  -941,   637,  -941,
     929,   616,   974,   748,   954,  -941,   211,  -139,    23,   978,
    -941,  -941,   979,  -941,   975,   590,   658,   656,   663,   664,
    -941,  -941,  -941,  -941,   836,  -941,  -941,  -941,  -941,  -941,
     666,  -941,  -941,  -941,  -941,   838,  -941,   667,   667,   667,
    -941,  -122,   804,  -941,   999,   -34,   686,   636,  1000,  1003,
    1004,  1001,  1005,   -97,   678,   681,   682,   936,    23,   936,
    -941,  2925,  2925,  2925,  -941,   732,   742,  -941,  -941,  -941,
    -941,   683,  2925,   757,  2553,    91,  1171,  -941,  1171,  -941,
    -941,  -941,   688,   654,   703,  -941,  -941,  -941,  -941,  1870,
    -941,  -941,  1171,  -941,   703,  1751,   695,  -941,  -941,  -941,
     696,  1171,  1899,  1920,   697,   259,  1398,  1495,  1516,  1537,
    -941,  -941,  -941,   703,  1015,   816,   866,  1817,  2253,  2253,
    -941,    46,  -941,   967,  1171,  1171,   706,  2875,   707,   709,
     710,   967,  1050,  1201,   741,   712,   543,   543,   989,   989,
     989,   989,  -941,  1031,  -941,   715,   722,  1036,  -941,  -941,
    -941,   212,  -941,    23,    23,   917,   882,  -941,  1043,  -941,
    -941,  -941,    39,   941,  1052,  1056,   798,  -941,  -941,   890,
    -941,  1064,  -941,  1080,  -941,  -941,  -941,   636,  1081,  -941,
    -941,   914,   758,   394,  -941,  -941,   540,   897,  1087,  1037,
    1037,   408,  1044,    43,   765,  -941,  -941,   763,  -941,   868,
      10,  -941,   -82,   268,   956,  -941,  1171,   950,    23,   874,
     533,  -941,  -941,   -26,   868,  -941,  1094,  1095,  -941,  1119,
    -941,  1127,   -78,   803,  -941,   891,   864,  -941,  -941,  -941,
    -941,   915,  -132,  -941,   918,  1127,   -77,  1110,  1114,  1138,
    -941,   874,  -941,   812,    42,  -941,   821,  1171,  1171,  -941,
    2798,  2574,  -941,  -941,  -941,  1171,  2925,  -941,  1171,  -941,
    -941,  2702,  1020,  1020,  -941,  -941,  -941,  -941,  -941,  -941,
    -941,   654,  -941,  -941,   823,   825,  2253,  2253,   967,  1171,
     757,   757,   757,   826,    98,  -941,  -941,    -7,   -96,   882,
     882,  1027,  1007,  -941,    23,  -941,  1032,  -941,  -941,  1157,
     841,   910,  -941,  -941,   303,  -941,  -941,  -941,  -941,  -941,
     175,  -941,  1033,  -941,  1170,  -941,  -941,    99,  1177,  1053,
     868,  1054,  1180,  1187,  -941,  1188,   365,  1189,   870,  -941,
    1193,  -941,  -941,  1195,    99,  -941,   919,  -941,  -941,  -941,
    1197,  2925,  1154,   610,   874,  -941,  -941,  -941,  -941,  -941,
    -941,   984,   985,   986,  1186,   -19,   879,  1055,  -941,  -941,
    1210,  1212,  1213,  1209,   -19,   879,  1055,  1128,  1133,  1135,
    1111,  1228,   383,  1065,  -941,  -941,  1576,  2925,  2819,  -941,
    1171,  2925,  1370,  1171,  1171,   911,   912,   703,  -941,  -941,
     913,   920,  1077,   922,   933,   951,  -941,  -941,  1171,  1239,
    1243,   950,  -941,  -941,    23,  1171,   -72,    23,   114,  1244,
    -941,  1157,  -941,  -941,  -941,  -941,  -941,  2563,   312,   935,
     953,  1122,   966,  -941,  1246,   868,  -941,  -941,  -941,  -941,
    -941,  -941,  -941,   274,  -941,   -75,   190,   757,   965,  1171,
    -941,  -941,   616,   616,   616,  1018,  1107,  1248,  -941,  1045,
     -17,  -941,   952,  -941,  1066,  -941,  -941,   -17,  -941,   659,
     659,   659,  1250,  1190,   969,  -941,  -941,  -941,  -941,  2925,
    1171,  -941,   970,  2434,  -941,  -941,  -941,  -941,  -941,  -941,
    -941,  -941,  2925,  1283,     4,   610,  -941,  2925,  1171,   973,
    -941,  -941,   -38,  -941,  -941,   977,   977,   977,  -941,   977,
    -941,  -941,   977,   977,   980,   977,   977,   977,  -941,  1262,
    -941,   977,  1262,   977,   981,   977,  1262,  -941,  -941,  -941,
     977,  1262,   988,   991,  -941,  -941,   992,   993,   868,   868,
     994,    99,  2563,  -941,  1286,  1153,    99,  -941,   -71,  -941,
    1307,  -941,  2840,    77,    77,    77,  -941,  2925,  1192,  1306,
    -941,   231,  2925,   998,  -941,  1327,  1328,  -941,    79,   540,
      79,  1171,  1171,   950,  -941,  1728,  -941,  -941,  -941,  -941,
    -941,  -941,  -941,  -941,  -941,  -941,  1171,  1171,  1329,  -941,
    2925,   868,  -941,  1331,  -941,  -941,  -941,  -941,  -941,  -941,
    1342,  -941,  -941,  -941,  -941,  -941,  -941,  -941,  -941,  1342,
    -941,  -941,  -941,  -941,  1343,  1345,   536,   868,   868,  1022,
    1023,   868,  -941,  -941,  1347,  1330,   304,  -941,   977,   306,
    -941,  -941,  -941,  1028,   219,  -941,  -941,  -941,  1120,  1121,
    1144,  -941,  1045,  -941,  1026,  -941,  -941,  1117,  2925,   610,
    -941,  2925,  2925,  1341,  1030,   308,  -111,    51,  -111,  -111,
    -941,   320,  -111,  -111,  -111,    51,  -111,    51,  -111,   322,
    -111,    51,  -111,    51,  1047,  1069,  1223,  -941,  1359,   614,
    -941,  -941,  1259,   -81,  1070,  1071,  -941,  -941,  1072,   536,
    1240,  -941,   -70,   364,  1307,  -941,  1171,  1247,  1171,  1171,
    1171,  1171,  -941,  1127,  -941,  2925,  -941,  2925,   327,  1412,
    1245,  -941,  1304,  1171,  -941,  1426,  -941,  -941,  -941,  1428,
    -941,  -941,  -941,  -941,  -941,  -941,  -941,  -941,  -941,  -941,
    -941,  -941,   868,  -941,  -941,  -941,  1277,  -941,  -941,  -941,
    -941,  -941,  2925,  2925,  2925,  2925,  1105,  -941,  -941,  -941,
     868,  -941,  2925,  1103,  -941,    51,    51,    51,  1104,  1434,
    -941,  -941,  -941,  -941
};

/* YYPGOTO[NTERM-NUM].  */
static const short int yypgoto[] =
{
    -941,  -941,   -33,  -330,  -286,  -941,  -940,   305,  -941,  -941,
    -801,  -192,  -941,  -647,  -941,  -179,  -941,  -362,  -941,  -549,
     653,  -307,  -941,  -941,  -135,  -941,  -941,  -941,   507,   702,
    -941,  -941,  -941,  1196,   790,  -941,  -342,   107,  1302,  -117,
     640,   375,  -679,  -941,  -941,  -941,  -941,  -941,   -55,  -941,
    -864,  -941,  1449,  -749,   478,   397,  -941,  -941,  -941,  -941,
    -941,   685,    41,  -941,  -941,  -941,  -941,  -941,  -941,   328,
    -941,  -941,   -73,  -941,   185,  -941,  -941,  -941,  -941,  -941,
    -941,   358,  -941,  1252,  1063,  -941,  -941,  -941,  -941,  -941,
    -941,  -941,   983,  -941,  -941,  -941,  1452,  -941,  -941,  -251,
    1025,  -941,  -421,  -941,  1461,  1202,  -881,  -941,  -941,  -941,
    1462,  1203,  -941,  -941,  -941,  -941,  -941,  -941,  -941,  -941,
     384,  -941,  -941,  -941,  -941,  -941,  -941,  -941,  -941,  -941,
    -941,  -941,  -329,  -337,  -941,  -941,  -941,   995,   873,   729,
    -941,  -941,   851,  -941,  -941,  -941,  -941,  -941,  -941,  -941,
    1059,  -941,  -941,   -22,  -941,  -941,  1168,   766,  -461,  -250,
    -242,   517,   505,  -319,  -941,  -941,   787,  1169
};

/* YYTABLE[YYPACT[STATE-NUM]].  What to do in state STATE-NUM.  If
   positive, shift that token.  If negative, reduce the rule which
   number is the opposite.  If zero, do what YYDEFACT says.
   If YYTABLE_NINF, syntax error.  */
#define YYTABLE_NINF -555
static const short int yytable[] =
{
     211,   353,   778,   435,   520,   535,   814,   510,   532,   636,
     242,   614,   534,   848,   779,   395,  -123,   800,   277,   278,
    -417,   987,   458,   797,  1027,   544,   395,  -125,   561,   562,
     155,   628,   218,   781,   596,   886,   546,   550,   563,   407,
     781,  -124,   748,  -103,   781,   781,   774,  1171,  -103,   268,
     935,  -304,   722,   248,   742,   253,  -103,  -301,   493,   163,
     164,   165,   166,   167,   168,   570,   595,  -300,  -302,   120,
     671,  1087,  -303,   169,   170,   498,   334,   571,   623,   259,
     171,   172,   303,   609,   938,   103,   142,   811,   594,   304,
     279,   658,   281,   494,   301,   685,   659,   782,    98,   173,
     149,   493,   867,   637,   782,   638,   174,   468,   782,   782,
     675,    20,    20,  1076,  1077,  1157,   618,   121,   938,   176,
     177,   178,   289,   876,  1067,   151,   302,  1085,  -305,  1086,
     469,   213,   314,   610,  1158,   143,   494,   896,   676,   896,
     305,   306,   150,   656,   106,   611,   691,   325,   327,   328,
     329,   330,   332,   180,   598,   798,   806,   815,   939,   371,
     372,   373,   374,   375,   376,   377,   307,   378,   379,   380,
     381,   382,   383,   384,   385,   386,   387,   388,   389,   390,
     526,   260,   698,   527,   364,   699,   499,   122,   444,    99,
     459,   261,   939,   696,   335,   856,  1180,   704,   812,   465,
     723,    20,   452,   104,   703,   868,   619,   707,   677,   275,
     599,   112,  1089,   572,   822,   869,  -103,   336,   275,   -63,
     262,  -103,   409,   275,  -304,   600,   720,   870,   983,  -103,
    -301,   275,   439,   850,   275,   624,   100,  1066,   219,   118,
    -300,  -302,   743,   724,   601,  -303,   459,   263,   783,   784,
    1172,   733,   123,   119,   725,   444,   986,  -136,  -137,   308,
     309,   310,   775,   280,   311,   282,   871,   280,  1177,   371,
     372,   373,   374,   375,   376,   377,   602,   378,   379,   380,
     381,   382,   383,   384,   385,   386,   387,   388,   389,   390,
     156,   631,   780,   511,   512,   513,   794,   157,   158,   282,
     603,  -305,   522,   444,   159,   936,  -417,   101,   942,   524,
     897,   124,  1003,   145,   440,   441,   442,   443,  1074,   625,
    1074,   823,   824,   849,   615,   539,  1137,   528,   690,   528,
     692,  1059,  1060,   545,  1028,   552,   553,   555,   556,   557,
     558,   559,   807,   396,  -123,   739,   862,   146,  -417,   565,
     566,   567,   568,   569,   396,  -125,   816,   577,   582,   583,
     584,   585,   586,   587,   588,   589,   590,   591,   592,  -124,
     749,   277,   278,   795,   672,   197,   198,   199,   200,   201,
     202,   203,   204,   205,  1094,   593,   686,   206,   223,   207,
     444,   226,   837,   444,   504,   635,   740,   639,   840,   147,
     843,   844,   845,  1071,   148,   820,   451,   315,   464,   841,
    1124,  1125,   411,   412,  1128,   414,   640,   505,   316,   610,
     229,   604,   231,   367,   322,   323,   518,   847,   976,   984,
     863,   611,   437,   163,   164,   165,   166,   167,   168,   985,
     977,   -89,   368,   859,   212,  1072,   943,   169,   170,   519,
     656,   369,   851,   641,   171,   172,   163,   164,   165,   166,
     167,   168,   864,   766,  1186,   901,  1138,  1139,  1140,  1141,
     169,   170,   906,   173,   908,   365,   215,   171,   172,   497,
     174,   560,   220,   366,   221,   314,   224,   314,  1008,  1009,
    1010,   175,   225,   176,   177,   178,   173,   283,   179,   547,
     926,  1047,   227,   174,   700,  1051,   701,   228,   891,   237,
    1053,   230,   769,   238,   922,   239,   176,   177,   178,   981,
     706,   243,   988,   244,   770,  1198,   642,   180,   284,   711,
     537,   538,   245,   393,   163,   164,   165,   166,   167,   168,
     -89,   454,   643,  1200,   232,   233,   181,   234,   169,   170,
     180,   246,   726,   727,  1116,   171,   172,   989,  1004,   364,
    1080,   247,   394,  1081,   578,  1007,   616,   617,   579,   386,
     387,   388,   389,   390,   173,   771,  1117,   574,   575,   107,
     576,   174,   787,   788,   548,   163,   164,   165,   166,   167,
     168,   715,  1118,   248,   176,   177,   178,   250,   627,   169,
     170,   890,   632,   415,   852,   853,   171,   172,   163,   164,
     165,   166,   167,   168,  1119,   252,   182,   108,  1166,  1167,
    1168,  1169,   169,   170,   254,   173,   283,  -198,   180,   171,
     172,   554,   174,   981,   791,  1134,  1132,  1155,  1135,   183,
    1156,   993,   994,   995,   257,   176,   177,   178,   173,  1159,
     663,  1159,  1160,   416,  1161,   174,  1080,   284,   258,  1187,
     915,   916,   285,   265,  1120,   267,   417,   269,   176,   177,
     178,   271,   286,   549,   272,   827,   828,   418,   273,   180,
     109,   768,   772,   831,   274,   290,   832,   419,   420,   291,
    1121,   287,   295,   294,   296,   298,   317,   110,   318,   421,
     422,   423,   180,  1122,   320,   157,   158,   842,   111,   321,
     324,   337,   338,   184,   339,   341,   340,   343,   342,   185,
     186,   112,   187,  -409,  -273,   188,   580,   189,   190,   191,
     345,   192,  -273,   193,   346,   113,   384,   385,   386,   387,
     388,   389,   390,  1029,   347,   349,   194,   195,   196,   197,
     198,   199,   200,   201,   202,   203,   204,   205,   350,   351,
    1123,   206,   114,   207,   383,   384,   385,   386,   387,   388,
     389,   390,   197,   198,   199,   200,   201,   202,   203,   204,
     205,   354,   355,   356,   206,   357,   207,   358,  1098,  1099,
     370,  1102,  1103,  1104,   359,  1097,  1106,   360,  1108,   361,
    1110,   362,  1112,  1105,   403,  1107,   363,   404,   919,  1111,
     407,  1113,   923,   413,   426,   427,   428,   429,   430,   432,
     331,   433,   436,   446,   438,   447,   932,   372,   373,   374,
     375,   376,   377,   937,   378,   379,   380,   381,   382,   383,
     384,   385,   386,   387,   388,   389,   390,   448,   449,   450,
     197,   198,   199,   200,   201,   202,   203,   204,   205,   451,
     453,   456,   581,   454,   207,   460,   461,   992,   463,   464,
     462,   466,   480,   997,   491,   492,   501,  1002,   373,   374,
     375,   376,   377,   503,   378,   379,   380,   381,   382,   383,
     384,   385,   386,   387,   388,   389,   390,  1152,  1015,   496,
     500,   197,   198,   199,   200,   201,   202,   203,   204,   205,
     502,   506,   507,   206,   508,   207,  1030,   514,  1195,  1196,
     509,  1197,   515,   516,   197,   198,   199,   200,   201,   202,
     203,   204,   205,   155,   523,   529,   206,   530,   207,   163,
     164,   165,   166,   167,   168,   531,    20,   533,   540,   536,
     541,   542,   564,   169,   170,   573,   597,   607,   629,   621,
     171,   172,   303,   633,   630,   444,   646,   647,   649,   648,
     163,   164,   165,   166,   167,   168,   650,   652,   653,   173,
     655,   660,   661,   662,   169,   170,   174,   664,   665,   992,
    1088,   171,   172,   666,   667,   668,   669,   670,   275,   176,
     177,   178,   674,   680,  1091,  1092,   681,   682,   687,   683,
     173,   688,   689,   684,   693,   695,   694,   174,   721,   390,
     702,   163,   164,   165,   166,   167,   168,   709,   710,   714,
     176,   177,   178,   180,   734,   169,   170,   728,   730,   737,
     731,   732,   171,   172,   744,   745,   747,   735,   163,   164,
     165,   166,   167,   168,   736,   753,   751,  1145,  1147,   754,
    1002,   173,   169,   170,   180,   756,   757,   758,   174,   171,
     172,   381,   382,   383,   384,   385,   386,   387,   388,   389,
     390,   176,   177,   178,   759,   761,   762,   444,   173,   764,
     765,   773,   766,   777,   776,   174,   996,   380,   381,   382,
     383,   384,   385,   386,   387,   388,   389,   390,   176,   177,
     178,   792,   790,   656,   992,   180,  1182,  1183,  1184,  1185,
     801,   802,   803,  1000,   163,   164,   165,   166,   167,   168,
     804,  1192,   615,   808,   810,   809,   817,   813,   169,   170,
     818,   819,   180,   821,   834,   171,   172,   163,   164,   165,
     166,   167,   168,   826,   854,   838,    20,   839,   846,   857,
     748,   169,   170,   855,   173,   126,   127,   860,   171,   172,
     859,   174,   865,   866,   163,   164,   165,   166,   167,   168,
     874,   875,   877,   878,   176,   177,   178,   173,   169,   170,
     879,   880,   882,   128,   174,   171,   172,   884,   885,  1144,
     888,   889,   883,   892,   893,   894,   895,   176,   177,   178,
     899,   896,   129,   902,   173,   903,   904,   905,   180,   912,
     130,   174,  1146,   382,   383,   384,   385,   386,   387,   388,
     389,   390,   914,   909,   176,   177,   178,   917,   910,   131,
     911,   180,   933,   924,   925,   927,   934,   944,   132,   982,
     980,   999,   928,   133,   929,   197,   198,   199,   200,   201,
     202,   203,   204,   205,   998,   930,   978,   206,   180,   207,
     379,   380,   381,   382,   383,   384,   385,   386,   387,   388,
     389,   390,  1005,   931,   979,  1006,   197,   198,   199,   200,
     201,   202,   203,   204,   205,   981,   990,  1011,   206,  1012,
     207,  1014,  1016,  1026,  1031,  1044,  1064,   134,  1033,  1065,
    1068,  1040,  1049,   163,   164,   165,   166,   167,   168,  1054,
    1078,   135,  1055,  1057,  1058,  1061,  1079,   169,   170,  1082,
    1083,  1084,  1093,  1131,   171,   172,  1095,   197,   198,   199,
     200,   201,   202,   203,   204,   205,  1100,  1143,  1114,   206,
    1115,   207,  1130,   173,  1126,  1127,  1149,  1136,  1150,   136,
     174,  1153,  1154,  1165,   197,   198,   199,   200,   201,   202,
     203,   204,   205,   176,   177,   178,   206,  1164,   207,  1162,
     371,   372,   373,   374,   375,   376,   377,  1170,   378,   379,
     380,   381,   382,   383,   384,   385,   386,   387,   388,   389,
     390,  1163,  1173,  1174,  1175,  1176,  1181,   326,   371,   372,
     373,   374,   375,   376,   377,  1188,   378,   379,   380,   381,
     382,   383,   384,   385,   386,   387,   388,   389,   390,  1190,
    1189,  1193,  1194,  1199,   897,  1201,  1202,  1203,  1178,   887,
     197,   198,   199,   200,   201,   202,   203,   204,   205,  1032,
     796,   858,   206,   300,   207,   907,   457,  1148,   125,  1062,
    1129,   881,  1179,   197,   198,   199,   200,   201,   202,   203,
     204,   205,   920,   622,   208,   206,   651,   207,   410,   679,
     152,   153,   479,   861,   763,   490,   634,   673,   755,   517,
     197,   198,   199,   200,   201,   202,   203,   204,   205,  1063,
     836,   525,   206,     0,   207,   371,   372,   373,   374,   375,
     376,   377,     0,   378,   379,   380,   381,   382,   383,   384,
     385,   386,   387,   388,   389,   390,   371,   372,   373,   374,
     375,   376,   377,     0,   378,   379,   380,   381,   382,   383,
     384,   385,   386,   387,   388,   389,   390,   371,   372,   373,
     374,   375,   376,   377,  1109,   378,   379,   380,   381,   382,
     383,   384,   385,   386,   387,   388,   389,   390,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,  -554,
       0,     0,     0,     0,     0,     0,  -554,  -554,  -554,  -554,
    -554,  -554,  -554,     0,  -554,  -554,  -554,  -554,  -554,  -554,
    -554,  -554,  -554,  -554,  -554,  -554,  -554,     0,     0,     0,
       0,     0,  -554,     0,  -554,  -554,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   197,
     198,   199,   200,   201,   202,   203,   204,   205,  -554,     0,
       0,   206,     0,   207,     0,     0,     0,     0,  -554,  -554,
    -554,  -554,     0,     0,     0,     0,     0,  -554,     0,     0,
       0,     0,     0,     0,     0,     0,     0,  -554,     0,     0,
    -554,     0,     0,     0,     0,     0,     0,     0,  -554,     0,
       0,  -554,     0,     0,  -554,  -554,     0,  -554,  -554,  -554,
       0,     0,     0,     0,  -554,     0,     0,     0,     0,     0,
       0,  -554,   921,  -554,     0,     0,     0,     0,     0,  -554,
    -554,     0,  1035,  1036,     0,  1037,     0,     0,  1038,  1039,
       0,  1041,  1042,  1043,     0,     0,     0,  1046,  -554,  1048,
     716,  1050,  -554,     0,     0,     0,  1052,  -554,   371,   372,
     373,   374,   375,   376,   377,     0,   378,   379,   380,   381,
     382,   383,   384,   385,   386,   387,   388,   389,   390,     0,
    -554,   371,   372,   373,   374,   375,   376,   377,  -554,   378,
     379,   380,   381,   382,   383,   384,   385,   386,   387,   388,
     389,   390,     0,     0,  -554,     0,     0,     0,     0,  -554,
     371,   372,   373,   374,   375,   376,   377,     0,   378,   379,
     380,   381,   382,   383,   384,   385,   386,   387,   388,   389,
     390,     0,     0,     0,  -554,  -554,     0,  -554,     0,  -554,
    -554,     0,     0,     0,     0,     0,     0,   717,     0,     0,
     374,   375,   376,   377,  1133,   378,   379,   380,   381,   382,
     383,   384,   385,   386,   387,   388,   389,   390,   718,     0,
       0,     0,     0,     0,     0,     0,   708,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   719,
       0,     0,     0,  -554,     0,     0,     0,     0,     0,  -554,
     371,   372,   373,   374,   375,   376,   377,     0,   378,   379,
     380,   381,   382,   383,   384,   385,   386,   387,   388,   389,
     390,     0,     0,     0,  -554,  -554,     0,     0,  -554,   371,
     372,   373,   374,   375,   376,   377,     0,   378,   379,   380,
     381,   382,   383,   384,   385,   386,   387,   388,   389,   390,
     371,   372,   373,   374,   375,   376,   377,     0,   378,   379,
     380,   381,   382,   383,   384,   385,   386,   387,   388,   389,
     390,     0,   240,   241,   371,   372,   373,   374,   375,   376,
     377,     0,   378,   379,   380,   381,   382,   383,   384,   385,
     386,   387,   388,   389,   390,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     2,     3,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   126,   470,   481,     0,     4,   371,   372,   373,   374,
     375,   376,   377,     5,   378,   379,   380,   381,   382,   383,
     384,   385,   386,   387,   388,   389,   390,     0,     0,   471,
     482,     0,     6,     7,     8,     0,     0,     0,     0,     0,
       9,     0,     0,     0,     0,     0,     0,     0,   472,   483,
       0,     0,     0,     0,     0,     0,   473,   484,     0,    10,
    1090,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,    11,     0,   474,   485,     0,     0,     0,
     402,     0,    12,     0,   475,   486,     0,     0,    13,   476,
     487,    14,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   402,
      15,     0,     0,     1,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,    16,    17,
       0,    18,     0,     0,     0,    19,     0,     0,     0,     0,
       0,    20,     0,   134,    21,    22,     2,     3,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   477,   488,     0,
       0,     0,     0,     0,     0,     4,     0,     0,     0,     0,
       0,     0,     0,     5,     0,     0,     0,    23,     0,    24,
       0,    25,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     6,     7,     8,   478,   489,     0,    26,   705,
       9,     0,     0,     0,    27,     0,     0,     0,     0,     0,
       0,     0,    28,     0,    29,    30,    31,     0,     0,    10,
       0,    32,    33,     0,     0,     0,     0,     0,   712,     0,
       0,     0,     0,    11,   331,     0,    34,    35,    36,     0,
      37,   391,    12,     0,     0,     0,     0,   392,    13,   713,
       0,    14,     0,     0,     0,     0,    38,     0,     0,     0,
       0,    39,    40,    41,    42,     0,  -555,  -555,  -555,  -555,
      15,   378,   379,   380,   381,   382,   383,   384,   385,   386,
     387,   388,   389,   390,     2,     3,     0,     0,    16,    17,
       0,    18,     0,     0,     0,    19,     0,     0,     0,     0,
       0,    20,     0,     4,    21,    22,     0,     0,     0,     0,
       0,     5,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       6,     7,     8,     0,     0,     0,     0,    23,     9,    24,
       0,    25,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,    10,    26,     0,
       0,     0,     0,     0,    27,     0,     0,     0,     0,     0,
       0,    11,    28,     0,    29,    30,    31,     0,     0,     0,
      12,    32,    33,     0,     0,     0,    13,     0,     0,    14,
       0,     0,     0,     0,     0,     0,    34,    35,    36,     0,
      37,     0,     0,     0,     0,     0,     0,     0,    15,     0,
       0,     0,     0,     0,     0,     0,    38,     0,     0,     0,
       0,    39,    40,    41,    42,     0,    16,    17,     0,    18,
       0,     0,     0,    19,     0,     0,     0,     0,     0,    20,
       0,     0,    21,    22,   371,   372,   373,   374,   375,   376,
     377,     0,   378,   379,   380,   381,   382,   383,   384,   385,
     386,   387,   388,   389,   390,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    23,     0,    24,     0,    25,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,    26,     0,     0,     0,
       0,     0,    27,     0,     0,     0,  1017,  1018,  1019,  1020,
      28,     0,    29,    30,    31,     0,     0,     0,     0,    32,
      33,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    34,    35,    36,     0,    37,     0,
       0,     0,     0,     0,     0,  1021,  1022,  1023,     0,     0,
       0,     0,     0,     0,    38,     0,     0,     0,     0,    39,
      40,    41,    42,   371,   372,   373,   374,   375,   376,   377,
       0,   378,   379,   380,   381,   382,   383,   384,   385,   386,
     387,   388,   389,   390,   371,   372,   373,   374,   375,   376,
     377,     0,   378,   379,   380,   381,   382,   383,   384,   385,
     386,   387,   388,   389,   390,   945,   946,   947,   948,     0,
       0,     0,     0,     0,     0,   949,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   950,   951,     0,     0,     0,     0,   952,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     953,     0,     0,     0,     0,     0,     0,     0,   954,     0,
       0,     0,     0,     0,   955,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,  1024,  1025,     0,
       0,     0,     0,     0,     0,   956,   957,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   958,   959,     0,     0,     0,   960,
     961,   962,   371,   372,   373,   374,   375,   376,   377,     0,
     378,   379,   380,   381,   382,   383,   384,   385,   386,   387,
     388,   389,   390,     0,     0,     0,     0,   963,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   964,     0,   965,     0,     0,
       0,     0,     0,     0,     0,     0,   697,     0,     0,     0,
       0,     0,     0,     0,   966,     0,     0,   967,   968,   969,
     970,   971,     0,     0,     0,     0,     0,   830,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   972,   973,
       0,     0,     0,     0,     0,     0,   974,   833,   371,   372,
     373,   374,   375,   376,   377,     0,   378,   379,   380,   381,
     382,   383,   384,   385,   386,   387,   388,   389,   390,   371,
     372,   373,   374,   375,   376,   377,     0,   378,   379,   380,
     381,   382,   383,   384,   385,   386,   387,   388,   389,   390,
     371,   372,   373,   374,   375,   376,   377,     0,   378,   379,
     380,   381,   382,   383,   384,   385,   386,   387,   388,   389,
     390,     0,     0,     0,     0,     0,     0,     0,     0,  1071,
       0,     0,     0,     0,     0,   371,   372,   373,   374,   375,
     376,   377,   829,   378,   379,   380,   381,   382,   383,   384,
     385,   386,   387,   388,   389,   390,     0,   493,     0,     0,
       0,   729,     0,   918,   371,   372,   373,   374,   375,   376,
     377,  1072,   378,   379,   380,   381,   382,   383,   384,   385,
     386,   387,   388,   389,   390,   371,   372,   373,   374,   375,
     376,   377,   494,   378,   379,   380,   381,   382,   383,   384,
     385,   386,   387,   388,   389,   390
};

/* YYCONFLP[YYPACT[STATE-NUM]] -- Pointer into YYCONFL of start of
   list of conflicting reductions corresponding to action entry for
   state STATE-NUM in yytable.  0 means no conflicts.  The list in
   yyconfl is terminated by a rule number of 0.  */
static const unsigned char yyconflp[] =
{
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,    21,     0,     0,     0,     0,    25,     0,
       0,     1,     0,     0,     0,     0,    29,     5,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     9,    13,     0,
       0,     0,    17,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   167,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,    33,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,    23,     0,     0,     0,
       0,    27,     0,     0,     3,     0,     0,     0,     0,    31,
       7,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      11,    15,     0,     0,     0,    19,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,    35,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,    37,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,    39,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,    41,     0,     0,     0,     0,     0,     0,
      43,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   169,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    45,
       0,     0,     0,     0,     0,     0,    47,    49,    51,    53,
      55,    57,    59,     0,    61,    63,    65,    67,    69,    71,
      73,    75,    77,    79,    81,    83,    85,     0,     0,     0,
       0,     0,    87,     0,    89,    91,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,    93,     0,
       0,     0,     0,     0,     0,     0,     0,     0,    95,    97,
      99,   101,     0,     0,     0,     0,     0,   103,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   105,     0,     0,
     107,     0,     0,     0,     0,     0,     0,     0,   109,     0,
       0,   111,     0,     0,   113,   115,     0,   117,   119,   121,
       0,     0,     0,     0,   123,     0,     0,     0,     0,     0,
       0,   125,     0,   127,     0,     0,     0,     0,     0,   129,
     131,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   133,     0,
       0,     0,   135,     0,     0,     0,     0,   137,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     139,     0,     0,     0,     0,     0,     0,     0,   141,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   143,     0,     0,     0,     0,   145,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   147,   149,     0,   151,     0,   153,
     155,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   157,     0,     0,     0,     0,     0,   159,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   161,   163,     0,     0,   165,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0
};

/* YYCONFL[I] -- lists of conflicting rule numbers, each terminated by
   0, pointed into by YYCONFLP.  */
static const short int yyconfl[] =
{
       0,   327,     0,   327,     0,   324,     0,   324,     0,   323,
       0,   323,     0,   325,     0,   325,     0,   326,     0,   326,
       0,   299,     0,   299,     0,   297,     0,   297,     0,   298,
       0,   298,     0,   328,     0,   328,     0,    29,     0,   431,
       0,    59,     0,   416,     0,   555,     0,   555,     0,   555,
       0,   555,     0,   555,     0,   555,     0,   555,     0,   555,
       0,   555,     0,   555,     0,   555,     0,   555,     0,   555,
       0,   555,     0,   555,     0,   555,     0,   555,     0,   555,
       0,   555,     0,   555,     0,   555,     0,   555,     0,   555,
       0,   555,     0,   555,     0,   555,     0,   555,     0,   555,
       0,   555,     0,   555,     0,   555,     0,   555,     0,   555,
       0,   555,     0,   555,     0,   555,     0,   555,     0,   555,
       0,   555,     0,   555,     0,   555,     0,   555,     0,   555,
       0,   555,     0,   555,     0,   555,     0,   555,     0,   555,
       0,   555,     0,   555,     0,   555,     0,   555,     0,   555,
       0,   555,     0,   555,     0,   555,     0,   555,     0,   555,
       0,   555,     0,   555,     0,   555,     0,    73,     0,   183,
       0
};

static const short int yycheck[] =
{
      22,   193,   649,   245,   323,   342,   685,   314,   338,   430,
      43,    16,   341,    20,     4,     3,     3,   664,   135,   136,
       3,   885,   105,    49,    20,   354,     3,     3,   365,   366,
       8,   105,     3,   115,   396,   784,   355,   356,   367,    36,
     115,     3,     3,   156,   115,   115,     3,   128,   156,   122,
     851,   156,     6,     3,   603,   110,   156,   156,     3,     3,
       4,     5,     6,     7,     8,     6,   396,   156,   156,    68,
     192,  1011,   156,    17,    18,   219,    68,    18,   219,     3,
      24,    25,    26,   402,   156,   153,   259,   219,   395,    33,
     156,   230,   156,    38,   156,   192,   458,   179,   328,    43,
     209,     3,     3,    52,   179,    54,    50,     3,   179,   179,
     144,   189,   189,   994,   995,   226,   176,   116,   156,    63,
      64,    65,   144,   770,   988,    18,   156,  1008,   156,  1010,
      26,    24,   154,   116,   245,   308,    38,   156,   172,   156,
      84,    85,   251,   239,   209,   128,   508,   169,   170,   171,
     172,   173,   174,    97,    62,   181,   234,   234,   230,    10,
      11,    12,    13,    14,    15,    16,   110,    18,    19,    20,
      21,    22,    23,    24,    25,    26,    27,    28,    29,    30,
      91,   105,    91,    94,   206,    94,   330,   186,   329,    68,
     329,   115,   230,   523,   186,   744,  1136,   534,   330,   272,
     154,   189,   257,   271,   533,   106,   266,   544,   242,   331,
     118,   210,  1013,   154,   172,   116,   329,   209,   331,   127,
     144,   329,   219,   331,   329,   133,   563,   128,   875,   329,
     329,   331,   171,   329,   331,   171,   115,   986,   209,     3,
     329,   329,   604,   573,   152,   329,   329,   171,   330,   331,
     331,   581,   251,     3,   573,   329,   331,   328,   328,   203,
     204,   205,   219,   329,   208,   329,   167,   329,  1132,    10,
      11,    12,    13,    14,    15,    16,   184,    18,    19,    20,
      21,    22,    23,    24,    25,    26,    27,    28,    29,    30,
     268,   241,   282,   315,   316,   317,   658,   275,   276,   329,
     208,   329,   324,   329,   282,   854,     3,   186,   857,   331,
     329,   310,   329,   116,   253,   254,   255,   256,   241,   255,
     241,   279,   280,   330,   329,   347,   107,   238,   507,   238,
     509,   978,   979,   355,   330,   357,   358,   359,   360,   361,
     362,   363,   672,   331,   331,   133,   171,   272,   331,   371,
     372,   373,   374,   375,   331,   331,   686,   379,   380,   381,
     382,   383,   384,   385,   386,   387,   388,   389,   390,   331,
     331,   488,   489,   659,   491,   319,   320,   321,   322,   323,
     324,   325,   326,   327,  1031,   330,   503,   331,    30,   333,
     329,    33,   721,   329,     3,   428,   184,    32,   728,    15,
     730,   731,   732,    39,   269,   691,   329,     9,   329,   728,
    1057,  1058,   227,   228,  1061,   230,    51,    26,    20,   116,
      36,   329,    38,   272,   330,   331,     3,   734,   116,   155,
     255,   128,   247,     3,     4,     5,     6,     7,     8,   165,
     128,   230,   291,   329,   272,    81,   332,    17,    18,    26,
     239,   300,   738,    88,    24,    25,     3,     4,     5,     6,
       7,     8,   287,    55,  1143,   807,   247,   248,   249,   250,
      17,    18,   814,    43,   816,   294,     3,    24,    25,   294,
      50,   332,   116,   302,   313,   507,   265,   509,   909,   910,
     911,    61,     3,    63,    64,    65,    43,    79,    68,    46,
     837,   962,   209,    50,   526,   966,   528,   209,   794,     3,
     971,   209,   104,   310,   833,     3,    63,    64,    65,   329,
     542,   328,   332,     3,   116,  1172,   161,    97,   110,   551,
     293,   294,     3,   115,     3,     4,     5,     6,     7,     8,
     329,   330,   177,  1190,   258,   259,   116,   261,    17,    18,
      97,   209,   574,   575,    18,    24,    25,   887,   900,   581,
     329,   209,   144,   332,    33,   907,   266,   267,    37,    26,
      27,    28,    29,    30,    43,   167,    40,    13,    14,    68,
      16,    50,   314,   315,   131,     3,     4,     5,     6,     7,
       8,   332,    56,     3,    63,    64,    65,   114,   413,    17,
      18,   793,   417,   209,   739,   740,    24,    25,     3,     4,
       5,     6,     7,     8,    78,   156,   186,   106,     4,     5,
       6,     7,    17,    18,     8,    43,    79,   116,    97,    24,
      25,    26,    50,   329,   656,   329,   332,   329,   332,   209,
     332,   892,   893,   894,   209,    63,    64,    65,    43,   329,
     465,   329,   332,   259,   332,    50,   329,   110,   116,   332,
     277,   278,   115,   114,   128,     3,   272,     8,    63,    64,
      65,     3,   125,   220,   209,   697,   698,   283,   283,    97,
     169,   640,   641,   705,   159,   125,   708,   293,   294,     3,
     154,   144,     3,   209,     3,     8,    20,   186,   102,   305,
     306,   307,    97,   167,   265,   275,   276,   729,   197,   329,
       9,     3,   331,   283,   105,   294,     3,    14,   192,   289,
     290,   210,   292,   293,   294,   295,   195,   297,   298,   299,
       3,   301,   302,   303,   296,   224,    24,    25,    26,    27,
      28,    29,    30,   935,   134,   102,   316,   317,   318,   319,
     320,   321,   322,   323,   324,   325,   326,   327,   294,   105,
     224,   331,   251,   333,    23,    24,    25,    26,    27,    28,
      29,    30,   319,   320,   321,   322,   323,   324,   325,   326,
     327,   272,   331,   331,   331,   331,   333,   331,  1038,  1039,
     293,  1041,  1042,  1043,   331,  1037,  1046,   331,  1048,   331,
    1050,   331,  1052,  1045,     3,  1047,   331,     3,   830,  1051,
      36,  1053,   834,   209,   260,   329,   105,     3,   230,   328,
     238,   328,     3,    18,   330,     3,   848,    11,    12,    13,
      14,    15,    16,   855,    18,    19,    20,    21,    22,    23,
      24,    25,    26,    27,    28,    29,    30,     3,     3,   281,
     319,   320,   321,   322,   323,   324,   325,   326,   327,   329,
       3,     3,   331,   330,   333,    97,     3,   889,     3,   329,
     156,     3,     3,   895,     3,   260,   219,   899,    12,    13,
      14,    15,    16,     3,    18,    19,    20,    21,    22,    23,
      24,    25,    26,    27,    28,    29,    30,  1089,   920,   329,
     329,   319,   320,   321,   322,   323,   324,   325,   326,   327,
     329,     3,   329,   331,   105,   333,   938,     8,  1160,  1161,
     329,  1163,    20,   273,   319,   320,   321,   322,   323,   324,
     325,   326,   327,     8,   331,     3,   331,     3,   333,     3,
       4,     5,     6,     7,     8,     3,   189,     3,     8,     4,
       3,    14,   105,    17,    18,   331,   192,    38,     3,   269,
      24,    25,    26,   329,   260,   329,     3,     3,   331,    97,
       3,     4,     5,     6,     7,     8,    47,     3,   230,    43,
      26,     3,     3,     8,    17,    18,    50,   329,   332,  1011,
    1012,    24,    25,   330,   330,   159,   330,   159,   331,    63,
      64,    65,     3,     3,  1026,  1027,     3,     3,   330,     8,
      43,   330,   330,     8,   282,   332,   274,    50,     3,    30,
     332,     3,     4,     5,     6,     7,     8,   332,   332,   332,
      63,    64,    65,    97,     3,    17,    18,   331,   331,     3,
     331,   331,    24,    25,   127,   163,     3,   332,     3,     4,
       5,     6,     7,     8,   332,     3,   115,  1079,  1080,     3,
    1082,    43,    17,    18,    97,   267,   176,     3,    50,    24,
      25,    21,    22,    23,    24,    25,    26,    27,    28,    29,
      30,    63,    64,    65,     4,     4,   172,   329,    43,   192,
       3,    47,    55,   330,   329,    50,    78,    20,    21,    22,
      23,    24,    25,    26,    27,    28,    29,    30,    63,    64,
      65,   161,   156,   239,  1136,    97,  1138,  1139,  1140,  1141,
      26,    26,     3,    78,     3,     4,     5,     6,     7,     8,
       3,  1153,   329,   242,   219,   271,    26,   219,    17,    18,
      26,     3,    97,   331,   124,    24,    25,     3,     4,     5,
       6,     7,     8,   332,   127,   332,   189,   332,   332,   127,
       3,    17,    18,   156,    43,    33,    34,   257,    24,    25,
     329,    50,   139,     3,     3,     4,     5,     6,     7,     8,
       3,   128,   128,     3,    63,    64,    65,    43,    17,    18,
       3,     3,     3,    61,    50,    24,    25,     4,     3,    78,
       3,    47,   332,   219,   219,   219,    20,    63,    64,    65,
     331,   156,    80,     3,    43,     3,     3,     8,    97,   108,
      88,    50,    78,    22,    23,    24,    25,    26,    27,    28,
      29,    30,     4,   105,    63,    64,    65,   172,   105,   107,
     105,    97,     3,   332,   332,   332,     3,     3,   116,     3,
     128,     3,   332,   121,   332,   319,   320,   321,   322,   323,
     324,   325,   326,   327,   157,   332,   331,   331,    97,   333,
      19,    20,    21,    22,    23,    24,    25,    26,    27,    28,
      29,    30,   330,   332,   331,   219,   319,   320,   321,   322,
     323,   324,   325,   326,   327,   329,   331,    47,   331,   109,
     333,   332,   332,    20,   331,    43,    20,   175,   331,   156,
       3,   331,   331,     3,     4,     5,     6,     7,     8,   331,
     128,   189,   331,   331,   331,   331,    20,    17,    18,   331,
       3,     3,     3,     3,    24,    25,     5,   319,   320,   321,
     322,   323,   324,   325,   326,   327,     4,   227,     5,   331,
       5,   333,     5,    43,   332,   332,   330,   329,   241,   227,
      50,    20,   332,     4,   319,   320,   321,   322,   323,   324,
     325,   326,   327,    63,    64,    65,   331,   154,   333,   332,
      10,    11,    12,    13,    14,    15,    16,   128,    18,    19,
      20,    21,    22,    23,    24,    25,    26,    27,    28,    29,
      30,   332,   332,   332,   332,   165,   159,    97,    10,    11,
      12,    13,    14,    15,    16,     3,    18,    19,    20,    21,
      22,    23,    24,    25,    26,    27,    28,    29,    30,   125,
     185,     5,     4,   156,   329,   332,   332,     3,  1133,   786,
     319,   320,   321,   322,   323,   324,   325,   326,   327,   942,
     660,   749,   331,   151,   333,   815,   260,  1082,     9,   981,
    1063,   776,  1134,   319,   320,   321,   322,   323,   324,   325,
     326,   327,   102,   410,    22,   331,   451,   333,   226,   496,
      19,    19,   280,   754,   633,   282,   427,   492,   615,   321,
     319,   320,   321,   322,   323,   324,   325,   326,   327,   982,
     713,   332,   331,    -1,   333,    10,    11,    12,    13,    14,
      15,    16,    -1,    18,    19,    20,    21,    22,    23,    24,
      25,    26,    27,    28,    29,    30,    10,    11,    12,    13,
      14,    15,    16,    -1,    18,    19,    20,    21,    22,    23,
      24,    25,    26,    27,    28,    29,    30,    10,    11,    12,
      13,    14,    15,    16,  1049,    18,    19,    20,    21,    22,
      23,    24,    25,    26,    27,    28,    29,    30,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,     3,
      -1,    -1,    -1,    -1,    -1,    -1,    10,    11,    12,    13,
      14,    15,    16,    -1,    18,    19,    20,    21,    22,    23,
      24,    25,    26,    27,    28,    29,    30,    -1,    -1,    -1,
      -1,    -1,    36,    -1,    38,    39,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   319,
     320,   321,   322,   323,   324,   325,   326,   327,    62,    -1,
      -1,   331,    -1,   333,    -1,    -1,    -1,    -1,    72,    73,
      74,    75,    -1,    -1,    -1,    -1,    -1,    81,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    91,    -1,    -1,
      94,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   102,    -1,
      -1,   105,    -1,    -1,   108,   109,    -1,   111,   112,   113,
      -1,    -1,    -1,    -1,   118,    -1,    -1,    -1,    -1,    -1,
      -1,   125,   332,   127,    -1,    -1,    -1,    -1,    -1,   133,
     134,    -1,   946,   947,    -1,   949,    -1,    -1,   952,   953,
      -1,   955,   956,   957,    -1,    -1,    -1,   961,   152,   963,
     332,   965,   156,    -1,    -1,    -1,   970,   161,    10,    11,
      12,    13,    14,    15,    16,    -1,    18,    19,    20,    21,
      22,    23,    24,    25,    26,    27,    28,    29,    30,    -1,
     184,    10,    11,    12,    13,    14,    15,    16,   192,    18,
      19,    20,    21,    22,    23,    24,    25,    26,    27,    28,
      29,    30,    -1,    -1,   208,    -1,    -1,    -1,    -1,   213,
      10,    11,    12,    13,    14,    15,    16,    -1,    18,    19,
      20,    21,    22,    23,    24,    25,    26,    27,    28,    29,
      30,    -1,    -1,    -1,   238,   239,    -1,   241,    -1,   243,
     244,    -1,    -1,    -1,    -1,    -1,    -1,   332,    -1,    -1,
      13,    14,    15,    16,  1068,    18,    19,    20,    21,    22,
      23,    24,    25,    26,    27,    28,    29,    30,   332,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   105,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   332,
      -1,    -1,    -1,   297,    -1,    -1,    -1,    -1,    -1,   303,
      10,    11,    12,    13,    14,    15,    16,    -1,    18,    19,
      20,    21,    22,    23,    24,    25,    26,    27,    28,    29,
      30,    -1,    -1,    -1,   328,   329,    -1,    -1,   332,    10,
      11,    12,    13,    14,    15,    16,    -1,    18,    19,    20,
      21,    22,    23,    24,    25,    26,    27,    28,    29,    30,
      10,    11,    12,    13,    14,    15,    16,    -1,    18,    19,
      20,    21,    22,    23,    24,    25,    26,    27,    28,    29,
      30,    -1,     0,     1,    10,    11,    12,    13,    14,    15,
      16,    -1,    18,    19,    20,    21,    22,    23,    24,    25,
      26,    27,    28,    29,    30,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    34,    35,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    33,    34,    34,    -1,    53,    10,    11,    12,    13,
      14,    15,    16,    61,    18,    19,    20,    21,    22,    23,
      24,    25,    26,    27,    28,    29,    30,    -1,    -1,    61,
      61,    -1,    80,    81,    82,    -1,    -1,    -1,    -1,    -1,
      88,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    80,    80,
      -1,    -1,    -1,    -1,    -1,    -1,    88,    88,    -1,   107,
     332,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   121,    -1,   107,   107,    -1,    -1,    -1,
     329,    -1,   130,    -1,   116,   116,    -1,    -1,   136,   121,
     121,   139,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   329,
     158,    -1,    -1,     1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   176,   177,
      -1,   179,    -1,    -1,    -1,   183,    -1,    -1,    -1,    -1,
      -1,   189,    -1,   175,   192,   193,    34,    35,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   189,   189,    -1,
      -1,    -1,    -1,    -1,    -1,    53,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    61,    -1,    -1,    -1,   225,    -1,   227,
      -1,   229,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    80,    81,    82,   227,   227,    -1,   246,   329,
      88,    -1,    -1,    -1,   252,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   260,    -1,   262,   263,   264,    -1,    -1,   107,
      -1,   269,   270,    -1,    -1,    -1,    -1,    -1,   329,    -1,
      -1,    -1,    -1,   121,   238,    -1,   284,   285,   286,    -1,
     288,   297,   130,    -1,    -1,    -1,    -1,   303,   136,   329,
      -1,   139,    -1,    -1,    -1,    -1,   304,    -1,    -1,    -1,
      -1,   309,   310,   311,   312,    -1,    13,    14,    15,    16,
     158,    18,    19,    20,    21,    22,    23,    24,    25,    26,
      27,    28,    29,    30,    34,    35,    -1,    -1,   176,   177,
      -1,   179,    -1,    -1,    -1,   183,    -1,    -1,    -1,    -1,
      -1,   189,    -1,    53,   192,   193,    -1,    -1,    -1,    -1,
      -1,    61,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      80,    81,    82,    -1,    -1,    -1,    -1,   225,    88,   227,
      -1,   229,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   107,   246,    -1,
      -1,    -1,    -1,    -1,   252,    -1,    -1,    -1,    -1,    -1,
      -1,   121,   260,    -1,   262,   263,   264,    -1,    -1,    -1,
     130,   269,   270,    -1,    -1,    -1,   136,    -1,    -1,   139,
      -1,    -1,    -1,    -1,    -1,    -1,   284,   285,   286,    -1,
     288,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   158,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   304,    -1,    -1,    -1,
      -1,   309,   310,   311,   312,    -1,   176,   177,    -1,   179,
      -1,    -1,    -1,   183,    -1,    -1,    -1,    -1,    -1,   189,
      -1,    -1,   192,   193,    10,    11,    12,    13,    14,    15,
      16,    -1,    18,    19,    20,    21,    22,    23,    24,    25,
      26,    27,    28,    29,    30,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   225,    -1,   227,    -1,   229,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   246,    -1,    -1,    -1,
      -1,    -1,   252,    -1,    -1,    -1,    72,    73,    74,    75,
     260,    -1,   262,   263,   264,    -1,    -1,    -1,    -1,   269,
     270,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   284,   285,   286,    -1,   288,    -1,
      -1,    -1,    -1,    -1,    -1,   111,   112,   113,    -1,    -1,
      -1,    -1,    -1,    -1,   304,    -1,    -1,    -1,    -1,   309,
     310,   311,   312,    10,    11,    12,    13,    14,    15,    16,
      -1,    18,    19,    20,    21,    22,    23,    24,    25,    26,
      27,    28,    29,    30,    10,    11,    12,    13,    14,    15,
      16,    -1,    18,    19,    20,    21,    22,    23,    24,    25,
      26,    27,    28,    29,    30,    42,    43,    44,    45,    -1,
      -1,    -1,    -1,    -1,    -1,    52,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    70,    71,    -1,    -1,    -1,    -1,    76,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      87,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    95,    -1,
      -1,    -1,    -1,    -1,   101,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   243,   244,    -1,
      -1,    -1,    -1,    -1,    -1,   122,   123,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   141,   142,    -1,    -1,    -1,   146,
     147,   148,    10,    11,    12,    13,    14,    15,    16,    -1,
      18,    19,    20,    21,    22,    23,    24,    25,    26,    27,
      28,    29,    30,    -1,    -1,    -1,    -1,   174,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   192,    -1,   194,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   213,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   211,    -1,    -1,   214,   215,   216,
     217,   218,    -1,    -1,    -1,    -1,    -1,   213,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   235,   236,
      -1,    -1,    -1,    -1,    -1,    -1,   243,   105,    10,    11,
      12,    13,    14,    15,    16,    -1,    18,    19,    20,    21,
      22,    23,    24,    25,    26,    27,    28,    29,    30,    10,
      11,    12,    13,    14,    15,    16,    -1,    18,    19,    20,
      21,    22,    23,    24,    25,    26,    27,    28,    29,    30,
      10,    11,    12,    13,    14,    15,    16,    -1,    18,    19,
      20,    21,    22,    23,    24,    25,    26,    27,    28,    29,
      30,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    39,
      -1,    -1,    -1,    -1,    -1,    10,    11,    12,    13,    14,
      15,    16,    94,    18,    19,    20,    21,    22,    23,    24,
      25,    26,    27,    28,    29,    30,    -1,     3,    -1,    -1,
      -1,    36,    -1,    94,    10,    11,    12,    13,    14,    15,
      16,    81,    18,    19,    20,    21,    22,    23,    24,    25,
      26,    27,    28,    29,    30,    10,    11,    12,    13,    14,
      15,    16,    38,    18,    19,    20,    21,    22,    23,    24,
      25,    26,    27,    28,    29,    30
};

/* YYSTOS[STATE-NUM] -- The (internal number of the) accessing
   symbol of state STATE-NUM.  */
static const unsigned short int yystos[] =
{
       0,     1,    34,    35,    53,    61,    80,    81,    82,    88,
     107,   121,   130,   136,   139,   158,   176,   177,   179,   183,
     189,   192,   193,   225,   227,   229,   246,   252,   260,   262,
     263,   264,   269,   270,   284,   285,   286,   288,   304,   309,
     310,   311,   312,   335,   336,   337,   365,   369,   377,   378,
     381,   383,   390,   391,   392,   393,   398,   404,   405,   407,
     410,   412,   413,   414,   416,   419,   420,   421,   422,   423,
     424,   428,   429,   432,   435,   437,   442,   443,   446,   447,
     448,   449,   451,   453,   455,   456,   458,   460,   461,   463,
     464,   470,   474,   477,   479,   482,   485,   488,   328,    68,
     115,   186,   397,   153,   271,   454,   209,    68,   106,   169,
     186,   197,   210,   224,   251,   386,   399,   366,     3,     3,
      68,   116,   186,   251,   310,   386,    33,    34,    61,    80,
      88,   107,   116,   121,   175,   189,   227,   438,   439,   444,
     445,   371,   259,   308,   478,   116,   272,   454,   269,   209,
     251,   371,   438,   444,   348,     8,   268,   275,   276,   282,
     430,   489,   490,     3,     4,     5,     6,     7,     8,    17,
      18,    24,    25,    43,    50,    61,    63,    64,    65,    68,
      97,   116,   186,   209,   283,   289,   290,   292,   295,   297,
     298,   299,   301,   303,   316,   317,   318,   319,   320,   321,
     322,   323,   324,   325,   326,   327,   331,   333,   430,   465,
     469,   487,   272,   371,   379,     3,   487,   497,     3,   209,
     116,   313,   415,   415,   265,     3,   415,   209,   209,   454,
     209,   454,   258,   259,   261,   483,   484,     3,   310,     3,
       0,     1,   336,   328,     3,     3,   209,   209,     3,   408,
     114,   382,   156,   382,     8,   433,   434,   209,   116,     3,
     105,   115,   144,   171,   367,   114,   406,     3,   406,     8,
     436,     3,   209,   283,   159,   331,   373,   373,   373,   156,
     329,   156,   329,    79,   110,   115,   125,   144,   372,   487,
     125,     3,   425,   426,   209,     3,     3,   411,     8,   450,
     372,   156,   156,    26,    33,    84,    85,   110,   203,   204,
     205,   208,   349,   350,   487,     9,    20,    20,   102,   452,
     265,   329,   330,   331,     9,   487,    97,   487,   487,   487,
     487,   238,   487,   501,    68,   186,   209,     3,   331,   105,
       3,   294,   192,    14,   467,     3,   296,   134,   345,   102,
     294,   105,   466,   345,   272,   331,   331,   331,   331,   331,
     331,   331,   331,   331,   487,   294,   302,   272,   291,   300,
     293,    10,    11,    12,    13,    14,    15,    16,    18,    19,
      20,    21,    22,    23,    24,    25,    26,    27,    28,    29,
      30,   297,   303,   115,   144,     3,   331,   351,   352,   353,
     356,   364,   329,     3,     3,   471,   472,    36,   417,   219,
     417,   408,   408,   209,   408,   209,   259,   272,   283,   293,
     294,   305,   306,   307,   475,   476,   260,   329,   105,     3,
     230,   486,   328,   328,   494,   494,     3,   408,   330,   171,
     253,   254,   255,   256,   329,   457,    18,     3,     3,     3,
     281,   329,   382,     3,   330,   368,     3,   367,   105,   329,
      97,     3,   156,     3,   329,   406,     3,   347,     3,    26,
      34,    61,    80,    88,   107,   116,   121,   189,   227,   439,
       3,    34,    61,    80,    88,   107,   116,   121,   189,   227,
     445,     3,   260,     3,    38,   355,   329,   408,   219,   330,
     329,   219,   329,     3,     3,    26,     3,   329,   105,   329,
     355,   487,   487,   487,     8,    20,   273,   490,     3,    26,
     497,   498,   487,   331,   487,   501,    91,    94,   238,     3,
       3,     3,   337,     3,   466,   467,     4,   293,   294,   487,
       8,     3,    14,   468,   466,   487,   497,    46,   131,   220,
     497,   499,   487,   487,    26,   487,   487,   487,   487,   487,
     332,   467,   467,   466,   105,   487,   487,   487,   487,   487,
       6,    18,   154,   331,    13,    14,    16,   487,    33,    37,
     195,   331,   487,   487,   487,   487,   487,   487,   487,   487,
     487,   487,   487,   330,   355,   337,   351,   192,    62,   118,
     133,   152,   184,   208,   329,   357,   359,    38,   354,   497,
     116,   128,   473,   480,    16,   329,   266,   267,   176,   266,
     418,   269,   418,   219,   171,   255,   459,   408,   105,     3,
     260,   241,   408,   329,   484,   336,   436,    52,    54,    32,
      51,    88,   161,   177,   394,   395,     3,     3,    97,   331,
      47,   434,     3,   230,   400,    26,   239,   338,   230,   351,
       3,     3,     8,   408,   329,   332,   330,   330,   159,   330,
     159,   192,   373,   471,     3,   144,   172,   242,   427,   426,
       3,     3,     3,     8,     8,   192,   373,   330,   330,   330,
     349,   351,   349,   282,   274,   332,   337,   213,    91,    94,
     487,   487,   332,   466,   467,   329,   487,   467,   105,   332,
     332,   487,   329,   329,   332,   332,   332,   332,   332,   332,
     467,     3,     6,   154,   337,   497,   487,   487,   331,    36,
     331,   331,   331,   337,     3,   332,   332,     3,   380,   133,
     184,   360,   353,   351,   127,   163,   358,     3,     3,   331,
     363,   115,   481,     3,     3,   472,   267,   176,     3,     4,
     462,     4,   172,   476,   192,     3,    55,   396,   396,   104,
     116,   167,   396,    47,     3,   219,   329,   330,   347,     4,
     282,   115,   179,   330,   331,   384,   385,   314,   315,   401,
     156,   487,   161,   344,   351,   338,   368,    49,   181,   409,
     347,    26,    26,     3,     3,   376,   234,   337,   242,   271,
     219,   219,   330,   219,   376,   234,   337,    26,    26,     3,
     338,   331,   172,   279,   280,   431,   332,   487,   487,    94,
     213,   487,   487,   105,   124,   500,   500,   466,   332,   332,
     337,   497,   487,   337,   337,   337,   332,   355,    20,   330,
     329,   338,   358,   358,   127,   156,   353,   127,   363,   329,
     257,   473,   171,   255,   287,   139,     3,     3,   106,   116,
     128,   167,   387,   388,     3,   128,   347,   128,     3,     3,
       3,   395,     3,   332,     4,     3,   387,   354,     3,    47,
     345,   338,   219,   219,   219,    20,   156,   329,   370,   331,
     374,   370,     3,     3,     3,     8,   370,   374,   370,   105,
     105,   105,   108,   339,     4,   277,   278,   172,    94,   487,
     102,   332,   497,   487,   332,   332,   467,   332,   332,   332,
     332,   332,   487,     3,     3,   344,   353,   487,   156,   230,
     361,   362,   353,   332,     3,    42,    43,    44,    45,    52,
      70,    71,    76,    87,    95,   101,   122,   123,   141,   142,
     146,   147,   148,   174,   192,   194,   211,   214,   215,   216,
     217,   218,   235,   236,   243,   495,   116,   128,   331,   331,
     128,   329,     3,   347,   155,   165,   331,   384,   332,   337,
     331,   340,   487,   433,   433,   433,    78,   487,   157,     3,
      78,   375,   487,   329,   370,   330,   219,   370,   436,   436,
     436,    47,   109,   343,   332,   487,   332,    72,    73,    74,
      75,   111,   112,   113,   243,   244,    20,    20,   330,   345,
     487,   331,   362,   331,   491,   491,   491,   491,   491,   491,
     331,   491,   491,   491,    43,   492,   491,   492,   491,   331,
     491,   492,   491,   492,   331,   331,   389,   331,   331,   347,
     347,   331,   388,   495,    20,   156,   387,   384,     3,   402,
     403,    39,    81,   341,   241,   440,   440,   440,   128,    20,
     329,   332,   331,     3,     3,   440,   440,   340,   487,   344,
     332,   487,   487,     3,   347,     5,   493,   494,   493,   493,
       4,   496,   493,   493,   493,   494,   493,   494,   493,   496,
     493,   494,   493,   494,     5,     5,    18,    40,    56,    78,
     128,   154,   167,   224,   347,   347,   332,   332,   347,   389,
       5,     3,   332,   491,   329,   332,   329,   107,   247,   248,
     249,   250,   441,   227,    78,   487,    78,   487,   375,   330,
     241,   342,   345,    20,   332,   329,   332,   226,   245,   329,
     332,   332,   332,   332,   154,     4,     4,     5,     6,     7,
     128,   128,   331,   332,   332,   332,   165,   384,   341,   403,
     340,   159,   487,   487,   487,   487,   376,   332,     3,   185,
     125,   346,   487,     5,     4,   494,   494,   494,   347,   156,
     347,   332,   332,     3
};


/* Prevent warning if -Wmissing-prototypes.  */
int yyparse (void);

/* Error token number */
#define YYTERROR 1

/* YYLLOC_DEFAULT -- Set CURRENT to span from RHS[1] to RHS[N].
   If N is 0, then set CURRENT to the empty location which ends
   the previous symbol: RHS[0] (always defined).  */


#define YYRHSLOC(Rhs, K) ((Rhs)[K].yystate.yyloc)
#ifndef YYLLOC_DEFAULT
# define YYLLOC_DEFAULT(Current, Rhs, N)				\
    do									\
      if (YYID (N))							\
	{								\
	  (Current).first_line   = YYRHSLOC (Rhs, 1).first_line;	\
	  (Current).first_column = YYRHSLOC (Rhs, 1).first_column;	\
	  (Current).last_line    = YYRHSLOC (Rhs, N).last_line;		\
	  (Current).last_column  = YYRHSLOC (Rhs, N).last_column;	\
	}								\
      else								\
	{								\
	  (Current).first_line   = (Current).last_line   =		\
	    YYRHSLOC (Rhs, 0).last_line;				\
	  (Current).first_column = (Current).last_column =		\
	    YYRHSLOC (Rhs, 0).last_column;				\
	}								\
    while (YYID (0))

/* YY_LOCATION_PRINT -- Print the location on the stream.
   This macro was not mandated originally: define only if we know
   we won't break user code: when these are the locations we know.  */

# define YY_LOCATION_PRINT(File, Loc)			\
    fprintf (File, "%d.%d-%d.%d",			\
	     (Loc).first_line, (Loc).first_column,	\
	     (Loc).last_line,  (Loc).last_column)
#endif


#ifndef YY_LOCATION_PRINT
# define YY_LOCATION_PRINT(File, Loc) ((void) 0)
#endif


/* YYLEX -- calling `yylex' with the right arguments.  */
#define YYLEX yylex ()

YYSTYPE yylval;

YYLTYPE yylloc;

int yynerrs;
int yychar;

static const int YYEOF = 0;
static const int YYEMPTY = -2;

typedef enum { yyok, yyaccept, yyabort, yyerr } YYRESULTTAG;

#define YYCHK(YYE)							     \
   do { YYRESULTTAG yyflag = YYE; if (yyflag != yyok) return yyflag; }	     \
   while (YYID (0))

#if YYDEBUG

# ifndef YYFPRINTF
#  define YYFPRINTF fprintf
# endif

# define YYDPRINTF(Args)			\
do {						\
  if (yydebug)					\
    YYFPRINTF Args;				\
} while (YYID (0))


/*--------------------------------.
| Print this symbol on YYOUTPUT.  |
`--------------------------------*/

/*ARGSUSED*/
static void
yy_symbol_value_print (FILE *yyoutput, int yytype, YYSTYPE const * const yyvaluep, YYLTYPE const * const yylocationp)
{
  if (!yyvaluep)
    return;
  YYUSE (yylocationp);
# ifdef YYPRINT
  if (yytype < YYNTOKENS)
    YYPRINT (yyoutput, yytoknum[yytype], *yyvaluep);
# else
  YYUSE (yyoutput);
# endif
  switch (yytype)
    {
      default:
	break;
    }
}


/*--------------------------------.
| Print this symbol on YYOUTPUT.  |
`--------------------------------*/

static void
yy_symbol_print (FILE *yyoutput, int yytype, YYSTYPE const * const yyvaluep, YYLTYPE const * const yylocationp)
{
  if (yytype < YYNTOKENS)
    YYFPRINTF (yyoutput, "token %s (", yytname[yytype]);
  else
    YYFPRINTF (yyoutput, "nterm %s (", yytname[yytype]);

  YY_LOCATION_PRINT (yyoutput, *yylocationp);
  YYFPRINTF (yyoutput, ": ");
  yy_symbol_value_print (yyoutput, yytype, yyvaluep, yylocationp);
  YYFPRINTF (yyoutput, ")");
}

# define YY_SYMBOL_PRINT(Title, Type, Value, Location)			    \
do {									    \
  if (yydebug)								    \
    {									    \
      YYFPRINTF (stderr, "%s ", Title);					    \
      yy_symbol_print (stderr, Type,					    \
		       Value, Location);  \
      YYFPRINTF (stderr, "\n");						    \
    }									    \
} while (YYID (0))

/* Nonzero means print parse trace.  It is left uninitialized so that
   multiple parsers can coexist.  */
int yydebug;

#else /* !YYDEBUG */

# define YYDPRINTF(Args)
# define YY_SYMBOL_PRINT(Title, Type, Value, Location)

#endif /* !YYDEBUG */

/* YYINITDEPTH -- initial size of the parser's stacks.  */
#ifndef	YYINITDEPTH
# define YYINITDEPTH 200
#endif

/* YYMAXDEPTH -- maximum size the stacks can grow to (effective only
   if the built-in stack extension method is used).

   Do not make this value too large; the results are undefined if
   SIZE_MAX < YYMAXDEPTH * sizeof (GLRStackItem)
   evaluated with infinite-precision integer arithmetic.  */

#ifndef YYMAXDEPTH
# define YYMAXDEPTH 10000
#endif

/* Minimum number of free items on the stack allowed after an
   allocation.  This is to allow allocation and initialization
   to be completed by functions that call yyexpandGLRStack before the
   stack is expanded, thus insuring that all necessary pointers get
   properly redirected to new data.  */
#define YYHEADROOM 2

#ifndef YYSTACKEXPANDABLE
# if (! defined __cplusplus \
      || (defined YYLTYPE_IS_TRIVIAL && YYLTYPE_IS_TRIVIAL \
	  && defined YYSTYPE_IS_TRIVIAL && YYSTYPE_IS_TRIVIAL))
#  define YYSTACKEXPANDABLE 1
# else
#  define YYSTACKEXPANDABLE 0
# endif
#endif

#if YYSTACKEXPANDABLE
# define YY_RESERVE_GLRSTACK(Yystack)			\
  do {							\
    if (Yystack->yyspaceLeft < YYHEADROOM)		\
      yyexpandGLRStack (Yystack);			\
  } while (YYID (0))
#else
# define YY_RESERVE_GLRSTACK(Yystack)			\
  do {							\
    if (Yystack->yyspaceLeft < YYHEADROOM)		\
      yyMemoryExhausted (Yystack);			\
  } while (YYID (0))
#endif


#if YYERROR_VERBOSE

# ifndef yystpcpy
#  if defined __GLIBC__ && defined _STRING_H && defined _GNU_SOURCE
#   define yystpcpy stpcpy
#  else
/* Copy YYSRC to YYDEST, returning the address of the terminating '\0' in
   YYDEST.  */
static char *
yystpcpy (char *yydest, const char *yysrc)
{
  char *yyd = yydest;
  const char *yys = yysrc;

  while ((*yyd++ = *yys++) != '\0')
    continue;

  return yyd - 1;
}
#  endif
# endif

# ifndef yytnamerr
/* Copy to YYRES the contents of YYSTR after stripping away unnecessary
   quotes and backslashes, so that it's suitable for yyerror.  The
   heuristic is that double-quoting is unnecessary unless the string
   contains an apostrophe, a comma, or backslash (other than
   backslash-backslash).  YYSTR is taken from yytname.  If YYRES is
   null, do not copy; instead, return the length of what the result
   would have been.  */
static size_t
yytnamerr (char *yyres, const char *yystr)
{
  if (*yystr == '"')
    {
      size_t yyn = 0;
      char const *yyp = yystr;

      for (;;)
	switch (*++yyp)
	  {
	  case '\'':
	  case ',':
	    goto do_not_strip_quotes;

	  case '\\':
	    if (*++yyp != '\\')
	      goto do_not_strip_quotes;
	    /* Fall through.  */
	  default:
	    if (yyres)
	      yyres[yyn] = *yyp;
	    yyn++;
	    break;

	  case '"':
	    if (yyres)
	      yyres[yyn] = '\0';
	    return yyn;
	  }
    do_not_strip_quotes: ;
    }

  if (! yyres)
    return strlen (yystr);

  return yystpcpy (yyres, yystr) - yyres;
}
# endif

#endif /* !YYERROR_VERBOSE */

/** State numbers, as in LALR(1) machine */
typedef int yyStateNum;

/** Rule numbers, as in LALR(1) machine */
typedef int yyRuleNum;

/** Grammar symbol */
typedef short int yySymbol;

/** Item references, as in LALR(1) machine */
typedef short int yyItemNum;

typedef struct yyGLRState yyGLRState;
typedef struct yyGLRStateSet yyGLRStateSet;
typedef struct yySemanticOption yySemanticOption;
typedef union yyGLRStackItem yyGLRStackItem;
typedef struct yyGLRStack yyGLRStack;

struct yyGLRState {
  /** Type tag: always true.  */
  yybool yyisState;
  /** Type tag for yysemantics.  If true, yysval applies, otherwise
   *  yyfirstVal applies.  */
  yybool yyresolved;
  /** Number of corresponding LALR(1) machine state.  */
  yyStateNum yylrState;
  /** Preceding state in this stack */
  yyGLRState* yypred;
  /** Source position of the first token produced by my symbol */
  size_t yyposn;
  union {
    /** First in a chain of alternative reductions producing the
     *  non-terminal corresponding to this state, threaded through
     *  yynext.  */
    yySemanticOption* yyfirstVal;
    /** Semantic value for this state.  */
    YYSTYPE yysval;
  } yysemantics;
  /** Source location for this state.  */
  YYLTYPE yyloc;
};

struct yyGLRStateSet {
  yyGLRState** yystates;
  /** During nondeterministic operation, yylookaheadNeeds tracks which
   *  stacks have actually needed the current lookahead.  During deterministic
   *  operation, yylookaheadNeeds[0] is not maintained since it would merely
   *  duplicate yychar != YYEMPTY.  */
  yybool* yylookaheadNeeds;
  size_t yysize, yycapacity;
};

struct yySemanticOption {
  /** Type tag: always false.  */
  yybool yyisState;
  /** Rule number for this reduction */
  yyRuleNum yyrule;
  /** The last RHS state in the list of states to be reduced.  */
  yyGLRState* yystate;
  /** The lookahead for this reduction.  */
  int yyrawchar;
  YYSTYPE yyval;
  YYLTYPE yyloc;
  /** Next sibling in chain of options.  To facilitate merging,
   *  options are chained in decreasing order by address.  */
  yySemanticOption* yynext;
};

/** Type of the items in the GLR stack.  The yyisState field
 *  indicates which item of the union is valid.  */
union yyGLRStackItem {
  yyGLRState yystate;
  yySemanticOption yyoption;
};

struct yyGLRStack {
  int yyerrState;
  /* To compute the location of the error token.  */
  yyGLRStackItem yyerror_range[3];

  YYJMP_BUF yyexception_buffer;
  yyGLRStackItem* yyitems;
  yyGLRStackItem* yynextFree;
  size_t yyspaceLeft;
  yyGLRState* yysplitPoint;
  yyGLRState* yylastDeleted;
  yyGLRStateSet yytops;
};

#if YYSTACKEXPANDABLE
static void yyexpandGLRStack (yyGLRStack* yystackp);
#endif

static void yyFail (yyGLRStack* yystackp, const char* yymsg)
  __attribute__ ((__noreturn__));
static void
yyFail (yyGLRStack* yystackp, const char* yymsg)
{
  if (yymsg != NULL)
    yyerror (yymsg);
  YYLONGJMP (yystackp->yyexception_buffer, 1);
}

static void yyMemoryExhausted (yyGLRStack* yystackp)
  __attribute__ ((__noreturn__));
static void
yyMemoryExhausted (yyGLRStack* yystackp)
{
  YYLONGJMP (yystackp->yyexception_buffer, 2);
}

#if YYDEBUG || YYERROR_VERBOSE
/** A printable representation of TOKEN.  */
static inline const char*
yytokenName (yySymbol yytoken)
{
  if (yytoken == YYEMPTY)
    return "";

  return yytname[yytoken];
}
#endif

/** Fill in YYVSP[YYLOW1 .. YYLOW0-1] from the chain of states starting
 *  at YYVSP[YYLOW0].yystate.yypred.  Leaves YYVSP[YYLOW1].yystate.yypred
 *  containing the pointer to the next state in the chain.  */
static void yyfillin (yyGLRStackItem *, int, int) __attribute__ ((__unused__));
static void
yyfillin (yyGLRStackItem *yyvsp, int yylow0, int yylow1)
{
  yyGLRState* s;
  int i;
  s = yyvsp[yylow0].yystate.yypred;
  for (i = yylow0-1; i >= yylow1; i -= 1)
    {
      YYASSERT (s->yyresolved);
      yyvsp[i].yystate.yyresolved = yytrue;
      yyvsp[i].yystate.yysemantics.yysval = s->yysemantics.yysval;
      yyvsp[i].yystate.yyloc = s->yyloc;
      s = yyvsp[i].yystate.yypred = s->yypred;
    }
}

/* Do nothing if YYNORMAL or if *YYLOW <= YYLOW1.  Otherwise, fill in
 * YYVSP[YYLOW1 .. *YYLOW-1] as in yyfillin and set *YYLOW = YYLOW1.
 * For convenience, always return YYLOW1.  */
static inline int yyfill (yyGLRStackItem *, int *, int, yybool)
     __attribute__ ((__unused__));
static inline int
yyfill (yyGLRStackItem *yyvsp, int *yylow, int yylow1, yybool yynormal)
{
  if (!yynormal && yylow1 < *yylow)
    {
      yyfillin (yyvsp, *yylow, yylow1);
      *yylow = yylow1;
    }
  return yylow1;
}

/** Perform user action for rule number YYN, with RHS length YYRHSLEN,
 *  and top stack item YYVSP.  YYLVALP points to place to put semantic
 *  value ($$), and yylocp points to place for location information
 *  (@$).  Returns yyok for normal return, yyaccept for YYACCEPT,
 *  yyerr for YYERROR, yyabort for YYABORT.  */
/*ARGSUSED*/ static YYRESULTTAG
yyuserAction (yyRuleNum yyn, int yyrhslen, yyGLRStackItem* yyvsp,
	      YYSTYPE* yyvalp,
	      YYLTYPE* YYOPTIONAL_LOC (yylocp),
	      yyGLRStack* yystackp
	      )
{
  yybool yynormal __attribute__ ((__unused__)) =
    (yystackp->yysplitPoint == NULL);
  int yylow;
# undef yyerrok
# define yyerrok (yystackp->yyerrState = 0)
# undef YYACCEPT
# define YYACCEPT return yyaccept
# undef YYABORT
# define YYABORT return yyabort
# undef YYERROR
# define YYERROR return yyerrok, yyerr
# undef YYRECOVERING
# define YYRECOVERING() (yystackp->yyerrState != 0)
# undef yyclearin
# define yyclearin (yychar = YYEMPTY)
# undef YYFILL
# define YYFILL(N) yyfill (yyvsp, &yylow, N, yynormal)
# undef YYBACKUP
# define YYBACKUP(Token, Value)						     \
  return yyerror (YY_("syntax error: cannot back up")),     \
	 yyerrok, yyerr

  yylow = 1;
  if (yyrhslen == 0)
    *yyvalp = yyval_default;
  else
    *yyvalp = yyvsp[YYFILL (1-yyrhslen)].yystate.yysemantics.yysval;
  YYLLOC_DEFAULT ((*yylocp), (yyvsp - yyrhslen), yyrhslen);
  yystackp->yyerror_range[1].yystate.yyloc = *yylocp;

  switch (yyn)
    {
        case 2:

/* Line 936 of glr.c  */
#line 432 "sql.ypp"
    { 		// 2014-3-4---增加对多条指令的识别支持---by余楷		// 2014-3-6---解析错误后置根结点为空---by余楷
  						printf("> \n"); 
  						((*yyvalp).ast_node) = newStmt(t_stmt, NULL, (((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (2))].yystate.yysemantics.yysval.ast_node)); 
  						if (errorNumber == 0) 
  						{
  							parsetreeroot = ((*yyvalp).ast_node);
  						}
  						else
  						{
  							parsetreeroot = NULL;
  						} 
  						/* output($$, 1);*/ 
  					;}
    break;

  case 3:

/* Line 936 of glr.c  */
#line 445 "sql.ypp"
    { 		// 2014-3-4---增加对多条指令的识别支持---by余楷		// 2014-3-6---解析错误后置根结点为空---by余楷
	  					printf(">> \n"); 
	  					((*yyvalp).ast_node) = newStmt(t_stmt, (((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (3))].yystate.yysemantics.yysval.ast_node), (((yyGLRStackItem const *)yyvsp)[YYFILL ((2) - (3))].yystate.yysemantics.yysval.ast_node)); 
	  					if (errorNumber == 0)
	  					{
	  						parsetreeroot = ((*yyvalp).ast_node);
	  					}
	  					else
	  					{
	  						parsetreeroot = NULL;
	  					}
	  					/*output($$, 1);*/ 
	  				;}
    break;

  case 4:

/* Line 936 of glr.c  */
#line 458 "sql.ypp"
    { 
  						printf("> \n");
  						parsetreeroot = NULL;	// 2014-3-6---解析错误后置根结点为空---by余楷
  						errorNumber++;		// 2014-3-6---错误数加1---by余楷
  						//yyerror("First statement discarded, input new statement"); 
  						yyclearin;
  						yyerrok; 				
  					;}
    break;

  case 5:

/* Line 936 of glr.c  */
#line 466 "sql.ypp"
    { 
  						printf(">> \n"); 
  						parsetreeroot = NULL;	// 2014-3-6---解析错误后置根结点为空---by余楷
  						errorNumber++;		// 2014-3-6---错误数加1---by余楷
  						//yyerror("Current statement discarded, input new statement"); 
  						yyclearin;
  						yyerrok; 
  					;}
    break;

  case 6:

/* Line 936 of glr.c  */
#line 478 "sql.ypp"
    { ((*yyvalp).ast_node)=(((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (1))].yystate.yysemantics.yysval.ast_node); /*output($$, 1);*/ puts("SQL parser： This is a select statement"); ;}
    break;

  case 7:

/* Line 936 of glr.c  */
#line 483 "sql.ypp"
    {
		((*yyvalp).ast_node) = newQueryStmt(t_query_stmt, NULL, (((yyGLRStackItem const *)yyvsp)[YYFILL ((2) - (3))].yystate.yysemantics.yysval.intval), (((yyGLRStackItem const *)yyvsp)[YYFILL ((3) - (3))].yystate.yysemantics.yysval.ast_node), NULL, NULL, NULL, NULL, NULL, NULL, NULL);
		/*puts("SQL parser： select_stmt1");	*/ // 2014-3-5---优化输出信息，指明来自SQL parser---by余楷
	;}
    break;

  case 8:

/* Line 936 of glr.c  */
#line 489 "sql.ypp"
    {
			((*yyvalp).ast_node) = newQueryStmt(t_query_stmt, NULL, (((yyGLRStackItem const *)yyvsp)[YYFILL ((2) - (11))].yystate.yysemantics.yysval.intval), (((yyGLRStackItem const *)yyvsp)[YYFILL ((3) - (11))].yystate.yysemantics.yysval.ast_node), (((yyGLRStackItem const *)yyvsp)[YYFILL ((5) - (11))].yystate.yysemantics.yysval.ast_node), (((yyGLRStackItem const *)yyvsp)[YYFILL ((6) - (11))].yystate.yysemantics.yysval.ast_node), (((yyGLRStackItem const *)yyvsp)[YYFILL ((7) - (11))].yystate.yysemantics.yysval.ast_node), (((yyGLRStackItem const *)yyvsp)[YYFILL ((8) - (11))].yystate.yysemantics.yysval.ast_node), (((yyGLRStackItem const *)yyvsp)[YYFILL ((9) - (11))].yystate.yysemantics.yysval.ast_node), (((yyGLRStackItem const *)yyvsp)[YYFILL ((10) - (11))].yystate.yysemantics.yysval.ast_node), (((yyGLRStackItem const *)yyvsp)[YYFILL ((11) - (11))].yystate.yysemantics.yysval.ast_node));
			/*puts("SQL parser： select_stmt2");	*/ // 2014-3-5---优化输出信息，指明来自SQL parser---by余楷
		;}
    break;

  case 9:

/* Line 936 of glr.c  */
#line 495 "sql.ypp"
    { ((*yyvalp).ast_node)= NULL; /*puts("opt_where is null")*/; ;}
    break;

  case 10:

/* Line 936 of glr.c  */
#line 496 "sql.ypp"
    { ((*yyvalp).ast_node) = newWhereList(t_where_list, NULL, (((yyGLRStackItem const *)yyvsp)[YYFILL ((2) - (2))].yystate.yysemantics.yysval.ast_node)); /*puts("opt_where");*/;}
    break;

  case 11:

/* Line 936 of glr.c  */
#line 498 "sql.ypp"
    { ((*yyvalp).ast_node)=NULL; /*puts("opt_groupby is null");*/ ;}
    break;

  case 12:

/* Line 936 of glr.c  */
#line 499 "sql.ypp"
    { ((*yyvalp).ast_node) = newGroupbyList(t_groupby_list, NULL, (((yyGLRStackItem const *)yyvsp)[YYFILL ((3) - (4))].yystate.yysemantics.yysval.ast_node), (((yyGLRStackItem const *)yyvsp)[YYFILL ((4) - (4))].yystate.yysemantics.yysval.intval)); /*puts("opt_groupby");*/;}
    break;

  case 13:

/* Line 936 of glr.c  */
#line 503 "sql.ypp"
    { 
		((*yyvalp).ast_node)=newGroupbyExpr(t_groupby_expr, (((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (2))].yystate.yysemantics.yysval.ast_node), (((yyGLRStackItem const *)yyvsp)[YYFILL ((2) - (2))].yystate.yysemantics.yysval.intval), NULL);
		/*puts("groupby_list1");*/
	;}
    break;

  case 14:

/* Line 936 of glr.c  */
#line 508 "sql.ypp"
    {
     	((*yyvalp).ast_node)=newGroupbyExpr(t_groupby_expr, (((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (4))].yystate.yysemantics.yysval.ast_node), (((yyGLRStackItem const *)yyvsp)[YYFILL ((2) - (4))].yystate.yysemantics.yysval.intval), (((yyGLRStackItem const *)yyvsp)[YYFILL ((4) - (4))].yystate.yysemantics.yysval.ast_node));
     	/*puts("groupby_list2");*/
    ;}
    break;

  case 15:

/* Line 936 of glr.c  */
#line 514 "sql.ypp"
    { ((*yyvalp).intval) = 0; ;}
    break;

  case 16:

/* Line 936 of glr.c  */
#line 515 "sql.ypp"
    { ((*yyvalp).intval) = 0; ;}
    break;

  case 17:

/* Line 936 of glr.c  */
#line 516 "sql.ypp"
    { ((*yyvalp).intval) = 1; ;}
    break;

  case 18:

/* Line 936 of glr.c  */
#line 519 "sql.ypp"
    { ((*yyvalp).intval) = 0; ;}
    break;

  case 19:

/* Line 936 of glr.c  */
#line 520 "sql.ypp"
    { ((*yyvalp).intval) = 1; ;}
    break;

  case 20:

/* Line 936 of glr.c  */
#line 523 "sql.ypp"
    { ((*yyvalp).ast_node)=NULL; /*puts("opt_having is null");*/ ;}
    break;

  case 21:

/* Line 936 of glr.c  */
#line 525 "sql.ypp"
    {
		((*yyvalp).ast_node)=newHavingList(t_having_list, NULL, (((yyGLRStackItem const *)yyvsp)[YYFILL ((2) - (2))].yystate.yysemantics.yysval.ast_node));
		/*puts("opt_having2");*/
	;}
    break;

  case 22:

/* Line 936 of glr.c  */
#line 531 "sql.ypp"
    { ((*yyvalp).ast_node)=NULL; /*puts("opt_orederby is null");*/ ;}
    break;

  case 23:

/* Line 936 of glr.c  */
#line 533 "sql.ypp"
    {
		((*yyvalp).ast_node)=newOrderbyList(t_groupby_list, NULL, (((yyGLRStackItem const *)yyvsp)[YYFILL ((3) - (3))].yystate.yysemantics.yysval.ast_node));
		/*puts("opt_orederby2");*/
	;}
    break;

  case 24:

/* Line 936 of glr.c  */
#line 539 "sql.ypp"
    {((*yyvalp).ast_node) = NULL; /*puts("opt_limit is null");*/ ;}
    break;

  case 25:

/* Line 936 of glr.c  */
#line 540 "sql.ypp"
    { ((*yyvalp).ast_node)=newLimitExpr(t_limit_expr, NULL, (((yyGLRStackItem const *)yyvsp)[YYFILL ((2) - (2))].yystate.yysemantics.yysval.ast_node)); /*puts("opt_limit2");*/ ;}
    break;

  case 26:

/* Line 936 of glr.c  */
#line 541 "sql.ypp"
    { ((*yyvalp).ast_node)=newLimitExpr(t_limit_expr, (((yyGLRStackItem const *)yyvsp)[YYFILL ((2) - (4))].yystate.yysemantics.yysval.ast_node), (((yyGLRStackItem const *)yyvsp)[YYFILL ((4) - (4))].yystate.yysemantics.yysval.ast_node)); ;}
    break;

  case 27:

/* Line 936 of glr.c  */
#line 544 "sql.ypp"
    { ((*yyvalp).ast_node)=NULL; /*puts("opt_into_list is null");*/ ;}
    break;

  case 28:

/* Line 936 of glr.c  */
#line 545 "sql.ypp"
    {  ;}
    break;

  case 29:

/* Line 936 of glr.c  */
#line 548 "sql.ypp"
    { ((*yyvalp).ast_node) = newColumn(t_column, (((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (1))].yystate.yysemantics.yysval.strval), NULL, NULL); ;}
    break;

  case 30:

/* Line 936 of glr.c  */
#line 549 "sql.ypp"
    { ((*yyvalp).ast_node) = newColumn(t_column, (((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (3))].yystate.yysemantics.yysval.strval), NULL, (((yyGLRStackItem const *)yyvsp)[YYFILL ((3) - (3))].yystate.yysemantics.yysval.ast_node)); ;}
    break;

  case 31:

/* Line 936 of glr.c  */
#line 552 "sql.ypp"
    { ((*yyvalp).intval) = 0; ;}
    break;

  case 32:

/* Line 936 of glr.c  */
#line 553 "sql.ypp"
    { if(((*yyvalp).intval) & 1) yyerror("duplicate ALL option"); ((*yyvalp).intval) = (((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (2))].yystate.yysemantics.yysval.intval) | 1; ;}
    break;

  case 33:

/* Line 936 of glr.c  */
#line 554 "sql.ypp"
    { if(((*yyvalp).intval) & 2) yyerror("duplicate DISTINCT option"); ((*yyvalp).intval) = (((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (2))].yystate.yysemantics.yysval.intval) | 2; ;}
    break;

  case 34:

/* Line 936 of glr.c  */
#line 555 "sql.ypp"
    { if(((*yyvalp).intval) & 4) yyerror("duplicate DISTINCTROW option"); ((*yyvalp).intval) = (((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (2))].yystate.yysemantics.yysval.intval) | 4; ;}
    break;

  case 35:

/* Line 936 of glr.c  */
#line 556 "sql.ypp"
    { if(((*yyvalp).intval) & 8) yyerror("duplicate HIGH_PRIORITY option"); ((*yyvalp).intval) = (((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (2))].yystate.yysemantics.yysval.intval) | 8; ;}
    break;

  case 36:

/* Line 936 of glr.c  */
#line 557 "sql.ypp"
    { if(((*yyvalp).intval) & 16) yyerror("duplicate STRAIGHT_JOIN option"); ((*yyvalp).intval) = (((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (2))].yystate.yysemantics.yysval.intval) | 16; ;}
    break;

  case 37:

/* Line 936 of glr.c  */
#line 558 "sql.ypp"
    { if(((*yyvalp).intval) & 32) yyerror("duplicate SQL_SMALL_RESULT option"); ((*yyvalp).intval) = (((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (2))].yystate.yysemantics.yysval.intval) | 32; ;}
    break;

  case 38:

/* Line 936 of glr.c  */
#line 559 "sql.ypp"
    { if(((*yyvalp).intval) & 64) yyerror("duplicate SQL_BIG_RESULT option"); ((*yyvalp).intval) = (((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (2))].yystate.yysemantics.yysval.intval) | 64; ;}
    break;

  case 39:

/* Line 936 of glr.c  */
#line 560 "sql.ypp"
    { if(((*yyvalp).intval) & 128) yyerror("duplicate SQL_CALC_FOUND_ROWS option"); ((*yyvalp).intval) = (((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (2))].yystate.yysemantics.yysval.intval) | 128; ;}
    break;

  case 40:

/* Line 936 of glr.c  */
#line 563 "sql.ypp"
    { ((*yyvalp).ast_node) = newSelectList(t_select_list, 0, (((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (1))].yystate.yysemantics.yysval.ast_node), NULL); /*puts("select_expr_list1");*/ ;}
    break;

  case 41:

/* Line 936 of glr.c  */
#line 564 "sql.ypp"
    { ((*yyvalp).ast_node) = newSelectList(t_select_list, 0, (((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (3))].yystate.yysemantics.yysval.ast_node), (((yyGLRStackItem const *)yyvsp)[YYFILL ((3) - (3))].yystate.yysemantics.yysval.ast_node)); /*puts("select_expr_list2");*/ ;}
    break;

  case 42:

/* Line 936 of glr.c  */
#line 565 "sql.ypp"
    { ((*yyvalp).ast_node) = newSelectList(t_select_list, 1, NULL, NULL); /*puts("select_expr_list3");*/ ;}
    break;

  case 43:

/* Line 936 of glr.c  */
#line 566 "sql.ypp"
    { ((*yyvalp).ast_node) = newSelectList(t_select_list, 1, NULL, (((yyGLRStackItem const *)yyvsp)[YYFILL ((3) - (3))].yystate.yysemantics.yysval.ast_node)); /*puts("select_expr_list4");*/ ;}
    break;

  case 44:

/* Line 936 of glr.c  */
#line 569 "sql.ypp"
    { ((*yyvalp).ast_node) = newSelectExpr(t_select_expr, (((yyGLRStackItem const *)yyvsp)[YYFILL ((2) - (2))].yystate.yysemantics.yysval.strval), (((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (2))].yystate.yysemantics.yysval.ast_node)); /*puts("select_expr");*/ ;}
    break;

  case 45:

/* Line 936 of glr.c  */
#line 572 "sql.ypp"
    { ((*yyvalp).ast_node)=newFromList(t_from_list, (((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (1))].yystate.yysemantics.yysval.ast_node), NULL); /*puts("table_references1");*/ ;}
    break;

  case 46:

/* Line 936 of glr.c  */
#line 573 "sql.ypp"
    { ((*yyvalp).ast_node)=newFromList(t_from_list, (((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (3))].yystate.yysemantics.yysval.ast_node), (((yyGLRStackItem const *)yyvsp)[YYFILL ((3) - (3))].yystate.yysemantics.yysval.ast_node)); /*puts("table_references2");*/ ;}
    break;

  case 47:

/* Line 936 of glr.c  */
#line 576 "sql.ypp"
    { ((*yyvalp).ast_node)=(((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (1))].yystate.yysemantics.yysval.ast_node); /*puts("table_referen1");*/ ;}
    break;

  case 48:

/* Line 936 of glr.c  */
#line 577 "sql.ypp"
    { ((*yyvalp).ast_node)=(((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (1))].yystate.yysemantics.yysval.ast_node); /*puts("table_referen2");*/ ;}
    break;

  case 49:

/* Line 936 of glr.c  */
#line 582 "sql.ypp"
    {
		((*yyvalp).ast_node)=newTable(t_table, NULL,(((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (2))].yystate.yysemantics.yysval.strval), (((yyGLRStackItem const *)yyvsp)[YYFILL ((2) - (2))].yystate.yysemantics.yysval.strval), 0, NULL);
		/*puts("table_factor1");*/
	;}
    break;

  case 50:

/* Line 936 of glr.c  */
#line 587 "sql.ypp"
    {
		((*yyvalp).ast_node)=newTable(t_table, (((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (4))].yystate.yysemantics.yysval.strval), (((yyGLRStackItem const *)yyvsp)[YYFILL ((3) - (4))].yystate.yysemantics.yysval.strval), (((yyGLRStackItem const *)yyvsp)[YYFILL ((4) - (4))].yystate.yysemantics.yysval.strval), 0, NULL);
   		/*puts("table_factor2");*/ 
   	;}
    break;

  case 51:

/* Line 936 of glr.c  */
#line 592 "sql.ypp"
    {
		((*yyvalp).ast_node)=newTable(t_table, NULL, NULL, (((yyGLRStackItem const *)yyvsp)[YYFILL ((3) - (3))].yystate.yysemantics.yysval.strval), 1, (((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (3))].yystate.yysemantics.yysval.ast_node));
		/*puts("table_factor3");*/
	;}
    break;

  case 52:

/* Line 936 of glr.c  */
#line 596 "sql.ypp"
    { ((*yyvalp).ast_node)=(((yyGLRStackItem const *)yyvsp)[YYFILL ((2) - (3))].yystate.yysemantics.yysval.ast_node); ;}
    break;

  case 55:

/* Line 936 of glr.c  */
#line 603 "sql.ypp"
    { ((*yyvalp).strval)=(((yyGLRStackItem const *)yyvsp)[YYFILL ((2) - (2))].yystate.yysemantics.yysval.strval); /*puts("opt_as_alias1");*/ ;}
    break;

  case 56:

/* Line 936 of glr.c  */
#line 604 "sql.ypp"
    { ((*yyvalp).strval)=(((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (1))].yystate.yysemantics.yysval.strval); /*puts("opt_as_alias2");*/ ;}
    break;

  case 57:

/* Line 936 of glr.c  */
#line 605 "sql.ypp"
    { ((*yyvalp).strval)=NULL; /*puts("opt_as_alias3");*/ ;}
    break;

  case 58:

/* Line 936 of glr.c  */
#line 621 "sql.ypp"
    {
		((*yyvalp).ast_node)=newJoin(t_join, (((yyGLRStackItem const *)yyvsp)[YYFILL ((2) - (5))].yystate.yysemantics.yysval.intval), (((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (5))].yystate.yysemantics.yysval.ast_node), (((yyGLRStackItem const *)yyvsp)[YYFILL ((4) - (5))].yystate.yysemantics.yysval.ast_node), (((yyGLRStackItem const *)yyvsp)[YYFILL ((5) - (5))].yystate.yysemantics.yysval.ast_node));
	;}
    break;

  case 59:

/* Line 936 of glr.c  */
#line 625 "sql.ypp"
    {
		((*yyvalp).ast_node)=newJoin(t_join, -1, (((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (3))].yystate.yysemantics.yysval.ast_node), (((yyGLRStackItem const *)yyvsp)[YYFILL ((3) - (3))].yystate.yysemantics.yysval.ast_node), NULL);
	;}
    break;

  case 60:

/* Line 936 of glr.c  */
#line 629 "sql.ypp"
    {
		Node *m=newCondition(t_condition, 1, (((yyGLRStackItem const *)yyvsp)[YYFILL ((5) - (5))].yystate.yysemantics.yysval.ast_node));
		((*yyvalp).ast_node)=newJoin(t_join, -1, (((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (5))].yystate.yysemantics.yysval.ast_node), (((yyGLRStackItem const *)yyvsp)[YYFILL ((3) - (5))].yystate.yysemantics.yysval.ast_node), m);
	;}
    break;

  case 61:

/* Line 936 of glr.c  */
#line 634 "sql.ypp"
    {
		((*yyvalp).ast_node)=newJoin(t_join, (((yyGLRStackItem const *)yyvsp)[YYFILL ((2) - (6))].yystate.yysemantics.yysval.intval)+(((yyGLRStackItem const *)yyvsp)[YYFILL ((3) - (6))].yystate.yysemantics.yysval.intval), (((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (6))].yystate.yysemantics.yysval.ast_node), (((yyGLRStackItem const *)yyvsp)[YYFILL ((5) - (6))].yystate.yysemantics.yysval.ast_node), (((yyGLRStackItem const *)yyvsp)[YYFILL ((6) - (6))].yystate.yysemantics.yysval.ast_node));
    	;}
    break;

  case 62:

/* Line 936 of glr.c  */
#line 638 "sql.ypp"
    {
		((*yyvalp).ast_node)=newJoin(t_join, 32 + (((yyGLRStackItem const *)yyvsp)[YYFILL ((3) - (5))].yystate.yysemantics.yysval.intval), (((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (5))].yystate.yysemantics.yysval.ast_node), (((yyGLRStackItem const *)yyvsp)[YYFILL ((5) - (5))].yystate.yysemantics.yysval.ast_node), NULL);
	;}
    break;

  case 63:

/* Line 936 of glr.c  */
#line 643 "sql.ypp"
    { ((*yyvalp).intval) = 0; ;}
    break;

  case 64:

/* Line 936 of glr.c  */
#line 644 "sql.ypp"
    { ((*yyvalp).intval) = 1; ;}
    break;

  case 65:

/* Line 936 of glr.c  */
#line 645 "sql.ypp"
    { ((*yyvalp).intval) = 2; ;}
    break;

  case 66:

/* Line 936 of glr.c  */
#line 648 "sql.ypp"
    { ((*yyvalp).intval) = 0; ;}
    break;

  case 67:

/* Line 936 of glr.c  */
#line 649 "sql.ypp"
    {((*yyvalp).intval) = 4; ;}
    break;

  case 68:

/* Line 936 of glr.c  */
#line 652 "sql.ypp"
    { ((*yyvalp).intval) = 8; ;}
    break;

  case 69:

/* Line 936 of glr.c  */
#line 653 "sql.ypp"
    { ((*yyvalp).intval) = 16; ;}
    break;

  case 70:

/* Line 936 of glr.c  */
#line 656 "sql.ypp"
    { ((*yyvalp).intval) = 8 + (((yyGLRStackItem const *)yyvsp)[YYFILL ((2) - (2))].yystate.yysemantics.yysval.intval); ;}
    break;

  case 71:

/* Line 936 of glr.c  */
#line 657 "sql.ypp"
    { ((*yyvalp).intval) = 16 + (((yyGLRStackItem const *)yyvsp)[YYFILL ((2) - (2))].yystate.yysemantics.yysval.intval); ;}
    break;

  case 72:

/* Line 936 of glr.c  */
#line 658 "sql.ypp"
    { ((*yyvalp).intval) = 0; ;}
    break;

  case 74:

/* Line 936 of glr.c  */
#line 662 "sql.ypp"
    {((*yyvalp).ast_node)=(((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (1))].yystate.yysemantics.yysval.ast_node);;}
    break;

  case 75:

/* Line 936 of glr.c  */
#line 670 "sql.ypp"
    {
    	((*yyvalp).ast_node)=newCondition(t_condition, 1, (((yyGLRStackItem const *)yyvsp)[YYFILL ((2) - (2))].yystate.yysemantics.yysval.ast_node));
    ;}
    break;

  case 76:

/* Line 936 of glr.c  */
#line 674 "sql.ypp"
    {
    	((*yyvalp).ast_node)=newCondition(t_condition, 2, (((yyGLRStackItem const *)yyvsp)[YYFILL ((3) - (4))].yystate.yysemantics.yysval.ast_node));
    ;}
    break;

  case 77:

/* Line 936 of glr.c  */
#line 693 "sql.ypp"
    { emit("INDEX %s", (((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (1))].yystate.yysemantics.yysval.strval));  ;}
    break;

  case 78:

/* Line 936 of glr.c  */
#line 694 "sql.ypp"
    { emit("INDEX %s", (((yyGLRStackItem const *)yyvsp)[YYFILL ((3) - (3))].yystate.yysemantics.yysval.strval)); ;}
    break;

  case 79:

/* Line 936 of glr.c  */
#line 697 "sql.ypp"
    {  ;}
    break;

  case 80:

/* Line 936 of glr.c  */
#line 702 "sql.ypp"
    { emit("STMT"); ;}
    break;

  case 81:

/* Line 936 of glr.c  */
#line 707 "sql.ypp"
    { emit("DELETEONE %d %s", (((yyGLRStackItem const *)yyvsp)[YYFILL ((2) - (7))].yystate.yysemantics.yysval.ast_node), (((yyGLRStackItem const *)yyvsp)[YYFILL ((4) - (7))].yystate.yysemantics.yysval.strval)); free((((yyGLRStackItem const *)yyvsp)[YYFILL ((4) - (7))].yystate.yysemantics.yysval.strval)); ;}
    break;

  case 82:

/* Line 936 of glr.c  */
#line 710 "sql.ypp"
    { ((*yyvalp).ast_node) = (((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (2))].yystate.yysemantics.yysval.ast_node) + 01; ;}
    break;

  case 83:

/* Line 936 of glr.c  */
#line 711 "sql.ypp"
    { ((*yyvalp).ast_node) = (((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (2))].yystate.yysemantics.yysval.ast_node) + 02; ;}
    break;

  case 84:

/* Line 936 of glr.c  */
#line 712 "sql.ypp"
    { ((*yyvalp).ast_node) = (((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (2))].yystate.yysemantics.yysval.ast_node) + 04; ;}
    break;

  case 85:

/* Line 936 of glr.c  */
#line 713 "sql.ypp"
    { ((*yyvalp).ast_node) = 0; ;}
    break;

  case 86:

/* Line 936 of glr.c  */
#line 719 "sql.ypp"
    {  ;}
    break;

  case 87:

/* Line 936 of glr.c  */
#line 721 "sql.ypp"
    {  ;}
    break;

  case 88:

/* Line 936 of glr.c  */
#line 722 "sql.ypp"
    {  ;}
    break;

  case 91:

/* Line 936 of glr.c  */
#line 730 "sql.ypp"
    { emit("DELETEMULTI %d %d %d", (((yyGLRStackItem const *)yyvsp)[YYFILL ((2) - (7))].yystate.yysemantics.yysval.ast_node), (((yyGLRStackItem const *)yyvsp)[YYFILL ((4) - (7))].yystate.yysemantics.yysval.ast_node), (((yyGLRStackItem const *)yyvsp)[YYFILL ((6) - (7))].yystate.yysemantics.yysval.ast_node)); ;}
    break;

  case 92:

/* Line 936 of glr.c  */
#line 735 "sql.ypp"
    { emit("STMT"); ;}
    break;

  case 93:

/* Line 936 of glr.c  */
#line 741 "sql.ypp"
    { emit("INSERTVALS %d %d %s", (((yyGLRStackItem const *)yyvsp)[YYFILL ((2) - (8))].yystate.yysemantics.yysval.intval), (((yyGLRStackItem const *)yyvsp)[YYFILL ((7) - (8))].yystate.yysemantics.yysval.ast_node), (((yyGLRStackItem const *)yyvsp)[YYFILL ((4) - (8))].yystate.yysemantics.yysval.strval)); free((((yyGLRStackItem const *)yyvsp)[YYFILL ((4) - (8))].yystate.yysemantics.yysval.strval)) ;}
    break;

  case 95:

/* Line 936 of glr.c  */
#line 745 "sql.ypp"
    { emit("DUPUPDATE %d", (((yyGLRStackItem const *)yyvsp)[YYFILL ((5) - (5))].yystate.yysemantics.yysval.ast_node)); ;}
    break;

  case 96:

/* Line 936 of glr.c  */
#line 748 "sql.ypp"
    { ((*yyvalp).intval) = 0; ;}
    break;

  case 97:

/* Line 936 of glr.c  */
#line 749 "sql.ypp"
    { ((*yyvalp).intval) = (((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (2))].yystate.yysemantics.yysval.intval) | 01 ; ;}
    break;

  case 98:

/* Line 936 of glr.c  */
#line 750 "sql.ypp"
    { ((*yyvalp).intval) = (((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (2))].yystate.yysemantics.yysval.intval) | 02 ; ;}
    break;

  case 99:

/* Line 936 of glr.c  */
#line 751 "sql.ypp"
    { ((*yyvalp).intval) = (((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (2))].yystate.yysemantics.yysval.intval) | 04 ; ;}
    break;

  case 100:

/* Line 936 of glr.c  */
#line 752 "sql.ypp"
    { ((*yyvalp).intval) = (((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (2))].yystate.yysemantics.yysval.intval) | 010 ; ;}
    break;

  case 104:

/* Line 936 of glr.c  */
#line 759 "sql.ypp"
    {  ;}
    break;

  case 111:

/* Line 936 of glr.c  */
#line 775 "sql.ypp"
    { emit("INSERTASGN %d %d %s", (((yyGLRStackItem const *)yyvsp)[YYFILL ((2) - (7))].yystate.yysemantics.yysval.intval), (((yyGLRStackItem const *)yyvsp)[YYFILL ((6) - (7))].yystate.yysemantics.yysval.ast_node), (((yyGLRStackItem const *)yyvsp)[YYFILL ((4) - (7))].yystate.yysemantics.yysval.strval)); free((((yyGLRStackItem const *)yyvsp)[YYFILL ((4) - (7))].yystate.yysemantics.yysval.strval)) ;}
    break;

  case 112:

/* Line 936 of glr.c  */
#line 780 "sql.ypp"
    { emit("INSERTSELECT %d %s", (((yyGLRStackItem const *)yyvsp)[YYFILL ((2) - (7))].yystate.yysemantics.yysval.intval), (((yyGLRStackItem const *)yyvsp)[YYFILL ((4) - (7))].yystate.yysemantics.yysval.strval)); free((((yyGLRStackItem const *)yyvsp)[YYFILL ((4) - (7))].yystate.yysemantics.yysval.strval)); ;}
    break;

  case 117:

/* Line 936 of glr.c  */
#line 799 "sql.ypp"
    { emit("STMT"); ;}
    break;

  case 118:

/* Line 936 of glr.c  */
#line 805 "sql.ypp"
    { emit("REPLACEVALS %d %d %s", (((yyGLRStackItem const *)yyvsp)[YYFILL ((2) - (8))].yystate.yysemantics.yysval.intval), (((yyGLRStackItem const *)yyvsp)[YYFILL ((7) - (8))].yystate.yysemantics.yysval.ast_node), (((yyGLRStackItem const *)yyvsp)[YYFILL ((4) - (8))].yystate.yysemantics.yysval.strval)); free((((yyGLRStackItem const *)yyvsp)[YYFILL ((4) - (8))].yystate.yysemantics.yysval.strval)) ;}
    break;

  case 119:

/* Line 936 of glr.c  */
#line 811 "sql.ypp"
    { emit("REPLACEASGN %d %d %s", (((yyGLRStackItem const *)yyvsp)[YYFILL ((2) - (7))].yystate.yysemantics.yysval.intval), (((yyGLRStackItem const *)yyvsp)[YYFILL ((6) - (7))].yystate.yysemantics.yysval.ast_node), (((yyGLRStackItem const *)yyvsp)[YYFILL ((4) - (7))].yystate.yysemantics.yysval.strval)); free((((yyGLRStackItem const *)yyvsp)[YYFILL ((4) - (7))].yystate.yysemantics.yysval.strval)) ;}
    break;

  case 120:

/* Line 936 of glr.c  */
#line 816 "sql.ypp"
    { emit("REPLACESELECT %d %s", (((yyGLRStackItem const *)yyvsp)[YYFILL ((2) - (7))].yystate.yysemantics.yysval.intval), (((yyGLRStackItem const *)yyvsp)[YYFILL ((4) - (7))].yystate.yysemantics.yysval.strval)); free((((yyGLRStackItem const *)yyvsp)[YYFILL ((4) - (7))].yystate.yysemantics.yysval.strval)); ;}
    break;

  case 121:

/* Line 936 of glr.c  */
#line 820 "sql.ypp"
    { emit("STMT"); ;}
    break;

  case 122:

/* Line 936 of glr.c  */
#line 827 "sql.ypp"
    { emit("UPDATE %d %d %d", (((yyGLRStackItem const *)yyvsp)[YYFILL ((2) - (8))].yystate.yysemantics.yysval.ast_node), (((yyGLRStackItem const *)yyvsp)[YYFILL ((3) - (8))].yystate.yysemantics.yysval.ast_node), (((yyGLRStackItem const *)yyvsp)[YYFILL ((5) - (8))].yystate.yysemantics.yysval.ast_node)); ;}
    break;

  case 126:

/* Line 936 of glr.c  */
#line 837 "sql.ypp"
    {
		/*
		if ($2 != 4 || $3->data.string_val == NULL) 
			yyerror("bad insert assignment to %s", $1);
	 	emit("ASSIGN %s", $1); 
	 	free($3); 
	 	$$ = $3->data.string_val;
	 	*/ 
	;}
    break;

  case 127:

/* Line 936 of glr.c  */
#line 847 "sql.ypp"
    { 
	/*
		if ($4 != 4) 
			yyerror("bad insert assignment to %s", $1); 
		emit("ASSIGN %s.%s", $1, $3);
		free($1); 
		free($3); 
		$$ = 1;
	*/
	;}
    break;

  case 130:

/* Line 936 of glr.c  */
#line 868 "sql.ypp"
    { ((*yyvalp).ast_node)=(((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (1))].yystate.yysemantics.yysval.ast_node); output(((*yyvalp).ast_node), 1); puts("SQL parser： This is a create_database statement"); ;}
    break;

  case 131:

/* Line 936 of glr.c  */
#line 873 "sql.ypp"
    { ((*yyvalp).ast_node) =  newCreateDatabaseStmt(t_create_database_stmt, 1, (((yyGLRStackItem const *)yyvsp)[YYFILL ((3) - (4))].yystate.yysemantics.yysval.intval), (((yyGLRStackItem const *)yyvsp)[YYFILL ((4) - (4))].yystate.yysemantics.yysval.strval)); ;}
    break;

  case 132:

/* Line 936 of glr.c  */
#line 875 "sql.ypp"
    { ((*yyvalp).ast_node) =  newCreateDatabaseStmt(t_create_database_stmt, 2, (((yyGLRStackItem const *)yyvsp)[YYFILL ((3) - (4))].yystate.yysemantics.yysval.intval), (((yyGLRStackItem const *)yyvsp)[YYFILL ((4) - (4))].yystate.yysemantics.yysval.strval)); ;}
    break;

  case 133:

/* Line 936 of glr.c  */
#line 878 "sql.ypp"
    { ((*yyvalp).intval) = 0; ;}
    break;

  case 134:

/* Line 936 of glr.c  */
#line 879 "sql.ypp"
    { ((*yyvalp).intval) = 1; ;}
    break;

  case 135:

/* Line 936 of glr.c  */
#line 884 "sql.ypp"
    { ((*yyvalp).ast_node)=(((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (1))].yystate.yysemantics.yysval.ast_node); output(((*yyvalp).ast_node), 1); puts("SQL parser： This is a create_table statement"); ;}
    break;

  case 136:

/* Line 936 of glr.c  */
#line 888 "sql.ypp"
    { ((*yyvalp).ast_node) = newCreateTableStmt(t_create_table_stmt, (((yyGLRStackItem const *)yyvsp)[YYFILL ((2) - (8))].yystate.yysemantics.yysval.intval), (((yyGLRStackItem const *)yyvsp)[YYFILL ((4) - (8))].yystate.yysemantics.yysval.intval), (((yyGLRStackItem const *)yyvsp)[YYFILL ((5) - (8))].yystate.yysemantics.yysval.strval), NULL, (((yyGLRStackItem const *)yyvsp)[YYFILL ((7) - (8))].yystate.yysemantics.yysval.ast_node), NULL); ;}
    break;

  case 137:

/* Line 936 of glr.c  */
#line 892 "sql.ypp"
    { ((*yyvalp).ast_node) = newCreateTableStmt(t_create_table_stmt, (((yyGLRStackItem const *)yyvsp)[YYFILL ((2) - (10))].yystate.yysemantics.yysval.intval), (((yyGLRStackItem const *)yyvsp)[YYFILL ((4) - (10))].yystate.yysemantics.yysval.intval), (((yyGLRStackItem const *)yyvsp)[YYFILL ((5) - (10))].yystate.yysemantics.yysval.strval), (((yyGLRStackItem const *)yyvsp)[YYFILL ((7) - (10))].yystate.yysemantics.yysval.strval), (((yyGLRStackItem const *)yyvsp)[YYFILL ((9) - (10))].yystate.yysemantics.yysval.ast_node), NULL); ;}
    break;

  case 138:

/* Line 936 of glr.c  */
#line 897 "sql.ypp"
    { ((*yyvalp).ast_node) = newCreateTableStmt(t_create_table_stmt, (((yyGLRStackItem const *)yyvsp)[YYFILL ((2) - (9))].yystate.yysemantics.yysval.intval), (((yyGLRStackItem const *)yyvsp)[YYFILL ((4) - (9))].yystate.yysemantics.yysval.intval), (((yyGLRStackItem const *)yyvsp)[YYFILL ((5) - (9))].yystate.yysemantics.yysval.strval), NULL, (((yyGLRStackItem const *)yyvsp)[YYFILL ((7) - (9))].yystate.yysemantics.yysval.ast_node), (((yyGLRStackItem const *)yyvsp)[YYFILL ((9) - (9))].yystate.yysemantics.yysval.ast_node)); ;}
    break;

  case 139:

/* Line 936 of glr.c  */
#line 901 "sql.ypp"
    { ((*yyvalp).ast_node) = newCreateTableStmt(t_create_table_stmt, (((yyGLRStackItem const *)yyvsp)[YYFILL ((2) - (6))].yystate.yysemantics.yysval.intval), (((yyGLRStackItem const *)yyvsp)[YYFILL ((4) - (6))].yystate.yysemantics.yysval.intval), (((yyGLRStackItem const *)yyvsp)[YYFILL ((5) - (6))].yystate.yysemantics.yysval.strval), NULL, NULL, (((yyGLRStackItem const *)yyvsp)[YYFILL ((6) - (6))].yystate.yysemantics.yysval.ast_node)); ;}
    break;

  case 140:

/* Line 936 of glr.c  */
#line 906 "sql.ypp"
    { ((*yyvalp).ast_node) = newCreateTableStmt(t_create_table_stmt, (((yyGLRStackItem const *)yyvsp)[YYFILL ((2) - (11))].yystate.yysemantics.yysval.intval), (((yyGLRStackItem const *)yyvsp)[YYFILL ((4) - (11))].yystate.yysemantics.yysval.intval), (((yyGLRStackItem const *)yyvsp)[YYFILL ((5) - (11))].yystate.yysemantics.yysval.strval), (((yyGLRStackItem const *)yyvsp)[YYFILL ((7) - (11))].yystate.yysemantics.yysval.strval), (((yyGLRStackItem const *)yyvsp)[YYFILL ((9) - (11))].yystate.yysemantics.yysval.ast_node), (((yyGLRStackItem const *)yyvsp)[YYFILL ((11) - (11))].yystate.yysemantics.yysval.ast_node)); ;}
    break;

  case 141:

/* Line 936 of glr.c  */
#line 911 "sql.ypp"
    { ((*yyvalp).ast_node) = newCreateTableStmt(t_create_table_stmt, (((yyGLRStackItem const *)yyvsp)[YYFILL ((2) - (8))].yystate.yysemantics.yysval.intval), (((yyGLRStackItem const *)yyvsp)[YYFILL ((4) - (8))].yystate.yysemantics.yysval.intval), (((yyGLRStackItem const *)yyvsp)[YYFILL ((5) - (8))].yystate.yysemantics.yysval.strval), (((yyGLRStackItem const *)yyvsp)[YYFILL ((7) - (8))].yystate.yysemantics.yysval.strval), NULL, (((yyGLRStackItem const *)yyvsp)[YYFILL ((8) - (8))].yystate.yysemantics.yysval.ast_node)); ;}
    break;

  case 142:

/* Line 936 of glr.c  */
#line 916 "sql.ypp"
    { ((*yyvalp).ast_node) = newCreateSelectStmt(t_create_select_stmt, (((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (3))].yystate.yysemantics.yysval.intval), 0, (((yyGLRStackItem const *)yyvsp)[YYFILL ((3) - (3))].yystate.yysemantics.yysval.ast_node)); ;}
    break;

  case 143:

/* Line 936 of glr.c  */
#line 919 "sql.ypp"
    { ((*yyvalp).intval) = 0; ;}
    break;

  case 144:

/* Line 936 of glr.c  */
#line 920 "sql.ypp"
    { ((*yyvalp).intval) = 1; ;}
    break;

  case 145:

/* Line 936 of glr.c  */
#line 921 "sql.ypp"
    { ((*yyvalp).intval) = 2; ;}
    break;

  case 146:

/* Line 936 of glr.c  */
#line 924 "sql.ypp"
    { ((*yyvalp).intval) = 0; ;}
    break;

  case 147:

/* Line 936 of glr.c  */
#line 925 "sql.ypp"
    { ((*yyvalp).intval) = 1; ;}
    break;

  case 148:

/* Line 936 of glr.c  */
#line 928 "sql.ypp"
    { ((*yyvalp).ast_node) = newCreateColList(t_create_col_list, (((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (1))].yystate.yysemantics.yysval.ast_node), NULL); ;}
    break;

  case 149:

/* Line 936 of glr.c  */
#line 929 "sql.ypp"
    { ((*yyvalp).ast_node) = newCreateColList(t_create_col_list, (((yyGLRStackItem const *)yyvsp)[YYFILL ((3) - (3))].yystate.yysemantics.yysval.ast_node), (((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (3))].yystate.yysemantics.yysval.ast_node)); ;}
    break;

  case 150:

/* Line 936 of glr.c  */
#line 933 "sql.ypp"
    { ((*yyvalp).ast_node) = newCreateDef( t_create_def, 1, (((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (3))].yystate.yysemantics.yysval.strval), (((yyGLRStackItem const *)yyvsp)[YYFILL ((2) - (3))].yystate.yysemantics.yysval.ast_node), (((yyGLRStackItem const *)yyvsp)[YYFILL ((3) - (3))].yystate.yysemantics.yysval.ast_node), NULL); ;}
    break;

  case 151:

/* Line 936 of glr.c  */
#line 934 "sql.ypp"
    { ((*yyvalp).ast_node) = newCreateDef( t_create_def, 2, NULL, NULL, NULL, (((yyGLRStackItem const *)yyvsp)[YYFILL ((4) - (5))].yystate.yysemantics.yysval.ast_node)); /*puts("primary key ok!");*/ ;}
    break;

  case 152:

/* Line 936 of glr.c  */
#line 935 "sql.ypp"
    { ((*yyvalp).ast_node) = newCreateDef( t_create_def, 3, NULL, NULL, NULL, (((yyGLRStackItem const *)yyvsp)[YYFILL ((3) - (4))].yystate.yysemantics.yysval.ast_node)); ;}
    break;

  case 153:

/* Line 936 of glr.c  */
#line 936 "sql.ypp"
    { ((*yyvalp).ast_node) = newCreateDef( t_create_def, 4, NULL, NULL, NULL, (((yyGLRStackItem const *)yyvsp)[YYFILL ((3) - (4))].yystate.yysemantics.yysval.ast_node)); ;}
    break;

  case 154:

/* Line 936 of glr.c  */
#line 937 "sql.ypp"
    { ((*yyvalp).ast_node) = newCreateDef( t_create_def, 5, NULL, NULL, NULL, (((yyGLRStackItem const *)yyvsp)[YYFILL ((4) - (5))].yystate.yysemantics.yysval.ast_node)); ;}
    break;

  case 155:

/* Line 936 of glr.c  */
#line 938 "sql.ypp"
    { ((*yyvalp).ast_node) = newCreateDef( t_create_def, 6, NULL, NULL, NULL, (((yyGLRStackItem const *)yyvsp)[YYFILL ((4) - (5))].yystate.yysemantics.yysval.ast_node)); ;}
    break;

  case 156:

/* Line 936 of glr.c  */
#line 941 "sql.ypp"
    { ((*yyvalp).ast_node) = newColumnAtts(t_column_atts, 0, 0, 0, NULL, NULL); ;}
    break;

  case 157:

/* Line 936 of glr.c  */
#line 942 "sql.ypp"
    { Column_atts *t = (Column_atts *)(((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (3))].yystate.yysemantics.yysval.ast_node);t->datatype += 01; ((*yyvalp).ast_node) = (Node *)t; ;}
    break;

  case 158:

/* Line 936 of glr.c  */
#line 943 "sql.ypp"
    { Column_atts *t = (Column_atts *)(((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (2))].yystate.yysemantics.yysval.ast_node);t->datatype += 02; ((*yyvalp).ast_node) = (Node *)t; ;}
    break;

  case 159:

/* Line 936 of glr.c  */
#line 944 "sql.ypp"
    { Column_atts *t = (Column_atts *)(((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (3))].yystate.yysemantics.yysval.ast_node);t->datatype += 04; t->s = (((yyGLRStackItem const *)yyvsp)[YYFILL ((3) - (3))].yystate.yysemantics.yysval.strval); ((*yyvalp).ast_node) = (Node *)t; ;}
    break;

  case 160:

/* Line 936 of glr.c  */
#line 945 "sql.ypp"
    { Column_atts *t = (Column_atts *)(((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (3))].yystate.yysemantics.yysval.ast_node);t->datatype += 010; t->num1 = (((yyGLRStackItem const *)yyvsp)[YYFILL ((3) - (3))].yystate.yysemantics.yysval.intval); ((*yyvalp).ast_node) = (Node *)t; ;}
    break;

  case 161:

/* Line 936 of glr.c  */
#line 946 "sql.ypp"
    { Column_atts *t = (Column_atts *)(((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (3))].yystate.yysemantics.yysval.ast_node);t->datatype += 020; t->num2 = (((yyGLRStackItem const *)yyvsp)[YYFILL ((3) - (3))].yystate.yysemantics.yysval.floatval); ((*yyvalp).ast_node) = (Node *)t; ;}
    break;

  case 162:

/* Line 936 of glr.c  */
#line 947 "sql.ypp"
    { Column_atts *t = (Column_atts *)(((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (3))].yystate.yysemantics.yysval.ast_node);t->datatype += 040; t->num1 = (((yyGLRStackItem const *)yyvsp)[YYFILL ((3) - (3))].yystate.yysemantics.yysval.intval); ((*yyvalp).ast_node) = (Node *)t; ;}
    break;

  case 163:

/* Line 936 of glr.c  */
#line 948 "sql.ypp"
    { Column_atts *t = (Column_atts *)(((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (2))].yystate.yysemantics.yysval.ast_node);t->datatype += 0100; ((*yyvalp).ast_node) = (Node *)t; ;}
    break;

  case 164:

/* Line 936 of glr.c  */
#line 949 "sql.ypp"
    { Column_atts *t = (Column_atts *)(((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (3))].yystate.yysemantics.yysval.ast_node);t->datatype += 0200; ((*yyvalp).ast_node) = (Node *)t; ;}
    break;

  case 165:

/* Line 936 of glr.c  */
#line 950 "sql.ypp"
    { Column_atts *t = (Column_atts *)(((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (3))].yystate.yysemantics.yysval.ast_node);t->datatype += 0400; ((*yyvalp).ast_node) = (Node *)t; ;}
    break;

  case 166:

/* Line 936 of glr.c  */
#line 951 "sql.ypp"
    { Column_atts *t = (Column_atts *)(((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (2))].yystate.yysemantics.yysval.ast_node);t->datatype += 01000; ((*yyvalp).ast_node) = (Node *)t; ;}
    break;

  case 167:

/* Line 936 of glr.c  */
#line 952 "sql.ypp"
    { Column_atts *t = (Column_atts *)(((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (3))].yystate.yysemantics.yysval.ast_node);t->datatype += 02000; ((*yyvalp).ast_node) = (Node *)t; ;}
    break;

  case 168:

/* Line 936 of glr.c  */
#line 953 "sql.ypp"
    { Column_atts *t = (Column_atts *)(((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (5))].yystate.yysemantics.yysval.ast_node);t->datatype += 04000; t->col_list = (((yyGLRStackItem const *)yyvsp)[YYFILL ((4) - (5))].yystate.yysemantics.yysval.ast_node); ((*yyvalp).ast_node) = (Node *)t; ;}
    break;

  case 169:

/* Line 936 of glr.c  */
#line 957 "sql.ypp"
    { ((*yyvalp).ast_node) = (((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (1))].yystate.yysemantics.yysval.ast_node); puts("SQL parser： This is a create_projection statement"); ;}
    break;

  case 170:

/* Line 936 of glr.c  */
#line 962 "sql.ypp"
    { ((*yyvalp).ast_node) = newCreateProjectionStmt(t_create_projection_stmt, (((yyGLRStackItem const *)yyvsp)[YYFILL ((4) - (10))].yystate.yysemantics.yysval.strval), (((yyGLRStackItem const *)yyvsp)[YYFILL ((6) - (10))].yystate.yysemantics.yysval.ast_node), 1, (((yyGLRStackItem const *)yyvsp)[YYFILL ((10) - (10))].yystate.yysemantics.yysval.strval)); ;}
    break;

  case 171:

/* Line 936 of glr.c  */
#line 964 "sql.ypp"
    { 
		if ((((yyGLRStackItem const *)yyvsp)[YYFILL ((9) - (13))].yystate.yysemantics.yysval.subtok) != 4) { yyerror("please give a specific number"); } 
		else { ((*yyvalp).ast_node) = newCreateProjectionStmt(t_create_projection_stmt, (((yyGLRStackItem const *)yyvsp)[YYFILL ((4) - (13))].yystate.yysemantics.yysval.strval), (((yyGLRStackItem const *)yyvsp)[YYFILL ((6) - (13))].yystate.yysemantics.yysval.ast_node), (((yyGLRStackItem const *)yyvsp)[YYFILL ((10) - (13))].yystate.yysemantics.yysval.intval), (((yyGLRStackItem const *)yyvsp)[YYFILL ((13) - (13))].yystate.yysemantics.yysval.strval)); }
	;}
    break;

  case 172:

/* Line 936 of glr.c  */
#line 974 "sql.ypp"
    { ((*yyvalp).ast_node) = newDoStmt(t_do_stmt,(((yyGLRStackItem const *)yyvsp)[YYFILL ((2) - (2))].yystate.yysemantics.yysval.ast_node)); puts("SQL parser： This is a do expr statement"); ;}
    break;

  case 173:

/* Line 936 of glr.c  */
#line 978 "sql.ypp"
    { ((*yyvalp).ast_node) = (((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (1))].yystate.yysemantics.yysval.ast_node); puts("SQL parser： This is a truncate_stmt statement"); ;}
    break;

  case 174:

/* Line 936 of glr.c  */
#line 981 "sql.ypp"
    { ((*yyvalp).ast_node) = newTruncateStmt(t_truncate_stmt, (((yyGLRStackItem const *)yyvsp)[YYFILL ((2) - (2))].yystate.yysemantics.yysval.strval)); ;}
    break;

  case 175:

/* Line 936 of glr.c  */
#line 982 "sql.ypp"
    { ((*yyvalp).ast_node) = newTruncateStmt(t_truncate_stmt, (((yyGLRStackItem const *)yyvsp)[YYFILL ((3) - (3))].yystate.yysemantics.yysval.strval)); ;}
    break;

  case 176:

/* Line 936 of glr.c  */
#line 986 "sql.ypp"
    { ((*yyvalp).ast_node)=(((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (1))].yystate.yysemantics.yysval.ast_node); output(((*yyvalp).ast_node), 1); puts("SQL parser： This is a alter_database statement"); ;}
    break;

  case 177:

/* Line 936 of glr.c  */
#line 990 "sql.ypp"
    { ((*yyvalp).ast_node) = newAlterDatabaseStmt(t_alter_database_stmt, 1, (((yyGLRStackItem const *)yyvsp)[YYFILL ((3) - (4))].yystate.yysemantics.yysval.strval), (((yyGLRStackItem const *)yyvsp)[YYFILL ((4) - (4))].yystate.yysemantics.yysval.ast_node)); ;}
    break;

  case 178:

/* Line 936 of glr.c  */
#line 991 "sql.ypp"
    { ((*yyvalp).ast_node) = newAlterDatabaseStmt(t_alter_database_stmt, 2, (((yyGLRStackItem const *)yyvsp)[YYFILL ((3) - (4))].yystate.yysemantics.yysval.strval), (((yyGLRStackItem const *)yyvsp)[YYFILL ((4) - (4))].yystate.yysemantics.yysval.ast_node)); ;}
    break;

  case 179:

/* Line 936 of glr.c  */
#line 995 "sql.ypp"
    { ((*yyvalp).ast_node)=(((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (1))].yystate.yysemantics.yysval.ast_node); output(((*yyvalp).ast_node), 1); puts("SQL parser： This is a alter_table statement"); ;}
    break;

  case 180:

/* Line 936 of glr.c  */
#line 1000 "sql.ypp"
    { ((*yyvalp).ast_node) = newAlterTableStmt(t_alter_table_stmt, (((yyGLRStackItem const *)yyvsp)[YYFILL ((2) - (5))].yystate.yysemantics.yysval.intval), (((yyGLRStackItem const *)yyvsp)[YYFILL ((4) - (5))].yystate.yysemantics.yysval.strval), (((yyGLRStackItem const *)yyvsp)[YYFILL ((5) - (5))].yystate.yysemantics.yysval.ast_node)); ;}
    break;

  case 183:

/* Line 936 of glr.c  */
#line 1009 "sql.ypp"
    {;}
    break;

  case 184:

/* Line 936 of glr.c  */
#line 1010 "sql.ypp"
    {;}
    break;

  case 189:

/* Line 936 of glr.c  */
#line 1015 "sql.ypp"
    {;}
    break;

  case 190:

/* Line 936 of glr.c  */
#line 1016 "sql.ypp"
    {;}
    break;

  case 192:

/* Line 936 of glr.c  */
#line 1020 "sql.ypp"
    { ((*yyvalp).intval) = 0; ;}
    break;

  case 193:

/* Line 936 of glr.c  */
#line 1021 "sql.ypp"
    { ((*yyvalp).intval) = 1; ;}
    break;

  case 194:

/* Line 936 of glr.c  */
#line 1025 "sql.ypp"
    { ((*yyvalp).intval) = 0; ;}
    break;

  case 195:

/* Line 936 of glr.c  */
#line 1026 "sql.ypp"
    { ((*yyvalp).intval) = 1; ;}
    break;

  case 196:

/* Line 936 of glr.c  */
#line 1030 "sql.ypp"
    { emit("STMT"); ;}
    break;

  case 198:

/* Line 936 of glr.c  */
#line 1037 "sql.ypp"
    { ((*yyvalp).intval) = 0;;}
    break;

  case 199:

/* Line 936 of glr.c  */
#line 1038 "sql.ypp"
    { ((*yyvalp).intval) = 1; ;}
    break;

  case 200:

/* Line 936 of glr.c  */
#line 1039 "sql.ypp"
    { ((*yyvalp).intval) = 2; ;}
    break;

  case 201:

/* Line 936 of glr.c  */
#line 1040 "sql.ypp"
    { ((*yyvalp).intval) = 3; ;}
    break;

  case 204:

/* Line 936 of glr.c  */
#line 1047 "sql.ypp"
    { ((*yyvalp).intval) = 1; ;}
    break;

  case 205:

/* Line 936 of glr.c  */
#line 1048 "sql.ypp"
    { ((*yyvalp).intval) = 2; ;}
    break;

  case 209:

/* Line 936 of glr.c  */
#line 1059 "sql.ypp"
    { emit("STMT"); ;}
    break;

  case 211:

/* Line 936 of glr.c  */
#line 1066 "sql.ypp"
    { emit("STMT"); ;}
    break;

  case 212:

/* Line 936 of glr.c  */
#line 1070 "sql.ypp"
    { emit("DROPDATABASE %d %s", (((yyGLRStackItem const *)yyvsp)[YYFILL ((3) - (4))].yystate.yysemantics.yysval.intval), (((yyGLRStackItem const *)yyvsp)[YYFILL ((4) - (4))].yystate.yysemantics.yysval.strval)); free((((yyGLRStackItem const *)yyvsp)[YYFILL ((4) - (4))].yystate.yysemantics.yysval.strval)); ;}
    break;

  case 213:

/* Line 936 of glr.c  */
#line 1071 "sql.ypp"
    { emit("DROPSCHEMA %d %s", (((yyGLRStackItem const *)yyvsp)[YYFILL ((3) - (4))].yystate.yysemantics.yysval.intval), (((yyGLRStackItem const *)yyvsp)[YYFILL ((4) - (4))].yystate.yysemantics.yysval.strval)); free((((yyGLRStackItem const *)yyvsp)[YYFILL ((4) - (4))].yystate.yysemantics.yysval.strval)); ;}
    break;

  case 214:

/* Line 936 of glr.c  */
#line 1074 "sql.ypp"
    { ((*yyvalp).intval) = 0; ;}
    break;

  case 215:

/* Line 936 of glr.c  */
#line 1075 "sql.ypp"
    { ((*yyvalp).intval) = 1; ;}
    break;

  case 216:

/* Line 936 of glr.c  */
#line 1079 "sql.ypp"
    { emit("STMT"); ;}
    break;

  case 217:

/* Line 936 of glr.c  */
#line 1082 "sql.ypp"
    { emit("DROP %d %d %d %d", (((yyGLRStackItem const *)yyvsp)[YYFILL ((2) - (6))].yystate.yysemantics.yysval.intval), (((yyGLRStackItem const *)yyvsp)[YYFILL ((4) - (6))].yystate.yysemantics.yysval.intval), (((yyGLRStackItem const *)yyvsp)[YYFILL ((5) - (6))].yystate.yysemantics.yysval.intval), (((yyGLRStackItem const *)yyvsp)[YYFILL ((6) - (6))].yystate.yysemantics.yysval.intval)); ;}
    break;

  case 218:

/* Line 936 of glr.c  */
#line 1085 "sql.ypp"
    { emit("TABLE %s", (((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (1))].yystate.yysemantics.yysval.strval));;}
    break;

  case 219:

/* Line 936 of glr.c  */
#line 1086 "sql.ypp"
    { emit("TABLE %s.%s", (((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (3))].yystate.yysemantics.yysval.strval), (((yyGLRStackItem const *)yyvsp)[YYFILL ((3) - (3))].yystate.yysemantics.yysval.strval)); ;}
    break;

  case 220:

/* Line 936 of glr.c  */
#line 1087 "sql.ypp"
    { emit("TABLE %s", (((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (3))].yystate.yysemantics.yysval.intval));;}
    break;

  case 221:

/* Line 936 of glr.c  */
#line 1088 "sql.ypp"
    { emit("TABLE %s.%s", (((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (5))].yystate.yysemantics.yysval.intval), (((yyGLRStackItem const *)yyvsp)[YYFILL ((3) - (5))].yystate.yysemantics.yysval.strval)); ;}
    break;

  case 222:

/* Line 936 of glr.c  */
#line 1091 "sql.ypp"
    { ((*yyvalp).intval) = 0; ;}
    break;

  case 223:

/* Line 936 of glr.c  */
#line 1092 "sql.ypp"
    { ((*yyvalp).intval) = 1; ;}
    break;

  case 224:

/* Line 936 of glr.c  */
#line 1093 "sql.ypp"
    { ((*yyvalp).intval) = 2; ;}
    break;

  case 225:

/* Line 936 of glr.c  */
#line 1098 "sql.ypp"
    { emit("STMT"); ;}
    break;

  case 226:

/* Line 936 of glr.c  */
#line 1101 "sql.ypp"
    {;}
    break;

  case 228:

/* Line 936 of glr.c  */
#line 1105 "sql.ypp"
    {;}
    break;

  case 229:

/* Line 936 of glr.c  */
#line 1106 "sql.ypp"
    {;}
    break;

  case 230:

/* Line 936 of glr.c  */
#line 1107 "sql.ypp"
    {;}
    break;

  case 231:

/* Line 936 of glr.c  */
#line 1108 "sql.ypp"
    {;}
    break;

  case 232:

/* Line 936 of glr.c  */
#line 1112 "sql.ypp"
    { emit("STMT"); ;}
    break;

  case 233:

/* Line 936 of glr.c  */
#line 1115 "sql.ypp"
    {;}
    break;

  case 234:

/* Line 936 of glr.c  */
#line 1116 "sql.ypp"
    {;}
    break;

  case 235:

/* Line 936 of glr.c  */
#line 1120 "sql.ypp"
    { emit("STMT"); ;}
    break;

  case 236:

/* Line 936 of glr.c  */
#line 1123 "sql.ypp"
    {;}
    break;

  case 237:

/* Line 936 of glr.c  */
#line 1127 "sql.ypp"
    { emit("STMT"); ;}
    break;

  case 238:

/* Line 936 of glr.c  */
#line 1128 "sql.ypp"
    { emit("STMT"); ;}
    break;

  case 239:

/* Line 936 of glr.c  */
#line 1129 "sql.ypp"
    { emit("STMT"); ;}
    break;

  case 244:

/* Line 936 of glr.c  */
#line 1140 "sql.ypp"
    { emit("STMT"); ;}
    break;

  case 245:

/* Line 936 of glr.c  */
#line 1143 "sql.ypp"
    { ((*yyvalp).intval) = 0; ;}
    break;

  case 246:

/* Line 936 of glr.c  */
#line 1144 "sql.ypp"
    { ((*yyvalp).intval) = 1; ;}
    break;

  case 247:

/* Line 936 of glr.c  */
#line 1145 "sql.ypp"
    { ((*yyvalp).intval) = 2; ;}
    break;

  case 248:

/* Line 936 of glr.c  */
#line 1148 "sql.ypp"
    { ((*yyvalp).intval) = 0; ;}
    break;

  case 249:

/* Line 936 of glr.c  */
#line 1149 "sql.ypp"
    { ((*yyvalp).intval) = 1; ;}
    break;

  case 250:

/* Line 936 of glr.c  */
#line 1150 "sql.ypp"
    { ((*yyvalp).intval) = 2; ;}
    break;

  case 251:

/* Line 936 of glr.c  */
#line 1153 "sql.ypp"
    { emit("STMT"); ;}
    break;

  case 252:

/* Line 936 of glr.c  */
#line 1157 "sql.ypp"
    { emit("STMT"); ;}
    break;

  case 260:

/* Line 936 of glr.c  */
#line 1180 "sql.ypp"
    { emit("STMT"); ;}
    break;

  case 265:

/* Line 936 of glr.c  */
#line 1193 "sql.ypp"
    { ((*yyvalp).intval) = 1; ;}
    break;

  case 266:

/* Line 936 of glr.c  */
#line 1194 "sql.ypp"
    { ((*yyvalp).intval) = 2; ;}
    break;

  case 267:

/* Line 936 of glr.c  */
#line 1195 "sql.ypp"
    { ((*yyvalp).intval) = 3; ;}
    break;

  case 268:

/* Line 936 of glr.c  */
#line 1196 "sql.ypp"
    { ((*yyvalp).intval) = 4; ;}
    break;

  case 273:

/* Line 936 of glr.c  */
#line 1214 "sql.ypp"
    { ((*yyvalp).intval) = 0; ;}
    break;

  case 274:

/* Line 936 of glr.c  */
#line 1215 "sql.ypp"
    { ((*yyvalp).intval) = 1; ;}
    break;

  case 275:

/* Line 936 of glr.c  */
#line 1216 "sql.ypp"
    { ((*yyvalp).intval) = 2; ;}
    break;

  case 276:

/* Line 936 of glr.c  */
#line 1220 "sql.ypp"
    { ((*yyvalp).intval) = 1; ;}
    break;

  case 277:

/* Line 936 of glr.c  */
#line 1221 "sql.ypp"
    { ((*yyvalp).intval) = 2; ;}
    break;

  case 278:

/* Line 936 of glr.c  */
#line 1222 "sql.ypp"
    { ((*yyvalp).intval) = 3; ;}
    break;

  case 279:

/* Line 936 of glr.c  */
#line 1223 "sql.ypp"
    { ((*yyvalp).intval) = 4; ;}
    break;

  case 280:

/* Line 936 of glr.c  */
#line 1227 "sql.ypp"
    { emit("STMT"); ;}
    break;

  case 287:

/* Line 936 of glr.c  */
#line 1243 "sql.ypp"
    { emit("STMT"); ;}
    break;

  case 291:

/* Line 936 of glr.c  */
#line 1254 "sql.ypp"
    { emit("STMT"); ;}
    break;

  case 295:

/* Line 936 of glr.c  */
#line 1265 "sql.ypp"
    { ((*yyvalp).intval) = 01; ;}
    break;

  case 296:

/* Line 936 of glr.c  */
#line 1266 "sql.ypp"
    { ((*yyvalp).intval) = 02; ;}
    break;

  case 297:

/* Line 936 of glr.c  */
#line 1267 "sql.ypp"
    { ((*yyvalp).intval) = 04; ;}
    break;

  case 298:

/* Line 936 of glr.c  */
#line 1268 "sql.ypp"
    { ((*yyvalp).intval) = 010; ;}
    break;

  case 299:

/* Line 936 of glr.c  */
#line 1269 "sql.ypp"
    { ((*yyvalp).intval) = 020; ;}
    break;

  case 300:

/* Line 936 of glr.c  */
#line 1270 "sql.ypp"
    { ((*yyvalp).intval) = 040; ;}
    break;

  case 301:

/* Line 936 of glr.c  */
#line 1271 "sql.ypp"
    { ((*yyvalp).intval) = 0100; ;}
    break;

  case 302:

/* Line 936 of glr.c  */
#line 1272 "sql.ypp"
    { ((*yyvalp).intval) = 0200; ;}
    break;

  case 303:

/* Line 936 of glr.c  */
#line 1273 "sql.ypp"
    { ((*yyvalp).intval) = 0400; ;}
    break;

  case 304:

/* Line 936 of glr.c  */
#line 1274 "sql.ypp"
    { ((*yyvalp).intval) = 01000; ;}
    break;

  case 305:

/* Line 936 of glr.c  */
#line 1275 "sql.ypp"
    { ((*yyvalp).intval) = 02000; ;}
    break;

  case 306:

/* Line 936 of glr.c  */
#line 1276 "sql.ypp"
    { ((*yyvalp).intval) = 04000; ;}
    break;

  case 314:

/* Line 936 of glr.c  */
#line 1292 "sql.ypp"
    { emit("STMT"); ;}
    break;

  case 316:

/* Line 936 of glr.c  */
#line 1299 "sql.ypp"
    { emit("STMT"); ;}
    break;

  case 329:

/* Line 936 of glr.c  */
#line 1328 "sql.ypp"
    { emit("STMT"); ;}
    break;

  case 331:

/* Line 936 of glr.c  */
#line 1334 "sql.ypp"
    { emit("STMT"); ;}
    break;

  case 333:

/* Line 936 of glr.c  */
#line 1340 "sql.ypp"
    { emit("STMT"); ;}
    break;

  case 335:

/* Line 936 of glr.c  */
#line 1347 "sql.ypp"
    { emit("STMT"); ;}
    break;

  case 339:

/* Line 936 of glr.c  */
#line 1359 "sql.ypp"
    { emit("STMT"); ;}
    break;

  case 343:

/* Line 936 of glr.c  */
#line 1370 "sql.ypp"
    { emit("STMT"); ;}
    break;

  case 345:

/* Line 936 of glr.c  */
#line 1376 "sql.ypp"
    { ((*yyvalp).intval) = 0; ;}
    break;

  case 346:

/* Line 936 of glr.c  */
#line 1377 "sql.ypp"
    { ((*yyvalp).intval) = 1; ;}
    break;

  case 347:

/* Line 936 of glr.c  */
#line 1378 "sql.ypp"
    { ((*yyvalp).intval) = 2; ;}
    break;

  case 348:

/* Line 936 of glr.c  */
#line 1382 "sql.ypp"
    { emit("STMT"); ;}
    break;

  case 350:

/* Line 936 of glr.c  */
#line 1389 "sql.ypp"
    { emit("STMT"); ;}
    break;

  case 352:

/* Line 936 of glr.c  */
#line 1395 "sql.ypp"
    { ((*yyvalp).intval) = 0; ;}
    break;

  case 353:

/* Line 936 of glr.c  */
#line 1396 "sql.ypp"
    { ((*yyvalp).intval) = 1; ;}
    break;

  case 354:

/* Line 936 of glr.c  */
#line 1397 "sql.ypp"
    { ((*yyvalp).intval) = 2; ;}
    break;

  case 355:

/* Line 936 of glr.c  */
#line 1398 "sql.ypp"
    { ((*yyvalp).intval) = 3; ;}
    break;

  case 356:

/* Line 936 of glr.c  */
#line 1399 "sql.ypp"
    { ((*yyvalp).intval) = 4; ;}
    break;

  case 357:

/* Line 936 of glr.c  */
#line 1400 "sql.ypp"
    { ((*yyvalp).intval) = 5; ;}
    break;

  case 358:

/* Line 936 of glr.c  */
#line 1404 "sql.ypp"
    { emit("STMT"); ;}
    break;

  case 360:

/* Line 936 of glr.c  */
#line 1410 "sql.ypp"
    { ((*yyvalp).intval) = 0; ;}
    break;

  case 361:

/* Line 936 of glr.c  */
#line 1411 "sql.ypp"
    { ((*yyvalp).intval) = 1; ;}
    break;

  case 362:

/* Line 936 of glr.c  */
#line 1412 "sql.ypp"
    { ((*yyvalp).intval) = 2; ;}
    break;

  case 363:

/* Line 936 of glr.c  */
#line 1416 "sql.ypp"
    { emit("STMT"); ;}
    break;

  case 365:

/* Line 936 of glr.c  */
#line 1423 "sql.ypp"
    { emit("STMT"); ;}
    break;

  case 371:

/* Line 936 of glr.c  */
#line 1436 "sql.ypp"
    { emit("STMT"); ;}
    break;

  case 401:

/* Line 936 of glr.c  */
#line 1475 "sql.ypp"
    { ((*yyvalp).intval) = 0; ;}
    break;

  case 402:

/* Line 936 of glr.c  */
#line 1476 "sql.ypp"
    { ((*yyvalp).intval) = 1; ;}
    break;

  case 417:

/* Line 936 of glr.c  */
#line 1510 "sql.ypp"
    { ((*yyvalp).intval) = 0; ;}
    break;

  case 418:

/* Line 936 of glr.c  */
#line 1511 "sql.ypp"
    { ((*yyvalp).intval) = 1; ;}
    break;

  case 419:

/* Line 936 of glr.c  */
#line 1512 "sql.ypp"
    { ((*yyvalp).intval) = 2; ;}
    break;

  case 424:

/* Line 936 of glr.c  */
#line 1526 "sql.ypp"
    { ((*yyvalp).intval) = 1; ;}
    break;

  case 425:

/* Line 936 of glr.c  */
#line 1527 "sql.ypp"
    { ((*yyvalp).intval) = 2; ;}
    break;

  case 426:

/* Line 936 of glr.c  */
#line 1528 "sql.ypp"
    { ((*yyvalp).intval) = 04; ;}
    break;

  case 427:

/* Line 936 of glr.c  */
#line 1529 "sql.ypp"
    { ((*yyvalp).intval) = 010; ;}
    break;

  case 428:

/* Line 936 of glr.c  */
#line 1530 "sql.ypp"
    { ((*yyvalp).intval) = 020; ;}
    break;

  case 429:

/* Line 936 of glr.c  */
#line 1531 "sql.ypp"
    { ((*yyvalp).intval) = 040; ;}
    break;

  case 430:

/* Line 936 of glr.c  */
#line 1532 "sql.ypp"
    { ((*yyvalp).intval) = 0100; ;}
    break;

  case 431:

/* Line 936 of glr.c  */
#line 1533 "sql.ypp"
    { ((*yyvalp).intval) = 0200; ;}
    break;

  case 432:

/* Line 936 of glr.c  */
#line 1534 "sql.ypp"
    { ((*yyvalp).intval) = 0400; ;}
    break;

  case 433:

/* Line 936 of glr.c  */
#line 1535 "sql.ypp"
    { ((*yyvalp).intval) = 01000; ;}
    break;

  case 436:

/* Line 936 of glr.c  */
#line 1545 "sql.ypp"
    { ((*yyvalp).intval) = 0; ;}
    break;

  case 437:

/* Line 936 of glr.c  */
#line 1546 "sql.ypp"
    { ((*yyvalp).intval) = 1; ;}
    break;

  case 438:

/* Line 936 of glr.c  */
#line 1547 "sql.ypp"
    { ((*yyvalp).intval) = 2; ;}
    break;

  case 449:

/* Line 936 of glr.c  */
#line 1578 "sql.ypp"
    { ((*yyvalp).intval) = 1; ;}
    break;

  case 450:

/* Line 936 of glr.c  */
#line 1579 "sql.ypp"
    { ((*yyvalp).intval) = 2; ;}
    break;

  case 451:

/* Line 936 of glr.c  */
#line 1580 "sql.ypp"
    { ((*yyvalp).intval) = 3; ;}
    break;

  case 460:

/* Line 936 of glr.c  */
#line 1605 "sql.ypp"
    { emit("STMT"); ;}
    break;

  case 464:

/* Line 936 of glr.c  */
#line 1614 "sql.ypp"
    { if ((((yyGLRStackItem const *)yyvsp)[YYFILL ((2) - (3))].yystate.yysemantics.yysval.subtok) != 4) yyerror("bad set to @%s", (((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (3))].yystate.yysemantics.yysval.strval));
		emit("SET %s", (((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (3))].yystate.yysemantics.yysval.strval)); free((((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (3))].yystate.yysemantics.yysval.strval));
	;}
    break;

  case 465:

/* Line 936 of glr.c  */
#line 1618 "sql.ypp"
    {
    	emit("SET %s", (((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (3))].yystate.yysemantics.yysval.strval)); 
    	free((((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (3))].yystate.yysemantics.yysval.strval)); 
    ;}
    break;

  case 466:

/* Line 936 of glr.c  */
#line 1625 "sql.ypp"
    { ((*yyvalp).ast_node) = NULL; ;}
    break;

  case 467:

/* Line 936 of glr.c  */
#line 1626 "sql.ypp"
    { ((*yyvalp).ast_node) = newLength (t_length, (((yyGLRStackItem const *)yyvsp)[YYFILL ((2) - (3))].yystate.yysemantics.yysval.intval), 0); ;}
    break;

  case 468:

/* Line 936 of glr.c  */
#line 1627 "sql.ypp"
    { ((*yyvalp).ast_node) = newLength (t_length, (((yyGLRStackItem const *)yyvsp)[YYFILL ((2) - (5))].yystate.yysemantics.yysval.intval), (((yyGLRStackItem const *)yyvsp)[YYFILL ((4) - (5))].yystate.yysemantics.yysval.intval)); ;}
    break;

  case 469:

/* Line 936 of glr.c  */
#line 1630 "sql.ypp"
    { ((*yyvalp).intval) = 0; ;}
    break;

  case 470:

/* Line 936 of glr.c  */
#line 1631 "sql.ypp"
    { ((*yyvalp).intval) = 1; ;}
    break;

  case 471:

/* Line 936 of glr.c  */
#line 1634 "sql.ypp"
    { ((*yyvalp).intval) = 0; ;}
    break;

  case 472:

/* Line 936 of glr.c  */
#line 1635 "sql.ypp"
    { ((*yyvalp).intval) = (((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (2))].yystate.yysemantics.yysval.intval) | 01; ;}
    break;

  case 473:

/* Line 936 of glr.c  */
#line 1636 "sql.ypp"
    { ((*yyvalp).intval) = (((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (2))].yystate.yysemantics.yysval.intval) | 02; ;}
    break;

  case 474:

/* Line 936 of glr.c  */
#line 1639 "sql.ypp"
    { ((*yyvalp).ast_node) =newOptCsc(t_opt_csc, 0, NULL, NULL);;}
    break;

  case 475:

/* Line 936 of glr.c  */
#line 1641 "sql.ypp"
    { Opt_csc * t = (Opt_csc *)(((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (4))].yystate.yysemantics.yysval.ast_node); t->datatype = t->datatype | 01; t->s1 = (((yyGLRStackItem const *)yyvsp)[YYFILL ((4) - (4))].yystate.yysemantics.yysval.strval); ((*yyvalp).ast_node) = (Node *)t; ;}
    break;

  case 476:

/* Line 936 of glr.c  */
#line 1643 "sql.ypp"
    { Opt_csc * t = (Opt_csc *)(((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (3))].yystate.yysemantics.yysval.ast_node); t->datatype = t->datatype | 02; t->s2 = (((yyGLRStackItem const *)yyvsp)[YYFILL ((3) - (3))].yystate.yysemantics.yysval.strval); ((*yyvalp).ast_node) = (Node *)t; ;}
    break;

  case 477:

/* Line 936 of glr.c  */
#line 1647 "sql.ypp"
    { ((*yyvalp).ast_node) = newDatatype (t_datatype, 1, (((yyGLRStackItem const *)yyvsp)[YYFILL ((2) - (2))].yystate.yysemantics.yysval.ast_node), 0, NULL, 0, NULL); ;}
    break;

  case 478:

/* Line 936 of glr.c  */
#line 1648 "sql.ypp"
    { ((*yyvalp).ast_node) = newDatatype (t_datatype, 2, (((yyGLRStackItem const *)yyvsp)[YYFILL ((2) - (3))].yystate.yysemantics.yysval.ast_node), (((yyGLRStackItem const *)yyvsp)[YYFILL ((3) - (3))].yystate.yysemantics.yysval.intval), NULL, 0, NULL); ;}
    break;

  case 479:

/* Line 936 of glr.c  */
#line 1649 "sql.ypp"
    { ((*yyvalp).ast_node) = newDatatype (t_datatype, 3, (((yyGLRStackItem const *)yyvsp)[YYFILL ((2) - (3))].yystate.yysemantics.yysval.ast_node), (((yyGLRStackItem const *)yyvsp)[YYFILL ((3) - (3))].yystate.yysemantics.yysval.intval), NULL, 0, NULL); ;}
    break;

  case 480:

/* Line 936 of glr.c  */
#line 1650 "sql.ypp"
    { ((*yyvalp).ast_node) = newDatatype (t_datatype, 4, (((yyGLRStackItem const *)yyvsp)[YYFILL ((2) - (3))].yystate.yysemantics.yysval.ast_node), (((yyGLRStackItem const *)yyvsp)[YYFILL ((3) - (3))].yystate.yysemantics.yysval.intval), NULL, 0, NULL); ;}
    break;

  case 481:

/* Line 936 of glr.c  */
#line 1651 "sql.ypp"
    { ((*yyvalp).ast_node) = newDatatype (t_datatype, 5, (((yyGLRStackItem const *)yyvsp)[YYFILL ((2) - (3))].yystate.yysemantics.yysval.ast_node), (((yyGLRStackItem const *)yyvsp)[YYFILL ((3) - (3))].yystate.yysemantics.yysval.intval), NULL, 0, NULL); ;}
    break;

  case 482:

/* Line 936 of glr.c  */
#line 1652 "sql.ypp"
    { ((*yyvalp).ast_node) = newDatatype (t_datatype, 6, (((yyGLRStackItem const *)yyvsp)[YYFILL ((2) - (3))].yystate.yysemantics.yysval.ast_node), (((yyGLRStackItem const *)yyvsp)[YYFILL ((3) - (3))].yystate.yysemantics.yysval.intval), NULL, 0, NULL); ;}
    break;

  case 483:

/* Line 936 of glr.c  */
#line 1653 "sql.ypp"
    { ((*yyvalp).ast_node) = newDatatype (t_datatype, 7, (((yyGLRStackItem const *)yyvsp)[YYFILL ((2) - (3))].yystate.yysemantics.yysval.ast_node), (((yyGLRStackItem const *)yyvsp)[YYFILL ((3) - (3))].yystate.yysemantics.yysval.intval), NULL, 0, NULL); ;}
    break;

  case 484:

/* Line 936 of glr.c  */
#line 1654 "sql.ypp"
    { ((*yyvalp).ast_node) = newDatatype (t_datatype, 8, (((yyGLRStackItem const *)yyvsp)[YYFILL ((2) - (3))].yystate.yysemantics.yysval.ast_node), (((yyGLRStackItem const *)yyvsp)[YYFILL ((3) - (3))].yystate.yysemantics.yysval.intval), NULL, 0, NULL); ;}
    break;

  case 485:

/* Line 936 of glr.c  */
#line 1655 "sql.ypp"
    { ((*yyvalp).ast_node) = newDatatype (t_datatype, 9, (((yyGLRStackItem const *)yyvsp)[YYFILL ((2) - (3))].yystate.yysemantics.yysval.ast_node), (((yyGLRStackItem const *)yyvsp)[YYFILL ((3) - (3))].yystate.yysemantics.yysval.intval), NULL, 0, NULL); ;}
    break;

  case 486:

/* Line 936 of glr.c  */
#line 1656 "sql.ypp"
    { ((*yyvalp).ast_node) = newDatatype (t_datatype, 10, (((yyGLRStackItem const *)yyvsp)[YYFILL ((2) - (3))].yystate.yysemantics.yysval.ast_node), (((yyGLRStackItem const *)yyvsp)[YYFILL ((3) - (3))].yystate.yysemantics.yysval.intval), NULL, 0, NULL); ;}
    break;

  case 487:

/* Line 936 of glr.c  */
#line 1657 "sql.ypp"
    { ((*yyvalp).ast_node) = newDatatype (t_datatype, 11, (((yyGLRStackItem const *)yyvsp)[YYFILL ((2) - (3))].yystate.yysemantics.yysval.ast_node), (((yyGLRStackItem const *)yyvsp)[YYFILL ((3) - (3))].yystate.yysemantics.yysval.intval), NULL, 0, NULL); ;}
    break;

  case 488:

/* Line 936 of glr.c  */
#line 1658 "sql.ypp"
    { ((*yyvalp).ast_node) = newDatatype (t_datatype, 12, NULL, 0, NULL, 0, NULL); ;}
    break;

  case 489:

/* Line 936 of glr.c  */
#line 1659 "sql.ypp"
    { ((*yyvalp).ast_node) = newDatatype (t_datatype, 13, NULL, 0, NULL, 0, NULL); ;}
    break;

  case 490:

/* Line 936 of glr.c  */
#line 1660 "sql.ypp"
    { ((*yyvalp).ast_node) = newDatatype (t_datatype, 14, NULL, 0, NULL, 0, NULL); ;}
    break;

  case 491:

/* Line 936 of glr.c  */
#line 1661 "sql.ypp"
    { ((*yyvalp).ast_node) = newDatatype (t_datatype, 15, NULL, 0, NULL, 0, NULL); ;}
    break;

  case 492:

/* Line 936 of glr.c  */
#line 1662 "sql.ypp"
    { ((*yyvalp).ast_node) = newDatatype (t_datatype, 16, NULL, 0, NULL, 0, NULL); ;}
    break;

  case 493:

/* Line 936 of glr.c  */
#line 1663 "sql.ypp"
    { ((*yyvalp).ast_node) = newDatatype (t_datatype, 17, (((yyGLRStackItem const *)yyvsp)[YYFILL ((2) - (3))].yystate.yysemantics.yysval.ast_node), 0, (((yyGLRStackItem const *)yyvsp)[YYFILL ((3) - (3))].yystate.yysemantics.yysval.ast_node), 0, NULL); ;}
    break;

  case 494:

/* Line 936 of glr.c  */
#line 1664 "sql.ypp"
    { Node * t = newLength(t_datatype, (((yyGLRStackItem const *)yyvsp)[YYFILL ((3) - (5))].yystate.yysemantics.yysval.intval), NULL); ((*yyvalp).ast_node) = newDatatype (t_datatype, 18, t, 0, (((yyGLRStackItem const *)yyvsp)[YYFILL ((5) - (5))].yystate.yysemantics.yysval.ast_node), 0, NULL); ;}
    break;

  case 495:

/* Line 936 of glr.c  */
#line 1665 "sql.ypp"
    { ((*yyvalp).ast_node) = newDatatype (t_datatype, 19, (((yyGLRStackItem const *)yyvsp)[YYFILL ((2) - (2))].yystate.yysemantics.yysval.ast_node), 0, NULL, 0, NULL); ;}
    break;

  case 496:

/* Line 936 of glr.c  */
#line 1666 "sql.ypp"
    {  Node * t = newLength(t_datatype, (((yyGLRStackItem const *)yyvsp)[YYFILL ((3) - (4))].yystate.yysemantics.yysval.intval), NULL); ((*yyvalp).ast_node) = newDatatype (t_datatype, 20, t, 0, NULL, 0, NULL); ;}
    break;

  case 497:

/* Line 936 of glr.c  */
#line 1667 "sql.ypp"
    { ((*yyvalp).ast_node) = newDatatype (t_datatype, 21, NULL, 0, NULL, 0, NULL); ;}
    break;

  case 498:

/* Line 936 of glr.c  */
#line 1668 "sql.ypp"
    { ((*yyvalp).ast_node) = newDatatype (t_datatype, 22, NULL, 0, NULL, 0, NULL); ;}
    break;

  case 499:

/* Line 936 of glr.c  */
#line 1669 "sql.ypp"
    { ((*yyvalp).ast_node) = newDatatype (t_datatype, 23, NULL, 0, NULL, 0, NULL); ;}
    break;

  case 500:

/* Line 936 of glr.c  */
#line 1670 "sql.ypp"
    { ((*yyvalp).ast_node) = newDatatype (t_datatype, 24, NULL, 0, NULL, 0, NULL); ;}
    break;

  case 501:

/* Line 936 of glr.c  */
#line 1671 "sql.ypp"
    { ((*yyvalp).ast_node) = newDatatype (t_datatype, 25, NULL, 0, (((yyGLRStackItem const *)yyvsp)[YYFILL ((3) - (3))].yystate.yysemantics.yysval.ast_node), (((yyGLRStackItem const *)yyvsp)[YYFILL ((2) - (3))].yystate.yysemantics.yysval.intval), NULL); ;}
    break;

  case 502:

/* Line 936 of glr.c  */
#line 1672 "sql.ypp"
    { ((*yyvalp).ast_node) = newDatatype (t_datatype, 26, NULL, 0, (((yyGLRStackItem const *)yyvsp)[YYFILL ((3) - (3))].yystate.yysemantics.yysval.ast_node), (((yyGLRStackItem const *)yyvsp)[YYFILL ((2) - (3))].yystate.yysemantics.yysval.intval), NULL); ;}
    break;

  case 503:

/* Line 936 of glr.c  */
#line 1673 "sql.ypp"
    { ((*yyvalp).ast_node) = newDatatype (t_datatype, 27, NULL, 0, (((yyGLRStackItem const *)yyvsp)[YYFILL ((3) - (3))].yystate.yysemantics.yysval.ast_node), (((yyGLRStackItem const *)yyvsp)[YYFILL ((2) - (3))].yystate.yysemantics.yysval.intval), NULL); ;}
    break;

  case 504:

/* Line 936 of glr.c  */
#line 1674 "sql.ypp"
    { ((*yyvalp).ast_node) = newDatatype (t_datatype, 28, NULL, 0, (((yyGLRStackItem const *)yyvsp)[YYFILL ((3) - (3))].yystate.yysemantics.yysval.ast_node), (((yyGLRStackItem const *)yyvsp)[YYFILL ((2) - (3))].yystate.yysemantics.yysval.intval), NULL); ;}
    break;

  case 505:

/* Line 936 of glr.c  */
#line 1675 "sql.ypp"
    { ((*yyvalp).ast_node) = newDatatype (t_datatype, 29, NULL, 0, (((yyGLRStackItem const *)yyvsp)[YYFILL ((5) - (5))].yystate.yysemantics.yysval.ast_node), 0, (((yyGLRStackItem const *)yyvsp)[YYFILL ((3) - (5))].yystate.yysemantics.yysval.ast_node)); ;}
    break;

  case 506:

/* Line 936 of glr.c  */
#line 1676 "sql.ypp"
    { ((*yyvalp).ast_node) = newDatatype (t_datatype, 30, NULL, 0, (((yyGLRStackItem const *)yyvsp)[YYFILL ((5) - (5))].yystate.yysemantics.yysval.ast_node), 0, (((yyGLRStackItem const *)yyvsp)[YYFILL ((3) - (5))].yystate.yysemantics.yysval.ast_node)); ;}
    break;

  case 507:

/* Line 936 of glr.c  */
#line 1679 "sql.ypp"
    { ((*yyvalp).ast_node) = newEnumList( t_enum_list, (((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (1))].yystate.yysemantics.yysval.strval), NULL); ;}
    break;

  case 508:

/* Line 936 of glr.c  */
#line 1680 "sql.ypp"
    { ((*yyvalp).ast_node) = newEnumList( t_enum_list, (((yyGLRStackItem const *)yyvsp)[YYFILL ((3) - (3))].yystate.yysemantics.yysval.strval), (((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (3))].yystate.yysemantics.yysval.ast_node)); ;}
    break;

  case 509:

/* Line 936 of glr.c  */
#line 1686 "sql.ypp"
    { ((*yyvalp).ast_node) = newColumn(t_name, NULL, (((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (1))].yystate.yysemantics.yysval.strval), NULL); /*puts("expr1");*/ ;}
    break;

  case 510:

/* Line 936 of glr.c  */
#line 1687 "sql.ypp"
    { dataval d; d.string_val = (((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (1))].yystate.yysemantics.yysval.strval); ((*yyvalp).ast_node) = newExpr(t_name, d); ;}
    break;

  case 511:

/* Line 936 of glr.c  */
#line 1688 "sql.ypp"
    { ((*yyvalp).ast_node) = newColumn(t_name_name, (((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (3))].yystate.yysemantics.yysval.strval), (((yyGLRStackItem const *)yyvsp)[YYFILL ((3) - (3))].yystate.yysemantics.yysval.strval), NULL); /*puts("expr2");*/ ;}
    break;

  case 512:

/* Line 936 of glr.c  */
#line 1689 "sql.ypp"
    { ((*yyvalp).ast_node) = newColumn(t_name_name, (((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (3))].yystate.yysemantics.yysval.strval), "*", NULL);/* puts("expr22");*/;}
    break;

  case 513:

/* Line 936 of glr.c  */
#line 1690 "sql.ypp"
    { dataval d; d.string_val = (((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (1))].yystate.yysemantics.yysval.strval); ((*yyvalp).ast_node) = newExpr(t_stringval, d); /*puts("expr3");*/ ;}
    break;

  case 514:

/* Line 936 of glr.c  */
#line 1691 "sql.ypp"
    { dataval d; d.int_val=(((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (1))].yystate.yysemantics.yysval.intval); ((*yyvalp).ast_node) = newExpr(t_intnum, d); /*puts("expr4");*/ ;}
    break;

  case 515:

/* Line 936 of glr.c  */
#line 1692 "sql.ypp"
    { dataval d; d.double_val = (((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (1))].yystate.yysemantics.yysval.floatval); ((*yyvalp).ast_node) = newExpr(t_approxnum, d); ;}
    break;

  case 516:

/* Line 936 of glr.c  */
#line 1693 "sql.ypp"
    { dataval d; d.bool_val = (((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (1))].yystate.yysemantics.yysval.intval); ((*yyvalp).ast_node) = newExpr(t_bool, d); ;}
    break;

  case 517:

/* Line 936 of glr.c  */
#line 1696 "sql.ypp"
    { ((*yyvalp).ast_node)=newExprCal(t_expr_cal, "+", NULL, 0, (((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (3))].yystate.yysemantics.yysval.ast_node), (((yyGLRStackItem const *)yyvsp)[YYFILL ((3) - (3))].yystate.yysemantics.yysval.ast_node)); ;}
    break;

  case 518:

/* Line 936 of glr.c  */
#line 1697 "sql.ypp"
    { ((*yyvalp).ast_node)=newExprCal(t_expr_cal, "-", NULL, 0, (((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (3))].yystate.yysemantics.yysval.ast_node), (((yyGLRStackItem const *)yyvsp)[YYFILL ((3) - (3))].yystate.yysemantics.yysval.ast_node)); ;}
    break;

  case 519:

/* Line 936 of glr.c  */
#line 1698 "sql.ypp"
    { ((*yyvalp).ast_node)=newExprCal(t_expr_cal, "*", NULL, 0, (((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (3))].yystate.yysemantics.yysval.ast_node), (((yyGLRStackItem const *)yyvsp)[YYFILL ((3) - (3))].yystate.yysemantics.yysval.ast_node)); ;}
    break;

  case 520:

/* Line 936 of glr.c  */
#line 1699 "sql.ypp"
    { ((*yyvalp).ast_node)=newExprCal(t_expr_cal, "/", NULL, 0, (((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (3))].yystate.yysemantics.yysval.ast_node), (((yyGLRStackItem const *)yyvsp)[YYFILL ((3) - (3))].yystate.yysemantics.yysval.ast_node)); ;}
    break;

  case 521:

/* Line 936 of glr.c  */
#line 1700 "sql.ypp"
    { ((*yyvalp).ast_node)=newExprCal(t_expr_cal, "%", NULL, 0, (((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (3))].yystate.yysemantics.yysval.ast_node), (((yyGLRStackItem const *)yyvsp)[YYFILL ((3) - (3))].yystate.yysemantics.yysval.ast_node)); ;}
    break;

  case 522:

/* Line 936 of glr.c  */
#line 1701 "sql.ypp"
    { ((*yyvalp).ast_node)=newExprCal(t_expr_cal, "MOD", NULL, 0, (((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (3))].yystate.yysemantics.yysval.ast_node), (((yyGLRStackItem const *)yyvsp)[YYFILL ((3) - (3))].yystate.yysemantics.yysval.ast_node)); ;}
    break;

  case 523:

/* Line 936 of glr.c  */
#line 1702 "sql.ypp"
    { ((*yyvalp).ast_node)=newExprCal(t_expr_cal, "--", NULL, 0, NULL, (((yyGLRStackItem const *)yyvsp)[YYFILL ((2) - (2))].yystate.yysemantics.yysval.ast_node)); ;}
    break;

  case 524:

/* Line 936 of glr.c  */
#line 1703 "sql.ypp"
    { ((*yyvalp).ast_node)=newExprCal(t_expr_cal, "++", NULL, 0, NULL, (((yyGLRStackItem const *)yyvsp)[YYFILL ((2) - (2))].yystate.yysemantics.yysval.ast_node)); ;}
    break;

  case 525:

/* Line 936 of glr.c  */
#line 1704 "sql.ypp"
    { ((*yyvalp).ast_node)=newExprCal(t_expr_cal, "ANDOP", NULL, 0, (((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (3))].yystate.yysemantics.yysval.ast_node), (((yyGLRStackItem const *)yyvsp)[YYFILL ((3) - (3))].yystate.yysemantics.yysval.ast_node)); ;}
    break;

  case 526:

/* Line 936 of glr.c  */
#line 1705 "sql.ypp"
    { ((*yyvalp).ast_node)=newExprCal(t_expr_cal, "OR", NULL, 0, (((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (3))].yystate.yysemantics.yysval.ast_node), (((yyGLRStackItem const *)yyvsp)[YYFILL ((3) - (3))].yystate.yysemantics.yysval.ast_node)); ;}
    break;

  case 527:

/* Line 936 of glr.c  */
#line 1706 "sql.ypp"
    { ((*yyvalp).ast_node)=newExprCal(t_expr_cal, "XOR", NULL, 0, (((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (3))].yystate.yysemantics.yysval.ast_node), (((yyGLRStackItem const *)yyvsp)[YYFILL ((3) - (3))].yystate.yysemantics.yysval.ast_node)); ;}
    break;

  case 528:

/* Line 936 of glr.c  */
#line 1707 "sql.ypp"
    { ((*yyvalp).ast_node) = newExprCal(t_expr_cal, "CMP", NULL, (((yyGLRStackItem const *)yyvsp)[YYFILL ((2) - (3))].yystate.yysemantics.yysval.subtok), (((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (3))].yystate.yysemantics.yysval.ast_node), (((yyGLRStackItem const *)yyvsp)[YYFILL ((3) - (3))].yystate.yysemantics.yysval.ast_node));/* puts("=");*/ ;}
    break;

  case 529:

/* Line 936 of glr.c  */
#line 1708 "sql.ypp"
    { ((*yyvalp).ast_node)=newExprCal(t_expr_cal, "CMP", NULL, (((yyGLRStackItem const *)yyvsp)[YYFILL ((2) - (5))].yystate.yysemantics.yysval.subtok), (((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (5))].yystate.yysemantics.yysval.ast_node), (((yyGLRStackItem const *)yyvsp)[YYFILL ((4) - (5))].yystate.yysemantics.yysval.ast_node)); ;}
    break;

  case 530:

/* Line 936 of glr.c  */
#line 1709 "sql.ypp"
    { ((*yyvalp).ast_node)=newExprCal(t_expr_cal, "CMP", "ANY", (((yyGLRStackItem const *)yyvsp)[YYFILL ((2) - (6))].yystate.yysemantics.yysval.subtok), (((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (6))].yystate.yysemantics.yysval.ast_node), (((yyGLRStackItem const *)yyvsp)[YYFILL ((5) - (6))].yystate.yysemantics.yysval.ast_node)); ;}
    break;

  case 531:

/* Line 936 of glr.c  */
#line 1710 "sql.ypp"
    { ((*yyvalp).ast_node)=newExprCal(t_expr_cal, "CMP", "SOME", (((yyGLRStackItem const *)yyvsp)[YYFILL ((2) - (6))].yystate.yysemantics.yysval.subtok), (((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (6))].yystate.yysemantics.yysval.ast_node), (((yyGLRStackItem const *)yyvsp)[YYFILL ((5) - (6))].yystate.yysemantics.yysval.ast_node)); ;}
    break;

  case 532:

/* Line 936 of glr.c  */
#line 1711 "sql.ypp"
    { ((*yyvalp).ast_node)=newExprCal(t_expr_cal, "CMP", "ALL", (((yyGLRStackItem const *)yyvsp)[YYFILL ((2) - (6))].yystate.yysemantics.yysval.subtok), (((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (6))].yystate.yysemantics.yysval.ast_node), (((yyGLRStackItem const *)yyvsp)[YYFILL ((5) - (6))].yystate.yysemantics.yysval.ast_node)); ;}
    break;

  case 533:

/* Line 936 of glr.c  */
#line 1712 "sql.ypp"
    { ((*yyvalp).ast_node)=newExprCal(t_expr_cal, "|", NULL, 0, (((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (3))].yystate.yysemantics.yysval.ast_node), (((yyGLRStackItem const *)yyvsp)[YYFILL ((3) - (3))].yystate.yysemantics.yysval.ast_node)); ;}
    break;

  case 534:

/* Line 936 of glr.c  */
#line 1713 "sql.ypp"
    { ((*yyvalp).ast_node)=newExprCal(t_expr_cal, "&", NULL, 0, (((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (3))].yystate.yysemantics.yysval.ast_node), (((yyGLRStackItem const *)yyvsp)[YYFILL ((3) - (3))].yystate.yysemantics.yysval.ast_node)); ;}
    break;

  case 535:

/* Line 936 of glr.c  */
#line 1714 "sql.ypp"
    { ((*yyvalp).ast_node)=newExprCal(t_expr_cal, "^", NULL, 0, (((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (3))].yystate.yysemantics.yysval.ast_node), (((yyGLRStackItem const *)yyvsp)[YYFILL ((3) - (3))].yystate.yysemantics.yysval.ast_node)); ;}
    break;

  case 536:

/* Line 936 of glr.c  */
#line 1715 "sql.ypp"
    { ((*yyvalp).ast_node)=newExprCal(t_expr_cal, (SHIFT==1?"LS":"RS"), NULL, 0, (((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (3))].yystate.yysemantics.yysval.ast_node), (((yyGLRStackItem const *)yyvsp)[YYFILL ((3) - (3))].yystate.yysemantics.yysval.ast_node)); ;}
    break;

  case 537:

/* Line 936 of glr.c  */
#line 1716 "sql.ypp"
    { ((*yyvalp).ast_node)=newExprCal(t_expr_cal, "NOT", NULL, 0, NULL, (((yyGLRStackItem const *)yyvsp)[YYFILL ((2) - (2))].yystate.yysemantics.yysval.ast_node)); ;}
    break;

  case 538:

/* Line 936 of glr.c  */
#line 1717 "sql.ypp"
    { ((*yyvalp).ast_node)=newExprCal(t_expr_cal, "!", NULL, 0, NULL, (((yyGLRStackItem const *)yyvsp)[YYFILL ((2) - (2))].yystate.yysemantics.yysval.ast_node)); ;}
    break;

  case 539:

/* Line 936 of glr.c  */
#line 1718 "sql.ypp"
    { ((*yyvalp).ast_node)=NULL; ;}
    break;

  case 540:

/* Line 936 of glr.c  */
#line 1719 "sql.ypp"
    { ((*yyvalp).ast_node) = (((yyGLRStackItem const *)yyvsp)[YYFILL ((2) - (3))].yystate.yysemantics.yysval.ast_node); ;}
    break;

  case 541:

/* Line 936 of glr.c  */
#line 1722 "sql.ypp"
    { ((*yyvalp).ast_node)=newExprFunc(t_expr_func, "ISN", (((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (3))].yystate.yysemantics.yysval.ast_node), NULL, NULL, NULL); ;}
    break;

  case 542:

/* Line 936 of glr.c  */
#line 1723 "sql.ypp"
    { ((*yyvalp).ast_node)=newExprFunc(t_expr_func, "ISNN", (((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (4))].yystate.yysemantics.yysval.ast_node), NULL, NULL, NULL); ;}
    break;

  case 543:

/* Line 936 of glr.c  */
#line 1724 "sql.ypp"
    { ((*yyvalp).ast_node)=newExprFunc(t_expr_func, "ISB", (((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (3))].yystate.yysemantics.yysval.ast_node), NULL, NULL, NULL); ;}
    break;

  case 544:

/* Line 936 of glr.c  */
#line 1725 "sql.ypp"
    { ((*yyvalp).ast_node)=newExprFunc(t_expr_func, "ISNB", (((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (4))].yystate.yysemantics.yysval.ast_node), NULL, NULL, NULL); ;}
    break;

  case 545:

/* Line 936 of glr.c  */
#line 1728 "sql.ypp"
    { ((*yyvalp).ast_node)=newExprFunc(t_expr_func, "BA", (((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (5))].yystate.yysemantics.yysval.ast_node), (((yyGLRStackItem const *)yyvsp)[YYFILL ((3) - (5))].yystate.yysemantics.yysval.ast_node), (((yyGLRStackItem const *)yyvsp)[YYFILL ((5) - (5))].yystate.yysemantics.yysval.ast_node), NULL); ;}
    break;

  case 546:

/* Line 936 of glr.c  */
#line 1732 "sql.ypp"
    { ((*yyvalp).ast_node) = newExprList(t_expr_list, (((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (1))].yystate.yysemantics.yysval.ast_node), NULL); ;}
    break;

  case 547:

/* Line 936 of glr.c  */
#line 1733 "sql.ypp"
    { ((*yyvalp).ast_node) = newExprList(t_expr_list, (((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (3))].yystate.yysemantics.yysval.ast_node), (((yyGLRStackItem const *)yyvsp)[YYFILL ((3) - (3))].yystate.yysemantics.yysval.ast_node)); ;}
    break;

  case 549:

/* Line 936 of glr.c  */
#line 1737 "sql.ypp"
    {((*yyvalp).ast_node)=(((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (1))].yystate.yysemantics.yysval.ast_node);;}
    break;

  case 550:

/* Line 936 of glr.c  */
#line 1740 "sql.ypp"
    { ((*yyvalp).ast_node)=newExprCal(t_expr_cal, "INV", NULL, 0, (((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (5))].yystate.yysemantics.yysval.ast_node), (((yyGLRStackItem const *)yyvsp)[YYFILL ((4) - (5))].yystate.yysemantics.yysval.ast_node)); ;}
    break;

  case 551:

/* Line 936 of glr.c  */
#line 1741 "sql.ypp"
    { ((*yyvalp).ast_node)=newExprCal(t_expr_cal, "NIN", NULL, 0, (((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (6))].yystate.yysemantics.yysval.ast_node), (((yyGLRStackItem const *)yyvsp)[YYFILL ((5) - (6))].yystate.yysemantics.yysval.ast_node)); ;}
    break;

  case 552:

/* Line 936 of glr.c  */
#line 1742 "sql.ypp"
    { ((*yyvalp).ast_node)=newExprCal(t_expr_cal, "INS", NULL, 0, (((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (5))].yystate.yysemantics.yysval.ast_node), (((yyGLRStackItem const *)yyvsp)[YYFILL ((4) - (5))].yystate.yysemantics.yysval.ast_node)); ;}
    break;

  case 553:

/* Line 936 of glr.c  */
#line 1743 "sql.ypp"
    { ((*yyvalp).ast_node)=newExprCal(t_expr_cal, "NINS", NULL, 0, (((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (6))].yystate.yysemantics.yysval.ast_node), (((yyGLRStackItem const *)yyvsp)[YYFILL ((5) - (6))].yystate.yysemantics.yysval.ast_node)); ;}
    break;

  case 554:

/* Line 936 of glr.c  */
#line 1744 "sql.ypp"
    { ((*yyvalp).ast_node)=newExprCal(t_expr_cal, "EX", NULL, 0, NULL, (((yyGLRStackItem const *)yyvsp)[YYFILL ((3) - (4))].yystate.yysemantics.yysval.ast_node)); ;}
    break;

  case 555:

/* Line 936 of glr.c  */
#line 1745 "sql.ypp"
    { ((*yyvalp).ast_node)=newExprCal(t_expr_cal, "NINS", NULL, 0, NULL, (((yyGLRStackItem const *)yyvsp)[YYFILL ((4) - (5))].yystate.yysemantics.yysval.ast_node)); ;}
    break;

  case 556:

/* Line 936 of glr.c  */
#line 1748 "sql.ypp"
    { ((*yyvalp).ast_node)=newExprCal(t_expr_cal, "NAME", (((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (4))].yystate.yysemantics.yysval.strval), 0, NULL, (((yyGLRStackItem const *)yyvsp)[YYFILL ((3) - (4))].yystate.yysemantics.yysval.ast_node)); ;}
    break;

  case 557:

/* Line 936 of glr.c  */
#line 1752 "sql.ypp"
    { ((*yyvalp).ast_node)=newExprFunc(t_expr_func, "FCOUNTALL", NULL, NULL, NULL, NULL); ;}
    break;

  case 558:

/* Line 936 of glr.c  */
#line 1753 "sql.ypp"
    { ((*yyvalp).ast_node)=newExprFunc(t_expr_func, "FCOUNT", NULL, (((yyGLRStackItem const *)yyvsp)[YYFILL ((3) - (4))].yystate.yysemantics.yysval.ast_node), NULL, NULL); ;}
    break;

  case 559:

/* Line 936 of glr.c  */
#line 1755 "sql.ypp"
    { ((*yyvalp).ast_node)=newExprFunc(t_expr_func, "FSUM", NULL, (((yyGLRStackItem const *)yyvsp)[YYFILL ((3) - (4))].yystate.yysemantics.yysval.ast_node), NULL, NULL); ;}
    break;

  case 560:

/* Line 936 of glr.c  */
#line 1756 "sql.ypp"
    { ((*yyvalp).ast_node)=newExprFunc(t_expr_func, "FAVG", NULL, (((yyGLRStackItem const *)yyvsp)[YYFILL ((3) - (4))].yystate.yysemantics.yysval.ast_node), NULL, NULL); ;}
    break;

  case 561:

/* Line 936 of glr.c  */
#line 1757 "sql.ypp"
    { ((*yyvalp).ast_node)=newExprFunc(t_expr_func, "FMIN", NULL, (((yyGLRStackItem const *)yyvsp)[YYFILL ((3) - (4))].yystate.yysemantics.yysval.ast_node), NULL, NULL); ;}
    break;

  case 562:

/* Line 936 of glr.c  */
#line 1758 "sql.ypp"
    { ((*yyvalp).ast_node)=newExprFunc(t_expr_func, "FMAX", NULL, (((yyGLRStackItem const *)yyvsp)[YYFILL ((3) - (4))].yystate.yysemantics.yysval.ast_node), NULL, NULL); ;}
    break;

  case 563:

/* Line 936 of glr.c  */
#line 1762 "sql.ypp"
    { ((*yyvalp).ast_node)=newExprFunc(t_expr_func, "FSUBSTRING", NULL, (((yyGLRStackItem const *)yyvsp)[YYFILL ((3) - (4))].yystate.yysemantics.yysval.ast_node), NULL, NULL); ;}
    break;

  case 564:

/* Line 936 of glr.c  */
#line 1763 "sql.ypp"
    { ((*yyvalp).ast_node)=newExprFunc(t_expr_func, "FSUBSTRING2", NULL, (((yyGLRStackItem const *)yyvsp)[YYFILL ((3) - (6))].yystate.yysemantics.yysval.ast_node), (((yyGLRStackItem const *)yyvsp)[YYFILL ((5) - (6))].yystate.yysemantics.yysval.ast_node), NULL);;}
    break;

  case 565:

/* Line 936 of glr.c  */
#line 1764 "sql.ypp"
    { ((*yyvalp).ast_node)=newExprFunc(t_expr_func, "FSUBSTRING3", (((yyGLRStackItem const *)yyvsp)[YYFILL ((7) - (8))].yystate.yysemantics.yysval.ast_node), (((yyGLRStackItem const *)yyvsp)[YYFILL ((3) - (8))].yystate.yysemantics.yysval.ast_node), (((yyGLRStackItem const *)yyvsp)[YYFILL ((5) - (8))].yystate.yysemantics.yysval.ast_node), NULL); ;}
    break;

  case 566:

/* Line 936 of glr.c  */
#line 1765 "sql.ypp"
    { ((*yyvalp).ast_node)=newExprFunc(t_expr_func, "FTRIM", NULL, (((yyGLRStackItem const *)yyvsp)[YYFILL ((3) - (4))].yystate.yysemantics.yysval.ast_node), NULL, NULL); ;}
    break;

  case 567:

/* Line 936 of glr.c  */
#line 1766 "sql.ypp"
    { ;}
    break;

  case 568:

/* Line 936 of glr.c  */
#line 1769 "sql.ypp"
    { emit("INT 1"); ;}
    break;

  case 569:

/* Line 936 of glr.c  */
#line 1770 "sql.ypp"
    { emit("INT 2"); ;}
    break;

  case 570:

/* Line 936 of glr.c  */
#line 1771 "sql.ypp"
    { emit("INT 3"); ;}
    break;

  case 573:

/* Line 936 of glr.c  */
#line 1778 "sql.ypp"
    { emit("NUMBER 1"); ;}
    break;

  case 574:

/* Line 936 of glr.c  */
#line 1779 "sql.ypp"
    { emit("NUMBER 2"); ;}
    break;

  case 575:

/* Line 936 of glr.c  */
#line 1780 "sql.ypp"
    { emit("NUMBER 3"); ;}
    break;

  case 576:

/* Line 936 of glr.c  */
#line 1781 "sql.ypp"
    { emit("NUMBER 4"); ;}
    break;

  case 577:

/* Line 936 of glr.c  */
#line 1782 "sql.ypp"
    { emit("NUMBER 5"); ;}
    break;

  case 578:

/* Line 936 of glr.c  */
#line 1783 "sql.ypp"
    { emit("NUMBER 6"); ;}
    break;

  case 579:

/* Line 936 of glr.c  */
#line 1784 "sql.ypp"
    { emit("NUMBER 7"); ;}
    break;

  case 580:

/* Line 936 of glr.c  */
#line 1785 "sql.ypp"
    { emit("NUMBER 8"); ;}
    break;

  case 581:

/* Line 936 of glr.c  */
#line 1786 "sql.ypp"
    { emit("NUMBER 9"); ;}
    break;

  case 582:

/* Line 936 of glr.c  */
#line 1789 "sql.ypp"
    { ((*yyvalp).ast_node)=newExprFunc(t_expr_func, "CASE3", (((yyGLRStackItem const *)yyvsp)[YYFILL ((2) - (4))].yystate.yysemantics.yysval.ast_node), (((yyGLRStackItem const *)yyvsp)[YYFILL ((3) - (4))].yystate.yysemantics.yysval.ast_node), NULL, NULL); ;}
    break;

  case 583:

/* Line 936 of glr.c  */
#line 1790 "sql.ypp"
    { ((*yyvalp).ast_node)=newExprFunc(t_expr_func, "CASE2", (((yyGLRStackItem const *)yyvsp)[YYFILL ((2) - (6))].yystate.yysemantics.yysval.ast_node), (((yyGLRStackItem const *)yyvsp)[YYFILL ((3) - (6))].yystate.yysemantics.yysval.ast_node), (((yyGLRStackItem const *)yyvsp)[YYFILL ((5) - (6))].yystate.yysemantics.yysval.ast_node), NULL); ;}
    break;

  case 584:

/* Line 936 of glr.c  */
#line 1791 "sql.ypp"
    { ((*yyvalp).ast_node)=newExprFunc(t_expr_func, "CASE3", NULL, (((yyGLRStackItem const *)yyvsp)[YYFILL ((2) - (3))].yystate.yysemantics.yysval.ast_node), NULL, NULL); ;}
    break;

  case 585:

/* Line 936 of glr.c  */
#line 1792 "sql.ypp"
    { ((*yyvalp).ast_node)=newExprFunc(t_expr_func, "CASE4", NULL, (((yyGLRStackItem const *)yyvsp)[YYFILL ((2) - (5))].yystate.yysemantics.yysval.ast_node), (((yyGLRStackItem const *)yyvsp)[YYFILL ((4) - (5))].yystate.yysemantics.yysval.ast_node), NULL); ;}
    break;

  case 586:

/* Line 936 of glr.c  */
#line 1795 "sql.ypp"
    { ((*yyvalp).ast_node)=newExprFunc(t_expr_func, "WHEN1", NULL, (((yyGLRStackItem const *)yyvsp)[YYFILL ((2) - (4))].yystate.yysemantics.yysval.ast_node), (((yyGLRStackItem const *)yyvsp)[YYFILL ((4) - (4))].yystate.yysemantics.yysval.ast_node), NULL); ;}
    break;

  case 587:

/* Line 936 of glr.c  */
#line 1796 "sql.ypp"
    { ((*yyvalp).ast_node)=newExprFunc(t_expr_func, "WHEN2", NULL, (((yyGLRStackItem const *)yyvsp)[YYFILL ((3) - (5))].yystate.yysemantics.yysval.ast_node), (((yyGLRStackItem const *)yyvsp)[YYFILL ((5) - (5))].yystate.yysemantics.yysval.ast_node), (((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (5))].yystate.yysemantics.yysval.ast_node)); ;}
    break;

  case 588:

/* Line 936 of glr.c  */
#line 1799 "sql.ypp"
    { ((*yyvalp).ast_node)=newExprCal(t_expr_cal, "LIKE", NULL, 0, (((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (3))].yystate.yysemantics.yysval.ast_node), (((yyGLRStackItem const *)yyvsp)[YYFILL ((3) - (3))].yystate.yysemantics.yysval.ast_node)); ;}
    break;

  case 589:

/* Line 936 of glr.c  */
#line 1800 "sql.ypp"
    { ((*yyvalp).ast_node)=newExprCal(t_expr_cal, "NLIKE", NULL, 0, NULL, (((yyGLRStackItem const *)yyvsp)[YYFILL ((4) - (4))].yystate.yysemantics.yysval.ast_node)); ;}
    break;

  case 590:

/* Line 936 of glr.c  */
#line 1803 "sql.ypp"
    { ((*yyvalp).ast_node)=newExprCal(t_expr_cal, "REGEXP", NULL, 0, (((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (3))].yystate.yysemantics.yysval.ast_node), (((yyGLRStackItem const *)yyvsp)[YYFILL ((3) - (3))].yystate.yysemantics.yysval.ast_node)); ;}
    break;

  case 591:

/* Line 936 of glr.c  */
#line 1804 "sql.ypp"
    { ((*yyvalp).ast_node)=newExprCal(t_expr_cal, "NREGEXP", NULL, 0, (((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (4))].yystate.yysemantics.yysval.ast_node), (((yyGLRStackItem const *)yyvsp)[YYFILL ((4) - (4))].yystate.yysemantics.yysval.ast_node)); ;}
    break;



/* Line 936 of glr.c  */
#line 5800 "sql.tab.cpp"
      default: break;
    }

  return yyok;
# undef yyerrok
# undef YYABORT
# undef YYACCEPT
# undef YYERROR
# undef YYBACKUP
# undef yyclearin
# undef YYRECOVERING
}


/*ARGSUSED*/ static void
yyuserMerge (int yyn, YYSTYPE* yy0, YYSTYPE* yy1)
{
  YYUSE (yy0);
  YYUSE (yy1);

  switch (yyn)
    {
      
      default: break;
    }
}

			      /* Bison grammar-table manipulation.  */

/*-----------------------------------------------.
| Release the memory associated to this symbol.  |
`-----------------------------------------------*/

/*ARGSUSED*/
static void
yydestruct (const char *yymsg, int yytype, YYSTYPE *yyvaluep, YYLTYPE *yylocationp)
{
  YYUSE (yyvaluep);
  YYUSE (yylocationp);

  if (!yymsg)
    yymsg = "Deleting";
  YY_SYMBOL_PRINT (yymsg, yytype, yyvaluep, yylocationp);

  switch (yytype)
    {

      default:
	break;
    }
}

/** Number of symbols composing the right hand side of rule #RULE.  */
static inline int
yyrhsLength (yyRuleNum yyrule)
{
  return yyr2[yyrule];
}

static void
yydestroyGLRState (char const *yymsg, yyGLRState *yys)
{
  if (yys->yyresolved)
    yydestruct (yymsg, yystos[yys->yylrState],
		&yys->yysemantics.yysval, &yys->yyloc);
  else
    {
#if YYDEBUG
      if (yydebug)
	{
	  if (yys->yysemantics.yyfirstVal)
	    YYFPRINTF (stderr, "%s unresolved ", yymsg);
	  else
	    YYFPRINTF (stderr, "%s incomplete ", yymsg);
	  yy_symbol_print (stderr, yystos[yys->yylrState],
			   NULL, &yys->yyloc);
	  YYFPRINTF (stderr, "\n");
	}
#endif

      if (yys->yysemantics.yyfirstVal)
	{
	  yySemanticOption *yyoption = yys->yysemantics.yyfirstVal;
	  yyGLRState *yyrh;
	  int yyn;
	  for (yyrh = yyoption->yystate, yyn = yyrhsLength (yyoption->yyrule);
	       yyn > 0;
	       yyrh = yyrh->yypred, yyn -= 1)
	    yydestroyGLRState (yymsg, yyrh);
	}
    }
}

/** Left-hand-side symbol for rule #RULE.  */
static inline yySymbol
yylhsNonterm (yyRuleNum yyrule)
{
  return yyr1[yyrule];
}

#define yyis_pact_ninf(yystate) \
  ((yystate) == YYPACT_NINF)

/** True iff LR state STATE has only a default reduction (regardless
 *  of token).  */
static inline yybool
yyisDefaultedState (yyStateNum yystate)
{
  return yyis_pact_ninf (yypact[yystate]);
}

/** The default reduction for STATE, assuming it has one.  */
static inline yyRuleNum
yydefaultAction (yyStateNum yystate)
{
  return yydefact[yystate];
}

#define yyis_table_ninf(yytable_value) \
  ((yytable_value) == YYTABLE_NINF)

/** Set *YYACTION to the action to take in YYSTATE on seeing YYTOKEN.
 *  Result R means
 *    R < 0:  Reduce on rule -R.
 *    R = 0:  Error.
 *    R > 0:  Shift to state R.
 *  Set *CONFLICTS to a pointer into yyconfl to 0-terminated list of
 *  conflicting reductions.
 */
static inline void
yygetLRActions (yyStateNum yystate, int yytoken,
		int* yyaction, const short int** yyconflicts)
{
  int yyindex = yypact[yystate] + yytoken;
  if (yyindex < 0 || YYLAST < yyindex || yycheck[yyindex] != yytoken)
    {
      *yyaction = -yydefact[yystate];
      *yyconflicts = yyconfl;
    }
  else if (! yyis_table_ninf (yytable[yyindex]))
    {
      *yyaction = yytable[yyindex];
      *yyconflicts = yyconfl + yyconflp[yyindex];
    }
  else
    {
      *yyaction = 0;
      *yyconflicts = yyconfl + yyconflp[yyindex];
    }
}

static inline yyStateNum
yyLRgotoState (yyStateNum yystate, yySymbol yylhs)
{
  int yyr;
  yyr = yypgoto[yylhs - YYNTOKENS] + yystate;
  if (0 <= yyr && yyr <= YYLAST && yycheck[yyr] == yystate)
    return yytable[yyr];
  else
    return yydefgoto[yylhs - YYNTOKENS];
}

static inline yybool
yyisShiftAction (int yyaction)
{
  return 0 < yyaction;
}

static inline yybool
yyisErrorAction (int yyaction)
{
  return yyaction == 0;
}

				/* GLRStates */

/** Return a fresh GLRStackItem.  Callers should call
 * YY_RESERVE_GLRSTACK afterwards to make sure there is sufficient
 * headroom.  */

static inline yyGLRStackItem*
yynewGLRStackItem (yyGLRStack* yystackp, yybool yyisState)
{
  yyGLRStackItem* yynewItem = yystackp->yynextFree;
  yystackp->yyspaceLeft -= 1;
  yystackp->yynextFree += 1;
  yynewItem->yystate.yyisState = yyisState;
  return yynewItem;
}

/** Add a new semantic action that will execute the action for rule
 *  RULENUM on the semantic values in RHS to the list of
 *  alternative actions for STATE.  Assumes that RHS comes from
 *  stack #K of *STACKP. */
static void
yyaddDeferredAction (yyGLRStack* yystackp, size_t yyk, yyGLRState* yystate,
		     yyGLRState* rhs, yyRuleNum yyrule)
{
  yySemanticOption* yynewOption =
    &yynewGLRStackItem (yystackp, yyfalse)->yyoption;
  yynewOption->yystate = rhs;
  yynewOption->yyrule = yyrule;
  if (yystackp->yytops.yylookaheadNeeds[yyk])
    {
      yynewOption->yyrawchar = yychar;
      yynewOption->yyval = yylval;
      yynewOption->yyloc = yylloc;
    }
  else
    yynewOption->yyrawchar = YYEMPTY;
  yynewOption->yynext = yystate->yysemantics.yyfirstVal;
  yystate->yysemantics.yyfirstVal = yynewOption;

  YY_RESERVE_GLRSTACK (yystackp);
}

				/* GLRStacks */

/** Initialize SET to a singleton set containing an empty stack.  */
static yybool
yyinitStateSet (yyGLRStateSet* yyset)
{
  yyset->yysize = 1;
  yyset->yycapacity = 16;
  yyset->yystates = (yyGLRState**) YYMALLOC (16 * sizeof yyset->yystates[0]);
  if (! yyset->yystates)
    return yyfalse;
  yyset->yystates[0] = NULL;
  yyset->yylookaheadNeeds =
    (yybool*) YYMALLOC (16 * sizeof yyset->yylookaheadNeeds[0]);
  if (! yyset->yylookaheadNeeds)
    {
      YYFREE (yyset->yystates);
      return yyfalse;
    }
  return yytrue;
}

static void yyfreeStateSet (yyGLRStateSet* yyset)
{
  YYFREE (yyset->yystates);
  YYFREE (yyset->yylookaheadNeeds);
}

/** Initialize STACK to a single empty stack, with total maximum
 *  capacity for all stacks of SIZE.  */
static yybool
yyinitGLRStack (yyGLRStack* yystackp, size_t yysize)
{
  yystackp->yyerrState = 0;
  yynerrs = 0;
  yystackp->yyspaceLeft = yysize;
  yystackp->yyitems =
    (yyGLRStackItem*) YYMALLOC (yysize * sizeof yystackp->yynextFree[0]);
  if (!yystackp->yyitems)
    return yyfalse;
  yystackp->yynextFree = yystackp->yyitems;
  yystackp->yysplitPoint = NULL;
  yystackp->yylastDeleted = NULL;
  return yyinitStateSet (&yystackp->yytops);
}


#if YYSTACKEXPANDABLE
# define YYRELOC(YYFROMITEMS,YYTOITEMS,YYX,YYTYPE) \
  &((YYTOITEMS) - ((YYFROMITEMS) - (yyGLRStackItem*) (YYX)))->YYTYPE

/** If STACK is expandable, extend it.  WARNING: Pointers into the
    stack from outside should be considered invalid after this call.
    We always expand when there are 1 or fewer items left AFTER an
    allocation, so that we can avoid having external pointers exist
    across an allocation.  */
static void
yyexpandGLRStack (yyGLRStack* yystackp)
{
  yyGLRStackItem* yynewItems;
  yyGLRStackItem* yyp0, *yyp1;
  size_t yysize, yynewSize;
  size_t yyn;
  yysize = yystackp->yynextFree - yystackp->yyitems;
  if (YYMAXDEPTH - YYHEADROOM < yysize)
    yyMemoryExhausted (yystackp);
  yynewSize = 2*yysize;
  if (YYMAXDEPTH < yynewSize)
    yynewSize = YYMAXDEPTH;
  yynewItems = (yyGLRStackItem*) YYMALLOC (yynewSize * sizeof yynewItems[0]);
  if (! yynewItems)
    yyMemoryExhausted (yystackp);
  for (yyp0 = yystackp->yyitems, yyp1 = yynewItems, yyn = yysize;
       0 < yyn;
       yyn -= 1, yyp0 += 1, yyp1 += 1)
    {
      *yyp1 = *yyp0;
      if (*(yybool *) yyp0)
	{
	  yyGLRState* yys0 = &yyp0->yystate;
	  yyGLRState* yys1 = &yyp1->yystate;
	  if (yys0->yypred != NULL)
	    yys1->yypred =
	      YYRELOC (yyp0, yyp1, yys0->yypred, yystate);
	  if (! yys0->yyresolved && yys0->yysemantics.yyfirstVal != NULL)
	    yys1->yysemantics.yyfirstVal =
	      YYRELOC(yyp0, yyp1, yys0->yysemantics.yyfirstVal, yyoption);
	}
      else
	{
	  yySemanticOption* yyv0 = &yyp0->yyoption;
	  yySemanticOption* yyv1 = &yyp1->yyoption;
	  if (yyv0->yystate != NULL)
	    yyv1->yystate = YYRELOC (yyp0, yyp1, yyv0->yystate, yystate);
	  if (yyv0->yynext != NULL)
	    yyv1->yynext = YYRELOC (yyp0, yyp1, yyv0->yynext, yyoption);
	}
    }
  if (yystackp->yysplitPoint != NULL)
    yystackp->yysplitPoint = YYRELOC (yystackp->yyitems, yynewItems,
				 yystackp->yysplitPoint, yystate);

  for (yyn = 0; yyn < yystackp->yytops.yysize; yyn += 1)
    if (yystackp->yytops.yystates[yyn] != NULL)
      yystackp->yytops.yystates[yyn] =
	YYRELOC (yystackp->yyitems, yynewItems,
		 yystackp->yytops.yystates[yyn], yystate);
  YYFREE (yystackp->yyitems);
  yystackp->yyitems = yynewItems;
  yystackp->yynextFree = yynewItems + yysize;
  yystackp->yyspaceLeft = yynewSize - yysize;
}
#endif

static void
yyfreeGLRStack (yyGLRStack* yystackp)
{
  YYFREE (yystackp->yyitems);
  yyfreeStateSet (&yystackp->yytops);
}

/** Assuming that S is a GLRState somewhere on STACK, update the
 *  splitpoint of STACK, if needed, so that it is at least as deep as
 *  S.  */
static inline void
yyupdateSplit (yyGLRStack* yystackp, yyGLRState* yys)
{
  if (yystackp->yysplitPoint != NULL && yystackp->yysplitPoint > yys)
    yystackp->yysplitPoint = yys;
}

/** Invalidate stack #K in STACK.  */
static inline void
yymarkStackDeleted (yyGLRStack* yystackp, size_t yyk)
{
  if (yystackp->yytops.yystates[yyk] != NULL)
    yystackp->yylastDeleted = yystackp->yytops.yystates[yyk];
  yystackp->yytops.yystates[yyk] = NULL;
}

/** Undelete the last stack that was marked as deleted.  Can only be
    done once after a deletion, and only when all other stacks have
    been deleted.  */
static void
yyundeleteLastStack (yyGLRStack* yystackp)
{
  if (yystackp->yylastDeleted == NULL || yystackp->yytops.yysize != 0)
    return;
  yystackp->yytops.yystates[0] = yystackp->yylastDeleted;
  yystackp->yytops.yysize = 1;
  YYDPRINTF ((stderr, "Restoring last deleted stack as stack #0.\n"));
  yystackp->yylastDeleted = NULL;
}

static inline void
yyremoveDeletes (yyGLRStack* yystackp)
{
  size_t yyi, yyj;
  yyi = yyj = 0;
  while (yyj < yystackp->yytops.yysize)
    {
      if (yystackp->yytops.yystates[yyi] == NULL)
	{
	  if (yyi == yyj)
	    {
	      YYDPRINTF ((stderr, "Removing dead stacks.\n"));
	    }
	  yystackp->yytops.yysize -= 1;
	}
      else
	{
	  yystackp->yytops.yystates[yyj] = yystackp->yytops.yystates[yyi];
	  /* In the current implementation, it's unnecessary to copy
	     yystackp->yytops.yylookaheadNeeds[yyi] since, after
	     yyremoveDeletes returns, the parser immediately either enters
	     deterministic operation or shifts a token.  However, it doesn't
	     hurt, and the code might evolve to need it.  */
	  yystackp->yytops.yylookaheadNeeds[yyj] =
	    yystackp->yytops.yylookaheadNeeds[yyi];
	  if (yyj != yyi)
	    {
	      YYDPRINTF ((stderr, "Rename stack %lu -> %lu.\n",
			  (unsigned long int) yyi, (unsigned long int) yyj));
	    }
	  yyj += 1;
	}
      yyi += 1;
    }
}

/** Shift to a new state on stack #K of STACK, corresponding to LR state
 * LRSTATE, at input position POSN, with (resolved) semantic value SVAL.  */
static inline void
yyglrShift (yyGLRStack* yystackp, size_t yyk, yyStateNum yylrState,
	    size_t yyposn,
	    YYSTYPE* yyvalp, YYLTYPE* yylocp)
{
  yyGLRState* yynewState = &yynewGLRStackItem (yystackp, yytrue)->yystate;

  yynewState->yylrState = yylrState;
  yynewState->yyposn = yyposn;
  yynewState->yyresolved = yytrue;
  yynewState->yypred = yystackp->yytops.yystates[yyk];
  yynewState->yysemantics.yysval = *yyvalp;
  yynewState->yyloc = *yylocp;
  yystackp->yytops.yystates[yyk] = yynewState;

  YY_RESERVE_GLRSTACK (yystackp);
}

/** Shift stack #K of YYSTACK, to a new state corresponding to LR
 *  state YYLRSTATE, at input position YYPOSN, with the (unresolved)
 *  semantic value of YYRHS under the action for YYRULE.  */
static inline void
yyglrShiftDefer (yyGLRStack* yystackp, size_t yyk, yyStateNum yylrState,
		 size_t yyposn, yyGLRState* rhs, yyRuleNum yyrule)
{
  yyGLRState* yynewState = &yynewGLRStackItem (yystackp, yytrue)->yystate;

  yynewState->yylrState = yylrState;
  yynewState->yyposn = yyposn;
  yynewState->yyresolved = yyfalse;
  yynewState->yypred = yystackp->yytops.yystates[yyk];
  yynewState->yysemantics.yyfirstVal = NULL;
  yystackp->yytops.yystates[yyk] = yynewState;

  /* Invokes YY_RESERVE_GLRSTACK.  */
  yyaddDeferredAction (yystackp, yyk, yynewState, rhs, yyrule);
}

/** Pop the symbols consumed by reduction #RULE from the top of stack
 *  #K of STACK, and perform the appropriate semantic action on their
 *  semantic values.  Assumes that all ambiguities in semantic values
 *  have been previously resolved.  Set *VALP to the resulting value,
 *  and *LOCP to the computed location (if any).  Return value is as
 *  for userAction.  */
static inline YYRESULTTAG
yydoAction (yyGLRStack* yystackp, size_t yyk, yyRuleNum yyrule,
	    YYSTYPE* yyvalp, YYLTYPE* yylocp)
{
  int yynrhs = yyrhsLength (yyrule);

  if (yystackp->yysplitPoint == NULL)
    {
      /* Standard special case: single stack.  */
      yyGLRStackItem* rhs = (yyGLRStackItem*) yystackp->yytops.yystates[yyk];
      YYASSERT (yyk == 0);
      yystackp->yynextFree -= yynrhs;
      yystackp->yyspaceLeft += yynrhs;
      yystackp->yytops.yystates[0] = & yystackp->yynextFree[-1].yystate;
      return yyuserAction (yyrule, yynrhs, rhs,
			   yyvalp, yylocp, yystackp);
    }
  else
    {
      /* At present, doAction is never called in nondeterministic
       * mode, so this branch is never taken.  It is here in
       * anticipation of a future feature that will allow immediate
       * evaluation of selected actions in nondeterministic mode.  */
      int yyi;
      yyGLRState* yys;
      yyGLRStackItem yyrhsVals[YYMAXRHS + YYMAXLEFT + 1];
      yys = yyrhsVals[YYMAXRHS + YYMAXLEFT].yystate.yypred
	= yystackp->yytops.yystates[yyk];
      if (yynrhs == 0)
	/* Set default location.  */
	yyrhsVals[YYMAXRHS + YYMAXLEFT - 1].yystate.yyloc = yys->yyloc;
      for (yyi = 0; yyi < yynrhs; yyi += 1)
	{
	  yys = yys->yypred;
	  YYASSERT (yys);
	}
      yyupdateSplit (yystackp, yys);
      yystackp->yytops.yystates[yyk] = yys;
      return yyuserAction (yyrule, yynrhs, yyrhsVals + YYMAXRHS + YYMAXLEFT - 1,
			   yyvalp, yylocp, yystackp);
    }
}

#if !YYDEBUG
# define YY_REDUCE_PRINT(Args)
#else
# define YY_REDUCE_PRINT(Args)		\
do {					\
  if (yydebug)				\
    yy_reduce_print Args;		\
} while (YYID (0))

/*----------------------------------------------------------.
| Report that the RULE is going to be reduced on stack #K.  |
`----------------------------------------------------------*/

/*ARGSUSED*/ static inline void
yy_reduce_print (yyGLRStack* yystackp, size_t yyk, yyRuleNum yyrule,
		 YYSTYPE* yyvalp, YYLTYPE* yylocp)
{
  int yynrhs = yyrhsLength (yyrule);
  yybool yynormal __attribute__ ((__unused__)) =
    (yystackp->yysplitPoint == NULL);
  yyGLRStackItem* yyvsp = (yyGLRStackItem*) yystackp->yytops.yystates[yyk];
  int yylow = 1;
  int yyi;
  YYUSE (yyvalp);
  YYUSE (yylocp);
  YYFPRINTF (stderr, "Reducing stack %lu by rule %d (line %lu):\n",
	     (unsigned long int) yyk, yyrule - 1,
	     (unsigned long int) yyrline[yyrule]);
  /* The symbols being reduced.  */
  for (yyi = 0; yyi < yynrhs; yyi++)
    {
      YYFPRINTF (stderr, "   $%d = ", yyi + 1);
      yy_symbol_print (stderr, yyrhs[yyprhs[yyrule] + yyi],
		       &(((yyGLRStackItem const *)yyvsp)[YYFILL ((yyi + 1) - (yynrhs))].yystate.yysemantics.yysval)
		       , &(((yyGLRStackItem const *)yyvsp)[YYFILL ((yyi + 1) - (yynrhs))].yystate.yyloc)		       );
      YYFPRINTF (stderr, "\n");
    }
}
#endif

/** Pop items off stack #K of STACK according to grammar rule RULE,
 *  and push back on the resulting nonterminal symbol.  Perform the
 *  semantic action associated with RULE and store its value with the
 *  newly pushed state, if FORCEEVAL or if STACK is currently
 *  unambiguous.  Otherwise, store the deferred semantic action with
 *  the new state.  If the new state would have an identical input
 *  position, LR state, and predecessor to an existing state on the stack,
 *  it is identified with that existing state, eliminating stack #K from
 *  the STACK.  In this case, the (necessarily deferred) semantic value is
 *  added to the options for the existing state's semantic value.
 */
static inline YYRESULTTAG
yyglrReduce (yyGLRStack* yystackp, size_t yyk, yyRuleNum yyrule,
	     yybool yyforceEval)
{
  size_t yyposn = yystackp->yytops.yystates[yyk]->yyposn;

  if (yyforceEval || yystackp->yysplitPoint == NULL)
    {
      YYSTYPE yysval;
      YYLTYPE yyloc;

      YY_REDUCE_PRINT ((yystackp, yyk, yyrule, &yysval, &yyloc));
      YYCHK (yydoAction (yystackp, yyk, yyrule, &yysval,
			 &yyloc));
      YY_SYMBOL_PRINT ("-> $$ =", yyr1[yyrule], &yysval, &yyloc);
      yyglrShift (yystackp, yyk,
		  yyLRgotoState (yystackp->yytops.yystates[yyk]->yylrState,
				 yylhsNonterm (yyrule)),
		  yyposn, &yysval, &yyloc);
    }
  else
    {
      size_t yyi;
      int yyn;
      yyGLRState* yys, *yys0 = yystackp->yytops.yystates[yyk];
      yyStateNum yynewLRState;

      for (yys = yystackp->yytops.yystates[yyk], yyn = yyrhsLength (yyrule);
	   0 < yyn; yyn -= 1)
	{
	  yys = yys->yypred;
	  YYASSERT (yys);
	}
      yyupdateSplit (yystackp, yys);
      yynewLRState = yyLRgotoState (yys->yylrState, yylhsNonterm (yyrule));
      YYDPRINTF ((stderr,
		  "Reduced stack %lu by rule #%d; action deferred.  Now in state %d.\n",
		  (unsigned long int) yyk, yyrule - 1, yynewLRState));
      for (yyi = 0; yyi < yystackp->yytops.yysize; yyi += 1)
	if (yyi != yyk && yystackp->yytops.yystates[yyi] != NULL)
	  {
	    yyGLRState* yyp, *yysplit = yystackp->yysplitPoint;
	    yyp = yystackp->yytops.yystates[yyi];
	    while (yyp != yys && yyp != yysplit && yyp->yyposn >= yyposn)
	      {
		if (yyp->yylrState == yynewLRState && yyp->yypred == yys)
		  {
		    yyaddDeferredAction (yystackp, yyk, yyp, yys0, yyrule);
		    yymarkStackDeleted (yystackp, yyk);
		    YYDPRINTF ((stderr, "Merging stack %lu into stack %lu.\n",
				(unsigned long int) yyk,
				(unsigned long int) yyi));
		    return yyok;
		  }
		yyp = yyp->yypred;
	      }
	  }
      yystackp->yytops.yystates[yyk] = yys;
      yyglrShiftDefer (yystackp, yyk, yynewLRState, yyposn, yys0, yyrule);
    }
  return yyok;
}

static size_t
yysplitStack (yyGLRStack* yystackp, size_t yyk)
{
  if (yystackp->yysplitPoint == NULL)
    {
      YYASSERT (yyk == 0);
      yystackp->yysplitPoint = yystackp->yytops.yystates[yyk];
    }
  if (yystackp->yytops.yysize >= yystackp->yytops.yycapacity)
    {
      yyGLRState** yynewStates;
      yybool* yynewLookaheadNeeds;

      yynewStates = NULL;

      if (yystackp->yytops.yycapacity
	  > (YYSIZEMAX / (2 * sizeof yynewStates[0])))
	yyMemoryExhausted (yystackp);
      yystackp->yytops.yycapacity *= 2;

      yynewStates =
	(yyGLRState**) YYREALLOC (yystackp->yytops.yystates,
				  (yystackp->yytops.yycapacity
				   * sizeof yynewStates[0]));
      if (yynewStates == NULL)
	yyMemoryExhausted (yystackp);
      yystackp->yytops.yystates = yynewStates;

      yynewLookaheadNeeds =
	(yybool*) YYREALLOC (yystackp->yytops.yylookaheadNeeds,
			     (yystackp->yytops.yycapacity
			      * sizeof yynewLookaheadNeeds[0]));
      if (yynewLookaheadNeeds == NULL)
	yyMemoryExhausted (yystackp);
      yystackp->yytops.yylookaheadNeeds = yynewLookaheadNeeds;
    }
  yystackp->yytops.yystates[yystackp->yytops.yysize]
    = yystackp->yytops.yystates[yyk];
  yystackp->yytops.yylookaheadNeeds[yystackp->yytops.yysize]
    = yystackp->yytops.yylookaheadNeeds[yyk];
  yystackp->yytops.yysize += 1;
  return yystackp->yytops.yysize-1;
}

/** True iff Y0 and Y1 represent identical options at the top level.
 *  That is, they represent the same rule applied to RHS symbols
 *  that produce the same terminal symbols.  */
static yybool
yyidenticalOptions (yySemanticOption* yyy0, yySemanticOption* yyy1)
{
  if (yyy0->yyrule == yyy1->yyrule)
    {
      yyGLRState *yys0, *yys1;
      int yyn;
      for (yys0 = yyy0->yystate, yys1 = yyy1->yystate,
	   yyn = yyrhsLength (yyy0->yyrule);
	   yyn > 0;
	   yys0 = yys0->yypred, yys1 = yys1->yypred, yyn -= 1)
	if (yys0->yyposn != yys1->yyposn)
	  return yyfalse;
      return yytrue;
    }
  else
    return yyfalse;
}

/** Assuming identicalOptions (Y0,Y1), destructively merge the
 *  alternative semantic values for the RHS-symbols of Y1 and Y0.  */
static void
yymergeOptionSets (yySemanticOption* yyy0, yySemanticOption* yyy1)
{
  yyGLRState *yys0, *yys1;
  int yyn;
  for (yys0 = yyy0->yystate, yys1 = yyy1->yystate,
       yyn = yyrhsLength (yyy0->yyrule);
       yyn > 0;
       yys0 = yys0->yypred, yys1 = yys1->yypred, yyn -= 1)
    {
      if (yys0 == yys1)
	break;
      else if (yys0->yyresolved)
	{
	  yys1->yyresolved = yytrue;
	  yys1->yysemantics.yysval = yys0->yysemantics.yysval;
	}
      else if (yys1->yyresolved)
	{
	  yys0->yyresolved = yytrue;
	  yys0->yysemantics.yysval = yys1->yysemantics.yysval;
	}
      else
	{
	  yySemanticOption** yyz0p;
	  yySemanticOption* yyz1;
	  yyz0p = &yys0->yysemantics.yyfirstVal;
	  yyz1 = yys1->yysemantics.yyfirstVal;
	  while (YYID (yytrue))
	    {
	      if (yyz1 == *yyz0p || yyz1 == NULL)
		break;
	      else if (*yyz0p == NULL)
		{
		  *yyz0p = yyz1;
		  break;
		}
	      else if (*yyz0p < yyz1)
		{
		  yySemanticOption* yyz = *yyz0p;
		  *yyz0p = yyz1;
		  yyz1 = yyz1->yynext;
		  (*yyz0p)->yynext = yyz;
		}
	      yyz0p = &(*yyz0p)->yynext;
	    }
	  yys1->yysemantics.yyfirstVal = yys0->yysemantics.yyfirstVal;
	}
    }
}

/** Y0 and Y1 represent two possible actions to take in a given
 *  parsing state; return 0 if no combination is possible,
 *  1 if user-mergeable, 2 if Y0 is preferred, 3 if Y1 is preferred.  */
static int
yypreference (yySemanticOption* y0, yySemanticOption* y1)
{
  yyRuleNum r0 = y0->yyrule, r1 = y1->yyrule;
  int p0 = yydprec[r0], p1 = yydprec[r1];

  if (p0 == p1)
    {
      if (yymerger[r0] == 0 || yymerger[r0] != yymerger[r1])
	return 0;
      else
	return 1;
    }
  if (p0 == 0 || p1 == 0)
    return 0;
  if (p0 < p1)
    return 3;
  if (p1 < p0)
    return 2;
  return 0;
}

static YYRESULTTAG yyresolveValue (yyGLRState* yys,
				   yyGLRStack* yystackp);


/** Resolve the previous N states starting at and including state S.  If result
 *  != yyok, some states may have been left unresolved possibly with empty
 *  semantic option chains.  Regardless of whether result = yyok, each state
 *  has been left with consistent data so that yydestroyGLRState can be invoked
 *  if necessary.  */
static YYRESULTTAG
yyresolveStates (yyGLRState* yys, int yyn,
		 yyGLRStack* yystackp)
{
  if (0 < yyn)
    {
      YYASSERT (yys->yypred);
      YYCHK (yyresolveStates (yys->yypred, yyn-1, yystackp));
      if (! yys->yyresolved)
	YYCHK (yyresolveValue (yys, yystackp));
    }
  return yyok;
}

/** Resolve the states for the RHS of OPT, perform its user action, and return
 *  the semantic value and location.  Regardless of whether result = yyok, all
 *  RHS states have been destroyed (assuming the user action destroys all RHS
 *  semantic values if invoked).  */
static YYRESULTTAG
yyresolveAction (yySemanticOption* yyopt, yyGLRStack* yystackp,
		 YYSTYPE* yyvalp, YYLTYPE* yylocp)
{
  yyGLRStackItem yyrhsVals[YYMAXRHS + YYMAXLEFT + 1];
  int yynrhs;
  int yychar_current;
  YYSTYPE yylval_current;
  YYLTYPE yylloc_current;
  YYRESULTTAG yyflag;

  yynrhs = yyrhsLength (yyopt->yyrule);
  yyflag = yyresolveStates (yyopt->yystate, yynrhs, yystackp);
  if (yyflag != yyok)
    {
      yyGLRState *yys;
      for (yys = yyopt->yystate; yynrhs > 0; yys = yys->yypred, yynrhs -= 1)
	yydestroyGLRState ("Cleanup: popping", yys);
      return yyflag;
    }

  yyrhsVals[YYMAXRHS + YYMAXLEFT].yystate.yypred = yyopt->yystate;
  if (yynrhs == 0)
    /* Set default location.  */
    yyrhsVals[YYMAXRHS + YYMAXLEFT - 1].yystate.yyloc = yyopt->yystate->yyloc;
  yychar_current = yychar;
  yylval_current = yylval;
  yylloc_current = yylloc;
  yychar = yyopt->yyrawchar;
  yylval = yyopt->yyval;
  yylloc = yyopt->yyloc;
  yyflag = yyuserAction (yyopt->yyrule, yynrhs,
			   yyrhsVals + YYMAXRHS + YYMAXLEFT - 1,
			   yyvalp, yylocp, yystackp);
  yychar = yychar_current;
  yylval = yylval_current;
  yylloc = yylloc_current;
  return yyflag;
}

#if YYDEBUG
static void
yyreportTree (yySemanticOption* yyx, int yyindent)
{
  int yynrhs = yyrhsLength (yyx->yyrule);
  int yyi;
  yyGLRState* yys;
  yyGLRState* yystates[1 + YYMAXRHS];
  yyGLRState yyleftmost_state;

  for (yyi = yynrhs, yys = yyx->yystate; 0 < yyi; yyi -= 1, yys = yys->yypred)
    yystates[yyi] = yys;
  if (yys == NULL)
    {
      yyleftmost_state.yyposn = 0;
      yystates[0] = &yyleftmost_state;
    }
  else
    yystates[0] = yys;

  if (yyx->yystate->yyposn < yys->yyposn + 1)
    YYFPRINTF (stderr, "%*s%s -> <Rule %d, empty>\n",
	       yyindent, "", yytokenName (yylhsNonterm (yyx->yyrule)),
	       yyx->yyrule - 1);
  else
    YYFPRINTF (stderr, "%*s%s -> <Rule %d, tokens %lu .. %lu>\n",
	       yyindent, "", yytokenName (yylhsNonterm (yyx->yyrule)),
	       yyx->yyrule - 1, (unsigned long int) (yys->yyposn + 1),
	       (unsigned long int) yyx->yystate->yyposn);
  for (yyi = 1; yyi <= yynrhs; yyi += 1)
    {
      if (yystates[yyi]->yyresolved)
	{
	  if (yystates[yyi-1]->yyposn+1 > yystates[yyi]->yyposn)
	    YYFPRINTF (stderr, "%*s%s <empty>\n", yyindent+2, "",
		       yytokenName (yyrhs[yyprhs[yyx->yyrule]+yyi-1]));
	  else
	    YYFPRINTF (stderr, "%*s%s <tokens %lu .. %lu>\n", yyindent+2, "",
		       yytokenName (yyrhs[yyprhs[yyx->yyrule]+yyi-1]),
		       (unsigned long int) (yystates[yyi - 1]->yyposn + 1),
		       (unsigned long int) yystates[yyi]->yyposn);
	}
      else
	yyreportTree (yystates[yyi]->yysemantics.yyfirstVal, yyindent+2);
    }
}
#endif

/*ARGSUSED*/ static YYRESULTTAG
yyreportAmbiguity (yySemanticOption* yyx0,
		   yySemanticOption* yyx1)
{
  YYUSE (yyx0);
  YYUSE (yyx1);

#if YYDEBUG
  YYFPRINTF (stderr, "Ambiguity detected.\n");
  YYFPRINTF (stderr, "Option 1,\n");
  yyreportTree (yyx0, 2);
  YYFPRINTF (stderr, "\nOption 2,\n");
  yyreportTree (yyx1, 2);
  YYFPRINTF (stderr, "\n");
#endif

  yyerror (YY_("syntax is ambiguous"));
  return yyabort;
}

/** Starting at and including state S1, resolve the location for each of the
 *  previous N1 states that is unresolved.  The first semantic option of a state
 *  is always chosen.  */
static void
yyresolveLocations (yyGLRState* yys1, int yyn1,
		    yyGLRStack *yystackp)
{
  if (0 < yyn1)
    {
      yyresolveLocations (yys1->yypred, yyn1 - 1, yystackp);
      if (!yys1->yyresolved)
	{
	  yySemanticOption *yyoption;
	  yyGLRStackItem yyrhsloc[1 + YYMAXRHS];
	  int yynrhs;
	  int yychar_current;
	  YYSTYPE yylval_current;
	  YYLTYPE yylloc_current;
	  yyoption = yys1->yysemantics.yyfirstVal;
	  YYASSERT (yyoption != NULL);
	  yynrhs = yyrhsLength (yyoption->yyrule);
	  if (yynrhs > 0)
	    {
	      yyGLRState *yys;
	      int yyn;
	      yyresolveLocations (yyoption->yystate, yynrhs,
				  yystackp);
	      for (yys = yyoption->yystate, yyn = yynrhs;
		   yyn > 0;
		   yys = yys->yypred, yyn -= 1)
		yyrhsloc[yyn].yystate.yyloc = yys->yyloc;
	    }
	  else
	    {
	      /* Both yyresolveAction and yyresolveLocations traverse the GSS
		 in reverse rightmost order.  It is only necessary to invoke
		 yyresolveLocations on a subforest for which yyresolveAction
		 would have been invoked next had an ambiguity not been
		 detected.  Thus the location of the previous state (but not
		 necessarily the previous state itself) is guaranteed to be
		 resolved already.  */
	      yyGLRState *yyprevious = yyoption->yystate;
	      yyrhsloc[0].yystate.yyloc = yyprevious->yyloc;
	    }
	  yychar_current = yychar;
	  yylval_current = yylval;
	  yylloc_current = yylloc;
	  yychar = yyoption->yyrawchar;
	  yylval = yyoption->yyval;
	  yylloc = yyoption->yyloc;
	  YYLLOC_DEFAULT ((yys1->yyloc), yyrhsloc, yynrhs);
	  yychar = yychar_current;
	  yylval = yylval_current;
	  yylloc = yylloc_current;
	}
    }
}

/** Resolve the ambiguity represented in state S, perform the indicated
 *  actions, and set the semantic value of S.  If result != yyok, the chain of
 *  semantic options in S has been cleared instead or it has been left
 *  unmodified except that redundant options may have been removed.  Regardless
 *  of whether result = yyok, S has been left with consistent data so that
 *  yydestroyGLRState can be invoked if necessary.  */
static YYRESULTTAG
yyresolveValue (yyGLRState* yys, yyGLRStack* yystackp)
{
  yySemanticOption* yyoptionList = yys->yysemantics.yyfirstVal;
  yySemanticOption* yybest;
  yySemanticOption** yypp;
  yybool yymerge;
  YYSTYPE yysval;
  YYRESULTTAG yyflag;
  YYLTYPE *yylocp = &yys->yyloc;

  yybest = yyoptionList;
  yymerge = yyfalse;
  for (yypp = &yyoptionList->yynext; *yypp != NULL; )
    {
      yySemanticOption* yyp = *yypp;

      if (yyidenticalOptions (yybest, yyp))
	{
	  yymergeOptionSets (yybest, yyp);
	  *yypp = yyp->yynext;
	}
      else
	{
	  switch (yypreference (yybest, yyp))
	    {
	    case 0:
	      yyresolveLocations (yys, 1, yystackp);
	      return yyreportAmbiguity (yybest, yyp);
	      break;
	    case 1:
	      yymerge = yytrue;
	      break;
	    case 2:
	      break;
	    case 3:
	      yybest = yyp;
	      yymerge = yyfalse;
	      break;
	    default:
	      /* This cannot happen so it is not worth a YYASSERT (yyfalse),
		 but some compilers complain if the default case is
		 omitted.  */
	      break;
	    }
	  yypp = &yyp->yynext;
	}
    }

  if (yymerge)
    {
      yySemanticOption* yyp;
      int yyprec = yydprec[yybest->yyrule];
      yyflag = yyresolveAction (yybest, yystackp, &yysval,
				yylocp);
      if (yyflag == yyok)
	for (yyp = yybest->yynext; yyp != NULL; yyp = yyp->yynext)
	  {
	    if (yyprec == yydprec[yyp->yyrule])
	      {
		YYSTYPE yysval_other;
		YYLTYPE yydummy;
		yyflag = yyresolveAction (yyp, yystackp, &yysval_other,
					  &yydummy);
		if (yyflag != yyok)
		  {
		    yydestruct ("Cleanup: discarding incompletely merged value for",
				yystos[yys->yylrState],
				&yysval, yylocp);
		    break;
		  }
		yyuserMerge (yymerger[yyp->yyrule], &yysval, &yysval_other);
	      }
	  }
    }
  else
    yyflag = yyresolveAction (yybest, yystackp, &yysval, yylocp);

  if (yyflag == yyok)
    {
      yys->yyresolved = yytrue;
      yys->yysemantics.yysval = yysval;
    }
  else
    yys->yysemantics.yyfirstVal = NULL;
  return yyflag;
}

static YYRESULTTAG
yyresolveStack (yyGLRStack* yystackp)
{
  if (yystackp->yysplitPoint != NULL)
    {
      yyGLRState* yys;
      int yyn;

      for (yyn = 0, yys = yystackp->yytops.yystates[0];
	   yys != yystackp->yysplitPoint;
	   yys = yys->yypred, yyn += 1)
	continue;
      YYCHK (yyresolveStates (yystackp->yytops.yystates[0], yyn, yystackp
			     ));
    }
  return yyok;
}

static void
yycompressStack (yyGLRStack* yystackp)
{
  yyGLRState* yyp, *yyq, *yyr;

  if (yystackp->yytops.yysize != 1 || yystackp->yysplitPoint == NULL)
    return;

  for (yyp = yystackp->yytops.yystates[0], yyq = yyp->yypred, yyr = NULL;
       yyp != yystackp->yysplitPoint;
       yyr = yyp, yyp = yyq, yyq = yyp->yypred)
    yyp->yypred = yyr;

  yystackp->yyspaceLeft += yystackp->yynextFree - yystackp->yyitems;
  yystackp->yynextFree = ((yyGLRStackItem*) yystackp->yysplitPoint) + 1;
  yystackp->yyspaceLeft -= yystackp->yynextFree - yystackp->yyitems;
  yystackp->yysplitPoint = NULL;
  yystackp->yylastDeleted = NULL;

  while (yyr != NULL)
    {
      yystackp->yynextFree->yystate = *yyr;
      yyr = yyr->yypred;
      yystackp->yynextFree->yystate.yypred = &yystackp->yynextFree[-1].yystate;
      yystackp->yytops.yystates[0] = &yystackp->yynextFree->yystate;
      yystackp->yynextFree += 1;
      yystackp->yyspaceLeft -= 1;
    }
}

static YYRESULTTAG
yyprocessOneStack (yyGLRStack* yystackp, size_t yyk,
		   size_t yyposn)
{
  int yyaction;
  const short int* yyconflicts;
  yyRuleNum yyrule;

  while (yystackp->yytops.yystates[yyk] != NULL)
    {
      yyStateNum yystate = yystackp->yytops.yystates[yyk]->yylrState;
      YYDPRINTF ((stderr, "Stack %lu Entering state %d\n",
		  (unsigned long int) yyk, yystate));

      YYASSERT (yystate != YYFINAL);

      if (yyisDefaultedState (yystate))
	{
	  yyrule = yydefaultAction (yystate);
	  if (yyrule == 0)
	    {
	      YYDPRINTF ((stderr, "Stack %lu dies.\n",
			  (unsigned long int) yyk));
	      yymarkStackDeleted (yystackp, yyk);
	      return yyok;
	    }
	  YYCHK (yyglrReduce (yystackp, yyk, yyrule, yyfalse));
	}
      else
	{
	  yySymbol yytoken;
	  yystackp->yytops.yylookaheadNeeds[yyk] = yytrue;
	  if (yychar == YYEMPTY)
	    {
	      YYDPRINTF ((stderr, "Reading a token: "));
	      yychar = YYLEX;
	    }

	  if (yychar <= YYEOF)
	    {
	      yychar = yytoken = YYEOF;
	      YYDPRINTF ((stderr, "Now at end of input.\n"));
	    }
	  else
	    {
	      yytoken = YYTRANSLATE (yychar);
	      YY_SYMBOL_PRINT ("Next token is", yytoken, &yylval, &yylloc);
	    }

	  yygetLRActions (yystate, yytoken, &yyaction, &yyconflicts);

	  while (*yyconflicts != 0)
	    {
	      size_t yynewStack = yysplitStack (yystackp, yyk);
	      YYDPRINTF ((stderr, "Splitting off stack %lu from %lu.\n",
			  (unsigned long int) yynewStack,
			  (unsigned long int) yyk));
	      YYCHK (yyglrReduce (yystackp, yynewStack,
				  *yyconflicts, yyfalse));
	      YYCHK (yyprocessOneStack (yystackp, yynewStack,
					yyposn));
	      yyconflicts += 1;
	    }

	  if (yyisShiftAction (yyaction))
	    break;
	  else if (yyisErrorAction (yyaction))
	    {
	      YYDPRINTF ((stderr, "Stack %lu dies.\n",
			  (unsigned long int) yyk));
	      yymarkStackDeleted (yystackp, yyk);
	      break;
	    }
	  else
	    YYCHK (yyglrReduce (yystackp, yyk, -yyaction,
				yyfalse));
	}
    }
  return yyok;
}

/*ARGSUSED*/ static void
yyreportSyntaxError (yyGLRStack* yystackp)
{
  if (yystackp->yyerrState == 0)
    {
#if YYERROR_VERBOSE
      int yyn;
      yyn = yypact[yystackp->yytops.yystates[0]->yylrState];
      if (YYPACT_NINF < yyn && yyn <= YYLAST)
	{
	  yySymbol yytoken = YYTRANSLATE (yychar);
	  size_t yysize0 = yytnamerr (NULL, yytokenName (yytoken));
	  size_t yysize = yysize0;
	  size_t yysize1;
	  yybool yysize_overflow = yyfalse;
	  char* yymsg = NULL;
	  enum { YYERROR_VERBOSE_ARGS_MAXIMUM = 5 };
	  char const *yyarg[YYERROR_VERBOSE_ARGS_MAXIMUM];
	  int yyx;
	  char *yyfmt;
	  char const *yyf;
	  static char const yyunexpected[] = "syntax error, unexpected %s";
	  static char const yyexpecting[] = ", expecting %s";
	  static char const yyor[] = " or %s";
	  char yyformat[sizeof yyunexpected
			+ sizeof yyexpecting - 1
			+ ((YYERROR_VERBOSE_ARGS_MAXIMUM - 2)
			   * (sizeof yyor - 1))];
	  char const *yyprefix = yyexpecting;

	  /* Start YYX at -YYN if negative to avoid negative indexes in
	     YYCHECK.  */
	  int yyxbegin = yyn < 0 ? -yyn : 0;

	  /* Stay within bounds of both yycheck and yytname.  */
	  int yychecklim = YYLAST - yyn + 1;
	  int yyxend = yychecklim < YYNTOKENS ? yychecklim : YYNTOKENS;
	  int yycount = 1;

	  yyarg[0] = yytokenName (yytoken);
	  yyfmt = yystpcpy (yyformat, yyunexpected);

	  for (yyx = yyxbegin; yyx < yyxend; ++yyx)
	    if (yycheck[yyx + yyn] == yyx && yyx != YYTERROR)
	      {
		if (yycount == YYERROR_VERBOSE_ARGS_MAXIMUM)
		  {
		    yycount = 1;
		    yysize = yysize0;
		    yyformat[sizeof yyunexpected - 1] = '\0';
		    break;
		  }
		yyarg[yycount++] = yytokenName (yyx);
		yysize1 = yysize + yytnamerr (NULL, yytokenName (yyx));
		yysize_overflow |= yysize1 < yysize;
		yysize = yysize1;
		yyfmt = yystpcpy (yyfmt, yyprefix);
		yyprefix = yyor;
	      }

	  yyf = YY_(yyformat);
	  yysize1 = yysize + strlen (yyf);
	  yysize_overflow |= yysize1 < yysize;
	  yysize = yysize1;

	  if (!yysize_overflow)
	    yymsg = (char *) YYMALLOC (yysize);

	  if (yymsg)
	    {
	      char *yyp = yymsg;
	      int yyi = 0;
	      while ((*yyp = *yyf))
		{
		  if (*yyp == '%' && yyf[1] == 's' && yyi < yycount)
		    {
		      yyp += yytnamerr (yyp, yyarg[yyi++]);
		      yyf += 2;
		    }
		  else
		    {
		      yyp++;
		      yyf++;
		    }
		}
	      yyerror (yymsg);
	      YYFREE (yymsg);
	    }
	  else
	    {
	      yyerror (YY_("syntax error"));
	      yyMemoryExhausted (yystackp);
	    }
	}
      else
#endif /* YYERROR_VERBOSE */
	yyerror (YY_("syntax error"));
      yynerrs += 1;
    }
}

/* Recover from a syntax error on *YYSTACKP, assuming that *YYSTACKP->YYTOKENP,
   yylval, and yylloc are the syntactic category, semantic value, and location
   of the lookahead.  */
/*ARGSUSED*/ static void
yyrecoverSyntaxError (yyGLRStack* yystackp)
{
  size_t yyk;
  int yyj;

  if (yystackp->yyerrState == 3)
    /* We just shifted the error token and (perhaps) took some
       reductions.  Skip tokens until we can proceed.  */
    while (YYID (yytrue))
      {
	yySymbol yytoken;
	if (yychar == YYEOF)
	  yyFail (yystackp, NULL);
	if (yychar != YYEMPTY)
	  {
	    /* We throw away the lookahead, but the error range
	       of the shifted error token must take it into account.  */
	    yyGLRState *yys = yystackp->yytops.yystates[0];
	    yyGLRStackItem yyerror_range[3];
	    yyerror_range[1].yystate.yyloc = yys->yyloc;
	    yyerror_range[2].yystate.yyloc = yylloc;
	    YYLLOC_DEFAULT ((yys->yyloc), yyerror_range, 2);
	    yytoken = YYTRANSLATE (yychar);
	    yydestruct ("Error: discarding",
			yytoken, &yylval, &yylloc);
	  }
	YYDPRINTF ((stderr, "Reading a token: "));
	yychar = YYLEX;
	if (yychar <= YYEOF)
	  {
	    yychar = yytoken = YYEOF;
	    YYDPRINTF ((stderr, "Now at end of input.\n"));
	  }
	else
	  {
	    yytoken = YYTRANSLATE (yychar);
	    YY_SYMBOL_PRINT ("Next token is", yytoken, &yylval, &yylloc);
	  }
	yyj = yypact[yystackp->yytops.yystates[0]->yylrState];
	if (yyis_pact_ninf (yyj))
	  return;
	yyj += yytoken;
	if (yyj < 0 || YYLAST < yyj || yycheck[yyj] != yytoken)
	  {
	    if (yydefact[yystackp->yytops.yystates[0]->yylrState] != 0)
	      return;
	  }
	else if (yytable[yyj] != 0 && ! yyis_table_ninf (yytable[yyj]))
	  return;
      }

  /* Reduce to one stack.  */
  for (yyk = 0; yyk < yystackp->yytops.yysize; yyk += 1)
    if (yystackp->yytops.yystates[yyk] != NULL)
      break;
  if (yyk >= yystackp->yytops.yysize)
    yyFail (yystackp, NULL);
  for (yyk += 1; yyk < yystackp->yytops.yysize; yyk += 1)
    yymarkStackDeleted (yystackp, yyk);
  yyremoveDeletes (yystackp);
  yycompressStack (yystackp);

  /* Now pop stack until we find a state that shifts the error token.  */
  yystackp->yyerrState = 3;
  while (yystackp->yytops.yystates[0] != NULL)
    {
      yyGLRState *yys = yystackp->yytops.yystates[0];
      yyj = yypact[yys->yylrState];
      if (! yyis_pact_ninf (yyj))
	{
	  yyj += YYTERROR;
	  if (0 <= yyj && yyj <= YYLAST && yycheck[yyj] == YYTERROR
	      && yyisShiftAction (yytable[yyj]))
	    {
	      /* Shift the error token having adjusted its location.  */
	      YYLTYPE yyerrloc;
	      yystackp->yyerror_range[2].yystate.yyloc = yylloc;
	      YYLLOC_DEFAULT (yyerrloc, (yystackp->yyerror_range), 2);
	      YY_SYMBOL_PRINT ("Shifting", yystos[yytable[yyj]],
			       &yylval, &yyerrloc);
	      yyglrShift (yystackp, 0, yytable[yyj],
			  yys->yyposn, &yylval, &yyerrloc);
	      yys = yystackp->yytops.yystates[0];
	      break;
	    }
	}
      yystackp->yyerror_range[1].yystate.yyloc = yys->yyloc;
      if (yys->yypred != NULL)
	yydestroyGLRState ("Error: popping", yys);
      yystackp->yytops.yystates[0] = yys->yypred;
      yystackp->yynextFree -= 1;
      yystackp->yyspaceLeft += 1;
    }
  if (yystackp->yytops.yystates[0] == NULL)
    yyFail (yystackp, NULL);
}

#define YYCHK1(YYE)							     \
  do {									     \
    switch (YYE) {							     \
    case yyok:								     \
      break;								     \
    case yyabort:							     \
      goto yyabortlab;							     \
    case yyaccept:							     \
      goto yyacceptlab;							     \
    case yyerr:								     \
      goto yyuser_error;						     \
    default:								     \
      goto yybuglab;							     \
    }									     \
  } while (YYID (0))


/*----------.
| yyparse.  |
`----------*/

int
yyparse (void)
{
  int yyresult;
  yyGLRStack yystack;
  yyGLRStack* const yystackp = &yystack;
  size_t yyposn;

  YYDPRINTF ((stderr, "Starting parse\n"));

  yychar = YYEMPTY;
  yylval = yyval_default;

#if YYLTYPE_IS_TRIVIAL
  yylloc.first_line   = yylloc.last_line   = 1;
  yylloc.first_column = yylloc.last_column = 1;
#endif


  if (! yyinitGLRStack (yystackp, YYINITDEPTH))
    goto yyexhaustedlab;
  switch (YYSETJMP (yystack.yyexception_buffer))
    {
    case 0: break;
    case 1: goto yyabortlab;
    case 2: goto yyexhaustedlab;
    default: goto yybuglab;
    }
  yyglrShift (&yystack, 0, 0, 0, &yylval, &yylloc);
  yyposn = 0;

  while (YYID (yytrue))
    {
      /* For efficiency, we have two loops, the first of which is
	 specialized to deterministic operation (single stack, no
	 potential ambiguity).  */
      /* Standard mode */
      while (YYID (yytrue))
	{
	  yyRuleNum yyrule;
	  int yyaction;
	  const short int* yyconflicts;

	  yyStateNum yystate = yystack.yytops.yystates[0]->yylrState;
	  YYDPRINTF ((stderr, "Entering state %d\n", yystate));
	  if (yystate == YYFINAL)
	    goto yyacceptlab;
	  if (yyisDefaultedState (yystate))
	    {
	      yyrule = yydefaultAction (yystate);
	      if (yyrule == 0)
		{
		  yystack.yyerror_range[1].yystate.yyloc = yylloc;
		  yyreportSyntaxError (&yystack);
		  goto yyuser_error;
		}
	      YYCHK1 (yyglrReduce (&yystack, 0, yyrule, yytrue));
	    }
	  else
	    {
	      yySymbol yytoken;
	      if (yychar == YYEMPTY)
		{
		  YYDPRINTF ((stderr, "Reading a token: "));
		  yychar = YYLEX;
		}

	      if (yychar <= YYEOF)
		{
		  yychar = yytoken = YYEOF;
		  YYDPRINTF ((stderr, "Now at end of input.\n"));
		}
	      else
		{
		  yytoken = YYTRANSLATE (yychar);
		  YY_SYMBOL_PRINT ("Next token is", yytoken, &yylval, &yylloc);
		}

	      yygetLRActions (yystate, yytoken, &yyaction, &yyconflicts);
	      if (*yyconflicts != 0)
		break;
	      if (yyisShiftAction (yyaction))
		{
		  YY_SYMBOL_PRINT ("Shifting", yytoken, &yylval, &yylloc);
		  yychar = YYEMPTY;
		  yyposn += 1;
		  yyglrShift (&yystack, 0, yyaction, yyposn, &yylval, &yylloc);
		  if (0 < yystack.yyerrState)
		    yystack.yyerrState -= 1;
		}
	      else if (yyisErrorAction (yyaction))
		{
		  yystack.yyerror_range[1].yystate.yyloc = yylloc;
		  yyreportSyntaxError (&yystack);
		  goto yyuser_error;
		}
	      else
		YYCHK1 (yyglrReduce (&yystack, 0, -yyaction, yytrue));
	    }
	}

      while (YYID (yytrue))
	{
	  yySymbol yytoken_to_shift;
	  size_t yys;

	  for (yys = 0; yys < yystack.yytops.yysize; yys += 1)
	    yystackp->yytops.yylookaheadNeeds[yys] = yychar != YYEMPTY;

	  /* yyprocessOneStack returns one of three things:

	      - An error flag.  If the caller is yyprocessOneStack, it
		immediately returns as well.  When the caller is finally
		yyparse, it jumps to an error label via YYCHK1.

	      - yyok, but yyprocessOneStack has invoked yymarkStackDeleted
		(&yystack, yys), which sets the top state of yys to NULL.  Thus,
		yyparse's following invocation of yyremoveDeletes will remove
		the stack.

	      - yyok, when ready to shift a token.

	     Except in the first case, yyparse will invoke yyremoveDeletes and
	     then shift the next token onto all remaining stacks.  This
	     synchronization of the shift (that is, after all preceding
	     reductions on all stacks) helps prevent double destructor calls
	     on yylval in the event of memory exhaustion.  */

	  for (yys = 0; yys < yystack.yytops.yysize; yys += 1)
	    YYCHK1 (yyprocessOneStack (&yystack, yys, yyposn));
	  yyremoveDeletes (&yystack);
	  if (yystack.yytops.yysize == 0)
	    {
	      yyundeleteLastStack (&yystack);
	      if (yystack.yytops.yysize == 0)
		yyFail (&yystack, YY_("syntax error"));
	      YYCHK1 (yyresolveStack (&yystack));
	      YYDPRINTF ((stderr, "Returning to deterministic operation.\n"));
	      yystack.yyerror_range[1].yystate.yyloc = yylloc;
	      yyreportSyntaxError (&yystack);
	      goto yyuser_error;
	    }

	  /* If any yyglrShift call fails, it will fail after shifting.  Thus,
	     a copy of yylval will already be on stack 0 in the event of a
	     failure in the following loop.  Thus, yychar is set to YYEMPTY
	     before the loop to make sure the user destructor for yylval isn't
	     called twice.  */
	  yytoken_to_shift = YYTRANSLATE (yychar);
	  yychar = YYEMPTY;
	  yyposn += 1;
	  for (yys = 0; yys < yystack.yytops.yysize; yys += 1)
	    {
	      int yyaction;
	      const short int* yyconflicts;
	      yyStateNum yystate = yystack.yytops.yystates[yys]->yylrState;
	      yygetLRActions (yystate, yytoken_to_shift, &yyaction,
			      &yyconflicts);
	      /* Note that yyconflicts were handled by yyprocessOneStack.  */
	      YYDPRINTF ((stderr, "On stack %lu, ", (unsigned long int) yys));
	      YY_SYMBOL_PRINT ("shifting", yytoken_to_shift, &yylval, &yylloc);
	      yyglrShift (&yystack, yys, yyaction, yyposn,
			  &yylval, &yylloc);
	      YYDPRINTF ((stderr, "Stack %lu now in state #%d\n",
			  (unsigned long int) yys,
			  yystack.yytops.yystates[yys]->yylrState));
	    }

	  if (yystack.yytops.yysize == 1)
	    {
	      YYCHK1 (yyresolveStack (&yystack));
	      YYDPRINTF ((stderr, "Returning to deterministic operation.\n"));
	      yycompressStack (&yystack);
	      break;
	    }
	}
      continue;
    yyuser_error:
      yyrecoverSyntaxError (&yystack);
      yyposn = yystack.yytops.yystates[0]->yyposn;
    }

 yyacceptlab:
  yyresult = 0;
  goto yyreturn;

 yybuglab:
  YYASSERT (yyfalse);
  goto yyabortlab;

 yyabortlab:
  yyresult = 1;
  goto yyreturn;

 yyexhaustedlab:
  yyerror (YY_("memory exhausted"));
  yyresult = 2;
  goto yyreturn;

 yyreturn:
  if (yychar != YYEMPTY)
    yydestruct ("Cleanup: discarding lookahead",
		YYTRANSLATE (yychar),
		&yylval, &yylloc);

  /* If the stack is well-formed, pop the stack until it is empty,
     destroying its entries as we go.  But free the stack regardless
     of whether it is well-formed.  */
  if (yystack.yyitems)
    {
      yyGLRState** yystates = yystack.yytops.yystates;
      if (yystates)
	{
	  size_t yysize = yystack.yytops.yysize;
	  size_t yyk;
	  for (yyk = 0; yyk < yysize; yyk += 1)
	    if (yystates[yyk])
	      {
		while (yystates[yyk])
		  {
		    yyGLRState *yys = yystates[yyk];
		    yystack.yyerror_range[1].yystate.yyloc = yys->yyloc;
		    if (yys->yypred != NULL)
		      yydestroyGLRState ("Cleanup: popping", yys);
		    yystates[yyk] = yys->yypred;
		    yystack.yynextFree -= 1;
		    yystack.yyspaceLeft += 1;
		  }
		break;
	      }
	}
      yyfreeGLRStack (&yystack);
    }

  /* Make sure YYID is used.  */
  return YYID (yyresult);
}

/* DEBUGGING ONLY */
#if YYDEBUG
static void yypstack (yyGLRStack* yystackp, size_t yyk)
  __attribute__ ((__unused__));
static void yypdumpstack (yyGLRStack* yystackp) __attribute__ ((__unused__));

static void
yy_yypstack (yyGLRState* yys)
{
  if (yys->yypred)
    {
      yy_yypstack (yys->yypred);
      YYFPRINTF (stderr, " -> ");
    }
  YYFPRINTF (stderr, "%d@%lu", yys->yylrState,
             (unsigned long int) yys->yyposn);
}

static void
yypstates (yyGLRState* yyst)
{
  if (yyst == NULL)
    YYFPRINTF (stderr, "<null>");
  else
    yy_yypstack (yyst);
  YYFPRINTF (stderr, "\n");
}

static void
yypstack (yyGLRStack* yystackp, size_t yyk)
{
  yypstates (yystackp->yytops.yystates[yyk]);
}

#define YYINDEX(YYX)							     \
    ((YYX) == NULL ? -1 : (yyGLRStackItem*) (YYX) - yystackp->yyitems)


static void
yypdumpstack (yyGLRStack* yystackp)
{
  yyGLRStackItem* yyp;
  size_t yyi;
  for (yyp = yystackp->yyitems; yyp < yystackp->yynextFree; yyp += 1)
    {
      YYFPRINTF (stderr, "%3lu. ",
                 (unsigned long int) (yyp - yystackp->yyitems));
      if (*(yybool *) yyp)
	{
	  YYFPRINTF (stderr, "Res: %d, LR State: %d, posn: %lu, pred: %ld",
		     yyp->yystate.yyresolved, yyp->yystate.yylrState,
		     (unsigned long int) yyp->yystate.yyposn,
		     (long int) YYINDEX (yyp->yystate.yypred));
	  if (! yyp->yystate.yyresolved)
	    YYFPRINTF (stderr, ", firstVal: %ld",
		       (long int) YYINDEX (yyp->yystate
                                             .yysemantics.yyfirstVal));
	}
      else
	{
	  YYFPRINTF (stderr, "Option. rule: %d, state: %ld, next: %ld",
		     yyp->yyoption.yyrule - 1,
		     (long int) YYINDEX (yyp->yyoption.yystate),
		     (long int) YYINDEX (yyp->yyoption.yynext));
	}
      YYFPRINTF (stderr, "\n");
    }
  YYFPRINTF (stderr, "Tops:");
  for (yyi = 0; yyi < yystackp->yytops.yysize; yyi += 1)
    YYFPRINTF (stderr, "%lu: %ld; ", (unsigned long int) yyi,
	       (long int) YYINDEX (yystackp->yytops.yystates[yyi]));
  YYFPRINTF (stderr, "\n");
}
#endif



/* Line 2634 of glr.c  */
#line 1815 "sql.ypp"


void
emit(char *s, ...)
{
  extern int yylineno;

  va_list ap;
  va_start(ap, s);

  printf("rpn: ");
  vfprintf(stdout, s, ap);
  printf("\n");
}

void
yyerror(const char *s, ...)
{
	extern int  yylineno;

	va_list ap;
	va_start(ap, s);

	fprintf(stderr, "%d: error: ", yylineno);
	vfprintf(stderr, s, ap);
//  	printf("at %s",yytext);
	fprintf(stderr, "\n");
  
  //	yychar = YYEMPTY;
  	//yyerrstatus = 0;
//	yyclearin;
//	yyerrok;
}


int readInputForLexer( char *buffer, int *numBytesRead, int maxBytesToRead ) {
	int numBytesToRead = maxBytesToRead;
	int bytesRemaining = strlen(globalInputText)-globalReadOffset;
	int i;
	if ( numBytesToRead > bytesRemaining ) { numBytesToRead = bytesRemaining; }
	for ( i = 0; i < numBytesToRead; i++ ) {
		buffer[i] = globalInputText[globalReadOffset+i];
	}
	*numBytesRead = numBytesToRead;
	globalReadOffset += numBytesToRead;
	return 0;
}

/*
main(int ac, char **av)
{
	char c;
	while(1)
	{
		int charnum=0;
		memset(globalInputText, 0, sizeof(globalInputText));
		//cout<<"globalInputText: "<<globalInputText;
		globalReadOffset = 0;
	    	while(1)
		{
			c=getchar();
			globalInputText[charnum++]=c;
			if(c==';')
			{
				globalInputText[charnum++]='\0';
				break;
			}
		}
//		strcpy(globalInputText, "select a from b; sd; select a from b;");	// 2014-3-4---测试用---by余楷
		cout<<"globalInputText: "<<globalInputText<<endl;
		if(!yyparse())
	    		printf("SQL parse worked\n");
		else
    			printf("SQL parse failed\n");
		puts("input a number! 0 quit, 1 continue");
		c = getchar();
		while( c == '\n')
			c= getchar();
		if (c == '0') 
			break;
		else 
			cout<<c<<endl;
		getchar();
	}
} 
*/
