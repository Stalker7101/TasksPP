cmake_minimum_required(VERSION 3.24.1)
project(task_1_4)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

add_executable(task_1_4 main.cpp)
add_custom_target(run_task_1_4 ALL COMMAND task_1_4 > task_1_4.log)
