//
//  benchmark.c
//  LibJNI
//
//  Created by Cesar Valdez on 22/05/2016.
//  Copyright © 2016 Cesar Valdez. All rights reserved.
//

#include "jvm_handler.h"
#include "object.cpp"
#include "object.h"
#include "classpath.h"
#include <stdlib.h>
#include <cassert>
#include <ctime>
#include <memory>
#include "reflect.h"
#include "server.h"

#ifdef __APPLE__

#import <pthread/pthread.h>

std::vector<std::string> _clzpath = {
    "/Users/cvaldez/Desktop/NWR/java/lib",
    "/Users/cvaldez/Desktop/NWR/java/PDFHtml/bin"};

#else

#import <pthread.h>

std::vector<std::string> _clzpath = {"../../demo/lib/", "../../demo/PDFHtml/"};

#endif



void Print(std::string&& str) {
    
    std::cout << "====================" << std::endl;
    std::cout << str << std::endl;
    std::cout << "====================" << std::endl;
    
}

template <typename F, typename... Args>
void Timing(F& fn, std::string msg,  Args... args){
    
    Print(move(msg));
    
    std::clock_t    start;
    
    start = std::clock();
    
    
    fn(args...);
    
    std::cout << msg << std::endl;
    std::cout << "Time: " << (std::clock() - start) / (double)(CLOCKS_PER_SEC / 1000) << " ms" << std::endl;
    
}


std::shared_ptr<Object<Server>> test_create_pdf_obj(JVMLoader &vm, Server& server) {
    try {
        
        Utils::isNull(vm.GetJNIEnviorment());
        assert(vm.GetJNIEnviorment() != nullptr);
        std::shared_ptr<Object<Server>> pdfclazz(new Object<Server>(vm,server, "pdf.P2HService"));
        return pdfclazz;
    } catch (VMError &e) {
        std::cout << e.errorMessage << std::endl;
    }
    
    return nullptr;
}

std::shared_ptr<Object<Server>> test_create_str_buffer(JVMLoader &vm, Server& server) {
    try {
        
        Utils::isNull(vm.GetJNIEnviorment());
        assert(vm.GetJNIEnviorment() != nullptr);
        std::shared_ptr<Object<Server>> pdfclazz(new Object<Server>(vm,server, "java.lang.StringBuffer"));
        return pdfclazz;
    } catch (VMError &e) {
        std::cout << e.errorMessage << std::endl;
    }
    
    return nullptr;
}

void ObjectCreation(JVMLoader &vm, int size){
    Print("Object creation benchmark 100K");
    Server server(vm);
    for(int x=0; x<size; x++)
        std::unique_ptr<Object<Server>>( new Object<Server>(vm,server, "pdf.P2HService") );
}

void testing_reflection_server(JVMLoader& vm){
    Print("Testing Server Class");
    
    Server server(vm);
    
    //for(int x=0; x<size; x++)
    Object<Server> o(vm, server, "java.lang.StringBuffer");
    auto methods = o.MethodsNames();
    
    //for(auto method: methods)
    //  std::cout << method << std::endl;
    
    Object<Server> b(vm, server, "java.lang.StringBuffer");
    auto z = b.MethodsNames();
    //    for(auto method: z)
    //      std::cout << method << std::endl;
    
    Object<Server> a(vm, server, "java.lang.String");
    //auto al = a.MethodsNames();
    
    
    StringValue v{"Hello"};
    
    auto m = server.MethodDescription(b.GetObjectValue(), "append", {&v});
    
    
    auto mz = server.MethodDescription(b.GetObjectValue(), "append", {&v});
    
    cout << "method:" << m.method << std::endl;
    cout << "return type:" << m.returnType << std::endl;
    
    cout << "method:" << mz.method << std::endl;
    cout << "return type:" << mz.returnType << std::endl;
    
    // server.MethodDescription(a.GetObjectValue(), "indexOf", {});
};


void testing_method_caching(JVMLoader& vm){
    Print("Testing StringBuffer/append 45K calls");
    
    Server server(vm);
    
    Object<Server> a(vm, server, "java.lang.StringBuffer");
    StringValue v{"Hello"};
    IntValue x{1};
    FloatValue f{1.0};
    
    for(int i=0; i<15000; i++){
        server.MethodDescription(a.GetObjectValue(), "append", {&v});
        server.MethodDescription(a.GetObjectValue(), "append", {&x});
        server.MethodDescription(a.GetObjectValue(), "append", {&f});
    }
}

int main() {
    
    // mthread_test();
    
    std::cout << "[ Starting Java Virtual Machine ]" << std::endl;
    
    JVMLoader vm;
    
    // std::cout << "-> " << clazz << std::endl;
    
    try {
        
        auto clazz = ClassPath::LocateLibraries(_clzpath, true);
        
        
        vm.SetClassPath(clazz);
        
        vm.Start();
        Server server(vm);
        
        std::shared_ptr<Object<Server>> _pdf = test_create_pdf_obj(vm, server);
        std::shared_ptr<Object<Server>> _str_buff = test_create_str_buffer(vm,server);
        
   
        //Benchmark
        Timing(testing_method_caching,"Caching", vm);
        Timing(ObjectCreation, "creating 100000 objects", vm, 100000);
        
        
        
        
    } catch (VMError &error) {
        std::cout << error.errorMessage << std::endl;
    }
    
    exit(EXIT_SUCCESS);
};
