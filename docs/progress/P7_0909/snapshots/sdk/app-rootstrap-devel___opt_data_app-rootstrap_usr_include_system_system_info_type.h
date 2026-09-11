/*
 * Copyright (c) 2014 Samsung Electronics Co., Ltd All Rights Reserved
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


#ifndef __TIZEN_SYSTEM_SYSTEM_INFO_TYPE_H__
#define __TIZEN_SYSTEM_SYSTEM_INFO_TYPE_H__

#include <tizen.h>

#ifdef __cplusplus
extern "C"
{
#endif


/**
 * @addtogroup CAPI_SYSTEM_SYSTEM_INFO_MODULE
 * @{
 */


/**
 * @brief Enumeration for system information error codes.
 */
typedef enum {
	SYSTEM_INFO_ERROR_NONE = TIZEN_ERROR_NONE, /**< Successful */
	SYSTEM_INFO_ERROR_INVALID_PARAMETER = TIZEN_ERROR_INVALID_PARAMETER, /**< Invalid parameter */
	SYSTEM_INFO_ERROR_OUT_OF_MEMORY = TIZEN_ERROR_OUT_OF_MEMORY, /**< Out of memory */
	SYSTEM_INFO_ERROR_IO_ERROR = TIZEN_ERROR_IO_ERROR, /**< An input/output error occurred when reading value from system */
	SYSTEM_INFO_ERROR_PERMISSION_DENIED = TIZEN_ERROR_PERMISSION_DENIED, /**< No permission to use the API */
	SYSTEM_INFO_ERROR_NOT_SUPPORTED = TIZEN_ERROR_NOT_SUPPORTED, /**< Not supported parameter (Since 3.0) */
} system_info_error_e;


/**
 * @internal
 * @brief It is not decided if it should be opened to public.
 */
typedef enum {
	SYSTEM_INFO_TYPE_MIN = 0,
	SYSTEM_INFO_BOOL = SYSTEM_INFO_TYPE_MIN,
	SYSTEM_INFO_INT,
	SYSTEM_INFO_DOUBLE,
	SYSTEM_INFO_STRING,
	SYSTEM_INFO_TYPE_MAX,
} system_info_type_e;

/**
 * @internal
 * @brief Enumeration for system information key.
 */
typedef enum {
	SYSTEM_INFO_KEY_MODEL, /**< @internal The model of the device */
	SYSTEM_INFO_KEY_TIZEN_VERSION, /**< @internal The version of the Tizen API */
	SYSTEM_INFO_KEY_PLATFORM_NAME, /**< @internal The name of platform */
	SYSTEM_INFO_KEY_TIZEN_VERSION_NAME, /**< @internal The name of tizen version  */
	SYSTEM_INFO_KEY_MANUFACTURER, /**< @internal The manufacturer of the device */
	SYSTEM_INFO_KEY_CORE_CPU_ARCH, /**< @internal The CORE CPU architecture of model */
	SYSTEM_INFO_KEY_CORE_CPU_FREQ, /**< @internal The CORE CPU frequency of model */
	SYSTEM_INFO_KEY_BUILD_STRING, /**< @internal The build string of platform binary */
	SYSTEM_INFO_KEY_BUILD_DATE, /**< @internal The build date of platform binary */
	SYSTEM_INFO_KEY_BUILD_TIME, /**< @internal The build time of platform binary */
	SYSTEM_INFO_KEY_SCREEN_HEIGHT, /**< @internal The height of the screen in pixels */
	SYSTEM_INFO_KEY_SCREEN_WIDTH, /**< @internal The width of the screen in pixels */
	SYSTEM_INFO_KEY_PHYSICAL_SCREEN_HEIGHT, /**< @internal The physical screen height in millimeters */
	SYSTEM_INFO_KEY_PHYSICAL_SCREEN_WIDTH, /**< @internal The physical screen width in millimeters */
	SYSTEM_INFO_KEY_TETHERING_SUPPORTED, /**< @internal Indicates whether the device supports tethering */
} system_info_key_e;


/**
 * @}
 */

#ifdef __cplusplus
}
#endif

#endif /* __TIZEN_SYSTEM_SYSTEM_INFO_TYPE_H__ */
