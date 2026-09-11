/*
 * Copyright (c) 2018 Samsung Electronics Co., Ltd All Rights Reserved
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

#ifndef __TIZEN_UIX_AUTOFILL_ERROR_H__
#define __TIZEN_UIX_AUTOFILL_ERROR_H__

/**
 * @file autofill_error.h
 * @brief This file contains autofill error enumeration.
 */

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @addtogroup CAPI_UIX_AUTOFILL_COMMON_MODULE
 * @{
 */

/**
 * @brief Enumeration for autofill function error.
 *
 * @since_tizen 5.5
 */
typedef enum {
    AUTOFILL_ERROR_NONE = TIZEN_ERROR_NONE, /**< Successful */
    AUTOFILL_ERROR_INVALID_PARAMETER = TIZEN_ERROR_INVALID_PARAMETER, /**< Invalid parameter */
    AUTOFILL_ERROR_PERMISSION_DENIED = TIZEN_ERROR_PERMISSION_DENIED, /**< Permission denied */
    AUTOFILL_ERROR_NOT_INITIALIZED = TIZEN_ERROR_AUTOFILL | 0x0001, /**< Not initialized */
    AUTOFILL_ERROR_OPERATION_FAILED = TIZEN_ERROR_AUTOFILL | 0x0002, /**< Operation failed */
    AUTOFILL_ERROR_OUT_OF_MEMORY = TIZEN_ERROR_OUT_OF_MEMORY, /**< Out of memory */
    AUTOFILL_ERROR_AUTHENTICATION_FAILED = TIZEN_ERROR_AUTOFILL | 0x0003, /**< Authentication failed */
    AUTOFILL_ERROR_COMMIT_FAILED = TIZEN_ERROR_AUTOFILL | 0x0004, /**< Failed to save autofill data */
    AUTOFILL_ERROR_FILL_RESPONSE_FAILED = TIZEN_ERROR_AUTOFILL | 0x0005, /**< Failed to response fill request */
    AUTOFILL_ERROR_SERVICE_NOT_CONNECTED = TIZEN_ERROR_AUTOFILL | 0x0006, /**< Unable to connect to Autofill Service */
    AUTOFILL_ERROR_SERVICE_NOT_ALLOWED = TIZEN_ERROR_AUTOFILL | 0x0007, /**< Autofill Service is not allowed */
    AUTOFILL_ERROR_SERVICE_NOT_ACTIVATED = TIZEN_ERROR_AUTOFILL | 0x0008, /**< Autofill Service is not activated */
    AUTOFILL_ERROR_SAVED_VALUES_NOT_FOUND = TIZEN_ERROR_AUTOFILL | 0x0009, /**< Could not find saved values */
} autofill_error_e;

/**
 * @}
 */
#ifdef __cplusplus
}
#endif

#endif /* __TIZEN_UIX_AUTOFILL_ERROR_H__ */
