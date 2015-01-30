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
    return name.validate();
  }
};


struct EventHandlers : ArrayType<EventHandlers, EventHandler> {
  explicit EventHandlers(const std::vector<EventHandler>& args)
  : ArrayType<EventHandlers, EventHandler>(args) {}
};


} // namespace schema
} // namespace appc
