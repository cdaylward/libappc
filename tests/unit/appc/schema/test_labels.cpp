#include "gtest/gtest.h"

#include "appc/schema/labels.h"

using namespace appc::schema;


TEST(Label, from_json) {
  const Json json = { {"name", "name.given"}, {"value", "Herbert"} };
  std::shared_ptr<Label> result = Label::from_json(json);
  Label& label = *result;
  ASSERT_EQ(std::string{"name.given"}, label.name);
  ASSERT_EQ(std::string{"Herbert"}, label.value);
  ASSERT_TRUE(label.validate());
}
// TODO invalid labels, test known, os, arch, version

TEST(Label, to_json) {
  const Json expected_json = { {"name", "name.given"}, {"value", "Herbert"} };
  const Json json = Label::to_json(Label{"name.given", "Herbert"});
  ASSERT_EQ(expected_json, json);
}

bool labels_eq(const Labels& llabels, const Labels& rlabels) {
  const std::map<std::string, std::string> lhs = llabels;
  const std::map<std::string, std::string> rhs = rlabels;
  return lhs.size() == rhs.size() &&
         std::equal(lhs.begin(), lhs.end(),
                    rhs.begin());
}


TEST(Labels, from_json) {
  const Json json = {
    {
      {"name", "version"},
      {"value", "1.0.0"}
    },
    {
      {"name", "os"},
      {"value", "linux"}
    },
    {
      {"name", "arch"},
      {"value", "x86_64"}
    }
  };
  const Labels expected_labels{{
            Label("arch", "x86_64"),
            Label("os", "linux"),
            Label("version", "1.0.0") }};

  std::shared_ptr<Labels> result = Labels::from_json(json);
  Labels& labels = *result;
  ASSERT_TRUE(labels.validate());
  ASSERT_TRUE(labels_eq(expected_labels, labels));
}
