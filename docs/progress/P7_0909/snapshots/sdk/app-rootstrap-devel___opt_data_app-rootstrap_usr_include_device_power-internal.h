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

#ifndef __TIZEN_SYSTEM_POWER_INTERNAL_H__
#define __TIZEN_SYSTEM_POWER_INTERNAL_H__

/**
 * @addtogroup CAPI_SYSTEM_DEVICE_POWER_INTERNAL_MODULE
 * @{
 */

#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>
#include <glib.h>
#include <gio/gio.h>

#include "power.h"

/**
 * @platform
 * @brief Requests the current device's power state change to be changed to powered off.
 * @details It operates synchronously and powers off the current device.
 * @since_tizen 6.5
 * @privlevel platform
 * @privilege %http://tizen.org/privilege/reboot
 * @return @c 0 on success, otherwise a negative error value
 * @retval #DEVICE_ERROR_NONE Successful
 * @retval #DEVICE_ERROR_PERMISSION_DENIED Permission denied
 * @retval #DEVICE_ERROR_OPERATION_FAILED Operation failed
 *
 * @code
 * #include <device/power-internal.h>
 * ...
 * int ret = device_power_poweroff();
 * if (ret != DEVICE_ERROR_NONE) {
 *     printf("Device powered off failed.\n");
 * }
 * ...
 * @endcode
 */
int device_power_poweroff(void);

/**
 * @platform
 * @brief Reason for what triggered transition.
 * @since_tizen 7.0
 */
typedef enum {
	DEVICE_POWER_TRANSITION_REASON_UNKNOWN,			/**< Unknown reason */
	DEVICE_POWER_TRANSITION_REASON_POWER_KEY,		/**< Power key pressed */
	DEVICE_POWER_TRANSITION_REASON_VOLUME_UP_KEY,		/**< Volume up key pressed */
	DEVICE_POWER_TRANSITION_REASON_VOLUME_DOWN_KEY,		/**< Volume down key pressed */
	DEVICE_POWER_TRANSITION_REASON_BATTERY_NORMAL_LEVEL,	/**< Battery capacity reaches normal level*/
	DEVICE_POWER_TRANSITION_REASON_BATTERY_WARNING_LEVEL,	/**< Battery capacity reaches warning level */
	DEVICE_POWER_TRANSITION_REASON_BATTERY_CRITICAL_LEVEL,	/**< Battery capacity reaches critical level */
	DEVICE_POWER_TRANSITION_REASON_BATTERY_POWEROFF_LEVEL,	/**< Battery capacity reaches poweroff level */
	DEVICE_POWER_TRANSITION_REASON_DISPLAY_OFF,		/**< Display off */
	DEVICE_POWER_TRANSITION_REASON_DISPLAY_OFF_TIMEOUT,	/**< Display off after timeout */
	DEVICE_POWER_TRANSITION_REASON_TOUCH_KEY,		/**< Touch key pressed */
	DEVICE_POWER_TRANSITION_REASON_TOUCH_SCREEN,		/**< Touch screen pressed */
	DEVICE_POWER_TRANSITION_REASON_USB,			/**< USB attached or detached */
	DEVICE_POWER_TRANSITION_REASON_CHARGER,			/**< Charger attached or detached */
	DEVICE_POWER_TRANSITION_REASON_HDMI,			/**< HDMI cable attached or detached */
	DEVICE_POWER_TRANSITION_REASON_DISPLAY_PORT,		/**< Display port cable attached or detached */
	DEVICE_POWER_TRANSITION_REASON_EMBEDDED_DISPLAY_PORT,	/**< Embedded display port cable attached or detached */
	DEVICE_POWER_TRANSITION_REASON_WIFI,			/**< WIFI event */
	DEVICE_POWER_TRANSITION_REASON_BLUETOOTH,		/**< Bluetooth event */
	DEVICE_POWER_TRANSITION_REASON_NFC,			/**< NFC event */
	DEVICE_POWER_TRANSITION_REASON_TELEPHONY,		/**< Telephony event */
	DEVICE_POWER_TRANSITION_REASON_ZIGBEE,			/**< Zigbee event */
	DEVICE_POWER_TRANSITION_REASON_ETHERNET,		/**< Ethernet event */
	DEVICE_POWER_TRANSITION_REASON_AUDIO,			/**< Audio event */
	DEVICE_POWER_TRANSITION_REASON_ALARM,			/**< Alarm event */
	DEVICE_POWER_TRANSITION_REASON_SENSOR,			/**< Sensor event */
	DEVICE_POWER_TRANSITION_REASON_RTC,			/**< RTC event */
	DEVICE_POWER_TRANSITION_REASON_HEADSET,			/**< Headset attatched or detached or button pressed */
	DEVICE_POWER_TRANSITION_REASON_EXTERNAL_MEMORY,		/**< External memory inserted or deleted */

	DEVICE_POWER_TRANSITION_REASON_CUSTOM = 1000,		/**< Define custom reason from here */
} device_power_transition_reason_e;

/**
 * @brief Enumeration for power state.
 * @since_tizen 6.5
 */
typedef enum {
	DEVICE_POWER_STATE_START	= (1ULL << 4), /**< Initial state of power module. It is especially meaningful in that
							this can be used to identify the first transition and implement
							booting UX */
	DEVICE_POWER_STATE_NORMAL	= (1ULL << 5), /**< System keeps running */
	DEVICE_POWER_STATE_SLEEP	= (1ULL << 6), /**< System may be suspended at any time */
	DEVICE_POWER_STATE_POWEROFF	= (1ULL << 7), /**< Prepare for poweroff and perform `systemctl poweroff` */
	DEVICE_POWER_STATE_REBOOT	= (1ULL << 8), /**< Prepare for reboot and perform `systemctl reboot` */
	DEVICE_POWER_STATE_EXIT		= (1ULL << 9), /**< Prepare for exit and perform `systemctl exit` */
} device_power_state_e;

/**
 * @brief Enumeration for transient state.
 * @details Each enum denotes transient state between static state #device_power_state_e.
 * This enumeration is used to define the steps in power state transitions, allowing callback functions
 * to perform order-dependent operations during state changes.
 *
 * State transition sequence:
 * wakeup -> SUSPENDING_EARLY -> SUSPENDING -> SUSPENDING_LATE -> sleep
 * sleep -> RESUMING_EARLY -> RESUMING -> RESUMING_LATE -> wakeup
 *
 * For example, if there are two tasks that must be performed in a specific order:
 * - Task 1 must be performed before Task 2 when transitioning from wakeup to sleep
 * - Task 2 must be performed before Task 1 when transitioning from sleep to wakeup
 *
 * Then the implementation would be:
 * - In SUSPENDING_EARLY callback: Perform Task 1
 * - In SUSPENDING_LATE callback: Perform Task 2
 * - In RESUMING_EARLY callback: Perform Task 2
 * - In RESUMING_LATE callback: Perform Task 1
 *
 * This allows for proper sequencing of operations that must be performed in a specific order
 * during power state transitions.
 * @since_tizen 7.0
 */
typedef enum {
	DEVICE_POWER_TRANSIENT_STATE_RESUMING_EARLY	= (1ULL << 4), /**< The first step of transitioning from sleep to normal */
	DEVICE_POWER_TRANSIENT_STATE_RESUMING		= (1ULL << 5), /**< The second step of transitioning from sleep to normal */
	DEVICE_POWER_TRANSIENT_STATE_RESUMING_LATE	= (1ULL << 6), /**< The last step of transitioning from sleep to normal */
	DEVICE_POWER_TRANSIENT_STATE_SUSPENDING_EARLY	= (1ULL << 7), /**< The first step of transitioning from normal to sleep */
	DEVICE_POWER_TRANSIENT_STATE_SUSPENDING		= (1ULL << 8), /**< The second step of transitioning from normal to sleep */
	DEVICE_POWER_TRANSIENT_STATE_SUSPENDING_LATE	= (1ULL << 9), /**< The last step of transitioning from normal to sleep */
} device_power_transient_state_e;

/**
 * @brief Enumeration for checking power lock state locked or unlocked.
 * @since_tizen 7.0
 */
typedef enum {
	DEVICE_POWER_LOCK_STATE_UNLOCK, /**< Power lock is unlocked */
	DEVICE_POWER_LOCK_STATE_LOCK, /**< Power lock is locked */
} device_power_lock_state_e;

/**
 * @brief Notify the deviced that it is ready for the actual action.
 * @details Notify the deviced that it is ok to take an actual action of change state. \n
 *          This function only works on the id received from device_power_state_wait_callback() and device_power_transient_state_wait_callback().
 * @since_tizen 6.5
 * @remarks The @a wait_callback_id must have been previously registered using device_power_state_wait_callback() or device_power_transient_state_wait_callback().
 * @param[in] wait_callback_id Wait callback id to confirm
 * @return @c 0 on success, otherwise a negative error value
 * @retval #DEVICE_ERROR_NONE Successful
 * @retval #DEVICE_ERROR_OPERATION_FAILED Operation failed
 *
 * @code
 * #include <device/power-internal.h>
 * ...
 * static void power_state_change_cb(device_power_state_e prev_state,
 *                                    device_power_state_e next_state,
 *                                    uint64_t wait_callback_id,
 *                                    device_power_transition_reason_e transition_reason,
 *                                    void *user_data)
 * {
 *     if (next_state == DEVICE_POWER_STATE_SLEEP) {
 *         device_power_confirm_wait_callback(wait_callback_id);
 *     }
 *     ...
 * }
 * ...
 * int main(void)
 * {
 *     int ret = 0;
 *     ret = device_power_add_state_wait_callback(DEVICE_POWER_STATE_SLEEP,
 *                                                power_state_change_cb,
 *                                                NULL);
 *     if (ret != DEVICE_ERROR_NONE) {
 *         ...
 *     }
 * }
 * ...
 * @endcode
 * @see device_power_state_wait_callback()
 * @see device_power_transient_state_wait_callback()
 * @see device_power_cancel_wait_callback()
 */
int device_power_confirm_wait_callback(uint64_t wait_callback_id);

/**
 * @brief Notify the deviced that it needs undoing the current transition.
 * @details Notify the deviced that the current transition should be rewinded. \n
 *          This function only works on the id received from device_power_state_wait_callback() and device_power_transient_state_wait_callback().
 * @since_tizen 7.0
 * @remarks The @a wait_callback_id must have been previously registered using device_power_state_wait_callback() or device_power_transient_state_wait_callback().
 * @param[in] wait_callback_id Wait callback id to cancel
 * @return @c 0 on success, otherwise a negative error value
 * @retval #DEVICE_ERROR_NONE Successful
 * @retval #DEVICE_ERROR_OPERATION_FAILED Operation failed
 *
 * @code
 * #include <device/power-internal.h>
 * ...
 * static void power_state_change_cb(device_power_state_e prev_state,
 *                                    device_power_state_e next_state,
 *                                    uint64_t wait_callback_id,
 *                                    device_power_transition_reason_e transition_reason,
 *                                    void *user_data)
 * {
 *     if (transition_reason == DEVICE_POWER_TRANSITION_REASON_CUSTOM && next_state == DEVICE_POWER_STATE_SLEEP) {
 *         device_power_cancel_wait_callback(wait_callback_id);
 *     }
 *     ...
 * }
 * ...
 * int main(void)
 * {
 *     int ret = 0;
 *     ret = device_power_add_state_wait_callback(DEVICE_POWER_STATE_SLEEP,
 *                                                power_state_change_cb,
 *                                                NULL);
 *     if (ret != DEVICE_ERROR_NONE) {
 *         ...
 *     }
 * }
 * ...
 * @endcode
 * @see device_power_state_wait_callback()
 * @see device_power_transient_state_wait_callback()
 * @see device_device_power_confirm_wait_callback()
 */
int device_power_cancel_wait_callback(uint64_t wait_callback_id);

/**
 * @brief Callback function type for device power state change events.
 * @details If both device_power_state_wait_callback() and device_power_change_state_callback() have registered to the same power state, \n
 *          then the device_power_state_wait_callback() will be invoked first and the device_power_change_state_callback() will follow.
 * @since_tizen 6.5
 * @remarks The registered callback will be called when a power state transition occurs.
 * @param[out] prev_state Power state where transition has started
 * @param[out] next_state Power state to be changed by transition
 * @param[out] wait_callback_id Unique id for each callback invocation
 * @param[out] transition_reason Reason for what triggered the transition
 * @param[out] user_data User data passed from the callback registration
 *
 * @code
 * #include <device/power-internal.h>
 * ...
 * static void power_state_change_cb(device_power_state_e prev_state,
 *                                    device_power_state_e next_state,
 *                                    uint64_t wait_callback_id,
 *                                    device_power_transition_reason_e transition_reason,
 *                                    void *user_data)
 * {
 *     ...
 * }
 * ...
 * int main(void)
 * {
 *     int ret = 0;
 *     ret = device_power_add_state_wait_callback(DEVICE_POWER_STATE_NORMAL,
 *                                                power_state_change_cb,
 *                                                NULL);
 *     if (ret != DEVICE_ERROR_NONE) {
 *         ...
 *     }
 * }
 * ...
 * @endcode
 * @see device_power_add_state_wait_callback()
 * @see device_power_state_e
 * @see device_power_transition_reason_e
 */
typedef void (*device_power_state_wait_callback) (device_power_state_e prev_state, device_power_state_e next_state,
	uint64_t wait_callback_id, device_power_transition_reason_e transition_reason, void *user_data);

/**
 * @brief Adds a callback to observe the specified power state change.
 * @details Register a callback for specific power state. \n
 *          The callback will be invoked when the deviced changes state to the designated power state. \n
 *          An actual action, such as triggering poweroff on changing state to #DEVICE_POWER_STATE_POWEROFF or \n
 *          wake unlock on changing state to #DEVICE_POWER_STATE_SLEEP, will be deferred to give a chance \n
 *          for the API caller to prepare for the action.
 * @since_tizen 6.5
 * @remarks Added callback based on @a state_bits can be removed by device_power_remove_state_wait_callback(). \n
 * @param[in] state_bits Bitwise ORed #device_power_state_e
 * @param[in] cb Callback function
 * @param[in] user_data Data to be passed to the callback function
 * @return @c 0 on success, otherwise a negative error value
 * @retval #DEVICE_ERROR_NONE Successful
 * @retval #DEVICE_ERROR_OPERATION_FAILED Operation failed
 *
 * @code
 * #include <device/power-internal.h>
 * ...
 * static void power_state_change_cb(device_power_state_e prev_state,
 *                                    device_power_state_e next_state,
 *                                    uint64_t wait_callback_id,
 *                                    device_power_transition_reason_e transition_reason,
 *                                    void *user_data)
 * {
 *     ...
 * }
 * ...
 * int main(void)
 * {
 *     int ret = 0;
 *     ret = device_power_add_state_wait_callback(DEVICE_POWER_STATE_NORMAL,
 *                                                power_state_change_cb,
 *                                                NULL);
 *     if (ret != DEVICE_ERROR_NONE) {
 *         ...
 *     }
 * }
 * ...
 * @endcode
 * @see device_power_state_wait_callback()
 * @see device_power_remove_state_wait_callback()
 * @see device_power_state_e
 */
int device_power_add_state_wait_callback(device_power_state_e state_bits, device_power_state_wait_callback cb, void *user_data);

/**
 * @brief Removes the callback that has registered for a specific power state.
 * @details Unregisters a callback function that was previously registered using device_power_add_state_wait_callback().
 * @since_tizen 6.5
 * @remarks Removes the callback function only for the specified state bits.\n
 *          If the callback was registered for multiple state bits, only the specified state bits will be removed.
 * @param[in] state_bits Bitwise ORed #device_power_state_e
 *
 * @code
 * #include <device/power-internal.h>
 * ...
 * static void power_state_change_cb(device_power_state_e prev_state,
 *                                    device_power_state_e next_state,
 *                                    uint64_t wait_callback_id,
 *                                    device_power_transition_reason_e transition_reason,
 *                                    void *user_data)
 * {
 *     ...
 * }
 * ...
 * int main(void)
 * {
 *     int ret = 0;
 *     ret = device_power_add_state_wait_callback(DEVICE_POWER_STATE_NORMAL,
 *                                                power_state_change_cb,
 *                                                NULL);
 *     if (ret != DEVICE_ERROR_NONE) {
 *         ...
 *     }
 *     ...
 *     ret = device_power_remove_state_wait_callback(DEVICE_POWER_STATE_NORMAL);
 * }
 * ...
 * @endcode
 * @see device_power_add_state_wait_callback()
 * @see device_power_state_e
 */
void device_power_remove_state_wait_callback(device_power_state_e state_bits);

/**
 * @brief Callback function type for device power transient state change events.
 * @details It can be registered by device_power_add_transient_state_wait_callback(). \n
 *          Also it can be removed by device_power_remove_transient_state_wait_callback().
 * @since_tizen 6.5
 * @remarks The registered callback will be invoked when a power state transient transition occurs.
 * @param[out] transient_state Transient state to be changed by the transition
 * @param[out] wait_callback_id Unique id for each callback invocation
 * @param[out] transition_reason Reason for what triggered the transition
 * @param[out] user_data User data passed from the callback registration
 *
 * @code
 * #include <device/power-internal.h>
 * ...
 * static void power_transient_state_change_cb(device_power_transient_state_e transient_state,
 *                                   uint64_t wait_callback_id,
 *                                   device_power_transition_reason_e transition_reason,
 *                                   void *user_data)
 * {
 *     ...
 * }
 * ...
 * int main(void)
 * {
 *     int ret = 0;
 *     ret = device_power_add_transient_state_wait_callback(DEVICE_POWER_TRANSIENT_STATE_SUSPENDING,
 *                                                          power_transient_state_change_cb,
 *                                                          NULL);
 *     if (ret != DEVICE_ERROR_NONE) {
 *         ...
 *     }
 * }
 * ...
 * @endcode
 * @see device_power_add_transient_state_wait_callback()
 * @see device_power_transient_state_e
 * @see device_power_transition_reason_e
 */
typedef void (*device_power_transient_state_wait_callback) (device_power_transient_state_e transient_state,
	uint64_t wait_callback_id, device_power_transition_reason_e transition_reason, void *user_data);

/**
 * @brief Adds a callback to observe the specified transient power state.
 * @details Register a callback for specific transient power state. \n
 *          The callback will be invoked in the middle of transition process. \n
 *          Similar to the device_power_add_change_state_wait_callback(), it withholds transition \n
 *          until it is confirmed to be ready to go. \n
 * @since_tizen 7.0
 * @remarks Added callback based on @a transient_bits can be removed by device_power_remove_transient_state_wait_callback(). \n
 * @param[in] transient_bits Bitwise ORed #device_power_transient_state_e
 * @param[in] cb Callback function
 * @param[in] user_data Data to be passed to the callback function
 * @return @c 0 on success,
 *         otherwise a negative error value
 * @retval #DEVICE_ERROR_NONE Successful
 * @retval #DEVICE_ERROR_OPERATION_FAILED Operation failed
 *
 * @code
 * #include <device/power-internal.h>
 * ...
 * static void power_transient_state_change_cb(device_power_transient_state_e transient_state,
 *                                   uint64_t wait_callback_id,
 *                                   device_power_transition_reason_e transition_reason,
 *                                   void *user_data)
 * {
 *     ...
 * }
 * ...
 * int main(void)
 * {
 *     int ret = 0;
 *     ret = device_power_add_transient_state_wait_callback(DEVICE_POWER_TRANSIENT_STATE_SUSPENDING,
 *                                                          power_transient_state_change_cb,
 *                                                          NULL);
 *     if (ret != DEVICE_ERROR_NONE) {
 *         ...
 *     }
 * }
 * ...
 * @endcode
 * @see device_power_transient_state_wait_callback()
 * @see device_power_transient_remove_state_wait_callback()
 * @see device_power_transient_state_e
 */
int device_power_add_transient_state_wait_callback(device_power_transient_state_e transient_bits,
	device_power_transient_state_wait_callback cb, void *user_data);

/**
 * @brief Removes the callback that has registered for a specific transient power state.
 * @details Unregisters a callback function that was previously registered using device_power_add_state_wait_callback().
 * @since_tizen 7.0
 * @remarks Removes the callback function only for the specified transient state bits.\n
 *          If the callback was registered for multiple state bits, only the specified state bits will be removed.
 * @param[in] transient_bits Bitwise ORed #device_power_transient_state_e
 *
 * @code
 * #include <device/power-internal.h>
 * ...
 * static void power_transient_state_change_cb(device_power_transient_state_e transient_state,
 *                                   uint64_t wait_callback_id,
 *                                   device_power_transition_reason_e transition_reason,
 *                                   void *user_data)
 * {
 *     ...
 * }
 * ...
 * int main(void)
 * {
 *     int ret = 0;
 *     ret = device_power_add_transient_state_wait_callback(DEVICE_POWER_TRANSIENT_STATE_SUSPENDING,
 *                                                          power_transient_state_change_cb,
 *                                                          NULL);
 *     if (ret != DEVICE_ERROR_NONE) {
 *         ...
 *     }
 *     ...
 *     ret = device_power_remove_transient_state_wait_callback(DEVICE_POWER_TRANSIENT_STATE_SUSPENDING);
 * }
 * ...
 * @endcode
 * @see device_power_transient_state_wait_callback()
 * @see device_power_add_transient_state_wait_callback()
 * @see device_power_transient_state_e
 */
void device_power_remove_transient_state_wait_callback(device_power_transient_state_e transient_bits);

/**
 * @brief Asynchronous callback of device_power_change_state().
 * @details If both device_power_state_wait_callback() and device_power_change_state_callback() have registered to the same power state, \n
 *          then the device_power_state_wait_callback() will be invoked first and the device_power_change_state_callback() will follow.
 * @since_tizen 6.5
 * @remarks The registered callback will be called when a power state transition occurs.
 * @param[out] state State to be changed
 * @param[out] retval Return of change state
 * @param[out] user_data The user data passed from the change state function
 *
 * @code
 * #include <device/power-internal.h>
 * ...
 * static void power_state_change_cb(device_power_state_e state, int retval, void *user_data)
 * {
 *     ...
 * }
 * ...
 * int main(void)
 * {
 *     int ret = 0;
 *     ret = device_power_change_state(DEVICE_POWER_STATE_SLEEP, 3, power_state_change_cb, NULL);
 *     if (ret != DEVICE_ERROR_NONE) {
 *         ...
 *     }
 * }
 * ...
 * @endcode
 * @see device_power_change_state()
 * @see device_power_state_e
 */
typedef void (*device_power_change_state_callback) (device_power_state_e state, int retval, void *user_data);

/**
 * @brief Sends the request to change the power state asynchronously.
 * @since_tizen 6.5
 * @param[in] state Target power state
 * @param[in] timeout_sec Timeout for the async reply in second, maximum of 10 seconds
 * @param[in] cb Async callback of the request
 * @param[in] user_data Data to be passed to the callback function
 * @return @c 0 on success, otherwise a negative error value
 * @retval #DEVICE_ERROR_NONE Successful
 * @retval #DEVICE_ERROR_OPERATION_FAILED Operation failed
 * @remarks It is strongly encouraged that checking power lock count when it comes
 *          to changing state to DEVICE_POWER_STATE_SLEEP as it bypasses POWER_LOCK_CPU.
 *
 * @code
 * #include <device/power-internal.h>
 * ...
 * static void power_state_change_cb(device_power_state_e state, int retval, void *user_data)
 * {
 *     ...
 * }
 * ...
 * int main(void)
 * {
 *     int ret = 0;
 *     int count = 0;
 *
 *     ret = device_power_get_lock_count(POWER_LOCK_CPU , &count);
 *     if (ret != DEVICE_ERROR_NONE) {
 *        ...
 *        return 0;
 *     }
 *
 *     if (count > 0) {
 *        ...
 *        return 0;
 *     }
 *
 *     ret = device_power_change_state(DEVICE_POWER_STATE_SLEEP, 3, power_state_change_cb, NULL);
 *     if (ret != DEVICE_ERROR_NONE) {
 *         ...
 *     }
 * }
 * ...
 * @endcode
 * @see device_power_get_lock_count()
 * @see device_power_change_state_callback()
 * @see device_power_state_e
 */
int device_power_change_state(device_power_state_e state, int timeout_sec, device_power_change_state_callback cb, void *user_data);

/**
 * @brief Checks if a power reboot is possible on the current device state.
 * @details Checks if a reboot is allowed on the current device state.\n
 *          It returns @c 1 if a reboot is allowed, otherwise it returns @c 0.
 * @since_tizen 6.5
 * @remarks Decides whether a reboot is allowed from the HAL board module.
 * @return @c 1 if a device is able to reboot, otherwise return @c 0
 *
 * @code
 * #include <stdio.h>
 * #include <device/power-internal.h>
 * ...
 * int is_reboot_allowed = device_power_check_reboot_allowed();
 * if (is_reboot_allowed == 1) {
 *     printf("Reboot is allowed.\n");
 * } else {
 *     printf("Reboot is not allowed.\n");
 * }
 * ...
 * @endcode
 */
int device_power_check_reboot_allowed(void);

#ifndef EXCLUDE_INTERNAL_CAPI_SYSTEM_DEVICE
/**
 * @brief Gets the reason for the last device wakeup based on the scenario.
 * @details Retrieves the reason for the last device wakeup.\n
 *          The reason is returned as a @a device_power_transition_reason_e reason pointer.
 * @since_tizen 7.0
 * @remarks Ensure that the provided @a reason pointer is valid and has enough memory allocated.
 * @param[out] reason the reason why the device wakeup
 * @return @c 0 on success, otherwise a negative error value
 * @retval #DEVICE_ERROR_NONE Successful
 * @retval #DEVICE_ERROR_OPERATION_FAILED Operation failed
 * @retval #DEVICE_ERROR_INVALID_PARAMETER Invalid parameter
 *
 * @code
 * #include <stdio.h>
 * #include <device/power-internal.h>
 * ...
 * device_power_transition_reason_e reason;
 * int ret = device_power_get_wakeup_reason(&reason);
 * if (ret != DEVICE_ERROR_NONE) {
 *     return -1;
 * }
 * printf("Wakeup reason: %d\n", reason);
 * ...
 * @endcode
 * @see device_power_transition_reason_e
 */
int device_power_get_wakeup_reason(device_power_transition_reason_e *reason);
#endif

/**
 * @brief Callback function type for device power lock state change events.
 * @details It can be registered by device_power_add_lock_state_change_callback(). \n
 * @since_tizen 7.0
 * @remarks The callback function will be called when the state of a power lock changes.
 * @param[out] power_lock_type Type of power lock
 * @param[out] power_lock_state Status of power lock
 * @param[out] user_data User data passed from the callback registration
 *
 * @code
 * #include <stdio.h>
 * #include <device/power-internal.h>
 * ...
 * static void power_lock_state_changed_cb(power_lock_e power_lock_type, device_power_lock_state_e power_lock_state, void *user_data)
 * {
 *     printf("Power lock state changed: type=%d, state=%d\n", power_lock_type, power_lock_state);
 *     ...
 * }
 * ...
 * int main(void)
 * {
 *     int ret = device_power_add_lock_state_change_callback(POWER_LOCK_NORMAL, power_lock_state_changed, NULL);
 *     if (ret != DEVICE_ERROR_NONE) {
 *         return -1;
 *     }
 *     ...
 * }
 * ...
 * @endcode
 * @see device_power_add_lock_state_change_callback()
 * @see power_lock_e
 * @see device_power_lock_state_e
 */
typedef void (*device_power_lock_state_change_callback) (power_lock_e power_lock_type,
	device_power_lock_state_e power_lock_state, void *user_data);

/**
 * @brief Callback function type for the registered device power lock count is changed.
 * @details It can be registered by device_power_add_lock_count_change_callback(). \n
 * @since_tizen 7.0
 * @remarks The callback function will be called when the state of a power lock count is changed.
 * @param[out] power_lock_type Type of power lock
 * @param[out] power_lock_count Number of power lock
 * @param[out] user_data User data passed from the callback registration
 *
 * @code
 * #include <stdio.h>
 * #include <device/power-internal.h>
 * ...
 * static void power_lock_state_count_changed_cb(power_lock_e power_lock_type, int power_lock_count, void *user_data)
 * {
 *     printf("Power lock state count changed: type=%d, count=%d\n", power_lock_type, power_lock_count);
 *     ...
 * }
 * ...
 * int main(void)
 * {
 *     int ret = device_power_add_lock_count_change_callback(POWER_LOCK_NORMAL, power_lock_state_count_changed_cb, NULL);
 *     if (ret != DEVICE_ERROR_NONE) {
 *         return -1;
 *     }
 *     ...
 * }
 * ...
 * @endcode
 * @see device_power_add_lock_count_change_callback()
 * @see power_lock_e
 */
typedef void (*device_power_lock_count_change_callback) (power_lock_e power_lock_type,
	int power_lock_count, void *user_data);

/**
 * @brief Gets the status of power lock is locked or not based on specific power lock type.
 * @details Retrieves the status of a power lock.\n
 *          The status is returned as a @a device_power_lock_state_e power_lock_state pointer.
 * @since_tizen 7.0
 * @remarks Ensure that the provided @a power_lock_e power_lock_type pointer is valid and has enough memory allocated.
 * @param[in] power_lock_type Type of power lock
 *                            POWER_LOCK_CPU \n
 *                            POWER_LOCK_DISPLAY \n
 *                            POWER_LOCK_DISPLAY_DIM
 * @param[out] power_lock_state Status of power lock to be get
 *                              DEVICE_POWER_LOCK_STATE_UNLOCK \n
 *                              DEVICE_POWER_LOCK_STATE_LOCK
 * @return @c 0 on success, otherwise a negative error value
 * @retval #DEVICE_ERROR_NONE Successful
 * @retval #DEVICE_ERROR_INVALID_PARAMETER Invalid parameter
 *
 * @code
 * #include <stdio.h>
 * #include <device/power-internal.h>
 * ...
 * device_power_lock_state_e power_lock_state;
 * int ret = device_power_get_lock_state(POWER_LOCK_DISPLAY, &power_lock_state);
 * if (ret == DEVICE_ERROR_NONE) {
 *     ...
 * }
 * ...
 * @endcode
 * @see power_lock_e
 * @see device_power_lock_state_e
 */
int device_power_get_lock_state(power_lock_e power_lock_type,
	device_power_lock_state_e *power_lock_state);

/**
 * @brief Gets the number of locks acquired in the given power lock state.
 * @details Retrieves the status of a power lock.\n
 *          The status is returned as a int @a power_lock_count pointer.
 * @since_tizen 7.0
 * @remarks Ensure that the provided @a power_lock_count pointer is valid and has enough memory allocated.
 * @param[in] power_lock_type Type of power lock
 *                            POWER_LOCK_CPU \n
 *                            POWER_LOCK_DISPLAY \n
 *                            POWER_LOCK_DISPLAY_DIM
 * @param[out] power_lock_count Number of power lock to be get
 * @return @c 0 on success, otherwise a negative error value
 * @retval #DEVICE_ERROR_NONE Successful
 * @retval #DEVICE_ERROR_INVALID_PARAMETER Invalid parameter
 *
 * @code
 * #include <stdio.h>
 * #include <device/power-internal.h>
 * ...
 * int power_lock_count = 0;
 * int ret = device_power_get_lock_count(POWER_LOCK_DISPLAY, &power_lock_count);
 * if (ret == DEVICE_ERROR_NONE) {
 *     ...
 * }
 * ...
 * @endcode
 * @see power_lock_e
 */
int device_power_get_lock_count(power_lock_e power_lock_type,
	int *power_lock_count);

/**
 * @brief Adds a callback to observe status of registered specific power lock.
 * @details The callback will be invoked when the power lock type state is changed.
 * @since_tizen 7.0
 * @remarks Added callback based on @a power_lock_type can be removed by device_power_remove_lock_state_change_callback(). \n
 * @param[in] power_lock_type Power lock type to be observed
 *                            POWER_LOCK_CPU \n
 *                            POWER_LOCK_DISPLAY \n
 *                            POWER_LOCK_DISPLAY_DIM
 * @param[in] power_lock_state_change_callback Callback function
 * @param[in] user_data Data to be passed to the callback function
 * @return @c 0 on success, otherwise a negative error value
 * @retval #DEVICE_ERROR_NONE Successful
 * @retval #DEVICE_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #DEVICE_ERROR_OPERATION_FAILED Operation failed
 *
 * @code
 * #include <stdio.h>
 * #include <device/power-internal.h>
 * ...
 * static void power_lock_state_changed_cb(power_lock_e power_lock_type, device_power_lock_state_e power_lock_state, void *user_data)
 * {
 *     printf("Power lock state changed: type=%d, state=%d\n", power_lock_type, power_lock_state);
 *     ...
 * }
 * ...
 * int main(void)
 * {
 *     int ret = device_power_add_lock_state_change_callback(POWER_LOCK_NORMAL, power_lock_state_changed, NULL);
 *     if (ret != DEVICE_ERROR_NONE) {
 *         return -1;
 *     }
 *     ...
 * }
 * ...
 * @endcode
 * @see device_power_request_lock()
 * @see device_power_release_lock()
 * @see device_power_lock_state_change_callback()
 * @see power_lock_e
 */
int device_power_add_lock_state_change_callback(power_lock_e power_lock_type,
	device_power_lock_state_change_callback power_lock_state_change_callback,
	void *user_data);

/**
 * @brief Removes callback that has registered from device_power_add_lock_state_change_callback().
 * @details Unregisters a callback function that was previously registered using device_power_add_lock_state_change_callback().
 * @since_tizen 7.0
 * @remarks Removes the callback function only for the specified power lock type.
 * @param[in] power_lock_type Type of power lock
 *                            POWER_LOCK_CPU \n
 *                            POWER_LOCK_DISPLAY \n
 *                            POWER_LOCK_DISPLAY_DIM
 * @param[in] power_lock_state_change_callback Callback function to be removed
 * @return @c 0 on success, otherwise a negative error value
 * @retval #DEVICE_ERROR_NONE Successful
 * @retval #DEVICE_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #DEVICE_ERROR_OPERATION_FAILED Operation failed
 *
 * @code
 * #include <stdio.h>
 * #include <device/power-internal.h>
 * ...
 * static void power_lock_state_changed_cb(power_lock_e power_lock_type, device_power_lock_state_e power_lock_state, void *user_data)
 * {
 *     printf("Power lock state changed: type=%d, state=%d\n", power_lock_type, power_lock_state);
 *     ...
 * }
 * ...
 * int main(void)
 * {
 *     int ret = device_power_add_lock_state_change_callback(POWER_LOCK_NORMAL, power_lock_state_changed, NULL);
 *     if (ret != DEVICE_ERROR_NONE) {
 *         return -1;
 *     }
 *     ...
 *     ret = device_power_remove_lock_state_change_callback(POWER_LOCK_NORMAL, power_lock_state_changed);
 * }
 * ...
 * @endcode
 * @see device_power_add_lock_state_change_callback()
 * @see device_power_lock_state_change_callback()
 * @see power_lock_e
 */
int device_power_remove_lock_state_change_callback(power_lock_e power_lock_type,
	device_power_lock_state_change_callback power_lock_state_change_callback);

/**
 * @brief Adds a callback to observe number of lock status of registered specific power lock.
 * @details The callback will be invoked when the number of lock is changed based on speicific power lock type.
 * @since_tizen 7.0
 * @remarks The callback function will be called when the state of a power lock count is changed.
 * @param[in] power_lock_type Power lock type to be observed
 *                            POWER_LOCK_CPU \n
 *                            POWER_LOCK_DISPLAY \n
 *                            POWER_LOCK_DISPLAY_DIM
 * @param[in] power_lock_count_change_callback Callback function
 * @param[in] user_data Data to be passed to the callback function
 * @return @c 0 on success, otherwise a negative error value
 * @retval #DEVICE_ERROR_NONE Successful
 * @retval #DEVICE_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #DEVICE_ERROR_OPERATION_FAILED Operation failed
 *
 * @code
 * #include <stdio.h>
 * #include <device/power-internal.h>
 * ...
 * static void power_lock_state_count_changed_cb(power_lock_e power_lock_type, int power_lock_count, void *user_data)
 * {
 *     printf("Power lock state count changed: type=%d, count=%d\n", power_lock_type, power_lock_count);
 *     ...
 * }
 * ...
 * int main(void)
 * {
 *     int ret = device_power_add_lock_count_change_callback(POWER_LOCK_NORMAL, power_lock_state_count_changed_cb, NULL);
 *     if (ret != DEVICE_ERROR_NONE) {
 *         return -1;
 *     }
 *     ...
 * }
 * ...
 * @endcode
 * @see device_power_request_lock()
 * @see device_power_release_lock()
 * @see device_power_lock_count_change_callback()
 * @see power_lock_e
 */
int device_power_add_lock_count_change_callback(power_lock_e power_lock_type,
	device_power_lock_count_change_callback power_lock_count_change_callback,
	void *user_data);

/**
 * @brief Removes callback that has registered from device_power_add_lock_count_change_callback().
 * @details Unregisters a callback function that was previously registered using device_power_add_lock_count_change_callback().
 * @since_tizen 7.0
 * @remarks Removes the callback function only for the specified power lock type.
 * @param[in] power_lock_type Type of power lock
 *                            POWER_LOCK_CPU \n
 *                            POWER_LOCK_DISPLAY \n
 *                            POWER_LOCK_DISPLAY_DIM
 * @param[in] power_lock_count_change_callback Callback function to be removed
 * @return @c 0 on success, otherwise a negative error value
 * @retval #DEVICE_ERROR_NONE Successful
 * @retval #DEVICE_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #DEVICE_ERROR_OPERATION_FAILED Operation failed
 *
 * @code
 * #include <stdio.h>
 * #include <device/power-internal.h>
 * ...
 * static void power_lock_state_count_changed_cb(power_lock_e power_lock_type, int power_lock_count, void *user_data)
 * {
 *     printf("Power lock state count changed: type=%d, count=%d\n", power_lock_type, power_lock_count);
 *     ...
 * }
 * ...
 * int main(void)
 * {
 *     int ret = device_power_add_lock_count_change_callback(POWER_LOCK_NORMAL, power_lock_state_count_changed_cb, NULL);
 *     if (ret != DEVICE_ERROR_NONE) {
 *         return -1;
 *     }
 *     ...
 *     ret = device_power_remove_lock_count_change_callback(POWER_LOCK_NORMAL, power_lock_state_count_changed_cb);
 * }
 * ...
 * @endcode
 * @see device_power_add_lock_count_change_callback()
 * @see device_power_lock_count_change_callback()
 * @see power_lock_e
 */
int device_power_remove_lock_count_change_callback(power_lock_e power_lock_type,
	device_power_lock_count_change_callback power_lock_count_change_callback);

/**
 * @brief Get current power state such as normal, sleep and poweroff.
 * @details Get current power state and save it into the state argument.
 * @since_tizen 9.0
 * @privilege %http://tizen.org/privilege/power
 * @param[out] state The power state
 * @return @c 0 on success, otherwise a negative error value
 * @retval #DEVICE_ERROR_NONE Successful
 * @retval #DEVICE_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #DEVICE_ERROR_OPERATION_FAILED Operation failed
 *
 * @code
 * #include <device/power-internal.h>
 * ...
 * int ret = 0;
 * device_power_state_e state;
 *
 * ret = device_power_get_power_state(&state);
 * if (ret != DEVICE_ERROR_NONE) {
 *     ....
 * }
 * ...
 * @endcode
 */
int device_power_get_power_state(device_power_state_e *state);
#ifdef __cplusplus
}
#endif

/**
 * @}
 */

#endif /* __TIZEN_SYSTEM_POWER_INTERNAL_H__ */
