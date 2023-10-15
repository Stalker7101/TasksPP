include(CMakeFindDependencyMacro)
include(FetchContent)

cmake_minimum_required(VERSION 3.24.1)
project(lab1)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# program executable
add_executable(lab1 main.cpp MinDiffSiblings.cpp MinDiffSiblings.hpp)
set(THREADS_PREFER_PTHREAD_FLAG ON)
find_package(Threads REQUIRED)
target_link_libraries(lab1 PRIVATE Threads::Threads)
add_custom_target(run_lab1 ALL COMMAND lab1 > lab1.log)

# tests executable
FetchContent_Declare(
  googletest
  GIT_REPOSITORY https://github.com/google/googletest.git
  GIT_TAG        release-1.11.0
)
FetchContent_MakeAvailable(googletest)
add_library(GTest::GTest INTERFACE IMPORTED)
target_link_libraries(GTest::GTest INTERFACE gtest_main)
add_executable(lab1-test test_main.cpp MinDiffSiblings.cpp MinDiffSiblings.hpp)
target_link_libraries(lab1-test GTest::GTest)
add_test(NAME lab1-test COMMAND lab1-test)
add_custom_target(run_lab1-test ALL COMMAND lab1-test > lab1-test.log)