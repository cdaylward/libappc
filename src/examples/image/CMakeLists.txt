
include_directories(.)
include_directories(${3RDPARTY_USR}/include)
link_directories(${3RDPARTY_USR}/lib)

######

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/examples/image)

add_executable(check_image check_image.cpp)
target_link_libraries(check_image archive)
