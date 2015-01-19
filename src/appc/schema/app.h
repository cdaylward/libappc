#pragma once

#include "appc/schema/common.h"
#include "appc/schema/event_handlers.h"
#include "appc/schema/exec.h"
#include "appc/schema/group.h"
#include "appc/schema/isolators.h"
#include "appc/schema/mount_points.h"
#include "appc/schema/user.h"


namespace appc {
namespace schema {


struct App : Type<App> {
  const Exec exec;
  const User user;
  const Group group;
  const EventHandlers event_handlers;
  //const Environment env;
  const MountPoints mount_points;
  //const Ports ports;
  const Isolators isolators;

  explicit App(const Exec& exec,
               const User& user,
               const Group& group,
               const EventHandlers& event_handlers,
               //const Environment& env,
               const MountPoints& mount_points,
               //const Ports& ports,
               const Isolators& isolators)
    : exec(exec),
      user(user),
      group(group),
      event_handlers(event_handlers),
      //env(env),
      mount_points(mount_points),
      //ports(ports),
      isolators(isolators) {}

  static Try<App> from_json(const Json& json) {
    if (json.type() != Json::value_type::object) {
      return Failure<App>("App must be initialized from JSON object");
    }
    const Try<Exec> exec = Exec::from_json(json[std::string{"exec"}]);
    const Try<User> user = User::from_json(json[std::string{"user"}]);
    const Try<Group> group = Group::from_json(json[std::string{"group"}]);
    const Try<EventHandlers> event_handlers = EventHandlers::from_json(json[std::string{"eventHandlers"}]);
    //Environment::from_json(json["environment"]),
    const Try<MountPoints> mount_points = MountPoints::from_json(json[std::string{"mountPoints"}]);
    //Ports::from_json(json["ports"]),
    const Try<Isolators> isolators = Isolators::from_json(json[std::string{"isolators"}]);
    return Result(App(*exec,
                      *user,
                      *group,
                      *event_handlers,
                      *mount_points,
                      *isolators));
  }

  Status validate() const {
    // TODO(cdaylward)
    return Valid();
  }
};


} // namespace schema
} // namespace appc
