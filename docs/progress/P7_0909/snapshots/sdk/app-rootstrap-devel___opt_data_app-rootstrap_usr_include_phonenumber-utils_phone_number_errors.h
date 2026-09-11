/*
 * Phonenumber Utils
 *
 * Copyright (c) 2015 Samsung Electronics Co., Ltd. All rights reserved.
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
 *
 */

#ifndef __TIZEN_TELEPHONY_PHONE_NUMBER_UTILS_ERRORS_H__
#define __TIZEN_TELEPHONY_PHONE_NUMBER_UTILS_ERRORS_H__

#include <tizen.h>

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @file phone_number_errors.h
 */

/**
 * @addtogroup CAPI_TELEPHONY_PHONE_NUMBER_UTILS_MODULE
 *
 * @{
 */


/**
 * @brief Enumeration for phone number errors.
 * @since_tizen 2.4
 */
typedef enum {
	/* GENERAL */
	PHONE_NUMBER_ERROR_NONE = TIZEN_ERROR_NONE, /**< Successful */
	PHONE_NUMBER_ERROR_IO_ERROR = TIZEN_ERROR_IO_ERROR, /**< I/O Error */
	PHONE_NUMBER_ERROR_OUT_OF_MEMORY = TIZEN_ERROR_OUT_OF_MEMORY, /**< Out of memory */
	PHONE_NUMBER_ERROR_INVALID_PARAMETER = TIZEN_ERROR_INVALID_PARAMETER, /**< Invalid parameter */
	PHONE_NUMBER_ERROR_FILE_NO_SPACE_ON_DEVICE = TIZEN_ERROR_FILE_NO_SPACE_ON_DEVICE, /**< FS Full */
	PHONE_NUMBER_ERROR_PERMISSION_DENIED = TIZEN_ERROR_PERMISSION_DENIED, /**< Permission denied */
	PHONE_NUMBER_ERROR_NOT_SUPPORTED = TIZEN_ERROR_NOT_SUPPORTED, /**< Not supported */
	/* LOGIC & DATA */
	PHONE_NUMBER_ERROR_NO_DATA = TIZEN_ERROR_NO_DATA, /**< Requested data does not exist */
	/* ENVIRONMENT & OTHER MODULE */
	PHONE_NUMBER_ERROR_SYSTEM = TIZEN_ERROR_PHONENUMBER_UTILS | 0xEF,  /**< Internal error (Since 3.0) */
	PHONE_NUMBER_ERROR_DB_FAILED = TIZEN_ERROR_PHONENUMBER_UTILS | 0x02,  /**< No access to the database (Since 4.0) */
	PHONE_NUMBER_ERROR_IPC = TIZEN_ERROR_PHONENUMBER_UTILS | 0xBF,  /**< Unknown IPC error (Since 4.0) */
} phone_number_error_e;


/**
 * @}
 */

#ifdef __cplusplus
}
#endif

#endif /* __TIZEN_TELEPHONY_PHONE_NUMBER_UTILS_ERRORS_H__ */
