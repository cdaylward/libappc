#pragma once


#include "appc/schema/common.h"
#include "appc/schema/event_handlers.h"
#include "appc/schema/exec.h"
#include "appc/schema/group.h"
#include "appc/schema/isolators.h"
#include "appc/schema/mount_points.h"
#include "appc/schema/path.h"
#include "appc/schema/ports.h"
#include "appc/schema/try_json.h"
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
  const Option<Ports> ports;
  const Option<Isolators> isolators;

  explicit App(const Exec& exec,
               const User& user,
               const Group& group,
               const Option<EventHandlers>& event_handlers,
               const Option<Path>& working_directory,
               //const Environment& env,
               const Option<MountPoints>& mount_points,
               const Option<Ports>& ports,
               const Option<Isolators>& isolators)
    : exec(exec),
      user(user),
      group(group),
      event_handlers(event_handlers),
      working_directory(working_directory),
      //env(env),
      mount_points(mount_points),
      ports(ports),
      isolators(isolators) {}

  static Try<App> from_json(const Json& json) {
    const auto exec = try_from_json<Exec>(json, "exec");
    const auto user = try_from_json<User>(json, "user");
    const auto group = try_from_json<Group>(json, "group");

    const auto event_handlers = try_option_from_json<EventHandlers>(json, "eventHandlers");
    const auto working_directory = try_option_from_json<Path>(json, "workingDirectory");
    //Environment::from_json(json["environment"]),
    const auto mount_points = try_option_from_json<MountPoints>(json, "mountPoints");
    const auto ports = try_option_from_json<Ports>(json, "ports");
    const auto isolators = try_option_from_json<Isolators>(json, "isolators");

    if (!all_results(exec, user, group, event_handlers, working_directory, mount_points,
                     ports, isolators)) {
      return collect_failure_reasons<App>(exec, user, group, event_handlers, working_directory,
                                          mount_points, ports, isolators);
    }

    return Result(App(*exec,
                      *user,
                      *group,
                      *event_handlers,
                      *working_directory,
                      *mount_points,
                      *ports,
                      *isolators));
  }

  Status validate() const {
    return collect_status({
      exec.validate(),
      user.validate(),
      group.validate(),
      validate_if_some(event_handlers),
      validate_if_some(working_directory),
      validate_if_some(mount_points),
      validate_if_some(ports),
      validate_if_some(isolators)
    });
  }
};


} // namespace schema
} // namespace appc
