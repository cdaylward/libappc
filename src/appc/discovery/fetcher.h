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

#include "appc/discovery/types.h"
#include "appc/util/namespace.h"
#include "appc/util/try.h"


namespace appc {
namespace discovery {


// A Fetcher takes a possibly remote URI (generally produced from a resolver), performs some action
// to retrieve it and store it, and returns a URI to the local location of the image (on disk).
class AbstractFetcher {
public:
  virtual ~AbstractFetcher() {}
  virtual Try<URI> fetch(const URI& uri) = 0;
};


class Fetcher : public AbstractFetcher {
private:
  const std::shared_ptr<AbstractFetcher> impl;
public:
  Fetcher(AbstractFetcher* impl)
  : impl(impl) {}
  virtual ~Fetcher() {}
  virtual Try<URI> fetch(const URI& uri) {
    return impl->fetch(uri);
  }
};


} // namespace discovery
} // namespace appc
