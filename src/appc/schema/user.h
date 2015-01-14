#pragma once

#include "appc/schema/common.h"


namespace appc {
namespace schema {


struct User : StringType<User> {
  explicit User(const std::string& uid)
  : StringType<User>(uid) {}

  Status validate() const {
    //TODO(cdaylward)
    return Valid();
  }
};


} // namespace schema
} // namespace appc
