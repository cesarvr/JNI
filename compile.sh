g++ -I java/include/ -Ijava/include/linux/ -Ljava/jre/lib/amd64/server/ -ljvm main.c 
export LD_LIBRARY_PATH=$PWD #Tell the loader to search for shared libraries here.
