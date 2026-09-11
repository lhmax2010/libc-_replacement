/*
 * Copyright (c) 2021 Samsung Electronics Co., Ltd All Rights Reserved
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

#ifndef __TIZEN_SYSTEM_SENSOR_INTERNAL_H__
#define __TIZEN_SYSTEM_SENSOR_INTERNAL_H__

#include <sensor.h>
#include <hal-sensor-types.h>

#define CONVERT_TO_ATTR(type, index) ((type) << 8 | 0x80 | (index))

#ifdef __cplusplus
extern "C"
{
#endif

/**
 * @addtogroup CAPI_SYSTEM_SENSOR_INTERNAL_MODULE
 * @{
 */

/**
 * @brief Enumeration for the sensor types for the internal sensor.
 * @details This enumeration lists the different types of sensors.
 * @since_tizen 6.5
 *
 * @remarks These values are used internally to identify and manage sensors.
 */
typedef enum {
	SENSOR_LIDAR = 1000, /**< Sensor type LIDAR */
} sensor_type_internel_e;

/**
 * @brief Enumeration for the sensor handle attribute value.
 * @details This enumeration defines various attributes associated with sensor
 *          handles.
 * @since_tizen 7.0
 *
 * @remarks Each attribute corresponds to specific properties of different
 *          sensors.
 */
typedef enum
{
	SENSOR_HANDLE_ATTR_PROXIMITY_SENSITIVITY_LEVEL = CONVERT_TYPE_ATTR(SENSOR_PROXIMITY, 0x1), /**< Attribute for the sensitivity level of the proximity sensor */
	SENSOR_HANDLE_ATTR_LIDAR_POWER_STATE           = CONVERT_TYPE_ATTR(SENSOR_LIDAR, 0x1)      /**< Attribute for the power state of the LIDAR sensor */
} sensor_handle_attribute_e;

/**
 * @brief Enumeration for proximity sensitivity attribute value.
 * @details This enumeration defines the different levels of sensitivity for the
 *          proximity sensor.
 * @since_tizen 7.0
 *
 * @remarks With this enumeration, 3 levels are available: weak(1), medium(2)
 *          and strong(3).
 */
typedef enum
{
	SENSOR_PROXIMITY_SENSITIVITY_LEVEL_WEAK = 1,   /**< Sensitivity level of the proximity sensor, weak */
	SENSOR_PROXIMITY_SENSITIVITY_LEVEL_MEDIUM = 2, /**< Sensitivity level of the proximity sensor, medium */
	SENSOR_PROXIMITY_SENSITIVITY_LEVEL_STRONG = 3  /**< Sensitivity level of the proximity sensor, strong */
} sensor_proximity_sensitivity_level_e;

/**
 * @brief Enumeration for lidar power state attribute value.
 * @details This enumeration defines the possible power states for a LIDAR
 *          sensor.
 * @since_tizen 7.0
 *
 * @remarks With this enumeration, 2 levels are available: off(0) and on(1).
 */
typedef enum
{
	SENSOR_LIDAR_POWER_STATE_OFF = 0, /**< Power off state of LIDAR sensor */
	SENSOR_LIDAR_POWER_STATE_ON = 1   /**< Power on state of LIDAR sensor */
} sensor_lidar_power_state_e;

/**
 * @brief Sets the specified sensor attribute to a connected sensor.
 * @details Set the specified attributes of a connected sensor to the given value.
 * @since_tizen 6.5
 *
 * @param[in] type a sensor type of represensting a connected sensor
 * @param[in] attr an attribute to change
 * @param[in] value an attribute value
 * @return 0 on success, otherwise a negative error value
 * @retval 0 Successful
 * @retval -EINVAL Invalid parameter
 * @retval -EPERM Operation not permitted
 *
 * @par Example
 * @code
 * sensor_util_set_attribute_int(SENSOR_ACCELEROMETER, SENSOR_ATTRIBUTE_FLUSH, 1);
 * @endcode
 */
int sensor_util_set_attribute_int(sensor_type_e type, sensor_attribute_e attr, int value);

/**
 * @brief Gets the specified sensor attribute from a connected sensor.
 * @details Retrieve the specified sensor attribute value of a connected sensor.
 * @since_tizen 6.5
 *
 * @param[in] type a sensor type of represensting a connected sensor
 * @param[in] attr an attribute to get value
 * @param[out] value an attribute value
 * @return 0 on success, otherwise a negative error value
 * @retval 0 Successful
 * @retval -EINVAL Invalid parameter
 * @retval -EPERM Operation not permitted
 *
 * @par Example
 * @code
 * int orientation;
 * sensor_util_get_attribute_int(SENSOR_ACCELEROMETER, SENSOR_ATTRIBUTE_AXIS_ORIENTATION, orientation);
 * @endcode
 */
int sensor_util_get_attribute_int(sensor_type_e type, sensor_attribute_e attr, int *value);

/**
 * @brief   Gets an attribute to control the behavior of a sensor listener.
 * @details It is used to get the current setting of a specific attribute for a
 *          given sensor listener.
 * @since_tizen 7.0
 *
 * @remarks Applications can change the behavior of a sensor listener, for example,
 *          what is the reference coordinate of the sensor values,
 *          and when the system is allowed to turn off the sensor implicitly to reduce the power consumption.
 *          See #sensor_attribute_e for more details about the available control parameters.
 *
 * @param[in]   listener     A listener handle
 * @param[in]   attribute    An attribute to change.
 *                           It should be in the range each sensor_attribute_e,
 *                           or should be fixed enum value such as sensor_axis_e or sensor_pause_e.
 * @param[out]  value        An attribute value
 *
 * @return  #SENSOR_ERROR_NONE on success, otherwise a negative error value
 * @retval  #SENSOR_ERROR_NONE                 Successful
 * @retval  #SENSOR_ERROR_INVALID_PARAMETER    Invalid parameter
 * @retval  #SENSOR_ERROR_OPERATION_FAILED     Operation failed
 *
 * @par Example
 * @code
 * int sensitivity;
 * sensor_listener_get_attribute_int(listener, SENSOR_ATTRIBUTE_AXIS_ORIENTATION, &sensitivity);
 * @endcode
 */
int sensor_listener_get_attribute_int(sensor_listener_h listener, sensor_attribute_e attribute, int *value);

/**
 * @brief Sets the sensor handle attribute to a connected sensor.
 * @details Set the specified attribute of a connected sensor to the given value.
 *          Note that each attribute has a specific range of values or uses
 *          fixed enumerations.
 * @since_tizen 7.0
 *
 * @remarks Each attribute has a range of values or use fixed enum.
 *          For example, proximity sensitivity level attribute can be set to
 *          only sensor_proximity_sensitivity_level_e.
 *
 * @param[in] sensor a sensor handle of represensting a connected sensor
 * @param[in] attr an attribute to change
 * @param[in] value an attribute value
 *
 * @return  #SENSOR_ERROR_NONE on success, otherwise a negative error value
 * @retval  #SENSOR_ERROR_NONE                 Successful
 * @retval  #SENSOR_ERROR_INVALID_PARAMETER    Invalid parameter
 * @retval  #SENSOR_ERROR_OPERATION_FAILED     Operation failed
 *
 * @par Example
 * @code
 * sensor_set_attribute_int(sensor, SENSOR_HANDLE_ATTR_PROXIMITY_SENSITIVITY_LEVEL, 1);
 * @endcode
 */
int sensor_set_attribute_int(sensor_h sensor, sensor_handle_attribute_e attribute, int value);

/**
 * @brief Gets the sensor handle attribute from a connected sensor.
 * @details Retrieve the specified sensor handle attribute value of a connected sensor.
 * @since_tizen 7.0
 *
 * @remarks Each attribute has a range of values or use fixed enum.
 *          For example, the type of return value of proximity sensitivity level
 *          attribute can be only sensor_proximity_sensitivity_level_e.
 *
 * @param[in] sensor a sensor handle of represensting a connected sensor
 * @param[in] attr an attribute to change
 * @param[out] value an attribute value
 *
 * @return  #SENSOR_ERROR_NONE on success, otherwise a negative error value
 * @retval  #SENSOR_ERROR_NONE                 Successful
 * @retval  #SENSOR_ERROR_INVALID_PARAMETER    Invalid parameter
 * @retval  #SENSOR_ERROR_OPERATION_FAILED     Operation failed
 *
 * @par Example
 * @code
 * int sensitivity_level;
 * sensor_get_attribute_int(sensor, SENSOR_HANDLE_ATTR_PROXIMITY_SENSITIVITY_LEVEL, &sensitivity_level);
 * @endcode
 */
int sensor_get_attribute_int(sensor_h sensor, sensor_handle_attribute_e attribute, int *value);

/**
 * @}
 */

#ifdef __cplusplus
}
#endif

#endif /* __TIZEN_SYSTEM_SENSOR_INTERNAL_H__ */
