#pragma once

#include <memory>


template<typename T>
class Try {
  private:
  const std::shared_ptr<T> result;
  const std::string reason;

  public:
  explicit Try<T>(const std::shared_ptr<T>& result,
                  const std::string& reason)
    : result(result),
      reason(reason) {}

  explicit Try<T>(std::nullptr_t null,
                  const std::string& reason)
    : reason(reason) {}

  operator bool() const {
    return successful();
  }

  bool successful() const {
    return static_cast<bool>(result);
  }

  bool failure() const {
    return !successful();
  }

  const std::string& failure_reason() const {
    return reason;
  }

  operator std::shared_ptr<T>() const {
    return std::shared_ptr<T>(result);
  }

  T& operator*() const {
    return *result;
  }
  const T& operator*() {
    return *result;
  }
};

template<typename T>
Try<T> Result(const T& value) {
  return Try<T>(std::make_shared<T>(value), "");
}

template<typename T>
Try<T> Failure(const std::string& reason) {
  return Try<T>(nullptr, reason);
}

//template<typename T>
//std::shared_ptr<T> from_success(const Try<T>& success) {
//  return std::make_shared<T>(*success);
//}
