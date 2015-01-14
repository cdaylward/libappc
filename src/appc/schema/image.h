#pragma once

#include "appc/schema/ackind.h"
#include "appc/schema/acversion.h"
#include "appc/schema/imagename.h"
#include "appc/schema/labels.h"

namespace appc {
namespace schema {


struct ImageManifest : Type<ImageManifest> {
  const AcKind ac_kind;
  const AcVersion ac_version;
  const ImageName image_name;
  const Labels labels;
  //const ImageApp app;
  //const Dependencies dependencies;
  //const PathWhitelist pathWhitelist;
  //const Annotations annotations;

  explicit ImageManifest(const AcKind& ac_kind)
                         const AcVersion& ac_version,
                         const ImageName& image_name,
                         const Labels& labels);
                         //const ImageApp& app,
                         //const Dependencies& dependencies,
                         //const PathWhitelist& pathWhitelist,
                         //const Annotations& annotations)
   : ac_kind(ac_kind),
     ac_version(ac_version),
     image_name(image_name),
     labels(labels) {}
     //app(app),
     //dependencies(dependencies),
     //pathWhitelist(pathWhitelist),
     //annotations(annotations) {}

  static Try<ImageManifest> from_json(const Json& json) {
    if (json.type() != Json::value_type::object) {
      return Failure<ImageManifest>("Image manifest must be a JSON object.");
    }
    const Try<AcKind> ac_kind = AcKind::from_json(json[std::string{"acKind"}]);
    const Try<AcVersion> ac_version = AcVersion::from_json(json[std::string{"ac_version"}]);
    const Try<ImageName> image_name = ImageName::from_json(json[std::string{"name"}]);
    const Try<Labels> labels = Labels::from_json(json[std::string{"labels"}]);
    //const Try<ImageApp> imageApp { ImageApp::from_json(manifest["app"]) };
    //const Try<Dependencies> dependencies { Dependencies::from_json(manifest["dependencies"]) };
    //const Try<PathWhitelist> pathWhitelist { PathWhitelist::from_json(manifest["pathWhitelist"]) };
    //const Try<Annotations> annotations { Annotations::from_json(manifest["annotations"]) };
      return Result(ImageManifest(*ac_kind,
                                  *ac_version,
                                  *image_name,
                                  *labels));
                           //ac_version(),
                           //imageApp(),
                           //labels(),
                           //dependencies(),
                           //pathWhitelist(),
                           //annotations());
  }
  Status validate() const {
    return ac_kind.validate();
  }
};


} // namespace schema
} // namespace appc
