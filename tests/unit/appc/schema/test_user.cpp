#include "gtest/gtest.h"

#include <cstdint>

#include "appc/schema/user.h"

using namespace appc::schema;


TEST(User, is_integer) {
  const Json val = 100;
  ASSERT_TRUE(User::from_json(val));
  ASSERT_FALSE(User::from_json("100"));
}

TEST(User, is_in_range) {
  const Json supra = static_cast<int64_t>(INT32_MAX) + 1;
  const Json infra = static_cast<int64_t>(INT32_MIN) - 1;
  ASSERT_FALSE(User::from_json(supra)->validate());
  ASSERT_FALSE(User::from_json(infra)->validate());
}
