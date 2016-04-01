// JNIEngine.cpp: define el punto de entrada de la aplicación de consola.
//

#include <iostream>
#include "java_class.hpp"


int main()
{

	std::cout << "hello world" << std::endl;

	JavaClass jc;
	JVMLoader vm; 
	try {

		vm.SetClassPath("-Djava.class.path=.:C:/Users/Cesar/Documents/development/JNI-master/libs/itext-5.5.8/itext-xtra-5.5.8.jar:C:/Users/Cesar/Documents/development/JNI-master/libs/itext-5.5.8/xmlworker-5.5.8.jar:C:\\Users\\Cesar\\Documents\\development\\JNI-master");
		vm.Start();

		JavaClass jc = JavaClass::CreateObject(vm, "pdf/P2HService");

		JavaValue val = jc.Method("toString");

		std::cout << "hello: " << val.getStringValue() << std::endl;

	}
	catch (VMError& error) {
		std::cout << "error: " << error.errorMessage << std::endl;
	}

	std::cin.get();
	return 0;
}

 