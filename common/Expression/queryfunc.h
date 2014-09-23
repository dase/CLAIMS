/*
 * queryfunc.h
 *
 *  Created on: Aug 6, 2014
 *      Author: imdb
 */

#ifndef QUERYFUNC_H_
#define QUERYFUNC_H_
#include"qnode.h"
#include "../../common/data_type.h"
#include "../../common/Logging.h"
#define OPER_TYPE_NUM 100
#define DATA_TYPE_NUM 30
#include<string.h>
#include <math.h>
#include "boost/date_time/gregorian/parsers.hpp"
#include <boost/date_time/gregorian/greg_duration.hpp>
#include "boost/date_time/gregorian/formatters.hpp"
#define DELTA 1e-8
class ExectorFunction
{
public:
	static ExecFunc operator_function[DATA_TYPE_NUM][OPER_TYPE_NUM];
};

#define NextByte(p, plen)	((p)++, (plen)--)
#define NextChar(p, plen) NextByte((p), (plen))
#define LIKE_FALSE 0
#define LIKE_TRUE 1
#define LIKE_ABORT -1
#define GETCHAR(t) (t)
/*
 * stolen from pg
 */
static int MatchText(char *t, int tlen, char *p, int plen)
{
	/* Fast path for match-everything pattern */
	if (plen == 1 && *p == '%')
		return LIKE_TRUE;

	/*
	 * In this loop, we advance by char when matching wildcards (and thus on
	 * recursive entry to this function we are properly char-synced). On other
	 * occasions it is safe to advance by byte, as the text and pattern will
	 * be in lockstep. This allows us to perform all comparisons between the
	 * text and pattern on a byte by byte basis, even for multi-byte
	 * encodings.
	 */
	while (tlen > 0 && plen > 0)
	{
		if (*p == '\\')
		{
//			/* Next pattern byte must match literally, whatever it is */
//			NextByte(p, plen);
//			/* ... and there had better be one, per SQL standard */
//			if (plen <= 0)
//				ereport(ERROR,
//						(errcode(ERRCODE_INVALID_ESCAPE_SEQUENCE),
//				 errmsg("LIKE pattern must not end with escape character")));
//			if (GETCHAR(*p) != GETCHAR(*t))
//				return LIKE_FALSE;
		}
		else if (*p == '%')
		{
			char		firstpat;

			/*
			 * % processing is essentially a search for a text position at
			 * which the remainder of the text matches the remainder of the
			 * pattern, using a recursive call to check each potential match.
			 *
			 * If there are wildcards immediately following the %, we can skip
			 * over them first, using the idea that any sequence of N _'s and
			 * one or more %'s is equivalent to N _'s and one % (ie, it will
			 * match any sequence of at least N text characters).  In this way
			 * we will always run the recursive search loop using a pattern
			 * fragment that begins with a literal character-to-match, thereby
			 * not recursing more than we have to.
			 */
			NextByte(p, plen);

			while (plen > 0)
			{
				if (*p == '%')
					NextByte(p, plen);
				else if (*p == '_')
				{
					/* If not enough text left to match the pattern, ABORT */
					if (tlen <= 0)
						return LIKE_ABORT;
					NextChar(t, tlen);
					NextByte(p, plen);
				}
				else
					break;		/* Reached a non-wildcard pattern char */
			}

			/*
			 * If we're at end of pattern, match: we have a trailing % which
			 * matches any remaining text string.
			 */
			if (plen <= 0)
				return LIKE_TRUE;

			/*
			 * Otherwise, scan for a text position at which we can match the
			 * rest of the pattern.  The first remaining pattern char is known
			 * to be a regular or escaped literal character, so we can compare
			 * the first pattern byte to each text byte to avoid recursing
			 * more than we have to.  This fact also guarantees that we don't
			 * have to consider a match to the zero-length substring at the
			 * end of the text.
			 */
			if (*p == '\\')
			{
//				if (plen < 2)
//					ereport(ERROR,
//							(errcode(ERRCODE_INVALID_ESCAPE_SEQUENCE),
//							 errmsg("LIKE pattern must not end with escape character")));
//				firstpat = GETCHAR(p[1]);
			}
			else
				firstpat = GETCHAR(*p);

			while (tlen > 0)
			{
				if (GETCHAR(*t) == firstpat)
				{
					int			matched = MatchText(t, tlen, p, plen);

					if (matched != LIKE_FALSE)
						return matched; /* TRUE or ABORT */
				}

				NextChar(t, tlen);
			}

			/*
			 * End of text with no match, so no point in trying later places
			 * to start matching this pattern.
			 */
			return LIKE_ABORT;
		}
		else if (*p == '_')
		{
			/* _ matches any single character, and we know there is one */
			NextChar(t, tlen);
			NextByte(p, plen);
			continue;
		}
		else if (GETCHAR(*p) != GETCHAR(*t))
		{
			/* non-wildcard pattern char fails to match text char */
			return LIKE_FALSE;
		}

		/*
		 * Pattern and text match, so advance.
		 *
		 * It is safe to use NextByte instead of NextChar here, even for
		 * multi-byte character sets, because we are not following immediately
		 * after a wildcard character. If we are in the middle of a multibyte
		 * character, we must already have matched at least one byte of the
		 * character from both text and pattern; so we cannot get out-of-sync
		 * on character boundaries.  And we know that no backend-legal
		 * encoding allows ASCII characters such as '%' to appear as non-first
		 * bytes of characters, so we won't mistakenly detect a new wildcard.
		 */
		NextByte(t, tlen);
		NextByte(p, plen);
	}

	if (tlen > 0)
		return LIKE_FALSE;		/* end of pattern, but not of text */

	/*
	 * End of text, but perhaps not of pattern.  Match iff the remaining
	 * pattern can match a zero-length string, ie, it's zero or more %'s.
	 */
	while (plen > 0 && *p == '%')
		NextByte(p, plen);
	if (plen <= 0)
		return LIKE_TRUE;

	/*
	 * End of text with no match, so no point in trying later places to start
	 * matching this pattern.
	 */
	return LIKE_ABORT;
}	/* MatchText() */

inline void oper_not_support(FuncCallInfo fcinfo)
{
	SQLParse_elog("This oper_functions is not supported now!!!!!!!!!!!!!");
	assert(false);
}
/*******************int*************************/
inline void int_add(FuncCallInfo fcinfo)
{
	assert(fcinfo->nargs==2);
	*(int *)fcinfo->results=(*(int *)fcinfo->args[0]+(*(int *)fcinfo->args[1]));
}
inline void int_minus(FuncCallInfo fcinfo)
{
	assert(fcinfo->nargs==2);
	*(int *)fcinfo->results=(*(int *)fcinfo->args[0]-(*(int *)fcinfo->args[1]));
}
inline void int_multiply(FuncCallInfo fcinfo)
{
	assert(fcinfo->nargs==2);
	*(int *)fcinfo->results=(*(int *)fcinfo->args[0]*(*(int *)fcinfo->args[1]));
}
inline void int_divide(FuncCallInfo fcinfo)
{
	assert(fcinfo->nargs==2);
	int val=*(int *)fcinfo->args[1];
	if(val==0)
	{
		SQLParse_elog("error:The int_divide divided =0!!!!!");
		assert(val!=0);
	}
	*(int *)fcinfo->results=(*(int *)fcinfo->args[0]/(*(int *)fcinfo->args[1]));
}
inline void int_mod(FuncCallInfo fcinfo)
{
	assert(fcinfo->nargs==2);
	*(int *)fcinfo->results=(*(int *)fcinfo->args[0]%(*(int *)fcinfo->args[1]));
}
inline void int_equal(FuncCallInfo fcinfo)
{
	assert(fcinfo->nargs==2);
	*(bool *)fcinfo->results=(*(int *)fcinfo->args[0])==(*(int *)fcinfo->args[1]);
}
inline void int_not_equal(FuncCallInfo fcinfo)
{
	assert(fcinfo->nargs==2);
	*(bool *)fcinfo->results=(*(int *)fcinfo->args[0])!=(*(int *)fcinfo->args[1]);
}
inline void int_great(FuncCallInfo fcinfo)
{
	assert(fcinfo->nargs==2);
	*(bool *)fcinfo->results=(*(int *)fcinfo->args[0])>(*(int *)fcinfo->args[1]);
}
inline void int_great_equal(FuncCallInfo fcinfo)
{
	assert(fcinfo->nargs==2);
	*(bool *)fcinfo->results=(*(int *)fcinfo->args[0])>=(*(int *)fcinfo->args[1]);
}
inline void int_less(FuncCallInfo fcinfo)
{
	assert(fcinfo->nargs==2);
	*(bool *)fcinfo->results=(*(int *)fcinfo->args[0])<(*(int *)fcinfo->args[1]);
}
inline void int_less_equal(FuncCallInfo fcinfo)
{
	assert(fcinfo->nargs==2);
	*(bool *)fcinfo->results=(*(int *)fcinfo->args[0])<=(*(int *)fcinfo->args[1]);
}
inline void int_negative(FuncCallInfo fcinfo)
{
	assert(fcinfo->nargs==1);
	*(int *)fcinfo->results=(*(int *)fcinfo->args[0]*(-1));
}
/*******************int*************************/


/*******************u_long*************************/

inline void u_long_add(FuncCallInfo fcinfo)
{
	assert(fcinfo->nargs==2);
	*(unsigned long *)fcinfo->results=((*(unsigned long *)fcinfo->args[0])+(*(unsigned long *)fcinfo->args[1]));
}
inline void u_long_minus(FuncCallInfo fcinfo)
{
	assert(fcinfo->nargs==2);
	*(unsigned long *)fcinfo->results=((*(unsigned long *)fcinfo->args[0])-(*(unsigned long *)fcinfo->args[1]));
}
inline void u_long_multiply(FuncCallInfo fcinfo)
{
	assert(fcinfo->nargs==2);
	*(unsigned long *)fcinfo->results=((*(unsigned long *)fcinfo->args[0])*(*(unsigned long *)fcinfo->args[1]));
}
inline void u_long_divide(FuncCallInfo fcinfo)
{
	assert(fcinfo->nargs==2);
	unsigned long val=*(unsigned long *)fcinfo->args[1];
	if(val==0)
	{
		SQLParse_elog("The u_long_divide divided = 0");
		assert(false);
	}
	*(unsigned long *)fcinfo->results=((*(unsigned long *)fcinfo->args[0])/(*(unsigned long *)fcinfo->args[1]));
}
inline void u_long_mod(FuncCallInfo fcinfo)
{
	assert(fcinfo->nargs==2);
	unsigned long val=*(unsigned long *)fcinfo->args[1];
	if(val==0)
	{
		SQLParse_elog("The u_long_mod moded = 0");
		assert(false);
	}
	*(unsigned long *)fcinfo->results=((*(unsigned long *)fcinfo->args[0])%(*(unsigned long *)fcinfo->args[1]));
}
inline void u_long_equal(FuncCallInfo fcinfo)
{
	assert(fcinfo->nargs==2);
	*(bool *)fcinfo->results=(*(unsigned long *)fcinfo->args[0]==*(unsigned long *)fcinfo->args[1]);
}
inline void u_long_not_equal(FuncCallInfo fcinfo)
{
	assert(fcinfo->nargs==2);
	*(bool *)fcinfo->results=(*(unsigned long *)fcinfo->args[0])!=(*(unsigned long *)fcinfo->args[1]);

}
inline void u_long_great(FuncCallInfo fcinfo)
{
	assert(fcinfo->nargs==2);
	*(bool *)fcinfo->results=(*(unsigned long *)fcinfo->args[0])>(*(unsigned long *)fcinfo->args[1]);

}
inline void u_long_great_equal(FuncCallInfo fcinfo)
{
	assert(fcinfo->nargs==2);
	*(bool *)fcinfo->results=(*(unsigned long *)fcinfo->args[0])>=(*(unsigned long *)fcinfo->args[1]);
}
inline void u_long_less(FuncCallInfo fcinfo)
{
	assert(fcinfo->nargs==2);
	*(bool *)fcinfo->results=(*(unsigned long *)fcinfo->args[0])<(*(unsigned long *)fcinfo->args[1]);
}
inline void u_long_less_equal(FuncCallInfo fcinfo)
{
	assert(fcinfo->nargs==2);
	*(bool *)fcinfo->results=(*(unsigned long *)fcinfo->args[0])<=(*(unsigned long *)fcinfo->args[1]);
}
/*******************u_long*************************/


/*******************float*************************/
inline void float_add(FuncCallInfo fcinfo)
{
	assert(fcinfo->nargs==2);
	*(float *)fcinfo->results=((*(float *)fcinfo->args[0])+(*(float *)fcinfo->args[1]));
}
inline void float_minus(FuncCallInfo fcinfo)
{
	assert(fcinfo->nargs==2);
	*(float *)fcinfo->results=((*(float *)fcinfo->args[0])-(*(float *)fcinfo->args[1]));
}
inline void float_multiply(FuncCallInfo fcinfo)
{
	assert(fcinfo->nargs==2);
	*(float *)fcinfo->results=((*(float *)fcinfo->args[0])*(*(float *)fcinfo->args[1]));
}
inline void float_divide(FuncCallInfo fcinfo)
{
	assert(fcinfo->nargs==2);
	float val=*(float *)fcinfo->args[1];
	if(val==0)
	{
		SQLParse_elog("The float_divide divided = 0");
		assert(false);
	}
	*(float *)fcinfo->results=((*(float *)fcinfo->args[0])/(*(float *)fcinfo->args[1]));
}

inline void float_equal(FuncCallInfo fcinfo)
{
	assert(fcinfo->nargs==2);
	*(bool *)fcinfo->results=fabs(double((*(float *)fcinfo->args[0])-(*(float *)fcinfo->args[1])))<DELTA;
}
inline void float_not_equal(FuncCallInfo fcinfo)
{
	assert(fcinfo->nargs==2);
	float_equal(fcinfo);
	*(bool *)fcinfo->results=!(*(bool *)fcinfo->results);
}
inline void float_great(FuncCallInfo fcinfo)
{
	assert(fcinfo->nargs==2);
	*(bool *)fcinfo->results=(*(float *)fcinfo->args[0])>(*(float *)fcinfo->args[1]);
}
inline void float_great_equal(FuncCallInfo fcinfo)
{
	assert(fcinfo->nargs==2);
	*(bool *)fcinfo->results=(*(float *)fcinfo->args[0])>=(*(float *)fcinfo->args[1]);
}
inline void float_less(FuncCallInfo fcinfo)
{
	assert(fcinfo->nargs==2);
	*(bool *)fcinfo->results=(*(float *)fcinfo->args[0])<(*(float *)fcinfo->args[1]);
}
inline void float_less_equal(FuncCallInfo fcinfo)
{
	assert(fcinfo->nargs==2);
	*(bool *)fcinfo->results=(*(float *)fcinfo->args[0])<=(*(float *)fcinfo->args[1]);
}
inline void float_negative(FuncCallInfo fcinfo)
{
	assert(fcinfo->nargs==1);
	*(float *)fcinfo->results=((*(float *)fcinfo->args[0])*(-1));
}
/*******************float*************************/


/*******************double*************************/

inline void double_add(FuncCallInfo fcinfo)
{
	assert(fcinfo->nargs==2);
	*(double *)fcinfo->results=((*(double *)fcinfo->args[0])+(*(double *)fcinfo->args[1]));
}
inline void double_minus(FuncCallInfo fcinfo)
{
	assert(fcinfo->nargs==2);
	*(double *)fcinfo->results=((*(double *)fcinfo->args[0])-(*(double *)fcinfo->args[1]));
}
inline void double_multiply(FuncCallInfo fcinfo)
{
	assert(fcinfo->nargs==2);
	*(double *)fcinfo->results=((*(double *)fcinfo->args[0])*(*(double *)fcinfo->args[1]));
}
inline void double_divide(FuncCallInfo fcinfo)
{
	assert(fcinfo->nargs==2);
	double val=*(double *)fcinfo->args[1];
	if(val==0)
	{
		SQLParse_elog("The double_divide divided = 0");
		assert(false);
	}
	*(double *)fcinfo->results=((*(double *)fcinfo->args[0])/(*(double *)fcinfo->args[1]));
}

inline void double_equal(FuncCallInfo fcinfo)
{
	assert(fcinfo->nargs==2);
	*(bool *)fcinfo->results=fabs(*(double *)fcinfo->args[0]-*(double *)fcinfo->args[1])<DELTA;
}
inline void double_not_equal(FuncCallInfo fcinfo)
{
	assert(fcinfo->nargs==2);
	double_equal(fcinfo);
	*(bool *)fcinfo->results=!(*(bool *)fcinfo->results);
}
inline void double_great(FuncCallInfo fcinfo)
{
	assert(fcinfo->nargs==2);
	*(bool *)fcinfo->results=(*(double *)fcinfo->args[0]>*(double *)fcinfo->args[1]);
}
inline void double_great_equal(FuncCallInfo fcinfo)
{
	assert(fcinfo->nargs==2);
	*(bool *)fcinfo->results=(*(double *)fcinfo->args[0]>=*(double *)fcinfo->args[1]);
}
inline void double_less(FuncCallInfo fcinfo)
{
	assert(fcinfo->nargs==2);
	bool ans=1;
	*(bool *)fcinfo->results=(*(double *)fcinfo->args[0]<*(double *)fcinfo->args[1]);
}
inline void double_less_equal(FuncCallInfo fcinfo)
{
	assert(fcinfo->nargs==2);
	*(bool *)fcinfo->results=(*(double *)fcinfo->args[0]<=*(double *)fcinfo->args[1]);
}
inline void double_negative(FuncCallInfo fcinfo)
{
	assert(fcinfo->nargs==1);
	*(double *)fcinfo->results=((*(double *)fcinfo->args[0])*(-1));
}
/*******************double*************************/

/*******************smallInt*************************/

inline void smallInt_add(FuncCallInfo fcinfo)
{
	assert(fcinfo->nargs==2);
	*(short int *)fcinfo->results=(*(short int *)fcinfo->args[0])+(*(short int *)fcinfo->args[1]);
}
inline void smallInt_minus(FuncCallInfo fcinfo)
{
	assert(fcinfo->nargs==2);
	*(short int *)fcinfo->results=(*(short int *)fcinfo->args[0])-(*(short int *)fcinfo->args[1]);
}
inline void smallInt_multiply(FuncCallInfo fcinfo)
{
	assert(fcinfo->nargs==2);
	*(short int *)fcinfo->results=(*(short int *)fcinfo->args[0])*(*(short int *)fcinfo->args[1]);
}
inline void smallInt_divide(FuncCallInfo fcinfo)
{
	assert(fcinfo->nargs==2);
	short int val=*(short int *)fcinfo->args[1];
	if(val==0)
	{
		SQLParse_elog("The smallInt_divide divided = 0");
		assert(false);
	}
	*(short int *)fcinfo->results=(*(short int *)fcinfo->args[0])/(*(short int *)fcinfo->args[1]);
}
inline void smallInt_mod(FuncCallInfo fcinfo)
{
	assert(fcinfo->nargs==2);
	short int val=*(short int *)fcinfo->args[1];
	if(val==0)
	{
		SQLParse_elog("The smallInt_mod moded = 0");
		assert(false);
	}
	*(short int *)fcinfo->results=(*(short int *)fcinfo->args[0]%(*(short int *)fcinfo->args[1]));
}
inline void smallInt_equal(FuncCallInfo fcinfo)
{
	assert(fcinfo->nargs==2);
	*(bool *)fcinfo->results=(*(short int *)fcinfo->args[0]==*(short int *)fcinfo->args[1]);
}
inline void smallInt_not_equal(FuncCallInfo fcinfo)
{
	assert(fcinfo->nargs==2);
	*(bool *)fcinfo->results=(*(short int *)fcinfo->args[0]!=*(short int *)fcinfo->args[1]);
}
inline void smallInt_great(FuncCallInfo fcinfo)
{
	assert(fcinfo->nargs==2);
	*(bool *)fcinfo->results=(*(short int *)fcinfo->args[0]>*(short int *)fcinfo->args[1]);
}
inline void smallInt_great_equal(FuncCallInfo fcinfo)
{
	assert(fcinfo->nargs==2);
	*(bool *)fcinfo->results=(*(short int *)fcinfo->args[0]>=*(short int *)fcinfo->args[1]);
}
inline void smallInt_less(FuncCallInfo fcinfo)
{
	assert(fcinfo->nargs==2);
	*(bool *)fcinfo->results=(*(short int *)fcinfo->args[0]<*(short int *)fcinfo->args[1]);
}
inline void smallInt_less_equal(FuncCallInfo fcinfo)
{
	assert(fcinfo->nargs==2);
	*(bool *)fcinfo->results=(*(short int *)fcinfo->args[0]<=*(short int *)fcinfo->args[1]);
}
inline void smallInt_negative(FuncCallInfo fcinfo)
{
	assert(fcinfo->nargs==1);
	*(short int  *)fcinfo->results=(*(short int *)fcinfo->args[0]*(-1));
}
/*******************smallInt*************************/

/*****************boolean********************/


inline void boolean_and(FuncCallInfo fcinfo)
{
	assert(fcinfo->nargs==2);
	*(bool *)fcinfo->results=((*(bool *)fcinfo->args[0])&&(*(bool *)fcinfo->args[1]));
}
inline void boolean_or(FuncCallInfo fcinfo)
{
	assert(fcinfo->nargs==2);
	*(bool *)fcinfo->results=((*(bool *)fcinfo->args[0])||(*(bool *)fcinfo->args[1]));
}
inline void boolean_xor(FuncCallInfo fcinfo)
{
	assert(fcinfo->nargs==2);
	*(bool *)fcinfo->results=((*(bool *)fcinfo->args[0])^(*(bool *)fcinfo->args[1]));
}
inline void boolean_not(FuncCallInfo fcinfo)
{
	assert(fcinfo->nargs==1);
	*(bool *)fcinfo->results=(!(*(bool *)fcinfo->args[0]));
}
inline void boolean_equal(FuncCallInfo fcinfo)
{
	assert(fcinfo->nargs==2);
	*(bool *)fcinfo->results=(*(bool *)fcinfo->args[0]==*(bool *)fcinfo->args[1]);
}
inline void boolean_not_equal(FuncCallInfo fcinfo)
{
	assert(fcinfo->nargs==2);
	*(bool *)fcinfo->results=(*(bool *)fcinfo->args[0]!=*(bool *)fcinfo->args[1]);
}
inline void boolean_great(FuncCallInfo fcinfo)
{
	assert(fcinfo->nargs==2);
	*(bool *)fcinfo->results=(*(bool *)fcinfo->args[0]>*(bool *)fcinfo->args[1]);
}
inline void boolean_great_equal(FuncCallInfo fcinfo)
{
	assert(fcinfo->nargs==2);
	*(bool *)fcinfo->results=(*(bool *)fcinfo->args[0]>=*(bool *)fcinfo->args[1]);
}
inline void boolean_less(FuncCallInfo fcinfo)
{
	assert(fcinfo->nargs==2);
	*(bool *)fcinfo->results=(*(bool *)fcinfo->args[0]<*(bool *)fcinfo->args[1]);
}
inline void boolean_less_equal(FuncCallInfo fcinfo)
{
	assert(fcinfo->nargs==2);
	*(bool *)fcinfo->results=(*(bool *)fcinfo->args[0]<=*(bool *)fcinfo->args[1]);
}
/*****************boolean********************/


/*****************decimal********************/
inline void decimal_add(FuncCallInfo fcinfo)
{
	assert(fcinfo->nargs==2);
	*(NValue *)fcinfo->results=(*(NValue *)fcinfo->args[0]).op_add(*(NValue *)fcinfo->args[1]);
}
inline void decimal_minus(FuncCallInfo fcinfo)
{
	assert(fcinfo->nargs==2);
	*(NValue *)fcinfo->results=(*(NValue *)fcinfo->args[0]).op_subtract(*(NValue *)fcinfo->args[1]);
}
inline void decimal_multiply(FuncCallInfo fcinfo)
{
	assert(fcinfo->nargs==2);
	*(NValue *)fcinfo->results=(*(NValue *)fcinfo->args[0]).op_multiply(*(NValue *)fcinfo->args[1]);
}
inline void decimal_divide(FuncCallInfo fcinfo)
{
	assert(fcinfo->nargs==2);
	*(NValue *)fcinfo->results=(*(NValue *)fcinfo->args[0]).op_divide(*(NValue *)fcinfo->args[1]);
}
inline void decimal_equal(FuncCallInfo fcinfo)
{
	assert(fcinfo->nargs==2);
	*(bool *)fcinfo->results=(*(NValue *)fcinfo->args[0]).op_equals(*(NValue *)fcinfo->args[1]);
}
inline void decimal_not_equal(FuncCallInfo fcinfo)
{
	assert(fcinfo->nargs==2);
	*(bool *)fcinfo->results=(*(NValue *)fcinfo->args[0]).op_not_equals(*(NValue *)fcinfo->args[1]);
}
inline void decimal_great(FuncCallInfo fcinfo)
{
	assert(fcinfo->nargs==2);
	*(bool *)fcinfo->results=(*(NValue *)fcinfo->args[0]).op_great(*(NValue *)fcinfo->args[1]);
}
inline void decimal_great_equal(FuncCallInfo fcinfo)
{
	assert(fcinfo->nargs==2);
	*(bool *)fcinfo->results=(*(NValue *)fcinfo->args[0]).op_great_equals(*(NValue *)fcinfo->args[1]);
}
inline void decimal_less(FuncCallInfo fcinfo)
{
	assert(fcinfo->nargs==2);
	*(bool *)fcinfo->results=(*(NValue *)fcinfo->args[0]).op_less(*(NValue *)fcinfo->args[1]);
}
inline void decimal_less_equal(FuncCallInfo fcinfo)
{
	assert(fcinfo->nargs==2);
	*(bool *)fcinfo->results=(*(NValue *)fcinfo->args[0]).op_less_equals(*(NValue *)fcinfo->args[1]);
}
inline void decimal_negative(FuncCallInfo fcinfo)
{
	assert(fcinfo->nargs==1);
	*(NValue *)fcinfo->results=(*(NValue *)fcinfo->args[0]).op_multiply(NValue::getDecimalValueFromString("-1"));
}
/*****************decimal********************/


/*****************string********************/
//the limit is the left should be column

inline void string_equal(FuncCallInfo fcinfo)
{
	assert(fcinfo->nargs==2);
	*(bool *)fcinfo->results=(strcmp((char *)fcinfo->args[0],(char *)fcinfo->args[1])==0);
}
inline void string_not_equal(FuncCallInfo fcinfo)
{
	assert(fcinfo->nargs==2);
	*(bool *)fcinfo->results=(strcmp((char *)fcinfo->args[0],(char *)fcinfo->args[1])!=0);
}
inline void string_great(FuncCallInfo fcinfo)
{
	assert(fcinfo->nargs==2);
	*(bool *)fcinfo->results=(strcmp((char *)fcinfo->args[0],(char *)fcinfo->args[1])>0);
}
inline void string_great_equal(FuncCallInfo fcinfo)
{
	assert(fcinfo->nargs==2);
	*(bool *)fcinfo->results=(strcmp((char *)fcinfo->args[0],(char *)fcinfo->args[1])>=0);
}
inline void string_less(FuncCallInfo fcinfo)
{
	assert(fcinfo->nargs==2);
	*(bool *)fcinfo->results=(strcmp((char *)fcinfo->args[0],(char *)fcinfo->args[1])<0);
}
inline void string_less_equal(FuncCallInfo fcinfo)
{
	assert(fcinfo->nargs==2);
	*(bool *)fcinfo->results=(strcmp((char *)fcinfo->args[0],(char *)fcinfo->args[1])<=0);
}
inline void string_both_trim(FuncCallInfo fcinfo)
{
	assert(fcinfo->nargs==2);
	int begin=0,end=strlen((char *)fcinfo->args[1])-1;
	for(;((char *)fcinfo->args[1])[begin]==((char *)fcinfo->args[0])[0]&&((char *)fcinfo->args[1])[begin]!='\0';begin++);
	if(end>begin)
	for(;((char *)fcinfo->args[1])[end]==((char *)fcinfo->args[0])[0]&&end>=0;end--);
	strncpy((char *)fcinfo->results,(char *)fcinfo->args[1]+begin,end-begin+1);
	((char*)fcinfo->results)[end-begin+1]='\0';
}
inline void string_leading_trim(FuncCallInfo fcinfo)
{
	assert(fcinfo->nargs==2);
	int begin=0,end=strlen((char *)fcinfo->args[1])-1;
	for(;((char *)fcinfo->args[1])[begin]==((char *)fcinfo->args[0])[0]&&((char *)fcinfo->args[1])[begin]!='\0';begin++);
	strncpy((char *)fcinfo->results,(char *)fcinfo->args[1]+begin,end-begin+1);
	((char*)fcinfo->results)[end-begin+1]='\0';
}
inline void string_trailing_trim(FuncCallInfo fcinfo)
{
	assert(fcinfo->nargs==2);
	int begin=0,end=strlen((char *)fcinfo->args[1])-1;
	for(;((char *)fcinfo->args[1])[end]==((char *)fcinfo->args[0])[0]&&end>=0;end--);
	strncpy((char *)fcinfo->results,(char *)fcinfo->args[1]+begin,end-begin+1);
	((char*)fcinfo->results)[end-begin+1]='\0';
}
inline void string_like(FuncCallInfo fcinfo)
{
	assert(fcinfo->nargs==2);
    *(bool *)fcinfo->results=(MatchText((char *)fcinfo->args[0],strlen((char *)fcinfo->args[0]),(char *)fcinfo->args[1],strlen((char *)fcinfo->args[1]))==1);
}
inline void string_not_like(FuncCallInfo fcinfo)
{
	assert(fcinfo->nargs==2);
    *(bool *)fcinfo->results=(MatchText((char *)fcinfo->args[0],strlen((char *)fcinfo->args[0]),(char *)fcinfo->args[1],strlen((char *)fcinfo->args[1]))!=1);
}
inline void string_upper(FuncCallInfo fcinfo)
{
	assert(fcinfo->nargs==1);
	strcpy((char *)fcinfo->results,(char *)fcinfo->args[0]);
	int len=strlen((char *)fcinfo->args[0]);
	for(int i=0;i<len;i++)
	{
		if(((char *)fcinfo->results)[i]>='a'&&((char *)fcinfo->results)[i]<='z')
		((char *)fcinfo->results)[i]-=32;
	}
	((char *)fcinfo->results)[len]='\0';
}
inline void string_substring(FuncCallInfo fcinfo)
{
	assert(fcinfo->nargs==3);
	strncpy((char *)fcinfo->results,((char *)fcinfo->args[0])+(*(int *)fcinfo->args[1]),(*(int *)fcinfo->args[2])-(*(int *)fcinfo->args[1]));
}


/*****************string********************/

/*****************date********************/
inline void date_equal(FuncCallInfo fcinfo)
{
	assert(fcinfo->nargs==2);
	*(bool *)fcinfo->results=(*(date *)fcinfo->args[0]==*(date *)fcinfo->args[1]);
}
inline void date_not_equal(FuncCallInfo fcinfo)
{
	assert(fcinfo->nargs==2);
	*(bool *)fcinfo->results=(*(date *)fcinfo->args[0]!=*(date *)fcinfo->args[1]);
}
inline void date_great(FuncCallInfo fcinfo)
{
	assert(fcinfo->nargs==2);
	*(bool *)fcinfo->results=(*(date *)fcinfo->args[0]>*(date *)fcinfo->args[1]);
}
inline void date_great_equal(FuncCallInfo fcinfo)
{
	assert(fcinfo->nargs==2);
	*(bool *)fcinfo->results=(*(date *)fcinfo->args[0]>=*(date *)fcinfo->args[1]);
}
inline void date_less(FuncCallInfo fcinfo)
{
	assert(fcinfo->nargs==2);
	*(bool *)fcinfo->results=(*(date *)fcinfo->args[0]<*(date *)fcinfo->args[1]);
}
inline void date_less_equal(FuncCallInfo fcinfo)
{
	assert(fcinfo->nargs==2);
	*(bool *)fcinfo->results=(*(date *)fcinfo->args[0]<=*(date *)fcinfo->args[1]);
}

inline void date_add_day(FuncCallInfo fcinfo)
{
	assert(fcinfo->nargs==2);
	*(date *)fcinfo->results=(*(date *)fcinfo->args[0])+(*(date_duration *)fcinfo->args[1]);
}
inline void date_add_week(FuncCallInfo fcinfo)
{
	assert(fcinfo->nargs==2);
	*(date *)fcinfo->results=(*(date *)fcinfo->args[0])+(*(weeks *)fcinfo->args[1]);
}

inline void date_add_month(FuncCallInfo fcinfo)
{
	assert(fcinfo->nargs==2);
	*(date *)fcinfo->results=(*(date *)fcinfo->args[0])+(*(months *)fcinfo->args[1]);
}

inline void date_add_year(FuncCallInfo fcinfo)
{
	assert(fcinfo->nargs==2);
	*(date *)fcinfo->results=(*(date *)fcinfo->args[0])+(*(years *)fcinfo->args[1]);
}

inline void date_sub_day(FuncCallInfo fcinfo)
{
	assert(fcinfo->nargs==2);
	*(date *)fcinfo->results=(*(date *)fcinfo->args[0])-(*(date_duration *)fcinfo->args[1]);
}
inline void date_sub_week(FuncCallInfo fcinfo)
{
	assert(fcinfo->nargs==2);
	*(date *)fcinfo->results=(*(date *)fcinfo->args[0])-(*(weeks *)fcinfo->args[1]);
}
inline void date_sub_month(FuncCallInfo fcinfo)
{
	assert(fcinfo->nargs==2);
	*(date *)fcinfo->results=(*(date *)fcinfo->args[0])-(*(months *)fcinfo->args[1]);
}
inline void date_sub_year(FuncCallInfo fcinfo)
{
	assert(fcinfo->nargs==2);
	*(date *)fcinfo->results=(*(date *)fcinfo->args[0])-(*(years *)fcinfo->args[1]);
}

/*****************date********************/

/*****************time********************/
inline void time_equal(FuncCallInfo fcinfo)
{
	assert(fcinfo->nargs==2);
	*(bool *)fcinfo->results=(*(time_duration *)fcinfo->args[0]==*(time_duration *)fcinfo->args[1]);
}
inline void time_not_equal(FuncCallInfo fcinfo)
{
	assert(fcinfo->nargs==2);
	*(bool *)fcinfo->results=(*(time_duration *)fcinfo->args[0]!=*(time_duration *)fcinfo->args[1]);
}
inline void time_great(FuncCallInfo fcinfo)
{
	assert(fcinfo->nargs==2);
	*(bool *)fcinfo->results=(*(time_duration *)fcinfo->args[0]>*(time_duration *)fcinfo->args[1]);
}
inline void time_great_equal(FuncCallInfo fcinfo)
{
	assert(fcinfo->nargs==2);
	*(bool *)fcinfo->results=(*(time_duration *)fcinfo->args[0]>=*(time_duration *)fcinfo->args[1]);
}
inline void time_less(FuncCallInfo fcinfo)
{
	assert(fcinfo->nargs==2);
	*(bool *)fcinfo->results=(*(time_duration *)fcinfo->args[0]<*(time_duration *)fcinfo->args[1]);
}
inline void time_less_equal(FuncCallInfo fcinfo)
{
	assert(fcinfo->nargs==2);
	*(bool *)fcinfo->results=(*(time_duration *)fcinfo->args[0]<=*(time_duration *)fcinfo->args[1]);
}
/*****************time********************/

/*****************decimal********************/
/*****************decimal********************/
inline void initialize_operator_function()
{
	for(int i=0;i<DATA_TYPE_NUM;i++)
		for(int j=0;j<OPER_TYPE_NUM;j++)
			ExectorFunction::operator_function[i][j]=oper_not_support;
	/*****************int********************/
	ExectorFunction::operator_function[t_int][oper_add]=int_add;
	ExectorFunction::operator_function[t_int][oper_minus]=int_minus;
	ExectorFunction::operator_function[t_int][oper_multiply]=int_multiply;
	ExectorFunction::operator_function[t_int][oper_divide]=int_divide;
	ExectorFunction::operator_function[t_int][oper_mod]=int_mod;
	ExectorFunction::operator_function[t_int][oper_and]=oper_not_support;
	ExectorFunction::operator_function[t_int][oper_or]=oper_not_support;
	ExectorFunction::operator_function[t_int][oper_xor]=oper_not_support;
	ExectorFunction::operator_function[t_int][oper_not]=oper_not_support;
	ExectorFunction::operator_function[t_int][oper_equal]=int_equal;
	ExectorFunction::operator_function[t_int][oper_not_equal]=int_not_equal;
	ExectorFunction::operator_function[t_int][oper_great]=int_great;
	ExectorFunction::operator_function[t_int][oper_great_equal]=int_great_equal;
	ExectorFunction::operator_function[t_int][oper_less]=int_less;
	ExectorFunction::operator_function[t_int][oper_less_equal]=int_less_equal;
	ExectorFunction::operator_function[t_int][oper_negative]=int_negative;

	/*****************int********************/

	/*****************ulong********************/
	ExectorFunction::operator_function[t_u_long][oper_add]=u_long_add;
	ExectorFunction::operator_function[t_u_long][oper_minus]=u_long_minus;
	ExectorFunction::operator_function[t_u_long][oper_multiply]=u_long_multiply;
	ExectorFunction::operator_function[t_u_long][oper_divide]=u_long_divide;
	ExectorFunction::operator_function[t_u_long][oper_mod]=u_long_mod;
	ExectorFunction::operator_function[t_u_long][oper_and]=oper_not_support;
	ExectorFunction::operator_function[t_u_long][oper_or]=oper_not_support;
	ExectorFunction::operator_function[t_u_long][oper_xor]=oper_not_support;
	ExectorFunction::operator_function[t_u_long][oper_not]=oper_not_support;
	ExectorFunction::operator_function[t_u_long][oper_equal]=u_long_equal;
	ExectorFunction::operator_function[t_u_long][oper_not_equal]=u_long_not_equal;
	ExectorFunction::operator_function[t_u_long][oper_great]=u_long_great;
	ExectorFunction::operator_function[t_u_long][oper_great_equal]=u_long_great_equal;
	ExectorFunction::operator_function[t_u_long][oper_less]=u_long_less;
	ExectorFunction::operator_function[t_u_long][oper_less_equal]=u_long_less_equal;
	ExectorFunction::operator_function[t_u_long][oper_negative]=oper_not_support;

	/*****************ulong********************/

	/*****************float********************/
	ExectorFunction::operator_function[t_float][oper_add]=float_add;
	ExectorFunction::operator_function[t_float][oper_minus]=float_minus;
	ExectorFunction::operator_function[t_float][oper_multiply]=float_multiply;
	ExectorFunction::operator_function[t_float][oper_divide]=float_divide;
	ExectorFunction::operator_function[t_float][oper_mod]=oper_not_support;
	ExectorFunction::operator_function[t_float][oper_and]=oper_not_support;
	ExectorFunction::operator_function[t_float][oper_or]=oper_not_support;
	ExectorFunction::operator_function[t_float][oper_xor]=oper_not_support;
	ExectorFunction::operator_function[t_float][oper_not]=oper_not_support;
	ExectorFunction::operator_function[t_float][oper_equal]=float_equal;
	ExectorFunction::operator_function[t_float][oper_not_equal]=float_not_equal;
	ExectorFunction::operator_function[t_float][oper_great]=float_great;
	ExectorFunction::operator_function[t_float][oper_great_equal]=float_great_equal;
	ExectorFunction::operator_function[t_float][oper_less]=float_less;
	ExectorFunction::operator_function[t_float][oper_less_equal]=float_less_equal;
	ExectorFunction::operator_function[t_float][oper_negative]=float_negative;

	/*****************float********************/

	/*****************double********************/

	ExectorFunction::operator_function[t_double][oper_add]=double_add;
	ExectorFunction::operator_function[t_double][oper_minus]=double_minus;
	ExectorFunction::operator_function[t_double][oper_multiply]=double_multiply;
	ExectorFunction::operator_function[t_double][oper_divide]=double_divide;
	ExectorFunction::operator_function[t_double][oper_mod]=oper_not_support;
	ExectorFunction::operator_function[t_double][oper_and]=oper_not_support;
	ExectorFunction::operator_function[t_double][oper_or]=oper_not_support;
	ExectorFunction::operator_function[t_double][oper_xor]=oper_not_support;
	ExectorFunction::operator_function[t_double][oper_not]=oper_not_support;
	ExectorFunction::operator_function[t_double][oper_equal]=double_equal;
	ExectorFunction::operator_function[t_double][oper_not_equal]=double_not_equal;
	ExectorFunction::operator_function[t_double][oper_great]=double_great;
	ExectorFunction::operator_function[t_double][oper_great_equal]=double_great_equal;
	ExectorFunction::operator_function[t_double][oper_less]=double_less;
	ExectorFunction::operator_function[t_double][oper_less_equal]=double_less_equal;
	ExectorFunction::operator_function[t_double][oper_negative]=double_negative;

	/*****************double********************/

	/*****************smallInt********************/
	ExectorFunction::operator_function[t_smallInt][oper_add]=smallInt_add;
	ExectorFunction::operator_function[t_smallInt][oper_minus]=smallInt_minus;
	ExectorFunction::operator_function[t_smallInt][oper_multiply]=smallInt_multiply;
	ExectorFunction::operator_function[t_smallInt][oper_divide]=smallInt_divide;
	ExectorFunction::operator_function[t_smallInt][oper_mod]=smallInt_mod;
	ExectorFunction::operator_function[t_smallInt][oper_and]=oper_not_support;
	ExectorFunction::operator_function[t_smallInt][oper_or]=oper_not_support;
	ExectorFunction::operator_function[t_smallInt][oper_xor]=oper_not_support;
	ExectorFunction::operator_function[t_smallInt][oper_not]=oper_not_support;
	ExectorFunction::operator_function[t_smallInt][oper_equal]=smallInt_equal;
	ExectorFunction::operator_function[t_smallInt][oper_not_equal]=smallInt_not_equal;
	ExectorFunction::operator_function[t_smallInt][oper_great]=smallInt_great;
	ExectorFunction::operator_function[t_smallInt][oper_great_equal]=smallInt_great_equal;
	ExectorFunction::operator_function[t_smallInt][oper_less]=smallInt_less;
	ExectorFunction::operator_function[t_smallInt][oper_less_equal]=smallInt_less_equal;
	ExectorFunction::operator_function[t_smallInt][oper_negative]=smallInt_negative;

	/*****************smallInt********************/

	/*****************boolean********************/
	ExectorFunction::operator_function[t_boolean][oper_add]=oper_not_support;
	ExectorFunction::operator_function[t_boolean][oper_minus]=oper_not_support;
	ExectorFunction::operator_function[t_boolean][oper_multiply]=oper_not_support;
	ExectorFunction::operator_function[t_boolean][oper_divide]=oper_not_support;
	ExectorFunction::operator_function[t_boolean][oper_mod]=oper_not_support;
	ExectorFunction::operator_function[t_boolean][oper_and]=boolean_and;
	ExectorFunction::operator_function[t_boolean][oper_or]=boolean_or;
	ExectorFunction::operator_function[t_boolean][oper_xor]=boolean_xor;
	ExectorFunction::operator_function[t_boolean][oper_not]=boolean_not;
	ExectorFunction::operator_function[t_boolean][oper_equal]=boolean_equal;
	ExectorFunction::operator_function[t_boolean][oper_not_equal]=boolean_not_equal;
	ExectorFunction::operator_function[t_boolean][oper_great]=boolean_great;
	ExectorFunction::operator_function[t_boolean][oper_great_equal]=boolean_great_equal;
	ExectorFunction::operator_function[t_boolean][oper_less]=boolean_less;
	ExectorFunction::operator_function[t_boolean][oper_less_equal]=boolean_less_equal;
	ExectorFunction::operator_function[t_boolean][oper_negative]=oper_not_support;

	/*****************boolean********************/


	/*****************decimal********************/

	ExectorFunction::operator_function[t_decimal][oper_add]=decimal_add;
	ExectorFunction::operator_function[t_decimal][oper_minus]=decimal_minus;
	ExectorFunction::operator_function[t_decimal][oper_multiply]=decimal_multiply;
	ExectorFunction::operator_function[t_decimal][oper_divide]=decimal_divide;
	ExectorFunction::operator_function[t_decimal][oper_mod]=oper_not_support;
	ExectorFunction::operator_function[t_decimal][oper_and]=oper_not_support;
	ExectorFunction::operator_function[t_decimal][oper_or]=oper_not_support;
	ExectorFunction::operator_function[t_decimal][oper_xor]=oper_not_support;
	ExectorFunction::operator_function[t_decimal][oper_not]=oper_not_support;
	ExectorFunction::operator_function[t_decimal][oper_equal]=decimal_equal;
	ExectorFunction::operator_function[t_decimal][oper_not_equal]=decimal_not_equal;
	ExectorFunction::operator_function[t_decimal][oper_great]=decimal_great;
	ExectorFunction::operator_function[t_decimal][oper_great_equal]=decimal_great_equal;
	ExectorFunction::operator_function[t_decimal][oper_less]=decimal_less;
	ExectorFunction::operator_function[t_decimal][oper_less_equal]=decimal_less_equal;
	ExectorFunction::operator_function[t_decimal][oper_negative]=decimal_negative;

	/*****************decimal********************/


	/*****************string********************/
	ExectorFunction::operator_function[t_string][oper_add]=oper_not_support;
	ExectorFunction::operator_function[t_string][oper_minus]=oper_not_support;
	ExectorFunction::operator_function[t_string][oper_multiply]=oper_not_support;
	ExectorFunction::operator_function[t_string][oper_divide]=oper_not_support;
	ExectorFunction::operator_function[t_string][oper_mod]=oper_not_support;
	ExectorFunction::operator_function[t_string][oper_and]=oper_not_support;
	ExectorFunction::operator_function[t_string][oper_or]=oper_not_support;
	ExectorFunction::operator_function[t_string][oper_xor]=oper_not_support;
	ExectorFunction::operator_function[t_string][oper_not]=oper_not_support;
	ExectorFunction::operator_function[t_string][oper_equal]=string_equal;
	ExectorFunction::operator_function[t_string][oper_not_equal]=string_not_equal;
	ExectorFunction::operator_function[t_string][oper_great]=string_great;
	ExectorFunction::operator_function[t_string][oper_great_equal]=string_great_equal;
	ExectorFunction::operator_function[t_string][oper_less]=string_less;
	ExectorFunction::operator_function[t_string][oper_less_equal]=string_less_equal;

	ExectorFunction::operator_function[t_string][oper_both_trim]=string_both_trim;
	ExectorFunction::operator_function[t_string][oper_leading_trim]=string_leading_trim;
	ExectorFunction::operator_function[t_string][oper_trailing_trim]=string_trailing_trim;
	ExectorFunction::operator_function[t_string][oper_like]=string_like;
	ExectorFunction::operator_function[t_string][oper_not_like]=string_not_like;
	ExectorFunction::operator_function[t_string][oper_upper]=string_upper;
	ExectorFunction::operator_function[t_string][oper_substring]=string_substring;

	/*****************string********************/

	/*****************date********************/
	ExectorFunction::operator_function[t_date][oper_add]=oper_not_support;
	ExectorFunction::operator_function[t_date][oper_minus]=oper_not_support;
	ExectorFunction::operator_function[t_date][oper_multiply]=oper_not_support;
	ExectorFunction::operator_function[t_date][oper_divide]=oper_not_support;
	ExectorFunction::operator_function[t_date][oper_mod]=oper_not_support;
	ExectorFunction::operator_function[t_date][oper_and]=oper_not_support;
	ExectorFunction::operator_function[t_date][oper_or]=oper_not_support;
	ExectorFunction::operator_function[t_date][oper_xor]=oper_not_support;
	ExectorFunction::operator_function[t_date][oper_not]=oper_not_support;
	ExectorFunction::operator_function[t_date][oper_equal]=date_equal;
	ExectorFunction::operator_function[t_date][oper_not_equal]=date_not_equal;
	ExectorFunction::operator_function[t_date][oper_great]=date_great;
	ExectorFunction::operator_function[t_date][oper_great_equal]=date_great_equal;
	ExectorFunction::operator_function[t_date][oper_less]=date_less;
	ExectorFunction::operator_function[t_date][oper_less_equal]=date_less_equal;
	ExectorFunction::operator_function[t_date][oper_negative]=oper_not_support;


	ExectorFunction::operator_function[t_date][oper_date_add_day]=date_add_day;
	ExectorFunction::operator_function[t_date][oper_date_add_week]=date_add_week;
	ExectorFunction::operator_function[t_date][oper_date_add_month]=date_add_month;
	ExectorFunction::operator_function[t_date][oper_date_add_year]=date_add_year;
	ExectorFunction::operator_function[t_date][oper_date_sub_day]=date_sub_day;
	ExectorFunction::operator_function[t_date][oper_date_sub_week]=date_sub_week;
	ExectorFunction::operator_function[t_date][oper_date_sub_month]=date_sub_month;
	ExectorFunction::operator_function[t_date][oper_date_sub_year]=date_sub_year;


	/*****************date********************/

	/*****************time********************/
	ExectorFunction::operator_function[t_time][oper_equal]=time_equal;
	ExectorFunction::operator_function[t_time][oper_not_equal]=time_not_equal;
	ExectorFunction::operator_function[t_time][oper_great]=time_great;
	ExectorFunction::operator_function[t_time][oper_great_equal]=time_great_equal;
	ExectorFunction::operator_function[t_time][oper_less]=time_less;
	ExectorFunction::operator_function[t_time][oper_less_equal]=time_less_equal;

	/*****************time********************/

	/*****************double********************/
	/*****************double********************/



}
#endif /* QUERYFUNC_H_ */
