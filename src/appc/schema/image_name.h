#pragma once

#include "appc/schema/common.h"


namespace appc {
namespace schema {


struct ImageName : StringType<ImageName> {
  explicit ImageName(const std::string& name)
  : StringType<ImageName>(name) {}

  Status validate() const {
    //TODO(cdaylward)
    return Valid();
  }
};


} // namespace schema
} // namespace appc
