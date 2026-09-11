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


#ifndef __TIZEN_SYSTEM_CALLBACK_H__
#define __TIZEN_SYSTEM_CALLBACK_H__


#include <stdbool.h>
#include "device-error.h"


#ifdef __cplusplus
extern "C" {
#endif


/**
 * @addtogroup CAPI_SYSTEM_DEVICE_CALLBACK_MODULE
 * @{
 */


/**
 * @brief Enumeration for the device state callback.
 * @since_tizen 2.3
 */
typedef enum {
    DEVICE_CALLBACK_BATTERY_CAPACITY, /**< Called when a battery charge percentage is changed */
    DEVICE_CALLBACK_BATTERY_LEVEL, /**< Called when a battery level is changed */
    DEVICE_CALLBACK_BATTERY_CHARGING, /**< Called when battery charging state is changed */
    DEVICE_CALLBACK_DISPLAY_STATE, /**< Called when a display state is changed */
    DEVICE_CALLBACK_FLASH_BRIGHTNESS, /**< Called when a flash brightness is changed (Since Tizen 2.4) */
    DEVICE_CALLBACK_MAX,
} device_callback_e;


/**
 * @brief Called when a device status is changed (e.g., battery_capacity, display_state, etc).
 * @details Each device callback has a different output parameter type. \n
 *          So you need to check below output parameter before using this function. \n
 *          callback enum / output type \n
 *          #DEVICE_CALLBACK_BATTERY_CAPACITY / int \n
 *          #DEVICE_CALLBACK_BATTERY_LEVEL / int \n
 *          #DEVICE_CALLBACK_BATTERY_CHARGING / bool \n
 *          #DEVICE_CALLBACK_DISPLAY_STATE / int \n
 *          #DEVICE_CALLBACK_FLASH_BRIGHTNESS / int
 * @since_tizen 2.3
 * @remarks #DEVICE_CALLBACK_FLASH_BRIGHTNESS callback invoked when user set flash brightness by using device_flash_set_brightness(). It does not work by camera flash operation. To register #DEVICE_CALLBACK_FLASH_BRIGHTNESS callback, you need to declare the LED privilege (%http://tizen.org/privilege/led).
 * @param[out] type The device type to monitor
 * @param[out] value The changed value
 * @param[out] user_data The user data passed from the callback registration function
 * @see device_add_callback
 * @see device_remove_callback
 */
typedef void (*device_changed_cb)(device_callback_e type, void *value, void *user_data);


/**
 * @brief Adds a callback to the observing device state (e.g., battery, display, etc).
 * @details Registers a callback function to be invoked when a specified device status changes. \n
 *          When the device status changes, the registered callback function will be called with the new status value as an argument.
 * @since_tizen 2.3
 * @privilege %http://tizen.org/privilege/internal/default/platform
 * @privilege %http://tizen.org/privilege/led
 * @remarks The following feature should be supported for #DEVICE_CALLBACK_DISPLAY_STATE: %http://tizen.org/feature/display. \n
 *          #DEVICE_CALLBACK_BATTERY_CAPACITY: %http://tizen.org/feature/battery. \n
 *          #DEVICE_CALLBACK_BATTERY_LEVEL: %http://tizen.org/feature/battery. \n
 *          #DEVICE_CALLBACK_BATTERY_CHARGING: %http://tizen.org/feature/battery. \n
 *          #DEVICE_CALLBACK_FLASH_BRIGHTNESS: %http://tizen.org/feature/camera.back.flash. \n
 *          Otherwise #DEVICE_ERROR_NOT_SUPPORTED is returned.
 * @param[in] type The device type to monitor
 * @param[in] callback The callback function to add
 * @param[in] user_data The user data to be passed to the callback function
 * @return @c 0 on success, otherwise a negative error value
 * @retval #DEVICE_ERROR_NONE Successful
 * @retval #DEVICE_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #DEVICE_ERROR_ALREADY_IN_PROGRESS Operation already
 * @retval #DEVICE_ERROR_OPERATION_FAILED Operation failed
 * @retval #DEVICE_ERROR_NOT_SUPPORTED Not supported device
 *
 * @code
 * #include <device/callback.h>
 * ...
 * static void device_display_state_changed_cb(device_callback_e type, void *value, void *user_data)
 * {
 *      int display_state = (int)((intptr_t) value);
 *      ...
 * }
 * ...
 * int main(void)
 * {
 *     int ret = 0;
 *     device_add_callback(DEVICE_CALLBACK_DISPLAY_STATE, device_display_state_changed_cb, NULL);
 *     ...
 * }
 * ...
 * @endcode
 * @see device_callback_e
 * @see device_changed_cb
 * @see device_remove_callback
 */
int device_add_callback(device_callback_e type, device_changed_cb callback, void *user_data);


/**
 * @brief Removes a device callback function that was added with device_add_callback().
 * @details After calling this, the specified callback function will no longer be called when the device status changes.
 * @since_tizen 2.3
 * @remarks The following feature should be supported for #DEVICE_CALLBACK_DISPLAY_STATE: %http://tizen.org/feature/display. \n
 *          #DEVICE_CALLBACK_BATTERY_CAPACITY: %http://tizen.org/feature/battery. \n
 *          #DEVICE_CALLBACK_BATTERY_LEVEL: %http://tizen.org/feature/battery. \n
 *          #DEVICE_CALLBACK_BATTERY_CHARGING: %http://tizen.org/feature/battery. \n
 *          #DEVICE_CALLBACK_FLASH_BRIGHTNESS: %http://tizen.org/feature/camera.back.flash. \n
 *          Otherwise #DEVICE_ERROR_NOT_SUPPORTED is returned.
 * @param[in] type The device type to monitor
 * @param[in] callback The callback function to remove
 * @return @c 0 on success, otherwise a negative error value
 * @retval #DEVICE_ERROR_NONE Successful
 * @retval #DEVICE_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #DEVICE_ERROR_OPERATION_FAILED Operation failed
 * @retval #DEVICE_ERROR_NOT_SUPPORTED Not supported device
 *
 * @code
 * #include <device/callback.h>
 * ...
 * static void device_display_state_changed_cb(device_callback_e type, void *value, void *user_data)
 * {
 *      int display_state = (int)((intptr_t) value);
 *      ...
 * }
 * ...
 * int main(void)
 * {
 *     int ret = 0;
 *     device_add_callback(DEVICE_CALLBACK_DISPLAY_STATE, device_display_state_changed_cb, NULL);
 *     ...
 *     device_remove_callback(DEVICE_CALLBACK_DISPLAY_STATE, device_display_state_changed_cb);
 * }
 * ...
 * @endcode
 * @see device_callback_e
 * @see device_changed_cb
 * @see device_add_callback
 */
int device_remove_callback(device_callback_e type, device_changed_cb callback);


/**
 * @}
 */


#ifdef __cplusplus
}
#endif


#endif  // __TIZEN_SYSTEM_CALLBACK_H__
