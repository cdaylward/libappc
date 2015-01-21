#pragma once

#include "appc/schema/annotations.h"
#include "appc/schema/image_id.h"
#include "appc/schema/app_name.h"
#include "appc/schema/isolators.h"
#include "appc/schema/try_json.h"

#include "appc/util/try_option.h"


namespace appc {
namespace schema {


// AppRef is the CRM entity that refers to an existing app/image.
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
    const auto image_id = try_from_json<ImageID>(json, "imageID");

    const auto app_name = try_option_from_json<AppName>(json, "app");
    const auto isolators = try_option_from_json<Isolators>(json, "isolators");
    const auto annotations = try_option_from_json<Annotations>(json, "annotations");

    if (!SomeIfAll(image_id, app_name, isolators, annotations)) {
      return collect_failure_reasons<AppRef>(image_id, app_name, isolators, annotations);
    }

    return Result(AppRef(*image_id,
                         *app_name,
                         *isolators,
                         *annotations));
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
};


} // namespace schema
} // namespace appc
