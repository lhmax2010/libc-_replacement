/*
 * Copyright (c) 2024 Samsung Electronics Co., Ltd All Rights Reserved
 *
 *  Licensed under the Apache License, Version 2.0 (the "License");
 *  you may not use this file except in compliance with the License.
 *  You may obtain a copy of the License at
 *
 *               http://www.apache.org/licenses/LICENSE-2.0
 *
 *  Unless required by applicable law or agreed to in writing, software
 *  distributed under the License is distributed on an "AS IS" BASIS,
 *  WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 *  See the License for the specific language governing permissions and
 *  limitations under the License.
 *
 */


#ifndef __TIZEN_UIX_MMI_ERROR_H__
#define __TIZEN_UIX_MMI_ERROR_H__


#include <tizen_error.h>


/**
* @file mmi-error.h
*/


/**
* @addtogroup CAPI_UIX_MMI_COMMON_MODULE
* @{
*/


#ifdef __cplusplus
extern "C" {
#endif

/**
 * @brief Enumerations for MMI error codes.
 * @since_tizen 9.0
 */
typedef enum {
    MMI_ERROR_NONE = TIZEN_ERROR_NONE, /**< Successful */
    MMI_ERROR_OUT_OF_MEMORY = TIZEN_ERROR_OUT_OF_MEMORY, /**< Out of Memory */
    MMI_ERROR_IO_ERROR = TIZEN_ERROR_IO_ERROR, /**< I/O error */
    MMI_ERROR_INVALID_PARAMETER	= TIZEN_ERROR_INVALID_PARAMETER, /**< Invalid parameter */
    MMI_ERROR_TIMED_OUT = TIZEN_ERROR_TIMED_OUT, /**< No answer from the daemon */
    MMI_ERROR_PERMISSION_DENIED	= TIZEN_ERROR_PERMISSION_DENIED, /**< Permission denied */
    MMI_ERROR_NOT_SUPPORTED = TIZEN_ERROR_NOT_SUPPORTED, /**< MMI NOT supported */
    MMI_ERROR_OPERATION_FAILED = (TIZEN_ERROR_MMI | 0x01), /**< Operation failed */
    MMI_ERROR_RESOURCE_BUSY = (TIZEN_ERROR_MMI | 0x02), /**< Resource busy */
} mmi_error_e;


#ifdef __cplusplus
}
#endif


/**
 * @}
 */


#endif	/* __TIZEN_UIX_MMI_ERROR_H__ */
