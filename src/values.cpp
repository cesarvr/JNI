//
//  values.cpp
//  LibJNI
//
//  Created by Cesar Valdez on 06/04/2016.
//  Copyright © 2016 Cesar Valdez. All rights reserved.
//

#include "values.hpp"

void Arguments::Set(std::vector<LibJNI::BaseJavaValue *> args) {
    arguments = args;
}


std::vector< std::unique_ptr<jvalue> > Arguments::Get( JEnv env, ArgumentTypeInfo& argsTypeInfo ){
    
    int argNumber = 0;
    std::vector<std::unique_ptr<jvalue>> jniValues;
    jniValues.reserve( argsTypeInfo.GetNumberOfArguments() );
    
    if ( arguments.size() < argsTypeInfo.GetNumberOfArguments() ) {
        std::stringstream errorMessage;
        errorMessage << " Missing parameters expected " << argsTypeInfo.GetNumberOfArguments() << " provided " <<arguments.size();
        throw VMError{ errorMessage.str() };
    }
    
    
    for(auto args: arguments) {
        auto argumentType = argsTypeInfo[argNumber];
        if( args->GetType() == argumentType ) {
            jniValues.push_back( args->GetJavaValue( env ) );
        }else
            throw VMError{ "Type error expected " + argumentType + " type found: " + args->GetType() };
    }
    
    return jniValues;
}

void ArgumentTypeInfo::Set(std::vector<std::string> argsTypeList) {
    listTypes = argsTypeList;
    parametersNumber = listTypes.size();
}

