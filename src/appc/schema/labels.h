#pragma once

#include "appc/schema/common.h"


namespace appc {
namespace schema {


struct Label : NameValueType<Label> {
  explicit Label(const std::string& name,
                 const std::string& value)
  : NameValueType<Label>(name, value) {}

  Status validate() const {
    // TODO(cdaylward)
    return Valid();
  }
};


struct Labels : ArrayType<Labels, Label> {
  explicit Labels(const std::vector<Label>& labels)
  : ArrayType<Labels, Label>(labels) {}

  operator std::map<std::string, std::string>() const {
    std::map<std::string, std::string> map;
    for (auto& label : array) {
      map[label.name] = label.value;
    }
    return map;
  }

  Status validate() const {
    // TODO(cdaylward)
    return Valid();
  }
};


} // namespace schema
} // namespace appc
