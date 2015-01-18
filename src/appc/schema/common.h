#pragma once

#include "3rdparty/nlohmann/json.h"

#include "appc/util/try.h"


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


Status collect_status(const std::vector<Status>& statuses) {
  std::string message{};
  bool successful{true};
  for (auto& status: statuses) {
    if (!status) {
      successful = false;
      if (!message.empty()) {
        message += ", ";
      }
      message += "\"" + status.message + "\"";
    }
  }
  return Status(successful, message);
}


template<typename T>
struct Type {
  virtual ~Type() = default;
  virtual Status validate() const = 0;
};


template<typename T>
struct StringType : Type<T> {
  const std::string value;

  explicit StringType<T>(const std::string& value)
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
      return Failure<T>("StringType must be intialized from JSON string type.");
    }
    return Result(T(json.get<std::string>()));
  }

  static Json to_json(const T& string_type) {
    return string_type.value;
  }
};


template<typename T, typename E>
struct ArrayType : Type<T> {
  std::vector<E> array {};

  explicit ArrayType<T, E>(const std::vector<E>& array)
  : array(array) {}

  ArrayType<T, E>(std::vector<E>&& array)
  : array(array) {}

  ArrayType<T, E>(const ArrayType<T, E>& other)
  : array(other.array) {}

  ArrayType<T, E>(ArrayType<T, E>&& other)
  : array(std::move(other.array)) {}

  operator std::vector<E>() const {
    return array;
  }

  static Try<T> from_json(const Json& json) {
    static_assert(std::is_base_of<Type<E>, E>::value, "E not of Type<E> in ArrayType");

    if (json.type() != Json::value_type::array) {
      return Failure<T>("ArrayType must be initialized from JSON array.");
    }

    std::vector<E> array {};
    for (auto& json_e : json) {
      auto try_e = E::from_json(json_e);
      if (!try_e) {
        return Failure<T>("Could not construct array element: " + try_e.failure_reason());
      }
      array.push_back(*try_e);
    }
    return Result(T{array});
  }

  // TODO(cdaylward) provide validate() here?
};


template<typename T>
struct NameValueType : Type<T> {
  const std::string name;
  const std::string value;

  explicit NameValueType<T>(const std::string& name,
                            const std::string& value)
  : name(name),
    value(value) {}

  NameValueType<T>(const NameValueType<T>& other)
    : name(other.name),
      value(other.value) {}

  NameValueType<T>(NameValueType<T>&& other)
    : name(std::move(other.name)),
      value(std::move(other.value)) {}

  static Try<T> from_json(const Json& json) {
    if (json.type() != Json::value_type::object) {
      return Failure<T>("NameValue types must be initialized from JSON objects");
    }
    return Result(T(json[std::string{"name"}].get<std::string>(),
                    json[std::string{"value"}].get<std::string>()));
  }

  static Json to_json(const NameValueType& nameval) {
    const Json json_label = { {"name", nameval.name}, {"value", nameval.value} };
    return json_label;
  }
};


} // namespace schema
} // namespace appc
