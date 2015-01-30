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

#include <vector>


struct Status {
  const bool successful;
  const std::string message;

  explicit Status(const bool successful,
                  const std::string& message)
  : successful(successful),
    message(message) {}

  operator bool() const {
    return successful;
  }
};


Status Valid() {
  return Status(true, "");
}


Status Invalid(const std::string& message) {
  return Status(false, message);
}


Status Success() {
  return Status(true, "");
}


Status Error(const std::string& message) {
  return Status(false, message);
}


Status collect_status(const std::vector<Status>& statuses) {
  std::string message{};
  bool successful{true};
  for (auto& status: statuses) {
    if (!status) {
      successful = false;
      if (!message.empty()) {
        message += ", ";
      }
      message += "\"" + status.message + "\"";
    }
  }
  return Status(successful, message);
}
