/*
 * method_wrap.hpp
 *
 *  Created on: 2012-9-26
 *      Author: yanghuan
 */

#ifndef METHOD_WRAP_HPP_
#define METHOD_WRAP_HPP_

#include <boost/mpl/vector.hpp>
#include <boost/mpl/sort.hpp>

#include "type.hpp"
#include "string.hpp"
#include "tuple_cat.hpp"

namespace refection {

template<class Functor>
struct FunctorSignInfo;

template<class R, class ... ARG>
struct FunctorSignInfo<R (*)(ARG...)> {
    typedef R (*type)(ARG...);
};

template<class R, class Class, class ... ARG>
struct FunctorSignInfo<R (Class::*)(ARG...)> {
    typedef R (Object::*type)(ARG...);
};

template<class R, class Class, class ... ARG>
struct FunctorSignInfo<R (Class::*)(ARG...) const> {
    typedef R (Object::*type)(ARG...);
};

#if 0
template<class R, class ... ARG>
struct FunctorSignInfo<R(ARG...)> {
    typedef R (&type)(ARG...);
};

template<class R, class ... ARG>
struct FunctorSignInfo<R (&)(ARG...)> {
    typedef R (&type)(ARG...);
};
#endif


template<const Type* v>
struct TypePointer {
    static constexpr const Type* value = v;
};

template<class Class, class Functor, Functor functor, class String, class Pointer>
struct MethodInfoWrap {
    static_assert(std::is_base_of<Object,Class>::value,"");
    typedef Class class_type;
    typedef String string_type;
    typedef typename FunctorSignInfo<Functor>::type functor_type;

    static constexpr MethodInfo createMethodInfo() {
        return createMethodInfo(std::is_member_function_pointer<Functor>());
    }

    static constexpr MethodInfo createMethodInfo(std::true_type) {
        return {
            *Pointer::value,
            typeid(functor_type),
            reinterpret_cast<const MethodInfo::MemFuncPtr>(functor),
            C_str<string_type>::c_str()
        };
    }

    static constexpr MethodInfo createMethodInfo(std::false_type) {
        return {
            *Pointer::value,
            typeid(functor_type) ,
            reinterpret_cast<const MethodInfo::FuncPtr>(functor),
            C_str<string_type>::c_str()
        };
    }
};

template<class Class, class Functor, Functor functor, class String, class Pointer>
struct PairElementTrait<MethodInfoWrap<Class, Functor, functor, String, Pointer>> {
    typedef MethodInfo type;
    static constexpr MethodInfo create() {
        return MethodInfoWrap<Class, Functor, functor, String, Pointer>::createMethodInfo();
    }
};

#define METHOD_INFO_WRAP(O,n,Sign,type_pointer) \
    refection::MethodInfoWrap<O,Sign,&O::n,ST_CT(#n),type_pointer>

#define REFECTION_FULL_NAME(namespace_tuple,class_type) \
    BOOST_PP_CAT(BOOST_PP_CAT(REFECTION_TUPLE_CAT(namespace_tuple), REFECTION_SEQ_SIGN), class_type)

}  // namespace refection

#define REFECTION_CLASS_IMPLEMENT_BEGIN(Class) \
    constexpr Type _##Class##_createType(const char*); \
    static const refection::Type _##Class##_type_ = _##Class##_createType(#Class); \
    const refection::Type& Class::typeInstance() { \
        return _##Class##_type_; \
    } \
    inline constexpr Type _##Class##_createType(const char* n) { \
        typedef Class class_type; \
        typedef TypePointer<&_##Class##_type_> type_pointer; \
        typedef boost::mpl::vector<


#define REFECTION_CLASS_IMPLEMENT_END() \
        boost::mpl::na \
        > MethodListType; \
        typedef boost::mpl::sort<MethodListType, \
                refection::CompareClassWithString>::type SortedType; \
        typedef refection::C_Array<SortedType> result; \
        return InfoPackage{typeid(class_type), n, static_cast<const MethodInfo*>(result::array()), result::size}; \
    }

#define REFECTION_METHOD_IMPLEMENT(n,sign) METHOD_INFO_WRAP(class_type,n,sign,type_pointer)




#endif /* METHOD_WRAP_HPP_ */
