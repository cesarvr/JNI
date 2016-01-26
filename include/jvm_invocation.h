#ifndef jvm_invocation_h
#define jvm_invocation_h

#include <iostream>
#include <jni.h>
#include <nan.h>

#include <sstream>
#include <string>




#include "jvm_handler.h"

using JSArgument = const Nan::FunctionCallbackInfo<v8::Value>;

struct VMError {
  std::string errorMessage;
  int errorCode; 
};

struct JavaObject{
    jclass member;
    jmethodID constructor = nullptr;
    jobject object = nullptr;
};



class JVMInvoke {
private:
  void WriteError(VMError& error); 

  template <typename JObject, typename Name>
  void Check(JObject& jobject, Name& name) {
    auto instName = (name == 0)? "[no name]": name;  

    if(jobject ==0 || jobject == nullptr){
      std::stringstream msg;
      msg << "Error: " << instName << "Not found or Null"; 

      throw VMError { msg.str(), 1 }; 
    }
  };


public:
  std::string CreateObject( JVMLoader loader, std::string qClassName );
};

#endif /* JVM_Invocation */
