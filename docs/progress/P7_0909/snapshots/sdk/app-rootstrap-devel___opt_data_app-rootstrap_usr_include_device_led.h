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


#ifndef __TIZEN_SYSTEM_LED_H__
#define __TIZEN_SYSTEM_LED_H__


#include "device-error.h"


#ifdef __cplusplus
extern "C" {
#endif


/**
 * @addtogroup CAPI_SYSTEM_DEVICE_LED_MODULE
 * @{
 */


/**
 * @brief Gets the max brightness value of a LED that is located next to the camera.
 * @details Retrieves the maximum brightness level of the back camera flash.
 * @since_tizen 2.3
 * @privlevel public
 * @privilege %http://tizen.org/privilege/led
 * @remarks This API is related to the following feature: %http://tizen.org/feature/camera.back.flash
 * @param[out] max_brightness The max brightness value of the LED
 * @return @c 0 on success, otherwise a negative error value
 * @retval #DEVICE_ERROR_NONE Successful
 * @retval #DEVICE_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #DEVICE_ERROR_PERMISSION_DENIED Permission denied
 * @retval #DEVICE_ERROR_OPERATION_FAILED Operation failed
 * @retval #DEVICE_ERROR_NOT_SUPPORTED Not supported device
 *
 * @code
 * #include <device/led.h>
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
 * @brief Gets the brightness value of a LED that is located next to the camera.
 * @details Retrieves the current brightness level of the back camera flash. \n
 *          The brightness level is filled in the @a brightness parameter.
 * @since_tizen 2.3
 * @privlevel public
 * @privilege %http://tizen.org/privilege/led
 * @remarks This API is related to the following feature: %http://tizen.org/feature/camera.back.flash
 * @param[out] brightness The brightness value of LED (@c 0 ~ MAX)
 * @return @c 0 on success, otherwise a negative error value
 * @retval #DEVICE_ERROR_NONE Successful
 * @retval #DEVICE_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #DEVICE_ERROR_PERMISSION_DENIED Permission denied
 * @retval #DEVICE_ERROR_OPERATION_FAILED Operation failed
 * @retval #DEVICE_ERROR_NOT_SUPPORTED Not supported device
 *
 * @code
 * #include <device/led.h>
 * ...
 * int brightness = 0;
 * int ret = 0;
 * ...
 * ret = device_flash_get_brightness(&brightness);
 * ...
 * @endcode
 * @see device_flash_set_brightness()
 */
int device_flash_get_brightness(int *brightness);


/**
 * @brief Sets the brightness value of a LED that is located next to the camera.
 * @details Sets the brightness level of the back camera flash. \n
 *          The desired brightness level is specified in the @a brightness parameter.
 * @since_tizen 2.3
 * @privlevel public
 * @privilege %http://tizen.org/privilege/led
 * @remarks Since 2.4, this API check camera flash status whether camera API preempted flash or not, so it could be failed if flash was preempted by camera API. In this case, API will return #DEVICE_ERROR_RESOURCE_BUSY error.
 *          This API is related to the following feature: %http://tizen.org/feature/camera.back.flash
 * @param[in] brightness The brightness value of LED (@c 0 ~ MAX)
 * @return @c 0 on success, otherwise a negative error value
 * @retval #DEVICE_ERROR_NONE Successful
 * @retval #DEVICE_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #DEVICE_ERROR_PERMISSION_DENIED Permission denied
 * @retval #DEVICE_ERROR_OPERATION_FAILED Operation failed
 * @retval #DEVICE_ERROR_NOT_SUPPORTED Not supported device
 * @retval #DEVICE_ERROR_RESOURCE_BUSY Device or resource busy
 *
 * @code
 * #include <device/led.h>
 * ...
 * int brightness = 0;
 * int ret = 0;
 * ...
 * ret = device_flash_set_brightness(brightness);
 * ...
 * @endcode
 * @see device_flash_get_max_brightness()
 * @see device_flash_get_brightness()
 */
int device_flash_set_brightness(int brightness);


/**
 * @brief Enumeration for custom LED flags.
 * @since_tizen 2.3
 */
typedef enum {
    LED_CUSTOM_DUTY_ON = 1 << 0, /**< blink LED */
    LED_CUSTOM_DEFAULT = (LED_CUSTOM_DUTY_ON), /**< Default flag */
} led_custom_flags;


/**
 * @brief Plays the custom effect of the service LED that is located to the front of a device.
 * @since_tizen 2.3
 * @privlevel public
 * @privilege %http://tizen.org/privilege/led
 * @remarks This API is related to the following feature: %http://tizen.org/feature/led
 * @param[in] on Turn on time in milliseconds
 * @param[in] off Turn off time in milliseconds
 * @param[in] color The Color value \n
 *                  The first byte means opaque and the other 3 bytes are RGB values
 * @param[in] flags The combination of enum #led_custom_flags
 * @return @c 0 on success, otherwise a negative error value
 * @retval #DEVICE_ERROR_NONE Successful
 * @retval #DEVICE_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #DEVICE_ERROR_PERMISSION_DENIED Permission denied
 * @retval #DEVICE_ERROR_OPERATION_FAILED Operation failed
 * @retval #DEVICE_ERROR_NOT_SUPPORTED Not supported device
 *
 * @code
 * #include <stdio.h>
 * #include <device/led.h>
 * ...
 * int on_time = 100; // On time in milliseconds
 * int off_time = 200; // Off time in milliseconds
 * unsigned int color = 0xFF0000; // Red color with full opacity
 * unsigned int flags = LED_CUSTOM_DEFAULT; // Default flag
 * int ret = device_led_play_custom(on_time, off_time, color, flags);
 * if (ret != DEVICE_ERROR_NONE) {
 *     ...
 *     return -1;
 * }
 * printf("Custom effect played successfully.\n");
 * ...
 * @endcode
 * @see device_led_stop_custom()
 */
int device_led_play_custom(int on, int off, unsigned int color, unsigned int flags);


/**
 * @brief Stops the custom effect of the service LED that is located to the front of a device.
 * @details Stops the custom effect of the service LED located at the front of the device.
 *          The custom effect was started by calling device_led_play_custom().
 * @since_tizen 2.3
 * @privlevel public
 * @privilege %http://tizen.org/privilege/led
 * @remarks This API is related to the following feature: %http://tizen.org/feature/led \n
 *          Requested custom effects by device_led_play_custom() are removed.
 * @return @c 0 on success, otherwise a negative error value
 * @retval #DEVICE_ERROR_NONE Successful
 * @retval #DEVICE_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #DEVICE_ERROR_PERMISSION_DENIED Permission denied
 * @retval #DEVICE_ERROR_OPERATION_FAILED Operation failed
 * @retval #DEVICE_ERROR_NOT_SUPPORTED Not supported device
 *
 * @code
 * #include <device/led.h>
 * ...
 * int ret = device_led_play_custom(100, 200, 0xFF0000, LED_CUSTOM_DEFAULT);
 * if (ret != DEVICE_ERROR_NONE) {
 *     return -1;
 * }
 * sleep(5); // Wait for 5 seconds
 * ret = device_led_stop_custom();
 * ...
 * @endcode
 * @see device_led_play_custom()
 */
int device_led_stop_custom(void);


/**
 * @}
 */


#ifdef __cplusplus
}
#endif


#endif  // __TIZEN_SYSTEM_LED_H__
