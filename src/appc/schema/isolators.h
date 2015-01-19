#pragma once

#include "appc/schema/common.h"


namespace appc {
namespace schema {


struct IsolatorName : ACName<IsolatorName> {
  explicit IsolatorName(const std::string& name)
  : ACName<IsolatorName>(name) {}
};


struct Isolator : NameValueType<Isolator> {
  explicit Isolator(const std::string& name,
                    const std::string& value)
  : NameValueType<Isolator>(name, value) {}

  Status validate() const {
    return IsolatorName(name).validate();
  }
};


struct Isolators : ArrayType<Isolators, Isolator> {
  explicit Isolators(const std::vector<Isolator> array)
  : ArrayType<Isolators, Isolator>(array) {}

  Status validate() const {
    for (const auto& annotation : array) {
      auto valid = annotation.validate();
      if (!valid) {
        return valid;
      }
    }
    return Valid();
  }
};


} // namespace schema
} // namespace appc
