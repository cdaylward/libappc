#pragma once

#include "appc/schema/common.h"
#include "appc/schema/path.h"


namespace appc {
namespace schema {


struct PathWhitelist : ArrayType<PathWhitelist, Path> {
  explicit PathWhitelist(const std::vector<Path>& paths)
  : ArrayType<PathWhitelist, Path>(paths) {}
};


} // namespace schema
} // namespace appc
