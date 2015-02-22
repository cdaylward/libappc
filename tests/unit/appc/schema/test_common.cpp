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

TEST(Status, collect_status) {
  ASSERT_TRUE(collect_status({Valid(), Valid()}));
  auto invalid = collect_status({Invalid("one"), Invalid("two"), Valid()});
  ASSERT_FALSE(invalid);
  ASSERT_EQ(std::string{"\"one\", \"two\""}, invalid.message);
}

struct SomeStringType : StringType<SomeStringType> {
  explicit SomeStringType(const std::string& val)
  : StringType<SomeStringType>(val) {}
  Status validate() const {
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
  ASSERT_EQ(SomeStringType("A String"), *a_try);
}

TEST(StringType, not_from_other_json) {
  ASSERT_FALSE(SomeStringType::from_json(Json{}));
  ASSERT_FALSE(SomeStringType::from_json(Json{42}));
  std::shared_ptr<SomeStringType> ptr = SomeStringType::from_json(Json{42});
  ASSERT_FALSE(ptr);
}

TEST(StringType, to_json) {
  const Json expected_json = "A String";
  const Json json = SomeStringType::to_json(SomeStringType("A String"));
  ASSERT_EQ(Json::value_t::string, json.type());
  ASSERT_EQ(expected_json, json);
}

TEST(StringType, marshal_to_unmarshal) {
  const Json expected_json = "A String";
  const Try<SomeStringType> a_try = SomeStringType::from_json(expected_json);
  ASSERT_TRUE(a_try);
  const Json json = SomeStringType::to_json(*a_try);
  ASSERT_EQ(Json::value_t::string, json.type());
  ASSERT_EQ(expected_json, json);
}

