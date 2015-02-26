// Copyright 2015 Charles D. Aylward
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
//     http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.
//
// A (possibly updated) copy of of this software is available at
// https://github.com/cdaylward/libappc

#pragma once

#include <vector>

#include "appc/discovery/strategy.h"
#include "appc/util/namespace.h"
#include "appc/util/try.h"


namespace appc {
namespace discovery {


// An ImageProvider uses a list of strategies, in order, to attempt to resolve and fetch an image.
// An ImageProvider returns the on-disk URI for the image provided by the first strategy that won.
class ImageProvider {
private:
  std::vector<Strategy> strategies{};
public:
  ImageProvider(const std::vector<Strategy>& strategies)
  : strategies(strategies) {}

  Try<URI> get(const Name& name, const Labels& labels) {
    // TODO validate name
    // validate labels
    for (auto& strategy : strategies) {
      auto uri = strategy.get_resolver()->resolve(name, labels);
      if (!uri) {
        std::cerr << uri.failure_reason() << std::endl;
        continue;
      }
      // TODO logging, ;)
      std::cerr << "Resolved: " << name << " -> " << from_result(uri) << std::endl;
      auto fetched = strategy.get_fetcher()->fetch(from_result(uri));
      if (!fetched) {
        std::cerr << "Fetch failed: " << fetched.failure_reason() << std::endl;
        continue;
      }
      std::cerr << "Fetched: " << from_result(uri) << std::endl;
      std::cerr << "Stored: " << from_result(fetched) << std::endl;
      return fetched;
    }
    return Failure<URI>("Could not retrieve " + name);
  }
};


} // namespace discovery
} // namespace appc
