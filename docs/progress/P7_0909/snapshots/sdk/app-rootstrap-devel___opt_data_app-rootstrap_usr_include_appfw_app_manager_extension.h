/*
 * Copyright (c) 2011 - 2016 Samsung Electronics Co., Ltd All Rights Reserved
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

#ifndef __TIZEN_APPFW_APP_MANAGER_EXTENSION_H
#define __TIZEN_APPFW_APP_MANAGER_EXTENSION_H

#include <stdbool.h>

#include "app_manager.h"

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @file app_manager_extension.h
 */

/**
 * @addtogroup CAPI_APPLICATION_MANAGER_MODULE
 * @{
 */

/**
 * @brief Enumeration for Application Context Status Event.
 * @since_tizen 3.0
 */
typedef enum {
	APP_CONTEXT_STATUS_LAUNCHED,	/**< The application is launched */
	APP_CONTEXT_STATUS_TERMINATED,	/**< The application is terminated */
} app_context_status_e;

/**
 * @brief Called when an application is launched or terminated.
 * @since_tizen 3.0
 * @param[in]   app_context     The application context of the application launched or terminated
 * @param[in]   status          The application context status
 * @param[in]   user_data       The user data passed from app_manager_set_app_context_status_cb()
 * @pre This function is called when an application gets launched or terminated, after you register this callback using app_manager_set_app_context_status_cb().
 * @see app_manager_set_app_context_status_cb()
 */
typedef void (*app_manager_app_context_status_cb)(app_context_h app_context, app_context_status_e status, void *user_data);

/**
 * @brief  Terminates the application.
 * @since_tizen 2.3
 * @privlevel platform
 * @privilege %http://tizen.org/privilege/appmanager.kill
 * @param[in]   app_context  The application context
 * @return      @c 0 on success,
 *              otherwise a negative error value
 * @retval      #APP_MANAGER_ERROR_NONE                 Successful
 * @retval      #APP_MANAGER_ERROR_PERMISSION_DENIED    Permission denied
 * @retval      #APP_MANAGER_ERROR_INVALID_PARAMETER    Invalid parameter
 * @retval      #APP_MANAGER_ERROR_REQUEST_FAILED       Internal terminate error
 */
int app_manager_terminate_app(app_context_h app_context);

/**
 * @brief Sets the display flag to enable/disable the splash screen.
 * @since_tizen 3.0
 * @privlevel platform
 * @privilege %http://tizen.org/privilege/packagemanager.admin
 * @param[in]     app_id  The ID of the application
 * @param[in]     display The display flag to enable/disable the splash screen
 *
 * @return      @c 0 on success,
 *              otherwise a negative error value
 * @retval      #APP_MANAGER_ERROR_NONE                 Successful
 * @retval      #APP_MANAGER_ERROR_PERMISSION_DENIED    Permission denied
 * @retval      #APP_MANAGER_ERROR_INVALID_PARAMETER    Invalid parameter
 * @retval      #APP_MANAGER_ERROR_OUT_OF_MEMORY        Out of memory
 * @retval      #APP_MANAGER_ERROR_IO_ERROR             Internal I/O error
 */
int app_manager_set_splash_screen_display(const char *app_id, bool display);

/**
 * @brief Registers a callback function to be invoked when the application change status.
 * @since_tizen 3.0
 * @param[in]   callback        The callback function to register
 * @param[in]   appid           The appid to get status
 * @param[in]   user_data       The user data to be passed to the callback function
 * @return      @c 0 on success,
 *              otherwise a negative error value
 * @retval      #APP_MANAGER_ERROR_NONE                 Successful
 * @retval      #APP_MANAGER_ERROR_INVALID_PARAMETER    Invalid parameter
 * @retval      #APP_MANAGER_ERROR_IO_ERROR             Internal I/O error
 * @retval      #APP_MANAGER_ERROR_OUT_OF_MEMORY        Out of memory
 * @post It will invoke app_manager_app_context_status_cb() when the application is launched or terminated.
 * @see app_manager_app_context_status_cb()
 */
int app_manager_set_app_context_status_cb(app_manager_app_context_status_cb callback, const char *appid, void *user_data);

/**
 * @brief  Gets the application context for the given IDs of the application.
 * @since_tizen 3.0
 * @remarks  This function returns #APP_MANAGER_ERROR_NO_SUCH_APP if the application with the given application IDs is not running. \n
 *           You must release @a app_context using app_context_destroy().
 * @param[in]   app_id       The ID of the application
 * @param[in]   instance_id  The Instance ID of the application
 * @param[out]  app_context  The application context of the given application IDs
 * @return      @c 0 on success,
 *              otherwise a negative error value
 * @retval  #APP_MANAGER_ERROR_NONE               Successful
 * @retval  #APP_MANAGER_ERROR_INVALID_PARAMETER  Invalid parameter
 * @retval  #APP_MANAGER_ERROR_OUT_OF_MEMORY      Out of memory
 * @retval  #APP_MANAGER_ERROR_NO_SUCH_APP        No such application
 */
int app_manager_get_app_context_by_instance_id(const char *app_id, const char *instance_id, app_context_h *app_context);

/**
 * @brief  Gets the focused application context.
 * @since_tizen 3.0
 * @remarks  This function returns #APP_MANAGER_ERROR_NO_SUCH_APP if all applications don't have focus. \n
 *           You must release @a app_context using app_context_destroy().
 *
 * @param[out]  app_context  The focused application context
 * @return      @c 0 on success,
 *              otherwise a negative error value
 * @retval  #APP_MANAGER_ERROR_NONE               Successful
 * @retval  #APP_MANAGER_ERROR_INVALID_PARAMETER  Invalid parameter
 * @retval  #APP_MANAGER_ERROR_IO_ERROR           Internal I/O error
 * @retval  #APP_MANAGER_ERROR_OUT_OF_MEMORY      Out of memory
 * @retval  #APP_MANAGER_ERROR_NO_SUCH_APP        No such application
 */
int app_manager_get_focused_app_context(app_context_h *app_context);

/**
 * @brief  Attaches the window of the child application to the window of the parent application.
 * @since_tizen 3.0
 * @remarks This function is only available for platform level signed applications.
 * @param[in]   parent_app_id The ID of the parent application
 * @param[in]   child_app_id  The ID of the child applicatio
 * @return      @c 0 on success,
 *              otherwise a negative error value
 * @retval  #APP_MANAGER_ERROR_NONE               Successful
 * @retval  #APP_MANAGER_ERROR_INVALID_PARAMETER  Invalid parameter
 * @retval  #APP_MANAGER_ERROR_IO_ERROR           I/O error
 * @retval  #APP_MANAGER_ERROR_OUT_OF_MEMORY      Out of memory
 * @retval  #APP_MANAGER_ERROR_NO_SUCH_APP        No such application
 * @retval  #APP_MANAGER_ERROR_PERMISSION_DENIED  Permission denied
 *
 */
int app_manager_attach_window(const char *parent_app_id, const char *child_app_id);

/**
 * @brief  Detaches the window of the application from its parent window.
 * @since_tizen 3.0
 * @remarks This function is only available for platform level signed applications.
 * @param[in]  app_id  The ID of the application
 * @return      @c 0 on success,
 *              otherwise a negative error value
 * @retval  #APP_MANAGER_ERROR_NONE               Successful
 * @retval  #APP_MANAGER_ERROR_INVALID_PARAMETER  Invalid parameter
 * @retval  #APP_MANAGER_ERROR_IO_ERROR           I/O error
 * @retval  #APP_MANAGER_ERROR_OUT_OF_MEMORY      Out of memory
 * @retval  #APP_MANAGER_ERROR_NO_SUCH_APP        No such application
 * @retval  #APP_MANAGER_ERROR_PERMISSION_DENIED  Permission denied
 */
int app_manager_detach_window(const char *app_id);

/**
 * @brief Unregisters the callback function.
 * @since_tizen 3.0
 * @param[in]   callback        The registered callback function
 * @param[in]   appid           The registered appid
 * @return      @c 0 on success,
 *              otherwise a negative error value
 * @retval      #APP_MANAGER_ERROR_NONE                 Successful
 * @retval      #APP_MANAGER_ERROR_INVALID_PARAMETER    Invalid parameter
 * @see app_manager_app_context_status_cb()
 */
int app_manager_unset_app_context_status_cb(app_manager_app_context_status_cb callback, const char *appid);

/**
 * @brief  Retrieves all application contexts of visible applications.
 * @since_tizen 3.0
 * @param[in]   callback   The callback function to invoke
 * @param[in]   user_data  The user data to be passed to the callback function
 * @return      @c 0 on success,
 *              otherwise a negative error value
 * @retval  #APP_MANAGER_ERROR_NONE               Successful
 * @retval  #APP_MANAGER_ERROR_INVALID_PARAMETER  Invalid parameter
 * @retval  #APP_MANAGER_ERROR_IO_ERROR           Internal I/O error
 * @retval  #APP_MANAGER_ERROR_OUT_OF_MEMORY      Out of memory
 * @post   This function invokes app_manager_app_context_cb() for each application context.
 * @see app_manager_app_context_cb()
 */
int app_manager_foreach_visible_app_context(app_manager_app_context_cb callback, void *user_data);

/**
 * @brief Adds a new group information to the app group.
 * @details A new app group will be added using the given window ID.
 * @since_tizen 6.0
 *
 * @parma[in]   win_id          The window ID
 * @return      @c 0 on success,
 *              otherwise a negative error value
 * @retval  #APP_MANAGER_ERROR_NONE               Successful
 * @retval  #APP_MANAGER_ERROR_INVALID_PARAMETER  Invalid parameter
 * @retval  #APP_MANAGER_ERROR_IO_ERROR           I/O error
 * @retval  #APP_MANAGER_ERROR_OUT_OF_MEMORY      Out of memroy
 */
int app_manager_add_app_group(int win_id);

/**
 * @brief Removes the app group using the given window ID.
 * @since_tizen 6.0
 *
 * @param[in]   win_id          The window ID
 * @return      @c 0 on success,
 *              otherwise a negative error value
 * @retval  #APP_MANAGER_ERROR_NONE               Successful
 * @retval  #APP_MANAGER_ERROR_INVALID_PARAMETER  Invalid parameter
 * @retval  #APP_MANAGER_ERROR_IO_ERROR           I/O error
 * @retval  #APP_MANAGER_ERROR_OUT_OF_MEMORY      Out of memroy
 */
int app_manager_remove_app_group(int win_id);

/**
 * @brief  Attaches the window of the child application below the window of the parent application.
 * @since_tizen 6.5
 * @remarks This function is only available for platform level signed applications.
 * @param[in]   parent_app_id The ID of the parent application
 * @param[in]   child_app_id  The ID of the child applicatio
 * @return      @c 0 on success,
 *              otherwise a negative error value
 * @retval  #APP_MANAGER_ERROR_NONE               Successful
 * @retval  #APP_MANAGER_ERROR_INVALID_PARAMETER  Invalid parameter
 * @retval  #APP_MANAGER_ERROR_IO_ERROR           I/O error
 * @retval  #APP_MANAGER_ERROR_OUT_OF_MEMORY      Out of memory
 * @retval  #APP_MANAGER_ERROR_NO_SUCH_APP        No such application
 * @retval  #APP_MANAGER_ERROR_PERMISSION_DENIED  Permission denied
 */
int app_manager_attach_window_below(const char *parent_appid, const char *child_appid);

/**
 * @brief  Terminates the application without restarting.
 * @since_tizen 7.0
 * @privlevel platform
 * @privilege %http://tizen.org/privilege/appmanager.kill
 * @param[in]   app_context  The application context
 * @return      @c 0 on success,
 *              otherwise a negative error value
 * @retval      #APP_MANAGER_ERROR_NONE                 Successful
 * @retval      #APP_MANAGER_ERROR_PERMISSION_DENIED    Permission denied
 * @retval      #APP_MANAGER_IO_ERROR                   I/O error
 * @retval      #APP_MANAGER_ERROR_INVALID_PARAMETER    Invalid parameter
 * @retval      #APP_MANAGER_ERROR_REQUEST_FAILED       Internal terminate error
 */
int app_manager_terminate_app_without_restarting(app_context_h app_context);

/**
 * @brief  Gets the absolute path to the common shared data directory of the application specified
 *         with an application ID.
 * @details     An application can only read the files of other application's common shared data directory.
 * @since_tizen 10.0
 * @remarks     The specified @a path should be released with free().\n
 *              The shared/data directory is supported only for the applications that declare the %http://tizen.org/privilege/appdir.shareddata privilege.
 *              The function will fail with #APP_MANAGER_ERROR_NOT_SUPPORTED if the privilege is not declared.
 *
 * @param[in]   app_id  The ID of the application
 * @param[out]  path    The absolute path to the common shared data directory of the application
 *                         specified with the @a app_id
 * @return  @c 0 on success,
 *          otherwise a negative error value
 * @retval  #APP_MANAGER_ERROR_NONE               Successful
 * @retval  #APP_MANAGER_ERROR_NOT_SUPPORTED      Not supported
 * @retval  #APP_MANAGER_ERROR_INVALID_PARAMETER  Invalid parameter
 * @retval  #APP_MANAGER_ERROR_NO_SUCH_APP        No such application
 * @retval  #APP_MANAGER_ERROR_OUT_OF_MEMORY      Out of memory
 */
int app_manager_get_common_shared_data_path(const char *app_id, char **path);

/**
 * @brief  Gets the absolute path to the common shared trusted directory of the application specified
 *         with an application ID.
 * @details     An application signed with the same certificate can read and write the files of
 *              other application's common shared trusted directory.
 * @since_tizen 10.0
 * @remarks     The specified @a path should be released with free().
 *
 * @param[in]      app_id  The ID of the application
 * @param[in,out]  path    The absolute path to the common shared trusted directory of the application
 *                         specified with the @a app_id
 * @return  @c 0 on success,
 *          otherwise a negative error value
 * @retval  #APP_MANAGER_ERROR_NONE               Successful
 * @retval  #APP_MANAGER_ERROR_INVALID_PARAMETER  Invalid parameter
 * @retval  #APP_MANAGER_ERROR_NO_SUCH_APP        No such application
 * @retval  #APP_MANAGER_ERROR_OUT_OF_MEMORY      Out of memory
 */
int app_manager_get_common_shared_trusted_path(const char *app_id, char **path);

/**
 * @brief Requests to remount the application path for the specified subsession to reflect user changes.
 * @details This function unmounts and remounts the application's root path for the specified subsession,
 *          updating the mount source to correspond to the current user context.
 *          The application path itself remains unchanged, but the underlying source is replaced to reflect
 *          the user switch. This is typically used to ensure data isolation and consistency when the user
 *          changes while the application is running.
 * @since_tizen 10.0
 * @privlevel platform
 * @privilege %http://tizen.org/privilege/internal/default/platform
 *
 * @param[in] subsession_id The subsession identifier (unique per app launch/session)
 * @return @c 0 on success,
 *         otherwise a negative error value
 * @retval #APP_MANAGER_ERROR_NONE Sucessful
 * @retval #APP_MANAGER_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #APP_MANAGER_ERROR_IO_ERROR I/O error
 * @retval #APP_MANAGER_ERROR_PERMISSION_DENIED Permission denied
 *
 * @remarks This function should be called after a user switch to ensure the app's private storage path is remounted
 *          with the correct user-specific source. It is intended for use by platform services or privileged modules.
 */
int app_manager_request_remount_subsession(const char *subsession_id);

/**
 * @brief Request to remount the gadget path for the caller's gadget manager to access.
 * @details This function unmounts and remounts the application's gadget path of the caller application.
 *          This is typically used to apply change about resource package that hold gadget.
 *
 * @since_tizen 10.0
 * @param[out] pkglist Concatenated string of available resource package ids for application, Delimited by ":"
 * @return @c 0 on success,
 *         otherwise a negative error value
 * @retval #APP_MANAGER_ERROR_NONE Sucessful
 * @retval #APP_MANAGER_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #APP_MANAGER_ERROR_OUT_OF_MEMORY Out of Memory
 * @retval #APP_MANAGER_ERROR_IO_ERROR I/O error
 * @retval #APP_MANAGER_ERROR_REQUEST_FAILED Internal Error
 *
 * @remarks This function should be called after every gadget executable unloaded from application.
 *          Calling this function to update gadget that currently loaded lead to undefined behavior.
 *          The specified @a pkglist should be released with free().
 */
int app_manager_request_remount_gadget_path(char **pkglist);

/**
 * @brief Enumeration for application lifecycle state.
 * @since_tizen 10.0
 */
typedef enum {
	APP_MANAGER_LIFECYCLE_STATE_INITIALIZED = 0,    /**< The application is initialized. */
	APP_MANAGER_LIFECYCLE_STATE_CREATED,            /**< The application is created. */
	APP_MANAGER_LIFECYCLE_STATE_RESUMED,            /**< The application is resumed. */
	APP_MANAGER_LIFECYCLE_STATE_PAUSED,             /**< The application is paused. */
	APP_MANAGER_LIFECYCLE_STATE_DESTROYED,          /**< The application is terminated. */
} app_manager_lifecycle_state_e;

/**
 * @brief Called when the lifecycle state of the application is changed.
 * @since_tizen 10.0
 * @param[in] app_id The application ID
 * @param[in] pid The process ID
 * @param[in] state The state of the application lifecycle
 * @param[in] has_focus If it's true, the application has focus
 * @param[in] user_data The user data passed from the registration function
 * @see app_manager_set_lifecycle_state_changed_cb()
 * @see app_manager_unset_lifecycle_state_changed_cb()
 */
typedef void (*app_manager_lifecycle_state_changed_cb)(const char *app_id, pid_t pid,
	app_manager_lifecycle_state_e state, bool has_focus, void *user_data);

/**
 * @brief Registers a callback function to be invoked when the lifecycle state of the application is changed.
 * @since_tizen 10.0
 * @param[in] callback The callback function
 * @param[in] user_data The user data to be passed to the callback function
 * @return @c 0 on success, otherwise a negative error value
 * @retval #APP_MANAGER_ERROR_NONE Successful
 * @retval #APP_MANAGER_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #APP_MANAGER_ERROR_OUT_OF_MEMORY Out of memory
 * @retval #APP_MANAGER_ERROR_IO_ERROR I/O error
 * @see app_manager_lifecycle_state_changed_cb()
 * @see app_manager_unset_lifecycle_state_changed_cb()
 */
int app_manager_set_lifecycle_state_changed_cb(app_manager_lifecycle_state_changed_cb callback, void *user_data);

/**
 * @brief Deregisters the lifecycle state changed callback function.
 * @since_tizen 10.0
 * @see app_manager_set_lifecycle_state_changed_cb()
 */
void app_manager_unset_lifecycle_state_changed_cb(void);

/**
 * @brief The lifecycle state changed notification handle.
 * @since_tizen 10.0
 * @see app_manager_add_lifecycle_state_changed_cb()
 * @see app_manager_remove_lifecycle_state_changed_cb()
 */
typedef void *app_lifecycle_event_h;

/**
 * @brief Adds a callback to be invoked when the lifecycle state of any
 *        application changes.
 * @details Multiple callbacks can be registered. Each registered callback
 *          receives a unique handle that can be used to remove it later.
 * @since_tizen 10.0
 * @remarks You must release @a handle using
 *          app_manager_remove_lifecycle_state_changed_cb() when it is
 *          no longer needed.
 * @param[in]  callback  The callback function
 * @param[in]  user_data The user data to be passed to the callback function
 * @param[out] handle    The lifecycle notification handle
 * @return @c 0 on success, otherwise a negative error value
 * @retval #APP_MANAGER_ERROR_NONE Successful
 * @retval #APP_MANAGER_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #APP_MANAGER_ERROR_OUT_OF_MEMORY Out of memory
 * @retval #APP_MANAGER_ERROR_IO_ERROR I/O error
 * @see app_manager_lifecycle_state_changed_cb()
 * @see app_manager_remove_lifecycle_state_changed_cb()
 */
int app_manager_add_lifecycle_state_changed_cb(
		app_manager_lifecycle_state_changed_cb callback, void *user_data,
		app_lifecycle_event_h *handle);

/**
 * @brief Removes a previously registered lifecycle state changed callback.
 * @since_tizen 10.0
 * @param[in] handle The lifecycle notification handle returned by
 *            app_manager_add_lifecycle_state_changed_cb()
 * @return @c 0 on success, otherwise a negative error value
 * @retval #APP_MANAGER_ERROR_NONE Successful
 * @retval #APP_MANAGER_ERROR_INVALID_PARAMETER Invalid parameter
 * @see app_manager_add_lifecycle_state_changed_cb()
 */
int app_manager_remove_lifecycle_state_changed_cb(
		app_lifecycle_event_h handle);

/**
 * @}
 */

#ifdef __cplusplus
}
#endif

#endif /* __TIZEN_APPFW_APP_MANAGER_EXTENSION_H */

