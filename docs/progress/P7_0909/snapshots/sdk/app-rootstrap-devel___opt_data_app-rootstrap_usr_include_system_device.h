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




#ifndef __TIZEN_SYSTEM_DEVICE_H__
#define __TIZEN_SYSTEM_DEVICE_H__

#include <stdbool.h>
#include "device-error.h"

#ifdef __cplusplus
extern "C" {
#endif


/**
 * @addtogroup CAPI_SYSTEM_DEVICE_MODULE
 * @{
 */

/**
 * @brief Gets the number of display devices connected to current device.
 * @details Retrieves the number of display devices connected to the system.\n
 *          The number of displays is returned in the device_number parameter.
 * @since_tizen 2.1
 * @remarks Ensure that the provided @a device_number pointer is valid and has enough memory allocated.
 * @param[out] device_number A pointer to an integer where the number of display devices will be stored
 * @return @c 0 on success, otherwise a negative error value
 * @retval #DEVICE_ERROR_NONE Successful
 * @retval #DEVICE_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #DEVICE_ERROR_OPERATION_FAILED Operation failed
 * @retval #DEVICE_ERROR_NOT_SUPPORTED Not supported device
 *
 * @code
 * #include <system/device.h>
 * ...
 * int number_of_display = 0;
 * int ret = 0;
 * ...
 * ret = device_get_display_numbers(&number_of_display);
 * ...
 * @endcode
 * @see	device_get_brightness()
 * @see device_set_brightness()
 * @see device_get_max_brightness()
 * @see device_set_brightness_from_settings()
 * @see device_set_brightness_to_settings()
 */
int device_get_display_numbers(int* device_number);

/**
 * @brief Gets the display brightness value based on the display index.
 * @details Retrieves the current brightness level of a specific display device.\n
 *          The display device is identified by its index, which can be obtained using the device_get_display_numbers().\n
 *          The current brightness level is returned in the brightness parameter.
 * @since_tizen 2.1
 * @remarks Ensure that the provided @a brightness pointer is valid and has enough memory allocated.
 * @param[in] display_index	The index of the display, it be greater than or equal to @c 0 and less than \n
 *                          the number of displays returned by device_get_display_numbers().\n
 *                          The index zero is always assigned to the main display
 * @param[out] brightness	The current brightness value of the display
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
 * int number_of_display = 0, brightness = 0, display_index = 0;
 * int ret = 0;
 * ...
 * ret = device_get_display_numbers(&number_of_display);
 * if (ret == DEVICE_ERROR_NONE) {
 *     display_index = number_of_display - 1;
 *     device_get_brightness(display_index, &brightness);
 *     ...
 * }
 * ...
 * @endcode
 * @see device_get_display_numbers()
 * @see device_set_brightness()
 * @see device_get_max_brightness()
 * @see device_set_brightness_from_settings()
 * @see device_set_brightness_to_settings()
 */
int device_get_brightness(int display_index, int *brightness);

/**
 * @deprecated Deprecated since 3.0 [Use device_display_set_brightness() instead.]
 * @brief Sets the display brightness value based on the display index.
 * @details Sets the brightness level of a specific display device.\n
 *          The display device is identified by its index, which can be obtained using the device_get_display_numbers().\n
 *          The desired brightness level is specified in the brightness parameter.
 * @since_tizen 2.1
 * @remarks Ensure that the @a display_index has proper index value.
 * @param[in] display_index	The index of the display, it be greater than or equal to @c 0 and less than \n
 *                          the number of displays returned by device_get_display_numbers().\n
 *                          The index zero is always assigned to the main display
 * @param[in] brightness	The new brightness value to set \n
 *							The maximum value can be represented by device_get_max_brightness()
 * @return @c DEVICE_ERROR_NOT_SUPPORTED
 * @retval #DEVICE_ERROR_NOT_SUPPORTED Not supported device
 *
 * @code
 * #include <system/device.h>
 * ...
 * int number_of_display = 0, brightness = 100, display_index = 0;
 * int ret = 0;
 * ...
 * ret = device_get_display_numbers(&number_of_display);
 * if (ret == DEVICE_ERROR_NONE) {
 *     display_index = number_of_display - 1;
 *     device_set_brightness(display_index, brightness);
 *     ...
 * }
 * ...
 * @endcode
 * @see device_get_display_numbers()
 * @see device_get_max_brightness()
 * @see device_get_brightness()
 * @see device_set_brightness_from_settings()
 * @see device_set_brightness_to_settings()
 */
int device_set_brightness(int display_index, int brightness);

/**
 * @brief Gets the maximum brightness value that can be set based on the display index.
 * @details Retrieves the maximum brightness level of a specific display device.\n
 *          The display device is identified by its index, which can be obtained using the device_get_display_numbers() function.\n
 *          The maximum brightness level is returned in the max_brightness parameter.
 * @since_tizen 2.1
 * @remarks Ensure that the provided @a max_brightness pointer is valid and has enough memory allocated.
 * @param[in] display_index	The index of the display, it be greater than or equal to @c 0 and less than \n
 *                          the number of displays returned by device_get_display_numbers().\n
 *                          The index zero is always assigned to the main display
 * @param[out] max_brightness	The maximum brightness value of the display
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
 * int number_of_display = 0, brightness = 0, display_index = 0;
 * int ret = 0;
 * ...
 * ret = device_get_display_numbers(&number_of_display);
 * if (ret == DEVICE_ERROR_NONE) {
 *     display_index = number_of_display - 1;
 *     device_get_max_brightness(display_index, brightness);
 *     ...
 * }
 * ...
 * @endcode
 * @see device_get_display_numbers()
 * @see device_set_brightness()
 * @see device_get_brightness()
 * @see device_set_brightness_from_settings()
 * @see device_set_brightness_to_settings()
 */
int device_get_max_brightness(int display_index, int *max_brightness);

/**
 * @deprecated Deprecated since 3.0 
 * @brief Sets the display brightness value from registered in settings.
 * @details This function set display brightness to condition in the settings.\n
 * if auto brightness option is enabled in setting, display's brightness will be changed automatically.
 * @since_tizen 2.1
 * @param[in] display_index	The index of the display, it be greater than or equal to 0 and less than \n
 *                          the number of displays returned by device_get_display_numbers().\n
 *                          The index zero is always assigned to the main display
 * @return @c DEVICE_ERROR_NOT_SUPPORTED
 * @retval #DEVICE_ERROR_NOT_SUPPORTED Not supported device
 * @see device_get_display_numbers()
 * @see device_get_max_brightness()
 * @see device_set_brightness()
 * @see device_get_brightness()
 * @see device_set_brightness_to_settings()
 */
int device_set_brightness_from_settings(int display_index);

/**
 * @brief Sets the display brightness value to specific display and set to variable in settings.
 * @details This function set given brightness value to given index of display.\n
 *          And also brightness variable in settings will be changed to given brightness value too.
 * @since_tizen 2.1
 * @remarks Ensure that the @a display_index has proper index value.
 * @param[in] display_index	The index of the display, it be greater than or equal to 0 and less than \n
 *                          the number of displays returned by device_get_display_numbers().\n
 *                          The index zero is always assigned to the main display.
 * @param[in] brightness	The new brightness value to set \n
 *							The maximum value can be represented by device_get_max_brightness()
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
 * ret = device_get_display_numbers(&number_of_display);
 * if (ret == DEVICE_ERROR_NONE) {
 *     display_index = number_of_display - 1;
 *     device_set_brightness_to_settings(display_index, brightness);
 *     ...
 * }
 * ...
 * @endcode
 * @see device_get_display_numbers()
 * @see device_get_max_brightness()
 * @see device_set_brightness()
 * @see device_get_brightness()
 * @see device_set_brightness_from_settings()
 */
int device_set_brightness_to_settings(int display_index, int brightness);

/**
 * @brief Gets the brightness value of a LED that is located next to the camera.
 * @details Retrieves the current brightness level of the back camera flash. \n
 *          The brightness level is filled in the @a brightness parameter.
 * @since_tizen 2.3
 * @remarks This API is related to the following feature: %http://tizen.org/feature/camera.back.flash
 * @param[out] brightness brightness value of LED (@c 0 ~ @c MAX)
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
 * int brightness = 0;
 * int ret = 0;
 * ...
 * ret = device_flash_get_brightness(&brightness);
 * ...
 * @endcode
 */
int device_flash_get_brightness(int *brightness);

/**
 * @brief Sets the brightness value of a LED that is located next to the camera.
 * @details Sets the brightness level of the back camera flash. \n
 *          The desired brightness level is specified in the @a brightness parameter.
 * @since_tizen 2.3
 * @remarks Since 2.4, this API check camera flash status whether camera API preempted flash or not, so it could be failed if flash was preempted by camera API. In this case, API will return #DEVICE_ERROR_RESOURCE_BUSY error.
 *          This API is related to the following feature: %http://tizen.org/feature/camera.back.flash
 * @param[in] brightness brightness value of LED (@c 0 ~ @c MAX)
 * @return @c 0 on success, otherwise a negative error value.
 * @retval #DEVICE_ERROR_NONE Successful
 * @retval #DEVICE_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #DEVICE_ERROR_PERMISSION_DENIED Permission denied
 * @retval #DEVICE_ERROR_OPERATION_FAILED Operation failed
 * @retval #DEVICE_ERROR_NOT_SUPPORTED Not supported device
 * @retval #DEVICE_ERROR_RESOURCE_BUSY Device or resource busy
 *
 * @code
 * #include <system/device.h>
 * ...
 * int brightness = 0;
 * int ret = 0;
 * ...
 * ret = device_flash_set_brightness(brightness);
 * ...
 * @endcode
 */
int device_flash_set_brightness(int brightness);

/**
 * @brief Gets the max brightness value of a LED that is located next to the camera.
 * @details Retrieves the maximum brightness level of the back camera flash. \n
 * @since_tizen 2.3
 * @remarks This API is related to the following feature: %http://tizen.org/feature/camera.back.flash
 * @param[out] max_brightness max brightness value of LED
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
 * int max_brightness = 0;
 * int ret = 0;
 * ...
 * ret = device_flash_get_max_brightness(&max_brightness);
 * ...
 * @endcode
 */
int device_flash_get_max_brightness(int *max_brightness);

/**
 * @brief Gets the current device's total amount of physical RAM, in kilobytes
 * @details Retrieves the total memory size of the system in kilobytes (KB).
 *          The total memory size is returned in the @a total_mem parameter.
 * @since_tizen 2.1
 * @remarks Ensure that the provided @a total_mem pointer is valid and has enough memory allocated.
 * @param[out] total_mem total amount of physical RAM
 * @return @c 0 on success, otherwise a negative error value
 * @retval #DEVICE_ERROR_NONE Successful
 * @retval #DEVICE_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #DEVICE_ERROR_OPERATION_FAILED Operation failed
 *
 * @code
 * #include <system/device.h>
 * ...
 * unsigned int total_mem = 0;
 * int ret = 0;
 * ...
 * ret = device_memory_get_total(&total_mem);
 * ...
 * @endcode
 */
int device_memory_get_total(unsigned int *total_mem);

/**
 * @brief Gets the current device's available amount of physical RAM, in kilobytes
 * @details Retrieves the available memory size of the system in kilobytes (KB).
 *          The available memory size is returned in the @a avail_mem parameter.
 *          The available memory size is calculated as the sum of free memory, buffers, cached memory, and swap cache, minus shared memory.
 * @since_tizen 2.1
 * @remarks
 * Available amount is defined by following formula currently.
 * available mem = MemFree+Buffers+Cached+SwapCached-Shmem
 * @param[out] avail_mem available amount of physical RAM
 * @return @c 0 on success, otherwise a negative error value
 * @retval #DEVICE_ERROR_NONE Successful
 * @retval #DEVICE_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #DEVICE_ERROR_OPERATION_FAILED Operation failed
 *
 * @code
 * #include <system/device.h>
 * ...
 * unsigned int available_mem = 0;
 * int ret = 0;
 * ...
 * ret = device_memory_get_available(&available_mem);
 * ...
 * @endcode
 */
int device_memory_get_available(unsigned int *avail_mem);

/**
 * @}devic
 */

#ifdef __cplusplus
}
#endif

#endif  // __TIZEN_SYSTEM_DEVICE_H__
