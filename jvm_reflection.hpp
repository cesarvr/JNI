//
//  jvm_field.hpp
//  LibJNI
//
//  Created by Cesar Valdez on 03/02/2016.
//  Copyright © 2016 Cesar Valdez. All rights reserved.
//

#ifndef jvm_field_hpp
#define jvm_field_hpp



#include "jvm_handler.h"
#include "jvm_util.hpp"


const std::string METHOD_CLASS("java/lang/reflect/Method");
const std::string JAVA_CLASS("java/lang/Class");

const std::string STRING_RETURN_TYPE("()Ljava/lang/String;");
const std::string CLASS_MOD_RETURN_TYPE("()Ljava/lang/Class;");
const std::string CLASS_ARRAY("()[Ljava/lang/Class;");

const std::string CLASS_DEFAULT_CTS("<init>");
const std::string VOID_RETURN("()V");


class Field {
    
public:
    
    static jobject ObjectCreate( JVMLoader loader, std::string className ) {
        
        auto env = loader.GetJNIEnviorment();
        
        try {
            
            auto member = env->FindClass( className.c_str() );
            Util::Check(member, className);
            
            auto constructor = env->GetMethodID( member, CLASS_DEFAULT_CTS.c_str() , VOID_RETURN.c_str() );
            Util::Check( constructor, "Constructor" );
            
            auto object = env->NewObject( member, constructor );
            Util::Check( object, "Object" );
        
            return object;
        } catch ( VMError& error ) {
            env->ExceptionDescribe();
            return nullptr;
        }
    };
    
    
    template <typename RET>
    static RET GetterUsingString( JVMLoader loader, std::string className, std::string method, std::string returnType,  jobject& reflectObject ) {
        
        auto env = loader.GetJNIEnviorment();
        
        try {
            auto ClassRef = env->FindClass(className.c_str());
            Util::Check(ClassRef, className);
            
            auto ReflectMethodID = env->GetMethodID( ClassRef, method.c_str(), returnType.c_str() );
            Util::Check( ReflectMethodID, className );
            
            return (RET) env->CallObjectMethod( reflectObject , ReflectMethodID );
            
        }catch(VMError &error){
            env->ExceptionDescribe();
            throw error;
        }
    };
    
    template <typename RET>
    static RET MakeCall(JVMLoader loader, jobject java_object, jmethodID methodID) {
       
        
        
        auto env = loader.GetJNIEnviorment();
        
        try {
            return (RET) env->CallObjectMethod( java_object , methodID );
            
        }catch(VMError &error){
            env->ExceptionDescribe();
            throw error;
        }

    };
    
    template <typename RET>
    static RET MakeCallWithArguments(JVMLoader loader,
                                     jobject java_object,
                                     jmethodID methodID,
                                     std::unique_ptr<jvalue[]> args) {
        
        auto env = loader.GetJNIEnviorment();
        
        try {
            return (RET) env->CallObjectMethodA( java_object , methodID, args.get() );
            
        }catch(VMError &error){
            env->ExceptionDescribe();
            throw error;
        }
    };

    template <typename T>
    static std::string GetName( JVMLoader loader, T& reflectObject ) {
   
        try {
            jstring tmp = GetterUsingString<jstring>(loader,
                                                     METHOD_CLASS,
                                                     "getName",
                                                     STRING_RETURN_TYPE,
                                                     reflectObject);
        
            return Util::GetStringFromJVM(loader.GetJNIEnviorment(), tmp);
        }catch(VMError &error){
            throw error;
        }
    };
    
    template <typename T>
    static std::string GetReturnType( JVMLoader loader, T& reflectObject ) {
        
        try {
            
            jobject objectType = GetterUsingString<jobject>(loader,
                                                            METHOD_CLASS,
                                                            "getReturnType",
                                                            CLASS_MOD_RETURN_TYPE,
                                                            reflectObject);
            
            jstring toStringValue = GetterUsingString<jstring>(loader,
                                                               METHOD_CLASS,
                                                               "toString",
                                                               STRING_RETURN_TYPE,
                                                               objectType);
            
            
            return Util::GetStringFromJVM(loader.GetJNIEnviorment(), toStringValue);
        }catch(VMError &error){
            throw error;
        }
    };
    
    template <typename T>
    static std::vector<std::string> GetParameters( JVMLoader loader, T& reflectObject ) {
        
        try {
            
            jobjectArray objectArrayType = GetterUsingString<jobjectArray>(loader,
                                                                           METHOD_CLASS,
                                                                           "getParameterTypes",
                                                                           CLASS_ARRAY,
                                                                           reflectObject );
            
            
            using M = std::string (*)(JVMLoader loader, jobject& reflectObject);
            auto Fn = [](JVMLoader loader, jobject& object ) {
             
                jstring tmp = GetterUsingString<jstring>(loader,
                                                         JAVA_CLASS,
                                                         "getName",
                                                         STRING_RETURN_TYPE,
                                                         object);
                
                return Util::GetStringFromJVM(loader.GetJNIEnviorment(), tmp);
            };
            
          
            
            auto tmp = Util::IterateJObjectArray<M, std::string>(loader, objectArrayType , Fn );
           
            return tmp;
            
            
        }catch(VMError &error){
            throw error;
        }
    };
};

#endif /* jvm_field_hpp */
