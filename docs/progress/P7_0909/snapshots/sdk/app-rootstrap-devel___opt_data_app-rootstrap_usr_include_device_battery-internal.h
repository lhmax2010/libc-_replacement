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

/**
 * @addtogroup CAPI_SYSTEM_DEVICE_BATTERY_INTERNAL_MODULE
 * @{
 */

#ifndef __TIZEN_SYSTEM_BATTERY_INTERNAL_H__
#define __TIZEN_SYSTEM_BATTERY_INTERNAL_H__


#include <stdbool.h>
#include "device-error.h"


#ifdef __cplusplus
extern "C" {
#endif

#define BATTERY_INFO_MAX 32

/**
 * @brief Structure containing detailed battery information.
 * @details This structure holds various parameters describing the current state and condition
 *          of the device's battery.
 */
struct device_battery_info {
	char status[BATTERY_INFO_MAX]; /**< The current state of the battery (e.g., "Charging", "Discharging", "Full", "Not charging") */
	char health[BATTERY_INFO_MAX]; /**< The overall condition of the battery (e.g., "Good", "Overheat", "Dead", "Cold") */
	char power_source[BATTERY_INFO_MAX]; /**< The source of power being supplied (e.g., "USB", "AC", "Wireless") */
	int online; /**< This indicates whether the device is currently
				receiving power from an external source. (1: online, 0: offline) */
	int present; /**<  This indicates whether there is a battery inside the system physically,
				with 1 indicating it is connected and 0 otherwise. */
	int capacity; /**< The remaining capacity of the battery (percentage, 0-100) */
	int current_now; /**< The amount of current being consumed by the device at the moment (µA) */
	int current_average; /**< The average amount of current consumed by the device over time (µA).
	                       * Set to INT_MAX if battery hardware does not provide current average. */
	int voltage_now; /**< The current voltage level of the battery (µV) */
	int voltage_average; /**< The average voltage level of the battery over time (µV).
	                      * Set to INT_MAX if battery hardware does not provide voltage average. */
	int temperature; /**< The current temperature of the battery (decidegrees Celsius) */
};

/**
 * @brief Gets the current device's battery status from HAL battery module.
 * @details Retrieves detailed information about the battery status, including capacity, voltage averages and etc.
 * @since_tizen 6.0
 * @remarks Ensure that the provided @a info pointer is valid and has enough memory allocated.
 * @param[out] info battery status information.\n
 *                  The current_average is INT_MAX if battery hw does not provide current average.\n
 *                  The voltage_average is INT_MAX if battery hw does not provide voltage average.
 * @return @c 0 on success, otherwise a negative error value
 * @retval #DEVICE_ERROR_NONE Successful
 * @retval #DEVICE_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #DEVICE_ERROR_OPERATION_FAILED Operation failed
 * @retval #DEVICE_ERROR_NOT_SUPPORTED Not supported device
 *
 * @code
 * #include <device/battery-internal.h>
 * ...
 * struct device_battery_info info;
 * int ret = 0;
 * ...
 * ret = device_battery_get_info(&info);
 * ...
 * @endcode
 * @see device_battery_info
 */
int device_battery_get_info_direct(struct device_battery_info *info);

#ifdef __cplusplus
}
#endif

/**
 * @}
 */

#endif  // __TIZEN_SYSTEM_BATTERY_INTERNAL_H__
