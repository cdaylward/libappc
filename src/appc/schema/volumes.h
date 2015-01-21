#pragma once

#include "appc/schema/common.h"
#include "appc/schema/try_json.h"


namespace appc {
namespace schema {

using ReadOnly = bool;


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


// TODO this should be a Path type
struct VolumeSource : StringType<VolumeSource> {
  explicit VolumeSource(const std::string& path)
  : StringType<VolumeSource>(path) {}

  Status validate() const {
    // TODO
    return Valid();
  }
};


struct Volume : Type<Volume> {
  const VolumeKind kind;
  const MountPointNames fulfills;
  const VolumeSource source;
  const ReadOnly read_only;

  explicit Volume(const VolumeKind& kind,
                  const MountPointNames& fulfills,
                  const VolumeSource& source,
                  const ReadOnly read_only)
  : kind(kind),
    fulfills(fulfills),
    source(source),
    read_only(read_only) {}

  static Try<Volume> from_json(const Json& json) {
    const auto kind = try_from_json<VolumeKind>(json, "kind");
    const auto fulfills = try_from_json<MountPointNames>(json, "fulfills");

    const auto read_only_try = TryFrom<bool>([&json]() {
      return json[std::string{"readOnly"}].get<bool>();
    });

    if (!SomeIfAll(kind, fulfills)) {
      return collect_failure_reasons<Volume>(kind, fulfills);
    }

    ReadOnly read_only{false};
    if (read_only_try) {
      read_only = *read_only_try;
    }

    if (kind->value == "host") {
      const auto source = try_from_json<VolumeSource>(json, "source");
      return Result(Volume(*kind,
                           *fulfills,
                           *source,
                           read_only));
    }

    return Result(Volume(*kind,
                         *fulfills,
                         VolumeSource(""),
                         read_only));
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
