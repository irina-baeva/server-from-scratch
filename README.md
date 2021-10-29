### Server implementation written in c++ from scratch

### local environment 

install cmake to build the project
```s
brew install cmake
```

run cmake build from ./build folder, it will create two executable binaries: server and client
```s
mkdir build
cd build
cmake ..
cmake --build .
```
then run from terminal
```s
./src/server
```

from other terminal run
```s
./src/client
```
In order to see response from browser run server and open:
```
http://localhost:8081/
```
Unit tests (with Google Test) example (TODO: need more configuration)
from project dir

```
git clone https://github.com/google/googletest/
./tests/test_hello
```

### Resources:
1. <a href="https://medium.com/from-the-scratch/http-server-what-do-you-need-to-know-to-build-a-simple-http-server-from-scratch-d1ef8945e4fa">HTTP Server: Everything you need to know to Build a simple HTTP server from scratch</a>


### Steps:
1. Created simple echo server. All the code in separate files client.cpp and server.cpp. It is possible to run them with commands described above.

2. Extracting server socket into the own class -> serverSocker.h, serverSocket.cpp, main.cpp

To run class implementation of server for now possible only within ./src folder 
```
c++ main.cpp serverSocket.cpp -o main.out
./main.out
```
And run client from step 1.