/*
 * Copyright (c) 2019 - 2021 Samsung Electronics Co., Ltd.
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

#ifndef __TIZEN_COMPONENT_BASED_COMPONENT_COMMON_H__
#define __TIZEN_COMPONENT_BASED_COMPONENT_COMMON_H__

#include <tizen.h>
#include <app_control.h>

#ifdef __cplusplus
extern "C" {
#endif

#ifndef TIZEN_ERROR_COMPONENT
#define TIZEN_ERROR_COMPONENT -0x03030000
#endif

/**
 * @addtogroup COMPONENT_BASED_COMPONENT_COMMON_MODULE
 * @{
 */


/**
 * @deprecated Deprecated since 11.0.
 * @brief Enumeration for component error.
 * @since_tizen 5.5
 */
typedef enum {
	COMPONENT_ERROR_NONE = TIZEN_ERROR_NONE, /**< Successful */
	COMPONENT_ERROR_INVALID_PARAMETER = TIZEN_ERROR_INVALID_PARAMETER, /**< Invalid parameter */
	COMPONENT_ERROR_OUT_OF_MEMORY = TIZEN_ERROR_OUT_OF_MEMORY, /**< Out of memory */
	COMPONENT_ERROR_INVALID_CONTEXT = TIZEN_ERROR_COMPONENT | 0x01, /**< Invalid component context */
	COMPONENT_ERROR_NOT_SUPPORTED = TIZEN_ERROR_NOT_SUPPORTED, /**< Not supported */
	COMPONENT_ERROR_NOT_FOUND = TIZEN_ERROR_COMPONENT | 0x02, /**< Not found */
	COMPONENT_ERROR_LAUNCH_REJECTED = TIZEN_ERROR_COMPONENT | 0x03, /**< The application cannot be launched now */
	COMPONENT_ERROR_LAUNCH_FAILED = TIZEN_ERROR_COMPONENT | 0x04, /**< Internal launch error */
	COMPONENT_ERROR_TIMED_OUT = TIZEN_ERROR_TIMED_OUT, /**< Time out */
	COMPONENT_ERROR_PERMISSION_DENIED = TIZEN_ERROR_PERMISSION_DENIED, /**< Permission denied */
} component_error_e;

/**
 * @deprecated Deprecated since 11.0.
 * @brief Enumeration for device orientation.
 * @since_tizen 5.5
 */
typedef enum {
	COMPONENT_DEVICE_ORIENTATION_0 = 0, /**< The device is oriented in a natural position */
	COMPONENT_DEVICE_ORIENTATION_90 = 90, /**< The device's left side is at the top */
	COMPONENT_DEVICE_ORIENTATION_180 = 180, /**< The device is upside down */
	COMPONENT_DEVICE_ORIENTATION_270 = 270, /**< The device's right side is at the top */
} component_device_orientation_e;

/**
 * @deprecated Deprecated since 11.0.
 * @brief Enumeration for low memory status.
 * @since_tizen 5.5
 */
typedef enum {
	COMPONENT_LOW_MEMORY_NONE = 0x00, /**< None */
	COMPONENT_LOW_MEMORY_NORMAL = 0x01, /**< Normal status */
	COMPONENT_LOW_MEMORY_SOFT_WARNING = 0x02, /**< Soft warning status */
	COMPONENT_LOW_MEMORY_HARD_WARNING = 0x04, /**< Hard warning status */
} component_low_memory_status_e;

/**
 * @deprecated Deprecated since 11.0.
 * @brief Enumeration for battery status.
 * @since_tizen 5.5
 */
typedef enum {
	COMPONENT_LOW_BATTERY_NONE, /**< None */
	COMPONENT_LOW_BATTERY_POWER_OFF, /**< The battery status is under 1% */
	COMPONENT_LOW_BATTERY_CRITICAL_LOW, /**< The battery status is under 5% */
} component_low_battery_status_e;

/**
 * @deprecated Deprecated since 11.0.
 * @brief Enumeration for suspended state.
 * @since_tizen 5.5
 */
typedef enum {
	COMPONENT_SUSPENDED_STATE_WILL_ENTER = 0, /**< The component will enter the suspended state */
	COMPONENT_SUSPENDED_STATE_DID_EXIT, /**< The component did exit from the suspended state */
} component_suspended_state_e;

/**
 * @deprecated Deprecated since 11.0.
 * @brief Enumeration for display status.
 * @since_tizen 5.5
 */
typedef enum {
        COMPONENT_DISPLAY_STATUS_ON, /**< The display status is on */
        COMPONENT_DISPLAY_STATUS_OFF, /**< The display status is off */
} component_display_status_e;

/**
 * @deprecated Deprecated since 11.0.
 * @brief The component class handle.
 * @since_tizen 5.5
 */
typedef void *component_class_h;

/**
 * @deprecated Deprecated since 11.0.
 * @brief The component context handle.
 * @since_tizen 5.5
 */
typedef void *component_h;

/**
 * @deprecated Deprecated since 11.0.
 * @brief Gets the ID of the component.
 * @since_tizen 5.5
 * @remarks @c id must be released using free().
 *
 * @param[in]   context         The context of the component instance
 * @param[out]  id              The ID of the component
 * @return      @c 0 on success,
 *              otherwise a negative error value
 * @retval #COMPONENT_ERROR_NONE Successful
 * @retval #COMPONENT_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #COMPONENT_ERROR_OUT_OF_MEMORY Out of memory
 */
int component_get_id(component_h context, char **id) TIZEN_DEPRECATED_API;

/**
 * @deprecated Deprecated since 11.0.
 * @brief Gets the instance ID of the component.
 * @since_tizen 5.5
 * @remarks @c instance_id must be released using free().
 *
 * @param[in]   context         The context of the component instance
 * @param[out]  instance_id     The instance ID of the component
 * @return      @c 0 on success,
 *              otherwise a negative error value
 * @retval #COMPONENT_ERROR_NONE Successful
 * @retval #COMPONENT_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #COMPONENT_ERROR_OUT_OF_MEMORY Out of memory
 */
int component_get_instance_id(component_h context, char **instance_id) TIZEN_DEPRECATED_API;

/**
 * @deprecated Deprecated since 11.0.
 * @brief Registers the app_control action.
 * @details This function is for handling each application control action.
 *          @a action must match id attribute of app_control element in the tizen-manifest.xml.
 * @since_tizen 5.5
 *
 * @param[in]   context         The context of the component instance
 * @param[in]   action          The name of the app_control action
 *
 * @return      @c 0 on success,
 *              otherwise a negative error value
 * @retval #COMPONENT_ERROR_NONE Successful
 * @retval #COMPONENT_ERROR_INVALID_PARAMETER Invalid parameter
 * @see frame_component_action_cb()
 * @see component_deregister_action()
 */
int component_register_action(component_h context, const char *action) TIZEN_DEPRECATED_API;

/**
 * @deprecated Deprecated since 11.0.
 * @brief Deregisters the registered app_control action.
 * @since_tizen 5.5
 *
 * @param[in]   context         The context of the component instance
 * @param[in]   action          The name of the app_control action
 *
 * @return      @c 0 on success,
 *              otherwise a negative error value
 * @retval #COMPONENT_ERROR_NONE Successful
 * @retval #COMPONENT_ERROR_INVALID_PARAMETER Invalid parameter
 */
int component_deregister_action(component_h context,
		const char *action) TIZEN_DEPRECATED_API;

/**
 * @deprecated Deprecated since 11.0.
 * @brief Sends the launch request asynchronously.
 * @details To use group mode, you must use this function instead of app_control_send_launch_request_async().
 * @since_tizen 5.5
 * @privlevel public
 * @privilege %http://tizen.org/privilege/appmanager.launch
 *
 * @param[in]   context         The context of the component instance
 * @param[in]   app_control     The app_control handle
 * @param[in]   result_cb       The callback function to be called when the result is delivered
 * @param[in]   reply_cb        The callback function to be called when the reply is delivered
 * @param[in]   user_data       The user data to be passed to the callback function
 *
 * @return      @c 0 on success,
 *              otherwise a negative error value
 * @retval #COMPONENT_ERROR_NONE Successful
 * @retval #COMPONENT_ERROR_PERMISSION_DENIED Permission denied
 * @retval #COMPONENT_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #COMPONENT_ERROR_OUT_OF_MEMORY Out of memory
 * @retval #COMPONENT_ERROR_NOT_FOUND The application to run the given launch request is not found
 * @retval #COMPONENT_ERROR_LAUNCH_REJECTED The application cannot be launched in current context
 * @retval #COMPONENT_ERROR_LAUNCH_FAILED Failed to launch the application
 * @retval #COMPONENT_ERROR_TIMED_OUT Failed due to timeout. The application that handles @a app_control may be busy
 * @see app_control_result_cb()
 * @see app_control_reply_to_launch_request()
 * @see app_control_reply_cb()
 * @see app_control_enable_app_started_result_event()
 * @see app_control_send_launch_request_async()
 */
int component_send_launch_request_async(component_h context,
		app_control_h app_control,
                app_control_result_cb result_cb,
                app_control_reply_cb reply_cb,
                void *user_data) TIZEN_DEPRECATED_API;

/**
 * @deprecated Deprecated since 11.0.
 * @brief Sends the launch request synchronously.
 * @details To use group mode, you must use this function instead of app_control_send_launch_request_sync().
 * @since_tizen 5.5
 * @privlevel public
 * @privilege %http://tizen.org/privilege/appmanager.launch
 * @remarks The @a reply must be released using app_control_destroy().
 *
 * @param[in]   context         The context of the component instance
 * @param[in]   app_control     The app_control handle
 * @param[out]  reply           The app_control handle in which the results of the callee are contained
 * @param[out]  result          The result code of the launch request
 *
 * @return      @c 0 on success,
 *              otherwise a negative error value
 * @retval #COMPONENT_ERROR_NONE Successful
 * @retval #COMPONENT_ERROR_PERMISSION_DENIED Permission denied
 * @retval #COMPONENT_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #COMPONENT_ERROR_OUT_OF_MEMORY Out of memory
 * @retval #COMPONENT_ERROR_NOT_FOUND The application to run the given launch request is not found
 * @retval #COMPONENT_ERROR_LAUNCH_REJECTED The application cannot be launched in current context
 * @retval #COMPONENT_ERROR_LAUNCH_FAILED Failed to launch the application
 * @retval #COMPONENT_ERROR_TIMED_OUT Failed due to timeout. The application that handles @a app_control may be busy
 * @see app_control_destroy()
 * @see app_control_send_launch_request_sync()
 */
int component_send_launch_request_sync(component_h context,
                app_control_h app_control,
                app_control_h *reply,
		app_control_result_e *result) TIZEN_DEPRECATED_API;

/**
 * @deprecated Deprecated since 11.0.
 * @brief Finishes the component instance.
 * @since_tizen 5.5
 *
 * @param[in]   context         The context of the component instance
 *
 * @return @c 0 on success,
 *         otherwise a negative error value
 * @retval #COMPONENT_ERROR_NONE Successful
 * @retval #COMPONENT_ERROR_INVALID_PARAMETER Invalid parameter
 * @see frame_component_destroy_cb()
 * @see service_component_destroy_cb()
 */
int component_finish(component_h context) TIZEN_DEPRECATED_API;


/**
 * @}
 */

#ifdef __cplusplus
}
#endif
#endif /* __TIZEN_COMPONENT_BASED_COMPONENT_COMMON_H__ */

