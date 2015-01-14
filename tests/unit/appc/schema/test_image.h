#pragma once

#include "gtest/gtest.h"

#include "appc/schema/image.h"

using namespace appc::schema;


TEST(ImageManifest, ImageManifest) {
  Json json = {{"acKind", "ImageManifest"}};
  std::shared_ptr<ImageManifest> result = ImageManifest::from_json(json);
  ASSERT_TRUE(result->validate());
}

