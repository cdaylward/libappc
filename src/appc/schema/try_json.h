#pragma once

#include "appc/schema/common.h"
#include "appc/util/try.h"


namespace appc {
namespace schema {


template<typename T>
Try<T> try_from_json(const Json& json, const std::string& field_name) {
  try {
    return T::from_json(json[field_name]);
  }
  catch (const std::exception& err) {
    return Failure<T>(err.what());
  }
}


template<typename T>
Try<Option<T>> try_option_from_json(const Json& json, const std::string& field_name, const bool required = false) {

  const auto has_field = TryFrom<Json>([&json, &field_name]() {
    return json[field_name];
  });
  if (!required && !has_field) {
    return Result(None<T>());
  }
  auto a_try = try_from_json<T>(json, field_name);
  if (a_try) {
    return Result(Some(*a_try));
  }

  return Failure<Option<T>>(a_try.failure_reason());
}


} //namespace schema
} //namespace appc
