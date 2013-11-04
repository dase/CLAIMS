/*
 * foreach.hpp
 *
 *  Created on: 2013-5-21
 *      Author: root
 */

#ifndef FOREACH_HPP_
#define FOREACH_HPP_

#include <boost/foreach.hpp>

#include <boost/tuple/tuple.hpp>

#define BOOST_FOREACH_VALUE(VALUE, COL)                                                                                   \
    BOOST_FOREACH_PREAMBLE()                                                                                      \
    if (boost::foreach_detail_::auto_any_t BOOST_FOREACH_ID(_foreach_col) = BOOST_FOREACH_CONTAIN(COL)) {} else   \
    if (boost::foreach_detail_::auto_any_t BOOST_FOREACH_ID(_foreach_cur) = BOOST_FOREACH_BEGIN(COL)) {} else     \
    if (boost::foreach_detail_::auto_any_t BOOST_FOREACH_ID(_foreach_end) = BOOST_FOREACH_END(COL)) {} else       \
    for (bool BOOST_FOREACH_ID(_foreach_continue) = true;                                                         \
              BOOST_FOREACH_ID(_foreach_continue) && !BOOST_FOREACH_DONE(COL);                                    \
              BOOST_FOREACH_ID(_foreach_continue) ? BOOST_FOREACH_NEXT(COL) : (void)0)                            \
        if  (boost::foreach_detail_::set_false(BOOST_FOREACH_ID(_foreach_continue))) {} else                      \
        for (VALUE = BOOST_FOREACH_DEREF(COL).second; !BOOST_FOREACH_ID(_foreach_continue); BOOST_FOREACH_ID(_foreach_continue) = true)

#define BOOST_FOREACH_KEY(KEY, COL)                                                                                   \
    BOOST_FOREACH_PREAMBLE()                                                                                      \
    if (boost::foreach_detail_::auto_any_t BOOST_FOREACH_ID(_foreach_col) = BOOST_FOREACH_CONTAIN(COL)) {} else   \
    if (boost::foreach_detail_::auto_any_t BOOST_FOREACH_ID(_foreach_cur) = BOOST_FOREACH_BEGIN(COL)) {} else     \
    if (boost::foreach_detail_::auto_any_t BOOST_FOREACH_ID(_foreach_end) = BOOST_FOREACH_END(COL)) {} else       \
    for (bool BOOST_FOREACH_ID(_foreach_continue) = true;                                                         \
              BOOST_FOREACH_ID(_foreach_continue) && !BOOST_FOREACH_DONE(COL);                                    \
              BOOST_FOREACH_ID(_foreach_continue) ? BOOST_FOREACH_NEXT(COL) : (void)0)                            \
        if  (boost::foreach_detail_::set_false(BOOST_FOREACH_ID(_foreach_continue))) {} else                      \
        for (KEY = BOOST_FOREACH_DEREF(COL).first; !BOOST_FOREACH_ID(_foreach_continue); BOOST_FOREACH_ID(_foreach_continue) = true)

#define foreach BOOST_FOREACH
#define foreachvalue BOOST_FOREACH_VALUE
#define foreachkey BOOST_FOREACH_KEY



#endif /* FOREACH_HPP_ */
