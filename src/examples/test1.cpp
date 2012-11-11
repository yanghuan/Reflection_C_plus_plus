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
    static void aa() {}
    void e(int) {}
};

REFECTION_CLASS_IMPLEMENT_BEGIN(AA)
    REFECTION_METHOD_IMPLEMENT(f, void(AA::*)()),
    REFECTION_METHOD_IMPLEMENT(e, void(AA::*)(int)),
    REFECTION_METHOD_IMPLEMENT(aa, void(*)()),
REFECTION_CLASS_IMPLEMENT_END()

}  // namespace test

void test1() {
    const Type& t = test::AA::typeInstance();
    const MethodInfo* array = t.package_.method_list_;
    const size_t size = t.package_.method_list_size_;

    for(size_t i = 0 ; i < size; ++i)
    {
        cout << getDemangleString(array[i].declaring_type_.package_.type_info_.name()) << endl;
        cout << getDemangleString(array[i].signature_.name()) << endl;
        cout << array[i].name_ << endl;
        cout << array[i].declaring_type_.package_.name_ << endl;
    }
}

