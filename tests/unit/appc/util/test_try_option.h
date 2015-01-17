#pragma once

#include <exception>

#include "appc/util/try_option.h"


TEST(OptionFromTry, result_is_some) {
  auto some = OptionFromTry(Result(std::string{"bananas"}));
  ASSERT_TRUE(some);
  ASSERT_EQ(typeid(Option<std::string>), typeid(some));
  ASSERT_EQ(typeid(std::string), typeid(*some));
  ASSERT_EQ(std::string{"bananas"}, *some);
}

TEST(OptionFromTry, failure_is_none) {
  auto none = OptionFromTry(Failure<std::string>("exploded"));
  ASSERT_FALSE(none);
  ASSERT_EQ(typeid(Option<std::string>), typeid(none));
}

TEST(OptionFromTry, some_from_returning_lambda) {
  auto some = OptionFromTry<std::string>( [] () {
    return Result(std::string{"lambda"});
  });
  ASSERT_TRUE(some);
  ASSERT_EQ(typeid(Option<std::string>), typeid(some));
  ASSERT_EQ(std::string{"lambda"}, *some);
}

TEST(OptionFromTry, none_from_throwing_lambda) {
  const auto none = OptionFromTry<std::string>( [] () {
    throw std::runtime_error{"exploded"};
    return Result(std::string{"lambda"});
  });
  ASSERT_FALSE(none);
  ASSERT_EQ(typeid(Option<std::string>), typeid(none));
}
