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

#include <cerrno>
#include <cstring>
#include <sys/stat.h>

#include "3rdparty/cdaylward/pathname.h"

#include "appc/discovery/aci_name.h"
#include "appc/discovery/strategy.h"
#include "appc/util/namespace.h"
#include "appc/util/option.h"
#include "appc/util/status.h"
#include "appc/util/try.h"


namespace appc {
namespace discovery {
namespace strategy {
namespace local {


// Local Discovery - networkless lookup (basic file system lookup)


// Given a name and labels, render a local location for the common
// (simple) ACI name: <storage base path>/{name}-{version}-{os}-{arch}.aci
class Resolver : public appc::discovery::Resolver {
private:
  class Impl : public appc::discovery::AbstractResolver {
  private:
    const URI base_uri;
  public:
    Impl(const URI& base_uri)
    : base_uri(base_uri) {}

    virtual Try<URI> resolve(const Name& name, const Labels& labels) {
      const auto aci_name = render_aci_name(name, labels);
      if (!aci_name) return Failure<URI>(aci_name.failure_reason());
      return Result(ns::join("/", base_uri, from_result(aci_name)));
    }
  };
public:
  Resolver(const URI& base_uri)
  : appc::discovery::Resolver(new Impl(base_uri)) {}
};


// A Fetcher generally turns a remote uri into a local uri
// (Retrieve an image and return its on-disk location)
// Here we check if it exists.
class Fetcher : public appc::discovery::Fetcher {
private:
  class Impl : public appc::discovery::AbstractFetcher {
  public:
    virtual Try<URI> fetch(const URI& uri) {
      if (!valid_prefix(file_prefix, uri)) {
        return Failure<URI>("URI must begin with " + file_prefix + ", cannot fetch " + uri);
      }
      Path path = uri.substr(file_prefix.length());
      if (!pathname::is_absolute(path)) {
        return Failure<URI>("URI did not contain absolute path, will not fetch " + path);
      }
      struct stat path_stat;
      if (stat(path.c_str(), &path_stat) != 0) {
        return Failure<URI>(uri + " " + strerror(errno));
      }
      return Result(uri);
    }
  };
public:
  Fetcher()
  : appc::discovery::Fetcher(new Impl()) {}
};


class StrategyBuilder {
private:
  const URI base_uri;
public:
  StrategyBuilder(const URI& base_uri = "")
  : base_uri(base_uri) {}

  StrategyBuilder with_storage_base_uri(const URI& base_uri) {
    return StrategyBuilder(base_uri);
  }

  Try<appc::discovery::Strategy> build() {
    if (!valid_prefix(file_prefix, base_uri)) {
      return Failure<appc::discovery::Strategy>(
        "storage_base_uri must begin with " + file_prefix + ", is " + base_uri);
    }
    return Result(appc::discovery::Strategy(new Resolver(base_uri),
                                            new Fetcher()));
  }
};


} // namespace local
} // namespace strategy
} // namespace discovery
} // namespace appc
