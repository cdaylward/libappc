#pragma once

#include "appc/schema/common.h"


namespace appc {
namespace schema {


struct AppName : public ACName<AppName> {
  explicit AppName(const std::string& name)
  : ACName<AppName>(name) {}
};


} // namespace schema
} // namespace appc
