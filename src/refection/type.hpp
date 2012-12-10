/*
 * type.hpp
 *
 *  Created on: 2012-9-26
 *      Author: yanghuan
 */

#ifndef TYPE_HPP_
#define TYPE_HPP_

#include <algorithm>
#include <cstring>
#include <typeinfo>
#include <utility>
#include <stdexcept>

namespace refection {

class MethodInfo;
class Type;
class Object;

struct InfoPackage {
    const std::type_info& type_info_;
    const char* name_;
    const MethodInfo* method_list_;
    const size_t method_list_size_;
};

struct MethodInfo {
    typedef void (*FuncPtr)();
    typedef void (Object::*MemFuncPtr)();
    union FuncPtrDataTypes {
        FuncPtr func_ptr;
        MemFuncPtr mem_func_ptr;
    public:
        constexpr FuncPtrDataTypes(FuncPtr ptr) : func_ptr(ptr)  {}
        constexpr FuncPtrDataTypes(MemFuncPtr ptr) : mem_func_ptr(ptr)  {}
    };
public:
    constexpr MethodInfo(const Type& declaring_type, const std::type_info& signature,FuncPtr ptr,const char* name)
      : declaring_type_(declaring_type),signature_(signature),ptr_(ptr),name_(name) {
    }
    constexpr MethodInfo(const Type& declaring_type, const std::type_info& signature,MemFuncPtr ptr,const char* name)
      : declaring_type_(declaring_type),signature_(signature),ptr_(ptr),name_(name) {
    }

    template<class R, class ... Args>
    R invoke(Object* obj, Args&&... args) const {
        typedef R (Object::*MemFuncPtr)(Args...);
        typedef R (*StaticMemFuncPtr)(Args...);
        if(obj == nullptr) {
            if(typeid(StaticMemFuncPtr) == signature_) {
                StaticMemFuncPtr f = reinterpret_cast<StaticMemFuncPtr>(ptr_.func_ptr);
                return f(std::forward<Args>(args)...);
            }
        }
        else {
            if(typeid(MemFuncPtr) == signature_) {
                MemFuncPtr f = reinterpret_cast<MemFuncPtr>(ptr_.mem_func_ptr);
                return (obj->*f)(std::forward<Args>(args)...);
            }
        }
        throw std::invalid_argument("Argument not match");
    }
public:
    const Type& declaring_type_;
    const std::type_info& signature_;
    const FuncPtrDataTypes ptr_;
    const char* name_;
};

struct Type {
public:
    constexpr Type(const InfoPackage& info_package) : package_(info_package) {}

    const MethodInfo* getMethod(const char* name) const {
        auto begin = package_.method_list_;
        auto end = package_.method_list_ + package_.method_list_size_;
        begin = std::lower_bound(begin, end, name, [](const MethodInfo& m, const char* n){
            return std::strcmp(m.name_, n) < 0;
        });
        return (begin != end && std::strcmp(begin->name_, name) == 0) ? begin : nullptr;
    }
public:
    InfoPackage package_;
};

#define REFECTION_CLASS_DECLARE()                           \
    public:                                                 \
        virtual const Type& getType() {                     \
            return typeInstance();                          \
        }                                                   \
        static const Type& typeInstance()


class Object {
public:
    REFECTION_CLASS_DECLARE();
public:
    virtual ~Object() {
    }
};


}  // namespace refection


#endif /* TYPE_HPP_ */
