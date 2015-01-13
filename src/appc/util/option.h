#pragma once

#include <memory>


template<typename T>
using Option = std::unique_ptr<T>;

template<typename T>
Option<T>Some(const T& value) {
    return Option<T>( new T(value) );
}

template<typename T>
Option<T> None() {
  return Option<T>(nullptr);
}

template<typename T>
const T& fromSome(const Option<T>& option) {
  return *option;
}
