//
//  jvm_object.hpp
//  LibJNI
//
//  Created by Cesar Valdez on 04/02/2016.
//  Copyright © 2016 Cesar Valdez. All rights reserved.
//

#ifndef jvm_object_hpp
#define jvm_object_hpp

#include "jvm_reflection.hpp"
#include "jvm_global.hpp"
#include "jvm_argument.hpp"



struct JavaMethod {
    std::string name;
    std::string returnType;
    JavaArguments arguments;
    
    jmethodID methodPTR;
};

class JavaClass {
    
private:
    jobject classObject;
    std::vector<JavaMethod> Methods;
    
    std::string name;
    JVMLoader loader;
    JavaObject javaObject;
    
    
public:
    
    
    JavaClass();
    JavaClass(std::string _className, jobject _classObject, JVMLoader _loader );
    
    static JavaClass CreateObject(JVMLoader loader, std::string className);
    
    
    void SetMethods(jobjectArray methods);
    
    JavaMethod LookupMethod(std::string name);

    template <typename R>
    std::string CallJavaMethod(std::string methodName, std::vector<JavaValue> arg = {}){
        try{
            JavaMethod method = LookupMethod(methodName);
            jstring str;
            if(method.arguments.IsVoid()){
                str = Field::MakeCall<jstring>(loader, classObject , method.methodPTR);
            }else {
                str = Field::MakeCallWithArguments<jstring>(loader,
                                                            classObject ,
                                                            method.methodPTR,
                                                            method.arguments.GetArguments(loader, arg) );
            }
            return Util::GetStringFromJVM(loader.GetJNIEnviorment(), str);
            
        }catch(VMError& error) {
            throw error;
        }
        return "not found";
    };
};

#endif /* jvm_object_hpp */