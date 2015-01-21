#pragma once

#include "appc/schema/common.h"


namespace appc {
namespace schema {


// TODO cleanup, types
struct MountPoint : Type<MountPoint> {
  const std::string name;
  const std::string path;
  const bool readOnly;

  explicit MountPoint(const std::string& name,
                      const std::string& path,
                      const bool readOnly)
  : name(name),
    path(path),
    readOnly(readOnly) {}

  static Try<MountPoint> from_json(const Json& json) {
    if (json.type() != Json::value_type::object) {
      return Failure<MountPoint>("MountPoint must be built from JSON object");
    }
    return Result(MountPoint(json[std::string{"name"}].get<std::string>(),
                             json[std::string{"path"}].get<std::string>(),
                             json[std::string{"readOnly"}].get<bool>()));
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
