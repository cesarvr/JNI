#include <nan.h>
#include <jni.h>
#include <dlfcn.h>


JNIEnv *env;
JavaVM *javaVM;

typedef jint ( *CreateJVM_t ) (JavaVM **pvm, void **penv, void *args); 

struct Maquina {
  bool status; 
}JMaquina; 


struct JavaClazz {
  jclass clazz = nullptr; 
  jmethodID constructor =nullptr; 
  jobject object = nullptr;
  JNIEnv *env = nullptr;
  bool loaded = false;
};


template <typename JObject, typename Name, typename JEnv>
bool check(JObject &jobject, Name &name, JEnv& env) { 
  if(env->ExceptionCheck()){
    env->ExceptionDescribe();
    return false;
  }

  return check(jobject, name);
}
 
template <typename JObject, typename Name>
bool check(JObject &jobject, Name &name) {
  auto instName = (name == 0)? "[no name]": name;  

  if(jobject !=0 && jobject != nullptr){
    return true;
  }else{
    return false;
  }
}



template <typename E, typename Route, typename JavaObject >
void loadClass(E &env, Route route, JavaObject& java) {

    java.clazz = env->FindClass(route); //("com/example/Test");
    java.loaded = check(java.clazz, route, env);
    
    java.constructor = env->GetMethodID(java.clazz, "<init>", "()V"); 
    java.loaded = check(java.constructor, route, env);

    java.object = env->NewObject(java.clazz, java.constructor);
    java.loaded = check(java.object, route, env);
    java.env = env;
}


template <typename JObject, typename Argument> 
void HTML2PDFService(JObject object, Argument& argument, std::string value) {
  
  jstring jstr = object.env->NewStringUTF( value.c_str() );

  auto methodSignature = object.env->GetMethodID(object.clazz, argument, "(Ljava/lang/String;)V"); 
  object.loaded = check(methodSignature, "callMethodVoid::Method ", object.env );

  object.env->CallObjectMethod(object.object, methodSignature, jstr); 
}

void helloWorld(const Nan::FunctionCallbackInfo<v8::Value>& info){

  JavaClazz jclazz; 
  loadClass(env, "pdf/P2HService", jclazz);
  v8::Local<v8::Object> obj = Nan::New<v8::Object>();

  if(jclazz.loaded) {
    HTML2PDFService(jclazz, "html2pdf", "<html><body> This is my Project </body></html>");
    if(jclazz.clazz) {
      obj->Set(Nan::New("class").ToLocalChecked(),Nan::New("pdf/P2HService").ToLocalChecked());
    }

  }else {
    info.GetReturnValue().Set(Nan::New("Error loading class").ToLocalChecked());
  }

  info.GetReturnValue().Set(obj);

 //auto ret = callMethod(jclazz, "getSalute");
 //std::cout << "[JVM] -> Test.getSalute(): " << ret << std::endl;
}



void ShutDownJVM(const Nan::FunctionCallbackInfo<v8::Value>& info) { 
  v8::Local<v8::Object> obj = Nan::New<v8::Object>();
  javaVM->DestroyJavaVM();
  obj->Set(Nan::New("status").ToLocalChecked(), Nan::New("JVM:Finished").ToLocalChecked());

  info.GetReturnValue().Set(obj);
}

void CreateObject(const Nan::FunctionCallbackInfo<v8::Value>& info) {
    v8::Local<v8::Object> obj = Nan::New<v8::Object>();
    obj->Set(Nan::New("msg").ToLocalChecked(), info[0]->ToString());

    info.GetReturnValue().Set(obj);
}


void StartJVM(const Nan::FunctionCallbackInfo<v8::Value>& info) {

    v8::Local<v8::Object> obj = Nan::New<v8::Object>();
/*
    JavaVMOption options[1];
    JavaVMInitArgs vm_args;

    vm_args.version = JNI_VERSION_1_6;
    vm_args.nOptions = 1;
    char path[] = "-Djava.class.path=.:./:./libs/itext-5.5.8/itextpdf-5.5.8.jar:./libs/itext-5.5.8/xmlworker-5.5.8.jar";    
    options[0].optionString = path; 
    vm_args.options = options; 
 
    int status = JNI_CreateJavaVM(&javaVM, (void**)&env, &vm_args);

    if(status != JNI_ERR){
      
      JMaquina.status = true;

      //helloWorld(env);
      //pdfItext(env);
      obj->Set(Nan::New("status").ToLocalChecked(), Nan::New("JVM:Started").ToLocalChecked());
    }else{
      obj->Set(Nan::New("status").ToLocalChecked(), Nan::New("JVM:Error").ToLocalChecked());

      JMaquina.status = false;
    }
*/
    void* handle = dlopen("./libjvm.so", RTLD_LAZY);

    if (!handle) {
      obj->Set( Nan::New("status").ToLocalChecked(), 
                Nan::New("can't load shared library. libjvm.so :[ ").ToLocalChecked() );
    }else{
      obj->Set( Nan::New("lib").ToLocalChecked(), 
                Nan::New("Dynamic Lib loaded").ToLocalChecked() );
    }
    
    CreateJVM_t zjvm =  (CreateJVM_t) dlsym(handle, "JNI_CreateJavaVM"); 
    
    const char *dlsym_error = dlerror();
    if (dlsym_error) {
        std::string msg = "Cannot find symbol: JNI_CreateJavaVM  | "; 
        msg = msg + dlsym_error; 
    
        obj->Set( Nan::New("status").ToLocalChecked(), 
                Nan::New(msg).ToLocalChecked() );

        dlclose(handle);
    }else {
        obj->Set( Nan::New("status").ToLocalChecked(), 
                  Nan::New("Method Loaded successfully.").ToLocalChecked() );
    }



    JavaVMOption options[1];
    JavaVMInitArgs vm_args;

    vm_args.version = JNI_VERSION_1_6;
    vm_args.nOptions = 1;
    char path[] = "-Djava.class.path=.:./:./libs/itext-5.5.8/itextpdf-5.5.8.jar:./libs/itext-5.5.8/xmlworker-5.5.8.jar";    
    options[0].optionString = path; 
    vm_args.options = options; 
 
    int status = zjvm(&javaVM, (void**)&env, &vm_args);

    if(status != JNI_ERR){
      
      JMaquina.status = true;

      //helloWorld(env);
      //pdfItext(env);
      obj->Set(Nan::New("jvm").ToLocalChecked(), Nan::New("JVM:Started").ToLocalChecked());
    }else{
      obj->Set(Nan::New("jvm").ToLocalChecked(), Nan::New("JVM:Error").ToLocalChecked());

      JMaquina.status = false;
    }





    info.GetReturnValue().Set(obj);
}

void Init(v8::Local<v8::Object> exports) {

    exports->Set(Nan::New("start").ToLocalChecked(),
                       Nan::New<v8::FunctionTemplate>(StartJVM)->GetFunction());

    exports->Set(Nan::New("stop").ToLocalChecked(),
                       Nan::New<v8::FunctionTemplate>(ShutDownJVM)->GetFunction());

    exports->Set(Nan::New("newPDF").ToLocalChecked(),
                       Nan::New<v8::FunctionTemplate>(helloWorld)->GetFunction());
}

NODE_MODULE(hello, Init)
