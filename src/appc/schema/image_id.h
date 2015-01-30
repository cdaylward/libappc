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


struct ImageID : StringType<ImageID> {
  explicit ImageID(const std::string& id)
  : StringType<ImageID>(id) {}

  Status validate() const {
    // TODO limit to sha512-[a-fA-F0-9]{128} ?
    const std::regex pattern("^[a-zA-Z0-9]+-[a-fA-F0-9]+$",
                             std::regex::ECMAScript);
    if (std::regex_match(value, pattern)) {
      return Valid();
    }
    return Invalid("imageID must be <hash name>-<hex representation> format,");
  }
};


} // namespace schema
} // namespace appc
