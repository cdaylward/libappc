
include_directories(.)

######

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/examples/schema)

add_executable(parse parse.cpp)
add_executable(validate validate.cpp)
add_executable(generate_minimal_crm generate_minimal_crm.cpp)
add_executable(generate_complete_crm generate_complete_crm.cpp)
