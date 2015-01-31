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
#include <map>

#include "appc/discovery/fetcher.h"
#include "appc/discovery/resolver.h"
#include "appc/util/namespace.h"
#include "appc/util/try.h"


namespace appc {
namespace discovery {


// A Strategy provides a resolver and a fetcher to be used together.  A Strategy may or may not be a
// factory for resolvers and fetchers.
class AbstractStrategy {
public:
  virtual ~AbstractStrategy() {}
  virtual std::shared_ptr<Resolver> get_resolver() = 0;
  virtual std::shared_ptr<Fetcher> get_fetcher() = 0;
};


class Strategy : AbstractStrategy {
private:
  std::shared_ptr<Resolver> resolver;
  std::shared_ptr<Fetcher> fetcher;
public:
  Strategy(Resolver* resolver, Fetcher* fetcher)
  : resolver(resolver),
    fetcher(fetcher) {}
  virtual ~Strategy() {}

  virtual std::shared_ptr<Resolver> get_resolver() {
    return resolver;
  }
  virtual std::shared_ptr<Fetcher> get_fetcher() {
    return fetcher;
  }
};


} // namespace discovery
} // namespace appc
