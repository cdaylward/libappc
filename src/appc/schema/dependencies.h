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

#include "appc/schema/app_name.h"
#include "appc/schema/image_id.h"
#include "appc/schema/labels.h"
#include "appc/schema/try_json.h"
#include "appc/util/try.h"
#include "appc/util/try_option.h"


namespace appc {
namespace schema {


struct Dependency : Type<Dependency> {
  const AppName app_name;
  const Option<ImageID> image_id;
  const Option<Labels> labels;

  explicit Dependency(const AppName& app_name,
                      const Option<ImageID>& image_id,
                      const Option<Labels>& labels)
  : app_name(app_name),
    image_id(image_id),
    labels(labels) {}

  static Try<Dependency> from_json(const Json& json) {
    const auto app_name = try_from_json<AppName>(json, "app");

    const auto image_id = try_option_from_json<ImageID>(json, "imageID");
    const auto labels = try_option_from_json<Labels>(json, "labels");

    if (!all_results(app_name, image_id, labels)) {
      return collect_failure_reasons<Dependency>(app_name, image_id, labels);
    }
    return Result(Dependency(from_result(app_name),
                             from_result(image_id),
                             from_result(labels)));
  }

  Status validate() const {
    return collect_status({
      app_name.validate(),
      validate_if_some(image_id),
      validate_if_some(labels)
    });
  }
};


struct Dependencies : ArrayType<Dependencies, Dependency> {
  explicit Dependencies(const std::vector<Dependency>& dependencies)
  : ArrayType<Dependencies, Dependency>(dependencies) {}
};


} // namespace schema
} // namespace appc
