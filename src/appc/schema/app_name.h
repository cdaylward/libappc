#pragma once

#include "appc/schema/common.h"


namespace appc {
namespace schema {


struct AppName : StringType<AppName> {
  explicit AppName(const std::string& name)
  : StringType<AppName>(name) {}

  Status validate() const {
    //TODO(cdaylward)
    return Valid();
  }
};


} // namespace schema
} // namespace appc
