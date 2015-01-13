
enable_testing()

include_directories(${GTEST_INCLUDE_DIRS})

add_executable(test-schema unit/appc/schema/test_common.cpp)
target_link_libraries(test-schema gtest gtest_main)
add_test(test-schema
  COMMAND ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/test-schema
)

add_custom_target(check
  COMMAND ${CMAKE_CTEST_COMMAND} --verbose
  DEPENDS ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/test-schema
)

