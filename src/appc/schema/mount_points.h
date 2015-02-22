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

#include <set>

#include "appc/schema/ac_name.h"
#include "appc/schema/common.h"
#include "appc/schema/path.h"
#include "appc/schema/try_json.h"
#include "appc/util/try_option.h"


namespace appc {
namespace schema {


struct MountName : ACName<MountName> {
  explicit MountName(const std::string& name)
  : ACName<MountName>(name) {}
};


struct ReadOnly : BooleanType<ReadOnly> {
  explicit ReadOnly(const bool read_only)
  : BooleanType<ReadOnly>(read_only) {}
};


struct MountPoint : Type<MountPoint> {
  const MountName name;
  const Path path;
  const Option<ReadOnly> read_only;

  explicit MountPoint(const MountName& name,
                      const Path& path,
                      const Option<ReadOnly>& read_only)
  : name(name),
    path(path),
    read_only(read_only) {}

  static Try<MountPoint> from_json(const Json& json) {
    const auto name = try_from_json<MountName>(json, "name");
    const auto path = try_from_json<Path>(json, "path");
    const auto read_only = try_option_from_json<ReadOnly>(json, "readOnly");
    if (!all_results(name, path, read_only)) {
      return collect_failure_reasons<MountPoint>(name, path, read_only);
    }
    return Result(MountPoint(from_result(name),
                             from_result(path),
                             from_result(read_only)));
  }

  virtual Status validate() const {
    return collect_status({name.validate(),
                           path.validate()});
  }
};


struct MountPoints : ArrayType<MountPoints, MountPoint> {
  explicit MountPoints(const std::vector<MountPoint>& args)
  : ArrayType<MountPoints, MountPoint>(args) {}

  virtual Status validate() const {
    std::set<std::string> seen;
    for (const auto& mount_point : this->array) {
      if (seen.find(mount_point.name) != seen.end()) {
        return Invalid(std::string{"mountPoints has duplicate definition "} + mount_point.name.value);
      }
      seen.emplace(mount_point.name.value);
      auto valid = mount_point.validate();
      if (!valid) {
        return valid;
      }
    }
    return Valid();
  }
};


} // namespace schema
} // namespace appc
