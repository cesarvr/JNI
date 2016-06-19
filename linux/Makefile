

JNI=-I$(JAVA_HOME)/include/ -I$(JAVA_HOME)/include/linux
INCLUDE=-Isrc/
OPTIONS=-c -stdlib=libstdc++ -std=c++11 -Wc++11-extensions
OBJ=obj/
FLAGS=-ldl -pthread 

#$(OBJ)path.o $(OBJ)args.o $(OBJ)jvm_handler.o $(OBJ)jvm_object.o $(OBJ)main.o


default: test

test: build $(OBJ)path.o $(OBJ)args.o $(OBJ)reflect.o $(OBJ)object.o $(OBJ)jvm_handler.o $(OBJ)server.o $(OBJ)main.o
	clang++ $(FLAGS) -o $@ $(OBJ)path.o $(OBJ)args.o $(OBJ)jvm_handler.o $(OBJ)reflect.o $(OBJ)server.o $(OBJ)object.o $(OBJ)main.o

$(OBJ)main.o: main.cpp  
	clang++ $(OPTIONS) $(JNI) $(INCLUDE) -o $@ $<


$(OBJ)server.o: src/server.cpp src/server.h
	clang++ $(OPTIONS) $(JNI) $(INCLUDE) -o $@ $<

$(OBJ)jvm_handler.o: src/jvm_handler.cpp src/jvm_handler.h
	clang++ $(OPTIONS) $(JNI) $(INCLUDE) -o $@ $<

$(OBJ)object.o: ./src/object.cpp ./src/object.h 
	clang++ $(OPTIONS) $(JNI) $(INCLUDE) -o $@ $<

$(OBJ)reflect.o: ./src/reflect.cpp ./src/reflect.h 
	clang++ $(OPTIONS) $(JNI) $(INCLUDE) -o $@ $<

$(OBJ)args.o: ./src/args.cpp ./src/args.h
	clang++ $(OPTIONS) $(JNI) $(INCLUDE) -o $@ $<

$(OBJ)path.o: src/classpath.cpp src/classpath.h
	clang++ $(OPTIONS) $(JNI) $(INCLUDE) -o $@ $<

build: 
	mkdir -p obj

clean:
	rm obj/*.o
