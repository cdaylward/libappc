#include "gtest/gtest.h"

#include "appc/util/try.h"


TEST(Try, result_is_true) {
  auto a_try = Result(std::string{"result"});
  ASSERT_TRUE(a_try);
  ASSERT_TRUE(a_try.successful());
  ASSERT_FALSE(a_try.failure());
}

TEST(Try, failure_is_false) {
  auto a_try = Failure<std::string>("exploded");
  ASSERT_FALSE(a_try);
  ASSERT_FALSE(a_try.successful());
  ASSERT_TRUE(a_try.failure());
}

TEST(Try, failure_has_reason) {
  auto a_try = Failure<std::string>("exploded");
  ASSERT_EQ(std::string{"exploded"}, a_try.failure_reason());
}

TEST(Try, result_is_shared_ptr) {
  auto a_try = Result(std::string{"result"});
  std::shared_ptr<std::string> ptr = a_try;
  ASSERT_EQ(2, ptr.use_count());
}

TEST(Try, result_derefences) {
  auto a_try = Result(std::string{"result"});
  std::shared_ptr<std::string> ptr = a_try;
  ASSERT_EQ(typeid(std::string), typeid(*a_try));
  ASSERT_EQ(typeid(std::string), typeid(from_result(a_try)));
  ASSERT_EQ(std::string{"result"}, *a_try);
}

TEST(Try, result_from_returning_lambda) {
  auto a_try = TryFrom<std::string>([]() {
    return std::string{"lambda"};
  });
  ASSERT_TRUE(a_try);
  ASSERT_EQ(std::string{"lambda"}, *a_try);
}

TEST(Try, failure_from_throwing_lambda) {
  auto a_try = TryFrom<std::string>([]() {
    throw std::runtime_error{"exploded"};
    return std::string{"lambda"};
  });
  ASSERT_FALSE(a_try);
  ASSERT_EQ(std::string{"exploded"}, a_try.failure_reason());
}
