
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
     DAY = 319,
     DAY_MICROSECOND = 320,
     DAY_MINUTE = 321,
     DAY_SECOND = 322,
     DECIMAL = 323,
     DECLARE = 324,
     DEFAULT = 325,
     DELAYED = 326,
     DELETE = 327,
     DESC = 328,
     DESCRIBE = 329,
     DETERMINISTIC = 330,
     DISTINCT = 331,
     DISTINCTROW = 332,
     DIV = 333,
     DOUBLE = 334,
     DROP = 335,
     DUAL = 336,
     EACH = 337,
     ELSE = 338,
     ELSEIF = 339,
     ENCLOSED = 340,
     END = 341,
     ENUM = 342,
     ESCAPED = 343,
     EXISTS = 344,
     EXIT = 345,
     EXPLAIN = 346,
     FETCH = 347,
     FLOAT = 348,
     FOR = 349,
     FORCE = 350,
     FOREIGN = 351,
     FROM = 352,
     FULLTEXT = 353,
     GRANT = 354,
     GROUP = 355,
     HAVING = 356,
     HIGH_PRIORITY = 357,
     HOUR_MICROSECOND = 358,
     HOUR_MINUTE = 359,
     HOUR_SECOND = 360,
     IF = 361,
     IGNORE = 362,
     INDEX = 363,
     INFILE = 364,
     INNER = 365,
     INOUT = 366,
     INSENSITIVE = 367,
     INSERT = 368,
     INT = 369,
     INTEGER = 370,
     INTERVAL = 371,
     INTO = 372,
     ITERATE = 373,
     JOIN = 374,
     KEY = 375,
     KEYS = 376,
     KILL = 377,
     LEADING = 378,
     LEAVE = 379,
     LEFT = 380,
     LIMIT = 381,
     LINES = 382,
     LOAD = 383,
     LOCALTIME = 384,
     LOCALTIMESTAMP = 385,
     LOCK = 386,
     LONG = 387,
     LONGBLOB = 388,
     LONGTEXT = 389,
     LOOP = 390,
     LOW_PRIORITY = 391,
     MATCH = 392,
     MEDIUMBLOB = 393,
     MEDIUMINT = 394,
     MEDIUMTEXT = 395,
     MINUTE_MICROSECOND = 396,
     MINUTE_SECOND = 397,
     MODIFIES = 398,
     NATURAL = 399,
     NO_WRITE_TO_BINLOG = 400,
     NULLX = 401,
     NUMBER = 402,
     ON = 403,
     DUPLICATE = 404,
     OPTIMIZE = 405,
     OPTION = 406,
     OPTIONALLY = 407,
     ORDER = 408,
     OUT = 409,
     OUTER = 410,
     OUTFILE = 411,
     PARTITIONED = 412,
     PRECISION = 413,
     PRIMARY = 414,
     PROCEDURE = 415,
     PROJECTION = 416,
     PURGE = 417,
     QUICK = 418,
     QUARTER = 419,
     READ = 420,
     READS = 421,
     REAL = 422,
     REFERENCES = 423,
     RELEASE = 424,
     RENAME = 425,
     REPEAT = 426,
     REPLACE = 427,
     REQUIRE = 428,
     RESTRICT = 429,
     RETURN = 430,
     REVOKE = 431,
     RIGHT = 432,
     ROLLUP = 433,
     SCHEMA = 434,
     SCHEMAS = 435,
     SECOND_MICROSECOND = 436,
     SELECT = 437,
     SENSITIVE = 438,
     SEPARATOR = 439,
     SET = 440,
     SHOW = 441,
     SMALLINT = 442,
     SOME = 443,
     SONAME = 444,
     SPATIAL = 445,
     SPECIFIC = 446,
     SQL = 447,
     SQLEXCEPTION = 448,
     SQLSTATE = 449,
     SQLWARNING = 450,
     SQL_BIG_RESULT = 451,
     SQL_CALC_FOUND_ROWS = 452,
     SQL_SMALL_RESULT = 453,
     SSL = 454,
     STARTING = 455,
     STRAIGHT_JOIN = 456,
     TABLE = 457,
     TEMPORARY = 458,
     TEXT = 459,
     TERMINATED = 460,
     THEN = 461,
     TIME = 462,
     TIMESTAMP = 463,
     TINYBLOB = 464,
     TINYINT = 465,
     TINYTEXT = 466,
     TO = 467,
     TRAILING = 468,
     TRIGGER = 469,
     UNDO = 470,
     UNION = 471,
     UNIQUE = 472,
     UNLOCK = 473,
     UNSIGNED = 474,
     UPDATE = 475,
     USAGE = 476,
     USE = 477,
     USING = 478,
     UTC_DATE = 479,
     UTC_TIME = 480,
     UTC_TIMESTAMP = 481,
     VALUES = 482,
     VARBINARY = 483,
     VARCHAR = 484,
     VARYING = 485,
     WHEN = 486,
     WHERE = 487,
     WHILE = 488,
     WITH = 489,
     WRITE = 490,
     YEAR = 491,
     YEAR_MONTH = 492,
     ZEROFILL = 493,
     WEEK = 494,
     DO = 495,
     MAX_QUERIES_PER_HOUR = 496,
     MAX_UPDATES_PER_HOUR = 497,
     MAX_CONNECTIONS_PER_HOUR = 498,
     MAX_USER_CONNECTIONS = 499,
     USER = 500,
     TRUNCATE = 501,
     FAST = 502,
     MEDIUM = 503,
     EXTENDED = 504,
     CHANGED = 505,
     LEAVES = 506,
     MASTER = 507,
     QUERY = 508,
     CACHE = 509,
     SLAVE = 510,
     BEGINT = 511,
     COMMIT = 512,
     START = 513,
     TRANSACTION = 514,
     NO = 515,
     CHAIN = 516,
     AUTOCOMMIT = 517,
     SAVEPOINT = 518,
     ROLLBACK = 519,
     LOCAL = 520,
     TABLES = 521,
     ISOLATION = 522,
     LEVEL = 523,
     GLOBAL = 524,
     SESSION = 525,
     UNCOMMITTED = 526,
     COMMITTED = 527,
     REPEATABLE = 528,
     SERIALIZABLE = 529,
     IDENTIFIED = 530,
     PASSWORD = 531,
     PRIVILEGES = 532,
     BACKUP = 533,
     CHECKSUM = 534,
     REPAIR = 535,
     USE_FRM = 536,
     RESTORE = 537,
     CHARACTER = 538,
     COLLATION = 539,
     COLUMNS = 540,
     ENGINE = 541,
     LOGS = 542,
     STATUS = 543,
     STORAGE = 544,
     ENGINES = 545,
     ERRORS = 546,
     GRANTS = 547,
     INNODB = 548,
     PROCESSLIST = 549,
     TRIGGERS = 550,
     VARIABLES = 551,
     WARNINGS = 552,
     FLUSH = 553,
     HOSTS = 554,
     DES_KEY_FILE = 555,
     USER_RESOURCES = 556,
     CONNECTION = 557,
     RESET = 558,
     PREPARE = 559,
     DEALLOCATE = 560,
     EXECUTE = 561,
     WORK = 562,
     BTREE = 563,
     HASH = 564,
     BDB = 565,
     OPEN = 566,
     FULL = 567,
     FSUBSTRING = 568,
     FTRIM = 569,
     FDATE_ADD = 570,
     FDATE_SUB = 571,
     FCOUNT = 572,
     FUPPER = 573,
     FCAST = 574,
     FCOALESCE = 575,
     FCONVERT = 576,
     FSUM = 577,
     FAVG = 578,
     FMIN = 579,
     FMAX = 580
   };
#endif


#ifndef YYSTYPE
typedef union YYSTYPE
{

/* Line 2638 of glr.c  */
#line 47 "sql.ypp"

	int intval;		// no used ---4-28
	double floatval;	// no used ---4-28
	char *strval;
	int subtok;
	struct Node * ast_node;



/* Line 2638 of glr.c  */
#line 385 "sql.tab.hpp"
} YYSTYPE;
# define YYSTYPE_IS_TRIVIAL 1
#endif

#if ! defined YYLTYPE && ! defined YYLTYPE_IS_DECLARED
typedef struct YYLTYPE
{

  int first_line;
  int first_column;
  int last_line;
  int last_column;

} YYLTYPE;
# define YYLTYPE_IS_DECLARED 1
# define YYLTYPE_IS_TRIVIAL 1
#endif



extern YYSTYPE yylval;

extern YYLTYPE yylloc;


