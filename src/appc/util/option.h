// Copyright 2015 Charles D. Aylward
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
//     http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.
//
// A (possibly updated) copy of of this software is available at
// https://github.com/cdaylward/libappc

#pragma once

#include <memory>


template<typename T>
class Option {
  private:
  const std::shared_ptr<T> ptr;

  public:
  explicit Option<T>(const std::shared_ptr<T>& ptr)
  : ptr(ptr) {}

  explicit Option<T>(std::nullptr_t) : ptr{nullptr} {}

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

  T* operator->() const {
    return ptr.get();
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
const T& from_some(const Option<T>& option) {
  return *option;
}
