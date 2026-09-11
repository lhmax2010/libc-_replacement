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

#ifndef __SENSOR_H__
#define __SENSOR_H__

#include <tizen.h>
#include <time.h>

#ifdef __cplusplus
extern "C"
{
#endif

/**
 * @addtogroup CAPI_SYSTEM_SENSOR_MODULE
 * @{
 */

/**
 * @brief   Sensor handle.
 * @details The handle for controlling a specific sensor can be retrieved using sensor_get_default_sensor().@n
 *          The function returns the handle of the default sensor of a given type, and usually,
 *          a device has one sensor for one type.
 *          However, if the device supports multiple sensors of the same type,
 *          sensor_get_sensor_list() function can be used to get the list of all the sensors of the type.
 * @since_tizen 2.3
 */
typedef void* sensor_h;


/**
 * @brief   Enumeration for errors.
 * @since_tizen 2.3
 */
typedef enum {
	SENSOR_ERROR_NONE                  = TIZEN_ERROR_NONE,                 /**< Successful */
	SENSOR_ERROR_IO_ERROR              = TIZEN_ERROR_IO_ERROR,             /**< I/O error */
	SENSOR_ERROR_INVALID_PARAMETER     = TIZEN_ERROR_INVALID_PARAMETER,    /**< Invalid parameter */
	SENSOR_ERROR_NOT_SUPPORTED         = TIZEN_ERROR_NOT_SUPPORTED,        /**< Not supported */
	SENSOR_ERROR_PERMISSION_DENIED     = TIZEN_ERROR_PERMISSION_DENIED,    /**< Permission denied */
	SENSOR_ERROR_OUT_OF_MEMORY         = TIZEN_ERROR_OUT_OF_MEMORY,        /**< Out of memory */
	SENSOR_ERROR_NO_DATA               = TIZEN_ERROR_NO_DATA,              /**< No data available (Since 3.0) */
	SENSOR_ERROR_NOT_NEED_CALIBRATION  = TIZEN_ERROR_SENSOR | 0x03,        /**< Sensor doesn't need calibration */
	SENSOR_ERROR_OPERATION_FAILED      = TIZEN_ERROR_SENSOR | 0x06,        /**< Operation failed */
	SENSOR_ERROR_NOT_AVAILABLE         = TIZEN_ERROR_SENSOR | 0x07,        /**< The sensor is supported, but currently not available (Since 3.0) */
} sensor_error_e;


/**
 * @brief   Enumeration for proximity sensor events.
 * @details In its #sensor_event_s, #SENSOR_PROXIMITY reports the existence of
 *          nearby objects in front of the sensor as one of the followings.
 * @since_tizen 2.3
 */
typedef enum {
	SENSOR_PROXIMITY_NEAR = 0,    /**< An object is placed near the proximity sensor */
	SENSOR_PROXIMITY_FAR = 5,     /**< No object is placed near the proximity sensor */
} sensor_proximity_e;


/**
 * @brief   Enumeration for pedestrian state.
 * @details In its #sensor_event_s, #SENSOR_HUMAN_PEDOMETER reports the user's
 *          pedestrian state as one of the followings.
 * @since_tizen 3.0
 */
typedef enum {
	SENSOR_PEDOMETER_STATE_UNKNOWN = -1, /**< Uncertain */
	SENSOR_PEDOMETER_STATE_STOP,         /**< The user is not moving */
	SENSOR_PEDOMETER_STATE_WALK,         /**< The user is walking */
	SENSOR_PEDOMETER_STATE_RUN,          /**< The user is running */
} sensor_pedometer_state_e;


/**
 * @brief   Enumeration for sleep state.
 * @details In its #sensor_event_s, #SENSOR_HUMAN_SLEEP_MONITOR reports the user's
 *          sleep state as one of the followings.
 * @since_tizen 3.0
 */
typedef enum {
	SENSOR_SLEEP_STATE_UNKNOWN = -1, /**< Uncertain */
	SENSOR_SLEEP_STATE_WAKE,         /**< The user is awake */
	SENSOR_SLEEP_STATE_SLEEP,        /**< The user is asleep */
} sensor_sleep_state_e;

/**
 * @brief   Enumeration for heart-rate monitor batch sensor state.
 * @details In its #sensor_event_s, #SENSOR_HRM_BATCH reports the user's
 *          heart-rate monitor state as one of the followings.
 * @since_tizen 5.5
 */
typedef enum {
	SENSOR_HRM_BATCH_STATE_NODATA_FLUSH = -99,           /**< Flush but there was no batched data */
	SENSOR_HRM_BATCH_STATE_VERYLOW_RELIABILITY = -10,    /**< Very low measurement reliability */
	SENSOR_HRM_BATCH_STATE_LOW_RELIABILITY = -8,         /**< Low measurement reliability */
	SENSOR_HRM_BATCH_STATE_DETACHED_AUTO = -5,           /**< Device detachment was detected during auto measurement */
	SENSOR_HRM_BATCH_STATE_DETACHED  = -3,               /**< Device detachment was detected */
	SENSOR_HRM_BATCH_STATE_DETECT_MOVE = -2,             /**< The Movement was detected during on-demand measurement */
	SENSOR_HRM_BATCH_STATE_ATTACHED = -1,                /**< Device attachment was detected */
	SENSOR_HRM_BATCH_STATE_NONE = 0,                     /**< Initial state before measurement */
	SENSOR_HRM_BATCH_STATE_OK = 1                        /**< Heart-rate was measured normally */
} sensor_hrm_batch_state_e;

/**
 * @brief   Enumeration for sensor types.
 * @since_tizen 2.3
 */
typedef enum
{
	SENSOR_ALL = -1,                        /**< All sensors. This can be used to retrieve #sensor_h for all available sensors. */
	SENSOR_ACCELEROMETER,                   /**< Accelerometer */
	SENSOR_GRAVITY,                         /**< Gravity sensor */
	SENSOR_LINEAR_ACCELERATION,             /**< Linear acceleration sensor */
	SENSOR_MAGNETIC,                        /**< Magnetic sensor */
	SENSOR_ROTATION_VECTOR,                 /**< Rotation vector sensor */
	SENSOR_ORIENTATION,                     /**< Orientation sensor */
	SENSOR_GYROSCOPE,                       /**< Gyroscope */
	SENSOR_LIGHT,                           /**< Light sensor */
	SENSOR_PROXIMITY,                       /**< Proximity sensor */
	SENSOR_PRESSURE,                        /**< Pressure sensor */
	SENSOR_ULTRAVIOLET,                     /**< Ultraviolet sensor */
	SENSOR_TEMPERATURE,                     /**< Temperature sensor */
	SENSOR_HUMIDITY,                        /**< Humidity sensor */
	SENSOR_HRM,                             /**< Heart-rate monitor (Since 2.3.1)
	                                             @n Privilege : %http://tizen.org/privilege/healthinfo */
	SENSOR_HRM_LED_GREEN,                   /**< Green LED sensor of HRM (Since 2.3.1)
	                                             @n Privilege : %http://tizen.org/privilege/healthinfo */
	SENSOR_HRM_LED_IR,                      /**< Infra-Red LED sensor of HRM (Since 2.3.1)
	                                             @n Privilege : %http://tizen.org/privilege/healthinfo */
	SENSOR_HRM_LED_RED,                     /**< Red LED sensor of HRM (Since 2.3.1)
	                                             @n Privilege : %http://tizen.org/privilege/healthinfo */
	SENSOR_GYROSCOPE_UNCALIBRATED,          /**< Uncalibrated Gyroscope sensor (Since 2.4) */
	SENSOR_GEOMAGNETIC_UNCALIBRATED,        /**< Uncalibrated Geomagnetic sensor (Since 2.4) */
	SENSOR_GYROSCOPE_ROTATION_VECTOR,       /**< Gyroscope-based rotation vector sensor (Since 2.4) */
	SENSOR_GEOMAGNETIC_ROTATION_VECTOR,     /**< Geomagnetic-based rotation vector sensor (Since 2.4) */
	SENSOR_GYROSCOPE_ORIENTATION =  100,    /**< Orientation sensor based on gyroscope rotation vector Since 6.5 */
	SENSOR_GEOMAGNETIC_ORIENTATION = 105,   /**< Orientation sensor based on geomagnetic rotation vector Since 6.5 */
	SENSOR_SIGNIFICANT_MOTION = 256,        /**< Significant motion sensor (Since 4.0) */
	SENSOR_HRM_BATCH = 512,                 /**< Heart-rate monitor batch sensor (Since 5.5)
	                                             @n Privilege : %http://tizen.org/privilege/healthinfo */
	SENSOR_HRM_LED_GREEN_BATCH,             /**< Green LED of HRM batch sensor (Since 5.5)
	                                             @n Privilege : %http://tizen.org/privilege/healthinfo */
	SENSOR_HUMAN_PEDOMETER = 768,           /**< Pedometer (Since 3.0)
	                                             @n Privilege : %http://tizen.org/privilege/healthinfo */
	SENSOR_HUMAN_SLEEP_MONITOR,             /**< Sleep monitor (Since 3.0)
	                                             @n Privilege : %http://tizen.org/privilege/healthinfo */
	SENSOR_HUMAN_SLEEP_DETECTOR,            /**< Sleep detector (Since 3.0)
	                                             @n Privilege : %http://tizen.org/privilege/healthinfo */
	SENSOR_HUMAN_STRESS_MONITOR,            /**< Stress monitor (Since 3.0)
	                                             @n Privilege : %http://tizen.org/privilege/healthinfo
	                                             @deprecated Deprecated since 5.5 */
	SENSOR_LAST,                            /**< End of sensor enum values @deprecated Deprecated since 3.0 */
	SENSOR_CUSTOM = 10000,                  /**< Custom sensor @deprecated Deprecated since 3.0 */
} sensor_type_e;


/**
 * @brief   Checks whether a given sensor type is supported in the current device.
 * @details If sensor type is supported, @a supported will be true. Otherwise,
 *          it will be false and the return value will be an error.
 * @since_tizen 2.3
 *
 * @remarks If the given sensor type is not supported,
 *          sensor_get_default_sensor() will return an error.
 *          It is thus recommended to check the availability of the sensor
 *          before actually acquiring #sensor_h.
 *
 * @param[in]   type        A sensor type to check
 * @param[out]  supported   If supported, @c true; Otherwise @c false
 *
 * @return  #SENSOR_ERROR_NONE on success; Otherwise a negative error value
 * @retval  #SENSOR_ERROR_NONE                 Successful
 * @retval  #SENSOR_ERROR_INVALID_PARAMETER    Invalid parameter
 *
 * @see     sensor_is_supported_by_uri()
 *
 * @par Example
 * @code
 * ...
 * sensor_type_e type;
 * bool supported;
 * ...
 * sensor_is_supported(type, &supported);
 * if (!supported)
 *     return -1;
 * @endcode
 */
int sensor_is_supported(sensor_type_e type, bool *supported);

/**
 * @brief   Checks whether a sensor corresponding to a given URI is supported in the current device.
 * @details @a supported is set as @c true if the sensor is available; otherwise, it is set as @c false.
 *          It is recommended to check the availability of the sensors corresponding to the URI
 *          before actually acquiring #sensor_h.
 * @since_tizen 4.0
 *
 * @remarks @a uri should be valid and not null to correctly check the availability of a sensor.
 *          The valid form of URIs is explained in \ref CAPI_SYSTEM_SENSOR_LISTENER_MODULE_URI.\n
 *          If a URI with the sensor's name is given, for example, %http://tizen.org/sensor/general/light/front,
 *          the availability of the @"front@" light sensor is checked.
 *          Otherwise, if a URI without the name, %http://tizen.org/sensor/general/light, is given,
 *          this function checks the availability of any light sensor in the device.
 *
 * @param[in]   uri         A sensor or a sensor type URI to check
 * @param[out]  supported   If supported, @c true, otherwise @c false
 *
 * @return  #SENSOR_ERROR_NONE on success, otherwise a negative error value
 * @retval  #SENSOR_ERROR_NONE                Successful
 * @retval  #SENSOR_ERROR_INVALID_PARAMETER   Invalid parameter
 *
 * @see     sensor_is_supported()
 *
 * @par Example
 * @code
 * bool supported;
 * sensor_is_supported_by_uri("http://tizen.org/sensor/general/light/front", &supported);
 * if (!supported) {
 *     return -1;
 * }
 * @endcode
 */
int sensor_is_supported_by_uri(const char *uri, bool *supported);


/**
 * @brief   Checks whether a given sensor is a wake-up sensor or not.
 * @details If a sensor is a wake-up sensor, the sensor is able to wake-up the system
 *          to report its sensor data even if the system is in sleep mode.
 * @since_tizen 3.0
 *
 * @param[in]   sensor  A sensor handle to check
 * @param[out]  wakeup  If the sensor is a wake-up sensor, @c true;
 *                      Otherwise @c false
 *
 * @return  #SENSOR_ERROR_NONE on success, otherwise a negative error value
 * @retval  #SENSOR_ERROR_NONE                 Successful
 * @retval  #SENSOR_ERROR_INVALID_PARAMETER    Invalid parameter
 *
 * @pre     The handle @c sensor needs to be initialized using
 *          sensor_get_default_sensor() or sensor_get_sensor_list() in advance.
 *
 * @par Example
 * @code
 * bool is_wakeup_sensor = false;
 * sensor_is_wake_up(sensor, &is_wakeup_sensor);
 * is (!is_wakeup_sensor)
 *     return -1;
 * @endcode
 */
int sensor_is_wake_up(sensor_h sensor, bool *wakeup);

/**
 * @brief   Gets the handle for the default sensor of a given type.
 * @details This function returns the handle for the sensor of a given type.
 *          If the device has more than one sensor of the given type,
 *          this function returns the default sensor of the given type,
 *          which is designated by the device.
 * @since_tizen 2.3
 *
 * @remarks Some sensor types are privileged. An application should have the privilege
 *          %http://tizen.org/privilege/healthinfo to get handles for the following sensors:
 *          #SENSOR_HRM, #SENSOR_HRM_LED_GREEN, #SENSOR_HRM_LED_IR, #SENSOR_HRM_LED_RED,
 *          #SENSOR_HUMAN_PEDOMETER, #SENSOR_HUMAN_SLEEP_MONITOR, #SENSOR_HUMAN_SLEEP_DETECTOR,
 *          and #SENSOR_HUMAN_STRESS_MONITOR.
 * @remarks The @a sensor should not be released.
 *
 * @param[in]  type     A sensor type to get the handle of its default sensor
 * @param[out] sensor   The sensor handle of the default sensor
 *
 * @return  #SENSOR_ERROR_NONE on success; Otherwise a negative error value
 * @retval  #SENSOR_ERROR_NONE                 Successful
 * @retval  #SENSOR_ERROR_INVALID_PARAMETER    Invalid parameter
 * @retval  #SENSOR_ERROR_NOT_SUPPORTED        The sensor type is not supported in the current device
 * @retval  #SENSOR_ERROR_PERMISSION_DENIED    Permission denied
 *
 * @see     sensor_get_default_sensor_by_uri()
 * @see     sensor_get_sensor_list()
 *
 * @par Example
 * @code
 * #include <sensor.h>
 * ...
 * int ret = 0;
 * sensor_h sensor;
 * ret = sensor_get_default_sensor(SENSOR_ACCELEROMETER, &sensor);
 * if (ret != SENSOR_ERROR_NONE)
 *         return ret;
 * ...
 * @endcode
 */
int sensor_get_default_sensor(sensor_type_e type, sensor_h *sensor);

/**
 * @brief   Gets the handle for the default sensor of a given sensor URI.
 * @details This function returns the handle for the sensor of a given URI.
 *          If the device has more than one sensor of the given URI,
 *          this function returns the default sensor of the URI, which is designated by the device.\n
 *          See \ref CAPI_SYSTEM_SENSOR_LISTENER_MODULE_URI for more details about the URI format.
 * @since_tizen 4.0
 *
 * @remarks Some sensor types are privileged. An application should have the privilege
 *          %http://tizen.org/privilege/healthinfo to get handles for the sensor URIs of @"healthinfo@" category.
 *          See \ref CAPI_SYSTEM_SENSOR_LISTENER_MODULE_URI to find the @"healthinfo@" category sensor URIs.
 * @remarks The @a sensor should not be released.
 *
 * @param[in]    uri      A sensor or a sensor type URI to get the handle of its default sensor
 * @param[out]   sensor   The default sensor handle
 *
 * @return  #SENSOR_ERROR_NONE on success, otherwise a negative error value
 * @retval  #SENSOR_ERROR_NONE                 Successful
 * @retval  #SENSOR_ERROR_INVALID_PARAMETER    Invalid parameter
 * @retval  #SENSOR_ERROR_NOT_SUPPORTED        The URI is not supported in the current device
 * @retval  #SENSOR_ERROR_PERMISSION_DENIED    Permission denied
 *
 * @see     sensor_get_default_sensor()
 * @see     sensor_get_sensor_list_by_uri()
 *
 * @par Example
 * @code
 * #include <sensor.h>
 * ...
 * int ret = 0;
 * sensor_h sensor;
 * ret = sensor_get_default_sensor_by_uri("http://tizen.org/sensor/general/accelerometer", &sensor);
 * if (ret != SENSOR_ERROR_NONE)
 *         return ret;
 * ...
 * @endcode
 */
int sensor_get_default_sensor_by_uri(const char *uri, sensor_h *sensor);


/**
 * @brief   Gets the handle list of the sensors of a given type.
 * @details A device may have more than one sensors of the given type.
 *          In such case, this function can be used to get the handles of all sensors of the type.@n
 *          The first element of the @a list denotes the default sensor,
 *          which can be retrieved by sensor_get_default_sensor().
 * @since_tizen 2.3
 *
 * @remarks Some sensor types are privileged. An application should have the privilege
 *          %http://tizen.org/privilege/healthinfo to get handles for the following sensors:
 *          #SENSOR_HRM, #SENSOR_HRM_LED_GREEN, #SENSOR_HRM_LED_IR, #SENSOR_HRM_LED_RED,
 *          #SENSOR_HUMAN_PEDOMETER, #SENSOR_HUMAN_SLEEP_MONITOR, #SENSOR_HUMAN_SLEEP_DETECTOR,
 *          and #SENSOR_HUMAN_STRESS_MONITOR.@n
 *          Instead of specifying a sensor type, by using #SENSOR_ALL,
 *          applications can get the list of handles for all available sensors.@n
 *          The @a list must be released using free(), if not being used anymore.@n
 *
 * @param[in]  type         A sensor type to get the list of sensor handles
 * @param[out] list         An array of the sensor handles
 * @param[out] sensor_count The number of handles contained in @a list
 *
 * @return  #SENSOR_ERROR_NONE on success; Otherwise a negative error value
 * @retval  #SENSOR_ERROR_NONE                 Successful
 * @retval  #SENSOR_ERROR_INVALID_PARAMETER    Invalid parameter
 * @retval  #SENSOR_ERROR_NOT_SUPPORTED        The sensor type is not supported in the current device
 * @retval  #SENSOR_ERROR_PERMISSION_DENIED    Permission denied
 * @retval  #SENSOR_ERROR_OUT_OF_MEMORY        Out of memory
 *
 * @par Example
 * @code
 * sensor_h *sensor_list = NULL;
 * int sensor_list_len = 0;
 * sensor_get_sensor_list(SENSOR_ACCELEROMETER, &sensor_list, &sensor_list_len);
 * @endcode
 */
int sensor_get_sensor_list(sensor_type_e type, sensor_h **list, int *sensor_count);

/**
 * @brief   Gets the handle list of the sensors of a given URI.
 * @details A device may have more than one sensor of the given URI.
 *          In such case, this function can be used to get the handles of all sensors of the given URI.\n
 *          See \ref CAPI_SYSTEM_SENSOR_LISTENER_MODULE_URI for more details about the URI format.
 * @since_tizen 4.0
 *
 * @remarks Some sensors require one or more privileges. An application should have the corresponding privilege
 *          to get handles for such sensors.
 *          See sensor_get_default_sensor_by_uri() for more details.@n
 *          The @a list must be released using free(), if not being used anymore.
 *
 * @param[in]   uri            A sensor or a sensor type URI to get the list of sensor handles
 * @param[out]  list           An array of the sensor handles
 * @param[out]  sensor_count   The number of handles contained in @a list
 *
 * @return  #SENSOR_ERROR_NONE on success, otherwise a negative error value
 * @retval  #SENSOR_ERROR_NONE                Successful
 * @retval  #SENSOR_ERROR_INVALID_PARAMETER   Invalid parameter
 * @retval  #SENSOR_ERROR_NOT_SUPPORTED       The URI is not supported in the current device
 * @retval  #SENSOR_ERROR_PERMISSION_DENIED   Permission denied
 * @retval  #SENSOR_ERROR_OUT_OF_MEMORY       Out of memory
 *
 * @par Example
 * @code
 * sensor_h *sensor_list = NULL;
 * int sensor_list_len = 0;
 * sensor_get_sensor_list("http://tizen.org/sensor/general/accelerometer", &sensor_list, &sensor_list_len);
 * @endcode
 */
int sensor_get_sensor_list_by_uri(const char *uri, sensor_h **list, int *sensor_count);


/**
 * @brief   Gets the URI of a given sensor as a newly allocated string.
 * @details Get the URI of a given sensor and store it in the @a uri.
 * @since_tizen 4.0
 *
 * @remarks The @a uri must be released using free(), if not being used anymore.
 *
 * @param[in]   sensor   A sensor handle
 * @param[out]  uri      The URI of the sensor
 *
 * @return  #SENSOR_ERROR_NONE on success, otherwise a negative error value
 * @retval  #SENSOR_ERROR_NONE                 Successful
 * @retval  #SENSOR_ERROR_INVALID_PARAMETER    Invalid parameter
 *
 * @par Example
 * @code
 * ...
 * char *uri = NULL;
 * sensor_get_uri(sensor, &uri);
 * ...
 * free(uri);
 * @endcode
 */
int sensor_get_uri(sensor_h sensor, char **uri);

/**
 * @brief   Gets the name of a given sensor as a newly allocated string.
 * @details Get the name of a given sensor and store it in the @a name.
 * @since_tizen 2.3
 *
 * @remarks  The @a name must be released using free(), if not being used anymore.
 *
 * @param[in]   sensor  A sensor handle
 * @param[out]  name    The name of the sensor
 *
 * @return  #SENSOR_ERROR_NONE on success, otherwise a negative error value
 * @retval  #SENSOR_ERROR_NONE                 Successful
 * @retval  #SENSOR_ERROR_INVALID_PARAMETER    Invalid parameter
 *
 * @par Example
 * @code
 * ...
 * char *name = NULL;
 * sensor_get_name(sensor, &name);
 * ...
 * free(name);
 * @endcode
 */
int sensor_get_name(sensor_h sensor, char **name);


/**
 * @brief   Gets the vandor name of a given sensor as a newly allocated string.
 * @details Get the vendor name of a given sensor and store it in the @a vendor.
 * @since_tizen 2.3
 *
 * @remarks  The @a vendor must be released using free(), if not being used anymore.
 *
 * @param[in]   sensor  A sensor handle
 * @param[out]  vendor  The vendor of the sensor
 *
 * @return  #SENSOR_ERROR_NONE on success, otherwise a negative error value
 * @retval  #SENSOR_ERROR_NONE                 Successful
 * @retval  #SENSOR_ERROR_INVALID_PARAMETER    Invalid parameter
 *
 * @par Example
 * @code
 * ...
 * char *vendor_name = NULL;
 * sensor_get_vendor(sensor, &vendor_name);
 * ...
 * free(vendor_name);
 * @endcode
 */
int sensor_get_vendor(sensor_h sensor, char **vendor);


/**
 * @brief   Gets the type of a given sensor as #sensor_type_e data type.
 * @details Get the type of a given sensor if it belongs to the known types
 *          defined in #sensor_type_e.
 * @since_tizen 2.3
 *
 * @param[in]   sensor  A sensor handle
 * @param[out]  type    The type of the sensor
 *
 * @return  #SENSOR_ERROR_NONE on success, otherwise a negative error value
 * @retval  #SENSOR_ERROR_NONE                 Successful
 * @retval  #SENSOR_ERROR_INVALID_PARAMETER    Invalid parameter
 * @retval  #SENSOR_ERROR_NO_DATA              No known sensor type for the given sensor
 * @retval  #SENSOR_ERROR_OPERATION_FAILED     Operation failed
 *
 * @par Example
 * @code
 * ...
 * sensor_type_e sensor_type;
 * sensor_get_type(sensor, &sensor_type);
 * @endcode
 */
int sensor_get_type(sensor_h sensor, sensor_type_e *type);


/**
 * @brief   Gets the lower bound of the sensor reading of a sensor.
 * @details This function returns the lower bound of the range of possible sensor values,
 *          which are generated by the corresponding sensor denoted by a sensor handle.@n
 *          If all sensor values are in the same unit, e.g., \f$\mbox{m/s}^2\f$ or degrees,
 *          the lower bound of all sensor values is returned.
 *          Otherwise, the lower bound of the representative sensor value, e.g.,
 *          the step count of #SENSOR_HUMAN_PEDOMETER, is returned.
 * @since_tizen 2.3
 *
 * @param[in]   sensor      A sensor handle
 * @param[out]  min_range   The lower bound
 *
 * @return  #SENSOR_ERROR_NONE on success, otherwise a negative error value
 * @retval  #SENSOR_ERROR_NONE                 Successful
 * @retval  #SENSOR_ERROR_INVALID_PARAMETER    Invalid parameter
 * @retval  #SENSOR_ERROR_OPERATION_FAILED     Operation failed
 *
 * @see     sensor_get_max_range()
 *
 * @par Example
 * @code
 * ...
 * float min_range;
 * sensor_get_min_range(sensor, &min_range);
 * @endcode
 */
int sensor_get_min_range(sensor_h sensor, float *min_range);


/**
 * @brief   Gets the upper bound of the sensor readings of a sensor.
 * @details This function returns the upper bound of the range of possible sensor values,
 *          which are generated by the corresponding sensor denoted by a sensor handle.@n
 *          If all sensor values are in the same unit, e.g., \f$\mbox{m/s}^2\f$ or degrees,
 *          the upper bound of all sensor values is returned.
 *          Otherwise, the upper bound of the representative sensor value, e.g.,
 *          the step count of #SENSOR_HUMAN_PEDOMETER, is returned.
 * @since_tizen 2.3
 *
 * @param[in]   sensor      A sensor handle
 * @param[out]  max_range   The upper bound
 *
 * @return  #SENSOR_ERROR_NONE on success, otherwise a negative error value
 * @retval  #SENSOR_ERROR_NONE                 Successful
 * @retval  #SENSOR_ERROR_INVALID_PARAMETER    Invalid parameter
 * @retval  #SENSOR_ERROR_OPERATION_FAILED     Operation failed
 *
 * @see     sensor_get_min_range()
 *
 * @par Example
 * @code
 * ...
 * float max_range;
 * sensor_get_max_range(sensor, &max_range);
 * @endcode
 */
int sensor_get_max_range(sensor_h sensor, float *max_range);


/**
 * @brief   Gets the resolution of the sensor readings of a sensor.
 * @details This function returns the resolution of the sensor readings.
 *          The resolution denotes the smallest difference between sensor readings,
 *          each of which is in the range that can be verified by
 *          sensor_get_min_range() and sensor_get_max_range().
 * @since_tizen 2.3
 *
 * @param[in]   sensor      A sensor handle
 * @param[out]  resolution  The resolution
 *
 * @return  #SENSOR_ERROR_NONE on success, otherwise a negative error value
 * @retval  #SENSOR_ERROR_NONE                 Successful
 * @retval  #SENSOR_ERROR_INVALID_PARAMETER    Invalid parameter
 * @retval  #SENSOR_ERROR_OPERATION_FAILED     Operation failed
 *
 * @par Example
 * @code
 * ...
 * float resolution;
 * sensor_get_resolution(sensor, &resolution);
 * @endcode
 */
int sensor_get_resolution(sensor_h sensor, float *resolution);


/**
 * @brief   Gets the minimum update interval of a given sensor.
 * @details Get the minimum update interval and store it in @a min_interval.
 * @since_tizen 2.3
 *
 * @param[in]   sensor          A sensor handle
 * @param[out]  min_interval    The shorted interval in milliseconds
 *
 * @return  #SENSOR_ERROR_NONE on success, otherwise a negative error value
 * @retval  #SENSOR_ERROR_NONE                 Successful
 * @retval  #SENSOR_ERROR_INVALID_PARAMETER    Invalid parameter
 * @retval  #SENSOR_ERROR_OPERATION_FAILED     Operation failed
 *
 * @par Example
 * @code
 * ...
 * int min_interval;
 * sensor_get_min_interval(sensor, &min_interval);
 * @endcode
 */
int sensor_get_min_interval(sensor_h sensor, int *min_interval);


/**
 * @brief   Gets the size of the hardware FIFO of a given sensor.
 * @details This function returns the size of the hardware FIFO that may be used by
 *          a specific sensor to support batching.
 *          However, regarding the underlying hardware configuration,
 *          the returned count may not mean the maximum number of sensor data that can be batched.
 *          See sensor_get_max_batch_count() for such purpose, finding out the
 *          possible maximum number of batched data.
 * @since_tizen 2.3
 *
 * @param[in]   sensor          A sensor handle
 * @param[out]  fifo_count      The FIFO count
 *
 * @return  #SENSOR_ERROR_NONE on success, otherwise a negative error value
 * @retval  #SENSOR_ERROR_NONE                 Successful
 * @retval  #SENSOR_ERROR_INVALID_PARAMETER    Invalid parameter
 * @retval  #SENSOR_ERROR_OPERATION_FAILED     Operation failed
 *
 * @par Example
 * @code
 * ...
 * int fifo_count;
 * sensor_get_min_interval(sensor, &fifo_count);
 * @endcode
 */
int sensor_get_fifo_count(sensor_h sensor, int *fifo_count);


/**
 * @brief   Gets the maximum value of batch count of a given sensor.
 * @details This function returns the maximum number of sensor data events
 *          that can be possibly delivered when the batched data are flushed.
 *          Therefore, this count can be used to check whether the sensor supports
 *          batching or not.@n
 *          If this returns a positive count, i.e., the sensor supports batching,
 *          the count also can be used to guess the possible longest batch latency
 *          of the sensor, with respect to the update interval to use.
 * @since_tizen 2.3
 *
 * @param[in]   sensor          A sensor handle
 * @param[out]  max_batch_count If the sensor does not support batching, 0;
 *                              Otherwise a positive integer.
 *
 * @return  #SENSOR_ERROR_NONE on success, otherwise a negative error value
 * @retval  #SENSOR_ERROR_NONE                 Successful
 * @retval  #SENSOR_ERROR_INVALID_PARAMETER    Invalid parameter
 * @retval  #SENSOR_ERROR_OPERATION_FAILED     Operation failed
 *
 * @see     sensor_listener_set_max_batch_latency()
 *
 * @par Example
 * @code
 * ...
 * int max_batch_count = 0;
 * sensor_get_min_interval(sensor, &max_batch_count);
 * @endcode
 */
int sensor_get_max_batch_count(sensor_h sensor, int *max_batch_count);


/**
 * @brief   Callback function type to be called when a new sensor is added.
 * @details Will be called when a new sensor is added with parameters: uri of a
 *          newly added sensor and user_data which is passed to
 *          sensor_add_sensor_cb().
 * @since_tizen 4.0
 *
 * @remarks @a uri should not be freed and can be used only in the callback.
 *          To use outside the callback, make a copy.
 *
 * @param[in]  uri         The URI of the newly added sensor
 * @param[in]  user_data   The user data had passed to sensor_add_sensor_added_cb().
 *
 * @pre   A callback function needs to be set using sensor_add_sensor_added_cb().
 * @see   sensor_add_sensor_added_cb()
 * @see   sensor_remove_sensor_added_cb()
 *
 * @par Example
 * @code
 * void sensor_add_callback(const char *uri, void *user_data)
 * {
 *     ...
 * }
 * ...
 *     sensor_add_sensor_added_cb(sensor_add_callback, NULL);
 * ...
 *     sensor_remove_sensor_added_cb(sensor_add_callback);
 * @endcode
 */
typedef void (*sensor_added_cb)(const char *uri, void *user_data);

/**
 * @brief   Adds a callback function to be invoked when a new sensor is added.
 * @details Add a given callback function @a callback to be invoked when a new
 *          sensor is added. @a user_data will be passed to the callback
 *          function when it is called.
 * @since_tizen 4.0
 *
 * @param[in]  callback   A callback function to be added
 * @param[in]  user_data  A user data to be passed to the callback function
 *
 * @return  #SENSOR_ERROR_NONE on success, otherwise a negative error value
 * @retval  #SENSOR_ERROR_NONE                 Successful
 * @retval  #SENSOR_ERROR_INVALID_PARAMETER    Invalid parameter
 * @retval  #SENSOR_ERROR_OPERATION_FAILED     Operation failed
 *
 * @see     sensor_remove_sensor_added_cb()
 *
 * @par Example
 * @code
 * void sensor_add_callback(const char *uri, void *user_data)
 * {
 *     ...
 * }
 * ...
 *     sensor_add_sensor_added_cb(sensor_add_callback, NULL);
 * ...
 *     sensor_remove_sensor_added_cb(sensor_add_callback);
 * @endcode
 */
int sensor_add_sensor_added_cb(sensor_added_cb callback, void *user_data);

/**
 * @brief   Removes a callback added by sensor_add_sensor_added_cb().
 * @details Remove a given callback function which is added by
 *          sensor_add_sensor_added_cb() before.
 * @since_tizen 4.0
 *
 * @param[in]  callback  A callback function to be removed
 *
 * @return  #SENSOR_ERROR_NONE on success, otherwise a negative error value
 * @retval  #SENSOR_ERROR_NONE                 Successful
 * @retval  #SENSOR_ERROR_INVALID_PARAMETER    Invalid parameter
 * @retval  #SENSOR_ERROR_OPERATION_FAILED     Operation failed
 *
 * @see     sensor_add_sensor_added_cb()
 *
 * @par Example
 * @code
 * void sensor_add_callback(const char *uri, void *user_data)
 * {
 *     ...
 * }
 * ...
 *     sensor_add_sensor_added_cb(sensor_add_callback, NULL);
 * ...
 *     sensor_remove_sensor_added_cb(sensor_add_callback);
 * @endcode
 */
int sensor_remove_sensor_added_cb(sensor_added_cb callback);

/**
 * @brief   Callback function type to be called when a sensor is removed.
 * @details Will be called when a sensor is removed with parameters: uri of a
 *          removed sensor and user_data which is passed to
 *          sensor_add_sensor_cb().
 * @since_tizen 4.0
 *
 * @remarks @a uri should not be freed and can be used only in the callback.
 *          To use outside the callback, make a copy.
 *
 * @param[in]  uri        The URI of the removed sensor
 * @param[in]  user_data  The user data had passed to sensor_add_sensor_removed_cb()
 *
 * @pre   A callback function needs to be set using sensor_add_sensor_removed_cb().
 * @see   sensor_add_sensor_removed_cb()
 * @see   sensor_remove_sensor_removed_cb()
 *
 * @par Example
 * @code
 * void sensor_removed_callback(const char *uri, void *user_data)
 * {
 *     ...
 * }
 * ...
 *     sensor_add_sensor_removed_cb(sensor_add_callback, NULL);
 * ...
 *     sensor_remove_sensor_removed_cb(sensor_add_callback);
 * @endcode
 */
typedef void (*sensor_removed_cb)(const char *uri, void *user_data);

/**
 * @brief  Adds a callback function to be invoked when a sensor is removed.
 * @details Add a given callback function @a callback to be invoked when a
 *          sensor is removed. @a user_data will be passed to the callback
 *          function when it is called.
 * @since_tizen 4.0
 *
 * @param[in]  callback   A callback function to be removed
 * @param[in]  user_data  A user data to be passed to the callback function
 *
 * @return  #SENSOR_ERROR_NONE on success, otherwise a negative error value
 * @retval  #SENSOR_ERROR_NONE                 Successful
 * @retval  #SENSOR_ERROR_INVALID_PARAMETER    Invalid parameter
 * @retval  #SENSOR_ERROR_OPERATION_FAILED     Operation failed
 *
 * @see   sensor_remove_sensor_removed_cb()
 *
 * @par Example
 * @code
 * void sensor_removed_callback(const char *uri, void *user_data)
 * {
 *     ...
 * }
 * ...
 *     sensor_add_sensor_removed_cb(sensor_add_callback, NULL);
 * ...
 *     sensor_remove_sensor_removed_cb(sensor_add_callback);
 * @endcode
 */
int sensor_add_sensor_removed_cb(sensor_removed_cb callback, void *user_data);

/**
 * @brief  Removes a callback function added using sensor_add_sensor_removed_cb().
 * @details Remove a given callback function which is added by
 *          sensor_add_sensor_removed_cb() before.
 * @since_tizen 4.0
 *
 * @param[in]  callback  A callback function to be removed
 *
 * @return  #SENSOR_ERROR_NONE on success, otherwise a negative error value
 * @retval  #SENSOR_ERROR_NONE                 Successful
 * @retval  #SENSOR_ERROR_INVALID_PARAMETER    Invalid parameter
 * @retval  #SENSOR_ERROR_OPERATION_FAILED     Operation failed
 *
 * @see   sensor_add_sensor_removed_cb()
 *
 * @par Example
 * @code
 * void sensor_removed_callback(const char *uri, void *user_data)
 * {
 *     ...
 * }
 * ...
 *     sensor_add_sensor_removed_cb(sensor_add_callback, NULL);
 * ...
 *     sensor_remove_sensor_removed_cb(sensor_add_callback);
 * @endcode
 */
int sensor_remove_sensor_removed_cb(sensor_removed_cb callback);

/**
 * @}
 */

/**
 * @addtogroup CAPI_SYSTEM_SENSOR_LISTENER_MODULE
 * @{
 */

/**
 * @brief   The upper bound of #sensor_event_s::value_count.
 * @since_tizen 2.3
 */
#define MAX_VALUE_SIZE 16

/**
 * @brief   Sensor listener handle.
 * @details For each #sensor_h, one or more sensor listeners can be created by using sensor_create_listener().
 *          Then the sensor's data can observed asynchronously, can be read synchronously if available, via the listener.
 *          Applications are also able to control the behavior of each sensor, for example,
 *          update interval of sensor readings.
 * @since_tizen 2.3
 */
typedef struct sensor_listener_s *sensor_listener_h;


/**
 * @brief   Sensor data event delivered via sensor_event_cb().
 * @details A sensor data is delivered as a structure, which contains the accuracy of the data,
 *          the time when the data was observed, and the data array.
 *          The data array is a fixed size @c float array, and the number of data fields
 *          stored in the array varies with the sensor type.
 *          For example, #SENSOR_ACCELEROMETER reports 3-dimensional data,
 *          #sensor_event_s::value_count is thus set to 3.@n
 *          Note that, even if the data values are @c float, in some cases,
 *          it may contain one or more categorical data as in #sensor_proximity_e.
 * @since_tizen 2.3
 * @see     #sensor_pedometer_state_e
 * @see     #sensor_sleep_state_e
 */
typedef struct
{
	int accuracy;                  /**< Accuracy of sensor data */
	unsigned long long timestamp;  /**< Time when the sensor data was observed */
	int value_count;               /**< Number of sensor data values stored in #sensor_event_s::values */
	float values[MAX_VALUE_SIZE];  /**< Sensor data values */
} sensor_event_s;


/**
 * @brief   Enumeration for sensor data accuracy.
 * @since_tizen 2.3
 */
typedef enum
{
	SENSOR_DATA_ACCURACY_UNDEFINED   = -1,  /**< Undefined */
	SENSOR_DATA_ACCURACY_BAD         = 0,   /**< Not accurate */
	SENSOR_DATA_ACCURACY_NORMAL      = 1,   /**< Moderately accurate */
	SENSOR_DATA_ACCURACY_GOOD        = 2,   /**< Highly accurate */
	SENSOR_DATA_ACCURACY_VERYGOOD    = 3    /**< Very highly accurate */
} sensor_data_accuracy_e;


/**
 * @brief   Enumeration for sensor listener behavior attributes
 * @since_tizen 3.0
 */
typedef enum
{
	SENSOR_ATTRIBUTE_AXIS_ORIENTATION = 1,  /**< Reference orientation of sensor data to be reported.@n
	                                             See #sensor_axis_e for available attribute values. */
	SENSOR_ATTRIBUTE_PAUSE_POLICY,          /**< Pause-and-resume policy of sensors.@n
	                                             See #sensor_pause_e for available attribute values. */
} sensor_attribute_e;


/**
 * @brief   Enumeration for sensor options.
 * @since_tizen 2.3
 */
#ifndef __SENSOR_COMMON_H__
typedef enum
{
	SENSOR_OPTION_DEFAULT,              /**< Does not receive data when the LCD is off and in the power save mode */
	SENSOR_OPTION_ON_IN_SCREEN_OFF,     /**< Receives data when the LCD is off */
	SENSOR_OPTION_ON_IN_POWERSAVE_MODE, /**< Receives data in the power save mode */
	SENSOR_OPTION_ALWAYS_ON,            /**< Receives data when the LCD is off and in the power save mode */
} sensor_option_e;
#endif


/**
 * @brief   Enumeration for reference orientations of sensor data
 * @details The sensor's physical orientation may differ from what applications are aware of,
 *          in cases that the device has a rotated screen, physically or logically.
 *          For example, a watch device may have right hand mode, which logically rotates
 *          the display 180 degrees.
 *          Applications may not be aware of such situations, thus they may receives
 *          sensor data inverted in X and Y directions.
 *          With #SENSOR_AXIS_DISPLAY_ORIENTED option, applications can get data that
 *          are properly aligned with the orientation of which they are aware.@n
 *          By default, #SENSOR_AXIS_DISPLAY_ORIENTED is used.
 *          If you need to use the data that are not affected by display orientations,
 *          #SENSOR_AXIS_DEVICE_ORIENTED needs to be set.
 * @since_tizen 3.0
 */
typedef enum
{
	SENSOR_AXIS_DEVICE_ORIENTED = 1,    /**< Using the device orientation as the reference coordinate system */
	SENSOR_AXIS_DISPLAY_ORIENTED,       /**< Using the display orientation as the reference coordinate system */
} sensor_axis_e;


/**
 * @brief   Enumeration for pause policies of sensor listeners
 * @details To be power-efficient, you can set the policy of how to pause and resume
 *          a sensor listener regarding the system status.
 *          By default, #SENSOR_PAUSE_ALL is used to obtain the maximum power efficiency.
 * @since_tizen 3.0
 */
typedef enum
{
	SENSOR_PAUSE_NONE = 0,              /**< The sensor will not pause, unless the system goes into sleep mode */
	SENSOR_PAUSE_ON_DISPLAY_OFF = 1,    /**< The sensor pauses while the display is off*/
	SENSOR_PAUSE_ON_POWERSAVE_MODE = 2, /**< The sensor pauses while the power-save mode is enabled */
	SENSOR_PAUSE_ALL = 3,               /**< The sensor pauses in all the above cases */
} sensor_pause_e;


/**
 * @deprecated Deprecated since 5.5. Use sensor_events_cb() instead.
 * @brief   Called when a sensor event occurs.
 * @since_tizen 2.3
 *
 * @remarks @a sensor should not be freed, it's managed by platform.
 * @remarks @a event should not be freed and can be used only in the callback. To use outside the callback, make a copy.
 *
 * @param[in] sensor        The corresponding sensor handle
 * @param[in] event         A sensor event
 * @param[in] user_data     The user data had passed to sensor_listener_set_event_cb()
 *
 * @pre     The sensor needs to be started regarding a listener handle, using sensor_listener_start().
 */
typedef void (*sensor_event_cb)(sensor_h sensor, sensor_event_s *event, void *user_data) TIZEN_DEPRECATED_API;


/**
 * @brief   Callback function type to be called when sensor events occur.
 * @details Will be called when a sensor event is occured. Callback function can
 *          be added using function sensor_listener_set_events_cb().
 * @since_tizen 5.5
 *
 * @remarks @a sensor should not be freed, it's managed by platform.
 * @remarks @a events should not be freed and can be used only in the callback. To use outside the callback, make a copy.
 *
 * @param[in] sensor        The corresponding sensor handle
 * @param[in] events        The sensor events
 * @param[in] events_count  The number of events
 * @param[in] user_data     The user data passed to sensor_listener_set_events_cb()
 *
 * @pre     The sensor needs to be started regarding a listener handle, using sensor_listener_start().
 *
 * @par Example
 * @code
 * void accelerator_event_callback(sensor_h sensor, sensor_event_s events[], int events_count, void *user_data)
 * {
 *     ...
 * }
 * ...
 * sensor_listener_set_events_cb(accelerator_listener, accelerator_event_callback, NULL);
 * sensor_listener_unset_events_cb(accelerator_listener);
 * @endcode
 */
typedef void (*sensor_events_cb)(sensor_h sensor, sensor_event_s events[], int events_count, void *user_data);

/**
 * @brief   Callback function type to be called when the accuracy of a sensor is
 *          changed.
 * @details Sensors can be affected by the environment.
 *          For example, #SENSOR_MAGNETIC is sensitive to any surrounding objects that can influence
 *          electromagnetic fields. This function is called if the accuracy of the corresponding sensor is changed.
 * @since_tizen 2.3
 *
 * @param[in]   sensor      A sensor handle
 * @param[in]   timestamp   The time in milliseconds when the accuracy changed
 * @param[in]   accuracy    The current accuracy of the sensor
 * @param[in]   data        The user data had passed to sensor_listener_set_accuracy_cb()
 *
 * @par Example
 * @code
 * void accuracy_changed_callback(sensor_h sensor, unsigned long long timestamp, sensor_data_accuracy_e accuracy, void *data)
 * {
 *     ...
 * }
 * ...
 * sensor_listener_set_accuracy_cb(listener, accuracy_changed_callback, NULL);
 * ...
 * sensor_listener_unset_accuracy_cb(listener);
 * @endcode
 */
typedef void (*sensor_accuracy_changed_cb)(sensor_h sensor, unsigned long long timestamp, sensor_data_accuracy_e accuracy, void *data);


/**
 * @brief   Creates a sensor listener for a given sensor handle.
 * @details Create a listener(sensor_listener_h) for the specified sensor and
 *          return a handle of the listener to the caller through @a listener.
 * @since_tizen 2.3
 *
 * @remarks The @a listener must be released using sensor_destroy_listener().
 *
 * @param[in]  sensor          A sensor handle
 * @param[out] listener        A listener handle of @a sensor
 *
 * @return  #SENSOR_ERROR_NONE on success, otherwise a negative error value
 * @retval  #SENSOR_ERROR_NONE                 Successful
 * @retval  #SENSOR_ERROR_INVALID_PARAMETER    Invalid parameter
 * @retval  #SENSOR_ERROR_OUT_OF_MEMORY        Out of memory
 * @retval  #SENSOR_ERROR_OPERATION_FAILED     Operation failed
 *
 * @pre     The handle @a sensor needs to be initialized using
 *          sensor_get_default_sensor() or sensor_get_sensor_list() in advance.
 *
 * @par Example
 * @code
 * ...
 * sensor_listener_h listener;
 * sensor_create_listener(sensor, &listener);
 * ...
 * sensor_destroy_listener(listener);
 * @endcode
 */
int sensor_create_listener(sensor_h sensor, sensor_listener_h *listener);


/**
 * @brief   Destroy resources of a given listener for a sensor.
 * @details Release all the resources allocated for a given sensor listener.
 * @since_tizen 2.3
 *
 * @remarks If this function is called while the sensor is still running,
 *          that is, sensor_listener_start() was called but sensor_listener_stop() was not,
 *          then it is implicitly stopped.
 *
 * @param[in] listener  A listener handle
 *
 * @return  #SENSOR_ERROR_NONE on success, otherwise a negative error value
 * @retval  #SENSOR_ERROR_NONE                 Successful
 * @retval  #SENSOR_ERROR_INVALID_PARAMETER    Invalid parameter
 *
 * @see     sensor_create_listener()
 *
 * @par Example
 * @code
 * ...
 * sensor_listener_h listener;
 * sensor_create_listener(sensor, &listener);
 * ...
 * sensor_destroy_listener(listener);
 * @endcode
 */
int sensor_destroy_listener(sensor_listener_h listener);


/**
 * @brief   Starts observing the sensor events regarding a given sensor listener.
 * @details If a sensor listener is started, its event callback function starts to be called
 *          whenever the corresponding sensor events occur.@n
 *          For example, #SENSOR_ACCELEROMETER reports its sensor readings repeatedly,
 *          with a specific update interval.
 *          Note that, unlike the accelerometer, sensors like #SENSOR_PROXIMITY emit events
 *          only if their states change.
 * @since_tizen 2.3
 *
 * @remarks The @a listener will be stopped when sensor_listener_stop() is
 *          called or the @a listener is destroied by sensor_destroy_listener
 *          function call.
 *
 * @param[in]   listener  A listener handle
 *
 * @return  #SENSOR_ERROR_NONE on success, otherwise a negative error value
 * @retval  #SENSOR_ERROR_NONE                 Successful
 * @retval  #SENSOR_ERROR_INVALID_PARAMETER    Invalid parameter
 * @retval  #SENSOR_ERROR_OPERATION_FAILED     Operation failed
 *
 * @pre     The @a listener needs to be created in advance, by using sensor_create_listener().
 *          Then the callback function needs to be attached to the @a listener, by using
 *          sensor_listener_set_event_cb().
 * @see     sensor_listener_stop()
 *
 * @par Example
 * @code
 * ...
 * sensor_listener_h listener;
 * sensor_create_listener(sensor, &listener);
 * ...
 * sensor_listener_start(listener);
 * ...
 * sensor_destroy_listener(listener);
 * @endcode
 */
int sensor_listener_start(sensor_listener_h listener);


/**
 * @brief   Stops observing the sensor events regarding a given sensor listener.
 * @details The listener's event callback function stops being called.
 * @since_tizen 2.3
 *
 * @remarks The sensor(which is @a listener was listened to) itself may not be
 *          stopped if there are other listeners that are using the same sensor.
 *
 * @param[in]   listener  A listener handle
 *
 * @return  #SENSOR_ERROR_NONE on success, otherwise a negative error value
 * @retval  #SENSOR_ERROR_NONE                 Successful
 * @retval  #SENSOR_ERROR_INVALID_PARAMETER    Invalid parameter
 * @retval  #SENSOR_ERROR_OPERATION_FAILED     Operation failed
 *
 * @see     sensor_listener_start()
 *
 * @par Example
 * @code
 * ...
 * sensor_listener_h listener;
 * sensor_create_listener(sensor, &listener);
 * ...
 * sensor_listener_start(listener);
 * ...
 * sensor_destroy_listener(listener);
 * ...
 * sensor_listener_stop(listener);
 * @endcode
 */
int sensor_listener_stop(sensor_listener_h listener);


/**
 * @deprecated Deprecated since 5.5. Use sensor_listener_set_events_cb() instead.
 * @brief   Registers the callback function to be invoked when sensor events are delivered via a sensor listener.
 * @since_tizen 2.3
 *
 * @param[in]   listener    A listener handle
 * @param[in]   interval_ms A desired update interval between sensor events in milliseconds.@n
 *                          If 0, it will be automatically set to the default interval of the corresponding sensor.@n
 *                          See sensor_listener_set_interval() for more details.
 * @param[in]   callback    A callback function to attach with the @a listener handle
 * @param[in]   data        A user data to be passed to the callback function
 *
 * @return  #SENSOR_ERROR_NONE on success, otherwise a negative error value
 * @retval  #SENSOR_ERROR_NONE                 Successful
 * @retval  #SENSOR_ERROR_INVALID_PARAMETER    Invalid parameter
 * @retval  #SENSOR_ERROR_OPERATION_FAILED     Operation failed
 *
 * @see sensor_listener_unset_event_cb()
 */
int sensor_listener_set_event_cb(sensor_listener_h listener, unsigned int interval_ms, sensor_event_cb callback, void *data) TIZEN_DEPRECATED_API;


/**
 * @deprecated Deprecated since 5.5. Use sensor_listener_unset_events_cb() instead.
 * @brief   Unregisters the sensor event callback function attached to a given sensor listener.
 * @since_tizen 2.3
 *
 * @param[in]   listener    A listener handle
 *
 * @return  #SENSOR_ERROR_NONE on success, otherwise a negative error value
 * @retval  #SENSOR_ERROR_NONE                 Successful
 * @retval  #SENSOR_ERROR_INVALID_PARAMETER    Invalid parameter
 * @retval  #SENSOR_ERROR_OPERATION_FAILED     Operation failed
 *
 * @see     sensor_listener_set_event_cb()
 */
int sensor_listener_unset_event_cb(sensor_listener_h listener) TIZEN_DEPRECATED_API;


/**
 * @brief   Set the callback to be called when the sensor events occured.
 * @details Set the callback function to be invoked when sensor events are
 *          delivered via a sensor listener.
 * @since_tizen 5.5
 *
 * @param[in]   listener    A listener handle
 * @param[in]   callback    A callback function to attach to the @a listener handle
 * @param[in]   user_data   A user data to be passed to the callback function
 *
 * @return  #SENSOR_ERROR_NONE on success, otherwise a negative error value
 * @retval  #SENSOR_ERROR_NONE                 Successful
 * @retval  #SENSOR_ERROR_INVALID_PARAMETER    Invalid parameter
 * @retval  #SENSOR_ERROR_OPERATION_FAILED     Operation failed
 *
 * @see sensor_listener_unset_events_cb()
 *
 * @par Example
 * @code
 * void accelerator_event_callback(sensor_h sensor, sensor_event_s events[], int events_count, void *user_data)
 * {
 *     ...
 * }
 * ...
 * sensor_listener_set_events_cb(accelerator_listener, accelerator_event_callback, NULL);
 * sensor_listener_unset_events_cb(accelerator_listener);
 * @endcode
 */
int sensor_listener_set_events_cb(sensor_listener_h listener, sensor_events_cb callback, void *user_data);

/**
 * @brief   Unset the callback function from a sensor listener.
 * @details Unset the sensor events callback function attached to a given sensor
 *          listener.
 * @since_tizen 5.5
 *
 * @param[in]   listener    A listener handle
 *
 * @return  #SENSOR_ERROR_NONE on success, otherwise a negative error value
 * @retval  #SENSOR_ERROR_NONE                 Successful
 * @retval  #SENSOR_ERROR_INVALID_PARAMETER    Invalid parameter
 * @retval  #SENSOR_ERROR_OPERATION_FAILED     Operation failed
 *
 * @see     sensor_listener_set_events_cb()
 *
 * @par Example
 * @code
 * void accelerator_event_callback(sensor_h sensor, sensor_event_s events[], int events_count, void *user_data)
 * {
 *     ...
 * }
 * ...
 * sensor_listener_set_events_cb(accelerator_listener, accelerator_event_callback, NULL);
 * sensor_listener_unset_events_cb(accelerator_listener);
 * @endcode
 */
int sensor_listener_unset_events_cb(sensor_listener_h listener);

/**
 * @brief   Registers the callback function to be invoked when the accuracy of a sensor changes.
 * @details In addition to sensor_event_cb(), sensor_accuracy_changed_cb() also can be attached
 *          to sensor listeners. With this accuracy callback function, applications can be notified
 *          the changes of the corresponding sensors separately.
 * @since_tizen 2.3
 *
 * @param[in]   listener    A listener handle
 * @param[in]   callback    A callback function to attach with the @a listener handle
 * @param[in]   data        A user data to be passed to the callback function
 *
 * @return  #SENSOR_ERROR_NONE on success, otherwise a negative error value
 * @retval  #SENSOR_ERROR_NONE                 Successful
 * @retval  #SENSOR_ERROR_INVALID_PARAMETER    Invalid parameter
 * @retval  #SENSOR_ERROR_OPERATION_FAILED     Operation failed
 *
 * @pre     The @a listener needs to be started to get the change callbacks.
 * @see     sensor_listener_unset_accuracy_cb()
 *
 * @par Example
 * @code
 * void accuracy_changed_callback(sensor_h sensor, unsigned long long timestamp, sensor_data_accuracy_e accuracy, void *data)
 * {
 *     ...
 * }
 * ...
 * sensor_listener_set_accuracy_cb(listener, accuracy_changed_callback, NULL);
 * ...
 * sensor_listener_unset_accuracy_cb(listener);
 * @endcode
 */
int sensor_listener_set_accuracy_cb(sensor_listener_h listener, sensor_accuracy_changed_cb callback, void *data);


/**
 * @brief   Unregister the accuracy change callback from a sensor listener.
 * @details Unregister the sensor accuracy change callback function attached to
 *          a given sensor listener.
 * @since_tizen 2.3
 *
 * @param[in]   listener    A listener handle
 *
 * @return  #SENSOR_ERROR_NONE on success, otherwise a negative error value
 * @retval  #SENSOR_ERROR_NONE                 Successful
 * @retval  #SENSOR_ERROR_INVALID_PARAMETER    Invalid parameter
 * @retval  #SENSOR_ERROR_OPERATION_FAILED     Operation failed
 *
 * @see     sensor_listener_set_accuracy_cb()
 *
 * @par Example
 * @code
 * void accuracy_changed_callback(sensor_h sensor, unsigned long long timestamp, sensor_data_accuracy_e accuracy, void *data)
 * {
 *     ...
 * }
 * ...
 * sensor_listener_set_accuracy_cb(listener, accuracy_changed_callback, NULL);
 * ...
 * sensor_listener_unset_accuracy_cb(listener);
 * @endcode
 */
int sensor_listener_unset_accuracy_cb(sensor_listener_h listener);


/**
 * @deprecated Deprecated since 5.5. Use sensor_listener_read_data_list() instead.
 * @brief   Reads the current sensor data via a given sensor listener.
 * @details This function synchronously reads the sensor reading of the corresponding sensor, if available.
 *          Otherwise, if the sensor is not ready to report its values, this function fails and returns
 *          #SENSOR_ERROR_OPERATION_FAILED.
 * @since_tizen 2.3
 *
 * @remarks  As a sensor usually works in an event-driven manner, it may not be able to read its data on demand.
 *          Then this function tries to return the last known values.@n
 *          To be able to read the current values or the last known values, the sensor needs to be
 *          enabled in advance. As an application cannot be sure that the sensor is already enabled
 *          by other applications, it is recommended to start the sensor explicitly by using sensor_listener_start().@n
 *          But note that even if the sensor is started, on-demand reading can fail anyway,
 *          thus it would be better to use the asynchronous callback approach.
 *
 * @param[in]   listener    A listener handle
 * @param[out]  event       The retrieved sensor data
 *
 * @return  #SENSOR_ERROR_NONE on success, otherwise a negative error value
 * @retval  #SENSOR_ERROR_NONE                 Successful
 * @retval  #SENSOR_ERROR_INVALID_PARAMETER    Invalid parameter
 * @retval  #SENSOR_ERROR_OPERATION_FAILED     Operation failed
 */
int sensor_listener_read_data(sensor_listener_h listener, sensor_event_s *event) TIZEN_DEPRECATED_API;

/**
 * @brief   Reads the current sensor data list via a given sensor listener.
 * @details This function synchronously reads the sensor reading of the corresponding sensor, if available.
 *          Otherwise, if the sensor is not ready to report its values, this function fails and returns
 *          #SENSOR_ERROR_OPERATION_FAILED.
 * @since_tizen 5.5
 *
 * @remarks  As a sensor usually works in an event-driven manner, it may not be able to read its data on demand.
 *          In such cases this function tries to return the last known values.@n
 *          When there are last known values that are caused by multi-event publishing, the @a count is greater than or equal to 1.@n
 *          To be able to read the current values or the last known values, the sensor needs to be enabled in advance.
 *          As an application cannot be sure that the sensor is already enabled by other applications,
 *          it is recommended to start the sensor explicitly by using sensor_listener_start().@n
 *          But note that even if the sensor is started, on-demand reading can fail anyway,
 *          thus it would be better to use the asynchronous callback approach.
 *          The @a events must be released using free(), if not being used anymore.@n
 *
 * @param[in]   listener    A listener handle
 * @param[out]  events      The retrieved sensor data list
 * @param[out]  count       The number of event contained in @a events
 *
 * @return  #SENSOR_ERROR_NONE on success, otherwise a negative error value
 * @retval  #SENSOR_ERROR_NONE                 Successful
 * @retval  #SENSOR_ERROR_INVALID_PARAMETER    Invalid parameter
 * @retval  #SENSOR_ERROR_OPERATION_FAILED     Operation failed
 *
 * @par Example
 * @code
 * ...
 * sensor_event_s *events = NULL;
 * int events_count = 0;
 * sensor_listener_read_data_list(listener, &events, &events_count);
 * @endcode
 */
int sensor_listener_read_data_list(sensor_listener_h listener, sensor_event_s **events, int *count);

/**
 * @brief   Change the interval between updates for a given sensor.
 * @details The specified interval is only a suggested interval between sensor measurements.
 *          You will get at least one sensor measurement within the interval you specify,
 *          but the actual interval between sensor measurements can be affected by other applications and the system.
 *          To reduce the system overhead, it is recommended to set the longest interval that you can,
 *          because the system usually chooses the shortest interval among all intervals specified.
 * @since_tizen 2.3
 *
 * @remarks Normally, a sensor's default update interval is 100 ms,
 *          and you can use the default interval by setting the interval to 0.
 *          However, please note that, the default interval varies with the sensor and the device.@n
 *          In addition, a sensor has the lower and the upper bound of its update interval,
 *          usually 10 and 1000 ms respectively.
 *          These lower and upper bounds also can vary with the sensor and the device,
 *          any invalid input values exceeding the bounds will be implicitly adjusted into the valid range.
 *
 * @param[in]   listener    A listener handle
 * @param[in]   interval_ms A desired update interval between sensor events in milliseconds.
 *                          If 0, it will be automatically set to the default interval of the corresponding sensor.
 *
 * @return  #SENSOR_ERROR_NONE on success, otherwise a negative error value
 * @retval  #SENSOR_ERROR_NONE                 Successful
 * @retval  #SENSOR_ERROR_INVALID_PARAMETER    Invalid parameter
 * @retval  #SENSOR_ERROR_OPERATION_FAILED     Operation failed
 *
 * @see     sensor_get_min_interval()
 *
 * @par Example
 * @code
 * ...
 * int interval = 1000;
 * sensor_listener_set_interval(listener, interval);
 * @endcode
 */
int sensor_listener_set_interval(sensor_listener_h listener, unsigned int interval_ms);


/**
 * @brief   Sets the desired maximum batch latency of a sensor.
 * @details Sensors that support batching may allow applications to change their maximum batch latencies.
 *          For example, if you set the latency as 10,000 ms, the sensor may store its data
 *          up to 10,000 ms, before delivering the data through the HAL.@n
 *          In case of non-batching sensors, this function returns #SENSOR_ERROR_NONE,
 *          but nothing is affected by the input latency value.
 * @since_tizen 2.3
 *
 * @remarks Even if you set a batch latency, the sensor may not work as you intended,
 *          as one sensor can be used by more than one listeners.
 *          In addition, some batch sensors may already have fixed batching latency
 *          or batching queue size, which cannot be altered by applications.
 *
 * @param[in]   listener           A listener handle
 * @param[in]   max_batch_latency  A desired batch latency in milliseconds
 *
 * @return  #SENSOR_ERROR_NONE on success, otherwise a negative error value
 * @retval  #SENSOR_ERROR_NONE                 Successful
 * @retval  #SENSOR_ERROR_INVALID_PARAMETER    Invalid parameter
 *
 * @par Example
 * @code
 * ...
 * int max_batch_latency = 10;
 * sensor_listener_set_max_batch_latency(listener, max_batch_latency);
 * @endcode
 */
int sensor_listener_set_max_batch_latency(sensor_listener_h listener, unsigned int max_batch_latency);


/**
 * @brief   Sets an attribute to control the behavior of a sensor listener.
 * @details Applications can change the behavior of a sensor listener, for example,
 *          what is the reference coordinate of the sensor values,
 *          and when the system is allowed to turn off the sensor implicitly to reduce the power consumption.
 *          See #sensor_attribute_e for more details about the available control parameters.
 * @since_tizen 3.0
 *
 * @param[in]   listener	A listener handle
 * @param[in]   attribute	An attribute to change
 * @param[in]   value		An attribute value
 *
 * @return  #SENSOR_ERROR_NONE on success, otherwise a negative error value
 * @retval  #SENSOR_ERROR_NONE                 Successful
 * @retval  #SENSOR_ERROR_INVALID_PARAMETER    Invalid parameter
 *
 * @par Example
 * @code
 * ...
 * sensor_listener_set_attribute_int(listener,
 *         SENSOR_ATTRIBUTE_PAUSE_POLICY,
 *         SENSOR_PAUSE_ON_DISPLAY_OFF);
 * @endcode
 */
int sensor_listener_set_attribute_int(sensor_listener_h listener, sensor_attribute_e attribute, int value);


/**
 * @brief   Change the power-saving behavior of a sensor listener.
 * @details Change the pause policy of a sensor listener, refer the values of
 *          sensor_option_e enum type for the available options.
 * @since_tizen 2.3
 *
 * @remarks  sensor_listener_set_attribute_int() with #SENSOR_ATTRIBUTE_PAUSE_POLICY replaces this function.
 *
 * @param[in]   listener        A listener handle
 * @param[in]   option          A sensor option
 *
 * @return  #SENSOR_ERROR_NONE on success, otherwise a negative error value
 * @retval  #SENSOR_ERROR_NONE                 Successful
 * @retval  #SENSOR_ERROR_INVALID_PARAMETER    Invalid parameter
 * @retval  #SENSOR_ERROR_OPERATION_FAILED     Operation failed
 *
 * @par Example
 * @code
 * ...
 * sensor_listener_set_option(listener, SENSOR_PAUSE_ON_DISPLAY_OFF);
 * @endcode
 */
int sensor_listener_set_option(sensor_listener_h listener, sensor_option_e option);

/**
 * @brief    Flush the stored sensor data for the given listener.
 * @details  Flush all data of a sensor as if the max batch latency has expired.
 *          Data is delivered in the usual way through the event callback function. @n
 *          In case of non-batching sensors, this function does nothing and returns #SENSOR_ERROR_NONE.
 * @since_tizen 5.5
 *
 * @param[in]   listener        A listener handle
 * @return  #SENSOR_ERROR_NONE on success, otherwise a negative error value
 * @retval  #SENSOR_ERROR_NONE                 Successful
 * @retval  #SENSOR_ERROR_INVALID_PARAMETER    Invalid parameter
 * @retval  #SENSOR_ERROR_OPERATION_FAILED     Operation failed
 *
 * @par Example
 * @code
 * ...
 * sensor_listener_flush(listener);
 * @endcode
 */
int sensor_listener_flush(sensor_listener_h listener);

/**
 * @}
 */

/**
 * @addtogroup CAPI_SYSTEM_SENSOR_PROVIDER_MODULE
 * @{
 */

/**
 * @brief   Sensor provider handle.
 * @details One or more sensor providers can be created by using sensor_create_provider().
 *          The sensor's data can be published to listeners, via the provider.
 * @since_tizen 4.0
 *
 * @see  sensor_create_provider()
 * @see  sensor_destroy_provider()
 */
typedef struct _sensor_provider_s *sensor_provider_h;

/**
 * @brief   Create a sensor provider for a given valid URI string.
 * @details This function creates a sensor provider handle with a given URI.
 *          The URI should be in the valid form described in \ref CAPI_SYSTEM_SENSOR_LISTENER_MODULE_URI.
 *          In addition, it is not allowed to set the vendor in the URI to @"tizen.org@",
 *          as it is pre-assigned to the platform-defined sensors.\n
 * @since_tizen 4.0
 *
 * @remarks If your application needs to publish health-related sensor data,
 *          the URI category should be @"healthinfo@".
 *          Then the applications without the privilege %http://tizen.org/privilege/healthinfo
 *          are not allowed to listen your application's health-related sensor.
 *
 * @remarks The @a provider must be released using sensor_destroy_provider().
 *
 * @param[in]  uri             The URI of sensor to be created
 * @param[out] provider        The sensor provider handle
 *
 * @return  #SENSOR_ERROR_NONE on success, otherwise a negative error value
 * @retval  #SENSOR_ERROR_NONE                 Successful
 * @retval  #SENSOR_ERROR_INVALID_PARAMETER    Invalid parameter
 * @retval  #SENSOR_ERROR_OUT_OF_MEMORY        Out of memory
 * @retval  #SENSOR_ERROR_OPERATION_FAILED     Operation failed
 *
 * @see     sensor_destroy_provider()
 *
 * @par Example
 * @code
 * const char *provider_uri = "http://appsensor/sensor/general/providersensor/heartbeat";
 * sensor_provider_h sensor_provider;
 * sensor_create_provider(provider_uri, &sensor_provider);
 * @endcode
 */
int sensor_create_provider(const char *uri, sensor_provider_h *provider);

/**
 * @brief    Register the sensor provider created by sensor_create_provider().
 * @details  A sensor provider is created via sensor_create_provider(),
 *           and the three callback functions sensor_provider_start_cb(), sensor_provider_stop_cb(),
 *           and sensor_provider_set_interval_changed_cb() should be set in advance.
 * @since_tizen 4.0
 *
 * @param[in]   provider  The sensor provider handle
 *
 * @return  #SENSOR_ERROR_NONE on success, otherwise a negative error value
 * @retval  #SENSOR_ERROR_NONE                 Successful
 * @retval  #SENSOR_ERROR_INVALID_PARAMETER    Invalid parameter
 * @retval  #SENSOR_ERROR_PERMISSION_DENIED    Permission denied
 * @retval  #SENSOR_ERROR_OPERATION_FAILED     Operation failed
 *
 * @see     sensor_remove_provider()
 *
 * @par Example
 * @code
 * const char *provider_uri = "http://appsensor/sensor/general/providersensor/heartbeat";
 * sensor_provider_h sensor_provider;
 * sensor_create_provider(provider_uri, &sensor_provider);
 * ...
 * sensor_add_provider(sensor_provider);
 * @endcode
 */
int sensor_add_provider(sensor_provider_h provider);

/**
 * @brief   Unregisters the given sensor provider registered by sensor_add_provider().
 * @details A sensor provider that registered by sensor_add_provider() function
 *          call previously, can be unregistered by sensor_removed_provider().
 * @since_tizen 4.0
 *
 * @param[in]  provider  The sensor provider handle
 *
 * @return  #SENSOR_ERROR_NONE on success, otherwise a negative error value
 * @retval  #SENSOR_ERROR_NONE                 Successful
 * @retval  #SENSOR_ERROR_INVALID_PARAMETER    Invalid parameter
 * @retval  #SENSOR_ERROR_OPERATION_FAILED     Operation failed
 *
 * @see     sensor_add_provider()
 *
 * @par Example
 * @code
 * const char *provider_uri = "http://appsensor/sensor/general/providersensor/heartbeat";
 * sensor_provider_h sensor_provider;
 * sensor_create_provider(provider_uri, &sensor_provider);
 * ...
 * sensor_add_provider(sensor_provider);
 * ...
 * sensor_remove_provider(sensor_provider);
 * @endcode
 */
int sensor_remove_provider(sensor_provider_h provider);

/**
 * @brief   Destroy resources of the given sensor provider.
 * @details Release all the resources allocated for the sensor provider by the
 *          function sensor_create_provider().
 * @since_tizen 4.0
 *
 * @remarks If this function is called before sensor_remove_provider() is called,
 *          then the sensor provider will be implicitly unregistered.
 *
 * @param[in]  provider  The sensor provider handle
 *
 * @return  #SENSOR_ERROR_NONE on success, otherwise a negative error value
 * @retval  #SENSOR_ERROR_NONE                 Successful
 * @retval  #SENSOR_ERROR_INVALID_PARAMETER    Invalid parameter
 *
 * @see     sensor_create_provider()
 *
 * @par Example
 * @code
 * const char *provider_uri = "http://appsensor/sensor/general/providersensor/heartbeat";
 * sensor_provider_h sensor_provider;
 * sensor_create_provider(provider_uri, &sensor_provider);
 * ...
 * sensor_add_provider(sensor_provider);
 * ...
 * sensor_remove_provider(sensor_provider);
 * sensor_destroy_provider(sensor_provider);
 * @endcode
 */
int sensor_destroy_provider(sensor_provider_h provider);

/**
 * @brief   Set the name of the given sensor provider as @a name.
 * @details The name of the given sensor provider will be @a name if success.
 * @since_tizen 4.0
 *
 * @param[in]  provider    The sensor provider handle
 * @param[in]  name        The name of the sensor
 *
 * @return  #SENSOR_ERROR_NONE on success, otherwise a negative error value
 * @retval  #SENSOR_ERROR_NONE                 Successful
 * @retval  #SENSOR_ERROR_INVALID_PARAMETER    Invalid parameter
 *
 * @pre     sensor_create_provider()
 * @see     sensor_provider_set_vendor()
 *          sensor_provider_set_range()
 *          sensor_provider_set_resolution()
 *
 * @par Example
 * @code
 * const char *provider_uri = "http://appsensor/sensor/general/providersensor/heartbeat";
 * sensor_provider_h sensor_provider;
 * sensor_create_provider(provider_uri, &sensor_provider);
 * ...
 * sensor_provider_set_name(sensor_provider, "heartbeat");
 * @endcode
 */
int sensor_provider_set_name(sensor_provider_h provider, const char *name);

/**
 * @brief   Set the vendor name of the given sensor provider as @a vendor.
 * @details The vendor name of the given sensor provider will be @a vendor if
 *          success.
 * @since_tizen 4.0
 *
 * @param[in]  provider    The sensor provider handle
 * @param[in]  vendor      The vendor of the sensor
 *
 * @return  #SENSOR_ERROR_NONE on success, otherwise a negative error value
 * @retval  #SENSOR_ERROR_NONE                 Successful
 * @retval  #SENSOR_ERROR_INVALID_PARAMETER    Invalid parameter
 *
 * @pre     sensor_create_provider()
 * @see     sensor_provider_set_name()
 *          sensor_provider_set_range()
 *          sensor_provider_set_resolution()
 *
 * @par Example
 * @code
 * const char *provider_uri = "http://appsensor/sensor/general/providersensor/heartbeat";
 * sensor_provider_h sensor_provider;
 * sensor_create_provider(provider_uri, &sensor_provider);
 * ...
 * sensor_provider_set_vendor(sensor_provider, "appsensor");
 * @endcode
 */
int sensor_provider_set_vendor(sensor_provider_h provider, const char *vendor);

/**
 * @brief   Set the range of sensor values to the sensor provider.
 * @details Set the range of possible sensor values to the sensor provider
 *          handle.
 * @since_tizen 4.0
 *
 * @remarks    If the application does not set the range, the default values are 0 and 1.
 *
 * @param[in]  provider    The sensor provider handle
 * @param[in]  min_range   The lower bound
 * @param[in]  max_range   The upper bound
 *
 * @return  #SENSOR_ERROR_NONE on success, otherwise a negative error value
 * @retval  #SENSOR_ERROR_NONE                 Successful
 * @retval  #SENSOR_ERROR_INVALID_PARAMETER    Invalid parameter
 *
 * @pre     sensor_create_provider()
 * @see     sensor_provider_set_name()
 *          sensor_provider_set_vendor()
 *          sensor_provider_set_resolution()
 *
 * @par Example
 * @code
 * const char *provider_uri = "http://appsensor/sensor/general/providersensor/heartbeat";
 * sensor_provider_h sensor_provider;
 * sensor_create_provider(provider_uri, &sensor_provider);
 * ...
 * sensor_provider_set_range(sensor_provider, 0.0f, 1.0f);
 * @endcode
 */
int sensor_provider_set_range(sensor_provider_h provider, float min_range, float max_range);

/**
 * @brief   Set the resolution of sensor values of the sensor provider.
 * @details Set the resolution of sensor values as a float value of the given
 *          sensor provider.
 * @since_tizen 4.0
 *
 * @remarks    If the application does not set the resolution, the default value is 1.
 *
 * @param[in]  provider    The sensor provider handle
 * @param[in]  resolution  The resolution
 *
 * @return  #SENSOR_ERROR_NONE on success, otherwise a negative error value
 * @retval  #SENSOR_ERROR_NONE                 Successful
 * @retval  #SENSOR_ERROR_INVALID_PARAMETER    Invalid parameter
 *
 * @pre     sensor_create_provider()
 * @see     sensor_provider_set_name()
 *          sensor_provider_set_vendor()
 *          sensor_provider_set_range()
 *
 * @par Example
 * @code
 * const char *provider_uri = "http://appsensor/sensor/general/providersensor/heartbeat";
 * sensor_provider_h sensor_provider;
 * sensor_create_provider(provider_uri, &sensor_provider);
 * ...
 * sensor_provider_set_resolution(sensor_provider, 0.1f);
 * @endcode
 */
int sensor_provider_set_resolution(sensor_provider_h provider, float resolution);


/**
 * @brief   Callback function type to be called when a sensor listener starts
 *          the sensor provider.
 * @details Will be called when a sensor listener starts the sensor provider.
 *          Callback function can be added using function
 *          sensor_provider_set_start_cb().
 * @since_tizen 4.0
 *
 * @remarks @a provider is the object created with sensor_create_provider()
 *          and is the object for which the callback was triggered.
 *
 * @param[in]  provider    The sensor provider handle
 * @param[in]  user_data   The user data to be passed to the callback function
 *
 * @pre     A callback function needs to be set using sensor_provider_set_start_cb().
 * @see     sensor_provider_set_start_cb()
 *
 * @par Example
 * @code
 * void sensor_provider_start_callback(sensor_provider_h provider, void *user_data)
 * {
 *     ...
 * }
 * ...
 *     sensor_provider_set_start_cb(provider, sensor_provider_start_callback, NULL);
 * @endcode
 */
typedef void (*sensor_provider_start_cb)(sensor_provider_h provider, void *user_data);

/**
 * @brief   Register the callback to be called when a listener starts the sensor
 *          provider.
 * @details Register the callback function(sensor_provider_start_cb) to be
 *          invoked when a listener starts the sensor provider.
 * @since_tizen 4.0
 *
 * @param[in]  provider    The provider handle
 * @param[in]  callback    The callback function to attach to the @a provider handle
 * @param[in]  user_data   The user data to be passed to the callback function
 *
 * @return  #SENSOR_ERROR_NONE on success, otherwise a negative error value
 * @retval  #SENSOR_ERROR_NONE                 Successful
 * @retval  #SENSOR_ERROR_INVALID_PARAMETER    Invalid parameter
 * @retval  #SENSOR_ERROR_OPERATION_FAILED     Operation failed
 *
 * @par Example
 * @code
 * void sensor_provider_start_callback(sensor_provider_h provider, void *user_data)
 * {
 *     ...
 * }
 * ...
 *     sensor_provider_set_start_cb(provider, sensor_provider_start_callback, NULL);
 * @endcode
 */
int sensor_provider_set_start_cb(sensor_provider_h provider,
                                 sensor_provider_start_cb callback, void *user_data);


/**
 * @brief    Callback function type to be called when a sensor listener stops
 *           the sensor provider.
 * @details  Will be called when a sensor listener stops the sensor provider.
 *           Callback function can be added using function
 *           sensor_provider_set_stop_cb().
 * @since_tizen 4.0
 *
 * @remarks @a provider is the object created with sensor_create_provider()
 *          and is the object for which the callback was triggered.
 *
 * @param[in]  provider     The sensor provider handle
 * @param[in]  user_data    The user data to be passed to the callback function
 *
 * @pre    A callback function needs to be set using sensor_provider_set_stop_cb().
 * @see    sensor_provider_set_stop_cb()
 *
 * @par Example
 * @code
 * void sensor_provider_stop_callback(sensor_provider_h provider, void *user_data)
 * {
 *     ...
 * }
 * ...
 *     sensor_provider_set_stop_cb(provider, sensor_provider_start_callback, NULL);
 * @endcode
 */
typedef void (*sensor_provider_stop_cb)(sensor_provider_h provider, void *user_data);

/**
 * @brief   Register the callback to be called when a sensor listener stops the
 *          sensor provider.
 * @details Register the callback function(sensor_provider_stop_cb) to be
 *          invoked when a sensor listener stops the sensor provider.
 * @since_tizen 4.0
 *
 * @param[in]  provider    The sensor provider handle
 * @param[in]  callback    The callback function to attach to the @a provider handle
 * @param[in]  user_data   The user data to be passed to the callback function
 *
 * @return  #SENSOR_ERROR_NONE on success, otherwise a negative error value
 * @retval  #SENSOR_ERROR_NONE                 Successful
 * @retval  #SENSOR_ERROR_INVALID_PARAMETER    Invalid parameter
 * @retval  #SENSOR_ERROR_OPERATION_FAILED     Operation failed
 *
 * @par Example
 * @code
 * void sensor_provider_stop_callback(sensor_provider_h provider, void *user_data)
 * {
 *     ...
 * }
 * ...
 *     sensor_provider_set_stop_cb(provider, sensor_provider_start_callback, NULL);
 * @endcode
 */
int sensor_provider_set_stop_cb(sensor_provider_h provider,
                                sensor_provider_stop_cb callback, void *user_data);


/**
 * @brief    Callback function type to be called when the interval of the sensor
 *           provider is changed.
 * @details  Will be called when the interval of the sensor provider is changed.
 *           Callback function can be added using function
 *           sensor_provider_set_interval_changed_cb().
 * @since_tizen 4.0
 *
 * @remarks @a provider is the object created with sensor_create_provider()
 *          and is the object for which the callback was triggered.
 *
 * @param[in]  provider    The sensor provider handle
 * @param[in]  interval_ms The interval
 * @param[in]  user_data   The user data to be passed to the callback function
 *
 * @pre    A callback function needs to be set using sensor_provider_set_interval_changed_cb().
 * @see    sensor_provider_set_interval_changed_cb()
 *
 * @par Example
 * @code
 * void sensor_provider_interval_change_callback(sensor_provider_h provider, void *user_data)
 * {
 *     ...
 * }
 * ...
 *     sensor_provider_set_interval_changed_cb(provider, sensor_provider_interval_change_callback, NULL);
 * @endcode
 */
typedef void (*sensor_provider_interval_changed_cb)(sensor_provider_h provider,
                                            unsigned int interval_ms, void *user_data);

/**
 * @brief   Register the callback to be invoked when the interval is changed.
 * @details Register the callback function(sensor_provider_interval_changed_cb)
 *          to be invoked when the interval is changed.
 * @since_tizen 4.0
 *
 * @param[in]  provider    The sensor provider handle
 * @param[in]  callback    The callback function to attach to the @a provider handle
 * @param[in]  user_data   The user data to be passed to the callback function
 *
 * @return  #SENSOR_ERROR_NONE on success, otherwise a negative error value
 * @retval  #SENSOR_ERROR_NONE                 Successful
 * @retval  #SENSOR_ERROR_INVALID_PARAMETER    Invalid parameter
 * @retval  #SENSOR_ERROR_OPERATION_FAILED     Operation failed
 *
 * @par Example
 * @code
 * void sensor_provider_interval_change_callback(sensor_provider_h provider, void *user_data)
 * {
 *     ...
 * }
 * ...
 *     sensor_provider_set_interval_changed_cb(provider, sensor_provider_interval_change_callback, NULL);
 * @endcode
 */
int sensor_provider_set_interval_changed_cb(sensor_provider_h provider,
                                    sensor_provider_interval_changed_cb callback, void *user_data);


/**
 * @deprecated Deprecated since 5.5. Use sensor_provider_publish_events() instead.
 * @brief   Publishes a sensor event through the declared sensor.
 * @details This function publishes a sensor's data to its listeners.
 * @since_tizen 4.0
 *
 * @param[in]   provider    The sensor provider handle
 * @param[in]   event       The sensor event
 *
 * @return  #SENSOR_ERROR_NONE on success, otherwise a negative error value
 * @retval  #SENSOR_ERROR_NONE                 Successful
 * @retval  #SENSOR_ERROR_INVALID_PARAMETER    Invalid parameter
 * @retval  #SENSOR_ERROR_OPERATION_FAILED     Operation failed
 */
int sensor_provider_publish(sensor_provider_h provider, sensor_event_s event) TIZEN_DEPRECATED_API;

/**
 * @brief   Publishes sensor events through the declared sensor.
 * @details This function publishes a sensor's data to its listeners.
 * @since_tizen 5.5
 *
 * @remarks Both callbacks sensor_event_cb() (set with sensor_listener_set_event_cb())
 *          and sensor_events_cb() (set with sensor_listener_set_events_cb()) are available.
 *          Only one of the callbacks can be used at any given time.
 *          A call to sensor_listener_set_event_cb() or sensor_listener_set_events_cb() sets the new callback
 *          in place of the callback previously set by any of the callback setters.
 *          If sensor_events_cb() is set and a single event is published with sensor_provider_publish(),
 *          then the event will be delivered through this callback with events_count set to 1.
 *          If sensor_event_cb() is set and multiple events are published with sensor_provider_publish_events(),
 *          then only the first element of @a events will be delivered. This behavior is not guaranteed,
 *          it's strongly recommended not to use sensor_provider_publish_events() with sensor_event_cb().
 *
 * @param[in]   provider    The sensor provider handle
 * @param[in]   events      The sensor events
 * @param[in]   count       The number of events
 *
 * @return  #SENSOR_ERROR_NONE on success, otherwise a negative error value
 * @retval  #SENSOR_ERROR_NONE                 Successful
 * @retval  #SENSOR_ERROR_INVALID_PARAMETER    Invalid parameter
 * @retval  #SENSOR_ERROR_OPERATION_FAILED     Operation failed
 *
 * @par Example
 * @code
 * ...
 * sensor_event_s events[10];
 * ...
 * for (int i = 0; i < 10; ++i) {
 *     events[i].accuracy = 3;
 *     events[i].timestamp = timestamp;
 *     events[i].value_count = 3;
 *     events[i].values[0] = i;
 *     events[i].values[1] = i + 2;
 *     events[i].values[2] = i + 4;
 * }
 * ...
 * sensor_provider_publish_events(provider, events, 10);
 * @endcode
 */
int sensor_provider_publish_events(sensor_provider_h provider, sensor_event_s events[], int count);

/**
 * @}
 */

/**
 * @addtogroup CAPI_SYSTEM_SENSOR_RECORDER_MODULE
 * @{
 */

/**
 * @brief   Option handle to contain recording policies and parameters.
 * @details one or more sensor options can be created by using sensor_recorder_create_option().
 * @since_tizen 3.0
 */
typedef void *sensor_recorder_option_h;

/**
 * @brief   Query handle to contain filtering and aggregation parameters for recorded data.
 * @details one or more sensor options can be created by using sensor_recorder_create_query().
 * @since_tizen 3.0
 */
typedef void *sensor_recorder_query_h;

/**
 * @brief   Data handle to contain retrieved sensor records.
 * @since_tizen 3.0
 */
typedef void *sensor_recorder_data_h;

/**
 * @brief   Enumeration for option parameters for sensor recording.
 * @details None, one, or more option parameters can be set to #sensor_recorder_option_h,
 *          then applications can request to record a specific sensor with the parameters via
 *          sensor_recorder_start().
 *          If a parameter is not supported for the specified sensor type, it will be ignored.
 * @since_tizen 3.0
 */
typedef enum
{
	SENSOR_RECORDER_OPTION_RETENTION_PERIOD,      /**< Desired retention period for the recorded sensor data (hours); int; */
	SENSOR_RECORDER_OPTION_INTERVAL,              /**< Desired interval between data records (minutes); int; see #sensor_recorder_interval_e */
} sensor_recorder_option_e;

/**
 * @brief   Enumeration for intervals between data records.
 * @details Some sensor types including #SENSOR_HRM may not fit to continuous monitoring and recording.
 *          To reduce the battery use for recording such sensors, recording interval between each
 *          sensor data is chosen. For example, an application set the interval of 1 hour,
 *          the platform tries to retrieve the sensor data only once per hour, thus it will be more
 *          battery efficient than to listen the sensor data continuously.@n
 *          If the application does not set the interval explicitly, the default value is chosen.
 *          The default values are differ from sensor to sensor.
 * @since_tizen 3.0
 * @remarks If more than one applications set different intervals, the shortest value is chosen.
 */
typedef enum
{
	SENSOR_RECORDER_INTERVAL_10_MINUTES = 10,   /**< 10 Minutes */
	SENSOR_RECORDER_INTERVAL_1_HOUR = 60,       /**< 1 Hour */
	SENSOR_RECORDER_INTERVAL_3_HOURS = 180,     /**< 3 Hours */
	SENSOR_RECORDER_INTERVAL_6_HOURS = 360,     /**< 6 Hours */
	SENSOR_RECORDER_INTERVAL_12_HOURS = 720,    /**< 12 Hours */
	SENSOR_RECORDER_INTERVAL_1_DAY = 1440,      /**< 1 Day */
} sensor_recorder_interval_e;

/**
 * @brief   Enumeration for filtering and aggregation parameters for querying sensor records.
 * @details None, one, or more query parameters can be set to #sensor_recorder_query_h,
 *          to specify the data to be retrieved via sensor_recorder_read().
 *          If a necessary parameter is not set, the default value is chosen.
 * @since_tizen 3.0
 */
typedef enum
{
	SENSOR_RECORDER_QUERY_START_TIME = 0,   /**< Start time of the data to be queried (Epoch); time_t; If unspecified, 1 day ago */
	SENSOR_RECORDER_QUERY_END_TIME,         /**< End time of the data to be queried (Epoch); time_t; If unspecified, the current time */
	SENSOR_RECORDER_QUERY_ANCHOR_TIME,      /**< The anchor time to slice the querying duration of time (Epoch); time_t;
	                                             It needs to be set with #SENSOR_RECORDER_QUERY_TIME_INTERVAL, otherwise, it is ignored */
	SENSOR_RECORDER_QUERY_TIME_INTERVAL,    /**< The interval of each sliced querying duration (minutes); int */
} sensor_recorder_query_e;

/**
 * @brief   Enumeration for data attributes can be contained in #sensor_recorder_data_h.
 * @since_tizen 3.0
 * @see     sensor_recorder_data_get_int()
 * @see     sensor_recorder_data_get_double()
 */
typedef enum
{
	SENSOR_RECORDER_DATA_STEPS = 0x00,           /**< Count of both walking and running steps; int (Since 3.0) */
	SENSOR_RECORDER_DATA_WALK_STEPS,             /**< Count of walking steps; int (Since 3.0) */
	SENSOR_RECORDER_DATA_RUN_STEPS,              /**< Count of running steps; int (Since 3.0) */
	SENSOR_RECORDER_DATA_DISTANCE,               /**< Distance walked or ran (m); double (Since 3.0) */
	SENSOR_RECORDER_DATA_CALORIE,                /**< Calorie burned (kcal); double (Since 3.0) */
	SENSOR_RECORDER_DATA_HEART_RATE = 0x10,      /**< Heart Rate (BPM); int (Since 3.0) */
	SENSOR_RECORDER_DATA_SLEEP_STATE = 0x20,     /**< Sleep state; int; One of #sensor_sleep_state_e (Since 3.0) */
	SENSOR_RECORDER_DATA_PRESSURE = 0x30,        /**< Pressure; double */
	SENSOR_RECORDER_DATA_MAX_PRESSURE,           /**< Max pressure; double */
	SENSOR_RECORDER_DATA_MIN_PRESSURE,           /**< Min pressure; double */
	SENSOR_RECORDER_DATA_AVERAGE_PRESSURE,       /**< Average pressure; double */
} sensor_recorder_data_e;

/**
 * @brief   Checks whether it is supported to record a given sensor type.
 * @details Check if it is supported to record for a given sensor type and
 *          stores it to @a supported.
 * @since_tizen 3.0
 *
 * @param[in]   type        A sensor type to check
 * @param[out]  supported   If supported, @c true; Otherwise @c false
 *
 * @return  #SENSOR_ERROR_NONE on success; Otherwise a negative error value
 * @retval  #SENSOR_ERROR_NONE                 Successful
 * @retval  #SENSOR_ERROR_INVALID_PARAMETER    Invalid parameter
 * @retval  #SENSOR_ERROR_OPERATION_FAILED     Operation failed
 *
 * @par Example
 * @code
 * bool is_supported = false;
 * sensor_recorder_is_supported(SENSOR_ACCELEROMETER, &is_supported);
 * @endcode
 */
int sensor_recorder_is_supported(sensor_type_e type, bool *supported);

/**
 * @brief   Start to record for a given sensor type with specified option.
 * @details Start to record sensor for a given sensor type with specified
 *          option. Some sensor types are privileged, check remarks.
 * @since_tizen 3.0
 *
 * @remarks Some sensor types are privileged. An application should have the privilege
 *          %http://tizen.org/privilege/healthinfo to get access to the following sensors:
 *          #SENSOR_HRM, #SENSOR_HRM_LED_GREEN, #SENSOR_HRM_LED_IR, #SENSOR_HRM_LED_RED,
 *          #SENSOR_HUMAN_PEDOMETER, #SENSOR_HUMAN_SLEEP_MONITOR, #SENSOR_HUMAN_SLEEP_DETECTOR,
 *          and #SENSOR_HUMAN_STRESS_MONITOR.
 *
 * @param[in]  type    A sensor type to be recorded
 * @param[in]  option  Option
 *
 * @return  #SENSOR_ERROR_NONE on success, otherwise a negative error value
 * @retval  #SENSOR_ERROR_NONE                 Successful
 * @retval  #SENSOR_ERROR_INVALID_PARAMETER    Invalid parameter
 * @retval  #SENSOR_ERROR_NOT_SUPPORTED        Not supported to record the sensor type
 * @retval  #SENSOR_ERROR_PERMISSION_DENIED    Permission denied
 * @retval  #SENSOR_ERROR_OPERATION_FAILED     Operation failed
 * @retval  #SENSOR_ERROR_NOT_AVAILABLE        The sensor is already being recorded by the request of the current application
 *
 * @see sensor_recorder_stop()
 *
 * @par Example
 * @code
 * sensor_record_start(SENSOR_ACCELEROMETER, NULL);
 * @endcode
 */
int sensor_recorder_start(sensor_type_e type, sensor_recorder_option_h option);

/**
 * @brief   Stop to record sensor which is started to record for a given sensor
 *          type.
 * @details Stop the recording for a given sensor type, which is started by
 *          sensor_recorder_start().
 * @since_tizen 3.0
 *
 * @param[in]  type    A sensor type being recorded
 *
 * @return  #SENSOR_ERROR_NONE on success, otherwise a negative error value
 * @retval  #SENSOR_ERROR_NONE                 Successful
 * @retval  #SENSOR_ERROR_INVALID_PARAMETER    Invalid parameter
 * @retval  #SENSOR_ERROR_NOT_SUPPORTED        The sensor type is not supported
 * @retval  #SENSOR_ERROR_OPERATION_FAILED     Operation failed
 *
 * @see sensor_recorder_start()
 *
 * @par Example
 * @code
 * sensor_record_start(SENSOR_ACCELEROMETER, NULL);
 * ...
 * sensor_recorder_stop(SENSOR_ACCELEROMETER);
 * @endcode
 */
int sensor_recorder_stop(sensor_type_e type);

/**
 * @brief   Create a recorder option handle for sensor recorder.
 * @details Allocate a recorder option handle and store it to @a option.
 * @since_tizen 3.0
 *
 * @remarks The @a option must be released using sensor_recorder_destroy_option().
 *
 * @param[out]  option  Option handle
 *
 * @return  #SENSOR_ERROR_NONE on success, otherwise a negative error value
 * @retval  #SENSOR_ERROR_NONE                 Successful
 * @retval  #SENSOR_ERROR_INVALID_PARAMETER    Invalid parameter
 * @retval  #SENSOR_ERROR_OUT_OF_MEMORY        Out of memory
 * @retval  #SENSOR_ERROR_OPERATION_FAILED     Operation failed
 *
 * @par Example
 * @code
 * sensor_recorder_option_h option;
 * sensor_recorder_create_option(&option);
 * @endcode
 */
int sensor_recorder_create_option(sensor_recorder_option_h *option);

/**
 * @brief   Destroy a recorder option handle for sensor recorder.
 * @details Destroy a recorder option which is stored in @a option.
 * @since_tizen 3.0
 *
 * @param[in]  option  Option handle
 *
 * @return  #SENSOR_ERROR_NONE on success, otherwise a negative error value
 * @retval  #SENSOR_ERROR_NONE                 Successful
 * @retval  #SENSOR_ERROR_INVALID_PARAMETER    Invalid parameter
 * @retval  #SENSOR_ERROR_OPERATION_FAILED     Operation failed
 *
 * @par Example
 * @code
 * sensor_recorder_option_h option;
 * sensor_recorder_create_option(&option);
 * ...
 * sensor_recorder_destroy_option(option);
 * @endcode
 */
int sensor_recorder_destroy_option(sensor_recorder_option_h option);

/**
 * @brief   Set a recording option parameter to a given option handle @a option.
 * @details Set @a option as a recording option parameter @a param as @a value.
 * @since_tizen 3.0
 *
 * @param[in]  option      Option handle
 * @param[in]  param       Option parameter
 * @param[in]  value       Value
 *
 * @return  #SENSOR_ERROR_NONE on success, otherwise a negative error value
 * @retval  #SENSOR_ERROR_NONE                 Successful
 * @retval  #SENSOR_ERROR_INVALID_PARAMETER    Invalid parameter
 * @retval  #SENSOR_ERROR_OPERATION_FAILED     Operation failed
 *
 * @par Example
 * @code
 * sensor_recorder_option_h option;
 * sensor_recorder_create_option(&option);
 * sensor_recorder_option_set_int(option, SENSOR_RECORDER_OPTION_INTERVAL, 1000);
 * @endcode
 */
int sensor_recorder_option_set_int(sensor_recorder_option_h option, sensor_recorder_option_e param, int value);

/**
 * @brief   Create a recorder query handle for sensor recorder.
 * @details Allocate a recorder query handle and store it to @a query.
 * @since_tizen 3.0
 *
 * @remarks The @a query must be released using sensor_recorder_destroy_query().
 *
 * @param[out]  query  Query handle
 *
 * @return  #SENSOR_ERROR_NONE on success, otherwise a negative error value
 * @retval  #SENSOR_ERROR_NONE                 Successful
 * @retval  #SENSOR_ERROR_INVALID_PARAMETER    Invalid parameter
 * @retval  #SENSOR_ERROR_OUT_OF_MEMORY        Out of memory
 * @retval  #SENSOR_ERROR_OPERATION_FAILED     Operation failed
 *
 * @par Example
 * @code
 * sensor_recorder_query_h query;
 * sensor_recorder_create_query(&query);
 * @endcode
 */
int sensor_recorder_create_query(sensor_recorder_query_h *query);

/**
 * @brief   Destroy a recorder query handle for sensor recorder.
 * @details Destroy a recorder query which is stored in @a query.
 * @since_tizen 3.0
 *
 * @param[in]  query  Query handle
 *
 * @return  #SENSOR_ERROR_NONE on success, otherwise a negative error value
 * @retval  #SENSOR_ERROR_NONE                 Successful
 * @retval  #SENSOR_ERROR_INVALID_PARAMETER    Invalid parameter
 * @retval  #SENSOR_ERROR_OPERATION_FAILED     Operation failed
 *
 * @par Example
 * @code
 * sensor_recorder_query_h query;
 * sensor_recorder_create_query(&query);
 * ...
 * sensor_recorder_destroy_query(query);
 * @endcode
 */
int sensor_recorder_destroy_query(sensor_recorder_query_h query);

/**
 * @brief   Set an integer-type query parameter for a sensor recorder query.
 * @details Set a query parameter @a param as @a value for a given @a query.
 * @since_tizen 3.0
 *
 * @param[in]  query       Query handle
 * @param[in]  param       Query parameter
 * @param[in]  value       Value
 *
 * @return  #SENSOR_ERROR_NONE on success, otherwise a negative error value
 * @retval  #SENSOR_ERROR_NONE                 Successful
 * @retval  #SENSOR_ERROR_INVALID_PARAMETER    Invalid parameter
 * @retval  #SENSOR_ERROR_OPERATION_FAILED     Operation failed
 *
 * @par Example
 * @code
 * sensor_recorder_query_h query;
 * sensor_recorder_create_query(&query);
 * sensor_recorder_query_set_int(query, SENSOR_RECORDER_QUERY_TIME_INTERVAL, 1);
 * @endcode
 */
int sensor_recorder_query_set_int(sensor_recorder_query_h query, sensor_recorder_query_e param, int value);

/**
 * @brief   Set a time-type query parameter for a sensor recorder query.
 * @details Set a query parameter @a param as @a value for a given @a query.
 * @since_tizen 3.0
 *
 * @param[in]  query       Query handle
 * @param[in]  param       Query parameter
 * @param[in]  t           Time
 *
 * @return  #SENSOR_ERROR_NONE on success, otherwise a negative error value
 * @retval  #SENSOR_ERROR_NONE                 Successful
 * @retval  #SENSOR_ERROR_INVALID_PARAMETER    Invalid parameter
 * @retval  #SENSOR_ERROR_OPERATION_FAILED     Operation failed
 *
 * @par Example
 * @code
 * sensor_recorder_query_h query;
 * sensor_recorder_create_query(&query);
 * time_t start_time = time(NULL);
 * sensor_recorder_query_set_time(query, SENSOR_RECORDER_QUERY_START_TIME, start_time);
 * @endcode
 */
int sensor_recorder_query_set_time(sensor_recorder_query_h query, sensor_recorder_query_e param, time_t t);

/**
 * @brief   Callback function type to be called when the query results are retrieved.
 * @details One of the following errors can be delivered.\n
 *          #SENSOR_ERROR_NONE, Successful\n
 *          #SENSOR_ERROR_OPERATION_FAILED, Operation failed\n
 *          #SENSOR_ERROR_NO_DATA, No data retrieved.
 * @since_tizen 3.0
 *
 * @param[in]  type        Sensor type
 * @param[in]  data        Retrieved data record
 * @param[in]  remains     Number of remaining records to be delivered
 * @param[in]  error       Error
 * @param[in]  user_data   The user data passed from sensor_recorder_read() or sensor_recorder_read_sync()
 *
 * @return  If @c true, it continues to iterate to the next record; If @c false, the iteration stops
 *
 * @par Example
 * @code
 * bool sensor_recorder_data_callback(sensor_type_e type, sensor_recorder_data_h data, int remains, sensor_error_e error, void *user_data)
 * {
 *     ...
 * }
 * ...
 * sensor_recorder_read(type, query, sensor_recorder_data_callback, NULL);
 * ...
 * sensor_recorder_read_sync(type, query, sensor_recorder_data_callback, NULL);
 * @endcode
 */
typedef bool (*sensor_recorder_data_cb)(sensor_type_e type, sensor_recorder_data_h data, int remains, sensor_error_e error, void *user_data);

/**
 * @brief    Query the recorded data asynchronously for the given sensor type.
 * @details  Query and call the callback function @a cb for the recorded data
 *           asynchronously.
 * @since_tizen 3.0
 *
 * @remarks Some sensor types are privileged. An application should have the privilege
 *          %http://tizen.org/privilege/healthinfo to get access to the following sensors:
 *          #SENSOR_HRM, #SENSOR_HRM_LED_GREEN, #SENSOR_HRM_LED_IR, #SENSOR_HRM_LED_RED,
 *          #SENSOR_HUMAN_PEDOMETER, #SENSOR_HUMAN_SLEEP_MONITOR, #SENSOR_HUMAN_SLEEP_DETECTOR,
 *          and #SENSOR_HUMAN_STRESS_MONITOR.
 *
 * @param[in]  type        Sensor type
 * @param[in]  query       Query handle
 * @param[in]  cb          Callback function to receive the queried data
 * @param[in]  user_data   User data to be passed to the callback function
 *
 * @return  #SENSOR_ERROR_NONE on success, otherwise a negative error value
 * @retval  #SENSOR_ERROR_NONE                 Successful
 * @retval  #SENSOR_ERROR_INVALID_PARAMETER    Invalid parameter
 * @retval  #SENSOR_ERROR_NOT_SUPPORTED        Not supported to record the sensor type
 * @retval  #SENSOR_ERROR_PERMISSION_DENIED    Permission denied
 * @retval  #SENSOR_ERROR_OPERATION_FAILED     Operation failed
 *
 * @see     sensor_recorder_read_sync()
 *
 * @par Example
 * @code
 * bool sensor_recorder_data_callback(sensor_type_e type, sensor_recorder_data_h data, int remains, sensor_error_e error, void *user_data)
 * {
 *     ...
 * }
 * ...
 * sensor_recorder_read(type, query, sensor_recorder_data_callback, NULL);
 * @endcode
 */
int sensor_recorder_read(sensor_type_e type, sensor_recorder_query_h query, sensor_recorder_data_cb cb, void *user_data);

/**
 * @brief    Query the recorded data synchronously for the given sensor type.
 * @details  Query and call the callback function @a cb for the recorded data
 *           synchronously.
 * @since_tizen 3.0
 *
 * @remarks Some sensor types are privileged. An application should have the privilege
 *          %http://tizen.org/privilege/healthinfo to get access to the following sensors:
 *          #SENSOR_HRM, #SENSOR_HRM_LED_GREEN, #SENSOR_HRM_LED_IR, #SENSOR_HRM_LED_RED,
 *          #SENSOR_HUMAN_PEDOMETER, #SENSOR_HUMAN_SLEEP_MONITOR, #SENSOR_HUMAN_SLEEP_DETECTOR,
 *          and #SENSOR_HUMAN_STRESS_MONITOR.
 *
 * @param[in]  type        Sensor type
 * @param[in]  query       Query handle
 * @param[in]  cb          Callback function to receive the queried data
 * @param[in]  user_data   User data to be passed to the callback function
 *
 * @return  #SENSOR_ERROR_NONE on success, otherwise a negative error value
 * @retval  #SENSOR_ERROR_NONE                 Successful
 * @retval  #SENSOR_ERROR_INVALID_PARAMETER    Invalid parameter
 * @retval  #SENSOR_ERROR_NOT_SUPPORTED        Not supported to record the sensor type
 * @retval  #SENSOR_ERROR_PERMISSION_DENIED    Permission denied
 * @retval  #SENSOR_ERROR_OPERATION_FAILED     Operation failed
 * @retval  #SENSOR_ERROR_NO_DATA              No data retrieved
 *
 * @see     sensor_recorder_read()
 *
 * @par Example
 * @code
 * bool sensor_recorder_data_callback(sensor_type_e type, sensor_recorder_data_h data, int remains, sensor_error_e error, void *user_data)
 * {
 *     ...
 * }
 * ...
 * sensor_recorder_read_sync(type, query, sensor_recorder_data_callback, NULL);
 * @endcode
 */
int sensor_recorder_read_sync(sensor_type_e type, sensor_recorder_query_h query, sensor_recorder_data_cb cb, void *user_data);

/**
 * @brief   Get the start and the end time of the time period.
 * @details Get the start/end time of the time period for a given record data.
 * @since_tizen 3.0
 *
 * @param[in]  data        Record data handle
 * @param[out] start_time  Start time of the time period of the record
 * @param[out] end_time    End time of the time period of the record
 *
 * @return  #SENSOR_ERROR_NONE on success, otherwise a negative error value
 * @retval  #SENSOR_ERROR_NONE                 Successful
 * @retval  #SENSOR_ERROR_INVALID_PARAMETER    Invalid parameter
 * @retval  #SENSOR_ERROR_OPERATION_FAILED     Operation failed
 *
 * @par Example
 * @code
 * sensor_recorder_data_h data;
 * ... // data(sensor_recorded_data_h) is assigned by sensor_recorder_read_sync().
 * time_t start_time;
 * time_t end_time;
 * sensor_recorder_data_get_time(data, &start_time, &end_time);
 * @endcode
 */
int sensor_recorder_data_get_time(sensor_recorder_data_h data, time_t *start_time, time_t *end_time);

/**
 * @brief   Get an integer value from a record data according to the @a key.
 * @details Get an integer value from a given sensor record data according to
 *          the @a key and store it in @a value. For the available values of
 *          @a key, please refer the type sensor_recorder_data_e.
 * @since_tizen 3.0
 *
 * @param[in]  data        Record data handle
 * @param[in]  key         Data attribute to retrieve
 * @param[out] value       Retrieved value
 *
 * @return  #SENSOR_ERROR_NONE on success, otherwise a negative error value
 * @retval  #SENSOR_ERROR_NONE                 Successful
 * @retval  #SENSOR_ERROR_INVALID_PARAMETER    Invalid parameter
 * @retval  #SENSOR_ERROR_OPERATION_FAILED     Operation failed
 * @retval  #SENSOR_ERROR_NO_DATA              No data retrieved
 *
 * @par Example
 * @code
 * int steps;
 * sensor_recorder_data_get_int(data, SENSOR_RECORDER_DATA_STEPS, &steps);
 * @endcode
 */
int sensor_recorder_data_get_int(sensor_recorder_data_h data, sensor_recorder_data_e key, int *value);

/**
 * @brief   Get a double value from a record data according to the @a key.
 * @details Get a double value from a given sensor record data according to the
 *          @a key and store it in @a value. For the available values of @a key,
 *          please refer the type sensor_recorder_data_e.
 * @since_tizen 3.0
 *
 * @param[in]  data        Record data handle
 * @param[in]  key         Data attribute to retrieve
 * @param[out] value       Retrieved value
 *
 * @return  #SENSOR_ERROR_NONE on success, otherwise a negative error value
 * @retval  #SENSOR_ERROR_NONE                 Successful
 * @retval  #SENSOR_ERROR_INVALID_PARAMETER    Invalid parameter
 * @retval  #SENSOR_ERROR_OPERATION_FAILED     Operation failed
 * @retval  #SENSOR_ERROR_NO_DATA              No data retrieved
 *
 * @par Example
 * @code
 * double distance
 * sensor_recorder_data_get_double(data, SENSOR_RECORDER_DATA_DISTANCE, &distance);
 * @endcode
 */
int sensor_recorder_data_get_double(sensor_recorder_data_h data, sensor_recorder_data_e key, double *value);

/**
 * @}
 */

/**
 * @addtogroup CAPI_SYSTEM_SENSOR_UTILITY_MODULE
 * @{
 */

/**
 * @brief   Enumeration of the axis used in sensor_util_remap_coordinate_system().
 * @since_tizen 2.3
 */
typedef enum
{
    SENSOR_UTIL_AXIS_MINUS_X = 0,  /**< -X */
    SENSOR_UTIL_AXIS_MINUS_Y,      /**< -Y */
    SENSOR_UTIL_AXIS_MINUS_Z,      /**< -Z */
    SENSOR_UTIL_AXIS_X,            /**< +X */
    SENSOR_UTIL_AXIS_Y,            /**< +Y */
    SENSOR_UTIL_AXIS_Z,            /**< +Z */
    sensor_util_axis_minus_x = SENSOR_UTIL_AXIS_MINUS_X,
    sensor_util_axis_minus_y,
    sensor_util_axis_minus_z,
    sensor_util_axis_x,
    sensor_util_axis_y,
    sensor_util_axis_z,
} sensor_util_axis_e;

/**
 * @brief   Gets the inclination matrix @"I@" and rotation matrix @"R@" transforming a vector from the device coordinate to the world's coordinate.
 *
 * @details [0 0 g] = R * gravity (g = magnitude of gravity) \n
 *          [0 m 0] = I * R * geomagnetic (m = magnitude of the geomagnetic field) \n
 *          R is the identity matrix when the device is aligned with the world's coordinate system, that is, when the device's X axis points towards the East, the Y axis points to the North Pole and the device is facing the sky. \n
 *          I is a rotation matrix transforming the geomagnetic vector into the same coordinate space as gravity (the world's coordinate space). I is a simple rotation around the X axis. \n
 * @since_tizen 2.3
 *
 * @remarks Parameters Gx, Gy, and Gz can be obtained from the values returned by #SENSOR_GRAVITY. \n
 *          Parameters Mx, My, and Mz can be obtained from the values returned by #SENSOR_MAGNETIC.
 *          Output parameter R and I are always returned as a 3x3 matrix array of 9 floats like this form:
 *          <pre>
 *          { R[0], R[1], R[2],
 *            R[3], R[4], R[5],
 *            R[6], R[7], R[6] }
 *          </pre>
 *
 *
 * @param[in]  Gx   The X-axis gravity vector in the device's coordinate
 * @param[in]  Gy   The Y-axis gravity vector in the device's coordinate
 * @param[in]  Gz   The Z-axis gravity vector in the device's coordinate
 * @param[in]  Mx   The X-axis geomagnetic vector in the device's coordinate
 * @param[in]  My   The Y-axis geomagnetic vector in the device's coordinate
 * @param[in]  Mz   The Z-axis geomagnetic vector in the device's coordinate
 * @param[out] R    The array of 9 floats that represent the rotation matrix @"R@" \n
 *                  It can be null.
 * @param[out] I    The array of 9 floats that represent the inclination matrix @"I@" \n
 *                  It can be null.
 *
 * @return  #SENSOR_ERROR_NONE on success; Otherwise a negative error value
 * @retval  #SENSOR_ERROR_NONE                 Successful
 * @retval  #SENSOR_ERROR_INVALID_PARAMETER    Invalid parameter
 *
 * @par Example
 * @code
 * ...
 * float R[9];
 * float I[9];
 * sensor_util_get_rotation_matrix(Gx, Gy, Gz, Mx, My, Mz, R, I);
 * @endcode
 */
int sensor_util_get_rotation_matrix(float Gx, float Gy, float Gz,
        float Mx, float My, float Mz,
        float R[], float I[]);

/**
 * @brief Convert a rotation vector to a rotation matrix and store it to @a R.
 *
 * @details Rotation vectors (Vx, Vy, Vz) can be obtained from #SENSOR_ROTATION_VECTOR.
 *          It returns a 9 element rotation matrix in the array R. R must have length as 9.
 * @since_tizen 2.3
 *
 * @param[in]  Vx   The X-axis rotation vector
 * @param[in]  Vy   The Y-axis rotation vector
 * @param[in]  Vz   The Z-axis rotation vector
 * @param[out] R    A 9 element rotation matrix in the array R that must have length as 9
 *
 * @return  #SENSOR_ERROR_NONE on success; Otherwise a negative error value
 * @retval  #SENSOR_ERROR_NONE                 Successful
 * @retval  #SENSOR_ERROR_INVALID_PARAMETER    Invalid parameter
 *
 * @par Example
 * @code
 * ...
 * float R[9];
 * sensor_util_get_rotation_matrix_from_vector(Vx, Vy, Vz, R);
 * @endcode
 */
int sensor_util_get_rotation_matrix_from_vector(float Vx, float Vy, float Vz, float R[]);

/**
 * @brief Rotates the supplied rotation matrix so that it is expressed in a different coordinate system.
 *
 * @details This is typically used when an application needs to compute the three orientation angles of the device in a different coordinate system.
 * @since_tizen 2.3
 *
 * @remarks inR and outR can be the same array, but this is not recommended for performance reasons.
 *          This returns an error when X and Y define the same axis.
 *
 * @param[in]  inR  The rotation matrix (3x3) to be transformed
 * @param[in]  x    The world axis and direction on which the X axis of the device is mapped
 * @param[in]  y    The world axis and direction on which the Y axis of the device is mapped
 * @param[out] outR The transformed rotation matrix (3x3)
 *
 * @return  #SENSOR_ERROR_NONE on success; Otherwise a negative error value
 * @retval  #SENSOR_ERROR_NONE                 Successful
 * @retval  #SENSOR_ERROR_INVALID_PARAMETER    Invalid parameter
 *
 * @par Example
 * @code
 * float inR[9];
 * ...
 * float outR[9];
 * sensor_util_remap_coordinate_system(inR, SENSOR_UTIL_AXIS_X, SENSOR_UTIL_AXIS_Y, outR);
 * @endcode
 */
int sensor_util_remap_coordinate_system(float inR[], sensor_util_axis_e x, sensor_util_axis_e y, float outR[]);

/**
 * @brief   Compute the geomagnetic inclination angle in radians from the
 *          inclination matrix.
 * @details Compute the geomagnetic inclination angle in radians from the
 *          inclination matrix. Inclination matrix is a matrix which is returned
 *          by sensor_util_get_rotation_matrix().
 * @since_tizen 2.3
 *
 * @param[in]  I            The inclination matrix from sensor_util_get_rotation_matrix()
 * @param[out] inclination  The geomagnetic inclination angle in radians
 *
 * @return  #SENSOR_ERROR_NONE on success; Otherwise a negative error value
 * @retval  #SENSOR_ERROR_NONE                 Successful
 * @retval  #SENSOR_ERROR_INVALID_PARAMETER    Invalid parameter
 *
 * @see sensor_util_get_rotation_matrix()
 *
 * @par Example
 * @code
 * float I[9];
 * ...
 * float inclination;
 * sensor_util_get_inclination(I, &inclination);
 * @endcode
 */
int sensor_util_get_inclination(float I[], float* inclination);

/**
 * @brief Computes the device's orientation based on the rotation matrix.
 *
 * @details When it returns, the array values are filled with the result:
 *          - values[0]: azimuth, rotation around the Z axis.
 *          - values[1]: pitch, rotation around the X axis.
 *          - values[2]: roll, rotation around the Y axis.
 * @since_tizen 2.3
 *
 * @remarks Parameter R must be an array of 9 floats from sensor_util_get_rotation_matrix() \n
 *          Returned values are always arrays of 3 floats.
 *
 * @param[in]  R         A 9 element rotation matrix in the array
 * @param[out] values    An array of 3 floats to hold the result
 *
 * @return  #SENSOR_ERROR_NONE on success; Otherwise a negative error value
 * @retval  #SENSOR_ERROR_NONE                 Successful
 * @retval  #SENSOR_ERROR_INVALID_PARAMETER    Invalid parameter
 *
 * @see sensor_util_get_rotation_matrix()
 *
 * @par Example
 * @code
 * float R[9];
 * ...
 * float values[3];
 * sensor_util_get_orientation(R, values);
 * @endcode
 */
int sensor_util_get_orientation(float R[], float values[]);

/**
 * @brief Computes the angle change between two rotation matrices.
 *
 * @details Given a current rotation matrix (R) and a previous rotation matrix (prevR), it computes
 *          the rotation around the x,y, and z axes which transforms prevR to R.
 *          It outputs a 3 element vector containing the x,y, and z angle change at indexes 0, 1, and 2 respectively. \n
 * @since_tizen 2.3
 *
 * @remarks Each input matrix is a 3x3 matrix like this form:
 *          <pre>
 *          { R[0], R[1], R[2],
 *            R[3], R[4], R[5],
 *            R[6], R[7], R[6] }
 *          </pre>
 *
 * @param[in] R             The current rotation matrix
 * @param[in] prevR         The previous rotation matrix
 * @param[out] angleChange  An array of floats in which the angle change is stored
 *
 * @return  #SENSOR_ERROR_NONE on success; Otherwise a negative error value
 * @retval  #SENSOR_ERROR_NONE                 Successful
 * @retval  #SENSOR_ERROR_INVALID_PARAMETER    Invalid parameter
 *
 * @par Example
 * @code
 * float R[9];
 * float prevR[9];
 * ...
 * float angleChange[3];
 * sensor_util_get_angle_change(R, prevR, float angleChange[]);
 * @endcode
 */
int sensor_util_get_angle_change(float R[], float prevR[], float angleChange[]);

/**
 * @brief   Get the declination in degrees from geodetic coordinates.
 * @details Get the declination of the horizontal component of the magnetic
 *          field from true north, in degrees.
 * @since_tizen 2.3
 *
 * @param[in]  latitude     The latitude in geodetic coordinates
 * @param[in]  longitude    The longitude in geodetic coordinates
 * @param[in]  altitude     The altitude in geodetic coordinates
 * @param[out] declination  The declination of the horizontal component of the magnetic field in degrees
 *
 * @return  #SENSOR_ERROR_NONE on success; Otherwise a negative error value
 * @retval  #SENSOR_ERROR_NONE                 Successful
 * @retval  #SENSOR_ERROR_INVALID_PARAMETER    Invalid parameter
 *
 * @par Example
 * @code
 * ...
 * float declination;
 * sensor_util_get_declination(latitude, longitude, altitude, &declination);
 * @endcode
 */
int sensor_util_get_declination(float latitude, float longitude, float altitude, float* declination);

/**
 * @brief   Get the altitude in meters from pressure and temperature.
 * @details Get the altitude from the atmospheric pressure, the pressure at sea
 *          level and temperature, in meters.
 * @since_tizen 4.0
 *
 * @param[in]  pressure           The atmospheric pressure (hPa)
 * @param[in]  sea_level_pressure The sea level pressure (hPa) @n
 *                                If the sea level pressure is not known, you can use 1013.25 hPa, mean sea level pressure
 * @param[in]  temperature        The temperature (degrees Celsius) @n
 *                                If the temperature is not known, you can use 15 degrees Celsius
 * @param[out] altitude           The altitude (meters)
 *
 * @return  #SENSOR_ERROR_NONE on success; Otherwise a negative error value
 * @retval  #SENSOR_ERROR_NONE                 Successful
 * @retval  #SENSOR_ERROR_INVALID_PARAMETER    Invalid parameter
 *
 * @par Example
 * @code
 * ...
 * float altitude;
 * sensor_util_get_altitude(pressure, sea_level_pressure, temperature, &altitude);
 * @endcode
 */
int sensor_util_get_altitude(float pressure, float sea_level_pressure, float temperature, float* altitude);

/**
 * @}
 */

#ifdef __cplusplus
}
#endif

#endif /* __SENSOR_H__ */
