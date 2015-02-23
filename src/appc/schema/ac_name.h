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

#include <regex>

#include "appc/schema/common.h"


namespace appc {
namespace schema {


const unsigned int max_ac_name_length = 512;


template<typename T>
struct ACName : StringType<T> {
  explicit ACName<T>(const std::string& name)
  : StringType<T>(name) {}

  virtual Status validate() const {
    if (this->value.empty()) {
      return Invalid("ACName must not be empty.");
    }
    if (this->value.size() > max_ac_name_length) {
      return Invalid("ACName must not be longer than " + std::to_string(max_ac_name_length));
    }
    const std::regex pattern("^[A-Za-z0-9]+([-./][A-Za-z0-9]+)*$",
                             std::regex::ECMAScript);
    if (!std::regex_match(this->value, pattern)) {
      return Invalid("ACName must comply with rfc1123 + allow '/'");
    }
    return Valid();
  }
};


} // namespace schema
} // namespace appc
