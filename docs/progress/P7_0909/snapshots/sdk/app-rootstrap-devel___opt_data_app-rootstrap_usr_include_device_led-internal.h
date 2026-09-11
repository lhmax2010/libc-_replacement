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


#ifndef __TIZEN_SYSTEM_LED_INTERNAL_H__
#define __TIZEN_SYSTEM_LED_INTERNAL_H__


#include "device-error.h"

/**
 * @addtogroup CAPI_SYSTEM_DEVICE_LED_INTERNAL_MODULE
 * @{
 */

#ifdef __cplusplus
extern "C" {
#endif


/**
 * @brief Gets the the total number of LEDs present on the current device.
 * @details Retrieves the total number of LEDs present on the device. \n
 *          The number of LEDs is returned in the @a num_of_leds parameter.
 * @since_tizen 5.0
 * @privlevel public
 * @privilege %http://tizen.org/privilege/led
 * @remarks This API is related to the following feature: %http://tizen.org/feature/led
 * @param[out] num_of_leds A pointer to an integer where the number of LEDs will be stored.
 * @return @c 0 on success, otherwise a negative error value
 * @retval #DEVICE_ERROR_NONE Successful
 * @retval #DEVICE_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #DEVICE_ERROR_PERMISSION_DENIED Permission denied
 * @retval #DEVICE_ERROR_OPERATION_FAILED Operation failed
 * @retval #DEVICE_ERROR_NOT_SUPPORTED Not supported device
 *
 * @code
 * ...
 * #include <stdio.h>
 * #include <device/led-internal.h>
 * ...
 * int num_of_leds;
 * int ret = device_multi_led_get_number(&num_of_leds);
 * if (ret == DEVICE_ERROR_NONE) {
 *     ...
 * }
 * ...
 * @endcode
 */
int device_multi_led_get_number(int *num_of_leds);

/**
 * @brief Plays the current device's multiple LEDs at the same time.
 * @details This function controls the colors of multiple LEDs on the device simultaneously. \n
 *          The colors are specified in the color array parameter, where each element represents the color of one LED.
 *          The first byte of each element represents the opacity of the color, and the remaining three bytes represent the red, green, and blue components of the color.
 * @since_tizen 5.0
 * @privlevel public
 * @privilege %http://tizen.org/privilege/led
 * @remarks This API is related to the following feature: %http://tizen.org/feature/led
 * @remarks The array size must be the same as the number of LEDs, which is obtained by device_multi_led_get_number().
 * @param[in] color[] The Color value for each LED\n
 *                    The first byte means opaque, and the other 3 bytes mean RGB values
 * @return @c 0 on success, otherwise a negative error value
 * @retval #DEVICE_ERROR_NONE Successful
 * @retval #DEVICE_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #DEVICE_ERROR_PERMISSION_DENIED Permission denied
 * @retval #DEVICE_ERROR_OPERATION_FAILED Operation failed
 * @retval #DEVICE_ERROR_NOT_SUPPORTED Not supported device
 *
 * @code
 * #include <stdio.h>
 * #include <device/led-internal.h>
 * ...
 * int num_of_leds;
 * int ret = device_multi_led_get_number(&num_of_leds);
 * if (ret != DEVICE_ERROR_NONE) {
 *     return -1;
 * }
 * ...
 * unsigned int colors[num_of_leds];
 * for (int i = 0; i < num_of_leds; i++) {
 *     colors[i] = 0xFF0000; // Red color with full opacity
 * }
 * ret = device_multi_led_control(colors);
 * if (ret == DEVICE_ERROR_NONE) {
 *     ...
 * }
 * ...
 * @endcode
 * @see device_multi_led_get_number()
 */
int device_multi_led_control(unsigned int color[]);


#ifdef __cplusplus
}
#endif

/**
 * @}
 */

#endif  // __TIZEN_SYSTEM_LED_INTERNAL_H__
