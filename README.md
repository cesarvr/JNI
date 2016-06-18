## LibJNI
Is as C++ library to encapsulate all complexity of JNI in an object oriented fashion. The objective is to be fast and easy to use. More details in [Documentation]. 


## Requirements       

### MacOSX
It compiles out of the box because it make use of the Java Framework. Problem is the Framework is deprecated so don't know if will continue to work in the future. 


### Windows 
It need to have JDK 1.6 installed this was my target if somebody can help with JDK 8 would be great, Need JAVA_HOME environment variable. 


## Folders

### MacOSX 

the folder macosx/ include the xcode project, compiles with LLVM. 


### Windows 

Is just an VS2015 project solution. 


## Code 

Is in src/ folder. 


## Dependencies 

One of the main goals of this project is to keep Dependencies low, so it should run nicely by its own.


[Documentation]: https://cesarvr.github.io/JNI/docs/jinvoke.html
