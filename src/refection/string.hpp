/*
 * string.hpp
 *
 *  Created on: 2012-10-18
 *      Author: yanghuan
 */

#ifndef STRING_HPP_
#define STRING_HPP_

#include "pair_array.hpp"

namespace refection {

template<char c>
struct PairElementTrait<boost::mpl::char_<c>> {
    typedef char type;
    static constexpr char create() {
        return c;
    }
};

template<class String>
struct C_str {
    static const size_t size = boost::mpl::size<String>::value;
    typedef typename PairArray<char, size + 1>::type value_type;
    static const value_type value;
    static constexpr const char* c_str() {
        return reinterpret_cast<const char*>(&value);
    }
};

template<class String>
const typename C_str<String>::value_type C_str<String>::value = std::make_pair(getPairArray<String,char>(), '\0');

template<int... c>
struct StringTuple {
    typedef boost::mpl::string<c...> type;
};

template<int E, int B>
constexpr typename std::enable_if<(B + sizeof(int) <= E),int>::type
getCharInt(const char* s) {
    return ((s + B)[0] << 24) + ((s + B)[1] << 16) + ((s + B)[2] << 8) + (s + B)[3];
}

template<int E, int B>
constexpr typename std::enable_if<(B >= E),int>::type
getCharInt(const char* s) {
    return '\0';
}

template<int E, int B>
constexpr typename std::enable_if<(B + 1 == E),int>::type
getCharInt(const char* s) {
    return (s + B)[0];
}

template<int E, int B>
constexpr typename std::enable_if<(B + 2 == E),int>::type
getCharInt(const char* s) {
    return ((s + B)[0] << 8) + (s + B)[1];
}

template<int E, int B>
constexpr typename std::enable_if<(B + 3 == E),int>::type
getCharInt(const char* s) {
    return ((s + B)[0] << 16) + ((s + B)[1] << 8) + (s + B)[2];
}


template<class String1, class String2>
constexpr typename std::enable_if<boost::mpl::empty<String1>::value || boost::mpl::empty<String2>::value,bool>::type
lessString() {
    typedef boost::mpl::empty<String1> EmptyStr1;
    typedef boost::mpl::empty<String2> EmptyStr2;
    return (EmptyStr1::value && EmptyStr2::value ? false : (EmptyStr1::value && !EmptyStr2::value ? true : false));
}

template<class String1, class String2>
constexpr typename std::enable_if<!boost::mpl::empty<String1>::value && !boost::mpl::empty<String2>::value,bool>::type
lessString() {
    typedef typename boost::mpl::front<String1>::type First1;
    typedef typename boost::mpl::pop_front<String1>::type Taill;

    typedef typename boost::mpl::front<String2>::type First2;
    typedef typename boost::mpl::pop_front<String2>::type Tail2;

    return First1::value < First2::value ? true :
            (First1::value > First2::value ? false : lessString<Taill,Tail2>());
}

struct CompareString {
    template<class String1, class String2>
    struct apply {
        typedef boost::mpl::bool_<lessString<String1, String2>()> type;
    };
};

struct CompareClassWithString {
    template<class Class1, class Class2>
    struct apply {
        typedef typename CompareString::apply<typename Class1::string_type, typename Class2::string_type>::type type;
    };
};

namespace detail {

constexpr size_t strLenOf(const char* s, size_t len) {
    return *s == '\0' ? len : strLenOf(s + 1, len + 1);
}

constexpr int compareChar(char a, char b) {
    return a > b ? 1 : (a < b ? -1 : 0);
}

constexpr bool strEqual(const char* a, const char* b, size_t size, bool equal) {
   return size == 0 ? equal : (compareChar(*a, *b) == 0  ? strEqual(a + 1, b + 1, size - 1, equal) : false);
}

}  // namespace detail

constexpr size_t getStrLenOf(const char* s) {
    return detail::strLenOf(s, 0);
}

constexpr bool isStrEqual(const char* a, const char* b, size_t size) {
    return detail::strEqual(a, b, size, true);
}

namespace detail {

constexpr const char* lastOf(const char* s ,const char* sus, const char* pos) {
    return getStrLenOf(s) < getStrLenOf(sus) ? pos :
            (isStrEqual(s, sus, getStrLenOf(sus)) ? lastOf(s + getStrLenOf(sus), sus, s) : lastOf(s + 1, sus, pos));
}

}  // namespace detail

constexpr const char* getLastOf(const char* s ,const char* sus) {
    return detail::lastOf(s, sus ,nullptr);
}

}  // namespace refection

#define GET_CHAR_INT(s,inx) refection::getCharInt<(sizeof(s) -1), inx * sizeof(int)>(s)

#define ST_CT(s) refection::StringTuple< \
    GET_CHAR_INT(s,0), \
    GET_CHAR_INT(s,1), \
    GET_CHAR_INT(s,2), \
    GET_CHAR_INT(s,3), \
    GET_CHAR_INT(s,4), \
    GET_CHAR_INT(s,5), \
    GET_CHAR_INT(s,6), \
    GET_CHAR_INT(s,7) \
    >::type


#endif /* STRING_HPP_ */
