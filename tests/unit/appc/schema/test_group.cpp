#include "gtest/gtest.h"

#include "appc/schema/group.h"

using namespace appc::schema;


TEST(Group, is_integer) {
  const Json val = 100;
  ASSERT_TRUE(Group::from_json(val));
  ASSERT_FALSE(Group::from_json("100"));
}

TEST(Group, is_in_range) {
  const Json supra = static_cast<int64_t>(INT32_MAX) + 1;
  const Json infra = static_cast<int64_t>(INT32_MIN) - 1;
  ASSERT_FALSE(Group::from_json(supra)->validate());
  ASSERT_FALSE(Group::from_json(infra)->validate());
}
