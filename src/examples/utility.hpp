/*
 * utility.hpp
 *
 *  Created on: 2012-9-26
 *      Author: yanghuan
 */

#ifndef UTILITY_HPP_
#define UTILITY_HPP_

#include <cxxabi.h>
#include <cstdlib>
#include <cstddef>
#include <string>

template<class T, size_t N>
constexpr size_t sizeofArray(T (&)[N]) {
    return N;
}

inline std::string getDemangleString(const char* name) {
    int status;
    char* c_str = abi::__cxa_demangle(name, nullptr, 0, &status);
    std::string s;
    if (status == 0) {
        s.assign(c_str);
        std::free(c_str);
    } else {
        s.assign(c_str);
    }
    return s;
}


#endif /* UTILITY_HPP_ */
