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

#include <set>

#include "appc/schema/ac_name.h"
#include "appc/schema/common.h"
#include "appc/schema/exec.h"
#include "appc/schema/try_json.h"
#include "appc/util/try_option.h"


namespace appc {
namespace schema {


struct EventName : ACName<EventName> {
  explicit EventName(const std::string& name)
  : ACName<EventName>(name) {}

  bool operator<(const EventName& rhs) const {
    return value < rhs.value;
  }
};


struct EventHandler : Type<EventHandler> {
  const EventName name;
  const Exec exec;

  explicit EventHandler(const EventName& name,
                        const Exec& exec)
  : name(name),
    exec(exec) {}

  static Try<EventHandler> from_json(const Json& json) {
    const auto exec = try_from_json<Exec>(json, "exec");
    const auto event_name = try_from_json<EventName>(json, "name");

    if (!all_results(exec, event_name)) {
      return collect_failure_reasons<EventHandler>(exec, event_name);
    }

    return Result(EventHandler(from_result(event_name),
                               from_result(exec)));
  }

  Status validate() const {
    return collect_status({name.validate(),
                           exec.validate()});
  }
};


struct EventHandlers : ArrayType<EventHandlers, EventHandler> {
  explicit EventHandlers(const std::vector<EventHandler>& args)
  : ArrayType<EventHandlers, EventHandler>(args) {}

  operator std::map<EventName, Exec>() const {
    std::map<EventName, Exec> map;
    for (auto& handler : array) {
      map.emplace(handler.name, handler.exec);
    }
    return map;
  }

  virtual Status validate() const {
    std::set<EventName> seen;
    for (const auto& handler : this->array) {
      if (seen.find(handler.name) != seen.end()) {
        return Invalid(std::string{"event_handlers has duplicate definition "} + handler.name.value);
      }
      seen.emplace(handler.name);
      auto valid = handler.validate();
      if (!valid) {
        return valid;
      }
    }
    return Valid();
  }
};


} // namespace schema
} // namespace appc
