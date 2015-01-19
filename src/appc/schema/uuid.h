#pragma once

#include <regex>

#include "appc/schema/common.h"


namespace appc {
namespace schema {


struct UUID : StringType<UUID> {
  explicit UUID(const std::string& id)
  : StringType<UUID>(id) {}

  Status validate() const {
    // UUID                   = time-low "-" time-mid "-"
    //                          time-high-and-version "-"
    //                          clock-seq-and-reserved
    //                          clock-seq-low "-" node
    // time-low               = 4hexOctet
    // time-mid               = 2hexOctet
    // time-high-and-version  = 2hexOctet
    // clock-seq-and-reserved = hexOctet
    // clock-seq-low          = hexOctet
    // node                   = 6hexOctet
    // hexOctet               = hexDigit hexDigit
    // hexDigit =
    //       "0" / "1" / "2" / "3" / "4" / "5" / "6" / "7" / "8" / "9" /
    //       "a" / "b" / "c" / "d" / "e" / "f" /
    //       "A" / "B" / "C" / "D" / "E" / "F"
    const std::string hex_octet{"([0-9a-fA-F]{2})"};
    const std::string hex_octet2{hex_octet + "{2}"};
    const std::string hex_octet4{hex_octet + "{4}"};
    const std::string hex_octet6{hex_octet + "{6}"};
    const std::regex pattern(hex_octet4 + "-" + hex_octet2 + "-" +
                             hex_octet2 + "-" +
                             hex_octet +
                             hex_octet + "-" + hex_octet6,
                             std::regex::ECMAScript);
    if (std::regex_match(value, pattern)) {
      return Valid();
    }
    return Invalid("UUID must be a rfc4122-formatted string.");
  }
};


} // namespace schema
} // namespace appc
