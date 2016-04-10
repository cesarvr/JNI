 //
//  jvm_argument.hpp
//  LibJNI
//
//  Created by Cesar Valdez on 08/02/2016.
//  Copyright © 2016 Cesar Valdez. All rights reserved.
//

#ifndef jvm_argument_hpp
#define jvm_argument_hpp

#include "jvm_global.hpp"
#include "java_value.hpp"
#include "jvm_handler.h"


const std::string JSTRING = "java.lang.String";
const std::string JBYTE = "byte";
const std::string JINT = "int";
const std::string JBYTE_ARRAY = "[B";


class Arguments {
    std::string typeName;
    
    Arguments(std::string _typeName): typeName(_typeName) {};
    
    void Argument( std::string _type, JavaValue value ) {
    
    };
};



class JavaArguments{
    
private:
    std::vector<std::string> listOfArguments;

    
public:

    /* Accessor */
    bool IsVoid() { return listOfArguments.size() == 0; };

    JavaArguments(std::vector<std::string> arguments);
    JavaArguments(){};
    std::unique_ptr<jvalue[]> GetArguments( JEnv env, std::vector<JavaValue>& values );
};


#endif /* jvm_argument_hpp */
