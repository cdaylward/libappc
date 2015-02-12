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

#include <cerrno>
#include <cstdlib>
#include <cstring>
#include <iostream>
#include <sys/types.h>

#include "3rdparty/cdaylward/pathname.h"
#include "appc/util/status.h"


namespace appc {
namespace os {


inline Status mkdir(const std::string& dir, const mode_t mode, const bool create_parents = false) {
  if (create_parents && !pathname::is_absolute(dir)) {
    return Error(std::string{"Will not create parents for relative path: "} + dir);
  }
  // FIXME, use syscall, plumb mode
  std::string mkdir_command = "mkdir ";
  if (create_parents) {
    mkdir_command += "-p ";
  }
  mkdir_command += "-- " + dir;
  if (system(mkdir_command.c_str()) != 0) {
    return Error(std::string{"Could not create "} + dir + ": " + strerror(errno));
  }
  return Success();
}


} // namespace os
} // namespace appc

