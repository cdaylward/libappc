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
// https://github.com/cdaylward/pathname

#pragma once

#include <string>
#include <vector>
#include <sys/stat.h>


namespace pathname {


const char separator{'/'};


inline std::string trim_leading_slash(const std::string& path) {
  std::size_t pos;
  for (pos = 0; path[pos] == separator; pos++);
  return path.substr(pos);
}


inline std::string trim_trailing_slash(const std::string& path) {
  std::size_t pos;
  for (pos = path.length() - 1; path[pos] == separator; pos--);
  return path.substr(0, pos + 1);
}


inline std::string join(const std::string& first, const std::string& second) {
  if (first.empty()) return second;
  if (second.empty()) return first;
  return trim_trailing_slash(first) + separator + trim_leading_slash(second);
}


template<typename ...Path>
inline std::string join(const std::string& first, const std::string& second, const Path&... rest) {
  return join(join(first, second), rest...);
}


inline std::string base(const std::string& path) {
  if (path.length() == 1 && path[0] == separator) return path;
  if (*path.crbegin() == separator) return ".";
  const std::size_t pos = path.find_last_of(separator);
  if (pos == std::string::npos) return path;
  return trim_leading_slash(path.substr(pos));
}


inline std::string dir(const std::string& path) {
  if (path.length() == 1 && path[0] == separator) return path;
  const std::size_t pos = path.find_last_of(separator);
  if (pos == 0) return "/";
  if (pos == std::string::npos) return path;
  return trim_trailing_slash(path.substr(0, pos));
}


inline bool is_absolute(const std::string& path) {
  return path[0] == separator;
}


inline bool has_dot_dot(const std::string& path) {
  if (path == "..") return true;
  // /.. is a special case of / per IEEE Std 1003.1-2001
  if (path == "/..") return false;
  if (path.length() < 3) return false;
  if (path.compare(0, 3, "../") == 0) return true;
  if (path.compare(path.length() - 3, 3, "/..") == 0) return true;
  if (path.find("/../") != std::string::npos) return true;
  return false;
}


} // namespace pathname
