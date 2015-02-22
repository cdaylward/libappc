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
#include <set>

#include "appc/schema/common.h"


namespace appc {
namespace schema {


struct EnvironmentVariable : NameValueType<EnvironmentVariable> {
  explicit EnvironmentVariable(const std::string& name,
                               const std::string& value)
  : NameValueType<EnvironmentVariable>(name, value) {}

  Status validate() const {
    if (this->name.empty()) {
      return Invalid("Environment name must not be empty.");
    }
    const std::regex pattern("^[A-Za-z]+(_[A-Za-z0-9]+)*$",
                             std::regex::ECMAScript);
    if (!std::regex_match(this->name, pattern)) {
      return Invalid("Environment name must comply with rfc1123");
    }
    return Valid();
  }
};


struct Environment : ArrayType<Environment, EnvironmentVariable> {
  explicit Environment(const std::vector<EnvironmentVariable> array)
  : ArrayType<Environment, EnvironmentVariable>(array) {}

  operator std::map<std::string, std::string>() const {
    std::map<std::string, std::string> map;
    for (auto& var : array) {
      map[var.name] = var.value;
    }
    return map;
  }

  virtual Status validate() const {
    std::set<std::string> seen;
    for (const auto& var : this->array) {
      if (seen.find(var.name) != seen.end()) {
        return Invalid(std::string{"environment has duplicate definition "} + var.name);
      }
      seen.emplace(var.name);
      auto valid = var.validate();
      if (!valid) {
        return valid;
      }
    }
    return Valid();
  }
};


} // namespace schema
} // namespace appc
