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


#ifndef __TIZEN_SYSTEM_BATTERY_H__
#define __TIZEN_SYSTEM_BATTERY_H__


#include <stdbool.h>
#include "device-error.h"


#ifdef __cplusplus
extern "C" {
#endif


/**
 * @addtogroup CAPI_SYSTEM_DEVICE_BATTERY_MODULE
 * @{
 */


/**
 * @brief Enumeration for the battery level status.
 * @since_tizen 2.3
 */
typedef enum {
    DEVICE_BATTERY_LEVEL_EMPTY = 0, /**< The battery goes empty. Prepare for the safe termination of the application, because the device starts a shutdown process soon after entering this level. */
    DEVICE_BATTERY_LEVEL_CRITICAL, /**< The battery charge is at a critical state. You may have to stop using multimedia features, because they are not guaranteed to work correctly at this battery status. */
    DEVICE_BATTERY_LEVEL_LOW, /**< The battery has little charge left. */
    DEVICE_BATTERY_LEVEL_HIGH, /**< The battery status is not to be careful. */
    DEVICE_BATTERY_LEVEL_FULL, /**< The battery status is fully charged. It means no more charge. */
} device_battery_level_e;


/**
 * @brief Enumeration for battery health information.
 * @since_tizen 3.0
 */
typedef enum {
	DEVICE_BATTERY_HEALTH_GOOD, /**< The battery health is good */
	DEVICE_BATTERY_HEALTH_COLD, /**< The temperature of the battery is cold */
	DEVICE_BATTERY_HEALTH_DEAD, /**< The battery is dead */
	DEVICE_BATTERY_HEALTH_OVER_HEAT, /**< The temperature of the battery is high */
	DEVICE_BATTERY_HEALTH_OVER_VOLTAGE, /**< The battery is in over voltage state */
} device_battery_health_e;


/**
 * @brief Enumeration for power source information.
 * @since_tizen 3.0
 */
typedef enum {
	DEVICE_BATTERY_POWER_SOURCE_NONE, /**< There is no power source */
	DEVICE_BATTERY_POWER_SOURCE_AC, /**< AC power cable is connected */
	DEVICE_BATTERY_POWER_SOURCE_USB, /**< USB power cable is connected */
	DEVICE_BATTERY_POWER_SOURCE_WIRELESS, /**< Power is provided by a wireless source */
} device_battery_power_source_e;


/**
 * @brief Enumeration for battery property information.
 * @since_tizen 3.0
 * @remarks If the fuel gauge hardware does not provide average current or average voltage,
            CURRENT_AVERAGE/VOLTAGE_AVERAGE can be substituted by CURRENT_NOW/VOLTAGE_NOW, respectively.
 */
typedef enum {
	DEVICE_BATTERY_PROPERTY_CAPACITY, /**< The battery capacity (0 ~ 100 %) */
	DEVICE_BATTERY_PROPERTY_CURRENT_NOW, /**< The battery current (uA) */
	DEVICE_BATTERY_PROPERTY_CURRENT_AVERAGE, /**< The average battery current (uA) */
	DEVICE_BATTERY_PROPERTY_VOLTAGE_NOW, /**< The battery voltage (uV) (Since 5.0) */
	DEVICE_BATTERY_PROPERTY_VOLTAGE_AVERAGE, /**< The average battery voltage (uV) (Since 5.0) */
	DEVICE_BATTERY_PROPERTY_TEMPERATURE, /**< The battery temperature (`C) (Since 5.0) */
} device_battery_property_e;


/**
 * @brief Enumeration for battery status information.
 * @since_tizen 3.0
 */
typedef enum {
	DEVICE_BATTERY_STATUS_CHARGING, /**< Battery is charging */
	DEVICE_BATTERY_STATUS_DISCHARGING, /**< Battery is discharging */
	DEVICE_BATTERY_STATUS_FULL, /**< Battery is fully charged */
	DEVICE_BATTERY_STATUS_NOT_CHARGING, /**< Battery is not charging */
} device_battery_status_e;


/**
 * @brief Gets the current device's battery charge percentage as an integer value.
 * @details It returns an integer value from @c 0 to @c 100 that indicates remaining battery charge
 *          as a percentage of the maximum level.
 * @since_tizen 2.3
 * @remarks Ensure that the provided @a percent pointer is valid and has enough memory allocated.
 * @param[out] percent The remaining battery charge percentage (@c 0 ~ @c 100)
 * @return @c 0 on success, otherwise a negative error value
 * @retval #DEVICE_ERROR_NONE Successful
 * @retval #DEVICE_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #DEVICE_ERROR_PERMISSION_DENIED Permission denied
 * @retval #DEVICE_ERROR_OPERATION_FAILED Operation failed
 * @retval #DEVICE_ERROR_NOT_SUPPORTED Not supported device
 *
 * @code
 * #include <device/battery.h>
 * ...
 * int percent = 0;
 * int ret = 0;
 * ...
 * ret = device_battery_get_info(&percent);
 * ...
 * @endcode
 */
int device_battery_get_percent(int *percent);


/**
 * @brief Gets the current device's charging state which the battery is charging.
 * @details Checks whether the battery is currently being charged or not.
 * @since_tizen 2.3
 * @remarks Ensure that the provided @a charging pointer is valid and has enough memory allocated.
 * @param[out] charging The battery charging state
 * @return @c 0 on success, otherwise a negative error value
 * @retval #DEVICE_ERROR_NONE Successful
 * @retval #DEVICE_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #DEVICE_ERROR_OPERATION_FAILED Operation failed
 * @retval #DEVICE_ERROR_NOT_SUPPORTED Not supported device
 *
 * @code
 * #include <device/battery.h>
 * ...
 * bool is_charging = false;
 * int ret = 0;
 * ...
 * ret = device_battery_is_charging(&is_charging);
 * ...
 * @endcode
 * @see device_add_callback
 * @see device_remove_callback
 * @see #DEVICE_CALLBACK_BATTERY_CHARGING
 */
int device_battery_is_charging(bool *charging);


/**
 * @brief Gets the current device's battery level status as a @a device_battery_level_e.
 * @details Retrieves the current battery level status based on remaining battery capacity.
 * @since_tizen 2.3
 * @remarks Ensure that the provided @a status pointer is valid and has enough memory allocated.
 * @param[out] status The battery level status
 * @return @c 0 on success, otherwise a negative error value
 * @retval #DEVICE_ERROR_NONE Successful
 * @retval #DEVICE_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #DEVICE_ERROR_OPERATION_FAILED Operation failed
 * @retval #DEVICE_ERROR_NOT_SUPPORTED Not supported device
 *
 * @code
 * #include <device/battery.h>
 * ...
 * device_battery_level_e batt_lev_status = DEVICE_BATTERY_LEVEL_EMPTY;
 * int ret = 0;
 * ...
 * ret = device_battery_get_level_status(&batt_lev_status);
 * ...
 * @endcode
 * @see device_battery_level_e
 * @see device_add_callback
 * @see device_remove_callback
 * @see #DEVICE_CALLBACK_BATTERY_LEVEL
 */
int device_battery_get_level_status(device_battery_level_e *status);


/**
 * @brief Gets the current device's battery health information as a @a device_battery_health_e.
 * @details Retrieves the current battery health status information (e.g., good, overheat, dead, etc).
 * @since_tizen 3.0
 * @remarks Ensure that the provided @a health pointer is valid and has enough memory allocated.
 * @param[out] health The battery health information
 * @return @c 0 on success, otherwise a negative error value
 * @retval #DEVICE_ERROR_NONE Successful
 * @retval #DEVICE_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #DEVICE_ERROR_OPERATION_FAILED Operation failed
 * @retval #DEVICE_ERROR_NOT_SUPPORTED Not supported device
 *
 * @code
 * #include <device/battery.h>
 * ...
 * device_battery_health_e batt_health = DEVICE_BATTERY_HEALTH_GOOD;
 * int ret = 0;
 * ...
 * ret = device_battery_get_health(&batt_health);
 * ...
 * @endcode
 * @see device_battery_health_e
 */
int device_battery_get_health(device_battery_health_e *health);


/**
 * @brief Gets the current device's power source information from the battery.
 * @details Retrieves the current battery power source information (e.g., ac, usb, etc).
 * @since_tizen 3.0
 * @remarks Ensure that the provided @a source pointer is valid and has enough memory allocated.
 * @param[out] source The battery power source information
 * @return @c 0 on success, otherwise a negative error value
 * @retval #DEVICE_ERROR_NONE Successful
 * @retval #DEVICE_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #DEVICE_ERROR_OPERATION_FAILED Operation failed
 * @retval #DEVICE_ERROR_NOT_SUPPORTED Not supported device
 *
 * @code
 * #include <device/battery.h>
 * ...
 * device_battery_power_source_e batt_power_src = DEVICE_BATTERY_POWER_SOURCE_NONE;
 * int ret = 0;
 * ...
 * ret = device_battery_get_power_source(&batt_power_src);
 * ...
 * @endcode
 * @see device_battery_power_source_e
 */
int device_battery_get_power_source(device_battery_power_source_e *source);


/**
 * @brief Gets the current device's specified battery property as an integer value.
 * @details Retrieves the current battery property information (e.g., capacity, current_average, temperature, etc).
 * @since_tizen 3.0
 * @remarks Ensure that the provided @a value pointer is valid and has enough memory allocated.
 * @param[in] property The property type
 * @param[out] value The battery information for the property given
 * @return @c 0 on success, otherwise a negative error value
 * @retval #DEVICE_ERROR_NONE Successful
 * @retval #DEVICE_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #DEVICE_ERROR_OPERATION_FAILED Operation failed
 * @retval #DEVICE_ERROR_NOT_SUPPORTED Not supported device
 *
 * @code
 * #include <device/battery.h>
 * ...
 * int value = 0;
 * int ret = 0;
 * ...
 * ret = device_battery_get_property(DEVICE_BATTERY_PROPERTY_CAPACITY, &value);
 * ...
 * @endcode
 * @see device_battery_property_e
 */
int device_battery_get_property(device_battery_property_e property, int *value);


/**
 * @brief Gets the current device's battery status according to the degree of charge.
 * @details Retrieves the current battery status information (e.g., charging, discharging, full, etc).
 * @since_tizen 3.0
 * @remarks Ensure that the provided @a status pointer is valid and has enough memory allocated.
 * @param[out] status The battery status information
 * @return @c 0 on success, otherwise a negative error value
 * @retval #DEVICE_ERROR_NONE Successful
 * @retval #DEVICE_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #DEVICE_ERROR_OPERATION_FAILED Operation failed
 * @retval #DEVICE_ERROR_NOT_SUPPORTED Not supported device
 *
 * @code
 * #include <device/battery.h>
 * ...
 * device_battery_status_e batt_status = DEVICE_BATTERY_STATUS_CHARGING;
 * int ret = 0;
 * ...
 * ret = device_battery_get_property(&batt_status);
 * ...
 * @endcode
 * @see device_battery_status_e
 */
int device_battery_get_status(device_battery_status_e *status);


/**
 * @}
 */


#ifdef __cplusplus
}
#endif


#endif  // __TIZEN_SYSTEM_BATTERY_H__
