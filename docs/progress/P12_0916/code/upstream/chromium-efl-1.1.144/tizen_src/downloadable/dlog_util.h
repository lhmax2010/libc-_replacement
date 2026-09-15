// Copyright 2019 Samsung Electronics. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef DLOG_UTIL_H
#define DLOG_UTIL_H

#include <sstream>

#ifndef LOG_TAG
#define LOG_TAG "CHROMIUM"
#endif
#include <dlog/dlog.h>

#ifdef FILE_LOG_OUT
#include <sys/time.h>
#include <unistd.h>
#include <fstream>
#endif

#define __DLOG_UTIL_FMT__ "%s:%s(%d) > "
#define __DLOG_UTIL_FILE__                                                 \
  (__builtin_strrchr(__FILE__, '/') ? __builtin_strrchr(__FILE__, '/') + 1 \
                                    : __FILE__)

#define TIZEN_DLOG_PRINT(priority, fmt, ...)                               \
  dlog_print(priority, LOG_TAG, __DLOG_UTIL_FMT__ fmt, __DLOG_UTIL_FILE__, \
             __FUNCTION__, __LINE__, ##__VA_ARGS__)

class TizenDlogWrapper {
 public:
  TizenDlogWrapper(log_priority priority,
                   const char* file,
                   unsigned int line,
                   const char* func)
      : priority_(priority), file_(file), line_(line), func_(func) {}
  ~TizenDlogWrapper() {
    dlog_print(priority_, LOG_TAG, __DLOG_UTIL_FMT__ "%s", file_, func_, line_,
               stream_.str().data());
#ifdef FILE_LOG_OUT
    if (!fstream_.is_open())
      return;
    struct timespec ts;
    struct tm local;
    if (!clock_gettime(CLOCK_MONOTONIC, &ts) &&
        localtime_r(&ts.tv_sec, &local)) {
      char buffer[20];
      int ms = ts.tv_nsec / 1000000;
      strftime(buffer, sizeof(buffer), "%m-%d %H:%M:%S", &local);
      fstream_ << buffer;
      snprintf(buffer, sizeof(buffer), ":%03d ", ms);
      fstream_ << buffer;
    }
    fstream_ << "[" << getpid() << "] " << file_ << ":" << func_ << "(" << line_
             << ") > " << stream_.str() << std::endl;
#endif
  }
  std::ostream& stream() { return stream_; }

#ifdef FILE_LOG_OUT
  static std::ofstream fstream_;
#endif

 private:
  log_priority priority_;
  const char* file_;
  unsigned int line_;
  const char* func_;
  std::ostringstream stream_;
};

#define TIZEN_DLOG_STREAM(priority)                                      \
  TizenDlogWrapper(priority, __DLOG_UTIL_FILE__, __LINE__, __FUNCTION__) \
      .stream()

// redefine LOG macro function defined in dlog-internal.h
#ifdef LOG
#undef LOG
#endif
#define LOG(priority) TIZEN_DLOG_STREAM(DLOG_##priority)

#endif  // DLOG_UTIL_H
