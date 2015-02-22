#include "gtest/gtest.h"

#include "appc/schema/path.h"

using namespace appc::schema;


TEST(Path, cannot_be_empty) {
  ASSERT_FALSE(Path::from_json("")->validate());
}


TEST(Path, must_be_absolute) {
  ASSERT_TRUE(Path::from_json("/a/b/c")->validate());
  ASSERT_FALSE(Path::from_json("a/b/c")->validate());
}
