#include<jni.h>
#include <iostream>

template <typename JObject, typename Name>
void check(JObject &jobject, Name &name) {
  auto instName = (name == 0)? "[no name]": name;  

  if(jobject !=0){
    std::cout << instName <<  " <- found. \n" << std::endl; 
  }else{
    std::cout << "Error  \n" << instName << " not found !! " << std::endl;
  }
}

struct JavaClazz {
  jclass clazz = nullptr; 
  jmethodID constructor =nullptr; 
  jobject object = nullptr;
  JNIEnv *env = nullptr;
};

template <typename E, typename Route, typename JavaObject >
void loadClass(E &env, Route route, JavaObject& java) {

    java.clazz = env->FindClass(route); //("com/example/Test");
    check(java.clazz, route);

    java.constructor = env->GetMethodID(java.clazz, "<init>", "()V"); 
    check(java.clazz, "Class");
    check(java.constructor, "Constructor");

    java.object = env->NewObject(java.clazz, java.constructor);
    check(java.object, "JObject");
    java.env = env;
}

template <typename JObject, typename Argument> 
std::string callMethod(JObject object, Argument& argument) {

  auto methodSignature = object.env->GetMethodID(object.clazz, argument, "()Ljava/lang/String"); 
  jobject ret = dynamic_cast<jobject>(object.env->CallObjectMethod(object.clazz, methodSignature, 0)); 
  const char *s = object.env->GetStringUTFChars(dynamic_cast<jstring>(ret), false);
  std::string msg(s);
  return msg;
}



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

   JavaClazz jclazz; 
   loadClass(env, "com/example/Test", jclazz);
   auto ret = callMethod(jclazz, "getSalute");
   std::cout << "JVM Ret:" << ret << std::endl;
   // check(clazz, "Test");


    //auto mid = env->GetStaticMethodID(clazz, "mymain", "()V");


    std::cout << "destroy VM" << std::endl;
    jvm->DestroyJavaVM();
  }else{
    std::cout << "error creating JVM" << std::endl;
  }

  std::cout << "bye!" << std::endl;

}
