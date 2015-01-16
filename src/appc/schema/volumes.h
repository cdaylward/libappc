#pragma once

#include "appc/schema/common.h"


namespace appc {
namespace schema {

const std::string fulfills_field{"fulfills"};
const std::string kind_field{"kind"};
const std::string read_only_field{"readOnly"};
const std::string source_field{"source"};

// TODO(cdaylward) clean up types
struct Volume : Type<Volume> {
  const std::string kind;
  const std::string source;
  const bool readOnly;
  std::vector<std::string> fulfills;

  explicit Volume(const std::string& kind,
                  const std::string& source,
                  const bool readOnly,
                  const std::vector<std::string>& fulfills)
    : kind(kind),
      source(source),
      readOnly(readOnly),
      fulfills(fulfills) {}

  static Try<Volume> from_json(const Json& json) {
    std::vector<std::string> fulfills {};
    for (auto& target : json[fulfills_field]) {
      fulfills.push_back(target.get<std::string>());
    }
    bool readOnly { false };
    if (json.find(read_only_field) != json.end()) {
      readOnly = json[read_only_field].get<bool>();
    }
    std::string source {};
    if (json.find(source_field) != json.end()) {
      source = json[source_field].get<std::string>();;
    }
    return Result(Volume(json[kind_field].get<std::string>(),
                         source,
                         readOnly,
                         fulfills));
  }

  Status validate() const {
    return Valid();
  }
};


struct Volumes : ArrayType<Volumes, Volume> {
  explicit Volumes(const std::vector<Volume> array)
  : ArrayType<Volumes, Volume>(array) {}

  Status validate() const {
    // TODO
    return Valid();
  }
};


} // namespace schema
} // namespace appc
