#pragma once

#include "appc/schema/annotations.h"
#include "appc/schema/image_id.h"
#include "appc/schema/image_name.h"
#include "appc/schema/isolators.h"
#include "appc/util/try_option.h"


namespace appc {
namespace schema {


struct AppRef : Type<AppRef> {
  const ImageName image_name;
  const ImageID image_id;
  const Option<Isolators> isolators;
  const Option<Annotations> annotations;

  explicit AppRef(const ImageName& image_name,
                  const ImageID& image_id,
                  const Option<Isolators>& isolators,
                  const Option<Annotations>& annotations)
  : image_name(image_name),
    image_id(image_id),
    isolators(isolators),
    annotations(annotations) {}

  static Try<AppRef> from_json(const Json& json) {
    const auto image_name = TryFlatten<ImageName>([&json]() {
      return ImageName::from_json(json[std::string{"app"}]);
    });
    const auto image_id = TryFlatten<ImageID>([&json]() {
      return ImageID::from_json(json[std::string{"imageID"}]);
    });

    if (!SomeIfAll(image_name, image_id)) {
      return collect_failure_reasons<AppRef>(image_name, image_id);
    }

    const Option<Isolators> isolators = OptionFromTry<Isolators>( [&json]() {
      return Isolators::from_json(json[std::string{"isolators"}]);
    });
    const Option<Annotations> annotations = OptionFromTry<Annotations>( [&json]() {
      return Annotations::from_json(json[std::string{"annotations"}]);
    });

    return Result(AppRef(*image_name,
                         *image_id,
                         isolators,
                         annotations));
  }

  Status validate() const {
    return Valid();
  }
};


struct AppRefs : ArrayType<AppRefs, AppRef> {
  explicit AppRefs(const std::vector<AppRef>& images)
  : ArrayType<AppRefs, AppRef>(images) {}

  Status validate() const {
    // TODO(cdaylward)
    return Valid();
  }
};


} // namespace schema
} // namespace appc
