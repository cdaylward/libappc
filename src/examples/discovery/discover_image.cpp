#include <iostream>

#include "appc/discovery/strategy/local.h"
#include "appc/discovery/strategy/meta.h"
#include "appc/discovery/strategy/simple.h"


using namespace appc::discovery;


int main(int args, char** argv) {

  // Change these to something that exists.

  Name name{"example.com/worker"};

  Labels labels{
    {"os", "linux"},
    {"version", "0.0.1"},
    {"arch", "x86_64"}
  };

  // By configuring the local strategy and the simple strategy with the same
  // storage location, the local strategy functions as a cache.
  // (currently, the directory in the base uri must exist)

  const auto local_strategy = strategy::local::StrategyBuilder()
                                .with_storage_base_uri("file:///tmp/images")
                                .build();

  const auto simple_strategy = strategy::simple::StrategyBuilder()
                                 .with_storage_base_uri("file:///tmp/images")
                                 .build();

  auto provider = ImageProvider({
    from_result(local_strategy),
    from_result(simple_strategy)
  });

  const auto image_location = provider.get(name, labels);

  if (!image_location) {
    std::cerr << "Failed to retrieve image for " << name << std::endl;
    return EXIT_FAILURE;
  }

  std::cout << from_result(image_location) << std::endl;

  return EXIT_SUCCESS;
}
