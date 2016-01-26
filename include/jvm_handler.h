#ifndef jvm_loader_h
#define jvm_loader_h

#include <iostream>
#include <jni.h>
#include <nan.h>


using CreateJVM = jint (*) (JavaVM **pvm, void **penv, void *args);

class JVMLoader {
  protected:
      JavaVM *vm = nullptr;
      JNIEnv *env= nullptr;
      std::string classPath; 

     
      bool isJvmStarted = false;  

  public:
      JVMLoader();
      JNIEnv* getJNIEnviorment() { return env; };

      bool isVMReady(); 
      void SetClassPath(std::string _classPath);

      std::string VMStatus(int status);
      std::string Start( CreateJVM create_vm );
};

#endif /* JVM_Loader */
