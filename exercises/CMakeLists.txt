cmake_minimum_required(VERSION 3.10)
project(exercises)

SET(CMAKE_CXX_STANDARD 20)
SET(CMAKE_CXX_FLAGS -pthread)

add_executable(00_exercise_futures 00_exercise/futures.cpp)
add_executable(00_exercise_package_tasks 00_exercise/package_task.cpp)


add_executable(01_exercise 01_exercise/stack/stack-threadsafe.hpp 01_exercise/main.cpp)

add_executable(02_exercise_cv 02_exercise/conditional_variable.cpp)
add_executable(02_exercise_atomic 02_exercise/atomic.cpp)
add_executable(02_exercise_promises 02_exercise/promises.cpp)
add_executable(02_exercise_v_promises 02_exercise/v_promises.cpp)

add_executable(03_exercise_basic 03_exercise/shared_value_basic.cpp)
add_executable(03_exercise_atomic 03_exercise/shared_value_atomic.cpp)
add_executable(03_exercise_atomic_not_working 03_exercise/shared_value_atomic_not_working.cpp)

configure_file(04_exercise/graph.txt graph.txt COPYONLY)
add_executable(04_exercise_apsp 04_exercise/apsp.cpp)