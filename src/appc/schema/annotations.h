#pragma once

#include "appc/schema/common.h"


namespace appc {
namespace schema {


struct Annotation : NameValueType<Annotation> {
  explicit Annotation(const std::string& name,
                      const std::string& value)
  : NameValueType<Annotation>(name, value) {}

  Status validate() const{
    // TODO(cdaylward)
    return Valid();
  }
};


struct Annotations : ArrayType<Annotations, Annotation> {
  explicit Annotations(const std::vector<Annotation> array)
  : ArrayType<Annotations, Annotation>(array) {}

  Status validate() const {
    return Valid();
  }
};


} // namespace schema
} // namespace appc
