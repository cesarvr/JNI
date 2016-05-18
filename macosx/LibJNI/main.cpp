//
//  main.cpp
//  LibJNI
//
//  Created by Cesar Valdez on 05/02/2016.

#include "jvm_handler.h"
#include "jvm_object.h"
#include "classpath.h"
#include <stdlib.h>
#include <cassert>
#include <ctime>
#include <memory>

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

std::shared_ptr<Object> test_create_pdf_obj(JVMLoader &vm) {
    try {
        
        Utils::isNull(vm.GetJNIEnviorment());
        assert(vm.GetJNIEnviorment() != nullptr);
        std::shared_ptr<Object> pdfclazz(new Object(vm, "pdf.P2HService"));
        return pdfclazz;
    } catch (VMError &e) {
        std::cout << e.errorMessage << std::endl;
    }
    
    return nullptr;
}

std::shared_ptr<Object> test_create_str_buffer(JVMLoader &vm) {
    try {
        
        Utils::isNull(vm.GetJNIEnviorment());
        assert(vm.GetJNIEnviorment() != nullptr);
        std::shared_ptr<Object> pdfclazz(new Object(vm, "java.lang.StringBuffer"));
        return pdfclazz;
    } catch (VMError &e) {
        std::cout << e.errorMessage << std::endl;
    }
    
    return nullptr;
}

void testing_string_allocation(std::shared_ptr<Object> jobject,
                               std::string h1) {
    
    Print("testing_string_allocation [StringBuffer]: testing threading/sleep in JVM");
    
    for (int x = 0; x < 5; x++) {
        StringValue p1(h1);
        
        std::vector<LibJNI::BaseJavaValue *> args{&p1};
        jobject->Call<ObjectValue>("append", args); // (java) concat + sleep
    }
    
    auto x = jobject->Call<StringValue>("toString");
    
    std::cout << "class name: " << jobject->GetClassName() << std::endl;
    std::cout << "buffer.ToString [string] " << x.Get() << std::endl;
}

// Multi-Thread Tests

void errExitEN(int err, const char *msg) {
    printf("error: %d  message: %s", err, msg);
    exit(EXIT_FAILURE);
}

static pthread_mutex_t mtx = PTHREAD_MUTEX_INITIALIZER;

static void *DoTask(void *vm) {
    
    std::string *ss = nullptr;
    int mtx_status = 0;
    
    try {
        
        // printf("DoTask  --> JVM\n");
        JVMLoader ljvm = *reinterpret_cast<JVMLoader *>(vm);
        std::shared_ptr<Object> _pdf = test_create_pdf_obj(ljvm);
        
        StringValue h{"Hello "};
        StringValue w{"World"};
        IntValue t{1000};
        
        std::vector<LibJNI::BaseJavaValue *> args{&h, &w, &t};
        
        auto ret =
        _pdf->Call<StringValue>("concatHeavy", args); // (java) concat + sleep
        
        if ((mtx_status = pthread_mutex_lock(&mtx)) != 0) {
            errExitEN(mtx_status, "pthread_mutex_lock");
        }
        
        std::cout << "-> " << ret.Get() << std::endl;
        
        if ((mtx_status = pthread_mutex_unlock(&mtx)) != 0) {
            errExitEN(mtx_status, "pthread_mutex_unlock");
        }
        
        ss = new std::string(ret.Get()); // ret.getStringValue().c_str();
        
    } catch (VMError &error) {
        std::cout << " error: " << error.errorMessage << std::endl;
    }
    
    return (void *)ss;
}

void mthread_test(JVMLoader loader, int workers) {
    
    pthread_t grid[workers];
    
    Print("Testing Native Threads + JVM/Threading/Sleep");
    
    for (int i = 0; i < workers; i++) {
        int thread_err;
        if ((thread_err =
             pthread_create(&grid[i], NULL, DoTask, (void *)&loader)) != 0) {
            errExitEN(thread_err, "pthread_create");
        }
        
        // pthread_detach(grid[i]);
    }
    
    printf("main thread free to do more work... \n");
    
    for (int i = 0; i < workers; i++) {
        pthread_join(grid[i], NULL);
    }
}

void test_sleep_thread(std::shared_ptr<Object> jobject, std::string h1,
                       int time) {
    
    std::cout << "Sleep Thread Java " << std::endl;
    
    StringValue p1(h1);
    StringValue p2(" ->Attach");
    IntValue p3(time);
    
    std::vector<LibJNI::BaseJavaValue *> args{&p1, &p2, &p3};
    
    jobject->LookupMethod("concatHeavy", args);
    auto ret =
    jobject->Call<StringValue>("concatHeavy", args); // (java) concat + sleep
    
    std::cout << "class name: " << jobject->GetClassName() << std::endl;
    std::cout << "concat heavy sleep [string] " << ret.Get() << std::endl;
}

void test_concat(std::shared_ptr<Object> jobject, std::string x,
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
        
        auto ret = jobject->Call<StringValue>("concat", args);
        
        std::cout << "class name: " << jobject->GetClassName() << std::endl;
        std::cout << "|| concat[string] " << ret.Get() << std::endl;
        
    } catch (VMError &e) {
        std::cout << e.errorMessage << std::endl;
    }
}

void test_add_int(std::shared_ptr<Object> jobject, int x, int y) {
    Print("testing_add_int: testing int args");
    
    
    IntValue _x(x);
    IntValue _y(y);
    
    std::cout << "class name: " << jobject->GetClassName() << std::endl;
    
    std::vector<LibJNI::BaseJavaValue *> args{&_x, &_y};
    std::cout << " add " << jobject->Call<IntValue>("add", args).Get()
    << std::endl;
}

void test_add_float_overloading(std::shared_ptr<Object> object, float x,
                                float y) {
    
    Print("test_add_float_overloading: testing floats args");
    
    FloatValue _x(x);
    FloatValue _y(y);
    
    std::cout << "class name: " << object->GetClassName() << std::endl;
    
    std::vector<LibJNI::BaseJavaValue *> args{&_x, &_y};
    std::cout << " add " << object->Call<FloatValue>("add", args).Get()
    << std::endl;
    
    std::vector<LibJNI::BaseJavaValue *> empty;
    auto method = object->LookupMethod("getInts", empty);
    std::cout << "Method-> " << method.GetReturnTypeInfo() << std::endl;
}

void test_array(std::shared_ptr<Object> object) {
    
    Print("Testing Array");
    
    StringValue p1(
                   "<HTML> <H1> Hello World </H1> <H3> Your subtitle Here!</H3>  </HTML>");
    
    std::vector<LibJNI::BaseJavaValue *> args{&p1};
    
    auto method = object->LookupMethod("html2pdf", args);
    
    std::cout << "Method-> " << method.GetReturnTypeInfo() << std::endl;
    
    auto ret = object->Call<ByteArrayValue>("html2pdf", args);
    
    /*
     auto collection = ret.Get();
     for (auto c : collection) {
     printf("%c", c);
     }*/
    
    std::cout << "pdf size: " << ret.Get().size() << std::endl;
    
}

void test_int_array(std::shared_ptr<Object> object) {
    
    Print("Testing INT Array");
    
    auto ret = object->Call<IntArrayValue>("getInts");
    
    // auto collection = LibJNI::Collection<signed char>(vm.GetJNIEnviorment(),
    // ret);
    
    auto collection = ret.Get();
    for (auto c : collection) {
        printf("%d", c);
    }
}

void test_int_array_ex(std::shared_ptr<Object> object) {
    
    std::cout << "Testing INT Array" << std::endl;
    
    auto ret = object->Call<IntArrayValue>("getInts");
    
    // auto collection = LibJNI::Collection<signed char>(vm.GetJNIEnviorment(),
    // ret);
    
    auto collection = ret.Get();
    for (auto c : collection) {
        printf("%d", c);
    }
}

template <typename F, typename... Args>
void Timing(F& fn, std::string msg,  Args... args){
    
    std::clock_t    start;
    
    start = std::clock();
    
    
    fn(args...);
    
    std::cout << msg << std::endl;
    std::cout << "Time: " << (std::clock() - start) / (double)(CLOCKS_PER_SEC / 1000) << " ms" << std::endl;
    
}

void ObjectCreation(JVMLoader &vm, int size){
    
    for(int x=0; x<size; x++)
        std::unique_ptr<Object>( new Object(vm, "pdf.P2HService") );
}

void TestingCtorArgs(JVMLoader &vm) {
    
    StringValue v("Hello World");
    
   auto signature = Arguments::GetConstructorSignature(vm.GetJNIEnviorment(), {&v});
    assert(signature == "(Ljava/lang/String;)V");
    std::cout << signature << std::endl;
    std::vector<LibJNI::BaseJavaValue *> args{&v};
    auto o = Object(vm, "java.lang.String", args);
    std::cout << o.Call<StringValue>("toString").Get() << std::endl;
    
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
        
        std::shared_ptr<Object> _pdf = test_create_pdf_obj(vm);
        
        std::shared_ptr<Object> _str_buff = test_create_str_buffer(vm);

        test_add_int(_pdf, 5000, 5000);
        test_add_int(_pdf, 3, 5);
        test_add_float_overloading(_pdf, 3.55, 1.5);
        test_concat(_pdf, "Kobe", "Bryant");
        testing_string_allocation(_str_buff, "Hellow");
        test_array(_pdf);
        test_int_array(_pdf);
        mthread_test(vm, 5);
        
        for (int x=0; x<10; x++)
            std::cout << "\n";
        
        Timing(ObjectCreation, "creating 100000 objects", vm, 100000);

        
        TestingCtorArgs(vm);
        
        
        
    } catch (VMError &error) {
        std::cout << error.errorMessage << std::endl;
    }
    
    exit(EXIT_SUCCESS);
};
