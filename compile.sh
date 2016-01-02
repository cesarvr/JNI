
export LD_LIBRARY_PATH=$PWD #Tell the loader to search for shared libraries here.

g++ -std=c++11 -I java/include/ -Ijava/include/linux/ -Ljava/jre/lib/amd64/server/ -ljvm main.cpp -o hello 
./hello && rm hello && rm *.log
