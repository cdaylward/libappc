#pragma once

#include "appc/schema/common.h"


namespace appc {
namespace schema {


struct ImageID : StringType<ImageID> {
  explicit ImageID(const std::string& id)
  : StringType<ImageID>(id) {}

  Status validate() const {
    //TODO(cdaylward)
    return Valid();
  }
};


} // namespace schema
} // namespace appc
