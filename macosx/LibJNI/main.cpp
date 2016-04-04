//
//  main.cpp
//  LibJNI
//
//  Created by Cesar Valdez on 05/02/2016.
//  Copyright © 2016 Cesar Valdez. All rights reserved.
//

#include "jvm_handler.h"
#include "jvm_object.hpp"
#include <stdlib.h>
#include <cassert>

#import <pthread/pthread.h>


void test_return_int(std::shared_ptr<Object> jobject ){
    try{
        std::cout << "class name: " <<  jobject->GetClassName();
        std::cout << "  || hashCode [int]: " <<  jobject->Call("hashCode", {}).getIntValue() << std::endl;
        
    }catch(VMError& e){
        std::cout << e.errorMessage << std::endl;
    }
}



template <typename Value1, typename Value2>
int test_ints_param(std::shared_ptr<Object> jobject, Value1 x, Value2 y ){
    
    try{
        JavaValue p1(x);
        JavaValue p2(y);
        
        return jobject->Call("add", {p1, p2}).getIntValue();
    }catch(VMError& e){
        std::cout << e.errorMessage << std::endl;
        throw e;
    }
}


void test_string_param(std::shared_ptr<Object> jobject, std::string x, std::string y ){
    try{
        JavaValue p1(x);
        JavaValue p2(y);
        
        std::cout << "class name: " <<  jobject->GetClassName();
        std::cout << "|| concat[string] " <<  jobject->Call("concat", {p1, p2}).getStringValue() << std::endl;
        
    }catch(VMError& e){
        std::cout << e.errorMessage << std::endl;
    }
}


void test_pdf_byte_return(std::shared_ptr<Object> jobject, std::string html){
    std::cout << "\nrunning itext.jar -> html/pdf [test]" << std::endl;
    
    try{
        JavaValue p1(html);
        
        
        std::cout << "class name: " <<  jobject->GetClassName() << std::endl;
        auto obj = jobject->Call("html2pdf", {p1}).getArrayValue();
        /*
         for(auto b: obj.getArrayValue()){
         std::cout << b << std::endl;
         }*/
        
        std::cout << "[success] size of pdf file: " << obj.size() << '\n' << std::endl;

    }catch(VMError& e){
        std::cout << "[fail]" <<e.errorMessage << std::endl;
    }
}


std::shared_ptr<Object> test_create_string_obj(JVMLoader& vm){
    try {
        std::shared_ptr<Object> pdfclazz(new Object(vm, "java/lang/String"));
        return pdfclazz;
    }catch(VMError& e){
        std::cout << e.errorMessage << std::endl;
    }
    
    return nullptr;
}

std::shared_ptr<Object> test_create_pdf_obj(JVMLoader& vm){
    try {
        std::shared_ptr<Object> pdfclazz(new Object(vm, "pdf/P2HService"));
        return pdfclazz;
    }catch(VMError& e){
        std::cout << e.errorMessage << std::endl;
    }
    
    return nullptr;
}
void test_pdf_search_return_string(std::shared_ptr<Object> jobject, std::string url){
    std::cout << "\n testing pdf search" << std::endl;
    
    try{
        JavaValue p1(url);
        
        
        std::cout << "class name: " <<  jobject->GetClassName() << std::endl;
        auto obj = jobject->Call("Strip", {p1}).getStringValue();

        std::cout << "content pdf: " << obj << std::endl;
        
        
    }catch(VMError& e){
        std::cout << e.errorMessage << std::endl;
    }
}



void test_isnull() {
    try{
        std::cout << "checking null detection" << std::endl;
        int *x = NULL;
        Utils::isNull(x);
    }catch(VMError& error){
        std::cout << error.errorMessage << std::endl;
        
        /* Smart pointer check */
        
        try {
            std::cout << "\nchecking [small_ptr] null detection" << std::endl;
            std::shared_ptr<int *> xptr;
            Utils::isNull(xptr);
        } catch (VMError& error) {
            std::cout << "done :-] "  << error.errorMessage << std::endl;
        }
    }
}

void errExitEN(int err, const char *msg) {
    printf("error: %d  message: %s", err, msg);
    exit(EXIT_FAILURE);
}


void testing_exceptions_handling(JVMLoader vm){
    std::shared_ptr<Object> _pdf = test_create_pdf_obj(vm);
    test_isnull();
    
    try {
        test_ints_param<std::string,int>(_pdf, "hellow",1);
    } catch (VMError& error) {
        std::cout << "\n Exception translated succesfully. [pass]" <<std::endl;
    }

    
}

void testing_sync(JVMLoader vm){
    

    std::shared_ptr<Object> _str = test_create_string_obj(vm);
    std::shared_ptr<Object> _pdf = test_create_pdf_obj(vm);
    
    test_return_int(_str);
    test_return_int(_pdf);
    auto r = test_ints_param(_pdf, 4,5);
    
    assert(r == 4+5);
    
    /*
     sync: benchmark
     */
    time_t t = 0;
    srand(time(&t));
    for (int x = 0; x<40; x++) {
        int rx = rand() % 10+1;
        int ry = rand() % 10+1;

        auto r = test_ints_param<int,int>(_pdf, rx,ry);
        assert(r == (rx+ry));
    }
    
    
    
    test_string_param(_pdf, "Hello ", "World");
    
    /*
     std::string h{"Hella"};
     std::string w{" World"};
     for (int x = 0; x<400000000; x++) {
     test_string_param(_pdf, h, w);
     }*/
    
    std::string html{"<html><body> This is my Project </body></html>"};
    test_pdf_byte_return(_pdf, html);
    
}






/*
 Multi-Thread Tests
 */

static pthread_mutex_t mtx = PTHREAD_MUTEX_INITIALIZER;


static void *
DoTask(void *vm) {
    
    std::string *ss = nullptr;
    int mtx_status = 0;
  
    
    try{
        
        
        
       // printf("DoTask  --> JVM\n");
        JVMLoader ljvm = *reinterpret_cast<JVMLoader*>(vm);
        std::shared_ptr<Object> _pdf = test_create_pdf_obj(ljvm);
        
        JavaValue h{"Hello "};
        JavaValue w{"World"};
        JavaValue t{1000};
        
        auto ret = _pdf->Call("concatHeavy", {h,w,t}); // (java) concat + sleep
        
        
        if ((mtx_status = pthread_mutex_lock(&mtx)) != 0){
            errExitEN(mtx_status, "pthread_mutex_lock");
        }
        
        std::cout << "-> " << ret.getStringValue() << std::endl;
        
        
        if ((mtx_status = pthread_mutex_unlock(&mtx)) != 0){
            errExitEN(mtx_status, "pthread_mutex_unlock");
        }
        
        
        ss = new std::string(ret.getStringValue()); // ret.getStringValue().c_str();
        
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


int main(){
    
    //mthread_test();
    
    std::cout << "[ Starting Java Virtual Machine ]" << std::endl;
  
    JVMLoader vm;
    vm.SetClassPath("-Djava.class.path=.:/Users/cvaldez/Desktop/NWR/java/lib/itext-5.5.8/itextpdf-5.5.8.jar:/Users/cvaldez/Desktop/NWR/java/lib/itext-5.5.8/xmlworker-5.5.8.jar:/Users/cvaldez/Desktop/NWR/java/lib/pdfbox/pdfbox-app-1.8.11.jar:/Users/cvaldez/Desktop/NWR/java/PDFHtml/bin/");
    
    vm.Start();
    
    std::shared_ptr<Object> _pdf = test_create_pdf_obj(vm);
    //test_pdf_search_return_string(_pdf, "/Users/cvaldez/Documents/javascript/pdfs/en.wikipedia.org1459374728.pdf");
    
    testing_sync(vm);
    testing_exceptions_handling(vm);
    mthread_test(vm,5);
    exit(EXIT_SUCCESS);
};
