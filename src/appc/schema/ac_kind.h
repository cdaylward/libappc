#pragma once

#include "appc/schema/common.h"


namespace appc {
namespace schema {


struct AcKind : StringType<AcKind> {
  const std::string image_manifest { "ImageManifest" };
  const std::string runtime_manifest { "ContainerRuntimeManifest" };

  explicit AcKind(const std::string& kind)
  : StringType<AcKind>(kind) {}

  Status validate() const {
    if (value == image_manifest || value == runtime_manifest) {
      return Valid();
    }
    return Invalid("AcKind must be either " + image_manifest + " or " + runtime_manifest);
  }
};


} // namespace schema
} // namespace appc
