//
//  jvm_object.hpp
//  LibJNI
//
//  Created by Cesar Valdez on 04/02/2016.
//  Copyright © 2016 Cesar Valdez. All rights reserved.
//

#ifndef jvm_object_hpp
#define jvm_object_hpp

#include "jvm_field.hpp"
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

    template <typename R, typename Arg>
    std::string CallJavaMethod(std::string methodName, std::vector<Arg> arg){
        try{
            JavaMethod method = LookupMethod(methodName);
            jstring str;
            if(method.arguments.IsVoid()){
                str = Field::MakeCall<jstring>(loader, classObject , method.methodPTR);
            }else {
                
                
                std::string param = "Bjarne";
                
                jvalue arg[1];
                
                arg[0].l = loader.GetJNIEnviorment()->NewStringUTF(param.c_str());
                
                str = Field::MakeCallWithArguments<jstring, jvalue>(loader, classObject , method.methodPTR, arg );
            }
            return Util::GetStringFromJVM(loader.GetJNIEnviorment(), str);
            
        }catch(VMError& error) {
            throw error;
        }
        return "not found";
    };
};

#endif /* jvm_object_hpp */