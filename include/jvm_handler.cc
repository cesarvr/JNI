#include "jvm_handler.h"

JVMLoader::JVMLoader() {
    
    JVMLoader("*:."); //use current directory as classpath.
};

JVMLoader::JVMLoader(std::string classPath) {

    vm_args.version = JNI_VERSION_1_6;
    vm_args.nOptions = 1;
    options[0].optionString = (char *)classPath.c_str();
};

std::string JVMLoader::VMStatus(int status) {
   if(status != JNI_ERR)
      return "[jvm]: started successfully."; 
   else
      return "[jvm]: error starting virtual machine.";
};



