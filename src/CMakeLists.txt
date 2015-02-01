
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
include_directories(.)

######

add_subdirectory(examples/discovery)
add_subdirectory(examples/schema)
add_subdirectory(examples/image)

