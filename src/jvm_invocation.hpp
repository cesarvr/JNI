//
//  jvm_type.h
//  LibJNI
//

#ifndef jvm_type_h
#define jvm_type_h

#include "utils.h"
#include "jvm_global.hpp"

/*
    JNIType
    
    - specialized templates to tell what to do with specific types.
    - if you want to support a new type just add a new class.
*/

template<typename T>
struct JNIType: HandleEnv {
    T value;
    
    JNIType(JVMLoader env):HandleEnv(env){};
    
    void SetValue(T val){
        value = val;
    }
    
    T GetValue(){
        return value;
    }
};

template<>
struct JNIType<jint>:HandleEnv {
    jint value;
    
    //JNIType(JVMLoader env):HandleEnv(env){};
    
    void SetValue(jint val){
        value = (int)val;
    }
    
    int GetValue(){
        return value;
    }
};

template<>
struct JNIType<jstring>:HandleEnv {
    
    jstring value;
    
    JNIType(JVMLoader env):HandleEnv(env){};
    
    void SetValue(jobject val){
        value = (jstring)val;
    }
    
    std::string GetValue(){
        auto env = GetEnv();
        if(value == nullptr) return "Undefined";
        
        const char *str =env->GetStringUTFChars( (jstring)value , NULL );
        std::string tmp{str};
        env->ReleaseStringUTFChars( (jstring)value ,str );
        
        return tmp;
    };
    
};



template<> // CLANG LLVM
struct JNIType<std::__1::basic_string<char>>:HandleEnv {
    
    jstring value;
    
    JNIType(JVMLoader env):HandleEnv(env){};
    
    void SetValue(jobject val){
        value = (jstring)val;
    }
    
    std::string GetValue(){
        auto env = GetEnv();
        if(value == nullptr) return "Undefined";
        
        const char *str =env->GetStringUTFChars( (jstring)value , NULL );
        std::string tmp{str};
        env->ReleaseStringUTFChars( (jstring)value ,str );
        
        return tmp;
    };
    
};




/*
 Make JNI call and capture possible exceptions.
*/


template <typename Function, typename ...Args>
auto Wrapper(Function&& func, JEnv env,  Args &&... args)
-> decltype(func(env.get(), args...))
{
    try {
        Utils::isNull(func);
        Utils::isNull(env);
        
        auto x = func(env.get(), args...);
        
        if (env->ExceptionCheck()){
            throw VMError{"error in JVM: \n"};
            env->ExceptionDescribe();
        }
        
        return x;
    }catch(VMError& error){
        env->ExceptionDescribe();
        throw error;
    };
}

/*
 Class template to wrap JNI function pointers.
 
 - FunctionPtr: takes a function pointer and save it for later use in Call.
 */

template <typename FunctionPtr>
class Functor : HandleEnv {
    
private:
    JEnv env;
    FunctionPtr func;
    
public:
    
    Functor( JVMLoader _env , FunctionPtr _func ): HandleEnv(_env), func(_func) { };
    
    template <typename ReturnType, typename... Args>
    JNIType<ReturnType> Call(Args... args) {
        
        auto env = GetEnv();
        JNIType<ReturnType> ret(GetLoader());
        
        auto value = Wrapper(func, env, args...);
        ret.SetValue(value);
        
        
        
        return ret;
    };
};

#endif /* jvm_type_h */
