#include "gtest/gtest.h"

#include "appc/schema/user.h"

using namespace appc::schema;


TEST(User, empty) {
  ASSERT_FALSE(User::from_json("")->validate());
}

TEST(User, is_integer) {
  ASSERT_TRUE(User::from_json("100")->validate());
  ASSERT_FALSE(User::from_json("1OO")->validate());
}
