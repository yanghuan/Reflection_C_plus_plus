/*
 * test1.cpp
 *
 *  Created on: 2012-9-21
 *      Author: yanghuan
 */

#include <cstdlib>
#include <cstring>
#include <iostream>
#include <tuple>
#include <type_traits>

#include <functional>

#include <boost/mpl/at.hpp>
#include <boost/mpl/vector.hpp>
#include <boost/mpl/sort.hpp>

#include "refection/method_wrap.hpp"
#include "utility.hpp"

using namespace std;
using namespace boost;
using namespace refection;

namespace test {

struct AA : public refection::Object
{
public:
    REFECTION_CLASS_DECLARE();
public:
    void f() {}
    static int aa() {
        cout << "aa function is invoke" << endl;
        return 10;
    }
    void e(int) {
        cout << "e function is invoke" << endl;
    }
};

REFECTION_CLASS_IMPLEMENT_BEGIN(AA)
    REFECTION_METHOD_IMPLEMENT(f, void(AA::*)()),
    REFECTION_METHOD_IMPLEMENT(e, void(AA::*)(int)),
    REFECTION_METHOD_IMPLEMENT(aa, int(*)()),
REFECTION_CLASS_IMPLEMENT_END()

}  // namespace test

void test1() {
    test::AA aa;
    const Type& t = aa.getType();
    const MethodInfo* array = t.package_.method_list_;
    const size_t size = t.package_.method_list_size_;

    for(size_t i = 0 ; i < size; ++i) {
        cout << getDemangleString(array[i].declaring_type_.package_.type_info_.name()) << endl;
        cout << getDemangleString(array[i].signature_.name()) << endl;
        cout << array[i].name_ << endl;
        cout << array[i].declaring_type_.package_.name_ << endl;
        cout << reinterpret_cast<const void*>(array[i].ptr_.func_ptr) << endl;
    }

    /* invoke function  */
    const MethodInfo* func = t.getMethod("e");
    if(func != nullptr) {
        func->invoke<void>(&aa,30);
    }

    func = t.getMethod("aa");
    if (func != nullptr) {
        int resoult = func->invoke<int>(nullptr);
        cout << "resoult:" << resoult <<  endl;
    }
}

