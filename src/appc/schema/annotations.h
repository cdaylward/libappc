#pragma once

#include "appc/schema/common.h"
#include "appc/schema/ac_name.h"


namespace appc {
namespace schema {


struct Annotation : NameValueType<Annotation> {
  explicit Annotation(const std::string& name,
                      const std::string& value)
  : NameValueType<Annotation>(name, value) {}

  Status validate() const {
    return ACName(name).validate();
  }
};


struct Annotations : ArrayType<Annotations, Annotation> {
  explicit Annotations(const std::vector<Annotation> array)
  : ArrayType<Annotations, Annotation>(array) {}

  operator std::map<std::string, std::string>() const {
    std::map<std::string, std::string> map;
    for (auto& label : array) {
      map[label.name] = label.value;
    }
    return map;
  }

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
