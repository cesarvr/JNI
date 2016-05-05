## LibJNI
Is as C++ library to encapsulate all complexity of JNI in an object oriented fashion. I make heavy use of template as an excuse to learn it and a mean to save some lines but as well I try to not compromise in simplicity. Other advantage is the use of base class polymorphism to simplify the type handling, more details [here]. 


## API 

Documentation.


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


[here]: https://github.com/cesarvr/JNI/tree/master/src
[Documentation]: https://cesarvr.github.io/JNI/docs/jinvoke.html

