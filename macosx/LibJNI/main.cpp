//
//  main.cpp
//  LibJNI
//
//  Created by Cesar Valdez on 05/02/2016.
//  Copyright © 2016 Cesar Valdez. All rights reserved.
//

#include "jvm_handler.h"
#include "jvm_object.hpp"
#include "classpath.hpp"
#include <stdlib.h>
#include <cassert>

#import <pthread/pthread.h>


std::shared_ptr<Object> test_create_pdf_obj(JVMLoader& vm){
    try {
        
        Utils::isNull(vm.GetJNIEnviorment());
        assert(vm.GetJNIEnviorment() != nullptr);
        std::shared_ptr<Object> pdfclazz(new Object(vm, "pdf/P2HService"));
        return pdfclazz;
    }catch(VMError& e){
        std::cout << e.errorMessage << std::endl;
    }
    
    return nullptr;
}


std::shared_ptr<Object> test_create_str_buffer(JVMLoader& vm){
    try {
        
        Utils::isNull(vm.GetJNIEnviorment());
        assert(vm.GetJNIEnviorment() != nullptr);
        std::shared_ptr<Object> pdfclazz(new Object(vm, "java/lang/StringBuffer"));
        return pdfclazz;
    }catch(VMError& e){
        std::cout << e.errorMessage << std::endl;
    }
    
    return nullptr;
}



void testing_string_allocation(std::shared_ptr<Object> jobject, std::string h1) {
    
    std::cout << "Sleep Thread Java " << std::endl;
    
    
    for(int x=0; x<10; x++){
    LibJNI::Value<std::string> p1(h1);
    
    std::vector<LibJNI::BaseJavaValue*> args{&p1};
    jobject->Call<void>("append", args); // (java) concat + sleep
    }
    
    
    
    auto x = jobject->Call<std::string>("toString");
    
    
    std::cout << "class name: " <<  jobject->GetClassName() << std::endl;
    std::cout << "buffer.ToString [string] " << x.Get() << std::endl;
    
}




void errExitEN(int err, const char *msg) {
    printf("error: %d  message: %s", err, msg);
    exit(EXIT_FAILURE);
}



// Multi-Thread Tests


static pthread_mutex_t mtx = PTHREAD_MUTEX_INITIALIZER;


static void *
DoTask(void *vm) {
    
    std::string *ss = nullptr;
    int mtx_status = 0;
    
    
    try{
        
        // printf("DoTask  --> JVM\n");
        JVMLoader ljvm = *reinterpret_cast<JVMLoader*>(vm);
        std::shared_ptr<Object> _pdf = test_create_pdf_obj(ljvm);
        
        LibJNI::Value<std::string> h{"Hello "};
        LibJNI::Value<std::string> w{"World"};
        LibJNI::Value<int> t{1000};
        
        std::vector<LibJNI::BaseJavaValue*> args {&h,&w,&t};
        
        
        auto ret = _pdf->Call<std::string>("concatHeavy", args); // (java) concat + sleep
        
        
        if ((mtx_status = pthread_mutex_lock(&mtx)) != 0){
            errExitEN(mtx_status, "pthread_mutex_lock");
        }
        
        std::cout << "-> " << ret.Get() << std::endl;
        
        
        if ((mtx_status = pthread_mutex_unlock(&mtx)) != 0){
            errExitEN(mtx_status, "pthread_mutex_unlock");
        }
        
        
        ss = new std::string(ret.Get()); // ret.getStringValue().c_str();
        
    }catch(VMError& error){
        std::cout << " error: " << error.errorMessage << std::endl;
    }
    
    
    return (void *) ss;
}






void mthread_test(JVMLoader loader, int workers){
    
    
    pthread_t grid[workers];
    
    
    printf("spawming threads and doing alloc+sleep \n");
    
    for (int i=0; i < workers; i++) {
        int thread_err;
        if ((thread_err = pthread_create(&grid[i], NULL, DoTask, (void *)&loader)) != 0 ) {
            errExitEN(thread_err, "pthread_create");
        }
        
        //pthread_detach(grid[i]);
    }
    
    printf("main thread free to do more work... \n");
    
    
    for (int i=0; i<workers; i++) {
        pthread_join(grid[i],NULL);
    }
    
}

void test_sleep_thread(std::shared_ptr<Object> jobject, std::string h1, int time) {
    
    std::cout << "Sleep Thread Java " << std::endl;
    
    
    LibJNI::Value<std::string> p1(h1);
    LibJNI::Value<std::string> p2(" ->Attach");
    LibJNI::Value<int> p3(time);
    
    std::vector<LibJNI::BaseJavaValue*> args{&p1,&p2,&p3};
    
    jobject->LookupMethod("concatHeavy", args);
    auto ret = jobject->Call<std::string>("concatHeavy", args); // (java) concat + sleep
    
    std::cout << "class name: " <<  jobject->GetClassName() << std::endl;
    std::cout << "concat heavy sleep [string] " << ret.Get() << std::endl;
    
}







void test_add_int(std::shared_ptr<Object> jobject,  int x, int y) {
    
    LibJNI::Value<int> xx(x);
    LibJNI::Value<int> yy(y);
    
    std::cout << "class name: " <<  jobject->GetClassName() << std::endl;
    
    std::vector<LibJNI::BaseJavaValue*> args{&xx, &yy};
    std::cout << " add " << jobject->Call<int>("add", args).Get() << std::endl;
    
}

void test_concat(std::shared_ptr<Object> jobject, std::string x, std::string y ){
    try{
        LibJNI::Value<std::string> param1(x);
        LibJNI::Value<std::string> param2(y);
        
        std::vector<LibJNI::BaseJavaValue*> args{&param1, &param2};
        
        for( auto param : args ){
            std::cout << "Params Type: " << param->GetType() << std::endl;
            std::cout << "Value: " << param << std::endl;
        }
        
        auto ret =  jobject->Call<std::string>("concat", args);
        
        std::cout << "class name: " <<  jobject->GetClassName() << std::endl;
        std::cout << "|| concat[string] " << ret.Get() << std::endl;
        
    }catch(VMError& e){
        std::cout << e.errorMessage << std::endl;
    }
}



/* Testing supported values  */
void testing_values(std::shared_ptr<Object> jobject, std::string x, std::string y ){
    try{
        
        
        LibJNI::Value<std::string> param1(x);
        LibJNI::Value<std::string> param2(y);
        
        LibJNI::Value<int> param3(3);
        LibJNI::Value<int> param4(5);
        
        
        LibJNI::Value<float> param6(3.0f);
        LibJNI::Value<float> param7(5.0f);
        
        
        std::vector<LibJNI::BaseJavaValue*> args{&param1, &param2, &param3, &param4, &param6, &param7};
        
        
        for(auto arg: args){
            std::cout << "Param Type: " << arg->GetType() << std::endl;
        }
        
        
        
    }catch(VMError& e){
        std::cout << e.errorMessage << std::endl;
    }
}




void test_return_type(std::shared_ptr<Object> jobject, std::string h1, int time) {
    
    std::cout << "Sleep Thread Java " << std::endl;
    
    
    LibJNI::Value<std::string> p1(h1);
    LibJNI::Value<std::string> p2(" ->Attach");
    LibJNI::Value<int> p3(time);
    
    std::vector<LibJNI::BaseJavaValue*> args{&p3};
    
    auto method = jobject->LookupMethod("fff", args);
    
    std::cout << "Method-> " << method.GetReturnTypeInfo() << std::endl;

}


int main(){
    
    //mthread_test();
    
    std::cout << "[ Starting Java Virtual Machine ]" << std::endl;
    
    JVMLoader vm;
    
    auto clazz = ClassPath::LocateLibraries({ "/Users/cvaldez/Desktop/NWR/java/lib",
        "/Users/cvaldez/Desktop/NWR/java/PDFHtml/bin"}, true);
    
    
    std::cout << "-> " << clazz << std::endl;
    
    
    vm.SetClassPath(clazz);

    vm.Start();
    
    std::shared_ptr<Object> _pdf = test_create_pdf_obj(vm);
    
    std::shared_ptr<Object> _str_buff = test_create_str_buffer(vm);
    
    try {
        
        test_concat(_pdf, "Kobe", "Bryant");
        test_add_int(_pdf, 3,5);
        testing_string_allocation(_str_buff, "Hellow");
        test_sleep_thread(_pdf, "Sleep thread", 5000); // JVM put a thread to sleep.
        
        mthread_test(vm, 400); // Native-side create 400 Threads and attach JVM to each Thread.
        
        
        testing_values(_pdf, "Hello", "World");
        test_return_type(_pdf, "hello", 1000);
    } catch (VMError& error) {
        std::cout << error.errorMessage << std::endl;
    }
    
    exit(EXIT_SUCCESS);
};
