#include "jvm_invocation.h"
#include <string>

std::string GetClassName(const std::string& qname ) {
      return qname.substr( qname.find_last_of("/\\") +1 );
};

//void JVMInvoke::WriteError(VMError& error) {
 //  jsArgs.GetReturnValue().Set( Nan::New(error.errorMessage).ToLocalChecked() );
//};

void JVMInvoke::Init(JVMLoader loader) {
  env = loader.getJNIEnviorment();
};


std::string JVMInvoke::CreateObject( JVMLoader loader, std::string qClassName ) {
  
    JavaObject javaObject;

    try { 
      auto env = loader.getJNIEnviorment();
  
      auto member = env->FindClass( qClassName.c_str() );
      Check(member);
    
      auto constructor = env->GetMethodID( member, "<init>", "()V" );
      Check(constructor); 

      auto object = env->NewObject( member, constructor );
      Check(object) 
    
    }
      catch( VMError error ) 
    {
      return error.errorMessage; 
    }


    return qClassName + " Object loaded successfully";
};
