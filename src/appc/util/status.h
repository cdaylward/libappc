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
