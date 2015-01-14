#pragma once

#include "appc/schema/ackind.h"
#include "appc/schema/acversion.h"
#include "appc/schema/annotations.h"
#include "appc/schema/app.h"
#include "appc/schema/dependencies.h"
#include "appc/schema/imagename.h"
#include "appc/schema/labels.h"
#include "appc/schema/path_whitelist.h"


namespace appc {
namespace schema {


struct ImageManifest : Type<ImageManifest> {
  const AcKind ac_kind;
  const AcVersion ac_version;
  const ImageName image_name;
  const Labels labels;
  const App app;
  const Dependencies dependencies;
  const PathWhitelist path_whitelist;
  const Annotations annotations;

  explicit ImageManifest(const AcKind& ac_kind,
                         const AcVersion& ac_version,
                         const ImageName& image_name,
                         const Labels& labels,
                         const App& app,
                         const Dependencies& dependencies,
                         const PathWhitelist& path_whitelist,
                         const Annotations& annotations)
  : ac_kind(ac_kind),
    ac_version(ac_version),
    image_name(image_name),
    labels(labels),
    app(app),
    dependencies(dependencies),
    path_whitelist(path_whitelist),
    annotations(annotations) {}

  static Try<ImageManifest> from_json(const Json& json) {
    if (json.type() != Json::value_type::object) {
      return Failure<ImageManifest>("Image manifest must be a JSON object.");
    }
    try {
    const Try<AcKind> ac_kind = AcKind::from_json(json[std::string{"acKind"}]);
    const Try<AcVersion> ac_version = AcVersion::from_json(json[std::string{"acVersion"}]);
    const Try<ImageName> image_name = ImageName::from_json(json[std::string{"name"}]);
    const Try<Labels> labels = Labels::from_json(json[std::string{"labels"}]);
    const Try<App> app = App::from_json(json[std::string{"app"}]);
    const Try<Dependencies> dependencies = Dependencies::from_json(json[std::string{"dependencies"}]);
    const Try<PathWhitelist> path_whitelist = PathWhitelist::from_json(json[std::string{"pathWhitelist"}]);
    const Try<Annotations> annotations = Annotations::from_json(json[std::string{"annotations"}]);
    return Result(ImageManifest(*ac_kind,
                                *ac_version,
                                *image_name,
                                *labels,
                                *app,
                                *dependencies,
                                *path_whitelist,
                                *annotations));
    } catch (const std::exception& err) {
      return Failure<ImageManifest>(err.what());
    }
  }

  Status validate() const {
    // TODO(cdaylward)
    return ac_kind.validate();
  }
};


} // namespace schema
} // namespace appc
