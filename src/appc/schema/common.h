#pragma once

#include <cstdint>

#include "3rdparty/nlohmann/json.h"

#include "appc/util/option.h"
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
Status validate_if_some(const Option<T>& option) {
  if (option) {
    return option->validate();
  }
  return Valid();
}


template<typename T>
Json to_json_if_some(const Option<T>& option) {
  Json json{};
  if (option) {
    json = T::to_json(*option);
  }
  return json;
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


template<typename T>
struct IntegerType : Type<T> {
  // TODO hand-wavy
  const int64_t value;

  explicit IntegerType<T>(const double value)
  : value(value) {}

  bool operator==(const IntegerType<T>& other) const {
    return value == other.value;
  }

  bool operator!=(const IntegerType<T>& other) const {
    return !(*this == other);
  }

  operator int() const {
    return value;
  }

  static Try<T> from_json(const Json& json) {
    if (json.type() != Json::value_type::number) {
      return Failure<T>("IntegerType must be intialized from JSON number type.");
    }
    return Result(T(json.get<int>()));
  }

  static Json to_json(const T& int_type) {
    return int_type.value;
  }
};


template<typename T>
struct BooleanType : Type<T> {
  const bool value;

  explicit BooleanType<T>(const double value)
  : value(value) {}

  bool operator==(const BooleanType<T>& other) const {
    return value == other.value;
  }

  bool operator!=(const BooleanType<T>& other) const {
    return !(*this == other);
  }

  operator bool() const {
    return value;
  }

  static Try<T> from_json(const Json& json) {
    if (json.type() != Json::value_type::boolean) {
      return Failure<T>("BooleanType must be intialized from JSON boolean type.");
    }
    return Result(T(json.get<bool>()));
  }

  static Json to_json(const T& int_type) {
    return int_type.value;
  }

  virtual Status validate() const {
    return Valid();
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

  static Json to_json(const ArrayType<T, E>& array) {
    Json json;
    for (const auto& element : array.array) {
      json.push_back(E::to_json(element));
    }
    return json;
  }

  virtual Status validate() const {
    for (const auto& element : this->array) {
      auto valid = element.validate();
      if (!valid) {
        return valid;
      }
    }
    return Valid();
  }
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
    try {
      return Result(T(json[std::string{"name"}].get<std::string>(),
                      json[std::string{"value"}].get<std::string>()));
    } catch (const std::exception& err) {
      return Failure<T>("NameValue must be a {\"name\": \"<name>\", \"value\": \"<value>\" } object: " + std::string{err.what()});
    }
    return Failure<T>("NameValue must be a {\"name\": \"<name>\", \"value\": \"<value>\" } object.");
  }

  static Json to_json(const NameValueType& nameval) {
    const Json json_label = { {"name", nameval.name}, {"value", nameval.value} };
    return json_label;
  }
};


} // namespace schema
} // namespace appc
