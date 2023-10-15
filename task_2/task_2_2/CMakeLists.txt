cmake_minimum_required(VERSION 3.24.1)
project(task_2_2)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

add_executable(task_2_2 main.cpp)

set(THREADS_PREFER_PTHREAD_FLAG ON)
find_package(Threads REQUIRED)
target_link_libraries(task_2_2 PRIVATE Threads::Threads)

add_custom_target(run_task_2_2 ALL COMMAND task_2_2 > task_2_2.log)
