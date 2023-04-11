# CMake version
cmake_minimum_required (VERSION 3.16.3)

# create the project 
# todo : propper name
project(Demo CXX)

# all c++ flags (-O3 is our best choice here)
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_FLAGS "-pg -Wall -O3")

# create target
add_executable(main "src/main.cpp")

# link the sdl2
find_package(SDL2 REQUIRED)
target_link_libraries(main PRIVATE ${SDL2_LIBRARIES})