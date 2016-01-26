#include <nan.h>
#include <jni.h>

#ifdef __linux__
    #include <dlfcn.h>
#endif

#include "jvm_handler.h"

using JSArgument = const Nan::FunctionCallbackInfo<v8::Value>;


void StartVM( JSArgument& info ) {
    
    
    JVMLoader vm("-Djava.class.path=.:./:./libs/itext-5.5.8/itextpdf-5.5.8.jar:./libs/itext-5.5.8/xmlworker-5.5.8.jar");
    CreateJVM create_jvm;
    
    #ifdef __linux__
    
    
        void* handle = dlopen("./libjvm.so", RTLD_LAZY);
    
        if (!handle) {
            info.GetReturnValue().Set( Nan::New(" [jvm]: fatal! can't load shared library. libjvm.so").ToLocalChecked() );
            return;
        }
    
        create_jvm = (CreateJVM) dlsym(handle, "JNI_CreateJavaVM");
    
        const char *dlsym_error = dlerror();
        if (dlsym_error) {
        
            std::string msg = "Cannot find symbol: JNI_CreateJavaVM || msg: ";
            msg = msg + dlsym_error;
            info.GetReturnValue().Set( Nan::New(msg).ToLocalChecked() );
        
            dlclose(handle);
            return;
        }
    
    #elif __APPLE__
    
        create_jvm = JNI_CreateJavaVM;
    
    #endif
    
    
    std::string jmsg = vm.Start<CreateJVM>(create_jvm);
    info.GetReturnValue().Set( Nan::New(jmsg).ToLocalChecked() );
}

void InitAll(v8::Local<v8::Object> exports) {
    
    exports->Set( Nan::New("start").ToLocalChecked(),
                 Nan::New<v8::FunctionTemplate>(StartVM)->GetFunction());
}

NODE_MODULE(jvm, InitAll)
