//
//  jvm_reflect.hpp
//  LibJNI
//
//  Created by Cesar Valdez on 26/02/2016.
//  Copyright © 2016 Cesar Valdez. All rights reserved.
//

#ifndef jvm_reflect_hpp
#define jvm_reflect_hpp

#include "utils.h"
//#include "jvm_argument.hpp"
#include "values.hpp"
//#include "jvm_invocation.hpp"
#include "jvm_handler.h"
#include "jinvoke.hpp"

using ObjectInterface = jobject(*)(JNIEnv *env, jobject obj, jmethodID methodID, const jvalue * args);
using IntegerInterface = jint(*)(JNIEnv *env, jobject obj, jmethodID methodID, const jvalue * args);


const std::string JAVA_METHOD_CLASS("java/lang/reflect/Method");
const std::string JAVA_STRING_CLASS("java/lang/String");
const std::string CLASS_DEFAULT_CTS("<init>");
const std::string VOID_RETURN("()V");

/*
struct JavaMethod {
    std::string name;
    std::string returnType;
    Arguments arguments;
    jmethodID methodPTR;
}; */






class Reflect: HandleEnv {
private:
    
    const std::string JAVA_CLASS = "java/lang/Class";
    const std::string METHOD_CLASS = "java/lang/reflect/Method";
    
    const std::string METHOD_GET_CLASS = "getClass";
    const std::string METHOD_GET_METHODS = "getMethods";
    const std::string METHOD_RETURN_TYPE = "getReturnType";
    const std::string METHOD_TOSTRING = "toString";
    const std::string METHOD_GET_NAME = "getName";
    const std::string METHOD_GET_PARAMETER = "getParameterTypes";
    
    jobject clazz;
   // Functor<ObjectInterface> objectMethod;
    Invoke invoke;
    
public:
    
    void SetClass(jobject object);
    jobject GetReflectClass();
    
    Reflect(JVMLoader env);
    
    jmethodID GetMethod( std::string className, std::string method, std::string returnType );
    
    std::vector<JavaMethod> GetMethodsDefinition();
    
    // reflects methods.
    std::string GetReturnType(jobject object);
    std::string GetName(std::string className, jobject object);
    std::string ToString(std::string className, jobject object);
    std::vector<std::string> GetParameters(jobject object);
};





class Object: HandleEnv {
private:
    jobject object;
    std::string name;
    std::vector<JavaMethod> methods;
    
    Reflect reflect;
   // Functor<ObjectInterface> objectMethod;
   // Functor<IntegerInterface> intMethod;
    Invoke invoke;
    
public:
    Object(JVMLoader env, std::string className);
    
    JavaMethod FindMethod( std::string methodName );
    
    
    template <typename T>
    LibJNI::Value<T> Call(std::string methodName, std::vector<LibJNI::BaseJavaValue *>& arguments ) {
        
        auto method = FindMethod(methodName);
        
        auto javaValues = Arguments::Create(GetEnv(), method.ArgumentsType(), arguments);
        // objectMethod.Call<T>(object, method.GetMethodRef(), (jvalue*)&javaValues[0]);
        
        return invoke.Call<T>(object, method.GetMethodRef(), (jvalue*)&javaValues[0]);
    }

    
    const std::vector<JavaMethod>& GetMembers();
    
    std::string GetClassName();
    
    void ReleaseThread(){ Release(); }
};

#endif /* jvm_reflect_hpp */
