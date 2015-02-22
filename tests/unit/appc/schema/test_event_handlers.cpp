#include "gtest/gtest.h"

#include "appc/schema/event_handlers.h"

using namespace appc::schema;


TEST(EventHandlers, from_json) {
  const Json json = R"(
    [
        {
           "name": "pre-start",
           "exec": ["/bin/echo", "pre-start"]
        }
    ]
  )"_json;
  auto result = EventHandlers::from_json(json);
  ASSERT_TRUE(result);
  ASSERT_TRUE(result->validate());
}

