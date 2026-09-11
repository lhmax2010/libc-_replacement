/*
 * Copyright (c) 2019 Samsung Electronics Co., Ltd.
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 * http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#ifndef CION_COMMON_EXCEPTION_H_
#define CION_COMMON_EXCEPTION_H_

#include <string>
#include <exception>

#include "cion/common/cion_error.hh"
#include "cion/common/util/logging.hh"

#define THROW(error_code) throw cion::Exception(error_code, __FILE__, __LINE__)

namespace cion {

class Exception : public std::exception {
 public:
  explicit Exception(int error_code, std::string file = __FILE__,
      int line = __LINE__ ) {
    error_code_ = error_code;
    message_ = file.substr(file.find_last_of("/") + 1) + ":"
        + std::to_string(line) + GetErrorString(error_code);
    LOG(ERROR) << message_;
  }
  virtual ~Exception() {}
  virtual const char *what(void) const noexcept {
    return message_.c_str();
  }
  int GetErrorCode() {
    return error_code_;
  }

 private:
  int error_code_;
  std::string message_;
  std::string GetErrorString(int error_code) {
    switch (error_code) {
    case cion::error::CION_ERROR_NOT_PERMITTED:
      return ": CION_ERROR_NOT_PERMITTED";
    case cion::error::CION_ERROR_OUT_OF_MEMORY:
      return ": CION_ERROR_OUT_OF_MEMORY";
    case cion::error::CION_ERROR_PERMISSION_DENIED:
      return ": CION_ERROR_PERMISSION_DENIED";
    case cion::error::CION_ERROR_INVALID_PARAMETER:
      return ": CION_ERROR_INVALID_PARAMETER";
    case cion::error::CION_ERROR_INVALID_OPERATION:
      return ": CION_ERROR_INVALID_OPERATION";
    case cion::error::CION_ERROR_CONNECTION_TIME_OUT:
      return ": CION_ERROR_CONNECTION_TIME_OUT";
    case cion::error::CION_ERROR_NOW_IN_PROGRESS:
      return ": CION_ERROR_NOW_IN_PROGRESS";
    case cion::error::CION_ERROR_NOT_SUPPORTED:
      return ": CION_ERROR_NOT_SUPPORTED";
    case cion::error::CION_ERROR_NOT_INITIALIZED:
      return ": CION_ERROR_NOT_INITIALIZED";
    case cion::error::CION_ERROR_ALREADY_INITIALIZED:
      return ": CION_ERROR_ALREADY_INITIALIZED";
    case cion::error::CION_ERROR_ALREADY_ENABLED:
      return ": CION_ERROR_ALREADY_ENABLED";
    case cion::error::CION_ERROR_OPERATION_FAILED:
      return ": CION_ERROR_OPERATION_FAILED";
    case cion::error::CION_ERROR_NAME_CONFLICT:
      return ": CION_ERROR_NAME_CONFLICT";
    case cion::error::CION_ERROR_NO_READ_DATA:
      return ": CION_ERROR_NO_READ_DATA";
    case cion::error::CION_ERROR_REJECTED_BY_PEER:
      return ": CION_ERROR_REJECTED_BY_PEER";
    case cion::error::CION_ERROR_INTERFACE_DOWN:
      return ": CION_ERROR_INTERFACE_DOWN";
    case cion::error::CION_ERROR_TIMED_OUT:
      return ": CION_ERROR_TIMED_OUT";
    default:
      return "";
    }
  }
};  // class Exception

}  // namespace cion

#endif  // CION_COMMON_EXCEPTION_H_
