
include_directories(.)

######

add_executable(parse examples/parse.cpp)
set_target_properties(parse PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin/examples)

