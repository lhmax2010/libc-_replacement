/*
 * Copyright (c) 2021 Samsung Electronics Co., Ltd.
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

#ifndef CION_COMMON_CION_ERROR_HH_
#define CION_COMMON_CION_ERROR_HH_

#include <errno.h>
#include <tizen_error.h>

#define CION_ERROR -0x030C0000

namespace cion {
namespace error {

  enum CionError {
      /**
       * Successful
       */
      CION_ERROR_NONE = 0,
      /**
       * Operation not permitted(1)
       */
      CION_ERROR_NOT_PERMITTED = -EPERM,
      /**
       * IO error(5)
       */
      CION_ERROR_IO_ERROR = -EIO,
      /**
       * Out of memory(12)
       */
      CION_ERROR_OUT_OF_MEMORY = -ENOMEM,
      /**
       * Permission denied(13)
       */
      CION_ERROR_PERMISSION_DENIED = -EACCES,
      /**
       * Invalid function parameter(22)
       */
      CION_ERROR_INVALID_PARAMETER = -EINVAL,
      /**
       * Invalid operation(38)
       */
      CION_ERROR_INVALID_OPERATION = -ENOSYS,
      /**
       * Connection timed out(110)
       */
      CION_ERROR_CONNECTION_TIME_OUT = -ETIMEDOUT,
      /**
       * Now in progress(115)
       */
      CION_ERROR_NOW_IN_PROGRESS = -EINPROGRESS,
      /**
       * Not supported
       */
      CION_ERROR_NOT_SUPPORTED = TIZEN_ERROR_NOT_SUPPORTED,
      /**
       * Timed out
       */
      CION_ERROR_TIMED_OUT = TIZEN_ERROR_TIMED_OUT,
      /**
       * Operation failed
       */
      CION_ERROR_OPERATION_FAILED = CION_ERROR|0x01,
      /**
       * No initialized
       */
      CION_ERROR_NOT_INITIALIZED = CION_ERROR|0x02,
      /**
       * Already initialized
       */
      CION_ERROR_ALREADY_INITIALIZED = CION_ERROR|0x03,
      /**
       * Already enabled
       */
      CION_ERROR_ALREADY_ENABLED = CION_ERROR|0x04,
      /**
       * Service name conflicted
       */
      CION_ERROR_NAME_CONFLICT = CION_ERROR|0x05,
      /**
       * No read data
       */
      CION_ERROR_NO_READ_DATA = CION_ERROR|0x06,
      /**
       * Rejected by peer
       */
      CION_ERROR_REJECTED_BY_PEER = CION_ERROR|0x07,
      /**
       * Wi-Fi Interface is down
       */
      CION_ERROR_INTERFACE_DOWN = -ENETDOWN,
      /**
       * Service deregistered by a name conflict or similar problem
       */
      CION_ERROR_SERVICE_DEREGISTERED = CION_ERROR|0x08,
  };

}
}  // namespace cion

#endif  // CION_COMMON_CION_ERROR_HH_
