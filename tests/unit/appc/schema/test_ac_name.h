#pragma once

#include "gtest/gtest.h"

#include "appc/schema/ac_name.h"

using namespace appc::schema;

struct TestName : ACName<TestName> {
  explicit TestName(const std::string& name)
  : ACName<TestName>(name) {}
};


TEST(ACName, empty) {
  ASSERT_FALSE(TestName::from_json("")->validate());
}

TEST(ACName, rfc1123) {
  ASSERT_TRUE(TestName::from_json("aa00")->validate());
  ASSERT_TRUE(TestName::from_json("aa-00")->validate());
  ASSERT_TRUE(TestName::from_json("aa-00.bb-11")->validate());
  ASSERT_TRUE(TestName::from_json("aa-00.bb-11.cc-22")->validate());
  ASSERT_FALSE(TestName::from_json("-aa-00.bb-11.cc-22")->validate());
  ASSERT_FALSE(TestName::from_json("aa0-00.bb-11.cc-22-")->validate());
  ASSERT_FALSE(TestName::from_json(".aa-00.bb-11.cc-22")->validate());
  ASSERT_FALSE(TestName::from_json("aa-00.bb-11.cc-22.")->validate());
  ASSERT_FALSE(TestName::from_json("a#b")->validate());
}

TEST(ACName, rfc1123_plus_slash) {
  ASSERT_TRUE(TestName::from_json("aa-00.bb/11.cc-22")->validate());
  ASSERT_FALSE(TestName::from_json("/aa-00.bb/11.cc-22")->validate());
  ASSERT_FALSE(TestName::from_json("aa-00.bb/11.cc-22/")->validate());
}
