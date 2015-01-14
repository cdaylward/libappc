#pragma once

#include <regex>

#include "appc/schema/common.h"

namespace appc {
namespace schema {

struct AcVersion : StringType<AcVersion> {
  explicit AcVersion(const std::string& version)
  : StringType<AcVersion>(version) {}

  Status validate() const {
    // TODO(cdaylward) Now I have two problems
    const std::regex version_pattern("\\d+\\.\\d+\\.\\d+(\\-[a-zA-Z0-9]+)*(\\+[a-zA-Z0-9-.]+)*");
    if (!std::regex_match(value, version_pattern)) {
      return Invalid(value + " is not a valid semver 2.0 version string.");
    }
    return Valid();
  }
};


} // namespace schema
} // namespace appc
