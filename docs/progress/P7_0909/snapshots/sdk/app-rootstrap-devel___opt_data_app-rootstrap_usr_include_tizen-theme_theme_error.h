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

#ifndef __TIZEN_APPFW_THEME_MANAGER_THEME_ERROR_H__
#define __TIZEN_APPFW_THEME_MANAGER_THEME_ERROR_H__

#include <tizen.h>

/**
 * @file theme_error.h
 */

/**
 * @addtogroup CAPI_THEME_MODULE
 * @{
 */

/**
 * @brief Enumeration for theme manager errors.
 * @since_tizen 6.0
 */
typedef enum _theme_manager_error {
  THEME_MANAGER_ERROR_NONE = TIZEN_ERROR_NONE, /**< Success */
  THEME_MANAGER_ERROR_INVALID_PARAMETER =
      TIZEN_ERROR_INVALID_PARAMETER, /**< Invalid parameter */
  THEME_MANAGER_ERROR_OUT_OF_MEMORY =
      TIZEN_ERROR_OUT_OF_MEMORY,                       /**< Out of memory */
  THEME_MANAGER_ERROR_IO_ERROR = TIZEN_ERROR_IO_ERROR, /**< I/O error */
  THEME_MANAGER_ERROR_PERMISSION_DENIED =
      TIZEN_ERROR_PERMISSION_DENIED, /**< Permission denied */
  THEME_MANAGER_ERROR_NO_SUCH_THEME =
      TIZEN_ERROR_NO_SUCH_FILE, /**< No such theme */
  THEME_MANAGER_ERROR_KEY_NOT_AVAILABLE =
      TIZEN_ERROR_KEY_NOT_AVAILABLE, /**< Required key not available */
} theme_manager_error_e;

/**
 * @}
 */

#endif /* __TIZEN_APPFW_THEME_MANAGER_THEME_ERROR_H__ */
