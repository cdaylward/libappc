#pragma once

#include "appc/util/Try.h"

#include "3rdparty/nlohmann/json.h"


namespace appc {
namespace schema {

using Json = nlohmann::json;


struct Status {
  const bool successful;
  const std::string message;

  explicit Status(const bool successful,
                  const std::string& message)
  : successful(successful),
    message(message) {}

  operator bool() const {
    return successful;
  }
};


Status Valid() {
  return Status(true, "");
}


Status Invalid(const std::string& message) {
  return Status(false, message);
}


template<typename T>
struct Type {
  virtual ~Type() = default;
  virtual Status validate() = 0;
};


template<typename T>
struct StringType : Type<T> {
  const std::string value;

  explicit StringType<T>(const std::string value)
  : value(value) {}

  bool operator==(const StringType<T>& other) const {
    return value == other.value;
  }

  bool operator!=(const StringType<T>& other) const {
    return !(*this == other);
  }

  operator std::string() const {
    return value;
  }

  static Try<T> from_json(const Json& json) {
    if (json.type() != Json::value_type::string) {
      return Failure<T>("StringType must be intialized from JSON string type");
    }
    return Success(T(json.get<std::string>()));
  }

  static Json to_json(const T& string_type) {
    return Json { string_type.value };
  }
};


} // namespace schema
} // namespace appc
