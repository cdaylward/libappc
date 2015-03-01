# Discovery

## Overview

Discovery is structured as a set of strategies (e.g. the two from the App
Container specification are Simple, and Meta). Each strategy has a resolver
and a fetcher. A resolver takes an app name + labels and returns a URI where
the image is located. A fetcher takes a URI, performs the necessary action to
retrieve the image, stores it, and returns a URI of its on-disk location.
Strategies can be mixed-and-matched or chained together by providing them to
an image provider which wraps iterating through the strategies. An image
provider returns the on-disk URI for an image provided by the first strategy
that won.

## Example Use

```c++
Name name{"example.com/worker"};

Labels labels{
  {"os", "linux"},
  {"version", "1.0.0"},
  {"arch", "amd64"}
};

// By configuring the local strategy and the simple strategy with the same
// storage location, the local strategy functions as a cache.

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
```
