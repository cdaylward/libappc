#pragma once

#include "appc/util/option.h"


TEST(Option, none_is_false) {
  auto none = None<int>();
  ASSERT_FALSE(none);
}

TEST(Option, some_is_true) {
  auto some = Some(std::string(""));
  ASSERT_TRUE(some);
}

TEST(Option, some_is_shared_ptr) {
  auto some = Some(std::string(""));
  std::shared_ptr<std::string> ptr { some };
  ASSERT_EQ(2, ptr.use_count());
}

TEST(Option, some_dereferences) {
  auto some = Some(std::string("one"));
  ASSERT_EQ(typeid(std::string), typeid(*some));
  ASSERT_EQ(typeid(std::string), typeid(from_some(some)));
  ASSERT_EQ(std::string{"one"}, *some);
}
