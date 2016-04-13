//
//  values.cpp
//  LibJNI
//
//  Created by Cesar Valdez on 06/04/2016.
//  Copyright © 2016 Cesar Valdez. All rights reserved.
//

#include "values.hpp"

void ArgumentTypeInfo::Set(std::vector<std::string> argsTypeList) {
    listTypes = argsTypeList;
    parametersNumber = listTypes.size();
}


bool CheckSizeEquality(std::vector<LibJNI::BaseJavaValue *> arguments , ArgumentTypeInfo argumentsInfo ) {
    
    if ( arguments.size() == argumentsInfo.GetNumberOfArguments() )
        return true;
    else {
        std::stringstream errorMessage;
        errorMessage << " Missing parameters expected " << argumentsInfo.GetNumberOfArguments() << " provided " <<arguments.size();
        throw VMError{ errorMessage.str() };
    }
};


bool CheckTypeEquality(LibJNI::BaseJavaValue *value, std::string typeName ) {
    
    if( value->GetType() == typeName ){
        return true;
    }else {
        throw VMError{ "Type error expected " + typeName + " type found: " + value->GetType() };
    }
    
}

std::vector<jvalue>
Arguments::Create( JEnv jenv ,
                  ArgumentTypeInfo argumentsInfo ,
                  std::vector<LibJNI::BaseJavaValue *> arguments ) {
    
    int index = 0;
    std::vector<jvalue> values;
    CheckSizeEquality(arguments, argumentsInfo);
    
    for(auto arg: arguments ) {
        CheckTypeEquality(arg, argumentsInfo[index++]);
        values.push_back( arg->GetJavaValue( jenv ) );
    }
    
    return values;
}
