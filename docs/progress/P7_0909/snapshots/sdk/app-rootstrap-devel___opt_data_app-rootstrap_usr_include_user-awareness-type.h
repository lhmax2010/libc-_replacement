/*
 * Copyright (c) 2021 Samsung Electronics Co., Ltd. All rights reserved.
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

#ifndef __TIZEN_CAPI_NETWORK_USER_AWARENESS_TYPE_H__
#define __TIZEN_CAPI_NETWORK_USER_AWARENESS_TYPE_H__

#include <stdbool.h>
#include <glib.h>
#include <tizen.h>
#include <tizen_error.h>

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @file user-awareness-type.h
 */

#ifndef TIZEN_ERROR_USER_AWARENESS
#define TIZEN_ERROR_USER_AWARENESS 0x11170000
#endif

/**
 * @ingroup CAPI_NETWORK_FRAMEWORK
 * @addtogroup CAPI_NETWORK_UA_MODULE
 * @{
 */

/**
 * @brief Max. MAC address length.
 * @since_tizen 6.5
 */
#define UA_MAC_ADDRESS_STRING_LEN 18

/**
 * @brief Max. IPv4 address length.
 * @since_tizen 6.5
 */
#define UA_IPV4_ADDRESS_STRING_LEN 16

/**
 * @brief Max. IP address length (e.g. IPv6).
 * @since_tizen 6.5
 */
#define UA_IP_ADDRESS_MAX_STRING_LEN 50

/**
 * @brief Max. device ID length (e.g. UUID).
 * @since_tizen 6.5
 */
#define UA_MOBILE_ID_STRING_LEN 50

/**
 * @brief Max. user name length.
 * @since_tizen 6.5
 */
#define UA_USER_NAME_MAX_STRING_LEN 50

/**
 * @brief Max. user account length.
 * @since_tizen 6.5
 */
#define UA_USER_ACCOUNT_MAX_STRING_LEN 70

/**
 * @brief Device scan time multiplier.
 * @since_tizen 6.5
 */
#define UA_SCAN_TIME_MULTIPLIER 10

/**
 * @brief Max. service name length.
 * @since_tizen 6.5
 */
#define UA_MAX_SERVICE_LEN 50

/**
 * @brief Window step multiplier.
 * @details Detection window step is 5 seconds.
 * @since_tizen 6.5
 */
#define UA_DETECTION_WINDOW_STEP 5

/**
 * @brief Detection cycle multiplier.
 * @details Detection cycle step is 60 seconds.
 * @since_tizen 6.5
 */
#define UA_DETECTION_CYCLE_STEP	60

/**
 * @brief Default location callback period (in msec).
 * @since_tizen 6.5
 */
#define UA_LOCATION_PERIOD_DEFAULT 1000

/**
 * @brief Wifi location sensor.
 * @since_tizen 6.5
 */
#define UA_SENSOR_WIFI_LOCATION 0x00000040

/**
 * @brief Wifi location mac type.
 * @since_tizen 6.5
 */
#define UA_MAC_TYPE_WIFI_LOCATION 0x10

/**
 * @brief UWB sensor.
 * @since_tizen 6.5
 */
#define UA_SENSOR_UWB 0x00000080

/**
 * @brief UWB mac type.
 * @since_tizen 6.5
 */
#define UA_MAC_TYPE_UWB 0x20


/**
 * @brief Enumeration for UA framework error codes.
 * @since_tizen 6.5
 */
typedef enum {
	UA_ERROR_NONE = TIZEN_ERROR_NONE, /**< Successful*/
	UA_ERROR_NOT_PERMITTED = TIZEN_ERROR_NOT_PERMITTED, /**< Operation not permitted */
	UA_ERROR_INVALID_PARAMETER = TIZEN_ERROR_INVALID_PARAMETER, /**< Invalid parameter */
	UA_ERROR_OUT_OF_MEMORY = TIZEN_ERROR_OUT_OF_MEMORY, /**< Out of memory */
	UA_ERROR_PERMISSION_DENIED = TIZEN_ERROR_PERMISSION_DENIED, /**< Permission denied */
	UA_ERROR_RESOURCE_BUSY = TIZEN_ERROR_RESOURCE_BUSY, /**< Device or resource busy */
	UA_ERROR_TIMED_OUT = TIZEN_ERROR_TIMED_OUT, /**< Timeout error */
	UA_ERROR_CANCELED = TIZEN_ERROR_CANCELED, /**< Operation canceled */
	UA_ERROR_NOW_IN_PROGRESS = TIZEN_ERROR_NOW_IN_PROGRESS, /**< Operation now in progress */
	UA_ERROR_NOT_SUPPORTED = TIZEN_ERROR_NOT_SUPPORTED, /**< Not Supported */
	UA_ERROR_NO_DATA = TIZEN_ERROR_NO_DATA, /**< No data available */
	UA_ERROR_NOT_INITIALIZED = TIZEN_ERROR_USER_AWARENESS | 0x01, /**< Not initialized */
	UA_ERROR_NOT_IN_PROGRESS = TIZEN_ERROR_USER_AWARENESS | 0x02, /**< Operation Not in progress */
	UA_ERROR_ALREADY_DONE = TIZEN_ERROR_USER_AWARENESS | 0x03, /**< Operation already done */
	UA_ERROR_OPERATION_FAILED = TIZEN_ERROR_USER_AWARENESS | 0x04, /**< Operation Failed */
	UA_ERROR_NOT_READY = TIZEN_ERROR_USER_AWARENESS | 0x05, /**< Resource not ready */
	UA_ERROR_NOT_ENABLED = TIZEN_ERROR_USER_AWARENESS | 0x06, /**< Not enabled */
	UA_ERROR_NOT_FOUND = TIZEN_ERROR_USER_AWARENESS | 0x07, /**< Not found */
	UA_ERROR_ALREADY_REGISTERED = TIZEN_ERROR_USER_AWARENESS | 0x08, /**< Already registered */
	UA_ERROR_DB_FAILED = TIZEN_ERROR_USER_AWARENESS | 0x09, /**< DB operation failed */
	UA_ERROR_NOT_REGISTERED = TIZEN_ERROR_USER_AWARENESS | 0x0a, /**< Not registered */
} ua_error_e;

/**
 * @brief Enumeration for UA framework supported sensors.
 * @since_tizen 6.5
 */
typedef enum {
	UA_SENSOR_BT = 0x00000001, /**< BT Sensor */
	UA_SENSOR_BLE = 0x00000002, /**< BLE Sensor */
	UA_SENSOR_WIFI = 0x00000004, /**< Wi-Fi Sensor */
	UA_SENSOR_MOTION = 0x00000008, /**< Motion Sensor */
	UA_SENSOR_LIGHT = 0x00000010, /**< Light Sensor */
	UA_SENSOR_AUDIO = 0x00000020, /**< Audio Sensor */
	UA_SENSOR_MAX,
	UA_SENSOR_ALL = 0xFFFFFFFF /**< All sensors */
} ua_sensor_type_e;

/**
 * @brief Enumeration for UA framework user detection modes.
 * @since_tizen 6.5
 */
typedef enum {
	UA_DETECT_MODE_ALL_SENSORS = 0x01, /**< Detection mode all */
	UA_DETECT_MODE_ANY_SENSOR, /**< Detection mode any */
	UA_DETECT_MODE_INVALID /**< <Invalid detection mode */
} ua_detection_mode_e;

/**
 * @brief Enumeration for User device Operating Systems.
 * @since_tizen 6.5
 */
typedef enum {
	UA_OS_TYPE_NOT_DEFINED = 0x00, /**< OS not defined */
	UA_OS_TYPE_TIZEN, /**< Tizen */
	UA_OS_TYPE_ANDROID, /**< Android */
	UA_OS_TYPE_IOS, /**< iOS */
} ua_os_type_e;

/**
 * @brief Enumeration for User device MAC address types.
 * @since_tizen 6.5
 */
typedef enum {
	UA_MAC_TYPE_NONE = 0x00, /**< None */
	UA_MAC_TYPE_BT = 0x01, /**< BT MAC */
	UA_MAC_TYPE_BLE = 0x02, /**< BLE MAC */
	UA_MAC_TYPE_WIFI = 0x04, /**< Wi-Fi station mode MAC */
	UA_MAC_TYPE_P2P = 0x08, /**< Wi-Fi P2P mode MAC */
	UA_MAC_TYPE_INVALID
} ua_mac_type_e;

/**
 * @brief Enumeration for active scan type.
 * @since_tizen 6.5
 */
typedef enum {
	UA_ACTIVE_SCAN_TYPE_DEVICE_FOUND = 0x00, /**< Device found */
	UA_ACTIVE_SCAN_TYPE_COMPLETED = 0x01 /**< SCAN Completed */
} ua_active_scan_type_e;

/**
 * @brief Enumeration for sensor status report.
 * @since_tizen 6.5
 */
typedef enum {
	UA_SENSOR_REPORT_PRESENCE = 0x01, /**< Sensor reports PRESENCE status */
	UA_SENSOR_REPORT_ABSENCE = 0x02 /**< Sensor reports ABSENCE status */
} ua_sensor_report_e;

/**
 * @brief The handle of sensor information.
 * @since_tizen 6.5
 */
typedef void *ua_sensor_h;

/**
 * @brief The handle of user presence/absence monitor.
 * @since_tizen 6.5
 */
typedef void *ua_monitor_h;

/**
 * @brief The handle of user information.
 * @since_tizen 6.5
 */
typedef void *ua_user_h;

/**
 * @brief The handle of service information.
 * @since_tizen 6.5
 */
typedef void *ua_service_h;

/**
 * @brief The handle of device information.
 * @since_tizen 6.5
 */
typedef void *ua_device_h;

/**
 * @brief The handle of location information.
 * @since_tizen 6.5
 */
typedef void *ua_location_h;

/**
 * @brief Callback to be invoked if a sensor status changed (presence <-> absence)
 *        immediately during detection operation.
 * @since_tizen 6.5
 *
 * @remarks The @a monitor handle should not be released.
 * @remarks The @a monitor handle can be used only in the callback.
 * @remarks The @a sensor handle should not be released.
 * @remarks The @a sensor handle can be used only in the callback.
 *
 * @param[in] monitor The monitor handle for which sensor type was added.
 * @param[in] sensor The list of sensor handles on which user is not detected.
 * @param[in] sensor_type The type of sensor detected.
 * @param[in] report The status report of sensor detected.
 * @param[in] user_data The user data passed in ua_monitor_set_sensor_status_changed_cb().
 *
 */
typedef void (*ua_sensor_status_changed_cb)(
		ua_monitor_h monitor,
		ua_sensor_h sensor,
		ua_sensor_type_e sensor_type,
		ua_sensor_report_e report,
		void *user_data);

/**
 * @brief Callback to get all sensors for a handle.
 * @since_tizen 6.5
 *
 * @param[in] sensor_type Sensor type which is present in UA monitor.
 * @param[in] user_data User data passed in ua_monitor_foreach_sensor().
 *
 * @return @c true to continue with the next iteration of the loop, @c false to break out of the loop
 * and False to break out of the loop
 *
 * @see ua_monitor_foreach_sensor()
 */
typedef bool (*ua_monitor_sensor_cb)(
		ua_sensor_type_e sensor_type,
		void *user_data);

/**
 * @brief Callback to be invoked on completion scan for each device one by one.
 * @since_tizen 6.5
 *
 * @remarks The @a monitor handle should not be released.
 * @remarks The @a monitor handle can be used only in the callback.
 * @remarks The @a device handle should not be released.
 * @remarks The @a device handle can be used only in the callback.
 *
 * @param[in] type To indicate if active scan type is set to device being found or scan complete.
 * @param[in] monitor The monitor handle
 * @param[in] device The device handle to detected device's information. When scan
 *  is complete, it is set to NULL.
 * @param[in] user_data The user data passed in ua_monitor_start_scan()
 *
 * @see ua_monitor_start_scan()
 */
typedef void (*ua_scan_completed_cb)(
		ua_active_scan_type_e type,
		ua_monitor_h monitor,
		ua_device_h device,
		void *user_data);

/**
 * @brief Callback to be invoked on detection of user presence.
 * @details Following error codes can be delivered:
 * #UA_ERROR_NONE Successful.
 * #UA_ERROR_OPERATION_FAILED Operation failed
 * @since_tizen 6.5
 *
 * @remarks The @a monitor handle should not be released.
 * @remarks The @a monitor handle can be used only in the callback.
 * @remarks The @a device handle should not be released.
 * @remarks The @a device handle can be used only in the callback.
 * @remarks The @a sensor handle should not be released.
 * @remarks The @a sensor handle can be used only in the callback.
 * @remarks The @a service handle should not be released.
 * @remarks The @a service handle can be used only in the callback.
 *
 * @param[in] result The result error code of the requested operation.
 * result is #UA_ERROR_NONE on success, otherwise appropriate error value.
 * @param[in] monitor The monitor handle
 * @param[in] sensor_type Sensor type, In case monitor has more than one sensor and detection
 *  mode is not #UA_DETECT_MODE_ANY_SENSOR, sensor will be set to last sensor which
 *  reported user presence before invoking callback.
 * @param[in] service The service handle with which presence detection start was invoked.
 * @param[in] device The first device handle on which sensor is detected.
 * @param[in] sensor The handle of the sensor which reported user/device presence.
 * @param[in] user_data The user data passed in ua_monitor_start_presence_detection()
 *
 * @see ua_monitor_start_presence_detection()
 */
typedef void (*ua_presence_detected_cb)(
		ua_error_e result,
		ua_monitor_h monitor,
		ua_sensor_type_e sensor_type,
		ua_service_h service,
		ua_device_h device,
		ua_sensor_h sensor,
		void *user_data);

/**
 * @brief Callback to be invoked on detection of user absence.
 * @details Following error codes can be delivered:
 * #UA_ERROR_NONE Successful.
 * #UA_ERROR_OPERATION_FAILED Operation failed
 * @since_tizen 6.5
 *
 * @remarks The @a monitor handle should not be released.
 * @remarks The @a monitor handle can be used only in the callback.
 * @remarks The @a sensor handle should not be released.
 * @remarks The @a sensor handle can be used only in the callback.
 * @remarks The @a service handle should not be released.
 * @remarks The @a service handle can be used only in the callback.
 *
 * @param[in] result The result error code of the requested operation.
 * result is #UA_ERROR_NONE on success, otherwise appropriate error value.
 * @param[in] monitor The monitor handle with which absence detection start was invoked.
 * @param[in] service The service handle with which absence detection start was invoked.
 * @param[in] sensor_type Sensor type by which absence was detected.
 * @param[in] sensor The sensor handle for the detected sensor.
 * @param[in] user_data The user data passed in ua_monitor_start_absence_detection()
 *
 * @see ua_monitor_start_absence_detection()
 */
typedef void (*ua_absence_detected_cb)(
		ua_error_e result,
		ua_monitor_h monitor,
		ua_service_h service,
		ua_sensor_type_e sensor_type,
		ua_sensor_h sensor,
		void *user_data);

/**
 * @brief Callback to get all registered devices one by one.
 * @since_tizen 6.5
 *
 * @remarks The @a device handle should not be released.
 * @remarks The @a device handle can be used only in the callback.
 *
 * @param[in] device The device handle.
 * @param[in] user_data User data passed in ua_user_foreach_devices().
 *
 * @return @c true to continue with the next iteration of the loop, @c false to break out of the loop
 *
 * @see ua_user_foreach_devices()
 */
typedef bool (*ua_registered_device_cb)(
		ua_device_h device,
		void *user_data);

/**
 * @brief Callback to get all service handles.
 * @since_tizen 6.5
 *
 * @remarks The @a service handle should not be released.
 * @remarks The @a service handle can be used only in the callback.
 *
 * @param[in] service The service handle.
 * @param[in] user_data User data passed in ua_foreach_service().
 *
 * @return @c true to continue with the next iteration of the loop, @c false to break out of the loop
 *
 * @see ua_foreach_service()
 */
typedef bool (*ua_service_cb)(
		ua_service_h service,
		void *user_data);

/**
 * @brief Callback to get the user handle added to the service.
 * @since_tizen 6.5
 *
 * @remarks The @a service handle should not be released.
 * @remarks The @a service handle can be used only in the callback.
 * @remarks The @a user handle should not be released.
 * @remarks The @a user handle can be used only in the callback.
 *
 * @param[in] service The service handle.
 * @param[in] user The user handle.
 * @param[in] user_data User data passed in ua_service_foreach_users().
 *
 * @return @c true to continue with the next iteration of the loop, @c false to break out of the loop
 *
 * @see ua_service_foreach_users()
 */
typedef bool (*ua_service_user_cb)(
		ua_service_h service,
		ua_user_h user,
		void *user_data);

/**
 * @brief Callback to be invoked when adding a new device to UA is finished.
 * @details Following error codes can be delivered:
 * #UA_ERROR_NONE Successful.
 * #UA_ERROR_OPERATION_FAILED Operation failed
 * @since_tizen 6.5
 *
 * @remarks The @a device handle should not be released.
 * @remarks The @a device handle can be used only in the callback.
 *
 * @param[in] result The result error code of the requested operation.
 * result is #UA_ERROR_NONE on success, otherwise appropriate error value.
 * @param[in] device The added device's handle
 * @param[in] user_data The user data passed in ua_user_add_device()
 *
 * @see ua_user_add_device()
 */
typedef void (*ua_user_device_added_cb)(
		ua_error_e result,
		ua_device_h device,
		void *user_data);

/**
 * @brief Callback to be invoked on detection of user and device location.
 * It is invoked once for each user/device in location callback.
 * Callback period can be set by ua_monitor_set_location_period().
 * @details Following error codes can be delivered:
 * #UA_ERROR_NONE Successful.
 * #UA_ERROR_OPERATION_FAILED Operation failed
 * @since_tizen 6.5
 *
 * @remarks The @a monitor handle should not be released and can be used only in the callback.
 * @remarks The @a service handle should not be released and can be used only in the callback.
 * @remarks The @a sensor handle should not be released and can be used only in the callback.
 * @remarks The @a device handle should not be released and can be used only in the callback.
 * @remarks The @a user handle should not be released and can be used only in the callback.
 * @remarks The @a location handle should not be released and can be used only in the callback.
 *
 * @param[in] result The result error code of the requested operation.
 * result is #UA_ERROR_NONE on success, otherwise appropriate error value.
 * @param[in] monitor The monitor handle
 * @param[in] service The service handle with which location detection start was invoked.
 * @param[in] sensor_type Sensor type
 * @param[in] sensor The handle of the sensor which reported user/device location.
 * @param[in] device The device handle on which sensor is detected.
 * @param[in] user The user handle on which sensor is detected.
 * @param[in] location The location handle.
 * @param[in] user_data The user data passed in ua_monitor_start_location_detection()
 *
 * @see ua_monitor_set_location_period()
 * @see ua_monitor_start_location_detection()
 */
typedef void (*ua_location_detected_cb)(
		ua_error_e result,
		ua_monitor_h monitor,
		ua_service_h service,
		ua_sensor_type_e sensor_type,
		ua_sensor_h sensor,
		ua_device_h device,
		ua_user_h user,
		ua_location_h location,
		void *user_data);

/**
 * @}
 */

#ifdef __cplusplus
}
#endif
#endif /* __TIZEN_CAPI_NETWORK_USER_AWARENESS_TYPE_H__ */
