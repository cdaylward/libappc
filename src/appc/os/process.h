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
#include <iostream>

#include "appc/util/try.h"


namespace appc {
namespace os {


inline Try<std::string> get_process_output(const std::string& cmd, const std::size_t bytes_to_read) {
  FILE* process = popen(cmd.c_str(), "r");
  if (!process) return Failure<std::string>(cmd + " failed: " + strerror(errno));
  std::unique_ptr<char[]> buffer(new char[bytes_to_read]);
  fread(buffer.get(), sizeof(char), bytes_to_read, process);
  if (ferror(process)) {
    pclose(process);
    return Failure<std::string>(cmd + " could not be read: " + strerror(errno));
  }
  pclose(process);

  return Result(std::string{buffer.get()});
}


} // namespace os
} // namespace appc

