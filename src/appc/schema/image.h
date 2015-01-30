// Copyright 2015 Charles D. Aylward
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
//     http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.
//
// A (possibly updated) copy of of this software is available at
// https://github.com/cdaylward/libappc

#pragma once

#include "appc/schema/ac_kind.h"
#include "appc/schema/ac_version.h"
#include "appc/schema/annotations.h"
#include "appc/schema/app.h"
#include "appc/schema/app_name.h"
#include "appc/schema/dependencies.h"
#include "appc/schema/labels.h"
#include "appc/schema/path.h"
#include "appc/schema/path_whitelist.h"
#include "appc/schema/try_json.h"
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
    const auto ac_kind = try_from_json<AcKind>(json, "acKind");
    const auto ac_version = try_from_json<AcVersion>(json, "acVersion");
    const auto name = try_from_json<AppName>(json, "name");

    const auto labels = try_option_from_json<Labels>(json, "labels");
    const auto app = try_option_from_json<App>(json, "app");
    const auto dependencies = try_option_from_json<Dependencies>(json, "dependencies");
    const auto path_whitelist = try_option_from_json<PathWhitelist>(json, "pathWhitelist");
    const auto annotations = try_option_from_json<Annotations>(json, "annotations");

    if (!all_results(ac_kind, ac_version, name, labels, app,
                     dependencies, path_whitelist, annotations)) {
      return collect_failure_reasons<ImageManifest>(ac_kind, ac_version, name, labels, app,
                                                    dependencies, path_whitelist, annotations);
    }

    return Result(ImageManifest(from_result(ac_kind),
                                from_result(ac_version),
                                from_result(name),
                                from_result(labels),
                                from_result(app),
                                from_result(dependencies),
                                from_result(path_whitelist),
                                from_result(annotations)));
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
