cmake_minimum_required(VERSION 2.8.9)
project(libappc CXX)

set(CMAKE_CXX_FLAGS "-std=c++11 -Wall -Wextra -pedantic")
set(CMAKE_CXX_FLAGS_DISTRIBUTION "-O3")

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)

######

add_subdirectory(3rdparty)

find_library(LIB_ARCHIVE NAMES archive)
find_library(LIB_CURL NAMES curl)

######

include_directories(src)

######

add_subdirectory(${GTEST_ROOT})
add_subdirectory(tests)

######

add_subdirectory(src)

######
