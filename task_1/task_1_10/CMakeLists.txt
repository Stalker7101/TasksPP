cmake_minimum_required(VERSION 3.24.1)
project(task_1_10)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

add_executable(task_1_10_server main_server.cpp)
add_executable(task_1_10_client main_client.cpp)

add_custom_target(run_task_1_10_server ALL COMMAND task_1_10_server > task_1_10_server.log)
add_custom_target(run_task_1_10_client ALL COMMAND task_1_10_client > task_1_10_client.log)

