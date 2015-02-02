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

#include <cstdlib> // REM remove when system() hack is removed.

#include "appc/discovery/aci_name.h"
#include "appc/discovery/strategy.h"
#include "appc/util/namespace.h"
#include "appc/util/status.h"
#include "appc/util/try.h"


namespace appc {
namespace discovery {
namespace strategy {
namespace simple {


// Simple Discovery - WIP, just testing structure, uses curl! no safety!
//
// First, try to fetch the app container image by rendering the following template
// and directly retrieving the resulting URL:
//
// https://{name}-{version}-{os}-{arch}.aci
//
// For example, given the app name example.com/reduce-worker, with version 1.0.0,
// arch amd64, and os linux, try to retrieve:
//
// https://example.com/reduce-worker-1.0.0-linux-amd64.aci
//
// If this fails, move on to meta discovery. If this succeeds, try fetching the
// signature using the same template but with a .sig extension:
//
// https://example.com/reduce-worker-1.0.0-linux-amd64.sig


class Resolver : public appc::discovery::Resolver {
private:
  class Impl : public appc::discovery::AbstractResolver {
  public:
    virtual Try<URI> resolve(const Name& name, const Labels& labels) {
      const auto aci_name = render_aci_name(name, labels);
      if (!aci_name) return Failure<URI>(aci_name.failure_reason());
      return Result(https_prefix + from_result(aci_name));
    }
  };
public:
  Resolver()
  : appc::discovery::Resolver(new Impl()) {}
};


// fetch(remote uri) -> local uri (location after fetched)
class Fetcher : public appc::discovery::Fetcher {
private:
  class Impl : public appc::discovery::AbstractFetcher {
  private:
    const Path base_path;
  public:
    Impl(const Path& base_path)
    : base_path(base_path) {}

    virtual Try<URI> fetch(const URI& uri) {
      if (!valid_prefix(https_prefix, uri)) {
        return Failure<URI>("URI did is not HTTPS, will not fetch " + uri);
      }
      // Create directory for app distributor (/<base_path>/example.com)
      // TODO make recursive, make safe.
      const Path full_path = pathname::join(base_path, uri.substr(https_prefix.length()));
      const Path storage_dir = pathname::dir(full_path);
      if (!pathname::is_absolute(storage_dir) || pathname::has_dot_dot(storage_dir)) {
        return Failure<URI>("URI did not contain absolute path, will not store " + storage_dir);
      }
      struct stat dir_stat;
      if (stat(storage_dir.c_str(), &dir_stat) != 0) {
        mkdir(storage_dir.c_str(), 0755);
      }
      // Fetch
      // TODO obvious hack is obvious.
      std::string exec{"/usr/bin/curl -f -o "};
      exec += full_path + " -- " + uri;
      if (system(exec.c_str()) != 0) {
        return Failure<URI>("Failed to fetch " + uri);
      }

      return Result(file_prefix + full_path);
    }
  };
public:
  Fetcher(const Path& base_path)
  : appc::discovery::Fetcher(new Impl(base_path)) {}
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

  Try<Strategy> build() {
    if (!valid_prefix(file_prefix, base_uri)) {
      return Failure<Strategy>(
        "storage_base_uri must begin with " + file_prefix + ", is " + base_uri);
    }
    // TODO obvious cleanup
    Path path = base_uri.substr(file_prefix.length());
    return Result(Strategy(new Resolver(),
                           new Fetcher(path)));
  }
};


} // namespace simple
} // namespace strategy
} // namespace discovery
} // namespace appc
