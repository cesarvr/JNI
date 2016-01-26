#ifndef jvm_invocation_h
#define jvm_invocation_h

#include <iostream>
#include <jni.h>
#include <nan.h>

#include "jvm_handler.h"


struct JavaObject{
    jclass member;
    jmethodID constructor = nullptr;
    jobject object = nullptr;
};


class JVMInvoke {
protected:
    JNIEnv *env;

public:
    void Init(JVMLoader loader);
    JavaObject CreateObject( std::string qClassName );
};

#endif /* JVM_Invocation */
