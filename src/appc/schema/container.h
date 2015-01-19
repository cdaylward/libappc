#pragma once

#include "appc/schema/ac_kind.h"
#include "appc/schema/ac_version.h"
#include "appc/schema/annotations.h"
#include "appc/schema/app_refs.h"
#include "appc/schema/isolators.h"
#include "appc/schema/uuid.h"
#include "appc/schema/volumes.h"


namespace appc {
namespace schema {


struct ContainerRuntimeManifest {
  const AcVersion ac_version;
  const AcKind ac_kind;
  const UUID uuid;
  const AppRefs app_refs;
  const Option<Volumes> volumes;
  const Option<Isolators> isolators;
  const Option<Annotations> annotations;

  explicit ContainerRuntimeManifest(const AcVersion& ac_version,
                                    const AcKind& ac_kind,
                                    const UUID& uuid,
                                    const AppRefs& app_refs,
                                    const Option<Volumes>& volumes,
                                    const Option<Isolators>& isolators,
                                    const Option<Annotations>& annotations)
    : ac_version(ac_version),
      ac_kind(ac_kind),
      uuid(uuid),
      app_refs(app_refs),
      volumes(volumes),
      isolators(isolators),
      annotations(annotations) {}

  static Try<ContainerRuntimeManifest> from_json(const Json& json) {
    const auto ac_version = TryFlatten<AcVersion>([&json]() {
      return AcVersion::from_json(json[std::string{"acVersion"}]);
    });
    const auto ac_kind = TryFlatten<AcKind>([&json]() {
      return AcKind::from_json(json[std::string{"acKind"}]);
    });
    const auto uuid = TryFlatten<UUID>([&json]() {
      return UUID::from_json(json[std::string{"uuid"}]);
    });
    const auto app_refs = TryFlatten<AppRefs>([&json]() {
      return AppRefs::from_json(json[std::string{"apps"}]);
    });

    if (!SomeIfAll(ac_version, ac_kind, uuid, app_refs)) {
      return collect_failure_reasons<ContainerRuntimeManifest>(ac_version, ac_kind, uuid, app_refs);
    }

    const auto volumes = OptionFromTry<Volumes>([&json]() {
      return Volumes::from_json(json[std::string{"volumes"}]);
    });
    const auto isolators = OptionFromTry<Isolators>([&json]() {
      return Isolators::from_json(json[std::string{"isolators"}]);
    });
    const auto annotations = OptionFromTry<Annotations>([&json]() {
      return Annotations::from_json(json[std::string{"annotations"}]);
    });

    return Result(ContainerRuntimeManifest(
        *ac_version,
        *ac_kind,
        *uuid,
        *app_refs,
        volumes,
        isolators,
        annotations));
  }

  Status validate() const {
    return collect_status({
      ac_version.validate(),
      ac_kind.validate(),
      uuid.validate(),
      app_refs.validate(),
      validate_if_some(volumes),
      validate_if_some(isolators),
      validate_if_some(annotations),
    });
  }
};


} // namespace schema
} // namespace appc
