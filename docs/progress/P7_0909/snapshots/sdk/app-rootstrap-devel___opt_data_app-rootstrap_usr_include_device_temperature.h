/*
 * Copyright (c) 2019 Samsung Electronics Co., Ltd All Rights Reserved
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


#ifndef __TIZEN_SYSTEM_TEMPERATURE_H__
#define __TIZEN_SYSTEM_TEMPERATURE_H__


#include <stdbool.h>
#include "device-error.h"


#ifdef __cplusplus
extern "C" {
#endif


/**
 * @addtogroup CAPI_SYSTEM_DEVICE_THERMAL_MODULE
 * @{
 */


/**
 * @brief Enumeration for the device temperature.
 * @since_tizen 5.5
 */
typedef enum
{
    DEVICE_THERMAL_AP,      /**< Temperature for Application Processor */
    DEVICE_THERMAL_CP,      /**< Temperature for Communication Processor */
    DEVICE_THERMAL_BATTERY, /**< Temperature for Battery */
} device_thermal_e;


/**
 * @brief Gets the temperature value based on the specified device type.
 * @details Retrieves the current temperature value of a specified device.
 * @since_tizen 5.5
 * @remarks Ensure that the provided @a temp pointer is valid and has enough memory allocated.
 * @param[in] type The index of the device
 *                 DEVICE_THERMAL_AP \n
 *                 DEVICE_THERMAL_CP \n
 *                 DEVICE_THERMAL_BATTERY
 * @param[out] temp The temperature value in degrees Celsius
 * @return @c 0 on success, otherwise a negative error value
 * @retval #DEVICE_ERROR_NONE Successful
 * @retval #DEVICE_ERROR_NOT_SUPPORTED Not supported device
 * @retval #DEVICE_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #DEVICE_ERROR_OPERATION_FAILED Operation failed
 *
 * @code
 * #include <device/temperature.h>
 * ...
 * int temperature = 0;
 * int ret = 0;
 * ...
 * ret = device_thermal_get_temperature(DEVICE_THERMAL_AP, &temperature);
 * if (ret == DEVICE_ERROR_NONE) {
 *     ...
 * }
 * ...
 * @endcode
 * @see device_thermal_e
 */
int device_thermal_get_temperature(device_thermal_e type, int *temp);


/**
 * @}
 */


#ifdef __cplusplus
}
#endif


#endif  /* __TIZEN_SYSTEM_TEMPERATURE_H__ */
