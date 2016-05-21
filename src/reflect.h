//
//  jvm_reflect.hpp
//  LibJNI
//
//  Created by Cesar Valdez on 21/05/2016.
//  Copyright © 2016 Cesar Valdez. All rights reserved.
//

#ifndef jvm_reflect_hpp
#define jvm_reflect_hpp

#include <algorithm>
#include "utils.h"
#include "jvm_handler.h"
#include "jinvoke.h"
#include "args.h"

// this encapsulate all Java reflection functionality.
class Reflect: HandleEnv {
private:
    
    const std::string JAVA_CLASS = "java/lang/Class";
    const std::string METHOD_CLASS = "java/lang/reflect/Method";
    const std::string METHOD_CONSTRUCTOR = "java/lang/reflect/Constructor";
    
    const std::string METHOD_GET_CLASS = "getClass";
    const std::string METHOD_GET_METHODS = "getMethods";
    const std::string METHOD_RETURN_TYPE = "getReturnType";
    const std::string METHOD_TOSTRING = "toString";
    const std::string METHOD_GET_NAME = "getName";
    const std::string METHOD_GET_PARAMETER = "getParameterTypes";
    
    jobject clazz;
    Invoke invoke;
    ObjectArray methods;
    
public:
    
    void SetClass(jobject object);
    jobject GetReflectClass();
    
    Reflect(JVMLoader env);
    
    jmethodID GetMethod( std::string className, std::string method, std::string returnType );
    
    std::vector<JavaMethod> GetMethodsDefinition();
    std::vector<JavaMethod> GetMethodDefinition1(std::string& name,
                                                 std::vector<BaseJavaValue*>& values);
    
    
    
    bool ValidateArguments(std::vector<BaseJavaValue*>& values);
    
    // reflects methods.
    std::string GetReturnType(jobject object);
    std::string GetName(std::string className, jobject object);
    std::string ToString(std::string className, jobject object);
    
    std::vector<std::string> GetParameters(jobject object);
};


#endif /* jvm_reflect_hpp */
