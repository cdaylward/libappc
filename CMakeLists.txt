cmake_minimum_required(VERSION 2.8.9)
project(libappc CXX)

set(CMAKE_CXX_FLAGS "-std=c++11 -Wall -Wextra -pedantic")
set(CMAKE_CXX_FLAGS_DISTRIBUTION "-O3")

######

if(BIICODE)
	ADD_BIICODE_TARGETS()

	string(REPLACE " " ";" TARGET_CXX_FLAGS              ${CMAKE_CXX_FLAGS})
	string(REPLACE " " ";" TARGET_CXX_FLAGS_DISTRIBUTION ${CMAKE_CXX_FLAGS_DISTRIBUTION})

	target_include_directories(${BII_BLOCK_TARGET} INTERFACE src)
	target_compile_options(${BII_BLOCK_TARGET} INTERFACE ${TARGET_CXX_FLAGS}
		                   $<$<CONFIG:distribution>:${CMAKE_CXX_FLAGS_DISTRIBUTION}>)

	find_library(LIB_ARCHIVE NAMES archive)
	target_link_libraries(${BII_BLOCK_TARGET} INTERFACE ${LIB_ARCHIVE})

	return()
endif()

######

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