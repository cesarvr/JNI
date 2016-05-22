//
//  server.hpp
//  LibJNI
//
//  Created by Cesar Valdez on 21/05/2016.
//  Copyright © 2016 Cesar Valdez. All rights reserved.
//

#ifndef server_hpp
#define server_hpp


#include <utility>
#include <map>
#include "reflect.h"

using namespace std;



class Server: public HandleEnv {
public:
    Server(JVMLoader& loader): HandleEnv(loader) {};

    vector<string> GetMethods(ObjectValue object);
    ObjectArray GetMethodsNative(ObjectValue object);

    Method MethodDescription(ObjectValue object, string methodName, vector<BaseJavaValue* >&& args );
    string CreateSignature(string methodName, vector<BaseJavaValue*>& args);
    string GetReturnType();
    
    ~Server();
    
private:
    Method CreateMethod(JEnv& env, string name, jobject object, vector<BaseJavaValue* >& args);
    Method GetMethodFromCache(JEnv &env, string name, vector<LibJNI::BaseJavaValue *> &args);
    
    map<string, ObjectArray> mcache;
    map<string, Method> icache;
};


#endif /* server_hpp */
