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

#include "appc/util/namespace.h"
#include "appc/util/try.h"


namespace appc {
namespace discovery {


// TODO WIP place holders
using URI = std::string;
using Path = std::string;
using Name = std::string;
using Labels = std::map<std::string, std::string>;

// TODO still up in the air in the spec
const std::string aci_ext = ".aci";
const std::string sig_ext = ".aci.sig";

const URI file_prefix{"file://"}; // ignoring file://localhost/ for now
const URI https_prefix{"https://"};


bool valid_prefix(const URI& prefix, const URI& uri) {
  if (uri.length() <= prefix.length() ||
      uri.compare(0, prefix.length(), prefix) != 0) {
    return false;
  }
  return true;
}


Try<std::string> render_aci_name(const Name& name, const Labels& labels) {
  auto not_found = labels.end();
  if (labels.find("version") == not_found) return Failure<std::string>("version label required");
  if (labels.find("os") == not_found) return Failure<std::string>("os label required");
  if (labels.find("arch") == not_found) return Failure<std::string>("arch label required");
  return Result(ns::join("-", name, labels.at("version"), labels.at("os"), labels.at("arch")) + aci_ext);
}


// A Resolver takes a name and labels and renders a URI for the image.
// The resolver may or may not use network services to determine the
// URI (as is the case with meta discovery). The returned URI may
// be local or remote.
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


// A Fetcher takes a possibly remote URI (generally produced from a
// resolver), performs some action to retrieve it and store it, and
// returns a URI to the local location of the image (on disk).
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


// A Strategy provides a resolver and a fetcher to be used together.
// A Strategy may or may not be a factory for resolvers and fetchers.
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


// An ImageProvider uses a list of strategies, in order, to attempt
// to resolve and fetch an image. An ImageProvider returns the first
// image to be successfully resolved and fetched.
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
      std::cerr << "Fetched: " << from_result(fetched) << std::endl;
      return fetched;
    }
    return Failure<URI>("Could not retrieve " + name);
  }
};


} // namespace discovery
} // namespace appc
