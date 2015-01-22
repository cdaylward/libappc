#pragma once

#include <regex>

#include "appc/schema/common.h"


namespace appc {
namespace schema {


const unsigned int max_ac_name_length = 512;


template<typename T>
struct ACName : StringType<T> {
  explicit ACName<T>(const std::string& name)
  : StringType<T>(name) {}

  virtual Status validate() const {
    if (this->value.empty()) {
      return Invalid("ACName must not be empty.");
    }
    if (this->value.size() > max_ac_name_length) {
      return Invalid("ACName must not be longer than " + max_ac_name_length);
    }
    const std::regex pattern("^[A-Za-z0-9]+([\\-\\.\\/][A-Za-z0-9]+)*$",
                             std::regex::ECMAScript);
    if (!std::regex_match(this->value, pattern)) {
      return Invalid("ACName must comply with rfc1123 + allow '/'");
    }
    return Valid();
  }
};


} // namespace schema
} // namespace appc
