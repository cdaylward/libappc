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

#include <map>

#include "appc/util/namespace.h"
#include "appc/util/try.h"


namespace appc {
namespace discovery {


// TODO WIP place holders
using URI = std::string;
using Path = std::string;
using Name = std::string;
using Labels = std::map<std::string, std::string>;

// TODO still up in the air in the spec
const std::string aci_ext = ".aci";
const std::string sig_ext = ".aci.sig";

const URI file_prefix{"file://"}; // ignoring file://localhost/ for now
const URI https_prefix{"https://"};


bool valid_prefix(const URI& prefix, const URI& uri) {
  if (uri.length() <= prefix.length() ||
      uri.compare(0, prefix.length(), prefix) != 0) {
    return false;
  }
  return true;
}


} // namespace discovery
} // namespace appc
