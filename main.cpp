#include<jni.h>
#include <iostream>

class A {};



template <typename JObject, typename Name, typename JEnv>
void check(JObject &jobject, Name &name, JEnv& env) { 
  if(env->ExceptionCheck()){
    env->ExceptionDescribe();
  }

  check(jobject, name);
}
 


template <typename JObject, typename Name>
void check(JObject &jobject, Name &name) {
  auto instName = (name == 0)? "[no name]": name;  

  if(jobject !=0 && jobject != nullptr){
    std::cout << instName <<  " <- found. \n" << std::endl; 
  }else{
    std::cout << "Error: " << instName << " not found !! " << std::endl;
    exit(1);
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

    std::cout << "searching class: "<< route  << std::endl;
    java.clazz = env->FindClass(route); //("com/example/Test");
    check(java.clazz, route, env);

    java.constructor = env->GetMethodID(java.clazz, "<init>", "()V"); 
    check(java.constructor, "Constructor");

    java.object = env->NewObject(java.clazz, java.constructor);
    check(java.object, "JObject");
    java.env = env;
}

template <typename jObject>
std::string getValue(jObject o, jstring str ) {
  
  const char *s = o.env->GetStringUTFChars(str, (jboolean*)0);
  std::string tmp(s);
  return tmp; 
}

template <typename JObject, typename Argument> 
std::string callMethod(JObject object, Argument& argument) {


  auto methodSignature = object.env->GetMethodID(object.clazz, argument, "()Ljava/lang/String;"); 

  check(methodSignature, "callMethod::GetMethodID");
  auto ret = (jstring)object.env->CallObjectMethod(object.object, methodSignature, 0); 
    
  
  return getValue(object, ret);
}
/*
template <typename JObject>
jstring make
*/





template <typename JObject, typename Argument> 
void HTML2PDFService(JObject object, Argument& argument, std::string value) {
  
  jstring jstr = object.env->NewStringUTF( value.c_str() );
  std::cout << "value-> " << value << std::endl;
  std::cout << "Args-> " << argument << std::endl;

  auto methodSignature = object.env->GetMethodID(object.clazz, argument, "(Ljava/lang/String;)V"); 
  check(methodSignature, "callMethodVoid::Method ", object.env );
  object.env->CallObjectMethod(object.object, methodSignature, jstr); 
}

void helloWorld(JNIEnv *env){

 JavaClazz jclazz; 
 loadClass(env, "com/example/Test", jclazz);

 auto ret = callMethod(jclazz, "getSalute");
 std::cout << "[JVM] -> Test.getSalute(): " << ret << std::endl;
}


void pdfItext(JNIEnv *env){

 JavaClazz jclazz; 
 loadClass(env, "pdf/P2HService", jclazz);

 HTML2PDFService(jclazz, "html2pdf", "<html><body> This is my Project </body></html>");

}

int main(int argc, char** argv) {

  JavaVM *jvm;
  JNIEnv *env;
  JavaVMOption options[1];
  JavaVMInitArgs vm_args;

  vm_args.version = JNI_VERSION_1_6;
  vm_args.nOptions = 1;

  char path[] = "-Djava.class.path=.:./:./libs/itext-5.5.8/itextpdf-5.5.8.jar:./libs/itext-5.5.8/xmlworker-5.5.8.jar";  // "-Djava.class.path=.";      // ./libs/itext-5.5.8/itext-5.5.8.jar";  
  options[0].optionString = path; 
  vm_args.options = options; 
 
  long status = JNI_CreateJavaVM(&jvm, (void**)&env, &vm_args);


  if(status != JNI_ERR){

    //helloWorld(env);
    pdfItext(env);


    std::cout << "\n \n \n" << "================ \n" ;
    std::cout << "destroy VM" << std::endl;
    jvm->DestroyJavaVM();
  }else{
    std::cout << "error creating JVM" << std::endl;
  }

  std::cout << "bye!" << std::endl;

}
