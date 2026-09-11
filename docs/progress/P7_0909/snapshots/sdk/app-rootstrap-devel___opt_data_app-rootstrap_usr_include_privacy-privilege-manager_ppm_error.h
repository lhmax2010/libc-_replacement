/*
 *  Copyright (c) 2020 Samsung Electronics Co., Ltd All Rights Reserved
 *
 *  Licensed under the Apache License, Version 2.0 (the "License");
 *  you may not use this file except in compliance with the License.
 *  You may obtain a copy of the License at
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 *
 *  Unless required by applicable law or agreed to in writing, software
 *  distributed under the License is distributed on an "AS IS" BASIS,
 *  WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 *  See the License for the specific language governing permissions and
 *  limitations under the License
 */

#ifndef __PPM_ERROR_H__
#define __PPM_ERROR_H__

#include <tizen.h>

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @addtogroup CAPI_PRIVACY_PRIVILEGE_MANAGER_MODULE
 * @{
 */

/**
 * @brief Enumeration for error codes of Privacy Privilege Manager.
 * @since_tizen 4.0
 */
typedef enum
{
    /** Successful */
    PRIVACY_PRIVILEGE_MANAGER_ERROR_NONE                 = TIZEN_ERROR_NONE,
    /** I/O error */
    PRIVACY_PRIVILEGE_MANAGER_ERROR_IO_ERROR             = TIZEN_ERROR_IO_ERROR,
    /** Invalid parameter */
    PRIVACY_PRIVILEGE_MANAGER_ERROR_INVALID_PARAMETER    = TIZEN_ERROR_INVALID_PARAMETER,
    /** Operation already in progress */
    PRIVACY_PRIVILEGE_MANAGER_ERROR_ALREADY_IN_PROGRESS  = TIZEN_ERROR_ALREADY_IN_PROGRESS,
    /** Out of memory */
    PRIVACY_PRIVILEGE_MANAGER_ERROR_OUT_OF_MEMORY        = TIZEN_ERROR_OUT_OF_MEMORY,
    /** Permission denied */
    PRIVACY_PRIVILEGE_MANAGER_ERROR_PERMISSION_DENIED    = TIZEN_ERROR_PERMISSION_DENIED,
    /** Unknown error */
    PRIVACY_PRIVILEGE_MANAGER_ERROR_UNKNOWN              = TIZEN_ERROR_UNKNOWN,
    /** Not implemented yet */
    PRIVACY_PRIVILEGE_MANAGER_ERROR_NOT_IMPLEMENTED      = TIZEN_ERROR_INVALID_OPERATION,
} ppm_error_e;

/**
 * @}
 */

#ifdef __cplusplus
}
#endif

#endif /* __PPM_ERROR_H__ */
