#ifndef jvm_loader_h
#define jvm_loader_h

#include <iostream>
#include <jni.h>
#include <nan.h>


using CreateJVM = jint (*) (JavaVM **pvm, void **penv, void *args);

class JVMLoader {
  protected:
      JavaVM *vm;
      JNIEnv *env;

      JavaVMOption options[1];
      JavaVMInitArgs vm_args;
       
  public:
      JVMLoader(std::string classPath);
      JVMLoader();

      std::string VMStatus(int status);
      
      /* use the one loaded */
      template <typename T>
      std::string Start( T jniCreateFn ) {
        int status = jniCreateFn(&vm, (void**)&env, &vm_args);
        return VMStatus(status);
      };
      

     
      JNIEnv* getJNIEnviorment() { return env; };

};

#endif /* JVM_Loader */
