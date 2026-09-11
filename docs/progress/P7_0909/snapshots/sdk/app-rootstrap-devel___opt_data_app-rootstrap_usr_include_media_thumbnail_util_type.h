/*
* Copyright (c) 2011 Samsung Electronics Co., Ltd All Rights Reserved
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


#ifndef __TIZEN_MULTIMEDIA_THUMBNAIL_UTIL_TYPE_H__
#define __TIZEN_MULTIMEDIA_THUMBNAIL_UTIL_TYPE_H__

#include <stddef.h>
#include <tizen.h>

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

/**
* @addtogroup CAPI_MEDIA_THUMBNAIL_UTIL_MODULE
 * @{
 */

/**
* @brief Definition for Thumbnail util Error Class.
* @since_tizen 2.4
*
*/
#define THUMBNAIL_UTIL_ERROR_CLASS				TIZEN_ERROR_THUMBNAIL_UTIL

/**
 * @ingroup CAPI_MEDIA_THUMBNAIL_UTIL_MODULE
 * @brief Enumeration for a thumbnail util error.
 * @since_tizen 2.4
 */
typedef enum {
	THUMBNAIL_UTIL_ERROR_NONE                    = TIZEN_ERROR_NONE,                    /**< Successful */
	THUMBNAIL_UTIL_ERROR_INVALID_PARAMETER       = TIZEN_ERROR_INVALID_PARAMETER,       /**< Invalid parameter */
	THUMBNAIL_UTIL_ERROR_OUT_OF_MEMORY           = TIZEN_ERROR_OUT_OF_MEMORY,           /**< Out of memory */
	THUMBNAIL_UTIL_ERROR_INVALID_OPERATION       = TIZEN_ERROR_INVALID_OPERATION,       /**< Invalid Operation */
	THUMBNAIL_UTIL_ERROR_FILE_NO_SPACE_ON_DEVICE       = TIZEN_ERROR_FILE_NO_SPACE_ON_DEVICE, /**< No space left on device */
	THUMBNAIL_UTIL_ERROR_PERMISSION_DENIED		= TIZEN_ERROR_PERMISSION_DENIED,		  /**< Permission denied */
	THUMBNAIL_UTIL_ERROR_UNSUPPORTED_CONTENT		= THUMBNAIL_UTIL_ERROR_CLASS | 0x01,    /**< Unsupported Content (Since 4.0) */
} thumbnail_util_error_e;

/**
 * @}
 */

#ifdef __cplusplus
}
#endif /* __cplusplus */


#endif /*__TIZEN_MULTIMEDIA_THUMBNAIL_UTIL_TYPE_H__*/
