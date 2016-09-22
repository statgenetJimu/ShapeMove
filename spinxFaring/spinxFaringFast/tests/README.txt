# test for important functions
* test_VectorY.cpp
g++ test_VectorY.cpp -o v ../src/VectorY.cpp -I./Catch/include -I../include -L../src -std=c++1z -I/usr/include/suitesparse -I/usr/local/include -L/usr/local/lib -lspqr -lcholmod -lumfpack -lm
./v

* test_MeshY.cpp
g++ -I./Catch/include -I../include -std=c++1y -I/usr/includ
e/suitesparse -I/usr/local/include -o m ../obj/[^main.o]*.o -L/usr/lib -L/usr/local/lib -lspqr -lcholmod -lumfpack -lm test_MeshY.cpp
./m

注意： まだ全てのテストをパスしていません
