#pragma once

#include "appc/schema/common.h"


namespace appc {
namespace schema {


struct MountName : ACName<MountName> {
  explicit MountName(const std::string& name)
  : ACName<MountName>(name) {}
};


struct MountPath : StringType<MountPath> {
  explicit MountPath(const std::string& name)
  : StringType<MountPath>(name) {}

  Status validate() const {
    return Valid();
  }
};


struct ReadOnly : BooleanType<ReadOnly> {
  explicit ReadOnly(const bool read_only)
  : BooleanType<ReadOnly>(read_only) {}
};


struct MountPoint : Type<MountPoint> {
  const MountName name;
  const MountPath path;
  const Option<ReadOnly> read_only;

  explicit MountPoint(const MountName& name,
                      const MountPath& path,
                      const Option<ReadOnly>& read_only)
  : name(name),
    path(path),
    read_only(read_only) {}

  static Try<MountPoint> from_json(const Json& json) {
    const auto name = try_from_json<MountName>(json, "name");
    const auto path = try_from_json<MountPath>(json, "path");
    const auto read_only = try_option_from_json<ReadOnly>(json, "readOnly");
    if (!all_results(name, path, read_only)) {
      return collect_failure_reasons<MountPoint>(name, path, read_only);
    }
    return Result(MountPoint(*name,
                             *path,
                             *read_only));
  }

  Status validate() const {
    // TODO(cdaylward)
    return Valid();
  }
};


struct MountPoints : ArrayType<MountPoints, MountPoint> {
  explicit MountPoints(const std::vector<MountPoint>& args)
  : ArrayType<MountPoints, MountPoint>(args) {}
};


} // namespace schema
} // namespace appc
