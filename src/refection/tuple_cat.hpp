/*
 * tuple_cat.hpp
 *
 *  Created on: 2012-10-18
 *      Author: yanghuan
 */

#ifndef TUPLE_CAT_HPP_
#define TUPLE_CAT_HPP_

#include <boost/preprocessor/arithmetic/dec.hpp>
#include <boost/preprocessor/cat.hpp>
#include <boost/preprocessor/control/if.hpp>
#include <boost/preprocessor/seq/fold_left.hpp>
#include <boost/preprocessor/seq/size.hpp>
#include <boost/preprocessor/tuple/to_seq.hpp>

#define REFECTION_SEQ_SIGN $0
#define REFECTION_SEQ_OP(s, state, x) BOOST_PP_CAT(state, BOOST_PP_CAT(REFECTION_SEQ_SIGN,x))
#define REFECTION_SEQ_CAT_I(seq) BOOST_PP_SEQ_FOLD_LEFT(REFECTION_SEQ_OP, BOOST_PP_SEQ_HEAD(seq), BOOST_PP_SEQ_TAIL(seq))

#define REFECTION_SEQ_CAT(seq) \
    BOOST_PP_IF( \
        BOOST_PP_DEC(BOOST_PP_SEQ_SIZE(seq)), \
        REFECTION_SEQ_CAT_I, \
        BOOST_PP_SEQ_HEAD \
    )(seq)


#define REFECTION_TUPLE_CAT(tuple) REFECTION_SEQ_CAT(BOOST_PP_TUPLE_TO_SEQ(tuple))


#endif /* TUPLE_CAT_HPP_ */
