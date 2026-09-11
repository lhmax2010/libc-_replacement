/*
 * Copyright (c) 2020 Samsung Electronics Co., Ltd. All rights reserved.
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


#ifndef __TIZEN_APPFW_CION_ERROR_H__
#define __TIZEN_APPFW_CION_ERROR_H__

#include <tizen.h>

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @addtogroup CAPI_CION_MODULE
 * @{
 */

/**
 * @brief Enumeration for Cion errors.
 * @since_tizen 6.5
 */

typedef enum _cion_error {
  CION_ERROR_NONE = TIZEN_ERROR_NONE, /**< Success */
  CION_ERROR_IO_ERROR = TIZEN_ERROR_IO_ERROR, /**< I/O error */
  CION_ERROR_OUT_OF_MEMORY = TIZEN_ERROR_OUT_OF_MEMORY, /**< Out of memory */
  CION_ERROR_PERMISSION_DENIED = TIZEN_ERROR_PERMISSION_DENIED, /**< Permission denied */
  CION_ERROR_INVALID_PARAMETER = TIZEN_ERROR_INVALID_PARAMETER, /**< Invalid parameter */
  CION_ERROR_INVALID_OPERATION = TIZEN_ERROR_INVALID_OPERATION, /**< Function not implemented */
  CION_ERROR_ALREADY_IN_PROGRESS = TIZEN_ERROR_ALREADY_IN_PROGRESS, /**< Already in progress */
  CION_ERROR_NOT_SUPPORTED = TIZEN_ERROR_NOT_SUPPORTED, /**< Not supported */
  CION_ERROR_TIMED_OUT = TIZEN_ERROR_TIMED_OUT, /**< Timed out */
  CION_ERROR_OPERATION_FAILED = TIZEN_ERROR_CION | 0x01, /**< Operation failed */
} cion_error_e;

/**
 * @}
 */

#ifdef __cplusplus
}
#endif

#endif /* __TIZEN_APPFW_CION_ERROR_H__ */
