/*
 * Copyright (c) 2018 Samsung Electronics Co., Ltd. All rights reserved.
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
 *
 */

#ifndef __TIZEN_CAPI_NETWORK_USER_AWARENESS_H__
#define __TIZEN_CAPI_NETWORK_USER_AWARENESS_H__

#include <stdbool.h>
#include <glib.h>
#include <tizen.h>

#include <user-awareness-type.h>

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @file user-awareness.h
 */

/**
 * @ingroup CAPI_NETWORK_FRAMEWORK
 * @addtogroup CAPI_NETWORK_UA_MODULE
 * @{
 */

/**
 * @brief Initializes the user awareness framework.
 * @details This function initializes the user awareness
 *    framework, which is responsible for providing various
 *    user-related information such as presence, activity, and
 *    location. Initializing the framework allows applications
 *    to use its features and services.
 * @since_tizen 6.5
 *
 * @return 0 on success, otherwise a negative error value
 * @retval #UA_ERROR_NONE Successful
 * @retval #UA_ERROR_NOT_SUPPORTED Not Supported
 * @retval #UA_ERROR_OUT_OF_MEMORY Out of memory
 * @retval #UA_ERROR_ALREADY_DONE Already initialized
 *
 * @see ua_deinitialize()
 */
int ua_initialize(void);

/**
 * @brief De-initializes the user awareness framework.
 * @details This function de-initializes the user awareness
 *    framework, releasing all resources allocated during
 *    initialization. After calling this function, the framework
 *    will no longer be available for use until it is
 *    re-initialized with ua_initialize().
 * @since_tizen 6.5
 *
 * @return 0 on success, otherwise a negative error value
 * @retval #UA_ERROR_NONE Successful
 * @retval #UA_ERROR_NOT_SUPPORTED Not Supported
 * @retval #UA_ERROR_NOT_INITIALIZED Not initialized yet
 * @retval #UA_ERROR_ALREADY_DONE Already initialized
 *
 * @see ua_initialize()
 */
int ua_deinitialize(void);

/**
 * @brief Retrieves handles of all added services.
 * @details This function retrieves handles of all services
 *    that have been added to the user awareness framework.
 *    It iterates through the list of services and invokes
 *    the provided callback function for each service handle,
 *    passing the handle and user-provided data to the callback.
 * @since_tizen 6.5
 *
 * @param[in] foreach_cb Callback function to be invoked with each service handle.
 * @param[in] user_data The user data to be passed when callback is called.
 *
 * @return 0 on success, otherwise a negative error value
 * @retval #UA_ERROR_NONE Successful
 * @retval #UA_ERROR_NOT_SUPPORTED Not Supported
 * @retval #UA_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #UA_ERROR_NOT_INITIALIZED Not initialized yet
 *
 * @see ua_service_cb()
 */
int ua_foreach_service(
		ua_service_cb foreach_cb,
		void *user_data);

/**
 * @brief Gets default service handle.
 * @details This function retrieves the default service handle, which
 *    represents the primary service used by the user awareness framework.
 *    The default service handle can be used to access various features
 *    and functionalities provided by the framework.
 * @since_tizen 6.5
 *
 * @remarks The @a service handle is managed by the platform and will be released
 *             when ua_deinitialize() is called.
 * @param[out] service The service handle
 *
 * @return 0 on success, otherwise a negative error value
 * @retval #UA_ERROR_NONE Successful
 * @retval #UA_ERROR_NOT_SUPPORTED Not Supported
 * @retval #UA_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #UA_ERROR_OPERATION_FAILED Operation failed
 * @retval #UA_ERROR_NOT_INITIALIZED Not initialized yet
 *
 * @see ua_service_get_name()
 * @see ua_service_create()
 */
int ua_get_default_service(
		ua_service_h *service);

/**
 * @brief Gets service handle by service name for added service.
 * @details This function retrieves the service handle associated with
 *    the specified service name. If a service with the given name exists,
 *    its handle is returned; otherwise, an error is returned.
 * @since_tizen 6.5
 *
 * @remarks The @a service handle should be destroyed by using ua_service_destroy().
 *
 * @param[in] name The service name information
 * @param[out] service The service handle
 *
 * @return 0 on success, otherwise a negative error value
 * @retval #UA_ERROR_NONE Successful
 * @retval #UA_ERROR_NOT_SUPPORTED Not Supported
 * @retval #UA_ERROR_INVALID_PARAMETER Invalid parameter or if service with such name does not exist.
 * @retval #UA_ERROR_OPERATION_FAILED Operation failed
 * @retval #UA_ERROR_NOT_INITIALIZED Not initialized yet
 *
 * @see ua_service_create()
 */
int ua_get_service_by_name(
			const char *name,
			ua_service_h *service);

/**
 * @brief Creates the ua monitor's handle.
 * @details This function creates a new handle for a user awareness
 *    (UA) monitor, which can be used to observe and track user-related
 *    events and activities. The created monitor handle can be configured
 *    and used to receive notifications about specific events or changes
 *    in user behavior.
 * @since_tizen 6.5
 *
 * @remarks The @a monitor handle should be destroyed by using ua_monitor_destroy().
 *
 * @param[out] monitor The monitor handle
 *
 * @return 0 on success, otherwise a negative error value
 * @retval #UA_ERROR_NONE Successful
 * @retval #UA_ERROR_NOT_SUPPORTED Not Supported
 * @retval #UA_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #UA_ERROR_OUT_OF_MEMORY Out of memory
 * @retval #UA_ERROR_NOT_INITIALIZED Not initialized yet
 *
 * @see ua_monitor_destroy()
 */
int ua_monitor_create(
		ua_monitor_h *monitor);

/**
 * @brief Destroys the ua monitor's handle.
 * @details This function destroys the specified user awareness
 *    (UA) monitor handle, releasing all resources associated with it.
 *    Once the handle is destroyed, it can no longer be used to monitor
 *    user-related events or activities.
 * @since_tizen 6.5
 *
 * @param[in] monitor The monitor handle
 *
 * @return 0 on success, otherwise a negative error value
 * @retval #UA_ERROR_NONE Successful
 * @retval #UA_ERROR_NOT_SUPPORTED Not Supported
 * @retval #UA_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #UA_ERROR_NOT_INITIALIZED Not initialized yet
 *
 * @see ua_monitor_create()
 */
int ua_monitor_destroy(
		ua_monitor_h monitor);

/**
 * @brief Adds sensors to UA monitor for which monitoring is to be done.
 * Availability of sensors to be added can be checked using ua_monitor_is_sensor_available().
 * @details This function adds the specified sensor to the user awareness
 *    (UA) monitor, enabling it to track and monitor events related to the sensor.
 *    Before adding a sensor, its availability can be checked using the
 *    ua_monitor_is_sensor_available() function.
 * @since_tizen 6.5
 * @remarks If you want to add the BLE sensor,
 * you should add privilege %http://tizen.org/privilege/bluetooth.
 * if you want to add the Wi-Fi sensor,
 * you should add privilege %http://tizen.org/privilege/network.get.
 *
 * @param[in] monitor The monitor handle
 * @param[in] sensor The sensor to be added to monitor
 *
 * @return 0 on success, otherwise a negative error value
 * @retval #UA_ERROR_NONE Successful
 * @retval #UA_ERROR_NOT_SUPPORTED Not Supported
 * @retval #UA_ERROR_PERMISSION_DENIED Permission Denied
 * @retval #UA_ERROR_INVALID_PARAMETER Invalid parameter or sensor not available to add
 * @retval #UA_ERROR_NOT_PERMITTED Operation not permitted
 * @retval #UA_ERROR_ALREADY_DONE Already done
 * @retval #UA_ERROR_NOT_READY Resource not ready
 * @retval #UA_ERROR_OPERATION_FAILED Operation failed
 * @retval #UA_ERROR_NOT_INITIALIZED Not initialized yet
 *
 * @see ua_monitor_remove_sensor()
 * @see ua_monitor_foreach_sensor()
 */
int ua_monitor_add_sensor(
		ua_monitor_h monitor,
		ua_sensor_type_e sensor);

/**
 * @brief Removes sensors from UA monitor.
 * @details This function removes the specified sensor from the user awareness
 *    (UA) monitor, disabling it from tracking and monitoring events related
 *    to the sensor. Removing a sensor from the monitor stops any ongoing
 *    monitoring activities for that sensor.
 * @since_tizen 6.5
 * @remarks If you want to remove the BLE sensor,
 * you should add privilege %http://tizen.org/privilege/bluetooth.
 * if you want to remove the Wi-Fi sensor,
 * you should add privilege %http://tizen.org/privilege/network.get.
 *
 * @param[in] monitor The monitor handle
 * @param[in] sensor Sensor to be removed from monitor
 *
 * @return 0 on success, otherwise a negative error value
 * @retval #UA_ERROR_NONE Successful
 * @retval #UA_ERROR_NOT_SUPPORTED Not Supported
 * @retval #UA_ERROR_PERMISSION_DENIED Permission Denied
 * @retval #UA_ERROR_INVALID_PARAMETER Invalid parameter or sensor not available to remove
 * @retval #UA_ERROR_NOT_PERMITTED Operation not permitted
 * @retval #UA_ERROR_ALREADY_DONE Already done
 * @retval #UA_ERROR_NOT_READY Resource not ready
 * @retval #UA_ERROR_OPERATION_FAILED Operation failed
 * @retval #UA_ERROR_NOT_INITIALIZED Not initialized yet
 *
 * @see ua_monitor_add_sensor()
 * @see ua_monitor_foreach_sensor()
 */
int ua_monitor_remove_sensor(
		ua_monitor_h monitor,
		ua_sensor_type_e sensor);

/**
 * @brief Retrieves all sensors present in UA monitor.
 * @details This function retrieves all sensors that are currently present in
 *    the user awareness (UA) monitor. It iterates through the list of sensors
 *    and invokes the provided callback function for each sensor, passing the
 *    sensor type and user-provided data to the callback.
 * @since_tizen 6.5
 *
 * @param[in] monitor The monitor handle
 * @param[in] foreach_cb Callback function to be invoked foreach sensor present in monitor handle.
 * @param[in] user_data The user data to be passed when callback is called.
 *
 * @return 0 on success, otherwise a negative error value
 * @retval #UA_ERROR_NONE Successful
 * @retval #UA_ERROR_NOT_SUPPORTED Not Supported
 * @retval #UA_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #UA_ERROR_OPERATION_FAILED Operation failed
 * @retval #UA_ERROR_NOT_INITIALIZED Not initialized yet
 *
 * @see ua_monitor_add_sensor()
 * @see ua_monitor_remove_sensor()
 */
int ua_monitor_foreach_sensor(
		ua_monitor_h monitor,
		ua_monitor_sensor_cb foreach_cb,
		void *user_data);

/**
 * @brief Checks if sensor is available or not.
 * @details This function checks the availability of the specified sensor in
 *    the user awareness (UA) monitor. It returns a boolean value indicating
 *    whether the sensor is available or not.
 * @since_tizen 6.5
 *
 * @param[in] sensor The sensor for which status to be checked.
 * @param[out] available TRUE if sensor is available FALSE otherwise.
 *
 * @return 0 on success, otherwise a negative error value
 * @retval #UA_ERROR_NONE Successful
 * @retval #UA_ERROR_NOT_SUPPORTED Not Supported
 * @retval #UA_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #UA_ERROR_OPERATION_FAILED Operation failed
 * @retval #UA_ERROR_NOT_INITIALIZED Not initialized yet
 *
 */
int ua_monitor_is_sensor_available(
		ua_sensor_type_e sensor,
		bool *available);

/**
 * @brief Sets sensor status changed callback. Callback to be invoked
 * when sensor status changes from 'PRESENCE' to 'ABSENCE' or vice-versa.
 * @details This function sets a callback function to be invoked whenever
 *    the status of an environmental sensor changes. The callback function
 *    will be called with the updated sensor status and user-provided data
 *    whenever the sensor transitions between 'PRESENCE' and 'ABSENCE' states.
 * @since_tizen 6.5
 *
 * @remarks Sensor status change will be reported only for environmental
 * sensors (ie. the sensors for which user/device information is not available,
 * For example. Light and Motion Sensors)
 *
 * @param[in] monitor The monitor handle
 * @param[in] callback Sensor status changed callback
 * @param[in] user_data The user data to be passed in sensor state changed callback.
 *
 * @return 0 on success, otherwise a negative error value
 * @retval #UA_ERROR_NONE Successful
 * @retval #UA_ERROR_NOT_SUPPORTED Not Supported
 * @retval #UA_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #UA_ERROR_NOT_INITIALIZED Not initialized yet
 *
 */
int ua_monitor_set_sensor_status_changed_cb(
		ua_monitor_h monitor,
		ua_sensor_status_changed_cb callback,
		void *user_data);

/**
 * @brief Unsets sensor status changed callback.
 * @details This function unsets the previously set sensor status changed
 *    callback function for the user awareness (UA) monitor. After this
 *    function is called, the callback function will no longer be invoked
 *    when the status of environmental sensors changes.
 * @since_tizen 6.5
 *
 * @param[in] monitor The monitor handle
 *
 * @return 0 on success, otherwise a negative error value
 * @retval #UA_ERROR_NONE Successful
 * @retval #UA_ERROR_NOT_SUPPORTED Not Supported
 * @retval #UA_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #UA_ERROR_NOT_INITIALIZED Not initialized yet
 *
 * @see ua_monitor_set_sensor_status_changed_cb()
 */
int ua_monitor_unset_sensor_status_changed_cb(
		ua_monitor_h monitor);

/**
 * @brief Starts scan of registered devices.
 * @details This function initiates a scan of registered devices using the
 *    user awareness (UA) monitor. The scan can include Bluetooth Low Energy
 *    (BLE) and Wi-Fi devices, depending on the privileges granted and the
 *    sensors added to the monitor. The scan time can be controlled using
 *    the scan_time_multiplier parameter, which multiplies the predefined
 *    scan duration constant (#UA_SCAN_TIME_MULTIPLIER) to determine the
 *    maximum scan time in seconds.
 * @since_tizen 6.5
 * @privlevel public
 * @privilege %http://tizen.org/privilege/location
 * @remarks If you want to start BLE scan,
 * you should add privilege %http://tizen.org/privilege/bluetooth.
 * if you want to start Wi-Fi scan,
 * you should add privilege %http://tizen.org/privilege/network.get.
 *
 * @remarks Only one scan can run at one time.
 *
 * @param[in] monitor The monitor handle
 * @param[in] scan_time_multiplier A value which we will multiply by #UA_SCAN_TIME_MULTIPLIER.
 *            This is maximum time (in seconds) for which scan can run, but scan can complete earlier also.
 * @param[in] callback It will be called for each scanned device one by one, and on scan
 *            completion too.
 * @param[in] user_data The user data to be passed when callback is called.
 *
 * @return 0 on success, otherwise a negative error value
 * @retval #UA_ERROR_NONE Successful
 * @retval #UA_ERROR_NOT_SUPPORTED Not Supported
 * @retval #UA_ERROR_PERMISSION_DENIED Permission Denied
 * @retval #UA_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #UA_ERROR_NOW_IN_PROGRESS Operation now in progress
 * @retval #UA_ERROR_NO_DATA No sensor available
 * @retval #UA_ERROR_OPERATION_FAILED Operation failed
 * @retval #UA_ERROR_NOT_INITIALIZED Not initialized yet
 *
 * @see UA_SCAN_TIME_MULTIPLIER
 * @see ua_scan_completed_cb()
 * @see ua_monitor_stop_scan()
 */
int ua_monitor_start_scan(
		ua_monitor_h monitor,
		int scan_time_multiplier,
		ua_scan_completed_cb callback,
		void *user_data);

/**
 * @brief Stops ua_monitor_start_scan().
 * @details This function stops the ongoing scan initiated by
 *    ua_monitor_start_scan(). It terminates the scan process early
 *    and prevents further device discovery until another scan is started.
 * @since_tizen 6.5
 * @privlevel public
 * @privilege %http://tizen.org/privilege/location
 * @remarks If you want to stop BLE scan,
 * you should add privilege %http://tizen.org/privilege/bluetooth.
 * if you want to stop Wi-Fi scan,
 * you should add privilege %http://tizen.org/privilege/network.get.
 *
 * @param[in] monitor UA monitor's handle
 *
 * @return 0 on success, otherwise a negative error value
 * @retval #UA_ERROR_NONE Successful
 * @retval #UA_ERROR_NOT_SUPPORTED Not Supported
 * @retval #UA_ERROR_PERMISSION_DENIED Permission Denied
 * @retval #UA_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #UA_ERROR_NOT_IN_PROGRESS Operation not in progress
 * @retval #UA_ERROR_OPERATION_FAILED Operation failed
 * @retval #UA_ERROR_NOT_INITIALIZED Not initialized yet
 *
 * @see ua_monitor_start_scan()
 */
int ua_monitor_stop_scan(ua_monitor_h monitor);

/**
 * @brief Starts user presence detection.
 * @details This function initiates user presence detection using the user awareness
 *    (UA) monitor. The detection can include Bluetooth Low Energy (BLE) and Wi-Fi
 *    presence detection, depending on the privileges granted and the sensors added
 *    to the monitor. The detection mode parameter determines whether the presence
 *    detection callback will be invoked only after a user is detected by all sensors
 *    or as soon as a user is detected by any one of the sensors.
 * @since_tizen 6.5
 * @privlevel public
 * @privilege %http://tizen.org/privilege/location
 * @remarks If you want to start BLE presence detection,
 * you should add privilege %http://tizen.org/privilege/bluetooth.
 * if you want to start Wi-Fi presence detection,
 * you should add privilege %http://tizen.org/privilege/network.get.
 *
 * @remarks This function is not specific for any particular user's presence detection.
 * Therefore, the callback will be invoked when any user is detected by sensors.
 *
 * @param[in] monitor The monitor handle
 * @param[in] service The service handle. If service is NULL then monitor will start for default service.
 * @param[in] mode  User detection mode. \n
 *  #UA_DETECT_MODE_ALL_SENSORS : Detection callback will be invoked only after a user is
 *  detected by all sensors added to monitor. \n
 *  #UA_DETECT_MODE_ANY_SENSOR : Detection callback will be invoked as soon as a user is
 *  detected by any one of the sensors. \n
 * @param[in] callback Callback to be invoked after device presence detection.
 * @param[in] user_data The user data to be passed when callback is called.
 *
 * @return 0 on success, otherwise a negative error value
 * @retval #UA_ERROR_NONE Successful
 * @retval #UA_ERROR_NOT_SUPPORTED Not Supported
 * @retval #UA_ERROR_PERMISSION_DENIED Permission Denied
 * @retval #UA_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #UA_ERROR_NOW_IN_PROGRESS Operation now in progress
 * @retval #UA_ERROR_NO_DATA No sensor available
 * @retval #UA_ERROR_OPERATION_FAILED Operation failed
 * @retval #UA_ERROR_NOT_INITIALIZED Not initialized yet
 *
 * @see ua_presence_detected_cb()
 * @see ua_monitor_stop_presence_detection()
 */
int ua_monitor_start_presence_detection(
		ua_monitor_h monitor,
		ua_service_h service,
		ua_detection_mode_e mode,
		ua_presence_detected_cb callback,
		void *user_data);

/**
 * @brief Stops user presence detection.
 * @details This function stops the ongoing user presence detection
 *    initiated by ua_monitor_start_presence_detection(). It terminates
 *    the detection process early and prevents further presence detection
 *    until another detection is started.
 * @since_tizen 6.5
 * @privlevel public
 * @privilege %http://tizen.org/privilege/location
 * @remarks If you want to stop BLE presence detection,
 * you should add privilege %http://tizen.org/privilege/bluetooth.
 * if you want to stop Wi-Fi presence detection,
 * you should add privilege %http://tizen.org/privilege/network.get.
 *
 * @param[in] monitor UA monitor's handle
 *
 * @return 0 on success, otherwise a negative error value
 * @retval #UA_ERROR_NONE Successful
 * @retval #UA_ERROR_NOT_SUPPORTED Not Supported
 * @retval #UA_ERROR_PERMISSION_DENIED Permission Denied
 * @retval #UA_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #UA_ERROR_NOT_IN_PROGRESS Operation not in progress
 * @retval #UA_ERROR_OPERATION_FAILED Operation failed
 * @retval #UA_ERROR_NOT_INITIALIZED Not initialized yet
 *
 * @see ua_monitor_start_presence_detection()
 */
int ua_monitor_stop_presence_detection(
		ua_monitor_h monitor);

/**
 * @brief Starts user absence detection.
 * @details This function initiates user absence detection using the
 *    user awareness (UA) monitor. The detection can include Bluetooth
 *    Low Energy (BLE) and Wi-Fi absence detection, depending on the
 *    privileges granted and the sensors added to the monitor. The
 *    detection mode parameter determines whether the absence detection
 *    callback will be invoked only after no user is detected by all
 *    sensors or as soon as no user is detected by any one of the sensors.
 * @since_tizen 6.5
 * @privlevel public
 * @privilege %http://tizen.org/privilege/location
 * @remarks If you want to start BLE absence detection,
 * you should add privilege %http://tizen.org/privilege/bluetooth.
 * if you want to start Wi-Fi absence detection,
 * you should add privilege %http://tizen.org/privilege/network.get.
 *
 * @remarks This function is not specific for any particular user's absence detection.
 * Therefore, the callback will be invoked only when no user is detected by sensors.
 *
 * @param[in] monitor The monitor handle
 * @param[in] service The service handle. If service is NULL then monitor will start for default service.
 * @param[in] mode User detection mode. \n
 *  #UA_DETECT_MODE_ALL_SENSORS : Detection callback will be invoked only after no user is
 *  detected by all sensors added to monitor. \n
 *  #UA_DETECT_MODE_ANY_SENSOR : Detection callback will be invoked as soon as no user is
 *  detected by any one of the sensors. \n
 * @param[in] callback  Callback to be invoked after absence detection.
 * @param[in] user_data  The user data to be passed when callback is called.
 *
 * @return 0 on success, otherwise a negative error value
 * @retval #UA_ERROR_NONE Successful
 * @retval #UA_ERROR_NOT_SUPPORTED Not Supported
 * @retval #UA_ERROR_PERMISSION_DENIED Permission Denied
 * @retval #UA_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #UA_ERROR_NOW_IN_PROGRESS Operation now in progress
 * @retval #UA_ERROR_NO_DATA No sensor available
 * @retval #UA_ERROR_OPERATION_FAILED Operation failed
 * @retval #UA_ERROR_NOT_INITIALIZED Not initialized yet
 *
 * @see ua_absence_detected_cb()
 * @see ua_monitor_stop_absence_detection()
 */
int ua_monitor_start_absence_detection(
		ua_monitor_h monitor,
		ua_service_h service,
		ua_detection_mode_e mode,
		ua_absence_detected_cb callback,
		void *user_data);

/**
 * @brief Stops user absence detection.
 * @details This function stops the ongoing user absence detection
 *    initiated by ua_monitor_start_absence_detection(). It terminates
 *    the detection process early and prevents further absence detection
 *    until another detection is started.
 * @since_tizen 6.5
 * @privlevel public
 * @privilege %http://tizen.org/privilege/location
 * @remarks If you want to stop BLE absence detection,
 * you should add privilege %http://tizen.org/privilege/bluetooth.
 * if you want to stop Wi-Fi absence detection,
 * you should add privilege %http://tizen.org/privilege/network.get.
 *
 * @param[in] monitor The monitor handle
 *
 * @return 0 on success, otherwise a negative error value
 * @retval #UA_ERROR_NONE Successful
 * @retval #UA_ERROR_NOT_SUPPORTED Not Supported
 * @retval #UA_ERROR_PERMISSION_DENIED Permission Denied
 * @retval #UA_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #UA_ERROR_NOT_IN_PROGRESS Operation not in progress
 * @retval #UA_ERROR_OPERATION_FAILED Operation failed
 * @retval #UA_ERROR_NOT_INITIALIZED Not initialized yet
 *
 * @see ua_monitor_start_absence_detection()
 */
int ua_monitor_stop_absence_detection(
		ua_monitor_h monitor);

/**
 * @brief Sets the location callback period in milli-seconds.
 * If location period is not set, it will be automatically
 * set to #UA_LOCATION_PERIOD_DEFAULT.
 * @details This function sets the interval at which location
 *    updates are received from the user awareness (UA) monitor.
 *    If the location period is not explicitly set, it defaults
 *    to #UA_LOCATION_PERIOD_DEFAULT. The location period is
 *    specified in milliseconds and must be within the range
 *    of 0 to UINT_MAX.
 * @since_tizen 6.5
 *
 * @param[in] monitor The monitor handle
 * @param[in] location_period location period (0 to UINT_MAX)
 *
 * @return 0 on success, otherwise a negative error value
 * @retval #UA_ERROR_NONE Successful
 * @retval #UA_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #UA_ERROR_NOT_INITIALIZED Not initialized yet
 */
int ua_monitor_set_location_period(
	ua_monitor_h monitor, unsigned int location_period);

/**
 * @brief Starts user location detection.
 * @details This function initiates user location detection using the
 *    user awareness (UA) monitor. The detection can include Bluetooth
 *    Low Energy (BLE) and Wi-Fi location detection, depending on the
 *    privileges granted and the sensors added to the monitor. The
 *    location detection callback will be invoked periodically based
 *    on the location period set by ua_monitor_set_location_period()
 *    or the default period.
 * @since_tizen 6.5
 * @privlevel public
 * @privilege %http://tizen.org/privilege/location
 * @remarks If you want to start BLE location detection,
 * you should add privilege %http://tizen.org/privilege/bluetooth.
 * If you want to start Wi-Fi location detection,
 * you should add privilege %http://tizen.org/privilege/network.get.
 *
 * @param[in] monitor The monitor handle
 * @param[in] service The service handle. If service is NULL then monitor will start for default service.
 * @param[in] callback Callback to be invoked after device location detection.
 * @param[in] user_data The user data to be passed when callback is called.
 *
 * @return 0 on success, otherwise a negative error value
 * @retval #UA_ERROR_NONE Successful
 * @retval #UA_ERROR_NOT_SUPPORTED Not Supported
 * @retval #UA_ERROR_PERMISSION_DENIED Permission Denied
 * @retval #UA_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #UA_ERROR_NOW_IN_PROGRESS Operation now in progress
 * @retval #UA_ERROR_NO_DATA No sensor available
 * @retval #UA_ERROR_OPERATION_FAILED Operation failed
 * @retval #UA_ERROR_NOT_INITIALIZED Not initialized yet
 *
 * @see ua_location_detected_cb()
 * @see ua_monitor_set_location_period()
 * @see ua_monitor_stop_location_detection()
 */
int ua_monitor_start_location_detection(
		ua_monitor_h monitor,
		ua_service_h service,
		ua_location_detected_cb callback,
		void *user_data);

/**
 * @brief Stops user location detection.
 * @details This function stops the ongoing user location detection
 *    initiated by ua_monitor_start_location_detection(). It terminates
 *    the detection process early and prevents further location detection
 *    until another detection is started.
 * @since_tizen 6.5
 * @privlevel public
 * @privilege %http://tizen.org/privilege/location
 * @remarks If you want to stop BLE location detection,
 * you should add privilege %http://tizen.org/privilege/bluetooth.
 * If you want to stop Wi-Fi location detection,
 * you should add privilege %http://tizen.org/privilege/network.get.
 *
 * @param[in] monitor UA monitor's handle
 *
 * @return 0 on success, otherwise a negative error value
 * @retval #UA_ERROR_NONE Successful
 * @retval #UA_ERROR_NOT_SUPPORTED Not Supported
 * @retval #UA_ERROR_PERMISSION_DENIED Permission Denied
 * @retval #UA_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #UA_ERROR_NOT_IN_PROGRESS Operation not in progress
 * @retval #UA_ERROR_OPERATION_FAILED Operation failed
 * @retval #UA_ERROR_NOT_INITIALIZED Not initialized yet
 *
 * @see ua_monitor_start_location_detection()
 */
int ua_monitor_stop_location_detection(
			ua_monitor_h monitor);

/**
 * @brief Creates the user handle.
 * @details This function creates a new user handle with the given
 *    account information. The account parameter represents the user's
 *    unique identifier, such as an email address or username. The
 *    maximum length of the account string is limited to 254 characters.
 *    The created user handle can be used to interact with the user
 *    awareness (UA) framework.
 * @since_tizen 6.5
 *
 * @remarks The @a user handle should be destroyed by using ua_user_destroy().
 *
 * @param[in] account Account information. Maximum Account string
 * length allowed is 254.
 * @param[out] user The user handle
 *
 * @return 0 on success, otherwise a negative error value
 * @retval #UA_ERROR_NONE Successful
 * @retval #UA_ERROR_NOT_SUPPORTED Not Supported
 * @retval #UA_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #UA_ERROR_OUT_OF_MEMORY Out of memory
 * @retval #UA_ERROR_NOT_INITIALIZED Not initialized yet
 *
 * @see ua_user_destroy()
 */
int ua_user_create(const char *account,
		ua_user_h *user);

/**
 * @brief Destroys the user handle.
 * @details This function destroys the user handle created by ua_user_create().
 *    Destroying the user handle removes the user context and releases any
 *    allocated memory associated with it. After calling this function, the
 *    user handle becomes invalid and should not be used again.
 * @since_tizen 6.5
 *
 * @remarks User context will removed and all allocated memory (if any) will be released.
 *
 * @param[in] user The user handle
 *
 * @return 0 on success, otherwise a negative error value
 * @retval #UA_ERROR_NONE Successful
 * @retval #UA_ERROR_NOT_SUPPORTED Not Supported
 * @retval #UA_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #UA_ERROR_NOT_INITIALIZED Not initialized yet
 *
 * @see ua_user_create()
 */
int ua_user_destroy(
		ua_user_h user);

/**
 * @brief Adds the user. Added user is stored in the database
 * and is also available after the application restarts.
 * @details This function adds the user represented by the user
 *    handle to the user awareness (UA) framework's database.
 *    Once added, the user remains registered even after the
 *    application restarts, allowing the application to continue
 *    monitoring the user's presence and location.
 * @since_tizen 6.5
 *
 * @param[in] user The user handle
 *
 * @return 0 on success, otherwise a negative error value
 * @retval #UA_ERROR_NONE Successful
 * @retval #UA_ERROR_NOT_SUPPORTED Not Supported
 * @retval #UA_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #UA_ERROR_OPERATION_FAILED Operation failed
 * @retval #UA_ERROR_NOT_INITIALIZED Not initialized yet
 * @retval #UA_ERROR_ALREADY_REGISTERED Already Registered
 *
 * @see ua_user_remove()
 */
int ua_user_add(ua_user_h user);

/**
 * @brief Removes the user.
 * @details This function removes the user represented by the user
 *    handle from the user awareness (UA) framework's database.
 *    Removing the user handle effectively unregisters the user,
 *    making them unavailable for monitoring by the application.
 * @since_tizen 6.5
 *
 * @param[in] user The user handle
 *
 * @return 0 on success, otherwise a negative error value
 * @retval #UA_ERROR_NONE Successful
 * @retval #UA_ERROR_NOT_SUPPORTED Not Supported
 * @retval #UA_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #UA_ERROR_OPERATION_FAILED Operation failed
 * @retval #UA_ERROR_NOT_INITIALIZED Not initialized yet
 *
 * @see ua_user_add()
 */
int ua_user_remove(ua_user_h user);

/**
 * @brief Gets account info for user handle.
 * @details This function retrieves the account information
 *    associated with the user handle. The account information
 *    is returned as a null-terminated string and should be
 *    freed using g_free() after use.
 * @since_tizen 6.5
 *
 * @remarks You must release @a account using g_free().
 * @param[in] user The user handle
 * @param[out] account  Account information
 *
 * @return 0 on success, otherwise a negative error value
 * @retval #UA_ERROR_NONE Successful
 * @retval #UA_ERROR_NOT_SUPPORTED Not Supported
 * @retval #UA_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #UA_ERROR_OUT_OF_MEMORY Out of memory
 * @retval #UA_ERROR_NOT_INITIALIZED Not initialized yet
 *
 * @see ua_user_create()
 */
int ua_user_get_account(ua_user_h user, char **account);

/**
 * @brief Adds device for a user. This information is stored in database
 * and is also available after the application restarts.
 * @details This function adds a device to the user's profile in the user
 *    awareness (UA) framework's database. The device handle represents
 *    the device to be added, which can be a Bluetooth Low Energy (BLE)
 *    device or a Wi-Fi device, depending on the privileges granted and
 *    the type of device being added.
 * @since_tizen 6.5
 * @remarks If you want to add the BLE device,
 * you should add privilege %http://tizen.org/privilege/bluetooth.
 * if you want to add the Wi-Fi device,
 * you should add privilege %http://tizen.org/privilege/network.get.
 *
 * @param[in] user The user handle.
 * @param[in] device The device handle.
 * @param[in] callback Callback to be invoked after adding the user done.
 * @param[in] user_data The user data to be passed when callback is called.
 *
 * @return 0 on success, otherwise a negative error value
 * @retval #UA_ERROR_NONE Successful
 * @retval #UA_ERROR_NOT_SUPPORTED Not Supported
 * @retval #UA_ERROR_PERMISSION_DENIED Permission Denied
 * @retval #UA_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #UA_ERROR_NOT_INITIALIZED Not initialized yet
 *
 * @see ua_user_remove_device()
 * @see ua_user_device_added_cb()
 */
int ua_user_add_device(
		ua_user_h user,
		ua_device_h device,
		ua_user_device_added_cb callback,
		void *user_data);

/**
 * @brief Removes the device handle for a user handle.
 * @since_tizen 6.5
 * @details This function removes the specified device from the user's
 *    profile in the user awareness (UA) framework's database. The
 *    device handle represents the device to be removed, which can be
 *    a Bluetooth Low Energy (BLE) device or a Wi-Fi device, depending
 *    on the privileges granted and the type of device being removed.
 * @remarks If you want to remove the BLE device,
 * you should add privilege %http://tizen.org/privilege/bluetooth.
 * if you want to remove the Wi-Fi device,
 * you should add privilege %http://tizen.org/privilege/network.get.
 *
 * @param[in] user The user handle.
 * @param[in] device The device handle.
 *
 * @return 0 on success, otherwise a negative error value
 * @retval #UA_ERROR_NONE Successful
 * @retval #UA_ERROR_NOT_SUPPORTED Not Supported
 * @retval #UA_ERROR_PERMISSION_DENIED Permission Denied
 * @retval #UA_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #UA_ERROR_RESOURCE_BUSY Remove done. but another service uses this device.
 * @retval #UA_ERROR_OPERATION_FAILED Operation failed
 * @retval #UA_ERROR_NOT_INITIALIZED Not initialized yet
 *
 * @see ua_user_add_device()
 */
int ua_user_remove_device(
		ua_user_h user,
		ua_device_h device);

/**
 * @brief Retrieves the device handle of all the registered devices for a specific user.
 * @details This function retrieves all the device handles that are registered
 *    for the specified user in the user awareness (UA) framework's database.
 *    The foreach_cb callback function is called once for each registered device,
 *    providing the user and device handles along with the user data provided
 *    during the call.
 * @since_tizen 6.5
 *
 * @param[in] user The user handle.
 * @param[in] foreach_cb Callback function to be invoked with user device handle.
 * @param[in] user_data The user data to be passed when callback is called.
 *
 * @return 0 on success, otherwise a negative error value
 * @retval #UA_ERROR_NONE Successful
 * @retval #UA_ERROR_NOT_SUPPORTED Not Supported
 * @retval #UA_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #UA_ERROR_OUT_OF_MEMORY Out of memory
 * @retval #UA_ERROR_NOT_INITIALIZED Not initialized yet
 *
 * @see ua_registered_device_cb()
 */
int ua_user_foreach_devices(
		ua_user_h user,
		ua_registered_device_cb foreach_cb,
		void *user_data);

/**
 * @brief Creates the service handle.
 * @details This function creates a new service handle with the given
 *     service name information. The service name represents the unique
 *     identifier for the service, such as a service name or ID. The
 *     created service handle can be used to interact with the user
 *     awareness (UA) framework.
 * @since_tizen 6.5
 *
 * @remarks The @a service handle should be destroyed by using ua_service_destroy().
 *
 * @param[in] name The service name information
 * @param[out] service The service handle
 *
 * @return 0 on success, otherwise a negative error value
 * @retval #UA_ERROR_NONE Successful
 * @retval #UA_ERROR_NOT_SUPPORTED Not Supported
 * @retval #UA_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #UA_ERROR_OUT_OF_MEMORY Out of memory
 * @retval #UA_ERROR_NOT_INITIALIZED Not initialized yet
 *
 * @see ua_service_destroy()
 */
int ua_service_create(
	const char *name,
	ua_service_h *service);

/**
 * @brief Destroys the service handle.
 * @details This function destroys the service handle created by
 *    ua_service_create(). Destroying the service handle removes
 *    the service context and releases any allocated memory
 *    associated with it. After calling this function, the
 *    service handle becomes invalid and should not be used again.
 * @since_tizen 6.5
 *
 * @remarks Service context will be removed and all allocated memory (if any) will be released.
 *
 * @param[in] service The service handle
 *
 * @return 0 on success, otherwise a negative error value
 * @retval #UA_ERROR_NONE Successful
 * @retval #UA_ERROR_NOT_SUPPORTED Not Supported
 * @retval #UA_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #UA_ERROR_NOT_INITIALIZED Not initialized yet
 *
 * @see ua_service_create()
 */
int ua_service_destroy(
		ua_service_h service);

/**
 * @brief Adds the service. Added service is stored in database
 * and is also available after the application restarts.
 * @details This function adds the service represented by the service
 *    handle to the user awareness (UA) framework's database. Once added,
 *    the service remains registered even after the application restarts,
 *    allowing the application to continue monitoring the service's
 *    availability and status.
 * @since_tizen 6.5
 *
 * @param[in] service The service handle
 *
 * @return 0 on success, otherwise a negative error value
 * @retval #UA_ERROR_NONE Successful
 * @retval #UA_ERROR_NOT_SUPPORTED Not Supported
 * @retval #UA_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #UA_ERROR_OPERATION_FAILED Operation failed
 * @retval #UA_ERROR_NOT_INITIALIZED Not initialized yet
 *
 * @see ua_service_remove()
 */
int ua_service_add(
		ua_service_h service);

/**
 * @brief Updates the properties for the added service.
 * To update a property of a service its respective setter functions
 * should be called and then followed by the call of ua_service_update().
 * @details To update a property of a service, its respective setter functions
 *    should be called to modify the desired property values, and then ua_service_update()
 *    should be called to commit the changes to the user awareness (UA) framework's database.
 *    This ensures that the updated service properties are persisted and available after
 *    the application restarts.
 * @since_tizen 6.5
 *
 * @param[in] service The service handle
 *
 * @return 0 on success, otherwise a negative error value
 * @retval #UA_ERROR_NONE Successful
 * @retval #UA_ERROR_NOT_SUPPORTED Not Supported
 * @retval #UA_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #UA_ERROR_OPERATION_FAILED Operation failed
 * @retval #UA_ERROR_NOT_INITIALIZED Not initialized yet
 *
 * @see ua_service_create()
 * @see ua_service_remove()
 */
int ua_service_update(
		ua_service_h service);

/**
 * @brief Removes the service.
 * @details This function removes the service represented by the service
 *    handle from the user awareness (UA) framework's database. Removing
 *    the service handle effectively unregisters the service, making it
 *    unavailable for monitoring by the application.
 * @since_tizen 6.5
 *
 * @param[in] service The service handle
 *
 * @return 0 on success, otherwise a negative error value
 * @retval #UA_ERROR_NONE Successful
 * @retval #UA_ERROR_NOT_SUPPORTED Not Supported
 * @retval #UA_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #UA_ERROR_OPERATION_FAILED Operation failed
 * @retval #UA_ERROR_NOT_INITIALIZED Not initialized yet
 *
 * @see ua_service_add()
 */
int ua_service_remove(
		ua_service_h service);

/**
 * @brief Gets name info for service handle.
 * @details This function retrieves the name information associated
 *    with the service handle. The name information is returned as a
 *    null-terminated string and should be freed using g_free() after use.
 * @since_tizen 6.5
 *
 * @remarks You must release @a name using g_free().
 *
 * @param[in] service The service handle
 * @param[out] name The service name information
 *
 * @return 0 on success, otherwise a negative error value
 * @retval #UA_ERROR_NONE Successful
 * @retval #UA_ERROR_NOT_SUPPORTED Not Supported
 * @retval #UA_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #UA_ERROR_NOT_INITIALIZED Not initialized yet
 *
 * @see ua_service_create()
 */
int ua_service_get_name(
		ua_service_h service,
		char **name);

/**
 * @brief Sets detection threshold for service handle.
 * If the service has already been added, this call should be followed by
 * the call of ua_service_update() to reflect the modifications.
 * @details This function sets the detection thresholds for the service handle.
 *    The presence threshold specifies the minimum light level (in lux) required
 *    for the service to be considered present, while the absence threshold
 *    specifies the maximum light level (in lux) required for the service to be
 *    considered absent. If the service has already been added, this call should
 *    be followed by the call of ua_service_update() to reflect the modifications
 *    in the user awareness (UA) framework's database.
 * @since_tizen 6.5
 *
 * @param[in] service The service handle
 * @param[in] presence_threshold The service presence threshold information (in lux)
 * @param[in] absence_threshold The service absence threshold information (in lux)
 *
 * @return 0 on success, otherwise a negative error value
 * @retval #UA_ERROR_NONE Successful
 * @retval #UA_ERROR_NOT_SUPPORTED Not Supported
 * @retval #UA_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #UA_ERROR_NOT_INITIALIZED Not initialized yet
 *
 */
int ua_service_set_detection_threshold(
		ua_service_h service,
		unsigned int presence_threshold,
		unsigned int absence_threshold);

/**
 * @brief Gets detection threshold for service handle.
 * @details This function retrieves the detection thresholds for the service
 *    handle. The presence threshold specifies the minimum light level (in lux)
 *    required for the service to be considered present, while the absence
 *    threshold specifies the maximum light level (in lux) required for the
 *    service to be considered absent.
 * @since_tizen 6.5
 *
 * @param[in] service The service handle
 * @param[out] presence_threshold The service presence threshold information (in lux)
 * @param[out] absence_threshold The service absence threshold information (in lux)
 *
 * @return 0 on success, otherwise a negative error value
 * @retval #UA_ERROR_NONE Successful
 * @retval #UA_ERROR_NOT_SUPPORTED Not Supported
 * @retval #UA_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #UA_ERROR_NOT_INITIALIZED Not initialized yet
 *
 * @see ua_service_set_detection_threshold()
 */
int ua_service_get_detection_threshold(
		ua_service_h service,
		unsigned int *presence_threshold,
		unsigned int *absence_threshold);

/**
 * @brief Sets name info for service handle.
 * @details This function sets the name information for the service
 *    handle. The name information is used to identify the service
 *    within the user awareness (UA) framework.
 * @since_tizen 6.5
 *
 * @remarks The @a user should not be released.
 * @remarks The @a user is available until user is
 * removed and released by application using ua_user_remove()
 * and ua_user_destroy()
 *
 * @param[in] service The service handle
 * @param[in] account The user account information
 * @param[out] user The user handle
 *
 * @return 0 on success, otherwise a negative error value
 * @retval #UA_ERROR_NONE Successful
 * @retval #UA_ERROR_NOT_SUPPORTED Not Supported
 * @retval #UA_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #UA_ERROR_NOT_INITIALIZED Not initialized yet
 *
 */
int ua_service_get_user_by_account(ua_service_h service,
		const char* account, ua_user_h *user);

/**
 * @brief Adds a user for a specific service.
 * @details This function adds a user to a specific service within
 *    the user awareness (UA) framework. Adding a user allows the
 *    service to monitor the user's presence and status.
 * @since_tizen 6.5
 *
 * @param[in] service The service handle.
 * @param[in] user The user handle.
 *
 * @return 0 on success, otherwise a negative error value
 * @retval #UA_ERROR_NONE Successful
 * @retval #UA_ERROR_NOT_SUPPORTED Not Supported
 * @retval #UA_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #UA_ERROR_NOT_INITIALIZED Not initialized yet
 *
 * @see ua_service_remove_user()
 */
int ua_service_add_user(
		ua_service_h service,
		ua_user_h user);

/**
 * @brief Removes the user for the specific service handle.
 * @details This function removes a user from a specific service
 *    within the user awareness (UA) framework. Removing a user
 *    stops the service from monitoring the user's presence and status.
 * @since_tizen 6.5
 *
 * @param[in] service The service handle.
 * @param[in] user The user handle.
 *
 * @return 0 on success, otherwise a negative error value
 * @retval #UA_ERROR_NONE Successful
 * @retval #UA_ERROR_NOT_SUPPORTED Not Supported
 * @retval #UA_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #UA_ERROR_OPERATION_FAILED Operation failed
 * @retval #UA_ERROR_NOT_INITIALIZED Not initialized yet
 *
 * @see ua_service_add_user()
 */
int ua_service_remove_user(
		ua_service_h service,
		ua_user_h user);

/**
 * @brief Triggers callback for every user of a specific service.
 * @details This function iterates through all users associated with
 *    a specific service within the user awareness (UA) framework and
 *    triggers the provided callback function for each user. The
 *    callback function receives the user handle and any additional
 *    user data specified by the caller.
 * @since_tizen 6.5
 *
 * @param[in] service The service handle.
 * @param[in] foreach_cb Callback function to be invoked with user handle.
 * @param[in] user_data The user data to be passed when callback is called.
 *
 * @return 0 on success, otherwise a negative error value
 * @retval #UA_ERROR_NONE Successful
 * @retval #UA_ERROR_NOT_SUPPORTED Not Supported
 * @retval #UA_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #UA_ERROR_NOT_INITIALIZED Not initialized yet
 *
 * @see ua_service_user_cb()
 */
int ua_service_foreach_users(
		ua_service_h service,
		ua_service_user_cb foreach_cb,
		void *user_data);

/**
 * @brief Creates the device handle.
 * @details This function creates a new device handle for a given MAC address
 *    type, MAC address, and device ID. The device handle represents a physical
 *    device within the user awareness (UA) framework and can be used to manage
 *    the device's properties and interactions.
 * @since_tizen 6.5
 *
 * @remarks The @a device handle should be destroyed using ua_user_destroy().
 *
 * @param[in] mac_type The MAC address type of the device
 * @param[in] mac_address The device's MAC address.
 * @param[in] device_id The device ID.
 *
 * @param[out] device The device handle
 *
 * @return 0 on success, otherwise a negative error value
 * @retval #UA_ERROR_NONE Successful
 * @retval #UA_ERROR_NOT_SUPPORTED Not Supported
 * @retval #UA_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #UA_ERROR_OUT_OF_MEMORY Out of memory
 * @retval #UA_ERROR_NOT_INITIALIZED Not initialized yet
 *
 * @see ua_device_destroy()
 */
int ua_device_create(ua_mac_type_e mac_type,
			const char* mac_address,
			const char *device_id,
			ua_device_h *device);

/**
 * @brief Destroys the device handle.
 * @details This function destroys the device handle, removing the device
 *    context and releasing any allocated memory associated with it. After
 *    calling this function, the device handle becomes invalid and should
 *    not be used further.
 * @since_tizen 6.5
 *
 * @remarks Device context will be removed and all allocated memory (if any) will be released.
 *
 * @param[in] device The device handle
 *
 * @return 0 on success, otherwise a negative error value
 * @retval #UA_ERROR_NONE Successful
 * @retval #UA_ERROR_NOT_SUPPORTED Not Supported
 * @retval #UA_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #UA_ERROR_NOT_INITIALIZED Not initialized yet
 *
 * @see ua_device_create()
 */
int ua_device_destroy(
		ua_device_h device);

/**
 * @brief Gets device type info from the device.
 * @details This function retrieves the MAC address type of the device
 *    represented by the device handle. The MAC address type indicates
 *    whether the device uses a 48-bit or 64-bit MAC address.
 * @since_tizen 6.5
 *
 * @param[in] device The device handle
 * @param[out] mac_type The MAC address type of the device
 *
 * @return 0 on success, otherwise a negative error value
 * @retval #UA_ERROR_NONE Successful
 * @retval #UA_ERROR_INVALID_PARAMETER Invalid parameter
 *
 */
int ua_device_get_mac_type(
		ua_device_h device,
		ua_mac_type_e *mac_type);

/**
 * @brief Sets device's operating system info.
 * If the device has already been added, this call should be followed by
 * the call of ua_device_update() to reflect the modifications.
 * @details This function sets the operating system information for the
 *    device represented by the device handle. If the device has already
 *    been added to the user awareness (UA) framework, this call should
 *    be followed by the call of ua_device_update() to reflect the
 *    modifications in the UA framework's database.
 * @since_tizen 6.5
 *
 * @param[in] device The device handle
 * @param[in] os The device's operating system.
 *
 * @return 0 on success, otherwise a negative error value
 * @retval #UA_ERROR_NONE Successful
 * @retval #UA_ERROR_NOT_SUPPORTED Not Supported
 * @retval #UA_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #UA_ERROR_NOT_INITIALIZED Not initialized yet
 *
 * @see ua_device_get_os_info()
 */
int ua_device_set_os_info(
		ua_device_h device,
		ua_os_type_e os);

/**
 * @brief Gets device's operating system info.
 * @details This function retrieves the operating system information
 *    for the device represented by the device handle. The operating
 *    system information indicates which operating system the device
 *    is running.
 * @since_tizen 6.5
 *
 * @param[in] device The device handle
 * @param[out] os_info Device's operating system info.
 *
 * @return 0 on success, otherwise a negative error value
 * @retval #UA_ERROR_NONE Successful
 * @retval #UA_ERROR_INVALID_PARAMETER Invalid parameter
 *
 * @see ua_device_set_os_info()
 */
int ua_device_get_os_info(
		ua_device_h device,
		ua_os_type_e *os_info);

/**
 * @brief Gets device's MAC address.
 * @details This function retrieves the MAC address of the device
 *    represented by the device handle. The MAC address uniquely
 *    identifies the device within the network.
 * @since_tizen 6.5
 *
 * @remarks You must release @a mac_address using g_free().
 *
 * @param[in] device The device handle
 * @param[out] mac_address The device's MAC address.
 *
 * @return 0 on success, otherwise a negative error value
 * @retval #UA_ERROR_NONE Successful
 * @retval #UA_ERROR_INVALID_PARAMETER Invalid parameter
 *
 */
int ua_device_get_mac_address(
		ua_device_h device,
		char **mac_address);

/**
 * @brief Gets device ID of device, which was set while creating the device handle.
 * @details This function retrieves the device ID of the device represented by
 *    the device handle. The device ID is a unique identifier assigned to the
 *    device during its creation.
 * @since_tizen 6.5
 *
 * @remarks You must release @a device_id using g_free().
 * @param[in] device The device handle
 * @param[out] device_id The device ID.
 *
 * @return 0 on success, otherwise a negative error value
 * @retval #UA_ERROR_NONE Successful
 * @retval #UA_ERROR_INVALID_PARAMETER Invalid parameter
 *
 */
int ua_device_get_device_id(
		ua_device_h device,
		char **device_id);

/**
 * @brief Sets device's Wi-Fi BSSID.
 * If the device has already been added, this call should be followed by
 * the call of ua_device_update() to reflect the modifications.
 * @details This function sets the Wi-Fi Basic Service Set Identifier (BSSID)
 *    for the device represented by the device handle. The BSSID is a unique
 *    identifier for the wireless access point that the device is connected
 *    to. If the device has already been added to the user awareness (UA)
 *    framework, this call should be followed by the call of ua_device_update()
 *    to reflect the modifications in the UA framework's database.
 * @since_tizen 6.5
 *
 * @param[in] device The device handle
 * @param[in] bssid  The device's Wi-Fi BSSID.
 *
 * @return 0 on success, otherwise a negative error value
 * @retval #UA_ERROR_NONE Successful
 * @retval #UA_ERROR_NOT_SUPPORTED Not Supported
 * @retval #UA_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #UA_ERROR_NOT_INITIALIZED Not initialized yet
 *
 * @see ua_device_get_wifi_bssid()
 */
int ua_device_set_wifi_bssid(
		ua_device_h device,
		const char *bssid);

/**
 * @brief Gets device's Wi-Fi BSSID.
 * @details This function retrieves the Wi-Fi Basic Service Set Identifier
 *    (BSSID) for the device represented by the device handle. The BSSID
 *    is a unique identifier for the wireless access point that the device
 *    is connected to.
 * @since_tizen 6.5
 *
 * @remarks You must release @a bssid using g_free().
 * @param[in] device The device handle
 * @param[out] bssid The device's Wi-Fi BSSID.
 *
 * @return 0 on success, otherwise a negative error value
 * @retval #UA_ERROR_NONE Successful
 * @retval #UA_ERROR_INVALID_PARAMETER Invalid parameter
 *
 * @see ua_device_set_wifi_bssid()
 */
int ua_device_get_wifi_bssid(
		ua_device_h device,
		char **bssid);
/**
 * @brief Sets device's Wi-Fi IPv4 address.
 * If the device has already been added, this call should be followed by
 * the call of ua_device_update() to reflect the modifications.
 * @details This function sets the IPv4 address of the device's Wi-Fi interface
 *    for the device represented by the device handle. The IPv4 address is a
 *    unique identifier for the device within the network. If the device has
 *    already been added to the user awareness (UA) framework, this call should
 *    be followed by the call of ua_device_update() to reflect the modifications
 *    in the UA framework's database.
 * @since_tizen 6.5
 *
 * @param[in] device The device handle
 * @param[in] ipv4_address The device's wifi IPv4 address.
 *
 * @return 0 on success, otherwise a negative error value
 * @retval #UA_ERROR_NONE Successful
 * @retval #UA_ERROR_NOT_SUPPORTED Not Supported
 * @retval #UA_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #UA_ERROR_NOT_INITIALIZED Not initialized yet
 *
 * @see ua_device_get_wifi_ipv4_address()
 */
int ua_device_set_wifi_ipv4_address(
		ua_device_h device,
		const char *ipv4_address);

/**
 * @brief Gets device's Wi-Fi IPv4 address.
 * @details This function retrieves the IPv4 address of the device's
 *    Wi-Fi interface for the device represented by the device handle.
 *    The IPv4 address is a unique identifier for the device within
 *    the network.
 * @since_tizen 6.5
 *
 * @remarks You must release @a ipv4_address using g_free().
 *
 * @param[in] device The device handle
 * @param[out] ipv4_address The device's Wi-Fi IPv4 address.
 *
 * @return 0 on success, otherwise a negative error value
 * @retval #UA_ERROR_NONE Successful
 * @retval #UA_ERROR_INVALID_PARAMETER Invalid parameter
 *
 * @see ua_device_set_wifi_ipv4_address()
 */
int ua_device_get_wifi_ipv4_address(
		ua_device_h device,
		char **ipv4_address);

/**
 * @brief Gets last presence time for device handle.
 * @details This function retrieves the last presence timestamp for the device
 *    represented by the device handle. The last presence timestamp indicates
 *    the most recent time at which the device was detected or interacted with
 *    within the user awareness (UA) framework.
 * @since_tizen 6.5
 *
 * @param[in] device The device handle
 * @param[out] last_seen The last presence monotonic timestamp for UA device (in msec)
 *
 * @return 0 on success, otherwise a negative error value
 * @retval #UA_ERROR_NONE Successful
 * @retval #UA_ERROR_INVALID_PARAMETER Invalid parameter
 *
 */
int ua_device_get_last_presence(
		ua_device_h device,
		unsigned long long *last_seen);

/**
 * @brief Gets whether pairing is required for the user device.
 * @details This function retrieves the pairing requirement status for the
 *    device represented by the device handle. Pairing is a process where
 *    two devices establish a secure connection and exchange authentication
 *    credentials. If pairing is required, it means that the device needs
 *    to go through the pairing process before it can be used or accessed.
 * @since_tizen 6.5
 *
 * @param[in] device The device handle
 * @param[out] pairing_required TRUE if pairing is required, FALSE otherwise.
 *
 * @return 0 on success, otherwise a negative error value
 * @retval #UA_ERROR_NONE Successful
 * @retval #UA_ERROR_NOT_SUPPORTED Not Supported
 * @retval #UA_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #UA_ERROR_OUT_OF_MEMORY Out of memory
 * @retval #UA_ERROR_NOT_INITIALIZED Not initialized yet
 *
 * @see ua_device_create()
 */
int ua_device_get_pairing_required(
		ua_device_h device,
		bool *pairing_required);

/**
 * @brief Updates the properties for the added device.
 * To update a property of a device its respective setter functions
 * should be called and then followed by the call of ua_device_update().
 * @details This function updates the properties of an existing device in
 *    the user awareness (UA) framework. To update a property of a device,
 *    its respective setter functions should be called first, and then
 *    followed by the call of ua_device_update(). This ensures that the
 *    changes made to the device's properties are reflected in the UA
 *    framework's database.
 * @since_tizen 6.5
 * @remarks If you want to update BLE device,
 * you should add privilege %http://tizen.org/privilege/bluetooth.
 * if you want to update Wi-Fi device,
 * you should add privilege %http://tizen.org/privilege/network.get.
 *
 * @param[in] device The device handle
 *
 * @return 0 on success, otherwise a negative error value
 * @retval #UA_ERROR_NONE Successful
 * @retval #UA_ERROR_NOT_SUPPORTED Not Supported
 * @retval #UA_ERROR_PERMISSION_DENIED Permission Denied
 * @retval #UA_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #UA_ERROR_OPERATION_FAILED Operation failed
 * @retval #UA_ERROR_NOT_INITIALIZED Not initialized yet
 *
 * @see ua_device_set_os_info()
 * @see ua_device_set_wifi_ipv4_address()
 */
int ua_device_update(
		ua_device_h device);

/**
 * @brief Gets device handle by MAC address.
 * @details This function retrieves the device handle for a specific device
 *    identified by its MAC address. The MAC address is a unique identifier
 *    for the device within the network. The retrieved device handle can be
 *    used to access and manipulate the device's properties and functionalities
 *    within the user awareness (UA) framework.
 * @since_tizen 6.5
 *
 * @remarks The @a device handle should not be released.
 * @remarks The @a device handle is available until device is
 * removed and released by application using ua_user_remove_device()
 * and ua_device_destroy()
 *
 * @param[in] mac The device's MAC address
 * @param[out] device The device handle
 *
 * @return 0 on success, otherwise a negative error value
 * @retval #UA_ERROR_NONE Successful
 * @retval #UA_ERROR_NOT_SUPPORTED Not Supported
 * @retval #UA_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #UA_ERROR_OUT_OF_MEMORY Out of memory
 * @retval #UA_ERROR_NOT_INITIALIZED Not initialized yet
 *
 */
int ua_device_get_by_mac_address(
		const char *mac,
		ua_device_h *device);

/**
 * @brief Gets device handle by device ID.
 * @details This function retrieves the device handle for a specific device
 *    identified by its device ID and MAC type. The device ID is a unique
 *    identifier assigned to the device during its creation, while the MAC
 *    type specifies the type of MAC address (e.g., Wi-Fi, Bluetooth)
 *    associated with the device. The retrieved device handle can be used
 *    to access and manipulate the device's properties and functionalities
 *    within the user awareness (UA) framework.
 * @since_tizen 6.5
 *
 * @remarks The @a device handle should not be released.
 * @remarks The @a device handle is available until device is
 * removed and released by application using ua_user_remove_device()
 * and ua_device_destroy()
 *
 * @param[in] device_id The device ID
 * @param[in] mac_type The device's MAC type
 * @param[out] device The device handle.
 *
 * @return 0 on success, otherwise a negative error value
 * @retval #UA_ERROR_NONE Successful
 * @retval #UA_ERROR_NOT_SUPPORTED Not Supported
 * @retval #UA_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #UA_ERROR_OUT_OF_MEMORY Out of memory
 * @retval #UA_ERROR_NOT_INITIALIZED Not initialized yet
 *
 */
int ua_device_get_by_device_id(
		const char *device_id,
		ua_mac_type_e mac_type,
		ua_device_h *device);

/**
 * @brief Gets sensor's status report.
 * @details This function retrieves the status report of the sensor
 *    represented by the sensor handle. The status report provides
 *    information about the current state and operational status of
 *    the sensor, such as whether it is active, idle, or experiencing
 *    any issues.
 * @since_tizen 6.5
 *
 * @param[in] sensor The sensor handle
 * @param[out] report The sensor's status report.
 *
 * @return 0 on success, otherwise a negative error value
 * @retval #UA_ERROR_NONE Successful
 * @retval #UA_ERROR_NOT_SUPPORTED Not Supported
 * @retval #UA_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #UA_ERROR_NOT_INITIALIZED Not initialized yet
 *
 */
int ua_sensor_get_status(
		ua_sensor_h sensor,
		ua_sensor_report_e *report);

/**
 * @brief Gets sensor's timestamp.
 * @details This function retrieves the timestamp of the sensor represented
 *    by the sensor handle. The timestamp indicates the time at which the
 *    sensor data was captured or generated. It can be useful for determining
 *    the freshness or recency of the sensor readings.
 * @since_tizen 6.5
 *
 * @param[in] sensor The sensor handle
 * @param[out] timestamp The sensor's timestamp.
 *
 * @return 0 on success, otherwise a negative error value
 * @retval #UA_ERROR_NONE Successful
 * @retval #UA_ERROR_NOT_SUPPORTED Not Supported
 * @retval #UA_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #UA_ERROR_NOT_INITIALIZED Not initialized yet
 *
 */
int ua_sensor_get_timestamp(
		ua_sensor_h sensor,
		unsigned long long *timestamp);

/**
 * @brief Gets sensor's type.
 * @details This function retrieves the type of the sensor represented by
 *    the sensor handle. The sensor type indicates the kind of sensor,
 *    such as temperature, humidity, motion, etc. Knowing the sensor type
 *    helps in understanding the nature of the sensor and the kind of data
 *    it provides.
 * @since_tizen 6.5
 *
 * @param[in] sensor The sensor handle
 * @param[out] type The sensor's type.
 *
 * @return 0 on success, otherwise a negative error value
 * @retval #UA_ERROR_NONE Successful
 * @retval #UA_ERROR_NOT_SUPPORTED Not Supported
 * @retval #UA_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #UA_ERROR_NOT_INITIALIZED Not initialized yet
 *
 */
int ua_sensor_get_type(
		ua_sensor_h sensor,
		ua_sensor_type_e *type);

/**
 * @brief Gets the distance for the location handle.
 * @details This function retrieves the distance between the user and the
 *    detected device, represented by the location handle. The distance is
 *    measured in a unit specified by the implementation, such as meters
 *    or feet. This information can be useful for determining the proximity
 *    of the device to the user.
 * @since_tizen 6.5
 * @privlevel public
 * @privilege %http://tizen.org/privilege/location
 *
 * @param[in] location The location handle for location information.
 * @param[out] distance distance of the detected device and user.
 *
 * @return 0 on success, otherwise a negative error value
 * @retval #UA_ERROR_NONE Successful
 * @retval #UA_ERROR_PERMISSION_DENIED Permission Denied
 * @retval #UA_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #UA_ERROR_NOT_INITIALIZED Not initialized yet
 */
int ua_location_get_distance(ua_location_h location, int *distance);

/**
 * @brief Gets the position for the location handle.
 * @details This function retrieves the position coordinates (x, y, z) of
 *    the detected device, represented by the location handle. The position
 *    coordinates indicate the spatial location of the device within a
 *    coordinate system defined by the implementation. This information can
 *    be useful for determining the exact location of the device relative
 *    to the user or other reference points.
 * @since_tizen 6.5
 * @privlevel public
 * @privilege %http://tizen.org/privilege/location
 *
 * @param[in] location The location handle for location information.
 * @param[out] x x-coordinate of the location.
 * @param[out] y y-coordinate of the location.
 * @param[out] z z-coordinate of the location.
 *
 * @return 0 on success, otherwise a negative error value
 * @retval #UA_ERROR_NONE Successful
 * @retval #UA_ERROR_PERMISSION_DENIED Permission Denied
 * @retval #UA_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #UA_ERROR_NOT_INITIALIZED Not initialized yet
 */
int ua_location_get_position(ua_location_h location,
					int *x, int *y, int *z);

/**
 * @}
 */

#ifdef __cplusplus
}
#endif
#endif /* __TIZEN_CAPI_NETWORK_USER_AWARENESS_H__ */
