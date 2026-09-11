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

#ifndef __TIZEN_SYSTEM_TOUCHSCREEN_INTERNAL_H__
#define __TIZEN_SYSTEM_TOUCHSCREEN_INTERNAL_H__

/**
 * @addtogroup CAPI_SYSTEM_DEVICE_TOUCHSCREEN_INTERNAL_MODULE
 * @{
 */

#include <glib.h>

/**
 * @brief Called when a dbus callback is received.
 * @details If async dbus call is successful, the callback will be invoked with a valid GVariant object to @a var.
 * @since_tizen 7.0
 * @param[out] var If var is set to NULL, it means that the callback is invoked with an error.
 * @param[out] user_data The user data passed to callback
 * @param[out] GError Error information
 * @see device_touchscreen_enable()
 * @see device_touchscreen_disable()
 */
typedef void (*touchscreen_pending_cb)(GVariant *var, void *user_data, GError *err);

/**
 * @brief Enables the current device's touchscreen device to start.
 * @details Enables the touchscreen device and registers a callback function to be invoked when the operation is completed asynchronously.
 * @since_tizen 6.0
 * @privilege %http://tizen.org/privilege/internal/default/platform
 * @remarks Ensure the callback @a cb signature is the as described below.
 * @param[in] cb The callback function to be called when the operation is completed.
 *               The callback function should have the following signature:
 *               void callback(GVariant *result, void *data, GError *err);
 * @return 0 on success, otherwise a negative error value.
 * @retval #DEVICE_ERROR_NONE Successful
 * @retval #DEVICE_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #DEVICE_ERROR_PERMISSION_DENIED Permission denied
 * @retval #DEVICE_ERROR_OPERATION_FAILED Operation failed
 *
 * @code
 * #include <device/touchscreen-internal.h>
 * ...
 * void dbus_cb(GVariant *result, void *data, GError *err)
 * {
 *     int temp = 0;
 *
 *     if (!result) {
 *         _E("Can't get result of touchscreen enable request.:%s", err->message);
 *         return;
 *     }
 *     if (!g_variant_get_safe(result, "(i)", &temp)) {
 *         _E("Failed to get variant(%s): no call back message", g_variant_get_type_string(result));
 *         goto out;
 *     }
 *     _I("replay message(%d)", temp);
 * out:
 *     g_variant_unref(result);
 * }
 * ...
 * int main(void)
 * {
 *     int ret = 0;
 *
 *     ret = device_touchscreen_enable(dbus_cb);
 *     ...
 *     return 0;
 * }
 * ...
 * @endcode
 * @see device_touchscreen_disable()
 */
int device_touchscreen_enable(touchscreen_pending_cb cb);

/**
 * @brief Disables the current device's touchscreen device to stop.
 * @details Disables the touchscreen device and registers a callback function to be invoked when the operation is completed asynchronously.
 * @since_tizen 6.0
 * @privilege %http://tizen.org/privilege/internal/default/platform
 * @remarks Ensure the callback @a cb signature is the as described below.
 * @param[in] cb The callback function to be called when the operation is completed.
 *               The callback function should have the following signature:
 *               void callback(GVariant *result, void *data, GError *err);
 * @return 0 on success, otherwise a negative error value.
 * @retval #DEVICE_ERROR_NONE Successful
 * @retval #DEVICE_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #DEVICE_ERROR_PERMISSION_DENIED Permission denied
 * @retval #DEVICE_ERROR_OPERATION_FAILED Operation failed
 *
 * @code
 * #include <device/touchscreen-internal.h>
 * ...
 * void dbus_cb(GVariant *result, void *data, GError *err)
 * {
 *     int temp = 0;
 *
 *     if (!result) {
 *         _E("Can't get result of touchscreen device api request.:%s", err->message);
 *         return;
 *     }
 *     if (!g_variant_get_safe(result, "(i)", &temp)) {
 *         _E("Failed to get variant(%s): no call back message", g_variant_get_type_string(result));
 *         goto out;
 *     }
 *     _I("replay message(%d)", temp);
 * out:
 *     g_variant_unref(result);
 * }
 * ...
 * int main(void)
 * {
 *     int ret = 0;
 *
 *     ret = device_touchscreen_enable(dbus_cb);
 *     if (ret == DEVICE_ERROR_NONE) {
 *         ...
 *         ret = device_touchscreen_disable(dbus_cb);
 *     }
 *     return 0;
 * }
 * ...
 * @endcode
 * @see device_touchscreen_enable()
 */
int device_touchscreen_disable(touchscreen_pending_cb cb);

/**
 * @}
 */

#endif
