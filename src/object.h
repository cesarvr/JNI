//
//  jvm_reflect.h
//  LibJNI
//
//  Created by Cesar Valdez on 26/02/2016.
//  Copyright © 2016 Cesar Valdez. All rights reserved.
//

#ifndef jvm_object_hpp
#define jvm_object_hpp


#include <algorithm>
#include "reflect.h"
#include "jvm_global.h"
#include "values.h"
#include "jinvoke.h"
#include "jvm_handler.h"
#include "args.h"

using namespace LibJNI;

// Object encapsulate the java object and a list of methods structures.
class Object: HandleEnv {
private:
  
    std::string name;
    std::vector<JavaMethod> methods;
    
    Reflect reflect;
    Invoke invoke;
    
    ObjectValue object;
    
    void CreateObject(JVMLoader env, std::string classname, std::vector<BaseJavaValue *>& arguments);
    
public:
    Object(JVMLoader env, std::string className);
    Object(JVMLoader env, std::string className, std::vector<BaseJavaValue *>& arguments);
    
    JavaMethod FindFirstMethod( std::string methodName );
    std::vector<JavaMethod> FindMethod( std::string methodName );
    
    JavaMethod LookupMethod(std::string methodName, std::vector<BaseJavaValue *>& arguments );

    std::string GetName(){ return object.GetType(); }
    
    const std::vector<JavaMethod>& GetMembers();
    
    std::string GetClassName();
    

    template <typename T>
    T Call(std::string methodName) {
        std::vector<BaseJavaValue *> empty;
        return Call<T>(methodName, empty);
    }
    
    template <typename T>
    T Call(std::string methodName, std::vector<BaseJavaValue *>& arguments) {
        
        T tmp;
        JEnv jni = Env();
        
        auto method = LookupMethod(methodName, arguments);
        
        auto javaValues = Arguments::GetValues(jni, arguments);
        
        if(!tmp.isCompatible( method.GetReturnTypeInfo() ))
            throw VMError{"Not supported type using " +tmp.GetType()+ " expected " +  method.GetReturnTypeInfo()  };
        
        return invoke.Call<T>(object.Get(), method.GetMethodRef(), (jvalue*)&javaValues[0]);
    }
    
  
};

#endif /* jvm_reflect_hpp */
