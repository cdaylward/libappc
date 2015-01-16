
enable_testing()

set(TESTS_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin/tests)

include_directories(${GTEST_INCLUDE_DIRS})

###

add_executable(test-schema EXCLUDE_FROM_ALL ${CMAKE_SOURCE_DIR}/tests/unit/appc/schema/test.cpp)
target_link_libraries(test-schema gtest gtest_main)
set_target_properties(test-schema PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${TESTS_OUTPUT_DIRECTORY})

add_test(
  NAME test-schema
  COMMAND ${TESTS_OUTPUT_DIRECTORY}/test-schema
)

add_custom_target(check
  COMMAND ${CMAKE_CTEST_COMMAND} --verbose
  DEPENDS ${TESTS_OUTPUT_DIRECTORY}/test-schema
)

