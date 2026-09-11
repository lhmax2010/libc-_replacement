/*
 * Copyright (c) 2014 Samsung Electronics Co., Ltd All Rights Reserved
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


#ifndef __TIZEN_SYSTEM_POWER_H__
#define __TIZEN_SYSTEM_POWER_H__


#include <stdbool.h>
#include "device-error.h"


#ifdef __cplusplus
extern "C" {
#endif


/**
 * @addtogroup CAPI_SYSTEM_DEVICE_POWER_MODULE
 * @{
 */


/**
 * @brief Enumeration for lock type.
 * @details Each enum ensures that the suitable device is on until all the lock requests have been released or after a timeout.
 * <TABLE>
 * <TR><TH>Enum Type</TH><TH>CPU</TH><TH>Brightness(Display)</TH></TR>
 * <TR><TD>POWER_LOCK_CPU</TD><TD>ON</TD><TD>OFF</TD></TR>
 * <TR><TD>POWER_LOCK_DISPLAY</TD><TD>ON</TD><TD>ON(Normal)</TD></TR>
 * <TR><TD>POWER_LOCK_DISPLAY_DIM</TD><TD>ON</TD><TD>ON(Dim)</TD></TR>
 * </TABLE>
 * @since_tizen 2.3
 * @remarks An application can lock the specific type.
 * @remarks These enums are mutually exclusive.
 * @remarks You cannot combine with an enum below.
 * @remarks #POWER_LOCK_DISPLAY_DIM may be ignored if the DIM state is disabled on the platform.
 */
typedef enum {
    POWER_LOCK_CPU, /**< CPU lock */
    POWER_LOCK_DISPLAY, /**< Display normal lock */
    POWER_LOCK_DISPLAY_DIM, /**< Display dim lock */
} power_lock_e;


/**
 * @brief Locks the given specific power lock type for a specified time.
 * @details After the given @a timeout_ms (in milliseconds), unlock the given lock state automatically.
 * Scenario 1. If the same application requests the same power lock more than twice before it is released or expired, \n
 * the timeout will be updated to the latest requested time. \n
 * For example:
 *     The first request has a timeout of 5000ms. \n
 *     The second request has a timeout of 1000ms. \n
 *     If the second request is made before the first request is released or expired, \n
 *     the timeout will be updated to the latest requested time, which is 1000ms.
 *
 * Scenario 2. If different applications request the same power lock type, \n
 * the power state timeout will be set to the maximum of the requested timeouts. \n
 * For example:
 *     Application A requests POWER_LOCK_CPU with a timeout of 5000ms. \n
 *     Application B requests the same POWER_LOCK_CPU with a timeout of 1000ms. \n
 *     Unless Application A releases the POWER_LOCK_CPU lock, \n
 *     the timeout for POWER_LOCK_CPU will remain at the maximum requested time, which is 5000ms.
 *
 * @since_tizen 2.3
 * @privlevel public
 * @privilege %http://tizen.org/privilege/display
 * @remarks If the process dies after success request lock, then every lock will be removed.
 *          If display feature(http://tizen.org/feature/display) is false, then only the type POWER_LOCK_CPU is effective,
 *          and the other type returns DEVICE_ERROR_NOT_SUPPORTED. And even thoguh the display feature is false, it requires
 *          display privilege(http://tizen.org/privilege/display).
 * @param[in] type The power type to request lock
 * @param[in] timeout_ms The positive number in milliseconds or @c 0 for permanent lock \n
 *                       So you must release the permanent lock of power state with #device_power_release_lock() if @a timeout_ms is zero
 * @return @c 0 on success, otherwise a negative error value
 * @retval #DEVICE_ERROR_NONE Successful
 * @retval #DEVICE_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #DEVICE_ERROR_PERMISSION_DENIED Permission denied
 * @retval #DEVICE_ERROR_OPERATION_FAILED Operation failed
 *
 * @code
 * #include <device/power.h>
 * ...
 * int ret = 0;
 * ret = device_power_request_lock(POWER_LOCK_DISPLAY, 5000);
 * if (ret != DEVICE_ERROR_NONE) {
 *     ....
 * }
 * ...
 * @endcode
 * @see device_power_release_lock()
 * @see power_lock_e
 */
int device_power_request_lock(power_lock_e type, int timeout_ms);


/**
 * @brief Releases the given specific power lock type which was locked before.
 * @details Releases the lock of specific power lock type that was previously acquired using device_power_request_lock().
 * @since_tizen 2.3
 * @privlevel public
 * @privilege %http://tizen.org/privilege/display
 * @remarks If display feature(http://tizen.org/feature/display) is false, then only the type POWER_LOCK_CPU is effective,
 *          and the other type returns DEVICE_ERROR_NOT_SUPPORTED. And even thoguh the display feature is false, it requires
 *          display privilege(http://tizen.org/privilege/display).
 * @param[in] type The power type to release lock
 * @return @c 0 on success, otherwise a negative error value
 * @retval #DEVICE_ERROR_NONE Successful
 * @retval #DEVICE_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #DEVICE_ERROR_PERMISSION_DENIED Permission denied
 * @retval #DEVICE_ERROR_OPERATION_FAILED Operation failed
 *
 * @code
 * #include <device/power.h>
 * ...
 * int ret = 0;
 * ret = device_power_request_lock(POWER_LOCK_DISPLAY, 5000);
 * if (ret != DEVICE_ERROR_NONE) {
 *     ....
 * }
 * ret = device_power_release_lock(POWER_LOCK_DISPLAY);
 * ...
 * @endcode
 * @see device_power_request_lock()
 * @see power_lock_e
 */
int device_power_release_lock(power_lock_e type);


/**
 * @deprecated Deprecated Since 2.4. [Use device_display_change_state() instead.]
 * @brief Changes the current power state to the normal/dim state.
 * @since_tizen 2.3
 * @privlevel public
 * @privilege %http://tizen.org/privilege/display
 * @remarks This API triggers turn on process and then updates the status when it completes. While the operation is on-going, the device_display_get_state() function returns previous display state.
 * @remarks #DEVICE_ERROR_NOT_SUPPORTED is returned, when the following feature is not supported: %http://tizen.org/feature/display.state.
 * @param[in] dim Set @c true to set the dim state,
 *                otherwise set @c false to not set the dim state
 * @return @c 0 on success,
 *         otherwise a negative error value
 * @retval #DEVICE_ERROR_NONE Successful
 * @retval #DEVICE_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #DEVICE_ERROR_PERMISSION_DENIED Permission denied
 * @retval #DEVICE_ERROR_OPERATION_FAILED Operation failed
 * @retval #DEVICE_ERROR_NOT_SUPPORTED Not supported device
 * @post The device will be in #DISPLAY_STATE_NORMAL state.
 */
int device_power_wakeup(bool dim) TIZEN_DEPRECATED_API;


/**
 * @platform
 * @brief Sends a request to the deviced Rebooting the current device.
 * @details Will not return if the reboot is successful. \n
 *          It operates asynchronously.
 * @since_tizen 2.3.1
 * @privlevel platform
 * @privilege %http://tizen.org/privilege/reboot
 * @remarks If the power reboot @a reason is NULL, then it will be set as @c "Unknown".
 * @param[in] reason Pass to the platform and kernel to request special reboot reason, or null
 * @return @c 0 on success, otherwise a negative error value
 * @retval #DEVICE_ERROR_NONE Successful
 * @retval #DEVICE_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #DEVICE_ERROR_PERMISSION_DENIED Permission denied
 * @retval #DEVICE_ERROR_OPERATION_FAILED Operation failed
 *
 * @code
 * #include <device/power.h>
 * ...
 * int ret = device_power_reboot("Reason_for_reboot");
 * if (ret != DEVICE_ERROR_NONE) {
 *    ...
 * }
 * ...
 * @endcode
 * @see device_power_check_reboot_allowed()
 */
int device_power_reboot(const char *reason);


/**
 * @}
 */


#ifdef __cplusplus
}
#endif


#endif  // __TIZEN_SYSTEM_POWER_H__
