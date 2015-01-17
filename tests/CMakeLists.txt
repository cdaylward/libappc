
enable_testing()

set(TESTS_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin/tests)

include_directories(${GTEST_INCLUDE_DIRS})

macro(register_test NAME SOURCE)
  add_executable(${NAME} EXCLUDE_FROM_ALL ${CMAKE_SOURCE_DIR}/tests/${SOURCE})
  target_link_libraries(${NAME} gtest gtest_main)
  set_target_properties(${NAME} PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${TESTS_OUTPUT_DIRECTORY})

  add_test(
    NAME ${NAME}
    COMMAND ${TESTS_OUTPUT_DIRECTORY}/${NAME}
  )
  add_dependencies(check ${NAME})
endmacro(register_test)

###

add_custom_target(check
  COMMAND ${CMAKE_CTEST_COMMAND} --verbose
  DEPENDS ${TESTS_OUTPUT_DIRECTORY}/test-schema
)

###

register_test(test-util   unit/appc/util/test.cpp)
register_test(test-schema unit/appc/schema/test.cpp)


