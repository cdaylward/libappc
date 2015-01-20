#pragma once

#include "appc/schema/ac_kind.h"
#include "appc/schema/ac_version.h"
#include "appc/schema/annotations.h"
#include "appc/schema/app_name.h"
#include "appc/schema/app.h"
#include "appc/schema/dependencies.h"
#include "appc/schema/labels.h"
#include "appc/schema/path.h"
#include "appc/schema/path_whitelist.h"

#include "appc/util/try.h"
#include "appc/util/try_option.h"

namespace appc {
namespace schema {


struct ImageManifest : Type<ImageManifest> {
  const AcKind ac_kind;
  const AcVersion ac_version;
  const AppName name;
  const Option<Labels> labels;
  const Option<App> app;
  const Option<Dependencies> dependencies;
  const Option<PathWhitelist> path_whitelist;
  const Option<Annotations> annotations;

  explicit ImageManifest(const AcKind& ac_kind,
                         const AcVersion& ac_version,
                         const AppName& name,
                         const Option<Labels>& labels,
                         const Option<App>& app,
                         const Option<Dependencies>& dependencies,
                         const Option<PathWhitelist>& path_whitelist,
                         const Option<Annotations>& annotations)
  : ac_kind(ac_kind),
    ac_version(ac_version),
    name(name),
    labels(labels),
    app(app),
    dependencies(dependencies),
    path_whitelist(path_whitelist),
    annotations(annotations) {}

  static Try<ImageManifest> from_json(const Json& json) {
    const auto ac_kind = TryFlatten<AcKind>([&json]() {
      return AcKind::from_json(json[std::string{"acKind"}]);
    });
    const auto ac_version = TryFlatten<AcVersion>([&json]() {
      return AcVersion::from_json(json[std::string{"acVersion"}]);
    });
    const auto name = TryFlatten<AppName>([&json]() {
      return AppName::from_json(json[std::string{"name"}]);
    });

    if (!SomeIfAll(ac_kind, ac_version, name)) {
      return collect_failure_reasons<ImageManifest>(ac_kind, ac_version, name);
    }

    const auto labels = OptionFromTry<Labels>([&json]() {
      return Labels::from_json(json[std::string{"labels"}]);
    });
    const auto app = OptionFromTry<App>([&json]() {
      return App::from_json(json[std::string{"app"}]);
    });
    const auto dependencies = OptionFromTry<Dependencies>([&json]() {
      return Dependencies::from_json(json[std::string{"dependencies"}]);
    });
    const auto path_whitelist = OptionFromTry<PathWhitelist>([&json]() {
      return PathWhitelist::from_json(json[std::string{"pathWhitelist"}]);
    });
    const auto annotations = OptionFromTry<Annotations>([&json]() {
      return Annotations::from_json(json[std::string{"annotations"}]);
    });

    return Result(ImageManifest(*ac_kind,
                                *ac_version,
                                *name,
                                labels,
                                app,
                                dependencies,
                                path_whitelist,
                                annotations));
  }

  Status validate() const {
    return collect_status({
      ac_kind.validate(),
      ac_version.validate(),
      name.validate(),
      validate_if_some(labels),
      validate_if_some(app),
      validate_if_some(dependencies),
      validate_if_some(path_whitelist),
      validate_if_some(annotations)
    });
  }
};


} // namespace schema
} // namespace appc
