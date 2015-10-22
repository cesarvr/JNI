#include<jni.h>
#include <iostream>

int main(int argc, char** argv) {

  JavaVM *jvm;
  JNIEnv *env;
  JavaVMOption options[1];
  JavaVMInitArgs vm_args;

  vm_args.version = JNI_VERSION_1_6;
  vm_args.nOptions = 1;

  options[0].optionString = "-Djava.class.path=.:./";
  vm_args.options = options; 
 
  long status = JNI_CreateJavaVM(&jvm, (void**)&env, &vm_args);

  if(status != JNI_ERR){

    jclass Clazz = env->FindClass("com/example/Test");

  
    if(Clazz !=0){
      std::cout << "class [Test] loaded \n" << std::endl; 
    }else{
      std::cout << "error loading class \n" << std::endl;
    }


    std::cout << "destroy VM" << std::endl;
    jvm->DestroyJavaVM();
  }else{
    std::cout << "error creating JVM" << std::endl;
  }

  std::cout << "bye!" << std::endl;

}
