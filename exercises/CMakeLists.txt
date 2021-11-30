cmake_minimum_required(VERSION 3.20)
project(exercises)

SET(CMAKE_CXX_STANDARD 20)
SET(CMAKE_CXX_FLAGS -pthread)

set(HEADER_FILES)

add_executable(01_exercise 01_exercise/stack/stack-threadsafe.hpp 01_exercise/main.cpp)
add_executable(02_exercise 02_exercise/main.cpp)