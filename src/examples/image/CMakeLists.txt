
include_directories(.)
include_directories(${3RDPARTY_USR}/include)
link_directories(${3RDPARTY_USR}/lib)

######

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/examples/image)

set(LIB_ARCHIVE iconv lzma bz2 z xml2 ${3RDPARTY_USR}/lib/libarchive.a)

add_executable(check_image check_image.cpp)
target_link_libraries(check_image ${LIB_ARCHIVE})

add_executable(extract_image extract_image.cpp)
target_link_libraries(extract_image ${LIB_ARCHIVE})

add_executable(image_file_list image_file_list.cpp)
target_link_libraries(image_file_list ${LIB_ARCHIVE})

add_executable(show_image_manifest show_image_manifest.cpp)
target_link_libraries(show_image_manifest ${LIB_ARCHIVE})
