#pragma once

#include "appc/schema/annotations.h"
#include "appc/schema/image_id.h"
#include "appc/schema/app_name.h"
#include "appc/schema/isolators.h"
#include "appc/util/try_option.h"


namespace appc {
namespace schema {


struct AppRef : Type<AppRef> {
  const ImageID image_id;
  const Option<AppName> app_name;
  const Option<Isolators> isolators;
  const Option<Annotations> annotations;

  explicit AppRef(const ImageID& image_id,
                  const Option<AppName>& app_name,
                  const Option<Isolators>& isolators,
                  const Option<Annotations>& annotations)
  : image_id(image_id),
    app_name(app_name),
    isolators(isolators),
    annotations(annotations) {}

  static Try<AppRef> from_json(const Json& json) {
    const auto image_id = TryFlatten<ImageID>([&json]() {
      return ImageID::from_json(json[std::string{"imageID"}]);
    });

    if (!image_id) {
      return Failure<AppRef>("An app requires an imageID.");
    }

    const auto app_name = OptionFromTry<AppName>([&json]() {
      return AppName::from_json(json[std::string{"app"}]);
    });
    const auto isolators = OptionFromTry<Isolators>([&json]() {
      return Isolators::from_json(json[std::string{"isolators"}]);
    });
    const auto annotations = OptionFromTry<Annotations>([&json]() {
      return Annotations::from_json(json[std::string{"annotations"}]);
    });

    return Result(AppRef(*image_id,
                         app_name,
                         isolators,
                         annotations));
  }

  Status validate() const {
    return collect_status({
      image_id.validate(),
      validate_if_some(app_name),
      validate_if_some(isolators),
      validate_if_some(annotations)
    });
  }
};


struct AppRefs : ArrayType<AppRefs, AppRef> {
  explicit AppRefs(const std::vector<AppRef>& images)
  : ArrayType<AppRefs, AppRef>(images) {}

  Status validate() const {
    for (const auto& app_ref : array) {
      auto valid = app_ref.validate();
      if (!valid) {
        return valid;
      }
    }
    return Valid();
  }
};


} // namespace schema
} // namespace appc
