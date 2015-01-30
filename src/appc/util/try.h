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
class Try {
  private:
  const std::shared_ptr<T> result;
  const std::string reason;

  public:
  explicit Try<T>(const std::shared_ptr<T> result,
                  const std::string& reason)
  : result(std::move(result)),
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

  T* operator->() const {
    return result.get();
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


template<typename T>
const T& from_result(const Try<T>& success) {
  return *success;
}


template<typename T>
Try<T> TryFrom(const std::function<T ()>& func) {
  try {
    return Result<T>(func());
  } catch (const std::exception& err) {
    return Failure<T>(err.what());
  }
}
