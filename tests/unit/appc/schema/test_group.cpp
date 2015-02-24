#include "gtest/gtest.h"

#include "appc/schema/group.h"

using namespace appc::schema;


TEST(Group, empty) {
  ASSERT_FALSE(Group::from_json("")->validate());
}

TEST(Group, is_integer) {
  ASSERT_TRUE(Group::from_json("100")->validate());
  ASSERT_FALSE(Group::from_json("1OO")->validate());
}
