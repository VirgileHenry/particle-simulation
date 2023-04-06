cmake_minimum_required (VERSION 3.16.3)
project(Demo CXX)
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_FLAGS "-pg -Wall -O3")
# Commentaire
add_executable(main "src/main.cpp")