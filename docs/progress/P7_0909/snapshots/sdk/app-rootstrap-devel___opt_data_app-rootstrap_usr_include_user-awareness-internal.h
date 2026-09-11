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

#ifndef __TIZEN_CAPI_NETWORK_USER_AWARENESS_INTERNAL_H__
#define __TIZEN_CAPI_NETWORK_USER_AWARENESS_INTERNAL_H__

#include <tizen.h>

#include <user-awareness-internal-type.h>

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @file user-awareness-internal.h
 */

/**
 * @internal
 * @ingroup CAPI_NETWORK_UA_MODULE
 * @brief Gets default user handle.
 * @since_tizen 6.5
 *
 * @remarks The @a user_handle should be released using #ua_user_destroy().
 *
 * @param[out] user_handle The user handle
 *
 * @return 0 on success, otherwise a negative error value
 * @retval #UA_ERROR_NONE Successful
 * @retval #UA_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #UA_ERROR_OPERATION_FAILED Operation failed
 * @retval #UA_ERROR_NOT_INITIALIZED Not initialized yet
 *
 * @exception
 * @pre
 * @post
 *
 * @see ua_user_get_account()
 */
int ua_get_default_user(ua_user_h *user_handle);

/**
 * @internal
 * @ingroup CAPI_NETWORK_UA_MODULE
 * @brief Retrieves the user handle of all the registered users.
 * @since_tizen 6.5
 *
 * @param[in] foreach_cb Callback function to be invoked with user handle.
 * @param[in] user_data The user data to be passed when callback is called.
 *
 * @return 0 on success, otherwise a negative error value
 * @retval #UA_ERROR_NONE Successful
 * @retval #UA_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #UA_ERROR_OUT_OF_MEMORY Out of memory
 * @retval #UA_ERROR_OPERATION_FAILED Operation failed
 * @retval #UA_ERROR_NOT_INITIALIZED Not initialized yet
 *
 * @exception
 * @pre
 * @post
 *
 * @see ua_registered_user_cb()
 */
int ua_foreach_users(
	ua_registered_user_cb foreach_cb,
	void *user_data);

/**
 * @ingroup CAPI_NETWORK_UA_MODULE
 * @brief Retrieves the device handle of all the registered devices.
 * @since_tizen 6.5
 *
 * @param[in] foreach_cb Callback function to be invoked with all device handle.
 * @param[in] user_data The user data to be passed when callback is called.
 *
 * @return 0 on success, otherwise a negative error value
 * @retval #UA_ERROR_NONE Successful
 * @retval #UA_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #UA_ERROR_OUT_OF_MEMORY Out of memory
 * @retval #UA_ERROR_NOT_INITIALIZED Not initialized yet
 *
 * @exception
 * @pre
 * @post
 *
 * @see ua_registered_device_cb()
 */
int ua_foreach_devices(
		ua_registered_device_cb foreach_cb,
		void *user_data);

/**
 * @internal
 * @ingroup CAPI_NETWORK_UA_MODULE
 * @brief Sets the brightness absence and presence threshold level.
 * @since_tizen 6.5
 *
 * @param[in] handle The monitor handle
 * @param[in] presence_threshold Presence threshold (Unit. Lux).
 * @param[in] absence_threshold Absence threshold (Unit. Lux).
 *
 * @return 0 on success, otherwise a negative error value
 * @retval #UA_ERROR_NONE Successful
 * @retval #UA_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #UA_ERROR_NOT_INITIALIZED Not initialized yet
 */
int ua_monitor_set_brightness_threshold(ua_monitor_h handle,
	int presence_threshold, int absence_threshold);

/**
 * @internal
 * @ingroup CAPI_NETWORK_UA_MODULE
 * @brief Enables low power mode for host device.
 * @since_tizen 6.5
 *
 * @return 0 on success, otherwise a negative error value
 * @retval #UA_ERROR_NONE Successful
 * @retval #UA_ERROR_NOT_INITIALIZED Not initialized yet
 */
int ua_enable_low_power_mode(void);

/**
 * @internal
 * @ingroup CAPI_NETWORK_UA_MODULE
 * @brief Disables low power mode for host device.
 * @since_tizen 6.5
 *
 * @return 0 on success, otherwise a negative error value
 * @retval #UA_ERROR_NONE Successful
 * @retval #UA_ERROR_NOT_INITIALIZED Not initialized yet
 */
int ua_disable_low_power_mode(void);

/**
 * @internal
 * @ingroup CAPI_NETWORK_UA_MODULE
 * @brief Sets low power mode for each sensor.
 * @since_tizen 6.5
 *
 * @param[in] bitmask The sensor bitmask
 * @param[in] on_off Low power mode enable or not
 *
 * @return 0 on success, otherwise a negative error value
 * @retval #UA_ERROR_NONE Successful
 * @retval #UA_ERROR_NOT_INITIALIZED Not initialized yet
 */
int ua_set_low_power_mode(unsigned int bitmask, bool on_off);

/**
 * @internal
 * @ingroup CAPI_NETWORK_UA_MODULE
 * @brief Gets the current detection parameter set for sensors.
 * @since_tizen 6.5
 *
 * @param[out] cycle Time in seconds after which user detection procedure
 *             (BLE scan/ARPing etc.) is repeated.
 * @param[out] period Time in second for which User detection procedure is executed.
 * @param[out] retries Number of retries to detect users before reporting absence.
 *             A non-detection count is maintained against each user. After
 *             each detection cycle, if presence is detected, then count is
 *             reset to 0 and Presence is reported. Else, the count is incremented
 *             and once count reaches the retries value, Absence is reported.
 *
 * @return 0 on success, otherwise a negative error value
 * @retval #UA_ERROR_NONE Successful
 * @retval #UA_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #UA_ERROR_NOT_INITIALIZED Not initialized yet
 */
int ua_get_detection_params(int *cycle, int *period, int *retries);

/**
 * @internal
 * @ingroup CAPI_NETWORK_UA_MODULE
 * @brief Sets the detection parameter for sensors. Invoking this will stop any onging
 * detection and set the new detection parameters and the restart detection again.
 * @since_tizen 6.5
 *
 * @param[in] cycle Time in seconds after which user detection procedure
 *            (BLE scan/ARPing etc.) is repeated.
 * @param[in] period Time in second for which User detection procedure is executed.
 * @param[in] retries Number of retries to detect users before reporting absence.
 *            A non-detection count is maintained against each user. After
 *            each detection cycle, if presence is detected, then count is
 *            reset to 0 and Presence is reported. Else, the count is incremented
 *            and once count reaches the retries value, Absence is reported.
 *
 * @return 0 on success, otherwise a negative error value
 * @retval #UA_ERROR_NONE Successful
 * @retval #UA_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #UA_ERROR_NOT_INITIALIZED Not initialized yet
 */
int ua_set_detection_params(int cycle, int period, int retries);

/**
 * @internal
 * @ingroup CAPI_NETWORK_UA_MODULE
 * @brief Sets the detection cycle parameter for service.
 * @since_tizen 6.5
 *
 * @remarks The value will be reset to default after reboot.
 *
 * @param[in] service Service handle for which detection cycle is getting configure.
 * @param[in] cycle_time Time in seconds after which user detection procedure
 *            (BLE scan/ARPing etc) is repeated. Minimum value of cycle_time
 *            is 300 seconds, Applications to send multiplier of 300 seconds only
 *            in this parameter. Example : 1*300 = 300 secs, 2*300 = 600 secs etc.).
 *
 * @return 0 on success, otherwise a negative error value
 * @retval #UA_ERROR_NONE Successful
 * @retval #UA_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #UA_ERROR_NOT_INITIALIZED Not initialized yet
 *
 * @see #UA_DETECTION_CYCLE_STEP
 * @see #UA_MIN_DETECTION_CYCLE
 * @see ua_get_detection_cycle()
 */
int ua_set_detection_cycle(ua_service_h service, unsigned int cycle_time);

/**
 * @internal
 * @ingroup CAPI_NETWORK_UA_MODULE
 * @brief Gets the current detection cycle parameter set for particular service.
 * @since_tizen 6.5
 *
 * @param[in] service Service handle for which detection cycle is getting configure.
 * @param[in] cycle_time Time in second after which user detection procedure
 *            (BLE scan/ARPing etc.) is repeated.
 *
 * @return 0 on success, otherwise a negative error value
 * @retval #UA_ERROR_NONE Successful
 * @retval #UA_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #UA_ERROR_NOT_INITIALIZED Not initialized yet
 *
 * @see ua_set_detection_cycle()
 */
int ua_get_detection_cycle(ua_service_h service, unsigned int* cycle_time);

/**
 * @internal
 * @ingroup CAPI_NETWORK_UA_MODULE
 * @brief Sets the detection window parameter for sensors.
 * @since_tizen 6.5
 *
 * @remarks The value will be reset to default after reboot.
 *
 * @param[in] window Time in seconds for which User detection procedure is executed.
 *            Maximum value of window is 290 seconds, Applications to send multiplier of
 *            10 seconds only in this parameter. Example : 1*10 = 10 secs, 2*10 = 20
 *            secs etc.).
 *
 * @return 0 on success, otherwise a negative error value
 * @retval #UA_ERROR_NONE Successful
 * @retval #UA_ERROR_INVALID_PARAMETER Invalid parameter
 *
 * @see #UA_DETECTION_WINDOW_STEP
 * @see #UA_MAX_DETECTION_WINDOW
 * @see ua_get_detection_window()
 * @retval #UA_ERROR_NOT_INITIALIZED Not initialized yet
 */
int ua_set_detection_window(unsigned int window);

/**
 * @internal
 * @ingroup CAPI_NETWORK_UA_MODULE
 * @brief Gets the current detection window parameter.
 * @since_tizen 6.5
 *
 * @param[in] window Time for which User detection procedure is executed.
 *
 * @return 0 on success, otherwise a negative error value
 * @retval #UA_ERROR_NONE Successful
 * @retval #UA_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #UA_ERROR_NOT_INITIALIZED Not initialized yet
 *
 * @see ua_set_detection_window()
 */
int ua_get_detection_window(unsigned int* window);

/**
 * @internal
 * @ingroup CAPI_NETWORK_UA_MODULE
 * @brief Resets database.
 * @since_tizen 6.5
 *
 * @return 0 on success, otherwise a negative error value
 * @retval #UA_ERROR_NONE Successful
 * @retval #UA_ERROR_NOT_INITIALIZED Not initialized yet
 */
int ua_reset_database(void);

/**
 * @ingroup CAPI_NETWORK_UA_MODULE
 * @brief Sets device discriminant for a specific service.
 * @since_tizen 6.5
 *
 * @param[in] service The service handle.
 * @param[in] device The device handle.
 * @param[in] discriminant The device discriminant.
 *
 * @return 0 on success, otherwise a negative error value
 * @retval #UA_ERROR_NONE Successful
 * @retval #UA_ERROR_PERMISSION_DENIED Permission Denied
 * @retval #UA_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #UA_ERROR_NOT_INITIALIZED Not initialized yet
 *
 * @see ua_service_add_user()
 * @see ua_service_remove_user()
 */
int ua_service_set_device_discriminant(
		ua_service_h service,
		ua_device_h device,
		bool discriminant);

/**
 * @ingroup CAPI_NETWORK_UA_MODULE
 * @brief Gets device discriminant for a specific service.
 * @since_tizen 6.5
 *
 * @param[in] service The service handle.
 * @param[in] device The device handle.
 * @param[out] discriminant The device discriminant.
 *
 * @return 0 on success, otherwise a negative error value
 * @retval #UA_ERROR_NONE Successful
 * @retval #UA_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #UA_ERROR_NOT_INITIALIZED Not initialized yet
 *
 * @see ua_service_add_device()
 * @see ua_service_remove_device()
 */
int ua_service_get_device_discriminant(
		ua_service_h service,
		ua_device_h device,
		bool *discriminant);

/* Deprecated API's */

/**
 * @ingroup CAPI_NETWORK_UA_MODULE
 * @brief Callback to get the device handle added to the service.
 * @since_tizen 6.5
 *
 * @remarks The @a service_handle should not be released.
 * @remarks The @a service_handle can be used only in the callback.
 * @remarks The @a device_handle should not be released.
 * @remarks The @a device_handle can be used only in the callback.
 *
 * @param[in] service_handle The service handle.
 * @param[in] device_handle The device handle.
 * @param[in] user_data User data passed in ua_service_foreach_added_devices().
 *
 * @exception
 * @pre
 * @post
 *
 * @see ua_service_foreach_added_devices()
 */
typedef bool (*ua_service_added_device_cb)(
		ua_service_h service_handle,
		ua_device_h device_handle,
		void *user_data) TIZEN_DEPRECATED_API;

/**
 * @internal
 * @ingroup CAPI_NETWORK_UA_MODULE
 * @brief Adds device for a specific service.
 * @since_tizen 6.5
 *
 * @param[in] service_handle The service handle.
 * @param[in] device_handle The device handle.
 *
 * @return 0 on success, otherwise a negative error value
 * @retval #UA_ERROR_NONE Successful
 * @retval #UA_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #UA_ERROR_NOT_INITIALIZED Not initialized yet
 *
 * @exception
 * @pre
 * @post
 *
 * @see ua_service_remove_device()
 */
int ua_service_add_device(ua_service_h service_handle,
		ua_device_h device_handle) TIZEN_DEPRECATED_API;

/**
 * @internal
 * @ingroup CAPI_NETWORK_UA_MODULE
 * @brief Removes the device from a specific service handle.
 * @since_tizen 6.5
 *
 * @param[in] service_handle The service handle.
 * @param[in] device_handle The device handle.
 *
 * @return 0 on success, otherwise a negative error value
 * @retval #UA_ERROR_NONE Successful
 * @retval #UA_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #UA_ERROR_OPERATION_FAILED Operation failed
 * @retval #UA_ERROR_NOT_INITIALIZED Not initialized yet
 *
 * @exception
 * @pre
 * @post
 *
 * @see ua_service_add_device()
 */
int ua_service_remove_device(ua_service_h service_handle,
		ua_device_h device_handle) TIZEN_DEPRECATED_API;

/**
 * @internal
 * @ingroup CAPI_NETWORK_UA_MODULE
 * @brief Retrieves the device handle of all the added devices for a specific service.
 * @since_tizen 6.5
 *
 * @param[in] service_handle The service handle.
 * @param[in] foreach_cb Callback function to be invoked with device handle.
 * @param[in] user_data The user data to be passed when callback is called.
 *
 * @return 0 on success, otherwise a negative error value
 * @retval #UA_ERROR_NONE Successful
 * @retval #UA_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #UA_ERROR_NOT_INITIALIZED Not initialized yet
 *
 * @exception
 * @pre
 * @post
 *
 * @see ua_service_added_device_cb()
 */
int ua_service_foreach_added_devices(ua_service_h service_handle,
		ua_service_added_device_cb foreach_cb,
		void *user_data) TIZEN_DEPRECATED_API;

/**
 * @ingroup CAPI_NETWORK_UA_MODULE
 * @brief Gets service handle by service name.
 * @since_tizen 6.5
 *
 * @remarks The @a service_handle should be destroyed by using #ua_service_destroy().
 *
 * @param[in] name The service name information
 * @param[out] service_handle The service handle
 *
 * @return 0 on success, otherwise a negative error value
 * @retval #UA_ERROR_NONE Successful
 * @retval #UA_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #UA_ERROR_OPERATION_FAILED Operation failed
 * @retval #UA_ERROR_NOT_INITIALIZED Not initialized yet
 *
 * @exception
 * @pre
 * @post
 *
 * @see ua_service_get_name()
 * @see ua_service_set_name()
 */
int ua_service_get_by_name(
		const char *name,
		ua_service_h *service_handle) TIZEN_DEPRECATED_API;

/**
 * @ingroup CAPI_NETWORK_UA_MODULE
 * @brief Sets name info for service handle.
 * @since_tizen 6.5
 *
 * @param[in] service_handle The service handle
 * @param[in] name The service name information
 *
 * @return 0 on success, otherwise a negative error value
 * @retval #UA_ERROR_NONE Successful
 * @retval #UA_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #UA_ERROR_NOT_INITIALIZED Not initialized yet
 *
 * @exception
 * @pre
 * @post
 *
 * @see ua_service_get_name()
 */
int ua_service_set_name(
		ua_service_h service_handle,
		const char *name) TIZEN_DEPRECATED_API;

/**
 * @ingroup CAPI_NETWORK_UA_MODULE
 * @brief Sets a device type info for a device.
 * @since_tizen 6.5
 *
 * @param[in] device_handle The device handle
 * @param[in] mac_type The MAC address type of the device
 *
 * @return 0 on success, otherwise a negative error value
 * @retval #UA_ERROR_NONE Successful
 * @retval #UA_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #UA_ERROR_NOT_INITIALIZED Not initialized yet
 *
 * @exception
 * @pre
 * @post
 *
 * @see ua_device_get_mac_type()
 */
int ua_device_set_mac_type(
		ua_device_h device_handle,
		ua_mac_type_e mac_type) TIZEN_DEPRECATED_API;

/**
 * @ingroup CAPI_NETWORK_UA_MODULE
 * @brief Sets device's MAC address.
 * @since_tizen 6.5
 *
 * @param[in] device_handle The device handle
 * @param[in] mac_address The device's MAC address.
 *
 * @return 0 on success, otherwise a negative error value
 * @retval #UA_ERROR_NONE Successful
 * @retval #UA_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #UA_ERROR_NOT_INITIALIZED Not initialized yet
 *
 * @exception
 * @pre
 * @post
 *
 * @see ua_device_get_mac_address()
 */
int ua_device_set_mac_address(
		ua_device_h device_handle,
		const char *mac_address) TIZEN_DEPRECATED_API;

/**
 * @ingroup CAPI_NETWORK_UA_MODULE
 * @brief Sets device ID.
 * @since_tizen 6.5
 *
 * @param[in] device_handle The device handle
 * @param[in] device_id The device ID.
 *
 * @return 0 on success, otherwise a negative error value
 * @retval #UA_ERROR_NONE Successful
 * @retval #UA_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #UA_ERROR_NOT_INITIALIZED Not initialized yet
 *
 * @exception
 * @pre
 * @post
 *
 * @see ua_device_get_device_id()
 */
int ua_device_set_device_id(
		ua_device_h device_handle,
		const char *device_id) TIZEN_DEPRECATED_API;

/**
 * @internal
 * @ingroup CAPI_NETWORK_UA_MODULE
 * @brief Sets discriminant for a device.
 * @since_tizen 6.5
 *
 * @param[in] device_handle The device handle
 * @param[in] discriminant Determines whether to judge PRESENCE/ABSENCE.
 *
 * @return 0 on success, otherwise a negative error value
 * @retval #UA_ERROR_NONE Successful
 * @retval #UA_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #UA_ERROR_NOT_INITIALIZED Not initialized yet
 *
 * @exception
 * @pre
 * @post
 */
int ua_device_set_discriminant(ua_device_h device_handle,
			bool discriminant) TIZEN_DEPRECATED_API;

/**
 * @internal
 * @ingroup CAPI_NETWORK_UA_MODULE
 * @brief Retrieves the device handle of all the registered devices for one specific user.
 * @since_tizen 6.5
 *
 * @param[in] user_handle The user handle.
 * @param[in] foreach_cb Callback function to be invoked with user device handle.
 * @param[in] user_data The user data to be passed when callback is called.
 *
 * @return 0 on success, otherwise a negative error value
 * @retval #UA_ERROR_NONE Successful
 * @retval #UA_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #UA_ERROR_OUT_OF_MEMORY Out of memory
 * @retval #UA_ERROR_NOT_INITIALIZED Not initialized yet
 *
 * @exception
 * @pre
 * @post
 *
 * @see ua_registered_device_cb()
 */
int ua_device_foreach_added_by_user(
		ua_user_h user_handle,
		ua_registered_device_cb foreach_cb,
		void *user_data) TIZEN_DEPRECATED_API;

/**
 * @ingroup CAPI_NETWORK_UA_MODULE
 * @brief Clones device handle if application wants to store it after foreach device
 *        handles. It should be call before foreach callback return.
 * @since_tizen 6.5
 *
 * @remarks The @a cloned should be released using #ua_device_destroy().
 *
 * @param[out] cloned The cloned device handle.
 * @param[in] origin The input device handle
 *
 * @return 0 on success, otherwise a negative error value
 * @retval #UA_ERROR_NONE Successful
 * @retval #UA_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #UA_ERROR_OUT_OF_MEMORY Out of memory
 * @retval #UA_ERROR_NOT_INITIALIZED Not initialized yet
 *
 * @exception
 * @pre
 * @post
 *
 * @see ua_registered_device_cb()
 * @see ua_device_destroy()
 */
int ua_device_clone(
		ua_device_h *cloned,
		ua_device_h origin) TIZEN_DEPRECATED_API;

/**
 * @ingroup CAPI_NETWORK_UA_MODULE
 * @brief Gets user handle by account.
 * @since_tizen 6.5
 *
 * @remarks The @a user_handle should not be released.
 * @remarks The @a user_handle can be used only in the function.
 *
 * @param[in] account The user account information
 * @param[out] user_handle The user handle
 *
 * @return 0 on success, otherwise a negative error value
 * @retval #UA_ERROR_NONE Successful
 * @retval #UA_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #UA_ERROR_NOT_INITIALIZED Not initialized yet
 *
 * @exception
 * @pre
 * @post
 *
 */
int ua_user_get_handle_by_account(const char* account,
		ua_user_h *user_handle) TIZEN_DEPRECATED_API;

/**
 * @internal
 * @ingroup CAPI_NETWORK_UA_MODULE
 * @brief Clones user handle if application wants to store it after foreach user
 *        handles. It should be call before foreach callback return.
 * @since_tizen 6.5
 *
 * @remarks The @a cloned should be released using #ua_user_destroy().
 *
 * @param[out] cloned The cloned device handle.
 * @param[in] origin The input device handle
 *
 * @return 0 on success, otherwise a negative error value
 * @retval #UA_ERROR_NONE Successful
 * @retval #UA_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #UA_ERROR_OUT_OF_MEMORY Out of memory
 * @retval #UA_ERROR_NOT_INITIALIZED Not initialized yet
 *
 * @exception
 * @pre
 * @post
 *
 * @see ua_foreach_users()
 * @see ua_service_foreach_added_user()
 * @see ua_user_destroy()
 */
int ua_user_clone(ua_user_h *cloned, ua_user_h origin) TIZEN_DEPRECATED_API;

/**
 * @internal
 * @ingroup CAPI_NETWORK_UA_MODULE
 * @brief Gets default user handle.
 * @since_tizen 6.5
 *
 * @remarks The @a user_handle should be released using #ua_user_destroy().
 *
 * @param[out] user_handle The user handle
 *
 * @return 0 on success, otherwise a negative error value
 * @retval #UA_ERROR_NONE Successful
 * @retval #UA_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #UA_ERROR_OPERATION_FAILED Operation failed
 * @retval #UA_ERROR_NOT_INITIALIZED Not initialized yet
 *
 * @exception
 * @pre
 * @post
 *
 * @see ua_user_get_account()
 */
int ua_user_get_default_user(ua_user_h *user_handle) TIZEN_DEPRECATED_API;

/**
 * @internal
 * @ingroup CAPI_NETWORK_UA_MODULE
 * @brief Sets account info for device handle.
 * @since_tizen 6.5
 *
 * @param[in] user_handle The user handle
 * @param[in] account Account information
 *
 * @return 0 on success, otherwise a negative error value
 * @retval #UA_ERROR_NONE Successful
 * @retval #UA_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #UA_ERROR_NOT_INITIALIZED Not initialized yet
 *
 * @exception
 * @pre
 * @post
 *
 * @see ua_user_get_account()
 */
int ua_user_set_account(ua_user_h user_handle, const char *account) TIZEN_DEPRECATED_API;

/**
 * @internal
 * @ingroup CAPI_NETWORK_UA_MODULE
 * @brief Sets name info for user handle.
 * @since_tizen 6.5
 *
 * @param[in] user_handle The user handle
 * @param[in] name The user name information
 *
 * @return 0 on success, otherwise a negative error value
 * @retval #UA_ERROR_NONE Successful
 * @retval #UA_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #UA_ERROR_NOT_INITIALIZED Not initialized yet
 *
 * @exception
 * @pre
 * @post
 */
int ua_user_set_name(ua_user_h user_handle, const char *name) TIZEN_DEPRECATED_API;

/**
 * @internal
 * @ingroup CAPI_NETWORK_UA_MODULE
 * @brief Removes the device ID.
 * @since_tizen 6.5
 *
 * @param[in] device_id The device ID.
 * @param[in] mac_type The device's MAC address type.
 *
 * @return  0 on success, otherwise a negative error value
 * @retval #UA_ERROR_NONE Successful
 * @retcal #UA_ERROR_RESOURCE_BUSY Remove done. but another service uses thie device.
 * @retval #UA_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #UA_ERROR_NOT_INITIALIZED Not initialized yet
 *
 * @exception
 * @pre
 * @post
 */
int ua_user_remove_device_by_device_id(const char *device_id,
			ua_mac_type_e mac_type) TIZEN_DEPRECATED_API;

/**
 * @internal
 * @ingroup CAPI_NETWORK_UA_MODULE
 * @brief Retrieves the user handle of all the registered users.
 * @since_tizen 6.5
 *
 * @param[in] foreach_cb Callback function to be invoked with user handle.
 * @param[in] user_data The user data to be passed when callback is called.
 *
 * @return 0 on success, otherwise a negative error value
 * @retval #UA_ERROR_NONE Successful
 * @retval #UA_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #UA_ERROR_OUT_OF_MEMORY Out of memory
 * @retval #UA_ERROR_OPERATION_FAILED Operation failed
 * @retval #UA_ERROR_NOT_INITIALIZED Not initialized yet
 *
 * @exception
 * @pre
 * @post
 *
 * @see ua_registered_user_cb()
 */
int ua_user_foreach_added(ua_registered_user_cb foreach_cb,
			void *user_data) TIZEN_DEPRECATED_API;

/**
 * @}
 */

#ifdef __cplusplus
}
#endif
#endif /* __TIZEN_CAPI_NETWORK_USER_AWARENESS_INTERNAL_H__ */
