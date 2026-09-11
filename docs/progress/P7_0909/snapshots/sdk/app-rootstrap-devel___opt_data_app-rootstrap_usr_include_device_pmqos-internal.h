/*
 * Copyright (c) 2020 Samsung Electronics Co., Ltd All Rights Reserved
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


#ifndef __TIZEN_SYSTEM_PMQOS_H__
#define __TIZEN_SYSTEM_PMQOS_H__


#include <stdbool.h>
#include "device-error.h"


#ifdef __cplusplus
extern "C" {
#endif


/**
 * @addtogroup CAPI_SYSTEM_DEVICE_PMQOS_INTERNAL_MODULE
 * @{
 */

/**
 * @brief Request the PMQoS CPU boost for the AppLaunchHome scenario with timeout.
 * @details The @a timeout parameter specifies the duration of the CPU boost in milliseconds.
 * @since_tizen 6.0
 * @remarks The @a timeout parameter specifies the duration of the CPU boost in milliseconds. \n
 *          If the timeout value exceeds @c 3000 milliseconds, it will automatically be set to @c 3000 milliseconds.
 * @param[in] timeout Timeout to change state
 * @return @c 0 on success, otherwise a negative error value
 * @retval #DEVICE_ERROR_NONE Successful
 * @retval #DEVICE_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #DEVICE_ERROR_OPERATION_FAILED Operation failed
 * @retval #DEVICE_ERROR_NOT_SUPPORTED Not supported device
 *
 * @code
 * #include <stdio.h>
 * #include <device/pmqos-internal.h>
 * ...
 * int timeout = 3000; // Timeout in milliseconds
 * int ret = device_pmqos_app_launch_home(timeout);
 * if (ret != DEVICE_ERROR_NONE) {
 *     return -1;
 * }
 * printf("PMQoS CPU boost requested for App Launch Home scenario.\n");
 * ...
 * @endcode
 */
int device_pmqos_app_launch_home(int timeout);

/**
 * @brief Requests the PMQoS CPU boost for the HomeScreen scenario with timeout.
 * @details The @a timeout parameter specifies the duration of the CPU boost in milliseconds.
 * @since_tizen 6.0
 * @remarks The @a timeout parameter specifies the duration of the CPU boost in milliseconds. \n
 *          If the timeout value exceeds @c 3000 milliseconds, it will automatically be set to @c 3000 milliseconds.
 * @param[in] timeout Timeout to change state
 * @return @c 0 on success, otherwise a negative error value
 * @retval #DEVICE_ERROR_NONE Successful
 * @retval #DEVICE_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #DEVICE_ERROR_OPERATION_FAILED Operation failed
 * @retval #DEVICE_ERROR_NOT_SUPPORTED Not supported device
 *
 * @code
 * #include <stdio.h>
 * #include <device/pmqos.h>
 * ...
 * int timeout = 3000; // Timeout in milliseconds
 * int ret = device_pmqos_homescreen(timeout);
 * if (ret != DEVICE_ERROR_NONE) {
 *     return -1;
 * }
 * printf("PMQoS CPU boost requested for Home Screen scenario.\n");
 * ...
 * @endcode
 */
int device_pmqos_homescreen(int timeout);

/**
 * @}
 */


#ifdef __cplusplus
}
#endif


#endif  // __TIZEN_SYSTEM_PMQOS_H__
