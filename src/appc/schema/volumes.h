#pragma once

#include "appc/schema/common.h"


namespace appc {
namespace schema {

using ReadOnly = bool;

const std::string fulfills_field{"fulfills"};
const std::string kind_field{"kind"};
const std::string source_field{"source"};
const std::string read_only_field{"readOnly"};


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
    const auto kind = TryFlatten<VolumeKind>([&json]() {
      return VolumeKind::from_json(json[kind_field]);
    });
    const auto fulfills = TryFlatten<MountPointNames>([&json]() {
      return MountPointNames::from_json(json[fulfills_field]);
    });

    if (!SomeIfAll(kind, fulfills)) {
      return collect_failure_reasons<Volume>(kind, fulfills);
    }

    const auto read_only_try = TryFrom<bool>([&json]() {
      return json[read_only_field].get<bool>();
    });

    ReadOnly read_only{false};
    if (read_only_try) {
      read_only = *read_only_try;
    }

    if (kind->value == "host") {
      const auto source = TryFlatten<VolumeSource>([&json]() {
        return VolumeSource::from_json(json[source_field]);
      });
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
