#include "gtest/gtest.h"

#include "appc/schema/environment.h"

using namespace appc::schema;


static const Json good_json = R"(
  [
      {
         "name": "WORKER_DEBUG",
         "value": "TRUE"
      }
  ]
)"_json;

static const Json bad_name_json = R"(
  [
      {
         "name": "0DEBUG",
         "value": "TRUE"
      }
  ]
)"_json;

static const Json duplicate_json = R"(
  [
      {
         "name": "WORKER_DEBUG",
         "value": "TRUE"
      },
      {
         "name": "WORKER_DEBUG",
         "value": "TRUE"
      }
  ]
)"_json;


TEST(Environment, from_json) {
  auto result = Environment::from_json(good_json);
  ASSERT_TRUE(result);
  ASSERT_TRUE(result->validate());
}


TEST(Environment, rfc1123) {
  auto result = Environment::from_json(bad_name_json);
  ASSERT_TRUE(result);
  ASSERT_FALSE(result->validate());
}


TEST(Environment, disallow_duplicates) {
  auto result = Environment::from_json(duplicate_json);
  ASSERT_TRUE(result);
  ASSERT_FALSE(result->validate());
}
