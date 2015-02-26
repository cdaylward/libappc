#include <iostream>

#include "appc/discovery/provider.h"
#include "appc/discovery/strategy/local.h"
#include "appc/discovery/strategy/meta.h"
#include "appc/discovery/strategy/simple.h"


using namespace appc::discovery;


int main(int args, char** argv) {
  if (args < 2) {
    std::cerr << "Usage: " << argv[0] << " <app name>" << std::endl;
    std::cerr << "  e.g. " << argv[0] << " nosecone.net/example" << std::endl;
    return EXIT_FAILURE;
  }

  // Change these to something that exists.

  const Name name{argv[1]};

  const Labels labels{
    {"os", "linux"},
    {"version", "1.0.0"},
    {"arch", "amd64"}
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
