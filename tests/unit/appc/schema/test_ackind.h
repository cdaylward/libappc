#pragma once

#include "gtest/gtest.h"

#include "appc/schema/ackind.h"

using namespace appc::schema;


TEST(AcKind, ImageManifest) {
  Json json = "ImageManifest";
  std::shared_ptr<AcKind> result = AcKind::from_json(json);
  ASSERT_TRUE(result->validate());
}

TEST(AcKind, ContainerRuntimeManifest) {
  Json json = "ContainerRuntimeManifest";
  std::shared_ptr<AcKind> result = AcKind::from_json(json);
  ASSERT_TRUE(result->validate());
}

TEST(AcKind, neither) {
  Json json = "Neither";
  std::shared_ptr<AcKind> result = AcKind::from_json(json);
  ASSERT_FALSE(result->validate());
}
