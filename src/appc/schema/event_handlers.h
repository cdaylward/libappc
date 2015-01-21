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

    if (!SomeIfAll(exec, event_name)) {
      return collect_failure_reasons<EventHandler>(exec, event_name);
    }

    return Result(EventHandler(*event_name,
                               *exec));
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
