add_library(hello hello.cpp)
add_executable(server server.cpp)
target_link_libraries(server hello)