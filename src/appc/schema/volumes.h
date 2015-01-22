#pragma once

#include "appc/schema/common.h"
#include "appc/schema/mount_points.h"
#include "appc/schema/try_json.h"


namespace appc {
namespace schema {


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
  explicit MountPointNames(const std::vector<MountPointName>& array)
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
  const Option<VolumeSource> source;
  const Option<ReadOnly> read_only;

  explicit Volume(const VolumeKind& kind,
                  const MountPointNames& fulfills,
                  const Option<VolumeSource>& source = None<VolumeSource>(),
                  const Option<ReadOnly>& read_only = None<ReadOnly>())
  : kind(kind),
    fulfills(fulfills),
    source(source),
    read_only(read_only) {}

  static Try<Volume> from_json(const Json& json) {
    const auto kind = try_from_json<VolumeKind>(json, "kind");
    const auto fulfills = try_from_json<MountPointNames>(json, "fulfills");

    const auto source = try_option_from_json<VolumeSource>(json, "source");
    const auto read_only = try_option_from_json<ReadOnly>(json, "readOnly");

    if (!all_results(kind, fulfills, read_only, source)) {
      return collect_failure_reasons<Volume>(kind, fulfills, read_only, source);
    }

    return Result(Volume(*kind,
                         *fulfills,
                         *source,
                         *read_only));
  }

  static Json to_json(const Volume& volume) {
    Json json{};
    json["kind"] = VolumeKind::to_json(volume.kind);
    json["fulfills"] = MountPointNames::to_json(volume.fulfills);
    if (volume.source) {
      json["source"] = VolumeSource::to_json(*volume.source);
    }
    if (volume.read_only) {
      json["readOnly"] = ReadOnly::to_json(*volume.read_only);
    }
    return json;
  }

  Status validate() const {
    Status host_has_source = [this] {
      if (kind.value == "host" && !source) {
        return Invalid("volume must have source if kind is host");
      };
      return Valid();
    }();
    return collect_status({
      kind.validate(),
      fulfills.validate(),
      validate_if_some(source),
      validate_if_some(read_only),
      host_has_source
    });
  }
};


struct Volumes : ArrayType<Volumes, Volume> {
  explicit Volumes(const std::vector<Volume>& array)
  : ArrayType<Volumes, Volume>(array) {}
};


} // namespace schema
} // namespace appc
