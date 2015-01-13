#include "gtest/gtest.h"

#include "appc/schema/common.h"

using namespace appc::schema;


TEST(Status, valid_is_true) {
  ASSERT_TRUE(Valid().successful);
  ASSERT_TRUE(Valid());
}

TEST(Status, invalid_is_false) {
  ASSERT_FALSE(Invalid("invalid").successful);
  ASSERT_FALSE(Invalid("invalid"));
}


struct SomeStringType : StringType<SomeStringType> {
  explicit SomeStringType(const std::string& val)
  : StringType<SomeStringType>(val) {}
  Status validate() {
    return Valid();
  }
};

TEST(StringType, is_implicit_string) {
  std::string some_string { SomeStringType("a string") };
  ASSERT_EQ("a string", some_string);
}

TEST(StringType, same_values_equate) {
  ASSERT_EQ(SomeStringType("a string"), SomeStringType("a string"));
}

TEST(StringType, different_values_dont) {
  ASSERT_NE(SomeStringType("x string"), SomeStringType("y string"));
}

TEST(StringType, from_json_string) {
  const Json json = "A String";
  Try<SomeStringType> a_try = SomeStringType::from_json(json);
  ASSERT_TRUE(a_try);
  ASSERT_EQ(SomeStringType("A String"), from_success(a_try));
}

TEST(StringType, not_from_other_json) {
  ASSERT_FALSE(SomeStringType::from_json(Json{nullptr}));
  ASSERT_FALSE(SomeStringType::from_json(Json{42}));
}

TEST(StringType, to_json) {
  const Json expected_json { "A String" };
  const Json json = SomeStringType::to_json(SomeStringType("A String"));
  ASSERT_EQ(expected_json, json);
}
