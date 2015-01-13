#pragma once

#include <memory>


template<typename T>
class Try {
  private:
  const bool success;
  const T* result;
  const std::string reason;

  public:
  explicit Try<T>(const bool success,
                  const T* result,
                  const std::string& reason)
    : success(success),
      result(result),
      reason(reason) {}

  explicit Try<T>(const bool success,
                  std::nullptr_t null,
                  const std::string& reason)
    : success(success),
      reason(reason) {}

  ~Try() {
    delete result;
  }

  Try<T>(const Try<T>& other)
  : success(other.success),
    result(other.result),
    reason(other.reason)
  {}

  Try<T>(const Try<T>&& other)
  : success(std::move(other.success)),
    result(std::move(other.result)),
    reason(std::move(other.reason))
  {}

  operator bool() const {
    return success;
  }

  bool successful() {
    return success;
  }

  bool failure() {
    return !success;
  }

  const std::string& failureReason() {
    return reason;
  }

  T operator()() const {
    return *result;
  }
  T& operator*() const {
    return *result;
  }
  const T& operator*() {
    return *result;
  }
};

template<typename T>
Try<T> Success(const T& value) {
  return Try<T>(true, new T(value), "");
}

template<typename T>
Try<T> Failure(const std::string& reason) {
  return Try<T>(false, nullptr, reason);
}

template<typename T>
T from_success(const Try<T>& success) {
  return success();
}
