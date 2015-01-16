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
