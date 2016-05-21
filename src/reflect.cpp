//
//  jvm_reflect.cpp
//  LibJNI
//
//  Created by Cesar Valdez on 21/05/2016.
//  Copyright © 2016 Cesar Valdez. All rights reserved.
//

#include "reflect.h"


// Helper classes
std::string ReturnArrayOf(std::string className) {
    return "()[L" + className + ";";
};

std::string ReturnTypeOf(std::string className) {
    return "()L" + className + ";";
};


// Reflect class contains the reflection mechanism, that allow the API to
// instrospect the require Java object
// garther information about the methods the user wants to execute.

Reflect::Reflect(JVMLoader loader) : HandleEnv(loader), invoke(loader) {};

jmethodID Reflect::GetMethod(std::string className, std::string method,
                             std::string returnType) {
    auto env = Env();
    
    auto member = Wrapper(env->functions->FindClass, env, className.c_str());
    return Wrapper(env->functions->GetMethodID, env, member, method.c_str(),
                   returnType.c_str());
}

std::vector<JavaMethod> Reflect::GetMethodsDefinition() {
    
    auto jmethodArray =
    GetMethod(JAVA_CLASS, METHOD_GET_METHODS, ReturnArrayOf(METHOD_CLASS));
    
    methods = invoke.Call<ObjectArray>(clazz, jmethodArray, nullptr);
    
    auto Fn = [this](JEnv env, jobject &object) {
        JavaMethod javaMethod;
        
        javaMethod.SetName(GetName(METHOD_CLASS, object));
        
        javaMethod.SetReturnTypeInfo(GetReturnType(object));
        javaMethod.ArgumentsType().Set(GetParameters(object));
        
        auto methodId =
        Wrapper(env->functions->FromReflectedMethod, Env(), object);
        javaMethod.SetMethodByReference(methodId);
        
        return javaMethod;
    };
    
    return Utils::IterateJObjectArray<decltype(Fn), JavaMethod>(
                                                                Env(), methods.Get(), Fn);
}


bool Reflect::ValidateArguments(std::vector<BaseJavaValue*>& values) {
    
    return false;
}

std::vector<JavaMethod> Reflect::GetMethodDefinition1(std::string& name,
                                                      std::vector<BaseJavaValue*>& values){
    
    auto get_definition = [this, name](JEnv env, jobject object){
        auto _name = GetName(METHOD_CLASS, object);
        
        // if (_name == name)
    };
    std::vector<JavaMethod> v;
    return v;
};


std::string Reflect::GetName(std::string className, jobject object) {
    
    auto _tmp =
    GetMethod(className, METHOD_GET_NAME, ReturnTypeOf(JAVA_STRING_CLASS));
    
    return invoke.Call<StringValue>(object, _tmp, nullptr).Get();
}

std::string Reflect::ToString(std::string className, jobject object) {
    
    auto _tmp =
    GetMethod(METHOD_CLASS, METHOD_TOSTRING, ReturnTypeOf(JAVA_STRING_CLASS));
    
    return invoke.Call<StringValue>(object, _tmp, nullptr).Get();
}

std::string Reflect::GetReturnType(jobject object) {
    
    auto GetReflectionAPI =
    GetMethod(METHOD_CLASS, METHOD_RETURN_TYPE, ReturnTypeOf(JAVA_CLASS));
    
    auto tmp = invoke.Call<ObjectValue>(object, GetReflectionAPI, nullptr);
    
    return GetName(JAVA_CLASS, tmp.Get());
}

std::vector<std::string> Reflect::GetParameters(jobject object) {
    
    auto GetParametersArray =
    GetMethod(METHOD_CLASS, METHOD_GET_PARAMETER, ReturnArrayOf(JAVA_CLASS));
    
    auto array = invoke.Call<ObjectValue>(object, GetParametersArray, nullptr);
    
    auto Fn = [this](JEnv env, jobject &object) {
        return GetName(JAVA_CLASS, object);
    };
    
    return Utils::IterateJObjectArray<decltype(Fn), std::string>(
                                                                 Env(), (jobjectArray)array.Get(), Fn);
}

/* Class < Object > <-- GetClass() */
void Reflect::SetClass(jobject object) {
    
    auto getClass =
    GetMethod(JAVA_CLASS, METHOD_GET_CLASS.c_str(), ReturnTypeOf(JAVA_CLASS));
    clazz = invoke.Call<ObjectValue>(object, getClass, nullptr).Get();
}

jobject Reflect::GetReflectClass() { return clazz; }

