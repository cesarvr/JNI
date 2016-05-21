//
//  utils.hpp
//  LibJNI
//
//  Created by Cesar Valdez on 17/05/2016.
//  Copyright © 2016 Cesar Valdez. All rights reserved.
//

#ifndef utils_hpp
#define utils_hpp

#include <stdio.h>
#include "jvm_global.h"


namespace Utils {
    
    

    
    template<typename T>
    void chkNull(T&& param){
        assert(param != nullptr);
    }
    
    template<typename T>
    void isNull(T& first) {
        chkNull(first);
    }
    
    template<typename T, typename... Args>
    void isNull(T& first, Args... args) {
        chkNull(first);
        
        isNull(args...);
    }
    
    inline std::string normalizeClassName(std::string&& classname) {
        std::replace(classname.begin(), classname.end(), '.', '/');
        return std::move(classname);
        
    }
    
    template <typename T, typename R>
    std::vector<R> IterateJObjectArray( JEnv env, jobjectArray array, T cb ) {
        
        isNull(array);
        
        std::vector<R> list;
        
        jint count = env->GetArrayLength( array );
        list.reserve(count);
        
        for (int i=0; i < count; i++) {
            
            jobject element = env->GetObjectArrayElement(array, i);
            
            R ret = cb( env, element );
            
            list.push_back(ret);
            
            if(env->ExceptionOccurred())
                env->ExceptionDescribe();
            
            env->DeleteLocalRef( element );
        }
        return list;
    };
    
    
    
    // Iterate a objectArray and apply a function, if the function return true it finish.
    // useful for quick linear search.
    template <typename T>
    void Seek( JEnv env, jobjectArray array, T cb ) {
        
        isNull(array);
        
        jint count = env->GetArrayLength( array );
        
        for (int i=0; i < count; i++) {
            
            jobject element = env->GetObjectArrayElement(array, i);
            
            auto end = cb( env, element );
            
            if(env->ExceptionOccurred())
                env->ExceptionDescribe();
            
            env->DeleteLocalRef( element );
            
            if(end)
                break;
        }
    };

    
}










#endif /* utils_hpp */
