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

TEST(SomeIfAll, some_if_all) {
  auto option = SomeIfAll(Result(1), Result(std::string{"two"}), Result(3.01));
  ASSERT_TRUE(option);
}

TEST(SomeIfAll, none_if_not_first) {
  auto option = SomeIfAll(Failure<int>("explode"), Result(1), Result(std::string{"two"}));
  ASSERT_FALSE(option);
}

TEST(SomeIfAll, none_if_not_middle) {
  auto option = SomeIfAll(Result(1), Failure<int>("explode"), Result(std::string{"two"}));
  ASSERT_FALSE(option);
}

TEST(SomeIfAll, none_if_not_last) {
  auto option = SomeIfAll(Result(1), Result(std::string{"two"}), Failure<int>("explode"));
  ASSERT_FALSE(option);
}

TEST(collect_failure_reasons, has_all_reasons) {
  auto option = collect_failure_reasons<float>(Failure<int>("explode_1"), Failure<int>("explode_2"), Failure<int>("explode_3"));
  ASSERT_FALSE(option);
  ASSERT_EQ(std::string{"\"explode_1\", \"explode_2\", \"explode_3\""}, option.failure_reason());
}
