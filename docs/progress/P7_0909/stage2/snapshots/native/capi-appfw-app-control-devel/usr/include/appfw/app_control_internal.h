/*
 * Copyright (c) 2014 - 2016 Samsung Electronics Co., Ltd All Rights Reserved
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

#ifndef __TIZEN_APPFW_APP_CONTROL_INTERNAL_H__
#define __TIZEN_APPFW_APP_CONTROL_INTERNAL_H__

#include <bundle.h>

#include <app_control.h>

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @file app_control_internal.h
 */

/**
 * @addtogroup CAPI_APP_CONTROL_MODULE
 * @{
 */

/**
 * @brief Definition for the app_control data: Connects the previous application with the next application when the sub-application is terminated.
 * @details If a sub-application is terminated, the framework will connect the previous application with the next application.
 *          By default, this flag is 'false'
 */
#define APP_CONTROL_DATA_REROUTE "__K_REROUTE__"

/**
 * @brief Definition for the app_control data: The flag for attaching the application.
 * @details If this flag is set, the callee application will be attached to the caller application for a while and it will be detached when the callee application is lowered.
 *          By default, this flag is 'false'
 */
#define APP_CONTROL_DATA_SHIFT_WINDOW "__K_SHIFT_WINDOW"

/**
 * @brief Definition for the app_control data: The flag for supporting recycling processes.
 * @details By default, this flag is 'false'. Once it is set to 'true', the launched sub-application process will be reused even if it was lowered later.
 *
 */
#define APP_CONTROL_DATA_RECYCLE "__K_RECYCLE"

/**
 * @brief Definition for the app_control data: The value for supporting the relocating launched application under the callee application.
 * @details By default, this value is NULL. Once it is set to application ID, that application will be relocated under the callee applictaion after the callee application is resumed.
 * @remarks The value should be an application ID for the main application which has been launched before, otherwise it will be ignored.
 * @remarks This definition is only available for platform level signed applications.
 */
#define APP_CONTROL_DATA_RELOCATE_BELOW "__K_RELOCATE_BELOW"

/**
 * @brief Definition for the app_control data: The value for supporting the background launch mode.
 * @details By default, this flag is 'disable'. Once it is set to 'enable', A window of the callee application doesn't appear on the screen.
 */
#define APP_CONTROL_DATA_BACKGROUND_LAUNCH "__K_BG_LAUNCH"

/**
 * @brief Definition for the app_control data: The value for checking RPC-Port request.
 * @details If the value exists, the application is launched by RPC-Port request.
 */
#define APP_CONTROL_DATA_RPC_PORT "__K_RPC_PORT"

/**
 * @brief Definition for the app_control data: The value for supporting positioning of the callee app group.
 * @details If the value exists, the callee app information will be inserted into the app group before the given position.
 * @since_tizen 6.0
 */
#define APP_CONTROL_DATA_INSERT_BEFORE_WINDOW "__K_INSERT_BEFORE_WINDOW"

/**
 * @brief Definition for the app_control data: The value for supporting positioning of the callee app group.
 * @details If the value exists, the callee app information will be inserted into the app group after the the caller position.
 * @since_tizen 6.5
 */
#define APP_CONTROL_DATA_INSERT_AFTER_CALLER "__K_INSERT_AFTER_CALLER"

/**
 * @brief Definition for the app_control data: The value for attaching the window below the caller.
 * @details If the value exists, the window of the callee will be attached below the window of the caller.
 * @since_tizen 6.5
 */
#define APP_CONTROL_DATA_ATTACH_BELOW "__K_ATTACH_BELOW"

/**
 * @brief Definition for the app_control data: The value for resetting the restart crash count.
 * @details If the value is "true", the restart crash count of the callee app is reset.
 * @since_tizen 8.0
 */
#define APP_CONTROL_RESET_RESTART_CRASH_COUNT "__K_RESET_RESTART_CRASH_COUNT"

/**
 * @brief Definition for the app_control data: The value for action name.
 * @details If the value exists, the application is launched by tizen action framework.
 * @remarks The value is a name of action.
 * @since_tizen 10.0
 */
#define APP_CONTROL_ACTION_NAME "__K_ACTION_NAME"

/**
 * @brief Definition for the app_control data: The value for action model.
 * @details The action model is a JSON-formatted data that defines the structure and behavior of the action, including the action name, parameters, return types, and other metadata.
 * @remarks The value is a string representing a JSON object and is automatically set by the action framework during action execution. Applications can use this value to verify the model information of the executed action.
 * @since_tizen 10.0
 */
#define APP_CONTROL_ACTION_MODEL "__K_ACTION_MODEL"

/**
 * @brief Definition for the app_control data: The value for action name.
 * @details The action result is a JSON-formatted data that includes the success status of the action execution, return values, error information, and other execution details.
 * @remarks The value is a string representing a JSON object and is used to process the action execution result.
 * @since_tizen 10.0
 */
#define APP_CONTROL_ACTION_RESULT "__K_ACTION_RESULT"

/**
 * @brief Replaces all data in the app_control with the bundle.
 *
 * @remarks This function clears all data in the app_control and adds all key-value pairs in the bundle into the app_control.
 * @param [in]  app_control     The app_control handle
 * @param [in]  data            The bundle handle
 * @return      @c 0 on success,
 *              otherwise a negative error value
 * @retval      #APP_CONTROL_ERROR_NONE                 Successful
 * @retval      #APP_CONTROL_ERROR_INVALID_PARAMETER    Invalid parameter
 * @see         app_control_export_as_bundle()
 * @code
 *
 * #include <bundle.h>
 * #include <app_control.h>
 *
 * app_control_h app_control = NULL;
 * app_control_create(&app_control);
 * app_control_import_from_bundle(app_control, b);
 *
 * @endcode
 *
 */
int app_control_import_from_bundle(app_control_h app_control, bundle *data);

/**
 * @brief Returns a new bundle containing all data contained int the app_control.
 *
 * @remarks The @a data must be released with bundle_free() by you.
 * @param [in]  app_control     The app_control handle
 * @param [out] data            The bundle handle
 * @return      @c 0 on success,
 *              otherwise a negative error value
 * @retval      #APP_CONTROL_ERROR_NONE                 Successful
 * @retval      #APP_CONTROL_ERROR_INVALID_PARAMETER    Invalid parameter
 * @see         app_control_import_from_bundle()
 * @code
 *
 * #include <bundle.h>
 * #include <app_control.h>
 *
 * bundle* b = NULL;
 * app_control_export_as_bundle(app_control, &b);
 *
 * @endcode
 */
int app_control_export_as_bundle(app_control_h app_control, bundle **data);

int app_control_create_request(bundle *data, app_control_h *app_control);

int app_control_create_event(bundle *data, app_control_h *app_control);

int app_control_to_bundle(app_control_h app_control, bundle **data);

/**
 * @brief Sets the window ID of the application.
 *
 * @since_tizen 2.3
 * @param[in]   app_control      The app_control handle
 * @param[in]   id               The window ID of the caller application (if the @a id is not positive, it clears the previous value)
 * @return      @c 0 on success,
 *              otherwise a negative error value
 * @retval      #APP_CONTROL_ERROR_NONE                 Successful
 * @retval      #APP_CONTROL_ERROR_INVALID_PARAMETER    Invalid parameter
 * @retval      #APP_CONTROL_ERROR_OUT_OF_MEMORY        Out of memory
 * @see         app_control_get_window()
 * @remarks This function is deprecated from tizen 6.5.
 */
int app_control_set_window(app_control_h app_control, unsigned int id);

/**
 * @brief Gets the window ID of the application.
 *
 * @since_tizen 2.3
 * @param[in]   app_control     The app_control handle
 * @param[out]  id              The window ID of the caller application
 * @return      @c 0 on success,
 *              otherwise a negative error value
 * @retval      #APP_CONTROL_ERROR_NONE                 Successful
 * @retval      #APP_CONTROL_ERROR_INVALID_PARAMETER    Invalid parameter
 * @retval      #APP_CONTROL_ERROR_OUT_OF_MEMORY        Out of memory
 * @see         app_control_set_app_id()
 * @remarks This function is deprecated from tizen 6.5.
 */
int app_control_get_window(app_control_h app_control, unsigned int *id);

typedef int (*app_control_host_res_fn)(void *data);

/**
 * @brief Requests the specified callee window to be transient for the caller window.
 *
 * @since_tizen 2.3
 * @remarks The @a callee_id window is transient for the top-level caller window and should be handled accordingly.
 * @param[in]   app_control     The app_control handle
 * @param[in]   callee_id       The callee window ID
 * @param[in]   cbfunc          The callback function to be called when the transient is requested
 * @param[in]   data            A data pointer to pass to the callback function
 * @return      @c 0 on success,
 *              otherwise a negative error value
 * @retval      #APP_CONTROL_ERROR_NONE                 Successful
 * @retval      #APP_CONTROL_ERROR_INVALID_PARAMETER    Invalid parameter
 * @remarks This function is deprecated from tizen 6.5.
 */
int app_control_request_transient_app(app_control_h app_control, unsigned int callee_id, app_control_host_res_fn cbfunc, void *data);

/**
 * @platform
 * @brief Sets the ID of the default application associated with an operation, MIME-type, and URI.
 *
 * @since_tizen 3.0
 * @privlevel platform
 * @privilege %http://tizen.org/privilege/systemsettings.admin
 * @param[in]   app_control     The app_control handle
 * @param[in]   app_id          The ID of the application
 *
 * @return      @c 0 on success,
 *              otherwise a negative error value
 * @retval      #APP_CONTROL_ERROR_NONE Successful
 * @retval      #APP_CONTROL_ERROR_PERMISSION_DENIED Permission denied
 * @retval      #APP_CONTROL_ERROR_IO_ERROR             IO error
 * @retval      #APP_CONTROL_ERROR_INVALID_PARAMETER    Invalid parameter
 */
int app_control_set_defapp(app_control_h app_control, const char *app_id);

/**
 * @platform
 * @brief Unsets the default application control setting of an application.
 *
 * @details When the user calls this API, all the default application settings for the app_id are unset.
 *
 * @since_tizen 3.0
 * @privlevel platform
 * @privilege %http://tizen.org/privilege/systemsettings.admin
 * @param[in]   app_id          The ID of the application
 *
 * @return      @c 0 on success,
 *              otherwise a negative error value
 * @retval      #APP_CONTROL_ERROR_NONE                 Successful
 * @retval      #APP_CONTROL_ERROR_PERMISSION_DENIED    Permission denied
 * @retval      #APP_CONTROL_ERROR_IO_ERROR             IO error
 * @retval      #APP_CONTROL_ERROR_INVALID_PARAMETER    Invalid parameter
 */
int app_control_unset_defapp(const char *app_id);

/**
 * @brief Sets the instance ID of the application.
 *
 * @since_tizen 3.0
 * @param[in]   app_control     The app_control handle
 * @param[in]   instance_id     The instance ID of the application
 * @return      @c 0 on success,
 *              otherwise a negative error value
 * @retval      #APP_CONTROL_ERROR_NONE                 Successful
 * @retval      #APP_CONTROL_ERROR_INVALID_PARAMETER    Invalid parameter
 * @retval      #APP_CONTROL_ERROR_OUT_OF_MEMORY        Out of memory
 */
int app_control_set_instance_id(app_control_h app_control, const char *instance_id);

/**
 * @brief Gets the instance ID of the application.
 *
 * @since_tizen 3.0
 * @param[in]   app_control     The app_control handle
 * @param[out]  instance_id     The instance ID of the application
 * @return      @c 0 on success,
 *              otherwise a negative error value
 * @retval      #APP_CONTROL_ERROR_NONE                 Successful
 * @retval      #APP_CONTROL_ERROR_INVALID_PARAMETER    Invalid parameter
 * @retval      #APP_CONTROL_ERROROUT_OF_MEMORY         Out of memory
 */
int app_control_get_instance_id(app_control_h app_control, char **instance_id);

/**
 * @brief Sets the instance ID of the caller.
 *
 * @since_tizen 5.5
 * @param[in]   app_control     The app_control handle
 * @param[in]   instance_id     The instance ID of the caller
 * @return      @c on success,
 *              otherwise a negative error value
 * @retval      #APP_CONTROL_ERROR_NONE                 Successful
 * @retval      #APP_CONTROL_ERROR_INVALID_PARAMETER    Invalid parameter
 * @retval      #APP_CONTROL_ERROR_OUT_OF_MEMORY        Out of memory
 */
int app_control_set_caller_instance_id(app_control_h app_control, const char *instance_id);

/**
 * @brief Sends the resumption request asynchronously.
 *
 * @details While the callee application is getting the resumption request, the app_resume_cb() is called.
 * @remarks The app_control_cb() of the callee application is not called if the application is not running.
 *          This function is for resuming the application.
 * @since_tizen 5.5
 * @privlevel public
 * @privilege %http://tizen.org/privilege/appmanager.launch
 * @param[in]   app_control     The app_control handle
 * @param[in]   callback        The callback function to be called when the result is delivered
 * @param[in]   user_data       The user data to be passed to the callback function
 * @return      @c 0 on success,
 *              otherwise a negative error value
 * @retval      #APP_CONTROL_ERROR_NONE                 Successful
 * @retval      #APP_CONTROL_ERROR_PERMISSION_DENIED    Permission denied
 * @retval      #APP_CONTROL_ERROR_INVALID_PARAMETER    Invalid parameter
 * @retval      #APP_CONTROL_ERROR_OUT_OF_MEMORY        Out of memory
 * @retval      #APP_CONTROL_ERROR_APP_NOT_FOUND        The application to run the given resume request is not found
 * @retval      #APP_CONTROL_ERROR_LAUNCH_REJECTED      The application cannot be resumed in current context
 * @retval      #APP_CONTROL_ERROR_LAUNCH_FAILED        Failed to resume the application
 * @see app_control_result_cb()
 */
int app_control_send_resume_request(app_control_h app_control, app_control_result_cb callback, void *user_data);

/**
 * @brief Sends the preparation request for the app-defined loader.
 *
 * @since_tizen 5.5
 * @param[in]   app_control     The app_control handle
 * @param[in]   loader_id       The app-defined loader ID
 * @return      @c 0 on success,
 *              otherwise a negative error value
 * @retval      #APP_CONTROL_ERROR_NONE                 Successful
 * @retval      #APP_CONTROL_ERROR_PERMISSION_DENIED    Permission denied
 * @retval      #APP_CONTROL_ERROR_INVALID_PARAMETER    Invalid parameter
 * @retval      #APP_CONTROL_ERROR_OUT_OF_MEMORY        Out of memory
 * @retval      #APP_CONTROL_ERROR_IO_ERROR             I/O error
 *
 * @remarks This function is only available for platform level signed applications.
 */
int app_control_prepare_app_defined_loader(app_control_h app_control, const char *loader_id);

/**
 * @brief Enables the auto restart setting.
 * @details The functionality of this function only applies to the caller application.
 *          The auto restart cannot be applied to other applications.
 *          The application ID set in the app_control handle is ignored.
 *
 * @since_tizen 7.0
 * @param[in]   app_control     The app_control handle
 * @return      @c 0 on success,
 *              otherwise a negative error value
 * @retval      #APP_CONTROL_ERROR_NONE                 Successful
 * @retval      #APP_CONTROL_ERROR_PERMISSION_DENIED    Permission denied
 * @retval      #APP_CONTROL_ERROR_INVALID_PARAMETER    Invalid parameter
 * @retval      #APP_CONTROL_ERROR_OUT_OF_MEMORY        Out of memory
 * @retval      #APP_CONTROL_ERROR_IO_ERROR             I/O error
 *
 * @remarks This function is only available for platform level signed applications.
 * @see app_control_unset_auto_restart()
 */
int app_control_set_auto_restart(app_control_h app_control);

/**
 * @brief Disables the auto restart setting.
 * @details The functionality of this function only applies to the caller application.
 *
 * @since_tizen 7.0
 * @return      @c 0 on success,
 *              otherwise a negative error value
 * @retval      #APP_CONTROL_ERROR_NONE                 Successful
 * @retval      #APP_CONTROL_ERROR_PERMISSION_DENIED    Permission denied
 * @retval      #APP_CONTROL_ERROR_INVALID_PARAMETER    Invalid parameter
 * @retval      #APP_CONTROL_ERROR_OUT_OF_MEMORY        Out of memory
 * @retval      #APP_CONTROL_ERROR_IO_ERROR             I/O error
 *
 * @remarks This function is only available for platform level signed applications.
 * @see app_control_set_auto_restart()
 */
int app_control_unset_auto_restart(void);

/**
 * @brief Sends the launch request with setting timeout
 *
 * @details This is the extension of @ref app_control_send_launch_request() API. \n
 *          The operation is mandatory information for the launch request. \n
 *          If the operation is not specified, #APP_CONTROL_OPERATION_DEFAULT is used by default.
 *          If the operation is #APP_CONTROL_OPERATION_DEFAULT, the application ID is mandatory to explicitly launch the application. \n
 *          It can set receiving timeout using @a timeout parameter.
 *          If there is an error that is not related to timeout, the error is returned immediately regardless of the @a timeout value.
 * @since_tizen 8.0
 * @privlevel public
 * @privilege %http://tizen.org/privilege/appmanager.launch
 * @remarks The function returns #APP_CONTROL_ERROR_LAUNCH_REJECTED if the operation value is #APP_CONTROL_OPERATION_LAUNCH_ON_EVENT which is only for handling the event from the platform or other application, refer to the @ref CAPI_EVENT_MODULE module.
 *
 * @param[in]   app_control     The app_control handle
 * @param[in]   timeout         The timeout in milliseconds, the timeout range is 5000 to 30000
 * @param[in]   callback        The callback function to be called when the reply is delivered
 * @param[in]   user_data       The user data to be passed to the callback function
 * @return @c 0 on success,
 *         otherwise a negative error value
 * @retval #APP_CONTROL_ERROR_NONE Successful
 * @retval #APP_CONTROL_ERROR_PERMISSION_DENIED Permission denied
 * @retval #APP_CONTROL_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #APP_CONTROL_ERROR_OUT_OF_MEMORY Out of memory
 * @retval #APP_CONTROL_ERROR_APP_NOT_FOUND The application to run the given launch request is not found
 * @retval #APP_CONTROL_ERROR_LAUNCH_REJECTED The application cannot be launched in current context
 * @retval #APP_CONTROL_ERROR_LAUNCH_FAILED Failed to launch the application
 * @retval #APP_CONTROL_ERROR_TIMED_OUT Failed due to timeout. The application that handles @a app_control may be busy. the timeout interval is set by @a timeout parameter.
 * @post If the launch request is sent for the result, the result will come back through the app_control_reply_cb() from the callee application. Additional replies may be delivered on the app_control_enable_app_started_result_event() called.
 * @see app_control_reply_to_launch_request()
 * @see app_control_reply_cb()
 * @see app_control_enable_app_started_result_event()
 */
int app_control_send_launch_request_with_timeout(app_control_h app_control, unsigned int timeout, app_control_reply_cb callback, void *user_data);

/**
 * @brief Sends the launch request with setting timeout asynchronously.
 *
 * @details This is the extension of @ref app_control_send_launch_request_async() API. \n
 *          The operation is mandatory information for the launch request. \n
 *          If the operation is not specified, #APP_CONTROL_OPERATION_DEFAULT is used by default.
 *          If the operation is #APP_CONTROL_OPERATION_DEFAULT, the application ID is mandatory to explicitly launch the application. \n
 *          It can set receiving timeout using @a timeout parameter.
 * @details After the callee application is initialized or the launch request is delivered to the running application successfully, the result callback function will be invoked.
 * @since_tizen 9.0
 * @privlevel public
 * @privilege %http://tizen.org/privilege/appmanager.launch
 * @remarks The function returns #APP_CONTROL_ERROR_LAUNCH_REJECTED if the operation value is #APP_CONTROL_OPERATION_LAUNCH_ON_EVENT which is only for handling the event from the platform or other application, refer to the @ref CAPI_EVENT_MODULE Module.
 * @remarks The launch request of the service application over out of packages is restricted by the platform. Also, implicit launch requests are NOT delivered to service applications since 2.4. To launch a service application, an explicit launch request with application ID given by the app_control_set_app_id() must be sent.
 *
 * @param[in]   app_control     The app_control handle
 * @param[in]   timeout         The timeout in milliseconds, the timeout range is 5000 to 30000
 * @param[in]   result_cb       The callback function to be called when the result is delivered
 * @param[in]   reply_cb        The callback function to be called when the reply is delivered
 * @param[in]   user_data       The user data to be passed to the callback function
 * @return @c 0 on success,
 *         otherwise a negative error value
 * @retval #APP_CONTROL_ERROR_NONE Successful
 * @retval #APP_CONTROL_ERROR_PERMISSION_DENIED Permission denied
 * @retval #APP_CONTROL_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #APP_CONTROL_ERROR_OUT_OF_MEMORY Out of memory
 * @retval #APP_CONTROL_ERROR_APP_NOT_FOUND The application to run the given launch request is not found
 * @retval #APP_CONTROL_ERROR_LAUNCH_REJECTED The application cannot be launched in current context
 * @retval #APP_CONTROL_ERROR_LAUNCH_FAILED Failed to launch the application
 * @retval #APP_CONTROL_ERROR_TIMED_OUT Failed due to timeout. The application that handles @a app_control may be busy. the timeout interval is set by @a timeout parameter.
 * @post If the launch request is sent for the result, the result will come back through the app_control_reply_cb() from the callee application. Additional replies may be delivered if app_control_enable_app_started_result_event() was called.
 * @see app_control_result_cb()
 * @see app_control_reply_to_launch_request()
 * @see app_control_reply_cb()
 * @see app_control_enable_app_started_result_event()
 */
int app_control_send_launch_request_async_with_timeout(app_control_h app_control, unsigned int timeout, app_control_result_cb result_cb, app_control_reply_cb reply_cb, void* user_data);

/**
 * @brief Called when the default application information is delivered.
 * @remarks The @a appid MUST NOT be released by the application.
 * @since_tizen 8.0
 *
 * @param[in]   appid           The default application ID
 * @param[in]   user_data       The user data passed from the foreach function
 * @return @c true to  continue with the next iteration of the loop,
 *         otherwise @c false to break out of the loop.
 * @see app_control_foreach_default_application()
 */
typedef bool (*app_control_default_application_cb)(const char *appid, void *user_data);

/**
 * @brief Retrieves all default applications.
 * @since_tizen 8.0
 *
 * @param[in]   callback        The iteration callback function
 * @param[in]   user_data       The user data to be passed to the callback function
 * @return @c 0 on success,
 *         otherwise a negative error value
 * @retval #APP_CONTROL_ERROR_NONE Successful
 * @retval #APP_CONTROL_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #APP_CONTROL_ERROR_IO_ERROR I/O error
 * @see app_control_default_application_cb()
 * @see app_control_set_defapp()
 * @see app_control_unset_defapp()
 */
int app_control_foreach_default_application(app_control_default_application_cb callback, void *user_data);


/**
 * @brief Sets the screen name of the application.
 *
 * @since_tizen 10.0
 *
 * @param[in]   app_control     The app_control handle
 * @param[in]   screen_name     The screen name of the application
 * @return @c 0 on success,
 *         otherwise a negative error value
 * @retval #APP_CONTROL_ERROR_NONE Successful
 * @retval #APP_CONTROL_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #APP_CONTROL_ERROR_OUT_OF_MEMORY Out of memory
 * @see app_control_get_screen_name()
 */
int app_control_set_screen_name(app_control_h app_control, const char *screen_name);


/**
 * @brief Gets the screen name of the application.
 *
 * @since_tizen 10.0
 * @remarks The @a screen name must be released with free().
 *
 * @param[in]   app_control     The app_control handle
 * @param[out]  screen_name     The screen name of the application
 * @return @c 0 on success,
 *         otherwise a negative error value
 * @retval #APP_CONTROL_ERROR_NONE Successful
 * @retval #APP_CONTROL_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #APP_CONTROL_ERROR_OUT_OF_MEMORY Out of memory
 * @see app_control_set_screen_name()
 */
int app_control_get_screen_name(app_control_h app_control, char **screen_name);


/**
 * @}
 */

#ifdef __cplusplus
}
#endif

#endif /* __TIZEN_APPFW_APP_CONTROL_INTERNAL_H__ */
