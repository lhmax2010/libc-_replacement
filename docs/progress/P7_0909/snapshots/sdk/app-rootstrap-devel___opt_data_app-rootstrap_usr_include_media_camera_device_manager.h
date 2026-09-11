/*
* Copyright (c) 2023 Samsung Electronics Co., Ltd All Rights Reserved
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

#ifndef __TIZEN_MULTIMEDIA_CAMERA_DEVICE_MANAGER_H__
#define __TIZEN_MULTIMEDIA_CAMERA_DEVICE_MANAGER_H__

#include <camera_internal.h>

#ifdef __cplusplus
extern "C" {
#endif

#define CAMERA_DEVICE_MAX        ((CAMERA_DEVICE_CAMERA9 + 1) * 2)


/**
 * @internal
 * @brief The structure type of the camera device list.
 * @since_tizen 6.0
 */
typedef struct _camera_device_list_s {
	unsigned int count;
	camera_device_s device[CAMERA_DEVICE_MAX];
} camera_device_list_s;

/**
 * @internal
 * @brief Enumeration for camera device manager property type.
 * @since_tizen 8.0
 */
typedef enum {
	CAMERA_DEVICE_MANAGER_PROPERTY_TYPE_PRODUCT_ID = 0,
	CAMERA_DEVICE_MANAGER_PROPERTY_TYPE_VENDOR_ID
} camera_device_manager_property_type_e;

/**
 * @internal
 * @brief Gets a list of available camera devices.
 * @since_tizen 6.0
 * @param[in]  manager The handle to the camera device manager
 * @param[out] list    A list of available camera devices
 * @return @c 0 on success, otherwise a negative error value
 * @retval #CAMERA_ERROR_NONE Successful
 * @retval #CAMERA_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #CAMERA_ERROR_OUT_OF_MEMORY Out of memory
 * @retval #CAMERA_ERROR_INVALID_OPERATION Invalid operation
 * @retval #CAMERA_ERROR_NOT_SUPPORTED The feature is not supported
 */
int camera_device_manager_get_device_list(camera_device_manager_h manager, camera_device_list_s *list);

#ifdef __cplusplus
}
#endif

#endif /* __TIZEN_MULTIMEDIA_CAMERA_DEVICE_MANAGER_H__ */
