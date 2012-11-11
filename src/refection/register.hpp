/*
 * register.hpp
 *
 *  Created on: 2012-10-18
 *      Author: yanghuan
 */

#ifndef REGISTER_HPP_
#define REGISTER_HPP_

#include "tuple_cat.hpp"

namespace refection {

}

#define REFECTION_GLOBAL_NAME(namespace_tuple,class_type) \
    BOOST_PP_CAT(BOOST_PP_CAT(REFECTION_TUPLE_CAT(namespace_tuple), REFECTION_SEQ_SIGN), class_type)

#define REFECTION_CLASS_REGISTER_GLOBAL(namespace_tuple,class_type) \
    extern "C" const void* REFECTION_GLOBAL_NAME(namespace_tuple,class_type)() { \
        return &(_##class_type##_type_); \
    }

#endif // REGISTER_HPP_
