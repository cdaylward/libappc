#include "gtest/gtest.h"

#include "appc/schema/group.h"

using namespace appc::schema;


TEST(Group, empty) {
  ASSERT_FALSE(Group::from_json("")->validate());
}
