#pragma once

#include "appc/schema/annotations.h"
#include "appc/schema/image_id.h"
#include "appc/schema/image_name.h"
#include "appc/schema/isolators.h"
#include "appc/util/try_option.h"


namespace appc {
namespace schema {


struct Image : Type<Image> {
  const ImageName image_name;
  const ImageID image_id;
  const Option<Isolators> isolators;
  const Option<Annotations> annotations;

  explicit Image(const ImageName& image_name,
                 const ImageID& image_id,
                 const Option<Isolators>& isolators,
                 const Option<Annotations>& annotations)
  : image_name(image_name),
    image_id(image_id),
    isolators(isolators),
    annotations(annotations) {}

  static Try<Image> from_json(const Json& json) {
    // TODO check these
    const Try<ImageName> image_name = ImageName::from_json(json[std::string{"app"}]);
    const Try<ImageID> image_id = ImageID::from_json(json[std::string{"imageID"}]);
    const Option<Isolators> isolators = OptionFromTry<Isolators>( [&json]() {
      return Isolators::from_json(json[std::string{"isolators"}]);
    });
    const Option<Annotations> annotations = OptionFromTry<Annotations>( [&json]() {
      return Annotations::from_json(json[std::string{"annotations"}]);
    });

    return Result(Image(*image_name,
                        *image_id,
                        isolators,
                        annotations));
  }

  Status validate() const {
    return Valid();
  }
};


struct Images : ArrayType<Images, Image> {
  explicit Images(const std::vector<Image>& images)
  : ArrayType<Images, Image>(images) {}

  Status validate() const {
    // TODO(cdaylward)
    return Valid();
  }
};


} // namespace schema
} // namespace appc
