// Copyright 2015 Charles D. Aylward
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
//     http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.
//
// A (possibly updated) copy of of this software is available at
// https://github.com/cdaylward/libappc

#pragma once

#include "appc/schema/common.h"
#include "appc/schema/ac_name.h"


namespace appc {
namespace schema {


struct AnnotationName : ACName<AnnotationName> {
  explicit AnnotationName(const std::string& name)
  : ACName<AnnotationName>(name) {}
};


struct Annotation : NameValueType<Annotation> {
  explicit Annotation(const std::string& name,
                      const std::string& value)
  : NameValueType<Annotation>(name, value) {}

  Status validate() const {
    return AnnotationName(name).validate();
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
};


} // namespace schema
} // namespace appc
