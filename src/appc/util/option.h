#pragma once

#include <memory>


template<typename T>
class Option {
  private:
  const std::shared_ptr<T> ptr;

  public:
  explicit Option<T>(const std::shared_ptr<T>& ptr)
  : ptr(ptr) {}

  explicit Option<T>(std::nullptr_t null) : ptr{nullptr} {}

  operator bool() const {
    return static_cast<bool>(ptr);
  }

  operator std::shared_ptr<T>() const {
    return std::shared_ptr<T>(ptr);
  }

  T& operator*() const {
    return *ptr;
  }
  const T& operator*() {
    return *ptr;
  }
};


template<typename T>
Option<T>Some(const T& value) {
  return Option<T>(std::make_shared<T>(value));
}

template<typename T>
Option<T> None() {
  return Option<T>(nullptr);
}

template<typename T>
const T& fromSome(const Option<T>& option) {
  return *option;
}
