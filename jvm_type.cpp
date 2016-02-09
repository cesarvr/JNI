//
//  jvm_type.cpp
//  LibJNI
//
//  Created by Cesar Valdez on 08/02/2016.
//  Copyright © 2016 Cesar Valdez. All rights reserved.
//

#include "jvm_type.hpp"


JavaValue::JavaValue(std::string value) {
    //field = value;
    std::vector<char> tmp(value.begin(), value.end());
    bytes = tmp;
    type=JVMType::String;
};

JavaValue::JavaValue(float value) {
    bytes.push_back(*reinterpret_cast<char*>(&value));
    type=JVMType::Float;
};

JavaValue::JavaValue(int value) {
    bytes.push_back(*reinterpret_cast<char*>(&value));
    type=JVMType::Integer;
};


int JavaValue::getIntValue(){
    if( !IsInteger() ) throw VMError{ "Error: incompatible data. (expected Integer)", 01};
    return (int) bytes[0];
};

float JavaValue::getFloatValue(){
    if( !IsFloat() ) throw VMError{ "Error: incompatible data. (expected Float)", 03};
    std::cout << "size ->" << bytes.size() << std::endl;
    return (float) bytes[0];
};

std::string JavaValue::getStringValue(){
    if( !IsString() ) throw VMError{ "Error: incompatible data. (expected String) ", 02};
    std::string tmp(bytes.begin(), bytes.end());
    return tmp;
};

