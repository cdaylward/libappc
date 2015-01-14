#pragma once

#include "appc/schema/common.h"


namespace appc {
namespace schema {


struct Isolator : NameValueType<Isolator> {
  explicit Isolator(const std::string& name,
                    const std::string& value)
  : NameValueType<Isolator>(name, value) {}

  Status validate() const {
    // TODO(cdaylward)
    return Valid();
  }
};


struct Isolators : ArrayType<Isolators, Isolator> {
  explicit Isolators(const std::vector<Isolator> array)
  : ArrayType<Isolators, Isolator>(array) {}

  Status validate() const {
    // TODO(cdaylward)
    return Valid();
  }
};


} // namespace schema
} // namespace appc
