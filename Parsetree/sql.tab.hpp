
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
     PARTITIONED = 411,
     PRECISION = 412,
     PRIMARY = 413,
     PROCEDURE = 414,
     PROJECTION = 415,
     PURGE = 416,
     QUICK = 417,
     READ = 418,
     READS = 419,
     REAL = 420,
     REFERENCES = 421,
     RELEASE = 422,
     RENAME = 423,
     REPEAT = 424,
     REPLACE = 425,
     REQUIRE = 426,
     RESTRICT = 427,
     RETURN = 428,
     REVOKE = 429,
     RIGHT = 430,
     ROLLUP = 431,
     SCHEMA = 432,
     SCHEMAS = 433,
     SECOND_MICROSECOND = 434,
     SELECT = 435,
     SENSITIVE = 436,
     SEPARATOR = 437,
     SET = 438,
     SHOW = 439,
     SMALLINT = 440,
     SOME = 441,
     SONAME = 442,
     SPATIAL = 443,
     SPECIFIC = 444,
     SQL = 445,
     SQLEXCEPTION = 446,
     SQLSTATE = 447,
     SQLWARNING = 448,
     SQL_BIG_RESULT = 449,
     SQL_CALC_FOUND_ROWS = 450,
     SQL_SMALL_RESULT = 451,
     SSL = 452,
     STARTING = 453,
     STRAIGHT_JOIN = 454,
     TABLE = 455,
     TEMPORARY = 456,
     TEXT = 457,
     TERMINATED = 458,
     THEN = 459,
     TIME = 460,
     TIMESTAMP = 461,
     TINYBLOB = 462,
     TINYINT = 463,
     TINYTEXT = 464,
     TO = 465,
     TRAILING = 466,
     TRIGGER = 467,
     UNDO = 468,
     UNION = 469,
     UNIQUE = 470,
     UNLOCK = 471,
     UNSIGNED = 472,
     UPDATE = 473,
     USAGE = 474,
     USE = 475,
     USING = 476,
     UTC_DATE = 477,
     UTC_TIME = 478,
     UTC_TIMESTAMP = 479,
     VALUES = 480,
     VARBINARY = 481,
     VARCHAR = 482,
     VARYING = 483,
     WHEN = 484,
     WHERE = 485,
     WHILE = 486,
     WITH = 487,
     WRITE = 488,
     YEAR = 489,
     YEAR_MONTH = 490,
     ZEROFILL = 491,
     DO = 492,
     MAX_QUERIES_PER_HOUR = 493,
     MAX_UPDATES_PER_HOUR = 494,
     MAX_CONNECTIONS_PER_HOUR = 495,
     MAX_USER_CONNECTIONS = 496,
     USER = 497,
     TRUNCATE = 498,
     FAST = 499,
     MEDIUM = 500,
     EXTENDED = 501,
     CHANGED = 502,
     LEAVES = 503,
     MASTER = 504,
     QUERY = 505,
     CACHE = 506,
     SLAVE = 507,
     BEGINT = 508,
     COMMIT = 509,
     START = 510,
     TRANSACTION = 511,
     NO = 512,
     CHAIN = 513,
     AUTOCOMMIT = 514,
     SAVEPOINT = 515,
     ROLLBACK = 516,
     LOCAL = 517,
     TABLES = 518,
     ISOLATION = 519,
     LEVEL = 520,
     GLOBAL = 521,
     SESSION = 522,
     UNCOMMITTED = 523,
     COMMITTED = 524,
     REPEATABLE = 525,
     SERIALIZABLE = 526,
     IDENTIFIED = 527,
     PASSWORD = 528,
     PRIVILEGES = 529,
     BACKUP = 530,
     CHECKSUM = 531,
     REPAIR = 532,
     USE_FRM = 533,
     RESTORE = 534,
     CHARACTER = 535,
     COLLATION = 536,
     COLUMNS = 537,
     ENGINE = 538,
     LOGS = 539,
     STATUS = 540,
     STORAGE = 541,
     ENGINES = 542,
     ERRORS = 543,
     GRANTS = 544,
     INNODB = 545,
     PROCESSLIST = 546,
     TRIGGERS = 547,
     VARIABLES = 548,
     WARNINGS = 549,
     FLUSH = 550,
     HOSTS = 551,
     DES_KEY_FILE = 552,
     USER_RESOURCES = 553,
     CONNECTION = 554,
     RESET = 555,
     PREPARE = 556,
     DEALLOCATE = 557,
     EXECUTE = 558,
     WORK = 559,
     BTREE = 560,
     HASH = 561,
     BDB = 562,
     OPEN = 563,
     FULL = 564,
     FSUBSTRING = 565,
     FTRIM = 566,
     FDATE_ADD = 567,
     FDATE_SUB = 568,
     FCOUNT = 569,
     FSUM = 570,
     FAVG = 571,
     FMIN = 572,
     FMAX = 573
   };
#endif


#ifndef YYSTYPE
typedef union YYSTYPE
{

/* Line 2638 of glr.c  */
#line 46 "sql.ypp"
		// TODO:2014-3-9---新增一个支持最大数字的类型，dicimal，将所有识别到的整数和浮点数存为该类型，若要存入某个表中，再根据这个列的类型进行转化。 ！！！！！
	int intval;
	double floatval;
	char *strval;
	int subtok;
	struct Node * ast_node;



/* Line 2638 of glr.c  */
#line 378 "sql.tab.hpp"
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


