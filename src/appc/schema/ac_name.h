#pragma once

#include <regex>

#include "appc/schema/common.h"


namespace appc {
namespace schema {

const unsigned int max_ac_name_length = 512;

struct ACName : StringType<ACName> {
  explicit ACName(const std::string& name)
  : StringType<ACName>(name) {}

  virtual Status validate() const {
    if (value.empty()) {
      return Invalid("ACName must not be empty.");
    }
    if (value.size() > max_ac_name_length) {
      return Invalid("ACName must not be longer than " + max_ac_name_length);
    }
    const std::regex pattern("[a-z0-9]+([a-z0-9-\\./]*[a-z0-9])*",
                             std::regex::ECMAScript);
    if (!std::regex_match(value, pattern)) {
      return Invalid("ACName must comply with rfc1123 + allow '/'");
    }
    return Valid();
  }
};


} // namespace schema
} // namespace appc
