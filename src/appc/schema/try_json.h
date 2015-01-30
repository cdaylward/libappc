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

#include "appc/schema/common.h"
#include "appc/util/try.h"


namespace appc {
namespace schema {


template<typename T>
Try<T> try_from_json(const Json& json, const std::string& field_name) {
  try {
    return T::from_json(json[field_name]);
  }
  catch (const std::exception& err) {
    return Failure<T>(err.what());
  }
}


template<typename T>
Try<Option<T>> try_option_from_json(const Json& json, const std::string& field_name, const bool required = false) {

  const auto has_field = TryFrom<Json>([&json, &field_name]() {
    return json[field_name];
  });
  if (!required && !has_field) {
    return Result(None<T>());
  }
  auto a_try = try_from_json<T>(json, field_name);
  if (a_try) {
    return Result(Some(from_result(a_try)));
  }

  return Failure<Option<T>>(a_try.failure_reason());
}


} //namespace schema
} //namespace appc
