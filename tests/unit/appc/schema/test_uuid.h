#pragma once

#include "gtest/gtest.h"

#include "appc/schema/uuid.h"

using namespace appc::schema;


TEST(UUID, empty) {
  ASSERT_FALSE(UUID::from_json("")->validate());
}

TEST(UUID, rfc4122) {
  ASSERT_FALSE(UUID::from_json("")->validate());
  ASSERT_FALSE(UUID::from_json("z81d4fae-7dec-11d0-a765-00a0c91e6bf6")->validate());
  ASSERT_TRUE(UUID::from_json("f81d4fae-7dec-11d0-a765-00a0c91e6bf6")->validate());
}

