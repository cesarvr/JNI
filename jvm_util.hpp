//
//  Util.hpp
//  LibJNI
//
//  Created by Cesar Valdez on 05/02/2016.
//  Copyright © 2016 Cesar Valdez. All rights reserved.
//

#ifndef Util_hpp
#define Util_hpp


#include "jvm_global.hpp"

class Util {
    
private:
    static void replaceAll(std::string& str, const std::string& from, const std::string& to) {
        if(from.empty())
            return;
        size_t start_pos = 0;
        while((start_pos = str.find(from, start_pos)) != std::string::npos) {
            str.replace(start_pos, from.length(), to);
            start_pos += to.length(); // In case 'to' contains 'from', like replacing 'x' with 'yx'
        }
    }
    
    static bool replace(std::string& str, const std::string& from, const std::string& to) {
        size_t start_pos = str.find(from);
        if(start_pos == std::string::npos)
            return false;
        str.replace(start_pos, from.length(), to);
        return true;
    }
    
    
public:
    static void Check(void* javaOb, std::string name) {
        if( javaOb == NULL || javaOb == nullptr ){
            std::stringstream msg;
            msg << " : in: " << name;
            throw VMError { msg.str(), 1 };
        }
    };
    
    template <typename JEnv>
    static std::string GetStringFromJVM(JEnv env, jstring AStr) {
        
        const char *str = env->GetStringUTFChars(AStr,NULL);
        env->ReleaseStringUTFChars( AStr,str );
        std::string tmp = str;
        return tmp;
    }
    
    
    template <typename T, typename R>
    static  std::vector<R> IterateJObjectArray( JVMLoader loader, jobjectArray array, T cb ) {
        
        std::vector<R> list;
        jint i;
        
        auto env = loader.GetJNIEnviorment();
        jint count = env->GetArrayLength( array );
        
        for (i=0; i < count; i++) {
            
            jobject element = env->GetObjectArrayElement(array, i);
            
            R ret = cb( loader, element );
            
            list.push_back(ret);
            
            if(env->ExceptionOccurred()) {
                env->ExceptionDescribe();
                break;
            }
            
            env->DeleteLocalRef( element );
        }
        return list;
    };
    
    static std::string ReturnArrayOf(std::string className) { return "()[L" + className + ";";  };
    static std::string ReturnTypeOf(std::string className) { return "()L" + className + ";";  };
    static std::string GetJavaReturnType(std::string className) {
        replace(className, "class ", "");
        replaceAll(className, ".", "/");
        
        return ReturnTypeOf(className);
    };
    
    
};

#endif /* Util_hpp */
