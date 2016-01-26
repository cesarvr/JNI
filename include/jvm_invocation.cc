#include "jvm_invocation.h"


void JVMInvoke::Init(JVMLoader loader) {
  env = loader.getJNIEnviorment();
};


JavaObject JVMInvoke::CreateObject(std::string qClassName) {
    JavaObject javaObject;
    
    javaObject.member = env->FindClass( qClassName.c_str() );
    javaObject.constructor = env->GetMethodID(javaObject.member, "<init>", "()V");
    javaObject.object = env->NewObject(javaObject.member, javaObject.constructor);
    
    return javaObject;
};