//
//  jvm_global.h
//  LibJNI
//
//  Created by Cesar Valdez on 03/02/2016.
//  Copyright © 2016 Cesar Valdez. All rights reserved.
//

#ifndef jvm_global_h
#define jvm_global_h


#include <iostream>
#ifdef __APPLE__
    #include <JavaVM/jni.h>
#else
    #include <jni.h>
#endif

#include <sstream>
#include <vector>


struct VMError {
    std::string errorMessage;
    int errorCode;
};

struct JavaObject{
    jclass member = nullptr;
    jmethodID constructor = nullptr;
    jobject object = nullptr;
    jobjectArray methods = nullptr;
};



#endif /* jvm_global_h */
