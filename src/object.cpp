//
//  jvm_reflect.cpp
//  LibJNI
//
//  Created by Cesar Valdez on 26/02/2016.
//  Copyright © 2016 Cesar Valdez. All rights reserved.
//

#include "object.h"


// Represent an object in java.
Object::Object(JVMLoader loader, std::string className)
: HandleEnv(loader), reflect(loader), invoke(loader), object(className) {
    
    std::vector<BaseJavaValue*> _empty;
    
    CreateObject(loader, className, _empty);
};

Object::Object(JVMLoader loader, std::string className, std::vector<BaseJavaValue *>& arguments):
HandleEnv(loader), reflect(loader), invoke(loader),object(className) {
    
    CreateObject(loader, className, arguments);
};



void Object::CreateObject(JVMLoader env, std::string className, std::vector<BaseJavaValue *>& arguments) {
    
    jmethodID constructor;
    
    auto name = object.GetType();
    auto jni = Env();
    
    auto member = Wrapper(jni->functions->FindClass, jni, name.c_str());
    
    if(arguments.empty())
        constructor = Wrapper(jni->functions->GetMethodID, jni, member,
                               CLASS_DEFAULT_CTS.c_str(), VOID_RETURN.c_str());
    else
        constructor = Wrapper(jni->functions->GetMethodID, jni, member,
                              CLASS_DEFAULT_CTS.c_str(), Arguments::GetConstructorSignature(jni,arguments).c_str());

    
    auto javaValues = Arguments::GetValues(jni, arguments);
    auto tmp = Wrapper( jni->functions->NewObjectA, jni, member, constructor, (jvalue*)&javaValues[0]);
    
    object.Set(tmp);
}



// Find the first method by a given name.
JavaMethod Object::FindFirstMethod(std::string methodName) {
    auto objectMembers = GetMembers();
    
    for (auto &method : objectMembers) {
        if (method.GetName() == methodName) {
            return method;
        }
    }
    
    throw VMError({"Method not found: " + methodName});
}

// Find the first method by a given name.
std::vector<JavaMethod> Object::FindMethod(std::string methodName) {
    
    std::vector<JavaMethod> methodsList;
    auto objectMembers = GetMembers();
    
    for (auto &method : objectMembers) {
        if (method.GetName() == methodName)
            methodsList.push_back(method);
    }
    
    if (methodsList.size() == 0)
        throw VMError({"Method not found: " + methodName});
    
    return methodsList;
}

// Lazy loading the methods.
const std::vector<JavaMethod> &Object::GetMembers() {
    if (methods.empty()) {
        reflect.SetClass(object.Get());
        return methods = reflect.GetMethodsDefinition();
    }
    return methods;
};

std::string Object::GetClassName() { return name; };

//  Find the first method given name and a number of type/values.
//   It will try to find the right match, if he fails returns an exception.
//   if succeed then it return a iter pointer to the location of the JavaMethod
//   object.

JavaMethod
Object::LookupMethod(std::string methodName,
                     std::vector<LibJNI::BaseJavaValue *> &arguments) {
    
    auto methodCollection = FindMethod(methodName);
    
    auto validateArguments = [arguments](JavaMethod &method) {
        int index = 0;
        
        if (arguments.size() != method.ArgumentsType().GetNumberOfArguments())
            return false;
        
        for (auto arg : arguments)
            if (arg->GetType() != method.ArgumentsType()[index++])
                return false;
        
        return true;
    };
    
    auto iter = std::find_if(methodCollection.begin(), methodCollection.end(),
                             validateArguments);
    
    if (iter == methodCollection.end())
        throw VMError(
                      {"Method found but arguments are incorrect for " + methodName});
    
    return *iter;
};

