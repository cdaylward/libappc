#pragma once

#include "appc/schema/ackind.h"
#include "appc/schema/acversion.h"
#include "appc/schema/annotations.h"
#include "appc/schema/container_images.h"
#include "appc/schema/isolators.h"
#include "appc/schema/uuid.h"
#include "appc/schema/volumes.h"


namespace appc {
namespace schema {


struct ContainerRuntimeManifest {
  const AcVersion ac_version;
  const AcKind ac_kind;
  const UUID uuid;
  const Images images;
  const Volumes volumes;
  const Isolators isolators;
  const Annotations annotations;

  explicit ContainerRuntimeManifest(const AcVersion& ac_version,
                                    const AcKind& ac_kind,
                                    const UUID& uuid,
                                    const Images& images,
                                    const Volumes& volumes,
                                    const Isolators& isolators,
                                    const Annotations& annotations)
    : ac_version(ac_version),
      ac_kind(ac_kind),
      uuid(uuid),
      images(images),
      volumes(volumes),
      isolators(isolators),
      annotations(annotations) {}

  static Try<ContainerRuntimeManifest> from_json(const Json& json) {
    const Try<AcVersion> ac_version = AcVersion::from_json(json[std::string{"acVersion"}]);
    const Try<AcKind> ac_kind = AcKind::from_json(json[std::string{"acKind"}]);
    const Try<UUID> uuid = UUID::from_json(json[std::string{"uuid"}]);
    const Try<Images> images = Images::from_json(json[std::string{"apps"}]);
    const Try<Volumes> volumes = Volumes::from_json(json[std::string{"volumes"}]);
    const Try<Isolators> isolators = Isolators::from_json(json[std::string{"isolators"}]);
    const Try<Annotations> annotations = Annotations::from_json(json[std::string{"annotations"}]);

    return Result(ContainerRuntimeManifest(
        *ac_version,
        *ac_kind,
        *uuid,
        *images,
        *volumes,
        *isolators,
        *annotations));
  }

  Status validate() const {
    return Valid();
  }
};


} // namespace schema
} // namespace appc
