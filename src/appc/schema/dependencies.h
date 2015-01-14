#pragma once

#include "appc/schema/common.h"


namespace appc {
namespace schema {


struct Dependency : Type<Dependency> {
  //const AppName appName;
  //const ImageID imageID;
  //const Labels labels;
  //const Root root;
  //explicit Dependency(const AppName& appName,
  //                    const ImageID& imageID,
  //                    const Labels& labels,
  //                    const Root& root)
  //: appName(appName),
  //  imageID(imageID),
  //  labels(labels),
  //  root(root) {}

  //static Try<Dependency> from_json(const picojson::value json) {
  //  picojson::value::object dep { json.get<picojson::value::object>() };
  //  const Try<AppName> appName { AppName::from_json(dep["app"]) };
  //  const Try<ImageID> imageID { ImageID::from_json(dep["imageID"]) };
  //  const Try<Labels> labels { Labels::from_json(dep["labels"]) };
  //  const Try<Root> root { Root::from_json(dep["root"]) };
  //  return Success(Dependency(appName(),
  //                            imageID(),
  //                            labels(),
  //                            root()));
  //}
  static Try<Dependency> from_json(const Json& json) {
    return Result(Dependency());
  }

  Status validate() const {
    return Valid();
  }
};


struct Dependencies : ArrayType<Dependencies, Dependency> {
  explicit Dependencies(const std::vector<Dependency>& dependencies)
  : ArrayType<Dependencies, Dependency>(dependencies) {}

  Status validate() const {
    // TODO(cdaylward)
    return Valid();
  }
};


} // namespace schema
} // namespace appc
