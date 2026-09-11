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

#ifndef __TIZEN_COMPONENT_BASED_FRAME_COMPONENT_H__
#define __TIZEN_COMPONENT_BASED_FRAME_COMPONENT_H__

#include <Elementary.h>
#include <app_control.h>
#include <bundle.h>
#include <component_common.h>

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @addtogroup COMPONENT_BASED_FRAME_COMPONENT_MODULE
 * @{
 */


/**
 * @deprecated Deprecated since 10.0.
 * @brief Called when the frame component instance is created.
 * @details The returned Evas_Object MUST NOT be released using evas_object_del(). The platform frees the window when the frame component instance is destroyed.
 * @since_tizen 5.5
 * @remarks The @a context should not be released. The @a context can be used only in the callback.
 *
 * @param[in]   context         The context of the frame component instance
 * @param[in]   user_data       The user data passed from component_based_app_add_frame_component() function
 *
 * @return      The Evas object on success,
 *              otherwise a null pointer on failure.
 * @see frame_component_get_window()
 */
typedef Evas_Object *(*frame_component_create_cb)(
		component_h context,
		void *user_data);

/**
 * @deprecated Deprecated since 10.0.
 * @brief Called when the frame component instance is started.
 * @since_tizen 5.5
 * @remarks The @a context should not be released. The @a context can be used only in the callback.
 * @remarks The @a app_control should not be released. The @a app_control can be used only in the callback.
 *          To use outside, make a copy using app_control_clone().
 *
 * @param[in]   context         The context of the frame component instance
 * @param[in]   app_control     The app control handle
 * @param[in]   restarted       @c true, if the instance is restarted
 * @param[in]   user_data       The user data passed from component_based_app_add_frame_component() function
 */
typedef void (*frame_component_start_cb)(
		component_h context,
		app_control_h app_control,
		bool restarted,
		void *user_data);

/**
 * @deprecated Deprecated since 10.0.
 * @brief Called when the frame component becomes visible.
 * @since_tizen 5.5
 * @remarks The @a context should not be released. The @a context can be used only in the callback.
 *
 * @param[in]   context         The context of the frame component instance
 * @param[in]   user_data       The user data passed from component_based_app_add_frame_component() function
 */
typedef void (*frame_component_resume_cb)(
		component_h context,
		void *user_data);

/**
 * @deprecated Deprecated since 10.0.
 * @brief Called when the frame component becomes invisible.
 * @since_tizen 5.5
 * @remarks The @a context should not be released. The @a context can be used only in the callback.
 *
 * @param[in]   context         The context of the frame component instance
 * @param[in]   user_data       The user data passed from component_based_app_add_frame_component() function
 */
typedef void (*frame_component_pause_cb)(
		component_h context,
		void *user_data);

/**
 * @deprecated Deprecated since 10.0.
 * @brief Called before the frame component instance is stopped.
 * @since_tizen 5.5
 * @remarks The @a context should not be released. The @a context can be used only in the callback.
 *
 * @param[in]   context         The context of the frame component instance
 * @param[in]   user_data       The user data passed from component_based_app_add_frame_component() function
 */
typedef void (*frame_component_stop_cb)(
		component_h context,
		void *user_data);

/**
 * @deprecated Deprecated since 10.0.
 * @brief Called before the frame component instance is destroyed.
 * @since_tizen 5.5
 * @remarks The @a context should not be released. The @a context can be used only in the callback.
 *
 * @param[in]   context         The context of the frame component instance
 * @param[in]   user_data       The user data passed from component_based_app_add_frame_component() function
 */
typedef void (*frame_component_destroy_cb)(
		component_h context,
		void *user_data);

/**
 * @deprecated Deprecated since 10.0.
 * @brief Called after the content information of the frame component instance is restored.
 * @since_tizen 5.5
 * @remarks The @a content should not be released. The @a content can be used only in the callback.
 *          To use outside, make a copy using bundle_dup().
 *
 * @param[in]   context         The context of the frame component instance
 * @param[in]   content         The content information
 * @param[in]   user_data       The user data passed from component_based_app_add_frame_component() function
 */
typedef void (*frame_component_restore_content_cb)(
		component_h context,
		bundle *content,
		void *user_data);

/**
 * @deprecated Deprecated since 10.0.
 * @brief Called before the content information of the frame component instance is saved.
 * @since_tizen 5.5
 * @remarks The @a content should not be released. The @a content can be used only in the callback.
 *          To use outside, make a copy using bundle_dup().
 *
 * @param[in]   context         The context of the frame component instance
 * @param[in]   content         The content information
 * @param[in]   user_data       The user data passed from component_based_app_add_frame_component() function
 */
typedef void (*frame_component_save_content_cb)(
		component_h context,
		bundle *content,
		void *user_data);

/**
 * @deprecated Deprecated since 10.0.
 * @brief Called when another application sends a launch request to the component.
 * @details Before calling frame_component_start_cb(), this callback function is called.
 * @since_tizen 5.5
 * @remarks After this callback function returns, the handle of the app_control is released.
 *          Therefore, if you want to use the handle after returning this callback function, you MUST copy it by using app_control_clone() function.
 * @remarks The @a action must not be deallocated by the component. The @a action is managed by the platform and will be released when the app_control action is unregistered.
 * @remarks The @a context should not be released. The @a context can be used only in the callback.
 * @remarks The @a app_control should not be released. The @a app_control can be used only in the callback.
 *          To use outside, make a copy using app_control_clone().
 *
 * @param[in]   context         The context of the frame component instance
 * @param[in]   action          The name of the app_control action
 * @param[in]   app_control     The handle of the app_control
 * @param[in]   user_data       The user data passed from component_based_app_add_frame_component() function
 * @see component_register_action()
 * @see component_deregister_action()
 */
typedef void (*frame_component_action_cb)(
		component_h context,
		const char *action,
		app_control_h app_control,
		void *user_data);

/**
 * @deprecated Deprecated since 10.0.
 * @brief Called when the device orientation is changed.
 * @since_tizen 5.5
 * @remarks The @a context should not be released. The @a context can be used only in the callback.
 *
 * @param[in]   context         The context of the frame component instance
 * @param[in]   orientation     The device orientation
 * @param[in]   user_data       The user data passed from component_based_app_add_frame_component() function
 */
typedef void (*frame_component_device_orientation_changed_cb)(
		component_h context,
		component_device_orientation_e orientation,
		void *user_data);

/**
 * @deprecated Deprecated since 10.0.
 * @brief Called when the system language is changed.
 * @since_tizen 5.5
 * @remarks The @a context should not be released. The @a context can be used only in the callback.
 * @remarks The @a language must not be deallocated by the component. The @a language can be used only in the callback.
 *
 * @param[in]   context         The context of the frame component instance
 * @param[in]   language        The language
 * @param[in]   user_data       The user data passed from component_based_app_add_frame_component() function
 */
typedef void (*frame_component_language_changed_cb)(
		component_h context,
		const char *language,
		void *user_data);

/**
 * @deprecated Deprecated since 10.0.
 * @brief Called when the system region format is changed.
 * @since_tizen 5.5
 * @remarks The @a context should not be released. The @a context can be used only in the callback.
 * @remarks The @a region must not be deallocated by the component. The @a region can be used only in the callback.
 *
 * @param[in]   context         The context of the frame component instance
 * @param[in]   region          The region format
 * @param[in]   user_data       The user data passed from component_based_app_add_frame_component() function
 */
typedef void (*frame_component_region_format_changed_cb)(
		component_h context,
		const char *region,
		void *user_data);

/**
 * @deprecated Deprecated since 10.0.
 * @brief Called when the battery status is changed.
 * @since_tizen 5.5
 * @remarks The @a context should not be released. The @a context can be used only in the callback.
 *
 * @param[in]   context         The context of the frame component instance
 * @param[in]   status          The low battery status
 * @param[in]   user_data       The user data passed from component_based_app_add_frame_component() function
 */
typedef void (*frame_component_low_battery_cb)(
		component_h context,
		component_low_battery_status_e status,
		void *user_data);

/**
 * @deprecated Deprecated since 10.0.
 * @brief Called when the memory status is changed.
 * @since_tizen 5.5
 * @remarks The @a context should not be released. The @a context can be used only in the callback.
 *
 * @param[in]   context         The context of the frame component instance
 * @param[in]   status          The low memory status
 * @param[in]   user_data       The user data passed from component_based_app_add_frame_component() function
 */
typedef void (*frame_component_low_memory_cb)(
		component_h context,
		component_low_memory_status_e status,
		void *user_data);

/**
 * @deprecated Deprecated since 10.0.
 * @brief Called when the suspended state of the frame component is changed.
 * @since_tizen 5.5
 * @remarks The @a context should not be released. The @a context can be used only in the callback.
 *
 * @param[in]   context         The context of the frame component instance
 * @param[in]   state           The suspended state
 * @param[in]   user_data       The user data passed from component_based_app_add_frame_component() function
 */
typedef void (*frame_component_suspended_state_changed_cb)(
		component_h context,
		component_suspended_state_e state,
		void *user_data);

/**
 * @deprecated Deprecated since 10.0.
 * @brief Called when the time zone of the frame component is changed.
 * @details An application can obtain a changed time zone (e.g., "+9:00") and a time zone id (e.g., "Asia/Seoul").
 * @since_tizen 8.0
 * @remarks The @a context should not be released. The @a context can be used only in the callback.
 * @remarks @a time_zone and @a time_zone_id must not be deallocated by the component. @a time_zone and @a time_zone_id can be used only in the callback.
 *
 * @param[in]   context         The context of the frame component instance
 * @param[in]   time_zone       The time zone
 * @param[in]   time_zone_id    The time zone id
 * @param[in]   user_data       The user data passed from component_based_app_add_frame_component() function
 */
typedef void (*frame_component_time_zone_changed_cb)(
		component_h context,
		const char *time_zone,
		const char *time_zone_id,
		void *user_data);

/**
 * @deprecated Deprecated since 10.0.
 * @brief The structure type containing the set of callback functions for lifecycle of a frame component instance.
 * @since_tizen 5.5
 */
typedef struct {
	frame_component_create_cb create; /**< The callback function called after the frame component instance is created. */
	frame_component_start_cb start; /**< The callback function called when the frame component instance is started. */
	frame_component_resume_cb resume; /**< The callback function called when the frame component becomes visible. */
	frame_component_pause_cb pause; /**< The callback function called when the frame component becomes invisible. */
	frame_component_stop_cb stop; /**< The callback function called before the frame component instance is stopped. */
	frame_component_destroy_cb destroy; /**< The callback function called before the frame component instance is destroyed. */
	frame_component_restore_content_cb restore_content; /**< The callback function called when the content information of the frame component instance is restored. */
	frame_component_save_content_cb save_content; /**< The callback function called before the content information of the frame component instance is saved. */
	frame_component_action_cb action; /**< The callback function called when another application sends a launch request to the component. */
	frame_component_device_orientation_changed_cb device_orientation_changed; /**< The callback function called when the device orientation is changed. */
	frame_component_language_changed_cb language_changed; /**< The callback function called when the system language is changed */
	frame_component_region_format_changed_cb region_format_changed; /**< The callback function called when the system region format is changed */
	frame_component_low_battery_cb low_battery; /**< The callback function called when the battery status is changed. */
	frame_component_low_memory_cb low_memory; /**< The callback function called when the memory status is changed */
	frame_component_suspended_state_changed_cb suspended_state_changed; /**< The callback function called when the suspended state of the frame component is changed */
	frame_component_time_zone_changed_cb time_zone_changed; /**< The callback function called when the time zone of the frame component is changed (Since 8.0) */
} frame_component_lifecycle_callback_s;

/**
 * @deprecated Deprecated since 10.0.
 * @brief Gets the display status.
 * @since_tizen 5.5
 *
 * @param[in]   context         The context of the frame component instance
 * @param[out]  display_status  The display status
 *
 * @return      @c 0 on success,
 *              otherwise a negative error value
 * @retval #COMPONENT_ERROR_NONE Successful
 * @retval #COMPONENT_ERROR_NOT_SUPPORTED Not supported
 * @retval #COMPONENT_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #COMPONENT_ERROR_INVALID_CONTEXT The display is in an unknown state.
 */
int frame_component_get_display_status(component_h context,
		component_display_status_e *display_status) TIZEN_DEPRECATED_API;

/**
 * @deprecated Deprecated since 10.0.
 * @brief Gets an Evas object for the frame component.
 * @since_tizen 5.5
 * @remarks The @c window MUST NOT be released using evas_object_del().
 *          The platform frees window when the frame component instance is destroyed.
 *
 * @param[in]   context         The context of the frame component instance
 * @param[out]  window          The evas object for window
 *
 * @return      @c 0 on success,
 *              otherwise a negative error value
 * @retval #COMPONENT_ERROR_NONE Successful
 * @retval #COMPONENT_ERROR_INVALID_PARAMETER Invalid parameter
 */
int frame_component_get_window(component_h context,
		Evas_Object **window) TIZEN_DEPRECATED_API;


/**
 * @}
 */

#ifdef __cplusplus
}
#endif

#endif /* __TIZEN_COMPONENT_BASED_FRAME_COMPONENT_H__ */

