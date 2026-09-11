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


#ifndef __TIZEN_SYSTEM_DISPLAY_H__
#define __TIZEN_SYSTEM_DISPLAY_H__


#include "device-error.h"
#include "display-enum.h"

#ifdef __cplusplus
extern "C" {
#endif


/**
 * @addtogroup CAPI_SYSTEM_DEVICE_DISPLAY_MODULE
 * @{
 */


/**
 * @brief Gets the number of display devices connected to current device.
 * @details Retrieves the number of display devices connected to the system.\n
 *          The number of displays is returned in the @a device_number parameter.
 * @since_tizen 2.3
 * @privlevel public
 * @privilege %http://tizen.org/privilege/display
 * @remarks #DEVICE_ERROR_NOT_SUPPORTED is returned, when the following feature is not supported: %http://tizen.org/feature/display
 * @param[out] device_number A pointer to an integer where the total number of displays will be stored
 * @return @c 0 on success, otherwise a negative error value
 * @retval #DEVICE_ERROR_NONE Successful
 * @retval #DEVICE_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #DEVICE_ERROR_PERMISSION_DENIED Permission denied
 * @retval #DEVICE_ERROR_OPERATION_FAILED Operation failed
 * @retval #DEVICE_ERROR_NOT_SUPPORTED Not supported device
 *
 * @code
 * #include <device/display.h>
 * ...
 * int number_of_display = 0;
 * int ret = 0;
 * ...
 * ret = device_display_get_numbers(&number_of_display);
 * ...
 * @endcode
 * @see device_display_get_brightness()
 * @see device_display_set_brightness()
 * @see device_display_get_max_brightness()
 */
int device_display_get_numbers(int *device_number);


/**
 * @brief Gets the maximum brightness value that can be set based on the display index.
 * @details Retrieves the maximum brightness level of a specific display device.\n
 *          The display device is identified by its index, which can be obtained using the device_get_display_numbers() function.\n
 *          The maximum brightness level is returned in the @a max_brightness parameter.
 * @since_tizen 2.3
 * @privlevel public
 * @privilege %http://tizen.org/privilege/display
 * @remarks #DEVICE_ERROR_NOT_SUPPORTED is returned, when the following feature is not supported: %http://tizen.org/feature/display \n
 *          or when the display HW module (e.g., plug-in touchscreen) is detached.
 * @param[in] display_index The index of the display \n
 *                          It can be greater than or equal to @c 0 and less than the number of displays returned by device_display_get_numbers(). \n
 *                          The index zero is always assigned to the main display
 * @param[out] max_brightness The maximum brightness value of the display
 * @return @c 0 on success, otherwise a negative error value
 * @retval #DEVICE_ERROR_NONE Successful
 * @retval #DEVICE_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #DEVICE_ERROR_PERMISSION_DENIED Permission denied
 * @retval #DEVICE_ERROR_OPERATION_FAILED Operation failed
 * @retval #DEVICE_ERROR_NOT_SUPPORTED Not supported device
 *
 * @code
 * #include <device/display.h>
 * ...
 * int number_of_display = 0, brightness = 0, display_index = 0;
 * int ret = 0;
 * ...
 * ret = device_display_get_numbers(&number_of_display);
 * if (ret == DEVICE_ERROR_NONE) {
 *     display_index = number_of_display - 1;
 *     device_display_get_max_brightness(display_index, brightness);
 *     ...
 * }
 * ...
 * @endcode
 * @see device_display_get_numbers()
 * @see device_display_set_brightness()
 * @see device_display_get_brightness()
 */
int device_display_get_max_brightness(int display_index, int *max_brightness);


/**
 * @brief Gets the display brightness value based on the display index.
 * @details Retrieves the current brightness level of a specific display device.\n
 *          The display device is identified by its index, which can be obtained using the device_get_display_numbers().
 * @since_tizen 2.3
 * @privlevel public
 * @privilege %http://tizen.org/privilege/display
 * @remarks #DEVICE_ERROR_NOT_SUPPORTED is returned, when the following feature is not supported: %http://tizen.org/feature/display \n
 *          or when the display HW module (e.g., plug-in touchscreen) is detached.
 * @param[in] display_index The index of the display \n
 *                          It can be greater than or equal to @c 0 and less than the number of displays returned by device_display_get_numbers(). \n
 *                          The index zero is always assigned to the main display
 * @param[out] brightness The current brightness value of the display
 * @return @c 0 on success,
 *         otherwise a negative error value
 * @retval #DEVICE_ERROR_NONE Successful
 * @retval #DEVICE_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #DEVICE_ERROR_PERMISSION_DENIED Permission denied
 * @retval #DEVICE_ERROR_OPERATION_FAILED Operation failed
 * @retval #DEVICE_ERROR_NOT_SUPPORTED Not supported device
 *
 * @code
 * #include <device/display.h>
 * ...
 * int number_of_display = 0, brightness = 0, display_index = 0;
 * int ret = 0;
 * ...
 * ret = device_display_get_numbers(&number_of_display);
 * if (ret == DEVICE_ERROR_NONE) {
 *     display_index = number_of_display - 1;
 *     device_display_get_brightness(display_index, &brightness);
 *     ...
 * }
 * ...
 * @endcode
 * @see device_display_get_numbers()
 * @see device_display_set_brightness()
 * @see device_display_get_max_brightness()
 */
int device_display_get_brightness(int display_index, int *brightness);


/**
 * @brief Sets the display brightness value based on the display index.
 * @details Sets the brightness level of a specific display device.\n
 *          The display device is identified by its index, which can be obtained using the device_get_display_numbers().
 * @since_tizen 2.3
 * @privlevel public
 * @privilege %http://tizen.org/privilege/display
 * @remarks #DEVICE_ERROR_NOT_SUPPORTED is returned, when the following feature is not supported: %http://tizen.org/feature/display \n
 *          or when the display HW module (e.g., plug-in touchscreen) is detached.
 * @param[in] display_index The index of the display \n
 *                          It can be greater than or equal to @c 0 and less than the number of displays returned by device_display_get_numbers(). \n
 *                          The index zero is always assigned to the main display
 * @param[in] brightness The new brightness value to set \n
 *                       The maximum value can be represented by device_display_get_max_brightness()
 * @return @c 0 on success, otherwise a negative error value
 * @retval #DEVICE_ERROR_NONE Successful
 * @retval #DEVICE_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #DEVICE_ERROR_PERMISSION_DENIED Permission denied
 * @retval #DEVICE_ERROR_OPERATION_FAILED Operation failed
 * @retval #DEVICE_ERROR_NOT_SUPPORTED Not supported device
 *
 * @code
 * #include <system/device.h>
 * ...
 * int number_of_display = 0, brightness = 100, display_index = 0;
 * int ret = 0;
 * ...
 * ret = device_display_get_numbers(&number_of_display);
 * if (ret == DEVICE_ERROR_NONE) {
 *     display_index = number_of_display - 1;
 *     device_display_set_brightness(display_index, brightness);
 *     ...
 * }
 * ...
 * @endcode
 * @see device_display_get_numbers()
 * @see device_display_get_max_brightness()
 * @see device_display_get_brightness()
 */
int device_display_set_brightness(int display_index, int brightness);


/**
 * @brief Gets the current device's display state, including normal, dim, and off states.
 * @details Gets the current state of the display.\n
 *         The current display state is returned in the @a state parameter.
 * @since_tizen 2.3
 * @remarks #DEVICE_ERROR_NOT_SUPPORTED is returned, when the following feature is not supported: %http://tizen.org/feature/display
 * @param[out] state The display state
 *                  - DISPLAY_STATE_NORMAL: The normal display state.\n
 *                  - DISPLAY_STATE_DIM: The dimmed display state.\n
 *                  - DISPLAY_STATE_OFF: The off display state.
 * @return @c 0 on success, otherwise a negative error value
 * @retval #DEVICE_ERROR_NONE Successful
 * @retval #DEVICE_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #DEVICE_ERROR_OPERATION_FAILED Operation failed
 * @retval #DEVICE_ERROR_NOT_SUPPORTED Not supported device
 *
 * @code
 * #include <system/device.h>
 * ...
 * display_state_e state;
 * int ret = 0;
 * ...
 * ret = device_display_get_state(&state);
 * if (ret == DEVICE_ERROR_NONE) {
 *     ...
 * }
 * ...
 * @endcode
 * @see device_add_callback
 * @see device_remove_callback
 * @see #DEVICE_CALLBACK_DISPLAY_STATE
 */
int device_display_get_state(display_state_e *state);


/**
 * @brief Changes the current device's display state to the specified state by force.
 * @details It should be checked the profile version and supported display state.
 * @since_tizen 2.3
 * @privlevel public
 * @privilege %http://tizen.org/privilege/display
 * @remarks This API triggers display change process and then updates the status when it completes. While the operation is on-going, the device_display_get_state() function returns previous display state.
 * @remarks #DEVICE_ERROR_NOT_SUPPORTED is returned, when the following feature is not supported: %http://tizen.org/feature/display.state.
 * @param[in] state The display state
 * @return @c 0 on success, otherwise a negative error value
 * @retval #DEVICE_ERROR_NONE Successful
 * @retval #DEVICE_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #DEVICE_ERROR_PERMISSION_DENIED Permission denied
 * @retval #DEVICE_ERROR_OPERATION_FAILED Operation failed
 * @retval #DEVICE_ERROR_NOT_SUPPORTED Not supported device
 * @par Example
 *
 * @code
 *  ...
 *  result = device_display_change_state(DISPLAY_STATE_SCREEN_OFF);
 *  if (result < 0)
 *      dlog_print(DLOG_ERROR, LOG_TAG, "[ERROR] return value result =%d, \n",result);
 *  else
 *      dlog_print(DLOG_DEBUG, LOG_TAG, "[SUCCESS] return value result =%d \n",result);
 *  ...
 * @endcode
 * @see device_power_request_lock()
 * @see device_power_release_lock()
 * @see device_add_callback
 * @see device_remove_callback
 * @see #DEVICE_CALLBACK_DISPLAY_STATE
 */
int device_display_change_state(display_state_e state);


/**
 * @}
 */


#ifdef __cplusplus
}
#endif


#endif  // __TIZEN_SYSTEM_DISPLAY_H__
