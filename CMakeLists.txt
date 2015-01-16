cmake_minimum_required(VERSION 2.8)

set(CMAKE_C_COMPILER /usr/local/bin/gcc-4.9)
set(CMAKE_CXX_COMPILER /usr/local/bin/g++-4.9)
set(CMAKE_CXX_FLAGS "-std=c++11 -g -Wall")
set(CMAKE_CXX_FLAGS_DISTRIBUTION "-O3")

project(libappc CXX)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)

######

add_subdirectory(3rdparty)

#include_directories(${3RDPARTY_USR}/include)
#link_directories(${3RDPARTY_USR}/lib)

add_subdirectory(${GTEST_ROOT})

######

add_subdirectory(src)
include_directories(src)

######

add_subdirectory(tests)

######
