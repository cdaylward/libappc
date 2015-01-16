#pragma once

#include "appc/schema/common.h"


namespace appc {
namespace schema {


struct UUID : StringType<UUID> {
  explicit UUID(const std::string& id)
  : StringType<UUID>(id) {}

  Status validate() const {
    //TODO(cdaylward)
    return Valid();
  }
};


} // namespace schema
} // namespace appc
