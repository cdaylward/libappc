#pragma once

#include "appc/schema/common.h"
#include "appc/schema/exec.h"


namespace appc {
namespace schema {


struct EventName : StringType<EventName> {
  explicit EventName(const std::string& name)
  : StringType<EventName>(name) {}

  Status validate() const {
    // TODO(cdaylward)
    return Valid();
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
    if (json.type() != Json::value_type::object) {
      return Failure<EventHandler>("EventHandler must be built from JSON object");
    }
    // TODO check these
    const Try<Exec> exec { Exec::from_json(json[std::string{"exec"}]) };
    const Try<EventName> eventName { EventName::from_json(json[std::string{"name"}]) };
    return Result(EventHandler(*eventName,
                               *exec));
  }

  Status validate() const {
    return Valid();
  }
};


struct EventHandlers : ArrayType<EventHandlers, EventHandler> {
  explicit EventHandlers(const std::vector<EventHandler>& args)
  : ArrayType<EventHandlers, EventHandler>(args) {}

  Status validate() const {
    return Valid();
  }
};


} // namespace schema
} // namespace appc
