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

#include "appc/schema/common.h"
#include "appc/schema/environment.h"
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
  const Option<Environment> environment;
  const Option<MountPoints> mount_points;
  const Option<Ports> ports;
  const Option<Isolators> isolators;

  explicit App(const Exec& exec,
               const User& user,
               const Group& group,
               const Option<EventHandlers>& event_handlers,
               const Option<Path>& working_directory,
               const Option<Environment>& environment,
               const Option<MountPoints>& mount_points,
               const Option<Ports>& ports,
               const Option<Isolators>& isolators)
    : exec(exec),
      user(user),
      group(group),
      event_handlers(event_handlers),
      working_directory(working_directory),
      environment(environment),
      mount_points(mount_points),
      ports(ports),
      isolators(isolators) {}

  static Try<App> from_json(const Json& json) {
    const auto exec = try_from_json<Exec>(json, "exec");
    const auto user = try_from_json<User>(json, "user");
    const auto group = try_from_json<Group>(json, "group");

    const auto event_handlers = try_option_from_json<EventHandlers>(json, "eventHandlers");
    const auto working_directory = try_option_from_json<Path>(json, "workingDirectory");
    const auto environment = try_option_from_json<Environment>(json, "environment");
    const auto mount_points = try_option_from_json<MountPoints>(json, "mountPoints");
    const auto ports = try_option_from_json<Ports>(json, "ports");
    const auto isolators = try_option_from_json<Isolators>(json, "isolators");

    if (!all_results(exec, user, group, event_handlers, working_directory, environment,
                     mount_points, ports, isolators)) {
      return collect_failure_reasons<App>(exec, user, group, event_handlers, working_directory,
                                          environment, mount_points, ports, isolators);
    }

    return Result(App(from_result(exec),
                      from_result(user),
                      from_result(group),
                      from_result(event_handlers),
                      from_result(working_directory),
                      from_result(environment),
                      from_result(mount_points),
                      from_result(ports),
                      from_result(isolators)));
  }

  Status validate() const {
    return collect_status({
      exec.validate(),
      user.validate(),
      group.validate(),
      validate_if_some(event_handlers),
      validate_if_some(working_directory),
      validate_if_some(environment),
      validate_if_some(mount_points),
      validate_if_some(ports),
      validate_if_some(isolators)
    });
  }
};


} // namespace schema
} // namespace appc
