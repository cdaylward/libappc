#pragma once

#include "appc/schema/app_name.h"
#include "appc/schema/image_id.h"
#include "appc/schema/image_name.h"
#include "appc/schema/labels.h"


namespace appc {
namespace schema {



struct Dependency : Type<Dependency> {
  const AppName app_name;
  const ImageID image_id;
  const Labels labels;

  explicit Dependency(const AppName& app_name,
                      const ImageID& image_id,
                      const Labels& labels)
  : app_name(app_name),
    image_id(image_id),
    labels(labels) {}

  static Try<Dependency> from_json(const Json& json) {
    // TODO check these
    const Try<AppName> app_name = AppName::from_json(json[std::string{"app"}]);
    const Try<ImageID> image_id = ImageID::from_json(json[std::string{"imageID"}]);
    const Try<Labels> labels = Labels::from_json(json[std::string{"labels"}]);
    return Result(Dependency(*app_name,
                             *image_id,
                             *labels));
  }

  Status validate() const {
    return Valid();
  }
};


struct Dependencies : ArrayType<Dependencies, Dependency> {
  explicit Dependencies(const std::vector<Dependency>& dependencies)
  : ArrayType<Dependencies, Dependency>(dependencies) {}
};


} // namespace schema
} // namespace appc
