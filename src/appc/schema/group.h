#pragma once

#include "appc/schema/common.h"


namespace appc {
namespace schema {


struct Group : StringType<Group> {
  explicit Group(const std::string& gid)
  : StringType<Group>(gid) {}

  Status validate() const {
    //TODO(cdaylward)
    return Valid();
  }
};


} // namespace schema
} // namespace appc
