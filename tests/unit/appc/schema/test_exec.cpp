#include "gtest/gtest.h"

#include "appc/schema/exec.h"

using namespace appc::schema;


static const Json good_json = R"(
  [
    "/usr/local/bin/my_app",
    "--verbose"
  ]
)"_json;


TEST(Exec, from_json) {
  auto exec = Exec::from_json(good_json);
  ASSERT_TRUE(exec);
  ASSERT_TRUE(exec->validate());
}

TEST(Exec, not_empty) {
  auto exec = Exec::from_json(R"([])"_json);
  ASSERT_TRUE(exec);
  ASSERT_FALSE(exec->validate());
}

TEST(Exec, executable_is_absolute) {
  ASSERT_TRUE(Exec::from_json({"/bin/echo"})->validate());
  ASSERT_FALSE(Exec::from_json({"echo"})->validate());
}
