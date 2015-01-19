#pragma once

#include "appc/schema/common.h"


namespace appc {
namespace schema {


struct Path : StringType<Path> {
  explicit Path(const std::string& path)
  : StringType<Path>(path) {}

  Status validate() const {
    // TODO(cdaylward)
    return Valid();
  }
};


struct PathWhitelist : ArrayType<PathWhitelist, Path> {
  explicit PathWhitelist(const std::vector<Path>& paths)
  : ArrayType<PathWhitelist, Path>(paths) {}
};


} // namespace schema
} // namespace appc
