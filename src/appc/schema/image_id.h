#pragma once

#include <regex>

#include "appc/schema/common.h"


namespace appc {
namespace schema {


struct ImageID : StringType<ImageID> {
  explicit ImageID(const std::string& id)
  : StringType<ImageID>(id) {}

  Status validate() const {
    // TODO limit to sha512-[a-fA-F0-9]{128} ?
    const std::regex pattern("^[a-zA-Z0-9]+-[a-fA-F0-9]+$",
                             std::regex::ECMAScript);
    if (std::regex_match(value, pattern)) {
      return Valid();
    }
    return Invalid("imageID must be <hash name>-<hex representation> format,");
  }
};


} // namespace schema
} // namespace appc
