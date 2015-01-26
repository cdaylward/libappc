
include_directories(.)
include_directories(${3RDPARTY_USR}/include)
link_directories(${3RDPARTY_USR}/lib)

######

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/examples/image)

add_executable(check_image check_image.cpp)
target_link_libraries(check_image archive)

add_executable(extract_image extract_image.cpp)
target_link_libraries(extract_image archive)

add_executable(image_file_list image_file_list.cpp)
target_link_libraries(image_file_list archive)

add_executable(show_image_manifest show_image_manifest.cpp)
target_link_libraries(show_image_manifest archive)
