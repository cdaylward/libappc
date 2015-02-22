#include "gtest/gtest.h"

#include "appc/schema/ports.h"
#include "appc/util/option.h"

using namespace appc::schema;


static const Json good_json = R"(
  [
        {
            "name": "health",
            "port": 4000,
            "protocol": "tcp",
            "socketActivated": true
        }
  ]
)"_json;


TEST(Ports, from_json) {
  auto result = Ports::from_json(good_json);
  ASSERT_TRUE(result);
  // Asserting socketActivated option is Some
  std::vector<Port> ports = *result;
  ASSERT_TRUE(ports[0].socket_activated);
  ASSERT_TRUE(result->validate());
}

TEST(PortNumber, valid_range) {
  ASSERT_FALSE(PortNumber::from_json(0)->validate());
  ASSERT_TRUE(PortNumber(40000).validate());
  ASSERT_FALSE(PortNumber(65536).validate());
}

TEST(Protocol, valid_type) {
  ASSERT_TRUE(Protocol::from_json("udp")->validate());
  ASSERT_TRUE(Protocol::from_json("tcp")->validate());
  ASSERT_FALSE(Protocol::from_json("icmp")->validate());
}
