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
#include <boost/algorithm/string.hpp>
#include <sstream>
using namespace std;
#include "../ast_node/ast_node.h"
#include "../ast_node/ast_select_stmt.h"
#include "../ast_node/ast_expr_node.h"
#include "../ast_node/ast_create_stmt.h"
#include "../ast_node/ast_drop_stmt.h"
#include "../ast_node/ast_insert_stmt.h"
#include "../ast_node/ast_load_stmt.h"
#include "../ast_node/ast_show_stmt.h"
#include "../ast_node/ast_delete_stmt.h"

void yyerror(struct ParseResult *pp, const char *s, ...);
void emit(char *s, ...);

/* Line 207 of glr.c  */
#line 80 "sql.tab.cpp"

#ifndef YY_NULL
#if defined __cplusplus && 201103L <= __cplusplus
#define YY_NULL nullptr
#else
#define YY_NULL 0
#endif
#endif

#include "sql.tab.hpp"

/* Enabling verbose error messages.  */
#ifdef YYERROR_VERBOSE
#undef YYERROR_VERBOSE
#define YYERROR_VERBOSE 1
#else
#define YYERROR_VERBOSE 0
#endif

/* Default (constant) value used for initialization for null
   right-hand sides.  Unlike the standard yacc.c template, here we set
   the default value of $$ to a zeroed-out value.  Since the default
   value is undefined, this behavior is technically correct.  */
static YYSTYPE yyval_default;

/* Copy the second part of user declarations.  */
/* Line 230 of glr.c  */
#line 41 "sql.ypp"

#include "sql.lex.h"

#define YYLEX_PARAM result->yyscan_info_

/*
should replace YYLEX with the following clause in sql.tab.cpp, why so? I don't
know

#ifdef YYLEX_PARAM
# define YYLEX yylex (&yylval, YYLEX_PARAM)
#else
# define YYLEX yylex (&yylval)
#endif
*/

/* Line 230 of glr.c  */
#line 126 "sql.tab.cpp"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#ifndef YY_
#if defined YYENABLE_NLS && YYENABLE_NLS
#if ENABLE_NLS
#include <libintl.h> /* INFRINGES ON USER NAME SPACE */
#define YY_(Msgid) dgettext("bison-runtime", Msgid)
#endif
#endif
#ifndef YY_
#define YY_(Msgid) Msgid
#endif
#endif

/* Suppress unused-variable warnings by "using" E.  */
#if !defined lint || defined __GNUC__
#define YYUSE(E) ((void)(E))
#else
#define YYUSE(E) /* empty */
#endif

/* Identity function, used to suppress warnings about constant conditions.  */
#ifndef lint
#define YYID(N) (N)
#else
#if (defined __STDC__ || defined __C99__FUNC__ || defined __cplusplus || \
     defined _MSC_VER)
static int YYID(int i)
#else
static int YYID(i) int i;
#endif
{
  return i;
}
#endif

#ifndef YYFREE
#define YYFREE free
#endif
#ifndef YYMALLOC
#define YYMALLOC malloc
#endif
#ifndef YYREALLOC
#define YYREALLOC realloc
#endif

#define YYSIZEMAX ((size_t)-1)

#ifdef __cplusplus
typedef bool yybool;
#else
typedef unsigned char yybool;
#endif
#define yytrue 1
#define yyfalse 0

#ifndef YYSETJMP
#include <setjmp.h>
#define YYJMP_BUF jmp_buf
#define YYSETJMP(Env) setjmp(Env)
/* Pacify clang.  */
#define YYLONGJMP(Env, Val) (longjmp(Env, Val), YYASSERT(0))
#endif

/*-----------------.
| GCC extensions.  |
`-----------------*/

#ifndef __attribute__
/* This feature is available in gcc versions 2.5 and later.  */
#if (!defined __GNUC__ || __GNUC__ < 2 || (__GNUC__ == 2 && __GNUC_MINOR__ < 5))
#define __attribute__(Spec) /* empty */
#endif
#endif

#ifndef YYASSERT
#define YYASSERT(Condition) ((void)((Condition) || (abort(), 0)))
#endif

/* YYFINAL -- State number of the termination state.  */
#define YYFINAL 92
/* YYLAST -- Last index in YYTABLE.  */
#define YYLAST 2389

/* YYNTOKENS -- Number of terminals.  */
#define YYNTOKENS 342
/* YYNNTS -- Number of nonterminals.  */
#define YYNNTS 85
/* YYNRULES -- Number of rules.  */
#define YYNRULES 371
/* YYNRULES -- Number of states.  */
#define YYNSTATES 782
/* YYMAXRHS -- Maximum number of symbols on right-hand side of rule.  */
#define YYMAXRHS 13
/* YYMAXLEFT -- Maximum number of symbols to the left of a handle
   accessed by $0, $-1, etc., in any rule.  */
#define YYMAXLEFT 0

/* YYTRANSLATE(X) -- Bison symbol number corresponding to X.  */
#define YYUNDEFTOK 2
#define YYMAXUTOK 582

#define YYTRANSLATE(YYX) \
  ((unsigned int)(YYX) <= YYMAXUTOK ? yytranslate[YYX] : YYUNDEFTOK)

/* YYTRANSLATE[YYLEX] -- Bison symbol number corresponding to YYLEX.  */
static const unsigned short int yytranslate[] = {
    0,   2,   2,   2,   2,   2,   2,   2,   2,   2,   2,   2,   2,   2,   2,
    2,   2,   2,   2,   2,   2,   2,   2,   2,   2,   2,   2,   2,   2,   2,
    2,   2,   2,   17,  2,   2,   2,   28,  22,  2,   340, 341, 26,  24,  338,
    25,  339, 27,  2,   2,   2,   2,   2,   2,   2,   2,   2,   2,   2,   337,
    2,   2,   2,   2,   2,   2,   2,   2,   2,   2,   2,   2,   2,   2,   2,
    2,   2,   2,   2,   2,   2,   2,   2,   2,   2,   2,   2,   2,   2,   2,
    2,   2,   2,   2,   30,  2,   2,   2,   2,   2,   2,   2,   2,   2,   2,
    2,   2,   2,   2,   2,   2,   2,   2,   2,   2,   2,   2,   2,   2,   2,
    2,   2,   2,   2,   21,  2,   2,   2,   2,   2,   2,   2,   2,   2,   2,
    2,   2,   2,   2,   2,   2,   2,   2,   2,   2,   2,   2,   2,   2,   2,
    2,   2,   2,   2,   2,   2,   2,   2,   2,   2,   2,   2,   2,   2,   2,
    2,   2,   2,   2,   2,   2,   2,   2,   2,   2,   2,   2,   2,   2,   2,
    2,   2,   2,   2,   2,   2,   2,   2,   2,   2,   2,   2,   2,   2,   2,
    2,   2,   2,   2,   2,   2,   2,   2,   2,   2,   2,   2,   2,   2,   2,
    2,   2,   2,   2,   2,   2,   2,   2,   2,   2,   2,   2,   2,   2,   2,
    2,   2,   2,   2,   2,   2,   2,   2,   2,   2,   2,   2,   2,   2,   2,
    2,   2,   2,   2,   2,   2,   2,   2,   2,   2,   2,   2,   2,   2,   2,
    2,   1,   2,   3,   4,   5,   6,   7,   8,   9,   10,  11,  12,  13,  14,
    15,  16,  18,  19,  20,  23,  29,  31,  32,  33,  34,  35,  36,  37,  38,
    39,  40,  41,  42,  43,  44,  45,  46,  47,  48,  49,  50,  51,  52,  53,
    54,  55,  56,  57,  58,  59,  60,  61,  62,  63,  64,  65,  66,  67,  68,
    69,  70,  71,  72,  73,  74,  75,  76,  77,  78,  79,  80,  81,  82,  83,
    84,  85,  86,  87,  88,  89,  90,  91,  92,  93,  94,  95,  96,  97,  98,
    99,  100, 101, 102, 103, 104, 105, 106, 107, 108, 109, 110, 111, 112, 113,
    114, 115, 116, 117, 118, 119, 120, 121, 122, 123, 124, 125, 126, 127, 128,
    129, 130, 131, 132, 133, 134, 135, 136, 137, 138, 139, 140, 141, 142, 143,
    144, 145, 146, 147, 148, 149, 150, 151, 152, 153, 154, 155, 156, 157, 158,
    159, 160, 161, 162, 163, 164, 165, 166, 167, 168, 169, 170, 171, 172, 173,
    174, 175, 176, 177, 178, 179, 180, 181, 182, 183, 184, 185, 186, 187, 188,
    189, 190, 191, 192, 193, 194, 195, 196, 197, 198, 199, 200, 201, 202, 203,
    204, 205, 206, 207, 208, 209, 210, 211, 212, 213, 214, 215, 216, 217, 218,
    219, 220, 221, 222, 223, 224, 225, 226, 227, 228, 229, 230, 231, 232, 233,
    234, 235, 236, 237, 238, 239, 240, 241, 242, 243, 244, 245, 246, 247, 248,
    249, 250, 251, 252, 253, 254, 255, 256, 257, 258, 259, 260, 261, 262, 263,
    264, 265, 266, 267, 268, 269, 270, 271, 272, 273, 274, 275, 276, 277, 278,
    279, 280, 281, 282, 283, 284, 285, 286, 287, 288, 289, 290, 291, 292, 293,
    294, 295, 296, 297, 298, 299, 300, 301, 302, 303, 304, 305, 306, 307, 308,
    309, 310, 311, 312, 313, 314, 315, 316, 317, 318, 319, 320, 321, 322, 323,
    324, 325, 326, 327, 328, 329, 330, 331, 332, 333, 334, 335, 336};

#if YYDEBUG
/* YYPRHS[YYN] -- Index of the first RHS symbol of rule number YYN in
   YYRHS.  */
static const unsigned short int yyprhs[] = {
    0,    0,    3,    6,    10,   13,   17,   19,   23,   35,   36,   39,
    40,   45,   47,   51,   52,   54,   56,   57,   60,   61,   64,   65,
    69,   72,   77,   78,   81,   86,   87,   90,   92,   96,   97,   100,
    103,  106,  109,  112,  115,  118,  121,  123,  127,  129,  132,  134,
    138,  140,  142,  145,  150,  154,  158,  159,  161,  164,  166,  167,
    173,  177,  183,  190,  196,  197,  199,  201,  202,  204,  206,  208,
    211,  214,  215,  216,  218,  221,  226,  230,  232,  234,  238,  242,
    244,  246,  248,  250,  254,  258,  262,  266,  270,  274,  277,  280,
    284,  288,  292,  296,  302,  309,  316,  323,  327,  331,  335,  339,
    342,  345,  349,  353,  357,  362,  366,  371,  377,  379,  383,  387,
    389,  393,  399,  407,  414,  423,  429,  437,  444,  453,  458,  464,
    469,  474,  479,  484,  489,  494,  501,  508,  517,  526,  531,  539,
    544,  551,  556,  558,  560,  562,  563,  565,  567,  569,  571,  573,
    580,  587,  591,  595,  599,  603,  607,  611,  615,  619,  623,  628,
    635,  639,  645,  650,  656,  660,  665,  669,  674,  676,  681,  686,
    687,  691,  693,  702,  713,  723,  730,  742,  751,  755,  756,  758,
    760,  761,  763,  765,  769,  773,  779,  784,  789,  795,  801,  802,
    806,  809,  813,  817,  821,  825,  828,  832,  836,  839,  843,  849,
    852,  856,  860,  864,  868,  872,  876,  880,  884,  888,  892,  894,
    896,  898,  900,  902,  906,  910,  913,  918,  920,  922,  924,  926,
    930,  934,  938,  942,  948,  954,  956,  960,  961,  965,  971,  972,
    974,  975,  978,  981,  982,  987,  991,  993,  1004, 1018, 1020, 1031,
    1032, 1034, 1036, 1038, 1039, 1042, 1044, 1046, 1050, 1056, 1058, 1071,
    1081, 1091, 1104, 1106, 1112, 1114, 1119, 1124, 1125, 1128, 1130, 1137,
    1139, 1143, 1147, 1153, 1154, 1156, 1158, 1160, 1169, 1170, 1176, 1177,
    1180, 1183, 1186, 1189, 1191, 1192, 1193, 1197, 1201, 1207, 1209, 1211,
    1215, 1219, 1227, 1235, 1239, 1243, 1249, 1255, 1257, 1263, 1267, 1275,
    1280, 1285, 1290, 1294, 1298, 1303, 1308, 1312, 1316, 1320, 1325, 1331,
    1335, 1339, 1345, 1348, 1352, 1357, 1363, 1368, 1373, 1378, 1382, 1386,
    1387, 1389, 1390, 1393, 1394, 1397, 1398, 1401, 1402, 1404, 1405, 1407,
    1409, 1411, 1419, 1427, 1434, 1437, 1440, 1443, 1444, 1447, 1452, 1453};

/* YYRHS -- A `-1'-separated list of the rules' RHS.  */
static const short int yyrhs[] = {
    343, 0,   -1,  344, 337, -1,  343, 344, 337, -1,  1,   337, -1,  343, 1,
    337, -1,  345, -1,  193, 357, 358, -1,  193, 357, 358, 107, 360, 346, 347,
    351, 352, 354, 355, -1,  -1,  243, 373, -1,  -1,  110, 48,  348, 350, -1,
    373, -1,  373, 338, 348, -1,  -1,  40,  -1,  83,  -1,  -1,  245, 188, -1,
    -1,  111, 373, -1,  -1,  163, 48,  353, -1,  373, 349, -1,  373, 349, 338,
    353, -1,  -1,  136, 373, -1,  136, 373, 338, 373, -1,  -1,  127, 356, -1,
    3,   -1,  3,   338, 356, -1,  -1,  357, 33,  -1,  357, 86,  -1,  357, 87,
    -1,  357, 112, -1,  357, 212, -1,  357, 209, -1,  357, 207, -1,  357, 208,
    -1,  359, -1,  359, 338, 358, -1,  26,  -1,  373, 364, -1,  361, -1,  361,
    338, 360, -1,  362, -1,  365, -1,  3,   364, -1,  3,   339, 3,   364, -1,
    372, 363, 3,   -1,  340, 360, 341, -1,  -1,  39,  -1,  39,  3,   -1,  3,
    -1,  -1,  361, 366, 129, 362, 370, -1,  361, 212, 362, -1,  361, 212, 362,
    158, 373, -1,  361, 368, 367, 129, 362, 371, -1,  361, 154, 369, 129, 362,
    -1,  -1,  120, -1,  63,  -1,  -1,  165, -1,  135, -1,  187, -1,  135, 367,
    -1,  187, 367, -1,  -1,  -1,  371, -1,  158, 373, -1,  234, 340, 356, 341,
    -1,  340, 345, 341, -1,  3,   -1,  8,   -1,  3,   339, 3,   -1,  3,   339,
    26,  -1,  4,   -1,  5,   -1,  7,   -1,  6,   -1,  373, 24,  373, -1,  373,
    25,  373, -1,  373, 26,  373, -1,  373, 27,  373, -1,  373, 29,  373, -1,
    373, 28,  373, -1,  25,  373, -1,  24,  373, -1,  373, 12,  373, -1,  373,
    10,  373, -1,  373, 11,  373, -1,  373, 20,  373, -1,  373, 20,  340, 345,
    341, -1,  373, 20,  37,  340, 345, 341, -1,  373, 20,  199, 340, 345, 341,
    -1,  373, 20,  33,  340, 345, 341, -1,  373, 21,  373, -1,  373, 22,  373,
    -1,  373, 30,  373, -1,  373, 23,  373, -1,  17,  373, -1,  18,  373, -1,
    8,   9,   373, -1,  340, 373, 341, -1,  373, 15,  156, -1,  373, 15,  18,
    156, -1,  373, 15,  6,   -1,  373, 15,  18,  6,   -1,  373, 19,  373, 36,
    373, -1,  373, -1,  373, 338, 374, -1,  340, 374, 341, -1,  374, -1,  374,
    338, 375, -1,  373, 16,  340, 374, 341, -1,  340, 374, 341, 16,  340, 375,
    341, -1,  373, 18,  16,  340, 374, 341, -1,  340, 374, 341, 18,  16,  340,
    374, 341, -1,  373, 16,  340, 345, 341, -1,  340, 374, 341, 16,  340, 345,
    341, -1,  373, 18,  16,  340, 345, 341, -1,  340, 374, 341, 18,  16,  340,
    345, 341, -1,  99,  340, 345, 341, -1,  18,  99,  340, 345, 341, -1,  328,
    340, 26,  341, -1,  328, 340, 373, 341, -1,  333, 340, 373, 341, -1,  334,
    340, 373, 341, -1,  335, 340, 373, 341, -1,  336, 340, 373, 341, -1,  324,
    340, 373, 338, 373, 341, -1,  324, 340, 373, 107, 373, 341, -1,  324, 340,
    373, 338, 373, 338, 373, 341, -1,  324, 340, 373, 107, 373, 104, 373, 341,
    -1,  325, 340, 373, 341, -1,  325, 340, 376, 373, 107, 373, 341, -1,  329,
    340, 373, 341, -1,  330, 340, 373, 39,  377, 341, -1,  331, 340, 374, 341,
    -1,  133, -1,  224, -1,  47,  -1,  -1,  125, -1,  4,   -1,  89,  -1,  103,
    -1,  53,  -1,  326, 340, 373, 338, 378, 341, -1,  327, 340, 373, 338, 378,
    341, -1,  126, 373, 73,  -1,  126, 373, 75,  -1,  126, 373, 76,  -1,  126,
    373, 77,  -1,  126, 373, 74,  -1,  126, 373, 248, -1,  126, 373, 247, -1,
    126, 373, 250, -1,  126, 373, 174, -1,  51,  373, 379, 96,  -1,  51,  373,
    379, 93,  373, 96,  -1,  51,  379, 96,  -1,  51,  379, 93,  373, 96,  -1,
    242, 373, 217, 373, -1,  379, 242, 373, 217, 373, -1,  373, 14,  373, -1,
    373, 18,  14,  373, -1,  373, 13,  373, -1,  373, 18,  13,  373, -1,  380,
    -1,  62,  69,  381, 3,   -1,  62,  190, 381, 3,   -1,  -1,  116, 18,  99,
    -1,  382, -1,  62,  385, 213, 381, 3,   340, 386, 341, -1,  62,  385, 213,
    381, 3,   339, 3,   340, 386, 341, -1,  62,  385, 213, 381, 3,   340, 386,
    341, 383, -1,  62,  385, 213, 381, 3,   383, -1,  62,  385, 213, 381, 3,
    339, 3,   340, 386, 341, 383, -1,  62,  385, 213, 381, 3,   339, 3,   383,
    -1,  384, 363, 345, -1,  -1,  117, -1,  182, -1,  -1,  214, -1,  387, -1,
    387, 338, 386, -1,  3,   389, 388, -1,  169, 130, 340, 356, 341, -1,  130,
    340, 356, 341, -1,  118, 340, 356, 341, -1,  108, 118, 340, 356, 341, -1,
    108, 130, 340, 356, 341, -1,  -1,  388, 18,  156, -1,  388, 156, -1,  388,
    80,  4,   -1,  388, 80,  5,   -1,  388, 80,  7,   -1,  388, 80,  6,   -1,
    388, 41,  -1,  388, 228, 130, -1,  388, 169, 130, -1,  388, 130, -1,  388,
    57,  4,   -1,  388, 228, 340, 356, 341, -1,  45,  391, -1,  221, 391, 393,
    -1,  198, 391, 393, -1,  149, 391, 393, -1,  124, 391, 393, -1,  125, 391,
    393, -1,  43,  391, 393, -1,  177, 391, 393, -1,  89,  391, 393, -1,  103,
    391, 393, -1,  78,  391, 393, -1,  71,  -1,  218, -1,  219, -1,  72,  -1,
    247, -1,  53,  391, 394, -1,  240, 391, 394, -1,  44,  391, -1,  239, 340,
    5,   341, -1,  220, -1,  46,  -1,  148, -1,  143, -1,  222, 392, 394, -1,
    215, 392, 394, -1,  150, 392, 394, -1,  144, 392, 394, -1,  97,  340, 390,
    341, 394, -1,  196, 340, 390, 341, 394, -1,  4,   -1,  390, 338, 4,   -1,
    -1,  340, 5,   341, -1,  340, 5,   338, 5,   341, -1,  -1,  44,  -1,  -1,
    393, 230, -1,  393, 249, -1,  -1,  394, 53,  196, 3,   -1,  394, 55,  3,
    -1,  395, -1,  62,  171, 158, 3,   340, 356, 341, 167, 158, 3,   -1,  62,
    171, 158, 3,   340, 356, 341, 157, 20,  5,   167, 158, 3,   -1,  396, -1,
    62,  397, 118, 3,   398, 158, 3,   340, 400, 341, -1,  -1,  228, -1,  108,
    -1,  201, -1,  -1,  234, 399, -1,  319, -1,  320, -1,  3,   391, 349, -1,
    3,   391, 349, 338, 400, -1,  401, -1,  138, 213, 3,   107, 374, 245, 4,
    338, 4,   189, 20,  7,   -1,  138, 213, 3,   107, 374, 245, 4,   338, 4,
    -1,  38,  213, 3,   107, 374, 245, 4,   338, 4,   -1,  38,  213, 3,   107,
    374, 245, 4,   338, 4,   189, 20,  7,   -1,  402, -1,  90,  118, 3,   158,
    3,   -1,  403, -1,  90,  69,  404, 3,   -1,  90,  190, 404, 3,   -1,  -1,
    116, 99,  -1,  405, -1,  90,  385, 213, 404, 406, 407, -1,  3,   -1,  3,
    339, 3,   -1,  406, 338, 3,   -1,  406, 338, 3,   339, 3,   -1,  -1,  184,
    -1,  50,  -1,  408, -1,  123, 410, 411, 3,   412, 238, 413, 409, -1,  -1,
    158, 159, 130, 231, 415, -1,  -1,  410, 146, -1,  410, 81,  -1,  410, 112,
    -1,  410, 117, -1,  127, -1,  -1,  -1,  340, 356, 341, -1,  340, 414, 341,
    -1,  340, 414, 341, 338, 413, -1,  373, -1,  80,  -1,  373, 338, 414, -1,
    80,  338, 414, -1,  123, 410, 411, 3,   196, 415, 409, -1,  123, 410, 411,
    3,   412, 345, 409, -1,  3,   20,  373, -1,  3,   20,  80,  -1,  415, 338,
    3,   20,  373, -1,  415, 338, 3,   20,  80,  -1,  416, -1,  197, 417, 277,
    418, 419, -1,  197, 295, 419, -1,  197, 417, 296, 107, 3,   418, 419, -1,
    197, 62,  69,  3,   -1,  197, 62,  190, 3,   -1,  197, 62,  213, 3,   -1,
    197, 69,  3,   -1,  197, 190, 3,   -1,  197, 297, 3,   298, -1,  197, 297,
    3,   299, -1,  197, 300, 301, -1,  197, 302, 354, -1,  197, 373, 302, -1,
    197, 303, 104, 8,   -1,  197, 118, 107, 3,   418, -1,  197, 304, 299, -1,
    197, 421, 298, -1,  197, 322, 277, 418, 419, -1,  197, 288, -1,  197, 417,
    305, -1,  197, 422, 299, 419, -1,  197, 213, 299, 418, 419, -1,  197, 294,
    196, 419, -1,  197, 306, 418, 420, -1,  197, 422, 307, 419, -1,  197, 308,
    354, -1,  197, 373, 308, -1,  -1,  323, -1,  -1,  107, 3,   -1,  -1,  14,
    4,   -1,  -1,  14,  373, -1,  -1,  321, -1,  -1,  280, -1,  281, -1,  423,
    -1,  82,  424, 107, 360, 346, 352, 354, -1,  82,  424, 107, 425, 234, 360,
    346, -1,  82,  424, 425, 107, 360, 346, -1,  424, 146, -1,  424, 173, -1,
    424, 117, -1,  -1,  3,   426, -1,  425, 338, 3,   426, -1,  -1,  339, 26,
    -1};

/* YYRLINE[YYN] -- source line where rule number YYN was defined.  */
static const unsigned short int yyrline[] = {
    0,    435,  435,  449,  462,  471,  483,  487,  491,  498,  499,  502,
    503,  508,  509,  511,  512,  513,  516,  517,  520,  521,  524,  525,
    528,  529,  531,  532,  533,  536,  537,  541,  542,  545,  546,  547,
    548,  549,  550,  551,  552,  553,  557,  558,  559,  563,  566,  567,
    571,  572,  576,  578,  580,  581,  584,  585,  588,  589,  590,  604,
    605,  606,  607,  608,  611,  612,  613,  616,  617,  621,  622,  626,
    627,  628,  631,  632,  640,  641,  646,  653,  654,  655,  656,  657,
    658,  659,  660,  662,  663,  664,  665,  666,  667,  669,  670,  672,
    673,  674,  676,  677,  678,  679,  680,  682,  683,  684,  685,  687,
    688,  689,  690,  693,  694,  695,  696,  699,  702,  703,  704,  707,
    708,  711,  712,  713,  715,  717,  718,  719,  720,  722,  723,  733,
    734,  735,  736,  737,  738,  742,  743,  744,  745,  746,  747,  748,
    749,  750,  753,  754,  755,  758,  759,  760,  761,  762,  763,  766,
    767,  771,  772,  773,  774,  775,  776,  777,  778,  779,  783,  784,
    785,  786,  789,  790,  793,  794,  797,  798,  804,  808,  809,  812,
    813,  818,  821,  825,  829,  834,  838,  843,  847,  851,  852,  853,
    856,  857,  860,  861,  866,  867,  868,  869,  870,  871,  873,  874,
    875,  876,  877,  878,  879,  880,  881,  882,  883,  884,  885,  889,
    890,  891,  892,  893,  894,  895,  896,  897,  898,  899,  900,  901,
    902,  903,  904,  905,  906,  907,  908,  910,  911,  912,  913,  914,
    915,  916,  917,  918,  919,  921,  922,  925,  926,  927,  930,  931,
    934,  935,  936,  939,  940,  942,  947,  950,  952,  960,  964,  968,
    969,  970,  971,  974,  975,  978,  979,  982,  983,  987,  990,  995,
    996,  997,  1004, 1007, 1011, 1015, 1016, 1019, 1020, 1024, 1027, 1031,
    1032, 1033, 1034, 1037, 1038, 1039, 1045, 1048, 1054, 1055, 1058, 1059,
    1060, 1061, 1062, 1065, 1065, 1068, 1069, 1072, 1073, 1077, 1078, 1079,
    1080, 1083, 1087, 1092, 1095, 1098, 1101, 1106, 1109, 1110, 1111, 1112,
    1113, 1114, 1115, 1116, 1117, 1118, 1119, 1120, 1121, 1122, 1123, 1124,
    1125, 1126, 1127, 1128, 1129, 1130, 1131, 1132, 1133, 1134, 1135, 1137,
    1138, 1141, 1142, 1145, 1146, 1149, 1150, 1152, 1153, 1156, 1157, 1158,
    1161, 1163, 1168, 1172, 1177, 1178, 1179, 1180, 1184, 1185, 1188, 1189};
#endif

#if YYDEBUG || YYERROR_VERBOSE || 0
/* YYTNAME[SYMBOL-NUM] -- String name of the symbol SYMBOL-NUM.
   First, the terminals, then, starting at YYNTOKENS, nonterminals.  */
static const char *const yytname[] = {
    "$end",                     "error",
    "$undefined",               "NAME",
    "STRING",                   "INTNUM",
    "BOOL",                     "APPROXNUM",
    "USERVAR",                  "ASSIGN",
    "OR",                       "XOR",
    "ANDOP",                    "REGEXP",
    "LIKE",                     "IS",
    "IN",                       "'!'",
    "NOT",                      "BETWEEN",
    "COMPARISON",               "'|'",
    "'&'",                      "SHIFT",
    "'+'",                      "'-'",
    "'*'",                      "'/'",
    "'%'",                      "MOD",
    "'^'",                      "UMINUS",
    "ADD",                      "ALL",
    "ALTER",                    "ANALYZE",
    "AND",                      "ANY",
    "APPEND",                   "AS",
    "ASC",                      "AUTO_INCREMENT",
    "BEFORE",                   "BIGINT",
    "BINARY",                   "BIT",
    "BLOB",                     "BOTH",
    "BY",                       "CALL",
    "CASCADE",                  "CASE",
    "CHANGE",                   "CHAR",
    "CHECK",                    "COLLATE",
    "COLUMN",                   "COMMENT",
    "CONDITION",                "CONSTRAINT",
    "CONTINUE",                 "CONVERT",
    "CREATE",                   "CROSS",
    "CURRENT_DATE",             "CURRENT_TIME",
    "CURRENT_TIMESTAMP",        "CURRENT_USER",
    "CURSOR",                   "DATABASE",
    "DATABASES",                "DATE",
    "DATETIME",                 "DAY_HOUR",
    "DAY",                      "DAY_MICROSECOND",
    "DAY_MINUTE",               "DAY_SECOND",
    "DECIMAL",                  "DECLARE",
    "DEFAULT",                  "DELAYED",
    "DELETE",                   "DESC",
    "DESCRIBE",                 "DETERMINISTIC",
    "DISTINCT",                 "DISTINCTROW",
    "DIV",                      "DOUBLE",
    "DROP",                     "DUAL",
    "EACH",                     "ELSE",
    "ELSEIF",                   "ENCLOSED",
    "END",                      "ENUM",
    "ESCAPED",                  "EXISTS",
    "EXIT",                     "EXPLAIN",
    "FETCH",                    "FLOAT",
    "FOR",                      "FORCE",
    "FOREIGN",                  "FROM",
    "FULLTEXT",                 "GRANT",
    "GROUP",                    "HAVING",
    "HIGH_PRIORITY",            "HOUR_MICROSECOND",
    "HOUR_MINUTE",              "HOUR_SECOND",
    "IF",                       "IGNORE",
    "INDEX",                    "INFILE",
    "INNER",                    "INOUT",
    "INSENSITIVE",              "INSERT",
    "INT",                      "INTEGER",
    "INTERVAL",                 "INTO",
    "ITERATE",                  "JOIN",
    "KEY",                      "KEYS",
    "KILL",                     "LEADING",
    "LEAVE",                    "LEFT",
    "LIMIT",                    "LINES",
    "LOAD",                     "LOCALTIME",
    "LOCALTIMESTAMP",           "LOCK",
    "LONG",                     "LONGBLOB",
    "LONGTEXT",                 "LOOP",
    "LOW_PRIORITY",             "MATCH",
    "MEDIUMBLOB",               "MEDIUMINT",
    "MEDIUMTEXT",               "MINUTE_MICROSECOND",
    "MINUTE_SECOND",            "MODIFIES",
    "NATURAL",                  "NO_WRITE_TO_BINLOG",
    "NULLX",                    "NUMBER",
    "ON",                       "DUPLICATE",
    "OPTIMIZE",                 "OPTION",
    "OPTIONALLY",               "ORDER",
    "OUT",                      "OUTER",
    "OUTFILE",                  "PARTITIONED",
    "PRECISION",                "PRIMARY",
    "PROCEDURE",                "PROJECTION",
    "PURGE",                    "QUICK",
    "QUARTER",                  "READ",
    "READS",                    "REAL",
    "REFERENCES",               "RELEASE",
    "RENAME",                   "REPEAT",
    "REPLACE",                  "REQUIRE",
    "RESTRICT",                 "RETURN",
    "REVOKE",                   "RIGHT",
    "ROLLUP",                   "SAMPLE",
    "SCHEMA",                   "SCHEMAS",
    "SECOND_MICROSECOND",       "SELECT",
    "SENSITIVE",                "SEPARATOR",
    "SET",                      "SHOW",
    "SMALLINT",                 "SOME",
    "SONAME",                   "SPATIAL",
    "SPECIFIC",                 "SQL",
    "SQLEXCEPTION",             "SQLSTATE",
    "SQLWARNING",               "SQL_BIG_RESULT",
    "SQL_CALC_FOUND_ROWS",      "SQL_SMALL_RESULT",
    "SSL",                      "STARTING",
    "STRAIGHT_JOIN",            "TABLE",
    "TEMPORARY",                "TEXT",
    "TERMINATED",               "THEN",
    "TIME",                     "TIMESTAMP",
    "TINYBLOB",                 "TINYINT",
    "TINYTEXT",                 "TO",
    "TRAILING",                 "TRIGGER",
    "UNDO",                     "UNION",
    "UNIQUE",                   "UNLOCK",
    "UNSIGNED",                 "UPDATE",
    "USAGE",                    "USE",
    "USING",                    "UTC_DATE",
    "UTC_TIME",                 "UTC_TIMESTAMP",
    "VALUES",                   "VARBINARY",
    "VARCHAR",                  "VARYING",
    "WHEN",                     "WHERE",
    "WHILE",                    "WITH",
    "WRITE",                    "YEAR",
    "YEAR_MONTH",               "ZEROFILL",
    "WEEK",                     "DO",
    "MAX_QUERIES_PER_HOUR",     "MAX_UPDATES_PER_HOUR",
    "MAX_CONNECTIONS_PER_HOUR", "MAX_USER_CONNECTIONS",
    "USER",                     "TRUNCATE",
    "FAST",                     "MEDIUM",
    "EXTENDED",                 "CHANGED",
    "LEAVES",                   "MASTER",
    "QUERY",                    "CACHE",
    "SLAVE",                    "BEGINT",
    "COMMIT",                   "START",
    "TRANSACTION",              "NO",
    "CHAIN",                    "AUTOCOMMIT",
    "SAVEPOINT",                "ROLLBACK",
    "LOCAL",                    "TABLES",
    "ISOLATION",                "LEVEL",
    "GLOBAL",                   "SESSION",
    "UNCOMMITTED",              "COMMITTED",
    "REPEATABLE",               "SERIALIZABLE",
    "IDENTIFIED",               "PASSWORD",
    "PRIVILEGES",               "BACKUP",
    "CHECKSUM",                 "REPAIR",
    "USE_FRM",                  "RESTORE",
    "CHARACTER",                "COLLATION",
    "COLUMNS",                  "ENGINE",
    "LOGS",                     "STATUS",
    "STORAGE",                  "ENGINES",
    "ERRORS",                   "GRANTS",
    "INNODB",                   "PROCESSLIST",
    "TRIGGERS",                 "VARIABLES",
    "WARNINGS",                 "FLUSH",
    "HOSTS",                    "DES_KEY_FILE",
    "USER_RESOURCES",           "CONNECTION",
    "RESET",                    "PREPARE",
    "DEALLOCATE",               "EXECUTE",
    "WORK",                     "BTREE",
    "HASH",                     "BDB",
    "OPEN",                     "FULL",
    "FSUBSTRING",               "FTRIM",
    "FDATE_ADD",                "FDATE_SUB",
    "FCOUNT",                   "FUPPER",
    "FCAST",                    "FCOALESCE",
    "FCONVERT",                 "FSUM",
    "FAVG",                     "FMIN",
    "FMAX",                     "';'",
    "','",                      "'.'",
    "'('",                      "')'",
    "$accept",                  "stmt_list",
    "stmt",                     "select_stmt",
    "opt_where",                "opt_groupby",
    "groupby_list",             "opt_asc_desc",
    "opt_with_rollup",          "opt_having",
    "opt_orderby",              "orderby_list",
    "opt_limit",                "opt_into_list",
    "column_list",              "select_opts",
    "select_expr_list",         "select_expr",
    "table_references",         "table_reference",
    "table_factor",             "opt_as",
    "opt_as_alias",             "join_table",
    "opt_inner_cross",          "opt_outer",
    "left_or_right",            "opt_left_or_right_outer",
    "opt_join_condition",       "join_condition",
    "table_subquery",           "expr",
    "expr_list",                "opt_expr_list",
    "trim_ltb",                 "cast_data_type",
    "interval_exp",             "case_list",
    "create_database_stmt",     "opt_if_not_exists",
    "create_table_stmt",        "create_select_statement",
    "opt_ignore_replace",       "opt_temporary",
    "create_col_list",          "create_definition",
    "column_atts",              "data_type",
    "enum_list",                "opt_length",
    "opt_binary",               "opt_uz",
    "opt_csc",                  "create_projection_stmt",
    "create_index_stmt",        "index_att",
    "opt_using_type",           "index_type",
    "index_col_list",           "load_table_stmt",
    "drop_index_stmt",          "drop_database_stmt",
    "opt_if_exists",            "drop_table_stmt",
    "table_list",               "opt_rc",
    "insert_stmt",              "opt_ondupupdate",
    "insert_opts",              "opt_into",
    "opt_col_names",            "insert_vals_list",
    "insert_vals",              "insert_asgn_list",
    "show_stmt",                "opt_full",
    "opt_from",                 "opt_like_string",
    "opt_like_expr",            "opt_bdb",
    "opt_trans_level",          "delete_stmt",
    "delete_opts",              "delete_list",
    "opt_dot_star",             YY_NULL};
#endif

/* YYR1[YYN] -- Symbol number of symbol that rule YYN derives.  */
static const unsigned short int yyr1[] = {
    0,   342, 343, 343, 343, 343, 344, 345, 345, 346, 346, 347, 347, 348, 348,
    349, 349, 349, 350, 350, 351, 351, 352, 352, 353, 353, 354, 354, 354, 355,
    355, 356, 356, 357, 357, 357, 357, 357, 357, 357, 357, 357, 358, 358, 358,
    359, 360, 360, 361, 361, 362, 362, 362, 362, 363, 363, 364, 364, 364, 365,
    365, 365, 365, 365, 366, 366, 366, 367, 367, 368, 368, 369, 369, 369, 370,
    370, 371, 371, 372, 373, 373, 373, 373, 373, 373, 373, 373, 373, 373, 373,
    373, 373, 373, 373, 373, 373, 373, 373, 373, 373, 373, 373, 373, 373, 373,
    373, 373, 373, 373, 373, 373, 373, 373, 373, 373, 373, 374, 374, 374, 375,
    375, 373, 373, 373, 373, 373, 373, 373, 373, 373, 373, 373, 373, 373, 373,
    373, 373, 373, 373, 373, 373, 373, 373, 373, 373, 373, 376, 376, 376, 377,
    377, 377, 377, 377, 377, 373, 373, 378, 378, 378, 378, 378, 378, 378, 378,
    378, 373, 373, 373, 373, 379, 379, 373, 373, 373, 373, 344, 380, 380, 381,
    381, 344, 382, 382, 382, 382, 382, 382, 383, 384, 384, 384, 385, 385, 386,
    386, 387, 387, 387, 387, 387, 387, 388, 388, 388, 388, 388, 388, 388, 388,
    388, 388, 388, 388, 388, 389, 389, 389, 389, 389, 389, 389, 389, 389, 389,
    389, 389, 389, 389, 389, 389, 389, 389, 389, 389, 389, 389, 389, 389, 389,
    389, 389, 389, 389, 389, 390, 390, 391, 391, 391, 392, 392, 393, 393, 393,
    394, 394, 394, 344, 395, 395, 344, 396, 397, 397, 397, 397, 398, 398, 399,
    399, 400, 400, 344, 401, 401, 401, 401, 344, 402, 344, 403, 403, 404, 404,
    344, 405, 406, 406, 406, 406, 407, 407, 407, 344, 408, 409, 409, 410, 410,
    410, 410, 410, 411, 411, 412, 412, 413, 413, 414, 414, 414, 414, 408, 408,
    415, 415, 415, 415, 344, 416, 416, 416, 416, 416, 416, 416, 416, 416, 416,
    416, 416, 416, 416, 416, 416, 416, 416, 416, 416, 416, 416, 416, 416, 416,
    416, 416, 417, 417, 418, 418, 419, 419, 420, 420, 421, 421, 422, 422, 422,
    344, 423, 423, 423, 424, 424, 424, 424, 425, 425, 426, 426};

/* YYR2[YYN] -- Number of symbols composing right hand side of rule YYN.  */
static const unsigned char yyr2[] = {
    0, 2, 2, 3, 2, 3, 1, 3,  11, 0, 2,  0, 4, 1,  3, 0, 1, 1,  0,  2, 0,  2,
    0, 3, 2, 4, 0, 2, 4, 0,  2,  1, 3,  0, 2, 2,  2, 2, 2, 2,  2,  2, 1,  3,
    1, 2, 1, 3, 1, 1, 2, 4,  3,  3, 0,  1, 2, 1,  0, 5, 3, 5,  6,  5, 0,  1,
    1, 0, 1, 1, 1, 2, 2, 0,  0,  1, 2,  4, 3, 1,  1, 3, 3, 1,  1,  1, 1,  3,
    3, 3, 3, 3, 3, 2, 2, 3,  3,  3, 3,  5, 6, 6,  6, 3, 3, 3,  3,  2, 2,  3,
    3, 3, 4, 3, 4, 5, 1, 3,  3,  1, 3,  5, 7, 6,  8, 5, 7, 6,  8,  4, 5,  4,
    4, 4, 4, 4, 4, 6, 6, 8,  8,  4, 7,  4, 6, 4,  1, 1, 1, 0,  1,  1, 1,  1,
    1, 6, 6, 3, 3, 3, 3, 3,  3,  3, 3,  3, 4, 6,  3, 5, 4, 5,  3,  4, 3,  4,
    1, 4, 4, 0, 3, 1, 8, 10, 9,  6, 11, 8, 3, 0,  1, 1, 0, 1,  1,  3, 3,  5,
    4, 4, 5, 5, 0, 3, 2, 3,  3,  3, 3,  2, 3, 3,  2, 3, 5, 2,  3,  3, 3,  3,
    3, 3, 3, 3, 3, 3, 1, 1,  1,  1, 1,  3, 3, 2,  4, 1, 1, 1,  1,  3, 3,  3,
    3, 5, 5, 1, 3, 0, 3, 5,  0,  1, 0,  2, 2, 0,  4, 3, 1, 10, 13, 1, 10, 0,
    1, 1, 1, 0, 2, 1, 1, 3,  5,  1, 12, 9, 9, 12, 1, 5, 1, 4,  4,  0, 2,  1,
    6, 1, 3, 3, 5, 0, 1, 1,  1,  8, 0,  5, 0, 2,  2, 2, 2, 1,  0,  0, 3,  3,
    5, 1, 1, 3, 3, 7, 7, 3,  3,  5, 5,  1, 5, 3,  7, 4, 4, 4,  3,  3, 4,  4,
    3, 3, 3, 4, 5, 3, 3, 5,  2,  3, 4,  5, 4, 4,  4, 3, 3, 0,  1,  0, 2,  0,
    2, 0, 2, 0, 1, 0, 1, 1,  1,  7, 7,  6, 2, 2,  2, 0, 2, 4,  0,  2};

/* YYDPREC[RULE-NUM] -- Dynamic precedence of rule #RULE-NUM (0 if none).  */
static const unsigned char yydprec[] = {
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 2, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};

/* YYMERGER[RULE-NUM] -- Index of merging function for rule #RULE-NUM.  */
static const unsigned char yymerger[] = {
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};

/* YYDEFACT[S] -- default reduction number in state S.  Performed when
   YYTABLE doesn't specify something else to do.  Zero means the default
   is an error.  */
static const unsigned short int yydefact[] = {
    0,   0,   0,   192, 367, 192, 298, 0,   33,  347, 0,   0,   6,   176, 181,
    258, 261, 273, 278, 280, 285, 294, 319, 360, 4,   0,   179, 265, 0,   179,
    266, 193, 264, 0,   0,   0,   283, 0,   283, 0,   304, 0,   0,   79,  83,
    84,  86,  85,  80,  0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,
    358, 359, 338, 0,   351, 0,   0,   26,  0,   0,   349, 26,  356, 0,   348,
    0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,
    0,   0,   1,   0,   0,   2,   0,   0,   0,   0,   0,   179, 0,   370, 0,
    366, 364, 365, 0,   0,   0,   0,   0,   283, 300, 301, 302, 303, 299, 0,
    0,   44,  34,  35,  36,  37,  40,  41,  39,  38,  7,   42,  58,  0,   0,
    107, 0,   108, 94,  93,  0,   0,   0,   0,   0,   0,   326, 0,   0,   327,
    349, 351, 0,   321, 0,   330, 0,   331, 0,   335, 0,   353, 345, 349, 0,
    0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,
    0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,
    0,   0,   0,   0,   332, 346, 349, 0,   339, 336, 351, 351, 5,   3,   0,
    0,   177, 0,   178, 0,   267, 0,   368, 58,  0,   9,   46,  48,  49,  54,
    0,   0,   0,   284, 281, 0,   282, 0,   305, 0,   0,   0,   57,  0,   45,
    81,  82,  109, 0,   0,   0,   0,   168, 0,   323, 324, 325, 0,   349, 351,
    342, 352, 328, 329, 27,  333, 350, 0,   343, 351, 0,   148, 146, 147, 0,
    0,   0,   0,   0,   0,   0,   0,   116, 0,   0,   0,   0,   0,   0,   0,
    110, 0,   96,  97,  95,  174, 172, 113, 0,   111, 0,   0,   0,   0,   0,
    0,   0,   0,   0,   98,  103, 104, 106, 87,  88,  89,  90,  92,  91,  105,
    351, 0,   340, 344, 0,   180, 0,   189, 0,   0,   371, 0,   50,  58,  0,
    0,   0,   22,  66,  65,  69,  73,  70,  0,   0,   0,   67,  55,  0,   0,
    9,   370, 279, 287, 291, 0,   0,   0,   0,   9,   43,  56,  0,   0,   0,
    166, 0,   0,   129, 334, 341, 0,   354, 337, 0,   0,   141, 0,   0,   0,
    131, 132, 143, 149, 145, 133, 134, 135, 136, 118, 117, 0,   0,   114, 112,
    0,   0,   175, 173, 0,   0,   0,   0,   0,   0,   320, 349, 0,   31,  0,
    190, 191, 0,   0,   185, 54,  269, 270, 268, 0,   58,  0,   78,  53,  10,
    0,   26,  67,  67,  0,   60,  47,  0,   68,  0,   52,  9,   363, 369, 0,
    293, 292, 0,   286, 0,   296, 0,   0,   296, 0,   11,  129, 170, 0,   169,
    0,   28,  0,   0,   0,   0,   0,   0,   151, 154, 152, 153, 150, 0,   0,
    0,   125, 121, 0,   0,   115, 0,   0,   0,   99,  351, 0,   0,   0,   189,
    0,   0,   0,   0,   0,   0,   194, 0,   0,   51,  0,   361, 71,  72,  0,
    0,   74,  0,   362, 288, 289, 0,   0,   0,   313, 306, 0,   296, 314, 0,
    0,   20,  167, 171, 0,   138, 0,   137, 0,   0,   155, 156, 144, 0,   119,
    0,   0,   127, 123, 102, 100, 101, 322, 0,   32,  0,   0,   0,   187, 247,
    247, 247, 236, 247, 226, 229, 247, 247, 0,   247, 247, 247, 238, 250, 237,
    247, 250, 247, 0,   247, 250, 227, 228, 235, 247, 250, 0,   247, 230, 202,
    0,   0,   0,   0,   0,   189, 0,   188, 0,   23,  15,  63,  61,  0,   0,
    59,  75,  0,   0,   316, 315, 0,   0,   310, 309, 0,   295, 0,   0,   0,
    22,  0,   0,   142, 157, 161, 158, 159, 160, 165, 163, 162, 164, 126, 0,
    122, 0,   0,   276, 0,   0,   0,   0,   252, 233, 215, 255, 252, 252, 0,
    252, 252, 252, 251, 255, 252, 255, 252, 0,   252, 255, 252, 255, 0,   255,
    196, 0,   0,   0,   0,   0,   184, 195, 247, 0,   16,  17,  24,  76,  0,
    62,  290, 0,   0,   0,   0,   307, 275, 18,  13,  21,  26,  140, 139, 120,
    128, 124, 0,   0,   259, 189, 0,   221, 231, 225, 223, 245, 0,   224, 219,
    220, 242, 218, 241, 222, 0,   217, 240, 216, 239, 0,   232, 0,   209, 0,
    0,   212, 204, 0,   0,   0,   0,   199, 198, 0,   15,  262, 0,   0,   0,
    318, 317, 312, 311, 0,   0,   0,   12,  0,   29,  0,   0,   186, 0,   248,
    253, 254, 0,   0,   0,   255, 255, 234, 203, 213, 205, 206, 208, 207, 211,
    210, 0,   200, 201, 197, 271, 25,  77,  297, 308, 0,   19,  14,  0,   8,
    277, 0,   0,   0,   257, 246, 243, 244, 0,   0,   274, 30,  260, 249, 256,
    214, 272};

/* YYPDEFGOTO[NTERM-NUM].  */
static const short int yydefgoto[] = {
    -1,  10,  11,  12,  332, 511, 668, 657, 727, 600, 421, 579, 157, 764, 404,
    42,  130, 131, 220, 221, 222, 343, 327, 223, 340, 429, 341, 424, 585, 586,
    224, 277, 178, 525, 270, 463, 456, 142, 13,  98,  14,  409, 410, 33,  485,
    486, 645, 569, 687, 623, 634, 682, 683, 15,  16,  34,  324, 413, 654, 17,
    18,  19,  110, 20,  349, 438, 21,  504, 40,  119, 352, 507, 595, 440, 22,
    89,  161, 153, 263, 90,  91,  23,  35,  108, 217};

/* YYPACT[STATE-NUM] -- Index in YYTABLE of the portion describing
   STATE-NUM.  */
#define YYPACT_NINF -470
static const short int yypact[] = {
    1017, -256, -123, 330,  -470, 33,   -470, -80,  -470, 511,  509,  -198,
    -470, -470, -470, -470, -470, -470, -470, -470, -470, -470, -470, -470,
    -470, 165,  76,   -470, 47,   76,   -470, -470, -470, 2,    101,  83,
    105,  227,  105,  25,   283,  241,  708,  -87,  -470, -470, -470, -470,
    239,  1059, 1088, 1059, 1059, 92,   -37,  247,  -86,  151,  268,  -13,
    -470, -470, -470, 95,   286,  294,  5,    174,  210,  26,   216,  174,
    -470, 50,   -470, -11,  -8,   -7,   13,   29,   34,   38,   42,   44,
    48,   56,   61,   1117, 1793, -213, 70,   -91,  -470, 35,   46,   -470,
    258,  384,  408,  409,  410,  76,   411,  85,   18,   -470, -470, -470,
    -98,  332,  430,  276,  432,  105,  -470, -470, -470, -470, -470, 433,
    333,  -470, -470, -470, -470, -470, -470, -470, -470, -470, 335,  109,
    2254, 79,   1059, 1124, 99,   1124, -470, -470, 1059, 1835, -40,  446,
    447,  450,  -470, 261,  452,  -470, 216,  286,  453,  -470, -88,  -470,
    1059, -470, 448,  -470, 455,  466,  -470, 216,  1059, 12,   1059, 1059,
    873,  1059, 1059, 1117, 1059, 1059, 1059, 1059, 1117, 254,  125,  1059,
    1059, 1059, 1059, 1059, 43,   142,  180,  1059, 850,  1059, 1059, 1059,
    1059, 1059, 1059, 1059, 1059, 1059, 1059, -470, -470, 216,  377,  -470,
    -470, 286,  286,  -470, -470, 1117, 388,  -470, 150,  -470, 488,  259,
    468,  -470, 28,   4,    252,  106,  -470, -470, 457,  -196, 19,   495,
    -470, -470, 496,  -470, 497,  -192, 1117, 19,   901,  -470, 500,  -470,
    -470, -470, 2303, 261,  1856, -39,  1059, -470, 1059, -470, -470, -470,
    163,  216,  286,  -470, -470, -470, -470, 1665, -470, -470, 1059, -470,
    286,  752,  -470, -470, -470, 1448, 1059, 1686, 1707, 167,  1469, 1490,
    2211, 1728, 170,  1511, 1532, 1553, 1574, 172,  1117, -470, 90,   2323,
    1352, 2341, 2359, 2359, -470, 21,   -470, 926,  1059, 1059, 166,  2276,
    181,  187,  190,  926,  592,  615,  719,  836,  379,  379,  503,  503,
    503,  503,  -470, 286,  531,  -470, -470, 292,  -470, 535,  -107, -24,
    381,  -470, 86,   -470, 23,   199,  200,  1059, 380,  -470, -470, -470,
    -83,  -470, 19,   19,   417,  383,  -470, 546,  19,   252,  85,   -470,
    211,  -48,  548,  535,  -120, 307,  252,  -470, -470, 213,  1059, 1059,
    -470, 2146, 1898, -470, -470, -470, 1059, 2303, -470, 1059, 1059, -470,
    1919, 429,  429,  -470, -470, -470, 24,   -470, -470, -470, -470, -470,
    90,   -470, 217,  545,  -470, -470, 222,  223,  2359, 2359, 926,  1059,
    261,  261,  261,  224,  -470, 216,  562,  229,  231,  -470, -470, 565,
    32,   -470, 457,  -470, -470, -470, 567,  52,   572,  -470, -470, 2303,
    529,  174,  383,  383,  454,  421,  -470, 19,   -470, 458,  -470, 252,
    -470, -470, 578,  -470, -470, 579,  -470, 566,  -134, 253,  248,  435,
    593,  486,  449,  2303, 2167, -470, 1059, 2303, 657,  1333, 1059, 1059,
    257,  263,  -470, -470, -470, -470, -470, 264,  926,  260,  -470, -470,
    265,  267,  801,  270,  284,  285,  -470, 286,  271,  535,  -23,  -112,
    1908, -46,  262,  287,  494,  289,  290,  261,  291,  -470, 1059, -470,
    -470, -470, 19,   1059, -133, 19,   -470, -470, 295,  951,  474,  632,
    -470, -470, 979,  435,  -470, 308,  600,  538,  -470, 2303, 1059, -470,
    1059, -470, 1595, 1814, -470, -470, -470, 309,  313,  311,  926,  -470,
    -470, -470, -470, -470, -470, 649,  -470, 637,  501,  32,   -470, 318,
    318,  318,  -470, 318,  -470, -470, 318,  318,  320,  318,  318,  318,
    -470, 618,  -470, 318,  618,  318,  323,  318,  618,  -470, -470, -470,
    318,  618,  324,  318,  -470, -470, 325,  334,  535,  535,  348,  -103,
    32,   -470, 686,  -470, 2188, -470, 2303, 1059, 350,  -470, -470, -108,
    690,  -470, 2303, 568,  675,  362,  1749, 363,  -470, 699,  1059, 1059,
    380,  1616, 1637, -470, -470, -470, -470, -470, -470, -470, -470, -470,
    -470, -470, 1117, -470, 364,  366,  520,  705,  714,  382,  713,  -470,
    -470, -470, -470, -470, -470, 716,  -470, -470, -470, -470, -470, -470,
    -470, -470, 716,  -470, -470, -470, -470, 717,  -470, 274,  535,  535,
    387,  389,  535,  -470, -470, 318,  390,  -470, -470, 391,  2303, 535,
    -470, -470, 490,  1002, 979,  979,  397,  547,  482,  1772, 2303, 174,
    -470, -470, -470, -470, -470, 718,  570,  -470, -94,  -203, -156, 67,
    -156, -156, -470, -121, -156, -156, -156, 67,   -156, 67,   -156, -96,
    -156, 67,   -156, 67,   398,  67,   584,  -470, 746,  386,  -470, -470,
    622,  -118, 412,  413,  -470, -470, 414,  36,   -470, 1059, 415,  548,
    -470, 2303, -470, -470, 248,  738,  581,  -470, 1059, 633,  776,  626,
    -470, 780,  -470, -470, -470, 597,  785,  792,  -470, -470, -470, -470,
    -470, -470, -470, -470, -470, -470, -470, 535,  -470, -470, -470, 451,
    -470, -470, 459,  -470, 791,  -470, -470, 535,  -470, -470, 797,  460,
    799,  -470, -470, 67,   67,   462,  686,  -470, -470, -470, -470, -470,
    -470, -470};

/* YYPGOTO[NTERM-NUM].  */
static const short int yypgoto[] = {
    -470, -470, 794,  -89,  -285, -470, 84,   128,  -470, -470, 249,
    131,  -70,  -470, -348, -470, 580,  -470, -132, -470, -299, 440,
    -126, -470, -470, -119, -470, -470, -470, 282,  -470, -9,   -163,
    238,  -470, -470, 498,  729,  -470, 51,   -470, -434, -470, 866,
    -469, -470, -470, -470, 235,  -170, -347, -311, -385, -470, -470,
    -470, -470, -470, 108,  -470, -470, -470, 10,   -470, -470, -470,
    -470, -392, -470, -470, -470, 160,  -352, 169,  -470, -470, -116,
    -140, -470, -470, -470, -470, -470, 781,  540};

/* YYTABLE[YYPACT[STATE-NUM]].  What to do in state STATE-NUM.  If
   positive, shift that token.  If negative, reduce the rule which
   number is the opposite.  If YYTABLE_NINF, syntax error.  */
#define YYTABLE_NINF -371
static const short int yytable[] = {
    88,   162,  435,  441,  350,  405,  239,  328,  278,  226,  405,  255,
    750,  283,  405,  43,   44,   45,   46,   47,   48,   218,  328,  405,
    502,  583,  237,  388,  458,  49,   50,   237,  143,  132,  254,  480,
    51,   52,   344,  425,  135,  137,  138,  139,  141,  538,  319,  264,
    112,  292,  583,  508,  422,  246,  359,  237,  247,  360,  252,  266,
    432,  293,  238,  53,   201,  317,  318,  238,  621,  445,  406,  353,
    570,  8,    735,  406,  655,  459,  177,  406,  100,  24,   240,  202,
    571,  315,  103,  330,  406,  415,  25,   238,  203,  736,  345,  43,
    44,   45,   46,   47,   48,   584,  36,   354,  423,  241,  386,  652,
    387,  49,   50,   56,   325,  460,  365,  596,  51,   52,   442,  656,
    737,  385,  738,  232,  368,  242,  584,  461,  496,  534,  329,  244,
    391,  41,   535,  733,  436,  364,  734,  95,   481,  651,  227,  53,
    536,  267,  498,  259,  351,  462,  482,  37,   214,  144,  357,  265,
    269,  271,  272,  274,  275,  276,  483,  279,  280,  281,  282,  177,
    96,   333,  287,  288,  289,  290,  291,  400,  145,  389,  299,  304,
    305,  306,  307,  308,  309,  310,  311,  312,  313,  314,  104,  56,
    97,   296,  297,  581,  298,  8,    587,  294,  105,  484,  248,  248,
    503,  99,   390,  426,  205,  636,  257,  258,  431,  640,  399,  101,
    206,  739,  642,  102,  740,  109,  751,  38,   648,  649,  334,  132,
    537,  106,  111,  469,  407,  408,  -182, -64,  268,  361,  113,  362,
    227,  335,  739,  -183, 120,  741,  732,  31,   134,  691,  146,  693,
    133,  367,  147,  697,  107,  699,  148,  701,  336,  372,  -370, 443,
    179,  180,  181,  182,  183,  184,  185,  149,  186,  187,  188,  189,
    190,  191,  192,  193,  194,  195,  196,  197,  198,  475,  150,  392,
    393,  489,  437,  151,  702,  337,  177,  411,  412,  154,  710,  711,
    152,  524,  714,  492,  493,  468,  155,  471,  472,  473,  156,  718,
    722,  723,  158,  703,  684,  685,  338,  688,  689,  690,  419,  160,
    692,  159,  694,  163,  696,  164,  698,  704,  165,  166,  140,  532,
    75,   76,   77,   78,   79,   80,   81,   82,   219,  83,   84,   85,
    86,   447,  448,  491,  87,   167,  705,  771,  772,  451,  219,  219,
    452,  453,  416,  617,  114,  209,  -58,  326,  204,  168,  624,  625,
    207,  626,  169,  523,  627,  628,  170,  630,  631,  632,  171,  208,
    172,  635,  470,  637,  173,  639,  745,  746,  747,  748,  641,  115,
    174,  644,  577,  26,   116,  175,  210,  773,  706,  194,  195,  196,
    197,  198,  117,  211,  212,  213,  215,  776,  75,   76,   77,   78,
    79,   80,   81,   82,   216,  83,   84,   85,   86,   118,  707,  228,
    87,   229,  230,  231,  233,  616,  27,   243,  234,  513,  235,  708,
    339,  518,  519,  236,  -263, 249,  250,  524,  -129, 251,  8,    253,
    260,  256,  261,  -129, -129, -129, -129, -129, -129, -129, 286,  -129,
    -129, -129, -129, -129, -129, -129, -129, -129, -129, -129, -129, -129,
    262,  580,  295,  715,  316,  -129, 582,  320,  -129, -129, 321,  322,
    590,  323,  325,  331,  342,  594,  346,  347,  348,  28,   709,  356,
    363,  601,  394,  602,  375,  92,   93,   379,  -129, 384,  43,   44,
    45,   46,   47,   48,   29,   396,  -129, -129, -129, -129, -129, 397,
    49,   50,   398,  30,   -129, 198,  401,  51,   52,   402,  403,  414,
    417,  418,  -129, 420,  31,   -129, 427,  2,    428,  430,  434,  439,
    444,  -129, 446,  455,  -129, 464,  32,   -129, -129, 465,  53,   466,
    467,  474,  476,  477,  479,  -129, 488,  3,    478,  54,   658,  415,
    -129, 490,  -129, 495,  55,   499,  500,  494,  -129, -129, 501,  497,
    506,  669,  670,  4,    284,  502,  505,  285,  510,  509,  520,  5,
    526,  729,  572,  -129, 521,  522,  527,  -129, 528,  533,  56,   529,
    -129, 189,  190,  191,  192,  193,  194,  195,  196,  197,  198,  -129,
    574,  530,  531,  573,  576,  57,   575,  578,  6,    591,  588,  592,
    -129, 190,  191,  192,  193,  194,  195,  196,  197,  198,  597,  7,
    598,  599,  613,  614,  615,  618,  721,  594,  594,  619,  622,  620,
    629,  -129, 633,  638,  643,  646,  -129, 179,  180,  181,  182,  183,
    184,  185,  647,  186,  187,  188,  189,  190,  191,  192,  193,  194,
    195,  196,  197,  198,  650,  653,  659,  -129, -129, 661,  -129, 663,
    -129, -129, 662,  -129, 664,  58,   8,    667,  666,  675,  9,    676,
    580,  677,  678,  43,   44,   45,   46,   47,   48,   679,  681,  669,
    686,  719,  700,  680,  59,   49,   50,   726,  712,  717,  713,  716,
    51,   52,   121,  724,  725,  731,  730,  742,  743,  122,  191,  192,
    193,  194,  195,  196,  197,  198,  744,  -129, 749,  752,  753,  754,
    757,  -129, 760,  53,   763,  514,  179,  180,  181,  182,  183,  184,
    185,  761,  186,  187,  188,  189,  190,  191,  192,  193,  194,  195,
    196,  197,  198,  765,  766,  767,  -129, -129, 769,  774,  -129, 60,
    61,   768,  123,  124,  770,  503,  775,  62,   777,  778,  779,  780,
    94,   63,   64,   56,   65,   -355, -357, 66,   762,  67,   68,   69,
    355,  70,   -357, 71,   125,  188,  189,  190,  191,  192,  193,  194,
    195,  196,  197,  198,  72,   73,   74,   75,   76,   77,   78,   79,
    80,   81,   82,   755,  83,   84,   85,   86,   756,  671,  487,  87,
    674,  43,   44,   45,   46,   47,   48,   369,  192,  193,  194,  195,
    196,  197,  198,  49,   50,   660,  245,  39,   457,  695,  51,   52,
    43,   44,   45,   46,   47,   48,   781,  300,  759,  225,  433,  301,
    758,  0,    49,   50,   0,    0,    0,    0,    0,    51,   52,   273,
    0,    53,   0,    0,    43,   44,   45,   46,   47,   48,   0,    0,
    0,    0,    0,    126,  127,  128,  49,   50,   129,  0,    0,    0,
    53,   51,   52,   121,  0,    43,   44,   45,   46,   47,   48,   0,
    0,    0,    0,    0,    0,    0,    0,    49,   50,   0,    0,    0,
    0,    56,   51,   52,   53,   0,    43,   44,   45,   46,   47,   48,
    0,    0,    0,    0,    0,    0,    0,    0,    49,   50,   0,    0,
    56,   0,    0,    51,   52,   53,   0,    0,    0,    0,    43,   44,
    45,   46,   47,   48,   0,    0,    0,    0,    0,    0,    0,    0,
    49,   50,   515,  0,    56,   0,    53,   51,   52,   43,   44,   45,
    46,   47,   48,   0,    0,    0,    0,    0,    0,    0,    1,    49,
    50,   0,    0,    0,    0,    56,   51,   52,   0,    0,    53,   589,
    75,   76,   77,   78,   79,   80,   81,   82,   0,    83,   84,   85,
    86,   0,    0,    0,    87,   302,  56,   0,    0,    53,   0,    2,
    0,    0,    0,    593,  0,    0,    43,   44,   45,   46,   47,   48,
    0,    0,    0,    0,    0,    0,    0,    0,    49,   50,   56,   3,
    0,    0,    720,  51,   52,   0,    0,    0,    0,    0,    370,  43,
    44,   45,   46,   47,   48,   0,    0,    4,    0,    56,   0,    0,
    0,    49,   50,   5,    0,    0,    53,   0,    51,   52,   0,    0,
    0,    0,    0,    8,    43,   44,   45,   46,   47,   48,   0,    0,
    0,    0,    0,    0,    0,    0,    49,   50,   0,    0,    0,    53,
    6,    51,   52,   187,  188,  189,  190,  191,  192,  193,  194,  195,
    196,  197,  198,  7,    0,    0,    56,   0,    0,    0,    0,    0,
    0,    0,    0,    0,    53,   0,    0,    0,    0,    0,    75,   76,
    77,   78,   79,   80,   81,   82,   0,    83,   84,   85,   86,   136,
    0,    0,    303,  0,    0,    0,    0,    0,    0,    75,   76,   77,
    78,   79,   80,   81,   82,   0,    83,   84,   85,   86,   8,    0,
    0,    87,   9,    0,    56,   0,    0,    0,    0,    0,    0,    0,
    0,    75,   76,   77,   78,   79,   80,   81,   82,   0,    83,   84,
    85,   86,   0,    0,    0,    87,   0,    0,    0,    0,    0,    0,
    0,    0,    75,   76,   77,   78,   79,   80,   81,   82,   0,    83,
    84,   85,   86,   0,    0,    0,    176,  0,    0,    0,    0,    0,
    0,    0,    0,    75,   76,   77,   78,   79,   80,   81,   82,   0,
    83,   84,   85,   86,   0,    0,    0,    87,   0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    75,   76,   77,   78,   79,
    80,   81,   82,   0,    83,   84,   85,   86,   0,    0,    0,    87,
    0,    0,    0,    0,    0,    0,    75,   76,   77,   78,   79,   80,
    81,   82,   0,    83,   84,   85,   86,   0,    0,    0,    87,   179,
    180,  181,  182,  183,  184,  185,  0,    186,  187,  188,  189,  190,
    191,  192,  193,  194,  195,  196,  197,  198,  181,  182,  183,  184,
    185,  0,    186,  187,  188,  189,  190,  191,  192,  193,  194,  195,
    196,  197,  198,  75,   76,   77,   78,   79,   80,   81,   82,   0,
    83,   84,   85,   86,   0,    0,    0,    87,   0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    75,   76,   77,   78,
    79,   80,   81,   82,   0,    83,   84,   85,   86,   0,    0,    0,
    87,   0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    75,   76,   77,   78,   79,   80,   81,   82,   0,    83,   84,
    85,   86,   0,    0,    0,    176,  179,  180,  181,  182,  183,  184,
    185,  0,    186,  187,  188,  189,  190,  191,  192,  193,  194,  195,
    196,  197,  198,  179,  180,  181,  182,  183,  184,  185,  0,    186,
    187,  188,  189,  190,  191,  192,  193,  194,  195,  196,  197,  198,
    179,  180,  181,  182,  183,  184,  185,  0,    186,  187,  188,  189,
    190,  191,  192,  193,  194,  195,  196,  197,  198,  179,  180,  181,
    182,  183,  184,  185,  0,    186,  187,  188,  189,  190,  191,  192,
    193,  194,  195,  196,  197,  198,  179,  180,  181,  182,  183,  184,
    185,  0,    186,  187,  188,  189,  190,  191,  192,  193,  194,  195,
    196,  197,  198,  179,  180,  181,  182,  183,  184,  185,  0,    186,
    187,  188,  189,  190,  191,  192,  193,  194,  195,  196,  197,  198,
    179,  180,  181,  182,  183,  184,  185,  0,    186,  187,  188,  189,
    190,  191,  192,  193,  194,  195,  196,  197,  198,  179,  180,  181,
    182,  183,  184,  185,  0,    186,  187,  188,  189,  190,  191,  192,
    193,  194,  195,  196,  197,  198,  179,  180,  181,  182,  183,  184,
    185,  0,    186,  187,  188,  189,  190,  191,  192,  193,  194,  195,
    196,  197,  198,  179,  180,  181,  182,  183,  184,  185,  0,    186,
    187,  188,  189,  190,  191,  192,  193,  194,  195,  196,  197,  198,
    0,    0,    0,    516,  0,    0,    517,  179,  180,  181,  182,  183,
    184,  185,  0,    186,  187,  188,  189,  190,  191,  192,  193,  194,
    195,  196,  197,  198,  179,  180,  181,  182,  183,  184,  185,  0,
    186,  187,  188,  189,  190,  191,  192,  193,  194,  195,  196,  197,
    198,  179,  180,  181,  182,  183,  184,  185,  0,    186,  187,  188,
    189,  190,  191,  192,  193,  194,  195,  196,  197,  198,  179,  180,
    181,  182,  183,  184,  185,  0,    186,  187,  188,  189,  190,  191,
    192,  193,  194,  195,  196,  197,  198,  179,  180,  181,  182,  183,
    184,  185,  0,    186,  187,  188,  189,  190,  191,  192,  193,  194,
    195,  196,  197,  198,  0,    0,    179,  180,  181,  182,  183,  184,
    185,  371,  186,  187,  188,  189,  190,  191,  192,  193,  194,  195,
    196,  197,  198,  179,  180,  181,  182,  183,  184,  185,  376,  186,
    187,  188,  189,  190,  191,  192,  193,  194,  195,  196,  197,  198,
    179,  180,  181,  182,  183,  184,  185,  377,  186,  187,  188,  189,
    190,  191,  192,  193,  194,  195,  196,  197,  198,  179,  180,  181,
    182,  183,  184,  185,  380,  186,  187,  188,  189,  190,  191,  192,
    193,  194,  195,  196,  197,  198,  179,  180,  181,  182,  183,  184,
    185,  381,  186,  187,  188,  189,  190,  191,  192,  193,  194,  195,
    196,  197,  198,  604,  605,  606,  607,  608,  0,    0,    382,  0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    179,  180,  181,  182,  183,  184,  185,  383,  186,  187,  188,  189,
    190,  191,  192,  193,  194,  195,  196,  197,  198,  179,  180,  181,
    182,  183,  184,  185,  603,  186,  187,  188,  189,  190,  191,  192,
    193,  194,  195,  196,  197,  198,  0,    539,  540,  541,  542,  0,
    0,    672,  0,    0,    0,    543,  0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    673,  544,
    545,  0,    0,    0,    0,    0,    546,  0,    609,  0,    0,    0,
    0,    0,    0,    0,    0,    547,  0,    0,    0,    0,    0,    366,
    0,    548,  0,    0,    0,    0,    0,    549,  0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    373,  0,    454,  0,
    0,    0,    0,    0,    550,  551,  0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    374,  0,    0,    0,    0,    0,    552,
    553,  0,    0,    0,    554,  555,  556,  0,    0,    610,  611,  0,
    612,  0,    284,  0,    0,    0,    0,    0,    0,    358,  0,    0,
    0,    140,  0,    0,    0,    0,    0,    0,    0,    557,  0,    665,
    0,    0,    0,    0,    0,    0,    0,    199,  0,    0,    0,    0,
    0,    200,  0,    0,    558,  0,    559,  0,    0,    0,    728,  0,
    0,    0,    0,    450,  0,    0,    0,    0,    0,    0,    0,    560,
    0,    0,    561,  562,  563,  564,  565,  0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    566,
    567,  0,    0,    0,    0,    0,    0,    568,  179,  180,  181,  182,
    183,  184,  185,  0,    186,  187,  188,  189,  190,  191,  192,  193,
    194,  195,  196,  197,  198,  179,  180,  181,  182,  183,  184,  185,
    0,    186,  187,  188,  189,  190,  191,  192,  193,  194,  195,  196,
    197,  198,  179,  180,  181,  182,  183,  184,  185,  0,    186,  187,
    188,  189,  190,  191,  192,  193,  194,  195,  196,  197,  198,  0,
    0,    179,  180,  181,  182,  183,  184,  185,  655,  186,  187,  188,
    189,  190,  191,  192,  193,  194,  195,  196,  197,  198,  449,  0,
    0,    0,    0,    0,    0,    0,    378,  0,    0,    0,    0,    0,
    0,    237,  0,    0,    0,    0,    0,    512,  179,  180,  181,  182,
    183,  184,  185,  656,  186,  187,  188,  189,  190,  191,  192,  193,
    194,  195,  196,  197,  198,  0,    179,  180,  181,  182,  183,  184,
    185,  238,  186,  187,  188,  189,  190,  191,  192,  193,  194,  195,
    196,  197,  198,  0,    0,    0,    0,    0,    395,  179,  180,  181,
    182,  183,  184,  185,  0,    186,  187,  188,  189,  190,  191,  192,
    193,  194,  195,  196,  197,  198,  180,  181,  182,  183,  184,  185,
    0,    186,  187,  188,  189,  190,  191,  192,  193,  194,  195,  196,
    197,  198,  182,  183,  184,  185,  0,    186,  187,  188,  189,  190,
    191,  192,  193,  194,  195,  196,  197,  198,  -371, -371, -371, -371,
    0,    186,  187,  188,  189,  190,  191,  192,  193,  194,  195,  196,
    197,  198};

/* YYCONFLP[YYPACT[STATE-NUM]] -- Pointer into YYCONFL of start of
   list of conflicting reductions corresponding to action entry for
   state STATE-NUM in yytable.  0 means no conflicts.  The list in
   yyconfl is terminated by a rule number of 0.  */
static const unsigned char yyconflp[] = {
    0,  0,  0,   0,   0,   0,   0,   0,  0,   0,   0,   0,   0,   0,  0,  0,
    0,  0,  0,   0,   0,   0,   0,   0,  0,   129, 0,   0,   0,   0,  0,  0,
    0,  0,  0,   0,   0,   0,   0,   0,  0,   0,   0,   0,   0,   0,  0,  0,
    0,  0,  0,   0,   0,   0,   0,   0,  0,   0,   0,   0,   0,   0,  0,  0,
    0,  0,  0,   0,   0,   0,   0,   0,  0,   0,   0,   0,   0,   0,  0,  0,
    0,  0,  0,   0,   0,   0,   0,   0,  0,   0,   0,   0,   0,   0,  0,  0,
    0,  0,  0,   0,   0,   0,   0,   0,  0,   0,   0,   0,   0,   0,  0,  0,
    0,  0,  0,   0,   0,   0,   0,   0,  0,   0,   0,   0,   0,   0,  0,  0,
    0,  0,  0,   0,   0,   0,   0,   0,  0,   0,   0,   0,   0,   0,  0,  0,
    0,  0,  0,   0,   0,   0,   0,   0,  0,   0,   0,   0,   0,   0,  0,  0,
    0,  0,  0,   0,   0,   0,   0,   0,  0,   0,   0,   0,   0,   0,  0,  0,
    0,  0,  0,   0,   0,   0,   0,   0,  0,   0,   0,   0,   0,   0,  0,  0,
    0,  0,  0,   0,   0,   0,   0,   0,  0,   0,   0,   0,   0,   0,  0,  0,
    0,  0,  0,   0,   0,   0,   0,   0,  0,   0,   0,   0,   0,   0,  0,  0,
    0,  0,  0,   0,   0,   0,   0,   0,  0,   0,   0,   0,   0,   0,  0,  0,
    0,  0,  0,   0,   0,   0,   0,   0,  0,   0,   0,   0,   0,   0,  0,  0,
    0,  0,  0,   0,   0,   0,   0,   0,  0,   0,   0,   0,   0,   0,  0,  0,
    0,  0,  0,   0,   0,   0,   0,   0,  0,   0,   0,   0,   0,   0,  0,  0,
    0,  0,  0,   0,   0,   0,   0,   0,  0,   0,   0,   0,   0,   0,  0,  0,
    0,  0,  0,   0,   0,   0,   0,   0,  0,   0,   0,   0,   0,   0,  0,  0,
    0,  0,  0,   0,   0,   0,   0,   0,  0,   0,   0,   0,   0,   0,  0,  0,
    0,  0,  0,   0,   0,   0,   0,   0,  0,   0,   0,   0,   0,   0,  0,  0,
    0,  0,  0,   0,   0,   0,   0,   0,  0,   0,   0,   0,   0,   0,  3,  0,
    0,  0,  0,   0,   0,   0,   0,   0,  0,   0,   0,   0,   0,   0,  0,  0,
    0,  0,  0,   0,   0,   0,   0,   0,  0,   0,   0,   0,   0,   0,  0,  0,
    0,  0,  0,   0,   0,   0,   0,   0,  0,   0,   0,   0,   0,   0,  0,  0,
    0,  0,  0,   0,   0,   0,   0,   0,  0,   0,   0,   0,   0,   0,  0,  0,
    0,  0,  0,   0,   0,   0,   0,   0,  0,   0,   0,   0,   0,   0,  0,  0,
    0,  0,  0,   0,   9,   0,   0,   0,  0,   0,   0,   11,  13,  15, 17, 19,
    21, 23, 0,   25,  27,  29,  31,  33, 35,  37,  39,  41,  43,  45, 47, 49,
    0,  0,  0,   0,   0,   51,  0,   0,  53,  55,  0,   0,   0,   0,  0,  0,
    0,  0,  0,   0,   0,   0,   0,   0,  0,   0,   0,   0,   0,   0,  0,  0,
    57, 0,  0,   0,   0,   0,   0,   0,  0,   0,   59,  61,  63,  65, 67, 0,
    0,  0,  0,   0,   69,  0,   0,   0,  0,   0,   0,   0,   0,   0,  71, 0,
    0,  73, 0,   0,   0,   0,   0,   0,  0,   75,  0,   0,   77,  0,  0,  79,
    81, 0,  0,   0,   0,   0,   0,   0,  0,   83,  0,   0,   0,   0,  0,  0,
    85, 0,  87,  7,   0,   0,   0,   0,  89,  91,  0,   0,   0,   0,  0,  0,
    0,  0,  0,   1,   0,   0,   0,   0,  0,   0,   0,   93,  0,   0,  0,  95,
    0,  0,  0,   0,   97,  0,   0,   0,  0,   0,   0,   0,   0,   0,  0,  99,
    0,  0,  0,   0,   0,   0,   0,   0,  0,   0,   0,   0,   101, 0,  0,  0,
    0,  0,  0,   0,   0,   0,   0,   0,  0,   0,   0,   0,   0,   0,  0,  0,
    0,  0,  0,   0,   0,   103, 0,   0,  0,   0,   105, 0,   0,   0,  0,  0,
    0,  0,  0,   0,   0,   0,   0,   0,  0,   0,   0,   0,   0,   0,  0,  0,
    0,  0,  0,   107, 109, 0,   111, 0,  113, 115, 0,   117, 0,   0,  0,  0,
    0,  0,  0,   0,   0,   0,   0,   0,  0,   0,   0,   0,   0,   0,  0,  0,
    0,  0,  0,   0,   0,   0,   0,   0,  0,   0,   0,   0,   0,   0,  0,  0,
    0,  0,  0,   0,   0,   0,   0,   0,  0,   0,   0,   0,   0,   0,  0,  119,
    0,  0,  0,   0,   0,   121, 0,   0,  0,   0,   0,   0,   0,   0,  0,  0,
    0,  0,  0,   0,   0,   0,   0,   0,  0,   0,   0,   0,   0,   0,  0,  0,
    0,  0,  123, 125, 0,   0,   127, 0,  0,   0,   0,   0,   0,   0,  0,  0,
    0,  0,  0,   0,   0,   0,   0,   0,  0,   0,   0,   0,   0,   0,  0,  0,
    0,  0,  0,   0,   0,   0,   0,   0,  0,   0,   0,   0,   0,   0,  0,  0,
    0,  0,  0,   0,   0,   0,   0,   0,  0,   0,   0,   0,   0,   0,  0,  0,
    0,  0,  0,   0,   0,   0,   0,   0,  0,   0,   0,   0,   0,   0,  0,  0,
    0,  0,  0,   0,   0,   0,   0,   0,  0,   0,   0,   0,   0,   0,  0,  0,
    0,  0,  0,   0,   0,   0,   0,   0,  0,   0,   0,   0,   0,   0,  0,  0,
    0,  0,  0,   0,   0,   0,   0,   0,  0,   0,   0,   0,   0,   0,  0,  0,
    0,  0,  0,   0,   0,   0,   0,   0,  0,   0,   0,   0,   0,   0,  0,  0,
    0,  0,  0,   0,   0,   0,   0,   0,  0,   0,   0,   0,   0,   0,  0,  0,
    0,  0,  0,   0,   0,   0,   0,   0,  0,   0,   0,   0,   0,   0,  0,  0,
    0,  0,  0,   0,   0,   0,   0,   0,  0,   0,   0,   0,   0,   0,  0,  0,
    0,  0,  0,   0,   0,   0,   0,   0,  0,   0,   0,   0,   0,   0,  0,  0,
    0,  0,  0,   0,   0,   0,   0,   0,  0,   0,   0,   0,   0,   0,  0,  0,
    0,  0,  0,   0,   0,   0,   0,   0,  0,   0,   0,   0,   0,   0,  0,  0,
    0,  0,  0,   0,   0,   0,   0,   0,  0,   0,   0,   0,   0,   0,  0,  0,
    0,  0,  0,   0,   0,   0,   0,   0,  0,   0,   0,   0,   0,   0,  0,  0,
    0,  0,  0,   0,   0,   0,   0,   0,  0,   0,   0,   0,   0,   0,  0,  0,
    0,  0,  0,   0,   0,   0,   0,   0,  0,   0,   0,   0,   0,   0,  0,  0,
    0,  0,  0,   0,   0,   0,   0,   0,  0,   0,   0,   0,   0,   0,  0,  0,
    0,  0,  0,   0,   0,   0,   0,   0,  0,   0,   0,   0,   0,   0,  0,  0,
    0,  0,  0,   0,   0,   0,   0,   0,  0,   0,   0,   0,   0,   0,  0,  0,
    0,  0,  0,   0,   0,   0,   0,   0,  0,   0,   0,   0,   0,   0,  0,  0,
    0,  0,  0,   0,   0,   0,   0,   0,  0,   0,   0,   0,   0,   0,  0,  0,
    0,  0,  0,   0,   0,   0,   0,   0,  0,   0,   0,   0,   0,   0,  0,  0,
    0,  0,  0,   0,   0,   0,   0,   0,  0,   0,   0,   0,   0,   0,  0,  0,
    0,  0,  0,   0,   0,   0,   0,   0,  0,   0,   0,   0,   0,   0,  0,  0,
    0,  0,  0,   0,   0,   0,   0,   0,  0,   0,   0,   0,   0,   0,  0,  0,
    0,  0,  0,   0,   0,   0,   0,   0,  0,   0,   0,   0,   0,   0,  0,  0,
    0,  0,  0,   0,   0,   0,   0,   0,  0,   0,   0,   0,   0,   0,  0,  0,
    0,  0,  0,   0,   0,   0,   0,   0,  0,   0,   0,   0,   0,   0,  0,  0,
    0,  0,  0,   0,   0,   0,   0,   0,  0,   0,   0,   0,   0,   0,  0,  0,
    0,  0,  0,   0,   0,   0,   0,   0,  0,   0,   0,   0,   0,   0,  0,  0,
    0,  0,  0,   0,   0,   0,   0,   0,  0,   0,   0,   0,   0,   0,  0,  0,
    0,  0,  0,   0,   0,   0,   0,   0,  0,   0,   0,   0,   0,   0,  0,  0,
    0,  0,  0,   0,   0,   0,   0,   0,  0,   0,   0,   0,   0,   0,  0,  0,
    0,  0,  0,   0,   0,   0,   0,   0,  0,   0,   0,   0,   0,   0,  0,  0,
    0,  0,  0,   0,   0,   0,   0,   0,  0,   0,   0,   0,   0,   0,  0,  0,
    0,  0,  0,   0,   0,   0,   0,   0,  0,   0,   0,   0,   0,   0,  0,  0,
    0,  0,  0,   0,   0,   0,   0,   0,  0,   0,   0,   0,   0,   0,  0,  0,
    0,  0,  0,   0,   0,   0,   0,   0,  0,   0,   0,   0,   0,   0,  0,  0,
    0,  0,  0,   0,   0,   0,   0,   0,  0,   0,   0,   0,   0,   0,  0,  0,
    0,  0,  0,   0,   0,   0,   0,   0,  0,   0,   0,   0,   0,   0,  0,  0,
    0,  0,  0,   0,   0,   0,   0,   0,  0,   0,   0,   0,   0,   0,  0,  0,
    0,  0,  0,   0,   0,   0,   0,   0,  0,   0,   0,   0,   0,   0,  0,  0,
    0,  0,  0,   0,   0,   0,   0,   0,  0,   0,   0,   0,   0,   0,  0,  0,
    0,  0,  0,   0,   0,   0,   0,   0,  0,   0,   0,   0,   0,   0,  0,  0,
    0,  0,  0,   0,   0,   0,   0,   0,  0,   0,   0,   0,   0,   0,  0,  0,
    0,  0,  0,   0,   0,   0,   0,   0,  0,   0,   0,   0,   0,   0,  0,  0,
    0,  0,  0,   0,   0,   0,   0,   0,  0,   0,   0,   0,   0,   0,  0,  0,
    0,  0,  0,   0,   0,   0,   0,   0,  0,   0,   0,   0,   0,   0,  0,  0,
    0,  0,  0,   0,   0,   0,   0,   0,  0,   0,   0,   0,   0,   0,  0,  0,
    0,  0,  0,   0,   0,   0,   0,   0,  0,   0,   0,   0,   0,   0,  0,  0,
    0,  0,  0,   0,   0,   0,   0,   0,  0,   0,   0,   0,   0,   0,  0,  0,
    0,  0,  0,   0,   0,   0,   0,   0,  0,   0,   0,   0,   0,   0,  0,  0,
    0,  0,  0,   0,   0,   0,   0,   0,  0,   0,   0,   0,   0,   0,  0,  0,
    0,  0,  0,   0,   0,   0,   0,   0,  0,   0,   0,   0,   0,   0,  0,  0,
    0,  0,  0,   0,   0,   0,   0,   0,  0,   0,   0,   0,   0,   0,  0,  0,
    0,  0,  0,   0,   0,   0,   0,   0,  0,   0,   0,   0,   0,   0,  0,  0,
    0,  0,  0,   0,   0,   0,   0,   0,  0,   0,   0,   0,   0,   0,  0,  0,
    0,  0,  0,   0,   0,   0,   0,   0,  0,   0,   0,   0,   0,   0,  0,  0,
    0,  0,  0,   0,   0,   0,   0,   0,  0,   0,   0,   0,   0,   0,  0,  0,
    0,  0,  0,   0,   0,   0,   0,   0,  0,   0,   0,   0,   0,   0,  0,  0,
    0,  0,  0,   0,   0,   0,   0,   0,  0,   0,   0,   0,   0,   0,  0,  0,
    0,  0,  0,   0,   0,   0,   0,   0,  0,   0,   0,   0,   0,   0,  0,  0,
    0,  0,  0,   0,   0,   0,   0,   0,  0,   0,   0,   0,   0,   0,  0,  0,
    0,  0,  0,   0,   0,   0,   0,   0,  0,   0,   0,   0,   0,   0,  0,  0,
    0,  0,  0,   0,   0,   0,   0,   0,  0,   0,   0,   0,   0,   0,  0,  0,
    0,  0,  0,   0,   0,   0,   0,   0,  0,   0,   0,   0,   0,   0,  0,  0,
    0,  0,  0,   0,   0,   0,   0,   0,  0,   0,   0,   0,   0,   0,  0,  0,
    0,  0,  0,   0,   0,   0,   0,   0,  0,   0,   0,   0,   0,   0,  0,  0,
    0,  0,  0,   0,   0,   0,   0,   0,  0,   0,   0,   0,   0,   0,  0,  0,
    0,  0,  0,   0,   0,   0,   0,   0,  0,   0,   0,   0,   0,   0,  0,  0,
    0,  0,  0,   0,   0,   0,   0,   0,  0,   0,   0,   0,   0,   0,  0,  0,
    0,  0,  0,   0,   0,   0,   0,   0,  0,   0,   0,   0,   0,   0,  0,  0,
    0,  0,  0,   0,   0,   0,   0,   0,  0,   0,   0,   0,   0,   0,  0,  0,
    0,  0,  0,   0,   0,   0,   0,   0,  0,   0,   0,   0,   0,   0,  0,  0,
    0,  0,  0,   0,   0,   0,   0,   0,  0,   0,   0,   0,   0,   0,  0,  0,
    0,  0,  0,   0,   0,   0,   0,   0,  0,   0,   0,   0,   0,   0,  0,  0,
    0,  0,  0,   0,   0,   0,   0,   0,  0,   0,   0,   0,   0,   0,  0,  0,
    0,  0,  5,   0,   0,   0,   0,   0,  0,   0,   0,   0,   0,   0,  0,  0,
    0,  0,  0,   0,   0,   0,   0,   0,  0,   0,   0,   0,   0,   0,  0,  0,
    0,  0,  0,   0,   0,   0,   0,   0,  0,   0,   0,   0,   0,   0,  0,  0,
    0,  0,  0,   0,   0,   0,   0,   0,  0,   0,   0,   0,   0,   0,  0,  0,
    0,  0,  0,   0,   0,   0,   0,   0,  0,   0,   0,   0,   0,   0,  0,  0,
    0,  0,  0,   0,   0,   0,   0,   0,  0,   0,   0,   0,   0,   0,  0,  0,
    0,  0,  0,   0,   0,   0,   0,   0,  0,   0,   0,   0,   0,   0,  0,  0,
    0,  0,  0,   0,   0,   0,   0,   0,  0,   0,   0,   0,   0,   0,  0,  0,
    0,  0,  0,   0,   0,   0,   0,   0,  0,   0,   0,   0,   0,   0,  0,  0,
    0,  0,  0,   0,   0,   0,   0,   0,  0,   0,   0,   0,   0,   0,  0,  0,
    0,  0,  0,   0,   0,   0,   0,   0,  0,   0,   0,   0,   0,   0,  0,  0,
    0,  0,  0,   0,   0,   0,   0,   0,  0,   0,   0,   0,   0,   0,  0,  0,
    0,  0,  0,   0,   0,   0,   0,   0,  0,   0,   0,   0,   0,   0,  0,  0,
    0,  0,  0,   0,   0,   0,   0,   0,  0,   0,   0,   0,   0,   0,  0,  0,
    0,  0,  0,   0,   0,   0,   0,   0,  0,   0,   0,   0,   0,   0,  0,  0,
    0,  0,  0,   0,   0,   0,   0,   0,  0,   0,   0,   0,   0,   0,  0,  0,
    0,  0,  0,   0,   0,   0,   0,   0,  0,   0,   0,   0,   0,   0,  0,  0,
    0,  0,  0,   0,   0,   0,   0,   0,  0,   0,   0,   0,   0,   0,  0,  0,
    0,  0,  0,   0,   0,   0,   0,   0,  0,   0,   0,   0,   0,   0,  0,  0,
    0,  0,  0,   0,   0,   0,   0,   0,  0,   0,   0,   0,   0,   0,  0,  0,
    0,  0,  0,   0,   0,   0};

/* YYCONFL[I] -- lists of conflicting rule numbers, each terminated by
   0, pointed into by YYCONFLP.  */
static const short int yyconfl[] = {
    0, 116, 0, 370, 0, 116, 0, 60,  0, 130, 0, 130, 0, 130, 0, 130, 0, 130,
    0, 130, 0, 130, 0, 130, 0, 130, 0, 130, 0, 130, 0, 130, 0, 130, 0, 130,
    0, 130, 0, 130, 0, 130, 0, 130, 0, 130, 0, 130, 0, 130, 0, 130, 0, 130,
    0, 130, 0, 130, 0, 130, 0, 130, 0, 130, 0, 130, 0, 130, 0, 130, 0, 130,
    0, 130, 0, 130, 0, 130, 0, 130, 0, 130, 0, 130, 0, 130, 0, 130, 0, 130,
    0, 130, 0, 130, 0, 130, 0, 130, 0, 130, 0, 130, 0, 130, 0, 130, 0, 130,
    0, 130, 0, 130, 0, 130, 0, 130, 0, 130, 0, 130, 0, 130, 0, 130, 0, 130,
    0, 130, 0, 74,  0};

static const short int yycheck[] = {
    9,   71,  50,  351, 196, 117, 132, 3,   171, 107, 117, 151, 130, 176, 117,
    3,   4,   5,   6,   7,   8,   3,   3,   117, 158, 158, 3,   6,   4,   17,
    18,  3,   69,  42,  150, 3,   24,  25,  234, 338, 49,  50,  51,  52,  53,
    479, 209, 163, 38,  6,   158, 443, 135, 93,  93,  3,   96,  96,  147, 47,
    345, 18,  39,  51,  277, 205, 206, 39,  537, 354, 182, 234, 118, 193, 230,
    182, 40,  53,  87,  182, 29,  337, 3,   296, 130, 201, 3,   219, 182, 3,
    213, 39,  305, 249, 226, 3,   4,   5,   6,   7,   8,   234, 69,  235, 187,
    26,  16,  576, 18,  17,  18,  99,  26,  89,  254, 507, 24,  25,  238, 83,
    53,  284, 55,  113, 264, 134, 234, 103, 427, 477, 219, 140, 295, 213, 157,
    338, 184, 253, 341, 337, 108, 575, 338, 51,  167, 133, 431, 156, 340, 125,
    118, 118, 101, 190, 243, 164, 165, 166, 167, 168, 169, 170, 130, 172, 173,
    174, 175, 176, 3,   63,  179, 180, 181, 182, 183, 315, 213, 156, 187, 188,
    189, 190, 191, 192, 193, 194, 195, 196, 197, 198, 107, 99,  116, 13,  14,
    494, 16,  193, 497, 156, 117, 169, 242, 242, 338, 158, 295, 339, 299, 556,
    298, 299, 344, 560, 303, 213, 307, 338, 565, 118, 341, 116, 340, 190, 572,
    573, 120, 236, 340, 146, 3,   394, 339, 340, 337, 129, 224, 246, 213, 248,
    338, 135, 338, 337, 3,   341, 680, 214, 9,   634, 3,   636, 339, 262, 340,
    640, 173, 642, 107, 644, 154, 270, 234, 352, 10,  11,  12,  13,  14,  15,
    16,  3,   18,  19,  20,  21,  22,  23,  24,  25,  26,  27,  28,  29,  30,
    401, 299, 296, 297, 415, 338, 196, 18,  187, 303, 319, 320, 3,   646, 647,
    14,  464, 650, 422, 423, 394, 301, 396, 397, 398, 136, 659, 664, 665, 104,
    41,  627, 628, 212, 630, 631, 632, 331, 107, 635, 299, 637, 277, 639, 340,
    641, 57,  340, 340, 242, 475, 324, 325, 326, 327, 328, 329, 330, 331, 340,
    333, 334, 335, 336, 358, 359, 421, 340, 340, 80,  740, 741, 366, 340, 340,
    369, 370, 339, 526, 81,  107, 338, 339, 298, 340, 540, 541, 337, 543, 340,
    464, 546, 547, 340, 549, 550, 551, 340, 337, 340, 555, 395, 557, 340, 559,
    4,   5,   6,   7,   564, 112, 340, 567, 487, 69,  117, 340, 18,  751, 130,
    26,  27,  28,  29,  30,  127, 3,   3,   3,   3,   763, 324, 325, 326, 327,
    328, 329, 330, 331, 339, 333, 334, 335, 336, 146, 156, 99,  340, 3,   158,
    3,   3,   526, 108, 340, 107, 450, 107, 169, 338, 454, 455, 338, 118, 3,
    3,   614, 3,   3,   193, 3,   8,   4,   3,   10,  11,  12,  13,  14,  15,
    16,  341, 18,  19,  20,  21,  22,  23,  24,  25,  26,  27,  28,  29,  30,
    14,  490, 340, 653, 107, 36,  495, 99,  39,  40,  340, 3,   501, 234, 26,
    243, 39,  506, 3,   3,   3,   171, 228, 3,   341, 514, 340, 516, 341, 0,
    1,   341, 63,  341, 3,   4,   5,   6,   7,   8,   190, 340, 73,  74,  75,
    76,  77,  340, 17,  18,  340, 201, 83,  30,  3,   24,  25,  245, 3,   158,
    341, 341, 93,  163, 214, 96,  129, 38,  165, 3,   339, 3,   245, 104, 341,
    126, 107, 340, 228, 110, 111, 16,  51,  341, 341, 341, 4,   338, 3,   120,
    3,   62,  341, 62,  583, 3,   127, 48,  129, 158, 69,  3,   3,   129, 135,
    136, 20,  129, 340, 598, 599, 82,  338, 158, 341, 341, 110, 4,   341, 90,
    340, 671, 340, 154, 341, 341, 341, 158, 341, 338, 99,  341, 163, 21,  22,
    23,  24,  25,  26,  27,  28,  29,  30,  174, 130, 341, 341, 340, 338, 118,
    341, 340, 123, 159, 339, 3,   187, 22,  23,  24,  25,  26,  27,  28,  29,
    30,  338, 138, 48,  111, 341, 338, 341, 4,   663, 664, 665, 20,  340, 158,
    340, 212, 44,  340, 340, 340, 217, 10,  11,  12,  13,  14,  15,  16,  340,
    18,  19,  20,  21,  22,  23,  24,  25,  26,  27,  28,  29,  30,  340, 3,
    340, 242, 243, 3,   245, 20,  247, 248, 130, 250, 338, 190, 193, 4,   341,
    341, 197, 341, 717, 189, 5,   3,   4,   5,   6,   7,   8,   3,   5,   728,
    4,   231, 5,   341, 213, 17,  18,  245, 341, 338, 341, 341, 24,  25,  26,
    338, 189, 167, 20,  341, 156, 33,  23,  24,  25,  26,  27,  28,  29,  30,
    4,   302, 130, 341, 341, 341, 341, 308, 20,  51,  127, 104, 10,  11,  12,
    13,  14,  15,  16,  188, 18,  19,  20,  21,  22,  23,  24,  25,  26,  27,
    28,  29,  30,  7,   158, 5,   337, 338, 3,   338, 341, 280, 281, 196, 86,
    87,  4,   338, 7,   288, 3,   341, 3,   341, 10,  294, 295, 99,  297, 298,
    299, 300, 728, 302, 303, 304, 236, 306, 307, 308, 112, 20,  21,  22,  23,
    24,  25,  26,  27,  28,  29,  30,  321, 322, 323, 324, 325, 326, 327, 328,
    329, 330, 331, 715, 333, 334, 335, 336, 717, 600, 410, 340, 614, 3,   4,
    5,   6,   7,   8,   107, 24,  25,  26,  27,  28,  29,  30,  17,  18,  587,
    141, 5,   374, 638, 24,  25,  3,   4,   5,   6,   7,   8,   774, 33,  724,
    104, 346, 37,  719, -1,  17,  18,  -1,  -1,  -1,  -1,  -1,  24,  25,  26,
    -1,  51,  -1,  -1,  3,   4,   5,   6,   7,   8,   -1,  -1,  -1,  -1,  -1,
    207, 208, 209, 17,  18,  212, -1,  -1,  -1,  51,  24,  25,  26,  -1,  3,
    4,   5,   6,   7,   8,   -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  17,  18,
    -1,  -1,  -1,  -1,  99,  24,  25,  51,  -1,  3,   4,   5,   6,   7,   8,
    -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  17,  18,  -1,  -1,  99,  -1,  -1,
    24,  25,  51,  -1,  -1,  -1,  -1,  3,   4,   5,   6,   7,   8,   -1,  -1,
    -1,  -1,  -1,  -1,  -1,  -1,  17,  18,  341, -1,  99,  -1,  51,  24,  25,
    3,   4,   5,   6,   7,   8,   -1,  -1,  -1,  -1,  -1,  -1,  -1,  1,   17,
    18,  -1,  -1,  -1,  -1,  99,  24,  25,  -1,  -1,  51,  80,  324, 325, 326,
    327, 328, 329, 330, 331, -1,  333, 334, 335, 336, -1,  -1,  -1,  340, 199,
    99,  -1,  -1,  51,  -1,  38,  -1,  -1,  -1,  80,  -1,  -1,  3,   4,   5,
    6,   7,   8,   -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  17,  18,  99,  62,
    -1,  -1,  80,  24,  25,  -1,  -1,  -1,  -1,  -1,  338, 3,   4,   5,   6,
    7,   8,   -1,  -1,  82,  -1,  99,  -1,  -1,  -1,  17,  18,  90,  -1,  -1,
    51,  -1,  24,  25,  -1,  -1,  -1,  -1,  -1,  193, 3,   4,   5,   6,   7,
    8,   -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  17,  18,  -1,  -1,  -1,  51,
    123, 24,  25,  19,  20,  21,  22,  23,  24,  25,  26,  27,  28,  29,  30,
    138, -1,  -1,  99,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  51,  -1,
    -1,  -1,  -1,  -1,  324, 325, 326, 327, 328, 329, 330, 331, -1,  333, 334,
    335, 336, 99,  -1,  -1,  340, -1,  -1,  -1,  -1,  -1,  -1,  324, 325, 326,
    327, 328, 329, 330, 331, -1,  333, 334, 335, 336, 193, -1,  -1,  340, 197,
    -1,  99,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  324, 325, 326, 327, 328,
    329, 330, 331, -1,  333, 334, 335, 336, -1,  -1,  -1,  340, -1,  -1,  -1,
    -1,  -1,  -1,  -1,  -1,  324, 325, 326, 327, 328, 329, 330, 331, -1,  333,
    334, 335, 336, -1,  -1,  -1,  340, -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,
    324, 325, 326, 327, 328, 329, 330, 331, -1,  333, 334, 335, 336, -1,  -1,
    -1,  340, -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  324, 325,
    326, 327, 328, 329, 330, 331, -1,  333, 334, 335, 336, -1,  -1,  -1,  340,
    -1,  -1,  -1,  -1,  -1,  -1,  324, 325, 326, 327, 328, 329, 330, 331, -1,
    333, 334, 335, 336, -1,  -1,  -1,  340, 10,  11,  12,  13,  14,  15,  16,
    -1,  18,  19,  20,  21,  22,  23,  24,  25,  26,  27,  28,  29,  30,  12,
    13,  14,  15,  16,  -1,  18,  19,  20,  21,  22,  23,  24,  25,  26,  27,
    28,  29,  30,  324, 325, 326, 327, 328, 329, 330, 331, -1,  333, 334, 335,
    336, -1,  -1,  -1,  340, -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,
    -1,  -1,  324, 325, 326, 327, 328, 329, 330, 331, -1,  333, 334, 335, 336,
    -1,  -1,  -1,  340, -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,
    -1,  324, 325, 326, 327, 328, 329, 330, 331, -1,  333, 334, 335, 336, -1,
    -1,  -1,  340, 10,  11,  12,  13,  14,  15,  16,  -1,  18,  19,  20,  21,
    22,  23,  24,  25,  26,  27,  28,  29,  30,  10,  11,  12,  13,  14,  15,
    16,  -1,  18,  19,  20,  21,  22,  23,  24,  25,  26,  27,  28,  29,  30,
    10,  11,  12,  13,  14,  15,  16,  -1,  18,  19,  20,  21,  22,  23,  24,
    25,  26,  27,  28,  29,  30,  10,  11,  12,  13,  14,  15,  16,  -1,  18,
    19,  20,  21,  22,  23,  24,  25,  26,  27,  28,  29,  30,  10,  11,  12,
    13,  14,  15,  16,  -1,  18,  19,  20,  21,  22,  23,  24,  25,  26,  27,
    28,  29,  30,  10,  11,  12,  13,  14,  15,  16,  -1,  18,  19,  20,  21,
    22,  23,  24,  25,  26,  27,  28,  29,  30,  10,  11,  12,  13,  14,  15,
    16,  -1,  18,  19,  20,  21,  22,  23,  24,  25,  26,  27,  28,  29,  30,
    10,  11,  12,  13,  14,  15,  16,  -1,  18,  19,  20,  21,  22,  23,  24,
    25,  26,  27,  28,  29,  30,  10,  11,  12,  13,  14,  15,  16,  -1,  18,
    19,  20,  21,  22,  23,  24,  25,  26,  27,  28,  29,  30,  10,  11,  12,
    13,  14,  15,  16,  -1,  18,  19,  20,  21,  22,  23,  24,  25,  26,  27,
    28,  29,  30,  -1,  -1,  -1,  338, -1,  -1,  341, 10,  11,  12,  13,  14,
    15,  16,  -1,  18,  19,  20,  21,  22,  23,  24,  25,  26,  27,  28,  29,
    30,  10,  11,  12,  13,  14,  15,  16,  -1,  18,  19,  20,  21,  22,  23,
    24,  25,  26,  27,  28,  29,  30,  10,  11,  12,  13,  14,  15,  16,  -1,
    18,  19,  20,  21,  22,  23,  24,  25,  26,  27,  28,  29,  30,  10,  11,
    12,  13,  14,  15,  16,  -1,  18,  19,  20,  21,  22,  23,  24,  25,  26,
    27,  28,  29,  30,  10,  11,  12,  13,  14,  15,  16,  -1,  18,  19,  20,
    21,  22,  23,  24,  25,  26,  27,  28,  29,  30,  -1,  -1,  10,  11,  12,
    13,  14,  15,  16,  341, 18,  19,  20,  21,  22,  23,  24,  25,  26,  27,
    28,  29,  30,  10,  11,  12,  13,  14,  15,  16,  341, 18,  19,  20,  21,
    22,  23,  24,  25,  26,  27,  28,  29,  30,  10,  11,  12,  13,  14,  15,
    16,  341, 18,  19,  20,  21,  22,  23,  24,  25,  26,  27,  28,  29,  30,
    10,  11,  12,  13,  14,  15,  16,  341, 18,  19,  20,  21,  22,  23,  24,
    25,  26,  27,  28,  29,  30,  10,  11,  12,  13,  14,  15,  16,  341, 18,
    19,  20,  21,  22,  23,  24,  25,  26,  27,  28,  29,  30,  73,  74,  75,
    76,  77,  -1,  -1,  341, -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,
    -1,  -1,  -1,  10,  11,  12,  13,  14,  15,  16,  341, 18,  19,  20,  21,
    22,  23,  24,  25,  26,  27,  28,  29,  30,  10,  11,  12,  13,  14,  15,
    16,  341, 18,  19,  20,  21,  22,  23,  24,  25,  26,  27,  28,  29,  30,
    -1,  43,  44,  45,  46,  -1,  -1,  341, -1,  -1,  -1,  53,  -1,  -1,  -1,
    -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  341, 71,
    72,  -1,  -1,  -1,  -1,  -1,  78,  -1,  174, -1,  -1,  -1,  -1,  -1,  -1,
    -1,  -1,  89,  -1,  -1,  -1,  -1,  -1,  338, -1,  97,  -1,  -1,  -1,  -1,
    -1,  103, -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  338,
    -1,  107, -1,  -1,  -1,  -1,  -1,  124, 125, -1,  -1,  -1,  -1,  -1,  -1,
    -1,  -1,  -1,  -1,  -1,  338, -1,  -1,  -1,  -1,  -1,  143, 144, -1,  -1,
    -1,  148, 149, 150, -1,  -1,  247, 248, -1,  250, -1,  338, -1,  -1,  -1,
    -1,  -1,  -1,  217, -1,  -1,  -1,  242, -1,  -1,  -1,  -1,  -1,  -1,  -1,
    177, -1,  338, -1,  -1,  -1,  -1,  -1,  -1,  -1,  302, -1,  -1,  -1,  -1,
    -1,  308, -1,  -1,  196, -1,  198, -1,  -1,  -1,  338, -1,  -1,  -1,  -1,
    217, -1,  -1,  -1,  -1,  -1,  -1,  -1,  215, -1,  -1,  218, 219, 220, 221,
    222, -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,
    -1,  -1,  239, 240, -1,  -1,  -1,  -1,  -1,  -1,  247, 10,  11,  12,  13,
    14,  15,  16,  -1,  18,  19,  20,  21,  22,  23,  24,  25,  26,  27,  28,
    29,  30,  10,  11,  12,  13,  14,  15,  16,  -1,  18,  19,  20,  21,  22,
    23,  24,  25,  26,  27,  28,  29,  30,  10,  11,  12,  13,  14,  15,  16,
    -1,  18,  19,  20,  21,  22,  23,  24,  25,  26,  27,  28,  29,  30,  -1,
    -1,  10,  11,  12,  13,  14,  15,  16,  40,  18,  19,  20,  21,  22,  23,
    24,  25,  26,  27,  28,  29,  30,  96,  -1,  -1,  -1,  -1,  -1,  -1,  -1,
    39,  -1,  -1,  -1,  -1,  -1,  -1,  3,   -1,  -1,  -1,  -1,  -1,  96,  10,
    11,  12,  13,  14,  15,  16,  83,  18,  19,  20,  21,  22,  23,  24,  25,
    26,  27,  28,  29,  30,  -1,  10,  11,  12,  13,  14,  15,  16,  39,  18,
    19,  20,  21,  22,  23,  24,  25,  26,  27,  28,  29,  30,  -1,  -1,  -1,
    -1,  -1,  36,  10,  11,  12,  13,  14,  15,  16,  -1,  18,  19,  20,  21,
    22,  23,  24,  25,  26,  27,  28,  29,  30,  11,  12,  13,  14,  15,  16,
    -1,  18,  19,  20,  21,  22,  23,  24,  25,  26,  27,  28,  29,  30,  13,
    14,  15,  16,  -1,  18,  19,  20,  21,  22,  23,  24,  25,  26,  27,  28,
    29,  30,  13,  14,  15,  16,  -1,  18,  19,  20,  21,  22,  23,  24,  25,
    26,  27,  28,  29,  30};

/* YYSTOS[STATE-NUM] -- The (internal number of the) accessing
   symbol of state STATE-NUM.  */
static const unsigned short int yystos[] = {
    0,   1,   38,  62,  82,  90,  123, 138, 193, 197, 343, 344, 345, 380, 382,
    395, 396, 401, 402, 403, 405, 408, 416, 423, 337, 213, 69,  108, 171, 190,
    201, 214, 228, 385, 397, 424, 69,  118, 190, 385, 410, 213, 357, 3,   4,
    5,   6,   7,   8,   17,  18,  24,  25,  51,  62,  69,  99,  118, 190, 213,
    280, 281, 288, 294, 295, 297, 300, 302, 303, 304, 306, 308, 321, 322, 323,
    324, 325, 326, 327, 328, 329, 330, 331, 333, 334, 335, 336, 340, 373, 417,
    421, 422, 0,   1,   344, 337, 3,   116, 381, 158, 381, 213, 118, 3,   107,
    117, 146, 173, 425, 116, 404, 3,   404, 213, 81,  112, 117, 127, 146, 411,
    3,   26,  33,  86,  87,  112, 207, 208, 209, 212, 358, 359, 373, 339, 9,
    373, 99,  373, 373, 373, 242, 373, 379, 69,  190, 213, 3,   340, 107, 3,
    299, 196, 14,  419, 3,   301, 136, 354, 104, 299, 107, 418, 354, 277, 340,
    340, 340, 340, 340, 340, 340, 340, 340, 340, 340, 340, 340, 373, 374, 10,
    11,  12,  13,  14,  15,  16,  18,  19,  20,  21,  22,  23,  24,  25,  26,
    27,  28,  29,  30,  302, 308, 277, 296, 305, 298, 299, 307, 337, 337, 107,
    18,  3,   3,   3,   381, 3,   339, 426, 3,   340, 360, 361, 362, 365, 372,
    425, 107, 338, 99,  3,   158, 3,   404, 3,   107, 107, 338, 3,   39,  364,
    3,   26,  373, 340, 373, 379, 93,  96,  242, 3,   3,   3,   345, 3,   418,
    419, 4,   298, 299, 373, 8,   3,   14,  420, 418, 373, 47,  133, 224, 373,
    376, 373, 373, 26,  373, 373, 373, 373, 374, 373, 373, 373, 373, 374, 338,
    341, 341, 373, 373, 373, 373, 373, 6,   18,  156, 340, 13,  14,  16,  373,
    33,  37,  199, 340, 373, 373, 373, 373, 373, 373, 373, 373, 373, 373, 373,
    418, 107, 419, 419, 374, 99,  340, 3,   234, 398, 26,  339, 364, 3,   345,
    360, 243, 346, 63,  120, 135, 154, 187, 212, 338, 366, 368, 39,  363, 234,
    360, 3,   3,   3,   406, 196, 340, 412, 374, 360, 358, 3,   345, 217, 93,
    96,  373, 373, 341, 418, 419, 338, 373, 419, 107, 338, 341, 373, 338, 338,
    341, 341, 341, 39,  341, 341, 341, 341, 341, 341, 374, 16,  18,  6,   156,
    345, 374, 373, 373, 340, 36,  340, 340, 340, 345, 419, 3,   245, 3,   356,
    117, 182, 339, 340, 383, 384, 319, 320, 399, 158, 3,   339, 341, 341, 373,
    163, 352, 135, 187, 369, 362, 360, 129, 165, 367, 3,   360, 346, 426, 339,
    50,  184, 338, 407, 3,   415, 356, 238, 345, 245, 346, 341, 373, 373, 96,
    217, 373, 373, 373, 107, 126, 378, 378, 4,   53,  89,  103, 125, 377, 340,
    16,  341, 341, 345, 374, 373, 345, 345, 345, 341, 418, 4,   338, 341, 3,
    3,   108, 118, 130, 169, 386, 387, 363, 3,   364, 48,  354, 367, 367, 129,
    158, 362, 129, 346, 3,   3,   20,  158, 338, 409, 341, 340, 413, 409, 4,
    110, 347, 96,  373, 104, 341, 338, 341, 373, 373, 341, 341, 341, 345, 374,
    375, 340, 341, 341, 341, 341, 341, 419, 338, 356, 157, 167, 340, 383, 43,
    44,  45,  46,  53,  71,  72,  78,  89,  97,  103, 124, 125, 143, 144, 148,
    149, 150, 177, 196, 198, 215, 218, 219, 220, 221, 222, 239, 240, 247, 389,
    118, 130, 340, 340, 130, 341, 338, 345, 340, 353, 373, 362, 373, 158, 234,
    370, 371, 362, 339, 80,  373, 159, 3,   80,  373, 414, 409, 338, 48,  111,
    351, 373, 373, 341, 73,  74,  75,  76,  77,  174, 247, 248, 250, 341, 338,
    341, 345, 374, 4,   20,  158, 386, 340, 391, 391, 391, 391, 391, 391, 340,
    391, 391, 391, 44,  392, 391, 392, 391, 340, 391, 392, 391, 392, 340, 391,
    388, 340, 340, 356, 356, 340, 383, 386, 3,   400, 40,  83,  349, 373, 340,
    371, 3,   130, 20,  338, 338, 341, 4,   348, 373, 373, 352, 341, 341, 375,
    341, 341, 189, 5,   3,   341, 5,   393, 394, 393, 393, 4,   390, 393, 393,
    393, 394, 393, 394, 393, 390, 393, 394, 393, 394, 5,   394, 18,  41,  57,
    80,  130, 156, 169, 228, 356, 356, 341, 341, 356, 391, 341, 338, 356, 231,
    80,  373, 414, 414, 338, 189, 245, 350, 338, 354, 20,  167, 383, 338, 341,
    230, 249, 53,  55,  338, 341, 341, 341, 156, 4,   4,   5,   6,   7,   130,
    130, 340, 341, 341, 341, 349, 353, 341, 415, 413, 20,  188, 348, 127, 355,
    7,   158, 5,   196, 3,   4,   394, 394, 356, 338, 7,   356, 3,   341, 3,
    341, 400};

/* Error token number */
#define YYTERROR 1

/* This macro is provided for backward compatibility. */
#ifndef YY_LOCATION_PRINT
#define YY_LOCATION_PRINT(File, Loc) ((void)0)
#endif

/* YYLEX -- calling `yylex' with the right arguments.  */
#ifdef YYLEX_PARAM
#define YYLEX yylex(&yylval, YYLEX_PARAM)
#else
#define YYLEX yylex(&yylval)
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

#define YYCHK(YYE)                     \
  do {                                 \
    YYRESULTTAG yyflag = YYE;          \
    if (yyflag != yyok) return yyflag; \
  } while (YYID(0))

#if YYDEBUG

#ifndef YYFPRINTF
#define YYFPRINTF fprintf
#endif

#define YYDPRINTF(Args)          \
  do {                           \
    if (yydebug) YYFPRINTF Args; \
  } while (YYID(0))

/*--------------------------------.
| Print this symbol on YYOUTPUT.  |
`--------------------------------*/

/*ARGSUSED*/
static void yy_symbol_value_print(FILE *yyoutput, int yytype,
                                  YYSTYPE const *const yyvaluep,
                                  struct ParseResult *result) {
  FILE *yyo = yyoutput;
  YYUSE(yyo);
  if (!yyvaluep) return;
  YYUSE(result);
#ifdef YYPRINT
  if (yytype < YYNTOKENS) YYPRINT(yyoutput, yytoknum[yytype], *yyvaluep);
#else
  YYUSE(yyoutput);
#endif
  switch (yytype) {
    default:
      break;
  }
}

/*--------------------------------.
| Print this symbol on YYOUTPUT.  |
`--------------------------------*/

static void yy_symbol_print(FILE *yyoutput, int yytype,
                            YYSTYPE const *const yyvaluep,
                            struct ParseResult *result) {
  if (yytype < YYNTOKENS)
    YYFPRINTF(yyoutput, "token %s (", yytname[yytype]);
  else
    YYFPRINTF(yyoutput, "nterm %s (", yytname[yytype]);

  yy_symbol_value_print(yyoutput, yytype, yyvaluep, result);
  YYFPRINTF(yyoutput, ")");
}

#define YY_SYMBOL_PRINT(Title, Type, Value, Location) \
  do {                                                \
    if (yydebug) {                                    \
      YYFPRINTF(stderr, "%s ", Title);                \
      yy_symbol_print(stderr, Type, Value, result);   \
      YYFPRINTF(stderr, "\n");                        \
    }                                                 \
  } while (YYID(0))

/* Nonzero means print parse trace.  It is left uninitialized so that
   multiple parsers can coexist.  */
int yydebug;

#else /* !YYDEBUG */

#define YYDPRINTF(Args)
#define YY_SYMBOL_PRINT(Title, Type, Value, Location)

#endif /* !YYDEBUG */

/* YYINITDEPTH -- initial size of the parser's stacks.  */
#ifndef YYINITDEPTH
#define YYINITDEPTH 200
#endif

/* YYMAXDEPTH -- maximum size the stacks can grow to (effective only
   if the built-in stack extension method is used).

   Do not make this value too large; the results are undefined if
   SIZE_MAX < YYMAXDEPTH * sizeof (GLRStackItem)
   evaluated with infinite-precision integer arithmetic.  */

#ifndef YYMAXDEPTH
#define YYMAXDEPTH 10000
#endif

/* Minimum number of free items on the stack allowed after an
   allocation.  This is to allow allocation and initialization
   to be completed by functions that call yyexpandGLRStack before the
   stack is expanded, thus insuring that all necessary pointers get
   properly redirected to new data.  */
#define YYHEADROOM 2

#ifndef YYSTACKEXPANDABLE
#if (!defined __cplusplus || (defined YYSTYPE_IS_TRIVIAL && YYSTYPE_IS_TRIVIAL))
#define YYSTACKEXPANDABLE 1
#else
#define YYSTACKEXPANDABLE 0
#endif
#endif

#if YYSTACKEXPANDABLE
#define YY_RESERVE_GLRSTACK(Yystack)                                  \
  do {                                                                \
    if (Yystack->yyspaceLeft < YYHEADROOM) yyexpandGLRStack(Yystack); \
  } while (YYID(0))
#else
#define YY_RESERVE_GLRSTACK(Yystack)                                   \
  do {                                                                 \
    if (Yystack->yyspaceLeft < YYHEADROOM) yyMemoryExhausted(Yystack); \
  } while (YYID(0))
#endif

#if YYERROR_VERBOSE

#ifndef yystpcpy
#if defined __GLIBC__ && defined _STRING_H && defined _GNU_SOURCE
#define yystpcpy stpcpy
#else
/* Copy YYSRC to YYDEST, returning the address of the terminating '\0' in
   YYDEST.  */
static char *yystpcpy(char *yydest, const char *yysrc) {
  char *yyd = yydest;
  const char *yys = yysrc;

  while ((*yyd++ = *yys++) != '\0') continue;

  return yyd - 1;
}
#endif
#endif

#ifndef yytnamerr
/* Copy to YYRES the contents of YYSTR after stripping away unnecessary
   quotes and backslashes, so that it's suitable for yyerror.  The
   heuristic is that double-quoting is unnecessary unless the string
   contains an apostrophe, a comma, or backslash (other than
   backslash-backslash).  YYSTR is taken from yytname.  If YYRES is
   null, do not copy; instead, return the length of what the result
   would have been.  */
static size_t yytnamerr(char *yyres, const char *yystr) {
  if (*yystr == '"') {
    size_t yyn = 0;
    char const *yyp = yystr;

    for (;;) switch (*++yyp) {
        case '\'':
        case ',':
          goto do_not_strip_quotes;

        case '\\':
          if (*++yyp != '\\') goto do_not_strip_quotes;
        /* Fall through.  */
        default:
          if (yyres) yyres[yyn] = *yyp;
          yyn++;
          break;

        case '"':
          if (yyres) yyres[yyn] = '\0';
          return yyn;
      }
  do_not_strip_quotes:
    ;
  }

  if (!yyres) return strlen(yystr);

  return yystpcpy(yyres, yystr) - yyres;
}
#endif

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
  yyGLRState *yypred;
  /** Source position of the first token produced by my symbol */
  size_t yyposn;
  union {
    /** First in a chain of alternative reductions producing the
     *  non-terminal corresponding to this state, threaded through
     *  yynext.  */
    yySemanticOption *yyfirstVal;
    /** Semantic value for this state.  */
    YYSTYPE yysval;
  } yysemantics;
};

struct yyGLRStateSet {
  yyGLRState **yystates;
  /** During nondeterministic operation, yylookaheadNeeds tracks which
   *  stacks have actually needed the current lookahead.  During deterministic
   *  operation, yylookaheadNeeds[0] is not maintained since it would merely
   *  duplicate yychar != YYEMPTY.  */
  yybool *yylookaheadNeeds;
  size_t yysize, yycapacity;
};

struct yySemanticOption {
  /** Type tag: always false.  */
  yybool yyisState;
  /** Rule number for this reduction */
  yyRuleNum yyrule;
  /** The last RHS state in the list of states to be reduced.  */
  yyGLRState *yystate;
  /** The lookahead for this reduction.  */
  int yyrawchar;
  YYSTYPE yyval;
  /** Next sibling in chain of options.  To facilitate merging,
   *  options are chained in decreasing order by address.  */
  yySemanticOption *yynext;
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
  yyGLRStackItem *yyitems;
  yyGLRStackItem *yynextFree;
  size_t yyspaceLeft;
  yyGLRState *yysplitPoint;
  yyGLRState *yylastDeleted;
  yyGLRStateSet yytops;
};

#if YYSTACKEXPANDABLE
static void yyexpandGLRStack(yyGLRStack *yystackp);
#endif

static void yyFail(yyGLRStack *yystackp, struct ParseResult *result,
                   const char *yymsg) __attribute__((__noreturn__));
static void yyFail(yyGLRStack *yystackp, struct ParseResult *result,
                   const char *yymsg) {
  if (yymsg != YY_NULL) yyerror(result, yymsg);
  YYLONGJMP(yystackp->yyexception_buffer, 1);
}

static void yyMemoryExhausted(yyGLRStack *yystackp)
    __attribute__((__noreturn__));
static void yyMemoryExhausted(yyGLRStack *yystackp) {
  YYLONGJMP(yystackp->yyexception_buffer, 2);
}

#if YYDEBUG || YYERROR_VERBOSE
/** A printable representation of TOKEN.  */
static inline const char *yytokenName(yySymbol yytoken) {
  if (yytoken == YYEMPTY) return "";

  return yytname[yytoken];
}
#endif

/** Fill in YYVSP[YYLOW1 .. YYLOW0-1] from the chain of states starting
 *  at YYVSP[YYLOW0].yystate.yypred.  Leaves YYVSP[YYLOW1].yystate.yypred
 *  containing the pointer to the next state in the chain.  */
static void yyfillin(yyGLRStackItem *, int, int) __attribute__((__unused__));
static void yyfillin(yyGLRStackItem *yyvsp, int yylow0, int yylow1) {
  int i;
  yyGLRState *s = yyvsp[yylow0].yystate.yypred;
  for (i = yylow0 - 1; i >= yylow1; i -= 1) {
    YYASSERT(s->yyresolved);
    yyvsp[i].yystate.yyresolved = yytrue;
    yyvsp[i].yystate.yysemantics.yysval = s->yysemantics.yysval;
    s = yyvsp[i].yystate.yypred = s->yypred;
  }
}

/* Do nothing if YYNORMAL or if *YYLOW <= YYLOW1.  Otherwise, fill in
 * YYVSP[YYLOW1 .. *YYLOW-1] as in yyfillin and set *YYLOW = YYLOW1.
 * For convenience, always return YYLOW1.  */
static inline int yyfill(yyGLRStackItem *, int *, int, yybool)
    __attribute__((__unused__));
static inline int yyfill(yyGLRStackItem *yyvsp, int *yylow, int yylow1,
                         yybool yynormal) {
  if (!yynormal && yylow1 < *yylow) {
    yyfillin(yyvsp, *yylow, yylow1);
    *yylow = yylow1;
  }
  return yylow1;
}

/** Perform user action for rule number YYN, with RHS length YYRHSLEN,
 *  and top stack item YYVSP.  YYLVALP points to place to put semantic
 *  value ($$), and yylocp points to place for location information
 *  (@$).  Returns yyok for normal return, yyaccept for YYACCEPT,
 *  yyerr for YYERROR, yyabort for YYABORT.  */
/*ARGSUSED*/ static YYRESULTTAG yyuserAction(yyRuleNum yyn, int yyrhslen,
                                             yyGLRStackItem *yyvsp,
                                             yyGLRStack *yystackp,
                                             YYSTYPE *yyvalp,
                                             struct ParseResult *result) {
  yybool yynormal __attribute__((__unused__)) =
      (yystackp->yysplitPoint == YY_NULL);
  int yylow;
  YYUSE(result);
#undef yyerrok
#define yyerrok (yystackp->yyerrState = 0)
#undef YYACCEPT
#define YYACCEPT return yyaccept
#undef YYABORT
#define YYABORT return yyabort
#undef YYERROR
#define YYERROR return yyerrok, yyerr
#undef YYRECOVERING
#define YYRECOVERING() (yystackp->yyerrState != 0)
#undef yyclearin
#define yyclearin (yychar = YYEMPTY)
#undef YYFILL
#define YYFILL(N) yyfill(yyvsp, &yylow, N, yynormal)
#undef YYBACKUP
#define YYBACKUP(Token, Value) \
  return yyerror(result, YY_("syntax error: cannot back up")), yyerrok, yyerr

  yylow = 1;
  if (yyrhslen == 0)
    *yyvalp = yyval_default;
  else
    *yyvalp = yyvsp[YYFILL(1 - yyrhslen)].yystate.yysemantics.yysval;
  switch (yyn) {
    case 2:
/* Line 868 of glr.c  */
#line 436 "sql.ypp"
    {
      printf("> \n");
      ((*yyvalp).ast_node) = new AstStmtList(
          AST_STMT_LIST, (((yyGLRStackItem const *)yyvsp)[YYFILL((1) - (2))]
                              .yystate.yysemantics.yysval.ast_node),
          NULL);
      if (result->error_number == 0) {
        result->ast = ((*yyvalp).ast_node);
      } else {
        result->ast = NULL;
      }

    } break;

    case 3:
/* Line 868 of glr.c  */
#line 450 "sql.ypp"
    {
      printf(">> \n");
      ((*yyvalp).ast_node) = new AstStmtList(
          AST_STMT_LIST, (((yyGLRStackItem const *)yyvsp)[YYFILL((2) - (3))]
                              .yystate.yysemantics.yysval.ast_node),
          (((yyGLRStackItem const *)
            yyvsp)[YYFILL((1) - (3))].yystate.yysemantics.yysval.ast_node));
      if (result->error_number == 0) {
        result->ast = ((*yyvalp).ast_node);
      } else {
        result->ast = NULL;
      }
    } break;

    case 4:
/* Line 868 of glr.c  */
#line 463 "sql.ypp"
    {
      printf(">> \n");
      result->ast = NULL;
      result->error_number++;
      // yyerror(result,"First statement discarded, input new statement");
      yyclearin;
      yyerrok;
    } break;

    case 5:
/* Line 868 of glr.c  */
#line 472 "sql.ypp"
    {
      printf(">> \n");
      result->ast = NULL;
      result->error_number++;
      yyclearin;
      yyerrok;
    } break;

    case 6:
/* Line 868 of glr.c  */
#line 483 "sql.ypp"
    {
      ((*yyvalp).ast_node) = (((yyGLRStackItem const *)yyvsp)[YYFILL((1) - (1))]
                                  .yystate.yysemantics.yysval.ast_node);
    } break;

    case 7:
/* Line 868 of glr.c  */
#line 488 "sql.ypp"
    {
      ((*yyvalp).ast_node) = new AstSelectStmt(
          AST_SELECT_STMT, (((yyGLRStackItem const *)yyvsp)[YYFILL((2) - (3))]
                                .yystate.yysemantics.yysval.intval),
          (((yyGLRStackItem const *)
            yyvsp)[YYFILL((3) - (3))].yystate.yysemantics.yysval.ast_node),
          NULL, NULL, NULL, NULL, NULL, NULL, NULL);
    } break;

    case 8:
/* Line 868 of glr.c  */
#line 493 "sql.ypp"
    {
      ((*yyvalp).ast_node) = new AstSelectStmt(
          AST_SELECT_STMT, (((yyGLRStackItem const *)yyvsp)[YYFILL((2) - (11))]
                                .yystate.yysemantics.yysval.intval),
          (((yyGLRStackItem const *)
            yyvsp)[YYFILL((3) - (11))].yystate.yysemantics.yysval.ast_node),
          (((yyGLRStackItem const *)
            yyvsp)[YYFILL((5) - (11))].yystate.yysemantics.yysval.ast_node),
          (((yyGLRStackItem const *)
            yyvsp)[YYFILL((6) - (11))].yystate.yysemantics.yysval.ast_node),
          (((yyGLRStackItem const *)
            yyvsp)[YYFILL((7) - (11))].yystate.yysemantics.yysval.ast_node),
          (((yyGLRStackItem const *)
            yyvsp)[YYFILL((8) - (11))].yystate.yysemantics.yysval.ast_node),
          (((yyGLRStackItem const *)
            yyvsp)[YYFILL((9) - (11))].yystate.yysemantics.yysval.ast_node),
          (((yyGLRStackItem const *)
            yyvsp)[YYFILL((10) - (11))].yystate.yysemantics.yysval.ast_node),
          (((yyGLRStackItem const *)
            yyvsp)[YYFILL((11) - (11))].yystate.yysemantics.yysval.ast_node));
    } break;

    case 9:
/* Line 868 of glr.c  */
#line 498 "sql.ypp"
    {
      ((*yyvalp).ast_node) = NULL;
    } break;

    case 10:
/* Line 868 of glr.c  */
#line 499 "sql.ypp"
    {
      ((*yyvalp).ast_node) = new AstWhereClause(
          AST_WHERE_CLAUSE, (((yyGLRStackItem const *)yyvsp)[YYFILL((2) - (2))]
                                 .yystate.yysemantics.yysval.ast_node));
    } break;

    case 11:
/* Line 868 of glr.c  */
#line 502 "sql.ypp"
    {
      ((*yyvalp).ast_node) = NULL;
    } break;

    case 12:
/* Line 868 of glr.c  */
#line 503 "sql.ypp"
    {
      ((*yyvalp).ast_node) = new AstGroupByClause(
          AST_GROUPBY_CLAUSE,
          (((yyGLRStackItem const *)
            yyvsp)[YYFILL((3) - (4))].yystate.yysemantics.yysval.ast_node),
          (((yyGLRStackItem const *)
            yyvsp)[YYFILL((4) - (4))].yystate.yysemantics.yysval.intval));
    } break;

    case 13:
/* Line 868 of glr.c  */
#line 508 "sql.ypp"
    {
      ((*yyvalp).ast_node) = new AstGroupByList(
          AST_GROUPBY_LIST, (((yyGLRStackItem const *)yyvsp)[YYFILL((1) - (1))]
                                 .yystate.yysemantics.yysval.ast_node),
          NULL);
    } break;

    case 14:
/* Line 868 of glr.c  */
#line 509 "sql.ypp"
    {
      ((*yyvalp).ast_node) = new AstGroupByList(
          AST_GROUPBY_LIST, (((yyGLRStackItem const *)yyvsp)[YYFILL((1) - (3))]
                                 .yystate.yysemantics.yysval.ast_node),
          (((yyGLRStackItem const *)
            yyvsp)[YYFILL((3) - (3))].yystate.yysemantics.yysval.ast_node));
    } break;

    case 15:
/* Line 868 of glr.c  */
#line 511 "sql.ypp"
    {
      ((*yyvalp).intval) = 0;
    } break;

    case 16:
/* Line 868 of glr.c  */
#line 512 "sql.ypp"
    {
      ((*yyvalp).intval) = 0;
    } break;

    case 17:
/* Line 868 of glr.c  */
#line 513 "sql.ypp"
    {
      ((*yyvalp).intval) = 1;
    } break;

    case 18:
/* Line 868 of glr.c  */
#line 516 "sql.ypp"
    {
      ((*yyvalp).intval) = 0;
    } break;

    case 19:
/* Line 868 of glr.c  */
#line 517 "sql.ypp"
    {
      ((*yyvalp).intval) = 1;
    } break;

    case 20:
/* Line 868 of glr.c  */
#line 520 "sql.ypp"
    {
      ((*yyvalp).ast_node) = NULL;
    } break;

    case 21:
/* Line 868 of glr.c  */
#line 521 "sql.ypp"
    {
      ((*yyvalp).ast_node) = new AstHavingClause(
          AST_HAVING_CLAUSE, (((yyGLRStackItem const *)yyvsp)[YYFILL((2) - (2))]
                                  .yystate.yysemantics.yysval.ast_node));
    } break;

    case 22:
/* Line 868 of glr.c  */
#line 524 "sql.ypp"
    {
      ((*yyvalp).ast_node) = NULL;
    } break;

    case 23:
/* Line 868 of glr.c  */
#line 525 "sql.ypp"
    {
      ((*yyvalp).ast_node) = new AstOrderByClause(
          AST_ORDERBY_CLAUSE,
          (((yyGLRStackItem const *)
            yyvsp)[YYFILL((3) - (3))].yystate.yysemantics.yysval.ast_node));
    } break;

    case 24:
/* Line 868 of glr.c  */
#line 528 "sql.ypp"
    {
      ((*yyvalp).ast_node) = new AstOrderByList(
          AST_ORDERBY_LIST, (((yyGLRStackItem const *)yyvsp)[YYFILL((1) - (2))]
                                 .yystate.yysemantics.yysval.ast_node),
          (((yyGLRStackItem const *)
            yyvsp)[YYFILL((2) - (2))].yystate.yysemantics.yysval.intval),
          NULL);
    } break;

    case 25:
/* Line 868 of glr.c  */
#line 529 "sql.ypp"
    {
      ((*yyvalp).ast_node) = new AstOrderByList(
          AST_ORDERBY_LIST, (((yyGLRStackItem const *)yyvsp)[YYFILL((1) - (4))]
                                 .yystate.yysemantics.yysval.ast_node),
          (((yyGLRStackItem const *)
            yyvsp)[YYFILL((2) - (4))].yystate.yysemantics.yysval.intval),
          (((yyGLRStackItem const *)
            yyvsp)[YYFILL((4) - (4))].yystate.yysemantics.yysval.ast_node));
    } break;

    case 26:
/* Line 868 of glr.c  */
#line 531 "sql.ypp"
    {
      ((*yyvalp).ast_node) = NULL;
    } break;

    case 27:
/* Line 868 of glr.c  */
#line 532 "sql.ypp"
    {
      ((*yyvalp).ast_node) = new AstLimitClause(
          AST_LIMIT_CLAUSE, NULL,
          (((yyGLRStackItem const *)
            yyvsp)[YYFILL((2) - (2))].yystate.yysemantics.yysval.ast_node));
    } break;

    case 28:
/* Line 868 of glr.c  */
#line 533 "sql.ypp"
    {
      ((*yyvalp).ast_node) = new AstLimitClause(
          AST_LIMIT_CLAUSE, (((yyGLRStackItem const *)yyvsp)[YYFILL((2) - (4))]
                                 .yystate.yysemantics.yysval.ast_node),
          (((yyGLRStackItem const *)
            yyvsp)[YYFILL((4) - (4))].yystate.yysemantics.yysval.ast_node));
    } break;

    case 29:
/* Line 868 of glr.c  */
#line 536 "sql.ypp"
    {
      ((*yyvalp).ast_node) = NULL;
    } break;

    case 30:
/* Line 868 of glr.c  */
#line 537 "sql.ypp"
    {
      ((*yyvalp).ast_node) = NULL;
    } break;

    case 31:
/* Line 868 of glr.c  */
#line 541 "sql.ypp"
    {
      ((*yyvalp).ast_node) = new AstColumn(
          AST_COLUMN, string("NULL"),
          string((((yyGLRStackItem const *)yyvsp)[YYFILL((1) - (1))]
                      .yystate.yysemantics.yysval.strval)));
    } break;

    case 32:
/* Line 868 of glr.c  */
#line 542 "sql.ypp"
    {
      ((*yyvalp).ast_node) = new AstColumn(
          AST_COLUMN, string("NULL"),
          string((((yyGLRStackItem const *)yyvsp)[YYFILL((1) - (3))]
                      .yystate.yysemantics.yysval.strval)),
          (((yyGLRStackItem const *)
            yyvsp)[YYFILL((3) - (3))].yystate.yysemantics.yysval.ast_node));
    } break;

    case 33:
/* Line 868 of glr.c  */
#line 545 "sql.ypp"
    {
      ((*yyvalp).intval) = 0;
    } break;

    case 34:
/* Line 868 of glr.c  */
#line 546 "sql.ypp"
    {
      if (((*yyvalp).intval) & 1) yyerror(result, "duplicate ALL option");
      ((*yyvalp).intval) = (((yyGLRStackItem const *)yyvsp)[YYFILL((1) - (2))]
                                .yystate.yysemantics.yysval.intval) |
                           1;
    } break;

    case 35:
/* Line 868 of glr.c  */
#line 547 "sql.ypp"
    {
      if (((*yyvalp).intval) & 2) yyerror(result, "duplicate DISTINCT option");
      ((*yyvalp).intval) = (((yyGLRStackItem const *)yyvsp)[YYFILL((1) - (2))]
                                .yystate.yysemantics.yysval.intval) |
                           2;
    } break;

    case 36:
/* Line 868 of glr.c  */
#line 548 "sql.ypp"
    {
      if (((*yyvalp).intval) & 4)
        yyerror(result, "duplicate DISTINCTROW option");
      ((*yyvalp).intval) = (((yyGLRStackItem const *)yyvsp)[YYFILL((1) - (2))]
                                .yystate.yysemantics.yysval.intval) |
                           4;
    } break;

    case 37:
/* Line 868 of glr.c  */
#line 549 "sql.ypp"
    {
      if (((*yyvalp).intval) & 8)
        yyerror(result, "duplicate HIGH_PRIORITY option");
      ((*yyvalp).intval) = (((yyGLRStackItem const *)yyvsp)[YYFILL((1) - (2))]
                                .yystate.yysemantics.yysval.intval) |
                           8;
    } break;

    case 38:
/* Line 868 of glr.c  */
#line 550 "sql.ypp"
    {
      if (((*yyvalp).intval) & 16)
        yyerror(result, "duplicate STRAIGHT_JOIN option");
      ((*yyvalp).intval) = (((yyGLRStackItem const *)yyvsp)[YYFILL((1) - (2))]
                                .yystate.yysemantics.yysval.intval) |
                           16;
    } break;

    case 39:
/* Line 868 of glr.c  */
#line 551 "sql.ypp"
    {
      if (((*yyvalp).intval) & 32)
        yyerror(result, "duplicate SQL_SMALL_RESULT option");
      ((*yyvalp).intval) = (((yyGLRStackItem const *)yyvsp)[YYFILL((1) - (2))]
                                .yystate.yysemantics.yysval.intval) |
                           32;
    } break;

    case 40:
/* Line 868 of glr.c  */
#line 552 "sql.ypp"
    {
      if (((*yyvalp).intval) & 64)
        yyerror(result, "duplicate SQL_BIG_RESULT option");
      ((*yyvalp).intval) = (((yyGLRStackItem const *)yyvsp)[YYFILL((1) - (2))]
                                .yystate.yysemantics.yysval.intval) |
                           64;
    } break;

    case 41:
/* Line 868 of glr.c  */
#line 553 "sql.ypp"
    {
      if (((*yyvalp).intval) & 128)
        yyerror(result, "duplicate SQL_CALC_FOUND_ROWS option");
      ((*yyvalp).intval) = (((yyGLRStackItem const *)yyvsp)[YYFILL((1) - (2))]
                                .yystate.yysemantics.yysval.intval) |
                           128;
    } break;

    case 42:
/* Line 868 of glr.c  */
#line 557 "sql.ypp"
    {
      ((*yyvalp).ast_node) = new AstSelectList(
          AST_SELECT_LIST, 0,
          (((yyGLRStackItem const *)
            yyvsp)[YYFILL((1) - (1))].yystate.yysemantics.yysval.ast_node),
          NULL);
    } break;

    case 43:
/* Line 868 of glr.c  */
#line 558 "sql.ypp"
    {
      ((*yyvalp).ast_node) = new AstSelectList(
          AST_SELECT_LIST, 0,
          (((yyGLRStackItem const *)
            yyvsp)[YYFILL((1) - (3))].yystate.yysemantics.yysval.ast_node),
          (((yyGLRStackItem const *)
            yyvsp)[YYFILL((3) - (3))].yystate.yysemantics.yysval.ast_node));
    } break;

    case 44:
/* Line 868 of glr.c  */
#line 559 "sql.ypp"
    {
      ((*yyvalp).ast_node) = new AstSelectList(AST_SELECT_LIST, 1, NULL, NULL);
    } break;

    case 45:
/* Line 868 of glr.c  */
#line 563 "sql.ypp"
    {
      ((*yyvalp).ast_node) = new AstSelectExpr(
          AST_SELECT_EXPR,
          string((((yyGLRStackItem const *)yyvsp)[YYFILL((2) - (2))]
                      .yystate.yysemantics.yysval.strval)),
          (((yyGLRStackItem const *)
            yyvsp)[YYFILL((1) - (2))].yystate.yysemantics.yysval.ast_node));
    } break;

    case 46:
/* Line 868 of glr.c  */
#line 566 "sql.ypp"
    {
      ((*yyvalp).ast_node) = new AstFromList(
          AST_FROM_LIST, (((yyGLRStackItem const *)yyvsp)[YYFILL((1) - (1))]
                              .yystate.yysemantics.yysval.ast_node),
          NULL);
    } break;

    case 47:
/* Line 868 of glr.c  */
#line 567 "sql.ypp"
    {
      ((*yyvalp).ast_node) = new AstFromList(
          AST_FROM_LIST, (((yyGLRStackItem const *)yyvsp)[YYFILL((1) - (3))]
                              .yystate.yysemantics.yysval.ast_node),
          (((yyGLRStackItem const *)
            yyvsp)[YYFILL((3) - (3))].yystate.yysemantics.yysval.ast_node));
    } break;

    case 48:
/* Line 868 of glr.c  */
#line 571 "sql.ypp"
    {
      ((*yyvalp).ast_node) = (((yyGLRStackItem const *)yyvsp)[YYFILL((1) - (1))]
                                  .yystate.yysemantics.yysval.ast_node);
    } break;

    case 49:
/* Line 868 of glr.c  */
#line 572 "sql.ypp"
    {
      ((*yyvalp).ast_node) = (((yyGLRStackItem const *)yyvsp)[YYFILL((1) - (1))]
                                  .yystate.yysemantics.yysval.ast_node);
    } break;

    case 50:
/* Line 868 of glr.c  */
#line 577 "sql.ypp"
    {
      ((*yyvalp).ast_node) = new AstTable(
          AST_TABLE, string("NULL"),
          string((((yyGLRStackItem const *)yyvsp)[YYFILL((1) - (2))]
                      .yystate.yysemantics.yysval.strval)),
          string((((yyGLRStackItem const *)yyvsp)[YYFILL((2) - (2))]
                      .yystate.yysemantics.yysval.strval)));
    } break;

    case 51:
/* Line 868 of glr.c  */
#line 579 "sql.ypp"
    {
      ((*yyvalp).ast_node) = new AstTable(
          AST_TABLE, string((((yyGLRStackItem const *)yyvsp)[YYFILL((1) - (4))]
                                 .yystate.yysemantics.yysval.strval)),
          string((((yyGLRStackItem const *)yyvsp)[YYFILL((3) - (4))]
                      .yystate.yysemantics.yysval.strval)),
          string((((yyGLRStackItem const *)yyvsp)[YYFILL((4) - (4))]
                      .yystate.yysemantics.yysval.strval)));
    } break;

    case 52:
/* Line 868 of glr.c  */
#line 580 "sql.ypp"
    {
      ((*yyvalp).ast_node) = new AstSubquery(
          AST_SUBQUERY,
          string((((yyGLRStackItem const *)yyvsp)[YYFILL((3) - (3))]
                      .yystate.yysemantics.yysval.strval)),
          (((yyGLRStackItem const *)
            yyvsp)[YYFILL((1) - (3))].yystate.yysemantics.yysval.ast_node));
    } break;

    case 53:
/* Line 868 of glr.c  */
#line 581 "sql.ypp"
    {
      ((*yyvalp).ast_node) = (((yyGLRStackItem const *)yyvsp)[YYFILL((2) - (3))]
                                  .yystate.yysemantics.yysval.ast_node);
    } break;

    case 56:
/* Line 868 of glr.c  */
#line 588 "sql.ypp"
    {
      ((*yyvalp).strval) = (((yyGLRStackItem const *)yyvsp)[YYFILL((2) - (2))]
                                .yystate.yysemantics.yysval.strval);
    } break;

    case 57:
/* Line 868 of glr.c  */
#line 589 "sql.ypp"
    {
      ((*yyvalp).strval) = (((yyGLRStackItem const *)yyvsp)[YYFILL((1) - (1))]
                                .yystate.yysemantics.yysval.strval);
    } break;

    case 58:
/* Line 868 of glr.c  */
#line 590 "sql.ypp"
    {
      ((*yyvalp).strval) = "NULL";
    } break;

    case 59:
/* Line 868 of glr.c  */
#line 604 "sql.ypp"
    {
      ((*yyvalp).ast_node) = new AstJoin(
          AST_JOIN, (((yyGLRStackItem const *)yyvsp)[YYFILL((2) - (5))]
                         .yystate.yysemantics.yysval.intval),
          (((yyGLRStackItem const *)
            yyvsp)[YYFILL((1) - (5))].yystate.yysemantics.yysval.ast_node),
          (((yyGLRStackItem const *)
            yyvsp)[YYFILL((4) - (5))].yystate.yysemantics.yysval.ast_node),
          (((yyGLRStackItem const *)
            yyvsp)[YYFILL((5) - (5))].yystate.yysemantics.yysval.ast_node));
    } break;

    case 60:
/* Line 868 of glr.c  */
#line 605 "sql.ypp"
    {
      ((*yyvalp).ast_node) = new AstJoin(
          AST_JOIN, -1, (((yyGLRStackItem const *)yyvsp)[YYFILL((1) - (3))]
                             .yystate.yysemantics.yysval.ast_node),
          (((yyGLRStackItem const *)
            yyvsp)[YYFILL((3) - (3))].yystate.yysemantics.yysval.ast_node),
          NULL);
    } break;

    case 61:
/* Line 868 of glr.c  */
#line 606 "sql.ypp"
    {
      ((*yyvalp).ast_node) = new AstJoin(
          AST_JOIN, -1, (((yyGLRStackItem const *)yyvsp)[YYFILL((1) - (5))]
                             .yystate.yysemantics.yysval.ast_node),
          (((yyGLRStackItem const *)
            yyvsp)[YYFILL((3) - (5))].yystate.yysemantics.yysval.ast_node),
          (((yyGLRStackItem const *)
            yyvsp)[YYFILL((5) - (5))].yystate.yysemantics.yysval.ast_node));
    } break;

    case 62:
/* Line 868 of glr.c  */
#line 607 "sql.ypp"
    {
      ((*yyvalp).ast_node) = new AstJoin(
          AST_JOIN,
          (((yyGLRStackItem const *)
            yyvsp)[YYFILL((2) - (6))].yystate.yysemantics.yysval.intval) +
              (((yyGLRStackItem const *)
                yyvsp)[YYFILL((3) - (6))].yystate.yysemantics.yysval.intval),
          (((yyGLRStackItem const *)
            yyvsp)[YYFILL((1) - (6))].yystate.yysemantics.yysval.ast_node),
          (((yyGLRStackItem const *)
            yyvsp)[YYFILL((5) - (6))].yystate.yysemantics.yysval.ast_node),
          (((yyGLRStackItem const *)
            yyvsp)[YYFILL((6) - (6))].yystate.yysemantics.yysval.ast_node));
    } break;

    case 63:
/* Line 868 of glr.c  */
#line 608 "sql.ypp"
    {
      ((*yyvalp).ast_node) = new AstJoin(
          AST_JOIN, 32 + (((yyGLRStackItem const *)yyvsp)[YYFILL((3) - (5))]
                              .yystate.yysemantics.yysval.intval),
          (((yyGLRStackItem const *)
            yyvsp)[YYFILL((1) - (5))].yystate.yysemantics.yysval.ast_node),
          (((yyGLRStackItem const *)
            yyvsp)[YYFILL((5) - (5))].yystate.yysemantics.yysval.ast_node),
          NULL);
    } break;

    case 64:
/* Line 868 of glr.c  */
#line 611 "sql.ypp"
    {
      ((*yyvalp).intval) = 0;
    } break;

    case 65:
/* Line 868 of glr.c  */
#line 612 "sql.ypp"
    {
      ((*yyvalp).intval) = 1;
    } break;

    case 66:
/* Line 868 of glr.c  */
#line 613 "sql.ypp"
    {
      ((*yyvalp).intval) = 2;
    } break;

    case 67:
/* Line 868 of glr.c  */
#line 616 "sql.ypp"
    {
      ((*yyvalp).intval) = 4;
    } break;

    case 68:
/* Line 868 of glr.c  */
#line 617 "sql.ypp"
    {
      ((*yyvalp).intval) = 4;
    } break;

    case 69:
/* Line 868 of glr.c  */
#line 621 "sql.ypp"
    {
      ((*yyvalp).intval) = 8;
    } break;

    case 70:
/* Line 868 of glr.c  */
#line 622 "sql.ypp"
    {
      ((*yyvalp).intval) = 16;
    } break;

    case 71:
/* Line 868 of glr.c  */
#line 626 "sql.ypp"
    {
      ((*yyvalp).intval) =
          8 + (((yyGLRStackItem const *)
                yyvsp)[YYFILL((2) - (2))].yystate.yysemantics.yysval.intval);
    } break;

    case 72:
/* Line 868 of glr.c  */
#line 627 "sql.ypp"
    {
      ((*yyvalp).intval) =
          16 + (((yyGLRStackItem const *)
                 yyvsp)[YYFILL((2) - (2))].yystate.yysemantics.yysval.intval);
    } break;

    case 73:
/* Line 868 of glr.c  */
#line 628 "sql.ypp"
    {
      ((*yyvalp).intval) = 0;
    } break;

    case 74:
/* Line 868 of glr.c  */
#line 631 "sql.ypp"
    {
      ((*yyvalp).ast_node) = NULL;
    } break;

    case 75:
/* Line 868 of glr.c  */
#line 632 "sql.ypp"
    {
      ((*yyvalp).ast_node) = (((yyGLRStackItem const *)yyvsp)[YYFILL((1) - (1))]
                                  .yystate.yysemantics.yysval.ast_node);
    } break;

    case 76:
/* Line 868 of glr.c  */
#line 640 "sql.ypp"
    {
      ((*yyvalp).ast_node) = new AstJoinCondition(
          AST_JOIN_CONDITION, "ON",
          (((yyGLRStackItem const *)
            yyvsp)[YYFILL((2) - (2))].yystate.yysemantics.yysval.ast_node));
    } break;

    case 77:
/* Line 868 of glr.c  */
#line 641 "sql.ypp"
    {
      ((*yyvalp).ast_node) = new AstJoinCondition(
          AST_JOIN_CONDITION, "USING",
          (((yyGLRStackItem const *)
            yyvsp)[YYFILL((3) - (4))].yystate.yysemantics.yysval.ast_node));
    } break;

    case 78:
/* Line 868 of glr.c  */
#line 646 "sql.ypp"
    {
      ((*yyvalp).ast_node) = (((yyGLRStackItem const *)yyvsp)[YYFILL((2) - (3))]
                                  .yystate.yysemantics.yysval.ast_node);
    } break;

    case 79:
/* Line 868 of glr.c  */
#line 653 "sql.ypp"
    {
      ((*yyvalp).ast_node) = new AstColumn(
          AST_COLUMN, string("NULL"),
          string((((yyGLRStackItem const *)yyvsp)[YYFILL((1) - (1))]
                      .yystate.yysemantics.yysval.strval)));
    } break;

    case 80:
/* Line 868 of glr.c  */
#line 654 "sql.ypp"
    {
      ((*yyvalp).ast_node) = new AstExprConst(
          AST_EXPR_CONST, "CONST",
          string((((yyGLRStackItem const *)yyvsp)[YYFILL((1) - (1))]
                      .yystate.yysemantics.yysval.strval)));
    } break;

    case 81:
/* Line 868 of glr.c  */
#line 655 "sql.ypp"
    {
      ((*yyvalp).ast_node) = new AstColumn(
          AST_COLUMN, string((((yyGLRStackItem const *)yyvsp)[YYFILL((1) - (3))]
                                  .yystate.yysemantics.yysval.strval)),
          string((((yyGLRStackItem const *)yyvsp)[YYFILL((3) - (3))]
                      .yystate.yysemantics.yysval.strval)));
    } break;

    case 82:
/* Line 868 of glr.c  */
#line 656 "sql.ypp"
    {
      ((*yyvalp).ast_node) = new AstColumn(
          AST_COLUMN_ALL,
          string((((yyGLRStackItem const *)yyvsp)[YYFILL((1) - (3))]
                      .yystate.yysemantics.yysval.strval)),
          string("*"));
    } break;

    case 83:
/* Line 868 of glr.c  */
#line 657 "sql.ypp"
    {
      ((*yyvalp).ast_node) = new AstExprConst(
          AST_EXPR_CONST, "CONST_STRING",
          string((((yyGLRStackItem const *)yyvsp)[YYFILL((1) - (1))]
                      .yystate.yysemantics.yysval.strval)));
    } break;

    case 84:
/* Line 868 of glr.c  */
#line 658 "sql.ypp"
    {
      ((*yyvalp).ast_node) = new AstExprConst(
          AST_EXPR_CONST, "CONST_INT",
          string((((yyGLRStackItem const *)yyvsp)[YYFILL((1) - (1))]
                      .yystate.yysemantics.yysval.strval)));
    } break;

    case 85:
/* Line 868 of glr.c  */
#line 659 "sql.ypp"
    {
      ((*yyvalp).ast_node) = new AstExprConst(
          AST_EXPR_CONST, "CONST_DOUBLE",
          string((((yyGLRStackItem const *)yyvsp)[YYFILL((1) - (1))]
                      .yystate.yysemantics.yysval.strval)));
    } break;

    case 86:
/* Line 868 of glr.c  */
#line 660 "sql.ypp"
    {
      ((*yyvalp).ast_node) = new AstExprConst(
          AST_EXPR_CONST, "CONST_BOOL",
          string((((yyGLRStackItem const *)yyvsp)[YYFILL((1) - (1))]
                      .yystate.yysemantics.yysval.strval)));
    } break;

    case 87:
/* Line 868 of glr.c  */
#line 662 "sql.ypp"
    {
      ((*yyvalp).ast_node) = new AstExprCalBinary(
          AST_EXPR_CAL_BINARY, "+",
          (((yyGLRStackItem const *)
            yyvsp)[YYFILL((1) - (3))].yystate.yysemantics.yysval.ast_node),
          (((yyGLRStackItem const *)
            yyvsp)[YYFILL((3) - (3))].yystate.yysemantics.yysval.ast_node));
    } break;

    case 88:
/* Line 868 of glr.c  */
#line 663 "sql.ypp"
    {
      ((*yyvalp).ast_node) = new AstExprCalBinary(
          AST_EXPR_CAL_BINARY, "-",
          (((yyGLRStackItem const *)
            yyvsp)[YYFILL((1) - (3))].yystate.yysemantics.yysval.ast_node),
          (((yyGLRStackItem const *)
            yyvsp)[YYFILL((3) - (3))].yystate.yysemantics.yysval.ast_node));
    } break;

    case 89:
/* Line 868 of glr.c  */
#line 664 "sql.ypp"
    {
      ((*yyvalp).ast_node) = new AstExprCalBinary(
          AST_EXPR_CAL_BINARY, "*",
          (((yyGLRStackItem const *)
            yyvsp)[YYFILL((1) - (3))].yystate.yysemantics.yysval.ast_node),
          (((yyGLRStackItem const *)
            yyvsp)[YYFILL((3) - (3))].yystate.yysemantics.yysval.ast_node));
    } break;

    case 90:
/* Line 868 of glr.c  */
#line 665 "sql.ypp"
    {
      ((*yyvalp).ast_node) = new AstExprCalBinary(
          AST_EXPR_CAL_BINARY, "/",
          (((yyGLRStackItem const *)
            yyvsp)[YYFILL((1) - (3))].yystate.yysemantics.yysval.ast_node),
          (((yyGLRStackItem const *)
            yyvsp)[YYFILL((3) - (3))].yystate.yysemantics.yysval.ast_node));
    } break;

    case 91:
/* Line 868 of glr.c  */
#line 666 "sql.ypp"
    {
      ((*yyvalp).ast_node) = new AstExprCalBinary(
          AST_EXPR_CAL_BINARY, "MOD",
          (((yyGLRStackItem const *)
            yyvsp)[YYFILL((1) - (3))].yystate.yysemantics.yysval.ast_node),
          (((yyGLRStackItem const *)
            yyvsp)[YYFILL((3) - (3))].yystate.yysemantics.yysval.ast_node));
    } break;

    case 92:
/* Line 868 of glr.c  */
#line 667 "sql.ypp"
    {
      ((*yyvalp).ast_node) = new AstExprCalBinary(
          AST_EXPR_CAL_BINARY, "%",
          (((yyGLRStackItem const *)
            yyvsp)[YYFILL((1) - (3))].yystate.yysemantics.yysval.ast_node),
          (((yyGLRStackItem const *)
            yyvsp)[YYFILL((3) - (3))].yystate.yysemantics.yysval.ast_node));
    } break;

    case 93:
/* Line 868 of glr.c  */
#line 669 "sql.ypp"
    {
      ((*yyvalp).ast_node) = new AstExprUnary(
          AST_EXPR_UNARY, "-",
          (((yyGLRStackItem const *)
            yyvsp)[YYFILL((2) - (2))].yystate.yysemantics.yysval.ast_node));
    } break;

    case 94:
/* Line 868 of glr.c  */
#line 670 "sql.ypp"
    {
      ((*yyvalp).ast_node) = new AstExprUnary(
          AST_EXPR_UNARY, "+",
          (((yyGLRStackItem const *)
            yyvsp)[YYFILL((2) - (2))].yystate.yysemantics.yysval.ast_node));
    } break;

    case 95:
/* Line 868 of glr.c  */
#line 672 "sql.ypp"
    {
      ((*yyvalp).ast_node) = new AstExprCalBinary(
          AST_EXPR_BOOL_BINARY, "AND",
          (((yyGLRStackItem const *)
            yyvsp)[YYFILL((1) - (3))].yystate.yysemantics.yysval.ast_node),
          (((yyGLRStackItem const *)
            yyvsp)[YYFILL((3) - (3))].yystate.yysemantics.yysval.ast_node));
    } break;

    case 96:
/* Line 868 of glr.c  */
#line 673 "sql.ypp"
    {
      ((*yyvalp).ast_node) = new AstExprCalBinary(
          AST_EXPR_BOOL_BINARY, "OR",
          (((yyGLRStackItem const *)
            yyvsp)[YYFILL((1) - (3))].yystate.yysemantics.yysval.ast_node),
          (((yyGLRStackItem const *)
            yyvsp)[YYFILL((3) - (3))].yystate.yysemantics.yysval.ast_node));
    } break;

    case 97:
/* Line 868 of glr.c  */
#line 674 "sql.ypp"
    {
      ((*yyvalp).ast_node) = new AstExprCalBinary(
          AST_EXPR_BOOL_BINARY, "XOR",
          (((yyGLRStackItem const *)
            yyvsp)[YYFILL((1) - (3))].yystate.yysemantics.yysval.ast_node),
          (((yyGLRStackItem const *)
            yyvsp)[YYFILL((3) - (3))].yystate.yysemantics.yysval.ast_node));
    } break;

    case 98:
/* Line 868 of glr.c  */
#line 676 "sql.ypp"
    {
      ((*yyvalp).ast_node) = new AstExprCmpBinary(
          AST_EXPR_CMP_BINARY, "",
          (((yyGLRStackItem const *)
            yyvsp)[YYFILL((2) - (3))].yystate.yysemantics.yysval.subtok),
          (((yyGLRStackItem const *)
            yyvsp)[YYFILL((1) - (3))].yystate.yysemantics.yysval.ast_node),
          (((yyGLRStackItem const *)
            yyvsp)[YYFILL((3) - (3))].yystate.yysemantics.yysval.ast_node));
    } break;

    case 99:
/* Line 868 of glr.c  */
#line 677 "sql.ypp"
    {
      ((*yyvalp).ast_node) = new AstExprCmpBinary(
          AST_EXPR_CMP_BINARY, "SUBQUERY",
          (((yyGLRStackItem const *)
            yyvsp)[YYFILL((2) - (5))].yystate.yysemantics.yysval.subtok),
          (((yyGLRStackItem const *)
            yyvsp)[YYFILL((1) - (5))].yystate.yysemantics.yysval.ast_node),
          (((yyGLRStackItem const *)
            yyvsp)[YYFILL((4) - (5))].yystate.yysemantics.yysval.ast_node));
    } break;

    case 100:
/* Line 868 of glr.c  */
#line 678 "sql.ypp"
    {
      ((*yyvalp).ast_node) = new AstExprCmpBinary(
          AST_EXPR_CMP_BINARY, "ANY",
          (((yyGLRStackItem const *)
            yyvsp)[YYFILL((2) - (6))].yystate.yysemantics.yysval.subtok),
          (((yyGLRStackItem const *)
            yyvsp)[YYFILL((1) - (6))].yystate.yysemantics.yysval.ast_node),
          (((yyGLRStackItem const *)
            yyvsp)[YYFILL((5) - (6))].yystate.yysemantics.yysval.ast_node));
    } break;

    case 101:
/* Line 868 of glr.c  */
#line 679 "sql.ypp"
    {
      ((*yyvalp).ast_node) = new AstExprCmpBinary(
          AST_EXPR_CMP_BINARY, "SOME",
          (((yyGLRStackItem const *)
            yyvsp)[YYFILL((2) - (6))].yystate.yysemantics.yysval.subtok),
          (((yyGLRStackItem const *)
            yyvsp)[YYFILL((1) - (6))].yystate.yysemantics.yysval.ast_node),
          (((yyGLRStackItem const *)
            yyvsp)[YYFILL((5) - (6))].yystate.yysemantics.yysval.ast_node));
    } break;

    case 102:
/* Line 868 of glr.c  */
#line 680 "sql.ypp"
    {
      ((*yyvalp).ast_node) = new AstExprCmpBinary(
          AST_EXPR_CMP_BINARY, "ALL",
          (((yyGLRStackItem const *)
            yyvsp)[YYFILL((2) - (6))].yystate.yysemantics.yysval.subtok),
          (((yyGLRStackItem const *)
            yyvsp)[YYFILL((1) - (6))].yystate.yysemantics.yysval.ast_node),
          (((yyGLRStackItem const *)
            yyvsp)[YYFILL((5) - (6))].yystate.yysemantics.yysval.ast_node));
    } break;

    case 103:
/* Line 868 of glr.c  */
#line 682 "sql.ypp"
    {
      ((*yyvalp).ast_node) = new AstExprCalBinary(
          AST_EXPR_CAL_BINARY, "|",
          (((yyGLRStackItem const *)
            yyvsp)[YYFILL((1) - (3))].yystate.yysemantics.yysval.ast_node),
          (((yyGLRStackItem const *)
            yyvsp)[YYFILL((3) - (3))].yystate.yysemantics.yysval.ast_node));
    } break;

    case 104:
/* Line 868 of glr.c  */
#line 683 "sql.ypp"
    {
      ((*yyvalp).ast_node) = new AstExprCalBinary(
          AST_EXPR_CAL_BINARY, "&",
          (((yyGLRStackItem const *)
            yyvsp)[YYFILL((1) - (3))].yystate.yysemantics.yysval.ast_node),
          (((yyGLRStackItem const *)
            yyvsp)[YYFILL((3) - (3))].yystate.yysemantics.yysval.ast_node));
    } break;

    case 105:
/* Line 868 of glr.c  */
#line 684 "sql.ypp"
    {
      ((*yyvalp).ast_node) = new AstExprCalBinary(
          AST_EXPR_CAL_BINARY, "^",
          (((yyGLRStackItem const *)
            yyvsp)[YYFILL((1) - (3))].yystate.yysemantics.yysval.ast_node),
          (((yyGLRStackItem const *)
            yyvsp)[YYFILL((3) - (3))].yystate.yysemantics.yysval.ast_node));
    } break;

    case 106:
/* Line 868 of glr.c  */
#line 685 "sql.ypp"
    {
      ((*yyvalp).ast_node) = new AstExprCalBinary(
          AST_EXPR_CAL_BINARY, SHIFT == 1 ? "LSHIFT" : "RSHIFT",
          (((yyGLRStackItem const *)
            yyvsp)[YYFILL((1) - (3))].yystate.yysemantics.yysval.ast_node),
          (((yyGLRStackItem const *)
            yyvsp)[YYFILL((3) - (3))].yystate.yysemantics.yysval.ast_node));
    } break;

    case 107:
/* Line 868 of glr.c  */
#line 687 "sql.ypp"
    {
      ((*yyvalp).ast_node) = new AstExprUnary(
          AST_EXPR_UNARY, "!",
          (((yyGLRStackItem const *)
            yyvsp)[YYFILL((2) - (2))].yystate.yysemantics.yysval.ast_node));
    } break;

    case 108:
/* Line 868 of glr.c  */
#line 688 "sql.ypp"
    {
      ((*yyvalp).ast_node) = new AstExprUnary(
          AST_EXPR_UNARY, "NOT",
          (((yyGLRStackItem const *)
            yyvsp)[YYFILL((2) - (2))].yystate.yysemantics.yysval.ast_node));
    } break;

    case 109:
/* Line 868 of glr.c  */
#line 689 "sql.ypp"
    {
      ((*yyvalp).ast_node) = NULL;
    } break;

    case 110:
/* Line 868 of glr.c  */
#line 690 "sql.ypp"
    {
      ((*yyvalp).ast_node) = (((yyGLRStackItem const *)yyvsp)[YYFILL((2) - (3))]
                                  .yystate.yysemantics.yysval.ast_node);
    } break;

    case 111:
/* Line 868 of glr.c  */
#line 693 "sql.ypp"
    {
      ((*yyvalp).ast_node) = new AstExprUnary(
          AST_EXPR_UNARY, "IS_NULL",
          (((yyGLRStackItem const *)
            yyvsp)[YYFILL((1) - (3))].yystate.yysemantics.yysval.ast_node));
    } break;

    case 112:
/* Line 868 of glr.c  */
#line 694 "sql.ypp"
    {
      ((*yyvalp).ast_node) = new AstExprUnary(
          AST_EXPR_UNARY, "IS_NOT_NULL",
          (((yyGLRStackItem const *)
            yyvsp)[YYFILL((1) - (4))].yystate.yysemantics.yysval.ast_node));
    } break;

    case 113:
/* Line 868 of glr.c  */
#line 695 "sql.ypp"
    {
      ((*yyvalp).ast_node) = new AstExprUnary(
          AST_EXPR_UNARY, "IS_BOOL",
          (((yyGLRStackItem const *)
            yyvsp)[YYFILL((1) - (3))].yystate.yysemantics.yysval.ast_node));
    } break;

    case 114:
/* Line 868 of glr.c  */
#line 696 "sql.ypp"
    {
      ((*yyvalp).ast_node) = new AstExprUnary(
          AST_EXPR_UNARY, "IS_NOT_BOOL",
          (((yyGLRStackItem const *)
            yyvsp)[YYFILL((1) - (4))].yystate.yysemantics.yysval.ast_node));
    } break;

    case 115:
/* Line 868 of glr.c  */
#line 699 "sql.ypp"
    {
      ((*yyvalp).ast_node) = new AstExprFunc(
          AST_EXPR_FUNC, "BETWEEN_AND",
          (((yyGLRStackItem const *)
            yyvsp)[YYFILL((1) - (5))].yystate.yysemantics.yysval.ast_node),
          (((yyGLRStackItem const *)
            yyvsp)[YYFILL((3) - (5))].yystate.yysemantics.yysval.ast_node),
          (((yyGLRStackItem const *)
            yyvsp)[YYFILL((5) - (5))].yystate.yysemantics.yysval.ast_node));
    } break;

    case 116:
/* Line 868 of glr.c  */
#line 702 "sql.ypp"
    {
      ((*yyvalp).ast_node) = new AstExprList(
          AST_EXPR_LIST, (((yyGLRStackItem const *)yyvsp)[YYFILL((1) - (1))]
                              .yystate.yysemantics.yysval.ast_node),
          NULL);
    } break;

    case 117:
/* Line 868 of glr.c  */
#line 703 "sql.ypp"
    {
      ((*yyvalp).ast_node) = new AstExprList(
          AST_EXPR_LIST, (((yyGLRStackItem const *)yyvsp)[YYFILL((1) - (3))]
                              .yystate.yysemantics.yysval.ast_node),
          (((yyGLRStackItem const *)
            yyvsp)[YYFILL((3) - (3))].yystate.yysemantics.yysval.ast_node));
    } break;

    case 118:
/* Line 868 of glr.c  */
#line 704 "sql.ypp"
    {
      ((*yyvalp).ast_node) = (((yyGLRStackItem const *)yyvsp)[YYFILL((2) - (3))]
                                  .yystate.yysemantics.yysval.ast_node);
    } break;

    case 119:
/* Line 868 of glr.c  */
#line 707 "sql.ypp"
    {
      ((*yyvalp).ast_node) = new AstExprList(
          AST_EXPR_LIST, (((yyGLRStackItem const *)yyvsp)[YYFILL((1) - (1))]
                              .yystate.yysemantics.yysval.ast_node),
          NULL);
    } break;

    case 120:
/* Line 868 of glr.c  */
#line 708 "sql.ypp"
    {
      ((*yyvalp).ast_node) = new AstExprList(
          AST_EXPR_LIST, (((yyGLRStackItem const *)yyvsp)[YYFILL((1) - (3))]
                              .yystate.yysemantics.yysval.ast_node),
          (((yyGLRStackItem const *)
            yyvsp)[YYFILL((3) - (3))].yystate.yysemantics.yysval.ast_node));
    } break;

    case 121:
/* Line 868 of glr.c  */
#line 711 "sql.ypp"
    {
      ((*yyvalp).ast_node) = new AstExprCmpBinary(
          AST_EXPR_CMP_BINARY, "EXPR_IN_LIST",
          (((yyGLRStackItem const *)
            yyvsp)[YYFILL((1) - (5))].yystate.yysemantics.yysval.ast_node),
          (((yyGLRStackItem const *)
            yyvsp)[YYFILL((4) - (5))].yystate.yysemantics.yysval.ast_node));
    } break;

    case 122:
/* Line 868 of glr.c  */
#line 712 "sql.ypp"
    {
      ((*yyvalp).ast_node) = new AstExprCmpBinary(
          AST_EXPR_CMP_BINARY, "LIST_IN_LIST",
          (((yyGLRStackItem const *)
            yyvsp)[YYFILL((2) - (7))].yystate.yysemantics.yysval.ast_node),
          (((yyGLRStackItem const *)
            yyvsp)[YYFILL((6) - (7))].yystate.yysemantics.yysval.ast_node));
    } break;

    case 123:
/* Line 868 of glr.c  */
#line 713 "sql.ypp"
    {
      AstNode *tmp_node = new AstExprCmpBinary(
          AST_EXPR_CMP_BINARY, "EXPR_IN_LIST",
          (((yyGLRStackItem const *)
            yyvsp)[YYFILL((1) - (6))].yystate.yysemantics.yysval.ast_node),
          (((yyGLRStackItem const *)
            yyvsp)[YYFILL((5) - (6))].yystate.yysemantics.yysval.ast_node));
      ((*yyvalp).ast_node) = new AstExprUnary(AST_EXPR_UNARY, "NOT", tmp_node);
    } break;

    case 124:
/* Line 868 of glr.c  */
#line 715 "sql.ypp"
    {
      AstNode *tmp_node = new AstExprCmpBinary(
          AST_EXPR_CMP_BINARY, "LIST_IN_LIST",
          (((yyGLRStackItem const *)
            yyvsp)[YYFILL((2) - (8))].yystate.yysemantics.yysval.ast_node),
          (((yyGLRStackItem const *)
            yyvsp)[YYFILL((7) - (8))].yystate.yysemantics.yysval.ast_node));
      ((*yyvalp).ast_node) = new AstExprUnary(AST_EXPR_UNARY, "NOT", tmp_node);
    } break;

    case 125:
/* Line 868 of glr.c  */
#line 717 "sql.ypp"
    {
      ((*yyvalp).ast_node) = new AstExprCmpBinary(
          AST_EXPR_CMP_BINARY, "EXPR_IN_SELECT",
          (((yyGLRStackItem const *)
            yyvsp)[YYFILL((1) - (5))].yystate.yysemantics.yysval.ast_node),
          (((yyGLRStackItem const *)
            yyvsp)[YYFILL((4) - (5))].yystate.yysemantics.yysval.ast_node));
    } break;

    case 126:
/* Line 868 of glr.c  */
#line 718 "sql.ypp"
    {
      ((*yyvalp).ast_node) = new AstExprCmpBinary(
          AST_EXPR_CMP_BINARY, "LIST_IN_SELECT",
          (((yyGLRStackItem const *)
            yyvsp)[YYFILL((2) - (7))].yystate.yysemantics.yysval.ast_node),
          (((yyGLRStackItem const *)
            yyvsp)[YYFILL((6) - (7))].yystate.yysemantics.yysval.ast_node));
    } break;

    case 127:
/* Line 868 of glr.c  */
#line 719 "sql.ypp"
    {
      ((*yyvalp).ast_node) = new AstExprCmpBinary(
          AST_EXPR_CMP_BINARY, "EXPR_NOT_IN_SELECT",
          (((yyGLRStackItem const *)
            yyvsp)[YYFILL((1) - (6))].yystate.yysemantics.yysval.ast_node),
          (((yyGLRStackItem const *)
            yyvsp)[YYFILL((5) - (6))].yystate.yysemantics.yysval.ast_node));
    } break;

    case 128:
/* Line 868 of glr.c  */
#line 720 "sql.ypp"
    {
      ((*yyvalp).ast_node) = new AstExprCmpBinary(
          AST_EXPR_CMP_BINARY, "LIST_NOT_IN_SELECT",
          (((yyGLRStackItem const *)
            yyvsp)[YYFILL((2) - (8))].yystate.yysemantics.yysval.ast_node),
          (((yyGLRStackItem const *)
            yyvsp)[YYFILL((7) - (8))].yystate.yysemantics.yysval.ast_node));
    } break;

    case 129:
/* Line 868 of glr.c  */
#line 722 "sql.ypp"
    {
      ((*yyvalp).ast_node) = new AstExprUnary(
          AST_EXPR_UNARY, "EXSIST",
          (((yyGLRStackItem const *)
            yyvsp)[YYFILL((3) - (4))].yystate.yysemantics.yysval.ast_node));
    } break;

    case 130:
/* Line 868 of glr.c  */
#line 723 "sql.ypp"
    {
      ((*yyvalp).ast_node) = new AstExprUnary(
          AST_EXPR_UNARY, "NOT_EXSIST",
          (((yyGLRStackItem const *)
            yyvsp)[YYFILL((4) - (5))].yystate.yysemantics.yysval.ast_node));
    } break;

    case 131:
/* Line 868 of glr.c  */
#line 733 "sql.ypp"
    {
      ((*yyvalp).ast_node) =
          new AstExprUnary(AST_EXPR_UNARY, "COUNT_ALL", NULL);
    } break;

    case 132:
/* Line 868 of glr.c  */
#line 734 "sql.ypp"
    {
      ((*yyvalp).ast_node) = new AstExprUnary(
          AST_EXPR_UNARY, "COUNT",
          (((yyGLRStackItem const *)
            yyvsp)[YYFILL((3) - (4))].yystate.yysemantics.yysval.ast_node));
    } break;

    case 133:
/* Line 868 of glr.c  */
#line 735 "sql.ypp"
    {
      ((*yyvalp).ast_node) = new AstExprUnary(
          AST_EXPR_UNARY, "SUM",
          (((yyGLRStackItem const *)
            yyvsp)[YYFILL((3) - (4))].yystate.yysemantics.yysval.ast_node));
    } break;

    case 134:
/* Line 868 of glr.c  */
#line 736 "sql.ypp"
    {
      ((*yyvalp).ast_node) = new AstExprUnary(
          AST_EXPR_UNARY, "AVG",
          (((yyGLRStackItem const *)
            yyvsp)[YYFILL((3) - (4))].yystate.yysemantics.yysval.ast_node));
    } break;

    case 135:
/* Line 868 of glr.c  */
#line 737 "sql.ypp"
    {
      ((*yyvalp).ast_node) = new AstExprUnary(
          AST_EXPR_UNARY, "MIN",
          (((yyGLRStackItem const *)
            yyvsp)[YYFILL((3) - (4))].yystate.yysemantics.yysval.ast_node));
    } break;

    case 136:
/* Line 868 of glr.c  */
#line 738 "sql.ypp"
    {
      ((*yyvalp).ast_node) = new AstExprUnary(
          AST_EXPR_UNARY, "MAX",
          (((yyGLRStackItem const *)
            yyvsp)[YYFILL((3) - (4))].yystate.yysemantics.yysval.ast_node));
    } break;

    case 137:
/* Line 868 of glr.c  */
#line 742 "sql.ypp"
    {
      ((*yyvalp).ast_node) = new AstExprFunc(
          AST_EXPR_FUNC, "SUBSTRING_EXPR_EXPR",
          (((yyGLRStackItem const *)
            yyvsp)[YYFILL((3) - (6))].yystate.yysemantics.yysval.ast_node),
          (((yyGLRStackItem const *)
            yyvsp)[YYFILL((5) - (6))].yystate.yysemantics.yysval.ast_node),
          NULL);
    } break;

    case 138:
/* Line 868 of glr.c  */
#line 743 "sql.ypp"
    {
      ((*yyvalp).ast_node) = new AstExprFunc(
          AST_EXPR_FUNC, "SUBSTRING_EXPR_FROM_EXPR",
          (((yyGLRStackItem const *)
            yyvsp)[YYFILL((3) - (6))].yystate.yysemantics.yysval.ast_node),
          (((yyGLRStackItem const *)
            yyvsp)[YYFILL((5) - (6))].yystate.yysemantics.yysval.ast_node),
          NULL);
    } break;

    case 139:
/* Line 868 of glr.c  */
#line 744 "sql.ypp"
    {
      ((*yyvalp).ast_node) = new AstExprFunc(
          AST_EXPR_FUNC, "SUBSTRING_EXPR_EXPR_EXPR",
          (((yyGLRStackItem const *)
            yyvsp)[YYFILL((3) - (8))].yystate.yysemantics.yysval.ast_node),
          (((yyGLRStackItem const *)
            yyvsp)[YYFILL((5) - (8))].yystate.yysemantics.yysval.ast_node),
          (((yyGLRStackItem const *)
            yyvsp)[YYFILL((7) - (8))].yystate.yysemantics.yysval.ast_node));
    } break;

    case 140:
/* Line 868 of glr.c  */
#line 745 "sql.ypp"
    {
      ((*yyvalp).ast_node) = new AstExprFunc(
          AST_EXPR_FUNC, "SUBSTRING_EXPR_FROM_EXPR_FOR_EXPR",
          (((yyGLRStackItem const *)
            yyvsp)[YYFILL((3) - (8))].yystate.yysemantics.yysval.ast_node),
          (((yyGLRStackItem const *)
            yyvsp)[YYFILL((5) - (8))].yystate.yysemantics.yysval.ast_node),
          (((yyGLRStackItem const *)
            yyvsp)[YYFILL((7) - (8))].yystate.yysemantics.yysval.ast_node));
    } break;

    case 141:
/* Line 868 of glr.c  */
#line 746 "sql.ypp"
    {
      ((*yyvalp).ast_node) = new AstExprFunc(
          AST_EXPR_FUNC, "TRIM_BOTH", NULL,
          (((yyGLRStackItem const *)
            yyvsp)[YYFILL((3) - (4))].yystate.yysemantics.yysval.ast_node),
          NULL);
    } break;

    case 142:
/* Line 868 of glr.c  */
#line 747 "sql.ypp"
    {
      ((*yyvalp).ast_node) = new AstExprFunc(
          AST_EXPR_FUNC, (((yyGLRStackItem const *)yyvsp)[YYFILL((3) - (7))]
                              .yystate.yysemantics.yysval.strval),
          (((yyGLRStackItem const *)
            yyvsp)[YYFILL((4) - (7))].yystate.yysemantics.yysval.ast_node),
          (((yyGLRStackItem const *)
            yyvsp)[YYFILL((6) - (7))].yystate.yysemantics.yysval.ast_node),
          NULL);
    } break;

    case 143:
/* Line 868 of glr.c  */
#line 748 "sql.ypp"
    {
      ((*yyvalp).ast_node) = new AstExprFunc(
          AST_EXPR_FUNC, "UPPER",
          (((yyGLRStackItem const *)
            yyvsp)[YYFILL((3) - (4))].yystate.yysemantics.yysval.ast_node),
          NULL, NULL);
    } break;

    case 144:
/* Line 868 of glr.c  */
#line 749 "sql.ypp"
    {
      ((*yyvalp).ast_node) = new AstExprFunc(
          AST_EXPR_FUNC, "CAST",
          (((yyGLRStackItem const *)
            yyvsp)[YYFILL((3) - (6))].yystate.yysemantics.yysval.ast_node),
          (((yyGLRStackItem const *)
            yyvsp)[YYFILL((5) - (6))].yystate.yysemantics.yysval.ast_node),
          NULL);
    } break;

    case 145:
/* Line 868 of glr.c  */
#line 750 "sql.ypp"
    {
      ((*yyvalp).ast_node) = new AstExprFunc(
          AST_EXPR_FUNC, "COALESCE",
          (((yyGLRStackItem const *)
            yyvsp)[YYFILL((3) - (4))].yystate.yysemantics.yysval.ast_node),
          NULL, NULL);
    } break;

    case 146:
/* Line 868 of glr.c  */
#line 753 "sql.ypp"
    {
      ((*yyvalp).strval) = "TRIM_LEADING";
    } break;

    case 147:
/* Line 868 of glr.c  */
#line 754 "sql.ypp"
    {
      ((*yyvalp).strval) = "TRIM_TRAILING";
    } break;

    case 148:
/* Line 868 of glr.c  */
#line 755 "sql.ypp"
    {
      ((*yyvalp).strval) = "TRIM_BOTH";
    } break;

    case 149:
/* Line 868 of glr.c  */
#line 758 "sql.ypp"
    {
      ((*yyvalp).ast_node) = NULL;
    } break;

    case 150:
/* Line 868 of glr.c  */
#line 759 "sql.ypp"
    {
      ((*yyvalp).ast_node) = new AstExprConst(AST_EXPR_CONST, "INT", NULL);
    } break;

    case 151:
/* Line 868 of glr.c  */
#line 760 "sql.ypp"
    {
      ((*yyvalp).ast_node) = new AstExprConst(AST_EXPR_CONST, "STRING", NULL);
    } break;

    case 152:
/* Line 868 of glr.c  */
#line 761 "sql.ypp"
    {
      ((*yyvalp).ast_node) = new AstExprConst(AST_EXPR_CONST, "DOUBLE", NULL);
    } break;

    case 153:
/* Line 868 of glr.c  */
#line 762 "sql.ypp"
    {
      ((*yyvalp).ast_node) = new AstExprConst(AST_EXPR_CONST, "FLOAT", NULL);
    } break;

    case 154:
/* Line 868 of glr.c  */
#line 763 "sql.ypp"
    {
      ((*yyvalp).ast_node) = new AstExprConst(AST_EXPR_CONST, "CHAR", NULL);
    } break;

    case 155:
/* Line 868 of glr.c  */
#line 766 "sql.ypp"
    {
      ((*yyvalp).ast_node) = new AstExprFunc(
          AST_EXPR_FUNC, "DATE_ADD",
          (((yyGLRStackItem const *)
            yyvsp)[YYFILL((3) - (6))].yystate.yysemantics.yysval.ast_node),
          (((yyGLRStackItem const *)
            yyvsp)[YYFILL((5) - (6))].yystate.yysemantics.yysval.ast_node),
          NULL);
    } break;

    case 156:
/* Line 868 of glr.c  */
#line 767 "sql.ypp"
    {
      ((*yyvalp).ast_node) = new AstExprFunc(
          AST_EXPR_FUNC, "DATE_SUB",
          (((yyGLRStackItem const *)
            yyvsp)[YYFILL((3) - (6))].yystate.yysemantics.yysval.ast_node),
          (((yyGLRStackItem const *)
            yyvsp)[YYFILL((5) - (6))].yystate.yysemantics.yysval.ast_node),
          NULL);
    } break;

    case 157:
/* Line 868 of glr.c  */
#line 771 "sql.ypp"
    {
      ((*yyvalp).ast_node) = new AstExprFunc(
          AST_EXPR_FUNC, "INTERVAL_HOUR",
          (((yyGLRStackItem const *)
            yyvsp)[YYFILL((2) - (3))].yystate.yysemantics.yysval.ast_node),
          NULL, NULL);
    } break;

    case 158:
/* Line 868 of glr.c  */
#line 772 "sql.ypp"
    {
      ((*yyvalp).ast_node) = new AstExprFunc(
          AST_EXPR_FUNC, "INTERVAL_MICROSECOND",
          (((yyGLRStackItem const *)
            yyvsp)[YYFILL((2) - (3))].yystate.yysemantics.yysval.ast_node),
          NULL, NULL);
    } break;

    case 159:
/* Line 868 of glr.c  */
#line 773 "sql.ypp"
    {
      ((*yyvalp).ast_node) = new AstExprFunc(
          AST_EXPR_FUNC, "INTERVAL_MINUTE",
          (((yyGLRStackItem const *)
            yyvsp)[YYFILL((2) - (3))].yystate.yysemantics.yysval.ast_node),
          NULL, NULL);
    } break;

    case 160:
/* Line 868 of glr.c  */
#line 774 "sql.ypp"
    {
      ((*yyvalp).ast_node) = new AstExprFunc(
          AST_EXPR_FUNC, "INTERVAL_SECOND",
          (((yyGLRStackItem const *)
            yyvsp)[YYFILL((2) - (3))].yystate.yysemantics.yysval.ast_node),
          NULL, NULL);
    } break;

    case 161:
/* Line 868 of glr.c  */
#line 775 "sql.ypp"
    {
      ((*yyvalp).ast_node) = new AstExprFunc(
          AST_EXPR_FUNC, "INTERVAL_DAY",
          (((yyGLRStackItem const *)
            yyvsp)[YYFILL((2) - (3))].yystate.yysemantics.yysval.ast_node),
          NULL, NULL);
    } break;

    case 162:
/* Line 868 of glr.c  */
#line 776 "sql.ypp"
    {
      ((*yyvalp).ast_node) = new AstExprFunc(
          AST_EXPR_FUNC, "INTERVAL_MONTH",
          (((yyGLRStackItem const *)
            yyvsp)[YYFILL((2) - (3))].yystate.yysemantics.yysval.ast_node),
          NULL, NULL);
    } break;

    case 163:
/* Line 868 of glr.c  */
#line 777 "sql.ypp"
    {
      ((*yyvalp).ast_node) = new AstExprFunc(
          AST_EXPR_FUNC, "INTERVAL_YEAR",
          (((yyGLRStackItem const *)
            yyvsp)[YYFILL((2) - (3))].yystate.yysemantics.yysval.ast_node),
          NULL, NULL);
    } break;

    case 164:
/* Line 868 of glr.c  */
#line 778 "sql.ypp"
    {
      ((*yyvalp).ast_node) = new AstExprFunc(
          AST_EXPR_FUNC, "INTERVAL_WEEK",
          (((yyGLRStackItem const *)
            yyvsp)[YYFILL((2) - (3))].yystate.yysemantics.yysval.ast_node),
          NULL, NULL);
    } break;

    case 165:
/* Line 868 of glr.c  */
#line 779 "sql.ypp"
    {
      ((*yyvalp).ast_node) = new AstExprFunc(
          AST_EXPR_FUNC, "INTERVAL_QUARTER",
          (((yyGLRStackItem const *)
            yyvsp)[YYFILL((2) - (3))].yystate.yysemantics.yysval.ast_node),
          NULL, NULL);
    } break;

    case 166:
/* Line 868 of glr.c  */
#line 783 "sql.ypp"
    {
      ((*yyvalp).ast_node) = new AstExprFunc(
          AST_EXPR_FUNC, "CASE1",
          (((yyGLRStackItem const *)
            yyvsp)[YYFILL((2) - (4))].yystate.yysemantics.yysval.ast_node),
          (((yyGLRStackItem const *)
            yyvsp)[YYFILL((3) - (4))].yystate.yysemantics.yysval.ast_node),
          NULL);
    } break;

    case 167:
/* Line 868 of glr.c  */
#line 784 "sql.ypp"
    {
      ((*yyvalp).ast_node) = new AstExprFunc(
          AST_EXPR_FUNC, "CASE1_ELSE",
          (((yyGLRStackItem const *)
            yyvsp)[YYFILL((2) - (6))].yystate.yysemantics.yysval.ast_node),
          (((yyGLRStackItem const *)
            yyvsp)[YYFILL((3) - (6))].yystate.yysemantics.yysval.ast_node),
          (((yyGLRStackItem const *)
            yyvsp)[YYFILL((5) - (6))].yystate.yysemantics.yysval.ast_node));
    } break;

    case 168:
/* Line 868 of glr.c  */
#line 785 "sql.ypp"
    {
      ((*yyvalp).ast_node) = new AstExprFunc(
          AST_EXPR_FUNC, "CASE2", NULL,
          (((yyGLRStackItem const *)
            yyvsp)[YYFILL((2) - (3))].yystate.yysemantics.yysval.ast_node),
          NULL);
    } break;

    case 169:
/* Line 868 of glr.c  */
#line 786 "sql.ypp"
    {
      ((*yyvalp).ast_node) = new AstExprFunc(
          AST_EXPR_FUNC, "CASE2_ELSE", NULL,
          (((yyGLRStackItem const *)
            yyvsp)[YYFILL((2) - (5))].yystate.yysemantics.yysval.ast_node),
          (((yyGLRStackItem const *)
            yyvsp)[YYFILL((4) - (5))].yystate.yysemantics.yysval.ast_node));
    } break;

    case 170:
/* Line 868 of glr.c  */
#line 789 "sql.ypp"
    {
      ((*yyvalp).ast_node) = new AstExprFunc(
          AST_EXPR_FUNC, "WHEN",
          (((yyGLRStackItem const *)
            yyvsp)[YYFILL((2) - (4))].yystate.yysemantics.yysval.ast_node),
          (((yyGLRStackItem const *)
            yyvsp)[YYFILL((4) - (4))].yystate.yysemantics.yysval.ast_node),
          NULL);
    } break;

    case 171:
/* Line 868 of glr.c  */
#line 790 "sql.ypp"
    {
      ((*yyvalp).ast_node) = new AstExprFunc(
          AST_EXPR_FUNC, "WHEN",
          (((yyGLRStackItem const *)
            yyvsp)[YYFILL((3) - (5))].yystate.yysemantics.yysval.ast_node),
          (((yyGLRStackItem const *)
            yyvsp)[YYFILL((5) - (5))].yystate.yysemantics.yysval.ast_node),
          (((yyGLRStackItem const *)
            yyvsp)[YYFILL((1) - (5))].yystate.yysemantics.yysval.ast_node));
    } break;

    case 172:
/* Line 868 of glr.c  */
#line 793 "sql.ypp"
    {
      ((*yyvalp).ast_node) = new AstExprCalBinary(
          AST_EXPR_CAL_BINARY, "LIKE",
          (((yyGLRStackItem const *)
            yyvsp)[YYFILL((1) - (3))].yystate.yysemantics.yysval.ast_node),
          (((yyGLRStackItem const *)
            yyvsp)[YYFILL((3) - (3))].yystate.yysemantics.yysval.ast_node));
    } break;

    case 173:
/* Line 868 of glr.c  */
#line 794 "sql.ypp"
    {
      ((*yyvalp).ast_node) = new AstExprCalBinary(
          AST_EXPR_CAL_BINARY, "NOT_LIKE",
          (((yyGLRStackItem const *)
            yyvsp)[YYFILL((1) - (4))].yystate.yysemantics.yysval.ast_node),
          (((yyGLRStackItem const *)
            yyvsp)[YYFILL((4) - (4))].yystate.yysemantics.yysval.ast_node));
    } break;

    case 174:
/* Line 868 of glr.c  */
#line 797 "sql.ypp"
    {
      ((*yyvalp).ast_node) = new AstExprCalBinary(
          AST_EXPR_CAL_BINARY, "REGEXP",
          (((yyGLRStackItem const *)
            yyvsp)[YYFILL((1) - (3))].yystate.yysemantics.yysval.ast_node),
          (((yyGLRStackItem const *)
            yyvsp)[YYFILL((3) - (3))].yystate.yysemantics.yysval.ast_node));
    } break;

    case 175:
/* Line 868 of glr.c  */
#line 798 "sql.ypp"
    {
      ((*yyvalp).ast_node) = new AstExprCalBinary(
          AST_EXPR_CAL_BINARY, "NOT_REGEXP",
          (((yyGLRStackItem const *)
            yyvsp)[YYFILL((1) - (4))].yystate.yysemantics.yysval.ast_node),
          (((yyGLRStackItem const *)
            yyvsp)[YYFILL((4) - (4))].yystate.yysemantics.yysval.ast_node));
    } break;

    case 176:
/* Line 868 of glr.c  */
#line 804 "sql.ypp"
    {
      ((*yyvalp).ast_node) = (((yyGLRStackItem const *)yyvsp)[YYFILL((1) - (1))]
                                  .yystate.yysemantics.yysval.ast_node);
    } break;

    case 177:
/* Line 868 of glr.c  */
#line 808 "sql.ypp"
    {
      string temp =
          ((((yyGLRStackItem const *)yyvsp)[YYFILL((4) - (4))]
                .yystate.yysemantics.yysval.strval) == NULL)
              ? ""
              : string((((yyGLRStackItem const *)yyvsp)[YYFILL((4) - (4))]
                            .yystate.yysemantics.yysval.strval));
      ((*yyvalp).ast_node) = new AstCreateDatabase(
          AST_CREATE_DATABASE, 1,
          (((yyGLRStackItem const *)
            yyvsp)[YYFILL((3) - (4))].yystate.yysemantics.yysval.intval),
          temp);
    } break;

    case 178:
/* Line 868 of glr.c  */
#line 809 "sql.ypp"
    {
      string temp =
          ((((yyGLRStackItem const *)yyvsp)[YYFILL((4) - (4))]
                .yystate.yysemantics.yysval.strval) == NULL)
              ? ""
              : string((((yyGLRStackItem const *)yyvsp)[YYFILL((4) - (4))]
                            .yystate.yysemantics.yysval.strval));
      ((*yyvalp).ast_node) = new AstCreateDatabase(
          AST_CREATE_SCHEMA, 2,
          (((yyGLRStackItem const *)
            yyvsp)[YYFILL((3) - (4))].yystate.yysemantics.yysval.intval),
          temp);
    } break;

    case 179:
/* Line 868 of glr.c  */
#line 812 "sql.ypp"
    {
      ((*yyvalp).intval) = 0;
    } break;

    case 180:
/* Line 868 of glr.c  */
#line 813 "sql.ypp"
    {
      ((*yyvalp).intval) = 1;
    } break;

    case 181:
/* Line 868 of glr.c  */
#line 818 "sql.ypp"
    {
      ((*yyvalp).ast_node) = (((yyGLRStackItem const *)yyvsp)[YYFILL((1) - (1))]
                                  .yystate.yysemantics.yysval.ast_node);
    } break;

    case 182:
/* Line 868 of glr.c  */
#line 822 "sql.ypp"
    {
      ((*yyvalp).ast_node) = new AstCreateTable(
          AST_CREATE_TABLE_LIST,
          (((yyGLRStackItem const *)
            yyvsp)[YYFILL((2) - (8))].yystate.yysemantics.yysval.intval),
          (((yyGLRStackItem const *)
            yyvsp)[YYFILL((4) - (8))].yystate.yysemantics.yysval.intval),
          string((((yyGLRStackItem const *)yyvsp)[YYFILL((5) - (8))]
                      .yystate.yysemantics.yysval.strval)),
          "", (((yyGLRStackItem const *)
                yyvsp)[YYFILL((7) - (8))].yystate.yysemantics.yysval.ast_node),
          NULL);
    } break;

    case 183:
/* Line 868 of glr.c  */
#line 826 "sql.ypp"
    {
      ((*yyvalp).ast_node) = new AstCreateTable(
          AST_CREATE_TABLE_LIST,
          (((yyGLRStackItem const *)
            yyvsp)[YYFILL((2) - (10))].yystate.yysemantics.yysval.intval),
          (((yyGLRStackItem const *)
            yyvsp)[YYFILL((4) - (10))].yystate.yysemantics.yysval.intval),
          string((((yyGLRStackItem const *)yyvsp)[YYFILL((5) - (10))]
                      .yystate.yysemantics.yysval.strval)),
          string((((yyGLRStackItem const *)yyvsp)[YYFILL((7) - (10))]
                      .yystate.yysemantics.yysval.strval)),
          (((yyGLRStackItem const *)
            yyvsp)[YYFILL((9) - (10))].yystate.yysemantics.yysval.ast_node),
          NULL);
    } break;

    case 184:
/* Line 868 of glr.c  */
#line 831 "sql.ypp"
    {
      ((*yyvalp).ast_node) = new AstCreateTable(
          AST_CREATE_TABLE_LIST_SEL,
          (((yyGLRStackItem const *)
            yyvsp)[YYFILL((2) - (9))].yystate.yysemantics.yysval.intval),
          (((yyGLRStackItem const *)
            yyvsp)[YYFILL((4) - (9))].yystate.yysemantics.yysval.intval),
          string((((yyGLRStackItem const *)yyvsp)[YYFILL((5) - (9))]
                      .yystate.yysemantics.yysval.strval)),
          "", (((yyGLRStackItem const *)
                yyvsp)[YYFILL((7) - (9))].yystate.yysemantics.yysval.ast_node),
          (((yyGLRStackItem const *)
            yyvsp)[YYFILL((9) - (9))].yystate.yysemantics.yysval.ast_node));
    } break;

    case 185:
/* Line 868 of glr.c  */
#line 835 "sql.ypp"
    {
      ((*yyvalp).ast_node) = new AstCreateTable(
          AST_CREATE_TABLE_SEL,
          (((yyGLRStackItem const *)
            yyvsp)[YYFILL((2) - (6))].yystate.yysemantics.yysval.intval),
          (((yyGLRStackItem const *)
            yyvsp)[YYFILL((4) - (6))].yystate.yysemantics.yysval.intval),
          string((((yyGLRStackItem const *)yyvsp)[YYFILL((5) - (6))]
                      .yystate.yysemantics.yysval.strval)),
          "", NULL, (((yyGLRStackItem const *)yyvsp)[YYFILL((6) - (6))]
                         .yystate.yysemantics.yysval.ast_node));
    } break;

    case 186:
/* Line 868 of glr.c  */
#line 840 "sql.ypp"
    {
      ((*yyvalp).ast_node) = new AstCreateTable(
          AST_CREATE_TABLE_LIST_SEL,
          (((yyGLRStackItem const *)
            yyvsp)[YYFILL((2) - (11))].yystate.yysemantics.yysval.intval),
          (((yyGLRStackItem const *)
            yyvsp)[YYFILL((4) - (11))].yystate.yysemantics.yysval.intval),
          string((((yyGLRStackItem const *)yyvsp)[YYFILL((5) - (11))]
                      .yystate.yysemantics.yysval.strval)),
          string((((yyGLRStackItem const *)yyvsp)[YYFILL((7) - (11))]
                      .yystate.yysemantics.yysval.strval)),
          (((yyGLRStackItem const *)
            yyvsp)[YYFILL((9) - (11))].yystate.yysemantics.yysval.ast_node),
          (((yyGLRStackItem const *)
            yyvsp)[YYFILL((11) - (11))].yystate.yysemantics.yysval.ast_node));
    } break;

    case 187:
/* Line 868 of glr.c  */
#line 844 "sql.ypp"
    {
      ((*yyvalp).ast_node) = new AstCreateTable(
          AST_CREATE_TABLE_SEL,
          (((yyGLRStackItem const *)
            yyvsp)[YYFILL((2) - (8))].yystate.yysemantics.yysval.intval),
          (((yyGLRStackItem const *)
            yyvsp)[YYFILL((4) - (8))].yystate.yysemantics.yysval.intval),
          string((((yyGLRStackItem const *)yyvsp)[YYFILL((5) - (8))]
                      .yystate.yysemantics.yysval.strval)),
          string((((yyGLRStackItem const *)yyvsp)[YYFILL((7) - (8))]
                      .yystate.yysemantics.yysval.strval)),
          NULL, (((yyGLRStackItem const *)yyvsp)[YYFILL((8) - (8))]
                     .yystate.yysemantics.yysval.ast_node));
    } break;

    case 188:
/* Line 868 of glr.c  */
#line 848 "sql.ypp"
    {
      ((*yyvalp).ast_node) = new AstCreateSelect(
          AST_CREATE_SEL, (((yyGLRStackItem const *)yyvsp)[YYFILL((1) - (3))]
                               .yystate.yysemantics.yysval.intval),
          0, (((yyGLRStackItem const *)
               yyvsp)[YYFILL((3) - (3))].yystate.yysemantics.yysval.ast_node));
    } break;

    case 189:
/* Line 868 of glr.c  */
#line 851 "sql.ypp"
    {
      ((*yyvalp).intval) = 0;
    } break;

    case 190:
/* Line 868 of glr.c  */
#line 852 "sql.ypp"
    {
      ((*yyvalp).intval) = 1;
    } break;

    case 191:
/* Line 868 of glr.c  */
#line 853 "sql.ypp"
    {
      ((*yyvalp).intval) = 2;
    } break;

    case 192:
/* Line 868 of glr.c  */
#line 856 "sql.ypp"
    {
      ((*yyvalp).intval) = 0;
    } break;

    case 193:
/* Line 868 of glr.c  */
#line 857 "sql.ypp"
    {
      ((*yyvalp).intval) = 1;
    } break;

    case 194:
/* Line 868 of glr.c  */
#line 860 "sql.ypp"
    {
      ((*yyvalp).ast_node) = new AstCreateColList(
          AST_CREATE_COL_LIST,
          (((yyGLRStackItem const *)
            yyvsp)[YYFILL((1) - (1))].yystate.yysemantics.yysval.ast_node),
          NULL);
    } break;

    case 195:
/* Line 868 of glr.c  */
#line 861 "sql.ypp"
    {
      ((*yyvalp).ast_node) = new AstCreateColList(
          AST_CREATE_COL_LIST,
          (((yyGLRStackItem const *)
            yyvsp)[YYFILL((1) - (3))].yystate.yysemantics.yysval.ast_node),
          (((yyGLRStackItem const *)
            yyvsp)[YYFILL((3) - (3))].yystate.yysemantics.yysval.ast_node));
    } break;

    case 196:
/* Line 868 of glr.c  */
#line 866 "sql.ypp"
    {
      ((*yyvalp).ast_node) = new AstCreateDef(
          AST_CREATE_DEF_NAME, 1,
          string((((yyGLRStackItem const *)yyvsp)[YYFILL((1) - (3))]
                      .yystate.yysemantics.yysval.strval)),
          (((yyGLRStackItem const *)
            yyvsp)[YYFILL((2) - (3))].yystate.yysemantics.yysval.ast_node),
          (((yyGLRStackItem const *)
            yyvsp)[YYFILL((3) - (3))].yystate.yysemantics.yysval.ast_node),
          NULL);
    } break;

    case 197:
/* Line 868 of glr.c  */
#line 867 "sql.ypp"
    {
      ((*yyvalp).ast_node) = new AstCreateDef(
          AST_CREATE_DEF_PR_KEY, 2, "", NULL, NULL,
          (((yyGLRStackItem const *)
            yyvsp)[YYFILL((4) - (5))].yystate.yysemantics.yysval.ast_node));
    } break;

    case 198:
/* Line 868 of glr.c  */
#line 868 "sql.ypp"
    {
      ((*yyvalp).ast_node) = new AstCreateDef(
          AST_CREATE_DEF_KEY, 3, "", NULL, NULL,
          (((yyGLRStackItem const *)
            yyvsp)[YYFILL((3) - (4))].yystate.yysemantics.yysval.ast_node));
    } break;

    case 199:
/* Line 868 of glr.c  */
#line 869 "sql.ypp"
    {
      ((*yyvalp).ast_node) = new AstCreateDef(
          AST_CREATE_DEF_INDEX, 4, "", NULL, NULL,
          (((yyGLRStackItem const *)
            yyvsp)[YYFILL((3) - (4))].yystate.yysemantics.yysval.ast_node));
    } break;

    case 200:
/* Line 868 of glr.c  */
#line 870 "sql.ypp"
    {
      ((*yyvalp).ast_node) = new AstCreateDef(
          AST_CREATE_DEF_FTEXT_INDEX, 5, "", NULL, NULL,
          (((yyGLRStackItem const *)
            yyvsp)[YYFILL((4) - (5))].yystate.yysemantics.yysval.ast_node));
    } break;

    case 201:
/* Line 868 of glr.c  */
#line 871 "sql.ypp"
    {
      ((*yyvalp).ast_node) = new AstCreateDef(
          AST_CREATE_DEF_FTEXT_KEY, 6, "", NULL, NULL,
          (((yyGLRStackItem const *)
            yyvsp)[YYFILL((4) - (5))].yystate.yysemantics.yysval.ast_node));
    } break;

    case 202:
/* Line 868 of glr.c  */
#line 873 "sql.ypp"
    {
      ((*yyvalp).ast_node) =
          new AstColumnAtts(AST_COLUMN_ATTS, 0, 0, 0, "", NULL);
    } break;

    case 203:
/* Line 868 of glr.c  */
#line 874 "sql.ypp"
    {
      AstColumnAtts *temp = static_cast<AstColumnAtts *>(
          (((yyGLRStackItem const *)
            yyvsp)[YYFILL((1) - (3))].yystate.yysemantics.yysval.ast_node));
      temp->datatype_ |= 01;
      ((*yyvalp).ast_node) = temp;
    } break;

    case 204:
/* Line 868 of glr.c  */
#line 875 "sql.ypp"
    {
      AstColumnAtts *temp = static_cast<AstColumnAtts *>(
          (((yyGLRStackItem const *)
            yyvsp)[YYFILL((1) - (2))].yystate.yysemantics.yysval.ast_node));
      temp->datatype_ |= 02;
      ((*yyvalp).ast_node) = temp;
    } break;

    case 205:
/* Line 868 of glr.c  */
#line 876 "sql.ypp"
    {
      AstColumnAtts *temp = static_cast<AstColumnAtts *>(
          (((yyGLRStackItem const *)
            yyvsp)[YYFILL((1) - (3))].yystate.yysemantics.yysval.ast_node));
      temp->datatype_ |= 04;
      temp->default_string_ =
          ((((yyGLRStackItem const *)yyvsp)[YYFILL((3) - (3))]
                .yystate.yysemantics.yysval.strval) == NULL)
              ? ""
              : string((((yyGLRStackItem const *)yyvsp)[YYFILL((3) - (3))]
                            .yystate.yysemantics.yysval.strval));
      ((*yyvalp).ast_node) = temp;
    } break;

    case 206:
/* Line 868 of glr.c  */
#line 877 "sql.ypp"
    {
      AstColumnAtts *temp = static_cast<AstColumnAtts *>(
          (((yyGLRStackItem const *)
            yyvsp)[YYFILL((1) - (3))].yystate.yysemantics.yysval.ast_node));
      temp->datatype_ |= 010;
      temp->int_num_ = atoi((((yyGLRStackItem const *)yyvsp)[YYFILL((3) - (3))]
                                 .yystate.yysemantics.yysval.strval));
      ((*yyvalp).ast_node) = temp;
    } break;

    case 207:
/* Line 868 of glr.c  */
#line 878 "sql.ypp"
    {
      AstColumnAtts *temp = static_cast<AstColumnAtts *>(
          (((yyGLRStackItem const *)
            yyvsp)[YYFILL((1) - (3))].yystate.yysemantics.yysval.ast_node));
      temp->datatype_ |= 020;
      temp->double_num_ =
          atof((((yyGLRStackItem const *)
                 yyvsp)[YYFILL((3) - (3))].yystate.yysemantics.yysval.strval));
      ((*yyvalp).ast_node) = temp;
    } break;

    case 208:
/* Line 868 of glr.c  */
#line 879 "sql.ypp"
    {
      AstColumnAtts *temp = static_cast<AstColumnAtts *>(
          (((yyGLRStackItem const *)
            yyvsp)[YYFILL((1) - (3))].yystate.yysemantics.yysval.ast_node));
      temp->datatype_ |= 040;
      temp->int_num_ = atoi((((yyGLRStackItem const *)yyvsp)[YYFILL((3) - (3))]
                                 .yystate.yysemantics.yysval.strval));
      ((*yyvalp).ast_node) = temp;
    } break;

    case 209:
/* Line 868 of glr.c  */
#line 880 "sql.ypp"
    {
      AstColumnAtts *temp = static_cast<AstColumnAtts *>(
          (((yyGLRStackItem const *)
            yyvsp)[YYFILL((1) - (2))].yystate.yysemantics.yysval.ast_node));
      temp->datatype_ |= 0100;
      ((*yyvalp).ast_node) = temp;
    } break;

    case 210:
/* Line 868 of glr.c  */
#line 881 "sql.ypp"
    {
      AstColumnAtts *temp = static_cast<AstColumnAtts *>(
          (((yyGLRStackItem const *)
            yyvsp)[YYFILL((1) - (3))].yystate.yysemantics.yysval.ast_node));
      temp->datatype_ |= 0200;
      ((*yyvalp).ast_node) = temp;
    } break;

    case 211:
/* Line 868 of glr.c  */
#line 882 "sql.ypp"
    {
      AstColumnAtts *temp = static_cast<AstColumnAtts *>(
          (((yyGLRStackItem const *)
            yyvsp)[YYFILL((1) - (3))].yystate.yysemantics.yysval.ast_node));
      temp->datatype_ |= 0400;
      ((*yyvalp).ast_node) = temp;
    } break;

    case 212:
/* Line 868 of glr.c  */
#line 883 "sql.ypp"
    {
      AstColumnAtts *temp = static_cast<AstColumnAtts *>(
          (((yyGLRStackItem const *)
            yyvsp)[YYFILL((1) - (2))].yystate.yysemantics.yysval.ast_node));
      temp->datatype_ |= 01000;
      ((*yyvalp).ast_node) = temp;
    } break;

    case 213:
/* Line 868 of glr.c  */
#line 884 "sql.ypp"
    {
      AstColumnAtts *temp = static_cast<AstColumnAtts *>(
          (((yyGLRStackItem const *)
            yyvsp)[YYFILL((1) - (3))].yystate.yysemantics.yysval.ast_node));
      temp->datatype_ |= 02000;
      ((*yyvalp).ast_node) = temp;
    } break;

    case 214:
/* Line 868 of glr.c  */
#line 885 "sql.ypp"
    {
      AstColumnAtts *temp = static_cast<AstColumnAtts *>(
          (((yyGLRStackItem const *)
            yyvsp)[YYFILL((1) - (5))].yystate.yysemantics.yysval.ast_node));
      temp->datatype_ |= 04000;
      temp->col_list_ = (((yyGLRStackItem const *)yyvsp)[YYFILL((4) - (5))]
                             .yystate.yysemantics.yysval.ast_node);
      ((*yyvalp).ast_node) = temp;
    } break;

    case 215:
/* Line 868 of glr.c  */
#line 889 "sql.ypp"
    {
      ((*yyvalp).ast_node) = new AstDataType(
          AST_DATA_TYPE, 1, (((yyGLRStackItem const *)yyvsp)[YYFILL((2) - (2))]
                                 .yystate.yysemantics.yysval.ast_node),
          0, NULL, 0, NULL);
    } break;

    case 216:
/* Line 868 of glr.c  */
#line 890 "sql.ypp"
    {
      ((*yyvalp).ast_node) = new AstDataType(
          AST_DATA_TYPE, 2, (((yyGLRStackItem const *)yyvsp)[YYFILL((2) - (3))]
                                 .yystate.yysemantics.yysval.ast_node),
          (((yyGLRStackItem const *)
            yyvsp)[YYFILL((3) - (3))].yystate.yysemantics.yysval.intval),
          NULL, 0, NULL);
    } break;

    case 217:
/* Line 868 of glr.c  */
#line 891 "sql.ypp"
    {
      ((*yyvalp).ast_node) = new AstDataType(
          AST_DATA_TYPE, 3, (((yyGLRStackItem const *)yyvsp)[YYFILL((2) - (3))]
                                 .yystate.yysemantics.yysval.ast_node),
          (((yyGLRStackItem const *)
            yyvsp)[YYFILL((3) - (3))].yystate.yysemantics.yysval.intval),
          NULL, 0, NULL);
    } break;

    case 218:
/* Line 868 of glr.c  */
#line 892 "sql.ypp"
    {
      ((*yyvalp).ast_node) = new AstDataType(
          AST_DATA_TYPE, 4, (((yyGLRStackItem const *)yyvsp)[YYFILL((2) - (3))]
                                 .yystate.yysemantics.yysval.ast_node),
          (((yyGLRStackItem const *)
            yyvsp)[YYFILL((3) - (3))].yystate.yysemantics.yysval.intval),
          NULL, 0, NULL);
    } break;

    case 219:
/* Line 868 of glr.c  */
#line 893 "sql.ypp"
    {
      ((*yyvalp).ast_node) = new AstDataType(
          AST_DATA_TYPE, 5, (((yyGLRStackItem const *)yyvsp)[YYFILL((2) - (3))]
                                 .yystate.yysemantics.yysval.ast_node),
          (((yyGLRStackItem const *)
            yyvsp)[YYFILL((3) - (3))].yystate.yysemantics.yysval.intval),
          NULL, 0, NULL);
    } break;

    case 220:
/* Line 868 of glr.c  */
#line 894 "sql.ypp"
    {
      ((*yyvalp).ast_node) = new AstDataType(
          AST_DATA_TYPE, 6, (((yyGLRStackItem const *)yyvsp)[YYFILL((2) - (3))]
                                 .yystate.yysemantics.yysval.ast_node),
          (((yyGLRStackItem const *)
            yyvsp)[YYFILL((3) - (3))].yystate.yysemantics.yysval.intval),
          NULL, 0, NULL);
    } break;

    case 221:
/* Line 868 of glr.c  */
#line 895 "sql.ypp"
    {
      ((*yyvalp).ast_node) = new AstDataType(
          AST_DATA_TYPE, 7, (((yyGLRStackItem const *)yyvsp)[YYFILL((2) - (3))]
                                 .yystate.yysemantics.yysval.ast_node),
          (((yyGLRStackItem const *)
            yyvsp)[YYFILL((3) - (3))].yystate.yysemantics.yysval.intval),
          NULL, 0, NULL);
    } break;

    case 222:
/* Line 868 of glr.c  */
#line 896 "sql.ypp"
    {
      ((*yyvalp).ast_node) = new AstDataType(
          AST_DATA_TYPE, 8, (((yyGLRStackItem const *)yyvsp)[YYFILL((2) - (3))]
                                 .yystate.yysemantics.yysval.ast_node),
          (((yyGLRStackItem const *)
            yyvsp)[YYFILL((3) - (3))].yystate.yysemantics.yysval.intval),
          NULL, 0, NULL);
    } break;

    case 223:
/* Line 868 of glr.c  */
#line 897 "sql.ypp"
    {
      ((*yyvalp).ast_node) = new AstDataType(
          AST_DATA_TYPE, 9, (((yyGLRStackItem const *)yyvsp)[YYFILL((2) - (3))]
                                 .yystate.yysemantics.yysval.ast_node),
          (((yyGLRStackItem const *)
            yyvsp)[YYFILL((3) - (3))].yystate.yysemantics.yysval.intval),
          NULL, 0, NULL);
    } break;

    case 224:
/* Line 868 of glr.c  */
#line 898 "sql.ypp"
    {
      ((*yyvalp).ast_node) = new AstDataType(
          AST_DATA_TYPE, 10, (((yyGLRStackItem const *)yyvsp)[YYFILL((2) - (3))]
                                  .yystate.yysemantics.yysval.ast_node),
          (((yyGLRStackItem const *)
            yyvsp)[YYFILL((3) - (3))].yystate.yysemantics.yysval.intval),
          NULL, 0, NULL);
    } break;

    case 225:
/* Line 868 of glr.c  */
#line 899 "sql.ypp"
    {
      ((*yyvalp).ast_node) = new AstDataType(
          AST_DATA_TYPE, 11, (((yyGLRStackItem const *)yyvsp)[YYFILL((2) - (3))]
                                  .yystate.yysemantics.yysval.ast_node),
          (((yyGLRStackItem const *)
            yyvsp)[YYFILL((3) - (3))].yystate.yysemantics.yysval.intval),
          NULL, 0, NULL);
    } break;

    case 226:
/* Line 868 of glr.c  */
#line 900 "sql.ypp"
    {
      ((*yyvalp).ast_node) =
          new AstDataType(AST_DATA_TYPE, 12, NULL, 0, NULL, 0, NULL);
    } break;

    case 227:
/* Line 868 of glr.c  */
#line 901 "sql.ypp"
    {
      ((*yyvalp).ast_node) =
          new AstDataType(AST_DATA_TYPE, 13, NULL, 0, NULL, 0, NULL);
    } break;

    case 228:
/* Line 868 of glr.c  */
#line 902 "sql.ypp"
    {
      ((*yyvalp).ast_node) =
          new AstDataType(AST_DATA_TYPE, 14, NULL, 0, NULL, 0, NULL);
    } break;

    case 229:
/* Line 868 of glr.c  */
#line 903 "sql.ypp"
    {
      ((*yyvalp).ast_node) =
          new AstDataType(AST_DATA_TYPE, 15, NULL, 0, NULL, 0, NULL);
    } break;

    case 230:
/* Line 868 of glr.c  */
#line 904 "sql.ypp"
    {
      ((*yyvalp).ast_node) =
          new AstDataType(AST_DATA_TYPE, 16, NULL, 0, NULL, 0, NULL);
    } break;

    case 231:
/* Line 868 of glr.c  */
#line 905 "sql.ypp"
    {
      ((*yyvalp).ast_node) = new AstDataType(
          AST_DATA_TYPE, 17, (((yyGLRStackItem const *)yyvsp)[YYFILL((2) - (3))]
                                  .yystate.yysemantics.yysval.ast_node),
          0, (((yyGLRStackItem const *)
               yyvsp)[YYFILL((3) - (3))].yystate.yysemantics.yysval.ast_node),
          0, NULL);
    } break;

    case 232:
/* Line 868 of glr.c  */
#line 906 "sql.ypp"
    {
      ((*yyvalp).ast_node) = new AstDataType(
          AST_DATA_TYPE, 18, (((yyGLRStackItem const *)yyvsp)[YYFILL((2) - (3))]
                                  .yystate.yysemantics.yysval.ast_node),
          0, (((yyGLRStackItem const *)
               yyvsp)[YYFILL((3) - (3))].yystate.yysemantics.yysval.ast_node),
          0, NULL);
    } break;

    case 233:
/* Line 868 of glr.c  */
#line 907 "sql.ypp"
    {
      ((*yyvalp).ast_node) = new AstDataType(
          AST_DATA_TYPE, 19, (((yyGLRStackItem const *)yyvsp)[YYFILL((2) - (2))]
                                  .yystate.yysemantics.yysval.ast_node),
          0, NULL, 0, NULL);
    } break;

    case 234:
/* Line 868 of glr.c  */
#line 908 "sql.ypp"
    {
      AstNode *temp = new AstOptLength(
          AST_OPT_LENGTH,
          atoi((((yyGLRStackItem const *)
                 yyvsp)[YYFILL((3) - (4))].yystate.yysemantics.yysval.strval)),
          NULL);
      ((*yyvalp).ast_node) =
          new AstDataType(AST_DATA_TYPE, 20, temp, 0, NULL, 0, NULL);
    } break;

    case 235:
/* Line 868 of glr.c  */
#line 910 "sql.ypp"
    {
      ((*yyvalp).ast_node) =
          new AstDataType(AST_DATA_TYPE, 21, NULL, 0, NULL, 0, NULL);
    } break;

    case 236:
/* Line 868 of glr.c  */
#line 911 "sql.ypp"
    {
      ((*yyvalp).ast_node) =
          new AstDataType(AST_DATA_TYPE, 22, NULL, 0, NULL, 0, NULL);
    } break;

    case 237:
/* Line 868 of glr.c  */
#line 912 "sql.ypp"
    {
      ((*yyvalp).ast_node) =
          new AstDataType(AST_DATA_TYPE, 23, NULL, 0, NULL, 0, NULL);
    } break;

    case 238:
/* Line 868 of glr.c  */
#line 913 "sql.ypp"
    {
      ((*yyvalp).ast_node) =
          new AstDataType(AST_DATA_TYPE, 24, NULL, 0, NULL, 0, NULL);
    } break;

    case 239:
/* Line 868 of glr.c  */
#line 914 "sql.ypp"
    {
      ((*yyvalp).ast_node) = new AstDataType(
          AST_DATA_TYPE, 25, NULL, 0,
          (((yyGLRStackItem const *)
            yyvsp)[YYFILL((3) - (3))].yystate.yysemantics.yysval.ast_node),
          (((yyGLRStackItem const *)
            yyvsp)[YYFILL((2) - (3))].yystate.yysemantics.yysval.intval),
          NULL);
    } break;

    case 240:
/* Line 868 of glr.c  */
#line 915 "sql.ypp"
    {
      ((*yyvalp).ast_node) = new AstDataType(
          AST_DATA_TYPE, 26, NULL, 0,
          (((yyGLRStackItem const *)
            yyvsp)[YYFILL((3) - (3))].yystate.yysemantics.yysval.ast_node),
          (((yyGLRStackItem const *)
            yyvsp)[YYFILL((2) - (3))].yystate.yysemantics.yysval.intval),
          NULL);
    } break;

    case 241:
/* Line 868 of glr.c  */
#line 916 "sql.ypp"
    {
      ((*yyvalp).ast_node) = new AstDataType(
          AST_DATA_TYPE, 27, NULL, 0,
          (((yyGLRStackItem const *)
            yyvsp)[YYFILL((3) - (3))].yystate.yysemantics.yysval.ast_node),
          (((yyGLRStackItem const *)
            yyvsp)[YYFILL((2) - (3))].yystate.yysemantics.yysval.intval),
          NULL);
    } break;

    case 242:
/* Line 868 of glr.c  */
#line 917 "sql.ypp"
    {
      ((*yyvalp).ast_node) = new AstDataType(
          AST_DATA_TYPE, 28, NULL, 0,
          (((yyGLRStackItem const *)
            yyvsp)[YYFILL((3) - (3))].yystate.yysemantics.yysval.ast_node),
          (((yyGLRStackItem const *)
            yyvsp)[YYFILL((2) - (3))].yystate.yysemantics.yysval.intval),
          NULL);
    } break;

    case 243:
/* Line 868 of glr.c  */
#line 918 "sql.ypp"
    {
      ((*yyvalp).ast_node) = new AstDataType(
          AST_DATA_TYPE, 29, NULL, 0,
          (((yyGLRStackItem const *)
            yyvsp)[YYFILL((5) - (5))].yystate.yysemantics.yysval.ast_node),
          0, (((yyGLRStackItem const *)
               yyvsp)[YYFILL((3) - (5))].yystate.yysemantics.yysval.ast_node));
    } break;

    case 244:
/* Line 868 of glr.c  */
#line 919 "sql.ypp"
    {
      ((*yyvalp).ast_node) = new AstDataType(
          AST_DATA_TYPE, 30, NULL, 0,
          (((yyGLRStackItem const *)
            yyvsp)[YYFILL((5) - (5))].yystate.yysemantics.yysval.ast_node),
          0, (((yyGLRStackItem const *)
               yyvsp)[YYFILL((3) - (5))].yystate.yysemantics.yysval.ast_node));
    } break;

    case 245:
/* Line 868 of glr.c  */
#line 921 "sql.ypp"
    {
      ((*yyvalp).ast_node) = new AstEnumList(
          AST_ENUM, string((((yyGLRStackItem const *)yyvsp)[YYFILL((1) - (1))]
                                .yystate.yysemantics.yysval.strval)),
          NULL);
    } break;

    case 246:
/* Line 868 of glr.c  */
#line 922 "sql.ypp"
    {
      ((*yyvalp).ast_node) = new AstEnumList(
          AST_ENUM_LIST,
          string((((yyGLRStackItem const *)yyvsp)[YYFILL((3) - (3))]
                      .yystate.yysemantics.yysval.strval)),
          (((yyGLRStackItem const *)
            yyvsp)[YYFILL((1) - (3))].yystate.yysemantics.yysval.ast_node));
    } break;

    case 247:
/* Line 868 of glr.c  */
#line 925 "sql.ypp"
    {
      ((*yyvalp).ast_node) = NULL;
    } break;

    case 248:
/* Line 868 of glr.c  */
#line 926 "sql.ypp"
    {
      ((*yyvalp).ast_node) = new AstOptLength(
          AST_OPT_LENGTH,
          atoi((((yyGLRStackItem const *)
                 yyvsp)[YYFILL((2) - (3))].yystate.yysemantics.yysval.strval)),
          0);
    } break;

    case 249:
/* Line 868 of glr.c  */
#line 927 "sql.ypp"
    {
      ((*yyvalp).ast_node) = new AstOptLength(
          AST_OPT_LENGTH,
          atoi((((yyGLRStackItem const *)
                 yyvsp)[YYFILL((2) - (5))].yystate.yysemantics.yysval.strval)),
          atoi((((yyGLRStackItem const *)
                 yyvsp)[YYFILL((4) - (5))].yystate.yysemantics.yysval.strval)));
    } break;

    case 250:
/* Line 868 of glr.c  */
#line 930 "sql.ypp"
    {
      ((*yyvalp).intval) = 0;
    } break;

    case 251:
/* Line 868 of glr.c  */
#line 931 "sql.ypp"
    {
      ((*yyvalp).intval) = 1;
    } break;

    case 252:
/* Line 868 of glr.c  */
#line 934 "sql.ypp"
    {
      ((*yyvalp).intval) = 0;
    } break;

    case 253:
/* Line 868 of glr.c  */
#line 935 "sql.ypp"
    {
      ((*yyvalp).intval) = (((yyGLRStackItem const *)yyvsp)[YYFILL((1) - (2))]
                                .yystate.yysemantics.yysval.intval) |
                           01;
    } break;

    case 254:
/* Line 868 of glr.c  */
#line 936 "sql.ypp"
    {
      ((*yyvalp).intval) = (((yyGLRStackItem const *)yyvsp)[YYFILL((1) - (2))]
                                .yystate.yysemantics.yysval.intval) |
                           02;
    } break;

    case 255:
/* Line 868 of glr.c  */
#line 939 "sql.ypp"
    {
      ((*yyvalp).ast_node) = new AstOptCsc(AST_OPT_CSC, 0, "", "");
    } break;

    case 256:
/* Line 868 of glr.c  */
#line 941 "sql.ypp"
    {
      AstOptCsc *temp = static_cast<AstOptCsc *>(
          (((yyGLRStackItem const *)
            yyvsp)[YYFILL((1) - (4))].yystate.yysemantics.yysval.ast_node));
      temp->data_type_ |= 01;
      temp->str1_ = string((((yyGLRStackItem const *)yyvsp)[YYFILL((4) - (4))]
                                .yystate.yysemantics.yysval.strval));
      ((*yyvalp).ast_node) = temp;
    } break;

    case 257:
/* Line 868 of glr.c  */
#line 943 "sql.ypp"
    {
      AstOptCsc *temp = static_cast<AstOptCsc *>(
          (((yyGLRStackItem const *)
            yyvsp)[YYFILL((1) - (3))].yystate.yysemantics.yysval.ast_node));
      temp->data_type_ |= 02;
      temp->str2_ = string((((yyGLRStackItem const *)yyvsp)[YYFILL((3) - (3))]
                                .yystate.yysemantics.yysval.strval));
      ((*yyvalp).ast_node) = temp;
    } break;

    case 258:
/* Line 868 of glr.c  */
#line 947 "sql.ypp"
    {
      ((*yyvalp).ast_node) = (((yyGLRStackItem const *)yyvsp)[YYFILL((1) - (1))]
                                  .yystate.yysemantics.yysval
                                  .ast_node); /* puts("SQL parser： This is a
                                                 create_projection
                                                 statement");*/
    } break;

    case 259:
/* Line 868 of glr.c  */
#line 951 "sql.ypp"
    {
      ((*yyvalp).ast_node) = new AstCreateProjection(
          AST_CREATE_PROJECTION,
          string((((yyGLRStackItem const *)yyvsp)[YYFILL((4) - (10))]
                      .yystate.yysemantics.yysval.strval)),
          (((yyGLRStackItem const *)
            yyvsp)[YYFILL((6) - (10))].yystate.yysemantics.yysval.ast_node),
          1, string((((yyGLRStackItem const *)yyvsp)[YYFILL((10) - (10))]
                         .yystate.yysemantics.yysval.strval)));
    } break;

    case 260:
/* Line 868 of glr.c  */
#line 953 "sql.ypp"
    {
      if ((((yyGLRStackItem const *)yyvsp)[YYFILL((9) - (13))]
               .yystate.yysemantics.yysval.subtok) != 4) {
        yyerror(result, "please give a specific number");
      } else {
        ((*yyvalp).ast_node) = new AstCreateProjection(
            AST_CREATE_PROJECTION_NUM,
            string((((yyGLRStackItem const *)yyvsp)[YYFILL((4) - (13))]
                        .yystate.yysemantics.yysval.strval)),
            (((yyGLRStackItem const *)
              yyvsp)[YYFILL((6) - (13))].yystate.yysemantics.yysval.ast_node),
            atoi((((yyGLRStackItem const *)yyvsp)[YYFILL((10) - (13))]
                      .yystate.yysemantics.yysval.strval)),
            string((((yyGLRStackItem const *)yyvsp)[YYFILL((13) - (13))]
                        .yystate.yysemantics.yysval.strval)));
      }
    } break;

    case 261:
/* Line 868 of glr.c  */
#line 960 "sql.ypp"
    {
      ((*yyvalp).ast_node) = (((yyGLRStackItem const *)yyvsp)[YYFILL((1) - (1))]
                                  .yystate.yysemantics.yysval.ast_node);
    } break;

    case 262:
/* Line 868 of glr.c  */
#line 965 "sql.ypp"
    {
      ((*yyvalp).ast_node) = new AstCreateIndex(
          AST_CREATE_INDEX, (((yyGLRStackItem const *)yyvsp)[YYFILL((2) - (10))]
                                 .yystate.yysemantics.yysval.intval),
          string((((yyGLRStackItem const *)yyvsp)[YYFILL((4) - (10))]
                      .yystate.yysemantics.yysval.strval)),
          (((yyGLRStackItem const *)
            yyvsp)[YYFILL((5) - (10))].yystate.yysemantics.yysval.intval),
          (((yyGLRStackItem const *)
            yyvsp)[YYFILL((7) - (10))].yystate.yysemantics.yysval.strval),
          (((yyGLRStackItem const *)
            yyvsp)[YYFILL((9) - (10))].yystate.yysemantics.yysval.ast_node));
    } break;

    case 263:
/* Line 868 of glr.c  */
#line 968 "sql.ypp"
    {
      ((*yyvalp).intval) = 0;
    } break;

    case 264:
/* Line 868 of glr.c  */
#line 969 "sql.ypp"
    {
      ((*yyvalp).intval) = 1;
    } break;

    case 265:
/* Line 868 of glr.c  */
#line 970 "sql.ypp"
    {
      ((*yyvalp).intval) = 2;
    } break;

    case 266:
/* Line 868 of glr.c  */
#line 971 "sql.ypp"
    {
      ((*yyvalp).intval) = 3;
    } break;

    case 267:
/* Line 868 of glr.c  */
#line 974 "sql.ypp"
    {
      ((*yyvalp).intval) = 0;
    } break;

    case 268:
/* Line 868 of glr.c  */
#line 975 "sql.ypp"
    {
      ((*yyvalp).intval) = (((yyGLRStackItem const *)yyvsp)[YYFILL((2) - (2))]
                                .yystate.yysemantics.yysval.intval);
    } break;

    case 269:
/* Line 868 of glr.c  */
#line 978 "sql.ypp"
    {
      ((*yyvalp).intval) = 1;
    } break;

    case 270:
/* Line 868 of glr.c  */
#line 979 "sql.ypp"
    {
      ((*yyvalp).intval) = 2;
    } break;

    case 271:
/* Line 868 of glr.c  */
#line 982 "sql.ypp"
    {
      ((*yyvalp).ast_node) = new AstIndexColList(
          AST_INDEX_COL,
          string((((yyGLRStackItem const *)yyvsp)[YYFILL((1) - (3))]
                      .yystate.yysemantics.yysval.strval)),
          (((yyGLRStackItem const *)
            yyvsp)[YYFILL((2) - (3))].yystate.yysemantics.yysval.ast_node),
          (((yyGLRStackItem const *)
            yyvsp)[YYFILL((3) - (3))].yystate.yysemantics.yysval.intval),
          NULL);
    } break;

    case 272:
/* Line 868 of glr.c  */
#line 983 "sql.ypp"
    {
      ((*yyvalp).ast_node) = new AstIndexColList(
          AST_INDEX_COL_LIST,
          string((((yyGLRStackItem const *)yyvsp)[YYFILL((1) - (5))]
                      .yystate.yysemantics.yysval.strval)),
          (((yyGLRStackItem const *)
            yyvsp)[YYFILL((2) - (5))].yystate.yysemantics.yysval.ast_node),
          (((yyGLRStackItem const *)
            yyvsp)[YYFILL((3) - (5))].yystate.yysemantics.yysval.intval),
          (((yyGLRStackItem const *)
            yyvsp)[YYFILL((5) - (5))].yystate.yysemantics.yysval.ast_node));
    } break;

    case 273:
/* Line 868 of glr.c  */
#line 987 "sql.ypp"
    {
      ((*yyvalp).ast_node) = (((yyGLRStackItem const *)yyvsp)[YYFILL((1) - (1))]
                                  .yystate.yysemantics.yysval.ast_node);
    } break;

    case 274:
/* Line 868 of glr.c  */
#line 991 "sql.ypp"
    {
      if ((((yyGLRStackItem const *)yyvsp)[YYFILL((11) - (12))]
               .yystate.yysemantics.yysval.subtok) != 4) {
        yyerror(result, "please give a specific number");
      } else {
        ((*yyvalp).ast_node) = new AstLoadTable(
            AST_LOAD_TABLE,
            string((((yyGLRStackItem const *)yyvsp)[YYFILL((3) - (12))]
                        .yystate.yysemantics.yysval.strval)),
            (((yyGLRStackItem const *)
              yyvsp)[YYFILL((5) - (12))].yystate.yysemantics.yysval.ast_node),
            string((((yyGLRStackItem const *)yyvsp)[YYFILL((7) - (12))]
                        .yystate.yysemantics.yysval.strval)),
            string((((yyGLRStackItem const *)yyvsp)[YYFILL((9) - (12))]
                        .yystate.yysemantics.yysval.strval)),
            atof((((yyGLRStackItem const *)yyvsp)[YYFILL((12) - (12))]
                      .yystate.yysemantics.yysval.strval)),
            0);
      }
    } break;

    case 275:
/* Line 868 of glr.c  */
#line 995 "sql.ypp"
    {
      ((*yyvalp).ast_node) = new AstLoadTable(
          AST_LOAD_TABLE,
          string((((yyGLRStackItem const *)yyvsp)[YYFILL((3) - (9))]
                      .yystate.yysemantics.yysval.strval)),
          (((yyGLRStackItem const *)
            yyvsp)[YYFILL((5) - (9))].yystate.yysemantics.yysval.ast_node),
          string((((yyGLRStackItem const *)yyvsp)[YYFILL((7) - (9))]
                      .yystate.yysemantics.yysval.strval)),
          string((((yyGLRStackItem const *)yyvsp)[YYFILL((9) - (9))]
                      .yystate.yysemantics.yysval.strval)),
          1.0, 0);
    } break;

    case 276:
/* Line 868 of glr.c  */
#line 996 "sql.ypp"
    {
      ((*yyvalp).ast_node) = new AstLoadTable(
          AST_LOAD_TABLE,
          string((((yyGLRStackItem const *)yyvsp)[YYFILL((3) - (9))]
                      .yystate.yysemantics.yysval.strval)),
          (((yyGLRStackItem const *)
            yyvsp)[YYFILL((5) - (9))].yystate.yysemantics.yysval.ast_node),
          string((((yyGLRStackItem const *)yyvsp)[YYFILL((7) - (9))]
                      .yystate.yysemantics.yysval.strval)),
          string((((yyGLRStackItem const *)yyvsp)[YYFILL((9) - (9))]
                      .yystate.yysemantics.yysval.strval)),
          1.0, 1);
    } break;

    case 277:
/* Line 868 of glr.c  */
#line 998 "sql.ypp"
    {
      if ((((yyGLRStackItem const *)yyvsp)[YYFILL((11) - (12))]
               .yystate.yysemantics.yysval.subtok) != 4) {
        yyerror(result, "please give a specific number");
      } else {
        ((*yyvalp).ast_node) = new AstLoadTable(
            AST_LOAD_TABLE,
            string((((yyGLRStackItem const *)yyvsp)[YYFILL((3) - (12))]
                        .yystate.yysemantics.yysval.strval)),
            (((yyGLRStackItem const *)
              yyvsp)[YYFILL((5) - (12))].yystate.yysemantics.yysval.ast_node),
            string((((yyGLRStackItem const *)yyvsp)[YYFILL((7) - (12))]
                        .yystate.yysemantics.yysval.strval)),
            string((((yyGLRStackItem const *)yyvsp)[YYFILL((9) - (12))]
                        .yystate.yysemantics.yysval.strval)),
            atof((((yyGLRStackItem const *)yyvsp)[YYFILL((12) - (12))]
                      .yystate.yysemantics.yysval.strval)),
            1);
      }
    } break;

    case 278:
/* Line 868 of glr.c  */
#line 1004 "sql.ypp"
    {
      ((*yyvalp).ast_node) = (((yyGLRStackItem const *)yyvsp)[YYFILL((1) - (1))]
                                  .yystate.yysemantics.yysval
                                  .ast_node); /*output($$, 1); puts("SQL
                                                 parser： This is a drop_index
                                                 statement");*/
    } break;

    case 279:
/* Line 868 of glr.c  */
#line 1007 "sql.ypp"
    {
      ((*yyvalp).ast_node) = new AstDropIndex(
          AST_DROP_INDEX,
          string((((yyGLRStackItem const *)yyvsp)[YYFILL((3) - (5))]
                      .yystate.yysemantics.yysval.strval)),
          string((((yyGLRStackItem const *)yyvsp)[YYFILL((5) - (5))]
                      .yystate.yysemantics.yysval.strval)));
    } break;

    case 280:
/* Line 868 of glr.c  */
#line 1011 "sql.ypp"
    {
      ((*yyvalp).ast_node) = (((yyGLRStackItem const *)yyvsp)[YYFILL((1) - (1))]
                                  .yystate.yysemantics.yysval
                                  .ast_node); /* output($$, 1); puts("SQL
                                                 parser： This is a
                                                 drop_database statement");*/
    } break;

    case 281:
/* Line 868 of glr.c  */
#line 1015 "sql.ypp"
    {
      ((*yyvalp).ast_node) = new AstDropDatabase(
          AST_DROP_DB, 1, (((yyGLRStackItem const *)yyvsp)[YYFILL((3) - (4))]
                               .yystate.yysemantics.yysval.intval),
          string((((yyGLRStackItem const *)yyvsp)[YYFILL((4) - (4))]
                      .yystate.yysemantics.yysval.strval)));
    } break;

    case 282:
/* Line 868 of glr.c  */
#line 1016 "sql.ypp"
    {
      ((*yyvalp).ast_node) = new AstDropDatabase(
          AST_DROP_SCHEMA, 2,
          (((yyGLRStackItem const *)
            yyvsp)[YYFILL((3) - (4))].yystate.yysemantics.yysval.intval),
          string((((yyGLRStackItem const *)yyvsp)[YYFILL((4) - (4))]
                      .yystate.yysemantics.yysval.strval)));
    } break;

    case 283:
/* Line 868 of glr.c  */
#line 1019 "sql.ypp"
    {
      ((*yyvalp).intval) = 0;
    } break;

    case 284:
/* Line 868 of glr.c  */
#line 1020 "sql.ypp"
    {
      ((*yyvalp).intval) = 1;
    } break;

    case 285:
/* Line 868 of glr.c  */
#line 1024 "sql.ypp"
    {
      ((*yyvalp).ast_node) = (((yyGLRStackItem const *)yyvsp)[YYFILL((1) - (1))]
                                  .yystate.yysemantics.yysval
                                  .ast_node); /*output($$, 1); puts("SQL
                                                 parser： This is a drop_table
                                                 statement"); */
    } break;

    case 286:
/* Line 868 of glr.c  */
#line 1028 "sql.ypp"
    {
      ((*yyvalp).ast_node) = new AstDropTable(
          AST_DROP_TABLE, (((yyGLRStackItem const *)yyvsp)[YYFILL((2) - (6))]
                               .yystate.yysemantics.yysval.intval),
          (((yyGLRStackItem const *)
            yyvsp)[YYFILL((4) - (6))].yystate.yysemantics.yysval.intval),
          (((yyGLRStackItem const *)
            yyvsp)[YYFILL((6) - (6))].yystate.yysemantics.yysval.intval),
          (((yyGLRStackItem const *)
            yyvsp)[YYFILL((5) - (6))].yystate.yysemantics.yysval.ast_node));
    } break;

    case 287:
/* Line 868 of glr.c  */
#line 1031 "sql.ypp"
    {
      ((*yyvalp).ast_node) = new AstDropTableList(
          AST_DROP_TABLE_LIST, "",
          string((((yyGLRStackItem const *)yyvsp)[YYFILL((1) - (1))]
                      .yystate.yysemantics.yysval.strval)),
          NULL);
    } break;

    case 288:
/* Line 868 of glr.c  */
#line 1032 "sql.ypp"
    {
      ((*yyvalp).ast_node) = new AstDropTableList(
          AST_DROP_TABLE_LIST,
          string((((yyGLRStackItem const *)yyvsp)[YYFILL((1) - (3))]
                      .yystate.yysemantics.yysval.strval)),
          string((((yyGLRStackItem const *)yyvsp)[YYFILL((3) - (3))]
                      .yystate.yysemantics.yysval.strval)),
          NULL);
    } break;

    case 289:
/* Line 868 of glr.c  */
#line 1033 "sql.ypp"
    {
      ((*yyvalp).ast_node) = new AstDropTableList(
          AST_DROP_TABLE_LIST, "",
          string((((yyGLRStackItem const *)yyvsp)[YYFILL((3) - (3))]
                      .yystate.yysemantics.yysval.strval)),
          (((yyGLRStackItem const *)
            yyvsp)[YYFILL((1) - (3))].yystate.yysemantics.yysval.ast_node));
    } break;

    case 290:
/* Line 868 of glr.c  */
#line 1034 "sql.ypp"
    {
      ((*yyvalp).ast_node) = new AstDropTableList(
          AST_DROP_TABLE_LIST,
          string((((yyGLRStackItem const *)yyvsp)[YYFILL((3) - (5))]
                      .yystate.yysemantics.yysval.strval)),
          string((((yyGLRStackItem const *)yyvsp)[YYFILL((5) - (5))]
                      .yystate.yysemantics.yysval.strval)),
          (((yyGLRStackItem const *)
            yyvsp)[YYFILL((1) - (5))].yystate.yysemantics.yysval.ast_node));
    } break;

    case 291:
/* Line 868 of glr.c  */
#line 1037 "sql.ypp"
    {
      ((*yyvalp).intval) = 0;
    } break;

    case 292:
/* Line 868 of glr.c  */
#line 1038 "sql.ypp"
    {
      ((*yyvalp).intval) = 1;
    } break;

    case 293:
/* Line 868 of glr.c  */
#line 1039 "sql.ypp"
    {
      ((*yyvalp).intval) = 2;
    } break;

    case 294:
/* Line 868 of glr.c  */
#line 1045 "sql.ypp"
    {
      ((*yyvalp).ast_node) = (((yyGLRStackItem const *)yyvsp)[YYFILL((1) - (1))]
                                  .yystate.yysemantics.yysval.ast_node);
    } break;

    case 295:
/* Line 868 of glr.c  */
#line 1051 "sql.ypp"
    {
      ((*yyvalp).ast_node) = new AstInsertStmt(
          AST_INSERT_STMT, (((yyGLRStackItem const *)yyvsp)[YYFILL((2) - (8))]
                                .yystate.yysemantics.yysval.intval),
          string((((yyGLRStackItem const *)yyvsp)[YYFILL((4) - (8))]
                      .yystate.yysemantics.yysval.strval)),
          (((yyGLRStackItem const *)
            yyvsp)[YYFILL((5) - (8))].yystate.yysemantics.yysval.ast_node),
          (((yyGLRStackItem const *)
            yyvsp)[YYFILL((7) - (8))].yystate.yysemantics.yysval.ast_node),
          (((yyGLRStackItem const *)
            yyvsp)[YYFILL((8) - (8))].yystate.yysemantics.yysval.ast_node),
          NULL, NULL);
    } break;

    case 296:
/* Line 868 of glr.c  */
#line 1054 "sql.ypp"
    {
      ((*yyvalp).ast_node) = NULL;
    } break;

    case 297:
/* Line 868 of glr.c  */
#line 1055 "sql.ypp"
    {
      ((*yyvalp).ast_node) = (((yyGLRStackItem const *)yyvsp)[YYFILL((5) - (5))]
                                  .yystate.yysemantics.yysval.ast_node);
    } break;

    case 298:
/* Line 868 of glr.c  */
#line 1058 "sql.ypp"
    {
      ((*yyvalp).intval) = 0;
    } break;

    case 299:
/* Line 868 of glr.c  */
#line 1059 "sql.ypp"
    {
      ((*yyvalp).intval) = (((yyGLRStackItem const *)yyvsp)[YYFILL((1) - (2))]
                                .yystate.yysemantics.yysval.intval) |
                           01;
    } break;

    case 300:
/* Line 868 of glr.c  */
#line 1060 "sql.ypp"
    {
      ((*yyvalp).intval) = (((yyGLRStackItem const *)yyvsp)[YYFILL((1) - (2))]
                                .yystate.yysemantics.yysval.intval) |
                           02;
    } break;

    case 301:
/* Line 868 of glr.c  */
#line 1061 "sql.ypp"
    {
      ((*yyvalp).intval) = (((yyGLRStackItem const *)yyvsp)[YYFILL((1) - (2))]
                                .yystate.yysemantics.yysval.intval) |
                           04;
    } break;

    case 302:
/* Line 868 of glr.c  */
#line 1062 "sql.ypp"
    {
      ((*yyvalp).intval) = (((yyGLRStackItem const *)yyvsp)[YYFILL((1) - (2))]
                                .yystate.yysemantics.yysval.intval) |
                           010;
    } break;

    case 305:
/* Line 868 of glr.c  */
#line 1068 "sql.ypp"
    {
      ((*yyvalp).ast_node) = NULL;
    } break;

    case 306:
/* Line 868 of glr.c  */
#line 1069 "sql.ypp"
    {
      ((*yyvalp).ast_node) = (((yyGLRStackItem const *)yyvsp)[YYFILL((2) - (3))]
                                  .yystate.yysemantics.yysval.ast_node);
    } break;

    case 307:
/* Line 868 of glr.c  */
#line 1072 "sql.ypp"
    {
      ((*yyvalp).ast_node) = new AstInsertValList(
          AST_INSERT_VALUE_LIST,
          (((yyGLRStackItem const *)
            yyvsp)[YYFILL((2) - (3))].yystate.yysemantics.yysval.ast_node),
          NULL);
    } break;

    case 308:
/* Line 868 of glr.c  */
#line 1073 "sql.ypp"
    {
      ((*yyvalp).ast_node) = new AstInsertValList(
          AST_INSERT_VALUE_LIST,
          (((yyGLRStackItem const *)
            yyvsp)[YYFILL((2) - (5))].yystate.yysemantics.yysval.ast_node),
          (((yyGLRStackItem const *)
            yyvsp)[YYFILL((5) - (5))].yystate.yysemantics.yysval.ast_node));
    } break;

    case 309:
/* Line 868 of glr.c  */
#line 1077 "sql.ypp"
    {
      ((*yyvalp).ast_node) = new AstInsertVals(
          AST_INSERT_VALUE, 0,
          (((yyGLRStackItem const *)
            yyvsp)[YYFILL((1) - (1))].yystate.yysemantics.yysval.ast_node),
          NULL);
    } break;

    case 310:
/* Line 868 of glr.c  */
#line 1078 "sql.ypp"
    {
      ((*yyvalp).ast_node) = new AstInsertVals(AST_INSERT_VALUE, 1, NULL, NULL);
    } break;

    case 311:
/* Line 868 of glr.c  */
#line 1079 "sql.ypp"
    {
      ((*yyvalp).ast_node) = new AstInsertVals(
          AST_INSERT_VALUE, 0,
          (((yyGLRStackItem const *)
            yyvsp)[YYFILL((1) - (3))].yystate.yysemantics.yysval.ast_node),
          (((yyGLRStackItem const *)
            yyvsp)[YYFILL((3) - (3))].yystate.yysemantics.yysval.ast_node));
    } break;

    case 312:
/* Line 868 of glr.c  */
#line 1080 "sql.ypp"
    {
      ((*yyvalp).ast_node) = new AstInsertVals(
          AST_INSERT_VALUE, 1, NULL,
          (((yyGLRStackItem const *)
            yyvsp)[YYFILL((3) - (3))].yystate.yysemantics.yysval.ast_node));
    } break;

    case 313:
/* Line 868 of glr.c  */
#line 1084 "sql.ypp"
    {
      ((*yyvalp).ast_node) = new AstInsertStmt(
          AST_INSERT_STMT, (((yyGLRStackItem const *)yyvsp)[YYFILL((2) - (7))]
                                .yystate.yysemantics.yysval.intval),
          string((((yyGLRStackItem const *)yyvsp)[YYFILL((4) - (7))]
                      .yystate.yysemantics.yysval.strval)),
          NULL, NULL, (((yyGLRStackItem const *)yyvsp)[YYFILL((7) - (7))]
                           .yystate.yysemantics.yysval.ast_node),
          (((yyGLRStackItem const *)
            yyvsp)[YYFILL((6) - (7))].yystate.yysemantics.yysval.ast_node),
          NULL);
    } break;

    case 314:
/* Line 868 of glr.c  */
#line 1088 "sql.ypp"
    {
      ((*yyvalp).ast_node) = new AstInsertStmt(
          AST_INSERT_STMT, (((yyGLRStackItem const *)yyvsp)[YYFILL((2) - (7))]
                                .yystate.yysemantics.yysval.intval),
          string((((yyGLRStackItem const *)yyvsp)[YYFILL((4) - (7))]
                      .yystate.yysemantics.yysval.strval)),
          (((yyGLRStackItem const *)
            yyvsp)[YYFILL((5) - (7))].yystate.yysemantics.yysval.ast_node),
          NULL, (((yyGLRStackItem const *)yyvsp)[YYFILL((7) - (7))]
                     .yystate.yysemantics.yysval.ast_node),
          NULL, (((yyGLRStackItem const *)yyvsp)[YYFILL((6) - (7))]
                     .yystate.yysemantics.yysval.ast_node));
    } break;

    case 315:
/* Line 868 of glr.c  */
#line 1092 "sql.ypp"
    {
      if ((((yyGLRStackItem const *)
            yyvsp)[YYFILL((2) - (3))].yystate.yysemantics.yysval.subtok) != 4)
        yyerror(result, "bad insert assignment to %s",
                (((yyGLRStackItem const *)
                  yyvsp)[YYFILL((1) - (3))].yystate.yysemantics.yysval.strval));
      else
        ((*yyvalp).ast_node) = new AstInsertAssignList(
            AST_INSERT_ASSIGN_LIST,
            string((((yyGLRStackItem const *)yyvsp)[YYFILL((1) - (3))]
                        .yystate.yysemantics.yysval.strval)),
            0, (((yyGLRStackItem const *)
                 yyvsp)[YYFILL((3) - (3))].yystate.yysemantics.yysval.ast_node),
            NULL);
    } break;

    case 316:
/* Line 868 of glr.c  */
#line 1095 "sql.ypp"
    {
      if ((((yyGLRStackItem const *)
            yyvsp)[YYFILL((2) - (3))].yystate.yysemantics.yysval.subtok) != 4)
        yyerror(result, "bad insert assignment to %s",
                (((yyGLRStackItem const *)
                  yyvsp)[YYFILL((1) - (3))].yystate.yysemantics.yysval.strval));
      else
        ((*yyvalp).ast_node) = new AstInsertAssignList(
            AST_INSERT_ASSIGN_LIST,
            string((((yyGLRStackItem const *)yyvsp)[YYFILL((1) - (3))]
                        .yystate.yysemantics.yysval.strval)),
            1, NULL, NULL);
    } break;

    case 317:
/* Line 868 of glr.c  */
#line 1098 "sql.ypp"
    {
      if ((((yyGLRStackItem const *)
            yyvsp)[YYFILL((4) - (5))].yystate.yysemantics.yysval.subtok) != 4)
        yyerror(result, "bad insert assignment to %s",
                (((yyGLRStackItem const *)yyvsp)[YYFILL((1) - (5))]
                     .yystate.yysemantics.yysval.ast_node));
      else
        ((*yyvalp).ast_node) = new AstInsertAssignList(
            AST_INSERT_ASSIGN_LIST,
            string((((yyGLRStackItem const *)yyvsp)[YYFILL((3) - (5))]
                        .yystate.yysemantics.yysval.strval)),
            0, (((yyGLRStackItem const *)
                 yyvsp)[YYFILL((5) - (5))].yystate.yysemantics.yysval.ast_node),
            (((yyGLRStackItem const *)
              yyvsp)[YYFILL((1) - (5))].yystate.yysemantics.yysval.ast_node));
    } break;

    case 318:
/* Line 868 of glr.c  */
#line 1101 "sql.ypp"
    {
      if ((((yyGLRStackItem const *)
            yyvsp)[YYFILL((4) - (5))].yystate.yysemantics.yysval.subtok) != 4)
        yyerror(result, "bad insert assignment to %s",
                (((yyGLRStackItem const *)yyvsp)[YYFILL((1) - (5))]
                     .yystate.yysemantics.yysval.ast_node));
      else
        ((*yyvalp).ast_node) = new AstInsertAssignList(
            AST_INSERT_ASSIGN_LIST,
            string((((yyGLRStackItem const *)yyvsp)[YYFILL((3) - (5))]
                        .yystate.yysemantics.yysval.strval)),
            1, NULL, (((yyGLRStackItem const *)yyvsp)[YYFILL((1) - (5))]
                          .yystate.yysemantics.yysval.ast_node));
    } break;

    case 319:
/* Line 868 of glr.c  */
#line 1106 "sql.ypp"
    {
      ((*yyvalp).ast_node) =
          (((yyGLRStackItem const *)yyvsp)[YYFILL((1) - (1))]
               .yystate.yysemantics.yysval.ast_node); /*output($$, 1); puts("SQL
                                                         parser： This is a show
                                                         statement");*/
    } break;

    case 320:
/* Line 868 of glr.c  */
#line 1109 "sql.ypp"
    {
      ((*yyvalp).ast_node) = new AstShowStmt(
          AST_SHOW_STMT, 1, (((yyGLRStackItem const *)yyvsp)[YYFILL((2) - (5))]
                                 .yystate.yysemantics.yysval.intval),
          ((((yyGLRStackItem const *)yyvsp)[YYFILL((4) - (5))]
                .yystate.yysemantics.yysval.intval) == NULL)
              ? ""
              : string((((yyGLRStackItem const *)yyvsp)[YYFILL((4) - (5))]
                            .yystate.yysemantics.yysval.intval)),
          ((((yyGLRStackItem const *)yyvsp)[YYFILL((5) - (5))]
                .yystate.yysemantics.yysval.strval) == NULL)
              ? ""
              : string((((yyGLRStackItem const *)yyvsp)[YYFILL((5) - (5))]
                            .yystate.yysemantics.yysval.strval)));
    } break;

    case 321:
/* Line 868 of glr.c  */
#line 1110 "sql.ypp"
    {
      ((*yyvalp).ast_node) = NULL;
    } break;

    case 322:
/* Line 868 of glr.c  */
#line 1111 "sql.ypp"
    {
      ((*yyvalp).ast_node) = NULL;
    } break;

    case 323:
/* Line 868 of glr.c  */
#line 1112 "sql.ypp"
    {
      ((*yyvalp).ast_node) = NULL;
    } break;

    case 324:
/* Line 868 of glr.c  */
#line 1113 "sql.ypp"
    {
      ((*yyvalp).ast_node) = NULL;
    } break;

    case 325:
/* Line 868 of glr.c  */
#line 1114 "sql.ypp"
    {
      ((*yyvalp).ast_node) = NULL;
    } break;

    case 326:
/* Line 868 of glr.c  */
#line 1115 "sql.ypp"
    {
      ((*yyvalp).ast_node) = NULL;
    } break;

    case 327:
/* Line 868 of glr.c  */
#line 1116 "sql.ypp"
    {
      ((*yyvalp).ast_node) = NULL;
    } break;

    case 328:
/* Line 868 of glr.c  */
#line 1117 "sql.ypp"
    {
      ((*yyvalp).ast_node) = NULL;
    } break;

    case 329:
/* Line 868 of glr.c  */
#line 1118 "sql.ypp"
    {
      ((*yyvalp).ast_node) = NULL;
    } break;

    case 330:
/* Line 868 of glr.c  */
#line 1119 "sql.ypp"
    {
      ((*yyvalp).ast_node) = NULL;
    } break;

    case 331:
/* Line 868 of glr.c  */
#line 1120 "sql.ypp"
    {
      ((*yyvalp).ast_node) = NULL;
    } break;

    case 332:
/* Line 868 of glr.c  */
#line 1121 "sql.ypp"
    {
      ((*yyvalp).ast_node) = NULL;
    } break;

    case 333:
/* Line 868 of glr.c  */
#line 1122 "sql.ypp"
    {
      ((*yyvalp).ast_node) = NULL;
    } break;

    case 334:
/* Line 868 of glr.c  */
#line 1123 "sql.ypp"
    {
      ((*yyvalp).ast_node) = NULL;
    } break;

    case 335:
/* Line 868 of glr.c  */
#line 1124 "sql.ypp"
    {
      ((*yyvalp).ast_node) = NULL;
    } break;

    case 336:
/* Line 868 of glr.c  */
#line 1125 "sql.ypp"
    {
      ((*yyvalp).ast_node) = NULL;
    } break;

    case 337:
/* Line 868 of glr.c  */
#line 1126 "sql.ypp"
    {
      ((*yyvalp).ast_node) = NULL;
    } break;

    case 338:
/* Line 868 of glr.c  */
#line 1127 "sql.ypp"
    {
      ((*yyvalp).ast_node) = NULL;
    } break;

    case 339:
/* Line 868 of glr.c  */
#line 1128 "sql.ypp"
    {
      ((*yyvalp).ast_node) = NULL;
    } break;

    case 340:
/* Line 868 of glr.c  */
#line 1129 "sql.ypp"
    {
      ((*yyvalp).ast_node) = NULL;
    } break;

    case 341:
/* Line 868 of glr.c  */
#line 1130 "sql.ypp"
    {
      ((*yyvalp).ast_node) = NULL;
    } break;

    case 342:
/* Line 868 of glr.c  */
#line 1131 "sql.ypp"
    {
      ((*yyvalp).ast_node) = NULL;
    } break;

    case 343:
/* Line 868 of glr.c  */
#line 1132 "sql.ypp"
    {
      ((*yyvalp).ast_node) = NULL;
    } break;

    case 344:
/* Line 868 of glr.c  */
#line 1133 "sql.ypp"
    {
      ((*yyvalp).ast_node) = NULL;
    } break;

    case 345:
/* Line 868 of glr.c  */
#line 1134 "sql.ypp"
    {
      ((*yyvalp).ast_node) = NULL;
    } break;

    case 346:
/* Line 868 of glr.c  */
#line 1135 "sql.ypp"
    {
      ((*yyvalp).ast_node) = NULL;
    } break;

    case 347:
/* Line 868 of glr.c  */
#line 1137 "sql.ypp"
    {
      ((*yyvalp).intval) = 0;
    } break;

    case 348:
/* Line 868 of glr.c  */
#line 1138 "sql.ypp"
    {
      ((*yyvalp).intval) = 1;
    } break;

    case 349:
/* Line 868 of glr.c  */
#line 1141 "sql.ypp"
    {
      ((*yyvalp).intval) = NULL;
    } break;

    case 350:
/* Line 868 of glr.c  */
#line 1142 "sql.ypp"
    {
      ((*yyvalp).intval) = (((yyGLRStackItem const *)yyvsp)[YYFILL((2) - (2))]
                                .yystate.yysemantics.yysval.strval);
    } break;

    case 351:
/* Line 868 of glr.c  */
#line 1145 "sql.ypp"
    {
      ((*yyvalp).strval) = NULL;
    } break;

    case 352:
/* Line 868 of glr.c  */
#line 1146 "sql.ypp"
    {
      ((*yyvalp).strval) = (((yyGLRStackItem const *)yyvsp)[YYFILL((2) - (2))]
                                .yystate.yysemantics.yysval.strval);
    } break;

    case 357:
/* Line 868 of glr.c  */
#line 1156 "sql.ypp"
    {
      ((*yyvalp).intval) = 0;
    } break;

    case 358:
/* Line 868 of glr.c  */
#line 1157 "sql.ypp"
    {
      ((*yyvalp).intval) = 1;
    } break;

    case 359:
/* Line 868 of glr.c  */
#line 1158 "sql.ypp"
    {
      ((*yyvalp).intval) = 2;
    } break;

    case 360:
/* Line 868 of glr.c  */
#line 1161 "sql.ypp"
    {
      ((*yyvalp).ast_node) = (((yyGLRStackItem const *)yyvsp)[YYFILL((1) - (1))]
                                  .yystate.yysemantics.yysval.ast_node);
    } break;

    case 361:
/* Line 868 of glr.c  */
#line 1165 "sql.ypp"
    {
      ((*yyvalp).ast_node) = new AstDeleteStmt(
          AST_DELETE_STMT, (((yyGLRStackItem const *)yyvsp)[YYFILL((4) - (7))]
                                .yystate.yysemantics.yysval.ast_node),
          (((yyGLRStackItem const *)
            yyvsp)[YYFILL((5) - (7))].yystate.yysemantics.yysval.ast_node),
          (((yyGLRStackItem const *)
            yyvsp)[YYFILL((2) - (7))].yystate.yysemantics.yysval.ast_node));
    } break;

    case 362:
/* Line 868 of glr.c  */
#line 1169 "sql.ypp"
    {
      emit("DELETEMULTI %d %d %d",
           (((yyGLRStackItem const *)
             yyvsp)[YYFILL((2) - (7))].yystate.yysemantics.yysval.ast_node),
           (((yyGLRStackItem const *)
             yyvsp)[YYFILL((4) - (7))].yystate.yysemantics.yysval.ast_node),
           (((yyGLRStackItem const *)
             yyvsp)[YYFILL((6) - (7))].yystate.yysemantics.yysval.ast_node));
    } break;

    case 363:
/* Line 868 of glr.c  */
#line 1174 "sql.ypp"
    {
    } break;

    case 364:
/* Line 868 of glr.c  */
#line 1177 "sql.ypp"
    {
      ((*yyvalp).ast_node) = (((yyGLRStackItem const *)yyvsp)[YYFILL((1) - (2))]
                                  .yystate.yysemantics.yysval.ast_node) +
                             01;
    } break;

    case 365:
/* Line 868 of glr.c  */
#line 1178 "sql.ypp"
    {
      ((*yyvalp).ast_node) = (((yyGLRStackItem const *)yyvsp)[YYFILL((1) - (2))]
                                  .yystate.yysemantics.yysval.ast_node) +
                             02;
    } break;

    case 366:
/* Line 868 of glr.c  */
#line 1179 "sql.ypp"
    {
      ((*yyvalp).ast_node) = (((yyGLRStackItem const *)yyvsp)[YYFILL((1) - (2))]
                                  .yystate.yysemantics.yysval.ast_node) +
                             04;
    } break;

    case 367:
/* Line 868 of glr.c  */
#line 1180 "sql.ypp"
    {
      ((*yyvalp).ast_node) = 0;
    } break;

    case 368:
/* Line 868 of glr.c  */
#line 1184 "sql.ypp"
    {
    } break;

    case 369:
/* Line 868 of glr.c  */
#line 1185 "sql.ypp"
    {
    } break;

/* Line 868 of glr.c  */
#line 4488 "sql.tab.cpp"
    default:
      break;
  }

  return yyok;
#undef yyerrok
#undef YYABORT
#undef YYACCEPT
#undef YYERROR
#undef YYBACKUP
#undef yyclearin
#undef YYRECOVERING
}

/*ARGSUSED*/ static void yyuserMerge(int yyn, YYSTYPE *yy0, YYSTYPE *yy1) {
  YYUSE(yy0);
  YYUSE(yy1);

  switch (yyn) {
    default:
      break;
  }
}

/* Bison grammar-table manipulation.  */

/*-----------------------------------------------.
| Release the memory associated to this symbol.  |
`-----------------------------------------------*/

/*ARGSUSED*/
static void yydestruct(const char *yymsg, int yytype, YYSTYPE *yyvaluep,
                       struct ParseResult *result) {
  YYUSE(yyvaluep);
  YYUSE(result);

  if (!yymsg) yymsg = "Deleting";
  YY_SYMBOL_PRINT(yymsg, yytype, yyvaluep, yylocationp);

  switch (yytype) {
    default:
      break;
  }
}

/** Number of symbols composing the right hand side of rule #RULE.  */
static inline int yyrhsLength(yyRuleNum yyrule) { return yyr2[yyrule]; }

static void yydestroyGLRState(char const *yymsg, yyGLRState *yys,
                              struct ParseResult *result) {
  if (yys->yyresolved)
    yydestruct(yymsg, yystos[yys->yylrState], &yys->yysemantics.yysval, result);
  else {
#if YYDEBUG
    if (yydebug) {
      if (yys->yysemantics.yyfirstVal)
        YYFPRINTF(stderr, "%s unresolved ", yymsg);
      else
        YYFPRINTF(stderr, "%s incomplete ", yymsg);
      yy_symbol_print(stderr, yystos[yys->yylrState], YY_NULL, result);
      YYFPRINTF(stderr, "\n");
    }
#endif

    if (yys->yysemantics.yyfirstVal) {
      yySemanticOption *yyoption = yys->yysemantics.yyfirstVal;
      yyGLRState *yyrh;
      int yyn;
      for (yyrh = yyoption->yystate, yyn = yyrhsLength(yyoption->yyrule);
           yyn > 0; yyrh = yyrh->yypred, yyn -= 1)
        yydestroyGLRState(yymsg, yyrh, result);
    }
  }
}

/** Left-hand-side symbol for rule #RULE.  */
static inline yySymbol yylhsNonterm(yyRuleNum yyrule) { return yyr1[yyrule]; }

#define yypact_value_is_default(Yystate) (!!((Yystate) == (-470)))

/** True iff LR state STATE has only a default reduction (regardless
 *  of token).  */
static inline yybool yyisDefaultedState(yyStateNum yystate) {
  return yypact_value_is_default(yypact[yystate]);
}

/** The default reduction for STATE, assuming it has one.  */
static inline yyRuleNum yydefaultAction(yyStateNum yystate) {
  return yydefact[yystate];
}

#define yytable_value_is_error(Yytable_value) (!!((Yytable_value) == (-371)))

/** Set *YYACTION to the action to take in YYSTATE on seeing YYTOKEN.
 *  Result R means
 *    R < 0:  Reduce on rule -R.
 *    R = 0:  Error.
 *    R > 0:  Shift to state R.
 *  Set *CONFLICTS to a pointer into yyconfl to 0-terminated list of
 *  conflicting reductions.
 */
static inline void yygetLRActions(yyStateNum yystate, int yytoken,
                                  int *yyaction,
                                  const short int **yyconflicts) {
  int yyindex = yypact[yystate] + yytoken;
  if (yypact_value_is_default(yypact[yystate]) || yyindex < 0 ||
      YYLAST < yyindex || yycheck[yyindex] != yytoken) {
    *yyaction = -yydefact[yystate];
    *yyconflicts = yyconfl;
  } else if (!yytable_value_is_error(yytable[yyindex])) {
    *yyaction = yytable[yyindex];
    *yyconflicts = yyconfl + yyconflp[yyindex];
  } else {
    *yyaction = 0;
    *yyconflicts = yyconfl + yyconflp[yyindex];
  }
}

static inline yyStateNum yyLRgotoState(yyStateNum yystate, yySymbol yylhs) {
  int yyr;
  yyr = yypgoto[yylhs - YYNTOKENS] + yystate;
  if (0 <= yyr && yyr <= YYLAST && yycheck[yyr] == yystate)
    return yytable[yyr];
  else
    return yydefgoto[yylhs - YYNTOKENS];
}

static inline yybool yyisShiftAction(int yyaction) { return 0 < yyaction; }

static inline yybool yyisErrorAction(int yyaction) { return yyaction == 0; }

/* GLRStates */

/** Return a fresh GLRStackItem.  Callers should call
 * YY_RESERVE_GLRSTACK afterwards to make sure there is sufficient
 * headroom.  */

static inline yyGLRStackItem *yynewGLRStackItem(yyGLRStack *yystackp,
                                                yybool yyisState) {
  yyGLRStackItem *yynewItem = yystackp->yynextFree;
  yystackp->yyspaceLeft -= 1;
  yystackp->yynextFree += 1;
  yynewItem->yystate.yyisState = yyisState;
  return yynewItem;
}

/** Add a new semantic action that will execute the action for rule
 *  RULENUM on the semantic values in RHS to the list of
 *  alternative actions for STATE.  Assumes that RHS comes from
 *  stack #K of *STACKP. */
static void yyaddDeferredAction(yyGLRStack *yystackp, size_t yyk,
                                yyGLRState *yystate, yyGLRState *rhs,
                                yyRuleNum yyrule) {
  yySemanticOption *yynewOption =
      &yynewGLRStackItem(yystackp, yyfalse)->yyoption;
  yynewOption->yystate = rhs;
  yynewOption->yyrule = yyrule;
  if (yystackp->yytops.yylookaheadNeeds[yyk]) {
    yynewOption->yyrawchar = yychar;
    yynewOption->yyval = yylval;
  } else
    yynewOption->yyrawchar = YYEMPTY;
  yynewOption->yynext = yystate->yysemantics.yyfirstVal;
  yystate->yysemantics.yyfirstVal = yynewOption;

  YY_RESERVE_GLRSTACK(yystackp);
}

/* GLRStacks */

/** Initialize SET to a singleton set containing an empty stack.  */
static yybool yyinitStateSet(yyGLRStateSet *yyset) {
  yyset->yysize = 1;
  yyset->yycapacity = 16;
  yyset->yystates = (yyGLRState **)YYMALLOC(16 * sizeof yyset->yystates[0]);
  if (!yyset->yystates) return yyfalse;
  yyset->yystates[0] = YY_NULL;
  yyset->yylookaheadNeeds =
      (yybool *)YYMALLOC(16 * sizeof yyset->yylookaheadNeeds[0]);
  if (!yyset->yylookaheadNeeds) {
    YYFREE(yyset->yystates);
    return yyfalse;
  }
  return yytrue;
}

static void yyfreeStateSet(yyGLRStateSet *yyset) {
  YYFREE(yyset->yystates);
  YYFREE(yyset->yylookaheadNeeds);
}

/** Initialize STACK to a single empty stack, with total maximum
 *  capacity for all stacks of SIZE.  */
static yybool yyinitGLRStack(yyGLRStack *yystackp, size_t yysize) {
  yystackp->yyerrState = 0;
  yynerrs = 0;
  yystackp->yyspaceLeft = yysize;
  yystackp->yyitems =
      (yyGLRStackItem *)YYMALLOC(yysize * sizeof yystackp->yynextFree[0]);
  if (!yystackp->yyitems) return yyfalse;
  yystackp->yynextFree = yystackp->yyitems;
  yystackp->yysplitPoint = YY_NULL;
  yystackp->yylastDeleted = YY_NULL;
  return yyinitStateSet(&yystackp->yytops);
}

#if YYSTACKEXPANDABLE
#define YYRELOC(YYFROMITEMS, YYTOITEMS, YYX, YYTYPE) \
  &((YYTOITEMS) - ((YYFROMITEMS) - (yyGLRStackItem *)(YYX)))->YYTYPE

/** If STACK is expandable, extend it.  WARNING: Pointers into the
    stack from outside should be considered invalid after this call.
    We always expand when there are 1 or fewer items left AFTER an
    allocation, so that we can avoid having external pointers exist
    across an allocation.  */
static void yyexpandGLRStack(yyGLRStack *yystackp) {
  yyGLRStackItem *yynewItems;
  yyGLRStackItem *yyp0, *yyp1;
  size_t yynewSize;
  size_t yyn;
  size_t yysize = yystackp->yynextFree - yystackp->yyitems;
  if (YYMAXDEPTH - YYHEADROOM < yysize) yyMemoryExhausted(yystackp);
  yynewSize = 2 * yysize;
  if (YYMAXDEPTH < yynewSize) yynewSize = YYMAXDEPTH;
  yynewItems = (yyGLRStackItem *)YYMALLOC(yynewSize * sizeof yynewItems[0]);
  if (!yynewItems) yyMemoryExhausted(yystackp);
  for (yyp0 = yystackp->yyitems, yyp1 = yynewItems, yyn = yysize; 0 < yyn;
       yyn -= 1, yyp0 += 1, yyp1 += 1) {
    *yyp1 = *yyp0;
    if (*(yybool *)yyp0) {
      yyGLRState *yys0 = &yyp0->yystate;
      yyGLRState *yys1 = &yyp1->yystate;
      if (yys0->yypred != YY_NULL)
        yys1->yypred = YYRELOC(yyp0, yyp1, yys0->yypred, yystate);
      if (!yys0->yyresolved && yys0->yysemantics.yyfirstVal != YY_NULL)
        yys1->yysemantics.yyfirstVal =
            YYRELOC(yyp0, yyp1, yys0->yysemantics.yyfirstVal, yyoption);
    } else {
      yySemanticOption *yyv0 = &yyp0->yyoption;
      yySemanticOption *yyv1 = &yyp1->yyoption;
      if (yyv0->yystate != YY_NULL)
        yyv1->yystate = YYRELOC(yyp0, yyp1, yyv0->yystate, yystate);
      if (yyv0->yynext != YY_NULL)
        yyv1->yynext = YYRELOC(yyp0, yyp1, yyv0->yynext, yyoption);
    }
  }
  if (yystackp->yysplitPoint != YY_NULL)
    yystackp->yysplitPoint =
        YYRELOC(yystackp->yyitems, yynewItems, yystackp->yysplitPoint, yystate);

  for (yyn = 0; yyn < yystackp->yytops.yysize; yyn += 1)
    if (yystackp->yytops.yystates[yyn] != YY_NULL)
      yystackp->yytops.yystates[yyn] =
          YYRELOC(yystackp->yyitems, yynewItems, yystackp->yytops.yystates[yyn],
                  yystate);
  YYFREE(yystackp->yyitems);
  yystackp->yyitems = yynewItems;
  yystackp->yynextFree = yynewItems + yysize;
  yystackp->yyspaceLeft = yynewSize - yysize;
}
#endif

static void yyfreeGLRStack(yyGLRStack *yystackp) {
  YYFREE(yystackp->yyitems);
  yyfreeStateSet(&yystackp->yytops);
}

/** Assuming that S is a GLRState somewhere on STACK, update the
 *  splitpoint of STACK, if needed, so that it is at least as deep as
 *  S.  */
static inline void yyupdateSplit(yyGLRStack *yystackp, yyGLRState *yys) {
  if (yystackp->yysplitPoint != YY_NULL && yystackp->yysplitPoint > yys)
    yystackp->yysplitPoint = yys;
}

/** Invalidate stack #K in STACK.  */
static inline void yymarkStackDeleted(yyGLRStack *yystackp, size_t yyk) {
  if (yystackp->yytops.yystates[yyk] != YY_NULL)
    yystackp->yylastDeleted = yystackp->yytops.yystates[yyk];
  yystackp->yytops.yystates[yyk] = YY_NULL;
}

/** Undelete the last stack that was marked as deleted.  Can only be
    done once after a deletion, and only when all other stacks have
    been deleted.  */
static void yyundeleteLastStack(yyGLRStack *yystackp) {
  if (yystackp->yylastDeleted == YY_NULL || yystackp->yytops.yysize != 0)
    return;
  yystackp->yytops.yystates[0] = yystackp->yylastDeleted;
  yystackp->yytops.yysize = 1;
  YYDPRINTF((stderr, "Restoring last deleted stack as stack #0.\n"));
  yystackp->yylastDeleted = YY_NULL;
}

static inline void yyremoveDeletes(yyGLRStack *yystackp) {
  size_t yyi, yyj;
  yyi = yyj = 0;
  while (yyj < yystackp->yytops.yysize) {
    if (yystackp->yytops.yystates[yyi] == YY_NULL) {
      if (yyi == yyj) {
        YYDPRINTF((stderr, "Removing dead stacks.\n"));
      }
      yystackp->yytops.yysize -= 1;
    } else {
      yystackp->yytops.yystates[yyj] = yystackp->yytops.yystates[yyi];
      /* In the current implementation, it's unnecessary to copy
         yystackp->yytops.yylookaheadNeeds[yyi] since, after
         yyremoveDeletes returns, the parser immediately either enters
         deterministic operation or shifts a token.  However, it doesn't
         hurt, and the code might evolve to need it.  */
      yystackp->yytops.yylookaheadNeeds[yyj] =
          yystackp->yytops.yylookaheadNeeds[yyi];
      if (yyj != yyi) {
        YYDPRINTF((stderr, "Rename stack %lu -> %lu.\n", (unsigned long int)yyi,
                   (unsigned long int)yyj));
      }
      yyj += 1;
    }
    yyi += 1;
  }
}

/** Shift to a new state on stack #K of STACK, corresponding to LR state
 * LRSTATE, at input position POSN, with (resolved) semantic value SVAL.  */
static inline void yyglrShift(yyGLRStack *yystackp, size_t yyk,
                              yyStateNum yylrState, size_t yyposn,
                              YYSTYPE *yyvalp) {
  yyGLRState *yynewState = &yynewGLRStackItem(yystackp, yytrue)->yystate;

  yynewState->yylrState = yylrState;
  yynewState->yyposn = yyposn;
  yynewState->yyresolved = yytrue;
  yynewState->yypred = yystackp->yytops.yystates[yyk];
  yynewState->yysemantics.yysval = *yyvalp;
  yystackp->yytops.yystates[yyk] = yynewState;

  YY_RESERVE_GLRSTACK(yystackp);
}

/** Shift stack #K of YYSTACK, to a new state corresponding to LR
 *  state YYLRSTATE, at input position YYPOSN, with the (unresolved)
 *  semantic value of YYRHS under the action for YYRULE.  */
static inline void yyglrShiftDefer(yyGLRStack *yystackp, size_t yyk,
                                   yyStateNum yylrState, size_t yyposn,
                                   yyGLRState *rhs, yyRuleNum yyrule) {
  yyGLRState *yynewState = &yynewGLRStackItem(yystackp, yytrue)->yystate;

  yynewState->yylrState = yylrState;
  yynewState->yyposn = yyposn;
  yynewState->yyresolved = yyfalse;
  yynewState->yypred = yystackp->yytops.yystates[yyk];
  yynewState->yysemantics.yyfirstVal = YY_NULL;
  yystackp->yytops.yystates[yyk] = yynewState;

  /* Invokes YY_RESERVE_GLRSTACK.  */
  yyaddDeferredAction(yystackp, yyk, yynewState, rhs, yyrule);
}

/** Pop the symbols consumed by reduction #RULE from the top of stack
 *  #K of STACK, and perform the appropriate semantic action on their
 *  semantic values.  Assumes that all ambiguities in semantic values
 *  have been previously resolved.  Set *VALP to the resulting value,
 *  and *LOCP to the computed location (if any).  Return value is as
 *  for userAction.  */
static inline YYRESULTTAG yydoAction(yyGLRStack *yystackp, size_t yyk,
                                     yyRuleNum yyrule, YYSTYPE *yyvalp,
                                     struct ParseResult *result) {
  int yynrhs = yyrhsLength(yyrule);

  if (yystackp->yysplitPoint == YY_NULL) {
    /* Standard special case: single stack.  */
    yyGLRStackItem *rhs = (yyGLRStackItem *)yystackp->yytops.yystates[yyk];
    YYASSERT(yyk == 0);
    yystackp->yynextFree -= yynrhs;
    yystackp->yyspaceLeft += yynrhs;
    yystackp->yytops.yystates[0] = &yystackp->yynextFree[-1].yystate;
    return yyuserAction(yyrule, yynrhs, rhs, yystackp, yyvalp, result);
  } else {
    /* At present, doAction is never called in nondeterministic
     * mode, so this branch is never taken.  It is here in
     * anticipation of a future feature that will allow immediate
     * evaluation of selected actions in nondeterministic mode.  */
    int yyi;
    yyGLRState *yys;
    yyGLRStackItem yyrhsVals[YYMAXRHS + YYMAXLEFT + 1];
    yys = yyrhsVals[YYMAXRHS + YYMAXLEFT].yystate.yypred =
        yystackp->yytops.yystates[yyk];
    for (yyi = 0; yyi < yynrhs; yyi += 1) {
      yys = yys->yypred;
      YYASSERT(yys);
    }
    yyupdateSplit(yystackp, yys);
    yystackp->yytops.yystates[yyk] = yys;
    return yyuserAction(yyrule, yynrhs, yyrhsVals + YYMAXRHS + YYMAXLEFT - 1,
                        yystackp, yyvalp, result);
  }
}

#if !YYDEBUG
#define YY_REDUCE_PRINT(Args)
#else
#define YY_REDUCE_PRINT(Args)          \
  do {                                 \
    if (yydebug) yy_reduce_print Args; \
  } while (YYID(0))

/*----------------------------------------------------------.
| Report that the RULE is going to be reduced on stack #K.  |
`----------------------------------------------------------*/

/*ARGSUSED*/ static inline void yy_reduce_print(yyGLRStack *yystackp,
                                                size_t yyk, yyRuleNum yyrule,
                                                YYSTYPE *yyvalp,
                                                struct ParseResult *result) {
  int yynrhs = yyrhsLength(yyrule);
  yybool yynormal __attribute__((__unused__)) =
      (yystackp->yysplitPoint == YY_NULL);
  yyGLRStackItem *yyvsp = (yyGLRStackItem *)yystackp->yytops.yystates[yyk];
  int yylow = 1;
  int yyi;
  YYUSE(yyvalp);
  YYUSE(result);
  YYFPRINTF(stderr, "Reducing stack %lu by rule %d (line %lu):\n",
            (unsigned long int)yyk, yyrule - 1,
            (unsigned long int)yyrline[yyrule]);
  /* The symbols being reduced.  */
  for (yyi = 0; yyi < yynrhs; yyi++) {
    YYFPRINTF(stderr, "   $%d = ", yyi + 1);
    yy_symbol_print(
        stderr, yyrhs[yyprhs[yyrule] + yyi],
        &(((yyGLRStackItem const *)
           yyvsp)[YYFILL((yyi + 1) - (yynrhs))].yystate.yysemantics.yysval),
        result);
    YYFPRINTF(stderr, "\n");
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
static inline YYRESULTTAG yyglrReduce(yyGLRStack *yystackp, size_t yyk,
                                      yyRuleNum yyrule, yybool yyforceEval,
                                      struct ParseResult *result) {
  size_t yyposn = yystackp->yytops.yystates[yyk]->yyposn;

  if (yyforceEval || yystackp->yysplitPoint == YY_NULL) {
    YYSTYPE yysval;

    YY_REDUCE_PRINT((yystackp, yyk, yyrule, &yysval, result));
    YYCHK(yydoAction(yystackp, yyk, yyrule, &yysval, result));
    YY_SYMBOL_PRINT("-> $$ =", yyr1[yyrule], &yysval, &yyloc);
    yyglrShift(yystackp, yyk,
               yyLRgotoState(yystackp->yytops.yystates[yyk]->yylrState,
                             yylhsNonterm(yyrule)),
               yyposn, &yysval);
  } else {
    size_t yyi;
    int yyn;
    yyGLRState *yys, *yys0 = yystackp->yytops.yystates[yyk];
    yyStateNum yynewLRState;

    for (yys = yystackp->yytops.yystates[yyk], yyn = yyrhsLength(yyrule);
         0 < yyn; yyn -= 1) {
      yys = yys->yypred;
      YYASSERT(yys);
    }
    yyupdateSplit(yystackp, yys);
    yynewLRState = yyLRgotoState(yys->yylrState, yylhsNonterm(yyrule));
    YYDPRINTF(
        (stderr,
         "Reduced stack %lu by rule #%d; action deferred.  Now in state %d.\n",
         (unsigned long int)yyk, yyrule - 1, yynewLRState));
    for (yyi = 0; yyi < yystackp->yytops.yysize; yyi += 1)
      if (yyi != yyk && yystackp->yytops.yystates[yyi] != YY_NULL) {
        yyGLRState *yysplit = yystackp->yysplitPoint;
        yyGLRState *yyp = yystackp->yytops.yystates[yyi];
        while (yyp != yys && yyp != yysplit && yyp->yyposn >= yyposn) {
          if (yyp->yylrState == yynewLRState && yyp->yypred == yys) {
            yyaddDeferredAction(yystackp, yyk, yyp, yys0, yyrule);
            yymarkStackDeleted(yystackp, yyk);
            YYDPRINTF((stderr, "Merging stack %lu into stack %lu.\n",
                       (unsigned long int)yyk, (unsigned long int)yyi));
            return yyok;
          }
          yyp = yyp->yypred;
        }
      }
    yystackp->yytops.yystates[yyk] = yys;
    yyglrShiftDefer(yystackp, yyk, yynewLRState, yyposn, yys0, yyrule);
  }
  return yyok;
}

static size_t yysplitStack(yyGLRStack *yystackp, size_t yyk) {
  if (yystackp->yysplitPoint == YY_NULL) {
    YYASSERT(yyk == 0);
    yystackp->yysplitPoint = yystackp->yytops.yystates[yyk];
  }
  if (yystackp->yytops.yysize >= yystackp->yytops.yycapacity) {
    yyGLRState **yynewStates;
    yybool *yynewLookaheadNeeds;

    yynewStates = YY_NULL;

    if (yystackp->yytops.yycapacity > (YYSIZEMAX / (2 * sizeof yynewStates[0])))
      yyMemoryExhausted(yystackp);
    yystackp->yytops.yycapacity *= 2;

    yynewStates = (yyGLRState **)YYREALLOC(
        yystackp->yytops.yystates,
        (yystackp->yytops.yycapacity * sizeof yynewStates[0]));
    if (yynewStates == YY_NULL) yyMemoryExhausted(yystackp);
    yystackp->yytops.yystates = yynewStates;

    yynewLookaheadNeeds = (yybool *)YYREALLOC(
        yystackp->yytops.yylookaheadNeeds,
        (yystackp->yytops.yycapacity * sizeof yynewLookaheadNeeds[0]));
    if (yynewLookaheadNeeds == YY_NULL) yyMemoryExhausted(yystackp);
    yystackp->yytops.yylookaheadNeeds = yynewLookaheadNeeds;
  }
  yystackp->yytops.yystates[yystackp->yytops.yysize] =
      yystackp->yytops.yystates[yyk];
  yystackp->yytops.yylookaheadNeeds[yystackp->yytops.yysize] =
      yystackp->yytops.yylookaheadNeeds[yyk];
  yystackp->yytops.yysize += 1;
  return yystackp->yytops.yysize - 1;
}

/** True iff Y0 and Y1 represent identical options at the top level.
 *  That is, they represent the same rule applied to RHS symbols
 *  that produce the same terminal symbols.  */
static yybool yyidenticalOptions(yySemanticOption *yyy0,
                                 yySemanticOption *yyy1) {
  if (yyy0->yyrule == yyy1->yyrule) {
    yyGLRState *yys0, *yys1;
    int yyn;
    for (yys0 = yyy0->yystate, yys1 = yyy1->yystate,
        yyn = yyrhsLength(yyy0->yyrule);
         yyn > 0; yys0 = yys0->yypred, yys1 = yys1->yypred, yyn -= 1)
      if (yys0->yyposn != yys1->yyposn) return yyfalse;
    return yytrue;
  } else
    return yyfalse;
}

/** Assuming identicalOptions (Y0,Y1), destructively merge the
 *  alternative semantic values for the RHS-symbols of Y1 and Y0.  */
static void yymergeOptionSets(yySemanticOption *yyy0, yySemanticOption *yyy1) {
  yyGLRState *yys0, *yys1;
  int yyn;
  for (yys0 = yyy0->yystate, yys1 = yyy1->yystate,
      yyn = yyrhsLength(yyy0->yyrule);
       yyn > 0; yys0 = yys0->yypred, yys1 = yys1->yypred, yyn -= 1) {
    if (yys0 == yys1)
      break;
    else if (yys0->yyresolved) {
      yys1->yyresolved = yytrue;
      yys1->yysemantics.yysval = yys0->yysemantics.yysval;
    } else if (yys1->yyresolved) {
      yys0->yyresolved = yytrue;
      yys0->yysemantics.yysval = yys1->yysemantics.yysval;
    } else {
      yySemanticOption **yyz0p = &yys0->yysemantics.yyfirstVal;
      yySemanticOption *yyz1 = yys1->yysemantics.yyfirstVal;
      while (YYID(yytrue)) {
        if (yyz1 == *yyz0p || yyz1 == YY_NULL)
          break;
        else if (*yyz0p == YY_NULL) {
          *yyz0p = yyz1;
          break;
        } else if (*yyz0p < yyz1) {
          yySemanticOption *yyz = *yyz0p;
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
static int yypreference(yySemanticOption *y0, yySemanticOption *y1) {
  yyRuleNum r0 = y0->yyrule, r1 = y1->yyrule;
  int p0 = yydprec[r0], p1 = yydprec[r1];

  if (p0 == p1) {
    if (yymerger[r0] == 0 || yymerger[r0] != yymerger[r1])
      return 0;
    else
      return 1;
  }
  if (p0 == 0 || p1 == 0) return 0;
  if (p0 < p1) return 3;
  if (p1 < p0) return 2;
  return 0;
}

static YYRESULTTAG yyresolveValue(yyGLRState *yys, yyGLRStack *yystackp,
                                  struct ParseResult *result);

/** Resolve the previous N states starting at and including state S.  If result
 *  != yyok, some states may have been left unresolved possibly with empty
 *  semantic option chains.  Regardless of whether result = yyok, each state
 *  has been left with consistent data so that yydestroyGLRState can be invoked
 *  if necessary.  */
static YYRESULTTAG yyresolveStates(yyGLRState *yys, int yyn,
                                   yyGLRStack *yystackp,
                                   struct ParseResult *result) {
  if (0 < yyn) {
    YYASSERT(yys->yypred);
    YYCHK(yyresolveStates(yys->yypred, yyn - 1, yystackp, result));
    if (!yys->yyresolved) YYCHK(yyresolveValue(yys, yystackp, result));
  }
  return yyok;
}

/** Resolve the states for the RHS of OPT, perform its user action, and return
 *  the semantic value and location.  Regardless of whether result = yyok, all
 *  RHS states have been destroyed (assuming the user action destroys all RHS
 *  semantic values if invoked).  */
static YYRESULTTAG yyresolveAction(yySemanticOption *yyopt,
                                   yyGLRStack *yystackp, YYSTYPE *yyvalp,
                                   struct ParseResult *result) {
  yyGLRStackItem yyrhsVals[YYMAXRHS + YYMAXLEFT + 1];
  int yynrhs = yyrhsLength(yyopt->yyrule);
  YYRESULTTAG yyflag =
      yyresolveStates(yyopt->yystate, yynrhs, yystackp, result);
  if (yyflag != yyok) {
    yyGLRState *yys;
    for (yys = yyopt->yystate; yynrhs > 0; yys = yys->yypred, yynrhs -= 1)
      yydestroyGLRState("Cleanup: popping", yys, result);
    return yyflag;
  }

  yyrhsVals[YYMAXRHS + YYMAXLEFT].yystate.yypred = yyopt->yystate;
  {
    int yychar_current = yychar;
    YYSTYPE yylval_current = yylval;
    yychar = yyopt->yyrawchar;
    yylval = yyopt->yyval;
    yyflag = yyuserAction(yyopt->yyrule, yynrhs,
                          yyrhsVals + YYMAXRHS + YYMAXLEFT - 1, yystackp,
                          yyvalp, result);
    yychar = yychar_current;
    yylval = yylval_current;
  }
  return yyflag;
}

#if YYDEBUG
static void yyreportTree(yySemanticOption *yyx, int yyindent) {
  int yynrhs = yyrhsLength(yyx->yyrule);
  int yyi;
  yyGLRState *yys;
  yyGLRState *yystates[1 + YYMAXRHS];
  yyGLRState yyleftmost_state;

  for (yyi = yynrhs, yys = yyx->yystate; 0 < yyi; yyi -= 1, yys = yys->yypred)
    yystates[yyi] = yys;
  if (yys == YY_NULL) {
    yyleftmost_state.yyposn = 0;
    yystates[0] = &yyleftmost_state;
  } else
    yystates[0] = yys;

  if (yyx->yystate->yyposn < yys->yyposn + 1)
    YYFPRINTF(stderr, "%*s%s -> <Rule %d, empty>\n", yyindent, "",
              yytokenName(yylhsNonterm(yyx->yyrule)), yyx->yyrule - 1);
  else
    YYFPRINTF(stderr, "%*s%s -> <Rule %d, tokens %lu .. %lu>\n", yyindent, "",
              yytokenName(yylhsNonterm(yyx->yyrule)), yyx->yyrule - 1,
              (unsigned long int)(yys->yyposn + 1),
              (unsigned long int)yyx->yystate->yyposn);
  for (yyi = 1; yyi <= yynrhs; yyi += 1) {
    if (yystates[yyi]->yyresolved) {
      if (yystates[yyi - 1]->yyposn + 1 > yystates[yyi]->yyposn)
        YYFPRINTF(stderr, "%*s%s <empty>\n", yyindent + 2, "",
                  yytokenName(yyrhs[yyprhs[yyx->yyrule] + yyi - 1]));
      else
        YYFPRINTF(stderr, "%*s%s <tokens %lu .. %lu>\n", yyindent + 2, "",
                  yytokenName(yyrhs[yyprhs[yyx->yyrule] + yyi - 1]),
                  (unsigned long int)(yystates[yyi - 1]->yyposn + 1),
                  (unsigned long int)yystates[yyi]->yyposn);
    } else
      yyreportTree(yystates[yyi]->yysemantics.yyfirstVal, yyindent + 2);
  }
}
#endif

/*ARGSUSED*/ static YYRESULTTAG yyreportAmbiguity(yySemanticOption *yyx0,
                                                  yySemanticOption *yyx1,
                                                  struct ParseResult *result) {
  YYUSE(yyx0);
  YYUSE(yyx1);

#if YYDEBUG
  YYFPRINTF(stderr, "Ambiguity detected.\n");
  YYFPRINTF(stderr, "Option 1,\n");
  yyreportTree(yyx0, 2);
  YYFPRINTF(stderr, "\nOption 2,\n");
  yyreportTree(yyx1, 2);
  YYFPRINTF(stderr, "\n");
#endif

  yyerror(result, YY_("syntax is ambiguous"));
  return yyabort;
}

/** Resolve the ambiguity represented in state S, perform the indicated
 *  actions, and set the semantic value of S.  If result != yyok, the chain of
 *  semantic options in S has been cleared instead or it has been left
 *  unmodified except that redundant options may have been removed.  Regardless
 *  of whether result = yyok, S has been left with consistent data so that
 *  yydestroyGLRState can be invoked if necessary.  */
static YYRESULTTAG yyresolveValue(yyGLRState *yys, yyGLRStack *yystackp,
                                  struct ParseResult *result) {
  yySemanticOption *yyoptionList = yys->yysemantics.yyfirstVal;
  yySemanticOption *yybest = yyoptionList;
  yySemanticOption **yypp;
  yybool yymerge = yyfalse;
  YYSTYPE yysval;
  YYRESULTTAG yyflag;

  for (yypp = &yyoptionList->yynext; *yypp != YY_NULL;) {
    yySemanticOption *yyp = *yypp;

    if (yyidenticalOptions(yybest, yyp)) {
      yymergeOptionSets(yybest, yyp);
      *yypp = yyp->yynext;
    } else {
      switch (yypreference(yybest, yyp)) {
        case 0:
          return yyreportAmbiguity(yybest, yyp, result);
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

  if (yymerge) {
    yySemanticOption *yyp;
    int yyprec = yydprec[yybest->yyrule];
    yyflag = yyresolveAction(yybest, yystackp, &yysval, result);
    if (yyflag == yyok)
      for (yyp = yybest->yynext; yyp != YY_NULL; yyp = yyp->yynext) {
        if (yyprec == yydprec[yyp->yyrule]) {
          YYSTYPE yysval_other;
          yyflag = yyresolveAction(yyp, yystackp, &yysval_other, result);
          if (yyflag != yyok) {
            yydestruct("Cleanup: discarding incompletely merged value for",
                       yystos[yys->yylrState], &yysval, result);
            break;
          }
          yyuserMerge(yymerger[yyp->yyrule], &yysval, &yysval_other);
        }
      }
  } else
    yyflag = yyresolveAction(yybest, yystackp, &yysval, result);

  if (yyflag == yyok) {
    yys->yyresolved = yytrue;
    yys->yysemantics.yysval = yysval;
  } else
    yys->yysemantics.yyfirstVal = YY_NULL;
  return yyflag;
}

static YYRESULTTAG yyresolveStack(yyGLRStack *yystackp,
                                  struct ParseResult *result) {
  if (yystackp->yysplitPoint != YY_NULL) {
    yyGLRState *yys;
    int yyn;

    for (yyn = 0, yys = yystackp->yytops.yystates[0];
         yys != yystackp->yysplitPoint; yys = yys->yypred, yyn += 1)
      continue;
    YYCHK(yyresolveStates(yystackp->yytops.yystates[0], yyn, yystackp, result));
  }
  return yyok;
}

static void yycompressStack(yyGLRStack *yystackp) {
  yyGLRState *yyp, *yyq, *yyr;

  if (yystackp->yytops.yysize != 1 || yystackp->yysplitPoint == YY_NULL) return;

  for (yyp = yystackp->yytops.yystates[0], yyq = yyp->yypred, yyr = YY_NULL;
       yyp != yystackp->yysplitPoint; yyr = yyp, yyp = yyq, yyq = yyp->yypred)
    yyp->yypred = yyr;

  yystackp->yyspaceLeft += yystackp->yynextFree - yystackp->yyitems;
  yystackp->yynextFree = ((yyGLRStackItem *)yystackp->yysplitPoint) + 1;
  yystackp->yyspaceLeft -= yystackp->yynextFree - yystackp->yyitems;
  yystackp->yysplitPoint = YY_NULL;
  yystackp->yylastDeleted = YY_NULL;

  while (yyr != YY_NULL) {
    yystackp->yynextFree->yystate = *yyr;
    yyr = yyr->yypred;
    yystackp->yynextFree->yystate.yypred = &yystackp->yynextFree[-1].yystate;
    yystackp->yytops.yystates[0] = &yystackp->yynextFree->yystate;
    yystackp->yynextFree += 1;
    yystackp->yyspaceLeft -= 1;
  }
}

static YYRESULTTAG yyprocessOneStack(yyGLRStack *yystackp, size_t yyk,
                                     size_t yyposn,
                                     struct ParseResult *result) {
  int yyaction;
  const short int *yyconflicts;
  yyRuleNum yyrule;

  while (yystackp->yytops.yystates[yyk] != YY_NULL) {
    yyStateNum yystate = yystackp->yytops.yystates[yyk]->yylrState;
    YYDPRINTF((stderr, "Stack %lu Entering state %d\n", (unsigned long int)yyk,
               yystate));

    YYASSERT(yystate != YYFINAL);

    if (yyisDefaultedState(yystate)) {
      yyrule = yydefaultAction(yystate);
      if (yyrule == 0) {
        YYDPRINTF((stderr, "Stack %lu dies.\n", (unsigned long int)yyk));
        yymarkStackDeleted(yystackp, yyk);
        return yyok;
      }
      YYCHK(yyglrReduce(yystackp, yyk, yyrule, yyfalse, result));
    } else {
      yySymbol yytoken;
      yystackp->yytops.yylookaheadNeeds[yyk] = yytrue;
      if (yychar == YYEMPTY) {
        YYDPRINTF((stderr, "Reading a token: "));
        yychar = YYLEX;
      }

      if (yychar <= YYEOF) {
        yychar = yytoken = YYEOF;
        YYDPRINTF((stderr, "Now at end of input.\n"));
      } else {
        yytoken = YYTRANSLATE(yychar);
        YY_SYMBOL_PRINT("Next token is", yytoken, &yylval, &yylloc);
      }

      yygetLRActions(yystate, yytoken, &yyaction, &yyconflicts);

      while (*yyconflicts != 0) {
        size_t yynewStack = yysplitStack(yystackp, yyk);
        YYDPRINTF((stderr, "Splitting off stack %lu from %lu.\n",
                   (unsigned long int)yynewStack, (unsigned long int)yyk));
        YYCHK(yyglrReduce(yystackp, yynewStack, *yyconflicts, yyfalse, result));
        YYCHK(yyprocessOneStack(yystackp, yynewStack, yyposn, result));
        yyconflicts += 1;
      }

      if (yyisShiftAction(yyaction))
        break;
      else if (yyisErrorAction(yyaction)) {
        YYDPRINTF((stderr, "Stack %lu dies.\n", (unsigned long int)yyk));
        yymarkStackDeleted(yystackp, yyk);
        break;
      } else
        YYCHK(yyglrReduce(yystackp, yyk, -yyaction, yyfalse, result));
    }
  }
  return yyok;
}

/*ARGSUSED*/ static void yyreportSyntaxError(yyGLRStack *yystackp,
                                             struct ParseResult *result) {
  if (yystackp->yyerrState != 0) return;
#if !YYERROR_VERBOSE
  yyerror(result, YY_("syntax error"));
#else
  {
    yySymbol yytoken = yychar == YYEMPTY ? YYEMPTY : YYTRANSLATE(yychar);
    size_t yysize0 = yytnamerr(YY_NULL, yytokenName(yytoken));
    size_t yysize = yysize0;
    yybool yysize_overflow = yyfalse;
    char *yymsg = YY_NULL;
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
    if (yytoken != YYEMPTY) {
      int yyn = yypact[yystackp->yytops.yystates[0]->yylrState];
      yyarg[yycount++] = yytokenName(yytoken);
      if (!yypact_value_is_default(yyn)) {
        /* Start YYX at -YYN if negative to avoid negative indexes in
           YYCHECK.  In other words, skip the first -YYN actions for this
           state because they are default actions.  */
        int yyxbegin = yyn < 0 ? -yyn : 0;
        /* Stay within bounds of both yycheck and yytname.  */
        int yychecklim = YYLAST - yyn + 1;
        int yyxend = yychecklim < YYNTOKENS ? yychecklim : YYNTOKENS;
        int yyx;
        for (yyx = yyxbegin; yyx < yyxend; ++yyx)
          if (yycheck[yyx + yyn] == yyx && yyx != YYTERROR &&
              !yytable_value_is_error(yytable[yyx + yyn])) {
            if (yycount == YYERROR_VERBOSE_ARGS_MAXIMUM) {
              yycount = 1;
              yysize = yysize0;
              break;
            }
            yyarg[yycount++] = yytokenName(yyx);
            {
              size_t yysz = yysize + yytnamerr(YY_NULL, yytokenName(yyx));
              yysize_overflow |= yysz < yysize;
              yysize = yysz;
            }
          }
      }
    }

    switch (yycount) {
#define YYCASE_(N, S) \
  case N:             \
    yyformat = S;     \
    break
      YYCASE_(0, YY_("syntax error"));
      YYCASE_(1, YY_("syntax error, unexpected %s"));
      YYCASE_(2, YY_("syntax error, unexpected %s, expecting %s"));
      YYCASE_(3, YY_("syntax error, unexpected %s, expecting %s or %s"));
      YYCASE_(4, YY_("syntax error, unexpected %s, expecting %s or %s or %s"));
      YYCASE_(
          5,
          YY_("syntax error, unexpected %s, expecting %s or %s or %s or %s"));
#undef YYCASE_
    }

    {
      size_t yysz = yysize + strlen(yyformat);
      yysize_overflow |= yysz < yysize;
      yysize = yysz;
    }

    if (!yysize_overflow) yymsg = (char *)YYMALLOC(yysize);

    if (yymsg) {
      char *yyp = yymsg;
      int yyi = 0;
      while ((*yyp = *yyformat)) {
        if (*yyp == '%' && yyformat[1] == 's' && yyi < yycount) {
          yyp += yytnamerr(yyp, yyarg[yyi++]);
          yyformat += 2;
        } else {
          yyp++;
          yyformat++;
        }
      }
      yyerror(result, yymsg);
      YYFREE(yymsg);
    } else {
      yyerror(result, YY_("syntax error"));
      yyMemoryExhausted(yystackp);
    }
  }
#endif /* YYERROR_VERBOSE */
  yynerrs += 1;
}

/* Recover from a syntax error on *YYSTACKP, assuming that *YYSTACKP->YYTOKENP,
   yylval, and yylloc are the syntactic category, semantic value, and location
   of the lookahead.  */
/*ARGSUSED*/ static void yyrecoverSyntaxError(yyGLRStack *yystackp,
                                              struct ParseResult *result) {
  size_t yyk;
  int yyj;

  if (yystackp->yyerrState == 3)
    /* We just shifted the error token and (perhaps) took some
       reductions.  Skip tokens until we can proceed.  */
    while (YYID(yytrue)) {
      yySymbol yytoken;
      if (yychar == YYEOF) yyFail(yystackp, result, YY_NULL);
      if (yychar != YYEMPTY) {
        yytoken = YYTRANSLATE(yychar);
        yydestruct("Error: discarding", yytoken, &yylval, result);
      }
      YYDPRINTF((stderr, "Reading a token: "));
      yychar = YYLEX;
      if (yychar <= YYEOF) {
        yychar = yytoken = YYEOF;
        YYDPRINTF((stderr, "Now at end of input.\n"));
      } else {
        yytoken = YYTRANSLATE(yychar);
        YY_SYMBOL_PRINT("Next token is", yytoken, &yylval, &yylloc);
      }
      yyj = yypact[yystackp->yytops.yystates[0]->yylrState];
      if (yypact_value_is_default(yyj)) return;
      yyj += yytoken;
      if (yyj < 0 || YYLAST < yyj || yycheck[yyj] != yytoken) {
        if (yydefact[yystackp->yytops.yystates[0]->yylrState] != 0) return;
      } else if (!yytable_value_is_error(yytable[yyj]))
        return;
    }

  /* Reduce to one stack.  */
  for (yyk = 0; yyk < yystackp->yytops.yysize; yyk += 1)
    if (yystackp->yytops.yystates[yyk] != YY_NULL) break;
  if (yyk >= yystackp->yytops.yysize) yyFail(yystackp, result, YY_NULL);
  for (yyk += 1; yyk < yystackp->yytops.yysize; yyk += 1)
    yymarkStackDeleted(yystackp, yyk);
  yyremoveDeletes(yystackp);
  yycompressStack(yystackp);

  /* Now pop stack until we find a state that shifts the error token.  */
  yystackp->yyerrState = 3;
  while (yystackp->yytops.yystates[0] != YY_NULL) {
    yyGLRState *yys = yystackp->yytops.yystates[0];
    yyj = yypact[yys->yylrState];
    if (!yypact_value_is_default(yyj)) {
      yyj += YYTERROR;
      if (0 <= yyj && yyj <= YYLAST && yycheck[yyj] == YYTERROR &&
          yyisShiftAction(yytable[yyj])) {
        /* Shift the error token.  */
        YY_SYMBOL_PRINT("Shifting", yystos[yytable[yyj]], &yylval, &yyerrloc);
        yyglrShift(yystackp, 0, yytable[yyj], yys->yyposn, &yylval);
        yys = yystackp->yytops.yystates[0];
        break;
      }
    }
    if (yys->yypred != YY_NULL)
      yydestroyGLRState("Error: popping", yys, result);
    yystackp->yytops.yystates[0] = yys->yypred;
    yystackp->yynextFree -= 1;
    yystackp->yyspaceLeft += 1;
  }
  if (yystackp->yytops.yystates[0] == YY_NULL)
    yyFail(yystackp, result, YY_NULL);
}

#define YYCHK1(YYE)        \
  do {                     \
    switch (YYE) {         \
      case yyok:           \
        break;             \
      case yyabort:        \
        goto yyabortlab;   \
      case yyaccept:       \
        goto yyacceptlab;  \
      case yyerr:          \
        goto yyuser_error; \
      default:             \
        goto yybuglab;     \
    }                      \
  } while (YYID(0))

/*----------.
| yyparse.  |
`----------*/

int yyparse(struct ParseResult *result) {
  int yyresult;
  yyGLRStack yystack;
  yyGLRStack *const yystackp = &yystack;
  size_t yyposn;

  YYDPRINTF((stderr, "Starting parse\n"));

  yychar = YYEMPTY;
  yylval = yyval_default;

  if (!yyinitGLRStack(yystackp, YYINITDEPTH)) goto yyexhaustedlab;
  switch (YYSETJMP(yystack.yyexception_buffer)) {
    case 0:
      break;
    case 1:
      goto yyabortlab;
    case 2:
      goto yyexhaustedlab;
    default:
      goto yybuglab;
  }
  yyglrShift(&yystack, 0, 0, 0, &yylval);
  yyposn = 0;

  while (YYID(yytrue)) {
    /* For efficiency, we have two loops, the first of which is
       specialized to deterministic operation (single stack, no
       potential ambiguity).  */
    /* Standard mode */
    while (YYID(yytrue)) {
      yyRuleNum yyrule;
      int yyaction;
      const short int *yyconflicts;

      yyStateNum yystate = yystack.yytops.yystates[0]->yylrState;
      YYDPRINTF((stderr, "Entering state %d\n", yystate));
      if (yystate == YYFINAL) goto yyacceptlab;
      if (yyisDefaultedState(yystate)) {
        yyrule = yydefaultAction(yystate);
        if (yyrule == 0) {
          yyreportSyntaxError(&yystack, result);
          goto yyuser_error;
        }
        YYCHK1(yyglrReduce(&yystack, 0, yyrule, yytrue, result));
      } else {
        yySymbol yytoken;
        if (yychar == YYEMPTY) {
          YYDPRINTF((stderr, "Reading a token: "));
          yychar = YYLEX;
        }

        if (yychar <= YYEOF) {
          yychar = yytoken = YYEOF;
          YYDPRINTF((stderr, "Now at end of input.\n"));
        } else {
          yytoken = YYTRANSLATE(yychar);
          YY_SYMBOL_PRINT("Next token is", yytoken, &yylval, &yylloc);
        }

        yygetLRActions(yystate, yytoken, &yyaction, &yyconflicts);
        if (*yyconflicts != 0) break;
        if (yyisShiftAction(yyaction)) {
          YY_SYMBOL_PRINT("Shifting", yytoken, &yylval, &yylloc);
          yychar = YYEMPTY;
          yyposn += 1;
          yyglrShift(&yystack, 0, yyaction, yyposn, &yylval);
          if (0 < yystack.yyerrState) yystack.yyerrState -= 1;
        } else if (yyisErrorAction(yyaction)) {
          yyreportSyntaxError(&yystack, result);
          goto yyuser_error;
        } else
          YYCHK1(yyglrReduce(&yystack, 0, -yyaction, yytrue, result));
      }
    }

    while (YYID(yytrue)) {
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
        YYCHK1(yyprocessOneStack(&yystack, yys, yyposn, result));
      yyremoveDeletes(&yystack);
      if (yystack.yytops.yysize == 0) {
        yyundeleteLastStack(&yystack);
        if (yystack.yytops.yysize == 0)
          yyFail(&yystack, result, YY_("syntax error"));
        YYCHK1(yyresolveStack(&yystack, result));
        YYDPRINTF((stderr, "Returning to deterministic operation.\n"));

        yyreportSyntaxError(&yystack, result);
        goto yyuser_error;
      }

      /* If any yyglrShift call fails, it will fail after shifting.  Thus,
         a copy of yylval will already be on stack 0 in the event of a
         failure in the following loop.  Thus, yychar is set to YYEMPTY
         before the loop to make sure the user destructor for yylval isn't
         called twice.  */
      yytoken_to_shift = YYTRANSLATE(yychar);
      yychar = YYEMPTY;
      yyposn += 1;
      for (yys = 0; yys < yystack.yytops.yysize; yys += 1) {
        int yyaction;
        const short int *yyconflicts;
        yyStateNum yystate = yystack.yytops.yystates[yys]->yylrState;
        yygetLRActions(yystate, yytoken_to_shift, &yyaction, &yyconflicts);
        /* Note that yyconflicts were handled by yyprocessOneStack.  */
        YYDPRINTF((stderr, "On stack %lu, ", (unsigned long int)yys));
        YY_SYMBOL_PRINT("shifting", yytoken_to_shift, &yylval, &yylloc);
        yyglrShift(&yystack, yys, yyaction, yyposn, &yylval);
        YYDPRINTF((stderr, "Stack %lu now in state #%d\n",
                   (unsigned long int)yys,
                   yystack.yytops.yystates[yys]->yylrState));
      }

      if (yystack.yytops.yysize == 1) {
        YYCHK1(yyresolveStack(&yystack, result));
        YYDPRINTF((stderr, "Returning to deterministic operation.\n"));
        yycompressStack(&yystack);
        break;
      }
    }
    continue;
  yyuser_error:
    yyrecoverSyntaxError(&yystack, result);
    yyposn = yystack.yytops.yystates[0]->yyposn;
  }

yyacceptlab:
  yyresult = 0;
  goto yyreturn;

yybuglab:
  YYASSERT(yyfalse);
  goto yyabortlab;

yyabortlab:
  yyresult = 1;
  goto yyreturn;

yyexhaustedlab:
  yyerror(result, YY_("memory exhausted"));
  yyresult = 2;
  goto yyreturn;

yyreturn:
  if (yychar != YYEMPTY)
    yydestruct("Cleanup: discarding lookahead", YYTRANSLATE(yychar), &yylval,
               result);

  /* If the stack is well-formed, pop the stack until it is empty,
     destroying its entries as we go.  But free the stack regardless
     of whether it is well-formed.  */
  if (yystack.yyitems) {
    yyGLRState **yystates = yystack.yytops.yystates;
    if (yystates) {
      size_t yysize = yystack.yytops.yysize;
      size_t yyk;
      for (yyk = 0; yyk < yysize; yyk += 1)
        if (yystates[yyk]) {
          while (yystates[yyk]) {
            yyGLRState *yys = yystates[yyk];
            if (yys->yypred != YY_NULL)
              yydestroyGLRState("Cleanup: popping", yys, result);
            yystates[yyk] = yys->yypred;
            yystack.yynextFree -= 1;
            yystack.yyspaceLeft += 1;
          }
          break;
        }
    }
    yyfreeGLRStack(&yystack);
  }

  /* Make sure YYID is used.  */
  return YYID(yyresult);
}

/* DEBUGGING ONLY */
#if YYDEBUG
static void yypstack(yyGLRStack *yystackp, size_t yyk)
    __attribute__((__unused__));
static void yypdumpstack(yyGLRStack *yystackp) __attribute__((__unused__));

static void yy_yypstack(yyGLRState *yys) {
  if (yys->yypred) {
    yy_yypstack(yys->yypred);
    YYFPRINTF(stderr, " -> ");
  }
  YYFPRINTF(stderr, "%d@%lu", yys->yylrState, (unsigned long int)yys->yyposn);
}

static void yypstates(yyGLRState *yyst) {
  if (yyst == YY_NULL)
    YYFPRINTF(stderr, "<null>");
  else
    yy_yypstack(yyst);
  YYFPRINTF(stderr, "\n");
}

static void yypstack(yyGLRStack *yystackp, size_t yyk) {
  yypstates(yystackp->yytops.yystates[yyk]);
}

#define YYINDEX(YYX) \
  ((YYX) == YY_NULL ? -1 : (yyGLRStackItem *)(YYX) - yystackp->yyitems)

static void yypdumpstack(yyGLRStack *yystackp) {
  yyGLRStackItem *yyp;
  size_t yyi;
  for (yyp = yystackp->yyitems; yyp < yystackp->yynextFree; yyp += 1) {
    YYFPRINTF(stderr, "%3lu. ", (unsigned long int)(yyp - yystackp->yyitems));
    if (*(yybool *)yyp) {
      YYFPRINTF(stderr, "Res: %d, LR State: %d, posn: %lu, pred: %ld",
                yyp->yystate.yyresolved, yyp->yystate.yylrState,
                (unsigned long int)yyp->yystate.yyposn,
                (long int)YYINDEX(yyp->yystate.yypred));
      if (!yyp->yystate.yyresolved)
        YYFPRINTF(stderr, ", firstVal: %ld",
                  (long int)YYINDEX(yyp->yystate.yysemantics.yyfirstVal));
    } else {
      YYFPRINTF(stderr, "Option. rule: %d, state: %ld, next: %ld",
                yyp->yyoption.yyrule - 1,
                (long int)YYINDEX(yyp->yyoption.yystate),
                (long int)YYINDEX(yyp->yyoption.yynext));
    }
    YYFPRINTF(stderr, "\n");
  }
  YYFPRINTF(stderr, "Tops:");
  for (yyi = 0; yyi < yystackp->yytops.yysize; yyi += 1)
    YYFPRINTF(stderr, "%lu: %ld; ", (unsigned long int)yyi,
              (long int)YYINDEX(yystackp->yytops.yystates[yyi]));
  YYFPRINTF(stderr, "\n");
}
#endif
/* Line 2575 of glr.c  */
#line 1192 "sql.ypp"

void emit(char *s, ...) {
  /*
  extern int yylineno;
  va_list ap;
  va_start(ap, s);
  printf("rpn: ");
  vfprintf(stdout, s, ap);
  printf("\n");
  */
}

void yyerror(struct ParseResult *pp, const char *s, ...) {
  va_list ap;
  va_start(ap, s);

  string sql_clause = pp->sql_clause;
  vector<string> tokens;
  boost::split(tokens, sql_clause, boost::is_any_of("\n"));
  ostringstream ostr;
  int columnno = 0;
  int lineno = yyget_lineno(pp->yyscan_info_);
  lineno = lineno % tokens.size();
  lineno++;
  // Because find() can only find the first place where the word appears,
  // so this method may not find the certain place.
  /*
  for (int i = 0; i < tokens.size(); i++) {
    columnno = tokens[i].find(yyget_text(pp->yyscan_info_)) + 1;
    if (-1 != columnno && ((lineno - 1) == i)) {
      cout << "In clause \'"
           << "\e[1m" << tokens[i] << "\e[0m\'" << endl;
      for (int j = 0; j < (columnno + 9); j++) {
        cout << ".";
      }
      cout << "^" << endl;
      ostr << "In clause \'"
           << "\e[1m" << tokens[i] << "\e[0m\'" << endl;
      for (int j = 0; j < (columnno + 9); j++) {
        ostr << ".";
      }
      ostr << "^" << endl;
      break;
    }
  }
  */
  string wrong_clause = tokens[lineno - 1];
  vector<string> words_in_clause;
  boost::split(words_in_clause, tokens[lineno - 1], boost::is_any_of(" "));
  int yyget_col_no = yyget_column(pp->yyscan_info_);
  if (yyget_col_no == 0) {
    yyget_col_no = 1;
  }
  cout << "yyget_col_no = " << yyget_col_no << endl;
  int column_num = 1;
  for (int i = 0; i < words_in_clause.size(); i++) {
    if (i == (yyget_col_no - 1)) {
      cout << "In clause \'"
           << "\e[1m" << tokens[lineno - 1] << "\e[0m\'" << endl;
      ostr << "In clause \'"
           << "\e[1m" << tokens[lineno - 1] << "\e[0m\'" << endl;
      for (int j = 0; j < (column_num + 10); j++) {
        ostr << ".";
        cout << ".";
      }
      cout << "^" << endl;
      ostr << "^" << endl;

      break;
    } else {
      column_num = column_num + words_in_clause[i].size() + 1;
    }
  }
  ostr << "SQL syntax error at \e[1mline: " << lineno << ","
       << "\e[0m near \'\e[1m";
  ostr << yyget_text(pp->yyscan_info_);
  // ostr << "\e[1mLINE: " << lineno << "," << columnno << "\e[0m error: ";
  // ostr << "near \'\e[1m";
  // ostr << yyget_text(pp->yyscan_info_);
  ostr << "\e[0m\'." << endl;
  pp->error_info_ = ostr.str();
  cout << "SQL syntax error at \e[1mline: " << lineno << ","
       << "\e[0m near \'\e[1m";
  cout << yyget_text(pp->yyscan_info_);
  cout << "\e[0m\'." << endl;
  /*
  cout << "\e[1mLINE: " << lineno << "\e[0m error: ";
  cout << "near \'\e[1m";
  cout << yyget_text(pp->yyscan_info_);
  cout << "\e[0m\'." << endl;
  */
  yyset_column(0, pp->yyscan_info_);

  vfprintf(stderr, s, ap);
  fprintf(stderr, "\n");
}
