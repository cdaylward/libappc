#pragma once

#include "appc/schema/common.h"


namespace appc {
namespace schema {


struct LabelName : ACName<LabelName> {
  explicit LabelName(const std::string& name)
  : ACName<LabelName>(name) {}
};


struct Label : NameValueType<Label> {
  explicit Label(const std::string& name,
                 const std::string& value)
  : NameValueType<Label>(name, value) {}

  Status validate() const {
    return LabelName(name).validate();
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
    for (const auto& label : array) {
      auto valid = label.validate();
      if (!valid) {
        return valid;
      }
    }
    return Valid();
  }
};


} // namespace schema
} // namespace appc
