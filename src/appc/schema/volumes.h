#pragma once

#include "appc/schema/common.h"


namespace appc {
namespace schema {


const std::string fulfills_field{"fulfills"};
const std::string kind_field{"kind"};


struct VolumeKind : StringType<VolumeKind> {
  explicit VolumeKind(const std::string& kind)
  : StringType<VolumeKind>(kind) {}

  Status validate() const {
    if (value != "empty" && value != "host") {
      return Invalid("Volume kind must be 'empty' or 'host'");
    }
    return Valid();
  }
};


struct MountPointName : ACName<MountPointName> {
  explicit MountPointName(const std::string& mount_point)
  : ACName<MountPointName>(mount_point) {}
};


struct MountPointNames : ArrayType<MountPointNames, MountPointName> {
  explicit MountPointNames(const std::vector<MountPointName> array)
  : ArrayType<MountPointNames, MountPointName>(array) {}
};


struct Volume : Type<Volume> {
  const VolumeKind kind;
  const MountPointNames fulfills;

  explicit Volume(const VolumeKind& kind,
                  const MountPointNames& fulfills)
  : kind(kind),
    fulfills(fulfills) {}

  static Try<Volume> from_json(const Json& json) {
    const auto kind = TryFlatten<VolumeKind>([&json]() {
      return VolumeKind::from_json(json[kind_field]);
    });
    const auto fulfills = TryFlatten<MountPointNames>([&json]() {
      return MountPointNames::from_json(json[fulfills_field]);
    });

    if (!SomeIfAll(kind, fulfills)) {
      return collect_failure_reasons<Volume>(kind, fulfills);
    }

    return Result(Volume(*kind,
                         *fulfills));
  }

  Status validate() const {
    return collect_status({
      kind.validate(),
      fulfills.validate()
    });
  }
};


struct Volumes : ArrayType<Volumes, Volume> {
  explicit Volumes(const std::vector<Volume> array)
  : ArrayType<Volumes, Volume>(array) {}
};


} // namespace schema
} // namespace appc
