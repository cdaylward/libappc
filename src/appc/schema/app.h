#pragma once


#include "appc/schema/common.h"
#include "appc/schema/event_handlers.h"
#include "appc/schema/exec.h"
#include "appc/schema/group.h"
#include "appc/schema/isolators.h"
#include "appc/schema/mount_points.h"
#include "appc/schema/path.h"
#include "appc/schema/user.h"

#include "appc/util/try.h"
#include "appc/util/try_option.h"


namespace appc {
namespace schema {


struct App : Type<App> {
  const Exec exec;
  const User user;
  const Group group;
  const Option<EventHandlers> event_handlers;
  const Option<Path> working_directory;
  //const Option<Environment> env;
  const Option<MountPoints> mount_points;
  //const Option<Ports> ports;
  const Option<Isolators> isolators;

  explicit App(const Exec& exec,
               const User& user,
               const Group& group,
               const Option<EventHandlers>& event_handlers,
               const Option<Path>& working_directory,
               //const Environment& env,
               const Option<MountPoints>& mount_points,
               //const Ports& ports,
               const Option<Isolators>& isolators)
    : exec(exec),
      user(user),
      group(group),
      event_handlers(event_handlers),
      working_directory(working_directory),
      //env(env),
      mount_points(mount_points),
      //ports(ports),
      isolators(isolators) {}

  static Try<App> from_json(const Json& json) {
    const auto exec = TryFlatten<Exec>([&json]() {
      return Exec::from_json(json[std::string{"exec"}]);
    });
    const auto user = TryFlatten<User>([&json]() {
      return User::from_json(json[std::string{"user"}]);
    });
    const auto group = TryFlatten<Group>([&json]() {
      return Group::from_json(json[std::string{"group"}]);
    });

    if (!SomeIfAll(exec, user, group)) {
      return collect_failure_reasons<App>(exec, user, group);
    }

    const auto event_handlers = OptionFromTry<EventHandlers>([&json]() {
      return EventHandlers::from_json(json[std::string{"eventHandlers"}]);
    });
    const auto working_directory = OptionFromTry<Path>([&json]() {
      return Path::from_json(json[std::string{"workingDirectory"}]);
    });
    //Environment::from_json(json["environment"]),
    const auto mount_points = OptionFromTry<MountPoints>([&json]() {
      return MountPoints::from_json(json[std::string{"mountPoints"}]);
    });
    //Ports::from_json(json["ports"]),
    const auto isolators = OptionFromTry<Isolators>([&json]() {
      return Isolators::from_json(json[std::string{"isolators"}]);
    });
    return Result(App(*exec,
                      *user,
                      *group,
                      event_handlers,
                      working_directory,
                      mount_points,
                      isolators));
  }

  Status validate() const {
    return collect_status({
      exec.validate(),
      user.validate(),
      group.validate(),
      validate_if_some(event_handlers),
      validate_if_some(working_directory),
      validate_if_some(mount_points),
      validate_if_some(isolators)
    });
  }
};


} // namespace schema
} // namespace appc
