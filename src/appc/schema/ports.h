#pragma once

#include <cstdint>

#include "appc/schema/common.h"
#include "appc/schema/ac_name.h"

#include "appc/util/try.h"
#include "appc/util/try_option.h"


namespace appc {
namespace schema {


struct PortName : ACName<PortName> {
  explicit PortName(const std::string& name)
  : ACName<PortName>(name) {}
};


struct PortNumber : IntegerType<PortNumber> {
  explicit PortNumber(const uint16_t port)
  : IntegerType<PortNumber>(port) {}

  Status validate() const {
    if (1 < value && value < 65535) {
      return Valid();
    }
    return Invalid("port must be a number between 1 and 65535");
  }
};


struct Protocol : StringType<Protocol> {
  explicit Protocol(const std::string& proto)
  : StringType<Protocol>(proto) {}

  Status validate() const {
    // TODO is this true?
    if (value == "tcp" || value == "udp") {
      return Valid();
    }
    return Invalid("protocol must be tcp or udp");
  }
};


struct SocketActivated : BooleanType<SocketActivated> {
  explicit SocketActivated(const bool socket_activated)
  : BooleanType<SocketActivated>(socket_activated) {}
};


struct Port : Type<Port> {
  const PortName name;
  const PortNumber port;
  const Protocol protocol;
  const Option<SocketActivated> socket_activated;

  explicit Port(const PortName& name,
                const PortNumber& port,
                const Protocol& protocol,
                const Option<SocketActivated>& socket_activated)
  : name(name),
    port(port),
    protocol(protocol),
    socket_activated(socket_activated) {}

  static Try<Port> from_json(const Json& json) {
    const auto name = TryFlatten<PortName>([&json]() {
      return PortName::from_json(json[std::string{"name"}]);
    });
    const auto port = TryFlatten<PortNumber>([&json]() {
      return PortNumber::from_json(json[std::string{"port"}]);
    });
    const auto protocol = TryFlatten<Protocol>([&json]() {
      return Protocol::from_json(json[std::string{"protocol"}]);
    });
    if (!SomeIfAll(name, port, protocol)) {
      return collect_failure_reasons<Port>(name, port, protocol);
    }
    const auto socket_activated = OptionFromTry<SocketActivated>([&json]() {
      return SocketActivated::from_json(json[std::string{"socketActivated"}]);
    });
    return Result(Port(*name,
                       *port,
                       *protocol,
                       socket_activated));
  }

  Status validate() const {
    return collect_status({
      name.validate(),
      port.validate(),
      protocol.validate(),
      validate_if_some(socket_activated)
    });
  }
};


struct Ports : ArrayType<Ports, Port> {
  explicit Ports(const std::vector<Port> array)
  : ArrayType<Ports, Port>(array) {}
};


} // namespace schema
} // namespace appc
