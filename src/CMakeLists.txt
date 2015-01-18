
include_directories(.)

######

add_executable(parse examples/parse.cpp)
set_target_properties(parse PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin/examples)

add_executable(validate_manifest examples/validate_manifest.cpp)
set_target_properties(validate_manifest PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin/examples)
