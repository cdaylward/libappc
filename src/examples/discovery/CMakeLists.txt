
include_directories(.)

######

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/examples/discovery)

add_executable(discover_image discover_image.cpp)
target_link_libraries(discover_image ${LIB_CURL})
