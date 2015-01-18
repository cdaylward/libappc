#pragma once

#include "appc/util/try.h"
#include "appc/util/option.h"


template<typename T>
Option<T> OptionFromTry(const Try<T>& a_try) {
  if (a_try) {
    return Some(*a_try);
  }

  return None<T>();
}


template<typename T>
Option<T> OptionFromTry(const std::function<Try<T>()>& func) {
  try {
    return OptionFromTry<T>(func());
  } catch (const std::exception& err) {
    return None<T>();
  }
}


template<typename... TryTypes>
Option<bool> SomeIfAll(const TryTypes&...tries) {
  if (all_results(tries...)) {
    return Some(true);
  }

  return None<bool>();
}


template<typename T>
bool all_results(const Try<T>& a_try) {
  return a_try;
}


template<typename T, typename... Tail>
bool all_results(const Try<T>& a_try, const Tail&... tail) {
  if (!a_try) {
    return false;
  }
  return all_results(tail...);
}


template<typename R, typename T>
Try<T> collect_failure_reasons(const Try<T>& a_try) {
  if (a_try) {
    return a_try;
  }
  return Failure<T>("\"" + a_try.failure_reason() + "\"");
}


template<typename R, typename T, typename... Tail>
Try<R> collect_failure_reasons(const Try<T>& a_try, const Tail&... tail) {
  std::string all_reasons{};
  if (!a_try) {
    all_reasons += "\"" + a_try.failure_reason() + "\", ";
  }
  auto other_tries = collect_failure_reasons<R>(tail...);
  if (!other_tries) {
    all_reasons += other_tries.failure_reason();
  }
  return Failure<R>(all_reasons);
}
