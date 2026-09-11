/*
 * Copyright (c) 2019 Samsung Electronics Co., Ltd All Rights Reserved
 *
 * Licensed under the Apache License, Version 2.0 (the License);
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 * http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an AS IS BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#ifndef __TIZEN_UIX_STICKER_ERROR_H__
#define __TIZEN_UIX_STICKER_ERROR_H__

#include <tizen.h>

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @file sticker_error.h
 * @brief This file contains sticker error enumeration.
 */

/**
 * @addtogroup CAPI_UIX_STICKER_MODULE
 * @{
 */

/**
 * @brief Enumeration for sticker function error.
 *
 * @since_tizen 5.5
 */
typedef enum {
    STICKER_ERROR_NONE = TIZEN_ERROR_NONE, /**< Successful */
    STICKER_ERROR_NOT_SUPPORTED = TIZEN_ERROR_NOT_SUPPORTED, /**< Not supported */
    STICKER_ERROR_PERMISSION_DENIED = TIZEN_ERROR_PERMISSION_DENIED, /**< Permission denied */
    STICKER_ERROR_INVALID_PARAMETER = TIZEN_ERROR_INVALID_PARAMETER, /**< Invalid parameter */
    STICKER_ERROR_OUT_OF_MEMORY = TIZEN_ERROR_OUT_OF_MEMORY, /**< Out of memory */
    STICKER_ERROR_OPERATION_FAILED = TIZEN_ERROR_STICKER | 0x0001, /**< Operation failed */
    STICKER_ERROR_FILE_EXISTS = TIZEN_ERROR_FILE_EXISTS, /**< File exists */
    STICKER_ERROR_NO_DATA = TIZEN_ERROR_NO_DATA, /**< No data available */
    STICKER_ERROR_NO_SUCH_FILE = TIZEN_ERROR_NO_SUCH_FILE, /**< No such file */
} sticker_error_e;

/**
 * @}
 */
#ifdef __cplusplus
}
#endif

#endif /* __TIZEN_UIX_STICKER_ERROR_H__ */