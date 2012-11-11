/*
 * pair_array.hpp
 *
 *  Created on: 2012-10-18
 *      Author: yanghuan
 */

#ifndef PAIR_ARRAY_HPP_
#define PAIR_ARRAY_HPP_

#include <type_traits>
#include <utility>

#include <boost/mpl/front.hpp>
#include <boost/mpl/pop_front.hpp>
#include <boost/mpl/string.hpp>

namespace refection {

template<class T,size_t Size>
struct PairArray {
    typedef std::pair<typename PairArray<T,Size - 1>::type,T> type;
};

template<class T>
struct PairArray<T,1> {
    typedef T type;
};

template<class T>
struct PairArray<T,0> {

};

template<class T>
struct PairElementTrait {

};

template<class Sequence, size_t size, class T, class Pair>
constexpr typename std::enable_if<size == 0, Pair>::type
getPairArray(Pair p) {
    return p;
}

template<class Sequence, size_t size, class T, class Pair>
constexpr typename std::enable_if<size != 0, typename PairArray<T, size + sizeof(Pair) / sizeof(T)>::type>::type
getPairArray(Pair p) {
    typedef typename boost::mpl::front<Sequence>::type First;
    typedef typename boost::mpl::pop_front<Sequence>::type Tail;
    typedef PairElementTrait<First> ElementTrait;
    return getPairArray<Tail, boost::mpl::size<Tail>::value, T>(std::make_pair(p, ElementTrait::create()));
}

template<class Sequence,class T>
constexpr typename PairArray<T, boost::mpl::size<Sequence>::value>::type
getPairArray() {
    typedef typename boost::mpl::front<Sequence>::type First;
    typedef typename boost::mpl::pop_front<Sequence>::type Tail;
    typedef PairElementTrait<First> ElementTrait;
    return getPairArray<Tail, boost::mpl::size<Tail>::value, T>(ElementTrait::create());
}

namespace detail {

template<class Sequence, size_t Size>
struct Array {
    typedef typename boost::mpl::front<Sequence>::type First;
    typedef PairElementTrait<First> ElementTrait;
    typedef typename ElementTrait::type element_type;
    typedef typename PairArray<element_type,Size>::type value_type;
    static_assert(sizeof(value_type) == Size * sizeof(element_type),"");
    static const value_type value;
    static constexpr const element_type* array() {
        return reinterpret_cast<const element_type*>(&value);
    }
};

template<class Sequence, size_t Size>
const typename Array<Sequence, Size>::value_type Array<Sequence, Size>::value
    = getPairArray<Sequence,typename Array<Sequence, Size>::element_type>();

template<class Sequence>
struct Array<Sequence, 0> {
    static constexpr const void* array() {
        return nullptr;
    }
};

}  // namespace detail

template<class Sequence>
struct C_Array {
    static const size_t size = boost::mpl::size<Sequence>::value;
    typedef detail::Array<Sequence,size> array_type;
    static constexpr auto array() ->decltype(array_type::array()) {
        return array_type::array();
    }
};

} // namespace refection


#endif /* PAIR_ARRAY_HPP_ */
