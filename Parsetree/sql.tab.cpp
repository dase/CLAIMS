
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
#line 22 "sql.ypp"

#include <stdlib.h>
#include <stdarg.h>
#include <string.h>
#include <iostream>
#include <stdio.h>
#include <malloc.h>
#include<set>
#include<algorithm>
#include "sql_node_struct.h"
using namespace std;

extern "C" int yylex(); ////////////////////////
extern "C" int yyparse();////////////////////
extern "C" int yywrap();/////////
extern "C" void yyerror(const char *s, ...);
extern "C" void emit(char *s, ...);
Node * parsetreeroot;
char globalInputText[10000];


/* Line 172 of glr.c  */
#line 81 "sql.tab.cpp"



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
#line 116 "sql.tab.cpp"

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
#define YYFINAL  31
/* YYLAST -- Last index in YYTABLE.  */
#define YYLAST   1713

/* YYNTOKENS -- Number of terminals.  */
#define YYNTOKENS  254
/* YYNNTS -- Number of nonterminals.  */
#define YYNNTS  66
/* YYNRULES -- Number of rules.  */
#define YYNRULES  252
/* YYNRULES -- Number of states.  */
#define YYNSTATES  510
/* YYMAXRHS -- Maximum number of symbols on right-hand side of rule.  */
#define YYMAXRHS 11
/* YYMAXLEFT -- Maximum number of symbols to the left of a handle
   accessed by $0, $-1, etc., in any rule.  */
#define YYMAXLEFT 0

/* YYTRANSLATE(X) -- Bison symbol number corresponding to X.  */
#define YYUNDEFTOK  2
#define YYMAXUTOK   494

#define YYTRANSLATE(YYX)						\
  ((unsigned int) (YYX) <= YYMAXUTOK ? yytranslate[YYX] : YYUNDEFTOK)

/* YYTRANSLATE[YYLEX] -- Bison symbol number corresponding to YYLEX.  */
static const unsigned char yytranslate[] =
{
       0,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,    17,     2,     2,     2,    28,    22,     2,
     252,   253,    26,    24,   250,    25,   251,    27,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,   249,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
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
     244,   245,   246,   247,   248
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
     202,   203,   205,   208,   213,   217,   219,   227,   230,   233,
     236,   237,   244,   247,   252,   253,   256,   264,   266,   275,
     276,   282,   283,   286,   289,   292,   295,   297,   298,   299,
     303,   307,   313,   315,   317,   321,   325,   333,   341,   345,
     349,   355,   361,   363,   372,   380,   388,   390,   399,   400,
     403,   406,   410,   416,   422,   430,   432,   437,   442,   443,
     447,   449,   458,   469,   479,   486,   498,   507,   509,   513,
     514,   519,   525,   530,   535,   541,   547,   548,   552,   555,
     559,   563,   567,   571,   574,   580,   584,   588,   591,   595,
     596,   600,   601,   603,   605,   606,   608,   610,   613,   615,
     619,   623,   627,   629,   631,   635,   637,   639,   641,   643,
     647,   651,   655,   659,   663,   667,   670,   674,   678,   682,
     686,   692,   699,   706,   713,   717,   721,   725,   729,   732,
     735,   739,   743,   747,   752,   756,   761,   767,   769,   773,
     774,   776,   782,   789,   795,   802,   807,   813,   818,   823,
     828,   833,   840,   849,   854,   862,   864,   866,   868,   875,
     882,   886,   890,   894,   898,   902,   906,   910,   914,   918,
     923,   930,   934,   940,   945,   951,   955,   960,   964,   969,
     971,   973,   975
};

/* YYRHS -- A `-1'-separated list of the rules' RHS.  */
static const short int yyrhs[] =
{
     255,     0,    -1,   256,   249,    -1,   255,   256,   249,    -1,
     257,    -1,   187,   268,   269,    -1,   187,   268,   269,   105,
     271,   258,   259,   263,   264,   265,   266,    -1,    -1,   237,
     314,    -1,    -1,   108,    47,   260,   262,    -1,   314,   261,
      -1,   260,   250,   314,   261,    -1,    -1,    39,    -1,    81,
      -1,    -1,   239,   183,    -1,    -1,   109,   314,    -1,    -1,
     161,    47,   260,    -1,    -1,   134,   314,    -1,   134,   314,
     250,   314,    -1,    -1,   125,   267,    -1,     3,    -1,   267,
     250,     3,    -1,    -1,   268,    33,    -1,   268,    84,    -1,
     268,    85,    -1,   268,   110,    -1,   268,   206,    -1,   268,
     203,    -1,   268,   201,    -1,   268,   202,    -1,   270,    -1,
     269,   250,   270,    -1,    26,    -1,   314,   275,    -1,   272,
      -1,   271,   250,   272,    -1,   273,    -1,   276,    -1,     3,
     275,    -1,     3,   251,     3,   275,    -1,   283,   274,     3,
      -1,   252,   271,   253,    -1,    -1,    38,    -1,    38,     3,
      -1,     3,    -1,    -1,   272,   277,   127,   273,   281,    -1,
     272,   206,   273,    -1,   272,   206,   273,   156,   314,    -1,
     272,   279,   278,   127,   273,   282,    -1,   272,   152,   280,
     127,   273,    -1,    -1,   118,    -1,    62,    -1,    -1,   163,
      -1,   133,    -1,   182,    -1,   133,   278,    -1,   182,   278,
      -1,    -1,    -1,   282,    -1,   156,   314,    -1,   228,   252,
     267,   253,    -1,   252,   257,   253,    -1,   284,    -1,    80,
     285,   105,     3,   258,   264,   265,    -1,   285,   144,    -1,
     285,   169,    -1,   285,   115,    -1,    -1,    80,   285,   286,
     105,   271,   258,    -1,     3,   287,    -1,   286,   250,     3,
     287,    -1,    -1,   251,    26,    -1,    80,   285,   105,   286,
     228,   271,   258,    -1,   288,    -1,   121,   290,   291,     3,
     292,   232,   293,   289,    -1,    -1,   156,   157,   128,   225,
     295,    -1,    -1,   290,   144,    -1,   290,    79,    -1,   290,
     110,    -1,   290,   115,    -1,   125,    -1,    -1,    -1,   252,
     267,   253,    -1,   252,   294,   253,    -1,   293,   250,   252,
     294,   253,    -1,   314,    -1,    78,    -1,   294,   250,   314,
      -1,   294,   250,    78,    -1,   121,   290,   291,     3,   190,
     295,   289,    -1,   121,   290,   291,     3,   292,   257,   289,
      -1,     3,    20,   314,    -1,     3,    20,    78,    -1,   295,
     250,     3,    20,   314,    -1,   295,   250,     3,    20,    78,
      -1,   296,    -1,   177,   290,   291,     3,   292,   232,   293,
     289,    -1,   177,   290,   291,     3,   190,   295,   289,    -1,
     177,   290,   291,     3,   292,   257,   289,    -1,   297,    -1,
     225,   298,   271,   190,   299,   258,   264,   265,    -1,    -1,
     290,   144,    -1,   290,   115,    -1,     3,    20,   314,    -1,
       3,   251,     3,    20,   314,    -1,   299,   250,     3,    20,
     314,    -1,   299,   250,     3,   251,     3,    20,   314,    -1,
     300,    -1,    61,    68,   301,     3,    -1,    61,   184,   301,
       3,    -1,    -1,   114,    18,    97,    -1,   302,    -1,    61,
     310,   207,   301,     3,   252,   303,   253,    -1,    61,   310,
     207,   301,     3,   251,     3,   252,   303,   253,    -1,    61,
     310,   207,   301,     3,   252,   303,   253,   308,    -1,    61,
     310,   207,   301,     3,   308,    -1,    61,   310,   207,   301,
       3,   251,     3,   252,   303,   253,   308,    -1,    61,   310,
     207,   301,     3,   251,     3,   308,    -1,   304,    -1,   303,
     250,   304,    -1,    -1,   305,     3,   307,   306,    -1,   166,
     128,   252,   267,   253,    -1,   128,   252,   267,   253,    -1,
     116,   252,   267,   253,    -1,   106,   116,   252,   267,   253,
      -1,   106,   128,   252,   267,   253,    -1,    -1,   306,    18,
     154,    -1,   306,   154,    -1,   306,    78,     4,    -1,   306,
      78,     5,    -1,   306,    78,     7,    -1,   306,    78,     6,
      -1,   306,    40,    -1,   306,   222,   252,   267,   253,    -1,
     306,   222,   128,    -1,   306,   166,   128,    -1,   306,   128,
      -1,   306,    56,     4,    -1,    -1,   309,   274,   257,    -1,
      -1,   115,    -1,   177,    -1,    -1,   208,    -1,   311,    -1,
     190,   312,    -1,   313,    -1,   312,   250,   313,    -1,     8,
      20,   314,    -1,     8,     9,   314,    -1,     3,    -1,     8,
      -1,     3,   251,     3,    -1,     4,    -1,     5,    -1,     7,
      -1,     6,    -1,   314,    24,   314,    -1,   314,    25,   314,
      -1,   314,    26,   314,    -1,   314,    27,   314,    -1,   314,
      28,   314,    -1,   314,    29,   314,    -1,    25,   314,    -1,
     314,    12,   314,    -1,   314,    10,   314,    -1,   314,    11,
     314,    -1,   314,    20,   314,    -1,   314,    20,   252,   257,
     253,    -1,   314,    20,    37,   252,   257,   253,    -1,   314,
      20,   193,   252,   257,   253,    -1,   314,    20,    33,   252,
     257,   253,    -1,   314,    21,   314,    -1,   314,    22,   314,
      -1,   314,    30,   314,    -1,   314,    23,   314,    -1,    18,
     314,    -1,    17,   314,    -1,     8,     9,   314,    -1,   252,
     314,   253,    -1,   314,    15,   154,    -1,   314,    15,    18,
     154,    -1,   314,    15,     6,    -1,   314,    15,    18,     6,
      -1,   314,    19,   314,    36,   314,    -1,   314,    -1,   314,
     250,   315,    -1,    -1,   315,    -1,   314,    16,   252,   315,
     253,    -1,   314,    18,    16,   252,   315,   253,    -1,   314,
      16,   252,   257,   253,    -1,   314,    18,    16,   252,   257,
     253,    -1,    97,   252,   257,   253,    -1,    18,    97,   252,
     257,   253,    -1,     3,   252,   316,   253,    -1,   248,   252,
      26,   253,    -1,   248,   252,   314,   253,    -1,   244,   252,
     315,   253,    -1,   244,   252,   314,   105,   314,   253,    -1,
     244,   252,   314,   105,   314,   102,   314,   253,    -1,   245,
     252,   315,   253,    -1,   245,   252,   317,   314,   105,   315,
     253,    -1,   131,    -1,   218,    -1,    46,    -1,   246,   252,
     314,   250,   318,   253,    -1,   247,   252,   314,   250,   318,
     253,    -1,   124,   314,    72,    -1,   124,   314,    73,    -1,
     124,   314,    74,    -1,   124,   314,    75,    -1,   124,   314,
     242,    -1,   124,   314,   241,    -1,   124,   314,   111,    -1,
     124,   314,   112,    -1,   124,   314,   113,    -1,    50,   314,
     319,    94,    -1,    50,   314,   319,    91,   314,    94,    -1,
      50,   319,    94,    -1,    50,   319,    91,   314,    94,    -1,
     236,   314,   211,   314,    -1,   319,   236,   314,   211,   314,
      -1,   314,    14,   314,    -1,   314,    18,    14,   314,    -1,
     314,    13,   314,    -1,   314,    18,    13,   314,    -1,    65,
      -1,    63,    -1,    64,    -1,    43,   314,    -1
};

/* YYRLINE[YYN] -- source line where rule number YYN was defined.  */
static const unsigned short int yyrline[] =
{
       0,   331,   331,   332,   337,   341,   347,   368,   369,   371,
     372,   379,   384,   391,   392,   393,   396,   397,   400,   401,
     408,   409,   416,   417,   422,   432,   433,   436,   437,   440,
     441,   442,   443,   444,   445,   446,   447,   448,   451,   456,
     461,   468,   477,   482,   489,   490,   494,   499,   503,   508,
     511,   512,   515,   516,   517,   532,   536,   540,   545,   549,
     555,   556,   557,   560,   561,   564,   565,   568,   569,   570,
     573,   574,   581,   585,   609,   614,   617,   622,   623,   624,
     625,   628,   633,   634,   638,   638,   640,   648,   651,   657,
     658,   661,   662,   663,   664,   665,   668,   668,   671,   672,
     675,   676,   679,   680,   681,   682,   685,   691,   697,   700,
     703,   706,   712,   715,   721,   727,   733,   736,   743,   744,
     745,   749,   752,   754,   756,   763,   767,   768,   771,   772,
     776,   779,   783,   788,   793,   797,   803,   808,   809,   812,
     812,   815,   816,   817,   818,   819,   822,   823,   824,   825,
     826,   827,   828,   829,   830,   831,   832,   833,   834,   856,
     893,   896,   897,   898,   901,   902,   907,   910,   912,   912,
     915,   917,   922,   923,   924,   925,   926,   927,   928,   931,
     932,   933,   934,   935,   936,   937,   938,   939,   940,   941,
     942,   943,   944,   945,   946,   947,   948,   949,   950,   951,
     952,   953,   956,   957,   958,   959,   962,   966,   967,   970,
     971,   974,   975,   976,   977,   978,   979,   982,   986,   987,
     989,   990,   991,   992,   993,   996,   997,   998,  1001,  1002,
    1005,  1006,  1007,  1008,  1009,  1010,  1011,  1012,  1013,  1016,
    1017,  1018,  1019,  1022,  1023,  1026,  1027,  1030,  1031,  1034,
    1035,  1036,  1039
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
  "WITH", "WRITE", "YEAR", "YEAR_MONTH", "ZEROFILL", "FSUBSTRING", "FTRIM",
  "FDATE_ADD", "FDATE_SUB", "FCOUNT", "';'", "','", "'.'", "'('", "')'",
  "$accept", "stmt_list", "stmt", "select_stmt", "opt_where",
  "opt_groupby", "groupby_list", "opt_asc_desc", "opt_with_rollup",
  "opt_having", "opt_orderby", "opt_limit", "opt_into_list", "column_list",
  "select_opts", "select_expr_list", "select_expr", "table_references",
  "table_reference", "table_factor", "opt_as", "opt_as_alias",
  "join_table", "opt_inner_cross", "opt_outer", "left_or_right",
  "opt_left_or_right_outer", "opt_join_condition", "join_condition",
  "table_subquery", "delete_stmt", "delete_opts", "delete_list",
  "opt_dot_star", "insert_stmt", "opt_ondupupdate", "insert_opts",
  "opt_into", "opt_col_names", "insert_vals_list", "insert_vals",
  "insert_asgn_list", "replace_stmt", "update_stmt", "update_opts",
  "update_asgn_list", "create_database_stmt", "opt_if_not_exists",
  "create_table_stmt", "create_col_list", "create_definition", "$@1",
  "column_atts", "data_type", "create_select_statement",
  "opt_ignore_replace", "opt_temporary", "set_stmt", "set_list",
  "set_expr", "expr", "val_list", "opt_val_list", "trim_ltb",
  "interval_exp", "case_list", 0
};
#endif

/* YYR1[YYN] -- Symbol number of symbol that rule YYN derives.  */
static const unsigned short int yyr1[] =
{
       0,   254,   255,   255,   256,   257,   257,   258,   258,   259,
     259,   260,   260,   261,   261,   261,   262,   262,   263,   263,
     264,   264,   265,   265,   265,   266,   266,   267,   267,   268,
     268,   268,   268,   268,   268,   268,   268,   268,   269,   269,
     269,   270,   271,   271,   272,   272,   273,   273,   273,   273,
     274,   274,   275,   275,   275,   276,   276,   276,   276,   276,
     277,   277,   277,   278,   278,   279,   279,   280,   280,   280,
     281,   281,   282,   282,   283,   256,   284,   285,   285,   285,
     285,   284,   286,   286,   287,   287,   284,   256,   288,   289,
     289,   290,   290,   290,   290,   290,   291,   291,   292,   292,
     293,   293,   294,   294,   294,   294,   288,   288,   295,   295,
     295,   295,   256,   296,   296,   296,   256,   297,   298,   298,
     298,   299,   299,   299,   299,   256,   300,   300,   301,   301,
     256,   302,   302,   302,   302,   302,   302,   303,   303,   305,
     304,   304,   304,   304,   304,   304,   306,   306,   306,   306,
     306,   306,   306,   306,   306,   306,   306,   306,   306,   307,
     308,   309,   309,   309,   310,   310,   256,   311,   312,   312,
     313,   313,   314,   314,   314,   314,   314,   314,   314,   314,
     314,   314,   314,   314,   314,   314,   314,   314,   314,   314,
     314,   314,   314,   314,   314,   314,   314,   314,   314,   314,
     314,   314,   314,   314,   314,   314,   314,   315,   315,   316,
     316,   314,   314,   314,   314,   314,   314,   314,   314,   314,
     314,   314,   314,   314,   314,   317,   317,   317,   314,   314,
     318,   318,   318,   318,   318,   318,   318,   318,   318,   314,
     314,   314,   314,   319,   319,   314,   314,   314,   314,   314,
     314,   314,   314
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
       0,     1,     2,     4,     3,     1,     7,     2,     2,     2,
       0,     6,     2,     4,     0,     2,     7,     1,     8,     0,
       5,     0,     2,     2,     2,     2,     1,     0,     0,     3,
       3,     5,     1,     1,     3,     3,     7,     7,     3,     3,
       5,     5,     1,     8,     7,     7,     1,     8,     0,     2,
       2,     3,     5,     5,     7,     1,     4,     4,     0,     3,
       1,     8,    10,     9,     6,    11,     8,     1,     3,     0,
       4,     5,     4,     4,     5,     5,     0,     3,     2,     3,
       3,     3,     3,     2,     5,     3,     3,     2,     3,     0,
       3,     0,     1,     1,     0,     1,     1,     2,     1,     3,
       3,     3,     1,     1,     3,     1,     1,     1,     1,     3,
       3,     3,     3,     3,     3,     2,     3,     3,     3,     3,
       5,     6,     6,     6,     3,     3,     3,     3,     2,     2,
       3,     3,     3,     4,     3,     4,     5,     1,     3,     0,
       1,     5,     6,     5,     6,     4,     5,     4,     4,     4,
       4,     6,     8,     4,     7,     1,     1,     1,     6,     6,
       3,     3,     3,     3,     3,     3,     3,     3,     3,     4,
       6,     3,     5,     4,     5,     3,     4,     3,     4,     1,
       1,     1,     2
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
       0,     0,     0,     0,     0,     0,     0,     0,     1,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     2,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0
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
       0,     0,     0
};

/* YYDEFACT[S] -- default rule to reduce with in state S when YYTABLE
   doesn't specify something else to do.  Zero means the default is an
   error.  */
static const unsigned char yydefact[] =
{
       0,   164,    80,    91,    91,    29,     0,    91,     0,     0,
       4,    75,    87,   112,   116,   125,   130,   166,   128,   128,
     165,     0,     0,    97,    97,     0,     0,   167,   168,     0,
       0,     1,     0,     2,     0,     0,     0,   128,    84,     0,
      79,    77,    78,     0,    93,    94,    95,    96,    92,     0,
       0,   172,   175,   176,   178,   177,   173,     0,     0,     0,
      40,    30,     0,     0,   250,   251,   249,    31,    32,     0,
      33,    36,    37,    35,    34,     0,     0,     0,     0,     0,
       0,     5,    38,    54,     0,     0,     0,    95,    92,    54,
       0,     0,    42,    44,    45,    50,     3,     0,   126,   127,
       0,     0,    82,     7,     0,     0,     0,    98,    98,     0,
     209,     0,   199,     0,   198,   185,   252,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    53,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,    41,   171,   170,   169,     0,    46,     0,     0,     0,
       0,    62,    61,    65,    69,    66,     0,     0,    63,    51,
       0,   129,   161,    85,     0,    20,     0,     7,    84,     0,
       0,     0,     0,     0,   174,   207,   210,     0,   200,     0,
       0,     0,     0,   241,     0,     0,   207,     0,   227,   225,
     226,     0,     0,     0,     0,     0,     0,   201,     7,    39,
     187,   188,   186,   247,   245,   204,     0,   202,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   189,   194,   195,
     197,   179,   180,   181,   182,   183,   184,   196,    52,    54,
      74,    49,     0,     7,    43,    63,    63,     0,    56,     0,
      64,     0,    48,   162,   163,     0,   139,   134,    50,     8,
       0,    22,     7,    81,    83,     0,    89,    27,     0,     0,
      89,    89,     0,    89,     0,   217,     0,     0,     0,   239,
       0,     0,   215,     0,   220,   223,     0,     0,     0,   218,
     219,     9,   205,   203,     0,     0,   248,   246,     0,     0,
       0,     0,     0,     0,    47,     0,     0,     0,    20,    67,
      68,     0,     0,    70,     0,   161,     0,     0,     0,     0,
       0,   137,     0,     0,     0,     0,    76,    86,     0,     0,
       0,   106,     0,    99,     0,    89,   107,   114,    89,   115,
     208,   215,   243,     0,   242,     0,     0,     0,     0,     0,
       0,     0,    18,   213,   211,     0,     0,   206,     0,     0,
       0,   190,   121,     0,     0,    22,    59,    57,     0,     0,
      55,    71,     0,   139,   136,     0,     0,     0,     0,     0,
     139,   161,   159,   160,    21,    13,    23,   109,   108,     0,
       0,    28,   103,     0,   102,     0,    88,   113,   240,   244,
       0,   221,     0,     0,   228,   229,     0,     0,    20,   214,
     212,   193,   191,   192,     0,     0,     0,   117,    72,     0,
      58,     0,     0,     0,     0,     0,     0,   138,   133,   146,
       0,    14,    15,    11,     0,     0,     0,     0,   100,     0,
       0,   224,   230,   231,   232,   233,   236,   237,   238,   235,
     234,    16,    19,    22,   122,   123,     0,     0,   161,     0,
       0,   143,   142,     0,   140,    13,    24,     0,   111,   110,
     105,   104,     0,   222,     0,    10,    25,     0,    73,   135,
     144,   145,   141,     0,   153,     0,     0,   157,   148,     0,
       0,    12,    90,   101,    17,     0,     6,   124,   147,   158,
     149,   150,   152,   151,   156,   155,     0,    26,     0,   154
};

/* YYPDEFGOTO[NTERM-NUM].  */
static const short int yydefgoto[] =
{
      -1,     8,     9,    10,   175,   352,   384,   433,   475,   408,
     261,   326,   496,   268,    25,    81,    82,    91,    92,    93,
     170,   151,    94,   167,   251,   168,   247,   370,   371,    95,
      11,    22,    43,   102,    12,   331,    23,    49,   181,   335,
     393,   266,    13,    14,    30,   243,    15,    35,    16,   320,
     321,   322,   464,   429,   257,   258,    21,    17,    27,    28,
     185,   186,   187,   202,   349,   119
};

/* YYPACT[STATE-NUM] -- Index in YYTABLE of the portion describing
   STATE-NUM.  */
#define YYPACT_NINF -314
static const short int yypact[] =
{
     236,   -45,  -314,  -314,  -314,  -314,    16,     2,    85,  -204,
    -314,  -314,  -314,  -314,  -314,  -314,  -314,  -314,   -61,   -61,
    -314,  -106,    22,   331,   331,   125,   187,  -201,  -314,   306,
       3,  -314,  -147,  -314,   127,   149,   173,   -61,   -68,   195,
    -314,  -314,  -314,  -102,  -314,  -314,  -314,  -314,  -314,   201,
     205,  -158,  -314,  -314,  -314,  -314,   203,   987,  1010,   987,
    -314,  -314,   987,   590,  -314,  -314,  -314,  -314,  -314,   -38,
    -314,  -314,  -314,  -314,  -314,    19,    27,    29,    31,    36,
     987,   -95,  -314,  1576,   987,   987,    16,     4,     5,     8,
      -1,  -139,   297,  -314,  -314,   212,  -314,   176,  -314,  -314,
     303,   292,  -314,    40,  -193,     3,   321,  -169,  -108,   327,
     987,   987,   469,   114,   469,  -314,  -314,   987,  1319,   -74,
     180,   987,   459,   987,   987,   617,   218,     3,   987,  -314,
     987,   987,   987,   987,   987,    10,   116,   157,   987,   541,
     987,   987,   987,   987,   987,   987,   987,   987,   987,   987,
     371,  -314,  1645,  1645,  -314,   373,  -314,   131,   -48,   375,
       3,  -314,  -314,  -314,  -124,  -314,     3,   259,   224,  -314,
     388,  -314,   -73,  -314,   987,   231,     3,  -103,   -68,   391,
     399,  -144,   391,  -143,  -314,  1179,  -314,   150,  1645,   180,
    1373,   -64,   987,  -314,   987,   151,  1158,   153,  -314,  -314,
    -314,   155,   987,  1200,  1254,   166,  1116,  -314,  -103,  -314,
    1665,  1338,   939,  1683,  1683,  -314,     7,  -314,   658,   987,
     987,   170,  1618,   175,   177,   181,   658,   703,   622,   718,
     602,   484,   484,   395,   395,   395,   395,  -314,  -314,    87,
    -314,  -314,    -2,   -57,   297,   224,   224,   305,   280,     3,
    -314,   313,  -314,  -314,  -314,   439,    97,  -314,   212,  1645,
     396,   311,  -103,  -314,  -314,   427,  -115,  -314,   -27,   196,
     298,  -115,   196,   298,   987,  -314,   202,   987,   987,  -314,
    1454,  1412,  -314,   987,  -314,  -314,  1433,   333,   333,  -314,
    -314,   350,  -314,  -314,   215,   216,  1683,  1683,   658,   987,
     180,   180,   180,   217,  -314,   987,   470,   471,   231,  -314,
    -314,     3,   987,  -141,     3,   -65,   -60,   220,   228,   353,
     -26,  -314,   480,   180,   987,   987,  -314,  -314,   732,   343,
     498,  -314,   501,  -314,   755,  -109,  -314,  -314,  -109,  -314,
    -314,   326,  1645,  1487,  -314,   987,    51,   987,   987,   253,
     254,   468,   408,  -314,  -314,   265,   266,   668,   267,   268,
     272,  -314,  1645,   506,     9,   311,  -314,  1645,   987,   276,
    -314,  -314,  -137,    97,  -314,   277,   281,   399,   399,   283,
      97,   -93,  -314,  -314,   286,  1546,  1275,  -314,  1645,   410,
     519,  -314,  -314,    11,  1645,   288,  -314,  -314,  -314,  1645,
     987,  -314,   289,  1298,  -314,  -314,   987,   987,   231,  -314,
    -314,  -314,  -314,  -314,   987,   987,   538,  -314,  1645,   399,
    -314,    39,   399,   399,    43,    48,   399,  -314,  -314,  -314,
     987,  -314,  -314,  -314,   987,   318,   850,   873,  -314,   755,
    1137,  -314,  -314,  -314,  -314,  -314,  -314,  -314,  -314,  -314,
    -314,   -39,  1645,   311,  1645,  1645,   530,    52,   -67,    58,
      62,  -314,  -314,    64,   141,  1546,  1645,   391,  -314,  1645,
    -314,  1645,    69,  -314,   368,  -314,   428,   987,  -314,  -314,
    -314,  -314,  -314,   398,  -314,   550,   328,  -314,  -314,   429,
    -114,  -314,   310,  -314,  -314,   399,  -314,  1645,  -314,  -314,
    -314,  -314,  -314,  -314,  -314,  -314,   399,   314,   130,  -314
};

/* YYPGOTO[NTERM-NUM].  */
static const short int yypgoto[] =
{
    -314,  -314,   547,    95,   117,  -314,   163,    96,  -314,  -314,
    -282,  -313,  -314,   108,  -314,  -314,   442,    94,   411,  -154,
     315,   -85,  -314,  -314,    21,  -314,  -314,  -314,   200,  -314,
    -314,  -314,   542,   402,  -314,   -53,   354,   553,   474,   316,
     144,  -181,  -314,  -314,  -314,  -314,  -314,    17,  -314,   213,
     207,  -314,  -314,  -314,  -284,  -314,  -314,  -314,  -314,   499,
     -25,   -82,  -314,  -314,   301,   481
};

/* YYTABLE[YYPACT[STATE-NUM]].  What to do in state STATE-NUM.  If
   positive, shift that token.  If negative, reduce the rule which
   number is the opposite.  If zero, do what YYDEFACT says.
   If YYTABLE_NINF, syntax error.  */
#define YYTABLE_NINF -216
static const short int yytable[] =
{
      83,   271,    89,   105,   156,  -118,    89,  -120,  -119,   245,
     127,   129,   248,   292,   505,   368,   215,   192,   305,   368,
     193,   179,   253,    18,    26,    38,   365,   278,   216,   415,
     279,   374,   112,   114,   115,   176,    36,   116,   118,   197,
     201,   329,   253,     5,     5,    33,   150,   329,   253,    86,
     253,   159,   417,    34,   100,   126,   375,   106,   246,   152,
     153,   130,   131,   132,   133,   134,   135,   136,   376,   137,
     138,   139,   140,   141,   142,   143,   144,   145,   146,   147,
     148,   149,   182,   180,   254,    31,   188,   369,   269,   272,
     129,   369,   190,   109,   110,   313,   196,   428,   203,   204,
     206,    37,    96,    83,   254,   210,   211,   212,   213,   214,
     254,   160,   254,   222,   227,   228,   229,   230,   231,   232,
     233,   234,   235,   236,   237,   150,   453,    39,    51,    52,
      53,    54,    55,    56,   174,   330,   295,    40,   506,    19,
     476,   395,    57,    58,   180,    97,     1,   160,   106,   259,
      59,    60,    98,   400,   304,   128,  -131,   366,    61,   483,
     372,   293,   194,    20,   217,     2,    41,   280,    62,   281,
     219,   220,   194,   221,   479,    63,    99,   286,   255,   256,
     174,   484,  -132,   101,   158,   157,     5,   373,    64,    65,
      66,    42,   340,   307,   296,   297,    84,   485,   103,   177,
     474,   126,   160,   316,   107,   241,     3,    85,   108,    67,
      68,   430,   111,   317,   120,   195,   356,   336,   337,   486,
     339,   208,    69,   332,   380,   318,   333,   381,   130,   131,
     132,   133,   134,   135,   136,    70,   137,   138,   139,   140,
     141,   142,   143,   144,   145,   146,   147,   148,   149,   306,
     169,    90,   342,   343,  -118,    90,  -120,  -119,   346,   155,
     416,   437,     4,   319,   438,   402,   309,   310,   -84,   487,
     262,   121,     5,   171,   357,     6,   270,   174,   273,   122,
     362,   123,   396,   124,   276,   397,   492,   367,   125,   380,
     -84,   101,   458,   332,   263,   488,   461,     1,   332,   385,
     386,   462,   332,   388,   401,   478,   172,   489,   332,   394,
       7,   480,   332,   294,   332,   481,     2,   482,   173,   437,
     399,   303,   493,   403,   178,   291,    71,    72,    73,  -215,
     184,    74,   500,   501,   502,   503,  -215,  -215,  -215,  -215,
    -215,  -215,  -215,   418,  -215,  -215,  -215,  -215,  -215,  -215,
    -215,  -215,  -215,  -215,  -215,  -215,  -215,     3,    24,   161,
     308,    29,  -215,   490,  -215,  -215,   189,     5,   218,    75,
      76,    77,    78,    79,   238,   440,   239,    80,   242,   327,
     332,   385,   452,   509,   240,    44,   249,   250,  -215,   454,
     455,   252,   260,   355,   265,   358,   359,   360,  -215,  -215,
    -215,  -215,   267,   275,   282,   465,   284,  -215,   285,   466,
      44,   469,   471,     4,   394,   162,    45,  -215,   383,   289,
    -215,    87,   298,     5,   -60,   149,     6,   300,  -215,   301,
     163,  -215,   311,   302,  -215,  -215,   312,  -215,  -215,  -215,
     314,    45,   315,   324,  -215,   325,    46,   328,   334,   164,
      88,  -215,   497,  -215,   329,   341,    47,   348,   351,  -215,
    -215,     7,    51,    52,    53,    54,    55,    56,   353,   354,
     361,   207,   377,   363,   364,    48,    57,    58,  -215,   165,
     378,   379,  -215,   382,    59,   424,   425,  -215,   138,   139,
     140,   141,   142,   143,   144,   145,   146,   147,   148,   149,
     389,   390,    62,   166,   391,   198,   404,   405,  -215,    63,
     145,   146,   147,   148,   149,   406,  -215,   407,   409,   410,
     411,   412,    64,    65,    66,   413,   414,   457,   419,   422,
     459,   460,  -215,   423,   463,   426,   430,  -215,   435,   436,
     439,   456,   441,   467,    51,    52,    53,    54,    55,    56,
     477,   494,   498,   495,   499,    32,    69,   504,    57,    58,
     330,   491,  -215,  -215,   332,  -215,    59,  -215,  -215,   451,
     209,   244,   420,   323,   223,  -215,  -215,    50,   224,  -215,
     264,   104,   183,   472,    62,   154,   421,   427,   338,   350,
     199,    63,     0,    51,    52,    53,    54,    55,    56,   191,
       0,     0,     0,   507,    64,    65,    66,    57,    58,     0,
       0,     0,     0,     0,   508,    59,     0,     0,     0,     0,
      51,    52,    53,    54,    55,    56,   143,   144,   145,   146,
     147,   148,   149,    62,    57,    58,     0,     0,    69,     0,
      63,     0,    59,   205,   141,   142,   143,   144,   145,   146,
     147,   148,   149,    64,    65,    66,     0,     0,     0,     0,
      62,    51,    52,    53,    54,    55,    56,    63,     0,     0,
       0,     0,     0,     0,     0,    57,    58,   200,     0,     0,
      64,    65,    66,    59,     0,     0,     0,    69,   139,   140,
     141,   142,   143,   144,   145,   146,   147,   148,   149,     0,
       0,    62,     0,    75,    76,    77,    78,    79,    63,     0,
       0,    80,     0,     0,    69,     0,     0,     0,     0,     0,
       0,    64,    65,    66,   140,   141,   142,   143,   144,   145,
     146,   147,   148,   149,   225,    51,    52,    53,    54,    55,
      56,   142,   143,   144,   145,   146,   147,   148,   149,    57,
      58,     0,     0,     0,     0,    69,     0,    59,    51,    52,
      53,    54,    55,    56,     0,     0,     0,     0,     0,     0,
       0,     0,    57,    58,     0,    62,     0,     0,     0,     0,
      59,     0,    63,     0,     0,    75,    76,    77,    78,    79,
       0,     0,     0,   226,     0,    64,    65,    66,    62,     0,
       0,     0,     0,     0,     0,    63,     0,     0,     0,     0,
     387,     0,     0,     0,     0,     0,     0,     0,    64,    65,
      66,     0,     0,     0,     0,     0,   117,     0,     0,    69,
       0,     0,     0,   392,    75,    76,    77,    78,    79,     0,
       0,     0,    80,     0,     0,     5,     0,     0,     0,     0,
       0,     0,    69,    51,    52,    53,    54,    55,    56,     0,
       0,    75,    76,    77,    78,    79,     0,    57,    58,    80,
       0,     0,     0,     0,     0,    59,    51,    52,    53,    54,
      55,    56,     0,     0,     0,     0,     0,     0,     0,     0,
      57,    58,     0,    62,     0,     0,     0,     0,    59,     0,
      63,     0,    75,    76,    77,    78,    79,     0,     0,     0,
      80,     0,     0,    64,    65,    66,    62,     0,     0,     0,
       0,     0,     0,    63,     0,     0,     0,     0,   468,     0,
       0,     0,     0,     0,     0,     0,    64,    65,    66,     0,
       0,     0,     0,     0,     0,     0,     0,    69,     0,     0,
       0,   470,   133,   134,   135,   136,     0,   137,   138,   139,
     140,   141,   142,   143,   144,   145,   146,   147,   148,   149,
      69,     0,     0,     0,     0,     0,    75,    76,    77,    78,
      79,     0,     0,     0,    80,     0,     0,     0,     0,     0,
      51,    52,    53,    54,    55,    56,     0,     0,     0,    75,
      76,    77,    78,    79,    57,    58,     0,    80,     0,     0,
       0,     0,    59,    51,    52,    53,    54,    55,    56,     0,
       0,     0,     0,     0,     0,     0,     0,    57,    58,     0,
      62,     0,     0,     0,     0,    59,     0,    63,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      64,    65,    66,    62,     0,     0,     0,     0,     0,     0,
      63,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,    64,    65,    66,     0,     0,     0,     0,
       0,     0,     0,     0,    69,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    75,    76,    77,    78,    79,     0,
       0,     0,    80,     0,     0,     0,     0,   113,     0,     0,
       0,     0,     0,     0,     0,     0,     0,    75,    76,    77,
      78,    79,     0,     0,     0,    80,   130,   131,   132,   133,
     134,   135,   136,     0,   137,   138,   139,   140,   141,   142,
     143,   144,   145,   146,   147,   148,   149,   130,   131,   132,
     133,   134,   135,   136,     0,   137,   138,   139,   140,   141,
     142,   143,   144,   145,   146,   147,   148,   149,   130,   131,
     132,   133,   134,   135,   136,     0,   137,   138,   139,   140,
     141,   142,   143,   144,   145,   146,   147,   148,   149,   130,
     131,   132,   133,   134,   135,   136,     0,   137,   138,   139,
     140,   141,   142,   143,   144,   145,   146,   147,   148,   149,
     130,   131,   132,   133,   134,   135,   136,     0,   137,   138,
     139,   140,   141,   142,   143,   144,   145,   146,   147,   148,
     149,    75,    76,    77,    78,    79,     0,     0,     0,    80,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    75,    76,    77,    78,    79,     0,
       0,     0,    80,   283,   130,   131,   132,   133,   134,   135,
     136,     0,   137,   138,   139,   140,   141,   142,   143,   144,
     145,   146,   147,   148,   149,   130,   131,   132,   133,   134,
     135,   136,     0,   137,   138,   139,   140,   141,   142,   143,
     144,   145,   146,   147,   148,   149,     0,     0,   130,   131,
     132,   133,   134,   135,   136,     0,   137,   138,   139,   140,
     141,   142,   143,   144,   145,   146,   147,   148,   149,   130,
     131,   132,   133,   134,   135,   136,     0,   137,   138,   139,
     140,   141,   142,   143,   144,   145,   146,   147,   148,   149,
     132,   133,   134,   135,   136,     0,   137,   138,   139,   140,
     141,   142,   143,   144,   145,   146,   147,   148,   149,   290,
     442,   443,   444,   445,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   130,   131,   132,   133,   134,   135,   136,
     473,   137,   138,   139,   140,   141,   142,   143,   144,   145,
     146,   147,   148,   149,     0,     0,     0,     0,   274,   446,
     447,   448,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   130,   131,   132,   133,   134,   135,   136,   274,
     137,   138,   139,   140,   141,   142,   143,   144,   145,   146,
     147,   148,   149,   130,   131,   132,   133,   134,   135,   136,
     287,   137,   138,   139,   140,   141,   142,   143,   144,   145,
     146,   147,   148,   149,   130,   131,   132,   133,   134,   135,
     136,     0,   137,   138,   139,   140,   141,   142,   143,   144,
     145,   146,   147,   148,   149,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   130,   131,   132,
     133,   134,   135,   136,   288,   137,   138,   139,   140,   141,
     142,   143,   144,   145,   146,   147,   148,   149,     0,     0,
       0,     0,     0,     0,     0,   434,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   347,   449,
     450,     0,     0,     0,     0,     0,     0,     0,   344,     0,
       0,     0,     0,     0,     0,   117,   130,   131,   132,   133,
     134,   135,   136,     0,   137,   138,   139,   140,   141,   142,
     143,   144,   145,   146,   147,   148,   149,     0,     0,   129,
       0,   398,     0,     0,   277,   431,   130,   131,   132,   133,
     134,   135,   136,     0,   137,   138,   139,   140,   141,   142,
     143,   144,   145,   146,   147,   148,   149,     0,     0,     0,
       0,     0,     0,     0,   150,     0,     0,     0,     0,     0,
       0,     0,     0,   345,     0,     0,     0,   432,   130,   131,
     132,   133,   134,   135,   136,     0,   137,   138,   139,   140,
     141,   142,   143,   144,   145,   146,   147,   148,   149,     0,
       0,     0,     0,     0,   299,   130,   131,   132,   133,   134,
     135,   136,     0,   137,   138,   139,   140,   141,   142,   143,
     144,   145,   146,   147,   148,   149,   131,   132,   133,   134,
     135,   136,     0,   137,   138,   139,   140,   141,   142,   143,
     144,   145,   146,   147,   148,   149,  -216,  -216,  -216,  -216,
       0,   137,   138,   139,   140,   141,   142,   143,   144,   145,
     146,   147,   148,   149
};

/* YYCONFLP[YYPACT[STATE-NUM]] -- Pointer into YYCONFL of start of
   list of conflicting reductions corresponding to action entry for
   state STATE-NUM in yytable.  0 means no conflicts.  The list in
   yyconfl is terminated by a rule number of 0.  */
static const unsigned char yyconflp[] =
{
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     3,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     5,
       0,     0,     0,     0,     0,     0,     7,     9,    11,    13,
      15,    17,    19,     0,    21,    23,    25,    27,    29,    31,
      33,    35,    37,    39,    41,    43,    45,     0,     0,     0,
       0,     0,    47,     0,    49,    51,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,    53,     0,
       0,     0,     0,     0,     0,     0,     0,     0,    55,    57,
      59,    61,     0,     0,     0,     0,     0,    63,     0,     0,
       0,     0,     0,     0,     0,     0,     0,    65,     0,     0,
      67,     0,     0,     0,     0,     0,     0,     0,    69,     0,
       0,    71,     0,     0,    73,    75,     1,    77,    79,    81,
       0,     0,     0,     0,    83,     0,     0,     0,     0,     0,
       0,    85,     0,    87,     0,     0,     0,     0,     0,    89,
      91,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,    93,     0,
       0,     0,    95,     0,     0,     0,     0,    97,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,    99,     0,
       0,     0,     0,     0,     0,     0,   101,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   103,     0,     0,     0,     0,   105,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   107,   109,     0,   111,     0,   113,   115,     0,
       0,     0,     0,     0,     0,   117,   119,     0,     0,   121,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0
};

/* YYCONFL[I] -- lists of conflicting rule numbers, each terminated by
   0, pointed into by YYCONFLP.  */
static const short int yyconfl[] =
{
       0,    56,     0,    70,     0,   216,     0,   216,     0,   216,
       0,   216,     0,   216,     0,   216,     0,   216,     0,   216,
       0,   216,     0,   216,     0,   216,     0,   216,     0,   216,
       0,   216,     0,   216,     0,   216,     0,   216,     0,   216,
       0,   216,     0,   216,     0,   216,     0,   216,     0,   216,
       0,   216,     0,   216,     0,   216,     0,   216,     0,   216,
       0,   216,     0,   216,     0,   216,     0,   216,     0,   216,
       0,   216,     0,   216,     0,   216,     0,   216,     0,   216,
       0,   216,     0,   216,     0,   216,     0,   216,     0,   216,
       0,   216,     0,   216,     0,   216,     0,   216,     0,   216,
       0,   216,     0,   216,     0,   216,     0,   216,     0,   216,
       0,   216,     0,   216,     0,   216,     0,   216,     0,   216,
       0,   216,     0
};

static const short int yycheck[] =
{
      25,   182,     3,   105,    89,     3,     3,     3,     3,   133,
     105,     3,   166,     6,   128,   156,     6,    91,    20,   156,
      94,   190,   115,    68,     8,     3,   308,    91,    18,    20,
      94,   315,    57,    58,    59,   228,    19,    62,    63,   121,
     122,   156,   115,   187,   187,   249,    38,   156,   115,   250,
     115,   190,   365,   114,    37,    80,   116,   250,   182,    84,
      85,    10,    11,    12,    13,    14,    15,    16,   128,    18,
      19,    20,    21,    22,    23,    24,    25,    26,    27,    28,
      29,    30,   190,   252,   177,     0,   111,   228,   232,   232,
       3,   228,   117,   251,   252,   249,   121,   381,   123,   124,
     125,   207,   249,   128,   177,   130,   131,   132,   133,   134,
     177,   250,   177,   138,   139,   140,   141,   142,   143,   144,
     145,   146,   147,   148,   149,    38,   408,   105,     3,     4,
       5,     6,     7,     8,   237,   250,   218,   115,   252,   184,
     453,   250,    17,    18,   252,    18,    61,   250,   250,   174,
      25,    26,     3,   102,   239,   250,   249,   311,    33,    18,
     314,   154,   236,   208,   154,    80,   144,   192,    43,   194,
      13,    14,   236,    16,   458,    50,     3,   202,   251,   252,
     237,    40,   249,   251,    90,    90,   187,   252,    63,    64,
      65,   169,   274,   250,   219,   220,     9,    56,     3,   105,
     239,   226,   250,   106,     3,   253,   121,    20,     3,    84,
      85,   250,     9,   116,   252,   120,   298,   270,   271,    78,
     273,   127,    97,   250,   250,   128,   253,   253,    10,    11,
      12,    13,    14,    15,    16,   110,    18,    19,    20,    21,
      22,    23,    24,    25,    26,    27,    28,    29,    30,   251,
      38,   252,   277,   278,   252,   252,   252,   252,   283,   251,
     251,   250,   177,   166,   253,   347,   245,   246,   228,   128,
     176,   252,   187,    97,   299,   190,   181,   237,   183,   252,
     305,   252,   335,   252,   189,   338,   467,   312,   252,   250,
     250,   251,   253,   250,   177,   154,   253,    61,   250,   324,
     325,   253,   250,   328,   253,   253,     3,   166,   250,   334,
     225,   253,   250,   218,   250,   253,    80,   253,    26,   250,
     345,   226,   253,   348,     3,   208,   201,   202,   203,     3,
       3,   206,     4,     5,     6,     7,    10,    11,    12,    13,
      14,    15,    16,   368,    18,    19,    20,    21,    22,    23,
      24,    25,    26,    27,    28,    29,    30,   121,     4,    62,
     243,     7,    36,   222,    38,    39,   252,   187,   252,   244,
     245,   246,   247,   248,     3,   400,     3,   252,     3,   262,
     250,   406,   407,   253,   253,    79,   127,   163,    62,   414,
     415,     3,   161,   298,     3,   300,   301,   302,    72,    73,
      74,    75,     3,   253,   253,   430,   253,    81,   253,   434,
      79,   436,   437,   177,   439,   118,   110,    91,   323,   253,
      94,   115,   252,   187,   127,    30,   190,   252,   102,   252,
     133,   105,   127,   252,   108,   109,   156,   111,   112,   113,
     127,   110,     3,    47,   118,   134,   115,    20,   252,   152,
     144,   125,   477,   127,   156,   253,   125,   124,   108,   133,
     134,   225,     3,     4,     5,     6,     7,     8,   253,   253,
     253,   253,   252,     3,     3,   144,    17,    18,   152,   182,
     252,   128,   156,     3,    25,   377,   378,   161,    19,    20,
      21,    22,    23,    24,    25,    26,    27,    28,    29,    30,
     157,     3,    43,   206,     3,    46,   253,   253,   182,    50,
      26,    27,    28,    29,    30,    47,   190,   109,   253,   253,
     253,   253,    63,    64,    65,   253,    20,   419,   252,   252,
     422,   423,   206,   252,   426,   252,   250,   211,   128,    20,
     252,     3,   253,   225,     3,     4,     5,     6,     7,     8,
      20,   183,   154,   125,     4,     8,    97,   128,    17,    18,
     250,   465,   236,   237,   250,   239,    25,   241,   242,   406,
     128,   160,   372,   258,    33,   249,   250,    24,    37,   253,
     178,    39,   108,   439,    43,    86,   373,   380,   272,   288,
     131,    50,    -1,     3,     4,     5,     6,     7,     8,   118,
      -1,    -1,    -1,   495,    63,    64,    65,    17,    18,    -1,
      -1,    -1,    -1,    -1,   506,    25,    -1,    -1,    -1,    -1,
       3,     4,     5,     6,     7,     8,    24,    25,    26,    27,
      28,    29,    30,    43,    17,    18,    -1,    -1,    97,    -1,
      50,    -1,    25,    26,    22,    23,    24,    25,    26,    27,
      28,    29,    30,    63,    64,    65,    -1,    -1,    -1,    -1,
      43,     3,     4,     5,     6,     7,     8,    50,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    17,    18,   218,    -1,    -1,
      63,    64,    65,    25,    -1,    -1,    -1,    97,    20,    21,
      22,    23,    24,    25,    26,    27,    28,    29,    30,    -1,
      -1,    43,    -1,   244,   245,   246,   247,   248,    50,    -1,
      -1,   252,    -1,    -1,    97,    -1,    -1,    -1,    -1,    -1,
      -1,    63,    64,    65,    21,    22,    23,    24,    25,    26,
      27,    28,    29,    30,   193,     3,     4,     5,     6,     7,
       8,    23,    24,    25,    26,    27,    28,    29,    30,    17,
      18,    -1,    -1,    -1,    -1,    97,    -1,    25,     3,     4,
       5,     6,     7,     8,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    17,    18,    -1,    43,    -1,    -1,    -1,    -1,
      25,    -1,    50,    -1,    -1,   244,   245,   246,   247,   248,
      -1,    -1,    -1,   252,    -1,    63,    64,    65,    43,    -1,
      -1,    -1,    -1,    -1,    -1,    50,    -1,    -1,    -1,    -1,
      78,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    63,    64,
      65,    -1,    -1,    -1,    -1,    -1,   236,    -1,    -1,    97,
      -1,    -1,    -1,    78,   244,   245,   246,   247,   248,    -1,
      -1,    -1,   252,    -1,    -1,   187,    -1,    -1,    -1,    -1,
      -1,    -1,    97,     3,     4,     5,     6,     7,     8,    -1,
      -1,   244,   245,   246,   247,   248,    -1,    17,    18,   252,
      -1,    -1,    -1,    -1,    -1,    25,     3,     4,     5,     6,
       7,     8,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      17,    18,    -1,    43,    -1,    -1,    -1,    -1,    25,    -1,
      50,    -1,   244,   245,   246,   247,   248,    -1,    -1,    -1,
     252,    -1,    -1,    63,    64,    65,    43,    -1,    -1,    -1,
      -1,    -1,    -1,    50,    -1,    -1,    -1,    -1,    78,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    63,    64,    65,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    97,    -1,    -1,
      -1,    78,    13,    14,    15,    16,    -1,    18,    19,    20,
      21,    22,    23,    24,    25,    26,    27,    28,    29,    30,
      97,    -1,    -1,    -1,    -1,    -1,   244,   245,   246,   247,
     248,    -1,    -1,    -1,   252,    -1,    -1,    -1,    -1,    -1,
       3,     4,     5,     6,     7,     8,    -1,    -1,    -1,   244,
     245,   246,   247,   248,    17,    18,    -1,   252,    -1,    -1,
      -1,    -1,    25,     3,     4,     5,     6,     7,     8,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    17,    18,    -1,
      43,    -1,    -1,    -1,    -1,    25,    -1,    50,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      63,    64,    65,    43,    -1,    -1,    -1,    -1,    -1,    -1,
      50,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    63,    64,    65,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    97,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   244,   245,   246,   247,   248,    -1,
      -1,    -1,   252,    -1,    -1,    -1,    -1,    97,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   244,   245,   246,
     247,   248,    -1,    -1,    -1,   252,    10,    11,    12,    13,
      14,    15,    16,    -1,    18,    19,    20,    21,    22,    23,
      24,    25,    26,    27,    28,    29,    30,    10,    11,    12,
      13,    14,    15,    16,    -1,    18,    19,    20,    21,    22,
      23,    24,    25,    26,    27,    28,    29,    30,    10,    11,
      12,    13,    14,    15,    16,    -1,    18,    19,    20,    21,
      22,    23,    24,    25,    26,    27,    28,    29,    30,    10,
      11,    12,    13,    14,    15,    16,    -1,    18,    19,    20,
      21,    22,    23,    24,    25,    26,    27,    28,    29,    30,
      10,    11,    12,    13,    14,    15,    16,    -1,    18,    19,
      20,    21,    22,    23,    24,    25,    26,    27,    28,    29,
      30,   244,   245,   246,   247,   248,    -1,    -1,    -1,   252,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   244,   245,   246,   247,   248,    -1,
      -1,    -1,   252,   105,    10,    11,    12,    13,    14,    15,
      16,    -1,    18,    19,    20,    21,    22,    23,    24,    25,
      26,    27,    28,    29,    30,    10,    11,    12,    13,    14,
      15,    16,    -1,    18,    19,    20,    21,    22,    23,    24,
      25,    26,    27,    28,    29,    30,    -1,    -1,    10,    11,
      12,    13,    14,    15,    16,    -1,    18,    19,    20,    21,
      22,    23,    24,    25,    26,    27,    28,    29,    30,    10,
      11,    12,    13,    14,    15,    16,    -1,    18,    19,    20,
      21,    22,    23,    24,    25,    26,    27,    28,    29,    30,
      12,    13,    14,    15,    16,    -1,    18,    19,    20,    21,
      22,    23,    24,    25,    26,    27,    28,    29,    30,   253,
      72,    73,    74,    75,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    10,    11,    12,    13,    14,    15,    16,
     253,    18,    19,    20,    21,    22,    23,    24,    25,    26,
      27,    28,    29,    30,    -1,    -1,    -1,    -1,   250,   111,
     112,   113,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    10,    11,    12,    13,    14,    15,    16,   250,
      18,    19,    20,    21,    22,    23,    24,    25,    26,    27,
      28,    29,    30,    10,    11,    12,    13,    14,    15,    16,
     250,    18,    19,    20,    21,    22,    23,    24,    25,    26,
      27,    28,    29,    30,    10,    11,    12,    13,    14,    15,
      16,    -1,    18,    19,    20,    21,    22,    23,    24,    25,
      26,    27,    28,    29,    30,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    10,    11,    12,
      13,    14,    15,    16,   250,    18,    19,    20,    21,    22,
      23,    24,    25,    26,    27,    28,    29,    30,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   250,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   105,   241,
     242,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    94,    -1,
      -1,    -1,    -1,    -1,    -1,   236,    10,    11,    12,    13,
      14,    15,    16,    -1,    18,    19,    20,    21,    22,    23,
      24,    25,    26,    27,    28,    29,    30,    -1,    -1,     3,
      -1,    94,    -1,    -1,   211,    39,    10,    11,    12,    13,
      14,    15,    16,    -1,    18,    19,    20,    21,    22,    23,
      24,    25,    26,    27,    28,    29,    30,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    38,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   211,    -1,    -1,    -1,    81,    10,    11,
      12,    13,    14,    15,    16,    -1,    18,    19,    20,    21,
      22,    23,    24,    25,    26,    27,    28,    29,    30,    -1,
      -1,    -1,    -1,    -1,    36,    10,    11,    12,    13,    14,
      15,    16,    -1,    18,    19,    20,    21,    22,    23,    24,
      25,    26,    27,    28,    29,    30,    11,    12,    13,    14,
      15,    16,    -1,    18,    19,    20,    21,    22,    23,    24,
      25,    26,    27,    28,    29,    30,    13,    14,    15,    16,
      -1,    18,    19,    20,    21,    22,    23,    24,    25,    26,
      27,    28,    29,    30
};

/* YYSTOS[STATE-NUM] -- The (internal number of the) accessing
   symbol of state STATE-NUM.  */
static const unsigned short int yystos[] =
{
       0,    61,    80,   121,   177,   187,   190,   225,   255,   256,
     257,   284,   288,   296,   297,   300,   302,   311,    68,   184,
     208,   310,   285,   290,   290,   268,     8,   312,   313,   290,
     298,     0,   256,   249,   114,   301,   301,   207,     3,   105,
     115,   144,   169,   286,    79,   110,   115,   125,   144,   291,
     291,     3,     4,     5,     6,     7,     8,    17,    18,    25,
      26,    33,    43,    50,    63,    64,    65,    84,    85,    97,
     110,   201,   202,   203,   206,   244,   245,   246,   247,   248,
     252,   269,   270,   314,     9,    20,   250,   115,   144,     3,
     252,   271,   272,   273,   276,   283,   249,    18,     3,     3,
     301,   251,   287,     3,   286,   105,   250,     3,     3,   251,
     252,     9,   314,    97,   314,   314,   314,   236,   314,   319,
     252,   252,   252,   252,   252,   252,   314,   105,   250,     3,
      10,    11,    12,    13,    14,    15,    16,    18,    19,    20,
      21,    22,    23,    24,    25,    26,    27,    28,    29,    30,
      38,   275,   314,   314,   313,   251,   275,   257,   271,   190,
     250,    62,   118,   133,   152,   182,   206,   277,   279,    38,
     274,    97,     3,    26,   237,   258,   228,   271,     3,   190,
     252,   292,   190,   292,     3,   314,   315,   316,   314,   252,
     314,   319,    91,    94,   236,   257,   314,   315,    46,   131,
     218,   315,   317,   314,   314,    26,   314,   253,   271,   270,
     314,   314,   314,   314,   314,     6,    18,   154,   252,    13,
      14,    16,   314,    33,    37,   193,   252,   314,   314,   314,
     314,   314,   314,   314,   314,   314,   314,   314,     3,     3,
     253,   253,     3,   299,   272,   133,   182,   280,   273,   127,
     163,   278,     3,   115,   177,   251,   252,   308,   309,   314,
     161,   264,   271,   258,   287,     3,   295,     3,   267,   232,
     257,   295,   232,   257,   250,   253,   257,   211,    91,    94,
     314,   314,   253,   105,   253,   253,   314,   250,   250,   253,
     253,   258,     6,   154,   257,   315,   314,   314,   252,    36,
     252,   252,   252,   257,   275,    20,   251,   250,   258,   278,
     278,   127,   156,   273,   127,     3,   106,   116,   128,   166,
     303,   304,   305,   274,    47,   134,   265,   258,    20,   156,
     250,   289,   250,   253,   252,   293,   289,   289,   293,   289,
     315,   253,   314,   314,    94,   211,   314,   105,   124,   318,
     318,   108,   259,   253,   253,   257,   315,   314,   257,   257,
     257,   253,   314,     3,     3,   264,   273,   314,   156,   228,
     281,   282,   273,   252,   308,   116,   128,   252,   252,   128,
     250,   253,     3,   257,   260,   314,   314,    78,   314,   157,
       3,     3,    78,   294,   314,   250,   289,   289,    94,   314,
     102,   253,   315,   314,   253,   253,    47,   109,   263,   253,
     253,   253,   253,   253,    20,    20,   251,   265,   314,   252,
     282,   303,   252,   252,   267,   267,   252,   304,   308,   307,
     250,    39,    81,   261,   250,   128,    20,   250,   253,   252,
     314,   253,    72,    73,    74,    75,   111,   112,   113,   241,
     242,   260,   314,   264,   314,   314,     3,   267,   253,   267,
     267,   253,   253,   267,   306,   314,   314,   225,    78,   314,
      78,   314,   294,   253,   239,   262,   265,    20,   253,   308,
     253,   253,   253,    18,    40,    56,    78,   128,   154,   166,
     222,   261,   295,   253,   183,   125,   266,   314,   154,     4,
       4,     5,     6,     7,   128,   128,   252,   267,   267,   253
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
#line 331 "sql.ypp"
    { printf("> \n"); ((*yyvalp).ast_node) = (((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (2))].yystate.yysemantics.yysval.ast_node); /*output($$, 1);*/ ;}
    break;

  case 3:

/* Line 936 of glr.c  */
#line 332 "sql.ypp"
    { printf("> \n"); ((*yyvalp).ast_node) = (((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (3))].yystate.yysemantics.yysval.ast_node); /*output($$, 1);*/ ;}
    break;

  case 4:

/* Line 936 of glr.c  */
#line 337 "sql.ypp"
    { ((*yyvalp).ast_node)=(((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (1))].yystate.yysemantics.yysval.ast_node); parsetreeroot=((*yyvalp).ast_node); output(parsetreeroot,1); puts("This is a select statement"); ;}
    break;

  case 5:

/* Line 936 of glr.c  */
#line 342 "sql.ypp"
    {
		((*yyvalp).ast_node) = newQueryStmt(t_query_stmt, NULL, (((yyGLRStackItem const *)yyvsp)[YYFILL ((2) - (3))].yystate.yysemantics.yysval.intval), (((yyGLRStackItem const *)yyvsp)[YYFILL ((3) - (3))].yystate.yysemantics.yysval.ast_node),
				NULL, NULL, NULL, NULL, NULL, NULL, NULL);
		puts("select_stmt1");
	;}
    break;

  case 6:

/* Line 936 of glr.c  */
#line 349 "sql.ypp"
    {
			((*yyvalp).ast_node) = newQueryStmt(t_query_stmt, NULL, (((yyGLRStackItem const *)yyvsp)[YYFILL ((2) - (11))].yystate.yysemantics.yysval.intval), (((yyGLRStackItem const *)yyvsp)[YYFILL ((3) - (11))].yystate.yysemantics.yysval.ast_node), (((yyGLRStackItem const *)yyvsp)[YYFILL ((5) - (11))].yystate.yysemantics.yysval.ast_node), (((yyGLRStackItem const *)yyvsp)[YYFILL ((6) - (11))].yystate.yysemantics.yysval.ast_node), (((yyGLRStackItem const *)yyvsp)[YYFILL ((7) - (11))].yystate.yysemantics.yysval.ast_node), (((yyGLRStackItem const *)yyvsp)[YYFILL ((8) - (11))].yystate.yysemantics.yysval.ast_node), (((yyGLRStackItem const *)yyvsp)[YYFILL ((9) - (11))].yystate.yysemantics.yysval.ast_node), (((yyGLRStackItem const *)yyvsp)[YYFILL ((10) - (11))].yystate.yysemantics.yysval.ast_node), (((yyGLRStackItem const *)yyvsp)[YYFILL ((11) - (11))].yystate.yysemantics.yysval.ast_node));
		/*	output($$, 1);
			puts("select_stmt2>>>>>>>>");
			if($6!=NULL)
			{
			struct Where_list * curt=(struct Where_list *)$6;
			struct Node *cur=(struct Node *)(curt->next);
		//	output(cur,0);
			departwc(cur,$5);
				puts("partree complete!!!");
				
			}
			puts("after>>>>>>>>>>>>>");
			output($5, 1);
			*/
		;}
    break;

  case 7:

/* Line 936 of glr.c  */
#line 368 "sql.ypp"
    { ((*yyvalp).ast_node)= NULL; puts("opt_where is null"); ;}
    break;

  case 8:

/* Line 936 of glr.c  */
#line 369 "sql.ypp"
    { ((*yyvalp).ast_node) = newWhereList(t_where_list, NULL, (((yyGLRStackItem const *)yyvsp)[YYFILL ((2) - (2))].yystate.yysemantics.yysval.ast_node)); puts("opt_where");;}
    break;

  case 9:

/* Line 936 of glr.c  */
#line 371 "sql.ypp"
    { ((*yyvalp).ast_node)=NULL; puts("opt_groupby is null"); ;}
    break;

  case 10:

/* Line 936 of glr.c  */
#line 373 "sql.ypp"
    {
     	((*yyvalp).ast_node) = newGroupbyList(t_groupby_list, NULL, (((yyGLRStackItem const *)yyvsp)[YYFILL ((3) - (4))].yystate.yysemantics.yysval.ast_node), (((yyGLRStackItem const *)yyvsp)[YYFILL ((4) - (4))].yystate.yysemantics.yysval.intval));
      	puts("opt_groupby");
	;}
    break;

  case 11:

/* Line 936 of glr.c  */
#line 380 "sql.ypp"
    { 
		((*yyvalp).ast_node)=newGroupbyExpr(t_groupby_expr, (((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (2))].yystate.yysemantics.yysval.ast_node), (((yyGLRStackItem const *)yyvsp)[YYFILL ((2) - (2))].yystate.yysemantics.yysval.intval), NULL);
		puts("groupby_list1");
	;}
    break;

  case 12:

/* Line 936 of glr.c  */
#line 385 "sql.ypp"
    {
     	((*yyvalp).ast_node)=newGroupbyExpr(t_groupby_expr, (((yyGLRStackItem const *)yyvsp)[YYFILL ((3) - (4))].yystate.yysemantics.yysval.ast_node), (((yyGLRStackItem const *)yyvsp)[YYFILL ((4) - (4))].yystate.yysemantics.yysval.intval), (((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (4))].yystate.yysemantics.yysval.ast_node));
     	puts("groupby_list2");
    ;}
    break;

  case 13:

/* Line 936 of glr.c  */
#line 391 "sql.ypp"
    { ((*yyvalp).intval) = 0; ;}
    break;

  case 14:

/* Line 936 of glr.c  */
#line 392 "sql.ypp"
    { ((*yyvalp).intval) = 0; ;}
    break;

  case 15:

/* Line 936 of glr.c  */
#line 393 "sql.ypp"
    { ((*yyvalp).intval) = 1; ;}
    break;

  case 16:

/* Line 936 of glr.c  */
#line 396 "sql.ypp"
    { ((*yyvalp).intval) = 0; ;}
    break;

  case 17:

/* Line 936 of glr.c  */
#line 397 "sql.ypp"
    { ((*yyvalp).intval) = 1; ;}
    break;

  case 18:

/* Line 936 of glr.c  */
#line 400 "sql.ypp"
    { ((*yyvalp).ast_node)=NULL; puts("opt_having is null"); ;}
    break;

  case 19:

/* Line 936 of glr.c  */
#line 402 "sql.ypp"
    {
		((*yyvalp).ast_node)=newHavingList(t_having_list, NULL, (((yyGLRStackItem const *)yyvsp)[YYFILL ((2) - (2))].yystate.yysemantics.yysval.ast_node));
		puts("opt_having2");
	;}
    break;

  case 20:

/* Line 936 of glr.c  */
#line 408 "sql.ypp"
    { ((*yyvalp).ast_node)=NULL; puts("opt_orederby is null"); ;}
    break;

  case 21:

/* Line 936 of glr.c  */
#line 410 "sql.ypp"
    {
		((*yyvalp).ast_node)=newOrderbyList(t_groupby_list, NULL, (((yyGLRStackItem const *)yyvsp)[YYFILL ((3) - (3))].yystate.yysemantics.yysval.ast_node));
		puts("opt_orederby2");
	;}
    break;

  case 22:

/* Line 936 of glr.c  */
#line 416 "sql.ypp"
    {((*yyvalp).ast_node) = NULL; puts("opt_limit is null");;}
    break;

  case 23:

/* Line 936 of glr.c  */
#line 418 "sql.ypp"
    {
		((*yyvalp).ast_node)=newLimitExpr(t_limit_expr, NULL, (((yyGLRStackItem const *)yyvsp)[YYFILL ((2) - (2))].yystate.yysemantics.yysval.ast_node));
		puts("opt_limit2");
	;}
    break;

  case 24:

/* Line 936 of glr.c  */
#line 423 "sql.ypp"
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
#line 432 "sql.ypp"
    { ((*yyvalp).ast_node)=NULL; puts("opt_into_list is null"); ;}
    break;

  case 26:

/* Line 936 of glr.c  */
#line 433 "sql.ypp"
    {  ;}
    break;

  case 27:

/* Line 936 of glr.c  */
#line 436 "sql.ypp"
    { Columns * n=(Columns *)malloc(sizeof(struct Columns));n->type=t_column;n->parameter1=(((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (1))].yystate.yysemantics.yysval.strval);((*yyvalp).ast_node)=(Node*)n; ;}
    break;

  case 28:

/* Line 936 of glr.c  */
#line 437 "sql.ypp"
    { Columns * n=(Columns *)malloc(sizeof(struct Columns));n->type=t_column;n->parameter1=(((yyGLRStackItem const *)yyvsp)[YYFILL ((3) - (3))].yystate.yysemantics.yysval.strval);n->next=(((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (3))].yystate.yysemantics.yysval.ast_node);((*yyvalp).ast_node)=(Node*)n; ;}
    break;

  case 29:

/* Line 936 of glr.c  */
#line 440 "sql.ypp"
    { ((*yyvalp).intval) = 0; ;}
    break;

  case 30:

/* Line 936 of glr.c  */
#line 441 "sql.ypp"
    { if(((*yyvalp).intval) & 1) yyerror("duplicate ALL option"); ((*yyvalp).intval) = (((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (2))].yystate.yysemantics.yysval.intval) | 1; ;}
    break;

  case 31:

/* Line 936 of glr.c  */
#line 442 "sql.ypp"
    { if(((*yyvalp).intval) & 2) yyerror("duplicate DISTINCT option"); ((*yyvalp).intval) = (((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (2))].yystate.yysemantics.yysval.intval) | 2; ;}
    break;

  case 32:

/* Line 936 of glr.c  */
#line 443 "sql.ypp"
    { if(((*yyvalp).intval) & 4) yyerror("duplicate DISTINCTROW option"); ((*yyvalp).intval) = (((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (2))].yystate.yysemantics.yysval.intval) | 4; ;}
    break;

  case 33:

/* Line 936 of glr.c  */
#line 444 "sql.ypp"
    { if(((*yyvalp).intval) & 8) yyerror("duplicate HIGH_PRIORITY option"); ((*yyvalp).intval) = (((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (2))].yystate.yysemantics.yysval.intval) | 8; ;}
    break;

  case 34:

/* Line 936 of glr.c  */
#line 445 "sql.ypp"
    { if(((*yyvalp).intval) & 16) yyerror("duplicate STRAIGHT_JOIN option"); ((*yyvalp).intval) = (((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (2))].yystate.yysemantics.yysval.intval) | 16; ;}
    break;

  case 35:

/* Line 936 of glr.c  */
#line 446 "sql.ypp"
    { if(((*yyvalp).intval) & 32) yyerror("duplicate SQL_SMALL_RESULT option"); ((*yyvalp).intval) = (((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (2))].yystate.yysemantics.yysval.intval) | 32; ;}
    break;

  case 36:

/* Line 936 of glr.c  */
#line 447 "sql.ypp"
    { if(((*yyvalp).intval) & 64) yyerror("duplicate SQL_BIG_RESULT option"); ((*yyvalp).intval) = (((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (2))].yystate.yysemantics.yysval.intval) | 64; ;}
    break;

  case 37:

/* Line 936 of glr.c  */
#line 448 "sql.ypp"
    { if(((*yyvalp).intval) & 128) yyerror("duplicate SQL_CALC_FOUND_ROWS option"); ((*yyvalp).intval) = (((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (2))].yystate.yysemantics.yysval.intval) | 128; ;}
    break;

  case 38:

/* Line 936 of glr.c  */
#line 452 "sql.ypp"
    {
			((*yyvalp).ast_node) = newSelectList(t_select_list, 0, (((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (1))].yystate.yysemantics.yysval.ast_node), NULL);
			puts("select_expr_list1"); 
		;}
    break;

  case 39:

/* Line 936 of glr.c  */
#line 457 "sql.ypp"
    {
    		((*yyvalp).ast_node) = newSelectList(t_select_list, 0, (((yyGLRStackItem const *)yyvsp)[YYFILL ((3) - (3))].yystate.yysemantics.yysval.ast_node), (((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (3))].yystate.yysemantics.yysval.ast_node));
    		puts("select_expr_list2");
    	;}
    break;

  case 40:

/* Line 936 of glr.c  */
#line 462 "sql.ypp"
    {
			((*yyvalp).ast_node) = newSelectList(t_select_list, 1, NULL, NULL);
			puts("select_expr_list3");
		;}
    break;

  case 41:

/* Line 936 of glr.c  */
#line 469 "sql.ypp"
    {
			// newSelectExpr(nodetype type, char *ascolname, Node * colname) 
			
			((*yyvalp).ast_node) = newSelectExpr(t_select_expr, (((yyGLRStackItem const *)yyvsp)[YYFILL ((2) - (2))].yystate.yysemantics.yysval.strval), (((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (2))].yystate.yysemantics.yysval.ast_node));
			puts("select_expr");
		;}
    break;

  case 42:

/* Line 936 of glr.c  */
#line 478 "sql.ypp"
    {
		((*yyvalp).ast_node)=newFromList(t_from_list, (((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (1))].yystate.yysemantics.yysval.ast_node), NULL);
		puts("table_references1");
	;}
    break;

  case 43:

/* Line 936 of glr.c  */
#line 483 "sql.ypp"
    {
   		((*yyvalp).ast_node)=newFromList(t_from_list, (((yyGLRStackItem const *)yyvsp)[YYFILL ((3) - (3))].yystate.yysemantics.yysval.ast_node), (((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (3))].yystate.yysemantics.yysval.ast_node));
    	puts("table_references2");
    ;}
    break;

  case 44:

/* Line 936 of glr.c  */
#line 489 "sql.ypp"
    { ((*yyvalp).ast_node)=(((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (1))].yystate.yysemantics.yysval.ast_node);puts("table_referen1");;}
    break;

  case 45:

/* Line 936 of glr.c  */
#line 490 "sql.ypp"
    { ((*yyvalp).ast_node)=(((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (1))].yystate.yysemantics.yysval.ast_node);puts("table_referen2");;}
    break;

  case 46:

/* Line 936 of glr.c  */
#line 495 "sql.ypp"
    {
	    ((*yyvalp).ast_node)=newTable(t_table, NULL,(((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (2))].yystate.yysemantics.yysval.strval), (((yyGLRStackItem const *)yyvsp)[YYFILL ((2) - (2))].yystate.yysemantics.yysval.strval), 0, NULL);
	    puts("table_factor1");
    ;}
    break;

  case 47:

/* Line 936 of glr.c  */
#line 500 "sql.ypp"
    {
		((*yyvalp).ast_node)=newTable(t_table, (((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (4))].yystate.yysemantics.yysval.strval), (((yyGLRStackItem const *)yyvsp)[YYFILL ((3) - (4))].yystate.yysemantics.yysval.strval), (((yyGLRStackItem const *)yyvsp)[YYFILL ((4) - (4))].yystate.yysemantics.yysval.strval), 0, NULL);
   		puts("table_factor2");;}
    break;

  case 48:

/* Line 936 of glr.c  */
#line 504 "sql.ypp"
    {
		((*yyvalp).ast_node)=newTable(t_table, NULL, NULL, (((yyGLRStackItem const *)yyvsp)[YYFILL ((3) - (3))].yystate.yysemantics.yysval.strval), 1, (((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (3))].yystate.yysemantics.yysval.ast_node));
		puts("table_factor3");
	;}
    break;

  case 49:

/* Line 936 of glr.c  */
#line 508 "sql.ypp"
    {((*yyvalp).ast_node)=(((yyGLRStackItem const *)yyvsp)[YYFILL ((2) - (3))].yystate.yysemantics.yysval.ast_node);;}
    break;

  case 52:

/* Line 936 of glr.c  */
#line 515 "sql.ypp"
    { ((*yyvalp).strval)=(((yyGLRStackItem const *)yyvsp)[YYFILL ((2) - (2))].yystate.yysemantics.yysval.strval); puts("opt_as_alias1");;}
    break;

  case 53:

/* Line 936 of glr.c  */
#line 516 "sql.ypp"
    { ((*yyvalp).strval)=(((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (1))].yystate.yysemantics.yysval.strval); puts("opt_as_alias2");;}
    break;

  case 54:

/* Line 936 of glr.c  */
#line 517 "sql.ypp"
    { ((*yyvalp).strval)=NULL; puts("opt_as_alias3");;}
    break;

  case 55:

/* Line 936 of glr.c  */
#line 533 "sql.ypp"
    {
		((*yyvalp).ast_node)=newJoin(t_join, (((yyGLRStackItem const *)yyvsp)[YYFILL ((2) - (5))].yystate.yysemantics.yysval.intval), (((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (5))].yystate.yysemantics.yysval.ast_node), (((yyGLRStackItem const *)yyvsp)[YYFILL ((4) - (5))].yystate.yysemantics.yysval.ast_node), (((yyGLRStackItem const *)yyvsp)[YYFILL ((5) - (5))].yystate.yysemantics.yysval.ast_node));
	;}
    break;

  case 56:

/* Line 936 of glr.c  */
#line 537 "sql.ypp"
    {
		((*yyvalp).ast_node)=newJoin(t_join, -1, (((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (3))].yystate.yysemantics.yysval.ast_node), (((yyGLRStackItem const *)yyvsp)[YYFILL ((3) - (3))].yystate.yysemantics.yysval.ast_node), NULL);
	;}
    break;

  case 57:

/* Line 936 of glr.c  */
#line 541 "sql.ypp"
    {
		Node *m=newCondition(t_condition, 1, (((yyGLRStackItem const *)yyvsp)[YYFILL ((5) - (5))].yystate.yysemantics.yysval.ast_node));
		((*yyvalp).ast_node)=newJoin(t_join, -1, (((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (5))].yystate.yysemantics.yysval.ast_node), (((yyGLRStackItem const *)yyvsp)[YYFILL ((3) - (5))].yystate.yysemantics.yysval.ast_node), m);
	;}
    break;

  case 58:

/* Line 936 of glr.c  */
#line 546 "sql.ypp"
    {
		((*yyvalp).ast_node)=newJoin(t_join, (((yyGLRStackItem const *)yyvsp)[YYFILL ((2) - (6))].yystate.yysemantics.yysval.intval)+(((yyGLRStackItem const *)yyvsp)[YYFILL ((3) - (6))].yystate.yysemantics.yysval.intval), (((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (6))].yystate.yysemantics.yysval.ast_node), (((yyGLRStackItem const *)yyvsp)[YYFILL ((5) - (6))].yystate.yysemantics.yysval.ast_node), (((yyGLRStackItem const *)yyvsp)[YYFILL ((6) - (6))].yystate.yysemantics.yysval.ast_node));
    ;}
    break;

  case 59:

/* Line 936 of glr.c  */
#line 550 "sql.ypp"
    {
		((*yyvalp).ast_node)=newJoin(t_join, 32 + (((yyGLRStackItem const *)yyvsp)[YYFILL ((3) - (5))].yystate.yysemantics.yysval.intval), (((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (5))].yystate.yysemantics.yysval.ast_node), (((yyGLRStackItem const *)yyvsp)[YYFILL ((5) - (5))].yystate.yysemantics.yysval.ast_node), NULL);
	;}
    break;

  case 60:

/* Line 936 of glr.c  */
#line 555 "sql.ypp"
    { ((*yyvalp).intval) = 0; ;}
    break;

  case 61:

/* Line 936 of glr.c  */
#line 556 "sql.ypp"
    { ((*yyvalp).intval) = 1; ;}
    break;

  case 62:

/* Line 936 of glr.c  */
#line 557 "sql.ypp"
    { ((*yyvalp).intval) = 2; ;}
    break;

  case 63:

/* Line 936 of glr.c  */
#line 560 "sql.ypp"
    { ((*yyvalp).intval) = 0; ;}
    break;

  case 64:

/* Line 936 of glr.c  */
#line 561 "sql.ypp"
    {((*yyvalp).intval) = 4; ;}
    break;

  case 65:

/* Line 936 of glr.c  */
#line 564 "sql.ypp"
    { ((*yyvalp).intval) = 8; ;}
    break;

  case 66:

/* Line 936 of glr.c  */
#line 565 "sql.ypp"
    { ((*yyvalp).intval) = 16; ;}
    break;

  case 67:

/* Line 936 of glr.c  */
#line 568 "sql.ypp"
    { ((*yyvalp).intval) = 8 + (((yyGLRStackItem const *)yyvsp)[YYFILL ((2) - (2))].yystate.yysemantics.yysval.intval); ;}
    break;

  case 68:

/* Line 936 of glr.c  */
#line 569 "sql.ypp"
    { ((*yyvalp).intval) = 16 + (((yyGLRStackItem const *)yyvsp)[YYFILL ((2) - (2))].yystate.yysemantics.yysval.intval); ;}
    break;

  case 69:

/* Line 936 of glr.c  */
#line 570 "sql.ypp"
    { ((*yyvalp).intval) = 0; ;}
    break;

  case 71:

/* Line 936 of glr.c  */
#line 574 "sql.ypp"
    {((*yyvalp).ast_node)=(((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (1))].yystate.yysemantics.yysval.ast_node);;}
    break;

  case 72:

/* Line 936 of glr.c  */
#line 582 "sql.ypp"
    {
    	((*yyvalp).ast_node)=newCondition(t_condition, 1, (((yyGLRStackItem const *)yyvsp)[YYFILL ((2) - (2))].yystate.yysemantics.yysval.ast_node));
    ;}
    break;

  case 73:

/* Line 936 of glr.c  */
#line 586 "sql.ypp"
    {
    	((*yyvalp).ast_node)=newCondition(t_condition, 2, (((yyGLRStackItem const *)yyvsp)[YYFILL ((3) - (4))].yystate.yysemantics.yysval.ast_node));
    ;}
    break;

  case 74:

/* Line 936 of glr.c  */
#line 609 "sql.ypp"
    { ((*yyvalp).ast_node)=(((yyGLRStackItem const *)yyvsp)[YYFILL ((2) - (3))].yystate.yysemantics.yysval.ast_node); ;}
    break;

  case 75:

/* Line 936 of glr.c  */
#line 614 "sql.ypp"
    { emit("STMT"); ;}
    break;

  case 76:

/* Line 936 of glr.c  */
#line 619 "sql.ypp"
    { emit("DELETEONE %d %s", (((yyGLRStackItem const *)yyvsp)[YYFILL ((2) - (7))].yystate.yysemantics.yysval.ast_node), (((yyGLRStackItem const *)yyvsp)[YYFILL ((4) - (7))].yystate.yysemantics.yysval.strval)); free((((yyGLRStackItem const *)yyvsp)[YYFILL ((4) - (7))].yystate.yysemantics.yysval.strval)); ;}
    break;

  case 77:

/* Line 936 of glr.c  */
#line 622 "sql.ypp"
    { ((*yyvalp).ast_node) = (((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (2))].yystate.yysemantics.yysval.ast_node) + 01; ;}
    break;

  case 78:

/* Line 936 of glr.c  */
#line 623 "sql.ypp"
    { ((*yyvalp).ast_node) = (((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (2))].yystate.yysemantics.yysval.ast_node) + 02; ;}
    break;

  case 79:

/* Line 936 of glr.c  */
#line 624 "sql.ypp"
    { ((*yyvalp).ast_node) = (((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (2))].yystate.yysemantics.yysval.ast_node) + 04; ;}
    break;

  case 80:

/* Line 936 of glr.c  */
#line 625 "sql.ypp"
    { ((*yyvalp).ast_node) = 0; ;}
    break;

  case 81:

/* Line 936 of glr.c  */
#line 631 "sql.ypp"
    {  ;}
    break;

  case 82:

/* Line 936 of glr.c  */
#line 633 "sql.ypp"
    {  ;}
    break;

  case 83:

/* Line 936 of glr.c  */
#line 635 "sql.ypp"
    {  ;}
    break;

  case 86:

/* Line 936 of glr.c  */
#line 643 "sql.ypp"
    { emit("DELETEMULTI %d %d %d", (((yyGLRStackItem const *)yyvsp)[YYFILL ((2) - (7))].yystate.yysemantics.yysval.ast_node), (((yyGLRStackItem const *)yyvsp)[YYFILL ((4) - (7))].yystate.yysemantics.yysval.ast_node), (((yyGLRStackItem const *)yyvsp)[YYFILL ((6) - (7))].yystate.yysemantics.yysval.ast_node)); ;}
    break;

  case 87:

/* Line 936 of glr.c  */
#line 648 "sql.ypp"
    { emit("STMT"); ;}
    break;

  case 88:

/* Line 936 of glr.c  */
#line 654 "sql.ypp"
    { emit("INSERTVALS %d %d %s", (((yyGLRStackItem const *)yyvsp)[YYFILL ((2) - (8))].yystate.yysemantics.yysval.ast_node), (((yyGLRStackItem const *)yyvsp)[YYFILL ((7) - (8))].yystate.yysemantics.yysval.ast_node), (((yyGLRStackItem const *)yyvsp)[YYFILL ((4) - (8))].yystate.yysemantics.yysval.strval)); free((((yyGLRStackItem const *)yyvsp)[YYFILL ((4) - (8))].yystate.yysemantics.yysval.strval)) ;}
    break;

  case 90:

/* Line 936 of glr.c  */
#line 658 "sql.ypp"
    { emit("DUPUPDATE %d", (((yyGLRStackItem const *)yyvsp)[YYFILL ((5) - (5))].yystate.yysemantics.yysval.ast_node)); ;}
    break;

  case 99:

/* Line 936 of glr.c  */
#line 672 "sql.ypp"
    { ((*yyvalp).ast_node)=NULL; ;}
    break;

  case 106:

/* Line 936 of glr.c  */
#line 688 "sql.ypp"
    { emit("INSERTASGN %d %d %s", (((yyGLRStackItem const *)yyvsp)[YYFILL ((2) - (7))].yystate.yysemantics.yysval.ast_node), (((yyGLRStackItem const *)yyvsp)[YYFILL ((6) - (7))].yystate.yysemantics.yysval.ast_node), (((yyGLRStackItem const *)yyvsp)[YYFILL ((4) - (7))].yystate.yysemantics.yysval.strval)); free((((yyGLRStackItem const *)yyvsp)[YYFILL ((4) - (7))].yystate.yysemantics.yysval.strval)) ;}
    break;

  case 107:

/* Line 936 of glr.c  */
#line 693 "sql.ypp"
    { emit("INSERTSELECT %d %s", (((yyGLRStackItem const *)yyvsp)[YYFILL ((2) - (7))].yystate.yysemantics.yysval.ast_node), (((yyGLRStackItem const *)yyvsp)[YYFILL ((4) - (7))].yystate.yysemantics.yysval.strval)); free((((yyGLRStackItem const *)yyvsp)[YYFILL ((4) - (7))].yystate.yysemantics.yysval.strval)); ;}
    break;

  case 112:

/* Line 936 of glr.c  */
#line 712 "sql.ypp"
    { emit("STMT"); ;}
    break;

  case 113:

/* Line 936 of glr.c  */
#line 718 "sql.ypp"
    { emit("REPLACEVALS %d %d %s", (((yyGLRStackItem const *)yyvsp)[YYFILL ((2) - (8))].yystate.yysemantics.yysval.ast_node), (((yyGLRStackItem const *)yyvsp)[YYFILL ((7) - (8))].yystate.yysemantics.yysval.ast_node), (((yyGLRStackItem const *)yyvsp)[YYFILL ((4) - (8))].yystate.yysemantics.yysval.strval)); free((((yyGLRStackItem const *)yyvsp)[YYFILL ((4) - (8))].yystate.yysemantics.yysval.strval)) ;}
    break;

  case 114:

/* Line 936 of glr.c  */
#line 724 "sql.ypp"
    { emit("REPLACEASGN %d %d %s", (((yyGLRStackItem const *)yyvsp)[YYFILL ((2) - (7))].yystate.yysemantics.yysval.ast_node), (((yyGLRStackItem const *)yyvsp)[YYFILL ((6) - (7))].yystate.yysemantics.yysval.ast_node), (((yyGLRStackItem const *)yyvsp)[YYFILL ((4) - (7))].yystate.yysemantics.yysval.strval)); free((((yyGLRStackItem const *)yyvsp)[YYFILL ((4) - (7))].yystate.yysemantics.yysval.strval)) ;}
    break;

  case 115:

/* Line 936 of glr.c  */
#line 729 "sql.ypp"
    { emit("REPLACESELECT %d %s", (((yyGLRStackItem const *)yyvsp)[YYFILL ((2) - (7))].yystate.yysemantics.yysval.ast_node), (((yyGLRStackItem const *)yyvsp)[YYFILL ((4) - (7))].yystate.yysemantics.yysval.strval)); free((((yyGLRStackItem const *)yyvsp)[YYFILL ((4) - (7))].yystate.yysemantics.yysval.strval)); ;}
    break;

  case 116:

/* Line 936 of glr.c  */
#line 733 "sql.ypp"
    { emit("STMT"); ;}
    break;

  case 117:

/* Line 936 of glr.c  */
#line 740 "sql.ypp"
    { emit("UPDATE %d %d %d", (((yyGLRStackItem const *)yyvsp)[YYFILL ((2) - (8))].yystate.yysemantics.yysval.ast_node), (((yyGLRStackItem const *)yyvsp)[YYFILL ((3) - (8))].yystate.yysemantics.yysval.ast_node), (((yyGLRStackItem const *)yyvsp)[YYFILL ((5) - (8))].yystate.yysemantics.yysval.ast_node)); ;}
    break;

  case 125:

/* Line 936 of glr.c  */
#line 763 "sql.ypp"
    { emit("STMT"); ;}
    break;

  case 126:

/* Line 936 of glr.c  */
#line 767 "sql.ypp"
    { emit("CREATEDATABASE %d %s", (((yyGLRStackItem const *)yyvsp)[YYFILL ((3) - (4))].yystate.yysemantics.yysval.ast_node), (((yyGLRStackItem const *)yyvsp)[YYFILL ((4) - (4))].yystate.yysemantics.yysval.strval)); free((((yyGLRStackItem const *)yyvsp)[YYFILL ((4) - (4))].yystate.yysemantics.yysval.strval)); ;}
    break;

  case 127:

/* Line 936 of glr.c  */
#line 768 "sql.ypp"
    { emit("CREATEDATABASE %d %s", (((yyGLRStackItem const *)yyvsp)[YYFILL ((3) - (4))].yystate.yysemantics.yysval.ast_node), (((yyGLRStackItem const *)yyvsp)[YYFILL ((4) - (4))].yystate.yysemantics.yysval.strval)); free((((yyGLRStackItem const *)yyvsp)[YYFILL ((4) - (4))].yystate.yysemantics.yysval.strval)); ;}
    break;

  case 130:

/* Line 936 of glr.c  */
#line 776 "sql.ypp"
    { emit("STMT"); ;}
    break;

  case 131:

/* Line 936 of glr.c  */
#line 780 "sql.ypp"
    { emit("CREATE %d %d %d %s", (((yyGLRStackItem const *)yyvsp)[YYFILL ((2) - (8))].yystate.yysemantics.yysval.ast_node), (((yyGLRStackItem const *)yyvsp)[YYFILL ((4) - (8))].yystate.yysemantics.yysval.ast_node), (((yyGLRStackItem const *)yyvsp)[YYFILL ((7) - (8))].yystate.yysemantics.yysval.ast_node), (((yyGLRStackItem const *)yyvsp)[YYFILL ((5) - (8))].yystate.yysemantics.yysval.strval)); free((((yyGLRStackItem const *)yyvsp)[YYFILL ((5) - (8))].yystate.yysemantics.yysval.strval)); ;}
    break;

  case 132:

/* Line 936 of glr.c  */
#line 784 "sql.ypp"
    { emit("CREATE %d %d %d %s.%s", (((yyGLRStackItem const *)yyvsp)[YYFILL ((2) - (10))].yystate.yysemantics.yysval.ast_node), (((yyGLRStackItem const *)yyvsp)[YYFILL ((4) - (10))].yystate.yysemantics.yysval.ast_node), (((yyGLRStackItem const *)yyvsp)[YYFILL ((9) - (10))].yystate.yysemantics.yysval.ast_node), (((yyGLRStackItem const *)yyvsp)[YYFILL ((5) - (10))].yystate.yysemantics.yysval.strval), (((yyGLRStackItem const *)yyvsp)[YYFILL ((7) - (10))].yystate.yysemantics.yysval.strval));
                          free((((yyGLRStackItem const *)yyvsp)[YYFILL ((5) - (10))].yystate.yysemantics.yysval.strval)); free((((yyGLRStackItem const *)yyvsp)[YYFILL ((7) - (10))].yystate.yysemantics.yysval.strval)); ;}
    break;

  case 133:

/* Line 936 of glr.c  */
#line 790 "sql.ypp"
    { emit("CREATESELECT %d %d %d %s", (((yyGLRStackItem const *)yyvsp)[YYFILL ((2) - (9))].yystate.yysemantics.yysval.ast_node), (((yyGLRStackItem const *)yyvsp)[YYFILL ((4) - (9))].yystate.yysemantics.yysval.ast_node), (((yyGLRStackItem const *)yyvsp)[YYFILL ((7) - (9))].yystate.yysemantics.yysval.ast_node), (((yyGLRStackItem const *)yyvsp)[YYFILL ((5) - (9))].yystate.yysemantics.yysval.strval)); free((((yyGLRStackItem const *)yyvsp)[YYFILL ((5) - (9))].yystate.yysemantics.yysval.strval)); ;}
    break;

  case 134:

/* Line 936 of glr.c  */
#line 794 "sql.ypp"
    { emit("CREATESELECT %d %d 0 %s", (((yyGLRStackItem const *)yyvsp)[YYFILL ((2) - (6))].yystate.yysemantics.yysval.ast_node), (((yyGLRStackItem const *)yyvsp)[YYFILL ((4) - (6))].yystate.yysemantics.yysval.ast_node), (((yyGLRStackItem const *)yyvsp)[YYFILL ((5) - (6))].yystate.yysemantics.yysval.strval)); free((((yyGLRStackItem const *)yyvsp)[YYFILL ((5) - (6))].yystate.yysemantics.yysval.strval)); ;}
    break;

  case 135:

/* Line 936 of glr.c  */
#line 799 "sql.ypp"
    { emit("CREATESELECT %d %d 0 %s.%s", (((yyGLRStackItem const *)yyvsp)[YYFILL ((2) - (11))].yystate.yysemantics.yysval.ast_node), (((yyGLRStackItem const *)yyvsp)[YYFILL ((4) - (11))].yystate.yysemantics.yysval.ast_node), (((yyGLRStackItem const *)yyvsp)[YYFILL ((5) - (11))].yystate.yysemantics.yysval.strval), (((yyGLRStackItem const *)yyvsp)[YYFILL ((7) - (11))].yystate.yysemantics.yysval.strval));
                              free((((yyGLRStackItem const *)yyvsp)[YYFILL ((5) - (11))].yystate.yysemantics.yysval.strval)); free((((yyGLRStackItem const *)yyvsp)[YYFILL ((7) - (11))].yystate.yysemantics.yysval.strval)); ;}
    break;

  case 136:

/* Line 936 of glr.c  */
#line 804 "sql.ypp"
    { emit("CREATESELECT %d %d 0 %s.%s", (((yyGLRStackItem const *)yyvsp)[YYFILL ((2) - (8))].yystate.yysemantics.yysval.ast_node), (((yyGLRStackItem const *)yyvsp)[YYFILL ((4) - (8))].yystate.yysemantics.yysval.ast_node), (((yyGLRStackItem const *)yyvsp)[YYFILL ((5) - (8))].yystate.yysemantics.yysval.strval), (((yyGLRStackItem const *)yyvsp)[YYFILL ((7) - (8))].yystate.yysemantics.yysval.strval));
                          free((((yyGLRStackItem const *)yyvsp)[YYFILL ((5) - (8))].yystate.yysemantics.yysval.strval)); free((((yyGLRStackItem const *)yyvsp)[YYFILL ((7) - (8))].yystate.yysemantics.yysval.strval)); ;}
    break;

  case 137:

/* Line 936 of glr.c  */
#line 808 "sql.ypp"
    { ;}
    break;

  case 138:

/* Line 936 of glr.c  */
#line 809 "sql.ypp"
    { ;}
    break;

  case 139:

/* Line 936 of glr.c  */
#line 812 "sql.ypp"
    { emit("STARTCOL"); ;}
    break;

  case 140:

/* Line 936 of glr.c  */
#line 813 "sql.ypp"
    { emit("COLUMNDEF %d %s", (((yyGLRStackItem const *)yyvsp)[YYFILL ((3) - (4))].yystate.yysemantics.yysval.intval), (((yyGLRStackItem const *)yyvsp)[YYFILL ((2) - (4))].yystate.yysemantics.yysval.strval)); free((((yyGLRStackItem const *)yyvsp)[YYFILL ((2) - (4))].yystate.yysemantics.yysval.strval)); ;}
    break;

  case 146:

/* Line 936 of glr.c  */
#line 822 "sql.ypp"
    { ((*yyvalp).ast_node) = 0; ;}
    break;

  case 147:

/* Line 936 of glr.c  */
#line 823 "sql.ypp"
    { emit("ATTR NOTNULL"); ((*yyvalp).ast_node) = (((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (3))].yystate.yysemantics.yysval.ast_node) + 1; ;}
    break;

  case 149:

/* Line 936 of glr.c  */
#line 825 "sql.ypp"
    { emit("ATTR DEFAULT STRING %s", (((yyGLRStackItem const *)yyvsp)[YYFILL ((3) - (3))].yystate.yysemantics.yysval.strval)); free((((yyGLRStackItem const *)yyvsp)[YYFILL ((3) - (3))].yystate.yysemantics.yysval.strval)); ((*yyvalp).ast_node) = (((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (3))].yystate.yysemantics.yysval.ast_node) + 1; ;}
    break;

  case 150:

/* Line 936 of glr.c  */
#line 826 "sql.ypp"
    { emit("ATTR DEFAULT NUMBER %d", (((yyGLRStackItem const *)yyvsp)[YYFILL ((3) - (3))].yystate.yysemantics.yysval.intval)); ((*yyvalp).ast_node) = (((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (3))].yystate.yysemantics.yysval.ast_node) + 1; ;}
    break;

  case 151:

/* Line 936 of glr.c  */
#line 827 "sql.ypp"
    { emit("ATTR DEFAULT FLOAT %g", (((yyGLRStackItem const *)yyvsp)[YYFILL ((3) - (3))].yystate.yysemantics.yysval.floatval)); ((*yyvalp).ast_node) = (((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (3))].yystate.yysemantics.yysval.ast_node) + 1; ;}
    break;

  case 152:

/* Line 936 of glr.c  */
#line 828 "sql.ypp"
    { emit("ATTR DEFAULT BOOL %d", (((yyGLRStackItem const *)yyvsp)[YYFILL ((3) - (3))].yystate.yysemantics.yysval.intval)); ((*yyvalp).ast_node) = (((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (3))].yystate.yysemantics.yysval.ast_node) + 1; ;}
    break;

  case 153:

/* Line 936 of glr.c  */
#line 829 "sql.ypp"
    { emit("ATTR AUTOINC"); ((*yyvalp).ast_node) = (((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (2))].yystate.yysemantics.yysval.ast_node) + 1; ;}
    break;

  case 154:

/* Line 936 of glr.c  */
#line 830 "sql.ypp"
    { emit("ATTR UNIQUEKEY %d", (((yyGLRStackItem const *)yyvsp)[YYFILL ((4) - (5))].yystate.yysemantics.yysval.ast_node)); ((*yyvalp).ast_node) = (((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (5))].yystate.yysemantics.yysval.ast_node) + 1; ;}
    break;

  case 155:

/* Line 936 of glr.c  */
#line 831 "sql.ypp"
    { emit("ATTR UNIQUEKEY"); ((*yyvalp).ast_node) = (((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (3))].yystate.yysemantics.yysval.ast_node) + 1; ;}
    break;

  case 156:

/* Line 936 of glr.c  */
#line 832 "sql.ypp"
    { emit("ATTR PRIKEY"); ((*yyvalp).ast_node) = (((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (3))].yystate.yysemantics.yysval.ast_node) + 1; ;}
    break;

  case 157:

/* Line 936 of glr.c  */
#line 833 "sql.ypp"
    { emit("ATTR PRIKEY"); ((*yyvalp).ast_node) = (((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (2))].yystate.yysemantics.yysval.ast_node) + 1; ;}
    break;

  case 158:

/* Line 936 of glr.c  */
#line 834 "sql.ypp"
    { emit("ATTR COMMENT %s", (((yyGLRStackItem const *)yyvsp)[YYFILL ((3) - (3))].yystate.yysemantics.yysval.strval)); free((((yyGLRStackItem const *)yyvsp)[YYFILL ((3) - (3))].yystate.yysemantics.yysval.strval)); ((*yyvalp).ast_node) = (((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (3))].yystate.yysemantics.yysval.ast_node) + 1; ;}
    break;

  case 160:

/* Line 936 of glr.c  */
#line 893 "sql.ypp"
    { emit("CREATESELECT %d", (((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (3))].yystate.yysemantics.yysval.ast_node)) ;}
    break;

  case 166:

/* Line 936 of glr.c  */
#line 907 "sql.ypp"
    { emit("STMT"); ;}
    break;

  case 170:

/* Line 936 of glr.c  */
#line 915 "sql.ypp"
    { if ((((yyGLRStackItem const *)yyvsp)[YYFILL ((2) - (3))].yystate.yysemantics.yysval.subtok) != 4) yyerror("bad set to @%s", (((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (3))].yystate.yysemantics.yysval.strval));
		 emit("SET %s", (((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (3))].yystate.yysemantics.yysval.strval)); free((((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (3))].yystate.yysemantics.yysval.strval)); ;}
    break;

  case 171:

/* Line 936 of glr.c  */
#line 917 "sql.ypp"
    { emit("SET %s", (((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (3))].yystate.yysemantics.yysval.strval)); free((((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (3))].yystate.yysemantics.yysval.strval)); ;}
    break;

  case 172:

/* Line 936 of glr.c  */
#line 922 "sql.ypp"
    { dataval d; d.string_val = (((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (1))].yystate.yysemantics.yysval.strval); ((*yyvalp).ast_node) = newExpr(t_name, d);  puts("expr1");;}
    break;

  case 173:

/* Line 936 of glr.c  */
#line 923 "sql.ypp"
    { dataval d; d.string_val = (((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (1))].yystate.yysemantics.yysval.strval); ((*yyvalp).ast_node) = newExpr(t_name, d); ;}
    break;

  case 174:

/* Line 936 of glr.c  */
#line 924 "sql.ypp"
    { ((*yyvalp).ast_node) = newColumn(t_name_name, (((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (3))].yystate.yysemantics.yysval.strval), (((yyGLRStackItem const *)yyvsp)[YYFILL ((3) - (3))].yystate.yysemantics.yysval.strval), NULL); puts("expr2");;}
    break;

  case 175:

/* Line 936 of glr.c  */
#line 925 "sql.ypp"
    { dataval d; d.string_val = (((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (1))].yystate.yysemantics.yysval.strval); ((*yyvalp).ast_node) = newExpr(t_stringval, d); puts("expr3");;}
    break;

  case 176:

/* Line 936 of glr.c  */
#line 926 "sql.ypp"
    { dataval d; d.int_val=(((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (1))].yystate.yysemantics.yysval.intval); ((*yyvalp).ast_node) = newExpr(t_intnum, d); puts("expr4");;}
    break;

  case 177:

/* Line 936 of glr.c  */
#line 927 "sql.ypp"
    { dataval d; d.double_val = (((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (1))].yystate.yysemantics.yysval.floatval); ((*yyvalp).ast_node) = newExpr(t_approxnum, d); ;}
    break;

  case 178:

/* Line 936 of glr.c  */
#line 928 "sql.ypp"
    { dataval d; d.bool_val = (((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (1))].yystate.yysemantics.yysval.intval); ((*yyvalp).ast_node) = newExpr(t_bool, d); ;}
    break;

  case 179:

/* Line 936 of glr.c  */
#line 931 "sql.ypp"
    {Expr_cal *n=(Expr_cal *)malloc(sizeof(struct Expr_cal));n->type=t_expr_cal;n->lnext=(((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (3))].yystate.yysemantics.yysval.ast_node);n->sign="+";n->rnext=(((yyGLRStackItem const *)yyvsp)[YYFILL ((3) - (3))].yystate.yysemantics.yysval.ast_node);((*yyvalp).ast_node)=(Node*)n;;}
    break;

  case 180:

/* Line 936 of glr.c  */
#line 932 "sql.ypp"
    {Expr_cal *n=(Expr_cal *)malloc(sizeof(struct Expr_cal ));n->type=t_expr_cal;n->lnext=(((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (3))].yystate.yysemantics.yysval.ast_node);n->sign="-";n->rnext=(((yyGLRStackItem const *)yyvsp)[YYFILL ((3) - (3))].yystate.yysemantics.yysval.ast_node);((*yyvalp).ast_node)=(Node*)n;;}
    break;

  case 181:

/* Line 936 of glr.c  */
#line 933 "sql.ypp"
    {Expr_cal *n=(Expr_cal *)malloc(sizeof(struct Expr_cal ));n->type=t_expr_cal;n->lnext=(((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (3))].yystate.yysemantics.yysval.ast_node);n->sign="*";n->rnext=(((yyGLRStackItem const *)yyvsp)[YYFILL ((3) - (3))].yystate.yysemantics.yysval.ast_node);((*yyvalp).ast_node)=(Node*)n;;}
    break;

  case 182:

/* Line 936 of glr.c  */
#line 934 "sql.ypp"
    {Expr_cal *n=(Expr_cal *)malloc(sizeof(struct Expr_cal ));n->type=t_expr_cal;n->lnext=(((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (3))].yystate.yysemantics.yysval.ast_node);n->sign="/";n->rnext=(((yyGLRStackItem const *)yyvsp)[YYFILL ((3) - (3))].yystate.yysemantics.yysval.ast_node);((*yyvalp).ast_node)=(Node*)n;;}
    break;

  case 183:

/* Line 936 of glr.c  */
#line 935 "sql.ypp"
    {Expr_cal *n=(Expr_cal *)malloc(sizeof(struct Expr_cal) );n->type=t_expr_cal;n->lnext=(((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (3))].yystate.yysemantics.yysval.ast_node);n->sign="%";n->rnext=(((yyGLRStackItem const *)yyvsp)[YYFILL ((3) - (3))].yystate.yysemantics.yysval.ast_node);((*yyvalp).ast_node)=(Node*)n;;}
    break;

  case 184:

/* Line 936 of glr.c  */
#line 936 "sql.ypp"
    {Expr_cal *n=(Expr_cal *)malloc(sizeof(struct  Expr_cal ));n->type=t_expr_cal;n->lnext=(((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (3))].yystate.yysemantics.yysval.ast_node);n->sign="MOD";n->rnext=(((yyGLRStackItem const *)yyvsp)[YYFILL ((3) - (3))].yystate.yysemantics.yysval.ast_node);((*yyvalp).ast_node)=(Node*)n;;}
    break;

  case 185:

/* Line 936 of glr.c  */
#line 937 "sql.ypp"
    {Expr_cal *n=(Expr_cal *)malloc(sizeof (struct Expr_cal ));n->type=t_expr_cal;    n->sign="--";n->rnext=(((yyGLRStackItem const *)yyvsp)[YYFILL ((2) - (2))].yystate.yysemantics.yysval.ast_node);((*yyvalp).ast_node)=(Node*)n;;}
    break;

  case 186:

/* Line 936 of glr.c  */
#line 938 "sql.ypp"
    {Expr_cal *n=(Expr_cal *)malloc(sizeof(struct Expr_cal) );n->type=t_expr_cal;n->lnext=(((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (3))].yystate.yysemantics.yysval.ast_node);n->sign="ANDOP";n->rnext=(((yyGLRStackItem const *)yyvsp)[YYFILL ((3) - (3))].yystate.yysemantics.yysval.ast_node);((*yyvalp).ast_node)=(Node*)n;;}
    break;

  case 187:

/* Line 936 of glr.c  */
#line 939 "sql.ypp"
    {Expr_cal *n=(Expr_cal *)malloc(sizeof(struct Expr_cal ));n->type=t_expr_cal;n->lnext=(((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (3))].yystate.yysemantics.yysval.ast_node);n->sign="OR";n->rnext=(((yyGLRStackItem const *)yyvsp)[YYFILL ((3) - (3))].yystate.yysemantics.yysval.ast_node);((*yyvalp).ast_node)=(Node*)n;;}
    break;

  case 188:

/* Line 936 of glr.c  */
#line 940 "sql.ypp"
    {Expr_cal *n=(Expr_cal *)malloc(sizeof(struct Expr_cal ));n->type=t_expr_cal;n->lnext=(((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (3))].yystate.yysemantics.yysval.ast_node);n->sign="XOR";n->rnext=(((yyGLRStackItem const *)yyvsp)[YYFILL ((3) - (3))].yystate.yysemantics.yysval.ast_node);((*yyvalp).ast_node)=(Node*)n;;}
    break;

  case 189:

/* Line 936 of glr.c  */
#line 941 "sql.ypp"
    { ((*yyvalp).ast_node) = newExpr_cal(t_expr_cal, "CMP", NULL, (((yyGLRStackItem const *)yyvsp)[YYFILL ((2) - (3))].yystate.yysemantics.yysval.subtok), (((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (3))].yystate.yysemantics.yysval.ast_node), (((yyGLRStackItem const *)yyvsp)[YYFILL ((3) - (3))].yystate.yysemantics.yysval.ast_node)); puts("="); ;}
    break;

  case 190:

/* Line 936 of glr.c  */
#line 942 "sql.ypp"
    {Expr_cal *n=(Expr_cal *)malloc(sizeof(struct Expr_cal ));n->type=t_expr_cal;n->lnext=(((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (5))].yystate.yysemantics.yysval.ast_node);n->cmp=(((yyGLRStackItem const *)yyvsp)[YYFILL ((2) - (5))].yystate.yysemantics.yysval.subtok);n->sign="CMP";n->rnext=(((yyGLRStackItem const *)yyvsp)[YYFILL ((4) - (5))].yystate.yysemantics.yysval.ast_node);((*yyvalp).ast_node)=(Node*)n;;}
    break;

  case 191:

/* Line 936 of glr.c  */
#line 943 "sql.ypp"
    {Expr_cal *n=(Expr_cal *)malloc(sizeof(struct Expr_cal ));n->type=t_expr_cal;n->lnext=(((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (6))].yystate.yysemantics.yysval.ast_node);n->cmp=(((yyGLRStackItem const *)yyvsp)[YYFILL ((2) - (6))].yystate.yysemantics.yysval.subtok);n->sign="CMP";n->parameter="ANY";n->rnext=(((yyGLRStackItem const *)yyvsp)[YYFILL ((5) - (6))].yystate.yysemantics.yysval.ast_node);((*yyvalp).ast_node)=(Node*)n;;}
    break;

  case 192:

/* Line 936 of glr.c  */
#line 944 "sql.ypp"
    {Expr_cal *n=(Expr_cal *)malloc(sizeof(struct Expr_cal ));n->type=t_expr_cal;n->lnext=(((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (6))].yystate.yysemantics.yysval.ast_node);n->cmp=(((yyGLRStackItem const *)yyvsp)[YYFILL ((2) - (6))].yystate.yysemantics.yysval.subtok);n->sign="CMP";n->parameter="SOME";n->rnext=(((yyGLRStackItem const *)yyvsp)[YYFILL ((5) - (6))].yystate.yysemantics.yysval.ast_node);((*yyvalp).ast_node)=(Node*)n;;}
    break;

  case 193:

/* Line 936 of glr.c  */
#line 945 "sql.ypp"
    {Expr_cal *n=(Expr_cal *)malloc(sizeof(struct Expr_cal ));n->type=t_expr_cal;n->lnext=(((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (6))].yystate.yysemantics.yysval.ast_node);n->cmp=(((yyGLRStackItem const *)yyvsp)[YYFILL ((2) - (6))].yystate.yysemantics.yysval.subtok);n->sign="CMP";n->parameter="ALL";n->rnext=(((yyGLRStackItem const *)yyvsp)[YYFILL ((5) - (6))].yystate.yysemantics.yysval.ast_node);((*yyvalp).ast_node)=(Node*)n;;}
    break;

  case 194:

/* Line 936 of glr.c  */
#line 946 "sql.ypp"
    {Expr_cal *n=(Expr_cal *)malloc(sizeof(struct Expr_cal ));n->type=t_expr_cal;n->lnext=(((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (3))].yystate.yysemantics.yysval.ast_node);n->sign="|";n->rnext=(((yyGLRStackItem const *)yyvsp)[YYFILL ((3) - (3))].yystate.yysemantics.yysval.ast_node);((*yyvalp).ast_node)=(Node*)n;;}
    break;

  case 195:

/* Line 936 of glr.c  */
#line 947 "sql.ypp"
    {Expr_cal *n=(Expr_cal *)malloc(sizeof(struct Expr_cal ));n->type=t_expr_cal;n->lnext=(((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (3))].yystate.yysemantics.yysval.ast_node);n->sign="&";n->rnext=(((yyGLRStackItem const *)yyvsp)[YYFILL ((3) - (3))].yystate.yysemantics.yysval.ast_node);((*yyvalp).ast_node)=(Node*)n;;}
    break;

  case 196:

/* Line 936 of glr.c  */
#line 948 "sql.ypp"
    {Expr_cal *n=(Expr_cal *)malloc(sizeof(struct Expr_cal ));n->type=t_expr_cal;n->lnext=(((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (3))].yystate.yysemantics.yysval.ast_node);n->sign="^";n->rnext=(((yyGLRStackItem const *)yyvsp)[YYFILL ((3) - (3))].yystate.yysemantics.yysval.ast_node);((*yyvalp).ast_node)=(Node*)n;;}
    break;

  case 197:

/* Line 936 of glr.c  */
#line 949 "sql.ypp"
    {Expr_cal *n=(Expr_cal *)malloc(sizeof(struct Expr_cal ));n->type=t_expr_cal;n->lnext=(((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (3))].yystate.yysemantics.yysval.ast_node);n->sign=(SHIFT==1?"LS":"RS");n->rnext=(((yyGLRStackItem const *)yyvsp)[YYFILL ((3) - (3))].yystate.yysemantics.yysval.ast_node);((*yyvalp).ast_node)=(Node*)n;;}
    break;

  case 198:

/* Line 936 of glr.c  */
#line 950 "sql.ypp"
    {Expr_cal *n=(Expr_cal *)malloc(sizeof(struct Expr_cal ));n->type=t_expr_cal;n->sign="NOT";n->rnext=(((yyGLRStackItem const *)yyvsp)[YYFILL ((2) - (2))].yystate.yysemantics.yysval.ast_node);((*yyvalp).ast_node)=(Node*)n;;}
    break;

  case 199:

/* Line 936 of glr.c  */
#line 951 "sql.ypp"
    {Expr_cal *n=(Expr_cal *)malloc(sizeof(struct Expr_cal ));n->type=t_expr_cal;n->sign="!";n->rnext=(((yyGLRStackItem const *)yyvsp)[YYFILL ((2) - (2))].yystate.yysemantics.yysval.ast_node);((*yyvalp).ast_node)=(Node*)n;;}
    break;

  case 200:

/* Line 936 of glr.c  */
#line 952 "sql.ypp"
    { ((*yyvalp).ast_node)=NULL; ;}
    break;

  case 201:

/* Line 936 of glr.c  */
#line 953 "sql.ypp"
    { ((*yyvalp).ast_node) = (((yyGLRStackItem const *)yyvsp)[YYFILL ((2) - (3))].yystate.yysemantics.yysval.ast_node); ;}
    break;

  case 202:

/* Line 936 of glr.c  */
#line 956 "sql.ypp"
    {Expr_func *n=(Expr_func *)malloc(sizeof(struct Expr_func));n->type=t_expr_func;n->funname="ISN";n->args=(((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (3))].yystate.yysemantics.yysval.ast_node);((*yyvalp).ast_node)=(Node*)n;;}
    break;

  case 203:

/* Line 936 of glr.c  */
#line 957 "sql.ypp"
    {Expr_func *n=(Expr_func *)malloc(sizeof(struct Expr_func));n->type=t_expr_func;n->funname="ISNN";n->args=(((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (4))].yystate.yysemantics.yysval.ast_node);((*yyvalp).ast_node)=(Node*)n;;}
    break;

  case 204:

/* Line 936 of glr.c  */
#line 958 "sql.ypp"
    {Expr_func *n=(Expr_func *)malloc(sizeof(struct Expr_func));n->type=t_expr_func;n->funname="ISB";n->args=(((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (3))].yystate.yysemantics.yysval.ast_node);((*yyvalp).ast_node)=(Node*)n;;}
    break;

  case 205:

/* Line 936 of glr.c  */
#line 959 "sql.ypp"
    {Expr_func *n=(Expr_func *)malloc(sizeof(struct Expr_func));n->type=t_expr_func;n->funname="ISNB";n->args=(((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (4))].yystate.yysemantics.yysval.ast_node);((*yyvalp).ast_node)=(Node*)n;;}
    break;

  case 206:

/* Line 936 of glr.c  */
#line 962 "sql.ypp"
    {Expr_func *n=(Expr_func *)malloc(sizeof(struct Expr_func));n->type=t_expr_func;n->funname="BA";n->args=(((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (5))].yystate.yysemantics.yysval.ast_node);n->parameter1=(((yyGLRStackItem const *)yyvsp)[YYFILL ((3) - (5))].yystate.yysemantics.yysval.ast_node);n->parameter2=(((yyGLRStackItem const *)yyvsp)[YYFILL ((5) - (5))].yystate.yysemantics.yysval.ast_node);((*yyvalp).ast_node)=(Node*)n;;}
    break;

  case 207:

/* Line 936 of glr.c  */
#line 966 "sql.ypp"
    { Expr_list *n=(Expr_list *)malloc(sizeof(struct Expr_list ));n->type=t_expr_list;n->data=(((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (1))].yystate.yysemantics.yysval.ast_node);n->next= NULL;((*yyvalp).ast_node)=(Node*)n;;}
    break;

  case 208:

/* Line 936 of glr.c  */
#line 967 "sql.ypp"
    { Expr_list *n=(Expr_list *)malloc(sizeof(struct Expr_list ));n->type=t_expr_list;n->data=(((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (3))].yystate.yysemantics.yysval.ast_node);n->next=(((yyGLRStackItem const *)yyvsp)[YYFILL ((3) - (3))].yystate.yysemantics.yysval.ast_node);((*yyvalp).ast_node)=(Node*)n;;}
    break;

  case 210:

/* Line 936 of glr.c  */
#line 971 "sql.ypp"
    {((*yyvalp).ast_node)=(((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (1))].yystate.yysemantics.yysval.ast_node);;}
    break;

  case 211:

/* Line 936 of glr.c  */
#line 974 "sql.ypp"
    {Expr_cal *n=(Expr_cal *)malloc(sizeof(struct Expr_cal ));n->type=t_expr_cal;n->sign="INV";n->lnext=(((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (5))].yystate.yysemantics.yysval.ast_node);n->rnext=(((yyGLRStackItem const *)yyvsp)[YYFILL ((4) - (5))].yystate.yysemantics.yysval.ast_node);((*yyvalp).ast_node)=(Node*)n;;}
    break;

  case 212:

/* Line 936 of glr.c  */
#line 975 "sql.ypp"
    {Expr_cal *n=(Expr_cal *)malloc(sizeof(struct Expr_cal) );n->type=t_expr_cal;n->sign="NIN";n->lnext=(((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (6))].yystate.yysemantics.yysval.ast_node);n->rnext=(((yyGLRStackItem const *)yyvsp)[YYFILL ((5) - (6))].yystate.yysemantics.yysval.ast_node);((*yyvalp).ast_node)=(Node*)n;;}
    break;

  case 213:

/* Line 936 of glr.c  */
#line 976 "sql.ypp"
    {Expr_cal *n=(Expr_cal *)malloc(sizeof(struct Expr_cal ));n->type=t_expr_cal;n->sign="INS";n->lnext=(((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (5))].yystate.yysemantics.yysval.ast_node);n->rnext=(((yyGLRStackItem const *)yyvsp)[YYFILL ((4) - (5))].yystate.yysemantics.yysval.ast_node);((*yyvalp).ast_node)=(Node*)n;;}
    break;

  case 214:

/* Line 936 of glr.c  */
#line 977 "sql.ypp"
    {Expr_cal *n=(Expr_cal *)malloc(sizeof(struct Expr_cal ));n->type=t_expr_cal;n->sign="NINS";n->lnext=(((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (6))].yystate.yysemantics.yysval.ast_node);n->rnext=(((yyGLRStackItem const *)yyvsp)[YYFILL ((5) - (6))].yystate.yysemantics.yysval.ast_node);((*yyvalp).ast_node)=(Node*)n;;}
    break;

  case 215:

/* Line 936 of glr.c  */
#line 978 "sql.ypp"
    {Expr_cal *n=(Expr_cal *)malloc(sizeof(struct Expr_cal ));n->type=t_expr_cal;n->sign="EX";n->rnext=(((yyGLRStackItem const *)yyvsp)[YYFILL ((3) - (4))].yystate.yysemantics.yysval.ast_node);((*yyvalp).ast_node)=(Node*)n;;}
    break;

  case 216:

/* Line 936 of glr.c  */
#line 979 "sql.ypp"
    {Expr_cal *n=(Expr_cal *)malloc(sizeof(struct Expr_cal ));n->type=t_expr_cal;n->sign="NEX";n->rnext=(((yyGLRStackItem const *)yyvsp)[YYFILL ((4) - (5))].yystate.yysemantics.yysval.ast_node);((*yyvalp).ast_node)=(Node*)n;;}
    break;

  case 217:

/* Line 936 of glr.c  */
#line 982 "sql.ypp"
    {Expr_cal *n=(Expr_cal *)malloc(sizeof(struct Expr_cal ));n->type=t_expr_cal;n->sign="NAME";n->parameter=(((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (4))].yystate.yysemantics.yysval.strval);n->rnext=(((yyGLRStackItem const *)yyvsp)[YYFILL ((3) - (4))].yystate.yysemantics.yysval.ast_node);((*yyvalp).ast_node)=(Node*)n;;}
    break;

  case 218:

/* Line 936 of glr.c  */
#line 986 "sql.ypp"
    {Expr_func *n=(Expr_func *)malloc(sizeof(struct Expr_func));n->type=t_expr_func;n->funname="FCOUNTALL";((*yyvalp).ast_node)=(Node*)n;;}
    break;

  case 219:

/* Line 936 of glr.c  */
#line 987 "sql.ypp"
    {Expr_func *n=(Expr_func *)malloc(sizeof(struct Expr_func));n->type=t_expr_func;n->funname="FCOUNT";n->parameter1=(((yyGLRStackItem const *)yyvsp)[YYFILL ((3) - (4))].yystate.yysemantics.yysval.ast_node);((*yyvalp).ast_node)=(Node*)n;;}
    break;

  case 220:

/* Line 936 of glr.c  */
#line 989 "sql.ypp"
    {Expr_func *n=(Expr_func *)malloc(sizeof(struct Expr_func));n->type=t_expr_func;n->funname="FSUBSTRING";n->parameter1=(((yyGLRStackItem const *)yyvsp)[YYFILL ((3) - (4))].yystate.yysemantics.yysval.ast_node);((*yyvalp).ast_node)=(Node*)n;;}
    break;

  case 221:

/* Line 936 of glr.c  */
#line 990 "sql.ypp"
    {Expr_func *n=(Expr_func *)malloc(sizeof(struct Expr_func));n->type=t_expr_func;n->funname="FSUBSTRING";n->parameter1=(((yyGLRStackItem const *)yyvsp)[YYFILL ((3) - (6))].yystate.yysemantics.yysval.ast_node);n->parameter2=(((yyGLRStackItem const *)yyvsp)[YYFILL ((5) - (6))].yystate.yysemantics.yysval.ast_node);((*yyvalp).ast_node)=(Node*)n;;}
    break;

  case 222:

/* Line 936 of glr.c  */
#line 991 "sql.ypp"
    {Expr_func *n=(Expr_func *)malloc(sizeof(struct Expr_func));n->type=t_expr_func;n->funname="FSUBSTRING3";n->parameter1=(((yyGLRStackItem const *)yyvsp)[YYFILL ((3) - (8))].yystate.yysemantics.yysval.ast_node);n->parameter2=(((yyGLRStackItem const *)yyvsp)[YYFILL ((5) - (8))].yystate.yysemantics.yysval.ast_node);n->args=(((yyGLRStackItem const *)yyvsp)[YYFILL ((7) - (8))].yystate.yysemantics.yysval.ast_node);((*yyvalp).ast_node)=(Node*)n;;}
    break;

  case 223:

/* Line 936 of glr.c  */
#line 992 "sql.ypp"
    {Expr_func *n=(Expr_func *)malloc(sizeof(struct Expr_func));n->type=t_expr_func;n->funname="FTRIM";n->parameter1=(((yyGLRStackItem const *)yyvsp)[YYFILL ((3) - (4))].yystate.yysemantics.yysval.ast_node);((*yyvalp).ast_node)=(Node*)n;;}
    break;

  case 224:

/* Line 936 of glr.c  */
#line 993 "sql.ypp"
    {  ;}
    break;

  case 225:

/* Line 936 of glr.c  */
#line 996 "sql.ypp"
    { emit("INT 1"); ;}
    break;

  case 226:

/* Line 936 of glr.c  */
#line 997 "sql.ypp"
    { emit("INT 2"); ;}
    break;

  case 227:

/* Line 936 of glr.c  */
#line 998 "sql.ypp"
    { emit("INT 3"); ;}
    break;

  case 230:

/* Line 936 of glr.c  */
#line 1005 "sql.ypp"
    { emit("NUMBER 1"); ;}
    break;

  case 231:

/* Line 936 of glr.c  */
#line 1006 "sql.ypp"
    { emit("NUMBER 2"); ;}
    break;

  case 232:

/* Line 936 of glr.c  */
#line 1007 "sql.ypp"
    { emit("NUMBER 3"); ;}
    break;

  case 233:

/* Line 936 of glr.c  */
#line 1008 "sql.ypp"
    { emit("NUMBER 4"); ;}
    break;

  case 234:

/* Line 936 of glr.c  */
#line 1009 "sql.ypp"
    { emit("NUMBER 5"); ;}
    break;

  case 235:

/* Line 936 of glr.c  */
#line 1010 "sql.ypp"
    { emit("NUMBER 6"); ;}
    break;

  case 236:

/* Line 936 of glr.c  */
#line 1011 "sql.ypp"
    { emit("NUMBER 7"); ;}
    break;

  case 237:

/* Line 936 of glr.c  */
#line 1012 "sql.ypp"
    { emit("NUMBER 8"); ;}
    break;

  case 238:

/* Line 936 of glr.c  */
#line 1013 "sql.ypp"
    { emit("NUMBER 9"); ;}
    break;

  case 239:

/* Line 936 of glr.c  */
#line 1016 "sql.ypp"
    {Expr_func *n=(Expr_func *)malloc(sizeof(struct Expr_func));n->type=t_expr_func;n->funname="CASE1";n->parameter1=(((yyGLRStackItem const *)yyvsp)[YYFILL ((3) - (4))].yystate.yysemantics.yysval.ast_node);n->args=(((yyGLRStackItem const *)yyvsp)[YYFILL ((2) - (4))].yystate.yysemantics.yysval.ast_node);((*yyvalp).ast_node)=(Node*)n;;}
    break;

  case 240:

/* Line 936 of glr.c  */
#line 1017 "sql.ypp"
    {Expr_func *n=(Expr_func *)malloc(sizeof(struct Expr_func));n->type=t_expr_func;n->funname="CASE2";n->parameter1=(((yyGLRStackItem const *)yyvsp)[YYFILL ((3) - (6))].yystate.yysemantics.yysval.ast_node);n->args=(((yyGLRStackItem const *)yyvsp)[YYFILL ((2) - (6))].yystate.yysemantics.yysval.ast_node);((*yyvalp).ast_node)=(Node*)n;;}
    break;

  case 241:

/* Line 936 of glr.c  */
#line 1018 "sql.ypp"
    {Expr_func *n=(Expr_func *)malloc(sizeof(struct Expr_func));n->type=t_expr_func;n->funname="CASE3";n->parameter1=(((yyGLRStackItem const *)yyvsp)[YYFILL ((2) - (3))].yystate.yysemantics.yysval.ast_node);((*yyvalp).ast_node)=(Node*)n;;}
    break;

  case 242:

/* Line 936 of glr.c  */
#line 1019 "sql.ypp"
    {Expr_func *n=(Expr_func *)malloc(sizeof(struct Expr_func));n->type=t_expr_func;n->funname="CASE4";n->parameter1=(((yyGLRStackItem const *)yyvsp)[YYFILL ((2) - (5))].yystate.yysemantics.yysval.ast_node);n->parameter2=(((yyGLRStackItem const *)yyvsp)[YYFILL ((4) - (5))].yystate.yysemantics.yysval.ast_node);((*yyvalp).ast_node)=(Node*)n;;}
    break;

  case 243:

/* Line 936 of glr.c  */
#line 1022 "sql.ypp"
    {Expr_func *n=(Expr_func *)malloc(sizeof(struct Expr_func));n->type=t_expr_func;n->funname="WHEN";n->parameter1=(((yyGLRStackItem const *)yyvsp)[YYFILL ((2) - (4))].yystate.yysemantics.yysval.ast_node);n->parameter2=(((yyGLRStackItem const *)yyvsp)[YYFILL ((4) - (4))].yystate.yysemantics.yysval.ast_node);((*yyvalp).ast_node)=(Node*)n;;}
    break;

  case 244:

/* Line 936 of glr.c  */
#line 1023 "sql.ypp"
    {Expr_func *n=(Expr_func *)malloc(sizeof(struct Expr_func));n->type=t_expr_func;n->funname="WHEN";n->parameter1=(((yyGLRStackItem const *)yyvsp)[YYFILL ((3) - (5))].yystate.yysemantics.yysval.ast_node);n->parameter2=(((yyGLRStackItem const *)yyvsp)[YYFILL ((5) - (5))].yystate.yysemantics.yysval.ast_node);n->next=(((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (5))].yystate.yysemantics.yysval.ast_node);((*yyvalp).ast_node)=(Node*)n;;}
    break;

  case 245:

/* Line 936 of glr.c  */
#line 1026 "sql.ypp"
    {Expr_cal *n=(Expr_cal *)malloc(sizeof(struct Expr_cal ));n->type=t_expr_cal;n->lnext=(((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (3))].yystate.yysemantics.yysval.ast_node);n->sign="LIKE";n->rnext=(((yyGLRStackItem const *)yyvsp)[YYFILL ((3) - (3))].yystate.yysemantics.yysval.ast_node);((*yyvalp).ast_node)=(Node*)n;;}
    break;

  case 246:

/* Line 936 of glr.c  */
#line 1027 "sql.ypp"
    {Expr_cal *n=(Expr_cal *)malloc(sizeof(struct Expr_cal ));n->type=t_expr_cal;n->lnext=(((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (4))].yystate.yysemantics.yysval.ast_node);n->sign="NLIKE";n->rnext=(((yyGLRStackItem const *)yyvsp)[YYFILL ((4) - (4))].yystate.yysemantics.yysval.ast_node);((*yyvalp).ast_node)=(Node*)n;;}
    break;

  case 247:

/* Line 936 of glr.c  */
#line 1030 "sql.ypp"
    {Expr_cal *n=(Expr_cal *)malloc(sizeof(struct Expr_cal) );n->type=t_expr_cal;n->lnext=(((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (3))].yystate.yysemantics.yysval.ast_node);n->sign="REGEXP";n->rnext=(((yyGLRStackItem const *)yyvsp)[YYFILL ((3) - (3))].yystate.yysemantics.yysval.ast_node);((*yyvalp).ast_node)=(Node*)n;;}
    break;

  case 248:

/* Line 936 of glr.c  */
#line 1031 "sql.ypp"
    {Expr_cal *n=(Expr_cal *)malloc(sizeof(struct Expr_cal) );n->type=t_expr_cal;n->lnext=(((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (4))].yystate.yysemantics.yysval.ast_node);n->sign="NREGEXP";n->rnext=(((yyGLRStackItem const *)yyvsp)[YYFILL ((4) - (4))].yystate.yysemantics.yysval.ast_node);((*yyvalp).ast_node)=(Node*)n;;}
    break;



/* Line 936 of glr.c  */
#line 3301 "sql.tab.cpp"
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
#line 1042 "sql.ypp"


/* 	t_none, t_name,t_uservar,t_name_name,t_stringval,t_intnum,t_approxnum,t_bool, t_table,
	t_column,t_join,t_subquery,t_condition, t_query_stmt, t_select_list,
	t_select_opts,t_select_expr, t_expr,t_expr_list,t_expr_cal,t_expr_func,t_expr_relation,
 	t_from_list,t_from_expr,t_from_table, t_where_list, t_groupby_list,t_groupby_expr, 
	t_having_list, t_orderby_list,t_orderby_expr, t_limit_list,t_limit_expr
 */

struct Node * newExpr(nodetype t, dataval d)
{
	struct Expr * a= (struct Expr *)malloc(sizeof(struct Expr));
	if(!a)
	{
		yyerror("out of space!");
		exit(0);
	}
	
	a->type = t;
	a->data = d;
	//cout<<"newExpr is created"<<endl;
	return (struct Node *)a;
}

struct Node * newExprList(nodetype t, Node * data, Node * next)
{
	struct Expr_list * a= (struct Expr_list *)malloc(sizeof(struct Expr_list));
	if(!a)
	{
		yyerror("out of space!");
		exit(0);
	}
	
	a->type = t;
	a->data = data;
	a->next = next;
	//cout<<"newExpr is created"<<endl;
	return (struct Node *)a;
}

struct Node * newColumn(nodetype t, char * parameter1, char *parameter2, Node * next)
{
	struct Columns * a= (struct Columns *)malloc(sizeof(struct Columns));
	if(!a)
	{
		yyerror("out of space!");
		exit(0);
	}
	
	a->type = t;
	a->parameter1 = parameter1;
	a->parameter2 = parameter2;
	a->next = next;
	//cout<<"newExpr is created"<<endl;
	return (struct Node *)a;
}

struct Node * newExpr_cal(nodetype type, char * sign, char *parameter,
	int cmp, Node *lnext, Node *rnext)
{
	struct Expr_cal * a= (struct Expr_cal *)malloc(sizeof(struct Expr_cal));
	if(!a)
	{
		yyerror("out of space!");
		exit(0);
	}
	
	a->type = type;
	a->sign = sign;
	a->parameter = parameter;
	a->cmp = cmp;
	a->lnext = lnext;
	a->rnext = rnext;
	//cout<<"newExpr is created"<<endl;
	return (struct Node *)a;
}

struct Node * newExpr_func(nodetype type, char * funname, Node *args,
	Node * parameter1, Node *parameter2, Node *next)
{
	struct Expr_func * a= (struct Expr_func *)malloc(sizeof(struct Expr_func));
	if(!a) {
		yyerror("out of space!");
		exit(0);
	}
	
	a->type = type;
	a->funname = funname;
	a->args = args;
	a->parameter1 = parameter1;
	a->parameter2 = parameter2;
	a->next = next;
	//cout<<"newExpr is created"<<endl;
	return (struct Node *)a;
}
struct Node *newExprlistheader(nodetype type,Node * header,Node * tail)
{
	struct Expr_list_header *node=(struct Expr_list_header *)malloc(sizeof(struct Expr_list_header));
	node->type=t_expr_list_header;
	node->header=header;
	node->tail=tail;
	return (struct Node *)node;
}

struct Node * newTable(nodetype type, char * dbname, char *tablename,
	char *astablename, int issubquery, Node *subquery)
{
	struct Table * a= (struct Table *)malloc(sizeof(struct Table)+10);
	if(!a) {
		yyerror("out of space!");
		exit(0);
	}
	
	a->type = type;
	a->dbname = dbname;
	a->tablename = tablename;
	a->astablename = astablename;
	a->issubquery = issubquery;
	a->subquery = subquery;

	a->whcdn=newExprlistheader(t_expr_list_header,NULL,NULL);
	
	return (struct Node *)a;
}

struct Node * newJoin(nodetype type, int jointype,Node *lnext, Node *rnext, Node *condition)
{
	struct Join * a= (struct Join *)malloc(sizeof(struct Join));
	if(!a) {
		yyerror("out of space!");
		exit(0);
	}
	
	a->type = type;
	a->jointype = jointype;
	a->lnext = lnext;
	a->rnext = rnext;
	a->condition = condition;
	//cout<<"Join is created"<<endl;
	return (struct Node *)a;
}

struct Node * newCondition(nodetype type, int conditiontype, Node * args)
{
	struct Condition * a= (struct Condition *)malloc(sizeof(struct Condition));
	if(!a) {
		yyerror("out of space!");
		exit(0);
	}
	
	a->type = type;
	a->conditiontype = conditiontype;
	a->args = args;
	//cout<<"Condition is created"<<endl;
	return (struct Node *)a;
}

struct Node * newSubquery(nodetype type, char * querystring, Node *next)
{
	struct Subquery * a= (struct Subquery *)malloc(sizeof(struct Subquery));
	if(!a) {
		yyerror("out of space!");
		exit(0);
	}
	
	a->type = type;
	a->querystring = querystring;
	a->next = next;
	//cout<<"Subquery is created"<<endl;
	return (struct Node *)a;
}

struct Node * newQueryStmt(nodetype t, char * querystring, int select_opts,
	Node *select_list, Node *from_list, Node *where_list, Node *groupby_list, 
	Node *having_list, Node *orderby_list, Node *limit_list, Node *into_list)
{
	struct Query_stmt * a= (struct Query_stmt *)malloc(sizeof(struct Query_stmt));
	if(!a)
	{
		yyerror("out of space!");
		exit(0);
	}
	
	a->type = t;
	a->querystring = querystring;
	a->select_opts = select_opts;
	a->select_list = select_list;
	a->from_list = from_list;
	a->where_list = where_list;
	a->groupby_list = groupby_list;
	a->having_list = having_list;
	a->orderby_list = orderby_list;
	a->limit_list = limit_list;
	a->into_list = into_list;
	
	//cout<<"Query_stmt is created"<<endl;
	return (struct Node *)a;
}

struct Node * newSelectList(nodetype type,int isall,Node * args,Node *next)
{
	struct Select_list * a= (struct Select_list *)malloc(sizeof(struct Select_list));
	if(!a)
	{
		yyerror("out of space!");
		exit(0);
	}
	a->type = type;
	a->isall = isall;
	a->args = args;
	a->next = next;
	
	//cout<<"Select_list is created"<<endl;
	return (struct Node *)a;
}

struct Node * newSelectExpr(nodetype type, char *ascolname, Node * colname) 
{
	struct Select_expr * a= (struct Select_expr *)malloc(sizeof(struct Select_expr));
	if(!a)
	{
		yyerror("out of space!");
		exit(0);
	}
	a->type = type;
	a->ascolname = ascolname;
	a->colname = colname;
	
	//cout<<"Select_expr is created"<<endl;
	return (struct Node *)a;
};

struct Node *  newFromList(nodetype type, Node * args, Node *next)
{
	struct From_list * a= (struct From_list *)malloc(sizeof(struct From_list));
	if(!a)
	{
		yyerror("out of space!");
		exit(0);
	}
	a->type = type;
	a->args = args;
	a->next = next;
//	a->condition.clear();
	//a->wcondition.clear();
	a->whcdn=newExprlistheader(t_expr_list_header,NULL,NULL);
	//cout<<"From_list is created"<<endl;
	return (struct Node *)a;
};

struct Node * newFromExpr( nodetype type, char * astablename, Node *next)
{
	struct From_expr * a= (struct From_expr *)malloc(sizeof(struct From_expr));
	if(!a)
	{
		yyerror("out of space!");
		exit(0);
	}
	a->type = type;
	a->astablename = astablename;
	a->next = next;
	
	//cout<<"From_expr is created"<<endl;
	return (struct Node *)a;
};

struct Node * newWhereList(nodetype type, char * wherestring, Node *next)
{
	struct Where_list * a= (struct Where_list *)malloc(sizeof(struct Where_list));
	if(!a)
	{
		yyerror("out of space!");
		exit(0);
	}
	a->type = type;
	a->wherestring = wherestring;
	a->next = next;
	
	//cout<<"Where_list is created"<<endl;
	return (struct Node *)a;
};

struct Node * newGroupbyList(nodetype type, char * groupbystring, Node *next, int with_rollup)
{
	struct Groupby_list * a= (struct Groupby_list *)malloc(sizeof(struct Groupby_list));
	if(!a)
	{
		yyerror("out of space!");
		exit(0);
	}
	a->type = type;
	a->groupbystring = groupbystring;
	a->next = next;
	a->with_rollup = with_rollup;
	
	//cout<<"Groupby_list is created"<<endl;
	return (struct Node *)a;
};

struct Node * newGroupbyExpr(nodetype type, Node *args, int sorttype, Node *next)
{
	struct Groupby_expr * a= (struct Groupby_expr *)malloc(sizeof(struct Groupby_expr));
	if(!a)
	{
		yyerror("out of space!");
		exit(0);
	}
	a->type = type;
	a->args = args;
	a->sorttype = sorttype;
	a->next = next;
	
	//cout<<"Groupby_expr is created"<<endl;
	return (struct Node *)a;
};

struct Node * newHavingList(nodetype type, char * havingstring, Node *next)
{
	struct Having_list * a= (struct Having_list *)malloc(sizeof(struct Having_list));
	if(!a)
	{
		yyerror("out of space!");
		exit(0);
	}
	a->type = type;
	a->havingstring = havingstring;
	a->next = next;
	
	//cout<<"Having_list is created"<<endl;
	return (struct Node *)a;
};


struct Node * newOrderbyList(nodetype type,char * orderbystring, Node *next)
{
	struct Orderby_list * a= (struct Orderby_list *)malloc(sizeof(struct Orderby_list));
	if(!a)
	{
		yyerror("out of space!");
		exit(0);
	}
	a->type = type;
	a->orderbystring = orderbystring;
	a->next = next;
	
	//cout<<"Orderby_list is created"<<endl;
	return (struct Node *)a;
};

struct Node * newOrderbyExpr(nodetype type, Node *args, char * sorttype, Node *next)
{
	struct Orderby_expr * a= (struct Orderby_expr *)malloc(sizeof(struct Orderby_expr));
	if(!a)
	{
		yyerror("out of space!");
		exit(0);
	}
	a->type = type;
	a->args = args;
	a->sorttype = sorttype;
	a->next = next;
	
	//cout<<"Orderby_expr is created"<<endl;
	return (struct Node *)a;
};

struct Node * newLimitExpr(nodetype type, Node * offset, Node * row_count)
{
	struct Limit_expr * a= (struct Limit_expr *)malloc(sizeof(struct Limit_expr));
	if(!a)
	{
		yyerror("out of space!");
		exit(0);
	}
	a->type = type;
	a->offset = offset;
	a->row_count = row_count;
	
	//cout<<"Limit_expr is created"<<endl;
	return (struct Node *)a;
};

void outputSpace(int f)
{
	while(f--)
		cout<<"  ";
}

void output(Node * oldnode, int floor)
{
	//puts("there is output()");
	if(oldnode == NULL)
	{
		//cout<<"This is Nothing!"<<endl;
		return;
	}
	nodetype type = oldnode->type;
	switch(type)
	{
		
		case t_name_name:
		{
			Columns * node = (Columns *) oldnode;
			outputSpace(floor);
			cout<<"Columns: "<< node->parameter1<< "  "<< node->parameter2<<endl;
			break;
		}
		case t_name:
		case t_stringval:
		{
			Expr * node = (Expr *) oldnode;
			outputSpace(floor);
			cout<<"name: "<<node->data.string_val<<endl;
			break;
		}
		case t_intnum:
		{
			Expr * node = (Expr *) oldnode;
			outputSpace(floor);
			cout<<"name: "<<node->data.int_val<<endl;
			break;
		}
		case t_approxnum:
		{
			Expr * node = (Expr *) oldnode;
			outputSpace(floor);
			cout<<"name: "<<node->data.double_val<<endl;
			break;
		}
		case t_bool:
		{
			Expr * node = (Expr *) oldnode;
			outputSpace(floor);
			cout<<"name: "<<node->data.bool_val<<endl;
			break;
		}	
		
		case t_expr_list:
		{
			Expr_list * node = (Expr_list *) oldnode;
			outputSpace(floor);
			cout<<"Expr_list: ";cout<<endl;
			
			if(node->data!=NULL) output(node->data,floor+1);
			//if(node->next!=NULL) output(node->next);
			
			
			
			break;
		}
		case t_column:
		/* nodetype type;char * parameter1;char *parameter2;Node * next; */
		{
			Columns * node = (Columns *) oldnode;
			outputSpace(floor);
			cout<<"Columns: ";
			if (node->parameter1)
				cout<<node->parameter1<<" ";
			if (node->parameter2)
				cout<<node->parameter2<<" ";
				
			cout<<endl;
			if (node->next)
				output(node->next, floor + 1);
			
			break;
		}
		/*
		struct Expr_cal//计算表达式,二元表达式
		{
			nodetype type;
			char * sign,*parameter;
			int cmp;
			Node *lnext,*rnext;
		};
		*/
		case t_expr_cal: 
		{
			Expr_cal * node = (Expr_cal *) oldnode;
			outputSpace(floor);
			cout<<"Expr_cal: ";
			if (node->sign)
				cout<<node->sign<<" ";
			if (node->parameter)
				cout<<node->parameter<<" ";
			if (node->cmp)
				cout<<node->cmp<<" ";
				
			cout<<endl;
			output(node->lnext, floor + 1);
			output(node->rnext, floor + 1);
			break;
		}
		
		/*
		struct Expr_func //函数表达式，将is null/exist等判断抽象成函数
		{
			nodetype type;
			char * funname;
			Node *args;
			Node * parameter1,*parameter2;//函数中的参数列表，处理between...and.../case...when...then...end等
			Node *next;
		}; 
		*/
		case t_expr_func:
		{
			Expr_func * node = (Expr_func *) oldnode;
			outputSpace(floor);
			cout<<"Expr_func: ";
			if (node->funname)
				cout<<node->funname<<" ";
				
			cout<<endl;
			output(node->args, floor + 1);
			output(node->parameter1, floor + 1);
			output(node->parameter2, floor + 1);
			output(node->next, floor + 1);
			break;
		}
		
		case t_table://///////////////////////////////////////////////////
		/* nodetype type;	char * dbname,*tablename,*astablename;	
			int issubquery;	Node *subquery; Node * condition */
		{
			Table * node = (Table *) oldnode;
			
			outputSpace(floor);
			cout<<"Table: ";
			if (node->dbname)
				cout<<node->dbname<<" ";
			if (node->tablename)
				cout<<node->tablename<<" ";
			if (node->astablename)
				cout<<node->astablename<<" ";
			if (node->issubquery)
				cout<<node->issubquery<<" ";
				
			cout<<endl;
			output(node->subquery, floor + 1);
			//output(node->condition, floor + 1);
		//	set<Node*>::iterator it;
		//	vector<Node*>::iterator it;
		//	for(it=node->wcondition.begin();it!=node->wcondition.end();it++)
		//	{
		//		output((Node *)*it,floor+1);
		//	}
			Node * p;
			Expr_list_header * lists=(Expr_list_header *)node->whcdn;
			for(p=lists->header;p!=NULL;p=((Expr_list *)p)->next)
			{
				
				output(p,floor+1);
			}
			
			break;
			
		}
		
		/*************************************************/
		
		case t_query_stmt:
		/* nodetype type;char * querystring;int select_opts;Node *select_list;
			 Node *from_list; Node *where_list; Node *groupby_list;
			Node *having_list; Node *orderby_list; Node *limit_list; Node *into_list; */
		{
			Query_stmt * node = (Query_stmt *) oldnode;
			outputSpace(floor);
			cout<<"Query_stmt: ";
			if(node->querystring)
				cout<<node->querystring<<" ";
			if(node->select_opts)
				cout<<node->select_opts<<" "; 
				
			cout<<endl;
			output(node->select_list, floor + 1);
			output(node->from_list, floor + 1);
			output(node->where_list, floor + 1);
			output(node->groupby_list, floor + 1);
			output(node->having_list, floor + 1);
			output(node->orderby_list, floor + 1);
			output(node->limit_list, floor + 1);
			output(node->into_list, floor + 1);
			
			/* 可继续添加 */
			break;
		}
		case t_select_list:
		/* nodetype type; int isall; Node * args; Node *next; */
		{
			Select_list * node = (Select_list *) oldnode;
			outputSpace(floor);
			cout<<"select_list: ";
			if (node->isall) cout<<"is all ";
			
			cout<<endl;
			output(node->args, floor + 1);
			output(node->next, floor + 1);
			
			break;
		}
		case t_select_expr:
		/* nodetype type;	char *ascolname;	Node * colname; */
		{
			Select_expr * node = (Select_expr *) oldnode;
			outputSpace(floor);
			cout<<"Select_expr: ";
			if (node->ascolname)
				cout<<node->ascolname<< " ";
				
			cout<<endl;
			output(node->colname, floor + 1);
			
			break;
		}
		
		case t_from_list:
		/* nodetype type;	Node * args;	Node *next;  Node * condition */
		{
			From_list * node = (From_list *) oldnode;
			outputSpace(floor);
			cout<<"From_list: ";
			
			cout<<endl;
			//output(node->condition, floor + 1);////////////////////////////////
			Node * p;
			Expr_list_header * lists=(Expr_list_header *)node->whcdn;
			for(p=lists->header;p!=NULL;p=((Expr_list *)p)->next)
			{
				output(p,floor+1);
			}
			output(node->args, floor + 1);
			output(node->next, floor + 1);
			
			break;
		}
		case t_from_expr:
		/* nodetype type;	char * astablename;	Node *next; */
		{
			From_expr * node = (From_expr *) oldnode;
			outputSpace(floor);
			cout<<"From_expr: ";
			if(node->astablename)
				cout<<node->astablename;
			cout<<endl;
			output(node->next, floor+1);
			break;
		}
		case t_where_list:
		/* nodetype type;	char * wherestring;	Node *next; */
		{
			Where_list * node = (Where_list *) oldnode;
			outputSpace(floor);
			cout<<"Where_list: ";
			if(node->wherestring)
				cout<<node->wherestring;
			cout<<endl;
			output(node->next, floor+1);
			break;
		}
		case t_groupby_list:
		/* nodetype type;char * groupbystring;Node *next;int with_rollup; */
		{
			Groupby_list * node = (Groupby_list *) oldnode;
			outputSpace(floor);
			cout<<"Groupby_list: ";
			if(node->groupbystring)
				cout<<node->groupbystring<<" ";
			if (node->with_rollup)
				cout<<node->with_rollup<<" ";
			cout<<endl;
			output(node->next, floor+1);
			break;
		}
		case t_groupby_expr:
		/* nodetype type;	Node *args;	int sorttype;	Node *next; */
		{
			Groupby_expr * node = (Groupby_expr *) oldnode;
			outputSpace(floor);
			cout<<"Groupby_expr: ";
			
			if(node->sorttype)
				cout<<node->sorttype<<" ";
			cout<<endl;
			output(node->args, floor+1);
			output(node->next, floor+1);
			break;
		}
		
		case t_having_list:
		/* nodetype type;	char * havingstring;	Node *next; */
		{
			Having_list * node = (Having_list *) oldnode;
			outputSpace(floor);
			cout<<"Having_list: ";
			if(node->havingstring)
				cout<<node->havingstring<<" ";
				
			cout<<endl;
			output(node->next, floor+1);
			break;
		}
		case t_orderby_list:
		/* nodetype type;	char * orderbystring;	Node *next; */
		{
			Orderby_list * node = (Orderby_list *) oldnode;
			outputSpace(floor);
			cout<<"Orderby_list: ";
			if(node->orderbystring)
				cout<<node->orderbystring<<" ";
				
			cout<<endl;
			output(node->next, floor+1);
			break;
		}
		
		case t_orderby_expr:
		/* nodetype type;	Node *args;	char * sorttype;	Node *next; */
		{
			Orderby_expr * node = (Orderby_expr *) oldnode;
			outputSpace(floor);
			cout<<"Orderby_expr: ";
			if(node->sorttype)
				cout<<node->sorttype<<" ";
				
			cout<<endl;
			output(node->args, floor+1);
			output(node->next, floor+1);
			break;
		}
		
		case t_limit_expr:
		/* nodetype type;	Node * offset;	Node *row_count; */
		{
			Limit_expr * node = (Limit_expr *) oldnode;
			outputSpace(floor);
			cout<<"Limit_expr: ";
			
			cout<<endl;
			output(node->offset, floor+1);
			output(node->row_count, floor+1);
			break;
		}
	}
}

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

/*int main()
{
	globalReadOffset = 0;
	char str[1000];
	gets(str);	
	strcpy(globalInputText,str);
  if(!yyparse())
    printf("SQL parse worked\n");
  else
    printf("SQL parse failed\n");
    return 0;
} *//* main */

int yywrap()
{
	return -1;
}


