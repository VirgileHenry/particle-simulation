# CMake version
cmake_minimum_required (VERSION 3.16.3)

# create the project 
# todo : proper name
project(Demo CXX)

# all c++ flags (-O3 is our best choice here)
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_FLAGS "-Wall -O3")

# "create" the header only lib 
include_directories("src")


# tests
include(CTest)
add_executable(vector_test "test/vector.cpp")
add_test(NAME VectorTest COMMAND "./vector_test")

# examples
add_executable(main "examples/test.cpp")

# link the sdl2
find_package(SDL2 REQUIRED)
target_link_libraries(main PRIVATE ${SDL2_LIBRARIES})