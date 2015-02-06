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

#include "appc/util/namespace.h"
#include "appc/util/try.h"


namespace appc {
namespace discovery {


// Given a name and labels, returns the canonical "common" name for an image.
// Experimental.
inline Try<std::string> render_aci_name(const Name& name, const Labels& labels) {
  auto not_found = labels.end();
  if (labels.find("version") == not_found) return Failure<std::string>("version label required");
  if (labels.find("os") == not_found) return Failure<std::string>("os label required");
  if (labels.find("arch") == not_found) return Failure<std::string>("arch label required");
  return Result(ns::join("-", name, labels.at("version"), labels.at("os"), labels.at("arch"))
                + aci_ext);
}


} // namespace discovery
} // namespace appc
