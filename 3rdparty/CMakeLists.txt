cmake_minimum_required(VERSION 2.8.9)
include(ExternalProject)

set(3RDPARTY_USR ${CMAKE_CURRENT_LIST_DIR}/usr)
set(3RDPARTY_USR ${CMAKE_CURRENT_LIST_DIR}/usr PARENT_SCOPE)

set(GTEST_DL_DIR gtest-1.7.0)
ExternalProject_add(_get_gtest
  PREFIX ${GTEST_DL_DIR}
  TMP_DIR ./tmp
  STAMP_DIR ./stamp
  DOWNLOAD_DIR .
  URL https://googletest.googlecode.com/files/gtest-1.7.0.zip
  URL_HASH SHA1=f85f6d2481e2c6c4a18539e391aa4ea8ab0394af
  SOURCE_DIR ${GTEST_DL_DIR}
  CONFIGURE_COMMAND ./configure --prefix=${3RDPARTY_USR}
  BUILD_COMMAND make all
  BUILD_IN_SOURCE 1
  TEST_COMMAND ""
  TEST_BEFORE_INSTALL 0
  TEST_AFTER_INSTALL 0
  INSTALL_COMMAND ""
  LOG_DOWNLOAD 1
  LOG_CONFIGURE 1
  LOG_BUILD 1
  LOG_TEST 0
  LOG_INSTALL 0
)
ExternalProject_Get_Property(_get_gtest source_dir)
set(GTEST_ROOT ${source_dir} PARENT_SCOPE)
set(GTEST_INCLUDE_DIRS ${source_dir}/include PARENT_SCOPE)
