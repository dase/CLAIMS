/* A Bison parser, made by GNU Bison 2.7.  */

/* Skeleton implementation for Bison GLR parsers in C
   
      Copyright (C) 2002-2012 Free Software Foundation, Inc.
   
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
#define YYBISON_VERSION "2.7"

/* Skeleton name.  */
#define YYSKELETON_NAME "glr.c"

/* Pure parsers.  */
#define YYPURE 1






/* Copy the first part of user declarations.  */
/* Line 207 of glr.c  */
#line 10 "sql.ypp"

#include <stdlib.h>
#include <stdarg.h>
#include <string.h>
#include <iostream>
#include <string>
#include <stdio.h>
#include <malloc.h>
#include <vector>
using namespace std;
#include "../ast_node/ast_node.h"
#include "../ast_node/ast_select_stmt.h"
#include "../ast_node/ast_expr_node.h"
#include "../ast_node/ast_create_stmt.h"
#include "../ast_node/ast_drop_stmt.h"
#include "../ast_node/ast_insert_stmt.h"
#include "../ast_node/ast_load_stmt.h"

void yyerror(struct ParseResult *pp,const char *s, ...);
void emit(char *s, ...);

/* Line 207 of glr.c  */
#line 78 "sql.tab.cpp"

# ifndef YY_NULL
#  if defined __cplusplus && 201103L <= __cplusplus
#   define YY_NULL nullptr
#  else
#   define YY_NULL 0
#  endif
# endif

#include "../../sql_parser/parser/sql.tab.hpp"

/* Enabling verbose error messages.  */
#ifdef YYERROR_VERBOSE
# undef YYERROR_VERBOSE
# define YYERROR_VERBOSE 1
#else
# define YYERROR_VERBOSE 0
#endif

/* Default (constant) value used for initialization for null
   right-hand sides.  Unlike the standard yacc.c template, here we set
   the default value of $$ to a zeroed-out value.  Since the default
   value is undefined, this behavior is technically correct.  */
static YYSTYPE yyval_default;

/* Copy the second part of user declarations.  */
/* Line 230 of glr.c  */
#line 39 "sql.ypp"

#include "../../sql_parser/parser/sql.lex.h"

#define YYLEX_PARAM result->yyscan_info_

/*
should replace YYLEX with the following clause in sql.tab.cpp, why so? I don't know

#ifdef YYLEX_PARAM
# define YYLEX yylex (&yylval, YYLEX_PARAM)
#else
# define YYLEX yylex (&yylval)
#endif
*/


/* Line 230 of glr.c  */
#line 124 "sql.tab.cpp"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#ifndef YY_
# if defined YYENABLE_NLS && YYENABLE_NLS
#  if ENABLE_NLS
#   include <libintl.h> /* INFRINGES ON USER NAME SPACE */
#   define YY_(Msgid) dgettext ("bison-runtime", Msgid)
#  endif
# endif
# ifndef YY_
#  define YY_(Msgid) Msgid
# endif
#endif

/* Suppress unused-variable warnings by "using" E.  */
#if ! defined lint || defined __GNUC__
# define YYUSE(E) ((void) (E))
#else
# define YYUSE(E) /* empty */
#endif

/* Identity function, used to suppress warnings about constant conditions.  */
#ifndef lint
# define YYID(N) (N)
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
# define YYSETJMP(Env) setjmp (Env)
/* Pacify clang.  */
# define YYLONGJMP(Env, Val) (longjmp (Env, Val), YYASSERT (0))
#endif

/*-----------------.
| GCC extensions.  |
`-----------------*/

#ifndef __attribute__
/* This feature is available in gcc versions 2.5 and later.  */
# if (! defined __GNUC__ || __GNUC__ < 2 \
      || (__GNUC__ == 2 && __GNUC_MINOR__ < 5))
#  define __attribute__(Spec) /* empty */
# endif
#endif

#ifndef YYASSERT
# define YYASSERT(Condition) ((void) ((Condition) || (abort (), 0)))
#endif

/* YYFINAL -- State number of the termination state.  */
#define YYFINAL  38
/* YYLAST -- Last index in YYTABLE.  */
#define YYLAST   2140

/* YYNTOKENS -- Number of terminals.  */
#define YYNTOKENS  342
/* YYNNTS -- Number of nonterminals.  */
#define YYNNTS  74
/* YYNRULES -- Number of rules.  */
#define YYNRULES  319
/* YYNRULES -- Number of states.  */
#define YYNSTATES  677
/* YYMAXRHS -- Maximum number of symbols on right-hand side of rule.  */
#define YYMAXRHS 13
/* YYMAXLEFT -- Maximum number of symbols to the left of a handle
   accessed by $0, $-1, etc., in any rule.  */
#define YYMAXLEFT 0

/* YYTRANSLATE(X) -- Bison symbol number corresponding to X.  */
#define YYUNDEFTOK  2
#define YYMAXUTOK   582

#define YYTRANSLATE(YYX)                                                \
  ((unsigned int) (YYX) <= YYMAXUTOK ? yytranslate[YYX] : YYUNDEFTOK)

/* YYTRANSLATE[YYLEX] -- Bison symbol number corresponding to YYLEX.  */
static const unsigned short int yytranslate[] =
{
       0,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,    17,     2,     2,     2,    28,    22,     2,
     340,   341,    26,    24,   338,    25,   339,    27,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,   337,
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
     244,   245,   246,   247,   248,   249,   250,   251,   252,   253,
     254,   255,   256,   257,   258,   259,   260,   261,   262,   263,
     264,   265,   266,   267,   268,   269,   270,   271,   272,   273,
     274,   275,   276,   277,   278,   279,   280,   281,   282,   283,
     284,   285,   286,   287,   288,   289,   290,   291,   292,   293,
     294,   295,   296,   297,   298,   299,   300,   301,   302,   303,
     304,   305,   306,   307,   308,   309,   310,   311,   312,   313,
     314,   315,   316,   317,   318,   319,   320,   321,   322,   323,
     324,   325,   326,   327,   328,   329,   330,   331,   332,   333,
     334,   335,   336
};

#if YYDEBUG
/* YYPRHS[YYN] -- Index of the first RHS symbol of rule number YYN in
   YYRHS.  */
static const unsigned short int yyprhs[] =
{
       0,     0,     3,     6,    10,    13,    17,    19,    23,    35,
      36,    39,    40,    45,    47,    51,    52,    54,    56,    57,
      60,    61,    64,    65,    69,    72,    77,    78,    81,    86,
      87,    90,    92,    96,    97,   100,   103,   106,   109,   112,
     115,   118,   121,   123,   127,   129,   133,   136,   138,   142,
     144,   146,   149,   154,   158,   162,   163,   165,   168,   170,
     171,   177,   181,   187,   194,   200,   201,   203,   205,   206,
     208,   210,   212,   215,   218,   219,   220,   222,   225,   230,
     234,   236,   238,   242,   246,   248,   250,   252,   254,   258,
     262,   266,   270,   274,   278,   281,   284,   288,   292,   296,
     300,   306,   313,   320,   327,   331,   335,   339,   343,   346,
     349,   353,   357,   361,   366,   370,   375,   381,   383,   387,
     391,   393,   397,   403,   411,   418,   427,   433,   441,   448,
     457,   462,   468,   473,   478,   483,   488,   493,   498,   505,
     512,   521,   530,   535,   543,   548,   555,   560,   562,   564,
     566,   567,   569,   571,   573,   575,   577,   584,   591,   595,
     599,   603,   607,   611,   615,   619,   623,   627,   632,   639,
     643,   649,   654,   660,   664,   669,   673,   678,   680,   685,
     690,   691,   695,   697,   706,   717,   727,   734,   746,   755,
     759,   760,   762,   764,   765,   767,   769,   773,   777,   783,
     788,   793,   799,   805,   806,   810,   813,   817,   821,   825,
     829,   832,   836,   840,   843,   847,   853,   856,   860,   864,
     868,   872,   876,   880,   884,   888,   892,   896,   898,   900,
     902,   904,   906,   910,   914,   917,   922,   924,   926,   928,
     930,   934,   938,   942,   946,   952,   958,   960,   964,   965,
     969,   975,   976,   978,   979,   982,   985,   986,   991,   995,
     997,  1008,  1022,  1024,  1035,  1036,  1038,  1040,  1042,  1043,
    1046,  1048,  1050,  1054,  1060,  1062,  1075,  1085,  1095,  1108,
    1110,  1116,  1118,  1123,  1128,  1129,  1132,  1134,  1141,  1143,
    1147,  1151,  1157,  1158,  1160,  1162,  1164,  1173,  1174,  1180,
    1181,  1184,  1187,  1190,  1193,  1195,  1196,  1197,  1201,  1205,
    1211,  1213,  1215,  1219,  1223,  1231,  1239,  1243,  1247,  1253
};

/* YYRHS -- A `-1'-separated list of the rules' RHS.  */
static const short int yyrhs[] =
{
     343,     0,    -1,   344,   337,    -1,   343,   344,   337,    -1,
       1,   337,    -1,   343,     1,   337,    -1,   345,    -1,   193,
     357,   358,    -1,   193,   357,   358,   107,   360,   346,   347,
     351,   352,   354,   355,    -1,    -1,   243,   373,    -1,    -1,
     110,    48,   348,   350,    -1,   373,    -1,   373,   338,   348,
      -1,    -1,    40,    -1,    83,    -1,    -1,   245,   188,    -1,
      -1,   111,   373,    -1,    -1,   163,    48,   353,    -1,   373,
     349,    -1,   373,   349,   338,   353,    -1,    -1,   136,   373,
      -1,   136,   373,   338,   373,    -1,    -1,   127,   356,    -1,
       3,    -1,     3,   338,   356,    -1,    -1,   357,    33,    -1,
     357,    86,    -1,   357,    87,    -1,   357,   112,    -1,   357,
     212,    -1,   357,   209,    -1,   357,   207,    -1,   357,   208,
      -1,   359,    -1,   359,   338,   358,    -1,    26,    -1,    26,
     338,   358,    -1,   373,   364,    -1,   361,    -1,   361,   338,
     360,    -1,   362,    -1,   365,    -1,     3,   364,    -1,     3,
     339,     3,   364,    -1,   372,   363,     3,    -1,   340,   360,
     341,    -1,    -1,    39,    -1,    39,     3,    -1,     3,    -1,
      -1,   361,   366,   129,   362,   370,    -1,   361,   212,   362,
      -1,   361,   212,   362,   158,   373,    -1,   361,   368,   367,
     129,   362,   371,    -1,   361,   154,   369,   129,   362,    -1,
      -1,   120,    -1,    63,    -1,    -1,   165,    -1,   135,    -1,
     187,    -1,   135,   367,    -1,   187,   367,    -1,    -1,    -1,
     371,    -1,   158,   373,    -1,   234,   340,   356,   341,    -1,
     340,   345,   341,    -1,     3,    -1,     8,    -1,     3,   339,
       3,    -1,     3,   339,    26,    -1,     4,    -1,     5,    -1,
       7,    -1,     6,    -1,   373,    24,   373,    -1,   373,    25,
     373,    -1,   373,    26,   373,    -1,   373,    27,   373,    -1,
     373,    29,   373,    -1,   373,    28,   373,    -1,    25,   373,
      -1,    24,   373,    -1,   373,    12,   373,    -1,   373,    10,
     373,    -1,   373,    11,   373,    -1,   373,    20,   373,    -1,
     373,    20,   340,   345,   341,    -1,   373,    20,    37,   340,
     345,   341,    -1,   373,    20,   199,   340,   345,   341,    -1,
     373,    20,    33,   340,   345,   341,    -1,   373,    21,   373,
      -1,   373,    22,   373,    -1,   373,    30,   373,    -1,   373,
      23,   373,    -1,    17,   373,    -1,    18,   373,    -1,     8,
       9,   373,    -1,   340,   373,   341,    -1,   373,    15,   156,
      -1,   373,    15,    18,   156,    -1,   373,    15,     6,    -1,
     373,    15,    18,     6,    -1,   373,    19,   373,    36,   373,
      -1,   373,    -1,   373,   338,   374,    -1,   340,   374,   341,
      -1,   374,    -1,   374,   338,   375,    -1,   373,    16,   340,
     374,   341,    -1,   340,   374,   341,    16,   340,   375,   341,
      -1,   373,    18,    16,   340,   374,   341,    -1,   340,   374,
     341,    18,    16,   340,   374,   341,    -1,   373,    16,   340,
     345,   341,    -1,   340,   374,   341,    16,   340,   345,   341,
      -1,   373,    18,    16,   340,   345,   341,    -1,   340,   374,
     341,    18,    16,   340,   345,   341,    -1,    99,   340,   345,
     341,    -1,    18,    99,   340,   345,   341,    -1,   328,   340,
      26,   341,    -1,   328,   340,   373,   341,    -1,   333,   340,
     373,   341,    -1,   334,   340,   373,   341,    -1,   335,   340,
     373,   341,    -1,   336,   340,   373,   341,    -1,   324,   340,
     373,   338,   373,   341,    -1,   324,   340,   373,   107,   373,
     341,    -1,   324,   340,   373,   338,   373,   338,   373,   341,
      -1,   324,   340,   373,   107,   373,   104,   373,   341,    -1,
     325,   340,   373,   341,    -1,   325,   340,   376,   373,   107,
     373,   341,    -1,   329,   340,   373,   341,    -1,   330,   340,
     373,    39,   377,   341,    -1,   331,   340,   374,   341,    -1,
     133,    -1,   224,    -1,    47,    -1,    -1,   125,    -1,     4,
      -1,    89,    -1,   103,    -1,    53,    -1,   326,   340,   373,
     338,   378,   341,    -1,   327,   340,   373,   338,   378,   341,
      -1,   126,   373,    73,    -1,   126,   373,    75,    -1,   126,
     373,    76,    -1,   126,   373,    77,    -1,   126,   373,    74,
      -1,   126,   373,   248,    -1,   126,   373,   247,    -1,   126,
     373,   250,    -1,   126,   373,   174,    -1,    51,   373,   379,
      96,    -1,    51,   373,   379,    93,   373,    96,    -1,    51,
     379,    96,    -1,    51,   379,    93,   373,    96,    -1,   242,
     373,   217,   373,    -1,   379,   242,   373,   217,   373,    -1,
     373,    14,   373,    -1,   373,    18,    14,   373,    -1,   373,
      13,   373,    -1,   373,    18,    13,   373,    -1,   380,    -1,
      62,    69,   381,     3,    -1,    62,   190,   381,     3,    -1,
      -1,   116,    18,    99,    -1,   382,    -1,    62,   385,   213,
     381,     3,   340,   386,   341,    -1,    62,   385,   213,   381,
       3,   339,     3,   340,   386,   341,    -1,    62,   385,   213,
     381,     3,   340,   386,   341,   383,    -1,    62,   385,   213,
     381,     3,   383,    -1,    62,   385,   213,   381,     3,   339,
       3,   340,   386,   341,   383,    -1,    62,   385,   213,   381,
       3,   339,     3,   383,    -1,   384,   363,   345,    -1,    -1,
     117,    -1,   182,    -1,    -1,   214,    -1,   387,    -1,   387,
     338,   386,    -1,     3,   389,   388,    -1,   169,   130,   340,
     356,   341,    -1,   130,   340,   356,   341,    -1,   118,   340,
     356,   341,    -1,   108,   118,   340,   356,   341,    -1,   108,
     130,   340,   356,   341,    -1,    -1,   388,    18,   156,    -1,
     388,   156,    -1,   388,    80,     4,    -1,   388,    80,     5,
      -1,   388,    80,     7,    -1,   388,    80,     6,    -1,   388,
      41,    -1,   388,   228,   130,    -1,   388,   169,   130,    -1,
     388,   130,    -1,   388,    57,     4,    -1,   388,   228,   340,
     356,   341,    -1,    45,   391,    -1,   221,   391,   393,    -1,
     198,   391,   393,    -1,   149,   391,   393,    -1,   124,   391,
     393,    -1,   125,   391,   393,    -1,    43,   391,   393,    -1,
     177,   391,   393,    -1,    89,   391,   393,    -1,   103,   391,
     393,    -1,    78,   391,   393,    -1,    71,    -1,   218,    -1,
     219,    -1,    72,    -1,   247,    -1,    53,   391,   394,    -1,
     240,   391,   394,    -1,    44,   391,    -1,   239,   340,     5,
     341,    -1,   220,    -1,    46,    -1,   148,    -1,   143,    -1,
     222,   392,   394,    -1,   215,   392,   394,    -1,   150,   392,
     394,    -1,   144,   392,   394,    -1,    97,   340,   390,   341,
     394,    -1,   196,   340,   390,   341,   394,    -1,     4,    -1,
     390,   338,     4,    -1,    -1,   340,     5,   341,    -1,   340,
       5,   338,     5,   341,    -1,    -1,    44,    -1,    -1,   393,
     230,    -1,   393,   249,    -1,    -1,   394,    53,   196,     3,
      -1,   394,    55,     3,    -1,   395,    -1,    62,   171,   158,
       3,   340,   356,   341,   167,   158,     3,    -1,    62,   171,
     158,     3,   340,   356,   341,   157,    20,     5,   167,   158,
       3,    -1,   396,    -1,    62,   397,   118,     3,   398,   158,
       3,   340,   400,   341,    -1,    -1,   228,    -1,   108,    -1,
     201,    -1,    -1,   234,   399,    -1,   319,    -1,   320,    -1,
       3,   391,   349,    -1,     3,   391,   349,   338,   400,    -1,
     401,    -1,   138,   213,     3,   107,   374,   245,     4,   338,
       4,   189,    20,     7,    -1,   138,   213,     3,   107,   374,
     245,     4,   338,     4,    -1,    38,   213,     3,   107,   374,
     245,     4,   338,     4,    -1,    38,   213,     3,   107,   374,
     245,     4,   338,     4,   189,    20,     7,    -1,   402,    -1,
      90,   118,     3,   158,     3,    -1,   403,    -1,    90,    69,
     404,     3,    -1,    90,   190,   404,     3,    -1,    -1,   116,
      99,    -1,   405,    -1,    90,   385,   213,   404,   406,   407,
      -1,     3,    -1,     3,   339,     3,    -1,   406,   338,     3,
      -1,   406,   338,     3,   339,     3,    -1,    -1,   184,    -1,
      50,    -1,   408,    -1,   123,   410,   411,     3,   412,   238,
     413,   409,    -1,    -1,   158,   159,   130,   231,   415,    -1,
      -1,   410,   146,    -1,   410,    81,    -1,   410,   112,    -1,
     410,   117,    -1,   127,    -1,    -1,    -1,   340,   356,   341,
      -1,   340,   414,   341,    -1,   340,   414,   341,   338,   413,
      -1,   373,    -1,    80,    -1,   373,   338,   414,    -1,    80,
     338,   414,    -1,   123,   410,   411,     3,   196,   415,   409,
      -1,   123,   410,   411,     3,   412,   345,   409,    -1,     3,
      20,   373,    -1,     3,    20,    80,    -1,   415,   338,     3,
      20,   373,    -1,   415,   338,     3,    20,    80,    -1
};

/* YYRLINE[YYN] -- source line where rule number YYN was defined.  */
static const unsigned short int yyrline[] =
{
       0,   431,   431,   445,   458,   467,   479,   483,   487,   494,
     495,   498,   499,   504,   505,   507,   508,   509,   512,   513,
     516,   517,   520,   521,   524,   525,   527,   528,   529,   532,
     533,   537,   538,   541,   542,   543,   544,   545,   546,   547,
     548,   549,   553,   554,   555,   556,   560,   563,   564,   568,
     569,   573,   575,   577,   578,   581,   582,   585,   586,   587,
     601,   602,   603,   604,   605,   608,   609,   610,   613,   614,
     618,   619,   623,   624,   625,   628,   629,   637,   638,   643,
     650,   651,   652,   653,   654,   655,   656,   657,   659,   660,
     661,   662,   663,   664,   666,   667,   669,   670,   671,   673,
     674,   675,   676,   677,   679,   680,   681,   682,   684,   685,
     686,   687,   690,   691,   692,   693,   696,   699,   700,   701,
     704,   705,   708,   709,   710,   711,   712,   713,   714,   715,
     717,   718,   728,   729,   730,   731,   732,   733,   737,   738,
     739,   740,   741,   742,   743,   744,   745,   748,   749,   750,
     753,   754,   755,   756,   757,   758,   761,   762,   766,   767,
     768,   769,   770,   771,   772,   773,   774,   778,   779,   780,
     781,   784,   785,   788,   789,   792,   793,   799,   803,   804,
     807,   808,   813,   816,   820,   824,   829,   833,   838,   842,
     846,   847,   848,   851,   852,   855,   856,   861,   862,   863,
     864,   865,   866,   868,   869,   870,   871,   872,   873,   874,
     875,   876,   877,   878,   879,   880,   884,   885,   886,   887,
     888,   889,   890,   891,   892,   893,   894,   895,   896,   897,
     898,   899,   900,   901,   902,   903,   905,   906,   907,   908,
     909,   910,   911,   912,   913,   914,   916,   917,   920,   921,
     922,   925,   926,   929,   930,   931,   934,   935,   937,   942,
     945,   947,   955,   959,   963,   964,   965,   966,   969,   970,
     973,   974,   977,   978,   982,   985,   990,   991,   992,   999,
    1002,  1006,  1010,  1011,  1014,  1015,  1019,  1022,  1026,  1027,
    1028,  1029,  1032,  1033,  1034,  1040,  1043,  1049,  1050,  1053,
    1054,  1055,  1056,  1057,  1060,  1060,  1063,  1064,  1067,  1068,
    1072,  1073,  1074,  1075,  1078,  1082,  1087,  1090,  1093,  1096
};
#endif

#if YYDEBUG || YYERROR_VERBOSE || 0
/* YYTNAME[SYMBOL-NUM] -- String name of the symbol SYMBOL-NUM.
   First, the terminals, then, starting at YYNTOKENS, nonterminals.  */
static const char *const yytname[] =
{
  "$end", "error", "$undefined", "NAME", "STRING", "INTNUM", "BOOL",
  "APPROXNUM", "USERVAR", "ASSIGN", "OR", "XOR", "ANDOP", "REGEXP", "LIKE",
  "IS", "IN", "'!'", "NOT", "BETWEEN", "COMPARISON", "'|'", "'&'", "SHIFT",
  "'+'", "'-'", "'*'", "'/'", "'%'", "MOD", "'^'", "UMINUS", "ADD", "ALL",
  "ALTER", "ANALYZE", "AND", "ANY", "APPEND", "AS", "ASC",
  "AUTO_INCREMENT", "BEFORE", "BIGINT", "BINARY", "BIT", "BLOB", "BOTH",
  "BY", "CALL", "CASCADE", "CASE", "CHANGE", "CHAR", "CHECK", "COLLATE",
  "COLUMN", "COMMENT", "CONDITION", "CONSTRAINT", "CONTINUE", "CONVERT",
  "CREATE", "CROSS", "CURRENT_DATE", "CURRENT_TIME", "CURRENT_TIMESTAMP",
  "CURRENT_USER", "CURSOR", "DATABASE", "DATABASES", "DATE", "DATETIME",
  "DAY_HOUR", "DAY", "DAY_MICROSECOND", "DAY_MINUTE", "DAY_SECOND",
  "DECIMAL", "DECLARE", "DEFAULT", "DELAYED", "DELETE", "DESC", "DESCRIBE",
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
  "PURGE", "QUICK", "QUARTER", "READ", "READS", "REAL", "REFERENCES",
  "RELEASE", "RENAME", "REPEAT", "REPLACE", "REQUIRE", "RESTRICT",
  "RETURN", "REVOKE", "RIGHT", "ROLLUP", "SAMPLE", "SCHEMA", "SCHEMAS",
  "SECOND_MICROSECOND", "SELECT", "SENSITIVE", "SEPARATOR", "SET", "SHOW",
  "SMALLINT", "SOME", "SONAME", "SPATIAL", "SPECIFIC", "SQL",
  "SQLEXCEPTION", "SQLSTATE", "SQLWARNING", "SQL_BIG_RESULT",
  "SQL_CALC_FOUND_ROWS", "SQL_SMALL_RESULT", "SSL", "STARTING",
  "STRAIGHT_JOIN", "TABLE", "TEMPORARY", "TEXT", "TERMINATED", "THEN",
  "TIME", "TIMESTAMP", "TINYBLOB", "TINYINT", "TINYTEXT", "TO", "TRAILING",
  "TRIGGER", "UNDO", "UNION", "UNIQUE", "UNLOCK", "UNSIGNED", "UPDATE",
  "USAGE", "USE", "USING", "UTC_DATE", "UTC_TIME", "UTC_TIMESTAMP",
  "VALUES", "VARBINARY", "VARCHAR", "VARYING", "WHEN", "WHERE", "WHILE",
  "WITH", "WRITE", "YEAR", "YEAR_MONTH", "ZEROFILL", "WEEK", "DO",
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
  "FDATE_ADD", "FDATE_SUB", "FCOUNT", "FUPPER", "FCAST", "FCOALESCE",
  "FCONVERT", "FSUM", "FAVG", "FMIN", "FMAX", "';'", "','", "'.'", "'('",
  "')'", "$accept", "stmt_list", "stmt", "select_stmt", "opt_where",
  "opt_groupby", "groupby_list", "opt_asc_desc", "opt_with_rollup",
  "opt_having", "opt_orderby", "orderby_list", "opt_limit",
  "opt_into_list", "column_list", "select_opts", "select_expr_list",
  "select_expr", "table_references", "table_reference", "table_factor",
  "opt_as", "opt_as_alias", "join_table", "opt_inner_cross", "opt_outer",
  "left_or_right", "opt_left_or_right_outer", "opt_join_condition",
  "join_condition", "table_subquery", "expr", "expr_list", "opt_expr_list",
  "trim_ltb", "cast_data_type", "interval_exp", "case_list",
  "create_database_stmt", "opt_if_not_exists", "create_table_stmt",
  "create_select_statement", "opt_ignore_replace", "opt_temporary",
  "create_col_list", "create_definition", "column_atts", "data_type",
  "enum_list", "opt_length", "opt_binary", "opt_uz", "opt_csc",
  "create_projection_stmt", "create_index_stmt", "index_att",
  "opt_using_type", "index_type", "index_col_list", "load_table_stmt",
  "drop_index_stmt", "drop_database_stmt", "opt_if_exists",
  "drop_table_stmt", "table_list", "opt_rc", "insert_stmt",
  "opt_ondupupdate", "insert_opts", "opt_into", "opt_col_names",
  "insert_vals_list", "insert_vals", "insert_asgn_list", YY_NULL
};
#endif

/* YYR1[YYN] -- Symbol number of symbol that rule YYN derives.  */
static const unsigned short int yyr1[] =
{
       0,   342,   343,   343,   343,   343,   344,   345,   345,   346,
     346,   347,   347,   348,   348,   349,   349,   349,   350,   350,
     351,   351,   352,   352,   353,   353,   354,   354,   354,   355,
     355,   356,   356,   357,   357,   357,   357,   357,   357,   357,
     357,   357,   358,   358,   358,   358,   359,   360,   360,   361,
     361,   362,   362,   362,   362,   363,   363,   364,   364,   364,
     365,   365,   365,   365,   365,   366,   366,   366,   367,   367,
     368,   368,   369,   369,   369,   370,   370,   371,   371,   372,
     373,   373,   373,   373,   373,   373,   373,   373,   373,   373,
     373,   373,   373,   373,   373,   373,   373,   373,   373,   373,
     373,   373,   373,   373,   373,   373,   373,   373,   373,   373,
     373,   373,   373,   373,   373,   373,   373,   374,   374,   374,
     375,   375,   373,   373,   373,   373,   373,   373,   373,   373,
     373,   373,   373,   373,   373,   373,   373,   373,   373,   373,
     373,   373,   373,   373,   373,   373,   373,   376,   376,   376,
     377,   377,   377,   377,   377,   377,   373,   373,   378,   378,
     378,   378,   378,   378,   378,   378,   378,   373,   373,   373,
     373,   379,   379,   373,   373,   373,   373,   344,   380,   380,
     381,   381,   344,   382,   382,   382,   382,   382,   382,   383,
     384,   384,   384,   385,   385,   386,   386,   387,   387,   387,
     387,   387,   387,   388,   388,   388,   388,   388,   388,   388,
     388,   388,   388,   388,   388,   388,   389,   389,   389,   389,
     389,   389,   389,   389,   389,   389,   389,   389,   389,   389,
     389,   389,   389,   389,   389,   389,   389,   389,   389,   389,
     389,   389,   389,   389,   389,   389,   390,   390,   391,   391,
     391,   392,   392,   393,   393,   393,   394,   394,   394,   344,
     395,   395,   344,   396,   397,   397,   397,   397,   398,   398,
     399,   399,   400,   400,   344,   401,   401,   401,   401,   344,
     402,   344,   403,   403,   404,   404,   344,   405,   406,   406,
     406,   406,   407,   407,   407,   344,   408,   409,   409,   410,
     410,   410,   410,   410,   411,   411,   412,   412,   413,   413,
     414,   414,   414,   414,   408,   408,   415,   415,   415,   415
};

/* YYR2[YYN] -- Number of symbols composing right hand side of rule YYN.  */
static const unsigned char yyr2[] =
{
       0,     2,     2,     3,     2,     3,     1,     3,    11,     0,
       2,     0,     4,     1,     3,     0,     1,     1,     0,     2,
       0,     2,     0,     3,     2,     4,     0,     2,     4,     0,
       2,     1,     3,     0,     2,     2,     2,     2,     2,     2,
       2,     2,     1,     3,     1,     3,     2,     1,     3,     1,
       1,     2,     4,     3,     3,     0,     1,     2,     1,     0,
       5,     3,     5,     6,     5,     0,     1,     1,     0,     1,
       1,     1,     2,     2,     0,     0,     1,     2,     4,     3,
       1,     1,     3,     3,     1,     1,     1,     1,     3,     3,
       3,     3,     3,     3,     2,     2,     3,     3,     3,     3,
       5,     6,     6,     6,     3,     3,     3,     3,     2,     2,
       3,     3,     3,     4,     3,     4,     5,     1,     3,     3,
       1,     3,     5,     7,     6,     8,     5,     7,     6,     8,
       4,     5,     4,     4,     4,     4,     4,     4,     6,     6,
       8,     8,     4,     7,     4,     6,     4,     1,     1,     1,
       0,     1,     1,     1,     1,     1,     6,     6,     3,     3,
       3,     3,     3,     3,     3,     3,     3,     4,     6,     3,
       5,     4,     5,     3,     4,     3,     4,     1,     4,     4,
       0,     3,     1,     8,    10,     9,     6,    11,     8,     3,
       0,     1,     1,     0,     1,     1,     3,     3,     5,     4,
       4,     5,     5,     0,     3,     2,     3,     3,     3,     3,
       2,     3,     3,     2,     3,     5,     2,     3,     3,     3,
       3,     3,     3,     3,     3,     3,     3,     1,     1,     1,
       1,     1,     3,     3,     2,     4,     1,     1,     1,     1,
       3,     3,     3,     3,     5,     5,     1,     3,     0,     3,
       5,     0,     1,     0,     2,     2,     0,     4,     3,     1,
      10,    13,     1,    10,     0,     1,     1,     1,     0,     2,
       1,     1,     3,     5,     1,    12,     9,     9,    12,     1,
       5,     1,     4,     4,     0,     2,     1,     6,     1,     3,
       3,     5,     0,     1,     1,     1,     8,     0,     5,     0,
       2,     2,     2,     2,     1,     0,     0,     3,     3,     5,
       1,     1,     3,     3,     7,     7,     3,     3,     5,     5
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
       0,     0,     0,     0,     0,     0,     0,     0,     0,     1,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     2,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
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
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0
};

/* YYDEFACT[S] -- default reduction number in state S.  Performed when
   YYTABLE doesn't specify something else to do.  Zero means the default
   is an error.  */
static const unsigned short int yydefact[] =
{
       0,     0,     0,   193,   193,   299,     0,    33,     0,     0,
       6,   177,   182,   259,   262,   274,   279,   281,   286,   295,
       4,     0,   180,   266,     0,   180,   267,   194,   265,     0,
       0,   284,     0,   284,     0,   305,     0,     0,     1,     0,
       0,     2,     0,     0,     0,     0,     0,   180,     0,     0,
       0,     0,     0,   284,   301,   302,   303,   304,   300,     0,
       0,    80,    84,    85,    87,    86,    81,     0,     0,     0,
       0,    44,    34,     0,    35,    36,     0,    37,    40,    41,
      39,    38,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     7,    42,    59,     5,     3,
       0,     0,   178,     0,   179,     0,   268,   285,   282,     0,
     283,     0,   306,     0,     0,     0,   108,     0,   109,    95,
      94,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,    58,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    46,   117,     0,   181,     0,
     190,     0,     0,   280,   288,   292,     0,     0,     0,     0,
      82,    83,   110,     0,    45,     0,     0,     0,   169,     0,
       0,     0,   149,   147,   148,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     111,     0,    59,     0,     9,    47,    49,    50,    55,    43,
      97,    98,    96,   175,   173,   114,     0,   112,     0,     0,
       0,     0,     0,     0,     0,     0,     0,    99,   104,   105,
     107,    88,    89,    90,    91,    93,    92,   106,    57,     0,
      31,     0,   191,   192,     0,     0,   186,    55,   270,   271,
     269,     0,     0,   294,   293,     0,   287,     0,   297,     0,
       0,   297,     0,     0,     0,     0,   167,     0,     0,   130,
       0,     0,   142,     0,     0,     0,   132,   133,   144,   150,
     146,   134,   135,   136,   137,   119,   118,     0,     0,     0,
      51,     0,     0,     0,    11,    67,    66,    70,    74,    71,
       0,     0,     0,    68,    56,     0,   115,   113,     0,     0,
     176,   174,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   190,     0,     0,     0,     0,     0,     0,   195,     0,
       0,   289,   290,     0,     0,     0,   314,   307,     0,   297,
     315,     0,   130,   171,     0,   170,     0,     0,     0,     0,
       0,     0,     0,   152,   155,   153,   154,   151,     0,     0,
       0,    59,    79,    54,    10,     0,    20,    68,    68,     0,
      61,    48,     0,    69,     0,    53,   126,   122,     0,     0,
     116,     0,     0,     0,   100,     0,    32,     0,     0,     0,
     188,   248,   248,   248,   237,   248,   227,   230,   248,   248,
       0,   248,   248,   248,   239,   251,   238,   248,   251,   248,
       0,   248,   251,   228,   229,   236,   248,   251,     0,   248,
     231,   203,     0,     0,     0,     0,     0,   190,     0,   189,
       0,     0,   317,   316,     0,     0,   311,   310,     0,   296,
       0,   168,   172,     0,   139,     0,   138,     0,     0,   156,
     157,   145,     0,   120,     0,     0,    52,     0,     0,    22,
      72,    73,     0,     0,    75,     0,   128,   124,   103,   101,
     102,   277,     0,     0,     0,     0,   253,   234,   216,   256,
     253,   253,     0,   253,   253,   253,   252,   256,   253,   256,
     253,     0,   253,   256,   253,   256,     0,   256,   197,     0,
       0,     0,     0,     0,   185,   196,   248,     0,   291,     0,
       0,     0,     0,   308,   276,     0,     0,   143,   158,   162,
     159,   160,   161,   166,   164,   163,   165,   127,     0,   123,
       0,     0,    18,    13,    21,     0,    26,    64,    62,     0,
       0,    60,    76,     0,     0,     0,   260,   190,     0,   222,
     232,   226,   224,   246,     0,   225,   220,   221,   243,   219,
     242,   223,     0,   218,   241,   217,   240,     0,   233,     0,
     210,     0,     0,   213,   205,     0,     0,     0,     0,   200,
     199,     0,    15,   263,     0,   319,   318,   313,   312,     0,
       0,   141,   140,   121,   129,   125,     0,    12,     0,     0,
       0,    29,    77,     0,    63,     0,     0,   187,     0,   249,
     254,   255,     0,     0,     0,   256,   256,   235,   204,   214,
     206,   207,   209,   208,   212,   211,     0,   201,   202,   198,
      16,    17,   272,   298,   309,     0,    19,    14,    23,    15,
      27,     0,     8,     0,   278,     0,     0,     0,   258,   247,
     244,   245,     0,     0,   275,    24,     0,    30,    78,   261,
     250,   257,   215,   273,     0,    28,    25
};

/* YYPDEFGOTO[NTERM-NUM].  */
static const short int yydefgoto[] =
{
      -1,     8,     9,    10,   304,   376,   542,   642,   607,   469,
     546,   648,   611,   652,   251,    37,    95,    96,   214,   215,
     216,   315,   165,   217,   312,   384,   313,   379,   551,   552,
     218,   166,   140,   464,   196,   368,   361,   124,    11,    44,
      12,   256,   257,    29,   337,   338,   508,   431,   564,   486,
     497,   559,   560,    13,    14,    30,   172,   260,   517,    15,
      16,    17,    50,    18,   175,   266,    19,   346,    35,    59,
     178,   349,   448,   268
};

/* YYPACT[STATE-NUM] -- Index in YYTABLE of the portion describing
   STATE-NUM.  */
#define YYPACT_NINF -440
static const short int yypact[] =
{
     132,  -206,  -165,   117,   -16,  -440,   -76,  -440,   424,  -199,
    -440,  -440,  -440,  -440,  -440,  -440,  -440,  -440,  -440,  -440,
    -440,   140,    74,  -440,    44,    74,  -440,  -440,  -440,    -8,
      94,    97,   212,    97,     6,    70,   221,    55,  -440,   -98,
     -97,  -440,   122,   223,   227,   242,   244,    74,   245,   150,
     248,    95,   249,    97,  -440,  -440,  -440,  -440,  -440,   254,
     149,   -74,  -440,  -440,  -440,  -440,   251,   766,   790,   766,
     766,   -69,  -440,   397,  -440,  -440,   -72,  -440,  -440,  -440,
    -440,  -440,   -65,   -51,   -50,   -47,   -35,   -32,   -31,   -30,
     -24,   -23,   -20,   -14,   839,   164,   -46,  2027,  -440,  -440,
     839,   188,  -440,   -13,  -440,   321,    96,  -440,  -440,   325,
    -440,   326,  -172,   839,    18,   766,  1493,    -5,  1493,  -440,
    -440,   500,   766,  1599,   -78,   139,   766,   203,   766,   766,
     587,   766,   766,   839,   766,   766,   766,   766,   839,   970,
      26,     0,   500,  -440,   766,   766,   766,   766,   766,     7,
       1,   142,   766,   552,   766,   766,   766,   766,   766,   766,
     766,   766,   766,   766,   340,  -440,  1410,   108,  -440,   365,
    -108,  -265,   213,  -440,    34,   -44,   367,   365,   -49,   133,
    -440,  -440,  2071,   139,  -440,  1647,   -70,   766,  -440,   766,
      36,  1389,  -440,  -440,  -440,  1170,   766,  1453,  1479,    51,
    1191,  1212,  2049,    52,  1233,  1254,  1275,  1302,    53,   839,
    -440,    25,    -1,     2,   153,    49,  -440,  -440,   358,  -440,
    2091,  2110,   880,  1428,  1428,  -440,    10,  -440,   616,   766,
     766,    58,  1998,    66,    67,    68,   616,   845,   906,   674,
     271,   253,   253,   390,   390,   390,   390,  -440,  -440,   422,
      90,    92,  -440,  -440,   431,    31,  -440,   358,  -440,  -440,
    -440,   432,   435,  -440,  -440,   436,  -440,   421,  -147,   103,
     105,   291,   446,   110,   766,   766,  -440,  1919,  1671,  -440,
     766,   766,  -440,  1898,   327,   327,  -440,  -440,  -440,   370,
    -440,  -440,  -440,  -440,  -440,    25,  -440,   112,   438,   452,
    -440,   120,   123,   766,   347,  -440,  -440,  -440,  -116,  -440,
       0,     0,   329,   301,  -440,   464,  -440,  -440,   127,   128,
    1428,  1428,   616,   766,   139,   139,   139,   129,   136,   365,
     -21,  -107,  1660,   -79,   137,   138,   346,   141,   143,   139,
     147,  -440,   152,   639,   320,   477,  -440,  -440,   688,   291,
    -440,   151,   336,  2071,  1940,  -440,   766,  1023,  1121,   766,
     766,   156,   157,  -440,  -440,  -440,  -440,  -440,   159,   616,
     148,    37,  -440,  -440,  2071,   444,   382,   301,   301,   372,
     344,  -440,     0,  -440,   380,  -440,  -440,  -440,   170,   172,
     981,   174,   175,   178,  -440,   516,  -440,   501,   364,    31,
    -440,   195,   195,   195,  -440,   195,  -440,  -440,   195,   195,
     200,   195,   195,   195,  -440,   497,  -440,   195,   497,   195,
     202,   195,   497,  -440,  -440,  -440,   195,   497,   204,   195,
    -440,  -440,   205,   206,   365,   365,   209,   -95,    31,  -440,
     547,   549,  -440,  2071,   433,   534,   226,  1515,   220,  -440,
     561,  -440,  2071,   766,  -440,   766,  -440,  1323,  1578,  -440,
    -440,  -440,   225,   229,   233,   616,  -440,   766,   766,   405,
    -440,  -440,     0,   766,  -138,     0,  -440,  -440,  -440,  -440,
    -440,   386,   575,   579,   246,   583,  -440,  -440,  -440,  -440,
    -440,  -440,   592,  -440,  -440,  -440,  -440,  -440,  -440,  -440,
    -440,   592,  -440,  -440,  -440,  -440,   593,  -440,   116,   365,
     365,   256,   259,   365,  -440,  -440,   195,   260,  -440,   371,
     738,   688,   688,   268,   418,  1347,  1368,  -440,  -440,  -440,
    -440,  -440,  -440,  -440,  -440,  -440,  -440,  -440,   839,  -440,
     267,   269,   369,  1536,  2071,   567,   473,  -440,  2071,   766,
     276,  -440,  -440,  -131,   598,   458,  -440,   -53,  -289,  -183,
     107,  -183,  -183,  -440,  -193,  -183,  -183,  -183,   107,  -183,
     107,  -183,  -137,  -183,   107,  -183,   107,   285,   107,   471,
    -440,   624,   308,  -440,  -440,   502,  -123,   289,   290,  -440,
    -440,   294,    64,  -440,   367,  -440,  2071,  -440,  -440,   105,
     628,  -440,  -440,  -440,  -440,  -440,   448,  -440,   766,   766,
     766,   522,  2071,   365,  -440,   643,   494,  -440,   648,  -440,
    -440,  -440,   459,   651,   654,  -440,  -440,  -440,  -440,  -440,
    -440,  -440,  -440,  -440,  -440,  -440,   365,  -440,  -440,  -440,
    -440,  -440,   322,   323,  -440,   652,  -440,  -440,  -440,  1961,
    1557,   365,  -440,   324,  -440,   659,   328,   663,  -440,  -440,
     107,   107,   330,   547,  -440,   332,   766,  -440,  -440,  -440,
    -440,  -440,  -440,  -440,   766,  2071,  -440
};

/* YYPGOTO[NTERM-NUM].  */
static const short int yypgoto[] =
{
    -440,  -440,   660,   -48,  -440,  -440,    73,    23,  -440,  -440,
    -440,     8,  -440,  -440,  -176,  -440,   -86,  -440,  -184,  -440,
    -296,   427,  -200,  -440,  -440,  -308,  -440,  -440,  -440,   135,
    -440,   -37,   -96,   169,  -440,  -440,   423,   586,  -440,    21,
    -440,  -323,  -440,   706,  -371,  -440,  -440,  -440,   210,  1353,
    -204,   185,  -322,  -440,  -440,  -440,  -440,  -440,    54,  -440,
    -440,  -440,    12,  -440,  -440,  -440,  -440,  -221,  -440,  -440,
    -440,   115,  -439,   124
};

/* YYTABLE[YYPACT[STATE-NUM]].  What to do in state STATE-NUM.  If
   positive, shift that token.  If negative, reduce the rule which
   number is the opposite.  If YYTABLE_NINF, syntax error.  */
#define YYTABLE_NINF -265
static const short int yytable[] =
{
      97,   269,   143,   212,   167,   212,   263,   635,   400,   252,
     252,   344,   300,   225,   380,   187,   316,   179,   188,   377,
     549,   180,   252,   275,   176,   226,   276,   549,   484,   302,
     116,   118,   119,   120,   332,   184,   123,   203,   164,   432,
     143,   297,   208,   298,   181,    52,    46,   620,    21,   618,
     350,   433,   619,    31,   258,   259,   219,   139,    61,    62,
      63,    64,    65,    66,   252,   111,   621,   515,   105,   470,
     471,   378,    67,    68,   253,   253,   164,   190,   182,    69,
      70,    71,   597,   598,    97,   185,   474,   253,    72,   191,
     195,   197,   198,   200,   201,   202,   550,   204,   205,   206,
     207,   139,    32,   550,   640,    97,    73,   220,   221,   222,
     223,   224,   305,   296,   514,   232,   237,   238,   239,   240,
     241,   242,   243,   244,   245,   246,   247,   381,   449,   253,
     271,    20,   319,     1,   579,   273,   397,    36,    41,   333,
     264,    74,    75,    42,     7,   624,   398,   641,   625,   334,
     277,    54,   278,   396,    76,   229,   230,   580,   231,   283,
     622,   335,   623,   227,   189,   301,   317,    77,   177,   306,
       2,   466,   189,   581,    33,   568,   547,   570,   -65,   553,
     318,   574,    55,   576,   307,   578,    22,    56,   327,   270,
      43,   345,   320,   321,     3,     7,   582,    57,    27,   139,
     336,   624,    45,   308,   626,    47,    61,    62,    63,    64,
      65,    66,    48,    49,   499,    51,    58,   636,   503,    53,
      67,    68,     4,   505,    60,    23,   389,    69,    70,   100,
     102,   254,   255,   399,   617,  -264,   309,   353,   354,    98,
      99,   101,  -183,   357,   358,   103,   583,   104,   106,   107,
     192,   108,   110,   109,    73,     5,   113,   112,   511,   512,
     115,   310,    78,    79,    80,   114,   374,    81,   125,   121,
       6,   141,   584,   463,   388,   126,   391,   392,   393,   159,
     160,   161,   162,   163,  -184,   585,   390,   168,    24,   127,
     128,   439,   142,   129,   265,   157,   158,   159,   160,   161,
     162,   163,    76,   660,   661,   130,   443,    25,   131,   132,
     133,   447,   630,   631,   632,   633,   134,   135,    26,   452,
     136,   462,   457,   458,   170,     7,   137,   169,   173,   174,
     171,    27,     7,   587,   588,   183,   193,   591,   299,  -130,
     213,   228,   213,   248,   586,    28,  -130,  -130,  -130,  -130,
    -130,  -130,  -130,   249,  -130,  -130,  -130,  -130,  -130,  -130,
    -130,  -130,  -130,  -130,  -130,  -130,  -130,   211,   250,   541,
     267,   261,  -130,   262,   363,  -130,  -130,   279,   272,    82,
      83,    84,    85,    86,    87,    88,    89,   311,    90,    91,
      92,    93,   286,   290,   295,    94,   303,   314,   322,  -130,
      61,    62,    63,    64,    65,    66,   324,   325,   326,  -130,
    -130,  -130,  -130,  -130,    67,    68,   525,   540,   526,  -130,
     163,    69,    70,   364,    38,    39,   328,   194,   329,  -130,
     543,   544,  -130,   330,   331,   340,   548,   653,   341,   342,
    -130,   343,   463,  -130,   347,   348,  -130,  -130,    73,   344,
     351,   352,   369,   360,   370,   371,  -130,   375,   382,   365,
     662,   372,     2,  -130,   373,  -130,   383,   385,   386,   387,
     394,  -130,  -130,   366,   395,   667,   436,   434,   435,   444,
     445,   438,   437,   596,   447,   447,     3,   440,   465,   450,
    -130,   441,   467,   468,  -130,   367,    76,   459,   460,  -130,
     461,   472,   473,    61,    62,    63,    64,    65,    66,   475,
    -130,   476,   612,   477,     4,   478,   479,    67,    68,   480,
     481,   482,   483,  -130,    69,    70,    71,    82,    83,    84,
      85,    86,    87,    88,    89,   485,    90,    91,    92,    93,
     492,   496,   501,    94,   506,   509,   510,     5,  -130,   513,
     516,    73,   518,  -130,   520,    61,    62,    63,    64,    65,
      66,   523,     6,   519,   521,   524,   537,   538,   545,    67,
      68,   543,   649,   650,   539,   554,    69,    70,  -130,  -130,
     555,  -130,   556,  -130,  -130,   233,  -130,   557,   558,   234,
      61,    62,    63,    64,    65,    66,   563,   589,   577,    76,
     590,   593,   594,    73,    67,    68,   599,   600,   604,   610,
     605,    69,    70,   199,   606,   609,   613,     7,   615,    61,
      62,    63,    64,    65,    66,   616,   627,   628,   629,   675,
     637,   638,   634,    67,    68,   639,   646,   649,    73,   122,
      69,    70,    61,    62,    63,    64,    65,    66,   645,   651,
     654,    76,   655,   656,   658,   657,    67,    68,   659,   664,
     663,   345,   669,    69,    70,   668,   671,    73,    40,   670,
     674,   672,   665,  -130,  -130,   561,   562,  -130,   565,   566,
     567,   647,   676,   569,   339,   571,    76,   573,   614,   575,
      73,    61,    62,    63,    64,    65,    66,   156,   157,   158,
     159,   160,   161,   162,   163,    67,    68,   603,   362,   186,
      34,   572,    69,    70,   644,    76,     0,   673,   643,   442,
       0,    82,    83,    84,    85,    86,    87,    88,    89,     0,
      90,    91,    92,    93,     0,     0,     0,    94,    76,    73,
       0,    61,    62,    63,    64,    65,    66,     0,     0,     0,
       0,   235,     0,     0,     0,    67,    68,     0,     0,     0,
       0,     0,    69,    70,     0,     0,     0,     0,   446,    61,
      62,    63,    64,    65,    66,     0,     0,     0,     0,     0,
       0,     0,     0,    67,    68,     0,     0,    76,     0,    73,
      69,    70,     0,    61,    62,    63,    64,    65,    66,     0,
       0,     0,     0,     0,     0,     0,     0,    67,    68,     7,
       0,     0,     0,     0,    69,    70,     0,    73,   595,     0,
       0,     0,     0,     0,    82,    83,    84,    85,    86,    87,
      88,    89,     0,    90,    91,    92,    93,    76,     0,     0,
      94,    73,    61,    62,    63,    64,    65,    66,     0,     0,
       0,     0,     0,     0,     0,     0,    67,    68,     0,     0,
       0,     0,     0,    69,    70,    76,   154,   155,   156,   157,
     158,   159,   160,   161,   162,   163,    82,    83,    84,    85,
      86,    87,    88,    89,     0,    90,    91,    92,    93,   117,
      73,     0,   236,   147,   148,   149,   150,     0,   151,   152,
     153,   154,   155,   156,   157,   158,   159,   160,   161,   162,
     163,    82,    83,    84,    85,    86,    87,    88,    89,     0,
      90,    91,    92,    93,     0,     0,     0,    94,   155,   156,
     157,   158,   159,   160,   161,   162,   163,     0,    76,     0,
      82,    83,    84,    85,    86,    87,    88,    89,     0,    90,
      91,    92,    93,     0,     0,     0,   138,     0,     0,     0,
       0,     0,     0,    82,    83,    84,    85,    86,    87,    88,
      89,     0,    90,    91,    92,    93,     0,     0,     0,    94,
     144,   145,   146,   147,   148,   149,   150,     0,   151,   152,
     153,   154,   155,   156,   157,   158,   159,   160,   161,   162,
     163,   153,   154,   155,   156,   157,   158,   159,   160,   161,
     162,   163,    82,    83,    84,    85,    86,    87,    88,    89,
       0,    90,    91,    92,    93,     0,     0,     0,    94,     0,
       0,     0,     0,   144,   145,   146,   147,   148,   149,   150,
       0,   151,   152,   153,   154,   155,   156,   157,   158,   159,
     160,   161,   162,   163,     0,     0,     0,     0,     0,     0,
       0,     0,    82,    83,    84,    85,    86,    87,    88,    89,
       0,    90,    91,    92,    93,     0,     0,     0,    94,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      82,    83,    84,    85,    86,    87,    88,    89,     0,    90,
      91,    92,    93,     0,     0,     0,    94,     0,     0,     0,
       0,     0,     0,     0,    82,    83,    84,    85,    86,    87,
      88,    89,     0,    90,    91,    92,    93,   453,     0,     0,
      94,   144,   145,   146,   147,   148,   149,   150,     0,   151,
     152,   153,   154,   155,   156,   157,   158,   159,   160,   161,
     162,   163,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,    82,    83,    84,    85,    86,    87,    88,
      89,     0,    90,    91,    92,    93,     0,     0,     0,   138,
     144,   145,   146,   147,   148,   149,   150,     0,   151,   152,
     153,   154,   155,   156,   157,   158,   159,   160,   161,   162,
     163,   144,   145,   146,   147,   148,   149,   150,     0,   151,
     152,   153,   154,   155,   156,   157,   158,   159,   160,   161,
     162,   163,   144,   145,   146,   147,   148,   149,   150,     0,
     151,   152,   153,   154,   155,   156,   157,   158,   159,   160,
     161,   162,   163,   144,   145,   146,   147,   148,   149,   150,
       0,   151,   152,   153,   154,   155,   156,   157,   158,   159,
     160,   161,   162,   163,   144,   145,   146,   147,   148,   149,
     150,     0,   151,   152,   153,   154,   155,   156,   157,   158,
     159,   160,   161,   162,   163,   144,   145,   146,   147,   148,
     149,   150,     0,   151,   152,   153,   154,   155,   156,   157,
     158,   159,   160,   161,   162,   163,     0,     0,   209,     0,
       0,   210,   144,   145,   146,   147,   148,   149,   150,     0,
     151,   152,   153,   154,   155,   156,   157,   158,   159,   160,
     161,   162,   163,   144,   145,   146,   147,   148,   149,   150,
       0,   151,   152,   153,   154,   155,   156,   157,   158,   159,
     160,   161,   162,   163,     0,     0,     0,   144,   145,   146,
     147,   148,   149,   150,   454,   151,   152,   153,   154,   155,
     156,   157,   158,   159,   160,   161,   162,   163,   144,   145,
     146,   147,   148,   149,   150,     0,   151,   152,   153,   154,
     155,   156,   157,   158,   159,   160,   161,   162,   163,   144,
     145,   146,   147,   148,   149,   150,     0,   151,   152,   153,
     154,   155,   156,   157,   158,   159,   160,   161,   162,   163,
     144,   145,   146,   147,   148,   149,   150,     0,   151,   152,
     153,   154,   155,   156,   157,   158,   159,   160,   161,   162,
     163,  -265,  -265,  -265,  -265,     0,   151,   152,   153,   154,
     155,   156,   157,   158,   159,   160,   161,   162,   163,   455,
       0,     0,   456,   144,   145,   146,   147,   148,   149,   150,
       0,   151,   152,   153,   154,   155,   156,   157,   158,   159,
     160,   161,   162,   163,     0,     0,     0,     0,     0,   144,
     145,   146,   147,   148,   149,   150,   280,   151,   152,   153,
     154,   155,   156,   157,   158,   159,   160,   161,   162,   163,
       0,   282,   152,   153,   154,   155,   156,   157,   158,   159,
     160,   161,   162,   163,     0,   144,   145,   146,   147,   148,
     149,   150,   287,   151,   152,   153,   154,   155,   156,   157,
     158,   159,   160,   161,   162,   163,   144,   145,   146,   147,
     148,   149,   150,   288,   151,   152,   153,   154,   155,   156,
     157,   158,   159,   160,   161,   162,   163,   144,   145,   146,
     147,   148,   149,   150,   291,   151,   152,   153,   154,   155,
     156,   157,   158,   159,   160,   161,   162,   163,   144,   145,
     146,   147,   148,   149,   150,   292,   151,   152,   153,   154,
     155,   156,   157,   158,   159,   160,   161,   162,   163,   144,
     145,   146,   147,   148,   149,   150,   293,   151,   152,   153,
     154,   155,   156,   157,   158,   159,   160,   161,   162,   163,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   294,     0,     0,     0,     0,     0,     0,
       0,   528,   529,   530,   531,   532,     0,   144,   145,   146,
     147,   148,   149,   150,   527,   151,   152,   153,   154,   155,
     156,   157,   158,   159,   160,   161,   162,   163,     0,     0,
       0,   144,   145,   146,   147,   148,   149,   150,   601,   151,
     152,   153,   154,   155,   156,   157,   158,   159,   160,   161,
     162,   163,     0,   401,   402,   403,   404,     0,     0,   602,
       0,     0,     0,   405,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   281,     0,     0,
       0,   406,   407,     0,     0,     0,     0,     0,   408,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   209,   409,
       0,     0,   533,     0,     0,   487,   488,   410,   489,     0,
       0,   490,   491,   411,   493,   494,   495,     0,     0,     0,
     498,     0,   500,     0,   502,     0,     0,     0,     0,   504,
       0,     0,   507,     0,   412,   413,     0,     0,     0,     0,
       0,   284,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   414,   415,     0,     0,     0,   416,   417,
     418,     0,     0,     0,     0,     0,     0,   285,     0,     0,
       0,     0,     0,     0,     0,   534,   535,     0,   536,     0,
       0,     0,     0,     0,     0,     0,     0,   419,     0,     0,
       0,   122,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   522,     0,     0,   420,     0,   421,     0,
       0,     0,     0,     0,   274,     0,     0,     0,     0,   592,
       0,     0,     0,     0,   608,   422,     0,     0,   423,   424,
     425,   426,   427,     0,     0,     0,     0,     0,   356,     0,
       0,     0,     0,     0,     0,   666,     0,     0,     0,   428,
     429,     0,     0,     0,     0,     0,     0,   430,   144,   145,
     146,   147,   148,   149,   150,     0,   151,   152,   153,   154,
     155,   156,   157,   158,   159,   160,   161,   162,   163,   144,
     145,   146,   147,   148,   149,   150,     0,   151,   152,   153,
     154,   155,   156,   157,   158,   159,   160,   161,   162,   163,
     144,   145,   146,   147,   148,   149,   150,     0,   151,   152,
     153,   154,   155,   156,   157,   158,   159,   160,   161,   162,
     163,   144,   145,   146,   147,   148,   149,   150,     0,   151,
     152,   153,   154,   155,   156,   157,   158,   159,   160,   161,
     162,   163,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   640,     0,     0,     0,   359,     0,     0,   144,   145,
     146,   147,   148,   149,   150,   355,   151,   152,   153,   154,
     155,   156,   157,   158,   159,   160,   161,   162,   163,     0,
     143,     0,     0,     0,   323,     0,   451,   144,   145,   146,
     147,   148,   149,   150,   641,   151,   152,   153,   154,   155,
     156,   157,   158,   159,   160,   161,   162,   163,     0,   144,
     145,   146,   147,   148,   149,   150,   164,   151,   152,   153,
     154,   155,   156,   157,   158,   159,   160,   161,   162,   163,
       0,   144,   145,   146,   147,   148,   149,   150,   289,   151,
     152,   153,   154,   155,   156,   157,   158,   159,   160,   161,
     162,   163,   145,   146,   147,   148,   149,   150,     0,   151,
     152,   153,   154,   155,   156,   157,   158,   159,   160,   161,
     162,   163,   146,   147,   148,   149,   150,     0,   151,   152,
     153,   154,   155,   156,   157,   158,   159,   160,   161,   162,
     163
};

/* YYCONFLP[YYPACT[STATE-NUM]] -- Pointer into YYCONFL of start of
   list of conflicting reductions corresponding to action entry for
   state STATE-NUM in yytable.  0 means no conflicts.  The list in
   yyconfl is terminated by a rule number of 0.  */
static const unsigned char yyconflp[] =
{
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     123,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
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
       0,     0,    47,     0,     0,    49,    51,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    53,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    55,
      57,    59,    61,    63,     0,     0,     0,     0,     0,    65,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    67,
       0,     0,    69,     0,     0,     0,     0,     0,     0,     0,
      71,     0,     0,    73,     0,     0,    75,    77,     0,     0,
       0,     0,     0,     0,     0,     0,    79,     0,     0,     0,
       0,     0,     0,    81,     0,    83,     0,     0,     0,     0,
       0,    85,    87,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      89,     0,     0,     0,    91,     0,     0,     0,     0,    93,
       0,     0,   121,     0,     0,     0,     0,     0,     0,     0,
      95,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,    97,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,    99,     0,
       0,     0,     0,   101,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   103,   105,
       0,   107,     0,   109,   111,     0,   113,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   115,   117,     0,     0,   119,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
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
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     3,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0
};

/* YYCONFL[I] -- lists of conflicting rule numbers, each terminated by
   0, pointed into by YYCONFLP.  */
static const short int yyconfl[] =
{
       0,   117,     0,   117,     0,   131,     0,   131,     0,   131,
       0,   131,     0,   131,     0,   131,     0,   131,     0,   131,
       0,   131,     0,   131,     0,   131,     0,   131,     0,   131,
       0,   131,     0,   131,     0,   131,     0,   131,     0,   131,
       0,   131,     0,   131,     0,   131,     0,   131,     0,   131,
       0,   131,     0,   131,     0,   131,     0,   131,     0,   131,
       0,   131,     0,   131,     0,   131,     0,   131,     0,   131,
       0,   131,     0,   131,     0,   131,     0,   131,     0,   131,
       0,   131,     0,   131,     0,   131,     0,   131,     0,   131,
       0,   131,     0,   131,     0,   131,     0,   131,     0,   131,
       0,   131,     0,   131,     0,   131,     0,   131,     0,   131,
       0,   131,     0,   131,     0,   131,     0,   131,     0,   131,
       0,    61,     0,    75,     0
};

static const short int yycheck[] =
{
      37,   177,     3,     3,   100,     3,    50,   130,   331,   117,
     117,   158,   212,     6,   310,    93,     6,   113,    96,   135,
     158,     3,   117,    93,   196,    18,    96,   158,   399,   213,
      67,    68,    69,    70,     3,   121,    73,   133,    39,   118,
       3,    16,   138,    18,    26,    33,    25,   230,   213,   338,
     271,   130,   341,    69,   319,   320,   142,    94,     3,     4,
       5,     6,     7,     8,   117,    53,   249,   438,    47,   377,
     378,   187,    17,    18,   182,   182,    39,   125,   115,    24,
      25,    26,   521,   522,   121,   122,   382,   182,    33,   126,
     127,   128,   129,   130,   131,   132,   234,   134,   135,   136,
     137,   138,   118,   234,    40,   142,    51,   144,   145,   146,
     147,   148,    63,   209,   437,   152,   153,   154,   155,   156,
     157,   158,   159,   160,   161,   162,   163,   311,   349,   182,
     178,   337,   228,     1,    18,   183,   157,   213,   337,   108,
     184,    86,    87,     3,   193,   338,   167,    83,   341,   118,
     187,    81,   189,   329,    99,    13,    14,    41,    16,   196,
      53,   130,    55,   156,   242,   213,   156,   112,   340,   120,
      38,   371,   242,    57,   190,   497,   472,   499,   129,   475,
     228,   503,   112,   505,   135,   507,    69,   117,   236,   238,
     116,   338,   229,   230,    62,   193,    80,   127,   214,   236,
     169,   338,   158,   154,   341,   213,     3,     4,     5,     6,
       7,     8,   118,   116,   418,     3,   146,   340,   422,   213,
      17,    18,    90,   427,     3,   108,   322,    24,    25,   107,
       3,   339,   340,   340,   557,   118,   187,   274,   275,   337,
     337,    18,   337,   280,   281,     3,   130,     3,     3,    99,
      47,     3,     3,   158,    51,   123,   107,     3,   434,   435,
       9,   212,   207,   208,   209,   339,   303,   212,   340,   338,
     138,   107,   156,   369,   322,   340,   324,   325,   326,    26,
      27,    28,    29,    30,   337,   169,   323,    99,   171,   340,
     340,   339,   338,   340,   338,    24,    25,    26,    27,    28,
      29,    30,    99,   625,   626,   340,   343,   190,   340,   340,
     340,   348,     4,     5,     6,     7,   340,   340,   201,   356,
     340,   369,   359,   360,     3,   193,   340,   340,     3,     3,
     234,   214,   193,   509,   510,   340,   133,   513,   339,     3,
     340,   340,   340,     3,   228,   228,    10,    11,    12,    13,
      14,    15,    16,   245,    18,    19,    20,    21,    22,    23,
      24,    25,    26,    27,    28,    29,    30,   341,     3,   465,
       3,   158,    36,   339,     4,    39,    40,   341,   245,   324,
     325,   326,   327,   328,   329,   330,   331,   338,   333,   334,
     335,   336,   341,   341,   341,   340,   243,    39,   340,    63,
       3,     4,     5,     6,     7,     8,   340,   340,   340,    73,
      74,    75,    76,    77,    17,    18,   453,   465,   455,    83,
      30,    24,    25,    53,     0,     1,     4,   224,   338,    93,
     467,   468,    96,   341,     3,     3,   473,   613,     3,     3,
     104,    20,   538,   107,   341,   340,   110,   111,    51,   158,
       4,   341,   340,   126,    16,     3,   120,   110,   129,    89,
     636,   341,    38,   127,   341,   129,   165,     3,   341,   341,
     341,   135,   136,   103,   338,   651,   130,   340,   340,   159,
       3,   338,   341,   520,   521,   522,    62,   340,   340,   338,
     154,   339,    48,   111,   158,   125,    99,   341,   341,   163,
     341,   129,   158,     3,     4,     5,     6,     7,     8,   129,
     174,   341,   549,   341,    90,   341,   341,    17,    18,   341,
       4,    20,   158,   187,    24,    25,    26,   324,   325,   326,
     327,   328,   329,   330,   331,   340,   333,   334,   335,   336,
     340,    44,   340,   340,   340,   340,   340,   123,   212,   340,
       3,    51,     3,   217,    20,     3,     4,     5,     6,     7,
       8,   341,   138,   130,   338,     4,   341,   338,   163,    17,
      18,   608,   609,   610,   341,   189,    24,    25,   242,   243,
       5,   245,     3,   247,   248,    33,   250,   341,     5,    37,
       3,     4,     5,     6,     7,     8,     4,   341,     5,    99,
     341,   341,   231,    51,    17,    18,   338,   189,   341,   136,
     341,    24,    25,    26,   245,    48,   340,   193,    20,     3,
       4,     5,     6,     7,     8,   167,   341,   156,     4,   666,
     341,   341,   130,    17,    18,   341,   188,   674,    51,   242,
      24,    25,     3,     4,     5,     6,     7,     8,    20,   127,
       7,    99,   158,     5,     3,   196,    17,    18,     4,     7,
     338,   338,     3,    24,    25,   341,     3,    51,     8,   341,
     338,   341,   649,   337,   338,   490,   491,   341,   493,   494,
     495,   608,   674,   498,   257,   500,    99,   502,   553,   504,
      51,     3,     4,     5,     6,     7,     8,    23,    24,    25,
      26,    27,    28,    29,    30,    17,    18,   538,   285,   123,
       4,   501,    24,    25,   599,    99,    -1,   663,   594,    80,
      -1,   324,   325,   326,   327,   328,   329,   330,   331,    -1,
     333,   334,   335,   336,    -1,    -1,    -1,   340,    99,    51,
      -1,     3,     4,     5,     6,     7,     8,    -1,    -1,    -1,
      -1,   199,    -1,    -1,    -1,    17,    18,    -1,    -1,    -1,
      -1,    -1,    24,    25,    -1,    -1,    -1,    -1,    80,     3,
       4,     5,     6,     7,     8,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    17,    18,    -1,    -1,    99,    -1,    51,
      24,    25,    -1,     3,     4,     5,     6,     7,     8,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    17,    18,   193,
      -1,    -1,    -1,    -1,    24,    25,    -1,    51,    80,    -1,
      -1,    -1,    -1,    -1,   324,   325,   326,   327,   328,   329,
     330,   331,    -1,   333,   334,   335,   336,    99,    -1,    -1,
     340,    51,     3,     4,     5,     6,     7,     8,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    17,    18,    -1,    -1,
      -1,    -1,    -1,    24,    25,    99,    21,    22,    23,    24,
      25,    26,    27,    28,    29,    30,   324,   325,   326,   327,
     328,   329,   330,   331,    -1,   333,   334,   335,   336,    99,
      51,    -1,   340,    13,    14,    15,    16,    -1,    18,    19,
      20,    21,    22,    23,    24,    25,    26,    27,    28,    29,
      30,   324,   325,   326,   327,   328,   329,   330,   331,    -1,
     333,   334,   335,   336,    -1,    -1,    -1,   340,    22,    23,
      24,    25,    26,    27,    28,    29,    30,    -1,    99,    -1,
     324,   325,   326,   327,   328,   329,   330,   331,    -1,   333,
     334,   335,   336,    -1,    -1,    -1,   340,    -1,    -1,    -1,
      -1,    -1,    -1,   324,   325,   326,   327,   328,   329,   330,
     331,    -1,   333,   334,   335,   336,    -1,    -1,    -1,   340,
      10,    11,    12,    13,    14,    15,    16,    -1,    18,    19,
      20,    21,    22,    23,    24,    25,    26,    27,    28,    29,
      30,    20,    21,    22,    23,    24,    25,    26,    27,    28,
      29,    30,   324,   325,   326,   327,   328,   329,   330,   331,
      -1,   333,   334,   335,   336,    -1,    -1,    -1,   340,    -1,
      -1,    -1,    -1,    10,    11,    12,    13,    14,    15,    16,
      -1,    18,    19,    20,    21,    22,    23,    24,    25,    26,
      27,    28,    29,    30,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   324,   325,   326,   327,   328,   329,   330,   331,
      -1,   333,   334,   335,   336,    -1,    -1,    -1,   340,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     324,   325,   326,   327,   328,   329,   330,   331,    -1,   333,
     334,   335,   336,    -1,    -1,    -1,   340,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   324,   325,   326,   327,   328,   329,
     330,   331,    -1,   333,   334,   335,   336,   104,    -1,    -1,
     340,    10,    11,    12,    13,    14,    15,    16,    -1,    18,
      19,    20,    21,    22,    23,    24,    25,    26,    27,    28,
      29,    30,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   324,   325,   326,   327,   328,   329,   330,
     331,    -1,   333,   334,   335,   336,    -1,    -1,    -1,   340,
      10,    11,    12,    13,    14,    15,    16,    -1,    18,    19,
      20,    21,    22,    23,    24,    25,    26,    27,    28,    29,
      30,    10,    11,    12,    13,    14,    15,    16,    -1,    18,
      19,    20,    21,    22,    23,    24,    25,    26,    27,    28,
      29,    30,    10,    11,    12,    13,    14,    15,    16,    -1,
      18,    19,    20,    21,    22,    23,    24,    25,    26,    27,
      28,    29,    30,    10,    11,    12,    13,    14,    15,    16,
      -1,    18,    19,    20,    21,    22,    23,    24,    25,    26,
      27,    28,    29,    30,    10,    11,    12,    13,    14,    15,
      16,    -1,    18,    19,    20,    21,    22,    23,    24,    25,
      26,    27,    28,    29,    30,    10,    11,    12,    13,    14,
      15,    16,    -1,    18,    19,    20,    21,    22,    23,    24,
      25,    26,    27,    28,    29,    30,    -1,    -1,   338,    -1,
      -1,   341,    10,    11,    12,    13,    14,    15,    16,    -1,
      18,    19,    20,    21,    22,    23,    24,    25,    26,    27,
      28,    29,    30,    10,    11,    12,    13,    14,    15,    16,
      -1,    18,    19,    20,    21,    22,    23,    24,    25,    26,
      27,    28,    29,    30,    -1,    -1,    -1,    10,    11,    12,
      13,    14,    15,    16,   341,    18,    19,    20,    21,    22,
      23,    24,    25,    26,    27,    28,    29,    30,    10,    11,
      12,    13,    14,    15,    16,    -1,    18,    19,    20,    21,
      22,    23,    24,    25,    26,    27,    28,    29,    30,    10,
      11,    12,    13,    14,    15,    16,    -1,    18,    19,    20,
      21,    22,    23,    24,    25,    26,    27,    28,    29,    30,
      10,    11,    12,    13,    14,    15,    16,    -1,    18,    19,
      20,    21,    22,    23,    24,    25,    26,    27,    28,    29,
      30,    13,    14,    15,    16,    -1,    18,    19,    20,    21,
      22,    23,    24,    25,    26,    27,    28,    29,    30,   338,
      -1,    -1,   341,    10,    11,    12,    13,    14,    15,    16,
      -1,    18,    19,    20,    21,    22,    23,    24,    25,    26,
      27,    28,    29,    30,    -1,    -1,    -1,    -1,    -1,    10,
      11,    12,    13,    14,    15,    16,   107,    18,    19,    20,
      21,    22,    23,    24,    25,    26,    27,    28,    29,    30,
      -1,   341,    19,    20,    21,    22,    23,    24,    25,    26,
      27,    28,    29,    30,    -1,    10,    11,    12,    13,    14,
      15,    16,   341,    18,    19,    20,    21,    22,    23,    24,
      25,    26,    27,    28,    29,    30,    10,    11,    12,    13,
      14,    15,    16,   341,    18,    19,    20,    21,    22,    23,
      24,    25,    26,    27,    28,    29,    30,    10,    11,    12,
      13,    14,    15,    16,   341,    18,    19,    20,    21,    22,
      23,    24,    25,    26,    27,    28,    29,    30,    10,    11,
      12,    13,    14,    15,    16,   341,    18,    19,    20,    21,
      22,    23,    24,    25,    26,    27,    28,    29,    30,    10,
      11,    12,    13,    14,    15,    16,   341,    18,    19,    20,
      21,    22,    23,    24,    25,    26,    27,    28,    29,    30,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   341,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    73,    74,    75,    76,    77,    -1,    10,    11,    12,
      13,    14,    15,    16,   341,    18,    19,    20,    21,    22,
      23,    24,    25,    26,    27,    28,    29,    30,    -1,    -1,
      -1,    10,    11,    12,    13,    14,    15,    16,   341,    18,
      19,    20,    21,    22,    23,    24,    25,    26,    27,    28,
      29,    30,    -1,    43,    44,    45,    46,    -1,    -1,   341,
      -1,    -1,    -1,    53,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   338,    -1,    -1,
      -1,    71,    72,    -1,    -1,    -1,    -1,    -1,    78,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   338,    89,
      -1,    -1,   174,    -1,    -1,   402,   403,    97,   405,    -1,
      -1,   408,   409,   103,   411,   412,   413,    -1,    -1,    -1,
     417,    -1,   419,    -1,   421,    -1,    -1,    -1,    -1,   426,
      -1,    -1,   429,    -1,   124,   125,    -1,    -1,    -1,    -1,
      -1,   338,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   143,   144,    -1,    -1,    -1,   148,   149,
     150,    -1,    -1,    -1,    -1,    -1,    -1,   338,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   247,   248,    -1,   250,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   177,    -1,    -1,
      -1,   242,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   338,    -1,    -1,   196,    -1,   198,    -1,
      -1,    -1,    -1,    -1,   217,    -1,    -1,    -1,    -1,   516,
      -1,    -1,    -1,    -1,   338,   215,    -1,    -1,   218,   219,
     220,   221,   222,    -1,    -1,    -1,    -1,    -1,   217,    -1,
      -1,    -1,    -1,    -1,    -1,   338,    -1,    -1,    -1,   239,
     240,    -1,    -1,    -1,    -1,    -1,    -1,   247,    10,    11,
      12,    13,    14,    15,    16,    -1,    18,    19,    20,    21,
      22,    23,    24,    25,    26,    27,    28,    29,    30,    10,
      11,    12,    13,    14,    15,    16,    -1,    18,    19,    20,
      21,    22,    23,    24,    25,    26,    27,    28,    29,    30,
      10,    11,    12,    13,    14,    15,    16,    -1,    18,    19,
      20,    21,    22,    23,    24,    25,    26,    27,    28,    29,
      30,    10,    11,    12,    13,    14,    15,    16,    -1,    18,
      19,    20,    21,    22,    23,    24,    25,    26,    27,    28,
      29,    30,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    40,    -1,    -1,    -1,   107,    -1,    -1,    10,    11,
      12,    13,    14,    15,    16,    96,    18,    19,    20,    21,
      22,    23,    24,    25,    26,    27,    28,    29,    30,    -1,
       3,    -1,    -1,    -1,    36,    -1,    96,    10,    11,    12,
      13,    14,    15,    16,    83,    18,    19,    20,    21,    22,
      23,    24,    25,    26,    27,    28,    29,    30,    -1,    10,
      11,    12,    13,    14,    15,    16,    39,    18,    19,    20,
      21,    22,    23,    24,    25,    26,    27,    28,    29,    30,
      -1,    10,    11,    12,    13,    14,    15,    16,    39,    18,
      19,    20,    21,    22,    23,    24,    25,    26,    27,    28,
      29,    30,    11,    12,    13,    14,    15,    16,    -1,    18,
      19,    20,    21,    22,    23,    24,    25,    26,    27,    28,
      29,    30,    12,    13,    14,    15,    16,    -1,    18,    19,
      20,    21,    22,    23,    24,    25,    26,    27,    28,    29,
      30
};

/* YYSTOS[STATE-NUM] -- The (internal number of the) accessing
   symbol of state STATE-NUM.  */
static const unsigned short int yystos[] =
{
       0,     1,    38,    62,    90,   123,   138,   193,   343,   344,
     345,   380,   382,   395,   396,   401,   402,   403,   405,   408,
     337,   213,    69,   108,   171,   190,   201,   214,   228,   385,
     397,    69,   118,   190,   385,   410,   213,   357,     0,     1,
     344,   337,     3,   116,   381,   158,   381,   213,   118,   116,
     404,     3,   404,   213,    81,   112,   117,   127,   146,   411,
       3,     3,     4,     5,     6,     7,     8,    17,    18,    24,
      25,    26,    33,    51,    86,    87,    99,   112,   207,   208,
     209,   212,   324,   325,   326,   327,   328,   329,   330,   331,
     333,   334,   335,   336,   340,   358,   359,   373,   337,   337,
     107,    18,     3,     3,     3,   381,     3,    99,     3,   158,
       3,   404,     3,   107,   339,     9,   373,    99,   373,   373,
     373,   338,   242,   373,   379,   340,   340,   340,   340,   340,
     340,   340,   340,   340,   340,   340,   340,   340,   340,   373,
     374,   107,   338,     3,    10,    11,    12,    13,    14,    15,
      16,    18,    19,    20,    21,    22,    23,    24,    25,    26,
      27,    28,    29,    30,    39,   364,   373,   374,    99,   340,
       3,   234,   398,     3,     3,   406,   196,   340,   412,   374,
       3,    26,   373,   340,   358,   373,   379,    93,    96,   242,
     345,   373,    47,   133,   224,   373,   376,   373,   373,    26,
     373,   373,   373,   374,   373,   373,   373,   373,   374,   338,
     341,   341,     3,   340,   360,   361,   362,   365,   372,   358,
     373,   373,   373,   373,   373,     6,    18,   156,   340,    13,
      14,    16,   373,    33,    37,   199,   340,   373,   373,   373,
     373,   373,   373,   373,   373,   373,   373,   373,     3,   245,
       3,   356,   117,   182,   339,   340,   383,   384,   319,   320,
     399,   158,   339,    50,   184,   338,   407,     3,   415,   356,
     238,   345,   245,   345,   217,    93,    96,   373,   373,   341,
     107,   338,   341,   373,   338,   338,   341,   341,   341,    39,
     341,   341,   341,   341,   341,   341,   374,    16,    18,   339,
     364,   345,   360,   243,   346,    63,   120,   135,   154,   187,
     212,   338,   366,   368,    39,   363,     6,   156,   345,   374,
     373,   373,   340,    36,   340,   340,   340,   345,     4,   338,
     341,     3,     3,   108,   118,   130,   169,   386,   387,   363,
       3,     3,     3,    20,   158,   338,   409,   341,   340,   413,
     409,     4,   341,   373,   373,    96,   217,   373,   373,   107,
     126,   378,   378,     4,    53,    89,   103,   125,   377,   340,
      16,     3,   341,   341,   373,   110,   347,   135,   187,   369,
     362,   360,   129,   165,   367,     3,   341,   341,   345,   374,
     373,   345,   345,   345,   341,   338,   356,   157,   167,   340,
     383,    43,    44,    45,    46,    53,    71,    72,    78,    89,
      97,   103,   124,   125,   143,   144,   148,   149,   150,   177,
     196,   198,   215,   218,   219,   220,   221,   222,   239,   240,
     247,   389,   118,   130,   340,   340,   130,   341,   338,   345,
     340,   339,    80,   373,   159,     3,    80,   373,   414,   409,
     338,    96,   373,   104,   341,   338,   341,   373,   373,   341,
     341,   341,   345,   374,   375,   340,   364,    48,   111,   351,
     367,   367,   129,   158,   362,   129,   341,   341,   341,   341,
     341,     4,    20,   158,   386,   340,   391,   391,   391,   391,
     391,   391,   340,   391,   391,   391,    44,   392,   391,   392,
     391,   340,   391,   392,   391,   392,   340,   391,   388,   340,
     340,   356,   356,   340,   383,   386,     3,   400,     3,   130,
      20,   338,   338,   341,     4,   373,   373,   341,    73,    74,
      75,    76,    77,   174,   247,   248,   250,   341,   338,   341,
     345,   374,   348,   373,   373,   163,   352,   362,   373,   158,
     234,   370,   371,   362,   189,     5,     3,   341,     5,   393,
     394,   393,   393,     4,   390,   393,   393,   393,   394,   393,
     394,   393,   390,   393,   394,   393,   394,     5,   394,    18,
      41,    57,    80,   130,   156,   169,   228,   356,   356,   341,
     341,   356,   391,   341,   231,    80,   373,   414,   414,   338,
     189,   341,   341,   375,   341,   341,   245,   350,   338,    48,
     136,   354,   373,   340,   371,    20,   167,   383,   338,   341,
     230,   249,    53,    55,   338,   341,   341,   341,   156,     4,
       4,     5,     6,     7,   130,   130,   340,   341,   341,   341,
      40,    83,   349,   415,   413,    20,   188,   348,   353,   373,
     373,   127,   355,   356,     7,   158,     5,   196,     3,     4,
     394,   394,   356,   338,     7,   349,   338,   356,   341,     3,
     341,     3,   341,   400,   338,   373,   353
};

/* Error token number */
#define YYTERROR 1


/* This macro is provided for backward compatibility. */
#ifndef YY_LOCATION_PRINT
# define YY_LOCATION_PRINT(File, Loc) ((void) 0)
#endif


/* YYLEX -- calling `yylex' with the right arguments.  */
#ifdef YYLEX_PARAM
# define YYLEX yylex (&yylval, YYLEX_PARAM)
#else
# define YYLEX yylex (&yylval)
#endif

#undef yynerrs
#define yynerrs (yystackp->yyerrcnt)
#undef yychar
#define yychar (yystackp->yyrawchar)
#undef yylval
#define yylval (yystackp->yyval)
#undef yylloc
#define yylloc (yystackp->yyloc)


static const int YYEOF = 0;
static const int YYEMPTY = -2;

typedef enum { yyok, yyaccept, yyabort, yyerr } YYRESULTTAG;

#define YYCHK(YYE)                                                           \
   do { YYRESULTTAG yyflag = YYE; if (yyflag != yyok) return yyflag; }       \
   while (YYID (0))

#if YYDEBUG

# ifndef YYFPRINTF
#  define YYFPRINTF fprintf
# endif

# define YYDPRINTF(Args)                        \
do {                                            \
  if (yydebug)                                  \
    YYFPRINTF Args;                             \
} while (YYID (0))


/*--------------------------------.
| Print this symbol on YYOUTPUT.  |
`--------------------------------*/

/*ARGSUSED*/
static void
yy_symbol_value_print (FILE *yyoutput, int yytype, YYSTYPE const * const yyvaluep, struct ParseResult* result)
{
  FILE *yyo = yyoutput;
  YYUSE (yyo);
  if (!yyvaluep)
    return;
  YYUSE (result);
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
yy_symbol_print (FILE *yyoutput, int yytype, YYSTYPE const * const yyvaluep, struct ParseResult* result)
{
  if (yytype < YYNTOKENS)
    YYFPRINTF (yyoutput, "token %s (", yytname[yytype]);
  else
    YYFPRINTF (yyoutput, "nterm %s (", yytname[yytype]);

  yy_symbol_value_print (yyoutput, yytype, yyvaluep, result);
  YYFPRINTF (yyoutput, ")");
}

# define YY_SYMBOL_PRINT(Title, Type, Value, Location)          \
do {                                                            \
  if (yydebug)                                                  \
    {                                                           \
      YYFPRINTF (stderr, "%s ", Title);                         \
      yy_symbol_print (stderr, Type, Value, result);        \
      YYFPRINTF (stderr, "\n");                                 \
    }                                                           \
} while (YYID (0))

/* Nonzero means print parse trace.  It is left uninitialized so that
   multiple parsers can coexist.  */
int yydebug;

#else /* !YYDEBUG */

# define YYDPRINTF(Args)
# define YY_SYMBOL_PRINT(Title, Type, Value, Location)

#endif /* !YYDEBUG */

/* YYINITDEPTH -- initial size of the parser's stacks.  */
#ifndef YYINITDEPTH
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
# define YY_RESERVE_GLRSTACK(Yystack)                   \
  do {                                                  \
    if (Yystack->yyspaceLeft < YYHEADROOM)              \
      yyexpandGLRStack (Yystack);                       \
  } while (YYID (0))
#else
# define YY_RESERVE_GLRSTACK(Yystack)                   \
  do {                                                  \
    if (Yystack->yyspaceLeft < YYHEADROOM)              \
      yyMemoryExhausted (Yystack);                      \
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


  int yyerrcnt;
  int yyrawchar;
  YYSTYPE yyval;

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

static void yyFail (yyGLRStack* yystackp, struct ParseResult* result, const char* yymsg)
  __attribute__ ((__noreturn__));
static void
yyFail (yyGLRStack* yystackp, struct ParseResult* result, const char* yymsg)
{
  if (yymsg != YY_NULL)
    yyerror (result, yymsg);
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
  int i;
  yyGLRState *s = yyvsp[yylow0].yystate.yypred;
  for (i = yylow0-1; i >= yylow1; i -= 1)
    {
      YYASSERT (s->yyresolved);
      yyvsp[i].yystate.yyresolved = yytrue;
      yyvsp[i].yystate.yysemantics.yysval = s->yysemantics.yysval;
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
              yyGLRStack* yystackp,
              YYSTYPE* yyvalp, struct ParseResult* result)
{
  yybool yynormal __attribute__ ((__unused__)) =
    (yystackp->yysplitPoint == YY_NULL);
  int yylow;
  YYUSE (result);
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
# define YYBACKUP(Token, Value)                                              \
  return yyerror (result, YY_("syntax error: cannot back up")),     \
         yyerrok, yyerr

  yylow = 1;
  if (yyrhslen == 0)
    *yyvalp = yyval_default;
  else
    *yyvalp = yyvsp[YYFILL (1-yyrhslen)].yystate.yysemantics.yysval;
  switch (yyn)
    {
        case 2:
/* Line 868 of glr.c  */
#line 432 "sql.ypp"
    { 	
		printf("> \n"); 
		((*yyvalp).ast_node)=new AstStmtList(AST_STMT_LIST,(((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (2))].yystate.yysemantics.yysval.ast_node),NULL);
		if (result->error_number == 0) 
		{
			result->ast  = ((*yyvalp).ast_node);
		}
		else
		{
			result->ast  = NULL;
		} 
  						
  	}
    break;

  case 3:
/* Line 868 of glr.c  */
#line 446 "sql.ypp"
    { 	
		printf(">> \n"); 
		((*yyvalp).ast_node)=new AstStmtList(AST_STMT_LIST,(((yyGLRStackItem const *)yyvsp)[YYFILL ((2) - (3))].yystate.yysemantics.yysval.ast_node),(((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (3))].yystate.yysemantics.yysval.ast_node));
		if (result->error_number == 0)
		{
			result->ast  = ((*yyvalp).ast_node);
		}
		else
		{
			result->ast  = NULL;
		}
	}
    break;

  case 4:
/* Line 868 of glr.c  */
#line 459 "sql.ypp"
    { 
		printf(">> \n");
		result->ast  = NULL;	
		result->error_number++;	
		//yyerror(result,"First statement discarded, input new statement"); 
		yyclearin;
		yyerrok; 				
	}
    break;

  case 5:
/* Line 868 of glr.c  */
#line 468 "sql.ypp"
    { 
		printf(">> \n"); 
		result->ast  = NULL;	
		result->error_number++;		
		yyclearin;
		yyerrok; 
	}
    break;

  case 6:
/* Line 868 of glr.c  */
#line 479 "sql.ypp"
    { ((*yyvalp).ast_node) = (((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (1))].yystate.yysemantics.yysval.ast_node);}
    break;

  case 7:
/* Line 868 of glr.c  */
#line 484 "sql.ypp"
    {
		((*yyvalp).ast_node)=new AstSelectStmt(AST_SELECT_STMT, (((yyGLRStackItem const *)yyvsp)[YYFILL ((2) - (3))].yystate.yysemantics.yysval.intval),(((yyGLRStackItem const *)yyvsp)[YYFILL ((3) - (3))].yystate.yysemantics.yysval.ast_node),NULL,NULL,NULL,NULL,NULL,NULL,NULL);
	}
    break;

  case 8:
/* Line 868 of glr.c  */
#line 489 "sql.ypp"
    {
		((*yyvalp).ast_node)=new AstSelectStmt(AST_SELECT_STMT, (((yyGLRStackItem const *)yyvsp)[YYFILL ((2) - (11))].yystate.yysemantics.yysval.intval),(((yyGLRStackItem const *)yyvsp)[YYFILL ((3) - (11))].yystate.yysemantics.yysval.ast_node),(((yyGLRStackItem const *)yyvsp)[YYFILL ((5) - (11))].yystate.yysemantics.yysval.ast_node),(((yyGLRStackItem const *)yyvsp)[YYFILL ((6) - (11))].yystate.yysemantics.yysval.ast_node),(((yyGLRStackItem const *)yyvsp)[YYFILL ((7) - (11))].yystate.yysemantics.yysval.ast_node),(((yyGLRStackItem const *)yyvsp)[YYFILL ((8) - (11))].yystate.yysemantics.yysval.ast_node),(((yyGLRStackItem const *)yyvsp)[YYFILL ((9) - (11))].yystate.yysemantics.yysval.ast_node),(((yyGLRStackItem const *)yyvsp)[YYFILL ((10) - (11))].yystate.yysemantics.yysval.ast_node),(((yyGLRStackItem const *)yyvsp)[YYFILL ((11) - (11))].yystate.yysemantics.yysval.ast_node));
	}
    break;

  case 9:
/* Line 868 of glr.c  */
#line 494 "sql.ypp"
    { ((*yyvalp).ast_node) = NULL;}
    break;

  case 10:
/* Line 868 of glr.c  */
#line 495 "sql.ypp"
    { ((*yyvalp).ast_node) = new AstWhereClause(AST_WHERE_CLAUSE, (((yyGLRStackItem const *)yyvsp)[YYFILL ((2) - (2))].yystate.yysemantics.yysval.ast_node));}
    break;

  case 11:
/* Line 868 of glr.c  */
#line 498 "sql.ypp"
    { ((*yyvalp).ast_node) = NULL;}
    break;

  case 12:
/* Line 868 of glr.c  */
#line 499 "sql.ypp"
    { ((*yyvalp).ast_node) = new AstGroupByClause(AST_GROUPBY_CLAUSE, (((yyGLRStackItem const *)yyvsp)[YYFILL ((3) - (4))].yystate.yysemantics.yysval.ast_node), (((yyGLRStackItem const *)yyvsp)[YYFILL ((4) - (4))].yystate.yysemantics.yysval.intval));}
    break;

  case 13:
/* Line 868 of glr.c  */
#line 504 "sql.ypp"
    { ((*yyvalp).ast_node)=new AstGroupByList(AST_GROUPBY_LIST, (((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (1))].yystate.yysemantics.yysval.ast_node), NULL); }
    break;

  case 14:
/* Line 868 of glr.c  */
#line 505 "sql.ypp"
    { ((*yyvalp).ast_node)=new AstGroupByList(AST_GROUPBY_LIST, (((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (3))].yystate.yysemantics.yysval.ast_node), (((yyGLRStackItem const *)yyvsp)[YYFILL ((3) - (3))].yystate.yysemantics.yysval.ast_node)); }
    break;

  case 15:
/* Line 868 of glr.c  */
#line 507 "sql.ypp"
    { ((*yyvalp).intval) = 0; }
    break;

  case 16:
/* Line 868 of glr.c  */
#line 508 "sql.ypp"
    { ((*yyvalp).intval) = 0; }
    break;

  case 17:
/* Line 868 of glr.c  */
#line 509 "sql.ypp"
    { ((*yyvalp).intval) = 1; }
    break;

  case 18:
/* Line 868 of glr.c  */
#line 512 "sql.ypp"
    { ((*yyvalp).intval) = 0; }
    break;

  case 19:
/* Line 868 of glr.c  */
#line 513 "sql.ypp"
    { ((*yyvalp).intval) = 1; }
    break;

  case 20:
/* Line 868 of glr.c  */
#line 516 "sql.ypp"
    { ((*yyvalp).ast_node)=NULL; }
    break;

  case 21:
/* Line 868 of glr.c  */
#line 517 "sql.ypp"
    { ((*yyvalp).ast_node)=new AstHavingClause(AST_HAVING_CLAUSE, (((yyGLRStackItem const *)yyvsp)[YYFILL ((2) - (2))].yystate.yysemantics.yysval.ast_node));	}
    break;

  case 22:
/* Line 868 of glr.c  */
#line 520 "sql.ypp"
    { ((*yyvalp).ast_node)=NULL; }
    break;

  case 23:
/* Line 868 of glr.c  */
#line 521 "sql.ypp"
    { ((*yyvalp).ast_node)=new AstOrderByClause(AST_ORDERBY_CLAUSE, (((yyGLRStackItem const *)yyvsp)[YYFILL ((3) - (3))].yystate.yysemantics.yysval.ast_node)); }
    break;

  case 24:
/* Line 868 of glr.c  */
#line 524 "sql.ypp"
    { ((*yyvalp).ast_node)=new AstOrderByList(AST_ORDERBY_LIST, (((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (2))].yystate.yysemantics.yysval.ast_node), (((yyGLRStackItem const *)yyvsp)[YYFILL ((2) - (2))].yystate.yysemantics.yysval.intval), NULL);}
    break;

  case 25:
/* Line 868 of glr.c  */
#line 525 "sql.ypp"
    { ((*yyvalp).ast_node)=new AstOrderByList(AST_ORDERBY_LIST, (((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (4))].yystate.yysemantics.yysval.ast_node), (((yyGLRStackItem const *)yyvsp)[YYFILL ((2) - (4))].yystate.yysemantics.yysval.intval), (((yyGLRStackItem const *)yyvsp)[YYFILL ((4) - (4))].yystate.yysemantics.yysval.ast_node)); }
    break;

  case 26:
/* Line 868 of glr.c  */
#line 527 "sql.ypp"
    { ((*yyvalp).ast_node) = NULL; }
    break;

  case 27:
/* Line 868 of glr.c  */
#line 528 "sql.ypp"
    { ((*yyvalp).ast_node)=new AstLimitClause(AST_LIMIT_CLAUSE, NULL,(((yyGLRStackItem const *)yyvsp)[YYFILL ((2) - (2))].yystate.yysemantics.yysval.ast_node));}
    break;

  case 28:
/* Line 868 of glr.c  */
#line 529 "sql.ypp"
    { ((*yyvalp).ast_node)=new AstLimitClause(AST_LIMIT_CLAUSE, (((yyGLRStackItem const *)yyvsp)[YYFILL ((2) - (4))].yystate.yysemantics.yysval.ast_node),(((yyGLRStackItem const *)yyvsp)[YYFILL ((4) - (4))].yystate.yysemantics.yysval.ast_node));}
    break;

  case 29:
/* Line 868 of glr.c  */
#line 532 "sql.ypp"
    { ((*yyvalp).ast_node)=NULL; }
    break;

  case 30:
/* Line 868 of glr.c  */
#line 533 "sql.ypp"
    { ((*yyvalp).ast_node)=NULL;}
    break;

  case 31:
/* Line 868 of glr.c  */
#line 537 "sql.ypp"
    { ((*yyvalp).ast_node) = new AstColumn(AST_COLUMN, string("NULL"), string((((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (1))].yystate.yysemantics.yysval.strval)));}
    break;

  case 32:
/* Line 868 of glr.c  */
#line 538 "sql.ypp"
    { ((*yyvalp).ast_node) = new AstColumn(AST_COLUMN, string("NULL"), string((((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (3))].yystate.yysemantics.yysval.strval)), (((yyGLRStackItem const *)yyvsp)[YYFILL ((3) - (3))].yystate.yysemantics.yysval.ast_node));}
    break;

  case 33:
/* Line 868 of glr.c  */
#line 541 "sql.ypp"
    { ((*yyvalp).intval) = 0; }
    break;

  case 34:
/* Line 868 of glr.c  */
#line 542 "sql.ypp"
    { if(((*yyvalp).intval) & 1) yyerror(result,"duplicate ALL option"); ((*yyvalp).intval) = (((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (2))].yystate.yysemantics.yysval.intval) | 1; }
    break;

  case 35:
/* Line 868 of glr.c  */
#line 543 "sql.ypp"
    { if(((*yyvalp).intval) & 2) yyerror(result,"duplicate DISTINCT option"); ((*yyvalp).intval) = (((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (2))].yystate.yysemantics.yysval.intval) | 2; }
    break;

  case 36:
/* Line 868 of glr.c  */
#line 544 "sql.ypp"
    { if(((*yyvalp).intval) & 4) yyerror(result,"duplicate DISTINCTROW option"); ((*yyvalp).intval) = (((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (2))].yystate.yysemantics.yysval.intval) | 4; }
    break;

  case 37:
/* Line 868 of glr.c  */
#line 545 "sql.ypp"
    { if(((*yyvalp).intval) & 8) yyerror(result,"duplicate HIGH_PRIORITY option"); ((*yyvalp).intval) = (((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (2))].yystate.yysemantics.yysval.intval) | 8; }
    break;

  case 38:
/* Line 868 of glr.c  */
#line 546 "sql.ypp"
    { if(((*yyvalp).intval) & 16) yyerror(result,"duplicate STRAIGHT_JOIN option"); ((*yyvalp).intval) = (((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (2))].yystate.yysemantics.yysval.intval) | 16; }
    break;

  case 39:
/* Line 868 of glr.c  */
#line 547 "sql.ypp"
    { if(((*yyvalp).intval) & 32) yyerror(result,"duplicate SQL_SMALL_RESULT option"); ((*yyvalp).intval) = (((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (2))].yystate.yysemantics.yysval.intval) | 32; }
    break;

  case 40:
/* Line 868 of glr.c  */
#line 548 "sql.ypp"
    { if(((*yyvalp).intval) & 64) yyerror(result,"duplicate SQL_BIG_RESULT option"); ((*yyvalp).intval) = (((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (2))].yystate.yysemantics.yysval.intval) | 64; }
    break;

  case 41:
/* Line 868 of glr.c  */
#line 549 "sql.ypp"
    { if(((*yyvalp).intval) & 128) yyerror(result,"duplicate SQL_CALC_FOUND_ROWS option"); ((*yyvalp).intval) = (((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (2))].yystate.yysemantics.yysval.intval) | 128; }
    break;

  case 42:
/* Line 868 of glr.c  */
#line 553 "sql.ypp"
    { ((*yyvalp).ast_node) = new AstSelectList(AST_SELECT_LIST, 0,(((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (1))].yystate.yysemantics.yysval.ast_node),NULL);}
    break;

  case 43:
/* Line 868 of glr.c  */
#line 554 "sql.ypp"
    { ((*yyvalp).ast_node) = new AstSelectList(AST_SELECT_LIST, 0,(((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (3))].yystate.yysemantics.yysval.ast_node),(((yyGLRStackItem const *)yyvsp)[YYFILL ((3) - (3))].yystate.yysemantics.yysval.ast_node));}
    break;

  case 44:
/* Line 868 of glr.c  */
#line 555 "sql.ypp"
    { ((*yyvalp).ast_node) = new AstSelectList(AST_SELECT_LIST, 1,NULL,NULL);}
    break;

  case 45:
/* Line 868 of glr.c  */
#line 556 "sql.ypp"
    { ((*yyvalp).ast_node) = new AstSelectList(AST_SELECT_LIST, 1,NULL,(((yyGLRStackItem const *)yyvsp)[YYFILL ((3) - (3))].yystate.yysemantics.yysval.ast_node)); }
    break;

  case 46:
/* Line 868 of glr.c  */
#line 560 "sql.ypp"
    {((*yyvalp).ast_node) = new AstSelectExpr(AST_SELECT_EXPR, string((((yyGLRStackItem const *)yyvsp)[YYFILL ((2) - (2))].yystate.yysemantics.yysval.strval)),(((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (2))].yystate.yysemantics.yysval.ast_node));}
    break;

  case 47:
/* Line 868 of glr.c  */
#line 563 "sql.ypp"
    { ((*yyvalp).ast_node) = new AstFromList(AST_FROM_LIST, (((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (1))].yystate.yysemantics.yysval.ast_node), NULL); }
    break;

  case 48:
/* Line 868 of glr.c  */
#line 564 "sql.ypp"
    { ((*yyvalp).ast_node) = new AstFromList(AST_FROM_LIST, (((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (3))].yystate.yysemantics.yysval.ast_node),(((yyGLRStackItem const *)yyvsp)[YYFILL ((3) - (3))].yystate.yysemantics.yysval.ast_node));}
    break;

  case 49:
/* Line 868 of glr.c  */
#line 568 "sql.ypp"
    { ((*yyvalp).ast_node)=(((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (1))].yystate.yysemantics.yysval.ast_node); }
    break;

  case 50:
/* Line 868 of glr.c  */
#line 569 "sql.ypp"
    { ((*yyvalp).ast_node)=(((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (1))].yystate.yysemantics.yysval.ast_node); }
    break;

  case 51:
/* Line 868 of glr.c  */
#line 574 "sql.ypp"
    { ((*yyvalp).ast_node) = new AstTable(AST_TABLE, string("NULL"),string((((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (2))].yystate.yysemantics.yysval.strval)),string((((yyGLRStackItem const *)yyvsp)[YYFILL ((2) - (2))].yystate.yysemantics.yysval.strval)));}
    break;

  case 52:
/* Line 868 of glr.c  */
#line 576 "sql.ypp"
    { ((*yyvalp).ast_node) = new AstTable(AST_TABLE, string((((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (4))].yystate.yysemantics.yysval.strval)),string((((yyGLRStackItem const *)yyvsp)[YYFILL ((3) - (4))].yystate.yysemantics.yysval.strval)),string((((yyGLRStackItem const *)yyvsp)[YYFILL ((4) - (4))].yystate.yysemantics.yysval.strval))); }
    break;

  case 53:
/* Line 868 of glr.c  */
#line 577 "sql.ypp"
    { ((*yyvalp).ast_node) = new AstSubquery(AST_SUBQUERY, string((((yyGLRStackItem const *)yyvsp)[YYFILL ((3) - (3))].yystate.yysemantics.yysval.strval)),(((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (3))].yystate.yysemantics.yysval.ast_node)); }
    break;

  case 54:
/* Line 868 of glr.c  */
#line 578 "sql.ypp"
    { ((*yyvalp).ast_node) = (((yyGLRStackItem const *)yyvsp)[YYFILL ((2) - (3))].yystate.yysemantics.yysval.ast_node); }
    break;

  case 57:
/* Line 868 of glr.c  */
#line 585 "sql.ypp"
    { ((*yyvalp).strval) = (((yyGLRStackItem const *)yyvsp)[YYFILL ((2) - (2))].yystate.yysemantics.yysval.strval); }
    break;

  case 58:
/* Line 868 of glr.c  */
#line 586 "sql.ypp"
    { ((*yyvalp).strval) = (((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (1))].yystate.yysemantics.yysval.strval); }
    break;

  case 59:
/* Line 868 of glr.c  */
#line 587 "sql.ypp"
    { ((*yyvalp).strval) = "NULL"; }
    break;

  case 60:
/* Line 868 of glr.c  */
#line 601 "sql.ypp"
    { ((*yyvalp).ast_node) = new AstJoin(AST_JOIN, (((yyGLRStackItem const *)yyvsp)[YYFILL ((2) - (5))].yystate.yysemantics.yysval.intval), (((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (5))].yystate.yysemantics.yysval.ast_node), (((yyGLRStackItem const *)yyvsp)[YYFILL ((4) - (5))].yystate.yysemantics.yysval.ast_node), (((yyGLRStackItem const *)yyvsp)[YYFILL ((5) - (5))].yystate.yysemantics.yysval.ast_node));}
    break;

  case 61:
/* Line 868 of glr.c  */
#line 602 "sql.ypp"
    { ((*yyvalp).ast_node) = new AstJoin(AST_JOIN, -1, (((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (3))].yystate.yysemantics.yysval.ast_node), (((yyGLRStackItem const *)yyvsp)[YYFILL ((3) - (3))].yystate.yysemantics.yysval.ast_node), NULL);}
    break;

  case 62:
/* Line 868 of glr.c  */
#line 603 "sql.ypp"
    { ((*yyvalp).ast_node) = new AstJoin(AST_JOIN, -1, (((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (5))].yystate.yysemantics.yysval.ast_node), (((yyGLRStackItem const *)yyvsp)[YYFILL ((3) - (5))].yystate.yysemantics.yysval.ast_node), (((yyGLRStackItem const *)yyvsp)[YYFILL ((5) - (5))].yystate.yysemantics.yysval.ast_node));}
    break;

  case 63:
/* Line 868 of glr.c  */
#line 604 "sql.ypp"
    { ((*yyvalp).ast_node) = new AstJoin(AST_JOIN, (((yyGLRStackItem const *)yyvsp)[YYFILL ((2) - (6))].yystate.yysemantics.yysval.intval) + (((yyGLRStackItem const *)yyvsp)[YYFILL ((3) - (6))].yystate.yysemantics.yysval.intval), (((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (6))].yystate.yysemantics.yysval.ast_node), (((yyGLRStackItem const *)yyvsp)[YYFILL ((5) - (6))].yystate.yysemantics.yysval.ast_node), (((yyGLRStackItem const *)yyvsp)[YYFILL ((6) - (6))].yystate.yysemantics.yysval.ast_node));}
    break;

  case 64:
/* Line 868 of glr.c  */
#line 605 "sql.ypp"
    { ((*yyvalp).ast_node) = new AstJoin(AST_JOIN, 32 + (((yyGLRStackItem const *)yyvsp)[YYFILL ((3) - (5))].yystate.yysemantics.yysval.intval), (((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (5))].yystate.yysemantics.yysval.ast_node), (((yyGLRStackItem const *)yyvsp)[YYFILL ((5) - (5))].yystate.yysemantics.yysval.ast_node), NULL);}
    break;

  case 65:
/* Line 868 of glr.c  */
#line 608 "sql.ypp"
    { ((*yyvalp).intval) = 0; }
    break;

  case 66:
/* Line 868 of glr.c  */
#line 609 "sql.ypp"
    { ((*yyvalp).intval) = 1; }
    break;

  case 67:
/* Line 868 of glr.c  */
#line 610 "sql.ypp"
    { ((*yyvalp).intval) = 2; }
    break;

  case 68:
/* Line 868 of glr.c  */
#line 613 "sql.ypp"
    { ((*yyvalp).intval) = 4; }
    break;

  case 69:
/* Line 868 of glr.c  */
#line 614 "sql.ypp"
    { ((*yyvalp).intval) = 4; }
    break;

  case 70:
/* Line 868 of glr.c  */
#line 618 "sql.ypp"
    { ((*yyvalp).intval) = 8; }
    break;

  case 71:
/* Line 868 of glr.c  */
#line 619 "sql.ypp"
    { ((*yyvalp).intval) = 16; }
    break;

  case 72:
/* Line 868 of glr.c  */
#line 623 "sql.ypp"
    { ((*yyvalp).intval) = 8 + (((yyGLRStackItem const *)yyvsp)[YYFILL ((2) - (2))].yystate.yysemantics.yysval.intval); }
    break;

  case 73:
/* Line 868 of glr.c  */
#line 624 "sql.ypp"
    { ((*yyvalp).intval) = 16 + (((yyGLRStackItem const *)yyvsp)[YYFILL ((2) - (2))].yystate.yysemantics.yysval.intval); }
    break;

  case 74:
/* Line 868 of glr.c  */
#line 625 "sql.ypp"
    { ((*yyvalp).intval) = 0; }
    break;

  case 75:
/* Line 868 of glr.c  */
#line 628 "sql.ypp"
    {((*yyvalp).ast_node) = NULL;}
    break;

  case 76:
/* Line 868 of glr.c  */
#line 629 "sql.ypp"
    {((*yyvalp).ast_node) = (((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (1))].yystate.yysemantics.yysval.ast_node);}
    break;

  case 77:
/* Line 868 of glr.c  */
#line 637 "sql.ypp"
    {((*yyvalp).ast_node) = new AstJoinCondition(AST_JOIN_CONDITION, "ON", (((yyGLRStackItem const *)yyvsp)[YYFILL ((2) - (2))].yystate.yysemantics.yysval.ast_node)); }
    break;

  case 78:
/* Line 868 of glr.c  */
#line 638 "sql.ypp"
    {((*yyvalp).ast_node) = new AstJoinCondition(AST_JOIN_CONDITION, "USING", (((yyGLRStackItem const *)yyvsp)[YYFILL ((3) - (4))].yystate.yysemantics.yysval.ast_node)); }
    break;

  case 79:
/* Line 868 of glr.c  */
#line 643 "sql.ypp"
    { ((*yyvalp).ast_node)=(((yyGLRStackItem const *)yyvsp)[YYFILL ((2) - (3))].yystate.yysemantics.yysval.ast_node); }
    break;

  case 80:
/* Line 868 of glr.c  */
#line 650 "sql.ypp"
    { ((*yyvalp).ast_node) = new AstColumn(AST_COLUMN, string("NULL"),string((((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (1))].yystate.yysemantics.yysval.strval)));}
    break;

  case 81:
/* Line 868 of glr.c  */
#line 651 "sql.ypp"
    { ((*yyvalp).ast_node) = new AstExprConst(AST_EXPR_CONST, "CONST",string((((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (1))].yystate.yysemantics.yysval.strval))); }
    break;

  case 82:
/* Line 868 of glr.c  */
#line 652 "sql.ypp"
    { ((*yyvalp).ast_node) = new AstColumn(AST_COLUMN, string((((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (3))].yystate.yysemantics.yysval.strval)),string((((yyGLRStackItem const *)yyvsp)[YYFILL ((3) - (3))].yystate.yysemantics.yysval.strval))); }
    break;

  case 83:
/* Line 868 of glr.c  */
#line 653 "sql.ypp"
    { ((*yyvalp).ast_node) = new AstColumn(AST_COLUMN, string((((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (3))].yystate.yysemantics.yysval.strval)),string("*"));}
    break;

  case 84:
/* Line 868 of glr.c  */
#line 654 "sql.ypp"
    { ((*yyvalp).ast_node) = new AstExprConst(AST_EXPR_CONST, "CONST_STRING",	string((((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (1))].yystate.yysemantics.yysval.strval))); }
    break;

  case 85:
/* Line 868 of glr.c  */
#line 655 "sql.ypp"
    { ((*yyvalp).ast_node) = new AstExprConst(AST_EXPR_CONST, "CONST_INT",		string((((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (1))].yystate.yysemantics.yysval.strval))); }
    break;

  case 86:
/* Line 868 of glr.c  */
#line 656 "sql.ypp"
    { ((*yyvalp).ast_node) = new AstExprConst(AST_EXPR_CONST, "CONST_DOUBLE",	string((((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (1))].yystate.yysemantics.yysval.strval))); }
    break;

  case 87:
/* Line 868 of glr.c  */
#line 657 "sql.ypp"
    { ((*yyvalp).ast_node) = new AstExprConst(AST_EXPR_CONST, "CONST_BOOL",	string((((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (1))].yystate.yysemantics.yysval.strval))); }
    break;

  case 88:
/* Line 868 of glr.c  */
#line 659 "sql.ypp"
    { ((*yyvalp).ast_node) = new AstExprCalBinary(AST_EXPR_CAL_BINARY, "+",   (((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (3))].yystate.yysemantics.yysval.ast_node), (((yyGLRStackItem const *)yyvsp)[YYFILL ((3) - (3))].yystate.yysemantics.yysval.ast_node)); }
    break;

  case 89:
/* Line 868 of glr.c  */
#line 660 "sql.ypp"
    { ((*yyvalp).ast_node) = new AstExprCalBinary(AST_EXPR_CAL_BINARY, "-",   (((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (3))].yystate.yysemantics.yysval.ast_node), (((yyGLRStackItem const *)yyvsp)[YYFILL ((3) - (3))].yystate.yysemantics.yysval.ast_node)); }
    break;

  case 90:
/* Line 868 of glr.c  */
#line 661 "sql.ypp"
    { ((*yyvalp).ast_node) = new AstExprCalBinary(AST_EXPR_CAL_BINARY, "*",   (((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (3))].yystate.yysemantics.yysval.ast_node), (((yyGLRStackItem const *)yyvsp)[YYFILL ((3) - (3))].yystate.yysemantics.yysval.ast_node)); }
    break;

  case 91:
/* Line 868 of glr.c  */
#line 662 "sql.ypp"
    { ((*yyvalp).ast_node) = new AstExprCalBinary(AST_EXPR_CAL_BINARY, "/",   (((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (3))].yystate.yysemantics.yysval.ast_node), (((yyGLRStackItem const *)yyvsp)[YYFILL ((3) - (3))].yystate.yysemantics.yysval.ast_node)); }
    break;

  case 92:
/* Line 868 of glr.c  */
#line 663 "sql.ypp"
    { ((*yyvalp).ast_node) = new AstExprCalBinary(AST_EXPR_CAL_BINARY, "MOD", (((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (3))].yystate.yysemantics.yysval.ast_node), (((yyGLRStackItem const *)yyvsp)[YYFILL ((3) - (3))].yystate.yysemantics.yysval.ast_node)); }
    break;

  case 93:
/* Line 868 of glr.c  */
#line 664 "sql.ypp"
    { ((*yyvalp).ast_node) = new AstExprCalBinary(AST_EXPR_CAL_BINARY, "%",   (((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (3))].yystate.yysemantics.yysval.ast_node), (((yyGLRStackItem const *)yyvsp)[YYFILL ((3) - (3))].yystate.yysemantics.yysval.ast_node)); }
    break;

  case 94:
/* Line 868 of glr.c  */
#line 666 "sql.ypp"
    { ((*yyvalp).ast_node) = new AstExprUnary(AST_EXPR_UNARY, "-", (((yyGLRStackItem const *)yyvsp)[YYFILL ((2) - (2))].yystate.yysemantics.yysval.ast_node)); }
    break;

  case 95:
/* Line 868 of glr.c  */
#line 667 "sql.ypp"
    { ((*yyvalp).ast_node) = new AstExprUnary(AST_EXPR_UNARY, "+", (((yyGLRStackItem const *)yyvsp)[YYFILL ((2) - (2))].yystate.yysemantics.yysval.ast_node)); }
    break;

  case 96:
/* Line 868 of glr.c  */
#line 669 "sql.ypp"
    { ((*yyvalp).ast_node) = new AstExprCalBinary(AST_EXPR_CAL_BINARY, "AND", (((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (3))].yystate.yysemantics.yysval.ast_node), (((yyGLRStackItem const *)yyvsp)[YYFILL ((3) - (3))].yystate.yysemantics.yysval.ast_node)); }
    break;

  case 97:
/* Line 868 of glr.c  */
#line 670 "sql.ypp"
    { ((*yyvalp).ast_node) = new AstExprCalBinary(AST_EXPR_CAL_BINARY, "OR",  (((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (3))].yystate.yysemantics.yysval.ast_node), (((yyGLRStackItem const *)yyvsp)[YYFILL ((3) - (3))].yystate.yysemantics.yysval.ast_node)); }
    break;

  case 98:
/* Line 868 of glr.c  */
#line 671 "sql.ypp"
    { ((*yyvalp).ast_node) = new AstExprCalBinary(AST_EXPR_CAL_BINARY, "XOR", (((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (3))].yystate.yysemantics.yysval.ast_node), (((yyGLRStackItem const *)yyvsp)[YYFILL ((3) - (3))].yystate.yysemantics.yysval.ast_node)); }
    break;

  case 99:
/* Line 868 of glr.c  */
#line 673 "sql.ypp"
    { ((*yyvalp).ast_node) = new AstExprCmpBinary(AST_EXPR_CMP_BINARY, "", 		(((yyGLRStackItem const *)yyvsp)[YYFILL ((2) - (3))].yystate.yysemantics.yysval.subtok), (((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (3))].yystate.yysemantics.yysval.ast_node), (((yyGLRStackItem const *)yyvsp)[YYFILL ((3) - (3))].yystate.yysemantics.yysval.ast_node)); }
    break;

  case 100:
/* Line 868 of glr.c  */
#line 674 "sql.ypp"
    { ((*yyvalp).ast_node) = new AstExprCmpBinary(AST_EXPR_CMP_BINARY, "SUBQUERY", (((yyGLRStackItem const *)yyvsp)[YYFILL ((2) - (5))].yystate.yysemantics.yysval.subtok), (((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (5))].yystate.yysemantics.yysval.ast_node), (((yyGLRStackItem const *)yyvsp)[YYFILL ((4) - (5))].yystate.yysemantics.yysval.ast_node)); }
    break;

  case 101:
/* Line 868 of glr.c  */
#line 675 "sql.ypp"
    { ((*yyvalp).ast_node) = new AstExprCmpBinary(AST_EXPR_CMP_BINARY, "ANY",		(((yyGLRStackItem const *)yyvsp)[YYFILL ((2) - (6))].yystate.yysemantics.yysval.subtok), (((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (6))].yystate.yysemantics.yysval.ast_node), (((yyGLRStackItem const *)yyvsp)[YYFILL ((5) - (6))].yystate.yysemantics.yysval.ast_node)); }
    break;

  case 102:
/* Line 868 of glr.c  */
#line 676 "sql.ypp"
    { ((*yyvalp).ast_node) = new AstExprCmpBinary(AST_EXPR_CMP_BINARY, "SOME",		(((yyGLRStackItem const *)yyvsp)[YYFILL ((2) - (6))].yystate.yysemantics.yysval.subtok), (((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (6))].yystate.yysemantics.yysval.ast_node), (((yyGLRStackItem const *)yyvsp)[YYFILL ((5) - (6))].yystate.yysemantics.yysval.ast_node)); }
    break;

  case 103:
/* Line 868 of glr.c  */
#line 677 "sql.ypp"
    { ((*yyvalp).ast_node) = new AstExprCmpBinary(AST_EXPR_CMP_BINARY, "ALL",		(((yyGLRStackItem const *)yyvsp)[YYFILL ((2) - (6))].yystate.yysemantics.yysval.subtok), (((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (6))].yystate.yysemantics.yysval.ast_node), (((yyGLRStackItem const *)yyvsp)[YYFILL ((5) - (6))].yystate.yysemantics.yysval.ast_node)); }
    break;

  case 104:
/* Line 868 of glr.c  */
#line 679 "sql.ypp"
    { ((*yyvalp).ast_node) = new AstExprCalBinary(AST_EXPR_CAL_BINARY, "|",	(((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (3))].yystate.yysemantics.yysval.ast_node), (((yyGLRStackItem const *)yyvsp)[YYFILL ((3) - (3))].yystate.yysemantics.yysval.ast_node)); }
    break;

  case 105:
/* Line 868 of glr.c  */
#line 680 "sql.ypp"
    { ((*yyvalp).ast_node) = new AstExprCalBinary(AST_EXPR_CAL_BINARY, "&",	(((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (3))].yystate.yysemantics.yysval.ast_node), (((yyGLRStackItem const *)yyvsp)[YYFILL ((3) - (3))].yystate.yysemantics.yysval.ast_node)); }
    break;

  case 106:
/* Line 868 of glr.c  */
#line 681 "sql.ypp"
    { ((*yyvalp).ast_node) = new AstExprCalBinary(AST_EXPR_CAL_BINARY, "^",	(((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (3))].yystate.yysemantics.yysval.ast_node), (((yyGLRStackItem const *)yyvsp)[YYFILL ((3) - (3))].yystate.yysemantics.yysval.ast_node)); }
    break;

  case 107:
/* Line 868 of glr.c  */
#line 682 "sql.ypp"
    { ((*yyvalp).ast_node) = new AstExprCalBinary(AST_EXPR_CAL_BINARY, SHIFT==1?"LSHIFT":"RSHIFT",(((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (3))].yystate.yysemantics.yysval.ast_node),(((yyGLRStackItem const *)yyvsp)[YYFILL ((3) - (3))].yystate.yysemantics.yysval.ast_node)); }
    break;

  case 108:
/* Line 868 of glr.c  */
#line 684 "sql.ypp"
    { ((*yyvalp).ast_node) = new AstExprUnary(AST_EXPR_UNARY, "!",	(((yyGLRStackItem const *)yyvsp)[YYFILL ((2) - (2))].yystate.yysemantics.yysval.ast_node)); }
    break;

  case 109:
/* Line 868 of glr.c  */
#line 685 "sql.ypp"
    { ((*yyvalp).ast_node) = new AstExprUnary(AST_EXPR_UNARY, "NOT",	(((yyGLRStackItem const *)yyvsp)[YYFILL ((2) - (2))].yystate.yysemantics.yysval.ast_node)); }
    break;

  case 110:
/* Line 868 of glr.c  */
#line 686 "sql.ypp"
    { ((*yyvalp).ast_node) = NULL; }
    break;

  case 111:
/* Line 868 of glr.c  */
#line 687 "sql.ypp"
    { ((*yyvalp).ast_node) = (((yyGLRStackItem const *)yyvsp)[YYFILL ((2) - (3))].yystate.yysemantics.yysval.ast_node); }
    break;

  case 112:
/* Line 868 of glr.c  */
#line 690 "sql.ypp"
    { ((*yyvalp).ast_node) = new AstExprUnary(AST_EXPR_UNARY, "IS_NULL", 		(((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (3))].yystate.yysemantics.yysval.ast_node)); }
    break;

  case 113:
/* Line 868 of glr.c  */
#line 691 "sql.ypp"
    { ((*yyvalp).ast_node) = new AstExprUnary(AST_EXPR_UNARY, "IS_NOT_NULL", 	(((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (4))].yystate.yysemantics.yysval.ast_node)); }
    break;

  case 114:
/* Line 868 of glr.c  */
#line 692 "sql.ypp"
    { ((*yyvalp).ast_node) = new AstExprUnary(AST_EXPR_UNARY, "IS_BOOL", 		(((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (3))].yystate.yysemantics.yysval.ast_node)); }
    break;

  case 115:
/* Line 868 of glr.c  */
#line 693 "sql.ypp"
    { ((*yyvalp).ast_node) = new AstExprUnary(AST_EXPR_UNARY, "IS_NOT_BOOL", 	(((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (4))].yystate.yysemantics.yysval.ast_node)); }
    break;

  case 116:
/* Line 868 of glr.c  */
#line 696 "sql.ypp"
    { ((*yyvalp).ast_node) = new AstExprFunc(AST_EXPR_FUNC, "BETWEEN_AND", (((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (5))].yystate.yysemantics.yysval.ast_node), (((yyGLRStackItem const *)yyvsp)[YYFILL ((3) - (5))].yystate.yysemantics.yysval.ast_node), (((yyGLRStackItem const *)yyvsp)[YYFILL ((5) - (5))].yystate.yysemantics.yysval.ast_node)); }
    break;

  case 117:
/* Line 868 of glr.c  */
#line 699 "sql.ypp"
    { ((*yyvalp).ast_node) = new AstExprList(AST_EXPR_LIST, (((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (1))].yystate.yysemantics.yysval.ast_node), NULL);}
    break;

  case 118:
/* Line 868 of glr.c  */
#line 700 "sql.ypp"
    { ((*yyvalp).ast_node) = new AstExprList(AST_EXPR_LIST, (((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (3))].yystate.yysemantics.yysval.ast_node), (((yyGLRStackItem const *)yyvsp)[YYFILL ((3) - (3))].yystate.yysemantics.yysval.ast_node)); }
    break;

  case 119:
/* Line 868 of glr.c  */
#line 701 "sql.ypp"
    { ((*yyvalp).ast_node) = (((yyGLRStackItem const *)yyvsp)[YYFILL ((2) - (3))].yystate.yysemantics.yysval.ast_node); }
    break;

  case 120:
/* Line 868 of glr.c  */
#line 704 "sql.ypp"
    { ((*yyvalp).ast_node) = new AstExprList(AST_EXPR_LIST, (((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (1))].yystate.yysemantics.yysval.ast_node), NULL); }
    break;

  case 121:
/* Line 868 of glr.c  */
#line 705 "sql.ypp"
    { ((*yyvalp).ast_node) = new AstExprList(AST_EXPR_LIST, (((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (3))].yystate.yysemantics.yysval.ast_node), (((yyGLRStackItem const *)yyvsp)[YYFILL ((3) - (3))].yystate.yysemantics.yysval.ast_node)); }
    break;

  case 122:
/* Line 868 of glr.c  */
#line 708 "sql.ypp"
    { ((*yyvalp).ast_node) = new AstExprCmpBinary(AST_EXPR_CMP_BINARY, "EXPR_IN_LIST", 		(((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (5))].yystate.yysemantics.yysval.ast_node), (((yyGLRStackItem const *)yyvsp)[YYFILL ((4) - (5))].yystate.yysemantics.yysval.ast_node)); }
    break;

  case 123:
/* Line 868 of glr.c  */
#line 709 "sql.ypp"
    { ((*yyvalp).ast_node) = new AstExprCmpBinary(AST_EXPR_CMP_BINARY, "LIST_IN_LIST",			(((yyGLRStackItem const *)yyvsp)[YYFILL ((2) - (7))].yystate.yysemantics.yysval.ast_node), (((yyGLRStackItem const *)yyvsp)[YYFILL ((6) - (7))].yystate.yysemantics.yysval.ast_node)); }
    break;

  case 124:
/* Line 868 of glr.c  */
#line 710 "sql.ypp"
    { ((*yyvalp).ast_node) = new AstExprCmpBinary(AST_EXPR_CMP_BINARY, "EXPR_NOT_IN_LIST", 	(((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (6))].yystate.yysemantics.yysval.ast_node), (((yyGLRStackItem const *)yyvsp)[YYFILL ((5) - (6))].yystate.yysemantics.yysval.ast_node)); }
    break;

  case 125:
/* Line 868 of glr.c  */
#line 711 "sql.ypp"
    { ((*yyvalp).ast_node) = new AstExprCmpBinary(AST_EXPR_CMP_BINARY, "LIST_NOT_IN_LIST", 	(((yyGLRStackItem const *)yyvsp)[YYFILL ((2) - (8))].yystate.yysemantics.yysval.ast_node), (((yyGLRStackItem const *)yyvsp)[YYFILL ((7) - (8))].yystate.yysemantics.yysval.ast_node)); }
    break;

  case 126:
/* Line 868 of glr.c  */
#line 712 "sql.ypp"
    { ((*yyvalp).ast_node) = new AstExprCmpBinary(AST_EXPR_CMP_BINARY, "EXPR_IN_SELECT", 		(((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (5))].yystate.yysemantics.yysval.ast_node), (((yyGLRStackItem const *)yyvsp)[YYFILL ((4) - (5))].yystate.yysemantics.yysval.ast_node)); }
    break;

  case 127:
/* Line 868 of glr.c  */
#line 713 "sql.ypp"
    { ((*yyvalp).ast_node) = new AstExprCmpBinary(AST_EXPR_CMP_BINARY, "LIST_IN_SELECT", 		(((yyGLRStackItem const *)yyvsp)[YYFILL ((2) - (7))].yystate.yysemantics.yysval.ast_node), (((yyGLRStackItem const *)yyvsp)[YYFILL ((6) - (7))].yystate.yysemantics.yysval.ast_node)); }
    break;

  case 128:
/* Line 868 of glr.c  */
#line 714 "sql.ypp"
    { ((*yyvalp).ast_node) = new AstExprCmpBinary(AST_EXPR_CMP_BINARY, "EXPR_NOT_IN_SELECT", 	(((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (6))].yystate.yysemantics.yysval.ast_node), (((yyGLRStackItem const *)yyvsp)[YYFILL ((5) - (6))].yystate.yysemantics.yysval.ast_node)); }
    break;

  case 129:
/* Line 868 of glr.c  */
#line 715 "sql.ypp"
    { ((*yyvalp).ast_node) = new AstExprCmpBinary(AST_EXPR_CMP_BINARY, "LIST_NOT_IN_SELECT", 	(((yyGLRStackItem const *)yyvsp)[YYFILL ((2) - (8))].yystate.yysemantics.yysval.ast_node), (((yyGLRStackItem const *)yyvsp)[YYFILL ((7) - (8))].yystate.yysemantics.yysval.ast_node)); }
    break;

  case 130:
/* Line 868 of glr.c  */
#line 717 "sql.ypp"
    { ((*yyvalp).ast_node) = new AstExprUnary(AST_EXPR_UNARY, "EXSIST", 		(((yyGLRStackItem const *)yyvsp)[YYFILL ((3) - (4))].yystate.yysemantics.yysval.ast_node)); }
    break;

  case 131:
/* Line 868 of glr.c  */
#line 718 "sql.ypp"
    { ((*yyvalp).ast_node) = new AstExprUnary(AST_EXPR_UNARY, "NOT_EXSIST", 	(((yyGLRStackItem const *)yyvsp)[YYFILL ((4) - (5))].yystate.yysemantics.yysval.ast_node)); }
    break;

  case 132:
/* Line 868 of glr.c  */
#line 728 "sql.ypp"
    { ((*yyvalp).ast_node) = new AstExprUnary(AST_EXPR_UNARY, "COUNT_ALL", NULL); }
    break;

  case 133:
/* Line 868 of glr.c  */
#line 729 "sql.ypp"
    { ((*yyvalp).ast_node) = new AstExprUnary(AST_EXPR_UNARY, "COUNT",	 (((yyGLRStackItem const *)yyvsp)[YYFILL ((3) - (4))].yystate.yysemantics.yysval.ast_node)); }
    break;

  case 134:
/* Line 868 of glr.c  */
#line 730 "sql.ypp"
    { ((*yyvalp).ast_node) = new AstExprUnary(AST_EXPR_UNARY, "SUM",	 	 (((yyGLRStackItem const *)yyvsp)[YYFILL ((3) - (4))].yystate.yysemantics.yysval.ast_node)); }
    break;

  case 135:
/* Line 868 of glr.c  */
#line 731 "sql.ypp"
    { ((*yyvalp).ast_node) = new AstExprUnary(AST_EXPR_UNARY, "AVG",	 	 (((yyGLRStackItem const *)yyvsp)[YYFILL ((3) - (4))].yystate.yysemantics.yysval.ast_node)); }
    break;

  case 136:
/* Line 868 of glr.c  */
#line 732 "sql.ypp"
    { ((*yyvalp).ast_node) = new AstExprUnary(AST_EXPR_UNARY, "MIN",	 	 (((yyGLRStackItem const *)yyvsp)[YYFILL ((3) - (4))].yystate.yysemantics.yysval.ast_node)); }
    break;

  case 137:
/* Line 868 of glr.c  */
#line 733 "sql.ypp"
    { ((*yyvalp).ast_node) = new AstExprUnary(AST_EXPR_UNARY, "MAX",	 	 (((yyGLRStackItem const *)yyvsp)[YYFILL ((3) - (4))].yystate.yysemantics.yysval.ast_node)); }
    break;

  case 138:
/* Line 868 of glr.c  */
#line 737 "sql.ypp"
    { ((*yyvalp).ast_node) = new AstExprFunc(AST_EXPR_FUNC, "SUBSTRING_EXPR_EXPR", 				(((yyGLRStackItem const *)yyvsp)[YYFILL ((3) - (6))].yystate.yysemantics.yysval.ast_node), (((yyGLRStackItem const *)yyvsp)[YYFILL ((5) - (6))].yystate.yysemantics.yysval.ast_node), 	NULL); }
    break;

  case 139:
/* Line 868 of glr.c  */
#line 738 "sql.ypp"
    { ((*yyvalp).ast_node) = new AstExprFunc(AST_EXPR_FUNC, "SUBSTRING_EXPR_FROM_EXPR", 			(((yyGLRStackItem const *)yyvsp)[YYFILL ((3) - (6))].yystate.yysemantics.yysval.ast_node), (((yyGLRStackItem const *)yyvsp)[YYFILL ((5) - (6))].yystate.yysemantics.yysval.ast_node), 	NULL);}
    break;

  case 140:
/* Line 868 of glr.c  */
#line 739 "sql.ypp"
    { ((*yyvalp).ast_node) = new AstExprFunc(AST_EXPR_FUNC, "SUBSTRING_EXPR_EXPR_EXPR", 			(((yyGLRStackItem const *)yyvsp)[YYFILL ((3) - (8))].yystate.yysemantics.yysval.ast_node), (((yyGLRStackItem const *)yyvsp)[YYFILL ((5) - (8))].yystate.yysemantics.yysval.ast_node), 	(((yyGLRStackItem const *)yyvsp)[YYFILL ((7) - (8))].yystate.yysemantics.yysval.ast_node)); }
    break;

  case 141:
/* Line 868 of glr.c  */
#line 740 "sql.ypp"
    { ((*yyvalp).ast_node) = new AstExprFunc(AST_EXPR_FUNC, "SUBSTRING_EXPR_FROM_EXPR_FOR_EXPR", (((yyGLRStackItem const *)yyvsp)[YYFILL ((3) - (8))].yystate.yysemantics.yysval.ast_node), (((yyGLRStackItem const *)yyvsp)[YYFILL ((5) - (8))].yystate.yysemantics.yysval.ast_node), 	(((yyGLRStackItem const *)yyvsp)[YYFILL ((7) - (8))].yystate.yysemantics.yysval.ast_node)); }
    break;

  case 142:
/* Line 868 of glr.c  */
#line 741 "sql.ypp"
    { ((*yyvalp).ast_node) = new AstExprFunc(AST_EXPR_FUNC, "TRIM_TRAILING", 					(((yyGLRStackItem const *)yyvsp)[YYFILL ((3) - (4))].yystate.yysemantics.yysval.ast_node), NULL, 	NULL); }
    break;

  case 143:
/* Line 868 of glr.c  */
#line 742 "sql.ypp"
    { ((*yyvalp).ast_node) = new AstExprFunc(AST_EXPR_FUNC, (((yyGLRStackItem const *)yyvsp)[YYFILL ((3) - (7))].yystate.yysemantics.yysval.strval), 									(((yyGLRStackItem const *)yyvsp)[YYFILL ((4) - (7))].yystate.yysemantics.yysval.ast_node), (((yyGLRStackItem const *)yyvsp)[YYFILL ((6) - (7))].yystate.yysemantics.yysval.ast_node), 	NULL); }
    break;

  case 144:
/* Line 868 of glr.c  */
#line 743 "sql.ypp"
    { ((*yyvalp).ast_node) = new AstExprFunc(AST_EXPR_FUNC, "UPPER", 							(((yyGLRStackItem const *)yyvsp)[YYFILL ((3) - (4))].yystate.yysemantics.yysval.ast_node), NULL,	NULL); }
    break;

  case 145:
/* Line 868 of glr.c  */
#line 744 "sql.ypp"
    { ((*yyvalp).ast_node) = new AstExprFunc(AST_EXPR_FUNC, "CAST",								(((yyGLRStackItem const *)yyvsp)[YYFILL ((3) - (6))].yystate.yysemantics.yysval.ast_node), (((yyGLRStackItem const *)yyvsp)[YYFILL ((5) - (6))].yystate.yysemantics.yysval.ast_node),  	NULL); }
    break;

  case 146:
/* Line 868 of glr.c  */
#line 745 "sql.ypp"
    { ((*yyvalp).ast_node) = new AstExprFunc(AST_EXPR_FUNC, "COALESCE", 							(((yyGLRStackItem const *)yyvsp)[YYFILL ((3) - (4))].yystate.yysemantics.yysval.ast_node), NULL, 	NULL); }
    break;

  case 147:
/* Line 868 of glr.c  */
#line 748 "sql.ypp"
    { ((*yyvalp).strval)="TRIM_LEADING"; }
    break;

  case 148:
/* Line 868 of glr.c  */
#line 749 "sql.ypp"
    { ((*yyvalp).strval)="TRIM_TRAILING"; }
    break;

  case 149:
/* Line 868 of glr.c  */
#line 750 "sql.ypp"
    { ((*yyvalp).strval)="TRIM_BOTH"; }
    break;

  case 150:
/* Line 868 of glr.c  */
#line 753 "sql.ypp"
    { ((*yyvalp).ast_node) = NULL;}
    break;

  case 151:
/* Line 868 of glr.c  */
#line 754 "sql.ypp"
    {((*yyvalp).ast_node) = new AstExprConst(AST_EXPR_CONST, "INT", 	NULL);}
    break;

  case 152:
/* Line 868 of glr.c  */
#line 755 "sql.ypp"
    {((*yyvalp).ast_node) = new AstExprConst(AST_EXPR_CONST, "STRING", NULL);}
    break;

  case 153:
/* Line 868 of glr.c  */
#line 756 "sql.ypp"
    {((*yyvalp).ast_node) = new AstExprConst(AST_EXPR_CONST, "DOUBLE", NULL);}
    break;

  case 154:
/* Line 868 of glr.c  */
#line 757 "sql.ypp"
    {((*yyvalp).ast_node) = new AstExprConst(AST_EXPR_CONST, "FLOAT", 	NULL);}
    break;

  case 155:
/* Line 868 of glr.c  */
#line 758 "sql.ypp"
    {((*yyvalp).ast_node) = new AstExprConst(AST_EXPR_CONST, "CHAR", 	NULL);}
    break;

  case 156:
/* Line 868 of glr.c  */
#line 761 "sql.ypp"
    { ((*yyvalp).ast_node) = new AstExprFunc(AST_EXPR_FUNC, "DATE_ADD", (((yyGLRStackItem const *)yyvsp)[YYFILL ((3) - (6))].yystate.yysemantics.yysval.ast_node), (((yyGLRStackItem const *)yyvsp)[YYFILL ((5) - (6))].yystate.yysemantics.yysval.ast_node), NULL); }
    break;

  case 157:
/* Line 868 of glr.c  */
#line 762 "sql.ypp"
    { ((*yyvalp).ast_node) = new AstExprFunc(AST_EXPR_FUNC, "DATE_SUB", (((yyGLRStackItem const *)yyvsp)[YYFILL ((3) - (6))].yystate.yysemantics.yysval.ast_node), (((yyGLRStackItem const *)yyvsp)[YYFILL ((5) - (6))].yystate.yysemantics.yysval.ast_node), NULL); }
    break;

  case 158:
/* Line 868 of glr.c  */
#line 766 "sql.ypp"
    { ((*yyvalp).ast_node) = new AstExprFunc(AST_EXPR_FUNC, "INTERVAL_HOUR", 		(((yyGLRStackItem const *)yyvsp)[YYFILL ((2) - (3))].yystate.yysemantics.yysval.ast_node), NULL, NULL);  }
    break;

  case 159:
/* Line 868 of glr.c  */
#line 767 "sql.ypp"
    { ((*yyvalp).ast_node) = new AstExprFunc(AST_EXPR_FUNC, "INTERVAL_MICROSECOND", 	(((yyGLRStackItem const *)yyvsp)[YYFILL ((2) - (3))].yystate.yysemantics.yysval.ast_node), NULL, NULL);  }
    break;

  case 160:
/* Line 868 of glr.c  */
#line 768 "sql.ypp"
    { ((*yyvalp).ast_node) = new AstExprFunc(AST_EXPR_FUNC, "INTERVAL_MINUTE", 		(((yyGLRStackItem const *)yyvsp)[YYFILL ((2) - (3))].yystate.yysemantics.yysval.ast_node), NULL, NULL);  }
    break;

  case 161:
/* Line 868 of glr.c  */
#line 769 "sql.ypp"
    { ((*yyvalp).ast_node) = new AstExprFunc(AST_EXPR_FUNC, "INTERVAL_SECOND", 		(((yyGLRStackItem const *)yyvsp)[YYFILL ((2) - (3))].yystate.yysemantics.yysval.ast_node), NULL, NULL);  }
    break;

  case 162:
/* Line 868 of glr.c  */
#line 770 "sql.ypp"
    { ((*yyvalp).ast_node) = new AstExprFunc(AST_EXPR_FUNC, "INTERVAL_DAY", 			(((yyGLRStackItem const *)yyvsp)[YYFILL ((2) - (3))].yystate.yysemantics.yysval.ast_node), NULL, NULL);  }
    break;

  case 163:
/* Line 868 of glr.c  */
#line 771 "sql.ypp"
    { ((*yyvalp).ast_node) = new AstExprFunc(AST_EXPR_FUNC, "INTERVAL_MONTH", 		(((yyGLRStackItem const *)yyvsp)[YYFILL ((2) - (3))].yystate.yysemantics.yysval.ast_node), NULL, NULL);  }
    break;

  case 164:
/* Line 868 of glr.c  */
#line 772 "sql.ypp"
    { ((*yyvalp).ast_node) = new AstExprFunc(AST_EXPR_FUNC, "INTERVAL_YEAR", 		(((yyGLRStackItem const *)yyvsp)[YYFILL ((2) - (3))].yystate.yysemantics.yysval.ast_node), NULL, NULL);  }
    break;

  case 165:
/* Line 868 of glr.c  */
#line 773 "sql.ypp"
    { ((*yyvalp).ast_node) = new AstExprFunc(AST_EXPR_FUNC, "INTERVAL_WEEK", 		(((yyGLRStackItem const *)yyvsp)[YYFILL ((2) - (3))].yystate.yysemantics.yysval.ast_node), NULL, NULL);  }
    break;

  case 166:
/* Line 868 of glr.c  */
#line 774 "sql.ypp"
    { ((*yyvalp).ast_node) = new AstExprFunc(AST_EXPR_FUNC, "INTERVAL_QUARTER", 		(((yyGLRStackItem const *)yyvsp)[YYFILL ((2) - (3))].yystate.yysemantics.yysval.ast_node), NULL, NULL);  }
    break;

  case 167:
/* Line 868 of glr.c  */
#line 778 "sql.ypp"
    { ((*yyvalp).ast_node) = new AstExprFunc(AST_EXPR_FUNC, "CASE1", 		(((yyGLRStackItem const *)yyvsp)[YYFILL ((2) - (4))].yystate.yysemantics.yysval.ast_node), 	(((yyGLRStackItem const *)yyvsp)[YYFILL ((3) - (4))].yystate.yysemantics.yysval.ast_node), NULL); }
    break;

  case 168:
/* Line 868 of glr.c  */
#line 779 "sql.ypp"
    { ((*yyvalp).ast_node) = new AstExprFunc(AST_EXPR_FUNC, "CASE1_ELSE", 	(((yyGLRStackItem const *)yyvsp)[YYFILL ((2) - (6))].yystate.yysemantics.yysval.ast_node), 	(((yyGLRStackItem const *)yyvsp)[YYFILL ((3) - (6))].yystate.yysemantics.yysval.ast_node), (((yyGLRStackItem const *)yyvsp)[YYFILL ((5) - (6))].yystate.yysemantics.yysval.ast_node)); }
    break;

  case 169:
/* Line 868 of glr.c  */
#line 780 "sql.ypp"
    { ((*yyvalp).ast_node) = new AstExprFunc(AST_EXPR_FUNC, "CASE2", 		NULL,	(((yyGLRStackItem const *)yyvsp)[YYFILL ((2) - (3))].yystate.yysemantics.yysval.ast_node), NULL); }
    break;

  case 170:
/* Line 868 of glr.c  */
#line 781 "sql.ypp"
    { ((*yyvalp).ast_node) = new AstExprFunc(AST_EXPR_FUNC, "CASE2_ELSE", 	NULL, 	(((yyGLRStackItem const *)yyvsp)[YYFILL ((2) - (5))].yystate.yysemantics.yysval.ast_node), (((yyGLRStackItem const *)yyvsp)[YYFILL ((4) - (5))].yystate.yysemantics.yysval.ast_node)); }
    break;

  case 171:
/* Line 868 of glr.c  */
#line 784 "sql.ypp"
    { ((*yyvalp).ast_node) = new AstExprFunc(AST_EXPR_FUNC, "WHEN", (((yyGLRStackItem const *)yyvsp)[YYFILL ((2) - (4))].yystate.yysemantics.yysval.ast_node), (((yyGLRStackItem const *)yyvsp)[YYFILL ((4) - (4))].yystate.yysemantics.yysval.ast_node), NULL); }
    break;

  case 172:
/* Line 868 of glr.c  */
#line 785 "sql.ypp"
    { ((*yyvalp).ast_node) = new AstExprFunc(AST_EXPR_FUNC, "WHEN", (((yyGLRStackItem const *)yyvsp)[YYFILL ((3) - (5))].yystate.yysemantics.yysval.ast_node), (((yyGLRStackItem const *)yyvsp)[YYFILL ((5) - (5))].yystate.yysemantics.yysval.ast_node), (((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (5))].yystate.yysemantics.yysval.ast_node)); }
    break;

  case 173:
/* Line 868 of glr.c  */
#line 788 "sql.ypp"
    { ((*yyvalp).ast_node) = new AstExprCalBinary(AST_EXPR_CAL_BINARY, "LIKE", 	(((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (3))].yystate.yysemantics.yysval.ast_node), (((yyGLRStackItem const *)yyvsp)[YYFILL ((3) - (3))].yystate.yysemantics.yysval.ast_node)); }
    break;

  case 174:
/* Line 868 of glr.c  */
#line 789 "sql.ypp"
    { ((*yyvalp).ast_node) = new AstExprCalBinary(AST_EXPR_CAL_BINARY, "NOT_LIKE", (((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (4))].yystate.yysemantics.yysval.ast_node), (((yyGLRStackItem const *)yyvsp)[YYFILL ((4) - (4))].yystate.yysemantics.yysval.ast_node)); }
    break;

  case 175:
/* Line 868 of glr.c  */
#line 792 "sql.ypp"
    { ((*yyvalp).ast_node) = new AstExprCalBinary(AST_EXPR_CAL_BINARY, "REGEXP", 		(((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (3))].yystate.yysemantics.yysval.ast_node), (((yyGLRStackItem const *)yyvsp)[YYFILL ((3) - (3))].yystate.yysemantics.yysval.ast_node)); }
    break;

  case 176:
/* Line 868 of glr.c  */
#line 793 "sql.ypp"
    { ((*yyvalp).ast_node) = new AstExprCalBinary(AST_EXPR_CAL_BINARY, "NOT_REGEXP", 	(((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (4))].yystate.yysemantics.yysval.ast_node), (((yyGLRStackItem const *)yyvsp)[YYFILL ((4) - (4))].yystate.yysemantics.yysval.ast_node)); }
    break;

  case 177:
/* Line 868 of glr.c  */
#line 799 "sql.ypp"
    { ((*yyvalp).ast_node) = (((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (1))].yystate.yysemantics.yysval.ast_node); }
    break;

  case 178:
/* Line 868 of glr.c  */
#line 803 "sql.ypp"
    { string temp = ((((yyGLRStackItem const *)yyvsp)[YYFILL ((4) - (4))].yystate.yysemantics.yysval.strval) == NULL)?"":string((((yyGLRStackItem const *)yyvsp)[YYFILL ((4) - (4))].yystate.yysemantics.yysval.strval)); ((*yyvalp).ast_node) = new AstCreateDatabase(AST_CREATE_DATABASE, 1, (((yyGLRStackItem const *)yyvsp)[YYFILL ((3) - (4))].yystate.yysemantics.yysval.intval), temp); }
    break;

  case 179:
/* Line 868 of glr.c  */
#line 804 "sql.ypp"
    { string temp = ((((yyGLRStackItem const *)yyvsp)[YYFILL ((4) - (4))].yystate.yysemantics.yysval.strval) == NULL)?"":string((((yyGLRStackItem const *)yyvsp)[YYFILL ((4) - (4))].yystate.yysemantics.yysval.strval)); ((*yyvalp).ast_node) = new AstCreateDatabase(AST_CREATE_SCHEMA, 2, (((yyGLRStackItem const *)yyvsp)[YYFILL ((3) - (4))].yystate.yysemantics.yysval.intval), temp); }
    break;

  case 180:
/* Line 868 of glr.c  */
#line 807 "sql.ypp"
    { ((*yyvalp).intval) = 0; }
    break;

  case 181:
/* Line 868 of glr.c  */
#line 808 "sql.ypp"
    { ((*yyvalp).intval) = 1; }
    break;

  case 182:
/* Line 868 of glr.c  */
#line 813 "sql.ypp"
    { ((*yyvalp).ast_node) = (((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (1))].yystate.yysemantics.yysval.ast_node);}
    break;

  case 183:
/* Line 868 of glr.c  */
#line 817 "sql.ypp"
    { ((*yyvalp).ast_node) = new AstCreateTable(AST_CREATE_TABLE_LIST, (((yyGLRStackItem const *)yyvsp)[YYFILL ((2) - (8))].yystate.yysemantics.yysval.intval), (((yyGLRStackItem const *)yyvsp)[YYFILL ((4) - (8))].yystate.yysemantics.yysval.intval), string((((yyGLRStackItem const *)yyvsp)[YYFILL ((5) - (8))].yystate.yysemantics.yysval.strval)), "", (((yyGLRStackItem const *)yyvsp)[YYFILL ((7) - (8))].yystate.yysemantics.yysval.ast_node), NULL); }
    break;

  case 184:
/* Line 868 of glr.c  */
#line 821 "sql.ypp"
    { ((*yyvalp).ast_node) = new AstCreateTable(AST_CREATE_TABLE_LIST, (((yyGLRStackItem const *)yyvsp)[YYFILL ((2) - (10))].yystate.yysemantics.yysval.intval), (((yyGLRStackItem const *)yyvsp)[YYFILL ((4) - (10))].yystate.yysemantics.yysval.intval), string((((yyGLRStackItem const *)yyvsp)[YYFILL ((5) - (10))].yystate.yysemantics.yysval.strval)), string((((yyGLRStackItem const *)yyvsp)[YYFILL ((7) - (10))].yystate.yysemantics.yysval.strval)), (((yyGLRStackItem const *)yyvsp)[YYFILL ((9) - (10))].yystate.yysemantics.yysval.ast_node), NULL); }
    break;

  case 185:
/* Line 868 of glr.c  */
#line 826 "sql.ypp"
    { ((*yyvalp).ast_node) = new AstCreateTable(AST_CREATE_TABLE_LIST_SEL, (((yyGLRStackItem const *)yyvsp)[YYFILL ((2) - (9))].yystate.yysemantics.yysval.intval), (((yyGLRStackItem const *)yyvsp)[YYFILL ((4) - (9))].yystate.yysemantics.yysval.intval), string((((yyGLRStackItem const *)yyvsp)[YYFILL ((5) - (9))].yystate.yysemantics.yysval.strval)), "", (((yyGLRStackItem const *)yyvsp)[YYFILL ((7) - (9))].yystate.yysemantics.yysval.ast_node), (((yyGLRStackItem const *)yyvsp)[YYFILL ((9) - (9))].yystate.yysemantics.yysval.ast_node)); }
    break;

  case 186:
/* Line 868 of glr.c  */
#line 830 "sql.ypp"
    { ((*yyvalp).ast_node) = new AstCreateTable(AST_CREATE_TABLE_SEL, (((yyGLRStackItem const *)yyvsp)[YYFILL ((2) - (6))].yystate.yysemantics.yysval.intval), (((yyGLRStackItem const *)yyvsp)[YYFILL ((4) - (6))].yystate.yysemantics.yysval.intval), string((((yyGLRStackItem const *)yyvsp)[YYFILL ((5) - (6))].yystate.yysemantics.yysval.strval)), "", NULL, (((yyGLRStackItem const *)yyvsp)[YYFILL ((6) - (6))].yystate.yysemantics.yysval.ast_node)); }
    break;

  case 187:
/* Line 868 of glr.c  */
#line 835 "sql.ypp"
    { ((*yyvalp).ast_node) = new AstCreateTable(AST_CREATE_TABLE_LIST_SEL, (((yyGLRStackItem const *)yyvsp)[YYFILL ((2) - (11))].yystate.yysemantics.yysval.intval), (((yyGLRStackItem const *)yyvsp)[YYFILL ((4) - (11))].yystate.yysemantics.yysval.intval), string((((yyGLRStackItem const *)yyvsp)[YYFILL ((5) - (11))].yystate.yysemantics.yysval.strval)), string((((yyGLRStackItem const *)yyvsp)[YYFILL ((7) - (11))].yystate.yysemantics.yysval.strval)), (((yyGLRStackItem const *)yyvsp)[YYFILL ((9) - (11))].yystate.yysemantics.yysval.ast_node), (((yyGLRStackItem const *)yyvsp)[YYFILL ((11) - (11))].yystate.yysemantics.yysval.ast_node)); }
    break;

  case 188:
/* Line 868 of glr.c  */
#line 839 "sql.ypp"
    { ((*yyvalp).ast_node) = new AstCreateTable(AST_CREATE_TABLE_SEL, (((yyGLRStackItem const *)yyvsp)[YYFILL ((2) - (8))].yystate.yysemantics.yysval.intval), (((yyGLRStackItem const *)yyvsp)[YYFILL ((4) - (8))].yystate.yysemantics.yysval.intval), string((((yyGLRStackItem const *)yyvsp)[YYFILL ((5) - (8))].yystate.yysemantics.yysval.strval)), string((((yyGLRStackItem const *)yyvsp)[YYFILL ((7) - (8))].yystate.yysemantics.yysval.strval)), NULL, (((yyGLRStackItem const *)yyvsp)[YYFILL ((8) - (8))].yystate.yysemantics.yysval.ast_node)); }
    break;

  case 189:
/* Line 868 of glr.c  */
#line 843 "sql.ypp"
    { ((*yyvalp).ast_node) = new AstCreateSelect(AST_CREATE_SEL, (((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (3))].yystate.yysemantics.yysval.intval), 0, (((yyGLRStackItem const *)yyvsp)[YYFILL ((3) - (3))].yystate.yysemantics.yysval.ast_node)); }
    break;

  case 190:
/* Line 868 of glr.c  */
#line 846 "sql.ypp"
    { ((*yyvalp).intval) = 0; }
    break;

  case 191:
/* Line 868 of glr.c  */
#line 847 "sql.ypp"
    { ((*yyvalp).intval) = 1; }
    break;

  case 192:
/* Line 868 of glr.c  */
#line 848 "sql.ypp"
    { ((*yyvalp).intval) = 2; }
    break;

  case 193:
/* Line 868 of glr.c  */
#line 851 "sql.ypp"
    { ((*yyvalp).intval) = 0; }
    break;

  case 194:
/* Line 868 of glr.c  */
#line 852 "sql.ypp"
    { ((*yyvalp).intval) = 1; }
    break;

  case 195:
/* Line 868 of glr.c  */
#line 855 "sql.ypp"
    { ((*yyvalp).ast_node) = new AstCreateColList(AST_CREATE_COL_LIST, (((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (1))].yystate.yysemantics.yysval.ast_node), NULL); }
    break;

  case 196:
/* Line 868 of glr.c  */
#line 856 "sql.ypp"
    { ((*yyvalp).ast_node) = new AstCreateColList(AST_CREATE_COL_LIST, (((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (3))].yystate.yysemantics.yysval.ast_node), (((yyGLRStackItem const *)yyvsp)[YYFILL ((3) - (3))].yystate.yysemantics.yysval.ast_node)); }
    break;

  case 197:
/* Line 868 of glr.c  */
#line 861 "sql.ypp"
    { ((*yyvalp).ast_node) = new AstCreateDef( AST_CREATE_DEF_NAME, 1, string((((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (3))].yystate.yysemantics.yysval.strval)), (((yyGLRStackItem const *)yyvsp)[YYFILL ((2) - (3))].yystate.yysemantics.yysval.ast_node), (((yyGLRStackItem const *)yyvsp)[YYFILL ((3) - (3))].yystate.yysemantics.yysval.ast_node), NULL); }
    break;

  case 198:
/* Line 868 of glr.c  */
#line 862 "sql.ypp"
    { ((*yyvalp).ast_node) = new AstCreateDef( AST_CREATE_DEF_PR_KEY, 2, "", NULL, NULL, (((yyGLRStackItem const *)yyvsp)[YYFILL ((4) - (5))].yystate.yysemantics.yysval.ast_node));  }
    break;

  case 199:
/* Line 868 of glr.c  */
#line 863 "sql.ypp"
    { ((*yyvalp).ast_node) = new AstCreateDef( AST_CREATE_DEF_KEY, 3, "", NULL, NULL, (((yyGLRStackItem const *)yyvsp)[YYFILL ((3) - (4))].yystate.yysemantics.yysval.ast_node)); }
    break;

  case 200:
/* Line 868 of glr.c  */
#line 864 "sql.ypp"
    { ((*yyvalp).ast_node) = new AstCreateDef( AST_CREATE_DEF_INDEX, 4, "", NULL, NULL, (((yyGLRStackItem const *)yyvsp)[YYFILL ((3) - (4))].yystate.yysemantics.yysval.ast_node)); }
    break;

  case 201:
/* Line 868 of glr.c  */
#line 865 "sql.ypp"
    { ((*yyvalp).ast_node) = new AstCreateDef( AST_CREATE_DEF_FTEXT_INDEX, 5, "", NULL, NULL, (((yyGLRStackItem const *)yyvsp)[YYFILL ((4) - (5))].yystate.yysemantics.yysval.ast_node)); }
    break;

  case 202:
/* Line 868 of glr.c  */
#line 866 "sql.ypp"
    { ((*yyvalp).ast_node) = new AstCreateDef( AST_CREATE_DEF_FTEXT_KEY, 6, "", NULL, NULL, (((yyGLRStackItem const *)yyvsp)[YYFILL ((4) - (5))].yystate.yysemantics.yysval.ast_node)); }
    break;

  case 203:
/* Line 868 of glr.c  */
#line 868 "sql.ypp"
    { ((*yyvalp).ast_node) = new AstColumnAtts(AST_COLUMN_ATTS, 0, 0, 0, "", NULL); }
    break;

  case 204:
/* Line 868 of glr.c  */
#line 869 "sql.ypp"
    { AstColumnAtts* temp = static_cast<AstColumnAtts*>((((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (3))].yystate.yysemantics.yysval.ast_node));temp->datatype_ |= 01; ((*yyvalp).ast_node) = temp; }
    break;

  case 205:
/* Line 868 of glr.c  */
#line 870 "sql.ypp"
    { AstColumnAtts* temp = static_cast<AstColumnAtts*>((((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (2))].yystate.yysemantics.yysval.ast_node));temp->datatype_ |= 02; ((*yyvalp).ast_node) = temp; }
    break;

  case 206:
/* Line 868 of glr.c  */
#line 871 "sql.ypp"
    { AstColumnAtts* temp = static_cast<AstColumnAtts*>((((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (3))].yystate.yysemantics.yysval.ast_node));temp->datatype_ |= 04; temp->default_string_ = ((((yyGLRStackItem const *)yyvsp)[YYFILL ((3) - (3))].yystate.yysemantics.yysval.strval)==NULL)?"":string((((yyGLRStackItem const *)yyvsp)[YYFILL ((3) - (3))].yystate.yysemantics.yysval.strval)); ((*yyvalp).ast_node) = temp; }
    break;

  case 207:
/* Line 868 of glr.c  */
#line 872 "sql.ypp"
    { AstColumnAtts* temp = static_cast<AstColumnAtts*>((((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (3))].yystate.yysemantics.yysval.ast_node));temp->datatype_ |= 010;temp->int_num_ = atoi((((yyGLRStackItem const *)yyvsp)[YYFILL ((3) - (3))].yystate.yysemantics.yysval.strval)); ((*yyvalp).ast_node) = temp; }
    break;

  case 208:
/* Line 868 of glr.c  */
#line 873 "sql.ypp"
    { AstColumnAtts* temp = static_cast<AstColumnAtts*>((((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (3))].yystate.yysemantics.yysval.ast_node));temp->datatype_ |= 020; temp->double_num_ = atof((((yyGLRStackItem const *)yyvsp)[YYFILL ((3) - (3))].yystate.yysemantics.yysval.strval)); ((*yyvalp).ast_node) = temp; }
    break;

  case 209:
/* Line 868 of glr.c  */
#line 874 "sql.ypp"
    { AstColumnAtts* temp = static_cast<AstColumnAtts*>((((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (3))].yystate.yysemantics.yysval.ast_node));temp->datatype_ |= 040; temp->int_num_ = atoi((((yyGLRStackItem const *)yyvsp)[YYFILL ((3) - (3))].yystate.yysemantics.yysval.strval)); ((*yyvalp).ast_node) = temp; }
    break;

  case 210:
/* Line 868 of glr.c  */
#line 875 "sql.ypp"
    { AstColumnAtts* temp = static_cast<AstColumnAtts*>((((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (2))].yystate.yysemantics.yysval.ast_node));temp->datatype_ |= 0100; ((*yyvalp).ast_node) = temp; }
    break;

  case 211:
/* Line 868 of glr.c  */
#line 876 "sql.ypp"
    { AstColumnAtts* temp = static_cast<AstColumnAtts*>((((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (3))].yystate.yysemantics.yysval.ast_node));temp->datatype_ |= 0200; ((*yyvalp).ast_node) = temp; }
    break;

  case 212:
/* Line 868 of glr.c  */
#line 877 "sql.ypp"
    { AstColumnAtts* temp = static_cast<AstColumnAtts*>((((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (3))].yystate.yysemantics.yysval.ast_node));temp->datatype_ |= 0400; ((*yyvalp).ast_node) = temp; }
    break;

  case 213:
/* Line 868 of glr.c  */
#line 878 "sql.ypp"
    { AstColumnAtts* temp = static_cast<AstColumnAtts*>((((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (2))].yystate.yysemantics.yysval.ast_node));temp->datatype_ |= 01000; ((*yyvalp).ast_node) = temp; }
    break;

  case 214:
/* Line 868 of glr.c  */
#line 879 "sql.ypp"
    { AstColumnAtts* temp = static_cast<AstColumnAtts*>((((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (3))].yystate.yysemantics.yysval.ast_node));temp->datatype_ |= 02000; ((*yyvalp).ast_node) = temp; }
    break;

  case 215:
/* Line 868 of glr.c  */
#line 880 "sql.ypp"
    { AstColumnAtts* temp = static_cast<AstColumnAtts*>((((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (5))].yystate.yysemantics.yysval.ast_node));temp->datatype_ |= 04000; temp->col_list_ = (((yyGLRStackItem const *)yyvsp)[YYFILL ((4) - (5))].yystate.yysemantics.yysval.ast_node); ((*yyvalp).ast_node) = temp;}
    break;

  case 216:
/* Line 868 of glr.c  */
#line 884 "sql.ypp"
    { ((*yyvalp).ast_node) = new AstDataType (AST_DATA_TYPE, 1, (((yyGLRStackItem const *)yyvsp)[YYFILL ((2) - (2))].yystate.yysemantics.yysval.ast_node), 0, NULL, 0, NULL); }
    break;

  case 217:
/* Line 868 of glr.c  */
#line 885 "sql.ypp"
    { ((*yyvalp).ast_node) = new AstDataType (AST_DATA_TYPE, 2, (((yyGLRStackItem const *)yyvsp)[YYFILL ((2) - (3))].yystate.yysemantics.yysval.ast_node), (((yyGLRStackItem const *)yyvsp)[YYFILL ((3) - (3))].yystate.yysemantics.yysval.intval), NULL, 0, NULL); }
    break;

  case 218:
/* Line 868 of glr.c  */
#line 886 "sql.ypp"
    { ((*yyvalp).ast_node) = new AstDataType (AST_DATA_TYPE, 3, (((yyGLRStackItem const *)yyvsp)[YYFILL ((2) - (3))].yystate.yysemantics.yysval.ast_node), (((yyGLRStackItem const *)yyvsp)[YYFILL ((3) - (3))].yystate.yysemantics.yysval.intval), NULL, 0, NULL); }
    break;

  case 219:
/* Line 868 of glr.c  */
#line 887 "sql.ypp"
    { ((*yyvalp).ast_node) = new AstDataType (AST_DATA_TYPE, 4, (((yyGLRStackItem const *)yyvsp)[YYFILL ((2) - (3))].yystate.yysemantics.yysval.ast_node), (((yyGLRStackItem const *)yyvsp)[YYFILL ((3) - (3))].yystate.yysemantics.yysval.intval), NULL, 0, NULL); }
    break;

  case 220:
/* Line 868 of glr.c  */
#line 888 "sql.ypp"
    { ((*yyvalp).ast_node) = new AstDataType (AST_DATA_TYPE, 5, (((yyGLRStackItem const *)yyvsp)[YYFILL ((2) - (3))].yystate.yysemantics.yysval.ast_node), (((yyGLRStackItem const *)yyvsp)[YYFILL ((3) - (3))].yystate.yysemantics.yysval.intval), NULL, 0, NULL); }
    break;

  case 221:
/* Line 868 of glr.c  */
#line 889 "sql.ypp"
    { ((*yyvalp).ast_node) = new AstDataType (AST_DATA_TYPE, 6, (((yyGLRStackItem const *)yyvsp)[YYFILL ((2) - (3))].yystate.yysemantics.yysval.ast_node), (((yyGLRStackItem const *)yyvsp)[YYFILL ((3) - (3))].yystate.yysemantics.yysval.intval), NULL, 0, NULL); }
    break;

  case 222:
/* Line 868 of glr.c  */
#line 890 "sql.ypp"
    { ((*yyvalp).ast_node) = new AstDataType (AST_DATA_TYPE, 7, (((yyGLRStackItem const *)yyvsp)[YYFILL ((2) - (3))].yystate.yysemantics.yysval.ast_node), (((yyGLRStackItem const *)yyvsp)[YYFILL ((3) - (3))].yystate.yysemantics.yysval.intval), NULL, 0, NULL); }
    break;

  case 223:
/* Line 868 of glr.c  */
#line 891 "sql.ypp"
    { ((*yyvalp).ast_node) = new AstDataType (AST_DATA_TYPE, 8, (((yyGLRStackItem const *)yyvsp)[YYFILL ((2) - (3))].yystate.yysemantics.yysval.ast_node), (((yyGLRStackItem const *)yyvsp)[YYFILL ((3) - (3))].yystate.yysemantics.yysval.intval), NULL, 0, NULL); }
    break;

  case 224:
/* Line 868 of glr.c  */
#line 892 "sql.ypp"
    { ((*yyvalp).ast_node) = new AstDataType (AST_DATA_TYPE, 9, (((yyGLRStackItem const *)yyvsp)[YYFILL ((2) - (3))].yystate.yysemantics.yysval.ast_node), (((yyGLRStackItem const *)yyvsp)[YYFILL ((3) - (3))].yystate.yysemantics.yysval.intval), NULL, 0, NULL); }
    break;

  case 225:
/* Line 868 of glr.c  */
#line 893 "sql.ypp"
    { ((*yyvalp).ast_node) = new AstDataType (AST_DATA_TYPE, 10, (((yyGLRStackItem const *)yyvsp)[YYFILL ((2) - (3))].yystate.yysemantics.yysval.ast_node), (((yyGLRStackItem const *)yyvsp)[YYFILL ((3) - (3))].yystate.yysemantics.yysval.intval), NULL, 0, NULL); }
    break;

  case 226:
/* Line 868 of glr.c  */
#line 894 "sql.ypp"
    { ((*yyvalp).ast_node) = new AstDataType (AST_DATA_TYPE, 11, (((yyGLRStackItem const *)yyvsp)[YYFILL ((2) - (3))].yystate.yysemantics.yysval.ast_node), (((yyGLRStackItem const *)yyvsp)[YYFILL ((3) - (3))].yystate.yysemantics.yysval.intval), NULL, 0, NULL); }
    break;

  case 227:
/* Line 868 of glr.c  */
#line 895 "sql.ypp"
    { ((*yyvalp).ast_node) = new AstDataType (AST_DATA_TYPE, 12, NULL, 0, NULL, 0, NULL); }
    break;

  case 228:
/* Line 868 of glr.c  */
#line 896 "sql.ypp"
    { ((*yyvalp).ast_node) = new AstDataType (AST_DATA_TYPE, 13, NULL, 0, NULL, 0, NULL); }
    break;

  case 229:
/* Line 868 of glr.c  */
#line 897 "sql.ypp"
    { ((*yyvalp).ast_node) = new AstDataType (AST_DATA_TYPE, 14, NULL, 0, NULL, 0, NULL); }
    break;

  case 230:
/* Line 868 of glr.c  */
#line 898 "sql.ypp"
    { ((*yyvalp).ast_node) = new AstDataType (AST_DATA_TYPE, 15, NULL, 0, NULL, 0, NULL); }
    break;

  case 231:
/* Line 868 of glr.c  */
#line 899 "sql.ypp"
    { ((*yyvalp).ast_node) = new AstDataType (AST_DATA_TYPE, 16, NULL, 0, NULL, 0, NULL); }
    break;

  case 232:
/* Line 868 of glr.c  */
#line 900 "sql.ypp"
    { ((*yyvalp).ast_node) = new AstDataType (AST_DATA_TYPE, 17, (((yyGLRStackItem const *)yyvsp)[YYFILL ((2) - (3))].yystate.yysemantics.yysval.ast_node), 0, (((yyGLRStackItem const *)yyvsp)[YYFILL ((3) - (3))].yystate.yysemantics.yysval.ast_node), 0, NULL); }
    break;

  case 233:
/* Line 868 of glr.c  */
#line 901 "sql.ypp"
    { ((*yyvalp).ast_node) = new AstDataType (AST_DATA_TYPE, 18, (((yyGLRStackItem const *)yyvsp)[YYFILL ((2) - (3))].yystate.yysemantics.yysval.ast_node), 0, (((yyGLRStackItem const *)yyvsp)[YYFILL ((3) - (3))].yystate.yysemantics.yysval.ast_node), 0, NULL); }
    break;

  case 234:
/* Line 868 of glr.c  */
#line 902 "sql.ypp"
    { ((*yyvalp).ast_node) = new AstDataType (AST_DATA_TYPE, 19, (((yyGLRStackItem const *)yyvsp)[YYFILL ((2) - (2))].yystate.yysemantics.yysval.ast_node), 0, NULL, 0, NULL); }
    break;

  case 235:
/* Line 868 of glr.c  */
#line 903 "sql.ypp"
    { AstNode* temp = new AstOptLength(AST_OPT_LENGTH,atoi((((yyGLRStackItem const *)yyvsp)[YYFILL ((3) - (4))].yystate.yysemantics.yysval.strval)),NULL); 
                                              ((*yyvalp).ast_node) = new AstDataType (AST_DATA_TYPE, 20, temp, 0, NULL, 0, NULL); }
    break;

  case 236:
/* Line 868 of glr.c  */
#line 905 "sql.ypp"
    { ((*yyvalp).ast_node) = new AstDataType (AST_DATA_TYPE, 21, NULL, 0, NULL, 0, NULL); }
    break;

  case 237:
/* Line 868 of glr.c  */
#line 906 "sql.ypp"
    { ((*yyvalp).ast_node) = new AstDataType (AST_DATA_TYPE, 22, NULL, 0, NULL, 0, NULL); }
    break;

  case 238:
/* Line 868 of glr.c  */
#line 907 "sql.ypp"
    { ((*yyvalp).ast_node) = new AstDataType (AST_DATA_TYPE, 23, NULL, 0, NULL, 0, NULL); }
    break;

  case 239:
/* Line 868 of glr.c  */
#line 908 "sql.ypp"
    { ((*yyvalp).ast_node) = new AstDataType (AST_DATA_TYPE, 24, NULL, 0, NULL, 0, NULL); }
    break;

  case 240:
/* Line 868 of glr.c  */
#line 909 "sql.ypp"
    { ((*yyvalp).ast_node) = new AstDataType (AST_DATA_TYPE, 25, NULL, 0, (((yyGLRStackItem const *)yyvsp)[YYFILL ((3) - (3))].yystate.yysemantics.yysval.ast_node), (((yyGLRStackItem const *)yyvsp)[YYFILL ((2) - (3))].yystate.yysemantics.yysval.intval), NULL); }
    break;

  case 241:
/* Line 868 of glr.c  */
#line 910 "sql.ypp"
    { ((*yyvalp).ast_node) = new AstDataType (AST_DATA_TYPE, 26, NULL, 0, (((yyGLRStackItem const *)yyvsp)[YYFILL ((3) - (3))].yystate.yysemantics.yysval.ast_node), (((yyGLRStackItem const *)yyvsp)[YYFILL ((2) - (3))].yystate.yysemantics.yysval.intval), NULL); }
    break;

  case 242:
/* Line 868 of glr.c  */
#line 911 "sql.ypp"
    { ((*yyvalp).ast_node) = new AstDataType (AST_DATA_TYPE, 27, NULL, 0, (((yyGLRStackItem const *)yyvsp)[YYFILL ((3) - (3))].yystate.yysemantics.yysval.ast_node), (((yyGLRStackItem const *)yyvsp)[YYFILL ((2) - (3))].yystate.yysemantics.yysval.intval), NULL); }
    break;

  case 243:
/* Line 868 of glr.c  */
#line 912 "sql.ypp"
    { ((*yyvalp).ast_node) = new AstDataType (AST_DATA_TYPE, 28, NULL, 0, (((yyGLRStackItem const *)yyvsp)[YYFILL ((3) - (3))].yystate.yysemantics.yysval.ast_node), (((yyGLRStackItem const *)yyvsp)[YYFILL ((2) - (3))].yystate.yysemantics.yysval.intval), NULL); }
    break;

  case 244:
/* Line 868 of glr.c  */
#line 913 "sql.ypp"
    { ((*yyvalp).ast_node) = new AstDataType (AST_DATA_TYPE, 29, NULL, 0, (((yyGLRStackItem const *)yyvsp)[YYFILL ((5) - (5))].yystate.yysemantics.yysval.ast_node), 0, (((yyGLRStackItem const *)yyvsp)[YYFILL ((3) - (5))].yystate.yysemantics.yysval.ast_node)); }
    break;

  case 245:
/* Line 868 of glr.c  */
#line 914 "sql.ypp"
    { ((*yyvalp).ast_node) = new AstDataType (AST_DATA_TYPE, 30, NULL, 0, (((yyGLRStackItem const *)yyvsp)[YYFILL ((5) - (5))].yystate.yysemantics.yysval.ast_node), 0, (((yyGLRStackItem const *)yyvsp)[YYFILL ((3) - (5))].yystate.yysemantics.yysval.ast_node)); }
    break;

  case 246:
/* Line 868 of glr.c  */
#line 916 "sql.ypp"
    { ((*yyvalp).ast_node) = new AstEnumList( AST_ENUM, string((((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (1))].yystate.yysemantics.yysval.strval)), NULL); }
    break;

  case 247:
/* Line 868 of glr.c  */
#line 917 "sql.ypp"
    { ((*yyvalp).ast_node) = new AstEnumList( AST_ENUM_LIST, string((((yyGLRStackItem const *)yyvsp)[YYFILL ((3) - (3))].yystate.yysemantics.yysval.strval)), (((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (3))].yystate.yysemantics.yysval.ast_node)); }
    break;

  case 248:
/* Line 868 of glr.c  */
#line 920 "sql.ypp"
    { ((*yyvalp).ast_node) = NULL; }
    break;

  case 249:
/* Line 868 of glr.c  */
#line 921 "sql.ypp"
    { ((*yyvalp).ast_node) = new AstOptLength (AST_OPT_LENGTH, atoi((((yyGLRStackItem const *)yyvsp)[YYFILL ((2) - (3))].yystate.yysemantics.yysval.strval)), 0); }
    break;

  case 250:
/* Line 868 of glr.c  */
#line 922 "sql.ypp"
    { ((*yyvalp).ast_node) = new AstOptLength (AST_OPT_LENGTH, atoi((((yyGLRStackItem const *)yyvsp)[YYFILL ((2) - (5))].yystate.yysemantics.yysval.strval)), atoi((((yyGLRStackItem const *)yyvsp)[YYFILL ((4) - (5))].yystate.yysemantics.yysval.strval))); }
    break;

  case 251:
/* Line 868 of glr.c  */
#line 925 "sql.ypp"
    { ((*yyvalp).intval) = 0; }
    break;

  case 252:
/* Line 868 of glr.c  */
#line 926 "sql.ypp"
    { ((*yyvalp).intval) = 1; }
    break;

  case 253:
/* Line 868 of glr.c  */
#line 929 "sql.ypp"
    { ((*yyvalp).intval) = 0; }
    break;

  case 254:
/* Line 868 of glr.c  */
#line 930 "sql.ypp"
    { ((*yyvalp).intval) = (((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (2))].yystate.yysemantics.yysval.intval) | 01; }
    break;

  case 255:
/* Line 868 of glr.c  */
#line 931 "sql.ypp"
    { ((*yyvalp).intval) = (((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (2))].yystate.yysemantics.yysval.intval) | 02; }
    break;

  case 256:
/* Line 868 of glr.c  */
#line 934 "sql.ypp"
    { ((*yyvalp).ast_node) =new AstOptCsc(AST_OPT_CSC, 0, "", "");}
    break;

  case 257:
/* Line 868 of glr.c  */
#line 936 "sql.ypp"
    { AstOptCsc* temp=static_cast<AstOptCsc*>((((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (4))].yystate.yysemantics.yysval.ast_node));temp->data_type_ |= 01; temp->str1_ = string((((yyGLRStackItem const *)yyvsp)[YYFILL ((4) - (4))].yystate.yysemantics.yysval.strval)); ((*yyvalp).ast_node) = temp; }
    break;

  case 258:
/* Line 868 of glr.c  */
#line 938 "sql.ypp"
    { AstOptCsc* temp=static_cast<AstOptCsc*>((((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (3))].yystate.yysemantics.yysval.ast_node));temp->data_type_ |= 02; temp->str2_ = string((((yyGLRStackItem const *)yyvsp)[YYFILL ((3) - (3))].yystate.yysemantics.yysval.strval)); ((*yyvalp).ast_node) = temp; }
    break;

  case 259:
/* Line 868 of glr.c  */
#line 942 "sql.ypp"
    { ((*yyvalp).ast_node) = (((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (1))].yystate.yysemantics.yysval.ast_node);/* puts("SQL parser： This is a create_projection statement");*/ }
    break;

  case 260:
/* Line 868 of glr.c  */
#line 946 "sql.ypp"
    { ((*yyvalp).ast_node) = new AstCreateProjection(AST_CREATE_PROJECTION, string((((yyGLRStackItem const *)yyvsp)[YYFILL ((4) - (10))].yystate.yysemantics.yysval.strval)), (((yyGLRStackItem const *)yyvsp)[YYFILL ((6) - (10))].yystate.yysemantics.yysval.ast_node), 1, string((((yyGLRStackItem const *)yyvsp)[YYFILL ((10) - (10))].yystate.yysemantics.yysval.strval))); }
    break;

  case 261:
/* Line 868 of glr.c  */
#line 948 "sql.ypp"
    { 
		if ((((yyGLRStackItem const *)yyvsp)[YYFILL ((9) - (13))].yystate.yysemantics.yysval.subtok) != 4) { yyerror(result,"please give a specific number"); } 
		else                                { ((*yyvalp).ast_node) = new AstCreateProjection(AST_CREATE_PROJECTION_NUM, string((((yyGLRStackItem const *)yyvsp)[YYFILL ((4) - (13))].yystate.yysemantics.yysval.strval)), (((yyGLRStackItem const *)yyvsp)[YYFILL ((6) - (13))].yystate.yysemantics.yysval.ast_node), atoi((((yyGLRStackItem const *)yyvsp)[YYFILL ((10) - (13))].yystate.yysemantics.yysval.strval)), string((((yyGLRStackItem const *)yyvsp)[YYFILL ((13) - (13))].yystate.yysemantics.yysval.strval))); }
	}
    break;

  case 262:
/* Line 868 of glr.c  */
#line 955 "sql.ypp"
    { ((*yyvalp).ast_node)=(((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (1))].yystate.yysemantics.yysval.ast_node);}
    break;

  case 263:
/* Line 868 of glr.c  */
#line 960 "sql.ypp"
    { ((*yyvalp).ast_node) = new AstCreateIndex(AST_CREATE_INDEX, (((yyGLRStackItem const *)yyvsp)[YYFILL ((2) - (10))].yystate.yysemantics.yysval.intval), string((((yyGLRStackItem const *)yyvsp)[YYFILL ((4) - (10))].yystate.yysemantics.yysval.strval)), (((yyGLRStackItem const *)yyvsp)[YYFILL ((5) - (10))].yystate.yysemantics.yysval.intval), (((yyGLRStackItem const *)yyvsp)[YYFILL ((7) - (10))].yystate.yysemantics.yysval.strval), (((yyGLRStackItem const *)yyvsp)[YYFILL ((9) - (10))].yystate.yysemantics.yysval.ast_node)); }
    break;

  case 264:
/* Line 868 of glr.c  */
#line 963 "sql.ypp"
    { ((*yyvalp).intval) = 0; }
    break;

  case 265:
/* Line 868 of glr.c  */
#line 964 "sql.ypp"
    { ((*yyvalp).intval) = 1; }
    break;

  case 266:
/* Line 868 of glr.c  */
#line 965 "sql.ypp"
    { ((*yyvalp).intval) = 2; }
    break;

  case 267:
/* Line 868 of glr.c  */
#line 966 "sql.ypp"
    { ((*yyvalp).intval) = 3; }
    break;

  case 268:
/* Line 868 of glr.c  */
#line 969 "sql.ypp"
    { ((*yyvalp).intval) = 0;  }
    break;

  case 269:
/* Line 868 of glr.c  */
#line 970 "sql.ypp"
    { ((*yyvalp).intval) = (((yyGLRStackItem const *)yyvsp)[YYFILL ((2) - (2))].yystate.yysemantics.yysval.intval); }
    break;

  case 270:
/* Line 868 of glr.c  */
#line 973 "sql.ypp"
    { ((*yyvalp).intval) = 1; }
    break;

  case 271:
/* Line 868 of glr.c  */
#line 974 "sql.ypp"
    { ((*yyvalp).intval) = 2; }
    break;

  case 272:
/* Line 868 of glr.c  */
#line 977 "sql.ypp"
    { ((*yyvalp).ast_node) = new AstIndexColList(AST_INDEX_COL, string((((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (3))].yystate.yysemantics.yysval.strval)), (((yyGLRStackItem const *)yyvsp)[YYFILL ((2) - (3))].yystate.yysemantics.yysval.ast_node), (((yyGLRStackItem const *)yyvsp)[YYFILL ((3) - (3))].yystate.yysemantics.yysval.intval), NULL); }
    break;

  case 273:
/* Line 868 of glr.c  */
#line 978 "sql.ypp"
    { ((*yyvalp).ast_node) = new AstIndexColList(AST_INDEX_COL_LIST, string((((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (5))].yystate.yysemantics.yysval.strval)), (((yyGLRStackItem const *)yyvsp)[YYFILL ((2) - (5))].yystate.yysemantics.yysval.ast_node), (((yyGLRStackItem const *)yyvsp)[YYFILL ((3) - (5))].yystate.yysemantics.yysval.intval), (((yyGLRStackItem const *)yyvsp)[YYFILL ((5) - (5))].yystate.yysemantics.yysval.ast_node)); }
    break;

  case 274:
/* Line 868 of glr.c  */
#line 982 "sql.ypp"
    { ((*yyvalp).ast_node) = (((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (1))].yystate.yysemantics.yysval.ast_node);}
    break;

  case 275:
/* Line 868 of glr.c  */
#line 986 "sql.ypp"
    {
		  if ((((yyGLRStackItem const *)yyvsp)[YYFILL ((11) - (12))].yystate.yysemantics.yysval.subtok) != 4) { yyerror(result,"please give a specific number"); } 
		  else {((*yyvalp).ast_node) = new AstLoadTable(AST_LOAD_TABLE, string((((yyGLRStackItem const *)yyvsp)[YYFILL ((3) - (12))].yystate.yysemantics.yysval.strval)), (((yyGLRStackItem const *)yyvsp)[YYFILL ((5) - (12))].yystate.yysemantics.yysval.ast_node), string((((yyGLRStackItem const *)yyvsp)[YYFILL ((7) - (12))].yystate.yysemantics.yysval.strval)), string((((yyGLRStackItem const *)yyvsp)[YYFILL ((9) - (12))].yystate.yysemantics.yysval.strval)), atof((((yyGLRStackItem const *)yyvsp)[YYFILL ((12) - (12))].yystate.yysemantics.yysval.strval)), 0);}
		}
    break;

  case 276:
/* Line 868 of glr.c  */
#line 990 "sql.ypp"
    { ((*yyvalp).ast_node) = new AstLoadTable(AST_LOAD_TABLE, string((((yyGLRStackItem const *)yyvsp)[YYFILL ((3) - (9))].yystate.yysemantics.yysval.strval)), (((yyGLRStackItem const *)yyvsp)[YYFILL ((5) - (9))].yystate.yysemantics.yysval.ast_node), string((((yyGLRStackItem const *)yyvsp)[YYFILL ((7) - (9))].yystate.yysemantics.yysval.strval)), string((((yyGLRStackItem const *)yyvsp)[YYFILL ((9) - (9))].yystate.yysemantics.yysval.strval)), 1.0, 0);}
    break;

  case 277:
/* Line 868 of glr.c  */
#line 991 "sql.ypp"
    { ((*yyvalp).ast_node) = new AstLoadTable(AST_LOAD_TABLE, string((((yyGLRStackItem const *)yyvsp)[YYFILL ((3) - (9))].yystate.yysemantics.yysval.strval)), (((yyGLRStackItem const *)yyvsp)[YYFILL ((5) - (9))].yystate.yysemantics.yysval.ast_node), string((((yyGLRStackItem const *)yyvsp)[YYFILL ((7) - (9))].yystate.yysemantics.yysval.strval)), string((((yyGLRStackItem const *)yyvsp)[YYFILL ((9) - (9))].yystate.yysemantics.yysval.strval)), 1.0, 1);}
    break;

  case 278:
/* Line 868 of glr.c  */
#line 993 "sql.ypp"
    { if ((((yyGLRStackItem const *)yyvsp)[YYFILL ((11) - (12))].yystate.yysemantics.yysval.subtok) != 4) { yyerror(result,"please give a specific number"); } 
		 else {((*yyvalp).ast_node) = new AstLoadTable(AST_LOAD_TABLE, string((((yyGLRStackItem const *)yyvsp)[YYFILL ((3) - (12))].yystate.yysemantics.yysval.strval)), (((yyGLRStackItem const *)yyvsp)[YYFILL ((5) - (12))].yystate.yysemantics.yysval.ast_node), string((((yyGLRStackItem const *)yyvsp)[YYFILL ((7) - (12))].yystate.yysemantics.yysval.strval)), string((((yyGLRStackItem const *)yyvsp)[YYFILL ((9) - (12))].yystate.yysemantics.yysval.strval)), atof((((yyGLRStackItem const *)yyvsp)[YYFILL ((12) - (12))].yystate.yysemantics.yysval.strval)), 1);}
		}
    break;

  case 279:
/* Line 868 of glr.c  */
#line 999 "sql.ypp"
    { ((*yyvalp).ast_node) = (((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (1))].yystate.yysemantics.yysval.ast_node); /*output($$, 1); puts("SQL parser： This is a drop_index statement");*/ }
    break;

  case 280:
/* Line 868 of glr.c  */
#line 1002 "sql.ypp"
    { ((*yyvalp).ast_node) = new AstDropIndex(AST_DROP_INDEX, string((((yyGLRStackItem const *)yyvsp)[YYFILL ((3) - (5))].yystate.yysemantics.yysval.strval)), string((((yyGLRStackItem const *)yyvsp)[YYFILL ((5) - (5))].yystate.yysemantics.yysval.strval))); }
    break;

  case 281:
/* Line 868 of glr.c  */
#line 1006 "sql.ypp"
    { ((*yyvalp).ast_node) = (((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (1))].yystate.yysemantics.yysval.ast_node);/* output($$, 1); puts("SQL parser： This is a drop_database statement");*/ }
    break;

  case 282:
/* Line 868 of glr.c  */
#line 1010 "sql.ypp"
    { ((*yyvalp).ast_node) = new AstDropDatabase(AST_DROP_DB, 1, (((yyGLRStackItem const *)yyvsp)[YYFILL ((3) - (4))].yystate.yysemantics.yysval.intval), string((((yyGLRStackItem const *)yyvsp)[YYFILL ((4) - (4))].yystate.yysemantics.yysval.strval))); }
    break;

  case 283:
/* Line 868 of glr.c  */
#line 1011 "sql.ypp"
    { ((*yyvalp).ast_node) = new AstDropDatabase(AST_DROP_SCHEMA, 2, (((yyGLRStackItem const *)yyvsp)[YYFILL ((3) - (4))].yystate.yysemantics.yysval.intval), string((((yyGLRStackItem const *)yyvsp)[YYFILL ((4) - (4))].yystate.yysemantics.yysval.strval))); }
    break;

  case 284:
/* Line 868 of glr.c  */
#line 1014 "sql.ypp"
    { ((*yyvalp).intval) = 0; }
    break;

  case 285:
/* Line 868 of glr.c  */
#line 1015 "sql.ypp"
    { ((*yyvalp).intval) = 1; }
    break;

  case 286:
/* Line 868 of glr.c  */
#line 1019 "sql.ypp"
    { ((*yyvalp).ast_node) = (((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (1))].yystate.yysemantics.yysval.ast_node); /*output($$, 1); puts("SQL parser： This is a drop_table statement"); */}
    break;

  case 287:
/* Line 868 of glr.c  */
#line 1023 "sql.ypp"
    { ((*yyvalp).ast_node) = new AstDropTable(AST_DROP_TABLE, (((yyGLRStackItem const *)yyvsp)[YYFILL ((2) - (6))].yystate.yysemantics.yysval.intval), (((yyGLRStackItem const *)yyvsp)[YYFILL ((4) - (6))].yystate.yysemantics.yysval.intval), (((yyGLRStackItem const *)yyvsp)[YYFILL ((6) - (6))].yystate.yysemantics.yysval.intval), NULL); }
    break;

  case 288:
/* Line 868 of glr.c  */
#line 1026 "sql.ypp"
    { ((*yyvalp).ast_node) = new AstDropTableList(AST_DROP_TABLE_LIST, "", string((((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (1))].yystate.yysemantics.yysval.strval)), NULL); }
    break;

  case 289:
/* Line 868 of glr.c  */
#line 1027 "sql.ypp"
    { ((*yyvalp).ast_node) = new AstDropTableList(AST_DROP_TABLE_LIST, string((((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (3))].yystate.yysemantics.yysval.strval)), string((((yyGLRStackItem const *)yyvsp)[YYFILL ((3) - (3))].yystate.yysemantics.yysval.strval)), NULL); }
    break;

  case 290:
/* Line 868 of glr.c  */
#line 1028 "sql.ypp"
    { ((*yyvalp).ast_node) = new AstDropTableList(AST_DROP_TABLE_LIST, "", string((((yyGLRStackItem const *)yyvsp)[YYFILL ((3) - (3))].yystate.yysemantics.yysval.strval)), (((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (3))].yystate.yysemantics.yysval.ast_node)); }
    break;

  case 291:
/* Line 868 of glr.c  */
#line 1029 "sql.ypp"
    { ((*yyvalp).ast_node) = new AstDropTableList(AST_DROP_TABLE_LIST, string((((yyGLRStackItem const *)yyvsp)[YYFILL ((3) - (5))].yystate.yysemantics.yysval.strval)), string((((yyGLRStackItem const *)yyvsp)[YYFILL ((5) - (5))].yystate.yysemantics.yysval.strval)), (((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (5))].yystate.yysemantics.yysval.ast_node)); }
    break;

  case 292:
/* Line 868 of glr.c  */
#line 1032 "sql.ypp"
    { ((*yyvalp).intval) = 0; }
    break;

  case 293:
/* Line 868 of glr.c  */
#line 1033 "sql.ypp"
    { ((*yyvalp).intval) = 1; }
    break;

  case 294:
/* Line 868 of glr.c  */
#line 1034 "sql.ypp"
    { ((*yyvalp).intval) = 2; }
    break;

  case 295:
/* Line 868 of glr.c  */
#line 1040 "sql.ypp"
    { ((*yyvalp).ast_node) = (((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (1))].yystate.yysemantics.yysval.ast_node);}
    break;

  case 296:
/* Line 868 of glr.c  */
#line 1046 "sql.ypp"
    { ((*yyvalp).ast_node) = new AstInsertStmt(AST_INSERT_STMT, (((yyGLRStackItem const *)yyvsp)[YYFILL ((2) - (8))].yystate.yysemantics.yysval.intval), string((((yyGLRStackItem const *)yyvsp)[YYFILL ((4) - (8))].yystate.yysemantics.yysval.strval)), (((yyGLRStackItem const *)yyvsp)[YYFILL ((5) - (8))].yystate.yysemantics.yysval.ast_node), (((yyGLRStackItem const *)yyvsp)[YYFILL ((7) - (8))].yystate.yysemantics.yysval.ast_node), (((yyGLRStackItem const *)yyvsp)[YYFILL ((8) - (8))].yystate.yysemantics.yysval.ast_node), NULL, NULL); }
    break;

  case 297:
/* Line 868 of glr.c  */
#line 1049 "sql.ypp"
    { ((*yyvalp).ast_node) = NULL; }
    break;

  case 298:
/* Line 868 of glr.c  */
#line 1050 "sql.ypp"
    { ((*yyvalp).ast_node) = (((yyGLRStackItem const *)yyvsp)[YYFILL ((5) - (5))].yystate.yysemantics.yysval.ast_node); }
    break;

  case 299:
/* Line 868 of glr.c  */
#line 1053 "sql.ypp"
    { ((*yyvalp).intval) = 0; }
    break;

  case 300:
/* Line 868 of glr.c  */
#line 1054 "sql.ypp"
    { ((*yyvalp).intval) = (((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (2))].yystate.yysemantics.yysval.intval) | 01 ; }
    break;

  case 301:
/* Line 868 of glr.c  */
#line 1055 "sql.ypp"
    { ((*yyvalp).intval) = (((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (2))].yystate.yysemantics.yysval.intval) | 02 ; }
    break;

  case 302:
/* Line 868 of glr.c  */
#line 1056 "sql.ypp"
    { ((*yyvalp).intval) = (((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (2))].yystate.yysemantics.yysval.intval) | 04 ; }
    break;

  case 303:
/* Line 868 of glr.c  */
#line 1057 "sql.ypp"
    { ((*yyvalp).intval) = (((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (2))].yystate.yysemantics.yysval.intval) | 010 ; }
    break;

  case 306:
/* Line 868 of glr.c  */
#line 1063 "sql.ypp"
    { ((*yyvalp).ast_node) = NULL; }
    break;

  case 307:
/* Line 868 of glr.c  */
#line 1064 "sql.ypp"
    { ((*yyvalp).ast_node) = (((yyGLRStackItem const *)yyvsp)[YYFILL ((2) - (3))].yystate.yysemantics.yysval.ast_node); }
    break;

  case 308:
/* Line 868 of glr.c  */
#line 1067 "sql.ypp"
    { ((*yyvalp).ast_node) = new AstInsertValList(AST_INSERT_VALUE_LIST, (((yyGLRStackItem const *)yyvsp)[YYFILL ((2) - (3))].yystate.yysemantics.yysval.ast_node), NULL); }
    break;

  case 309:
/* Line 868 of glr.c  */
#line 1068 "sql.ypp"
    { ((*yyvalp).ast_node) = new AstInsertValList(AST_INSERT_VALUE_LIST, (((yyGLRStackItem const *)yyvsp)[YYFILL ((2) - (5))].yystate.yysemantics.yysval.ast_node), (((yyGLRStackItem const *)yyvsp)[YYFILL ((5) - (5))].yystate.yysemantics.yysval.ast_node)); }
    break;

  case 310:
/* Line 868 of glr.c  */
#line 1072 "sql.ypp"
    { ((*yyvalp).ast_node) = new AstInsertVals(AST_INSERT_VALUE,0, (((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (1))].yystate.yysemantics.yysval.ast_node), NULL); }
    break;

  case 311:
/* Line 868 of glr.c  */
#line 1073 "sql.ypp"
    { ((*yyvalp).ast_node) = new AstInsertVals(AST_INSERT_VALUE,1, NULL, NULL); }
    break;

  case 312:
/* Line 868 of glr.c  */
#line 1074 "sql.ypp"
    { ((*yyvalp).ast_node) = new AstInsertVals(AST_INSERT_VALUE,0, (((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (3))].yystate.yysemantics.yysval.ast_node), (((yyGLRStackItem const *)yyvsp)[YYFILL ((3) - (3))].yystate.yysemantics.yysval.ast_node)); }
    break;

  case 313:
/* Line 868 of glr.c  */
#line 1075 "sql.ypp"
    { ((*yyvalp).ast_node) = new AstInsertVals(AST_INSERT_VALUE,1, NULL, (((yyGLRStackItem const *)yyvsp)[YYFILL ((3) - (3))].yystate.yysemantics.yysval.ast_node)); }
    break;

  case 314:
/* Line 868 of glr.c  */
#line 1079 "sql.ypp"
    { ((*yyvalp).ast_node) = new AstInsertStmt(AST_INSERT_STMT, (((yyGLRStackItem const *)yyvsp)[YYFILL ((2) - (7))].yystate.yysemantics.yysval.intval), string((((yyGLRStackItem const *)yyvsp)[YYFILL ((4) - (7))].yystate.yysemantics.yysval.strval)), NULL, NULL, (((yyGLRStackItem const *)yyvsp)[YYFILL ((7) - (7))].yystate.yysemantics.yysval.ast_node), (((yyGLRStackItem const *)yyvsp)[YYFILL ((6) - (7))].yystate.yysemantics.yysval.ast_node), NULL); }
    break;

  case 315:
/* Line 868 of glr.c  */
#line 1083 "sql.ypp"
    { ((*yyvalp).ast_node) = new AstInsertStmt(AST_INSERT_STMT, (((yyGLRStackItem const *)yyvsp)[YYFILL ((2) - (7))].yystate.yysemantics.yysval.intval), string((((yyGLRStackItem const *)yyvsp)[YYFILL ((4) - (7))].yystate.yysemantics.yysval.strval)), (((yyGLRStackItem const *)yyvsp)[YYFILL ((5) - (7))].yystate.yysemantics.yysval.ast_node), NULL, (((yyGLRStackItem const *)yyvsp)[YYFILL ((7) - (7))].yystate.yysemantics.yysval.ast_node), NULL, (((yyGLRStackItem const *)yyvsp)[YYFILL ((6) - (7))].yystate.yysemantics.yysval.ast_node)); }
    break;

  case 316:
/* Line 868 of glr.c  */
#line 1087 "sql.ypp"
    { 
		if((((yyGLRStackItem const *)yyvsp)[YYFILL ((2) - (3))].yystate.yysemantics.yysval.subtok) != 4) yyerror(result,"bad insert assignment to %s", (((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (3))].yystate.yysemantics.yysval.strval)); 
		else ((*yyvalp).ast_node) = new AstInsertAssignList(AST_INSERT_ASSIGN_LIST, string((((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (3))].yystate.yysemantics.yysval.strval)), 0, (((yyGLRStackItem const *)yyvsp)[YYFILL ((3) - (3))].yystate.yysemantics.yysval.ast_node), NULL); }
    break;

  case 317:
/* Line 868 of glr.c  */
#line 1090 "sql.ypp"
    { 
		if ((((yyGLRStackItem const *)yyvsp)[YYFILL ((2) - (3))].yystate.yysemantics.yysval.subtok) != 4) yyerror(result,"bad insert assignment to %s", (((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (3))].yystate.yysemantics.yysval.strval));
 		else ((*yyvalp).ast_node) = new AstInsertAssignList(AST_INSERT_ASSIGN_LIST, string((((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (3))].yystate.yysemantics.yysval.strval)), 1, NULL, NULL); }
    break;

  case 318:
/* Line 868 of glr.c  */
#line 1093 "sql.ypp"
    { 
		if ((((yyGLRStackItem const *)yyvsp)[YYFILL ((4) - (5))].yystate.yysemantics.yysval.subtok) != 4) yyerror(result,"bad insert assignment to %s", (((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (5))].yystate.yysemantics.yysval.ast_node));		 
		else ((*yyvalp).ast_node) = new AstInsertAssignList(AST_INSERT_ASSIGN_LIST, string((((yyGLRStackItem const *)yyvsp)[YYFILL ((3) - (5))].yystate.yysemantics.yysval.strval)), 0, (((yyGLRStackItem const *)yyvsp)[YYFILL ((5) - (5))].yystate.yysemantics.yysval.ast_node), (((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (5))].yystate.yysemantics.yysval.ast_node)); }
    break;

  case 319:
/* Line 868 of glr.c  */
#line 1096 "sql.ypp"
    { 
		if ((((yyGLRStackItem const *)yyvsp)[YYFILL ((4) - (5))].yystate.yysemantics.yysval.subtok) != 4) yyerror(result,"bad insert assignment to %s", (((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (5))].yystate.yysemantics.yysval.ast_node));
		else ((*yyvalp).ast_node) = new AstInsertAssignList(AST_INSERT_ASSIGN_LIST, string((((yyGLRStackItem const *)yyvsp)[YYFILL ((3) - (5))].yystate.yysemantics.yysval.strval)), 1, NULL, (((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (5))].yystate.yysemantics.yysval.ast_node)); }
    break;


/* Line 868 of glr.c  */
#line 4041 "sql.tab.cpp"
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
yydestruct (const char *yymsg, int yytype, YYSTYPE *yyvaluep, struct ParseResult* result)
{
  YYUSE (yyvaluep);
  YYUSE (result);

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
yydestroyGLRState (char const *yymsg, yyGLRState *yys, struct ParseResult* result)
{
  if (yys->yyresolved)
    yydestruct (yymsg, yystos[yys->yylrState],
                &yys->yysemantics.yysval, result);
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
                           YY_NULL, result);
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
            yydestroyGLRState (yymsg, yyrh, result);
        }
    }
}

/** Left-hand-side symbol for rule #RULE.  */
static inline yySymbol
yylhsNonterm (yyRuleNum yyrule)
{
  return yyr1[yyrule];
}

#define yypact_value_is_default(Yystate) \
  (!!((Yystate) == (-440)))

/** True iff LR state STATE has only a default reduction (regardless
 *  of token).  */
static inline yybool
yyisDefaultedState (yyStateNum yystate)
{
  return yypact_value_is_default (yypact[yystate]);
}

/** The default reduction for STATE, assuming it has one.  */
static inline yyRuleNum
yydefaultAction (yyStateNum yystate)
{
  return yydefact[yystate];
}

#define yytable_value_is_error(Yytable_value) \
  (!!((Yytable_value) == (-265)))

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
  if (yypact_value_is_default (yypact[yystate])
      || yyindex < 0 || YYLAST < yyindex || yycheck[yyindex] != yytoken)
    {
      *yyaction = -yydefact[yystate];
      *yyconflicts = yyconfl;
    }
  else if (! yytable_value_is_error (yytable[yyindex]))
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
  yyset->yystates[0] = YY_NULL;
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
  yystackp->yysplitPoint = YY_NULL;
  yystackp->yylastDeleted = YY_NULL;
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
  size_t yynewSize;
  size_t yyn;
  size_t yysize = yystackp->yynextFree - yystackp->yyitems;
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
          if (yys0->yypred != YY_NULL)
            yys1->yypred =
              YYRELOC (yyp0, yyp1, yys0->yypred, yystate);
          if (! yys0->yyresolved && yys0->yysemantics.yyfirstVal != YY_NULL)
            yys1->yysemantics.yyfirstVal =
              YYRELOC (yyp0, yyp1, yys0->yysemantics.yyfirstVal, yyoption);
        }
      else
        {
          yySemanticOption* yyv0 = &yyp0->yyoption;
          yySemanticOption* yyv1 = &yyp1->yyoption;
          if (yyv0->yystate != YY_NULL)
            yyv1->yystate = YYRELOC (yyp0, yyp1, yyv0->yystate, yystate);
          if (yyv0->yynext != YY_NULL)
            yyv1->yynext = YYRELOC (yyp0, yyp1, yyv0->yynext, yyoption);
        }
    }
  if (yystackp->yysplitPoint != YY_NULL)
    yystackp->yysplitPoint = YYRELOC (yystackp->yyitems, yynewItems,
                                 yystackp->yysplitPoint, yystate);

  for (yyn = 0; yyn < yystackp->yytops.yysize; yyn += 1)
    if (yystackp->yytops.yystates[yyn] != YY_NULL)
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
  if (yystackp->yysplitPoint != YY_NULL && yystackp->yysplitPoint > yys)
    yystackp->yysplitPoint = yys;
}

/** Invalidate stack #K in STACK.  */
static inline void
yymarkStackDeleted (yyGLRStack* yystackp, size_t yyk)
{
  if (yystackp->yytops.yystates[yyk] != YY_NULL)
    yystackp->yylastDeleted = yystackp->yytops.yystates[yyk];
  yystackp->yytops.yystates[yyk] = YY_NULL;
}

/** Undelete the last stack that was marked as deleted.  Can only be
    done once after a deletion, and only when all other stacks have
    been deleted.  */
static void
yyundeleteLastStack (yyGLRStack* yystackp)
{
  if (yystackp->yylastDeleted == YY_NULL || yystackp->yytops.yysize != 0)
    return;
  yystackp->yytops.yystates[0] = yystackp->yylastDeleted;
  yystackp->yytops.yysize = 1;
  YYDPRINTF ((stderr, "Restoring last deleted stack as stack #0.\n"));
  yystackp->yylastDeleted = YY_NULL;
}

static inline void
yyremoveDeletes (yyGLRStack* yystackp)
{
  size_t yyi, yyj;
  yyi = yyj = 0;
  while (yyj < yystackp->yytops.yysize)
    {
      if (yystackp->yytops.yystates[yyi] == YY_NULL)
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
            YYSTYPE* yyvalp)
{
  yyGLRState* yynewState = &yynewGLRStackItem (yystackp, yytrue)->yystate;

  yynewState->yylrState = yylrState;
  yynewState->yyposn = yyposn;
  yynewState->yyresolved = yytrue;
  yynewState->yypred = yystackp->yytops.yystates[yyk];
  yynewState->yysemantics.yysval = *yyvalp;
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
  yynewState->yysemantics.yyfirstVal = YY_NULL;
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
            YYSTYPE* yyvalp, struct ParseResult* result)
{
  int yynrhs = yyrhsLength (yyrule);

  if (yystackp->yysplitPoint == YY_NULL)
    {
      /* Standard special case: single stack.  */
      yyGLRStackItem* rhs = (yyGLRStackItem*) yystackp->yytops.yystates[yyk];
      YYASSERT (yyk == 0);
      yystackp->yynextFree -= yynrhs;
      yystackp->yyspaceLeft += yynrhs;
      yystackp->yytops.yystates[0] = & yystackp->yynextFree[-1].yystate;
      return yyuserAction (yyrule, yynrhs, rhs, yystackp,
                           yyvalp, result);
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
                           yystackp, yyvalp, result);
    }
}

#if !YYDEBUG
# define YY_REDUCE_PRINT(Args)
#else
# define YY_REDUCE_PRINT(Args)          \
do {                                    \
  if (yydebug)                          \
    yy_reduce_print Args;               \
} while (YYID (0))

/*----------------------------------------------------------.
| Report that the RULE is going to be reduced on stack #K.  |
`----------------------------------------------------------*/

/*ARGSUSED*/ static inline void
yy_reduce_print (yyGLRStack* yystackp, size_t yyk, yyRuleNum yyrule,
                 YYSTYPE* yyvalp, struct ParseResult* result)
{
  int yynrhs = yyrhsLength (yyrule);
  yybool yynormal __attribute__ ((__unused__)) =
    (yystackp->yysplitPoint == YY_NULL);
  yyGLRStackItem* yyvsp = (yyGLRStackItem*) yystackp->yytops.yystates[yyk];
  int yylow = 1;
  int yyi;
  YYUSE (yyvalp);
  YYUSE (result);
  YYFPRINTF (stderr, "Reducing stack %lu by rule %d (line %lu):\n",
             (unsigned long int) yyk, yyrule - 1,
             (unsigned long int) yyrline[yyrule]);
  /* The symbols being reduced.  */
  for (yyi = 0; yyi < yynrhs; yyi++)
    {
      YYFPRINTF (stderr, "   $%d = ", yyi + 1);
      yy_symbol_print (stderr, yyrhs[yyprhs[yyrule] + yyi],
                       &(((yyGLRStackItem const *)yyvsp)[YYFILL ((yyi + 1) - (yynrhs))].yystate.yysemantics.yysval)
                                              , result);
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
             yybool yyforceEval, struct ParseResult* result)
{
  size_t yyposn = yystackp->yytops.yystates[yyk]->yyposn;

  if (yyforceEval || yystackp->yysplitPoint == YY_NULL)
    {
      YYSTYPE yysval;

      YY_REDUCE_PRINT ((yystackp, yyk, yyrule, &yysval, result));
      YYCHK (yydoAction (yystackp, yyk, yyrule, &yysval, result));
      YY_SYMBOL_PRINT ("-> $$ =", yyr1[yyrule], &yysval, &yyloc);
      yyglrShift (yystackp, yyk,
                  yyLRgotoState (yystackp->yytops.yystates[yyk]->yylrState,
                                 yylhsNonterm (yyrule)),
                  yyposn, &yysval);
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
        if (yyi != yyk && yystackp->yytops.yystates[yyi] != YY_NULL)
          {
            yyGLRState *yysplit = yystackp->yysplitPoint;
            yyGLRState *yyp = yystackp->yytops.yystates[yyi];
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
  if (yystackp->yysplitPoint == YY_NULL)
    {
      YYASSERT (yyk == 0);
      yystackp->yysplitPoint = yystackp->yytops.yystates[yyk];
    }
  if (yystackp->yytops.yysize >= yystackp->yytops.yycapacity)
    {
      yyGLRState** yynewStates;
      yybool* yynewLookaheadNeeds;

      yynewStates = YY_NULL;

      if (yystackp->yytops.yycapacity
          > (YYSIZEMAX / (2 * sizeof yynewStates[0])))
        yyMemoryExhausted (yystackp);
      yystackp->yytops.yycapacity *= 2;

      yynewStates =
        (yyGLRState**) YYREALLOC (yystackp->yytops.yystates,
                                  (yystackp->yytops.yycapacity
                                   * sizeof yynewStates[0]));
      if (yynewStates == YY_NULL)
        yyMemoryExhausted (yystackp);
      yystackp->yytops.yystates = yynewStates;

      yynewLookaheadNeeds =
        (yybool*) YYREALLOC (yystackp->yytops.yylookaheadNeeds,
                             (yystackp->yytops.yycapacity
                              * sizeof yynewLookaheadNeeds[0]));
      if (yynewLookaheadNeeds == YY_NULL)
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
          yySemanticOption** yyz0p = &yys0->yysemantics.yyfirstVal;
          yySemanticOption* yyz1 = yys1->yysemantics.yyfirstVal;
          while (YYID (yytrue))
            {
              if (yyz1 == *yyz0p || yyz1 == YY_NULL)
                break;
              else if (*yyz0p == YY_NULL)
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
                                   yyGLRStack* yystackp, struct ParseResult* result);


/** Resolve the previous N states starting at and including state S.  If result
 *  != yyok, some states may have been left unresolved possibly with empty
 *  semantic option chains.  Regardless of whether result = yyok, each state
 *  has been left with consistent data so that yydestroyGLRState can be invoked
 *  if necessary.  */
static YYRESULTTAG
yyresolveStates (yyGLRState* yys, int yyn,
                 yyGLRStack* yystackp, struct ParseResult* result)
{
  if (0 < yyn)
    {
      YYASSERT (yys->yypred);
      YYCHK (yyresolveStates (yys->yypred, yyn-1, yystackp, result));
      if (! yys->yyresolved)
        YYCHK (yyresolveValue (yys, yystackp, result));
    }
  return yyok;
}

/** Resolve the states for the RHS of OPT, perform its user action, and return
 *  the semantic value and location.  Regardless of whether result = yyok, all
 *  RHS states have been destroyed (assuming the user action destroys all RHS
 *  semantic values if invoked).  */
static YYRESULTTAG
yyresolveAction (yySemanticOption* yyopt, yyGLRStack* yystackp,
                 YYSTYPE* yyvalp, struct ParseResult* result)
{
  yyGLRStackItem yyrhsVals[YYMAXRHS + YYMAXLEFT + 1];
  int yynrhs = yyrhsLength (yyopt->yyrule);
  YYRESULTTAG yyflag =
    yyresolveStates (yyopt->yystate, yynrhs, yystackp, result);
  if (yyflag != yyok)
    {
      yyGLRState *yys;
      for (yys = yyopt->yystate; yynrhs > 0; yys = yys->yypred, yynrhs -= 1)
        yydestroyGLRState ("Cleanup: popping", yys, result);
      return yyflag;
    }

  yyrhsVals[YYMAXRHS + YYMAXLEFT].yystate.yypred = yyopt->yystate;
  {
    int yychar_current = yychar;
    YYSTYPE yylval_current = yylval;
    yychar = yyopt->yyrawchar;
    yylval = yyopt->yyval;
    yyflag = yyuserAction (yyopt->yyrule, yynrhs,
                           yyrhsVals + YYMAXRHS + YYMAXLEFT - 1,
                           yystackp, yyvalp, result);
    yychar = yychar_current;
    yylval = yylval_current;
  }
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
  if (yys == YY_NULL)
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
                   yySemanticOption* yyx1, struct ParseResult* result)
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

  yyerror (result, YY_("syntax is ambiguous"));
  return yyabort;
}

/** Resolve the ambiguity represented in state S, perform the indicated
 *  actions, and set the semantic value of S.  If result != yyok, the chain of
 *  semantic options in S has been cleared instead or it has been left
 *  unmodified except that redundant options may have been removed.  Regardless
 *  of whether result = yyok, S has been left with consistent data so that
 *  yydestroyGLRState can be invoked if necessary.  */
static YYRESULTTAG
yyresolveValue (yyGLRState* yys, yyGLRStack* yystackp, struct ParseResult* result)
{
  yySemanticOption* yyoptionList = yys->yysemantics.yyfirstVal;
  yySemanticOption* yybest = yyoptionList;
  yySemanticOption** yypp;
  yybool yymerge = yyfalse;
  YYSTYPE yysval;
  YYRESULTTAG yyflag;

  for (yypp = &yyoptionList->yynext; *yypp != YY_NULL; )
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
              return yyreportAmbiguity (yybest, yyp, result);
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
      yyflag = yyresolveAction (yybest, yystackp, &yysval, result);
      if (yyflag == yyok)
        for (yyp = yybest->yynext; yyp != YY_NULL; yyp = yyp->yynext)
          {
            if (yyprec == yydprec[yyp->yyrule])
              {
                YYSTYPE yysval_other;
                yyflag = yyresolveAction (yyp, yystackp, &yysval_other, result);
                if (yyflag != yyok)
                  {
                    yydestruct ("Cleanup: discarding incompletely merged value for",
                                yystos[yys->yylrState],
                                &yysval, result);
                    break;
                  }
                yyuserMerge (yymerger[yyp->yyrule], &yysval, &yysval_other);
              }
          }
    }
  else
    yyflag = yyresolveAction (yybest, yystackp, &yysval, result);

  if (yyflag == yyok)
    {
      yys->yyresolved = yytrue;
      yys->yysemantics.yysval = yysval;
    }
  else
    yys->yysemantics.yyfirstVal = YY_NULL;
  return yyflag;
}

static YYRESULTTAG
yyresolveStack (yyGLRStack* yystackp, struct ParseResult* result)
{
  if (yystackp->yysplitPoint != YY_NULL)
    {
      yyGLRState* yys;
      int yyn;

      for (yyn = 0, yys = yystackp->yytops.yystates[0];
           yys != yystackp->yysplitPoint;
           yys = yys->yypred, yyn += 1)
        continue;
      YYCHK (yyresolveStates (yystackp->yytops.yystates[0], yyn, yystackp
                             , result));
    }
  return yyok;
}

static void
yycompressStack (yyGLRStack* yystackp)
{
  yyGLRState* yyp, *yyq, *yyr;

  if (yystackp->yytops.yysize != 1 || yystackp->yysplitPoint == YY_NULL)
    return;

  for (yyp = yystackp->yytops.yystates[0], yyq = yyp->yypred, yyr = YY_NULL;
       yyp != yystackp->yysplitPoint;
       yyr = yyp, yyp = yyq, yyq = yyp->yypred)
    yyp->yypred = yyr;

  yystackp->yyspaceLeft += yystackp->yynextFree - yystackp->yyitems;
  yystackp->yynextFree = ((yyGLRStackItem*) yystackp->yysplitPoint) + 1;
  yystackp->yyspaceLeft -= yystackp->yynextFree - yystackp->yyitems;
  yystackp->yysplitPoint = YY_NULL;
  yystackp->yylastDeleted = YY_NULL;

  while (yyr != YY_NULL)
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
                   size_t yyposn, struct ParseResult* result)
{
  int yyaction;
  const short int* yyconflicts;
  yyRuleNum yyrule;

  while (yystackp->yytops.yystates[yyk] != YY_NULL)
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
          YYCHK (yyglrReduce (yystackp, yyk, yyrule, yyfalse, result));
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
                                  *yyconflicts, yyfalse, result));
              YYCHK (yyprocessOneStack (yystackp, yynewStack,
                                        yyposn, result));
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
                                yyfalse, result));
        }
    }
  return yyok;
}

/*ARGSUSED*/ static void
yyreportSyntaxError (yyGLRStack* yystackp, struct ParseResult* result)
{
  if (yystackp->yyerrState != 0)
    return;
#if ! YYERROR_VERBOSE
  yyerror (result, YY_("syntax error"));
#else
  {
  yySymbol yytoken = yychar == YYEMPTY ? YYEMPTY : YYTRANSLATE (yychar);
  size_t yysize0 = yytnamerr (YY_NULL, yytokenName (yytoken));
  size_t yysize = yysize0;
  yybool yysize_overflow = yyfalse;
  char* yymsg = YY_NULL;
  enum { YYERROR_VERBOSE_ARGS_MAXIMUM = 5 };
  /* Internationalized format string. */
  const char *yyformat = YY_NULL;
  /* Arguments of yyformat. */
  char const *yyarg[YYERROR_VERBOSE_ARGS_MAXIMUM];
  /* Number of reported tokens (one for the "unexpected", one per
     "expected").  */
  int yycount = 0;

  /* There are many possibilities here to consider:
     - If this state is a consistent state with a default action, then
       the only way this function was invoked is if the default action
       is an error action.  In that case, don't check for expected
       tokens because there are none.
     - The only way there can be no lookahead present (in yychar) is if
       this state is a consistent state with a default action.  Thus,
       detecting the absence of a lookahead is sufficient to determine
       that there is no unexpected or expected token to report.  In that
       case, just report a simple "syntax error".
     - Don't assume there isn't a lookahead just because this state is a
       consistent state with a default action.  There might have been a
       previous inconsistent state, consistent state with a non-default
       action, or user semantic action that manipulated yychar.
     - Of course, the expected token list depends on states to have
       correct lookahead information, and it depends on the parser not
       to perform extra reductions after fetching a lookahead from the
       scanner and before detecting a syntax error.  Thus, state merging
       (from LALR or IELR) and default reductions corrupt the expected
       token list.  However, the list is correct for canonical LR with
       one exception: it will still contain any token that will not be
       accepted due to an error action in a later state.
  */
  if (yytoken != YYEMPTY)
    {
      int yyn = yypact[yystackp->yytops.yystates[0]->yylrState];
      yyarg[yycount++] = yytokenName (yytoken);
      if (!yypact_value_is_default (yyn))
        {
          /* Start YYX at -YYN if negative to avoid negative indexes in
             YYCHECK.  In other words, skip the first -YYN actions for this
             state because they are default actions.  */
          int yyxbegin = yyn < 0 ? -yyn : 0;
          /* Stay within bounds of both yycheck and yytname.  */
          int yychecklim = YYLAST - yyn + 1;
          int yyxend = yychecklim < YYNTOKENS ? yychecklim : YYNTOKENS;
          int yyx;
          for (yyx = yyxbegin; yyx < yyxend; ++yyx)
            if (yycheck[yyx + yyn] == yyx && yyx != YYTERROR
                && !yytable_value_is_error (yytable[yyx + yyn]))
              {
                if (yycount == YYERROR_VERBOSE_ARGS_MAXIMUM)
                  {
                    yycount = 1;
                    yysize = yysize0;
                    break;
                  }
                yyarg[yycount++] = yytokenName (yyx);
                {
                  size_t yysz = yysize + yytnamerr (YY_NULL, yytokenName (yyx));
                  yysize_overflow |= yysz < yysize;
                  yysize = yysz;
                }
              }
        }
    }

  switch (yycount)
    {
#define YYCASE_(N, S)                   \
      case N:                           \
        yyformat = S;                   \
      break
      YYCASE_(0, YY_("syntax error"));
      YYCASE_(1, YY_("syntax error, unexpected %s"));
      YYCASE_(2, YY_("syntax error, unexpected %s, expecting %s"));
      YYCASE_(3, YY_("syntax error, unexpected %s, expecting %s or %s"));
      YYCASE_(4, YY_("syntax error, unexpected %s, expecting %s or %s or %s"));
      YYCASE_(5, YY_("syntax error, unexpected %s, expecting %s or %s or %s or %s"));
#undef YYCASE_
    }

  {
    size_t yysz = yysize + strlen (yyformat);
    yysize_overflow |= yysz < yysize;
    yysize = yysz;
  }

  if (!yysize_overflow)
    yymsg = (char *) YYMALLOC (yysize);

  if (yymsg)
    {
      char *yyp = yymsg;
      int yyi = 0;
      while ((*yyp = *yyformat))
        {
          if (*yyp == '%' && yyformat[1] == 's' && yyi < yycount)
            {
              yyp += yytnamerr (yyp, yyarg[yyi++]);
              yyformat += 2;
            }
          else
            {
              yyp++;
              yyformat++;
            }
        }
      yyerror (result, yymsg);
      YYFREE (yymsg);
    }
  else
    {
      yyerror (result, YY_("syntax error"));
      yyMemoryExhausted (yystackp);
    }
  }
#endif /* YYERROR_VERBOSE */
  yynerrs += 1;
}

/* Recover from a syntax error on *YYSTACKP, assuming that *YYSTACKP->YYTOKENP,
   yylval, and yylloc are the syntactic category, semantic value, and location
   of the lookahead.  */
/*ARGSUSED*/ static void
yyrecoverSyntaxError (yyGLRStack* yystackp, struct ParseResult* result)
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
          yyFail (yystackp, result, YY_NULL);
        if (yychar != YYEMPTY)
          {
            yytoken = YYTRANSLATE (yychar);
            yydestruct ("Error: discarding",
                        yytoken, &yylval, result);
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
        if (yypact_value_is_default (yyj))
          return;
        yyj += yytoken;
        if (yyj < 0 || YYLAST < yyj || yycheck[yyj] != yytoken)
          {
            if (yydefact[yystackp->yytops.yystates[0]->yylrState] != 0)
              return;
          }
        else if (! yytable_value_is_error (yytable[yyj]))
          return;
      }

  /* Reduce to one stack.  */
  for (yyk = 0; yyk < yystackp->yytops.yysize; yyk += 1)
    if (yystackp->yytops.yystates[yyk] != YY_NULL)
      break;
  if (yyk >= yystackp->yytops.yysize)
    yyFail (yystackp, result, YY_NULL);
  for (yyk += 1; yyk < yystackp->yytops.yysize; yyk += 1)
    yymarkStackDeleted (yystackp, yyk);
  yyremoveDeletes (yystackp);
  yycompressStack (yystackp);

  /* Now pop stack until we find a state that shifts the error token.  */
  yystackp->yyerrState = 3;
  while (yystackp->yytops.yystates[0] != YY_NULL)
    {
      yyGLRState *yys = yystackp->yytops.yystates[0];
      yyj = yypact[yys->yylrState];
      if (! yypact_value_is_default (yyj))
        {
          yyj += YYTERROR;
          if (0 <= yyj && yyj <= YYLAST && yycheck[yyj] == YYTERROR
              && yyisShiftAction (yytable[yyj]))
            {
              /* Shift the error token.  */
              YY_SYMBOL_PRINT ("Shifting", yystos[yytable[yyj]],
                               &yylval, &yyerrloc);
              yyglrShift (yystackp, 0, yytable[yyj],
                          yys->yyposn, &yylval);
              yys = yystackp->yytops.yystates[0];
              break;
            }
        }
      if (yys->yypred != YY_NULL)
        yydestroyGLRState ("Error: popping", yys, result);
      yystackp->yytops.yystates[0] = yys->yypred;
      yystackp->yynextFree -= 1;
      yystackp->yyspaceLeft += 1;
    }
  if (yystackp->yytops.yystates[0] == YY_NULL)
    yyFail (yystackp, result, YY_NULL);
}

#define YYCHK1(YYE)                                                          \
  do {                                                                       \
    switch (YYE) {                                                           \
    case yyok:                                                               \
      break;                                                                 \
    case yyabort:                                                            \
      goto yyabortlab;                                                       \
    case yyaccept:                                                           \
      goto yyacceptlab;                                                      \
    case yyerr:                                                              \
      goto yyuser_error;                                                     \
    default:                                                                 \
      goto yybuglab;                                                         \
    }                                                                        \
  } while (YYID (0))


/*----------.
| yyparse.  |
`----------*/

int
yyparse (struct ParseResult* result)
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
  yyglrShift (&yystack, 0, 0, 0, &yylval);
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

                  yyreportSyntaxError (&yystack, result);
                  goto yyuser_error;
                }
              YYCHK1 (yyglrReduce (&yystack, 0, yyrule, yytrue, result));
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
                  yyglrShift (&yystack, 0, yyaction, yyposn, &yylval);
                  if (0 < yystack.yyerrState)
                    yystack.yyerrState -= 1;
                }
              else if (yyisErrorAction (yyaction))
                {

                  yyreportSyntaxError (&yystack, result);
                  goto yyuser_error;
                }
              else
                YYCHK1 (yyglrReduce (&yystack, 0, -yyaction, yytrue, result));
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
            YYCHK1 (yyprocessOneStack (&yystack, yys, yyposn, result));
          yyremoveDeletes (&yystack);
          if (yystack.yytops.yysize == 0)
            {
              yyundeleteLastStack (&yystack);
              if (yystack.yytops.yysize == 0)
                yyFail (&yystack, result, YY_("syntax error"));
              YYCHK1 (yyresolveStack (&yystack, result));
              YYDPRINTF ((stderr, "Returning to deterministic operation.\n"));

              yyreportSyntaxError (&yystack, result);
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
                          &yylval);
              YYDPRINTF ((stderr, "Stack %lu now in state #%d\n",
                          (unsigned long int) yys,
                          yystack.yytops.yystates[yys]->yylrState));
            }

          if (yystack.yytops.yysize == 1)
            {
              YYCHK1 (yyresolveStack (&yystack, result));
              YYDPRINTF ((stderr, "Returning to deterministic operation.\n"));
              yycompressStack (&yystack);
              break;
            }
        }
      continue;
    yyuser_error:
      yyrecoverSyntaxError (&yystack, result);
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
  yyerror (result, YY_("memory exhausted"));
  yyresult = 2;
  goto yyreturn;

 yyreturn:
  if (yychar != YYEMPTY)
    yydestruct ("Cleanup: discarding lookahead",
                YYTRANSLATE (yychar), &yylval, result);

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
                  if (yys->yypred != YY_NULL)
                      yydestroyGLRState ("Cleanup: popping", yys, result);
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
  if (yyst == YY_NULL)
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

#define YYINDEX(YYX)                                                         \
    ((YYX) == YY_NULL ? -1 : (yyGLRStackItem*) (YYX) - yystackp->yyitems)


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
/* Line 2575 of glr.c  */
#line 1102 "sql.ypp"


void emit(char *s, ...)
{
 	/* 
 	extern int yylineno;
	va_list ap;
  	va_start(ap, s);
  	printf("rpn: ");
  	vfprintf(stdout, s, ap);
  	printf("\n");
  	*/
}

void yyerror(struct ParseResult *pp,const char *  s, ...)
{
	/*  
	va_list ap;
	va_start(ap, s);	
	fprintf(stderr, "%d: error: ", yyget_lineno(pp->yyscan_info_));
	vfprintf(stderr, s, ap);
	fprintf(stderr, "\n");
	*/
	fprintf (stderr, "%s\n", s);
}

