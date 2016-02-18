/*
 * data_type_oper.cpp
 *  Created on: May 29, 2015 11:28:14 AM
 *      Author: fzh
 *       Email: fzhedu@gmail.com
 *   Copyright: Copyright (c) @ ECNU.DaSE
 * Description:
 */

#ifndef COMMON_EXPRESSION_DATA_TYPE_OPER_H_
#define COMMON_EXPRESSION_DATA_TYPE_OPER_H_
#include <string.h>
#include <math.h>
#include <iosfwd>
#include "./expr_node.h"
#include "../../common/data_type.h"
#include "../../common/Logging.h"
#include "../../common/types/NValue.hpp"
#include "../../common/types/decimal.h"
#include "boost/date_time/gregorian/parsers.hpp"
#include <boost/date_time/gregorian/greg_duration.hpp>
#include "boost/date_time/gregorian/formatters.hpp"
namespace claims {
namespace common {
#define OPER_TYPE_NUM 100
#define DATA_TYPE_NUM 30
#define DELTA 1e-8
class DataTypeOper {
 public:
  static DataTypeOperFunc data_type_oper_func_[DATA_TYPE_NUM][OPER_TYPE_NUM];
  static AvgDivide avg_divide_[DATA_TYPE_NUM];
};

#define NextByte(p, plen) ((p)++, (plen)--)
#define NextChar(p, plen) NextByte((p), (plen))
#define LIKE_FALSE 0
#define LIKE_TRUE 1
#define LIKE_ABORT -1
#define GETCHAR(t) (t)
/*
 * stolen from pg
 */
static int MatchText(char *t, int tlen, char *p, int plen) {
  /* Fast path for match-everything pattern */
  if (plen == 1 && *p == '%') return LIKE_TRUE;

  /*
   * In this loop, we advance by char when matching wildcards (and thus on
   * recursive entry to this function we are properly char-synced). On other
   * occasions it is safe to advance by byte, as the text and pattern will
   * be in lockstep. This allows us to perform all comparisons between the
   * text and pattern on a byte by byte basis, even for multi-byte
   * encodings.
   */
  while (tlen > 0 && plen > 0) {
    if (*p == '\\') {
      //			/* Next pattern byte must match literally,
      // whatever it is */
      //			NextByte(p, plen);
      //			/* ... and there had better be one, per SQL
      // standard */
      //			if (plen <= 0)
      //				ereport(ERROR,
      //						(errcode(ERRCODE_INVALID_ESCAPE_SEQUENCE),
      //				 errmsg("LIKE pattern must not end with
      // escape character")));
      //			if (GETCHAR(*p) != GETCHAR(*t))
      //				return LIKE_FALSE;
    } else if (*p == '%') {
      char firstpat;

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

      while (plen > 0) {
        if (*p == '%')
          NextByte(p, plen);
        else if (*p == '_') {
          /* If not enough text left to match the pattern, ABORT */
          if (tlen <= 0) return LIKE_ABORT;
          NextChar(t, tlen);
          NextByte(p, plen);
        } else
          break; /* Reached a non-wildcard pattern char */
      }

      /*
       * If we're at end of pattern, match: we have a trailing % which
       * matches any remaining text string.
       */
      if (plen <= 0) return LIKE_TRUE;

      /*
       * Otherwise, scan for a text position at which we can match the
       * rest of the pattern.  The first remaining pattern char is known
       * to be a regular or escaped literal character, so we can compare
       * the first pattern byte to each text byte to avoid recursing
       * more than we have to.  This fact also guarantees that we don't
       * have to consider a match to the zero-length substring at the
       * end of the text.
       */
      if (*p == '\\') {
        //				if (plen < 2)
        //					ereport(ERROR,
        //							(errcode(ERRCODE_INVALID_ESCAPE_SEQUENCE),
        //							 errmsg("LIKE
        // pattern
        // must
        // not end with escape character")));
        //				firstpat = GETCHAR(p[1]);
      } else
        firstpat = GETCHAR(*p);

      while (tlen > 0) {
        if (GETCHAR(*t) == firstpat) {
          int matched = MatchText(t, tlen, p, plen);

          if (matched != LIKE_FALSE) return matched; /* TRUE or ABORT */
        }

        NextChar(t, tlen);
      }

      /*
       * End of text with no match, so no point in trying later places
       * to start matching this pattern.
       */
      return LIKE_ABORT;
    } else if (*p == '_') {
      /* _ matches any single character, and we know there is one */
      NextChar(t, tlen);
      NextByte(p, plen);
      continue;
    } else if (GETCHAR(*p) != GETCHAR(*t)) {
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

  if (tlen > 0) return LIKE_FALSE; /* end of pattern, but not of text */

  /*
   * End of text, but perhaps not of pattern.  Match iff the remaining
   * pattern can match a zero-length string, ie, it's zero or more %'s.
   */
  while (plen > 0 && *p == '%') NextByte(p, plen);
  if (plen <= 0) return LIKE_TRUE;

  /*
   * End of text with no match, so no point in trying later places to start
   * matching this pattern.
   */
  return LIKE_ABORT;
} /* MatchText() */

inline void oper_not_support(OperFuncInfo fcinfo) {
  SQLParse_elog("This oper_functions is not supported now!!!!!!!!!!!!!");
  assert(false);
}
/*******************int*************************/
inline void int_add(OperFuncInfo fcinfo) {
  assert(fcinfo->args_num_ == 2);
  *(int *)fcinfo->result_ =
      (*(int *)fcinfo->args_[0] + (*(int *)fcinfo->args_[1]));
}
inline void int_minus(OperFuncInfo fcinfo) {
  assert(fcinfo->args_num_ == 2);
  *(int *)fcinfo->result_ =
      (*(int *)fcinfo->args_[0] - (*(int *)fcinfo->args_[1]));
}
inline void int_multiply(OperFuncInfo fcinfo) {
  assert(fcinfo->args_num_ == 2);
  *(int *)fcinfo->result_ =
      (*(int *)fcinfo->args_[0] * (*(int *)fcinfo->args_[1]));
}
inline void int_divide(OperFuncInfo fcinfo) {
  assert(fcinfo->args_num_ == 2);
  int val = *(int *)fcinfo->args_[1];
  if (val == 0) {
    SQLParse_elog("error:The int_divide divided =0!!!!!");
    assert(val != 0);
  }
  *(int *)fcinfo->result_ =
      (*(int *)fcinfo->args_[0] / (*(int *)fcinfo->args_[1]));
}
inline void int_mod(OperFuncInfo fcinfo) {
  assert(fcinfo->args_num_ == 2);
  *(int *)fcinfo->result_ =
      (*(int *)fcinfo->args_[0] % (*(int *)fcinfo->args_[1]));
}
inline void int_equal(OperFuncInfo fcinfo) {
  assert(fcinfo->args_num_ == 2);
  *(bool *)fcinfo->result_ =
      (*(int *)fcinfo->args_[0]) == (*(int *)fcinfo->args_[1]);
}
inline void int_not_equal(OperFuncInfo fcinfo) {
  assert(fcinfo->args_num_ == 2);
  *(bool *)fcinfo->result_ =
      (*(int *)fcinfo->args_[0]) != (*(int *)fcinfo->args_[1]);
}
inline void int_great(OperFuncInfo fcinfo) {
  assert(fcinfo->args_num_ == 2);
  *(bool *)fcinfo->result_ =
      (*(int *)fcinfo->args_[0]) > (*(int *)fcinfo->args_[1]);
}
inline void int_great_equal(OperFuncInfo fcinfo) {
  assert(fcinfo->args_num_ == 2);
  *(bool *)fcinfo->result_ =
      (*(int *)fcinfo->args_[0]) >= (*(int *)fcinfo->args_[1]);
}
inline void int_less(OperFuncInfo fcinfo) {
  assert(fcinfo->args_num_ == 2);
  *(bool *)fcinfo->result_ =
      (*(int *)fcinfo->args_[0]) < (*(int *)fcinfo->args_[1]);
}
inline void int_less_equal(OperFuncInfo fcinfo) {
  assert(fcinfo->args_num_ == 2);
  *(bool *)fcinfo->result_ =
      (*(int *)fcinfo->args_[0]) <= (*(int *)fcinfo->args_[1]);
}
inline void int_negative(OperFuncInfo fcinfo) {
  assert(fcinfo->args_num_ == 1);
  *(int *)fcinfo->result_ = (*(int *)fcinfo->args_[0] * (-1));
}
inline void int_agg_min(OperFuncInfo fcinfo) {
  assert(fcinfo->args_num_ == 2);
  *(int *)fcinfo->result_ =
      ((*(int *)fcinfo->args_[0]) < (*(int *)fcinfo->args_[1])
           ? (*(int *)fcinfo->args_[0])
           : (*(int *)fcinfo->args_[1]));
}
inline void int_agg_max(OperFuncInfo fcinfo) {
  assert(fcinfo->args_num_ == 2);
  *(int *)fcinfo->result_ =
      ((*(int *)fcinfo->args_[0]) > (*(int *)fcinfo->args_[1])
           ? (*(int *)fcinfo->args_[0])
           : (*(int *)fcinfo->args_[1]));
}
inline void int_agg_sum(OperFuncInfo fcinfo) { int_add(fcinfo); }
inline void int_agg_count(OperFuncInfo fcinfo) { int_add(fcinfo); }

/*******************int*************************/

/*******************u_long*************************/

inline void u_long_add(OperFuncInfo fcinfo) {
  assert(fcinfo->args_num_ == 2);
  *(unsigned long *)fcinfo->result_ = ((*(unsigned long *)fcinfo->args_[0]) +
                                       (*(unsigned long *)fcinfo->args_[1]));
}
inline void u_long_minus(OperFuncInfo fcinfo) {
  assert(fcinfo->args_num_ == 2);
  *(unsigned long *)fcinfo->result_ = ((*(unsigned long *)fcinfo->args_[0]) -
                                       (*(unsigned long *)fcinfo->args_[1]));
}
inline void u_long_multiply(OperFuncInfo fcinfo) {
  assert(fcinfo->args_num_ == 2);
  *(unsigned long *)fcinfo->result_ = ((*(unsigned long *)fcinfo->args_[0]) *
                                       (*(unsigned long *)fcinfo->args_[1]));
}
inline void u_long_divide(OperFuncInfo fcinfo) {
  assert(fcinfo->args_num_ == 2);
  unsigned long val = *(unsigned long *)fcinfo->args_[1];
  if (val == 0) {
    SQLParse_elog("The u_long_divide divided = 0");
    assert(false);
  }
  *(unsigned long *)fcinfo->result_ = ((*(unsigned long *)fcinfo->args_[0]) /
                                       (*(unsigned long *)fcinfo->args_[1]));
}
inline void u_long_mod(OperFuncInfo fcinfo) {
  assert(fcinfo->args_num_ == 2);
  unsigned long val = *(unsigned long *)fcinfo->args_[1];
  if (val == 0) {
    SQLParse_elog("The u_long_mod moded = 0");
    assert(false);
  }
  *(unsigned long *)fcinfo->result_ = ((*(unsigned long *)fcinfo->args_[0]) %
                                       (*(unsigned long *)fcinfo->args_[1]));
}
inline void u_long_equal(OperFuncInfo fcinfo) {
  assert(fcinfo->args_num_ == 2);
  *(bool *)fcinfo->result_ = (*(unsigned long *)fcinfo->args_[0] ==
                              *(unsigned long *)fcinfo->args_[1]);
}
inline void u_long_not_equal(OperFuncInfo fcinfo) {
  assert(fcinfo->args_num_ == 2);
  *(bool *)fcinfo->result_ = (*(unsigned long *)fcinfo->args_[0]) !=
                             (*(unsigned long *)fcinfo->args_[1]);
}
inline void u_long_great(OperFuncInfo fcinfo) {
  assert(fcinfo->args_num_ == 2);
  *(bool *)fcinfo->result_ = (*(unsigned long *)fcinfo->args_[0]) >
                             (*(unsigned long *)fcinfo->args_[1]);
}
inline void u_long_great_equal(OperFuncInfo fcinfo) {
  assert(fcinfo->args_num_ == 2);
  *(bool *)fcinfo->result_ = (*(unsigned long *)fcinfo->args_[0]) >=
                             (*(unsigned long *)fcinfo->args_[1]);
}
inline void u_long_less(OperFuncInfo fcinfo) {
  assert(fcinfo->args_num_ == 2);
  *(bool *)fcinfo->result_ = (*(unsigned long *)fcinfo->args_[0]) <
                             (*(unsigned long *)fcinfo->args_[1]);
}
inline void u_long_less_equal(OperFuncInfo fcinfo) {
  assert(fcinfo->args_num_ == 2);
  *(bool *)fcinfo->result_ = (*(unsigned long *)fcinfo->args_[0]) <=
                             (*(unsigned long *)fcinfo->args_[1]);
}

inline void u_long_agg_min(OperFuncInfo fcinfo) {
  assert(fcinfo->args_num_ == 2);
  *(unsigned long *)fcinfo->result_ =
      ((*(unsigned long *)fcinfo->args_[0]) <
               (*(unsigned long *)fcinfo->args_[1])
           ? (*(unsigned long *)fcinfo->args_[0])
           : (*(unsigned long *)fcinfo->args_[1]));
}
inline void u_long_agg_max(OperFuncInfo fcinfo) {
  assert(fcinfo->args_num_ == 2);
  *(unsigned long *)fcinfo->result_ =
      ((*(unsigned long *)fcinfo->args_[0]) >
               (*(unsigned long *)fcinfo->args_[1])
           ? (*(unsigned long *)fcinfo->args_[0])
           : (*(unsigned long *)fcinfo->args_[1]));
}
inline void u_long_agg_sum(OperFuncInfo fcinfo) { u_long_add(fcinfo); }
inline void u_long_agg_count(OperFuncInfo fcinfo) { u_long_add(fcinfo); }
/*******************u_long*************************/

/*******************float*************************/
inline void float_add(OperFuncInfo fcinfo) {
  assert(fcinfo->args_num_ == 2);
  *(float *)fcinfo->result_ =
      ((*(float *)fcinfo->args_[0]) + (*(float *)fcinfo->args_[1]));
}
inline void float_minus(OperFuncInfo fcinfo) {
  assert(fcinfo->args_num_ == 2);
  *(float *)fcinfo->result_ =
      ((*(float *)fcinfo->args_[0]) - (*(float *)fcinfo->args_[1]));
}
inline void float_multiply(OperFuncInfo fcinfo) {
  assert(fcinfo->args_num_ == 2);
  *(float *)fcinfo->result_ =
      ((*(float *)fcinfo->args_[0]) * (*(float *)fcinfo->args_[1]));
}
inline void float_divide(OperFuncInfo fcinfo) {
  assert(fcinfo->args_num_ == 2);
  float val = *(float *)fcinfo->args_[1];
  if (val == 0) {
    SQLParse_elog("The float_divide divided = 0");
    assert(false);
  }
  *(float *)fcinfo->result_ =
      ((*(float *)fcinfo->args_[0]) / (*(float *)fcinfo->args_[1]));
}

inline void float_equal(OperFuncInfo fcinfo) {
  assert(fcinfo->args_num_ == 2);
  *(bool *)fcinfo->result_ = fabs(double((*(float *)fcinfo->args_[0]) -
                                         (*(float *)fcinfo->args_[1]))) < DELTA;
}
inline void float_not_equal(OperFuncInfo fcinfo) {
  assert(fcinfo->args_num_ == 2);
  float_equal(fcinfo);
  *(bool *)fcinfo->result_ = !(*(bool *)fcinfo->result_);
}
inline void float_great(OperFuncInfo fcinfo) {
  assert(fcinfo->args_num_ == 2);
  *(bool *)fcinfo->result_ =
      (*(float *)fcinfo->args_[0]) > (*(float *)fcinfo->args_[1]);
}
inline void float_great_equal(OperFuncInfo fcinfo) {
  assert(fcinfo->args_num_ == 2);
  *(bool *)fcinfo->result_ =
      (*(float *)fcinfo->args_[0]) >= (*(float *)fcinfo->args_[1]);
}
inline void float_less(OperFuncInfo fcinfo) {
  assert(fcinfo->args_num_ == 2);
  *(bool *)fcinfo->result_ =
      (*(float *)fcinfo->args_[0]) < (*(float *)fcinfo->args_[1]);
}
inline void float_less_equal(OperFuncInfo fcinfo) {
  assert(fcinfo->args_num_ == 2);
  *(bool *)fcinfo->result_ =
      (*(float *)fcinfo->args_[0]) <= (*(float *)fcinfo->args_[1]);
}
inline void float_negative(OperFuncInfo fcinfo) {
  assert(fcinfo->args_num_ == 1);
  *(float *)fcinfo->result_ = ((*(float *)fcinfo->args_[0]) * (-1));
}
inline void float_agg_max(OperFuncInfo fcinfo) {
  assert(fcinfo->args_num_ == 2);
  *(float *)fcinfo->result_ =
      ((*(float *)fcinfo->args_[0]) > (*(float *)fcinfo->args_[1])
           ? (*(float *)fcinfo->args_[0])
           : (*(float *)fcinfo->args_[1]));
}
inline void float_agg_min(OperFuncInfo fcinfo) {
  assert(fcinfo->args_num_ == 2);
  *(float *)fcinfo->result_ =
      ((*(float *)fcinfo->args_[0]) < (*(float *)fcinfo->args_[1])
           ? (*(float *)fcinfo->args_[0])
           : (*(float *)fcinfo->args_[1]));
}
inline void float_agg_sum(OperFuncInfo fcinfo) { float_add(fcinfo); }
inline void float_agg_count(OperFuncInfo fcinfo) { float_add(fcinfo); }

/*******************float*************************/

/*******************double*************************/

inline void double_add(OperFuncInfo fcinfo) {
  assert(fcinfo->args_num_ == 2);
  *(double *)fcinfo->result_ =
      ((*(double *)fcinfo->args_[0]) + (*(double *)fcinfo->args_[1]));
}
inline void double_minus(OperFuncInfo fcinfo) {
  assert(fcinfo->args_num_ == 2);
  *(double *)fcinfo->result_ =
      ((*(double *)fcinfo->args_[0]) - (*(double *)fcinfo->args_[1]));
}
inline void double_multiply(OperFuncInfo fcinfo) {
  assert(fcinfo->args_num_ == 2);
  *(double *)fcinfo->result_ =
      ((*(double *)fcinfo->args_[0]) * (*(double *)fcinfo->args_[1]));
}
inline void double_divide(OperFuncInfo fcinfo) {
  assert(fcinfo->args_num_ == 2);
  double val = *(double *)fcinfo->args_[1];
  if (val == 0) {
    SQLParse_elog("The double_divide divided = 0");
    assert(false);
  }
  *(double *)fcinfo->result_ =
      ((*(double *)fcinfo->args_[0]) / (*(double *)fcinfo->args_[1]));
}

inline void double_equal(OperFuncInfo fcinfo) {
  assert(fcinfo->args_num_ == 2);
  *(bool *)fcinfo->result_ =
      fabs(*(double *)fcinfo->args_[0] - *(double *)fcinfo->args_[1]) < DELTA;
}
inline void double_not_equal(OperFuncInfo fcinfo) {
  assert(fcinfo->args_num_ == 2);
  double_equal(fcinfo);
  *(bool *)fcinfo->result_ = !(*(bool *)fcinfo->result_);
}
inline void double_great(OperFuncInfo fcinfo) {
  assert(fcinfo->args_num_ == 2);
  *(bool *)fcinfo->result_ =
      (*(double *)fcinfo->args_[0] > *(double *)fcinfo->args_[1]);
}
inline void double_great_equal(OperFuncInfo fcinfo) {
  assert(fcinfo->args_num_ == 2);
  *(bool *)fcinfo->result_ =
      (*(double *)fcinfo->args_[0] >= *(double *)fcinfo->args_[1]);
}
inline void double_less(OperFuncInfo fcinfo) {
  assert(fcinfo->args_num_ == 2);
  bool ans = 1;
  *(bool *)fcinfo->result_ =
      (*(double *)fcinfo->args_[0] < *(double *)fcinfo->args_[1]);
}
inline void double_less_equal(OperFuncInfo fcinfo) {
  assert(fcinfo->args_num_ == 2);
  *(bool *)fcinfo->result_ =
      (*(double *)fcinfo->args_[0] <= *(double *)fcinfo->args_[1]);
}
inline void double_negative(OperFuncInfo fcinfo) {
  assert(fcinfo->args_num_ == 1);
  *(double *)fcinfo->result_ = ((*(double *)fcinfo->args_[0]) * (-1));
}
inline void double_agg_max(OperFuncInfo fcinfo) {
  assert(fcinfo->args_num_ == 2);
  *(double *)fcinfo->result_ =
      ((*(double *)fcinfo->args_[0]) > (*(double *)fcinfo->args_[1])
           ? (*(double *)fcinfo->args_[0])
           : (*(double *)fcinfo->args_[1]));
}
inline void double_agg_min(OperFuncInfo fcinfo) {
  assert(fcinfo->args_num_ == 2);
  *(double *)fcinfo->result_ =
      ((*(double *)fcinfo->args_[0]) < (*(double *)fcinfo->args_[1])
           ? (*(double *)fcinfo->args_[0])
           : (*(double *)fcinfo->args_[1]));
}
inline void double_agg_sum(OperFuncInfo fcinfo) { double_add(fcinfo); }
inline void double_agg_count(OperFuncInfo fcinfo) { double_add(fcinfo); }

/*******************double*************************/

/*******************smallInt*************************/

inline void smallInt_add(OperFuncInfo fcinfo) {
  assert(fcinfo->args_num_ == 2);
  *(short int *)fcinfo->result_ =
      (*(short int *)fcinfo->args_[0]) + (*(short int *)fcinfo->args_[1]);
}
inline void smallInt_minus(OperFuncInfo fcinfo) {
  assert(fcinfo->args_num_ == 2);
  *(short int *)fcinfo->result_ =
      (*(short int *)fcinfo->args_[0]) - (*(short int *)fcinfo->args_[1]);
}
inline void smallInt_multiply(OperFuncInfo fcinfo) {
  assert(fcinfo->args_num_ == 2);
  *(short int *)fcinfo->result_ =
      (*(short int *)fcinfo->args_[0]) * (*(short int *)fcinfo->args_[1]);
}
inline void smallInt_divide(OperFuncInfo fcinfo) {
  assert(fcinfo->args_num_ == 2);
  short int val = *(short int *)fcinfo->args_[1];
  if (val == 0) {
    SQLParse_elog("The smallInt_divide divided = 0");
    assert(false);
  }
  *(short int *)fcinfo->result_ =
      (*(short int *)fcinfo->args_[0]) / (*(short int *)fcinfo->args_[1]);
}
inline void smallInt_mod(OperFuncInfo fcinfo) {
  assert(fcinfo->args_num_ == 2);
  short int val = *(short int *)fcinfo->args_[1];
  if (val == 0) {
    SQLParse_elog("The smallInt_mod moded = 0");
    assert(false);
  }
  *(short int *)fcinfo->result_ =
      (*(short int *)fcinfo->args_[0] % (*(short int *)fcinfo->args_[1]));
}
inline void smallInt_equal(OperFuncInfo fcinfo) {
  assert(fcinfo->args_num_ == 2);
  *(bool *)fcinfo->result_ =
      (*(short int *)fcinfo->args_[0] == *(short int *)fcinfo->args_[1]);
}
inline void smallInt_not_equal(OperFuncInfo fcinfo) {
  assert(fcinfo->args_num_ == 2);
  *(bool *)fcinfo->result_ =
      (*(short int *)fcinfo->args_[0] != *(short int *)fcinfo->args_[1]);
}
inline void smallInt_great(OperFuncInfo fcinfo) {
  assert(fcinfo->args_num_ == 2);
  *(bool *)fcinfo->result_ =
      (*(short int *)fcinfo->args_[0] > *(short int *)fcinfo->args_[1]);
}
inline void smallInt_great_equal(OperFuncInfo fcinfo) {
  assert(fcinfo->args_num_ == 2);
  *(bool *)fcinfo->result_ =
      (*(short int *)fcinfo->args_[0] >= *(short int *)fcinfo->args_[1]);
}
inline void smallInt_less(OperFuncInfo fcinfo) {
  assert(fcinfo->args_num_ == 2);
  *(bool *)fcinfo->result_ =
      (*(short int *)fcinfo->args_[0] < *(short int *)fcinfo->args_[1]);
}
inline void smallInt_less_equal(OperFuncInfo fcinfo) {
  assert(fcinfo->args_num_ == 2);
  *(bool *)fcinfo->result_ =
      (*(short int *)fcinfo->args_[0] <= *(short int *)fcinfo->args_[1]);
}
inline void smallInt_negative(OperFuncInfo fcinfo) {
  assert(fcinfo->args_num_ == 1);
  *(short int *)fcinfo->result_ = (*(short int *)fcinfo->args_[0] * (-1));
}

inline void smallInt_agg_max(OperFuncInfo fcinfo) {
  assert(fcinfo->args_num_ == 2);
  *(short int *)fcinfo->result_ =
      ((*(short int *)fcinfo->args_[0]) > (*(short int *)fcinfo->args_[1])
           ? (*(short int *)fcinfo->args_[0])
           : (*(short int *)fcinfo->args_[1]));
}
inline void smallInt_agg_min(OperFuncInfo fcinfo) {
  assert(fcinfo->args_num_ == 2);
  *(short int *)fcinfo->result_ =
      ((*(short int *)fcinfo->args_[0]) < (*(short int *)fcinfo->args_[1])
           ? (*(short int *)fcinfo->args_[0])
           : (*(short int *)fcinfo->args_[1]));
}
inline void smallInt_agg_sum(OperFuncInfo fcinfo) { smallInt_add(fcinfo); }
inline void smallInt_agg_count(OperFuncInfo fcinfo) { smallInt_add(fcinfo); }

/*******************smallInt*************************/

/*****************boolean********************/

inline void boolean_and(OperFuncInfo fcinfo) {
  assert(fcinfo->args_num_ == 2);
  *(bool *)fcinfo->result_ =
      ((*(bool *)fcinfo->args_[0]) && (*(bool *)fcinfo->args_[1]));
}
inline void boolean_or(OperFuncInfo fcinfo) {
  assert(fcinfo->args_num_ == 2);
  *(bool *)fcinfo->result_ =
      ((*(bool *)fcinfo->args_[0]) || (*(bool *)fcinfo->args_[1]));
}
inline void boolean_xor(OperFuncInfo fcinfo) {
  assert(fcinfo->args_num_ == 2);
  *(bool *)fcinfo->result_ =
      ((*(bool *)fcinfo->args_[0]) ^ (*(bool *)fcinfo->args_[1]));
}
inline void boolean_not(OperFuncInfo fcinfo) {
  assert(fcinfo->args_num_ == 1);
  *(bool *)fcinfo->result_ = (!(*(bool *)fcinfo->args_[0]));
}
inline void boolean_equal(OperFuncInfo fcinfo) {
  assert(fcinfo->args_num_ == 2);
  *(bool *)fcinfo->result_ =
      (*(bool *)fcinfo->args_[0] == *(bool *)fcinfo->args_[1]);
}
inline void boolean_not_equal(OperFuncInfo fcinfo) {
  assert(fcinfo->args_num_ == 2);
  *(bool *)fcinfo->result_ =
      (*(bool *)fcinfo->args_[0] != *(bool *)fcinfo->args_[1]);
}
inline void boolean_great(OperFuncInfo fcinfo) {
  assert(fcinfo->args_num_ == 2);
  *(bool *)fcinfo->result_ =
      (*(bool *)fcinfo->args_[0] > *(bool *)fcinfo->args_[1]);
}
inline void boolean_great_equal(OperFuncInfo fcinfo) {
  assert(fcinfo->args_num_ == 2);
  *(bool *)fcinfo->result_ =
      (*(bool *)fcinfo->args_[0] >= *(bool *)fcinfo->args_[1]);
}
inline void boolean_less(OperFuncInfo fcinfo) {
  assert(fcinfo->args_num_ == 2);
  *(bool *)fcinfo->result_ =
      (*(bool *)fcinfo->args_[0] < *(bool *)fcinfo->args_[1]);
}
inline void boolean_less_equal(OperFuncInfo fcinfo) {
  assert(fcinfo->args_num_ == 2);
  *(bool *)fcinfo->result_ =
      (*(bool *)fcinfo->args_[0] <= *(bool *)fcinfo->args_[1]);
}
/*****************boolean********************/

/*****************decimal********************/
inline void decimal_add(OperFuncInfo fcinfo) {
  assert(fcinfo->args_num_ == 2);
  *(Decimal *)fcinfo->result_ =
      (*(Decimal *)fcinfo->args_[0]).op_add(*(Decimal *)fcinfo->args_[1]);
}
inline void decimal_minus(OperFuncInfo fcinfo) {
  assert(fcinfo->args_num_ == 2);
  *(Decimal *)fcinfo->result_ =
      (*(Decimal *)fcinfo->args_[0]).op_subtract(*(Decimal *)fcinfo->args_[1]);
}
inline void decimal_multiply(OperFuncInfo fcinfo) {
  assert(fcinfo->args_num_ == 2);
  *(Decimal *)fcinfo->result_ =
      (*(Decimal *)fcinfo->args_[0]).op_multiply(*(Decimal *)fcinfo->args_[1]);
}
inline void decimal_divide(OperFuncInfo fcinfo) {
  assert(fcinfo->args_num_ == 2);
  *(Decimal *)fcinfo->result_ =
      (*(Decimal *)fcinfo->args_[0]).op_divide(*(Decimal *)fcinfo->args_[1]);
}
inline void decimal_equal(OperFuncInfo fcinfo) {
  assert(fcinfo->args_num_ == 2);
  *(bool *)fcinfo->result_ =
      (*(Decimal *)fcinfo->args_[0]).op_equals(*(Decimal *)fcinfo->args_[1]);
}
inline void decimal_not_equal(OperFuncInfo fcinfo) {
  assert(fcinfo->args_num_ == 2);
  *(bool *)fcinfo->result_ = (*(Decimal *)fcinfo->args_[0])
                                 .op_not_equals(*(Decimal *)fcinfo->args_[1]);
}
inline void decimal_great(OperFuncInfo fcinfo) {
  assert(fcinfo->args_num_ == 2);
  *(bool *)fcinfo->result_ =
      (*(Decimal *)fcinfo->args_[0]).op_great(*(Decimal *)fcinfo->args_[1]);
}
inline void decimal_great_equal(OperFuncInfo fcinfo) {
  assert(fcinfo->args_num_ == 2);
  *(bool *)fcinfo->result_ = (*(Decimal *)fcinfo->args_[0])
                                 .op_great_equals(*(Decimal *)fcinfo->args_[1]);
}
inline void decimal_less(OperFuncInfo fcinfo) {
  assert(fcinfo->args_num_ == 2);
  *(bool *)fcinfo->result_ =
      (*(Decimal *)fcinfo->args_[0]).op_less(*(Decimal *)fcinfo->args_[1]);
}
inline void decimal_less_equal(OperFuncInfo fcinfo) {
  assert(fcinfo->args_num_ == 2);
  *(bool *)fcinfo->result_ = (*(Decimal *)fcinfo->args_[0])
                                 .op_less_equals(*(Decimal *)fcinfo->args_[1]);
}
inline void decimal_negative(OperFuncInfo fcinfo) {
  assert(fcinfo->args_num_ == 1);
  if((*(Decimal *)fcinfo->args_[0]).isNull())
  	*(Decimal *)fcinfo->result_ = *(Decimal *)fcinfo->args_[0];
  else
  {
    Decimal neg(1, 0, "-1");
    *(Decimal *)fcinfo->result_ = (*(Decimal *)fcinfo->args_[0]).op_multiply(neg);
  }
}
inline void decimal_agg_max(OperFuncInfo fcinfo) {
  assert(fcinfo->args_num_ == 2);
  if((*(Decimal *)fcinfo->args_[1]).isNull()||(*(Decimal *)fcinfo->args_[0]).isNull())
  {
	*(Decimal *)fcinfo->result_ = (*(Decimal *)fcinfo->args_[1]).isNull()?(*(Decimal *)fcinfo->args_[0]):(*(Decimal *)fcinfo->args_[1]);
  }
  else
  {
  *(Decimal *)fcinfo->result_ =
      (*(Decimal *)fcinfo->args_[0]).op_great(*(Decimal *)fcinfo->args_[1])
          ? (*(Decimal *)fcinfo->args_[0])
          : (*(Decimal *)fcinfo->args_[1]);
  }
}
inline void decimal_agg_min(OperFuncInfo fcinfo) {
  assert(fcinfo->args_num_ == 2);
  *(Decimal *)fcinfo->result_ =
      (*(Decimal *)fcinfo->args_[0]).op_less(*(Decimal *)fcinfo->args_[1])
          ? (*(Decimal *)fcinfo->args_[0])
          : (*(Decimal *)fcinfo->args_[1]);
}
inline void decimal_agg_sum(OperFuncInfo fcinfo) { decimal_add(fcinfo); }
inline void decimal_agg_count(OperFuncInfo fcinfo) { decimal_add(fcinfo); }
/*****************decimal********************/

/*****************string********************/
// the limit is the left should be column
inline void string_equal(OperFuncInfo fcinfo) {
  assert(fcinfo->args_num_ == 2);
  *(bool *)fcinfo->result_ =
      (strcmp((char *)fcinfo->args_[0], (char *)fcinfo->args_[1]) == 0);
}
inline void string_not_equal(OperFuncInfo fcinfo) {
  assert(fcinfo->args_num_ == 2);
  *(bool *)fcinfo->result_ =
      (strcmp((char *)fcinfo->args_[0], (char *)fcinfo->args_[1]) != 0);
}
inline void string_great(OperFuncInfo fcinfo) {
  assert(fcinfo->args_num_ == 2);
  *(bool *)fcinfo->result_ =
      (strcmp((char *)fcinfo->args_[0], (char *)fcinfo->args_[1]) > 0);
}
inline void string_great_equal(OperFuncInfo fcinfo) {
  assert(fcinfo->args_num_ == 2);
  *(bool *)fcinfo->result_ =
      (strcmp((char *)fcinfo->args_[0], (char *)fcinfo->args_[1]) >= 0);
}
inline void string_less(OperFuncInfo fcinfo) {
  assert(fcinfo->args_num_ == 2);
  *(bool *)fcinfo->result_ =
      (strcmp((char *)fcinfo->args_[0], (char *)fcinfo->args_[1]) < 0);
}
inline void string_less_equal(OperFuncInfo fcinfo) {
  assert(fcinfo->args_num_ == 2);
  *(bool *)fcinfo->result_ =
      (strcmp((char *)fcinfo->args_[0], (char *)fcinfo->args_[1]) <= 0);
}
inline void string_both_trim(OperFuncInfo fcinfo) {
  assert(fcinfo->args_num_ == 2);
  int begin = 0, end = strlen((char *)fcinfo->args_[1]) - 1;
  for (; ((char *)fcinfo->args_[1])[begin] == ((char *)fcinfo->args_[0])[0] &&
             ((char *)fcinfo->args_[1])[begin] != '\0';
       begin++)
    ;
  if (end > begin)
    for (; ((char *)fcinfo->args_[1])[end] == ((char *)fcinfo->args_[0])[0] &&
               end >= 0;
         end--)
      ;
  strncpy((char *)fcinfo->result_, (char *)fcinfo->args_[1] + begin,
          end - begin + 1);
  ((char *)fcinfo->result_)[end - begin + 1] = '\0';
}
inline void string_leading_trim(OperFuncInfo fcinfo) {
  assert(fcinfo->args_num_ == 2);
  int begin = 0, end = strlen((char *)fcinfo->args_[1]) - 1;
  for (; ((char *)fcinfo->args_[1])[begin] == ((char *)fcinfo->args_[0])[0] &&
             ((char *)fcinfo->args_[1])[begin] != '\0';
       begin++)
    ;
  strncpy((char *)fcinfo->result_, (char *)fcinfo->args_[1] + begin,
          end - begin + 1);
  ((char *)fcinfo->result_)[end - begin + 1] = '\0';
}
inline void string_trailing_trim(OperFuncInfo fcinfo) {
  assert(fcinfo->args_num_ == 2);
  int begin = 0, end = strlen((char *)fcinfo->args_[1]) - 1;
  for (; ((char *)fcinfo->args_[1])[end] == ((char *)fcinfo->args_[0])[0] &&
             end >= 0;
       end--)
    ;
  strncpy((char *)fcinfo->result_, (char *)fcinfo->args_[1] + begin,
          end - begin + 1);
  ((char *)fcinfo->result_)[end - begin + 1] = '\0';
}
inline void string_like(OperFuncInfo fcinfo) {
  assert(fcinfo->args_num_ == 2);
  *(bool *)fcinfo->result_ =
      (MatchText((char *)fcinfo->args_[0], strlen((char *)fcinfo->args_[0]),
                 (char *)fcinfo->args_[1],
                 strlen((char *)fcinfo->args_[1])) == 1);
}
inline void string_not_like(OperFuncInfo fcinfo) {
  assert(fcinfo->args_num_ == 2);
  *(bool *)fcinfo->result_ =
      (MatchText((char *)fcinfo->args_[0], strlen((char *)fcinfo->args_[0]),
                 (char *)fcinfo->args_[1],
                 strlen((char *)fcinfo->args_[1])) != 1);
}
inline void string_upper(OperFuncInfo fcinfo) {
  assert(fcinfo->args_num_ == 1);
  strcpy((char *)fcinfo->result_, (char *)fcinfo->args_[0]);
  int len = strlen((char *)fcinfo->args_[0]);
  for (int i = 0; i < len; i++) {
    if (((char *)fcinfo->result_)[i] >= 'a' &&
        ((char *)fcinfo->result_)[i] <= 'z')
      ((char *)fcinfo->result_)[i] -= 32;
  }
  ((char *)fcinfo->result_)[len] = '\0';
}
inline void string_substring(OperFuncInfo fcinfo) {
  assert(fcinfo->args_num_ == 3);
  strncpy((char *)fcinfo->result_,
          ((char *)fcinfo->args_[0]) + (*(int *)fcinfo->args_[1]),
          (*(int *)fcinfo->args_[2]) - (*(int *)fcinfo->args_[1]));
}

/*****************string********************/

/*****************date********************/
inline void date_equal(OperFuncInfo fcinfo) {
  assert(fcinfo->args_num_ == 2);
  *(bool *)fcinfo->result_ =
      (*(date *)fcinfo->args_[0] == *(date *)fcinfo->args_[1]);
}
inline void date_not_equal(OperFuncInfo fcinfo) {
  assert(fcinfo->args_num_ == 2);
  *(bool *)fcinfo->result_ =
      (*(date *)fcinfo->args_[0] != *(date *)fcinfo->args_[1]);
}
inline void date_great(OperFuncInfo fcinfo) {
  assert(fcinfo->args_num_ == 2);
  *(bool *)fcinfo->result_ =
      (*(date *)fcinfo->args_[0] > *(date *)fcinfo->args_[1]);
}
inline void date_great_equal(OperFuncInfo fcinfo) {
  assert(fcinfo->args_num_ == 2);
  *(bool *)fcinfo->result_ =
      (*(date *)fcinfo->args_[0] >= *(date *)fcinfo->args_[1]);
}
inline void date_less(OperFuncInfo fcinfo) {
  assert(fcinfo->args_num_ == 2);
  *(bool *)fcinfo->result_ =
      (*(date *)fcinfo->args_[0] < *(date *)fcinfo->args_[1]);
}
inline void date_less_equal(OperFuncInfo fcinfo) {
  assert(fcinfo->args_num_ == 2);
  *(bool *)fcinfo->result_ =
      (*(date *)fcinfo->args_[0] <= *(date *)fcinfo->args_[1]);
}

inline void date_add_day(OperFuncInfo fcinfo) {
  assert(fcinfo->args_num_ == 2);
  *(date *)fcinfo->result_ =
      (*(date *)fcinfo->args_[0]) + (*(date_duration *)fcinfo->args_[1]);
}
inline void date_add_week(OperFuncInfo fcinfo) {
  assert(fcinfo->args_num_ == 2);
  // *(date *)fcinfo->result_ =
  //    (*(date *)fcinfo->args_[0]) + (*(weeks *)fcinfo->args_[1]);
}

inline void date_add_month(OperFuncInfo fcinfo) {
  assert(fcinfo->args_num_ == 2);
  // *(date *)fcinfo->result_ =
  //    (*(date *)fcinfo->args_[0]) + (*(months *)fcinfo->args_[1]);
}

inline void date_add_year(OperFuncInfo fcinfo) {
  assert(fcinfo->args_num_ == 2);
  // *(date *)fcinfo->result_ =
  //    (*(date *)fcinfo->args_[0]) + (*(years *)fcinfo->args_[1]);
}

inline void date_sub_day(OperFuncInfo fcinfo) {
  assert(fcinfo->args_num_ == 2);
  *(date *)fcinfo->result_ =
      (*(date *)fcinfo->args_[0]) - (*(date_duration *)fcinfo->args_[1]);
}
inline void date_sub_week(OperFuncInfo fcinfo) {
  assert(fcinfo->args_num_ == 2);
  // *(date *)fcinfo->result_ =
  //    (*(date *)fcinfo->args_[0]) - (*(weeks *)fcinfo->args_[1]);
}
inline void date_sub_month(OperFuncInfo fcinfo) {
  assert(fcinfo->args_num_ == 2);
  // *(date *)fcinfo->result_ =
  //    (*(date *)fcinfo->args_[0]) - (*(months *)fcinfo->args_[1]);
}
inline void date_sub_year(OperFuncInfo fcinfo) {
  assert(fcinfo->args_num_ == 2);
  // *(date *)fcinfo->result_ =
  //    (*(date *)fcinfo->args_[0]) - (*(years *)fcinfo->args_[1]);
}
inline void date_agg_max(OperFuncInfo fcinfo) {
  assert(fcinfo->args_num_ == 2);
  *(date *)fcinfo->result_ =
      (*(date *)fcinfo->args_[0] > *(date *)fcinfo->args_[1])
          ? *(date *)fcinfo->args_[0]
          : *(date *)fcinfo->args_[1];
}
inline void date_agg_min(OperFuncInfo fcinfo) {
  assert(fcinfo->args_num_ == 2);
  *(date *)fcinfo->result_ =
      (*(date *)fcinfo->args_[0] < *(date *)fcinfo->args_[1])
          ? *(date *)fcinfo->args_[0]
          : *(date *)fcinfo->args_[1];
}

/*****************date********************/

/*****************time********************/
inline void time_equal(OperFuncInfo fcinfo) {
  assert(fcinfo->args_num_ == 2);
  *(bool *)fcinfo->result_ = (*(time_duration *)fcinfo->args_[0] ==
                              *(time_duration *)fcinfo->args_[1]);
}
inline void time_not_equal(OperFuncInfo fcinfo) {
  assert(fcinfo->args_num_ == 2);
  *(bool *)fcinfo->result_ = (*(time_duration *)fcinfo->args_[0] !=
                              *(time_duration *)fcinfo->args_[1]);
}
inline void time_great(OperFuncInfo fcinfo) {
  assert(fcinfo->args_num_ == 2);
  *(bool *)fcinfo->result_ =
      (*(time_duration *)fcinfo->args_[0] > *(time_duration *)fcinfo->args_[1]);
}
inline void time_great_equal(OperFuncInfo fcinfo) {
  assert(fcinfo->args_num_ == 2);
  *(bool *)fcinfo->result_ = (*(time_duration *)fcinfo->args_[0] >=
                              *(time_duration *)fcinfo->args_[1]);
}
inline void time_less(OperFuncInfo fcinfo) {
  assert(fcinfo->args_num_ == 2);
  *(bool *)fcinfo->result_ =
      (*(time_duration *)fcinfo->args_[0] < *(time_duration *)fcinfo->args_[1]);
}
inline void time_less_equal(OperFuncInfo fcinfo) {
  assert(fcinfo->args_num_ == 2);
  *(bool *)fcinfo->result_ = (*(time_duration *)fcinfo->args_[0] <=
                              *(time_duration *)fcinfo->args_[1]);
}

inline void time_agg_max(OperFuncInfo fcinfo) {
  assert(fcinfo->args_num_ == 2);
  *(time_duration *)fcinfo->result_ =
      (*(time_duration *)fcinfo->args_[0] > *(time_duration *)fcinfo->args_[1])
          ? *(time_duration *)fcinfo->args_[0]
          : *(time_duration *)fcinfo->args_[1];
}
inline void time_agg_min(OperFuncInfo fcinfo) {
  assert(fcinfo->args_num_ == 2);
  *(time_duration *)fcinfo->result_ =
      (*(time_duration *)fcinfo->args_[0] < *(time_duration *)fcinfo->args_[1])
          ? *(time_duration *)fcinfo->args_[0]
          : *(time_duration *)fcinfo->args_[1];
}
/*****************time********************/

/*****************datetime********************/
inline void datetime_equal(OperFuncInfo fcinfo) {
  assert(fcinfo->args_num_ == 2);
  *(bool *)fcinfo->result_ =
      (*(ptime *)fcinfo->args_[0] == *(ptime *)fcinfo->args_[1]);
}
inline void datetime_not_equal(OperFuncInfo fcinfo) {
  assert(fcinfo->args_num_ == 2);
  *(bool *)fcinfo->result_ =
      (*(ptime *)fcinfo->args_[0] != *(ptime *)fcinfo->args_[1]);
}
inline void datetime_great(OperFuncInfo fcinfo) {
  assert(fcinfo->args_num_ == 2);
  *(bool *)fcinfo->result_ =
      (*(ptime *)fcinfo->args_[0] > *(ptime *)fcinfo->args_[1]);
}
inline void datetime_great_equal(OperFuncInfo fcinfo) {
  assert(fcinfo->args_num_ == 2);
  *(bool *)fcinfo->result_ =
      (*(ptime *)fcinfo->args_[0] >= *(ptime *)fcinfo->args_[1]);
}
inline void datetime_less(OperFuncInfo fcinfo) {
  assert(fcinfo->args_num_ == 2);
  *(bool *)fcinfo->result_ =
      (*(ptime *)fcinfo->args_[0] < *(ptime *)fcinfo->args_[1]);
}
inline void datetime_less_equal(OperFuncInfo fcinfo) {
  assert(fcinfo->args_num_ == 2);
  *(bool *)fcinfo->result_ =
      (*(ptime *)fcinfo->args_[0] <= *(ptime *)fcinfo->args_[1]);
}
inline void datetime_agg_max(OperFuncInfo fcinfo) {
  assert(fcinfo->args_num_ == 2);
  *(ptime *)fcinfo->result_ =
      (*(ptime *)fcinfo->args_[0] > *(ptime *)fcinfo->args_[1])
          ? *(ptime *)fcinfo->args_[0]
          : *(ptime *)fcinfo->args_[1];
}
inline void datetime_agg_min(OperFuncInfo fcinfo) {
  assert(fcinfo->args_num_ == 2);
  *(ptime *)fcinfo->result_ =
      (*(ptime *)fcinfo->args_[0] < *(ptime *)fcinfo->args_[1])
          ? *(ptime *)fcinfo->args_[0]
          : *(ptime *)fcinfo->args_[1];
}
/*****************datetime********************/
inline void InitOperatorFunc() {
  for (int i = 0; i < DATA_TYPE_NUM; i++)
    for (int j = 0; j < OPER_TYPE_NUM; j++)
      DataTypeOper::data_type_oper_func_[i][j] = oper_not_support;
  /*****************int********************/
  DataTypeOper::data_type_oper_func_[t_int][oper_add] = int_add;
  DataTypeOper::data_type_oper_func_[t_int][oper_minus] = int_minus;
  DataTypeOper::data_type_oper_func_[t_int][oper_multiply] = int_multiply;
  DataTypeOper::data_type_oper_func_[t_int][oper_divide] = int_divide;
  DataTypeOper::data_type_oper_func_[t_int][oper_mod] = int_mod;
  DataTypeOper::data_type_oper_func_[t_int][oper_and] = oper_not_support;
  DataTypeOper::data_type_oper_func_[t_int][oper_or] = oper_not_support;
  DataTypeOper::data_type_oper_func_[t_int][oper_xor] = oper_not_support;
  DataTypeOper::data_type_oper_func_[t_int][oper_not] = oper_not_support;
  DataTypeOper::data_type_oper_func_[t_int][oper_equal] = int_equal;
  DataTypeOper::data_type_oper_func_[t_int][oper_not_equal] = int_not_equal;
  DataTypeOper::data_type_oper_func_[t_int][oper_great] = int_great;
  DataTypeOper::data_type_oper_func_[t_int][oper_great_equal] = int_great_equal;
  DataTypeOper::data_type_oper_func_[t_int][oper_less] = int_less;
  DataTypeOper::data_type_oper_func_[t_int][oper_less_equal] = int_less_equal;
  DataTypeOper::data_type_oper_func_[t_int][oper_negative] = int_negative;
  DataTypeOper::data_type_oper_func_[t_int][oper_max] = int_agg_max;
  DataTypeOper::data_type_oper_func_[t_int][oper_min] = int_agg_min;
  DataTypeOper::data_type_oper_func_[t_int][oper_agg_sum] = int_agg_sum;
  DataTypeOper::data_type_oper_func_[t_int][oper_agg_count] = int_agg_count;
  /*****************int********************/

  /*****************ulong********************/
  DataTypeOper::data_type_oper_func_[t_u_long][oper_add] = u_long_add;
  DataTypeOper::data_type_oper_func_[t_u_long][oper_minus] = u_long_minus;
  DataTypeOper::data_type_oper_func_[t_u_long][oper_multiply] = u_long_multiply;
  DataTypeOper::data_type_oper_func_[t_u_long][oper_divide] = u_long_divide;
  DataTypeOper::data_type_oper_func_[t_u_long][oper_mod] = u_long_mod;
  DataTypeOper::data_type_oper_func_[t_u_long][oper_and] = oper_not_support;
  DataTypeOper::data_type_oper_func_[t_u_long][oper_or] = oper_not_support;
  DataTypeOper::data_type_oper_func_[t_u_long][oper_xor] = oper_not_support;
  DataTypeOper::data_type_oper_func_[t_u_long][oper_not] = oper_not_support;
  DataTypeOper::data_type_oper_func_[t_u_long][oper_equal] = u_long_equal;
  DataTypeOper::data_type_oper_func_[t_u_long][oper_not_equal] =
      u_long_not_equal;
  DataTypeOper::data_type_oper_func_[t_u_long][oper_great] = u_long_great;
  DataTypeOper::data_type_oper_func_[t_u_long][oper_great_equal] =
      u_long_great_equal;
  DataTypeOper::data_type_oper_func_[t_u_long][oper_less] = u_long_less;
  DataTypeOper::data_type_oper_func_[t_u_long][oper_less_equal] =
      u_long_less_equal;
  DataTypeOper::data_type_oper_func_[t_u_long][oper_negative] =
      oper_not_support;
  DataTypeOper::data_type_oper_func_[t_u_long][oper_max] = u_long_agg_max;
  DataTypeOper::data_type_oper_func_[t_u_long][oper_min] = u_long_agg_min;
  DataTypeOper::data_type_oper_func_[t_u_long][oper_agg_sum] = u_long_agg_sum;
  DataTypeOper::data_type_oper_func_[t_u_long][oper_agg_count] =
      u_long_agg_count;
  /*****************ulong********************/

  /*****************float********************/
  DataTypeOper::data_type_oper_func_[t_float][oper_add] = float_add;
  DataTypeOper::data_type_oper_func_[t_float][oper_minus] = float_minus;
  DataTypeOper::data_type_oper_func_[t_float][oper_multiply] = float_multiply;
  DataTypeOper::data_type_oper_func_[t_float][oper_divide] = float_divide;
  DataTypeOper::data_type_oper_func_[t_float][oper_mod] = oper_not_support;
  DataTypeOper::data_type_oper_func_[t_float][oper_and] = oper_not_support;
  DataTypeOper::data_type_oper_func_[t_float][oper_or] = oper_not_support;
  DataTypeOper::data_type_oper_func_[t_float][oper_xor] = oper_not_support;
  DataTypeOper::data_type_oper_func_[t_float][oper_not] = oper_not_support;
  DataTypeOper::data_type_oper_func_[t_float][oper_equal] = float_equal;
  DataTypeOper::data_type_oper_func_[t_float][oper_not_equal] = float_not_equal;
  DataTypeOper::data_type_oper_func_[t_float][oper_great] = float_great;
  DataTypeOper::data_type_oper_func_[t_float][oper_great_equal] =
      float_great_equal;
  DataTypeOper::data_type_oper_func_[t_float][oper_less] = float_less;
  DataTypeOper::data_type_oper_func_[t_float][oper_less_equal] =
      float_less_equal;
  DataTypeOper::data_type_oper_func_[t_float][oper_negative] = float_negative;
  DataTypeOper::data_type_oper_func_[t_float][oper_max] = float_agg_max;
  DataTypeOper::data_type_oper_func_[t_float][oper_min] = float_agg_min;
  DataTypeOper::data_type_oper_func_[t_float][oper_agg_sum] = float_agg_sum;
  DataTypeOper::data_type_oper_func_[t_float][oper_agg_count] = float_agg_count;
  /*****************float********************/

  /*****************double********************/

  DataTypeOper::data_type_oper_func_[t_double][oper_add] = double_add;
  DataTypeOper::data_type_oper_func_[t_double][oper_minus] = double_minus;
  DataTypeOper::data_type_oper_func_[t_double][oper_multiply] = double_multiply;
  DataTypeOper::data_type_oper_func_[t_double][oper_divide] = double_divide;
  DataTypeOper::data_type_oper_func_[t_double][oper_mod] = oper_not_support;
  DataTypeOper::data_type_oper_func_[t_double][oper_and] = oper_not_support;
  DataTypeOper::data_type_oper_func_[t_double][oper_or] = oper_not_support;
  DataTypeOper::data_type_oper_func_[t_double][oper_xor] = oper_not_support;
  DataTypeOper::data_type_oper_func_[t_double][oper_not] = oper_not_support;
  DataTypeOper::data_type_oper_func_[t_double][oper_equal] = double_equal;
  DataTypeOper::data_type_oper_func_[t_double][oper_not_equal] =
      double_not_equal;
  DataTypeOper::data_type_oper_func_[t_double][oper_great] = double_great;
  DataTypeOper::data_type_oper_func_[t_double][oper_great_equal] =
      double_great_equal;
  DataTypeOper::data_type_oper_func_[t_double][oper_less] = double_less;
  DataTypeOper::data_type_oper_func_[t_double][oper_less_equal] =
      double_less_equal;
  DataTypeOper::data_type_oper_func_[t_double][oper_negative] = double_negative;
  DataTypeOper::data_type_oper_func_[t_double][oper_max] = double_agg_max;
  DataTypeOper::data_type_oper_func_[t_double][oper_min] = double_agg_min;
  DataTypeOper::data_type_oper_func_[t_double][oper_agg_sum] = double_agg_sum;
  DataTypeOper::data_type_oper_func_[t_double][oper_agg_count] =
      double_agg_count;
  /*****************double********************/

  /*****************smallInt********************/
  DataTypeOper::data_type_oper_func_[t_smallInt][oper_add] = smallInt_add;
  DataTypeOper::data_type_oper_func_[t_smallInt][oper_minus] = smallInt_minus;
  DataTypeOper::data_type_oper_func_[t_smallInt][oper_multiply] =
      smallInt_multiply;
  DataTypeOper::data_type_oper_func_[t_smallInt][oper_divide] = smallInt_divide;
  DataTypeOper::data_type_oper_func_[t_smallInt][oper_mod] = smallInt_mod;
  DataTypeOper::data_type_oper_func_[t_smallInt][oper_and] = oper_not_support;
  DataTypeOper::data_type_oper_func_[t_smallInt][oper_or] = oper_not_support;
  DataTypeOper::data_type_oper_func_[t_smallInt][oper_xor] = oper_not_support;
  DataTypeOper::data_type_oper_func_[t_smallInt][oper_not] = oper_not_support;
  DataTypeOper::data_type_oper_func_[t_smallInt][oper_equal] = smallInt_equal;
  DataTypeOper::data_type_oper_func_[t_smallInt][oper_not_equal] =
      smallInt_not_equal;
  DataTypeOper::data_type_oper_func_[t_smallInt][oper_great] = smallInt_great;
  DataTypeOper::data_type_oper_func_[t_smallInt][oper_great_equal] =
      smallInt_great_equal;
  DataTypeOper::data_type_oper_func_[t_smallInt][oper_less] = smallInt_less;
  DataTypeOper::data_type_oper_func_[t_smallInt][oper_less_equal] =
      smallInt_less_equal;
  DataTypeOper::data_type_oper_func_[t_smallInt][oper_negative] =
      smallInt_negative;
  DataTypeOper::data_type_oper_func_[t_smallInt][oper_max] = smallInt_agg_max;
  DataTypeOper::data_type_oper_func_[t_smallInt][oper_min] = smallInt_agg_min;
  DataTypeOper::data_type_oper_func_[t_smallInt][oper_agg_sum] =
      smallInt_agg_sum;
  DataTypeOper::data_type_oper_func_[t_smallInt][oper_agg_count] =
      smallInt_agg_count;
  /*****************smallInt********************/

  /*****************boolean********************/
  DataTypeOper::data_type_oper_func_[t_boolean][oper_add] = oper_not_support;
  DataTypeOper::data_type_oper_func_[t_boolean][oper_minus] = oper_not_support;
  DataTypeOper::data_type_oper_func_[t_boolean][oper_multiply] =
      oper_not_support;
  DataTypeOper::data_type_oper_func_[t_boolean][oper_divide] = oper_not_support;
  DataTypeOper::data_type_oper_func_[t_boolean][oper_mod] = oper_not_support;
  DataTypeOper::data_type_oper_func_[t_boolean][oper_and] = boolean_and;
  DataTypeOper::data_type_oper_func_[t_boolean][oper_or] = boolean_or;
  DataTypeOper::data_type_oper_func_[t_boolean][oper_xor] = boolean_xor;
  DataTypeOper::data_type_oper_func_[t_boolean][oper_not] = boolean_not;
  DataTypeOper::data_type_oper_func_[t_boolean][oper_equal] = boolean_equal;
  DataTypeOper::data_type_oper_func_[t_boolean][oper_not_equal] =
      boolean_not_equal;
  DataTypeOper::data_type_oper_func_[t_boolean][oper_great] = boolean_great;
  DataTypeOper::data_type_oper_func_[t_boolean][oper_great_equal] =
      boolean_great_equal;
  DataTypeOper::data_type_oper_func_[t_boolean][oper_less] = boolean_less;
  DataTypeOper::data_type_oper_func_[t_boolean][oper_less_equal] =
      boolean_less_equal;
  DataTypeOper::data_type_oper_func_[t_boolean][oper_negative] =
      oper_not_support;

  /*****************boolean********************/

  /*****************decimal********************/

  DataTypeOper::data_type_oper_func_[t_decimal][oper_add] = decimal_add;
  DataTypeOper::data_type_oper_func_[t_decimal][oper_minus] = decimal_minus;
  DataTypeOper::data_type_oper_func_[t_decimal][oper_multiply] =
      decimal_multiply;
  DataTypeOper::data_type_oper_func_[t_decimal][oper_divide] = decimal_divide;
  DataTypeOper::data_type_oper_func_[t_decimal][oper_mod] = oper_not_support;
  DataTypeOper::data_type_oper_func_[t_decimal][oper_and] = oper_not_support;
  DataTypeOper::data_type_oper_func_[t_decimal][oper_or] = oper_not_support;
  DataTypeOper::data_type_oper_func_[t_decimal][oper_xor] = oper_not_support;
  DataTypeOper::data_type_oper_func_[t_decimal][oper_not] = oper_not_support;
  DataTypeOper::data_type_oper_func_[t_decimal][oper_equal] = decimal_equal;
  DataTypeOper::data_type_oper_func_[t_decimal][oper_not_equal] =
      decimal_not_equal;
  DataTypeOper::data_type_oper_func_[t_decimal][oper_great] = decimal_great;
  DataTypeOper::data_type_oper_func_[t_decimal][oper_great_equal] =
      decimal_great_equal;
  DataTypeOper::data_type_oper_func_[t_decimal][oper_less] = decimal_less;
  DataTypeOper::data_type_oper_func_[t_decimal][oper_less_equal] =
      decimal_less_equal;
  DataTypeOper::data_type_oper_func_[t_decimal][oper_negative] =
      decimal_negative;
  DataTypeOper::data_type_oper_func_[t_decimal][oper_max] = decimal_agg_max;
  DataTypeOper::data_type_oper_func_[t_decimal][oper_min] = decimal_agg_min;
  DataTypeOper::data_type_oper_func_[t_decimal][oper_agg_sum] = decimal_agg_sum;
  DataTypeOper::data_type_oper_func_[t_decimal][oper_agg_count] =
      decimal_agg_count;
  /*****************decimal********************/

  /*****************string********************/
  DataTypeOper::data_type_oper_func_[t_string][oper_add] = oper_not_support;
  DataTypeOper::data_type_oper_func_[t_string][oper_minus] = oper_not_support;
  DataTypeOper::data_type_oper_func_[t_string][oper_multiply] =
      oper_not_support;
  DataTypeOper::data_type_oper_func_[t_string][oper_divide] = oper_not_support;
  DataTypeOper::data_type_oper_func_[t_string][oper_mod] = oper_not_support;
  DataTypeOper::data_type_oper_func_[t_string][oper_and] = oper_not_support;
  DataTypeOper::data_type_oper_func_[t_string][oper_or] = oper_not_support;
  DataTypeOper::data_type_oper_func_[t_string][oper_xor] = oper_not_support;
  DataTypeOper::data_type_oper_func_[t_string][oper_not] = oper_not_support;
  DataTypeOper::data_type_oper_func_[t_string][oper_equal] = string_equal;
  DataTypeOper::data_type_oper_func_[t_string][oper_not_equal] =
      string_not_equal;
  DataTypeOper::data_type_oper_func_[t_string][oper_great] = string_great;
  DataTypeOper::data_type_oper_func_[t_string][oper_great_equal] =
      string_great_equal;
  DataTypeOper::data_type_oper_func_[t_string][oper_less] = string_less;
  DataTypeOper::data_type_oper_func_[t_string][oper_less_equal] =
      string_less_equal;

  DataTypeOper::data_type_oper_func_[t_string][oper_both_trim] =
      string_both_trim;
  DataTypeOper::data_type_oper_func_[t_string][oper_leading_trim] =
      string_leading_trim;
  DataTypeOper::data_type_oper_func_[t_string][oper_trailing_trim] =
      string_trailing_trim;
  DataTypeOper::data_type_oper_func_[t_string][oper_like] = string_like;
  DataTypeOper::data_type_oper_func_[t_string][oper_not_like] = string_not_like;
  DataTypeOper::data_type_oper_func_[t_string][oper_upper] = string_upper;
  DataTypeOper::data_type_oper_func_[t_string][oper_substring] =
      string_substring;

  /*****************string********************/

  /*****************date********************/
  DataTypeOper::data_type_oper_func_[t_date][oper_add] = oper_not_support;
  DataTypeOper::data_type_oper_func_[t_date][oper_minus] = oper_not_support;
  DataTypeOper::data_type_oper_func_[t_date][oper_multiply] = oper_not_support;
  DataTypeOper::data_type_oper_func_[t_date][oper_divide] = oper_not_support;
  DataTypeOper::data_type_oper_func_[t_date][oper_mod] = oper_not_support;
  DataTypeOper::data_type_oper_func_[t_date][oper_and] = oper_not_support;
  DataTypeOper::data_type_oper_func_[t_date][oper_or] = oper_not_support;
  DataTypeOper::data_type_oper_func_[t_date][oper_xor] = oper_not_support;
  DataTypeOper::data_type_oper_func_[t_date][oper_not] = oper_not_support;
  DataTypeOper::data_type_oper_func_[t_date][oper_equal] = date_equal;
  DataTypeOper::data_type_oper_func_[t_date][oper_not_equal] = date_not_equal;
  DataTypeOper::data_type_oper_func_[t_date][oper_great] = date_great;
  DataTypeOper::data_type_oper_func_[t_date][oper_great_equal] =
      date_great_equal;
  DataTypeOper::data_type_oper_func_[t_date][oper_less] = date_less;
  DataTypeOper::data_type_oper_func_[t_date][oper_less_equal] = date_less_equal;
  DataTypeOper::data_type_oper_func_[t_date][oper_negative] = oper_not_support;

  DataTypeOper::data_type_oper_func_[t_date][oper_date_add_day] = date_add_day;
  DataTypeOper::data_type_oper_func_[t_date][oper_date_add_week] =
      date_add_week;
  DataTypeOper::data_type_oper_func_[t_date][oper_date_add_month] =
      date_add_month;
  DataTypeOper::data_type_oper_func_[t_date][oper_date_add_year] =
      date_add_year;
  DataTypeOper::data_type_oper_func_[t_date][oper_date_sub_day] = date_sub_day;
  DataTypeOper::data_type_oper_func_[t_date][oper_date_sub_week] =
      date_sub_week;
  DataTypeOper::data_type_oper_func_[t_date][oper_date_sub_month] =
      date_sub_month;
  DataTypeOper::data_type_oper_func_[t_date][oper_date_sub_year] =
      date_sub_year;
  DataTypeOper::data_type_oper_func_[t_date][oper_max] = date_agg_max;
  DataTypeOper::data_type_oper_func_[t_date][oper_min] = date_agg_min;
  /*****************date********************/

  /*****************time********************/
  DataTypeOper::data_type_oper_func_[t_time][oper_equal] = time_equal;
  DataTypeOper::data_type_oper_func_[t_time][oper_not_equal] = time_not_equal;
  DataTypeOper::data_type_oper_func_[t_time][oper_great] = time_great;
  DataTypeOper::data_type_oper_func_[t_time][oper_great_equal] =
      time_great_equal;
  DataTypeOper::data_type_oper_func_[t_time][oper_less] = time_less;
  DataTypeOper::data_type_oper_func_[t_time][oper_less_equal] = time_less_equal;
  DataTypeOper::data_type_oper_func_[t_time][oper_max] = time_agg_max;
  DataTypeOper::data_type_oper_func_[t_time][oper_min] = time_agg_min;
  /*****************time********************/

  /*****************datetime********************/
  DataTypeOper::data_type_oper_func_[t_datetime][oper_equal] = datetime_equal;
  DataTypeOper::data_type_oper_func_[t_datetime][oper_not_equal] =
      datetime_not_equal;
  DataTypeOper::data_type_oper_func_[t_datetime][oper_great] = datetime_great;
  DataTypeOper::data_type_oper_func_[t_datetime][oper_great_equal] =
      datetime_great_equal;
  DataTypeOper::data_type_oper_func_[t_datetime][oper_less] = datetime_less;
  DataTypeOper::data_type_oper_func_[t_datetime][oper_less_equal] =
      datetime_less_equal;
  DataTypeOper::data_type_oper_func_[t_datetime][oper_max] = datetime_agg_max;
  DataTypeOper::data_type_oper_func_[t_datetime][oper_min] = datetime_agg_min;
  /*****************datetime********************/
}
inline void avg_error_divide(void *sum_value, int64_t tuple_number,
                             void *result) {
  SQLParse_elog("This oper_functions is not supported now!!!!!!!!!!!!!");
  assert(false);
}
inline void avg_int_divide(void *sum_value, int64_t tuple_number,
                           void *result) {
  *(int *)result = (*(int *)sum_value / tuple_number);
}
inline void avg_float_divide(void *sum_value, int64_t tuple_number,
                             void *result) {
  *(float *)result = *(float *)sum_value / tuple_number;
}
inline void avg_double_divide(void *sum_value, int64_t tuple_number,
                              void *result) {
  *(double *)result = *(double *)sum_value / tuple_number;
}
inline void avg_ulong_divide(void *sum_value, int64_t tuple_number,
                             void *result) {
  *(unsigned long *)result = *(unsigned long *)sum_value / tuple_number;
}
inline void avg_smallint_divide(void *sum_value, int64_t tuple_number,
                                void *result) {
  *(short *)result = (*(short *)sum_value / tuple_number);
}
inline void avg_usmallint_divide(void *sum_value, int64_t tuple_number,
                                 void *result) {
  *(unsigned short *)result = (*(unsigned short *)sum_value / tuple_number);
}
inline void avg_decimal_divide(void *sum_value, int64_t tuple_number,
                               void *result) {
  *(Decimal *)result = *(Decimal *)sum_value;
  stringstream ss;
  ss << tuple_number;
  Decimal tn(CLAIMS_COMMON_DECIMAL_PSUBS, 0, ss.str());
  *(Decimal *)result = (*(Decimal *)result).op_divide(tn);
}

/*
 *avg=sum/tuple_number, use avg_divid[type] to avoid switching among all types
 *typedef void (*AvgDivide)(void *sum_value,int64_t tuple_number,void
 **result); in qnode.h
 *AvgDivide DataTypeOper::avg_divide_[DATA_TYPE_NUM]; in queryfunc.h
 */
inline void InitAggAvgDivide() {
  for (int i = 0; i < DATA_TYPE_NUM; i++)
    DataTypeOper::avg_divide_[i] = avg_error_divide;
  DataTypeOper::avg_divide_[t_int] = avg_int_divide;
  DataTypeOper::avg_divide_[t_float] = avg_float_divide;
  DataTypeOper::avg_divide_[t_double] = avg_double_divide;
  DataTypeOper::avg_divide_[t_u_long] = avg_ulong_divide;
  DataTypeOper::avg_divide_[t_smallInt] = avg_smallint_divide;
  DataTypeOper::avg_divide_[t_u_smallInt] = avg_usmallint_divide;
  DataTypeOper::avg_divide_[t_decimal] = avg_decimal_divide;
}

}  // namespace common
}  // namespace claims
#endif  //  COMMON_EXPRESSION_DATA_TYPE_OPER_H_
