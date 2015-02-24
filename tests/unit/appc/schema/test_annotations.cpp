#include "gtest/gtest.h"

#include "appc/schema/annotations.h"

using namespace appc::schema;


TEST(Annotation, from_json) {
  const Json json = { {"name", "name.given"}, {"value", "Herbert"} };
  std::shared_ptr<Annotation> result = Annotation::from_json(json);
  Annotation& annotation = *result;
  ASSERT_EQ(std::string{"name.given"}, annotation.name);
  ASSERT_EQ(std::string{"Herbert"}, annotation.value);
  ASSERT_TRUE(annotation.validate());
}

TEST(Annotation, non_acname_is_invalid) {
  const Json json = { {"name", "Given Name"}, {"value", "Herbert"} };
  ASSERT_FALSE(Annotation::from_json(json)->validate());
}

TEST(Annotations, from_json) {
  const Json json = {
    {
      {"name", "name.given"},
      {"value", "Carly"}
    },
    {
      {"name", "name.family"},
      {"value", "Container"}
    },
    {
      {"name", "dob"},
      {"value", "1985-05-20"}
    }
  };
  auto result = Annotations::from_json(json);
  std::map<std::string, std::string> map = *result;
  ASSERT_EQ(std::string{"Carly"}, map["name.given"]);
  ASSERT_EQ(std::string{"Container"}, map["name.family"]);
  ASSERT_EQ(std::string{"1985-05-20"}, map["dob"]);
  ASSERT_TRUE(result->validate());
}

TEST(Annotations, non_acname_element_is_invalid) {
  const Json json = {
    {
      {"name", "name.given"},
      {"value", "Carly"}
    },
    {
      {"name", "name of family"},
      {"value", "Container"}
    },
    {
      {"name", "dob"},
      {"value", "1985-05-20"}
    }
  };
  auto result = Annotations::from_json(json);
  ASSERT_FALSE(result->validate());
}

TEST(Annotations, homepage_must_be_http) {
  const Json json = {
    {
      {"name", "homepage"},
      {"value", "http://github.com/cdaylward/libappc"}
    }
  };
  auto result = Annotations::from_json(json);
  ASSERT_TRUE(result->validate());
  const Json bad_json = {
    {
      {"name", "homepage"},
      {"value", "github.com/cdaylward/libappc"}
    }
  };
  auto bad_result = Annotations::from_json(bad_json);
  ASSERT_FALSE(bad_result->validate());
}
