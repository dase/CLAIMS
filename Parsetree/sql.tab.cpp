
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
#define YYLSP_NEEDED 0




/* Copy the first part of user declarations.  */

/* Line 172 of glr.c  */
#line 23 "sql.ypp"

#include <stdlib.h>
#include <stdarg.h>
#include <string.h>
#include <iostream>
#include <stdio.h>
#include <malloc.h>
#include "sql_node_struct.h"
using namespace std;

Node * parsetreeroot;
char globalInputText[10000];
int globalReadOffset;
int readInputForLexer( char *buffer, int *numBytesRead, int maxBytesToRead );


/* Line 172 of glr.c  */
#line 76 "sql.tab.cpp"



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
#line 111 "sql.tab.cpp"

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


#ifdef __cplusplus
# define YYOPTIONAL_LOC(Name) /* empty */
#else
# define YYOPTIONAL_LOC(Name) Name __attribute__ ((__unused__))
#endif

#ifndef YYASSERT
# define YYASSERT(condition) ((void) ((condition) || (abort (), 0)))
#endif

/* YYFINAL -- State number of the termination state.  */
#define YYFINAL  235
/* YYLAST -- Last index in YYTABLE.  */
#define YYLAST   2789

/* YYNTOKENS -- Number of terminals.  */
#define YYNTOKENS  332
/* YYNNTS -- Number of nonterminals.  */
#define YYNNTS  168
/* YYNRULES -- Number of rules.  */
#define YYNRULES  589
/* YYNRULES -- Number of states.  */
#define YYNSTATES  1181
/* YYMAXRHS -- Maximum number of symbols on right-hand side of rule.  */
#define YYMAXRHS 11
/* YYMAXLEFT -- Maximum number of symbols to the left of a handle
   accessed by $0, $-1, etc., in any rule.  */
#define YYMAXLEFT 0

/* YYTRANSLATE(X) -- Bison symbol number corresponding to X.  */
#define YYUNDEFTOK  2
#define YYMAXUTOK   571

#define YYTRANSLATE(YYX)						\
  ((unsigned int) (YYX) <= YYMAXUTOK ? yytranslate[YYX] : YYUNDEFTOK)

/* YYTRANSLATE[YYLEX] -- Bison symbol number corresponding to YYLEX.  */
static const unsigned short int yytranslate[] =
{
       0,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,    17,     2,     2,     2,    28,    22,     2,
     329,   330,    26,    24,   327,    25,   328,    27,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,   326,
       2,     2,     2,   331,     2,     2,     2,     2,     2,     2,
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
     324,   325
};

#if YYDEBUG
/* YYPRHS[YYN] -- Index of the first RHS symbol of rule number YYN in
   YYRHS.  */
static const unsigned short int yyprhs[] =
{
       0,     0,     3,     6,    10,    12,    16,    28,    29,    32,
      33,    38,    41,    46,    47,    49,    51,    52,    55,    56,
      59,    60,    64,    65,    68,    73,    74,    77,    79,    83,
      84,    87,    90,    93,    96,    99,   102,   105,   108,   110,
     114,   116,   119,   121,   125,   127,   129,   132,   137,   141,
     145,   146,   148,   151,   153,   154,   160,   164,   170,   177,
     183,   184,   186,   188,   189,   191,   193,   195,   198,   201,
     202,   203,   205,   208,   213,   215,   219,   223,   225,   233,
     236,   239,   242,   243,   250,   253,   258,   259,   262,   270,
     272,   281,   282,   288,   289,   292,   295,   298,   301,   303,
     304,   305,   309,   313,   319,   321,   323,   327,   331,   339,
     347,   351,   355,   361,   367,   369,   378,   386,   394,   396,
     405,   406,   409,   412,   416,   422,   428,   436,   438,   443,
     448,   449,   453,   455,   464,   475,   485,   492,   504,   513,
     517,   518,   520,   522,   523,   525,   527,   531,   535,   541,
     546,   551,   557,   563,   564,   568,   571,   575,   579,   583,
     587,   590,   594,   598,   601,   605,   611,   614,   616,   619,
     623,   625,   630,   635,   637,   643,   649,   651,   655,   659,
     666,   670,   674,   678,   683,   684,   686,   687,   690,   694,
     698,   699,   701,   703,   714,   715,   717,   719,   721,   722,
     725,   727,   729,   731,   735,   739,   741,   747,   749,   754,
     759,   760,   763,   765,   772,   774,   778,   782,   788,   789,
     791,   793,   795,   799,   800,   804,   812,   818,   828,   830,
     833,   836,   838,   841,   843,   845,   847,   850,   853,   854,
     856,   861,   862,   866,   869,   870,   873,   875,   880,   882,
     887,   889,   891,   893,   896,   902,   906,   908,   912,   914,
     918,   922,   924,   927,   930,   932,   934,   937,   939,   946,
     947,   949,   951,   954,   957,   960,   962,   964,   968,   970,
     974,   976,   981,   987,   989,   993,   995,   999,  1001,  1011,
    1013,  1017,  1019,  1022,  1024,  1026,  1028,  1030,  1032,  1034,
    1036,  1038,  1041,  1043,  1044,  1047,  1050,  1053,  1056,  1059,
    1062,  1064,  1074,  1076,  1086,  1088,  1092,  1095,  1098,  1101,
    1103,  1105,  1107,  1109,  1111,  1114,  1116,  1125,  1127,  1137,
    1139,  1149,  1151,  1155,  1159,  1165,  1167,  1176,  1177,  1180,
    1182,  1187,  1188,  1190,  1192,  1194,  1200,  1202,  1207,  1208,
    1210,  1212,  1214,  1216,  1218,  1220,  1225,  1226,  1228,  1230,
    1232,  1237,  1239,  1245,  1246,  1249,  1252,  1255,  1257,  1263,
    1265,  1270,  1274,  1282,  1287,  1292,  1297,  1301,  1305,  1310,
    1315,  1319,  1323,  1327,  1332,  1338,  1342,  1346,  1352,  1355,
    1359,  1364,  1370,  1376,  1381,  1386,  1390,  1394,  1395,  1397,
    1398,  1401,  1402,  1405,  1406,  1409,  1410,  1412,  1414,  1420,
    1422,  1426,  1428,  1432,  1433,  1435,  1437,  1439,  1443,  1445,
    1449,  1451,  1453,  1455,  1457,  1460,  1462,  1465,  1468,  1473,
    1475,  1477,  1481,  1482,  1484,  1486,  1488,  1494,  1498,  1503,
    1504,  1507,  1509,  1512,  1514,  1518,  1520,  1523,  1525,  1527,
    1532,  1536,  1540,  1544,  1545,  1548,  1550,  1552,  1555,  1557,
    1561,  1565,  1569,  1570,  1574,  1580,  1581,  1583,  1584,  1587,
    1590,  1591,  1596,  1600,  1603,  1607,  1611,  1615,  1619,  1623,
    1627,  1631,  1635,  1639,  1643,  1645,  1647,  1649,  1651,  1653,
    1657,  1663,  1666,  1671,  1673,  1675,  1677,  1679,  1683,  1687,
    1691,  1695,  1701,  1707,  1709,  1713,  1715,  1717,  1721,  1723,
    1725,  1727,  1729,  1733,  1737,  1741,  1745,  1749,  1753,  1756,
    1760,  1764,  1768,  1772,  1778,  1785,  1792,  1799,  1803,  1807,
    1811,  1815,  1818,  1821,  1825,  1829,  1833,  1838,  1842,  1847,
    1853,  1855,  1859,  1860,  1862,  1868,  1875,  1881,  1888,  1893,
    1899,  1904,  1909,  1914,  1919,  1924,  1929,  1934,  1939,  1946,
    1955,  1960,  1968,  1970,  1972,  1974,  1981,  1988,  1992,  1996,
    2000,  2004,  2008,  2012,  2016,  2020,  2024,  2029,  2036,  2040,
    2046,  2051,  2057,  2061,  2066,  2070,  2075,  2077,  2079,  2081
};

/* YYRHS -- A `-1'-separated list of the rules' RHS.  */
static const short int yyrhs[] =
{
     333,     0,    -1,   334,   326,    -1,   333,   334,   326,    -1,
     335,    -1,   187,   346,   347,    -1,   187,   346,   347,   105,
     349,   336,   337,   341,   342,   343,   344,    -1,    -1,   237,
     485,    -1,    -1,   108,    47,   338,   340,    -1,   485,   339,
      -1,   338,   327,   485,   339,    -1,    -1,    39,    -1,    81,
      -1,    -1,   239,   183,    -1,    -1,   109,   485,    -1,    -1,
     161,    47,   338,    -1,    -1,   134,   485,    -1,   134,   485,
     327,   485,    -1,    -1,   125,   345,    -1,     3,    -1,   345,
     327,     3,    -1,    -1,   346,    33,    -1,   346,    84,    -1,
     346,    85,    -1,   346,   110,    -1,   346,   206,    -1,   346,
     203,    -1,   346,   201,    -1,   346,   202,    -1,   348,    -1,
     347,   327,   348,    -1,    26,    -1,   485,   353,    -1,   350,
      -1,   349,   327,   350,    -1,   351,    -1,   354,    -1,     3,
     353,    -1,     3,   328,     3,   353,    -1,   362,   352,     3,
      -1,   329,   349,   330,    -1,    -1,    38,    -1,    38,     3,
      -1,     3,    -1,    -1,   350,   355,   127,   351,   359,    -1,
     350,   206,   351,    -1,   350,   206,   351,   156,   485,    -1,
     350,   357,   356,   127,   351,   360,    -1,   350,   152,   358,
     127,   351,    -1,    -1,   118,    -1,    62,    -1,    -1,   163,
      -1,   133,    -1,   182,    -1,   133,   356,    -1,   182,   356,
      -1,    -1,    -1,   360,    -1,   156,   485,    -1,   228,   329,
     345,   330,    -1,     3,    -1,   361,   327,     3,    -1,   329,
     335,   330,    -1,   363,    -1,    80,   364,   105,     3,   336,
     342,   343,    -1,   364,   144,    -1,   364,   169,    -1,   364,
     115,    -1,    -1,    80,   364,   365,   105,   349,   336,    -1,
       3,   366,    -1,   365,   327,     3,   366,    -1,    -1,   328,
      26,    -1,    80,   364,   105,   365,   228,   349,   336,    -1,
     367,    -1,   121,   369,   370,     3,   371,   232,   372,   368,
      -1,    -1,   156,   157,   128,   225,   374,    -1,    -1,   369,
     144,    -1,   369,    79,    -1,   369,   110,    -1,   369,   115,
      -1,   125,    -1,    -1,    -1,   329,   345,   330,    -1,   329,
     373,   330,    -1,   372,   327,   329,   373,   330,    -1,   485,
      -1,    78,    -1,   373,   327,   485,    -1,   373,   327,    78,
      -1,   121,   369,   370,     3,   190,   374,   368,    -1,   121,
     369,   370,     3,   371,   335,   368,    -1,     3,    20,   485,
      -1,     3,    20,    78,    -1,   374,   327,     3,    20,   485,
      -1,   374,   327,     3,    20,    78,    -1,   375,    -1,   177,
     369,   370,     3,   371,   232,   372,   368,    -1,   177,   369,
     370,     3,   190,   374,   368,    -1,   177,   369,   370,     3,
     371,   335,   368,    -1,   376,    -1,   225,   377,   349,   190,
     378,   336,   342,   343,    -1,    -1,   369,   144,    -1,   369,
     115,    -1,     3,    20,   485,    -1,     3,   328,     3,    20,
     485,    -1,   378,   327,     3,    20,   485,    -1,   378,   327,
       3,   328,     3,    20,   485,    -1,   379,    -1,    61,    68,
     380,     3,    -1,    61,   184,   380,     3,    -1,    -1,   114,
      18,    97,    -1,   381,    -1,    61,   384,   207,   380,     3,
     329,   385,   330,    -1,    61,   384,   207,   380,     3,   328,
       3,   329,   385,   330,    -1,    61,   384,   207,   380,     3,
     329,   385,   330,   382,    -1,    61,   384,   207,   380,     3,
     382,    -1,    61,   384,   207,   380,     3,   328,     3,   329,
     385,   330,   382,    -1,    61,   384,   207,   380,     3,   328,
       3,   382,    -1,   383,   352,   335,    -1,    -1,   115,    -1,
     177,    -1,    -1,   208,    -1,   386,    -1,   385,   327,   386,
      -1,     3,   493,   387,    -1,   166,   128,   329,   345,   330,
      -1,   128,   329,   345,   330,    -1,   116,   329,   345,   330,
      -1,   106,   116,   329,   345,   330,    -1,   106,   128,   329,
     345,   330,    -1,    -1,   387,    18,   154,    -1,   387,   154,
      -1,   387,    78,     4,    -1,   387,    78,     5,    -1,   387,
      78,     7,    -1,   387,    78,     6,    -1,   387,    40,    -1,
     387,   222,   128,    -1,   387,   166,   128,    -1,   387,   128,
      -1,   387,    56,     4,    -1,   387,   222,   329,   345,   330,
      -1,   244,   495,    -1,   388,    -1,   250,     3,    -1,   250,
     207,     3,    -1,   389,    -1,    34,    68,     3,   492,    -1,
      34,   184,     3,   492,    -1,   390,    -1,    34,   395,   207,
       3,   391,    -1,    34,   395,   207,     3,   394,    -1,   392,
      -1,   391,   327,   392,    -1,    32,   393,   385,    -1,    51,
     393,     3,     3,   493,   387,    -1,    88,   393,     3,    -1,
      88,   166,   128,    -1,    88,   116,   345,    -1,    88,   104,
     128,   345,    -1,    -1,    55,    -1,    -1,   175,     3,    -1,
     175,   217,     3,    -1,   161,    47,     3,    -1,    -1,   115,
      -1,   396,    -1,    61,   397,   116,     3,   398,   156,     3,
     329,   400,   330,    -1,    -1,   222,    -1,   106,    -1,   195,
      -1,    -1,   228,   399,    -1,   312,    -1,   313,    -1,   401,
      -1,   400,   327,   401,    -1,     3,   489,   339,    -1,   402,
      -1,    88,   116,     3,   156,     3,    -1,   403,    -1,    88,
      68,   404,     3,    -1,    88,   184,   404,     3,    -1,    -1,
     114,    97,    -1,   405,    -1,    88,   384,   207,   404,   406,
     407,    -1,     3,    -1,     3,   328,     3,    -1,   406,   327,
       3,    -1,   406,   327,     3,   328,     3,    -1,    -1,   179,
      -1,    49,    -1,   408,    -1,   175,   207,   409,    -1,    -1,
       3,   217,     3,    -1,     3,   328,     3,   217,     3,   328,
       3,    -1,   409,   327,     3,   217,     3,    -1,   409,   327,
       3,   328,     3,   217,     3,   328,     3,    -1,   410,    -1,
      82,     3,    -1,    81,     3,    -1,   411,    -1,   227,     3,
      -1,   412,    -1,   414,    -1,   417,    -1,   262,   263,    -1,
     260,   413,    -1,    -1,   311,    -1,   261,   413,   415,   416,
      -1,    -1,    36,   264,   265,    -1,    36,   265,    -1,    -1,
     264,   174,    -1,   174,    -1,   268,   413,   415,   416,    -1,
     418,    -1,   190,   266,    20,   485,    -1,   419,    -1,   420,
      -1,   421,    -1,   267,     3,    -1,   268,   413,   217,   267,
       3,    -1,   174,   267,     3,    -1,   422,    -1,   139,   270,
     423,    -1,   424,    -1,   423,   327,   424,    -1,     3,   353,
     425,    -1,   170,    -1,   170,   269,    -1,   144,   240,    -1,
     240,    -1,   426,    -1,   223,   270,    -1,   427,    -1,   190,
     428,   263,   271,   272,   429,    -1,    -1,   273,    -1,   274,
      -1,   170,   275,    -1,   170,   276,    -1,   277,   170,    -1,
     278,    -1,   430,    -1,    61,   249,   431,    -1,   432,    -1,
     431,   327,   432,    -1,     8,    -1,     8,   279,    47,     4,
      -1,     8,   279,    47,   280,     4,    -1,   433,    -1,    88,
     249,   434,    -1,     8,    -1,   434,   327,     8,    -1,   435,
      -1,   107,   436,   156,    26,   328,    26,   217,   431,   438,
      -1,   437,    -1,   436,   327,   437,    -1,    33,    -1,    33,
     281,    -1,   187,    -1,   225,    -1,   121,    -1,    80,    -1,
      61,    -1,    88,    -1,   116,    -1,    34,    -1,   107,   159,
      -1,   173,    -1,    -1,   239,   439,    -1,   107,   159,    -1,
     245,   485,    -1,   246,   485,    -1,   247,   485,    -1,   248,
     485,    -1,   440,    -1,   107,   436,   156,     3,   328,    26,
     217,   431,   438,    -1,   441,    -1,   107,   442,   156,     3,
     328,     3,   217,   431,   438,    -1,   443,    -1,   442,   327,
     443,    -1,   187,   371,    -1,   225,   371,    -1,   121,   371,
      -1,    80,    -1,    61,    -1,    88,    -1,   116,    -1,    34,
      -1,   107,   159,    -1,   444,    -1,   181,   436,   156,    26,
     328,    26,   105,   434,    -1,   445,    -1,   181,   436,   156,
       3,   328,    26,   105,   434,   438,    -1,   446,    -1,   181,
     442,   156,     3,   328,     3,   105,   434,   438,    -1,   447,
      -1,   175,   249,   448,    -1,     8,   217,     8,    -1,   448,
     327,     8,   217,     8,    -1,   449,    -1,   190,   280,   450,
      20,   280,   329,     4,   330,    -1,    -1,   102,     8,    -1,
     451,    -1,    35,   452,   207,   406,    -1,    -1,   269,    -1,
     153,    -1,   453,    -1,   282,   207,   406,   217,     4,    -1,
     454,    -1,    53,   207,   406,   455,    -1,    -1,   169,    -1,
     251,    -1,   252,    -1,   253,    -1,   254,    -1,   456,    -1,
     283,   207,   406,   457,    -1,    -1,   169,    -1,   253,    -1,
     458,    -1,   158,   452,   207,   406,    -1,   459,    -1,   284,
     452,   207,   406,   460,    -1,    -1,   460,   169,    -1,   460,
     253,    -1,   460,   285,    -1,   461,    -1,   286,   207,   406,
     105,     4,    -1,   462,    -1,   191,   287,   190,   465,    -1,
     191,   288,   465,    -1,   191,   463,   289,   105,     3,   464,
     465,    -1,   191,    61,    68,     3,    -1,   191,    61,   184,
       3,    -1,   191,    61,   207,     3,    -1,   191,    68,     3,
      -1,   191,   184,     3,    -1,   191,   290,     3,   291,    -1,
     191,   290,     3,   292,    -1,   191,   293,   294,    -1,   191,
     295,   343,    -1,   191,   485,   295,    -1,   191,   296,   102,
       8,    -1,   191,   116,   105,     3,   464,    -1,   191,   297,
     292,    -1,   191,   467,   291,    -1,   191,   315,   270,   464,
     465,    -1,   191,   281,    -1,   191,   463,   298,    -1,   191,
     428,   292,   465,    -1,   191,   207,   292,   464,   465,    -1,
     191,   463,   270,   464,   465,    -1,   191,   299,   464,   466,
      -1,   191,   428,   300,   465,    -1,   191,   301,   343,    -1,
     191,   485,   301,    -1,    -1,   316,    -1,    -1,   105,     3,
      -1,    -1,    14,     4,    -1,    -1,    14,   485,    -1,    -1,
     314,    -1,   468,    -1,   258,   116,   469,    16,     3,    -1,
     470,    -1,   469,   327,   470,    -1,     3,    -1,     3,   471,
     361,    -1,    -1,   116,    -1,   128,    -1,   472,    -1,   302,
     452,   473,    -1,   474,    -1,   473,   327,   474,    -1,   303,
      -1,   304,    -1,   291,    -1,   281,    -1,   257,   258,    -1,
     292,    -1,   207,     3,    -1,   270,   406,    -1,   270,   239,
     170,   139,    -1,   305,    -1,   475,    -1,   130,   476,   485,
      -1,    -1,   306,    -1,   257,    -1,   477,    -1,   136,   116,
     125,   258,   469,    -1,     3,   478,   479,    -1,   471,   329,
     361,   330,    -1,    -1,   115,   255,    -1,   480,    -1,   307,
     481,    -1,   482,    -1,   481,   327,   482,    -1,   256,    -1,
     257,   258,    -1,   259,    -1,   483,    -1,   308,     3,   105,
     334,    -1,   310,     3,   484,    -1,   309,   308,     3,    -1,
      88,   308,     3,    -1,    -1,   228,   434,    -1,   331,    -1,
     486,    -1,   190,   487,    -1,   488,    -1,   487,   327,   488,
      -1,     8,    20,   485,    -1,     8,     9,   485,    -1,    -1,
     329,     5,   330,    -1,   329,     5,   327,     5,   330,    -1,
      -1,    43,    -1,    -1,   491,   224,    -1,   491,   243,    -1,
      -1,   492,    52,   190,     3,    -1,   492,    54,     3,    -1,
      44,   489,    -1,   215,   489,   491,    -1,   192,   489,   491,
      -1,   147,   489,   491,    -1,   122,   489,   491,    -1,   123,
     489,   491,    -1,    42,   489,   491,    -1,   172,   489,   491,
      -1,    87,   489,   491,    -1,   101,   489,   491,    -1,    76,
     489,   491,    -1,    70,    -1,   212,    -1,   213,    -1,    71,
      -1,   241,    -1,    52,   489,   492,    -1,   234,   329,     5,
     330,   492,    -1,    43,   489,    -1,   233,   329,     5,   330,
      -1,   214,    -1,    45,    -1,   146,    -1,   141,    -1,   216,
     490,   492,    -1,   209,   490,   492,    -1,   148,   490,   492,
      -1,   142,   490,   492,    -1,    95,   329,   494,   330,   492,
      -1,   190,   329,   494,   330,   492,    -1,     4,    -1,   494,
     327,     4,    -1,     3,    -1,     8,    -1,     3,   328,     3,
      -1,     4,    -1,     5,    -1,     7,    -1,     6,    -1,   485,
      24,   485,    -1,   485,    25,   485,    -1,   485,    26,   485,
      -1,   485,    27,   485,    -1,   485,    28,   485,    -1,   485,
      29,   485,    -1,    25,   485,    -1,   485,    12,   485,    -1,
     485,    10,   485,    -1,   485,    11,   485,    -1,   485,    20,
     485,    -1,   485,    20,   329,   335,   330,    -1,   485,    20,
      37,   329,   335,   330,    -1,   485,    20,   193,   329,   335,
     330,    -1,   485,    20,    33,   329,   335,   330,    -1,   485,
      21,   485,    -1,   485,    22,   485,    -1,   485,    30,   485,
      -1,   485,    23,   485,    -1,    18,   485,    -1,    17,   485,
      -1,     8,     9,   485,    -1,   329,   485,   330,    -1,   485,
      15,   154,    -1,   485,    15,    18,   154,    -1,   485,    15,
       6,    -1,   485,    15,    18,     6,    -1,   485,    19,   485,
      36,   485,    -1,   485,    -1,   485,   327,   495,    -1,    -1,
     495,    -1,   485,    16,   329,   495,   330,    -1,   485,    18,
      16,   329,   495,   330,    -1,   485,    16,   329,   335,   330,
      -1,   485,    18,    16,   329,   335,   330,    -1,    97,   329,
     335,   330,    -1,    18,    97,   329,   335,   330,    -1,     3,
     329,   496,   330,    -1,   321,   329,    26,   330,    -1,   321,
     329,   485,   330,    -1,   322,   329,   485,   330,    -1,   323,
     329,   485,   330,    -1,   324,   329,   485,   330,    -1,   325,
     329,   485,   330,    -1,   317,   329,   495,   330,    -1,   317,
     329,   485,   105,   485,   330,    -1,   317,   329,   485,   105,
     485,   102,   485,   330,    -1,   318,   329,   495,   330,    -1,
     318,   329,   497,   485,   105,   495,   330,    -1,   131,    -1,
     218,    -1,    46,    -1,   319,   329,   485,   327,   498,   330,
      -1,   320,   329,   485,   327,   498,   330,    -1,   124,   485,
      72,    -1,   124,   485,    73,    -1,   124,   485,    74,    -1,
     124,   485,    75,    -1,   124,   485,   242,    -1,   124,   485,
     241,    -1,   124,   485,   111,    -1,   124,   485,   112,    -1,
     124,   485,   113,    -1,    50,   485,   499,    94,    -1,    50,
     485,   499,    91,   485,    94,    -1,    50,   499,    94,    -1,
      50,   499,    91,   485,    94,    -1,   236,   485,   211,   485,
      -1,   499,   236,   485,   211,   485,    -1,   485,    14,   485,
      -1,   485,    18,    14,   485,    -1,   485,    13,   485,    -1,
     485,    18,    13,   485,    -1,    65,    -1,    63,    -1,    64,
      -1,    43,   485,    -1
};

/* YYRLINE[YYN] -- source line where rule number YYN was defined.  */
static const unsigned short int yyrline[] =
{
       0,   422,   422,   423,   428,   432,   437,   445,   446,   448,
     449,   456,   461,   468,   469,   470,   473,   474,   477,   478,
     485,   486,   493,   494,   499,   509,   510,   513,   514,   517,
     518,   519,   520,   521,   522,   523,   524,   525,   528,   529,
     530,   533,   536,   537,   540,   541,   545,   550,   554,   559,
     562,   563,   566,   567,   568,   583,   587,   591,   596,   600,
     606,   607,   608,   611,   612,   615,   616,   619,   620,   621,
     624,   625,   632,   636,   656,   657,   660,   665,   668,   673,
     674,   675,   676,   679,   684,   685,   688,   689,   692,   698,
     701,   707,   708,   711,   712,   713,   714,   715,   718,   718,
     721,   722,   725,   726,   729,   730,   731,   732,   735,   741,
     747,   750,   753,   756,   762,   765,   771,   777,   783,   786,
     793,   794,   795,   799,   809,   820,   822,   831,   835,   837,
     841,   842,   847,   850,   854,   858,   863,   867,   872,   878,
     882,   883,   884,   887,   888,   891,   892,   896,   897,   898,
     899,   900,   901,   904,   905,   906,   907,   908,   909,   910,
     911,   912,   913,   914,   915,   916,   925,   929,   932,   933,
     937,   941,   942,   946,   951,   952,   956,   957,   961,   962,
     963,   964,   965,   966,   969,   970,   973,   974,   975,   976,
     979,   980,   984,   988,   991,   992,   993,   994,   997,   998,
    1001,  1002,  1005,  1006,  1009,  1013,  1016,  1020,  1024,  1025,
    1028,  1029,  1033,  1036,  1039,  1040,  1041,  1042,  1045,  1046,
    1047,  1052,  1055,  1058,  1059,  1060,  1061,  1062,  1066,  1069,
    1070,  1074,  1077,  1081,  1082,  1083,  1086,  1087,  1090,  1091,
    1094,  1097,  1098,  1099,  1102,  1103,  1104,  1107,  1111,  1114,
    1118,  1119,  1120,  1124,  1127,  1130,  1134,  1137,  1140,  1141,
    1144,  1147,  1148,  1149,  1150,  1154,  1157,  1161,  1165,  1168,
    1169,  1170,  1174,  1175,  1176,  1177,  1181,  1184,  1187,  1188,
    1191,  1192,  1193,  1197,  1200,  1203,  1204,  1208,  1211,  1214,
    1215,  1219,  1220,  1221,  1222,  1223,  1224,  1225,  1226,  1227,
    1228,  1229,  1230,  1233,  1234,  1238,  1239,  1240,  1241,  1242,
    1246,  1249,  1253,  1256,  1259,  1260,  1264,  1265,  1266,  1267,
    1268,  1269,  1270,  1271,  1272,  1282,  1285,  1288,  1291,  1294,
    1297,  1301,  1304,  1308,  1309,  1313,  1316,  1319,  1320,  1324,
    1327,  1330,  1331,  1332,  1336,  1339,  1343,  1346,  1349,  1350,
    1351,  1352,  1353,  1354,  1358,  1361,  1364,  1365,  1366,  1370,
    1373,  1377,  1380,  1383,  1384,  1385,  1386,  1390,  1393,  1397,
    1400,  1401,  1402,  1403,  1404,  1405,  1406,  1407,  1408,  1409,
    1410,  1411,  1412,  1413,  1414,  1415,  1416,  1417,  1418,  1419,
    1420,  1421,  1422,  1423,  1424,  1425,  1426,  1429,  1430,  1433,
    1434,  1437,  1438,  1441,  1442,  1445,  1446,  1450,  1453,  1456,
    1457,  1460,  1461,  1464,  1465,  1466,  1470,  1473,  1476,  1477,
    1480,  1481,  1482,  1483,  1484,  1485,  1486,  1487,  1488,  1489,
    1493,  1496,  1499,  1500,  1501,  1505,  1508,  1511,  1514,  1517,
    1518,  1522,  1525,  1528,  1529,  1532,  1533,  1534,  1538,  1541,
    1542,  1543,  1544,  1547,  1548,  1551,  1559,  1562,  1564,  1564,
    1567,  1571,  1579,  1580,  1581,  1584,  1585,  1588,  1589,  1590,
    1593,  1594,  1596,  1601,  1602,  1603,  1604,  1605,  1606,  1607,
    1608,  1609,  1610,  1611,  1612,  1613,  1614,  1615,  1616,  1617,
    1618,  1620,  1621,  1623,  1624,  1625,  1626,  1627,  1628,  1629,
    1630,  1631,  1632,  1635,  1636,  1642,  1643,  1644,  1645,  1646,
    1647,  1648,  1651,  1652,  1653,  1654,  1655,  1656,  1657,  1658,
    1659,  1660,  1661,  1662,  1663,  1664,  1665,  1666,  1667,  1668,
    1669,  1670,  1671,  1672,  1673,  1676,  1677,  1678,  1679,  1682,
    1686,  1687,  1690,  1691,  1694,  1695,  1696,  1697,  1698,  1699,
    1702,  1706,  1707,  1709,  1710,  1711,  1712,  1717,  1718,  1719,
    1720,  1721,  1724,  1725,  1726,  1729,  1730,  1733,  1734,  1735,
    1736,  1737,  1738,  1739,  1740,  1741,  1744,  1745,  1746,  1747,
    1750,  1751,  1754,  1755,  1758,  1759,  1762,  1763,  1764,  1767
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
  "OPTION", "OPTIONALLY", "ORDER", "OUT", "OUTER", "OUTFILE", "PRECISION",
  "PRIMARY", "PROCEDURE", "PURGE", "QUICK", "READ", "READS", "REAL",
  "REFERENCES", "RELEASE", "RENAME", "REPEAT", "REPLACE", "REQUIRE",
  "RESTRICT", "RETURN", "REVOKE", "RIGHT", "ROLLUP", "SCHEMA", "SCHEMAS",
  "SECOND_MICROSECOND", "SELECT", "SENSITIVE", "SEPARATOR", "SET", "SHOW",
  "SMALLINT", "SOME", "SONAME", "SPATIAL", "SPECIFIC", "SQL",
  "SQLEXCEPTION", "SQLSTATE", "SQLWARNING", "SQL_BIG_RESULT",
  "SQL_CALC_FOUND_ROWS", "SQL_SMALL_RESULT", "SSL", "STARTING",
  "STRAIGHT_JOIN", "TABLE", "TEMPORARY", "TEXT", "TERMINATED", "THEN",
  "TIME", "TIMESTAMP", "TINYBLOB", "TINYINT", "TINYTEXT", "TO", "TRAILING",
  "TRIGGER", "UNDO", "UNION", "UNIQUE", "UNLOCK", "UNSIGNED", "UPDATE",
  "USAGE", "USE", "USING", "UTC_DATE", "UTC_TIME", "UTC_TIMESTAMP",
  "VALUES", "VARBINARY", "VARCHAR", "VARYING", "WHEN", "WHERE", "WHILE",
  "WITH", "WRITE", "YEAR", "YEAR_MONTH", "ZEROFILL", "DO",
  "MAX_QUERIES_PER_HOUR", "MAX_UPDATES_PER_HOUR",
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
  "create_definition", "column_atts", "truncate_stmt",
  "alter_database_stmt", "alter_table_stmt", "alter_definition",
  "alter_duplicate_definition", "opt_col", "alter_noduplicate_definition",
  "opt_ignore", "create_index_stmt", "index_att", "opt_using_type",
  "index_type", "index_col_list", "index_col_name", "drop_index_stmt",
  "drop_database_stmt", "opt_if_exists", "drop_table_stmt", "table_list",
  "opt_rc", "rename_table_stmt", "rename_list", "describe_stmt",
  "use_stmt", "start_transaction_stmt", "opt_work", "commit_stmt",
  "opt_chain", "opt_release", "rollback_stmt", "set_autocommit_stmt",
  "savepoint_stmt", "rollback_to_savepoint_stmt", "release_savepoint",
  "lock_table_stmt", "lock_table_list", "lock_table", "opt_read_write",
  "unlock_tables", "set_transaction_stmt", "opt_trans_level", "trans_att",
  "create_user_stmt", "create_user_list", "create_user", "drop_user_stmt",
  "user_list", "grant_all_stmt", "priv_database_list",
  "priv_database_type", "opt_with_opt", "with_opt", "grant_database_stmt",
  "grant_table_stmt", "priv_table_list", "priv_table_type",
  "revoke_all_stmt", "revoke_database_stmt", "revoke_table_stmt",
  "rename_user_stmt", "utou_list", "set_password_stmt", "opt_for_user",
  "analyze_table_stmt", "analyze_opt", "backup_table_stmt",
  "check_table_stmt", "check_opts", "checksum_table_stmt", "checksum_opt",
  "optimize_table_stmt", "repair_table_stmt", "repair_opts",
  "restore_table_stmt", "show_stmt", "opt_full", "opt_from",
  "opt_like_string", "opt_like_expr", "opt_bdb", "cache_index_stmt",
  "table_index_list", "table_index", "opt_index_key", "flush_stmt",
  "flush_opt_list", "flush_opt", "kill_stmt", "opt_connection_query",
  "load_index_stmt", "opt_index_key_list", "opt_ignore_leaves",
  "reset_stmt", "reset_opt_list", "reset_opt", "prepare_stmt",
  "opt_use_name", "expr", "set_stmt", "set_list", "set_expr", "opt_length",
  "opt_binary", "opt_uz", "opt_csc", "data_type", "enum_list", "expr_list",
  "opt_expr_list", "trim_ltb", "interval_exp", "case_list", 0
};
#endif

/* YYR1[YYN] -- Symbol number of symbol that rule YYN derives.  */
static const unsigned short int yyr1[] =
{
       0,   332,   333,   333,   334,   335,   335,   336,   336,   337,
     337,   338,   338,   339,   339,   339,   340,   340,   341,   341,
     342,   342,   343,   343,   343,   344,   344,   345,   345,   346,
     346,   346,   346,   346,   346,   346,   346,   346,   347,   347,
     347,   348,   349,   349,   350,   350,   351,   351,   351,   351,
     352,   352,   353,   353,   353,   354,   354,   354,   354,   354,
     355,   355,   355,   356,   356,   357,   357,   358,   358,   358,
     359,   359,   360,   360,   361,   361,   362,   334,   363,   364,
     364,   364,   364,   363,   365,   365,   366,   366,   363,   334,
     367,   368,   368,   369,   369,   369,   369,   369,   370,   370,
     371,   371,   372,   372,   373,   373,   373,   373,   367,   367,
     374,   374,   374,   374,   334,   375,   375,   375,   334,   376,
     377,   377,   377,   378,   378,   378,   378,   334,   379,   379,
     380,   380,   334,   381,   381,   381,   381,   381,   381,   382,
     383,   383,   383,   384,   384,   385,   385,   386,   386,   386,
     386,   386,   386,   387,   387,   387,   387,   387,   387,   387,
     387,   387,   387,   387,   387,   387,   334,   334,   388,   388,
     334,   389,   389,   334,   390,   390,   391,   391,   392,   392,
     392,   392,   392,   392,   393,   393,   394,   394,   394,   394,
     395,   395,   334,   396,   397,   397,   397,   397,   398,   398,
     399,   399,   400,   400,   401,   334,   402,   334,   403,   403,
     404,   404,   334,   405,   406,   406,   406,   406,   407,   407,
     407,   334,   408,   409,   409,   409,   409,   409,   334,   410,
     410,   334,   411,   334,   334,   334,   412,   412,   413,   413,
     414,   415,   415,   415,   416,   416,   416,   417,   334,   418,
     334,   334,   334,   419,   420,   421,   334,   422,   423,   423,
     424,   425,   425,   425,   425,   334,   426,   334,   427,   428,
     428,   428,   429,   429,   429,   429,   334,   430,   431,   431,
     432,   432,   432,   334,   433,   434,   434,   334,   435,   436,
     436,   437,   437,   437,   437,   437,   437,   437,   437,   437,
     437,   437,   437,   438,   438,   439,   439,   439,   439,   439,
     334,   440,   334,   441,   442,   442,   443,   443,   443,   443,
     443,   443,   443,   443,   443,   334,   444,   334,   445,   334,
     446,   334,   447,   448,   448,   334,   449,   450,   450,   334,
     451,   452,   452,   452,   334,   453,   334,   454,   455,   455,
     455,   455,   455,   455,   334,   456,   457,   457,   457,   334,
     458,   334,   459,   460,   460,   460,   460,   334,   461,   334,
     462,   462,   462,   462,   462,   462,   462,   462,   462,   462,
     462,   462,   462,   462,   462,   462,   462,   462,   462,   462,
     462,   462,   462,   462,   462,   462,   462,   463,   463,   464,
     464,   465,   465,   466,   466,   467,   467,   334,   468,   469,
     469,   470,   470,   471,   471,   471,   334,   472,   473,   473,
     474,   474,   474,   474,   474,   474,   474,   474,   474,   474,
     334,   475,   476,   476,   476,   334,   477,   469,   478,   479,
     479,   334,   480,   481,   481,   482,   482,   482,   334,   483,
     483,   483,   483,   484,   484,   485,   334,   486,   487,   487,
     488,   488,   489,   489,   489,   490,   490,   491,   491,   491,
     492,   492,   492,   493,   493,   493,   493,   493,   493,   493,
     493,   493,   493,   493,   493,   493,   493,   493,   493,   493,
     493,   493,   493,   493,   493,   493,   493,   493,   493,   493,
     493,   493,   493,   494,   494,   485,   485,   485,   485,   485,
     485,   485,   485,   485,   485,   485,   485,   485,   485,   485,
     485,   485,   485,   485,   485,   485,   485,   485,   485,   485,
     485,   485,   485,   485,   485,   485,   485,   485,   485,   485,
     495,   495,   496,   496,   485,   485,   485,   485,   485,   485,
     485,   485,   485,   485,   485,   485,   485,   485,   485,   485,
     485,   485,   497,   497,   497,   485,   485,   498,   498,   498,
     498,   498,   498,   498,   498,   498,   485,   485,   485,   485,
     499,   499,   485,   485,   485,   485,   485,   485,   485,   485
};

/* YYR2[YYN] -- Number of symbols composing right hand side of rule YYN.  */
static const unsigned char yyr2[] =
{
       0,     2,     2,     3,     1,     3,    11,     0,     2,     0,
       4,     2,     4,     0,     1,     1,     0,     2,     0,     2,
       0,     3,     0,     2,     4,     0,     2,     1,     3,     0,
       2,     2,     2,     2,     2,     2,     2,     2,     1,     3,
       1,     2,     1,     3,     1,     1,     2,     4,     3,     3,
       0,     1,     2,     1,     0,     5,     3,     5,     6,     5,
       0,     1,     1,     0,     1,     1,     1,     2,     2,     0,
       0,     1,     2,     4,     1,     3,     3,     1,     7,     2,
       2,     2,     0,     6,     2,     4,     0,     2,     7,     1,
       8,     0,     5,     0,     2,     2,     2,     2,     1,     0,
       0,     3,     3,     5,     1,     1,     3,     3,     7,     7,
       3,     3,     5,     5,     1,     8,     7,     7,     1,     8,
       0,     2,     2,     3,     5,     5,     7,     1,     4,     4,
       0,     3,     1,     8,    10,     9,     6,    11,     8,     3,
       0,     1,     1,     0,     1,     1,     3,     3,     5,     4,
       4,     5,     5,     0,     3,     2,     3,     3,     3,     3,
       2,     3,     3,     2,     3,     5,     2,     1,     2,     3,
       1,     4,     4,     1,     5,     5,     1,     3,     3,     6,
       3,     3,     3,     4,     0,     1,     0,     2,     3,     3,
       0,     1,     1,    10,     0,     1,     1,     1,     0,     2,
       1,     1,     1,     3,     3,     1,     5,     1,     4,     4,
       0,     2,     1,     6,     1,     3,     3,     5,     0,     1,
       1,     1,     3,     0,     3,     7,     5,     9,     1,     2,
       2,     1,     2,     1,     1,     1,     2,     2,     0,     1,
       4,     0,     3,     2,     0,     2,     1,     4,     1,     4,
       1,     1,     1,     2,     5,     3,     1,     3,     1,     3,
       3,     1,     2,     2,     1,     1,     2,     1,     6,     0,
       1,     1,     2,     2,     2,     1,     1,     3,     1,     3,
       1,     4,     5,     1,     3,     1,     3,     1,     9,     1,
       3,     1,     2,     1,     1,     1,     1,     1,     1,     1,
       1,     2,     1,     0,     2,     2,     2,     2,     2,     2,
       1,     9,     1,     9,     1,     3,     2,     2,     2,     1,
       1,     1,     1,     1,     2,     1,     8,     1,     9,     1,
       9,     1,     3,     3,     5,     1,     8,     0,     2,     1,
       4,     0,     1,     1,     1,     5,     1,     4,     0,     1,
       1,     1,     1,     1,     1,     4,     0,     1,     1,     1,
       4,     1,     5,     0,     2,     2,     2,     1,     5,     1,
       4,     3,     7,     4,     4,     4,     3,     3,     4,     4,
       3,     3,     3,     4,     5,     3,     3,     5,     2,     3,
       4,     5,     5,     4,     4,     3,     3,     0,     1,     0,
       2,     0,     2,     0,     2,     0,     1,     1,     5,     1,
       3,     1,     3,     0,     1,     1,     1,     3,     1,     3,
       1,     1,     1,     1,     2,     1,     2,     2,     4,     1,
       1,     3,     0,     1,     1,     1,     5,     3,     4,     0,
       2,     1,     2,     1,     3,     1,     2,     1,     1,     4,
       3,     3,     3,     0,     2,     1,     1,     2,     1,     3,
       3,     3,     0,     3,     5,     0,     1,     0,     2,     2,
       0,     4,     3,     2,     3,     3,     3,     3,     3,     3,
       3,     3,     3,     3,     1,     1,     1,     1,     1,     3,
       5,     2,     4,     1,     1,     1,     1,     3,     3,     3,
       3,     5,     5,     1,     3,     1,     1,     3,     1,     1,
       1,     1,     3,     3,     3,     3,     3,     3,     2,     3,
       3,     3,     3,     5,     6,     6,     6,     3,     3,     3,
       3,     2,     2,     3,     3,     3,     4,     3,     4,     5,
       1,     3,     0,     1,     5,     6,     5,     6,     4,     5,
       4,     4,     4,     4,     4,     4,     4,     4,     6,     8,
       4,     7,     1,     1,     1,     6,     6,     3,     3,     3,
       3,     3,     3,     3,     3,     3,     4,     6,     3,     5,
       4,     5,     3,     4,     3,     4,     1,     1,     1,     2
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
       0,     1,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     2,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0
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
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0
};

/* YYDEFACT[S] -- default rule to reduce with in state S when YYTABLE
   doesn't specify something else to do.  Zero means the default is an
   error.  */
static const unsigned short int yydefact[] =
{
       0,   190,   341,     0,   143,    82,     0,     0,   143,     0,
      93,   432,     0,     0,   341,     0,     0,    93,     0,    29,
     269,   397,     0,    93,     0,     0,     0,     0,   238,   238,
       0,     0,   238,     0,     0,   341,     0,   341,     0,     0,
       0,     0,     0,     0,     4,    77,    89,   114,   118,   127,
     132,   167,   170,   173,   192,   205,   207,   212,   221,   228,
     231,   233,   234,   235,   248,   250,   251,   252,   256,   265,
     267,   276,   283,   287,   310,   312,   325,   327,   329,   331,
     335,   339,   344,   346,   354,   359,   361,   367,   369,   407,
     416,   430,   435,   441,   448,   456,     0,   191,     0,     0,
     343,   342,     0,     0,   130,   196,   130,   197,   144,   195,
       0,     0,     0,     0,   230,   229,   210,     0,   210,     0,
       0,     0,   291,   300,   297,   296,   298,     0,   299,   100,
     302,   100,   100,     0,   289,     0,   314,    99,   434,   433,
       0,     0,     0,     0,     0,   223,     0,    99,     0,     0,
       0,     0,     0,   270,   271,   337,     0,   457,   458,   505,
     508,   509,   511,   510,   506,     0,     0,     0,     0,     0,
       0,   587,   588,   586,     0,     0,     0,     0,     0,   388,
       0,   401,     0,     0,    22,     0,     0,   399,    22,   406,
       0,   398,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   455,     0,     0,     0,     0,   266,     0,     0,
     232,   540,   166,   168,     0,     0,   239,   237,   241,   236,
     253,   241,     0,     0,     0,     0,     0,   445,     0,   447,
     442,   443,     0,     0,   453,     1,     0,     2,   470,   470,
       0,     0,   214,   348,     0,     0,     0,   280,   277,   278,
     130,     0,    86,     0,    81,    79,    80,     0,     0,     0,
       0,     0,   285,   284,   452,   210,   292,   301,     0,   318,
     316,   317,     0,     0,     0,     0,    95,    96,    97,    98,
      94,     0,   431,     0,    54,   257,   258,     0,   255,     0,
     222,     0,   332,     0,     0,     0,    40,    30,    31,    32,
      33,    36,    37,    35,    34,     5,    38,    54,     0,     0,
       0,     0,     0,     0,     0,     0,   542,     0,   532,     0,
     531,   518,   589,     0,     0,     0,     0,     0,     0,   376,
       0,     0,   377,   399,   401,     0,   371,     0,   380,     0,
     381,     0,   385,     0,   403,   395,   399,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   401,   401,   399,
       0,   389,   386,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   382,   396,    97,    94,    54,     0,     0,
      42,    44,    45,    50,     0,   169,   411,     0,   409,     0,
     244,     0,   244,     0,   356,     0,     0,     0,     0,     0,
     423,   422,   425,   420,   421,   429,   417,   418,   446,     0,
       0,   451,     0,   450,     3,   171,   172,   186,   340,     0,
     349,   350,   351,   352,   353,     0,   347,     0,   128,   129,
       0,     0,     0,   198,     0,    84,     7,     0,     0,     0,
     211,   208,     0,   209,     0,     0,    27,     0,     0,     0,
     300,   297,   296,   298,     0,   299,   295,   293,   294,   290,
       0,   323,   320,   319,   321,     0,   322,   100,   100,   100,
     315,   100,     0,    53,     0,     0,     0,   360,     0,     0,
       0,     0,     0,   100,     0,     0,     0,     0,     0,    41,
     461,   460,   249,   338,     0,     0,   459,   507,   543,     0,
     533,     0,     0,     0,     0,   578,     0,   373,   374,   375,
       0,   399,   401,   370,   402,   378,   379,    23,   383,   400,
       0,   393,   401,   540,     0,   564,   562,   563,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   534,   390,
     394,   401,     0,   520,   521,   519,   584,   582,   537,     0,
     535,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     522,   527,   528,   530,   512,   513,   514,   515,   516,   517,
     529,     0,    46,     0,     0,     0,     0,    62,    61,    65,
      69,    66,     0,     0,    63,    51,     0,   541,   414,   415,
       0,   439,     0,     0,     0,   243,   246,     0,   240,     0,
     247,     0,   357,   358,   355,   363,     0,   426,   424,     0,
     427,     0,   444,   449,   454,     0,     0,   184,   184,   184,
       0,     0,   174,   176,   175,   215,   216,   131,     0,   279,
     140,     0,     0,    87,     0,    20,     0,     7,    86,   206,
     286,   218,     0,   101,     0,     0,   301,     0,   324,     0,
       0,   436,    52,     0,   261,   264,   260,   259,   224,     0,
       0,   333,     0,     0,     0,     0,     0,     0,     7,    39,
       0,     0,   550,     0,     0,     0,   576,     0,     0,   548,
     384,   391,     0,   404,   387,     0,   557,   560,     0,     0,
       0,   551,   552,   553,   554,   555,   556,   392,   399,   538,
     536,     0,     0,   585,   583,     0,     0,     0,     0,     0,
       0,    54,    76,    49,     0,     7,    43,    63,    63,     0,
      56,     0,    64,     0,    48,    74,     0,   412,     0,   437,
     408,   411,   410,   242,   245,   254,   345,   362,   368,     0,
     419,     0,   472,   185,     0,     0,     0,     0,     0,     0,
       0,   187,     0,     0,     0,   281,     0,   141,   142,     0,
       0,   136,    50,   200,   201,   199,     0,     8,     0,    22,
       7,    83,    85,   220,   219,   213,    28,     0,     0,     0,
       0,    91,     0,    91,   263,   262,     0,     0,     0,     0,
      91,     0,    91,     0,     0,     0,     9,     0,     0,     0,
     275,   268,   548,   580,     0,   579,     0,    24,     0,     0,
       0,     0,     0,   401,   546,   544,     0,     0,   539,     0,
       0,     0,   523,    47,     0,     0,     0,    20,    67,    68,
       0,     0,    70,     0,     0,     0,   440,     0,   364,   365,
     366,   428,   471,     0,     0,     0,     0,     0,   178,   145,
       0,     0,   182,   181,   180,   189,   188,   177,   217,   282,
     140,     0,     0,     0,     0,    78,    88,     0,     0,     0,
       0,     0,     0,   108,     0,    91,   109,     0,   226,     0,
     334,   116,    91,   117,     0,     0,     0,     0,    18,     0,
     272,   273,   274,   577,   581,     0,   558,     0,     0,   565,
     566,   372,   547,   545,   526,   524,   525,   123,     0,     0,
      22,    59,    57,     0,     0,    55,    71,     0,   438,    75,
     462,   462,   462,   494,   462,   484,   487,   462,   462,     0,
     462,   462,   462,   496,   465,   495,   462,   465,   462,     0,
     462,   465,   485,   486,   493,   462,   465,     0,     0,   488,
     153,     0,     0,     0,     0,     0,     0,     0,   183,     0,
     138,   140,   139,     0,    21,    13,   303,   303,   303,   111,
     110,     0,     0,   105,     0,   104,     0,    90,     0,     0,
     115,   303,   326,   303,     0,     0,    20,   336,     0,   561,
     567,   568,   569,   570,   573,   574,   575,   572,   571,     0,
       0,     0,   119,    72,     0,    58,     0,   467,   491,   473,
     470,   467,   467,     0,   467,   467,   467,   466,   470,   467,
     470,   467,     0,   467,   470,   467,   470,     0,     0,   147,
       0,     0,     0,     0,     0,   146,   153,     0,   135,   462,
       0,   202,     0,    14,    15,    11,     0,   311,   288,   313,
       0,     0,     0,   102,     0,   225,     0,   328,   330,    16,
      19,    22,   559,   124,   125,     0,     0,     0,   479,   489,
     483,   481,   503,     0,   482,   477,   478,   500,   476,   499,
     480,     0,   475,   498,   474,   497,     0,     0,     0,   160,
       0,     0,   163,   155,     0,     0,     0,     0,   150,   149,
       0,   179,   140,    13,     0,   193,    13,     0,     0,     0,
       0,     0,   304,     0,   113,   112,   107,   106,     0,     0,
       0,    10,    25,     0,    73,     0,   463,   468,   469,     0,
     470,   470,   492,   470,   154,   164,   156,   157,   159,   158,
     162,   161,     0,   151,   152,   148,   137,   204,   203,    12,
     305,   306,   307,   308,   309,    92,   103,   227,    17,     0,
       6,   126,     0,   504,   501,   502,   490,     0,    26,   464,
     165
};

/* YYPDEFGOTO[NTERM-NUM].  */
static const short int yydefgoto[] =
{
      -1,    42,    43,    44,   645,   898,   974,  1055,  1131,   996,
     779,   340,  1170,   457,   150,   305,   306,   389,   390,   391,
     596,   485,   392,   593,   733,   594,   729,   925,   926,   737,
     393,    45,   113,   257,   445,    46,   883,   137,   281,   269,
     885,   984,   791,    47,    48,   209,   725,    49,   245,    50,
     771,   772,   111,   858,   859,  1039,    51,    52,    53,   632,
     633,   754,   634,    99,    54,   112,   642,   775,  1050,  1051,
      55,    56,   259,    57,   243,   785,    58,   290,    59,    60,
      61,   217,    62,   400,   608,    63,    64,    65,    66,    67,
      68,   285,   286,   666,    69,    70,   156,   811,    71,   248,
     249,    72,   263,    73,   133,   134,  1057,  1122,    74,    75,
     135,   136,    76,    77,    78,    79,   292,    80,   312,    81,
     102,    82,    83,   436,    84,   614,    85,    86,   747,    87,
      88,   204,   344,   336,   531,   205,    89,   397,   398,   600,
      90,   416,   417,    91,   140,    92,   601,   739,    93,   230,
     231,    94,   423,   211,    95,   157,   158,  1017,  1028,  1078,
     425,   960,  1083,   212,   509,   539,   821,   325
};

/* YYPACT[STATE-NUM] -- Index in YYTABLE of the portion describing
   STATE-NUM.  */
#define YYPACT_NINF -844
static const short int yypact[] =
{
    2122,   323,   -32,   -14,   356,  -844,   117,   206,   204,   422,
    -844,   112,   255,   110,   -32,   137,   203,  -844,   422,  -844,
      12,   423,   166,    11,   457,  1128,    25,   348,   192,   192,
     245,   512,   192,   312,   314,   -32,   320,   -32,    56,   543,
     242,   562,  1964,   261,  -844,  -844,  -844,  -844,  -844,  -844,
    -844,  -844,  -844,  -844,  -844,  -844,  -844,  -844,  -844,  -844,
    -844,  -844,  -844,  -844,  -844,  -844,  -844,  -844,  -844,  -844,
    -844,  -844,  -844,  -844,  -844,  -844,  -844,  -844,  -844,  -844,
    -844,  -844,  -844,  -844,  -844,  -844,  -844,  -844,  -844,  -844,
    -844,  -844,  -844,  -844,  -844,  -844,   593,  -844,   595,   397,
    -844,  -844,   401,   624,   475,  -844,   475,  -844,  -844,  -844,
     627,   468,   567,   281,  -844,  -844,   573,   686,   573,   685,
     700,   500,   427,  -102,   -53,   -52,   -51,   558,   -48,  -121,
    -844,  -113,  -106,   -45,  -844,   -44,  -844,   507,  -844,  -844,
    1128,   596,   748,   524,   752,   756,   765,   507,   -30,   -26,
     527,    98,   754,  -844,  -844,   675,   515,   452,  -844,   -12,
    -844,  -844,  -844,  -844,   771,  1128,  1326,  1128,  1128,   758,
     225,  -844,  -844,  -844,   778,   453,   688,   793,   505,  -844,
     612,   789,   801,   511,   672,   705,   517,   706,   672,  -844,
     542,  -844,   485,   486,   487,   488,   489,   490,   513,   514,
     525,  1128,  -844,   189,   -87,   566,  1956,  -844,   382,    18,
    -844,  1844,  -844,  -844,   857,   858,  -844,  -844,   826,  -844,
    -844,     3,   624,   624,   657,   624,  1415,  -844,   607,  -844,
     539,  -844,   762,   874,   650,  -844,   553,  -844,  -844,  -844,
     878,   624,   555,   130,   885,   903,   907,   632,   587,  -844,
     475,   913,   589,   917,  -844,  -844,  -844,   -57,   827,   922,
     772,   927,  -844,   605,  -844,   573,  -844,   -22,   931,  -844,
    -844,  -844,   184,   585,   932,  1163,  -844,  -844,  -844,  -844,
    -844,   946,  2759,   692,   370,   634,  -844,   624,  -844,  -101,
     636,   734,   638,   959,   491,   964,  -844,  -844,  -844,  -844,
    -844,  -844,  -844,  -844,  -844,   -56,  -844,  2698,  1128,  1128,
    1128,   960,   949,   699,   963,   969,  1128,  1128,  1274,   646,
    1274,  -844,  -844,  1128,  2008,    83,   973,   974,   975,  -844,
     792,   978,  -844,   706,   789,   979,  -844,   353,  -844,  1128,
    -844,   976,  -844,   982,   968,  -844,   706,  1128,   551,  1128,
    1128,   883,  1128,  1128,  1128,  1128,    46,   789,   789,   706,
     881,  -844,  -844,  1128,  1128,  1128,  1128,  1128,   183,   658,
     586,  1128,   635,  1128,  1128,  1128,  1128,  1128,  1128,  1128,
    1128,  1128,  1128,  -844,  -844,    19,    35,    39,    10,  -145,
     316,  -844,  -844,   952,  1128,  -844,     9,    62,  -844,   385,
     -42,   724,   -42,   -93,   -46,   624,   -54,   989,   735,    15,
    -844,  -844,  -844,  -844,  -844,  -844,   668,  -844,  -844,    56,
    2122,  -844,   685,  -844,  -844,    79,    79,   388,   669,  1000,
    -844,  -844,  -844,  -844,  -844,  1001,  -844,   908,  -844,  -844,
     961,   627,  1003,   779,   983,  -844,   209,   -89,    18,  1007,
    -844,  -844,  1010,  -844,  1006,   624,  -844,   198,   687,   689,
    -844,  -844,  -844,  -844,   859,  -844,  -844,  -844,  -844,  -844,
     704,  -844,  -844,  -844,  -844,   875,  -844,   707,   707,   707,
    -844,  -138,   858,  -844,  1013,   235,   748,   669,  1030,  1032,
    1035,  1031,  1034,   -84,   717,   725,   726,    18,  1128,  -844,
    2759,  2759,  2759,  -844,   775,   780,  -844,  -844,  -844,   732,
    2759,   792,  2054,    85,  1128,  -844,  1128,  -844,  -844,  -844,
     733,   706,   789,  -844,  -844,  -844,  -844,  1882,  -844,  -844,
    1128,  -844,   789,  1823,   736,  -844,  -844,  -844,   737,  1128,
    1911,  1932,   739,   139,   237,  1382,  1451,  1472,  -844,  -844,
    -844,   789,  1053,  1741,  1562,   811,  1790,  1790,  -844,    38,
    -844,   994,  1128,  1128,   741,  2732,   742,   743,   744,   994,
     999,   915,   869,   761,   554,   554,  1044,  1044,  1044,  1044,
    -844,  1061,  -844,   755,   258,  1062,    18,  -844,  -844,  -844,
     265,  -844,    18,   957,   925,  -844,  1087,  -844,  -844,  -844,
      37,   977,  1091,  1092,   831,  -844,  -844,   928,  -844,  1098,
    -844,  1099,  -844,  -844,  -844,   669,  1100,  -844,  -844,   935,
     782,  1415,  -844,  -844,   605,   920,  1108,  1058,  1058,   347,
    1067,    22,   795,  -844,  -844,  -844,   796,  -844,    20,  -844,
      -6,   308,   967,  -844,  1128,   965,    18,  -184,   589,  -844,
    -844,   -23,  1122,  -844,  1101,  1102,  -844,  1135,  -844,  1136,
     -47,   814,  -844,   902,   879,  -844,  -844,  -844,  -844,   926,
    -100,  -844,   930,  1136,    48,  1118,  1123,  1147,  -184,  -844,
     822,    33,  -844,   824,  1128,  1128,  -844,  2105,  2423,  -844,
    -844,  -844,  1128,  2759,  -844,  1128,  -844,  -844,  2551,  1028,
    1028,  -844,  -844,  -844,  -844,  -844,  -844,  -844,   706,  -844,
    -844,   828,   829,  1790,  1790,   994,  1128,   792,   792,   792,
     830,   370,  -844,  -844,    -3,  -123,   316,   925,   925,  1029,
    1005,    18,  -844,  1036,  -844,  -844,  1154,   837,   910,  -844,
    -844,   287,  -844,  -844,  -844,  -844,  -844,   205,  -844,  1027,
    -844,  1164,  -844,  -844,   305,  1165,  1041,   931,  1042,  1169,
    1170,  -844,  1171,   473,  1176,  -844,  1178,  -844,  -844,  1177,
     305,  -844,   952,  -844,  -844,  -844,  1180,  2759,  1137,   672,
    -184,  -844,  -844,  -844,  -844,  -844,  -844,   970,   971,   972,
    1166,   -20,   856,  1038,  -844,  -844,  1192,  1193,  1195,  1191,
     -20,   856,  1038,  1105,  1106,  1116,  1114,  1209,   386,  1056,
    -844,  -844,  1590,  2759,  2647,  -844,  1128,  2759,    71,  1128,
    1128,   893,   897,   789,  -844,  -844,   898,   901,  1357,   904,
     905,   906,  -844,  -844,  1128,  1229,  1230,   965,  -844,  -844,
      18,  1128,   -29,    18,   276,  1235,  -844,  1154,  -844,  -844,
    -844,  -844,  -844,  2412,   354,   911,   912,  1111,   929,  -844,
    1239,   931,   934,  -844,  -844,  -844,  -844,  -844,  -844,  -844,
       7,   283,   792,   923,  1128,  -844,  -844,   627,   627,   627,
    1043,  1096,  1241,  -844,  1112,   -18,  -844,   936,  -844,  1037,
    -844,  -844,   -18,  -844,   685,   685,   685,  1210,  1149,   933,
    -844,  -844,  -844,  -844,  2759,  1128,  -844,   938,  2304,  -844,
    -844,  -844,  -844,  -844,  -844,  -844,  -844,  2759,  1245,    -1,
     672,  -844,  2759,  1128,   937,  -844,  -844,   214,  -844,  -844,
     940,   940,   940,  -844,   940,  -844,  -844,   940,   940,   942,
     940,   940,   940,  -844,  1231,  -844,   940,  1231,   940,   943,
     940,  1231,  -844,  -844,  -844,   940,  1231,   944,   951,  -844,
    -844,   953,   954,   931,   931,   958,   305,  2412,   962,   305,
    -844,   -35,  -844,  1275,   981,  2668,  -192,  -192,  -192,  -844,
    2759,  1153,  1266,  -844,   298,  2759,   992,  -844,  1285,  1288,
    -844,   -98,   605,   -98,  1128,  1128,   965,  -844,  1493,  -844,
    -844,  -844,  -844,  -844,  -844,  -844,  -844,  -844,  -844,  1128,
    1128,  1307,  -844,  2759,   931,  -844,  1317,  -844,  -844,  -844,
    -844,  -844,  -844,  1320,  -844,  -844,  -844,  -844,  -844,  -844,
    -844,  -844,  1320,  -844,  -844,  -844,  -844,  1321,  1322,   357,
     931,   931,   299,   304,   931,  -844,  -844,   306,  -844,   940,
     327,  -844,  1128,  -844,  -844,  -844,    -5,  -844,  -844,  -844,
    1103,  1212,  1242,  -844,  1112,  -844,  1008,  -844,  -844,   -13,
    2759,   672,  -844,  2759,  2759,  1315,   328,   329,   -43,    79,
     -43,   -43,  -844,   339,   -43,   -43,   -43,    79,   -43,    79,
     -43,   340,   -43,    79,   -43,    79,  1011,  1012,  1183,  -844,
    1334,   568,  -844,  -844,  1217,   -99,   364,   375,  -844,  -844,
     426,   357,    -2,   378,  1275,  -844,  2668,  1181,  1128,  1128,
    1128,  1128,  -844,  1136,  -844,  2759,  -844,  2759,   440,  1343,
    1172,  -844,  1222,  1128,  -844,  1344,  -844,  -844,  -844,  1348,
    -844,  -844,  -844,  -844,  -844,  -844,  -844,  -844,  -844,  -844,
    -844,  -844,   931,  -844,  -844,  -844,  -844,  -844,  -844,  -844,
    -844,  2759,  2759,  2759,  2759,  1021,  -844,  -844,  -844,   931,
    -844,  2759,  1023,  -844,    79,    79,    79,   441,  1046,  -844,
    -844
};

/* YYPGOTO[NTERM-NUM].  */
static const short int yypgoto[] =
{
    -844,  -844,   -33,  -325,  -306,  -844,   360,  -404,  -844,  -844,
    -800,  -186,  -844,  -746,  -844,  -844,   860,  -278,   770,  -537,
     598,  -299,  -844,  -844,     8,  -844,  -844,  -844,   432,   621,
    -844,  -844,  -844,  1146,   723,  -844,  -386,   155,  1228,  -116,
     613,   349,  -669,  -844,  -844,  -844,  -844,  -844,   -60,  -844,
    -843,  -844,  1407,  -736,   450,   371,  -844,  -844,  -844,  -844,
     655,   121,  -844,  -844,  -844,  -844,  -844,  -844,  -844,   307,
    -844,  -844,   -77,  -844,   -10,  -844,  -844,  -844,  -844,  -844,
    -844,   728,  -844,  1198,  1018,  -844,  -844,  -844,  -844,  -844,
    -844,  -844,   939,  -844,  -844,  -844,  1401,  -844,  -844,  -152,
     985,  -844,  -416,  -844,  1406,  1155,  -477,  -844,  -844,  -844,
    1409,  1167,  -844,  -844,  -844,  -844,  -844,  -844,  -844,  -844,
     430,  -844,  -844,  -844,  -844,  -844,  -844,  -844,  -844,  -844,
    -844,  -844,  -323,  -327,  -844,  -844,  -844,   956,   836,   703,
    -844,  -844,   819,  -844,  -844,  -844,  -844,  -844,  -844,  -844,
    1039,  -844,  -844,   -21,  -844,  -844,  1140,   -19,  -385,  -345,
    -236,   493,   424,  -315,  -844,  -844,   768,  1131
};

/* YYTABLE[YYPACT[STATE-NUM]].  What to do in state STATE-NUM.  If
   positive, shift that token.  If negative, reduce the rule which
   number is the opposite.  If zero, do what YYDEFACT says.
   If YYTABLE_NINF, syntax error.  */
#define YYTABLE_NINF -549
static const short int yytable[] =
{
     206,   508,   345,   426,   800,   520,   624,   523,   499,   236,
     522,   862,  -413,   387,  -120,   270,   271,   834,   242,  1010,
     151,   387,  -122,   532,   765,   761,   783,   970,   213,  1151,
     549,   550,   534,   538,   871,  -100,   551,   920,  -121,   399,
     735,   261,   483,  -100,   709,   585,   246,  1056,   448,   497,
    -100,   616,   659,   644,  -300,   730,   363,   364,   365,   366,
     367,   368,   369,   583,   370,   371,   372,   373,   374,   375,
     376,   377,   378,   379,   380,   381,   382,   484,   602,   597,
     767,   363,   364,   365,   366,   367,   368,   369,   582,   370,
     371,   372,   373,   374,   375,   376,   377,   378,   379,   380,
     381,   382,  1117,  -297,  -296,  -298,   673,   308,  -299,   767,
     584,   272,   274,   767,   644,   968,   488,   797,   309,   282,
     114,   100,   767,   612,   611,   598,   294,   923,  1048,   307,
     295,   625,   606,   626,  -301,   441,   881,   599,   881,   646,
      19,  1056,   768,   586,   318,   320,   321,   322,   324,   363,
     364,   365,   366,   367,   368,   369,   784,   370,   371,   372,
     373,   374,   375,   376,   377,   378,   379,   380,   381,   382,
     647,   768,   147,   905,   514,   768,   685,   515,   208,   686,
     356,  1137,   586,   359,   768,   792,   683,   458,   455,   558,
     442,   268,   710,   103,   842,   691,  1071,    19,   690,   924,
    1138,   559,   360,   808,   836,   694,  -100,   613,   268,   115,
     459,   361,   403,   404,  -100,   406,   268,  1042,  1043,   678,
     401,  -100,   607,   268,   707,  -300,  1130,   489,   798,   454,
    1152,   428,   214,  1047,   435,    19,   711,   101,   449,   762,
    1118,  1119,  1120,  1121,   720,   268,   712,   363,   364,   365,
     366,   367,   368,   369,   619,   370,   371,   372,   373,   374,
     375,   376,   377,   378,   379,   380,   381,   382,  1076,  1156,
     449,   498,   116,   435,  -297,  -296,  -298,   487,   152,  -299,
     801,   435,   273,   275,   252,   153,   154,   500,   501,   502,
    -413,  -133,   155,   326,  1106,  1107,   510,   273,  1110,   430,
     766,   275,   512,   921,   435,  -301,   927,   882,   853,   986,
     809,   810,   227,   228,  1052,   229,   315,   316,   527,   516,
     117,   516,   769,   770,  -134,   835,   533,  1011,   540,   541,
     543,   544,   545,   546,   547,   793,   969,   560,  -413,   388,
    -120,   781,   553,   554,   555,   556,   557,   388,  -122,   802,
     565,   570,   571,   572,   573,   574,   575,   576,   577,   578,
     579,   580,   270,   271,  -121,   660,   736,   581,   780,   138,
     923,   141,   806,   483,   848,  1098,   548,   674,   587,   663,
     142,   431,   432,   433,   434,   823,   253,   623,   118,   603,
     826,    96,   829,   830,   831,   615,   254,  1099,   727,   620,
     827,   906,   753,   598,   144,   664,  1177,   886,   484,   327,
     145,   854,   108,  1100,   891,   599,   893,  1053,   139,   837,
     627,   855,   833,  1178,   104,   255,   159,   160,   161,   162,
     163,   164,   328,   856,   588,  1101,   207,   -86,    97,   628,
     165,   166,   924,   -60,   143,   651,   644,   728,   167,   589,
     256,   756,   146,   119,  1165,   122,   123,   435,   849,  1054,
     210,   276,   105,   757,   215,   224,   168,   226,   590,   702,
     961,   857,  -194,   169,   876,   665,   629,   307,   991,   992,
     993,   357,   962,   124,   170,  1102,   171,   172,   173,   358,
     850,   174,   277,   687,   494,   688,   911,   385,   591,   987,
    1058,  1059,   125,   216,   907,   627,   990,    98,   219,   693,
     126,  1103,   120,   758,  1067,   220,  1068,   495,   698,   222,
     175,   223,   592,  1104,   628,   652,   386,   225,   653,   127,
     159,   160,   161,   162,   163,   164,   -86,   444,   128,   176,
     106,   713,   714,   129,   165,   166,   232,   972,   356,   630,
     233,   107,   167,   296,   159,   160,   161,   162,   163,   164,
     297,   629,  1030,   631,   108,   234,  1034,   703,   165,   166,
     168,  1036,  1146,  1147,  1148,  1149,   167,   169,   109,  1105,
     378,   379,   380,   381,   382,   586,   276,   237,   723,   244,
     171,   172,   173,   875,   168,   130,   238,   535,   239,   562,
     563,   169,   564,   845,   240,   110,   928,   177,   241,   131,
     966,   298,   299,   971,   171,   172,   173,   277,   122,   460,
     773,   774,   278,   777,   175,  1062,   652,   242,  1063,  1108,
     178,   652,   279,   966,  1109,   247,  1112,   300,   159,   160,
     161,   162,   163,   164,   525,   526,   461,   132,   175,   604,
     605,   280,   165,   166,  1114,   652,  1135,  1115,  1134,  1136,
     167,   900,   901,   813,   814,   462,  1139,  1139,   566,  1140,
    1141,   817,   567,   463,   818,   250,  1080,  1081,   168,  1084,
    1085,  1086,   536,   251,  1088,   169,  1090,   258,  1092,   260,
    1094,   652,   464,   262,  1153,   828,   153,   154,   171,   172,
     173,   465,   652,   264,   179,  1154,   466,   265,   266,  1157,
     180,   181,  1159,   182,  -405,  -269,   183,   267,   184,   185,
     186,   283,   187,  -269,   188,   976,   977,   978,   301,   302,
     303,   287,   175,   304,  1012,   838,   839,   189,   190,   191,
     192,   193,   194,   195,   196,   197,   198,   199,   200,   755,
     759,   284,   201,   652,   202,   288,  1155,   218,   130,   289,
     221,   159,   160,   161,   162,   163,   164,  1062,   652,   537,
    1166,  1180,   467,   291,   310,   165,   166,   311,   313,   314,
     317,   329,   330,   167,  1079,   376,   377,   378,   379,   380,
     381,   382,  1087,   331,  1089,   904,   332,   333,  1093,   908,
    1095,   168,   334,   335,   337,   338,   339,   341,   169,   342,
     468,   343,   346,   917,   347,   348,   349,   350,   351,   352,
     922,   171,   172,   173,   366,   367,   368,   369,   568,   370,
     371,   372,   373,   374,   375,   376,   377,   378,   379,   380,
     381,   382,   353,   354,   192,   193,   194,   195,   196,   197,
     198,   199,   200,   975,   355,   175,   201,   362,   202,   980,
     395,   396,   399,   985,   405,   418,   419,   420,   192,   193,
     194,   195,   196,   197,   198,   199,   200,   421,   422,   424,
     201,   427,   202,   429,   998,  1132,   159,   160,   161,   162,
     163,   164,   375,   376,   377,   378,   379,   380,   381,   382,
     165,   166,  1013,   437,  1174,  1175,   438,  1176,   167,   542,
     439,   440,  1018,  1019,   441,  1020,   443,   444,  1021,  1022,
     446,  1024,  1025,  1026,   450,   451,   168,  1029,   452,  1031,
     453,  1033,   454,   169,   456,   470,  1035,   374,   375,   376,
     377,   378,   379,   380,   381,   382,   171,   172,   173,   481,
     482,   491,   192,   193,   194,   195,   196,   197,   198,   199,
     200,   486,   493,   490,   569,   492,   202,   496,   503,   504,
     505,   151,   507,   975,  1070,   511,   517,   518,   519,    19,
     175,   521,   530,   524,   528,   529,   552,   561,  1073,  1074,
     595,   609,   617,   618,   323,   621,   435,   159,   160,   161,
     162,   163,   164,   635,   636,   637,   640,   641,   638,   643,
     648,   165,   166,   649,   650,   654,   662,   655,   656,   167,
     373,   374,   375,   376,   377,   378,   379,   380,   381,   382,
    1113,  1116,   657,   668,   658,   669,   268,   168,   670,   671,
    1125,  1127,   672,   985,   169,   675,   159,   160,   161,   162,
     163,   164,   681,   676,   677,   680,   708,   171,   172,   173,
     165,   166,   682,   689,   721,   724,   696,   697,   167,   701,
     715,   717,   718,   719,   382,   192,   193,   194,   195,   196,
     197,   198,   199,   200,   731,   722,   168,   201,   732,   202,
     734,   175,   738,   169,   740,   741,   743,  1161,  1162,  1163,
    1164,   745,   744,   746,   748,   749,   171,   172,   173,   435,
     751,   752,  1171,   753,   760,   159,   160,   161,   162,   163,
     164,   979,   763,   776,   764,   786,   778,   787,   788,   165,
     166,   159,   160,   161,   162,   163,   164,   167,   789,   790,
     175,   603,   794,   796,   803,   165,   166,   799,   795,   804,
     805,   807,   820,   167,   812,   168,   840,   735,   824,   825,
     832,   841,   169,   843,   845,   846,   851,   852,   860,   861,
     863,   168,   864,   865,   866,   171,   172,   173,   169,   868,
     870,    19,   869,   873,   874,   884,   880,   877,   878,   879,
     983,   171,   172,   173,   881,   887,   888,   471,   889,   890,
     192,   193,   194,   195,   196,   197,   198,   199,   200,   175,
     894,   895,   201,   899,   202,   159,   160,   161,   162,   163,
     164,   896,   897,   909,   472,   175,   902,   910,   912,   165,
     166,   913,   918,   919,   914,   915,   916,   167,   929,   965,
     963,   964,   967,   473,   982,   159,   160,   161,   162,   163,
     164,   474,   973,   981,   989,   168,   966,   994,   995,   165,
     166,   652,   169,   997,   988,  1009,  1014,   167,   999,  1016,
     475,  1023,  1032,  1037,  1027,   171,   172,   173,  1049,   476,
    1038,  1060,  1040,  1041,   477,   168,  1061,  1044,  1065,   652,
    1124,  1066,   169,   371,   372,   373,   374,   375,   376,   377,
     378,   379,   380,   381,   382,   171,   172,   173,  1052,   175,
    1075,   192,   193,   194,   195,   196,   197,   198,   199,   200,
    1126,  1064,  1077,   201,  1082,   202,  1096,  1097,  1123,   159,
     160,   161,   162,   163,   164,  1133,  1129,  1144,  1145,   175,
    1160,  1142,  1143,   165,   166,  1150,  1167,  1169,   882,  1172,
     478,   167,  1173,  1179,  1069,  1168,   726,   844,   679,  1015,
     192,   193,   194,   195,   196,   197,   198,   199,   200,   168,
     872,   782,   201,   652,   202,   293,   169,   372,   373,   374,
     375,   376,   377,   378,   379,   380,   381,   382,   479,   171,
     172,   173,   363,   364,   365,   366,   367,   368,   369,   447,
     370,   371,   372,   373,   374,   375,   376,   377,   378,   379,
     380,   381,   382,  1128,   892,   121,  1045,  1111,   867,   402,
     610,  1158,   203,   319,   148,   667,   639,   149,   469,   192,
     193,   194,   195,   196,   197,   198,   199,   200,   661,   742,
     750,   201,   480,   202,   847,   192,   193,   194,   195,   196,
     197,   198,   199,   200,   506,   513,  1091,   201,   622,   202,
    1046,   363,   364,   365,   366,   367,   368,   369,   822,   370,
     371,   372,   373,   374,   375,   376,   377,   378,   379,   380,
     381,   382,   363,   364,   365,   366,   367,   368,   369,     0,
     370,   371,   372,   373,   374,   375,   376,   377,   378,   379,
     380,   381,   382,   363,   364,   365,   366,   367,   368,   369,
       0,   370,   371,   372,   373,   374,   375,   376,   377,   378,
     379,   380,   381,   382,     0,     0,     0,     0,     0,   192,
     193,   194,   195,   196,   197,   198,   199,   200,     0,     0,
       0,   201,     0,   202,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   192,
     193,   194,   195,   196,   197,   198,   199,   200,     0,     0,
       0,   201,     0,   202,   365,   366,   367,   368,   369,     0,
     370,   371,   372,   373,   374,   375,   376,   377,   378,   379,
     380,   381,   382,  -548,     0,     0,     0,     0,     0,     0,
    -548,  -548,  -548,  -548,  -548,  -548,  -548,     0,  -548,  -548,
    -548,  -548,  -548,  -548,  -548,  -548,  -548,  -548,  -548,  -548,
    -548,     0,   407,     0,     0,     0,  -548,     0,  -548,  -548,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   192,   193,   194,   195,   196,   197,   198,
     199,   200,  -548,     0,     0,   201,     0,   202,     0,     0,
       0,     0,  -548,  -548,  -548,  -548,     0,     0,     0,     0,
       0,  -548,   408,     0,     0,     0,     0,     0,     0,     0,
       0,  -548,     0,     0,  -548,   409,     0,     0,     0,     0,
       0,     0,  -548,     0,     0,  -548,   410,     0,  -548,  -548,
       0,  -548,  -548,  -548,     0,     0,   411,   412,  -548,     0,
       0,     0,   704,     0,     0,  -548,     0,  -548,   413,   414,
     415,     0,     0,  -548,  -548,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,  -548,     0,     0,     0,  -548,     0,     0,     0,
       0,  -548,   364,   365,   366,   367,   368,   369,     0,   370,
     371,   372,   373,   374,   375,   376,   377,   378,   379,   380,
     381,   382,  -548,     0,     0,     0,     0,     0,     0,     0,
    -548,   705,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,  -548,     0,     0,     0,
       0,  -548,   706,  -549,  -549,  -549,  -549,     0,   370,   371,
     372,   373,   374,   375,   376,   377,   378,   379,   380,   381,
     382,     0,     0,  1072,     0,     0,  -548,  -548,     0,  -548,
       0,  -548,  -548,   363,   364,   365,   366,   367,   368,   369,
       0,   370,   371,   372,   373,   374,   375,   376,   377,   378,
     379,   380,   381,   382,   363,   364,   365,   366,   367,   368,
     369,     0,   370,   371,   372,   373,   374,   375,   376,   377,
     378,   379,   380,   381,   382,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,  -548,     0,     0,     0,     0,
       0,  -548,   363,   364,   365,   366,   367,   368,   369,     0,
     370,   371,   372,   373,   374,   375,   376,   377,   378,   379,
     380,   381,   382,     0,     0,     0,  -548,  -548,     0,     0,
    -548,   363,   364,   365,   366,   367,   368,   369,   695,   370,
     371,   372,   373,   374,   375,   376,   377,   378,   379,   380,
     381,   382,   363,   364,   365,   366,   367,   368,   369,     0,
     370,   371,   372,   373,   374,   375,   376,   377,   378,   379,
     380,   381,   382,     0,   235,     0,   363,   364,   365,   366,
     367,   368,   369,     0,   370,   371,   372,   373,   374,   375,
     376,   377,   378,   379,   380,   381,   382,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     1,     2,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     3,   363,   364,
     365,   366,   367,   368,   369,     4,   370,   371,   372,   373,
     374,   375,   376,   377,   378,   379,   380,   381,   382,     0,
       0,     0,     0,     0,     5,     6,     7,     0,     0,     0,
       0,     0,     8,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   363,   364,   365,   366,   367,   368,
     369,     9,   370,   371,   372,   373,   374,   375,   376,   377,
     378,   379,   380,   381,   382,    10,     0,     0,     0,     0,
       0,     0,     0,     0,    11,     0,     0,     0,     0,     0,
      12,     0,     0,    13,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   363,   364,   365,   366,   367,
     368,   369,    14,   370,   371,   372,   373,   374,   375,   376,
     377,   378,   379,   380,   381,   382,     0,     0,    15,    16,
       0,    17,     0,     0,     0,    18,     0,     0,     0,     0,
     394,    19,     0,     0,    20,    21,     1,     2,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   394,     0,     0,     0,     3,     0,     0,     0,     0,
       0,     0,     0,     4,     0,     0,     0,    22,     0,    23,
       0,    24,     0,     0,     0,     0,     0,     0,     0,   815,
       0,     0,     5,     6,     7,     0,     0,     0,    25,   692,
       8,     0,     0,     0,    26,     0,     0,     0,     0,     0,
       0,     0,    27,     0,    28,    29,    30,     0,     0,     9,
       0,    31,    32,     0,     0,     0,     0,     0,   699,     0,
       0,     0,     0,    10,   323,     0,    33,    34,    35,     0,
      36,   383,    11,     0,     0,     0,     0,   384,    12,   700,
       0,    13,     0,     0,     0,   684,    37,     0,     0,     0,
       0,    38,    39,    40,    41,     0,     0,     0,     0,     0,
      14,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,    15,    16,     0,    17,
       0,     0,     0,    18,     0,     0,     0,     0,     0,    19,
       0,     0,    20,    21,   363,   364,   365,   366,   367,   368,
     369,     0,   370,   371,   372,   373,   374,   375,   376,   377,
     378,   379,   380,   381,   382,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    22,     0,    23,     0,    24,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,    25,     0,     0,     0,
       0,     0,    26,     0,     0,     0,  1000,  1001,  1002,  1003,
      27,     0,    28,    29,    30,     0,     0,     0,     0,    31,
      32,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    33,    34,    35,     0,    36,     0,
       0,     0,     0,     0,     0,  1004,  1005,  1006,     0,     0,
       0,     0,     0,     0,    37,     0,     0,     0,     0,    38,
      39,    40,    41,   363,   364,   365,   366,   367,   368,   369,
       0,   370,   371,   372,   373,   374,   375,   376,   377,   378,
     379,   380,   381,   382,   930,   931,   932,   933,     0,     0,
       0,     0,     0,     0,   934,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   935,   936,     0,     0,     0,     0,   937,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   938,
       0,     0,     0,     0,     0,     0,     0,   939,     0,     0,
       0,     0,     0,   940,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   941,   942,     0,     0,     0,     0,
       0,     0,     0,     0,     0,  1007,  1008,     0,     0,     0,
       0,     0,     0,   943,   944,     0,     0,     0,   945,   946,
     947,   363,   364,   365,   366,   367,   368,   369,     0,   370,
     371,   372,   373,   374,   375,   376,   377,   378,   379,   380,
     381,   382,     0,     0,   948,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   949,     0,   950,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   951,     0,     0,   952,   953,   954,   955,   956,     0,
       0,     0,     0,     0,   816,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   957,   958,     0,     0,     0,
       0,     0,     0,   959,     0,     0,   819,   363,   364,   365,
     366,   367,   368,   369,     0,   370,   371,   372,   373,   374,
     375,   376,   377,   378,   379,   380,   381,   382,   363,   364,
     365,   366,   367,   368,   369,     0,   370,   371,   372,   373,
     374,   375,   376,   377,   378,   379,   380,   381,   382,     0,
       0,   483,     0,     0,     0,     0,     0,  1053,   363,   364,
     365,   366,   367,   368,   369,     0,   370,   371,   372,   373,
     374,   375,   376,   377,   378,   379,   380,   381,   382,     0,
       0,     0,     0,     0,     0,     0,   484,     0,     0,     0,
       0,   903,   363,   364,   365,   366,   367,   368,   369,  1054,
     370,   371,   372,   373,   374,   375,   376,   377,   378,   379,
     380,   381,   382,     0,     0,     0,     0,     0,   716,   363,
     364,   365,   366,   367,   368,   369,     0,   370,   371,   372,
     373,   374,   375,   376,   377,   378,   379,   380,   381,   382
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
       0,     0,     0,     0,     0,    21,     0,     0,     0,     0,
       0,     0,     0,    25,     0,     0,     0,     0,     0,     0,
      29,     0,     0,     0,     1,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     5,     9,    13,     0,     0,    17,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   165,     0,     0,
       0,     0,     0,     0,    33,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,    23,     0,     0,     0,
       0,     0,     0,     0,    27,     0,     0,     0,     0,     0,
       0,    31,     0,     0,     0,     3,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     7,    11,    15,     0,     0,    19,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    35,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    37,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,    39,     0,     0,    41,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   167,     0,     0,     0,
       0,   169,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   171,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,    43,     0,     0,     0,     0,     0,     0,
      45,    47,    49,    51,    53,    55,    57,     0,    59,    61,
      63,    65,    67,    69,    71,    73,    75,    77,    79,    81,
      83,     0,     0,     0,     0,     0,    85,     0,    87,    89,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    91,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    93,    95,    97,    99,     0,     0,     0,     0,
       0,   101,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   103,     0,     0,   105,     0,     0,     0,     0,     0,
       0,     0,   107,     0,     0,   109,     0,     0,   111,   113,
       0,   115,   117,   119,     0,     0,     0,     0,   121,     0,
       0,     0,     0,     0,     0,   123,     0,   125,     0,     0,
       0,     0,     0,   127,   129,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   131,     0,     0,     0,   133,     0,     0,     0,
       0,   135,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   137,     0,     0,     0,     0,     0,     0,     0,
     139,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   141,     0,     0,     0,
       0,   143,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   145,   147,     0,   149,
       0,   151,   153,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   155,     0,     0,     0,     0,
       0,   157,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   159,   161,     0,     0,
     163,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0
};

/* YYCONFL[I] -- lists of conflicting rule numbers, each terminated by
   0, pointed into by YYCONFLP.  */
static const short int yyconfl[] =
{
       0,   323,     0,   323,     0,   320,     0,   320,     0,   319,
       0,   319,     0,   321,     0,   321,     0,   322,     0,   322,
       0,   295,     0,   295,     0,   293,     0,   293,     0,   294,
       0,   294,     0,   324,     0,   324,     0,   427,     0,    56,
       0,   412,     0,   549,     0,   549,     0,   549,     0,   549,
       0,   549,     0,   549,     0,   549,     0,   549,     0,   549,
       0,   549,     0,   549,     0,   549,     0,   549,     0,   549,
       0,   549,     0,   549,     0,   549,     0,   549,     0,   549,
       0,   549,     0,   549,     0,   549,     0,   549,     0,   549,
       0,   549,     0,   549,     0,   549,     0,   549,     0,   549,
       0,   549,     0,   549,     0,   549,     0,   549,     0,   549,
       0,   549,     0,   549,     0,   549,     0,   549,     0,   549,
       0,   549,     0,   549,     0,   549,     0,   549,     0,   549,
       0,   549,     0,   549,     0,   549,     0,   549,     0,   549,
       0,   549,     0,   549,     0,   549,     0,   549,     0,   549,
       0,   549,     0,   549,     0,   549,     0,   549,     0,   549,
       0,   549,     0,   549,     0,    70,     0,   178,     0,   182,
       0,   183,     0
};

static const short int yycheck[] =
{
      21,   316,   188,   239,   673,   330,   422,   334,   307,    42,
     333,   757,     3,     3,     3,   131,   132,    20,     3,    20,
       8,     3,     3,   346,     4,     3,    49,   870,     3,   128,
     357,   358,   347,   348,   770,   156,   359,   837,     3,    36,
       3,   118,     3,   156,     6,   190,   106,   239,   105,   105,
     156,   105,   190,   237,   156,   592,    10,    11,    12,    13,
      14,    15,    16,   388,    18,    19,    20,    21,    22,    23,
      24,    25,    26,    27,    28,    29,    30,    38,    16,   394,
     115,    10,    11,    12,    13,    14,    15,    16,   387,    18,
      19,    20,    21,    22,    23,    24,    25,    26,    27,    28,
      29,    30,   107,   156,   156,   156,   190,     9,   156,   115,
     388,   156,   156,   115,   237,   861,   217,   217,    20,   140,
       3,   153,   115,   169,   217,   116,   156,   156,   971,   150,
     156,    52,   174,    54,   156,   327,   156,   128,   156,   228,
     187,   239,   177,   327,   165,   166,   167,   168,   169,    10,
      11,    12,    13,    14,    15,    16,   179,    18,    19,    20,
      21,    22,    23,    24,    25,    26,    27,    28,    29,    30,
     448,   177,    17,   102,    91,   177,    91,    94,    23,    94,
     201,   224,   327,   270,   177,   232,   511,     3,   265,     6,
     250,   329,   154,   207,   731,   522,   996,   187,   521,   228,
     243,    18,   289,   170,   327,   532,   327,   253,   329,     3,
      26,   298,   222,   223,   327,   225,   329,   963,   964,   497,
     217,   327,   264,   329,   551,   327,   239,   328,   328,   327,
     329,   241,   207,   969,   327,   187,   561,   269,   327,   217,
     245,   246,   247,   248,   569,   329,   561,    10,    11,    12,
      13,    14,    15,    16,   239,    18,    19,    20,    21,    22,
      23,    24,    25,    26,    27,    28,    29,    30,  1014,  1112,
     327,   327,    68,   327,   327,   327,   327,   287,   266,   327,
     232,   327,   327,   327,     3,   273,   274,   308,   309,   310,
       3,   326,   280,    68,  1040,  1041,   317,   327,  1044,   169,
     280,   327,   323,   840,   327,   327,   843,   327,     3,   327,
     277,   278,   256,   257,   327,   259,   328,   329,   339,   236,
     116,   236,   328,   329,   326,   328,   347,   328,   349,   350,
     351,   352,   353,   354,   355,   660,   329,   154,   329,   329,
     329,   647,   363,   364,   365,   366,   367,   329,   329,   674,
     371,   372,   373,   374,   375,   376,   377,   378,   379,   380,
     381,   382,   478,   479,   329,   481,   329,   328,   646,   257,
     156,   116,   678,     3,   169,    18,   330,   493,    62,   144,
     270,   251,   252,   253,   254,   708,   105,   420,   184,   327,
     715,    68,   717,   718,   719,   405,   115,    40,   133,   409,
     715,   330,    55,   116,   267,   170,  1152,   793,    38,   184,
     207,   106,   208,    56,   800,   128,   802,    39,   306,   725,
      32,   116,   721,  1169,    68,   144,     3,     4,     5,     6,
       7,     8,   207,   128,   118,    78,   270,   228,   115,    51,
      17,    18,   228,   127,    14,   455,   237,   182,    25,   133,
     169,   104,   249,   249,  1123,    33,    34,   327,   253,    81,
       3,    79,   106,   116,   116,    35,    43,    37,   152,   330,
     116,   166,   116,    50,   780,   240,    88,   498,   894,   895,
     896,   292,   128,    61,    61,   128,    63,    64,    65,   300,
     285,    68,   110,   514,     3,   516,   823,   115,   182,   885,
     977,   978,    80,   311,   819,    32,   892,   184,   263,   530,
      88,   154,   308,   166,   991,     3,   993,    26,   539,   207,
      97,   207,   206,   166,    51,   327,   144,   207,   330,   107,
       3,     4,     5,     6,     7,     8,   327,   328,   116,   116,
     184,   562,   563,   121,    17,    18,     3,   872,   569,   161,
     308,   195,    25,    26,     3,     4,     5,     6,     7,     8,
      33,    88,   947,   175,   208,     3,   951,   330,    17,    18,
      43,   956,     4,     5,     6,     7,    25,    50,   222,   222,
      26,    27,    28,    29,    30,   327,    79,   326,   330,   114,
      63,    64,    65,   779,    43,   173,     3,    46,     3,    13,
      14,    50,    16,   327,   207,   249,   330,   184,   207,   187,
     327,    84,    85,   330,    63,    64,    65,   110,    33,    34,
     312,   313,   115,   644,    97,   327,   327,     3,   330,   330,
     207,   327,   125,   327,   330,     8,   330,   110,     3,     4,
       5,     6,     7,     8,   291,   292,    61,   225,    97,   264,
     265,   144,    17,    18,   327,   327,   327,   330,   330,   330,
      25,   275,   276,   684,   685,    80,   327,   327,    33,   330,
     330,   692,    37,    88,   695,   207,  1021,  1022,    43,  1024,
    1025,  1026,   131,   116,  1029,    50,  1031,   114,  1033,     3,
    1035,   327,   107,     8,   330,   716,   273,   274,    63,    64,
      65,   116,   327,     3,   281,   330,   121,   207,   281,  1113,
     287,   288,  1116,   290,   291,   292,   293,   159,   295,   296,
     297,   125,   299,   300,   301,   877,   878,   879,   201,   202,
     203,   207,    97,   206,   920,   727,   728,   314,   315,   316,
     317,   318,   319,   320,   321,   322,   323,   324,   325,   628,
     629,     3,   329,   327,   331,     3,   330,    29,   173,     3,
      32,     3,     4,     5,     6,     7,     8,   327,   327,   218,
     330,   330,   187,     8,    20,    17,    18,   102,   263,   327,
       9,     3,   329,    25,  1020,    24,    25,    26,    27,    28,
      29,    30,  1028,   105,  1030,   816,     3,   292,  1034,   820,
    1036,    43,   190,    14,     3,   294,   134,   102,    50,   292,
     225,   105,   270,   834,   329,   329,   329,   329,   329,   329,
     841,    63,    64,    65,    13,    14,    15,    16,   193,    18,
      19,    20,    21,    22,    23,    24,    25,    26,    27,    28,
      29,    30,   329,   329,   317,   318,   319,   320,   321,   322,
     323,   324,   325,   874,   329,    97,   329,   291,   331,   880,
       3,     3,    36,   884,   207,   258,   327,   105,   317,   318,
     319,   320,   321,   322,   323,   324,   325,     3,   228,   326,
     329,     3,   331,   328,   905,  1071,     3,     4,     5,     6,
       7,     8,    23,    24,    25,    26,    27,    28,    29,    30,
      17,    18,   923,    18,  1140,  1141,     3,  1143,    25,    26,
       3,   279,   931,   932,   327,   934,     3,   328,   937,   938,
       3,   940,   941,   942,    97,     3,    43,   946,   156,   948,
       3,   950,   327,    50,     3,     3,   955,    22,    23,    24,
      25,    26,    27,    28,    29,    30,    63,    64,    65,     3,
     258,   217,   317,   318,   319,   320,   321,   322,   323,   324,
     325,   327,     3,   327,   329,   327,   331,     3,     8,    20,
     271,     8,     3,   994,   995,   329,     3,     3,     3,   187,
      97,     3,    14,     4,     8,     3,   105,   329,  1009,  1010,
      38,   267,     3,   258,   236,   327,   327,     3,     4,     5,
       6,     7,     8,     3,     3,    97,     3,   228,    47,    26,
       3,    17,    18,     3,     8,   328,     3,   328,   159,    25,
      21,    22,    23,    24,    25,    26,    27,    28,    29,    30,
    1049,  1052,   328,     3,   159,     3,   329,    43,     3,     8,
    1061,  1062,     8,  1064,    50,   328,     3,     4,     5,     6,
       7,     8,   272,   328,   328,   280,     3,    63,    64,    65,
      17,    18,   330,   330,     3,     3,   330,   330,    25,   330,
     329,   329,   329,   329,    30,   317,   318,   319,   320,   321,
     322,   323,   324,   325,   127,   330,    43,   329,   163,   331,
       3,    97,   115,    50,     3,     3,   265,  1118,  1119,  1120,
    1121,     3,   174,     4,     4,   170,    63,    64,    65,   327,
     190,     3,  1133,    55,    47,     3,     4,     5,     6,     7,
       8,    78,   327,   156,   328,     3,   161,    26,    26,    17,
      18,     3,     4,     5,     6,     7,     8,    25,     3,     3,
      97,   327,   240,   217,    26,    17,    18,   217,   269,    26,
       3,   329,   124,    25,   330,    43,   127,     3,   330,   330,
     330,   156,    50,   127,   327,   255,   139,     3,     3,   128,
     128,    43,     3,     3,     3,    63,    64,    65,    50,     3,
       3,   187,     4,     3,    47,   329,    20,   217,   217,   217,
      78,    63,    64,    65,   156,     3,     3,    34,     3,     8,
     317,   318,   319,   320,   321,   322,   323,   324,   325,    97,
     105,   105,   329,     4,   331,     3,     4,     5,     6,     7,
       8,   105,   108,   330,    61,    97,   170,   330,   330,    17,
      18,   330,     3,     3,   330,   330,   330,    25,     3,   128,
     329,   329,     3,    80,     3,     3,     4,     5,     6,     7,
       8,    88,   329,   157,   217,    43,   327,    47,   109,    17,
      18,   327,    50,   330,   328,    20,   329,    25,   330,   329,
     107,   329,   329,   329,    43,    63,    64,    65,     3,   116,
     329,   128,   329,   329,   121,    43,    20,   329,     3,   327,
      78,     3,    50,    19,    20,    21,    22,    23,    24,    25,
      26,    27,    28,    29,    30,    63,    64,    65,   327,    97,
       3,   317,   318,   319,   320,   321,   322,   323,   324,   325,
      78,   329,     5,   329,     4,   331,     5,     5,   225,     3,
       4,     5,     6,     7,     8,    20,   328,   154,     4,    97,
     159,   330,   330,    17,    18,   128,     3,   125,   327,     5,
     187,    25,     4,   330,   994,   183,   586,   736,   498,   927,
     317,   318,   319,   320,   321,   322,   323,   324,   325,    43,
     772,   648,   329,   327,   331,   147,    50,    20,    21,    22,
      23,    24,    25,    26,    27,    28,    29,    30,   225,    63,
      64,    65,    10,    11,    12,    13,    14,    15,    16,   253,
      18,    19,    20,    21,    22,    23,    24,    25,    26,    27,
      28,    29,    30,  1064,   801,     8,   966,  1046,   763,   221,
     402,  1114,    21,    97,    18,   486,   441,    18,   273,   317,
     318,   319,   320,   321,   322,   323,   324,   325,   482,   603,
     621,   329,   275,   331,   741,   317,   318,   319,   320,   321,
     322,   323,   324,   325,   314,   324,  1032,   329,   419,   331,
     967,    10,    11,    12,    13,    14,    15,    16,   700,    18,
      19,    20,    21,    22,    23,    24,    25,    26,    27,    28,
      29,    30,    10,    11,    12,    13,    14,    15,    16,    -1,
      18,    19,    20,    21,    22,    23,    24,    25,    26,    27,
      28,    29,    30,    10,    11,    12,    13,    14,    15,    16,
      -1,    18,    19,    20,    21,    22,    23,    24,    25,    26,
      27,    28,    29,    30,    -1,    -1,    -1,    -1,    -1,   317,
     318,   319,   320,   321,   322,   323,   324,   325,    -1,    -1,
      -1,   329,    -1,   331,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   317,
     318,   319,   320,   321,   322,   323,   324,   325,    -1,    -1,
      -1,   329,    -1,   331,    12,    13,    14,    15,    16,    -1,
      18,    19,    20,    21,    22,    23,    24,    25,    26,    27,
      28,    29,    30,     3,    -1,    -1,    -1,    -1,    -1,    -1,
      10,    11,    12,    13,    14,    15,    16,    -1,    18,    19,
      20,    21,    22,    23,    24,    25,    26,    27,    28,    29,
      30,    -1,   207,    -1,    -1,    -1,    36,    -1,    38,    39,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   317,   318,   319,   320,   321,   322,   323,
     324,   325,    62,    -1,    -1,   329,    -1,   331,    -1,    -1,
      -1,    -1,    72,    73,    74,    75,    -1,    -1,    -1,    -1,
      -1,    81,   257,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    91,    -1,    -1,    94,   270,    -1,    -1,    -1,    -1,
      -1,    -1,   102,    -1,    -1,   105,   281,    -1,   108,   109,
      -1,   111,   112,   113,    -1,    -1,   291,   292,   118,    -1,
      -1,    -1,   330,    -1,    -1,   125,    -1,   127,   303,   304,
     305,    -1,    -1,   133,   134,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   152,    -1,    -1,    -1,   156,    -1,    -1,    -1,
      -1,   161,    11,    12,    13,    14,    15,    16,    -1,    18,
      19,    20,    21,    22,    23,    24,    25,    26,    27,    28,
      29,    30,   182,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     190,   330,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   206,    -1,    -1,    -1,
      -1,   211,   330,    13,    14,    15,    16,    -1,    18,    19,
      20,    21,    22,    23,    24,    25,    26,    27,    28,    29,
      30,    -1,    -1,   330,    -1,    -1,   236,   237,    -1,   239,
      -1,   241,   242,    10,    11,    12,    13,    14,    15,    16,
      -1,    18,    19,    20,    21,    22,    23,    24,    25,    26,
      27,    28,    29,    30,    10,    11,    12,    13,    14,    15,
      16,    -1,    18,    19,    20,    21,    22,    23,    24,    25,
      26,    27,    28,    29,    30,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   295,    -1,    -1,    -1,    -1,
      -1,   301,    10,    11,    12,    13,    14,    15,    16,    -1,
      18,    19,    20,    21,    22,    23,    24,    25,    26,    27,
      28,    29,    30,    -1,    -1,    -1,   326,   327,    -1,    -1,
     330,    10,    11,    12,    13,    14,    15,    16,   105,    18,
      19,    20,    21,    22,    23,    24,    25,    26,    27,    28,
      29,    30,    10,    11,    12,    13,    14,    15,    16,    -1,
      18,    19,    20,    21,    22,    23,    24,    25,    26,    27,
      28,    29,    30,    -1,     0,    -1,    10,    11,    12,    13,
      14,    15,    16,    -1,    18,    19,    20,    21,    22,    23,
      24,    25,    26,    27,    28,    29,    30,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    34,    35,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    53,    10,    11,
      12,    13,    14,    15,    16,    61,    18,    19,    20,    21,
      22,    23,    24,    25,    26,    27,    28,    29,    30,    -1,
      -1,    -1,    -1,    -1,    80,    81,    82,    -1,    -1,    -1,
      -1,    -1,    88,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    10,    11,    12,    13,    14,    15,
      16,   107,    18,    19,    20,    21,    22,    23,    24,    25,
      26,    27,    28,    29,    30,   121,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   130,    -1,    -1,    -1,    -1,    -1,
     136,    -1,    -1,   139,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    10,    11,    12,    13,    14,
      15,    16,   158,    18,    19,    20,    21,    22,    23,    24,
      25,    26,    27,    28,    29,    30,    -1,    -1,   174,   175,
      -1,   177,    -1,    -1,    -1,   181,    -1,    -1,    -1,    -1,
     327,   187,    -1,    -1,   190,   191,    34,    35,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   327,    -1,    -1,    -1,    53,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    61,    -1,    -1,    -1,   223,    -1,   225,
      -1,   227,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    94,
      -1,    -1,    80,    81,    82,    -1,    -1,    -1,   244,   327,
      88,    -1,    -1,    -1,   250,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   258,    -1,   260,   261,   262,    -1,    -1,   107,
      -1,   267,   268,    -1,    -1,    -1,    -1,    -1,   327,    -1,
      -1,    -1,    -1,   121,   236,    -1,   282,   283,   284,    -1,
     286,   295,   130,    -1,    -1,    -1,    -1,   301,   136,   327,
      -1,   139,    -1,    -1,    -1,   211,   302,    -1,    -1,    -1,
      -1,   307,   308,   309,   310,    -1,    -1,    -1,    -1,    -1,
     158,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   174,   175,    -1,   177,
      -1,    -1,    -1,   181,    -1,    -1,    -1,    -1,    -1,   187,
      -1,    -1,   190,   191,    10,    11,    12,    13,    14,    15,
      16,    -1,    18,    19,    20,    21,    22,    23,    24,    25,
      26,    27,    28,    29,    30,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   223,    -1,   225,    -1,   227,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   244,    -1,    -1,    -1,
      -1,    -1,   250,    -1,    -1,    -1,    72,    73,    74,    75,
     258,    -1,   260,   261,   262,    -1,    -1,    -1,    -1,   267,
     268,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   282,   283,   284,    -1,   286,    -1,
      -1,    -1,    -1,    -1,    -1,   111,   112,   113,    -1,    -1,
      -1,    -1,    -1,    -1,   302,    -1,    -1,    -1,    -1,   307,
     308,   309,   310,    10,    11,    12,    13,    14,    15,    16,
      -1,    18,    19,    20,    21,    22,    23,    24,    25,    26,
      27,    28,    29,    30,    42,    43,    44,    45,    -1,    -1,
      -1,    -1,    -1,    -1,    52,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    70,    71,    -1,    -1,    -1,    -1,    76,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    87,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    95,    -1,    -1,
      -1,    -1,    -1,   101,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   122,   123,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   241,   242,    -1,    -1,    -1,
      -1,    -1,    -1,   141,   142,    -1,    -1,    -1,   146,   147,
     148,    10,    11,    12,    13,    14,    15,    16,    -1,    18,
      19,    20,    21,    22,    23,    24,    25,    26,    27,    28,
      29,    30,    -1,    -1,   172,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   190,    -1,   192,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   209,    -1,    -1,   212,   213,   214,   215,   216,    -1,
      -1,    -1,    -1,    -1,   211,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   233,   234,    -1,    -1,    -1,
      -1,    -1,    -1,   241,    -1,    -1,   105,    10,    11,    12,
      13,    14,    15,    16,    -1,    18,    19,    20,    21,    22,
      23,    24,    25,    26,    27,    28,    29,    30,    10,    11,
      12,    13,    14,    15,    16,    -1,    18,    19,    20,    21,
      22,    23,    24,    25,    26,    27,    28,    29,    30,    -1,
      -1,     3,    -1,    -1,    -1,    -1,    -1,    39,    10,    11,
      12,    13,    14,    15,    16,    -1,    18,    19,    20,    21,
      22,    23,    24,    25,    26,    27,    28,    29,    30,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    38,    -1,    -1,    -1,
      -1,    94,    10,    11,    12,    13,    14,    15,    16,    81,
      18,    19,    20,    21,    22,    23,    24,    25,    26,    27,
      28,    29,    30,    -1,    -1,    -1,    -1,    -1,    36,    10,
      11,    12,    13,    14,    15,    16,    -1,    18,    19,    20,
      21,    22,    23,    24,    25,    26,    27,    28,    29,    30
};

/* YYSTOS[STATE-NUM] -- The (internal number of the) accessing
   symbol of state STATE-NUM.  */
static const unsigned short int yystos[] =
{
       0,    34,    35,    53,    61,    80,    81,    82,    88,   107,
     121,   130,   136,   139,   158,   174,   175,   177,   181,   187,
     190,   191,   223,   225,   227,   244,   250,   258,   260,   261,
     262,   267,   268,   282,   283,   284,   286,   302,   307,   308,
     309,   310,   333,   334,   335,   363,   367,   375,   376,   379,
     381,   388,   389,   390,   396,   402,   403,   405,   408,   410,
     411,   412,   414,   417,   418,   419,   420,   421,   422,   426,
     427,   430,   433,   435,   440,   441,   444,   445,   446,   447,
     449,   451,   453,   454,   456,   458,   459,   461,   462,   468,
     472,   475,   477,   480,   483,   486,    68,   115,   184,   395,
     153,   269,   452,   207,    68,   106,   184,   195,   208,   222,
     249,   384,   397,   364,     3,     3,    68,   116,   184,   249,
     308,   384,    33,    34,    61,    80,    88,   107,   116,   121,
     173,   187,   225,   436,   437,   442,   443,   369,   257,   306,
     476,   116,   270,   452,   267,   207,   249,   369,   436,   442,
     346,     8,   266,   273,   274,   280,   428,   487,   488,     3,
       4,     5,     6,     7,     8,    17,    18,    25,    43,    50,
      61,    63,    64,    65,    68,    97,   116,   184,   207,   281,
     287,   288,   290,   293,   295,   296,   297,   299,   301,   314,
     315,   316,   317,   318,   319,   320,   321,   322,   323,   324,
     325,   329,   331,   428,   463,   467,   485,   270,   369,   377,
       3,   485,   495,     3,   207,   116,   311,   413,   413,   263,
       3,   413,   207,   207,   452,   207,   452,   256,   257,   259,
     481,   482,     3,   308,     3,     0,   334,   326,     3,     3,
     207,   207,     3,   406,   114,   380,   380,     8,   431,   432,
     207,   116,     3,   105,   115,   144,   169,   365,   114,   404,
       3,   404,     8,   434,     3,   207,   281,   159,   329,   371,
     371,   371,   156,   327,   156,   327,    79,   110,   115,   125,
     144,   370,   485,   125,     3,   423,   424,   207,     3,     3,
     409,     8,   448,   370,   156,   156,    26,    33,    84,    85,
     110,   201,   202,   203,   206,   347,   348,   485,     9,    20,
      20,   102,   450,   263,   327,   328,   329,     9,   485,    97,
     485,   485,   485,   236,   485,   499,    68,   184,   207,     3,
     329,   105,     3,   292,   190,    14,   465,     3,   294,   134,
     343,   102,   292,   105,   464,   343,   270,   329,   329,   329,
     329,   329,   329,   329,   329,   329,   485,   292,   300,   270,
     289,   298,   291,    10,    11,    12,    13,    14,    15,    16,
      18,    19,    20,    21,    22,    23,    24,    25,    26,    27,
      28,    29,    30,   295,   301,   115,   144,     3,   329,   349,
     350,   351,   354,   362,   327,     3,     3,   469,   470,    36,
     415,   217,   415,   406,   406,   207,   406,   207,   257,   270,
     281,   291,   292,   303,   304,   305,   473,   474,   258,   327,
     105,     3,   228,   484,   326,   492,   492,     3,   406,   328,
     169,   251,   252,   253,   254,   327,   455,    18,     3,     3,
     279,   327,   380,     3,   328,   366,     3,   365,   105,   327,
      97,     3,   156,     3,   327,   404,     3,   345,     3,    26,
      34,    61,    80,    88,   107,   116,   121,   187,   225,   437,
       3,    34,    61,    80,    88,   107,   116,   121,   187,   225,
     443,     3,   258,     3,    38,   353,   327,   406,   217,   328,
     327,   217,   327,     3,     3,    26,     3,   105,   327,   353,
     485,   485,   485,     8,    20,   271,   488,     3,   495,   496,
     485,   329,   485,   499,    91,    94,   236,     3,     3,     3,
     335,     3,   464,   465,     4,   291,   292,   485,     8,     3,
      14,   466,   464,   485,   495,    46,   131,   218,   495,   497,
     485,   485,    26,   485,   485,   485,   485,   485,   330,   465,
     465,   464,   105,   485,   485,   485,   485,   485,     6,    18,
     154,   329,    13,    14,    16,   485,    33,    37,   193,   329,
     485,   485,   485,   485,   485,   485,   485,   485,   485,   485,
     485,   328,   353,   335,   349,   190,   327,    62,   118,   133,
     152,   182,   206,   355,   357,    38,   352,   495,   116,   128,
     471,   478,    16,   327,   264,   265,   174,   264,   416,   267,
     416,   217,   169,   253,   457,   406,   105,     3,   258,   239,
     406,   327,   482,   334,   434,    52,    54,    32,    51,    88,
     161,   175,   391,   392,   394,     3,     3,    97,    47,   432,
       3,   228,   398,    26,   237,   336,   228,   349,     3,     3,
       8,   406,   327,   330,   328,   328,   159,   328,   159,   190,
     371,   469,     3,   144,   170,   240,   425,   424,     3,     3,
       3,     8,     8,   190,   371,   328,   328,   328,   349,   348,
     280,   272,   330,   335,   211,    91,    94,   485,   485,   330,
     464,   465,   327,   485,   465,   105,   330,   330,   485,   327,
     327,   330,   330,   330,   330,   330,   330,   465,     3,     6,
     154,   335,   495,   485,   485,   329,    36,   329,   329,   329,
     335,     3,   330,   330,     3,   378,   350,   133,   182,   358,
     351,   127,   163,   356,     3,     3,   329,   361,   115,   479,
       3,     3,   470,   265,   174,     3,     4,   460,     4,   170,
     474,   190,     3,    55,   393,   393,   104,   116,   166,   393,
      47,     3,   217,   327,   328,     4,   280,   115,   177,   328,
     329,   382,   383,   312,   313,   399,   156,   485,   161,   342,
     349,   336,   366,    49,   179,   407,     3,    26,    26,     3,
       3,   374,   232,   335,   240,   269,   217,   217,   328,   217,
     374,   232,   335,    26,    26,     3,   336,   329,   170,   277,
     278,   429,   330,   485,   485,    94,   211,   485,   485,   105,
     124,   498,   498,   464,   330,   330,   335,   495,   485,   335,
     335,   335,   330,   353,    20,   328,   327,   336,   356,   356,
     127,   156,   351,   127,   361,   327,   255,   471,   169,   253,
     285,   139,     3,     3,   106,   116,   128,   166,   385,   386,
       3,   128,   345,   128,     3,     3,     3,   392,     3,     4,
       3,   385,   352,     3,    47,   343,   336,   217,   217,   217,
      20,   156,   327,   368,   329,   372,   368,     3,     3,     3,
       8,   368,   372,   368,   105,   105,   105,   108,   337,     4,
     275,   276,   170,    94,   485,   102,   330,   495,   485,   330,
     330,   465,   330,   330,   330,   330,   330,   485,     3,     3,
     342,   351,   485,   156,   228,   359,   360,   351,   330,     3,
      42,    43,    44,    45,    52,    70,    71,    76,    87,    95,
     101,   122,   123,   141,   142,   146,   147,   148,   172,   190,
     192,   209,   212,   213,   214,   215,   216,   233,   234,   241,
     493,   116,   128,   329,   329,   128,   327,     3,   345,   329,
     382,   330,   335,   329,   338,   485,   431,   431,   431,    78,
     485,   157,     3,    78,   373,   485,   327,   368,   328,   217,
     368,   434,   434,   434,    47,   109,   341,   330,   485,   330,
      72,    73,    74,    75,   111,   112,   113,   241,   242,    20,
      20,   328,   343,   485,   329,   360,   329,   489,   489,   489,
     489,   489,   489,   329,   489,   489,   489,    43,   490,   489,
     490,   489,   329,   489,   490,   489,   490,   329,   329,   387,
     329,   329,   345,   345,   329,   386,   493,   385,   382,     3,
     400,   401,   327,    39,    81,   339,   239,   438,   438,   438,
     128,    20,   327,   330,   329,     3,     3,   438,   438,   338,
     485,   342,   330,   485,   485,     3,   345,     5,   491,   492,
     491,   491,     4,   494,   491,   491,   491,   492,   491,   492,
     491,   494,   491,   492,   491,   492,     5,     5,    18,    40,
      56,    78,   128,   154,   166,   222,   345,   345,   330,   330,
     345,   387,   330,   489,   327,   330,   485,   107,   245,   246,
     247,   248,   439,   225,    78,   485,    78,   485,   373,   328,
     239,   340,   343,    20,   330,   327,   330,   224,   243,   327,
     330,   330,   330,   330,   154,     4,     4,     5,     6,     7,
     128,   128,   329,   330,   330,   330,   382,   339,   401,   339,
     159,   485,   485,   485,   485,   374,   330,     3,   183,   125,
     344,   485,     5,     4,   492,   492,   492,   345,   345,   330,
     330
};


/* Prevent warning if -Wmissing-prototypes.  */
int yyparse (void);

/* Error token number */
#define YYTERROR 1

/* YYLLOC_DEFAULT -- Set CURRENT to span from RHS[1] to RHS[N].
   If N is 0, then set CURRENT to the empty location which ends
   the previous symbol: RHS[0] (always defined).  */


#ifndef YYLLOC_DEFAULT
# define YYLLOC_DEFAULT(Current, Rhs, N) ((void) 0)
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
yy_symbol_value_print (FILE *yyoutput, int yytype, YYSTYPE const * const yyvaluep)
{
  if (!yyvaluep)
    return;
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
yy_symbol_print (FILE *yyoutput, int yytype, YYSTYPE const * const yyvaluep)
{
  if (yytype < YYNTOKENS)
    YYFPRINTF (yyoutput, "token %s (", yytname[yytype]);
  else
    YYFPRINTF (yyoutput, "nterm %s (", yytname[yytype]);

  yy_symbol_value_print (yyoutput, yytype, yyvaluep);
  YYFPRINTF (yyoutput, ")");
}

# define YY_SYMBOL_PRINT(Title, Type, Value, Location)			    \
do {									    \
  if (yydebug)								    \
    {									    \
      YYFPRINTF (stderr, "%s ", Title);					    \
      yy_symbol_print (stderr, Type,					    \
		       Value);  \
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
      || (defined YYSTYPE_IS_TRIVIAL && YYSTYPE_IS_TRIVIAL))
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

  switch (yyn)
    {
        case 2:

/* Line 936 of glr.c  */
#line 422 "sql.ypp"
    { printf("> \n"); ((*yyvalp).ast_node) = (((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (2))].yystate.yysemantics.yysval.ast_node); parsetreeroot = ((*yyvalp).ast_node); /*output($$, 1);*/ ;}
    break;

  case 3:

/* Line 936 of glr.c  */
#line 423 "sql.ypp"
    { printf("> \n");/* $$ = $1;*/ parsetreeroot = ((*yyvalp).ast_node); /*output($$, 1);*/ ;}
    break;

  case 4:

/* Line 936 of glr.c  */
#line 428 "sql.ypp"
    { ((*yyvalp).ast_node)=(((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (1))].yystate.yysemantics.yysval.ast_node); output(((*yyvalp).ast_node), 1); puts("This is a select statement"); ;}
    break;

  case 5:

/* Line 936 of glr.c  */
#line 433 "sql.ypp"
    {
		((*yyvalp).ast_node) = newQueryStmt(t_query_stmt, NULL, (((yyGLRStackItem const *)yyvsp)[YYFILL ((2) - (3))].yystate.yysemantics.yysval.intval), (((yyGLRStackItem const *)yyvsp)[YYFILL ((3) - (3))].yystate.yysemantics.yysval.ast_node), NULL, NULL, NULL, NULL, NULL, NULL, NULL);
		puts("select_stmt1");
	;}
    break;

  case 6:

/* Line 936 of glr.c  */
#line 439 "sql.ypp"
    {
			((*yyvalp).ast_node) = newQueryStmt(t_query_stmt, NULL, (((yyGLRStackItem const *)yyvsp)[YYFILL ((2) - (11))].yystate.yysemantics.yysval.intval), (((yyGLRStackItem const *)yyvsp)[YYFILL ((3) - (11))].yystate.yysemantics.yysval.ast_node), (((yyGLRStackItem const *)yyvsp)[YYFILL ((5) - (11))].yystate.yysemantics.yysval.ast_node), (((yyGLRStackItem const *)yyvsp)[YYFILL ((6) - (11))].yystate.yysemantics.yysval.ast_node), (((yyGLRStackItem const *)yyvsp)[YYFILL ((7) - (11))].yystate.yysemantics.yysval.ast_node), (((yyGLRStackItem const *)yyvsp)[YYFILL ((8) - (11))].yystate.yysemantics.yysval.ast_node), (((yyGLRStackItem const *)yyvsp)[YYFILL ((9) - (11))].yystate.yysemantics.yysval.ast_node), (((yyGLRStackItem const *)yyvsp)[YYFILL ((10) - (11))].yystate.yysemantics.yysval.ast_node), (((yyGLRStackItem const *)yyvsp)[YYFILL ((11) - (11))].yystate.yysemantics.yysval.ast_node));
			puts("select_stmt2");
		;}
    break;

  case 7:

/* Line 936 of glr.c  */
#line 445 "sql.ypp"
    { ((*yyvalp).ast_node)= NULL; puts("opt_where is null"); ;}
    break;

  case 8:

/* Line 936 of glr.c  */
#line 446 "sql.ypp"
    { ((*yyvalp).ast_node) = newWhereList(t_where_list, NULL, (((yyGLRStackItem const *)yyvsp)[YYFILL ((2) - (2))].yystate.yysemantics.yysval.ast_node)); puts("opt_where");;}
    break;

  case 9:

/* Line 936 of glr.c  */
#line 448 "sql.ypp"
    { ((*yyvalp).ast_node)=NULL; puts("opt_groupby is null"); ;}
    break;

  case 10:

/* Line 936 of glr.c  */
#line 450 "sql.ypp"
    {
     	((*yyvalp).ast_node) = newGroupbyList(t_groupby_list, NULL, (((yyGLRStackItem const *)yyvsp)[YYFILL ((3) - (4))].yystate.yysemantics.yysval.ast_node), (((yyGLRStackItem const *)yyvsp)[YYFILL ((4) - (4))].yystate.yysemantics.yysval.intval));
      	puts("opt_groupby");
	;}
    break;

  case 11:

/* Line 936 of glr.c  */
#line 457 "sql.ypp"
    { 
		((*yyvalp).ast_node)=newGroupbyExpr(t_groupby_expr, (((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (2))].yystate.yysemantics.yysval.ast_node), (((yyGLRStackItem const *)yyvsp)[YYFILL ((2) - (2))].yystate.yysemantics.yysval.intval), NULL);
		puts("groupby_list1");
	;}
    break;

  case 12:

/* Line 936 of glr.c  */
#line 462 "sql.ypp"
    {
     	((*yyvalp).ast_node)=newGroupbyExpr(t_groupby_expr, (((yyGLRStackItem const *)yyvsp)[YYFILL ((3) - (4))].yystate.yysemantics.yysval.ast_node), (((yyGLRStackItem const *)yyvsp)[YYFILL ((4) - (4))].yystate.yysemantics.yysval.intval), (((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (4))].yystate.yysemantics.yysval.ast_node));
     	puts("groupby_list2");
    ;}
    break;

  case 13:

/* Line 936 of glr.c  */
#line 468 "sql.ypp"
    { ((*yyvalp).intval) = 0; ;}
    break;

  case 14:

/* Line 936 of glr.c  */
#line 469 "sql.ypp"
    { ((*yyvalp).intval) = 0; ;}
    break;

  case 15:

/* Line 936 of glr.c  */
#line 470 "sql.ypp"
    { ((*yyvalp).intval) = 1; ;}
    break;

  case 16:

/* Line 936 of glr.c  */
#line 473 "sql.ypp"
    { ((*yyvalp).intval) = 0; ;}
    break;

  case 17:

/* Line 936 of glr.c  */
#line 474 "sql.ypp"
    { ((*yyvalp).intval) = 1; ;}
    break;

  case 18:

/* Line 936 of glr.c  */
#line 477 "sql.ypp"
    { ((*yyvalp).ast_node)=NULL; puts("opt_having is null"); ;}
    break;

  case 19:

/* Line 936 of glr.c  */
#line 479 "sql.ypp"
    {
		((*yyvalp).ast_node)=newHavingList(t_having_list, NULL, (((yyGLRStackItem const *)yyvsp)[YYFILL ((2) - (2))].yystate.yysemantics.yysval.ast_node));
		puts("opt_having2");
	;}
    break;

  case 20:

/* Line 936 of glr.c  */
#line 485 "sql.ypp"
    { ((*yyvalp).ast_node)=NULL; puts("opt_orederby is null"); ;}
    break;

  case 21:

/* Line 936 of glr.c  */
#line 487 "sql.ypp"
    {
		((*yyvalp).ast_node)=newOrderbyList(t_groupby_list, NULL, (((yyGLRStackItem const *)yyvsp)[YYFILL ((3) - (3))].yystate.yysemantics.yysval.ast_node));
		puts("opt_orederby2");
	;}
    break;

  case 22:

/* Line 936 of glr.c  */
#line 493 "sql.ypp"
    {((*yyvalp).ast_node) = NULL; puts("opt_limit is null");;}
    break;

  case 23:

/* Line 936 of glr.c  */
#line 495 "sql.ypp"
    {
		((*yyvalp).ast_node)=newLimitExpr(t_limit_expr, NULL, (((yyGLRStackItem const *)yyvsp)[YYFILL ((2) - (2))].yystate.yysemantics.yysval.ast_node));
		puts("opt_limit2");
	;}
    break;

  case 24:

/* Line 936 of glr.c  */
#line 500 "sql.ypp"
    {
  		Limit_expr *n=(Limit_expr *)malloc(sizeof(struct Limit_expr));
  		n->type=t_limit_expr;
  		n->offset=(((yyGLRStackItem const *)yyvsp)[YYFILL ((2) - (4))].yystate.yysemantics.yysval.ast_node);
  		n->row_count=(((yyGLRStackItem const *)yyvsp)[YYFILL ((4) - (4))].yystate.yysemantics.yysval.ast_node);
		((*yyvalp).ast_node)=(Node *)n;
	;}
    break;

  case 25:

/* Line 936 of glr.c  */
#line 509 "sql.ypp"
    { ((*yyvalp).ast_node)=NULL; puts("opt_into_list is null"); ;}
    break;

  case 26:

/* Line 936 of glr.c  */
#line 510 "sql.ypp"
    {  ;}
    break;

  case 27:

/* Line 936 of glr.c  */
#line 513 "sql.ypp"
    { ((*yyvalp).ast_node) = newColumn(t_column, (((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (1))].yystate.yysemantics.yysval.strval), NULL, NULL); ;}
    break;

  case 28:

/* Line 936 of glr.c  */
#line 514 "sql.ypp"
    { ((*yyvalp).ast_node) = newColumn(t_column, (((yyGLRStackItem const *)yyvsp)[YYFILL ((3) - (3))].yystate.yysemantics.yysval.strval), NULL, (((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (3))].yystate.yysemantics.yysval.ast_node)); ;}
    break;

  case 29:

/* Line 936 of glr.c  */
#line 517 "sql.ypp"
    { ((*yyvalp).intval) = 0; ;}
    break;

  case 30:

/* Line 936 of glr.c  */
#line 518 "sql.ypp"
    { if(((*yyvalp).intval) & 1) yyerror("duplicate ALL option"); ((*yyvalp).intval) = (((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (2))].yystate.yysemantics.yysval.intval) | 1; ;}
    break;

  case 31:

/* Line 936 of glr.c  */
#line 519 "sql.ypp"
    { if(((*yyvalp).intval) & 2) yyerror("duplicate DISTINCT option"); ((*yyvalp).intval) = (((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (2))].yystate.yysemantics.yysval.intval) | 2; ;}
    break;

  case 32:

/* Line 936 of glr.c  */
#line 520 "sql.ypp"
    { if(((*yyvalp).intval) & 4) yyerror("duplicate DISTINCTROW option"); ((*yyvalp).intval) = (((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (2))].yystate.yysemantics.yysval.intval) | 4; ;}
    break;

  case 33:

/* Line 936 of glr.c  */
#line 521 "sql.ypp"
    { if(((*yyvalp).intval) & 8) yyerror("duplicate HIGH_PRIORITY option"); ((*yyvalp).intval) = (((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (2))].yystate.yysemantics.yysval.intval) | 8; ;}
    break;

  case 34:

/* Line 936 of glr.c  */
#line 522 "sql.ypp"
    { if(((*yyvalp).intval) & 16) yyerror("duplicate STRAIGHT_JOIN option"); ((*yyvalp).intval) = (((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (2))].yystate.yysemantics.yysval.intval) | 16; ;}
    break;

  case 35:

/* Line 936 of glr.c  */
#line 523 "sql.ypp"
    { if(((*yyvalp).intval) & 32) yyerror("duplicate SQL_SMALL_RESULT option"); ((*yyvalp).intval) = (((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (2))].yystate.yysemantics.yysval.intval) | 32; ;}
    break;

  case 36:

/* Line 936 of glr.c  */
#line 524 "sql.ypp"
    { if(((*yyvalp).intval) & 64) yyerror("duplicate SQL_BIG_RESULT option"); ((*yyvalp).intval) = (((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (2))].yystate.yysemantics.yysval.intval) | 64; ;}
    break;

  case 37:

/* Line 936 of glr.c  */
#line 525 "sql.ypp"
    { if(((*yyvalp).intval) & 128) yyerror("duplicate SQL_CALC_FOUND_ROWS option"); ((*yyvalp).intval) = (((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (2))].yystate.yysemantics.yysval.intval) | 128; ;}
    break;

  case 38:

/* Line 936 of glr.c  */
#line 528 "sql.ypp"
    { ((*yyvalp).ast_node) = newSelectList(t_select_list, 0, (((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (1))].yystate.yysemantics.yysval.ast_node), NULL); puts("select_expr_list1"); ;}
    break;

  case 39:

/* Line 936 of glr.c  */
#line 529 "sql.ypp"
    { ((*yyvalp).ast_node) = newSelectList(t_select_list, 0, (((yyGLRStackItem const *)yyvsp)[YYFILL ((3) - (3))].yystate.yysemantics.yysval.ast_node), (((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (3))].yystate.yysemantics.yysval.ast_node)); puts("select_expr_list2"); ;}
    break;

  case 40:

/* Line 936 of glr.c  */
#line 530 "sql.ypp"
    { ((*yyvalp).ast_node) = newSelectList(t_select_list, 1, NULL, NULL); puts("select_expr_list3"); ;}
    break;

  case 41:

/* Line 936 of glr.c  */
#line 533 "sql.ypp"
    { ((*yyvalp).ast_node) = newSelectExpr(t_select_expr, (((yyGLRStackItem const *)yyvsp)[YYFILL ((2) - (2))].yystate.yysemantics.yysval.strval), (((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (2))].yystate.yysemantics.yysval.ast_node)); puts("select_expr"); ;}
    break;

  case 42:

/* Line 936 of glr.c  */
#line 536 "sql.ypp"
    { ((*yyvalp).ast_node)=newFromList(t_from_list, (((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (1))].yystate.yysemantics.yysval.ast_node), NULL); puts("table_references1"); ;}
    break;

  case 43:

/* Line 936 of glr.c  */
#line 537 "sql.ypp"
    { ((*yyvalp).ast_node)=newFromList(t_from_list, (((yyGLRStackItem const *)yyvsp)[YYFILL ((3) - (3))].yystate.yysemantics.yysval.ast_node), (((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (3))].yystate.yysemantics.yysval.ast_node)); puts("table_references2"); ;}
    break;

  case 44:

/* Line 936 of glr.c  */
#line 540 "sql.ypp"
    { ((*yyvalp).ast_node)=(((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (1))].yystate.yysemantics.yysval.ast_node);puts("table_referen1"); ;}
    break;

  case 45:

/* Line 936 of glr.c  */
#line 541 "sql.ypp"
    { ((*yyvalp).ast_node)=(((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (1))].yystate.yysemantics.yysval.ast_node);puts("table_referen2");;}
    break;

  case 46:

/* Line 936 of glr.c  */
#line 546 "sql.ypp"
    {
	    ((*yyvalp).ast_node)=newTable(t_table, NULL,(((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (2))].yystate.yysemantics.yysval.strval), (((yyGLRStackItem const *)yyvsp)[YYFILL ((2) - (2))].yystate.yysemantics.yysval.strval), 0, NULL);
	    puts("table_factor1");
    ;}
    break;

  case 47:

/* Line 936 of glr.c  */
#line 551 "sql.ypp"
    {
		((*yyvalp).ast_node)=newTable(t_table, (((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (4))].yystate.yysemantics.yysval.strval), (((yyGLRStackItem const *)yyvsp)[YYFILL ((3) - (4))].yystate.yysemantics.yysval.strval), (((yyGLRStackItem const *)yyvsp)[YYFILL ((4) - (4))].yystate.yysemantics.yysval.strval), 0, NULL);
   		puts("table_factor2");;}
    break;

  case 48:

/* Line 936 of glr.c  */
#line 555 "sql.ypp"
    {
		((*yyvalp).ast_node)=newTable(t_table, NULL, NULL, (((yyGLRStackItem const *)yyvsp)[YYFILL ((3) - (3))].yystate.yysemantics.yysval.strval), 1, (((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (3))].yystate.yysemantics.yysval.ast_node));
		puts("table_factor3");
	;}
    break;

  case 49:

/* Line 936 of glr.c  */
#line 559 "sql.ypp"
    { ((*yyvalp).ast_node)=(((yyGLRStackItem const *)yyvsp)[YYFILL ((2) - (3))].yystate.yysemantics.yysval.ast_node); ;}
    break;

  case 52:

/* Line 936 of glr.c  */
#line 566 "sql.ypp"
    { ((*yyvalp).strval)=(((yyGLRStackItem const *)yyvsp)[YYFILL ((2) - (2))].yystate.yysemantics.yysval.strval); puts("opt_as_alias1");;}
    break;

  case 53:

/* Line 936 of glr.c  */
#line 567 "sql.ypp"
    { ((*yyvalp).strval)=(((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (1))].yystate.yysemantics.yysval.strval); puts("opt_as_alias2");;}
    break;

  case 54:

/* Line 936 of glr.c  */
#line 568 "sql.ypp"
    { ((*yyvalp).strval)=NULL; puts("opt_as_alias3");;}
    break;

  case 55:

/* Line 936 of glr.c  */
#line 584 "sql.ypp"
    {
		((*yyvalp).ast_node)=newJoin(t_join, (((yyGLRStackItem const *)yyvsp)[YYFILL ((2) - (5))].yystate.yysemantics.yysval.intval), (((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (5))].yystate.yysemantics.yysval.ast_node), (((yyGLRStackItem const *)yyvsp)[YYFILL ((4) - (5))].yystate.yysemantics.yysval.ast_node), (((yyGLRStackItem const *)yyvsp)[YYFILL ((5) - (5))].yystate.yysemantics.yysval.ast_node));
	;}
    break;

  case 56:

/* Line 936 of glr.c  */
#line 588 "sql.ypp"
    {
		((*yyvalp).ast_node)=newJoin(t_join, -1, (((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (3))].yystate.yysemantics.yysval.ast_node), (((yyGLRStackItem const *)yyvsp)[YYFILL ((3) - (3))].yystate.yysemantics.yysval.ast_node), NULL);
	;}
    break;

  case 57:

/* Line 936 of glr.c  */
#line 592 "sql.ypp"
    {
		Node *m=newCondition(t_condition, 1, (((yyGLRStackItem const *)yyvsp)[YYFILL ((5) - (5))].yystate.yysemantics.yysval.ast_node));
		((*yyvalp).ast_node)=newJoin(t_join, -1, (((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (5))].yystate.yysemantics.yysval.ast_node), (((yyGLRStackItem const *)yyvsp)[YYFILL ((3) - (5))].yystate.yysemantics.yysval.ast_node), m);
	;}
    break;

  case 58:

/* Line 936 of glr.c  */
#line 597 "sql.ypp"
    {
		((*yyvalp).ast_node)=newJoin(t_join, (((yyGLRStackItem const *)yyvsp)[YYFILL ((2) - (6))].yystate.yysemantics.yysval.intval)+(((yyGLRStackItem const *)yyvsp)[YYFILL ((3) - (6))].yystate.yysemantics.yysval.intval), (((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (6))].yystate.yysemantics.yysval.ast_node), (((yyGLRStackItem const *)yyvsp)[YYFILL ((5) - (6))].yystate.yysemantics.yysval.ast_node), (((yyGLRStackItem const *)yyvsp)[YYFILL ((6) - (6))].yystate.yysemantics.yysval.ast_node));
    ;}
    break;

  case 59:

/* Line 936 of glr.c  */
#line 601 "sql.ypp"
    {
		((*yyvalp).ast_node)=newJoin(t_join, 32 + (((yyGLRStackItem const *)yyvsp)[YYFILL ((3) - (5))].yystate.yysemantics.yysval.intval), (((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (5))].yystate.yysemantics.yysval.ast_node), (((yyGLRStackItem const *)yyvsp)[YYFILL ((5) - (5))].yystate.yysemantics.yysval.ast_node), NULL);
	;}
    break;

  case 60:

/* Line 936 of glr.c  */
#line 606 "sql.ypp"
    { ((*yyvalp).intval) = 0; ;}
    break;

  case 61:

/* Line 936 of glr.c  */
#line 607 "sql.ypp"
    { ((*yyvalp).intval) = 1; ;}
    break;

  case 62:

/* Line 936 of glr.c  */
#line 608 "sql.ypp"
    { ((*yyvalp).intval) = 2; ;}
    break;

  case 63:

/* Line 936 of glr.c  */
#line 611 "sql.ypp"
    { ((*yyvalp).intval) = 0; ;}
    break;

  case 64:

/* Line 936 of glr.c  */
#line 612 "sql.ypp"
    {((*yyvalp).intval) = 4; ;}
    break;

  case 65:

/* Line 936 of glr.c  */
#line 615 "sql.ypp"
    { ((*yyvalp).intval) = 8; ;}
    break;

  case 66:

/* Line 936 of glr.c  */
#line 616 "sql.ypp"
    { ((*yyvalp).intval) = 16; ;}
    break;

  case 67:

/* Line 936 of glr.c  */
#line 619 "sql.ypp"
    { ((*yyvalp).intval) = 8 + (((yyGLRStackItem const *)yyvsp)[YYFILL ((2) - (2))].yystate.yysemantics.yysval.intval); ;}
    break;

  case 68:

/* Line 936 of glr.c  */
#line 620 "sql.ypp"
    { ((*yyvalp).intval) = 16 + (((yyGLRStackItem const *)yyvsp)[YYFILL ((2) - (2))].yystate.yysemantics.yysval.intval); ;}
    break;

  case 69:

/* Line 936 of glr.c  */
#line 621 "sql.ypp"
    { ((*yyvalp).intval) = 0; ;}
    break;

  case 71:

/* Line 936 of glr.c  */
#line 625 "sql.ypp"
    {((*yyvalp).ast_node)=(((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (1))].yystate.yysemantics.yysval.ast_node);;}
    break;

  case 72:

/* Line 936 of glr.c  */
#line 633 "sql.ypp"
    {
    	((*yyvalp).ast_node)=newCondition(t_condition, 1, (((yyGLRStackItem const *)yyvsp)[YYFILL ((2) - (2))].yystate.yysemantics.yysval.ast_node));
    ;}
    break;

  case 73:

/* Line 936 of glr.c  */
#line 637 "sql.ypp"
    {
    	((*yyvalp).ast_node)=newCondition(t_condition, 2, (((yyGLRStackItem const *)yyvsp)[YYFILL ((3) - (4))].yystate.yysemantics.yysval.ast_node));
    ;}
    break;

  case 74:

/* Line 936 of glr.c  */
#line 656 "sql.ypp"
    { emit("INDEX %s", (((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (1))].yystate.yysemantics.yysval.strval));  ;}
    break;

  case 75:

/* Line 936 of glr.c  */
#line 657 "sql.ypp"
    { emit("INDEX %s", (((yyGLRStackItem const *)yyvsp)[YYFILL ((3) - (3))].yystate.yysemantics.yysval.strval)); ;}
    break;

  case 76:

/* Line 936 of glr.c  */
#line 660 "sql.ypp"
    {  ;}
    break;

  case 77:

/* Line 936 of glr.c  */
#line 665 "sql.ypp"
    { emit("STMT"); ;}
    break;

  case 78:

/* Line 936 of glr.c  */
#line 670 "sql.ypp"
    { emit("DELETEONE %d %s", (((yyGLRStackItem const *)yyvsp)[YYFILL ((2) - (7))].yystate.yysemantics.yysval.ast_node), (((yyGLRStackItem const *)yyvsp)[YYFILL ((4) - (7))].yystate.yysemantics.yysval.strval)); free((((yyGLRStackItem const *)yyvsp)[YYFILL ((4) - (7))].yystate.yysemantics.yysval.strval)); ;}
    break;

  case 79:

/* Line 936 of glr.c  */
#line 673 "sql.ypp"
    { ((*yyvalp).ast_node) = (((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (2))].yystate.yysemantics.yysval.ast_node) + 01; ;}
    break;

  case 80:

/* Line 936 of glr.c  */
#line 674 "sql.ypp"
    { ((*yyvalp).ast_node) = (((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (2))].yystate.yysemantics.yysval.ast_node) + 02; ;}
    break;

  case 81:

/* Line 936 of glr.c  */
#line 675 "sql.ypp"
    { ((*yyvalp).ast_node) = (((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (2))].yystate.yysemantics.yysval.ast_node) + 04; ;}
    break;

  case 82:

/* Line 936 of glr.c  */
#line 676 "sql.ypp"
    { ((*yyvalp).ast_node) = 0; ;}
    break;

  case 83:

/* Line 936 of glr.c  */
#line 682 "sql.ypp"
    {  ;}
    break;

  case 84:

/* Line 936 of glr.c  */
#line 684 "sql.ypp"
    {  ;}
    break;

  case 85:

/* Line 936 of glr.c  */
#line 685 "sql.ypp"
    {  ;}
    break;

  case 88:

/* Line 936 of glr.c  */
#line 693 "sql.ypp"
    { emit("DELETEMULTI %d %d %d", (((yyGLRStackItem const *)yyvsp)[YYFILL ((2) - (7))].yystate.yysemantics.yysval.ast_node), (((yyGLRStackItem const *)yyvsp)[YYFILL ((4) - (7))].yystate.yysemantics.yysval.ast_node), (((yyGLRStackItem const *)yyvsp)[YYFILL ((6) - (7))].yystate.yysemantics.yysval.ast_node)); ;}
    break;

  case 89:

/* Line 936 of glr.c  */
#line 698 "sql.ypp"
    { emit("STMT"); ;}
    break;

  case 90:

/* Line 936 of glr.c  */
#line 704 "sql.ypp"
    { emit("INSERTVALS %d %d %s", (((yyGLRStackItem const *)yyvsp)[YYFILL ((2) - (8))].yystate.yysemantics.yysval.intval), (((yyGLRStackItem const *)yyvsp)[YYFILL ((7) - (8))].yystate.yysemantics.yysval.ast_node), (((yyGLRStackItem const *)yyvsp)[YYFILL ((4) - (8))].yystate.yysemantics.yysval.strval)); free((((yyGLRStackItem const *)yyvsp)[YYFILL ((4) - (8))].yystate.yysemantics.yysval.strval)) ;}
    break;

  case 92:

/* Line 936 of glr.c  */
#line 708 "sql.ypp"
    { emit("DUPUPDATE %d", (((yyGLRStackItem const *)yyvsp)[YYFILL ((5) - (5))].yystate.yysemantics.yysval.ast_node)); ;}
    break;

  case 93:

/* Line 936 of glr.c  */
#line 711 "sql.ypp"
    { ((*yyvalp).intval) = 0; ;}
    break;

  case 94:

/* Line 936 of glr.c  */
#line 712 "sql.ypp"
    { ((*yyvalp).intval) = (((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (2))].yystate.yysemantics.yysval.intval) | 01 ; ;}
    break;

  case 95:

/* Line 936 of glr.c  */
#line 713 "sql.ypp"
    { ((*yyvalp).intval) = (((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (2))].yystate.yysemantics.yysval.intval) | 02 ; ;}
    break;

  case 96:

/* Line 936 of glr.c  */
#line 714 "sql.ypp"
    { ((*yyvalp).intval) = (((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (2))].yystate.yysemantics.yysval.intval) | 04 ; ;}
    break;

  case 97:

/* Line 936 of glr.c  */
#line 715 "sql.ypp"
    { ((*yyvalp).intval) = (((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (2))].yystate.yysemantics.yysval.intval) | 010 ; ;}
    break;

  case 101:

/* Line 936 of glr.c  */
#line 722 "sql.ypp"
    {  ;}
    break;

  case 108:

/* Line 936 of glr.c  */
#line 738 "sql.ypp"
    { emit("INSERTASGN %d %d %s", (((yyGLRStackItem const *)yyvsp)[YYFILL ((2) - (7))].yystate.yysemantics.yysval.intval), (((yyGLRStackItem const *)yyvsp)[YYFILL ((6) - (7))].yystate.yysemantics.yysval.ast_node), (((yyGLRStackItem const *)yyvsp)[YYFILL ((4) - (7))].yystate.yysemantics.yysval.strval)); free((((yyGLRStackItem const *)yyvsp)[YYFILL ((4) - (7))].yystate.yysemantics.yysval.strval)) ;}
    break;

  case 109:

/* Line 936 of glr.c  */
#line 743 "sql.ypp"
    { emit("INSERTSELECT %d %s", (((yyGLRStackItem const *)yyvsp)[YYFILL ((2) - (7))].yystate.yysemantics.yysval.intval), (((yyGLRStackItem const *)yyvsp)[YYFILL ((4) - (7))].yystate.yysemantics.yysval.strval)); free((((yyGLRStackItem const *)yyvsp)[YYFILL ((4) - (7))].yystate.yysemantics.yysval.strval)); ;}
    break;

  case 114:

/* Line 936 of glr.c  */
#line 762 "sql.ypp"
    { emit("STMT"); ;}
    break;

  case 115:

/* Line 936 of glr.c  */
#line 768 "sql.ypp"
    { emit("REPLACEVALS %d %d %s", (((yyGLRStackItem const *)yyvsp)[YYFILL ((2) - (8))].yystate.yysemantics.yysval.intval), (((yyGLRStackItem const *)yyvsp)[YYFILL ((7) - (8))].yystate.yysemantics.yysval.ast_node), (((yyGLRStackItem const *)yyvsp)[YYFILL ((4) - (8))].yystate.yysemantics.yysval.strval)); free((((yyGLRStackItem const *)yyvsp)[YYFILL ((4) - (8))].yystate.yysemantics.yysval.strval)) ;}
    break;

  case 116:

/* Line 936 of glr.c  */
#line 774 "sql.ypp"
    { emit("REPLACEASGN %d %d %s", (((yyGLRStackItem const *)yyvsp)[YYFILL ((2) - (7))].yystate.yysemantics.yysval.intval), (((yyGLRStackItem const *)yyvsp)[YYFILL ((6) - (7))].yystate.yysemantics.yysval.ast_node), (((yyGLRStackItem const *)yyvsp)[YYFILL ((4) - (7))].yystate.yysemantics.yysval.strval)); free((((yyGLRStackItem const *)yyvsp)[YYFILL ((4) - (7))].yystate.yysemantics.yysval.strval)) ;}
    break;

  case 117:

/* Line 936 of glr.c  */
#line 779 "sql.ypp"
    { emit("REPLACESELECT %d %s", (((yyGLRStackItem const *)yyvsp)[YYFILL ((2) - (7))].yystate.yysemantics.yysval.intval), (((yyGLRStackItem const *)yyvsp)[YYFILL ((4) - (7))].yystate.yysemantics.yysval.strval)); free((((yyGLRStackItem const *)yyvsp)[YYFILL ((4) - (7))].yystate.yysemantics.yysval.strval)); ;}
    break;

  case 118:

/* Line 936 of glr.c  */
#line 783 "sql.ypp"
    { emit("STMT"); ;}
    break;

  case 119:

/* Line 936 of glr.c  */
#line 790 "sql.ypp"
    { emit("UPDATE %d %d %d", (((yyGLRStackItem const *)yyvsp)[YYFILL ((2) - (8))].yystate.yysemantics.yysval.ast_node), (((yyGLRStackItem const *)yyvsp)[YYFILL ((3) - (8))].yystate.yysemantics.yysval.ast_node), (((yyGLRStackItem const *)yyvsp)[YYFILL ((5) - (8))].yystate.yysemantics.yysval.ast_node)); ;}
    break;

  case 123:

/* Line 936 of glr.c  */
#line 800 "sql.ypp"
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

  case 124:

/* Line 936 of glr.c  */
#line 810 "sql.ypp"
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

  case 127:

/* Line 936 of glr.c  */
#line 831 "sql.ypp"
    { ((*yyvalp).ast_node)=(((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (1))].yystate.yysemantics.yysval.ast_node); output(((*yyvalp).ast_node), 1); puts("This is a create_database statement"); ;}
    break;

  case 128:

/* Line 936 of glr.c  */
#line 836 "sql.ypp"
    { ((*yyvalp).ast_node) =  newCreateDatabaseStmt(t_create_database_stmt, 1, (((yyGLRStackItem const *)yyvsp)[YYFILL ((3) - (4))].yystate.yysemantics.yysval.intval), (((yyGLRStackItem const *)yyvsp)[YYFILL ((4) - (4))].yystate.yysemantics.yysval.strval)); ;}
    break;

  case 129:

/* Line 936 of glr.c  */
#line 838 "sql.ypp"
    { ((*yyvalp).ast_node) =  newCreateDatabaseStmt(t_create_database_stmt, 2, (((yyGLRStackItem const *)yyvsp)[YYFILL ((3) - (4))].yystate.yysemantics.yysval.intval), (((yyGLRStackItem const *)yyvsp)[YYFILL ((4) - (4))].yystate.yysemantics.yysval.strval)); ;}
    break;

  case 130:

/* Line 936 of glr.c  */
#line 841 "sql.ypp"
    { ((*yyvalp).intval) = 0; ;}
    break;

  case 131:

/* Line 936 of glr.c  */
#line 842 "sql.ypp"
    { ((*yyvalp).intval) = 1; ;}
    break;

  case 132:

/* Line 936 of glr.c  */
#line 847 "sql.ypp"
    { ((*yyvalp).ast_node)=(((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (1))].yystate.yysemantics.yysval.ast_node); output(((*yyvalp).ast_node), 1); puts("This is a create_table statement"); ;}
    break;

  case 133:

/* Line 936 of glr.c  */
#line 851 "sql.ypp"
    { ((*yyvalp).ast_node) = newCreateTableStmt(t_create_table_stmt, (((yyGLRStackItem const *)yyvsp)[YYFILL ((2) - (8))].yystate.yysemantics.yysval.intval), (((yyGLRStackItem const *)yyvsp)[YYFILL ((4) - (8))].yystate.yysemantics.yysval.intval), (((yyGLRStackItem const *)yyvsp)[YYFILL ((5) - (8))].yystate.yysemantics.yysval.strval), NULL, (((yyGLRStackItem const *)yyvsp)[YYFILL ((7) - (8))].yystate.yysemantics.yysval.ast_node), NULL); ;}
    break;

  case 134:

/* Line 936 of glr.c  */
#line 855 "sql.ypp"
    { ((*yyvalp).ast_node) = newCreateTableStmt(t_create_table_stmt, (((yyGLRStackItem const *)yyvsp)[YYFILL ((2) - (10))].yystate.yysemantics.yysval.intval), (((yyGLRStackItem const *)yyvsp)[YYFILL ((4) - (10))].yystate.yysemantics.yysval.intval), (((yyGLRStackItem const *)yyvsp)[YYFILL ((5) - (10))].yystate.yysemantics.yysval.strval), (((yyGLRStackItem const *)yyvsp)[YYFILL ((7) - (10))].yystate.yysemantics.yysval.strval), (((yyGLRStackItem const *)yyvsp)[YYFILL ((9) - (10))].yystate.yysemantics.yysval.ast_node), NULL); ;}
    break;

  case 135:

/* Line 936 of glr.c  */
#line 860 "sql.ypp"
    { ((*yyvalp).ast_node) = newCreateTableStmt(t_create_table_stmt, (((yyGLRStackItem const *)yyvsp)[YYFILL ((2) - (9))].yystate.yysemantics.yysval.intval), (((yyGLRStackItem const *)yyvsp)[YYFILL ((4) - (9))].yystate.yysemantics.yysval.intval), (((yyGLRStackItem const *)yyvsp)[YYFILL ((5) - (9))].yystate.yysemantics.yysval.strval), NULL, (((yyGLRStackItem const *)yyvsp)[YYFILL ((7) - (9))].yystate.yysemantics.yysval.ast_node), (((yyGLRStackItem const *)yyvsp)[YYFILL ((9) - (9))].yystate.yysemantics.yysval.ast_node)); ;}
    break;

  case 136:

/* Line 936 of glr.c  */
#line 864 "sql.ypp"
    { ((*yyvalp).ast_node) = newCreateTableStmt(t_create_table_stmt, (((yyGLRStackItem const *)yyvsp)[YYFILL ((2) - (6))].yystate.yysemantics.yysval.intval), (((yyGLRStackItem const *)yyvsp)[YYFILL ((4) - (6))].yystate.yysemantics.yysval.intval), (((yyGLRStackItem const *)yyvsp)[YYFILL ((5) - (6))].yystate.yysemantics.yysval.strval), NULL, NULL, (((yyGLRStackItem const *)yyvsp)[YYFILL ((6) - (6))].yystate.yysemantics.yysval.ast_node)); ;}
    break;

  case 137:

/* Line 936 of glr.c  */
#line 869 "sql.ypp"
    { ((*yyvalp).ast_node) = newCreateTableStmt(t_create_table_stmt, (((yyGLRStackItem const *)yyvsp)[YYFILL ((2) - (11))].yystate.yysemantics.yysval.intval), (((yyGLRStackItem const *)yyvsp)[YYFILL ((4) - (11))].yystate.yysemantics.yysval.intval), (((yyGLRStackItem const *)yyvsp)[YYFILL ((5) - (11))].yystate.yysemantics.yysval.strval), (((yyGLRStackItem const *)yyvsp)[YYFILL ((7) - (11))].yystate.yysemantics.yysval.strval), (((yyGLRStackItem const *)yyvsp)[YYFILL ((9) - (11))].yystate.yysemantics.yysval.ast_node), (((yyGLRStackItem const *)yyvsp)[YYFILL ((11) - (11))].yystate.yysemantics.yysval.ast_node)); ;}
    break;

  case 138:

/* Line 936 of glr.c  */
#line 874 "sql.ypp"
    { ((*yyvalp).ast_node) = newCreateTableStmt(t_create_table_stmt, (((yyGLRStackItem const *)yyvsp)[YYFILL ((2) - (8))].yystate.yysemantics.yysval.intval), (((yyGLRStackItem const *)yyvsp)[YYFILL ((4) - (8))].yystate.yysemantics.yysval.intval), (((yyGLRStackItem const *)yyvsp)[YYFILL ((5) - (8))].yystate.yysemantics.yysval.strval), (((yyGLRStackItem const *)yyvsp)[YYFILL ((7) - (8))].yystate.yysemantics.yysval.strval), NULL, (((yyGLRStackItem const *)yyvsp)[YYFILL ((8) - (8))].yystate.yysemantics.yysval.ast_node)); ;}
    break;

  case 139:

/* Line 936 of glr.c  */
#line 879 "sql.ypp"
    { ((*yyvalp).ast_node) = newCreateSelectStmt(t_create_select_stmt, (((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (3))].yystate.yysemantics.yysval.intval), 0, (((yyGLRStackItem const *)yyvsp)[YYFILL ((3) - (3))].yystate.yysemantics.yysval.ast_node)); ;}
    break;

  case 140:

/* Line 936 of glr.c  */
#line 882 "sql.ypp"
    { ((*yyvalp).intval) = 0; ;}
    break;

  case 141:

/* Line 936 of glr.c  */
#line 883 "sql.ypp"
    { ((*yyvalp).intval) = 1; ;}
    break;

  case 142:

/* Line 936 of glr.c  */
#line 884 "sql.ypp"
    { ((*yyvalp).intval) = 2; ;}
    break;

  case 143:

/* Line 936 of glr.c  */
#line 887 "sql.ypp"
    { ((*yyvalp).intval) = 0; ;}
    break;

  case 144:

/* Line 936 of glr.c  */
#line 888 "sql.ypp"
    { ((*yyvalp).intval) = 1; ;}
    break;

  case 145:

/* Line 936 of glr.c  */
#line 891 "sql.ypp"
    { ((*yyvalp).ast_node) = newCreateColList(t_create_col_list, (((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (1))].yystate.yysemantics.yysval.ast_node), NULL); ;}
    break;

  case 146:

/* Line 936 of glr.c  */
#line 892 "sql.ypp"
    { ((*yyvalp).ast_node) = newCreateColList(t_create_col_list, (((yyGLRStackItem const *)yyvsp)[YYFILL ((3) - (3))].yystate.yysemantics.yysval.ast_node), (((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (3))].yystate.yysemantics.yysval.ast_node)); ;}
    break;

  case 147:

/* Line 936 of glr.c  */
#line 896 "sql.ypp"
    { ((*yyvalp).ast_node) = newCreateDef( t_create_def, 1, (((yyGLRStackItem const *)yyvsp)[YYFILL ((2) - (3))].yystate.yysemantics.yysval.ast_node), (((yyGLRStackItem const *)yyvsp)[YYFILL ((3) - (3))].yystate.yysemantics.yysval.ast_node), NULL); ;}
    break;

  case 148:

/* Line 936 of glr.c  */
#line 897 "sql.ypp"
    { ((*yyvalp).ast_node) = newCreateDef( t_create_def, 2, 0, NULL, (((yyGLRStackItem const *)yyvsp)[YYFILL ((4) - (5))].yystate.yysemantics.yysval.ast_node)); ;}
    break;

  case 149:

/* Line 936 of glr.c  */
#line 898 "sql.ypp"
    { ((*yyvalp).ast_node) = newCreateDef( t_create_def, 3, 0, NULL, (((yyGLRStackItem const *)yyvsp)[YYFILL ((3) - (4))].yystate.yysemantics.yysval.ast_node)); ;}
    break;

  case 150:

/* Line 936 of glr.c  */
#line 899 "sql.ypp"
    { ((*yyvalp).ast_node) = newCreateDef( t_create_def, 4, 0, NULL, (((yyGLRStackItem const *)yyvsp)[YYFILL ((3) - (4))].yystate.yysemantics.yysval.ast_node)); ;}
    break;

  case 151:

/* Line 936 of glr.c  */
#line 900 "sql.ypp"
    { ((*yyvalp).ast_node) = newCreateDef( t_create_def, 5, 0, NULL, (((yyGLRStackItem const *)yyvsp)[YYFILL ((4) - (5))].yystate.yysemantics.yysval.ast_node)); ;}
    break;

  case 152:

/* Line 936 of glr.c  */
#line 901 "sql.ypp"
    { ((*yyvalp).ast_node) = newCreateDef( t_create_def, 6, 0, NULL, (((yyGLRStackItem const *)yyvsp)[YYFILL ((4) - (5))].yystate.yysemantics.yysval.ast_node)); ;}
    break;

  case 153:

/* Line 936 of glr.c  */
#line 904 "sql.ypp"
    { ((*yyvalp).ast_node) = newColumnAtts(t_column_atts, 0, 0, 0, NULL, NULL); ;}
    break;

  case 154:

/* Line 936 of glr.c  */
#line 905 "sql.ypp"
    { Column_atts *t = (Column_atts *)(((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (3))].yystate.yysemantics.yysval.ast_node);t->datatype += 01; ((*yyvalp).ast_node) = (Node *)t; ;}
    break;

  case 155:

/* Line 936 of glr.c  */
#line 906 "sql.ypp"
    { Column_atts *t = (Column_atts *)(((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (2))].yystate.yysemantics.yysval.ast_node);t->datatype += 02; ((*yyvalp).ast_node) = (Node *)t; ;}
    break;

  case 156:

/* Line 936 of glr.c  */
#line 907 "sql.ypp"
    {  Column_atts *t = (Column_atts *)(((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (3))].yystate.yysemantics.yysval.ast_node);t->datatype += 04; t->s = (((yyGLRStackItem const *)yyvsp)[YYFILL ((3) - (3))].yystate.yysemantics.yysval.strval); ((*yyvalp).ast_node) = (Node *)t; ;}
    break;

  case 157:

/* Line 936 of glr.c  */
#line 908 "sql.ypp"
    {  Column_atts *t = (Column_atts *)(((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (3))].yystate.yysemantics.yysval.ast_node);t->datatype += 010; t->num1 = (((yyGLRStackItem const *)yyvsp)[YYFILL ((3) - (3))].yystate.yysemantics.yysval.intval); ((*yyvalp).ast_node) = (Node *)t; ;}
    break;

  case 158:

/* Line 936 of glr.c  */
#line 909 "sql.ypp"
    {  Column_atts *t = (Column_atts *)(((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (3))].yystate.yysemantics.yysval.ast_node);t->datatype += 020; t->num2 = (((yyGLRStackItem const *)yyvsp)[YYFILL ((3) - (3))].yystate.yysemantics.yysval.floatval); ((*yyvalp).ast_node) = (Node *)t; ;}
    break;

  case 159:

/* Line 936 of glr.c  */
#line 910 "sql.ypp"
    {  Column_atts *t = (Column_atts *)(((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (3))].yystate.yysemantics.yysval.ast_node);t->datatype += 040; t->num1 = (((yyGLRStackItem const *)yyvsp)[YYFILL ((3) - (3))].yystate.yysemantics.yysval.intval); ((*yyvalp).ast_node) = (Node *)t; ;}
    break;

  case 160:

/* Line 936 of glr.c  */
#line 911 "sql.ypp"
    {  Column_atts *t = (Column_atts *)(((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (2))].yystate.yysemantics.yysval.ast_node);t->datatype += 0100; ((*yyvalp).ast_node) = (Node *)t; ;}
    break;

  case 161:

/* Line 936 of glr.c  */
#line 912 "sql.ypp"
    {  Column_atts *t = (Column_atts *)(((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (3))].yystate.yysemantics.yysval.ast_node);t->datatype += 0200; ((*yyvalp).ast_node) = (Node *)t; ;}
    break;

  case 162:

/* Line 936 of glr.c  */
#line 913 "sql.ypp"
    {  Column_atts *t = (Column_atts *)(((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (3))].yystate.yysemantics.yysval.ast_node);t->datatype += 0400; ((*yyvalp).ast_node) = (Node *)t; ;}
    break;

  case 163:

/* Line 936 of glr.c  */
#line 914 "sql.ypp"
    {  Column_atts *t = (Column_atts *)(((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (2))].yystate.yysemantics.yysval.ast_node);t->datatype += 01000; ((*yyvalp).ast_node) = (Node *)t; ;}
    break;

  case 164:

/* Line 936 of glr.c  */
#line 915 "sql.ypp"
    {  Column_atts *t = (Column_atts *)(((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (3))].yystate.yysemantics.yysval.ast_node);t->datatype += 02000; ((*yyvalp).ast_node) = (Node *)t; ;}
    break;

  case 165:

/* Line 936 of glr.c  */
#line 916 "sql.ypp"
    {  Column_atts *t = (Column_atts *)(((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (5))].yystate.yysemantics.yysval.ast_node);t->datatype += 04000; t->col_list = (((yyGLRStackItem const *)yyvsp)[YYFILL ((4) - (5))].yystate.yysemantics.yysval.ast_node); ((*yyvalp).ast_node) = (Node *)t; ;}
    break;

  case 166:

/* Line 936 of glr.c  */
#line 925 "sql.ypp"
    { ((*yyvalp).ast_node) = newDoStmt(t_do_stmt,(((yyGLRStackItem const *)yyvsp)[YYFILL ((2) - (2))].yystate.yysemantics.yysval.ast_node)); puts("This is a do expr statement"); ;}
    break;

  case 167:

/* Line 936 of glr.c  */
#line 929 "sql.ypp"
    { ((*yyvalp).ast_node) = (((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (1))].yystate.yysemantics.yysval.ast_node); puts("This is a truncate_stmt statement"); ;}
    break;

  case 168:

/* Line 936 of glr.c  */
#line 932 "sql.ypp"
    { ((*yyvalp).ast_node) = newTruncateStmt(t_truncate_stmt, (((yyGLRStackItem const *)yyvsp)[YYFILL ((2) - (2))].yystate.yysemantics.yysval.strval)); ;}
    break;

  case 169:

/* Line 936 of glr.c  */
#line 933 "sql.ypp"
    { ((*yyvalp).ast_node) = newTruncateStmt(t_truncate_stmt, (((yyGLRStackItem const *)yyvsp)[YYFILL ((3) - (3))].yystate.yysemantics.yysval.strval)); ;}
    break;

  case 170:

/* Line 936 of glr.c  */
#line 937 "sql.ypp"
    { ((*yyvalp).ast_node)=(((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (1))].yystate.yysemantics.yysval.ast_node); output(((*yyvalp).ast_node), 1); puts("This is a alter_database statement"); ;}
    break;

  case 171:

/* Line 936 of glr.c  */
#line 941 "sql.ypp"
    { ((*yyvalp).ast_node) = newAlterDBStmt(t_alter_database_stmt, 1, (((yyGLRStackItem const *)yyvsp)[YYFILL ((3) - (4))].yystate.yysemantics.yysval.strval), (((yyGLRStackItem const *)yyvsp)[YYFILL ((4) - (4))].yystate.yysemantics.yysval.ast_node)); ;}
    break;

  case 172:

/* Line 936 of glr.c  */
#line 942 "sql.ypp"
    { ((*yyvalp).ast_node) = newAlterDBStmt(t_alter_database_stmt, 2, (((yyGLRStackItem const *)yyvsp)[YYFILL ((3) - (4))].yystate.yysemantics.yysval.strval), (((yyGLRStackItem const *)yyvsp)[YYFILL ((4) - (4))].yystate.yysemantics.yysval.ast_node)); ;}
    break;

  case 173:

/* Line 936 of glr.c  */
#line 946 "sql.ypp"
    { ((*yyvalp).ast_node)=(((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (1))].yystate.yysemantics.yysval.ast_node); output(((*yyvalp).ast_node), 1); puts("This is a alter_table statement"); ;}
    break;

  case 174:

/* Line 936 of glr.c  */
#line 951 "sql.ypp"
    { ((*yyvalp).ast_node) = newAlterTableStmt(t_alter_table_stmt, (((yyGLRStackItem const *)yyvsp)[YYFILL ((2) - (5))].yystate.yysemantics.yysval.intval), (((yyGLRStackItem const *)yyvsp)[YYFILL ((4) - (5))].yystate.yysemantics.yysval.strval), (((yyGLRStackItem const *)yyvsp)[YYFILL ((5) - (5))].yystate.yysemantics.yysval.ast_node)); ;}
    break;

  case 175:

/* Line 936 of glr.c  */
#line 952 "sql.ypp"
    { ((*yyvalp).ast_node) = newAlterTableStmt(t_alter_table_stmt, (((yyGLRStackItem const *)yyvsp)[YYFILL ((2) - (5))].yystate.yysemantics.yysval.intval), (((yyGLRStackItem const *)yyvsp)[YYFILL ((4) - (5))].yystate.yysemantics.yysval.strval), (((yyGLRStackItem const *)yyvsp)[YYFILL ((5) - (5))].yystate.yysemantics.yysval.ast_node)); ;}
    break;

  case 178:

/* Line 936 of glr.c  */
#line 961 "sql.ypp"
    {;}
    break;

  case 179:

/* Line 936 of glr.c  */
#line 962 "sql.ypp"
    {;}
    break;

  case 184:

/* Line 936 of glr.c  */
#line 969 "sql.ypp"
    { ((*yyvalp).intval) = 0; ;}
    break;

  case 185:

/* Line 936 of glr.c  */
#line 970 "sql.ypp"
    { ((*yyvalp).intval) = 1; ;}
    break;

  case 187:

/* Line 936 of glr.c  */
#line 974 "sql.ypp"
    {;}
    break;

  case 188:

/* Line 936 of glr.c  */
#line 975 "sql.ypp"
    {;}
    break;

  case 190:

/* Line 936 of glr.c  */
#line 979 "sql.ypp"
    { ((*yyvalp).intval) = 0; ;}
    break;

  case 191:

/* Line 936 of glr.c  */
#line 980 "sql.ypp"
    { ((*yyvalp).intval) = 1; ;}
    break;

  case 192:

/* Line 936 of glr.c  */
#line 984 "sql.ypp"
    { emit("STMT"); ;}
    break;

  case 194:

/* Line 936 of glr.c  */
#line 991 "sql.ypp"
    { ((*yyvalp).intval) = 0;;}
    break;

  case 195:

/* Line 936 of glr.c  */
#line 992 "sql.ypp"
    { ((*yyvalp).intval) = 1; ;}
    break;

  case 196:

/* Line 936 of glr.c  */
#line 993 "sql.ypp"
    { ((*yyvalp).intval) = 2; ;}
    break;

  case 197:

/* Line 936 of glr.c  */
#line 994 "sql.ypp"
    { ((*yyvalp).intval) = 3; ;}
    break;

  case 200:

/* Line 936 of glr.c  */
#line 1001 "sql.ypp"
    { ((*yyvalp).intval) = 1; ;}
    break;

  case 201:

/* Line 936 of glr.c  */
#line 1002 "sql.ypp"
    { ((*yyvalp).intval) = 2; ;}
    break;

  case 205:

/* Line 936 of glr.c  */
#line 1013 "sql.ypp"
    { emit("STMT"); ;}
    break;

  case 207:

/* Line 936 of glr.c  */
#line 1020 "sql.ypp"
    { emit("STMT"); ;}
    break;

  case 208:

/* Line 936 of glr.c  */
#line 1024 "sql.ypp"
    { emit("DROPDATABASE %d %s", (((yyGLRStackItem const *)yyvsp)[YYFILL ((3) - (4))].yystate.yysemantics.yysval.intval), (((yyGLRStackItem const *)yyvsp)[YYFILL ((4) - (4))].yystate.yysemantics.yysval.strval)); free((((yyGLRStackItem const *)yyvsp)[YYFILL ((4) - (4))].yystate.yysemantics.yysval.strval)); ;}
    break;

  case 209:

/* Line 936 of glr.c  */
#line 1025 "sql.ypp"
    { emit("DROPSCHEMA %d %s", (((yyGLRStackItem const *)yyvsp)[YYFILL ((3) - (4))].yystate.yysemantics.yysval.intval), (((yyGLRStackItem const *)yyvsp)[YYFILL ((4) - (4))].yystate.yysemantics.yysval.strval)); free((((yyGLRStackItem const *)yyvsp)[YYFILL ((4) - (4))].yystate.yysemantics.yysval.strval)); ;}
    break;

  case 210:

/* Line 936 of glr.c  */
#line 1028 "sql.ypp"
    { ((*yyvalp).intval) = 0; ;}
    break;

  case 211:

/* Line 936 of glr.c  */
#line 1029 "sql.ypp"
    { ((*yyvalp).intval) = 1; ;}
    break;

  case 212:

/* Line 936 of glr.c  */
#line 1033 "sql.ypp"
    { emit("STMT"); ;}
    break;

  case 213:

/* Line 936 of glr.c  */
#line 1036 "sql.ypp"
    { emit("DROP %d %d %d %d", (((yyGLRStackItem const *)yyvsp)[YYFILL ((2) - (6))].yystate.yysemantics.yysval.intval), (((yyGLRStackItem const *)yyvsp)[YYFILL ((4) - (6))].yystate.yysemantics.yysval.intval), (((yyGLRStackItem const *)yyvsp)[YYFILL ((5) - (6))].yystate.yysemantics.yysval.intval), (((yyGLRStackItem const *)yyvsp)[YYFILL ((6) - (6))].yystate.yysemantics.yysval.intval)); ;}
    break;

  case 214:

/* Line 936 of glr.c  */
#line 1039 "sql.ypp"
    { emit("TABLE %s", (((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (1))].yystate.yysemantics.yysval.strval));;}
    break;

  case 215:

/* Line 936 of glr.c  */
#line 1040 "sql.ypp"
    { emit("TABLE %s.%s", (((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (3))].yystate.yysemantics.yysval.strval), (((yyGLRStackItem const *)yyvsp)[YYFILL ((3) - (3))].yystate.yysemantics.yysval.strval)); ;}
    break;

  case 216:

/* Line 936 of glr.c  */
#line 1041 "sql.ypp"
    { emit("TABLE %s", (((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (3))].yystate.yysemantics.yysval.intval));;}
    break;

  case 217:

/* Line 936 of glr.c  */
#line 1042 "sql.ypp"
    { emit("TABLE %s.%s", (((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (5))].yystate.yysemantics.yysval.intval), (((yyGLRStackItem const *)yyvsp)[YYFILL ((3) - (5))].yystate.yysemantics.yysval.strval)); ;}
    break;

  case 218:

/* Line 936 of glr.c  */
#line 1045 "sql.ypp"
    { ((*yyvalp).intval) = 0; ;}
    break;

  case 219:

/* Line 936 of glr.c  */
#line 1046 "sql.ypp"
    { ((*yyvalp).intval) = 1; ;}
    break;

  case 220:

/* Line 936 of glr.c  */
#line 1047 "sql.ypp"
    { ((*yyvalp).intval) = 2; ;}
    break;

  case 221:

/* Line 936 of glr.c  */
#line 1052 "sql.ypp"
    { emit("STMT"); ;}
    break;

  case 222:

/* Line 936 of glr.c  */
#line 1055 "sql.ypp"
    {;}
    break;

  case 224:

/* Line 936 of glr.c  */
#line 1059 "sql.ypp"
    {;}
    break;

  case 225:

/* Line 936 of glr.c  */
#line 1060 "sql.ypp"
    {;}
    break;

  case 226:

/* Line 936 of glr.c  */
#line 1061 "sql.ypp"
    {;}
    break;

  case 227:

/* Line 936 of glr.c  */
#line 1062 "sql.ypp"
    {;}
    break;

  case 228:

/* Line 936 of glr.c  */
#line 1066 "sql.ypp"
    { emit("STMT"); ;}
    break;

  case 229:

/* Line 936 of glr.c  */
#line 1069 "sql.ypp"
    {;}
    break;

  case 230:

/* Line 936 of glr.c  */
#line 1070 "sql.ypp"
    {;}
    break;

  case 231:

/* Line 936 of glr.c  */
#line 1074 "sql.ypp"
    { emit("STMT"); ;}
    break;

  case 232:

/* Line 936 of glr.c  */
#line 1077 "sql.ypp"
    {;}
    break;

  case 233:

/* Line 936 of glr.c  */
#line 1081 "sql.ypp"
    { emit("STMT"); ;}
    break;

  case 234:

/* Line 936 of glr.c  */
#line 1082 "sql.ypp"
    { emit("STMT"); ;}
    break;

  case 235:

/* Line 936 of glr.c  */
#line 1083 "sql.ypp"
    { emit("STMT"); ;}
    break;

  case 240:

/* Line 936 of glr.c  */
#line 1094 "sql.ypp"
    { emit("STMT"); ;}
    break;

  case 241:

/* Line 936 of glr.c  */
#line 1097 "sql.ypp"
    { ((*yyvalp).intval) = 0; ;}
    break;

  case 242:

/* Line 936 of glr.c  */
#line 1098 "sql.ypp"
    { ((*yyvalp).intval) = 1; ;}
    break;

  case 243:

/* Line 936 of glr.c  */
#line 1099 "sql.ypp"
    { ((*yyvalp).intval) = 2; ;}
    break;

  case 244:

/* Line 936 of glr.c  */
#line 1102 "sql.ypp"
    { ((*yyvalp).intval) = 0; ;}
    break;

  case 245:

/* Line 936 of glr.c  */
#line 1103 "sql.ypp"
    { ((*yyvalp).intval) = 1; ;}
    break;

  case 246:

/* Line 936 of glr.c  */
#line 1104 "sql.ypp"
    { ((*yyvalp).intval) = 2; ;}
    break;

  case 247:

/* Line 936 of glr.c  */
#line 1107 "sql.ypp"
    { emit("STMT"); ;}
    break;

  case 248:

/* Line 936 of glr.c  */
#line 1111 "sql.ypp"
    { emit("STMT"); ;}
    break;

  case 256:

/* Line 936 of glr.c  */
#line 1134 "sql.ypp"
    { emit("STMT"); ;}
    break;

  case 261:

/* Line 936 of glr.c  */
#line 1147 "sql.ypp"
    { ((*yyvalp).intval) = 1; ;}
    break;

  case 262:

/* Line 936 of glr.c  */
#line 1148 "sql.ypp"
    { ((*yyvalp).intval) = 2; ;}
    break;

  case 263:

/* Line 936 of glr.c  */
#line 1149 "sql.ypp"
    { ((*yyvalp).intval) = 3; ;}
    break;

  case 264:

/* Line 936 of glr.c  */
#line 1150 "sql.ypp"
    { ((*yyvalp).intval) = 4; ;}
    break;

  case 269:

/* Line 936 of glr.c  */
#line 1168 "sql.ypp"
    { ((*yyvalp).intval) = 0; ;}
    break;

  case 270:

/* Line 936 of glr.c  */
#line 1169 "sql.ypp"
    { ((*yyvalp).intval) = 1; ;}
    break;

  case 271:

/* Line 936 of glr.c  */
#line 1170 "sql.ypp"
    { ((*yyvalp).intval) = 2; ;}
    break;

  case 272:

/* Line 936 of glr.c  */
#line 1174 "sql.ypp"
    { ((*yyvalp).intval) = 1; ;}
    break;

  case 273:

/* Line 936 of glr.c  */
#line 1175 "sql.ypp"
    { ((*yyvalp).intval) = 2; ;}
    break;

  case 274:

/* Line 936 of glr.c  */
#line 1176 "sql.ypp"
    { ((*yyvalp).intval) = 3; ;}
    break;

  case 275:

/* Line 936 of glr.c  */
#line 1177 "sql.ypp"
    { ((*yyvalp).intval) = 4; ;}
    break;

  case 276:

/* Line 936 of glr.c  */
#line 1181 "sql.ypp"
    { emit("STMT"); ;}
    break;

  case 283:

/* Line 936 of glr.c  */
#line 1197 "sql.ypp"
    { emit("STMT"); ;}
    break;

  case 287:

/* Line 936 of glr.c  */
#line 1208 "sql.ypp"
    { emit("STMT"); ;}
    break;

  case 291:

/* Line 936 of glr.c  */
#line 1219 "sql.ypp"
    { ((*yyvalp).intval) = 01; ;}
    break;

  case 292:

/* Line 936 of glr.c  */
#line 1220 "sql.ypp"
    { ((*yyvalp).intval) = 02; ;}
    break;

  case 293:

/* Line 936 of glr.c  */
#line 1221 "sql.ypp"
    { ((*yyvalp).intval) = 04; ;}
    break;

  case 294:

/* Line 936 of glr.c  */
#line 1222 "sql.ypp"
    { ((*yyvalp).intval) = 010; ;}
    break;

  case 295:

/* Line 936 of glr.c  */
#line 1223 "sql.ypp"
    { ((*yyvalp).intval) = 020; ;}
    break;

  case 296:

/* Line 936 of glr.c  */
#line 1224 "sql.ypp"
    { ((*yyvalp).intval) = 040; ;}
    break;

  case 297:

/* Line 936 of glr.c  */
#line 1225 "sql.ypp"
    { ((*yyvalp).intval) = 0100; ;}
    break;

  case 298:

/* Line 936 of glr.c  */
#line 1226 "sql.ypp"
    { ((*yyvalp).intval) = 0200; ;}
    break;

  case 299:

/* Line 936 of glr.c  */
#line 1227 "sql.ypp"
    { ((*yyvalp).intval) = 0400; ;}
    break;

  case 300:

/* Line 936 of glr.c  */
#line 1228 "sql.ypp"
    { ((*yyvalp).intval) = 01000; ;}
    break;

  case 301:

/* Line 936 of glr.c  */
#line 1229 "sql.ypp"
    { ((*yyvalp).intval) = 02000; ;}
    break;

  case 302:

/* Line 936 of glr.c  */
#line 1230 "sql.ypp"
    { ((*yyvalp).intval) = 04000; ;}
    break;

  case 310:

/* Line 936 of glr.c  */
#line 1246 "sql.ypp"
    { emit("STMT"); ;}
    break;

  case 312:

/* Line 936 of glr.c  */
#line 1253 "sql.ypp"
    { emit("STMT"); ;}
    break;

  case 325:

/* Line 936 of glr.c  */
#line 1282 "sql.ypp"
    { emit("STMT"); ;}
    break;

  case 327:

/* Line 936 of glr.c  */
#line 1288 "sql.ypp"
    { emit("STMT"); ;}
    break;

  case 329:

/* Line 936 of glr.c  */
#line 1294 "sql.ypp"
    { emit("STMT"); ;}
    break;

  case 331:

/* Line 936 of glr.c  */
#line 1301 "sql.ypp"
    { emit("STMT"); ;}
    break;

  case 335:

/* Line 936 of glr.c  */
#line 1313 "sql.ypp"
    { emit("STMT"); ;}
    break;

  case 339:

/* Line 936 of glr.c  */
#line 1324 "sql.ypp"
    { emit("STMT"); ;}
    break;

  case 341:

/* Line 936 of glr.c  */
#line 1330 "sql.ypp"
    { ((*yyvalp).intval) = 0; ;}
    break;

  case 342:

/* Line 936 of glr.c  */
#line 1331 "sql.ypp"
    { ((*yyvalp).intval) = 1; ;}
    break;

  case 343:

/* Line 936 of glr.c  */
#line 1332 "sql.ypp"
    { ((*yyvalp).intval) = 2; ;}
    break;

  case 344:

/* Line 936 of glr.c  */
#line 1336 "sql.ypp"
    { emit("STMT"); ;}
    break;

  case 346:

/* Line 936 of glr.c  */
#line 1343 "sql.ypp"
    { emit("STMT"); ;}
    break;

  case 348:

/* Line 936 of glr.c  */
#line 1349 "sql.ypp"
    { ((*yyvalp).intval) = 0; ;}
    break;

  case 349:

/* Line 936 of glr.c  */
#line 1350 "sql.ypp"
    { ((*yyvalp).intval) = 1; ;}
    break;

  case 350:

/* Line 936 of glr.c  */
#line 1351 "sql.ypp"
    { ((*yyvalp).intval) = 2; ;}
    break;

  case 351:

/* Line 936 of glr.c  */
#line 1352 "sql.ypp"
    { ((*yyvalp).intval) = 3; ;}
    break;

  case 352:

/* Line 936 of glr.c  */
#line 1353 "sql.ypp"
    { ((*yyvalp).intval) = 4; ;}
    break;

  case 353:

/* Line 936 of glr.c  */
#line 1354 "sql.ypp"
    { ((*yyvalp).intval) = 5; ;}
    break;

  case 354:

/* Line 936 of glr.c  */
#line 1358 "sql.ypp"
    { emit("STMT"); ;}
    break;

  case 356:

/* Line 936 of glr.c  */
#line 1364 "sql.ypp"
    { ((*yyvalp).intval) = 0; ;}
    break;

  case 357:

/* Line 936 of glr.c  */
#line 1365 "sql.ypp"
    { ((*yyvalp).intval) = 1; ;}
    break;

  case 358:

/* Line 936 of glr.c  */
#line 1366 "sql.ypp"
    { ((*yyvalp).intval) = 2; ;}
    break;

  case 359:

/* Line 936 of glr.c  */
#line 1370 "sql.ypp"
    { emit("STMT"); ;}
    break;

  case 361:

/* Line 936 of glr.c  */
#line 1377 "sql.ypp"
    { emit("STMT"); ;}
    break;

  case 367:

/* Line 936 of glr.c  */
#line 1390 "sql.ypp"
    { emit("STMT"); ;}
    break;

  case 397:

/* Line 936 of glr.c  */
#line 1429 "sql.ypp"
    { ((*yyvalp).intval) = 0; ;}
    break;

  case 398:

/* Line 936 of glr.c  */
#line 1430 "sql.ypp"
    { ((*yyvalp).intval) = 1; ;}
    break;

  case 413:

/* Line 936 of glr.c  */
#line 1464 "sql.ypp"
    { ((*yyvalp).intval) = 0; ;}
    break;

  case 414:

/* Line 936 of glr.c  */
#line 1465 "sql.ypp"
    { ((*yyvalp).intval) = 1; ;}
    break;

  case 415:

/* Line 936 of glr.c  */
#line 1466 "sql.ypp"
    { ((*yyvalp).intval) = 2; ;}
    break;

  case 420:

/* Line 936 of glr.c  */
#line 1480 "sql.ypp"
    { ((*yyvalp).intval) = 1; ;}
    break;

  case 421:

/* Line 936 of glr.c  */
#line 1481 "sql.ypp"
    { ((*yyvalp).intval) = 2; ;}
    break;

  case 422:

/* Line 936 of glr.c  */
#line 1482 "sql.ypp"
    { ((*yyvalp).intval) = 04; ;}
    break;

  case 423:

/* Line 936 of glr.c  */
#line 1483 "sql.ypp"
    { ((*yyvalp).intval) = 010; ;}
    break;

  case 424:

/* Line 936 of glr.c  */
#line 1484 "sql.ypp"
    { ((*yyvalp).intval) = 020; ;}
    break;

  case 425:

/* Line 936 of glr.c  */
#line 1485 "sql.ypp"
    { ((*yyvalp).intval) = 040; ;}
    break;

  case 426:

/* Line 936 of glr.c  */
#line 1486 "sql.ypp"
    { ((*yyvalp).intval) = 0100; ;}
    break;

  case 427:

/* Line 936 of glr.c  */
#line 1487 "sql.ypp"
    { ((*yyvalp).intval) = 0200; ;}
    break;

  case 428:

/* Line 936 of glr.c  */
#line 1488 "sql.ypp"
    { ((*yyvalp).intval) = 0400; ;}
    break;

  case 429:

/* Line 936 of glr.c  */
#line 1489 "sql.ypp"
    { ((*yyvalp).intval) = 01000; ;}
    break;

  case 432:

/* Line 936 of glr.c  */
#line 1499 "sql.ypp"
    { ((*yyvalp).intval) = 0; ;}
    break;

  case 433:

/* Line 936 of glr.c  */
#line 1500 "sql.ypp"
    { ((*yyvalp).intval) = 1; ;}
    break;

  case 434:

/* Line 936 of glr.c  */
#line 1501 "sql.ypp"
    { ((*yyvalp).intval) = 2; ;}
    break;

  case 445:

/* Line 936 of glr.c  */
#line 1532 "sql.ypp"
    { ((*yyvalp).intval) = 1; ;}
    break;

  case 446:

/* Line 936 of glr.c  */
#line 1533 "sql.ypp"
    { ((*yyvalp).intval) = 2; ;}
    break;

  case 447:

/* Line 936 of glr.c  */
#line 1534 "sql.ypp"
    { ((*yyvalp).intval) = 3; ;}
    break;

  case 456:

/* Line 936 of glr.c  */
#line 1559 "sql.ypp"
    { emit("STMT"); ;}
    break;

  case 460:

/* Line 936 of glr.c  */
#line 1568 "sql.ypp"
    { if ((((yyGLRStackItem const *)yyvsp)[YYFILL ((2) - (3))].yystate.yysemantics.yysval.subtok) != 4) yyerror("bad set to @%s", (((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (3))].yystate.yysemantics.yysval.strval));
		emit("SET %s", (((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (3))].yystate.yysemantics.yysval.strval)); free((((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (3))].yystate.yysemantics.yysval.strval));
	;}
    break;

  case 461:

/* Line 936 of glr.c  */
#line 1572 "sql.ypp"
    {
    	emit("SET %s", (((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (3))].yystate.yysemantics.yysval.strval)); 
    	free((((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (3))].yystate.yysemantics.yysval.strval)); 
    ;}
    break;

  case 462:

/* Line 936 of glr.c  */
#line 1579 "sql.ypp"
    { ((*yyvalp).ast_node) = NULL; ;}
    break;

  case 463:

/* Line 936 of glr.c  */
#line 1580 "sql.ypp"
    { ((*yyvalp).ast_node) = newLength (t_length, (((yyGLRStackItem const *)yyvsp)[YYFILL ((2) - (3))].yystate.yysemantics.yysval.intval), 0); ;}
    break;

  case 464:

/* Line 936 of glr.c  */
#line 1581 "sql.ypp"
    { ((*yyvalp).ast_node) = newLength (t_length, (((yyGLRStackItem const *)yyvsp)[YYFILL ((2) - (5))].yystate.yysemantics.yysval.intval), (((yyGLRStackItem const *)yyvsp)[YYFILL ((4) - (5))].yystate.yysemantics.yysval.intval)); ;}
    break;

  case 465:

/* Line 936 of glr.c  */
#line 1584 "sql.ypp"
    { ((*yyvalp).intval) = 0; ;}
    break;

  case 466:

/* Line 936 of glr.c  */
#line 1585 "sql.ypp"
    { ((*yyvalp).intval) = 1; ;}
    break;

  case 467:

/* Line 936 of glr.c  */
#line 1588 "sql.ypp"
    { ((*yyvalp).intval) = 0; ;}
    break;

  case 468:

/* Line 936 of glr.c  */
#line 1589 "sql.ypp"
    { ((*yyvalp).intval) = (((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (2))].yystate.yysemantics.yysval.intval) | 01; ;}
    break;

  case 469:

/* Line 936 of glr.c  */
#line 1590 "sql.ypp"
    { ((*yyvalp).intval) = (((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (2))].yystate.yysemantics.yysval.intval) | 02; ;}
    break;

  case 470:

/* Line 936 of glr.c  */
#line 1593 "sql.ypp"
    { ((*yyvalp).ast_node) =newOptCsc(t_opt_csc, 0, NULL, NULL);;}
    break;

  case 471:

/* Line 936 of glr.c  */
#line 1595 "sql.ypp"
    { Opt_csc * t = (Opt_csc *)(((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (4))].yystate.yysemantics.yysval.ast_node); t->datatype = t->datatype | 01; t->s1 = (((yyGLRStackItem const *)yyvsp)[YYFILL ((4) - (4))].yystate.yysemantics.yysval.strval); ((*yyvalp).ast_node) = (Node *)t; ;}
    break;

  case 472:

/* Line 936 of glr.c  */
#line 1597 "sql.ypp"
    { Opt_csc * t = (Opt_csc *)(((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (3))].yystate.yysemantics.yysval.ast_node); t->datatype = t->datatype | 02; t->s2 = (((yyGLRStackItem const *)yyvsp)[YYFILL ((3) - (3))].yystate.yysemantics.yysval.strval); ((*yyvalp).ast_node) = (Node *)t; ;}
    break;

  case 473:

/* Line 936 of glr.c  */
#line 1601 "sql.ypp"
    { ((*yyvalp).ast_node) = newDatatype (t_datatype, 01, (((yyGLRStackItem const *)yyvsp)[YYFILL ((2) - (2))].yystate.yysemantics.yysval.ast_node), 0, NULL, 0, NULL); ;}
    break;

  case 474:

/* Line 936 of glr.c  */
#line 1602 "sql.ypp"
    { ((*yyvalp).ast_node) = newDatatype (t_datatype, 02, (((yyGLRStackItem const *)yyvsp)[YYFILL ((2) - (3))].yystate.yysemantics.yysval.ast_node), (((yyGLRStackItem const *)yyvsp)[YYFILL ((3) - (3))].yystate.yysemantics.yysval.intval), NULL, 0, NULL); ;}
    break;

  case 475:

/* Line 936 of glr.c  */
#line 1603 "sql.ypp"
    { ((*yyvalp).ast_node) = newDatatype (t_datatype, 04, (((yyGLRStackItem const *)yyvsp)[YYFILL ((2) - (3))].yystate.yysemantics.yysval.ast_node), (((yyGLRStackItem const *)yyvsp)[YYFILL ((3) - (3))].yystate.yysemantics.yysval.intval), NULL, 0, NULL); ;}
    break;

  case 476:

/* Line 936 of glr.c  */
#line 1604 "sql.ypp"
    { ((*yyvalp).ast_node) = newDatatype (t_datatype, 010, (((yyGLRStackItem const *)yyvsp)[YYFILL ((2) - (3))].yystate.yysemantics.yysval.ast_node), (((yyGLRStackItem const *)yyvsp)[YYFILL ((3) - (3))].yystate.yysemantics.yysval.intval), NULL, 0, NULL); ;}
    break;

  case 477:

/* Line 936 of glr.c  */
#line 1605 "sql.ypp"
    { ((*yyvalp).ast_node) = newDatatype (t_datatype, 020, (((yyGLRStackItem const *)yyvsp)[YYFILL ((2) - (3))].yystate.yysemantics.yysval.ast_node), (((yyGLRStackItem const *)yyvsp)[YYFILL ((3) - (3))].yystate.yysemantics.yysval.intval), NULL, 0, NULL); ;}
    break;

  case 478:

/* Line 936 of glr.c  */
#line 1606 "sql.ypp"
    { ((*yyvalp).ast_node) = newDatatype (t_datatype, 040, (((yyGLRStackItem const *)yyvsp)[YYFILL ((2) - (3))].yystate.yysemantics.yysval.ast_node), (((yyGLRStackItem const *)yyvsp)[YYFILL ((3) - (3))].yystate.yysemantics.yysval.intval), NULL, 0, NULL); ;}
    break;

  case 479:

/* Line 936 of glr.c  */
#line 1607 "sql.ypp"
    { ((*yyvalp).ast_node) = newDatatype (t_datatype, 0100, (((yyGLRStackItem const *)yyvsp)[YYFILL ((2) - (3))].yystate.yysemantics.yysval.ast_node), (((yyGLRStackItem const *)yyvsp)[YYFILL ((3) - (3))].yystate.yysemantics.yysval.intval), NULL, 0, NULL); ;}
    break;

  case 480:

/* Line 936 of glr.c  */
#line 1608 "sql.ypp"
    { ((*yyvalp).ast_node) = newDatatype (t_datatype, 0200, (((yyGLRStackItem const *)yyvsp)[YYFILL ((2) - (3))].yystate.yysemantics.yysval.ast_node), (((yyGLRStackItem const *)yyvsp)[YYFILL ((3) - (3))].yystate.yysemantics.yysval.intval), NULL, 0, NULL); ;}
    break;

  case 481:

/* Line 936 of glr.c  */
#line 1609 "sql.ypp"
    { ((*yyvalp).ast_node) = newDatatype (t_datatype, 0400, (((yyGLRStackItem const *)yyvsp)[YYFILL ((2) - (3))].yystate.yysemantics.yysval.ast_node), (((yyGLRStackItem const *)yyvsp)[YYFILL ((3) - (3))].yystate.yysemantics.yysval.intval), NULL, 0, NULL); ;}
    break;

  case 482:

/* Line 936 of glr.c  */
#line 1610 "sql.ypp"
    { ((*yyvalp).ast_node) = newDatatype (t_datatype, 01000, (((yyGLRStackItem const *)yyvsp)[YYFILL ((2) - (3))].yystate.yysemantics.yysval.ast_node), (((yyGLRStackItem const *)yyvsp)[YYFILL ((3) - (3))].yystate.yysemantics.yysval.intval), NULL, 0, NULL); ;}
    break;

  case 483:

/* Line 936 of glr.c  */
#line 1611 "sql.ypp"
    { ((*yyvalp).ast_node) = newDatatype (t_datatype, 02000, (((yyGLRStackItem const *)yyvsp)[YYFILL ((2) - (3))].yystate.yysemantics.yysval.ast_node), (((yyGLRStackItem const *)yyvsp)[YYFILL ((3) - (3))].yystate.yysemantics.yysval.intval), NULL, 0, NULL); ;}
    break;

  case 484:

/* Line 936 of glr.c  */
#line 1612 "sql.ypp"
    { ((*yyvalp).ast_node) = newDatatype (t_datatype, 04000, NULL, 0, NULL, 0, NULL); ;}
    break;

  case 485:

/* Line 936 of glr.c  */
#line 1613 "sql.ypp"
    { ((*yyvalp).ast_node) = newDatatype (t_datatype, 010000, NULL, 0, NULL, 0, NULL); ;}
    break;

  case 486:

/* Line 936 of glr.c  */
#line 1614 "sql.ypp"
    { ((*yyvalp).ast_node) = newDatatype (t_datatype, 020000, NULL, 0, NULL, 0, NULL); ;}
    break;

  case 487:

/* Line 936 of glr.c  */
#line 1615 "sql.ypp"
    { ((*yyvalp).ast_node) = newDatatype (t_datatype, 040000, NULL, 0, NULL, 0, NULL); ;}
    break;

  case 488:

/* Line 936 of glr.c  */
#line 1616 "sql.ypp"
    { ((*yyvalp).ast_node) = newDatatype (t_datatype, 0100000, NULL, 0, NULL, 0, NULL); ;}
    break;

  case 489:

/* Line 936 of glr.c  */
#line 1617 "sql.ypp"
    { ((*yyvalp).ast_node) = newDatatype (t_datatype, 0200000, (((yyGLRStackItem const *)yyvsp)[YYFILL ((2) - (3))].yystate.yysemantics.yysval.ast_node), 0, (((yyGLRStackItem const *)yyvsp)[YYFILL ((3) - (3))].yystate.yysemantics.yysval.ast_node), 0, NULL); ;}
    break;

  case 490:

/* Line 936 of glr.c  */
#line 1618 "sql.ypp"
    { Node * t = newLength(t_datatype, (((yyGLRStackItem const *)yyvsp)[YYFILL ((3) - (5))].yystate.yysemantics.yysval.intval), NULL);
					((*yyvalp).ast_node) = newDatatype (t_datatype, 0400000, t, 0, (((yyGLRStackItem const *)yyvsp)[YYFILL ((5) - (5))].yystate.yysemantics.yysval.ast_node), 0, NULL); ;}
    break;

  case 491:

/* Line 936 of glr.c  */
#line 1620 "sql.ypp"
    { ((*yyvalp).ast_node) = newDatatype (t_datatype, 01000000, (((yyGLRStackItem const *)yyvsp)[YYFILL ((2) - (2))].yystate.yysemantics.yysval.ast_node), 0, NULL, 0, NULL); ;}
    break;

  case 492:

/* Line 936 of glr.c  */
#line 1621 "sql.ypp"
    {  Node * t = newLength(t_datatype, (((yyGLRStackItem const *)yyvsp)[YYFILL ((3) - (4))].yystate.yysemantics.yysval.intval), NULL);
				((*yyvalp).ast_node) = newDatatype (t_datatype, 02000000, NULL, 0, NULL, 0, NULL); ;}
    break;

  case 493:

/* Line 936 of glr.c  */
#line 1623 "sql.ypp"
    { ((*yyvalp).ast_node) = newDatatype (t_datatype, 04000000, NULL, 0, NULL, 0, NULL); ;}
    break;

  case 494:

/* Line 936 of glr.c  */
#line 1624 "sql.ypp"
    { ((*yyvalp).ast_node) = newDatatype (t_datatype, 010000000, NULL, 0, NULL, 0, NULL); ;}
    break;

  case 495:

/* Line 936 of glr.c  */
#line 1625 "sql.ypp"
    { ((*yyvalp).ast_node) = newDatatype (t_datatype, 020000000, NULL, 0, NULL, 0, NULL); ;}
    break;

  case 496:

/* Line 936 of glr.c  */
#line 1626 "sql.ypp"
    { ((*yyvalp).ast_node) = newDatatype (t_datatype, 040000000, NULL, 0, NULL, 0, NULL); ;}
    break;

  case 497:

/* Line 936 of glr.c  */
#line 1627 "sql.ypp"
    { ((*yyvalp).ast_node) = newDatatype (t_datatype, 0100000000, NULL, 0, (((yyGLRStackItem const *)yyvsp)[YYFILL ((3) - (3))].yystate.yysemantics.yysval.ast_node), (((yyGLRStackItem const *)yyvsp)[YYFILL ((2) - (3))].yystate.yysemantics.yysval.intval), NULL); ;}
    break;

  case 498:

/* Line 936 of glr.c  */
#line 1628 "sql.ypp"
    { ((*yyvalp).ast_node) = newDatatype (t_datatype, 0200000000, NULL, 0, (((yyGLRStackItem const *)yyvsp)[YYFILL ((3) - (3))].yystate.yysemantics.yysval.ast_node), (((yyGLRStackItem const *)yyvsp)[YYFILL ((2) - (3))].yystate.yysemantics.yysval.intval), NULL); ;}
    break;

  case 499:

/* Line 936 of glr.c  */
#line 1629 "sql.ypp"
    { ((*yyvalp).ast_node) = newDatatype (t_datatype, 0400000000, NULL,0, (((yyGLRStackItem const *)yyvsp)[YYFILL ((3) - (3))].yystate.yysemantics.yysval.ast_node), (((yyGLRStackItem const *)yyvsp)[YYFILL ((2) - (3))].yystate.yysemantics.yysval.intval), NULL); ;}
    break;

  case 500:

/* Line 936 of glr.c  */
#line 1630 "sql.ypp"
    { ((*yyvalp).ast_node) = newDatatype (t_datatype, 01000000000,NULL,0, (((yyGLRStackItem const *)yyvsp)[YYFILL ((3) - (3))].yystate.yysemantics.yysval.ast_node), (((yyGLRStackItem const *)yyvsp)[YYFILL ((2) - (3))].yystate.yysemantics.yysval.intval), NULL); ;}
    break;

  case 501:

/* Line 936 of glr.c  */
#line 1631 "sql.ypp"
    { ((*yyvalp).ast_node) = newDatatype (t_datatype, 02000000000, NULL, 0, (((yyGLRStackItem const *)yyvsp)[YYFILL ((5) - (5))].yystate.yysemantics.yysval.ast_node), 0, (((yyGLRStackItem const *)yyvsp)[YYFILL ((3) - (5))].yystate.yysemantics.yysval.ast_node)); ;}
    break;

  case 502:

/* Line 936 of glr.c  */
#line 1632 "sql.ypp"
    { ((*yyvalp).ast_node) = newDatatype (t_datatype, 04000000000, NULL, 0, (((yyGLRStackItem const *)yyvsp)[YYFILL ((5) - (5))].yystate.yysemantics.yysval.ast_node), 0, (((yyGLRStackItem const *)yyvsp)[YYFILL ((3) - (5))].yystate.yysemantics.yysval.ast_node)); ;}
    break;

  case 503:

/* Line 936 of glr.c  */
#line 1635 "sql.ypp"
    { ((*yyvalp).ast_node) = newEnumList( t_enum_list, (((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (1))].yystate.yysemantics.yysval.strval), NULL); ;}
    break;

  case 504:

/* Line 936 of glr.c  */
#line 1636 "sql.ypp"
    { ((*yyvalp).ast_node) = newEnumList( t_enum_list, (((yyGLRStackItem const *)yyvsp)[YYFILL ((3) - (3))].yystate.yysemantics.yysval.strval), (((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (3))].yystate.yysemantics.yysval.ast_node)); ;}
    break;

  case 505:

/* Line 936 of glr.c  */
#line 1642 "sql.ypp"
    { dataval d; d.string_val = (((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (1))].yystate.yysemantics.yysval.strval); ((*yyvalp).ast_node) = newExpr(t_name, d);  puts("expr1");;}
    break;

  case 506:

/* Line 936 of glr.c  */
#line 1643 "sql.ypp"
    { dataval d; d.string_val = (((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (1))].yystate.yysemantics.yysval.strval); ((*yyvalp).ast_node) = newExpr(t_name, d); ;}
    break;

  case 507:

/* Line 936 of glr.c  */
#line 1644 "sql.ypp"
    { ((*yyvalp).ast_node) = newColumn(t_name_name, (((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (3))].yystate.yysemantics.yysval.strval), (((yyGLRStackItem const *)yyvsp)[YYFILL ((3) - (3))].yystate.yysemantics.yysval.strval), NULL); puts("expr2");;}
    break;

  case 508:

/* Line 936 of glr.c  */
#line 1645 "sql.ypp"
    { dataval d; d.string_val = (((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (1))].yystate.yysemantics.yysval.strval); ((*yyvalp).ast_node) = newExpr(t_stringval, d); puts("expr3");;}
    break;

  case 509:

/* Line 936 of glr.c  */
#line 1646 "sql.ypp"
    { dataval d; d.int_val=(((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (1))].yystate.yysemantics.yysval.intval); ((*yyvalp).ast_node) = newExpr(t_intnum, d); puts("expr4");;}
    break;

  case 510:

/* Line 936 of glr.c  */
#line 1647 "sql.ypp"
    { dataval d; d.double_val = (((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (1))].yystate.yysemantics.yysval.floatval); ((*yyvalp).ast_node) = newExpr(t_approxnum, d); ;}
    break;

  case 511:

/* Line 936 of glr.c  */
#line 1648 "sql.ypp"
    { dataval d; d.bool_val = (((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (1))].yystate.yysemantics.yysval.intval); ((*yyvalp).ast_node) = newExpr(t_bool, d); ;}
    break;

  case 512:

/* Line 936 of glr.c  */
#line 1651 "sql.ypp"
    { ((*yyvalp).ast_node)=newExprCal(t_expr_cal, "+", NULL, 0, (((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (3))].yystate.yysemantics.yysval.ast_node), (((yyGLRStackItem const *)yyvsp)[YYFILL ((3) - (3))].yystate.yysemantics.yysval.ast_node)); ;}
    break;

  case 513:

/* Line 936 of glr.c  */
#line 1652 "sql.ypp"
    { ((*yyvalp).ast_node)=newExprCal(t_expr_cal, "-", NULL, 0, (((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (3))].yystate.yysemantics.yysval.ast_node), (((yyGLRStackItem const *)yyvsp)[YYFILL ((3) - (3))].yystate.yysemantics.yysval.ast_node)); ;}
    break;

  case 514:

/* Line 936 of glr.c  */
#line 1653 "sql.ypp"
    { ((*yyvalp).ast_node)=newExprCal(t_expr_cal, "*", NULL, 0, (((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (3))].yystate.yysemantics.yysval.ast_node), (((yyGLRStackItem const *)yyvsp)[YYFILL ((3) - (3))].yystate.yysemantics.yysval.ast_node)); ;}
    break;

  case 515:

/* Line 936 of glr.c  */
#line 1654 "sql.ypp"
    { ((*yyvalp).ast_node)=newExprCal(t_expr_cal, "/", NULL, 0, (((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (3))].yystate.yysemantics.yysval.ast_node), (((yyGLRStackItem const *)yyvsp)[YYFILL ((3) - (3))].yystate.yysemantics.yysval.ast_node)); ;}
    break;

  case 516:

/* Line 936 of glr.c  */
#line 1655 "sql.ypp"
    { ((*yyvalp).ast_node)=newExprCal(t_expr_cal, "%", NULL, 0, (((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (3))].yystate.yysemantics.yysval.ast_node), (((yyGLRStackItem const *)yyvsp)[YYFILL ((3) - (3))].yystate.yysemantics.yysval.ast_node)); ;}
    break;

  case 517:

/* Line 936 of glr.c  */
#line 1656 "sql.ypp"
    { ((*yyvalp).ast_node)=newExprCal(t_expr_cal, "MOD", NULL, 0, (((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (3))].yystate.yysemantics.yysval.ast_node), (((yyGLRStackItem const *)yyvsp)[YYFILL ((3) - (3))].yystate.yysemantics.yysval.ast_node)); ;}
    break;

  case 518:

/* Line 936 of glr.c  */
#line 1657 "sql.ypp"
    { ((*yyvalp).ast_node)=newExprCal(t_expr_cal, "--", NULL, 0, NULL, (((yyGLRStackItem const *)yyvsp)[YYFILL ((2) - (2))].yystate.yysemantics.yysval.ast_node)); ;}
    break;

  case 519:

/* Line 936 of glr.c  */
#line 1658 "sql.ypp"
    { ((*yyvalp).ast_node)=newExprCal(t_expr_cal, "ANDOP", NULL, 0, (((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (3))].yystate.yysemantics.yysval.ast_node), (((yyGLRStackItem const *)yyvsp)[YYFILL ((3) - (3))].yystate.yysemantics.yysval.ast_node)); ;}
    break;

  case 520:

/* Line 936 of glr.c  */
#line 1659 "sql.ypp"
    { ((*yyvalp).ast_node)=newExprCal(t_expr_cal, "OR", NULL, 0, (((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (3))].yystate.yysemantics.yysval.ast_node), (((yyGLRStackItem const *)yyvsp)[YYFILL ((3) - (3))].yystate.yysemantics.yysval.ast_node)); ;}
    break;

  case 521:

/* Line 936 of glr.c  */
#line 1660 "sql.ypp"
    { ((*yyvalp).ast_node)=newExprCal(t_expr_cal, "XOR", NULL, 0, (((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (3))].yystate.yysemantics.yysval.ast_node), (((yyGLRStackItem const *)yyvsp)[YYFILL ((3) - (3))].yystate.yysemantics.yysval.ast_node)); ;}
    break;

  case 522:

/* Line 936 of glr.c  */
#line 1661 "sql.ypp"
    { ((*yyvalp).ast_node) = newExprCal(t_expr_cal, "CMP", NULL, (((yyGLRStackItem const *)yyvsp)[YYFILL ((2) - (3))].yystate.yysemantics.yysval.subtok), (((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (3))].yystate.yysemantics.yysval.ast_node), (((yyGLRStackItem const *)yyvsp)[YYFILL ((3) - (3))].yystate.yysemantics.yysval.ast_node));/* puts("=");*/ ;}
    break;

  case 523:

/* Line 936 of glr.c  */
#line 1662 "sql.ypp"
    { ((*yyvalp).ast_node)=newExprCal(t_expr_cal, "CMP", NULL, (((yyGLRStackItem const *)yyvsp)[YYFILL ((2) - (5))].yystate.yysemantics.yysval.subtok), (((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (5))].yystate.yysemantics.yysval.ast_node), (((yyGLRStackItem const *)yyvsp)[YYFILL ((4) - (5))].yystate.yysemantics.yysval.ast_node)); ;}
    break;

  case 524:

/* Line 936 of glr.c  */
#line 1663 "sql.ypp"
    { ((*yyvalp).ast_node)=newExprCal(t_expr_cal, "CMP", "ANY", (((yyGLRStackItem const *)yyvsp)[YYFILL ((2) - (6))].yystate.yysemantics.yysval.subtok), (((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (6))].yystate.yysemantics.yysval.ast_node), (((yyGLRStackItem const *)yyvsp)[YYFILL ((5) - (6))].yystate.yysemantics.yysval.ast_node)); ;}
    break;

  case 525:

/* Line 936 of glr.c  */
#line 1664 "sql.ypp"
    { ((*yyvalp).ast_node)=newExprCal(t_expr_cal, "CMP", "SOME", (((yyGLRStackItem const *)yyvsp)[YYFILL ((2) - (6))].yystate.yysemantics.yysval.subtok), (((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (6))].yystate.yysemantics.yysval.ast_node), (((yyGLRStackItem const *)yyvsp)[YYFILL ((5) - (6))].yystate.yysemantics.yysval.ast_node)); ;}
    break;

  case 526:

/* Line 936 of glr.c  */
#line 1665 "sql.ypp"
    { ((*yyvalp).ast_node)=newExprCal(t_expr_cal, "CMP", "ALL", (((yyGLRStackItem const *)yyvsp)[YYFILL ((2) - (6))].yystate.yysemantics.yysval.subtok), (((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (6))].yystate.yysemantics.yysval.ast_node), (((yyGLRStackItem const *)yyvsp)[YYFILL ((5) - (6))].yystate.yysemantics.yysval.ast_node)); ;}
    break;

  case 527:

/* Line 936 of glr.c  */
#line 1666 "sql.ypp"
    { ((*yyvalp).ast_node)=newExprCal(t_expr_cal, "|", NULL, 0, (((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (3))].yystate.yysemantics.yysval.ast_node), (((yyGLRStackItem const *)yyvsp)[YYFILL ((3) - (3))].yystate.yysemantics.yysval.ast_node)); ;}
    break;

  case 528:

/* Line 936 of glr.c  */
#line 1667 "sql.ypp"
    { ((*yyvalp).ast_node)=newExprCal(t_expr_cal, "&", NULL, 0, (((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (3))].yystate.yysemantics.yysval.ast_node), (((yyGLRStackItem const *)yyvsp)[YYFILL ((3) - (3))].yystate.yysemantics.yysval.ast_node)); ;}
    break;

  case 529:

/* Line 936 of glr.c  */
#line 1668 "sql.ypp"
    { ((*yyvalp).ast_node)=newExprCal(t_expr_cal, "^", NULL, 0, (((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (3))].yystate.yysemantics.yysval.ast_node), (((yyGLRStackItem const *)yyvsp)[YYFILL ((3) - (3))].yystate.yysemantics.yysval.ast_node)); ;}
    break;

  case 530:

/* Line 936 of glr.c  */
#line 1669 "sql.ypp"
    { ((*yyvalp).ast_node)=newExprCal(t_expr_cal, (SHIFT==1?"LS":"RS"), NULL, 0, (((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (3))].yystate.yysemantics.yysval.ast_node), (((yyGLRStackItem const *)yyvsp)[YYFILL ((3) - (3))].yystate.yysemantics.yysval.ast_node)); ;}
    break;

  case 531:

/* Line 936 of glr.c  */
#line 1670 "sql.ypp"
    { ((*yyvalp).ast_node)=newExprCal(t_expr_cal, "NOT", NULL, 0, NULL, (((yyGLRStackItem const *)yyvsp)[YYFILL ((2) - (2))].yystate.yysemantics.yysval.ast_node)); ;}
    break;

  case 532:

/* Line 936 of glr.c  */
#line 1671 "sql.ypp"
    { ((*yyvalp).ast_node)=newExprCal(t_expr_cal, "!", NULL, 0, NULL, (((yyGLRStackItem const *)yyvsp)[YYFILL ((2) - (2))].yystate.yysemantics.yysval.ast_node)); ;}
    break;

  case 533:

/* Line 936 of glr.c  */
#line 1672 "sql.ypp"
    { ((*yyvalp).ast_node)=NULL; ;}
    break;

  case 534:

/* Line 936 of glr.c  */
#line 1673 "sql.ypp"
    { ((*yyvalp).ast_node) = (((yyGLRStackItem const *)yyvsp)[YYFILL ((2) - (3))].yystate.yysemantics.yysval.ast_node); ;}
    break;

  case 535:

/* Line 936 of glr.c  */
#line 1676 "sql.ypp"
    { ((*yyvalp).ast_node)=newExprFunc(t_expr_func, "ISN", (((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (3))].yystate.yysemantics.yysval.ast_node), NULL, NULL, NULL); ;}
    break;

  case 536:

/* Line 936 of glr.c  */
#line 1677 "sql.ypp"
    { ((*yyvalp).ast_node)=newExprFunc(t_expr_func, "ISNN", (((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (4))].yystate.yysemantics.yysval.ast_node), NULL, NULL, NULL); ;}
    break;

  case 537:

/* Line 936 of glr.c  */
#line 1678 "sql.ypp"
    { ((*yyvalp).ast_node)=newExprFunc(t_expr_func, "ISB", (((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (3))].yystate.yysemantics.yysval.ast_node), NULL, NULL, NULL); ;}
    break;

  case 538:

/* Line 936 of glr.c  */
#line 1679 "sql.ypp"
    { ((*yyvalp).ast_node)=newExprFunc(t_expr_func, "ISNB", (((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (4))].yystate.yysemantics.yysval.ast_node), NULL, NULL, NULL); ;}
    break;

  case 539:

/* Line 936 of glr.c  */
#line 1682 "sql.ypp"
    { ((*yyvalp).ast_node)=newExprFunc(t_expr_func, "BA", (((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (5))].yystate.yysemantics.yysval.ast_node), (((yyGLRStackItem const *)yyvsp)[YYFILL ((3) - (5))].yystate.yysemantics.yysval.ast_node), (((yyGLRStackItem const *)yyvsp)[YYFILL ((5) - (5))].yystate.yysemantics.yysval.ast_node), NULL); ;}
    break;

  case 540:

/* Line 936 of glr.c  */
#line 1686 "sql.ypp"
    { ((*yyvalp).ast_node) = newExprList(t_expr_list, (((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (1))].yystate.yysemantics.yysval.ast_node), NULL); ;}
    break;

  case 541:

/* Line 936 of glr.c  */
#line 1687 "sql.ypp"
    { ((*yyvalp).ast_node) = newExprList(t_expr_list, (((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (3))].yystate.yysemantics.yysval.ast_node), (((yyGLRStackItem const *)yyvsp)[YYFILL ((3) - (3))].yystate.yysemantics.yysval.ast_node)); ;}
    break;

  case 543:

/* Line 936 of glr.c  */
#line 1691 "sql.ypp"
    {((*yyvalp).ast_node)=(((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (1))].yystate.yysemantics.yysval.ast_node);;}
    break;

  case 544:

/* Line 936 of glr.c  */
#line 1694 "sql.ypp"
    { ((*yyvalp).ast_node)=newExprCal(t_expr_cal, "INV", NULL, 0, (((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (5))].yystate.yysemantics.yysval.ast_node), (((yyGLRStackItem const *)yyvsp)[YYFILL ((4) - (5))].yystate.yysemantics.yysval.ast_node)); ;}
    break;

  case 545:

/* Line 936 of glr.c  */
#line 1695 "sql.ypp"
    { ((*yyvalp).ast_node)=newExprCal(t_expr_cal, "NIN", NULL, 0, (((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (6))].yystate.yysemantics.yysval.ast_node), (((yyGLRStackItem const *)yyvsp)[YYFILL ((5) - (6))].yystate.yysemantics.yysval.ast_node)); ;}
    break;

  case 546:

/* Line 936 of glr.c  */
#line 1696 "sql.ypp"
    { ((*yyvalp).ast_node)=newExprCal(t_expr_cal, "INS", NULL, 0, (((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (5))].yystate.yysemantics.yysval.ast_node), (((yyGLRStackItem const *)yyvsp)[YYFILL ((4) - (5))].yystate.yysemantics.yysval.ast_node)); ;}
    break;

  case 547:

/* Line 936 of glr.c  */
#line 1697 "sql.ypp"
    { ((*yyvalp).ast_node)=newExprCal(t_expr_cal, "NINS", NULL, 0, (((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (6))].yystate.yysemantics.yysval.ast_node), (((yyGLRStackItem const *)yyvsp)[YYFILL ((5) - (6))].yystate.yysemantics.yysval.ast_node)); ;}
    break;

  case 548:

/* Line 936 of glr.c  */
#line 1698 "sql.ypp"
    { ((*yyvalp).ast_node)=newExprCal(t_expr_cal, "EX", NULL, 0, NULL, (((yyGLRStackItem const *)yyvsp)[YYFILL ((3) - (4))].yystate.yysemantics.yysval.ast_node)); ;}
    break;

  case 549:

/* Line 936 of glr.c  */
#line 1699 "sql.ypp"
    { ((*yyvalp).ast_node)=newExprCal(t_expr_cal, "NINS", NULL, 0, NULL, (((yyGLRStackItem const *)yyvsp)[YYFILL ((4) - (5))].yystate.yysemantics.yysval.ast_node)); ;}
    break;

  case 550:

/* Line 936 of glr.c  */
#line 1702 "sql.ypp"
    { ((*yyvalp).ast_node)=newExprCal(t_expr_cal, "NAME", (((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (4))].yystate.yysemantics.yysval.strval), 0, NULL, (((yyGLRStackItem const *)yyvsp)[YYFILL ((3) - (4))].yystate.yysemantics.yysval.ast_node)); ;}
    break;

  case 551:

/* Line 936 of glr.c  */
#line 1706 "sql.ypp"
    { ((*yyvalp).ast_node)=newExprFunc(t_expr_func, "FCOUNTALL", NULL, NULL, NULL, NULL); ;}
    break;

  case 552:

/* Line 936 of glr.c  */
#line 1707 "sql.ypp"
    { ((*yyvalp).ast_node)=newExprFunc(t_expr_func, "FCOUNT", NULL, (((yyGLRStackItem const *)yyvsp)[YYFILL ((3) - (4))].yystate.yysemantics.yysval.ast_node), NULL, NULL); ;}
    break;

  case 553:

/* Line 936 of glr.c  */
#line 1709 "sql.ypp"
    { ((*yyvalp).ast_node)=newExprFunc(t_expr_func, "FSUM", NULL, (((yyGLRStackItem const *)yyvsp)[YYFILL ((3) - (4))].yystate.yysemantics.yysval.ast_node), NULL, NULL); ;}
    break;

  case 554:

/* Line 936 of glr.c  */
#line 1710 "sql.ypp"
    { ((*yyvalp).ast_node)=newExprFunc(t_expr_func, "FAVG", NULL, (((yyGLRStackItem const *)yyvsp)[YYFILL ((3) - (4))].yystate.yysemantics.yysval.ast_node), NULL, NULL); ;}
    break;

  case 555:

/* Line 936 of glr.c  */
#line 1711 "sql.ypp"
    { ((*yyvalp).ast_node)=newExprFunc(t_expr_func, "FMIN", NULL, (((yyGLRStackItem const *)yyvsp)[YYFILL ((3) - (4))].yystate.yysemantics.yysval.ast_node), NULL, NULL); ;}
    break;

  case 556:

/* Line 936 of glr.c  */
#line 1712 "sql.ypp"
    { ((*yyvalp).ast_node)=newExprFunc(t_expr_func, "FMAX", NULL, (((yyGLRStackItem const *)yyvsp)[YYFILL ((3) - (4))].yystate.yysemantics.yysval.ast_node), NULL, NULL); ;}
    break;

  case 557:

/* Line 936 of glr.c  */
#line 1717 "sql.ypp"
    { ((*yyvalp).ast_node)=newExprFunc(t_expr_func, "FSUBSTRING", NULL, (((yyGLRStackItem const *)yyvsp)[YYFILL ((3) - (4))].yystate.yysemantics.yysval.ast_node), NULL, NULL); ;}
    break;

  case 558:

/* Line 936 of glr.c  */
#line 1718 "sql.ypp"
    { ((*yyvalp).ast_node)=newExprFunc(t_expr_func, "FSUBSTRING2", NULL, (((yyGLRStackItem const *)yyvsp)[YYFILL ((3) - (6))].yystate.yysemantics.yysval.ast_node), (((yyGLRStackItem const *)yyvsp)[YYFILL ((5) - (6))].yystate.yysemantics.yysval.ast_node), NULL);;}
    break;

  case 559:

/* Line 936 of glr.c  */
#line 1719 "sql.ypp"
    { ((*yyvalp).ast_node)=newExprFunc(t_expr_func, "FSUBSTRING3", (((yyGLRStackItem const *)yyvsp)[YYFILL ((7) - (8))].yystate.yysemantics.yysval.ast_node), (((yyGLRStackItem const *)yyvsp)[YYFILL ((3) - (8))].yystate.yysemantics.yysval.ast_node), (((yyGLRStackItem const *)yyvsp)[YYFILL ((5) - (8))].yystate.yysemantics.yysval.ast_node), NULL); ;}
    break;

  case 560:

/* Line 936 of glr.c  */
#line 1720 "sql.ypp"
    { ((*yyvalp).ast_node)=newExprFunc(t_expr_func, "FTRIM", NULL, (((yyGLRStackItem const *)yyvsp)[YYFILL ((3) - (4))].yystate.yysemantics.yysval.ast_node), NULL, NULL); ;}
    break;

  case 561:

/* Line 936 of glr.c  */
#line 1721 "sql.ypp"
    { ;}
    break;

  case 562:

/* Line 936 of glr.c  */
#line 1724 "sql.ypp"
    { emit("INT 1"); ;}
    break;

  case 563:

/* Line 936 of glr.c  */
#line 1725 "sql.ypp"
    { emit("INT 2"); ;}
    break;

  case 564:

/* Line 936 of glr.c  */
#line 1726 "sql.ypp"
    { emit("INT 3"); ;}
    break;

  case 567:

/* Line 936 of glr.c  */
#line 1733 "sql.ypp"
    { emit("NUMBER 1"); ;}
    break;

  case 568:

/* Line 936 of glr.c  */
#line 1734 "sql.ypp"
    { emit("NUMBER 2"); ;}
    break;

  case 569:

/* Line 936 of glr.c  */
#line 1735 "sql.ypp"
    { emit("NUMBER 3"); ;}
    break;

  case 570:

/* Line 936 of glr.c  */
#line 1736 "sql.ypp"
    { emit("NUMBER 4"); ;}
    break;

  case 571:

/* Line 936 of glr.c  */
#line 1737 "sql.ypp"
    { emit("NUMBER 5"); ;}
    break;

  case 572:

/* Line 936 of glr.c  */
#line 1738 "sql.ypp"
    { emit("NUMBER 6"); ;}
    break;

  case 573:

/* Line 936 of glr.c  */
#line 1739 "sql.ypp"
    { emit("NUMBER 7"); ;}
    break;

  case 574:

/* Line 936 of glr.c  */
#line 1740 "sql.ypp"
    { emit("NUMBER 8"); ;}
    break;

  case 575:

/* Line 936 of glr.c  */
#line 1741 "sql.ypp"
    { emit("NUMBER 9"); ;}
    break;

  case 576:

/* Line 936 of glr.c  */
#line 1744 "sql.ypp"
    { ((*yyvalp).ast_node)=newExprFunc(t_expr_func, "CASE3", (((yyGLRStackItem const *)yyvsp)[YYFILL ((2) - (4))].yystate.yysemantics.yysval.ast_node), (((yyGLRStackItem const *)yyvsp)[YYFILL ((3) - (4))].yystate.yysemantics.yysval.ast_node), NULL, NULL); ;}
    break;

  case 577:

/* Line 936 of glr.c  */
#line 1745 "sql.ypp"
    { ((*yyvalp).ast_node)=newExprFunc(t_expr_func, "CASE2", (((yyGLRStackItem const *)yyvsp)[YYFILL ((2) - (6))].yystate.yysemantics.yysval.ast_node), (((yyGLRStackItem const *)yyvsp)[YYFILL ((3) - (6))].yystate.yysemantics.yysval.ast_node), (((yyGLRStackItem const *)yyvsp)[YYFILL ((5) - (6))].yystate.yysemantics.yysval.ast_node), NULL); ;}
    break;

  case 578:

/* Line 936 of glr.c  */
#line 1746 "sql.ypp"
    { ((*yyvalp).ast_node)=newExprFunc(t_expr_func, "CASE3", NULL, (((yyGLRStackItem const *)yyvsp)[YYFILL ((2) - (3))].yystate.yysemantics.yysval.ast_node), NULL, NULL); ;}
    break;

  case 579:

/* Line 936 of glr.c  */
#line 1747 "sql.ypp"
    { ((*yyvalp).ast_node)=newExprFunc(t_expr_func, "CASE4", NULL, (((yyGLRStackItem const *)yyvsp)[YYFILL ((2) - (5))].yystate.yysemantics.yysval.ast_node), (((yyGLRStackItem const *)yyvsp)[YYFILL ((4) - (5))].yystate.yysemantics.yysval.ast_node), NULL); ;}
    break;

  case 580:

/* Line 936 of glr.c  */
#line 1750 "sql.ypp"
    { ((*yyvalp).ast_node)=newExprFunc(t_expr_func, "WHEN1", NULL, (((yyGLRStackItem const *)yyvsp)[YYFILL ((2) - (4))].yystate.yysemantics.yysval.ast_node), (((yyGLRStackItem const *)yyvsp)[YYFILL ((4) - (4))].yystate.yysemantics.yysval.ast_node), NULL); ;}
    break;

  case 581:

/* Line 936 of glr.c  */
#line 1751 "sql.ypp"
    { ((*yyvalp).ast_node)=newExprFunc(t_expr_func, "WHEN2", NULL, (((yyGLRStackItem const *)yyvsp)[YYFILL ((3) - (5))].yystate.yysemantics.yysval.ast_node), (((yyGLRStackItem const *)yyvsp)[YYFILL ((5) - (5))].yystate.yysemantics.yysval.ast_node), (((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (5))].yystate.yysemantics.yysval.ast_node)); ;}
    break;

  case 582:

/* Line 936 of glr.c  */
#line 1754 "sql.ypp"
    { ((*yyvalp).ast_node)=newExprCal(t_expr_cal, "LIKE", NULL, 0, (((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (3))].yystate.yysemantics.yysval.ast_node), (((yyGLRStackItem const *)yyvsp)[YYFILL ((3) - (3))].yystate.yysemantics.yysval.ast_node)); ;}
    break;

  case 583:

/* Line 936 of glr.c  */
#line 1755 "sql.ypp"
    { ((*yyvalp).ast_node)=newExprCal(t_expr_cal, "NLIKE", NULL, 0, NULL, (((yyGLRStackItem const *)yyvsp)[YYFILL ((4) - (4))].yystate.yysemantics.yysval.ast_node)); ;}
    break;

  case 584:

/* Line 936 of glr.c  */
#line 1758 "sql.ypp"
    { ((*yyvalp).ast_node)=newExprCal(t_expr_cal, "REGEXP", NULL, 0, (((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (3))].yystate.yysemantics.yysval.ast_node), (((yyGLRStackItem const *)yyvsp)[YYFILL ((3) - (3))].yystate.yysemantics.yysval.ast_node)); ;}
    break;

  case 585:

/* Line 936 of glr.c  */
#line 1759 "sql.ypp"
    { ((*yyvalp).ast_node)=newExprCal(t_expr_cal, "NREGEXP", NULL, 0, (((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (4))].yystate.yysemantics.yysval.ast_node), (((yyGLRStackItem const *)yyvsp)[YYFILL ((4) - (4))].yystate.yysemantics.yysval.ast_node)); ;}
    break;



/* Line 936 of glr.c  */
#line 5625 "sql.tab.cpp"
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
yydestruct (const char *yymsg, int yytype, YYSTYPE *yyvaluep)
{
  YYUSE (yyvaluep);

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
		&yys->yysemantics.yysval);
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
			   NULL);
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
		       		       );
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
				&yysval);
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
	    yytoken = YYTRANSLATE (yychar);
	    yydestruct ("Error: discarding",
			yytoken, &yylval);
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
	      YY_SYMBOL_PRINT ("Shifting", yystos[yytable[yyj]],
			       &yylval, &yyerrloc);
	      yyglrShift (yystackp, 0, yytable[yyj],
			  yys->yyposn, &yylval, &yyerrloc);
	      yys = yystackp->yytops.yystates[0];
	      break;
	    }
	}

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
		&yylval);

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
#line 1770 "sql.ypp"


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
  fprintf(stderr, "\n");
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
	
  if(!yyparse())
    printf("SQL parse worked\n");
  else
    printf("SQL parse failed\n");
} */

