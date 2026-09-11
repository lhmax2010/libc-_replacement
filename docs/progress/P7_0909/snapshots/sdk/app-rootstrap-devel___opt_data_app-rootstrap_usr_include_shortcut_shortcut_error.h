/*
 * Copyright (c) 2011 - 2017 Samsung Electronics Co., Ltd. All rights reserved.
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

#ifndef __SHORTCUT_ERROR_H__
#define __SHORTCUT_ERROR_H__

#include <tizen.h>

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @file shortcut_error.h
 */

/**
 * @addtogroup SHORTCUT_MODULE
 * @{
 */

/**
 * @deprecated Deprecated since 9.0
 * @brief Enumeration for values of shortcut response types.
 * @since_tizen 2.3
 */
enum shortcut_error_e {
	SHORTCUT_ERROR_NONE = TIZEN_ERROR_NONE,				/**< Successful */
	SHORTCUT_ERROR_INVALID_PARAMETER = TIZEN_ERROR_INVALID_PARAMETER,	/**< Invalid function parameter */
	SHORTCUT_ERROR_OUT_OF_MEMORY = TIZEN_ERROR_OUT_OF_MEMORY,	/**< Out of memory */
	SHORTCUT_ERROR_IO_ERROR = TIZEN_ERROR_IO_ERROR,		/**< I/O Error */
	SHORTCUT_ERROR_PERMISSION_DENIED = TIZEN_ERROR_PERMISSION_DENIED,	/**< Permission denied */
	SHORTCUT_ERROR_NOT_SUPPORTED = TIZEN_ERROR_NOT_SUPPORTED,	/**< Not supported */
	SHORTCUT_ERROR_RESOURCE_BUSY = TIZEN_ERROR_RESOURCE_BUSY,		/**< Device or resource busy */
	SHORTCUT_ERROR_NO_SPACE = TIZEN_ERROR_SHORTCUT | 0x0001,	/**< There is no space to add a new shortcut */
	SHORTCUT_ERROR_EXIST = TIZEN_ERROR_SHORTCUT | 0x0002,		/**< Shortcut is already added */
	SHORTCUT_ERROR_FAULT = TIZEN_ERROR_SHORTCUT | 0x0004,		/**< Unrecoverable error */
	SHORTCUT_ERROR_NOT_EXIST = TIZEN_ERROR_SHORTCUT | 0x0008,	/**< Not exist shortcut(@b Since: 3.0) */
	SHORTCUT_ERROR_COMM = TIZEN_ERROR_SHORTCUT | 0x0040		/**< Connection not established or communication problem */
};

/**
 * @}
 */

#ifdef __cplusplus
}
#endif

#endif
