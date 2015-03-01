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

#include <sys/stat.h>
#include <sys/types.h>

#include "3rdparty/cdaylward/pathname.h"

#include "appc/os/errno.h"
#include "appc/util/status.h"


namespace appc {
namespace os {


inline Status mkdir(const std::string& dir, const mode_t mode, const bool create_parents = false) {
  if (create_parents) {
    if (!pathname::is_absolute(dir)) {
      return Error(std::string{"Will not create parents for relative path: "} + dir);
    }
    std::string current{"/"};
    for (const auto& entry : pathname::split(dir)) {
      if (entry == "/") continue;
      current = pathname::join(current, entry);
      struct stat dummy;
      if (stat(current.c_str(), &dummy) == 0) continue;
      const int result = ::mkdir(current.c_str(), mode);
      if (result == -1) return Errno(dir + " could not be created", errno);
    }
  } else {
    const int result = ::mkdir(dir.c_str(), mode);
    if (result == -1) return Errno(dir + " could not be created", errno);
  }
  return Success();
}


} // namespace os
} // namespace appc

