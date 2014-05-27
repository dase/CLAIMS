/* Companion source code for "flex & bison", published by O'Reilly
 * Media, ISBN 978-0-596-15597-1
 * Copyright (c) 2009, Taughannock Networks. All rights reserved.
 * See the README file for license conditions and contact info.
 * $Header: /home/johnl/flnb/code/sql/RCS/glrmysql.l,v 2.1 2009/11/08 02:53:39 johnl Exp $
 */
/*
 * Scanner for mysql subset, GLR version
 */

%option noyywrap nodefault yylineno case-insensitive

%{
#include "sql_node_struct.h"
#include "sql.tab.hpp"

#include <stdarg.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <malloc.h>

extern "C" int yylex();
extern char globalInputText[10000];
extern int readInputForLexer( char *buffer, int *numBytesRead, int maxBytesToRead );
int oldstate;
extern char linebuf[500];

#undef YY_INPUT
#define YY_INPUT(b,r,s) readInputForLexer(b,&r,s)

%}

%x COMMENT
%s BTWMODE

%%

  /* keywords */

ADD	{ return ADD; }
ALL	{ return ALL; }
ALTER	{ return ALTER; }
ANALYZE	{ return ANALYZE; }

  /* Hack for BETWEEN ... AND ... 
   * return special AND token if BETWEEN seen
   */
<BTWMODE>AND	{ BEGIN INITIAL; return AND; }
AND	{ return ANDOP; }
ANY	{ return ANY; }
AS	{ return AS; }
ASC	{ return ASC; }
AUTO_INCREMENT	{ return AUTO_INCREMENT; }
BEFORE	{ return BEFORE; }
BETWEEN	{ BEGIN BTWMODE; return BETWEEN; }
INT8|BIGINT	{ return BIGINT; }
BINARY	{ return BINARY; }
BIT	{ return BIT; }
BLOB	{ return BLOB; }
BOTH	{ return BOTH; }
BTREE	{ return BTREE;}	// 2014-3-24---增加---by Yu
BY	{ return BY; }
CALL	{ return CALL; }
CASCADE	{ return CASCADE; }
CASE	{ return CASE; }
CHANGE	{ return CHANGE; }
CHAR(ACTER)?	{ return CHAR; }
CHECK	{ return CHECK; }
COLLATE	{ return COLLATE; }
COLUMN	{ return COLUMN; }
COMMENT	{ return COMMENT; }
CONDITION	{ return CONDITION; }
CONSTRAINT	{ return CONSTRAINT; }
CONTINUE	{ return CONTINUE; }
CONVERT	{ return CONVERT; }
CREATE	{ return CREATE; }
CROSS	{ return CROSS; }
CURRENT_DATE	{ return CURRENT_DATE; }
CURRENT_TIME	{ return CURRENT_TIME; }
CURRENT_TIMESTAMP	{ return CURRENT_TIMESTAMP; }
CURRENT_USER	{ return CURRENT_USER; }
CURSOR	{ return CURSOR; }
DATABASE	{ return DATABASE; }
DATABASES	{ return DATABASES; }
DAY   { return DAY;}//---5.26fzh---
DATE	{ return DATE; }
DATETIME	{ return DATETIME; }
HOUR	{ return DAY_HOUR; }//---5.26fzh---
MICROSECOND	{ return DAY_MICROSECOND; }//---5.26fzh---
MINUTE	{ return DAY_MINUTE; }//---5.26fzh---
SECOND	{ return DAY_SECOND; }//---5.26fzh---
NUMERIC|DEC|DECIMAL	{ return DECIMAL; }
DECLARE	{ return DECLARE; }
DEFAULT	{ return DEFAULT; }
DELAYED	{ return DELAYED; }
DELETE	{ return DELETE; }
DESC	{ return DESC; }
DESCRIBE	{ return DESCRIBE; }
DETERMINISTIC	{ return DETERMINISTIC; }
DISTINCT	{ return DISTINCT; }
DISTINCTROW	{ return DISTINCTROW; }
DIV	{ return DIV; }
FLOAT8|DOUBLE	{ return DOUBLE; }
DROP	{ return DROP; }
DUAL	{ return DUAL; }
EACH	{ return EACH; }
ELSE	{ return ELSE; }
ELSEIF	{ return ELSEIF; }
END	{ return END; }
ENUM { return ENUM; }
ESCAPED	{ return ESCAPED; }
EXISTS	{ return EXISTS; }
EXIT	{ return EXIT; }
EXPLAIN	{ return EXPLAIN; }
FETCH	{ return FETCH; }
FLOAT4?	{ return FLOAT; }
FOR	{ return FOR; }
FORCE	{ return FORCE; }
FOREIGN	{ return FOREIGN; }
FROM	{ return FROM; }
FULLTEXT	{ return FULLTEXT; }
GRANT	{ return GRANT; }
GROUP	{ return GROUP; }
HASH	{ return HASH; }	// 2014-3-24---增加---by Yu
HAVING	{ return HAVING; }
HIGH_PRIORITY	{ return HIGH_PRIORITY; }
HOUR_MICROSECOND	{ return HOUR_MICROSECOND; }
HOUR_MINUTE	{ return HOUR_MINUTE; }
HOUR_SECOND	{ return HOUR_SECOND; }
IF	{ return IF; }
IGNORE	{ return IGNORE; }
IN	{ return IN; }
INFILE	{ return INFILE; }
INNER	{ return INNER; }
INOUT	{ return INOUT; }
INSENSITIVE	{ return INSENSITIVE; }
INSERT	{ return INSERT; }
INT4?|INTEGER|INT	{ return INTEGER; }
INTERVAL	{ return INTERVAL; }
INTO	{ return INTO; }
IS	{ return IS; }
ITERATE	{ return ITERATE; }
JOIN	{ return JOIN; }
INDEX	{ return INDEX; }	// 2014-3-24---增加---by Yu
KEY	{ return KEY; }	// 2014-3-24---修改---by Yu
KEYS	{ return KEYS; }
KILL	{ return KILL; }
LEADING	{ return LEADING; }
LEAVE	{ return LEAVE; }
LEFT	{ return LEFT; }
LIKE	{ return LIKE; }
LIMIT	{ return LIMIT; }
LINES	{ return LINES; }
LOAD	{ return LOAD; }
LOCALTIME	{ return LOCALTIME; }
LOCALTIMESTAMP	{ return LOCALTIMESTAMP; }
LOCK	{ return LOCK; }
LONG	{ return LONG; }
LONGBLOB	{ return LONGBLOB; }
LONGTEXT	{ return LONGTEXT; }
LOOP	{ return LOOP; }
LOW_PRIORITY	{ return LOW_PRIORITY; }
MATCH	{ return MATCH; }
MEDIUMBLOB	{ return MEDIUMBLOB; }
MIDDLEINT|MEDIUMINT	{ return MEDIUMINT; }
MEDIUMTEXT	{ return MEDIUMTEXT; }
MINUTE_MICROSECOND	{ return MINUTE_MICROSECOND; }
MINUTE_SECOND	{ return MINUTE_SECOND; }
MOD	{ return MOD; }
MODIFIES	{ return MODIFIES; }
NATURAL	{ return NATURAL; }
NOT	{ return NOT; }
NO_WRITE_TO_BINLOG	{ return NO_WRITE_TO_BINLOG; }
NULL	{ return NULLX; }
NUMBER	{ return NUMBER; }
ON	{ return ON; }
DUPLICATE { return DUPLICATE; }
OPTIMIZE	{ return OPTIMIZE; }
OPTION	{ return OPTION; }
OPTIONALLY	{ return OPTIONALLY; }
OR	{ return OR; }
ORDER	{ return ORDER; }
OUT	{ return OUT; }
OUTER	{ return OUTER; }
OUTFILE	{ return OUTFILE; }
PARTITIONED	{ return PARTITIONED; }	// 2014-2-24---增加对这个词的识别---by余楷
PRECISION	{ return PRECISION; }
PRIMARY	{ return PRIMARY; }
PROCEDURE	{ return PROCEDURE; }
PROJECTION	{ return PROJECTION; }	// 2014-2-24---增加对这个词的识别---by余楷
PURGE	{ return PURGE; }
QUICK	{ return QUICK; }
QUARTER { return QUARTER;}
READ	{ return READ; }
READS	{ return READS; }
REAL	{ return REAL; }
REFERENCES	{ return REFERENCES; }
REGEXP|RLIKE	{ return REGEXP; }
RELEASE	{ return RELEASE; }
RENAME	{ return RENAME; }
REPEAT	{ return REPEAT; }
REPLACE	{ return REPLACE; }
REQUIRE	{ return REQUIRE; }
RESTRICT	{ return RESTRICT; }
RETURN	{ return RETURN; }
REVOKE	{ return REVOKE; }
RIGHT	{ return RIGHT; }
ROLLUP	{ return ROLLUP; }
SCHEMA	{ return SCHEMA; }
SCHEMAS	{ return SCHEMAS; }
SECOND_MICROSECOND	{ return SECOND_MICROSECOND; }
SELECT	{ return SELECT; }
SENSITIVE	{ return SENSITIVE; }
SEPARATOR	{ return SEPARATOR; }
SET	{ return SET; }
SHOW	{ return SHOW; }
INT2|SMALLINT	{ return SMALLINT; }
SOME	{ return SOME; }
SONAME	{ return SONAME; }
SPATIAL	{ return SPATIAL; }
SPECIFIC	{ return SPECIFIC; }
SQL	{ return SQL; }
SQLEXCEPTION	{ return SQLEXCEPTION; }
SQLSTATE	{ return SQLSTATE; }
SQLWARNING	{ return SQLWARNING; }
SQL_BIG_RESULT	{ return SQL_BIG_RESULT; }
SQL_CALC_FOUND_ROWS	{ return SQL_CALC_FOUND_ROWS; }
SQL_SMALL_RESULT	{ return SQL_SMALL_RESULT; }
SSL	{ return SSL; }
STARTING	{ return STARTING; }
STRAIGHT_JOIN	{ return STRAIGHT_JOIN; }
TABLE	{ return TABLE; }
TABLES	{ return TABLES; }
TEMPORARY	{ return TEMPORARY; }
TERMINATED	{ return TERMINATED; }
TEXT	{ return TEXT; }
THEN	{ return THEN; }
TIME	{ return TIME; }
TIMESTAMP	{ return TIMESTAMP; }
INT1|TINYINT	{ return TINYINT; }
TINYTEXT	{ return TINYTEXT; }
TO	{ return TO; }
TRAILING	{ return TRAILING; }
TRIGGER	{ return TRIGGER; }
UNDO	{ return UNDO; }
UNION	{ return UNION; }
UNIQUE	{ return UNIQUE; }
UNLOCK	{ return UNLOCK; }
UNSIGNED	{ return UNSIGNED; }
UPDATE	{ return UPDATE; }
USAGE	{ return USAGE; }
USE	{ return USE; }
USING	{ return USING; }
UTC_DATE	{ return UTC_DATE; }
UTC_TIME	{ return UTC_TIME; }
UTC_TIMESTAMP	{ return UTC_TIMESTAMP; }
VALUES?	{ return VALUES; }
VARBINARY	{ return VARBINARY; }
VARCHAR(ACTER)?	{ return VARCHAR; }
VARYING	{ return VARYING; }
WHEN	{ return WHEN; }
WHERE	{ return WHERE; }
WEEK    { return WEEK;}//---5.26fzh---
WHILE	{ return WHILE; }
WITH	{ return WITH; }
WRITE	{ return WRITE; }
XOR	{ return XOR; }
YEAR	{ return YEAR; }
MONTH	{ return YEAR_MONTH; }//---5.26fzh---
ZEROFILL	{ return ZEROFILL; }
BEGIN		{ return BEGINT; }

   /* numbers */


[0-9]+	{ yylval.strval = strdup(yytext); return INTNUM; } 	// 2014-4-14---save int value in string type


[0-9]+"."[0-9]* |
"."[0-9]+	|
[0-9]+E[-+]?[0-9]+	|
[0-9]+"."[0-9]*E[-+]?[0-9]+ |

"."[0-9]*E[-+]?[0-9]+	{ yylval.strval = strdup(yytext); return APPROXNUM; }	// 2014-4-14---save double value in string type

    /* booleans */	// 2014-4-14---save bool value in string type
TRUE	{ yylval.strval = "1"; return BOOL; }
UNKNOWN	{ yylval.strval = "-1"; return BOOL; }	// 2014-4-14---not supported now---Yu
FALSE	{ yylval.strval = "0"; return BOOL; }


   /* strings */

'(\\.|''|[^'\n])*'	|
\"(\\.|\"\"|[^"\n])*\"  {
				char *temp = strdup(yytext); 
				//printf("temp:%s\n",temp);
				yylval.strval = strdup(yytext);
				
				//2014-4-3---处理转义字符以及消去首尾的引号---by Yu
				GetCorrectString(yylval.strval, temp);
				
				//printf("the string is %s\n",yylval.strval);
				return STRING;
			}

'(\\.|[^'\n])*$		{ yyerror("Unterminated string %s", yytext); }
\"(\\.|[^"\n])*$		{ yyerror("Unterminated string %s", yytext); }

   /* hex strings */
X'[0-9A-F]+' |  
0X[0-9A-F]+  	{ yylval.strval = strdup(yytext); return STRING; }

   /* bit strings */

0B[01]+      |
B'[01]+'     { yylval.strval = strdup(yytext); return STRING; }


   /* operators */
[-+&~|^/%*(),.;!]   { return yytext[0]; }

"&&"	{ return ANDOP; }
"||"	{ return OR; }

"<"	{ yylval.subtok = 1; return COMPARISON; }
">"	{ yylval.subtok = 2; return COMPARISON; }
"!="	|
"<>"	{ yylval.subtok = 3; return COMPARISON; }
"="	{ yylval.subtok = 4; return COMPARISON; }
"<="	{ yylval.subtok = 5; return COMPARISON; }
">="	{ yylval.subtok = 6; return COMPARISON; }
"<=>"	{ yylval.subtok = 12; return COMPARISON; }

"<<"	{ yylval.subtok = 1; return SHIFT; }
">>"	{ yylval.subtok = 2; return SHIFT; }

        /* functions */

SUBSTR(ING)?/"(" { return FSUBSTRING; }
TRIM/"(" { return FTRIM; }
DATE_ADD/"(" { return FDATE_ADD; }
DATE_SUB/"(" { return FDATE_SUB; }
UPPER/"(" { return FUPPER; }//---3.25fzh---
COALESCE/"(" { return FCOALESCE; }
CAST/"(" { return FCAST; }
CONVERT/"(" { return FCONVERT; }
	/* 
         * peek ahead and return function if name(
         */
	/* 
        ---2.18add aggrection functions
         */
COUNT    { int c = yyinput(); unput(c);
           if(c == '(') return FCOUNT;
           yylval.strval = strdup(yytext);
           return NAME; }
SUM    { int c = yyinput(); unput(c);
           if(c == '(') return FSUM;
           yylval.strval = strdup(yytext);
           return NAME; }           
AVG    { int c = yyinput(); unput(c);
           if(c == '(') return FAVG;
           yylval.strval = strdup(yytext);
           return NAME; }
MIN    { int c = yyinput(); unput(c);
           if(c == '(') return FMIN;
           yylval.strval = strdup(yytext);
           return NAME; }
MAX    { int c = yyinput(); unput(c);
           if(c == '(') return FMAX;
           yylval.strval = strdup(yytext);
           return NAME; } 
           
[A-Za-z][A-Za-z0-9_]*	{ yylval.strval = strdup(yytext);
                          return NAME; }
`[^`/\\.\n]+`           { yylval.strval = strdup(yytext+1);
                          yylval.strval[yyleng-2] = 0;
                          return NAME; }

`[^`\n]*$               { yyerror("unterminated quoted name %s", yytext); }

	/* user variables */
@[0-9a-z_.$]+ |
@\"[^"\n]+\" |
@`[^`\n]+` |
@'[^'\n]+' { yylval.strval = strdup(yytext+1); return USERVAR; }

@\"[^"\n]*$ { yyerror("unterminated quoted user variable %s", yytext); }
@`[^`\n]*$ { yyerror("unterminated quoted user variable %s", yytext); }
@'[^'\n]*$ { yyerror("unterminated quoted user variable %s", yytext); }


":="     { return ASSIGN; }

	/* comments */   
#.*		;
"--"[ \t].*	;

"/*"            { oldstate = YY_START; BEGIN COMMENT; }
<COMMENT>"*/"   { BEGIN oldstate; }
<COMMENT>.|\n   ;
<COMMENT><<EOF>> { yyerror("unclosed comment"); }

	/* everything else */
[ \t\n]         /* white space */
.               { yyerror("mystery character '%c'", *yytext); }

%%

