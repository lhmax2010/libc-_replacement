/*
 * Copyright (c) 2015 - 2017 Samsung Electronics Co., Ltd All Rights Reserved
 *
 * Licensed under the Apache License, Version 2.0 (the License);
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 * http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an AS IS BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */


#ifndef __TIZEN_APPFW_WIDGET_APP_H__
#define __TIZEN_APPFW_WIDGET_APP_H__

#include <tizen.h>
#include <app_common.h>
#include <bundle.h>
#include <widget_errno.h>

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @addtogroup CAPI_WIDGET_APP_MODULE
 * @{
 */


/**
 * @deprecated Deprecated since 11.0.
 * @brief Enumeration for destroy type of widget instance.
 * @since_tizen 2.3.1
 */
typedef enum widget_app_destroy_type {
	WIDGET_APP_DESTROY_TYPE_PERMANENT = 0x00, /**< User deleted this widget from the viewer */
	WIDGET_APP_DESTROY_TYPE_TEMPORARY = 0x01, /**< Widget is deleted because of other reasons (e.g. widget process is terminated temporarily by the system) */
} widget_app_destroy_type_e;


/**
 * @deprecated Deprecated since 11.0.
 * @brief The widget class handle.
 * @since_tizen 2.3.1
 */
typedef struct _widget_class *widget_class_h;


/**
 * @deprecated Deprecated since 11.0.
 * @brief The widget context handle.
 * @since_tizen 2.3.1
 */
typedef struct _widget_context *widget_context_h;


/**
 * @deprecated Deprecated since 11.0.
 * @brief Called when the widget instance starts.
 * @details The callback function is called after widget instance is created.
 *          In this callback, you can initialize resources for this instance.
 * @since_tizen 2.3.1
 * @param[in] context The context of widget instance
 * @param[in] content The data set for the previous status
 * @param[in] w The pixel value for widget width
 * @param[in] h The pixel value for widget height
 * @param[in] user_data The user data passed from widget_app_class_create() function
 *
 * @return #WIDGET_ERROR_NONE on success,
 *         otherwise an error code (see WIDGET_ERROR_XXX) on failure
 */
typedef int (*widget_instance_create_cb)(widget_context_h context, bundle *content, int w, int h, void *user_data);


/**
 * @deprecated Deprecated since 11.0.
 * @brief Called before the widget instance is destroyed.
 * @details The callback function is called before widget instance is destroyed.
 *          In this callback, you can finalize resources for this instance.
 *          If reason is not #WIDGET_APP_DESTROY_TYPE_TEMPORARY, it should store the current status by using incoming bundle.
 * @since_tizen 2.3.1
 * @remarks Note that the parameter 'content' is used to save the status of the widget instance.
 *         As a input parameter, content contains the saved status of the widget instance.
 *         You can fill the content parameter with the current status in this callback,
 *         then the framework will save the content by receiving it as a output parameter.
 *         Consequently, you should not use widget_app_context_set_content_info() function in this callback.
 *         The content will be overwritten after this callback returns with the 'content' parameter.
 * @param[in] context The context of widget instance
 * @param[in] reason The reason for destruction
 * @param[in,out] content The data set to save
 * @param[in] user_data The user data passed from widget_app_class_create() function
 * @return #WIDGET_ERROR_NONE on success,
 *         otherwise an error code (see WIDGET_ERROR_XXX) on failure
 */
typedef int (*widget_instance_destroy_cb)(widget_context_h context, widget_app_destroy_type_e reason, bundle *content, void *user_data);


/**
 * @deprecated Deprecated since 11.0.
 * @brief Called when the widget is invisible.
 * @details The callback function is called when the widget is invisible.
 *          The paused instance may be destroyed by framework.
 * @since_tizen 2.3.1
 * @param[in] context The context of widget instance
 * @param[in] user_data The user data passed from widget_app_class_create() function
 * @return #WIDGET_ERROR_NONE on success,
 *         otherwise an error code (see WIDGET_ERROR_XXX) on failure
 */
typedef int (*widget_instance_pause_cb)(widget_context_h context, void *user_data);


/**
 * @deprecated Deprecated since 11.0.
 * @brief Called when the widget is visible.
 * @details The callback function is called when the widget is visible.
 * @since_tizen 2.3.1
 * @param[in] context The context of widget instance
 * @param[in] user_data The user data passed from widget_app_class_create() function
 * @return #WIDGET_ERROR_NONE on success,
 *         otherwise an error code (see WIDGET_ERROR_XXX) on failure
 */
typedef int (*widget_instance_resume_cb)(widget_context_h context, void *user_data);


/**
 * @deprecated Deprecated since 11.0.
 * @brief Called before the widget size is changed.
 * @details The callback function is called before the widget size is changed.
 * @since_tizen 2.3.1
 * @param[in] context The context of widget instance
 * @param[in] w The pixel value for widget width
 * @param[in] h The pixel value for widget height
 * @param[in] user_data The user data passed from widget_app_class_create() function
 * @return #WIDGET_ERROR_NONE on success,
 *         otherwise an error code (see WIDGET_ERROR_XXX) on failure
 */
typedef int (*widget_instance_resize_cb)(widget_context_h context, int w, int h, void *user_data);


/**
 * @deprecated Deprecated since 11.0.
 * @brief Called when the event for updating widget is received.
 * @details The callback function is called when the event for updating widget is received.
 * @since_tizen 2.3.1
 * @param[in] context The context of widget instance
 * @param[in] content The data set for updating this widget. It will be provided by requester.
 *                    Requester can use widget_service_trigger_update()
 * @param[in] force Although the widget is paused, if it is TRUE, the widget can be updated
 * @param[in] user_data The user data passed from widget_app_class_create() function
 * @return #WIDGET_ERROR_NONE on success,
 *         otherwise an error code (see WIDGET_ERROR_XXX) on failure
 * @see widget_service_trigger_update()
 */
typedef int (*widget_instance_update_cb)(widget_context_h context, bundle *content, int force, void *user_data);


/**
 * @deprecated Deprecated since 11.0.
 * @brief The structure type containing the set of callback functions for lifecycle of a widget instance.
 * @since_tizen 2.3.1
 */
typedef struct {
	widget_instance_create_cb create; /**< The callback function is called after widget instance is created. */
	widget_instance_destroy_cb destroy; /**< The callback function is called before widget instance is destroyed. */
	widget_instance_pause_cb pause; /**< The callback function is called when the widget is invisible. */
	widget_instance_resume_cb resume; /**< The callback function is called when the widget is visible. */
	widget_instance_resize_cb resize; /**< The callback function is called before the widget size is changed. */
	widget_instance_update_cb update; /**< The callback function is called when the event for updating widget is received. */
} widget_instance_lifecycle_callback_s;


/**
 * @deprecated Deprecated since 11.0.
 * @brief Called when the application starts.
 * @details The callback function is called before the main loop of the application starts.
 *          In this callback, you can initialize resources which can be shared among widget instances.
 *          This function should return the handle for widget class so that it will be used for making instances of widget.
 * @since_tizen 2.3.1
 * @param[in] user_data The user data passed from the callback registration function
 * @return The object of widget class
 * @see widget_app_main()
 * @code
 *
 * static widget_class_h __widget_app_created(void *user_data)
 * {
 *     widget_instance_lifecycle_callback_s callback = { .... };
 *
 *     return widget_app_class_create(callback);
 * }
 *
 * @endcode
 */
typedef widget_class_h (*widget_app_create_cb)(void *user_data);


/**
 * @deprecated Deprecated since 11.0.
 * @brief Called when the application's main loop exits.
 * @details This callback function is called once after the main loop of the application exits.
 *          You should release the application's resources in this function.
 * @since_tizen 2.3.1
 * @param[in] user_data The user data passed from the callback registration function
 * @see widget_app_main()
 */
typedef void (*widget_app_terminate_cb)(void *user_data);


/**
 * @deprecated Deprecated since 11.0.
 * @brief The structure for lifecycle of a widget application.
 * @since_tizen 2.3.1
 */
typedef struct {
	widget_app_create_cb create; /**< The callback function is called before the main loop of the application starts. */
	widget_app_terminate_cb terminate; /**< This callback function is called once after the main loop of the application exits. */
} widget_app_lifecycle_callback_s;


/**
 * @deprecated Deprecated since 11.0.
 * @brief Called for each widget context.
 * @details This function will be called in the function of widget_app_foreach_context() repeatedly.
 * @since_tizen 2.3.1
 * @param[in] context The context for widget instance
 * @param[in] user_data The data for caller
 * @return @c true to continue with the next iteration of the loop,
 *         otherwise @c false to break out of the loop
 * @see widget_app_foreach_context()
 */
typedef bool (*widget_context_cb)(widget_context_h context, void *user_data);


/**
 * @deprecated Deprecated since 11.0.
 * @brief Runs the main loop of the application until widget_app_exit() is called.
 * @since_tizen 2.3.1
 * @param[in] argc The argument count
 * @param[in] argv The argument vector
 * @param[in] callback The set of callback functions to handle application events
 * @param[in] user_data The user data to be passed to the callback functions
 * @return #WIDGET_ERROR_NONE on success,
 *         otherwise an error code (see WIDGET_ERROR_XXX) on failure
 * @retval #WIDGET_ERROR_NONE Successful
 * @retval #WIDGET_ERROR_NOT_SUPPORTED Not supported
 * @retval #WIDGET_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #WIDGET_ERROR_FAULT Unrecoverable error
 * @see widget_app_exit()
 */
int widget_app_main(int argc, char **argv, widget_app_lifecycle_callback_s *callback, void *user_data) TIZEN_DEPRECATED_API;


/**
 * @deprecated Deprecated since 11.0.
 * @brief Exits the main loop of the application.
 * @details The main loop of the application stops and widget_app_terminate_cb() is invoked.
 * @since_tizen 2.3.1
 * @return #WIDGET_ERROR_NONE on success,
 *         otherwise an error code (see WIDGET_ERROR_XXX) on failure
 * @retval #WIDGET_ERROR_NONE Successful
 * @retval #WIDGET_ERROR_NOT_SUPPORTED Not supported
 * @retval #WIDGET_ERROR_FAULT Unrecoverable error
 * @see widget_app_main()
 * @see widget_app_terminate_cb()
 */
int widget_app_exit(void) TIZEN_DEPRECATED_API;


/**
 * @deprecated Deprecated since 11.0.
 * @brief Finishes context for the widget instance.
 * @since_tizen 2.3.1
 * @param[in] context The context for widget instance
 * @return #WIDGET_ERROR_NONE on success,
 *         otherwise an error code (see WIDGET_ERROR_XXX) on failure
 * @retval #WIDGET_ERROR_NONE Successful
 * @retval #WIDGET_ERROR_NOT_SUPPORTED Not supported
 * @retval #WIDGET_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #WIDGET_ERROR_FAULT Unrecoverable error
 */
int widget_app_terminate_context(widget_context_h context) TIZEN_DEPRECATED_API;


/**
 * @deprecated Deprecated since 11.0.
 * @brief Retrieves all widget contexts in this application.
 * @since_tizen 2.3.1
 * @param[in] callback The iteration callback function
 * @param[in] data The data for the callback function
 * @return #WIDGET_ERROR_NONE on success,
 *         otherwise an error code (see WIDGET_ERROR_XXX) on failure
 * @retval #WIDGET_ERROR_NONE Successful
 * @retval #WIDGET_ERROR_NOT_SUPPORTED Not supported
 * @retval #WIDGET_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #WIDGET_ERROR_CANCELED The iteration is canceled
 * @retval #WIDGET_ERROR_FAULT Unrecoverable error
 * @see widget_context_cb()
 */
int widget_app_foreach_context(widget_context_cb callback, void *data) TIZEN_DEPRECATED_API;


/**
 * @deprecated Deprecated since 11.0.
 * @brief Adds the system event handler.
 * @since_tizen 2.3.1
 * @remarks The @a event_handler should be released using widget_app_remove_event_handler().
 * @param[out] event_handler The event handler
 * @param[in] event_type The system event type. #APP_EVENT_DEVICE_ORIENTATION_CHANGED is not supported
 * @param[in] callback The callback function
 * @param[in] user_data The user data to be passed to the callback function
 * @return #WIDGET_ERROR_NONE on success,
 *         otherwise an error code (see WIDGET_ERROR_XXX) on failure
 * @retval #WIDGET_ERROR_NONE Successful
 * @retval #WIDGET_ERROR_NOT_SUPPORTED Not supported
 * @retval #WIDGET_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #WIDGET_ERROR_OUT_OF_MEMORY Out of memory
 * @retval #WIDGET_ERROR_FAULT Unrecoverable error
 * @par Example
 * @code
#include <widget_app.h>

static void
widget_app_event_cb(app_event_info_h event_info, void *user_data)
{
}
...
int main()
{
  int ret;
  app_event_handler_h event_handler = NULL;

  r = widget_app_add_event_handler(&event_handler, APP_EVENT_LOW_BATTERY, widget_app_event_cb, NULL);
  ...
  r = widget_app_remove_event_handler(event_handler);

  return 0;
}
 * @endcode
 * @see app_event_type_e
 * @see app_event_cb()
 * @see widget_app_remove_event_handler()
 */
int widget_app_add_event_handler(app_event_handler_h *event_handler, app_event_type_e event_type,
		app_event_cb callback, void *user_data) TIZEN_DEPRECATED_API;

/**
 * @deprecated Deprecated since 11.0.
 * @brief Removes registered event handler.
 * @since_tizen 2.3.1
 * @param[in] event_handler The event handler
 * @return #WIDGET_ERROR_NONE on success,
 *         otherwise an error code (see WIDGET_ERROR_XXX) on failure
 * @retval #WIDGET_ERROR_NONE Successful
 * @retval #WIDGET_ERROR_NOT_SUPPORTED Not supported
 * @retval #WIDGET_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #WIDGET_ERROR_FAULT Unrecoverable error
 * @see widget_app_add_event_handler()
 */
int widget_app_remove_event_handler(app_event_handler_h event_handler) TIZEN_DEPRECATED_API;


/**
 * @deprecated Deprecated since 11.0.
 * @brief Gets a widget instance id.
 * @since_tizen 2.3.1
 * @remarks The specific error code can be obtained using the get_last_result() method. Error codes are described in Exception section.
 * @remarks You must not free returned widget instance id
 * @remarks The returned widget instance id is volatile. If the device reboots or the widget's process restarts, it will be changed.\n
 *      So, you should not assume this value is a persistent one.
 * @remarks widget_service_trigger_update(), widget_service_change_period(), widget_service_get_content_of_widget_instance()\n
 *      can be called with returned instance id.
 * @param[in] context The context for widget instance
 * @return widget instance id on success,
 *      otherwise NULL
 * @exception #WIDGET_ERROR_NONE Successful
 * @exception #WIDGET_ERROR_NOT_SUPPORTED Not supported
 * @exception #WIDGET_ERROR_INVALID_PARAMETER Invalid parameter
 * @exception #WIDGET_ERROR_FAULT Unrecoverable error
 * @see get_last_result()
 * @see widget_service_trigger_update()
 * @see widget_service_change_period()
 * @see widget_service_get_content_of_widget_instance()
 */
const char *widget_app_get_id(widget_context_h context) TIZEN_DEPRECATED_API;


/**
 * @deprecated Deprecated since 11.0.
 * @brief Makes a class for widget instances.
 * @since_tizen 2.3.1
 * @remarks The specific error code can be obtained using the get_last_result() method. Error codes are described in Exception section.
 * @param[in] callback The set of lifecycle callbacks
 * @param[in] user_data The user data to be passed to the callback functions
 * @return The new widget class object,
 *         NULL on error
 * @exception #WIDGET_ERROR_NONE Successfully added
 * @exception #WIDGET_ERROR_NOT_SUPPORTED Not supported
 * @exception #WIDGET_ERROR_INVALID_PARAMETER Invalid parameter
 * @exception #WIDGET_ERROR_OUT_OF_MEMORY Out of memory
 * @see get_last_result()
 */
widget_class_h widget_app_class_create(widget_instance_lifecycle_callback_s callback, void *user_data) TIZEN_DEPRECATED_API;


/**
 * @deprecated Deprecated since 11.0.
 * @brief Sets a tag in the context.
 * @since_tizen 2.3.1
 * @param[in] context The context for widget instance
 * @param[in] tag The value to save
 * @return #WIDGET_ERROR_NONE on success,
 *         otherwise an error code (see WIDGET_ERROR_XXX) on failure
 * @retval #WIDGET_ERROR_NONE Successful
 * @retval #WIDGET_ERROR_NOT_SUPPORTED Not supported
 * @retval #WIDGET_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #WIDGET_ERROR_FAULT Unrecoverable error
 */
int widget_app_context_set_tag(widget_context_h context, void *tag) TIZEN_DEPRECATED_API;


/**
 * @deprecated Deprecated since 11.0.
 * @brief Gets the tag in the context.
 * @since_tizen 2.3.1
 * @remarks Releasing the @a tag value depends on the value that set by widget_app_context_set_tag() function.
 * @param[in] context The context for widget instance
 * @param[out] tag The value to get
 * @return #WIDGET_ERROR_NONE on success,
 *         otherwise an error code (see WIDGET_ERROR_XXX) on failure
 * @retval #WIDGET_ERROR_NONE Successful
 * @retval #WIDGET_ERROR_NOT_SUPPORTED Not supported
 * @retval #WIDGET_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #WIDGET_ERROR_FAULT Unrecoverable error
 * @see widget_app_context_set_tag()
 */
int widget_app_context_get_tag(widget_context_h context, void **tag) TIZEN_DEPRECATED_API;


/**
 * @deprecated Deprecated since 11.0.
 * @brief Sets the content info to the widget.
 * @since_tizen 2.3.1
 * @param[in] context The context for widget instance
 * @param[in] content_info The data set to save
 * @return #WIDGET_ERROR_NONE on success,
 *         otherwise an error code (see WIDGET_ERROR_XXX) on failure
 * @retval #WIDGET_ERROR_NONE Successfully sent
 * @retval #WIDGET_ERROR_NOT_SUPPORTED Not supported
 * @retval #WIDGET_ERROR_INVALID_PARAMETER Invalid argument
 * @retval #WIDGET_ERROR_OUT_OF_MEMORY Out of memory
 * @retval #WIDGET_ERROR_FAULT Unrecoverable error
 */
int widget_app_context_set_content_info(widget_context_h context, bundle *content_info) TIZEN_DEPRECATED_API;


/**
 * @deprecated Deprecated since 11.0.
 * @brief Sends the title to the widget.
 * @since_tizen 2.3.1
 * @param[in] context The context for widget instance
 * @param[in] title When an accessibility mode is turned on, this string will be read
 * @return #WIDGET_ERROR_NONE on success,
 *         otherwise an error code (see WIDGET_ERROR_XXX) on failure
 * @retval #WIDGET_ERROR_NONE Successfully sent
 * @retval #WIDGET_ERROR_NOT_SUPPORTED Not supported
 * @retval #WIDGET_ERROR_INVALID_PARAMETER Invalid argument
 * @retval #WIDGET_ERROR_OUT_OF_MEMORY Out of memory
 * @retval #WIDGET_ERROR_FAULT Unrecoverable error
 */
int widget_app_context_set_title(widget_context_h context, const char *title) TIZEN_DEPRECATED_API;


/**
 * @deprecated Deprecated since 11.0.
 * @brief Adds an additional widget class for multi-class of widget instantiation.
 * @since_tizen 3.0
 * @remarks The specific error code can be obtained using the get_last_result() method. Error codes are described in Exception section.
 * @param[in] widget_class The result of widget_app_class_create()
 * @param[in] class_id The class id of provider
 * @param[in] callback The set of lifecycle callbacks
 * @param[in] user_data The user data to be passed to the callback functions
 * @return The new widget class object,
 *         NULL on error
 * @exception #WIDGET_ERROR_NONE Successfully added
 * @exception #WIDGET_ERROR_NOT_SUPPORTED Not supported
 * @exception #WIDGET_ERROR_INVALID_PARAMETER Invalid parameter
 * @exception #WIDGET_ERROR_OUT_OF_MEMORY Out of memory
 * @see get_last_result()
 */
widget_class_h widget_app_class_add(widget_class_h widget_class, const char *class_id,
		widget_instance_lifecycle_callback_s callback, void *user_data) TIZEN_DEPRECATED_API;

/**
 * @}
 */

#ifdef __cplusplus
}
#endif

#endif /* __TIZEN_APPFW_WIDGET_APP_H__ */
