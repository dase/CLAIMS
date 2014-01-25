
/* A Bison parser, made by GNU Bison 2.4.1.  */

/* Skeleton interface for Bison GLR parsers in C
   
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


/* Tokens.  */
#ifndef YYTOKENTYPE
# define YYTOKENTYPE
   /* Put the tokens into the symbol table, so that GDB and other debuggers
      know about them.  */
   enum yytokentype {
     NAME = 258,
     STRING = 259,
     INTNUM = 260,
     BOOL = 261,
     APPROXNUM = 262,
     USERVAR = 263,
     ASSIGN = 264,
     OR = 265,
     XOR = 266,
     ANDOP = 267,
     REGEXP = 268,
     LIKE = 269,
     IS = 270,
     IN = 271,
     NOT = 272,
     BETWEEN = 273,
     COMPARISON = 274,
     SHIFT = 275,
     MOD = 276,
     UMINUS = 277,
     ADD = 278,
     ALL = 279,
     ALTER = 280,
     ANALYZE = 281,
     AND = 282,
     ANY = 283,
     AS = 284,
     ASC = 285,
     AUTO_INCREMENT = 286,
     BEFORE = 287,
     BIGINT = 288,
     BINARY = 289,
     BIT = 290,
     BLOB = 291,
     BOTH = 292,
     BY = 293,
     CALL = 294,
     CASCADE = 295,
     CASE = 296,
     CHANGE = 297,
     CHAR = 298,
     CHECK = 299,
     COLLATE = 300,
     COLUMN = 301,
     COMMENT = 302,
     CONDITION = 303,
     CONSTRAINT = 304,
     CONTINUE = 305,
     CONVERT = 306,
     CREATE = 307,
     CROSS = 308,
     CURRENT_DATE = 309,
     CURRENT_TIME = 310,
     CURRENT_TIMESTAMP = 311,
     CURRENT_USER = 312,
     CURSOR = 313,
     DATABASE = 314,
     DATABASES = 315,
     DATE = 316,
     DATETIME = 317,
     DAY_HOUR = 318,
     DAY_MICROSECOND = 319,
     DAY_MINUTE = 320,
     DAY_SECOND = 321,
     DECIMAL = 322,
     DECLARE = 323,
     DEFAULT = 324,
     DELAYED = 325,
     DELETE = 326,
     DESC = 327,
     DESCRIBE = 328,
     DETERMINISTIC = 329,
     DISTINCT = 330,
     DISTINCTROW = 331,
     DIV = 332,
     DOUBLE = 333,
     DROP = 334,
     DUAL = 335,
     EACH = 336,
     ELSE = 337,
     ELSEIF = 338,
     ENCLOSED = 339,
     END = 340,
     ENUM = 341,
     ESCAPED = 342,
     EXISTS = 343,
     EXIT = 344,
     EXPLAIN = 345,
     FETCH = 346,
     FLOAT = 347,
     FOR = 348,
     FORCE = 349,
     FOREIGN = 350,
     FROM = 351,
     FULLTEXT = 352,
     GRANT = 353,
     GROUP = 354,
     HAVING = 355,
     HIGH_PRIORITY = 356,
     HOUR_MICROSECOND = 357,
     HOUR_MINUTE = 358,
     HOUR_SECOND = 359,
     IF = 360,
     IGNORE = 361,
     INDEX = 362,
     INFILE = 363,
     INNER = 364,
     INOUT = 365,
     INSENSITIVE = 366,
     INSERT = 367,
     INT = 368,
     INTEGER = 369,
     INTERVAL = 370,
     INTO = 371,
     ITERATE = 372,
     JOIN = 373,
     KEY = 374,
     KEYS = 375,
     KILL = 376,
     LEADING = 377,
     LEAVE = 378,
     LEFT = 379,
     LIMIT = 380,
     LINES = 381,
     LOAD = 382,
     LOCALTIME = 383,
     LOCALTIMESTAMP = 384,
     LOCK = 385,
     LONG = 386,
     LONGBLOB = 387,
     LONGTEXT = 388,
     LOOP = 389,
     LOW_PRIORITY = 390,
     MATCH = 391,
     MEDIUMBLOB = 392,
     MEDIUMINT = 393,
     MEDIUMTEXT = 394,
     MINUTE_MICROSECOND = 395,
     MINUTE_SECOND = 396,
     MODIFIES = 397,
     NATURAL = 398,
     NO_WRITE_TO_BINLOG = 399,
     NULLX = 400,
     NUMBER = 401,
     ON = 402,
     DUPLICATE = 403,
     OPTIMIZE = 404,
     OPTION = 405,
     OPTIONALLY = 406,
     ORDER = 407,
     OUT = 408,
     OUTER = 409,
     OUTFILE = 410,
     PRECISION = 411,
     PRIMARY = 412,
     PROCEDURE = 413,
     PURGE = 414,
     QUICK = 415,
     READ = 416,
     READS = 417,
     REAL = 418,
     REFERENCES = 419,
     RELEASE = 420,
     RENAME = 421,
     REPEAT = 422,
     REPLACE = 423,
     REQUIRE = 424,
     RESTRICT = 425,
     RETURN = 426,
     REVOKE = 427,
     RIGHT = 428,
     ROLLUP = 429,
     SCHEMA = 430,
     SCHEMAS = 431,
     SECOND_MICROSECOND = 432,
     SELECT = 433,
     SENSITIVE = 434,
     SEPARATOR = 435,
     SET = 436,
     SHOW = 437,
     SMALLINT = 438,
     SOME = 439,
     SONAME = 440,
     SPATIAL = 441,
     SPECIFIC = 442,
     SQL = 443,
     SQLEXCEPTION = 444,
     SQLSTATE = 445,
     SQLWARNING = 446,
     SQL_BIG_RESULT = 447,
     SQL_CALC_FOUND_ROWS = 448,
     SQL_SMALL_RESULT = 449,
     SSL = 450,
     STARTING = 451,
     STRAIGHT_JOIN = 452,
     TABLE = 453,
     TEMPORARY = 454,
     TEXT = 455,
     TERMINATED = 456,
     THEN = 457,
     TIME = 458,
     TIMESTAMP = 459,
     TINYBLOB = 460,
     TINYINT = 461,
     TINYTEXT = 462,
     TO = 463,
     TRAILING = 464,
     TRIGGER = 465,
     UNDO = 466,
     UNION = 467,
     UNIQUE = 468,
     UNLOCK = 469,
     UNSIGNED = 470,
     UPDATE = 471,
     USAGE = 472,
     USE = 473,
     USING = 474,
     UTC_DATE = 475,
     UTC_TIME = 476,
     UTC_TIMESTAMP = 477,
     VALUES = 478,
     VARBINARY = 479,
     VARCHAR = 480,
     VARYING = 481,
     WHEN = 482,
     WHERE = 483,
     WHILE = 484,
     WITH = 485,
     WRITE = 486,
     YEAR = 487,
     YEAR_MONTH = 488,
     ZEROFILL = 489,
     FSUBSTRING = 490,
     FTRIM = 491,
     FDATE_ADD = 492,
     FDATE_SUB = 493,
     FCOUNT = 494
   };
#endif


#ifndef YYSTYPE
typedef union YYSTYPE
{

/* Line 2638 of glr.c  */
#line 44 "sql.ypp"

	int intval;
	double floatval;
	char *strval;
	int subtok;
	struct Node * ast_node;



/* Line 2638 of glr.c  */
#line 299 "sql.tab.hpp"
} YYSTYPE;
# define YYSTYPE_IS_TRIVIAL 1
#endif

#if ! defined YYLTYPE && ! defined YYLTYPE_IS_DECLARED
typedef struct YYLTYPE
{

  char yydummy;

} YYLTYPE;
# define YYLTYPE_IS_DECLARED 1
# define YYLTYPE_IS_TRIVIAL 1
#endif



extern YYSTYPE yylval;



