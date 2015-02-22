#include "gtest/gtest.h"

#include "appc/schema/event_handlers.h"

using namespace appc::schema;


static const Json good_json = R"(
  [
      {
         "name": "pre-start",
         "exec": ["/bin/echo", "pre-start"]
      }
  ]
)"_json;

static const Json bad_json = R"(
  [
      {
         "name": "pre-start",
         "exec": ["/bin/echo", "pre-start"]
      },
      {
         "name": "pre-start",
         "exec": ["/bin/echo", "another"]
      }
  ]
)"_json;


TEST(EventHandlers, from_json) {
  auto result = EventHandlers::from_json(good_json);
  ASSERT_TRUE(result);
  ASSERT_TRUE(result->validate());
}


TEST(EventHandlers, disallow_duplicates) {
  auto result = EventHandlers::from_json(bad_json);
  ASSERT_TRUE(result);
  ASSERT_FALSE(result->validate());
}
