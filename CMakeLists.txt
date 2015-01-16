cmake_minimum_required(VERSION 2.8)
project(libappc CXX)

set(CMAKE_CXX_FLAGS "-std=c++11 -Wall")
set(CMAKE_CXX_FLAGS_DISTRIBUTION "-O3")

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)

######

add_subdirectory(3rdparty)

######

include_directories(src)

######

include(ExternalProject)
ExternalProject_Get_Property(_get_gtest source_dir)
set(GTEST_ROOT ${source_dir})
set(GTEST_INCLUDE_DIRS ${source_dir}/include)

add_subdirectory(${GTEST_ROOT})
add_subdirectory(tests)

######

add_subdirectory(src)

######
