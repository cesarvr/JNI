#include "jvm_handler.h"

JVMLoader::JVMLoader() {
    
    SetClassPath("*:."); //use current directory as classpath.
};

void JVMLoader::SetClassPath(std::string _classPath) {

  classPath = _classPath; 
};

bool JVMLoader::isVMReady() {
  
  return isJvmStarted; 
} 

std::string JVMLoader::VMStatus(int status) {
   if(status != JNI_ERR) {
      isJvmStarted = true;
      return "[jvm]: started successfully."; 
   } else {
      isJvmStarted = false;
      return "[jvm]: error starting virtual machine.";
   }
};


  /* use the one loaded */
std::string JVMLoader::Start( CreateJVM create_vm ) {

  JavaVMOption options[1];
  JavaVMInitArgs vm_args;

  vm_args.version = JNI_VERSION_1_6;
  vm_args.nOptions = 1;

  options[0].optionString = (char *)classPath.c_str();
  vm_args.options = options; 

  int status = create_vm(&vm, (void**)&env, &vm_args);

  return VMStatus(status);
};

