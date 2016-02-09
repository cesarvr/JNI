//
//  jvm_type.hpp
//  LibJNI
//
//  Created by Cesar Valdez on 08/02/2016.
//  Copyright © 2016 Cesar Valdez. All rights reserved.
//

#ifndef jvm_type_hpp
#define jvm_type_hpp

#include "jvm_global.hpp"


enum class JVMType { String, Integer, Float };


class JavaValue {
    JVMType type;
    std::vector<char> bytes;
    
public:
    
    JavaValue(std::string value);
    JavaValue(float value);
    JavaValue(int value);
    
    
    bool IsString(){  return type==JVMType::String; };
    bool IsInteger(){ return type==JVMType::Integer; };
    bool IsFloat(){   return type==JVMType::Float; };
    
    
    int getIntValue();
    
    float getFloatValue();
    
    std::string getStringValue();
    
};







#endif /* jvm_type_hpp */
