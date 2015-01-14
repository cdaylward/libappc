#pragma once

#include "gtest/gtest.h"

#include "appc/schema/acversion.h"

using namespace appc::schema;


TEST(AcVersion, 0_0_0) {
  Json json = "0.0.0";
  std::shared_ptr<AcVersion> result = AcVersion::from_json(json);
  ASSERT_TRUE(result->validate());
}

TEST(AcVersion, 1_0_0_prerelease) {
  Json json = "1.0.0-rc5";
  std::shared_ptr<AcVersion> result = AcVersion::from_json(json);
  ASSERT_TRUE(result->validate());
}

TEST(AcVersion, 1_0_0_empty) {
  Json json = "1.0.0-";
  std::shared_ptr<AcVersion> result = AcVersion::from_json(json);
  ASSERT_FALSE(result->validate());
}

TEST(AcVersion, 1_0_0_meta) {
  Json json = "1.0.0+meta.data";
  std::shared_ptr<AcVersion> result = AcVersion::from_json(json);
  ASSERT_TRUE(result->validate());
}

TEST(AcVersion, 1_0_0_bad_meta) {
  Json json = "1.0.0+meta_data";
  std::shared_ptr<AcVersion> result = AcVersion::from_json(json);
  ASSERT_FALSE(result->validate());
}


TEST(AcVersion, 1_0_0_prerelease_meta) {
  Json json = "1.0.0-rc1+meta.data";
  std::shared_ptr<AcVersion> result = AcVersion::from_json(json);
  ASSERT_TRUE(result->validate());
}
