
include(CTest) # Include testing cmake package.

# This is an executable that runs the tests.
add_executable(test_hello test_hello.cpp)

# Link the executable to needed libraries.
target_link_libraries(test_hello gtest gtest_main)