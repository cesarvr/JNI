//
//  main.cpp
//  LibJNI
//
//  Created by Cesar Valdez on 05/02/2016.

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


void test_add_int(std::shared_ptr<Object<Server>> jobject, int x, int y) {
    Print("testing_add_int: testing int args");
    
    
    IntValue _x(x);
    IntValue _y(y);
    
    cout << "class name: " << jobject->GetName() << std::endl;
    
    vector<LibJNI::BaseJavaValue *> args{&_x, &_y};
    cout << " add " << jobject->Call<IntValue>("add", move(args)).Get()
    << endl;
}


void test_add_float_overloading(std::shared_ptr<Object<Server>> object, float x,
                                float y) {
    
    Print("test_add_float_overloading: testing floats args");
    
    FloatValue _x(x);
    FloatValue _y(y);
    
    std::cout << "class name: " << object->GetName() << std::endl;
    
    std::vector<LibJNI::BaseJavaValue *> args{&_x, &_y};
    std::cout << " add " << object->Call<FloatValue>("add", move(args)).Get()
    << std::endl;
}

void test_concat(std::shared_ptr<Object<Server>> jobject, std::string x,
                 std::string y) {
    try {
        
        Print("test_concat: testing strings args");
        
        
        StringValue param1(x);
        StringValue param2(y);
        
        std::vector<LibJNI::BaseJavaValue *> args{&param1, &param2};
        
        for (auto param : args) {
            std::cout << "Params Type: " << param->GetType() << std::endl;
            std::cout << "Value: " << param << std::endl;
        }
        
        auto ret = jobject->Call<StringValue>("concat", move(args));
        
        std::cout << "class name: " << jobject->GetName() << std::endl;
        std::cout << "|| concat[string] " << ret.Get() << std::endl;
        
    } catch (VMError &e) {
        std::cout << e.errorMessage << std::endl;
    }
}

void testing_string_allocation(std::shared_ptr<Object<Server>> jobject,
                               std::string h1) {
    
    Print("testing_string_allocation [StringBuffer]: testing threading/sleep in JVM");
    
    for (int x = 0; x < 5; x++) {
        StringValue p1(h1);
        
        std::vector<LibJNI::BaseJavaValue *> args{&p1};
        jobject->Call<ObjectValue>("append", move(args)); // (java) concat + sleep
    }
    
    auto x = jobject->Call<StringValue>("toString", {});
    
    std::cout << "class name: " << jobject->GetName() << std::endl;
    std::cout << "buffer.ToString [string] " << x.Get() << std::endl;
}

void test_array(std::shared_ptr<Object<Server>> object) {
    
    Print("Testing Array");
    
    StringValue p1(
                   "<HTML> <H1> Hello World </H1> <H3> Your subtitle Here!</H3>  </HTML>");
    
    std::vector<LibJNI::BaseJavaValue *> args{&p1};
    
  
    
    auto ret = object->Call<ByteArrayValue>("html2pdf", move(args));
    
    
    auto collection = ret.Get();
    for (auto c : collection) {
        printf("%c", c);
    }
    
    std::cout << "pdf size: " << ret.Get().size() << std::endl;
    
}


void test_int_array(std::shared_ptr<Object<Server>> object) {
    
    Print("Testing INT Array");
    
    auto ret = object->Call<IntArrayValue>("getInts", {});
    
    // auto collection = LibJNI::Collection<signed char>(vm.GetJNIEnviorment(),
    // ret);
    
    auto collection = ret.Get();
    for (auto c : collection) {
        printf("%d", c);
    }
}





void test_sleep_thread(std::shared_ptr<Object<Server>> jobject, std::string h1,
                       int time) {
    
    std::cout << "Sleep Thread Java " << std::endl;
    
    StringValue p1(h1);
    StringValue p2(" ->Attach");
    IntValue p3(time);
    
    std::vector<LibJNI::BaseJavaValue *> args{&p1, &p2, &p3};
    
    auto ret =
    jobject->Call<StringValue>("concatHeavy", move(args)); // (java) concat + sleep
    

    std::cout << "concat heavy sleep [string] " << ret.Get() << std::endl;
}


void ObjectCreationWithStringArgs(JVMLoader &vm, Server server, string msg) {
    StringValue v(msg);
    Object<Server> a(vm, server, "java.lang.String", {&v});
    
    std::cout << "Ctor String-> " << a.Call<StringValue>("toString", {}).Get() << std::endl;
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
        
        
        
        
        //testing_reflection_server(vm);
        
        test_add_int(_pdf, 5000, 5000);
        test_add_int(_pdf, 3, 5);
        test_add_float_overloading(_pdf, 3.55, 1.5);
        test_concat(_pdf, "Kobe", "Bryant");
        testing_string_allocation(_str_buff, "Hellow");
        test_array(_pdf);
        test_int_array(_pdf);
        ObjectCreationWithStringArgs(vm, server, "Hello world");
        
       /*
        std::shared_ptr<Object> _pdf = test_create_pdf_obj(vm);
        
        std::shared_ptr<Object> _str_buff = test_create_str_buffer(vm);

        test_add_int(_pdf, 5000, 5000);
        test_add_int(_pdf, 3, 5);
        test_add_float_overloading(_pdf, 3.55, 1.5);
        test_concat(_pdf, "Kobe", "Bryant");
        testing_string_allocation(_str_buff, "Hellow");
        
        test_int_array(_pdf);
        mthread_test(vm, 5);
        
        for (int x=0; x<10; x++)
            std::cout << "\n";
        
        Timing(ObjectCreation, "creating 100000 objects", vm, 100000);

        
        TestingCtorArgs(vm);
        */
        
        
        
        //Benchmark
        //Timing(testing_method_caching,"Caching", vm);
        //Timing(ObjectCreation, "creating 100000 objects", vm, 100000);
        
        
        
        
    } catch (VMError &error) {
        std::cout << error.errorMessage << std::endl;
    }
    
    exit(EXIT_SUCCESS);
};
