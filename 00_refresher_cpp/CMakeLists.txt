cmake_minimum_required(VERSION 3.20)
project(00_refresher_cpp)

SET(CMAKE_CXX_STANDARD 20)
SET(CMAKE_CXX_FLAGS -pthread)

set(HEADER_FILES 03_threads/lib/range_accumulator.h 03_threads/lib/utility.h 03_threads/lib/vector_accumulator.h)

add_executable(basics_cpp 02_classes/basics.cpp)
add_executable(threads_vector 03_threads/threads-vector.cpp ${HEADER_FILES})
add_executable(threads_functor 03_threads/threads-functors.cpp ${HEADER_FILES})
add_executable(threads_lambda 03_threads/threads-lambda.cpp ${HEADER_FILES})