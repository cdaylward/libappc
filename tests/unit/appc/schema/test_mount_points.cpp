#include "gtest/gtest.h"

#include "appc/schema/mount_points.h"
#include "appc/util/option.h"

using namespace appc::schema;


static const Json good_json = R"(
  [
      {
         "name": "database",
         "path": "/var/mnt/db",
         "readOnly": true
      }
  ]
)"_json;

static const Json good_min_json = R"(
  [
      {
         "name": "database",
         "path": "/var/mnt/db"
      }
  ]
)"_json;

static const Json bad_path_json = R"(
  [
      {
         "name": "database",
         "path": "var/mnt/db",
         "readOnly": true
      }
  ]
)"_json;

static const Json duplicate_json = R"(
  [
      {
         "name": "database",
         "path": "/var/mnt/db"
      },
      {
         "name": "database",
         "path": "/var/mnt/db"
      }
  ]
)"_json;


TEST(MountPoints, from_json) {
  auto result = MountPoints::from_json(good_json);
  ASSERT_TRUE(result);
  // Asserting readOnly option is Some
  std::vector<MountPoint> mp = *result;
  ASSERT_TRUE(mp[0].read_only);
  ASSERT_TRUE(result->validate());

  auto result2 = MountPoints::from_json(good_min_json);
  ASSERT_TRUE(result2);
  mp = *result2;
  // Asserting readOnly option is None
  ASSERT_FALSE(mp[0].read_only);
  ASSERT_TRUE(result2->validate());
}


TEST(MountPoints, path_is_absolute) {
  auto result = MountPoints::from_json(bad_path_json);
  ASSERT_TRUE(result);
  ASSERT_FALSE(result->validate());
}


TEST(MountPoints, disallow_duplicates) {
  auto result = MountPoints::from_json(duplicate_json);
  ASSERT_TRUE(result);
  ASSERT_FALSE(result->validate());
}
