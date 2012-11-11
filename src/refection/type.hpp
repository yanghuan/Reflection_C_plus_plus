/*
 * type.hpp
 *
 *  Created on: 2012-9-26
 *      Author: yanghuan
 */

#ifndef TYPE_HPP_
#define TYPE_HPP_

#include <cstddef>
#include <typeinfo>

namespace refection {

class MethodInfo;
class Object;

struct InfoPackage {
    const std::type_info& type_info_;
    const char* name_;
    const MethodInfo* method_list_;
    const size_t method_list_size_;
};

struct Type {

public:
    constexpr Type(const InfoPackage& info_package) : package_(info_package) {}
public:
    InfoPackage package_;
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
public:
    const Type& declaring_type_;
    const std::type_info& signature_;
    const FuncPtrDataTypes ptr_;
    const char* name_;
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
