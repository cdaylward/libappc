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


// A Resolver takes a name and labels and renders a URI for the image.  The resolver may or may not
// use network services to determine the URI (as is the case with meta discovery). The returned URI
// may be local or remote.
class AbstractResolver {
public:
  virtual ~AbstractResolver() {}
  virtual Try<URI> resolve(const Name& name, const Labels& labels) = 0;
};


class Resolver : public AbstractResolver {
private:
  const std::shared_ptr<AbstractResolver> impl;
public:
  Resolver(AbstractResolver* impl)
  : impl(impl) {}

  virtual ~Resolver() {}
  virtual Try<URI> resolve(const Name& name, const Labels& labels) {
    return impl->resolve(name, labels);
  }
};


} // namespace discovery
} // namespace appc
