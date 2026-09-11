// Copyright (c) 2021 Samsung Electronics Co., Ltd All Rights Reserved
// Use of this source code is governed by a apache 2.0 license that can be
// found in the LICENSE file.

#ifndef CION_COMMON_UTIL_LOGGING_H_
#define CION_COMMON_UTIL_LOGGING_H_

#include <dlog.h>

#ifndef PROJECT_TAG
#define PROJECT_TAG ""
#endif

#ifdef LOG
#undef LOG
#endif

#include <cassert>
#include <cstring>
#include <iomanip>
#include <iostream>
#include <sstream>
#include <string>

#ifndef __FILENAME__
#define __FILENAME__                                                           \
    (strrchr(__FILE__, '/') ? strrchr(__FILE__, '/') + 1 : __FILE__)
#endif

namespace cion {

enum class LogLevel {
  LOG_ERROR,
  LOG_WARNING,
  LOG_INFO,
  LOG_DEBUG,
};

log_priority LogLevelToPriority(LogLevel level);

template<LogLevel> struct LogTag;
template<> struct LogTag<LogLevel::LOG_ERROR> {
  static constexpr const char* value = "\033[1;31m| ERROR   |\033[0m";
};
template<> struct LogTag<LogLevel::LOG_WARNING> {
  static constexpr const char* value = "\033[1;33m| WARNING |\033[0m";
};
template<> struct LogTag<LogLevel::LOG_INFO>  {
  static constexpr const char* value = "\033[1;32m| INFO    |\033[0m";
};
template<> struct LogTag<LogLevel::LOG_DEBUG> {
  static constexpr const char* value = "\033[0m| DEBUG   |\033[0m";
};

template <class charT, class traits = std::char_traits<charT>>
class StringStream : private std::basic_ostringstream<charT, traits> {
 public:
  using std::basic_ostringstream<charT, traits>::str;

  template <class T>
  StringStream&  operator<<(const T& value) {
    static_cast<std::basic_ostringstream<charT, traits> &>(*this) << value;
    return *this;
  }
};

class LogCatcher {
 public:
  LogCatcher(LogLevel level, const char* tag)
    : level_(level), tag_(tag) { }

  void operator&(const StringStream<char>& str) const {
    dlog_print(LogLevelToPriority(level_), tag_.c_str(), "%s",
        Escape(str.str()).c_str());
  }

 private:
  // Since LogCatcher passes input to dlog_print(), the input which contains
  // format string(such as %d, %n) can cause unexpected result.
  // This is simple function to escape '%'.
  // NOTE: Is there any gorgeous way instead of this?
  std::string Escape(const std::string& str) const {
    std::string escaped = std::string(str);
    size_t start_pos = 0;
    std::string from = "%";
    std::string to = "%%";
    while ((start_pos = escaped.find(from, start_pos)) != std::string::npos) {
      escaped.replace(start_pos, from.length(), to);
      start_pos += to.length();
    }
    return escaped;
  }
  LogLevel level_;
  std::string tag_;
};

}  // namespace cion

inline static const constexpr char* __tag_for_project() {
  return PROJECT_TAG;
}

// Simple logging macro of following usage:
//   LOG(LEVEL) << object_1 << object_2 << object_n;
//     where:
//       LEVEL = ERROR | WARNING | INFO | DEBUG
#define LOG(LEVEL)                                                             \
    ::cion::LogCatcher(                                                        \
      ::cion::LogLevel::LOG_ ## LEVEL, __tag_for_project())                    \
      & ::cion::StringStream<char>()                                           \
      << std::string(::cion::LogTag<::cion::LogLevel::LOG_ ## LEVEL>::value)   \
      << " : " << std::setw(36)                                                \
      << (std::string(__FILENAME__) + ":" + std::to_string(__LINE__)).c_str()  \
      << std::setw(0) << " : "                                                 \

#endif  // CION_COMMON_UTIL_LOGGING_H_
