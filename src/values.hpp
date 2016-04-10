//
//  values.hpp
//  LibJNI
//
//  Created by Cesar Valdez on 06/04/2016.
//  Copyright © 2016 Cesar Valdez. All rights reserved.
//

#ifndef values_hpp
#define values_hpp

#include "jvm_global.hpp"
#include "jvm_handler.h"

class JavaMethod;
class Arguments;
class ArgumentTypeInfo;



/*
 * Value : BaseValue
 *
 * Class in charge of handling the differents type between JAVA and C++, to implement a new type just need
 *  to make a template-specialization of the type you want to handle.
 *
 */

namespace LibJNI {
    
    class BaseJavaValue {
    public:
        virtual std::string GetType() { throw VMError{"Type not implemented yet."}; return ""; };
        virtual std::unique_ptr<jvalue>& GetJavaValue(JEnv& env) = 0;
    };
    
    template <typename T>
    class Value: public BaseJavaValue {};
    
    
    template<>
    class Value<std::string>: BaseJavaValue {
        std::string value;
        std::unique_ptr<jvalue> javaValue;
        
    public:
        Value(std::string _value):
        value(_value){};
        
        std::string GetType() { return "java.lang.String"; };
        
        std::unique_ptr<jvalue>& GetJavaValue(JEnv& env)  {
            javaValue->l =  env->NewStringUTF( value.c_str() );
            return javaValue;
        };
    };
    
    
    template<>
    class Value<int>: public BaseJavaValue {
        
    private:
        int value;
        std::unique_ptr<jvalue> javaValue;
        
    public:
        
        std::string GetType() { return "int"; };
        
        std::unique_ptr<jvalue>& GetJavaValue(JEnv& loader)  {
            javaValue->i =  value;
            return javaValue;
        };
    };
    
    
    template<>
    class Value<float>: public BaseJavaValue {
        
    private:
        int value;
        std::unique_ptr<jvalue> javaValue;
        
    public:
        std::unique_ptr<jvalue>& GetJavaValue(JEnv& loader)  {
            javaValue->i =  value;
            return javaValue;
        };
    };
}



class Arguments {
    
private:
    std::vector<LibJNI::BaseJavaValue *> arguments;
    
public:
    Arguments() {};
    
    void Set( std::vector<LibJNI::BaseJavaValue *> args );
    
    std::vector< std::unique_ptr<jvalue> > Get(  JEnv env, ArgumentTypeInfo& argsTypeInfo );
};


class ArgumentTypeInfo {
public:
    void Set(std::vector<std::string> arguments);
    std::string operator[](int index) { return listTypes[index]; };
    unsigned long GetNumberOfArguments(){ return parametersNumber; };
    
private:
    std::vector<std::string> listTypes;
    unsigned long parametersNumber = 0;
};


class JavaMethod: HandleEnv {
private:
    std::string name;
    std::string returnTypeInfo;
    jmethodID javaMethodRef;
    ArgumentTypeInfo argsType;
    
public:
    
    void SetName(std::string _name){ name = _name; };
    std::string GetName(){ return name; };
    
    void SetReturnTypeInfo(std::string _returnTypeInfo){ returnTypeInfo = _returnTypeInfo; };
    std::string GetReturnTypeInfo(){ return returnTypeInfo; };
    
    void SetMethodByReference(jmethodID& methodId){ javaMethodRef = methodId; };
    ArgumentTypeInfo& ArgumentsType(){ return argsType; };
    
    JavaMethod(JVMLoader loader): HandleEnv(loader) {};
};




#endif /* values_hpp */
