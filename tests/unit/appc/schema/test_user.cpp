#include "gtest/gtest.h"

#include <cstdint>

#include "appc/schema/user.h"

using namespace appc::schema;


TEST(User, empty) {
  ASSERT_FALSE(User::from_json("")->validate());
}
