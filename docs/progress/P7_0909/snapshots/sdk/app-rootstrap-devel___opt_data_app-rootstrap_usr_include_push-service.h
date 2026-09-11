/*
 * Copyright (c) 2000 - 2012 Samsung Electronics Co., Ltd All Rights Reserved
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

#ifndef __PUSH_PUBLIC_LIB_H__
#define __PUSH_PUBLIC_LIB_H__

#include <tizen_error.h>
#include <app_control.h>

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @file push-service.h
 */

/**
 * @addtogroup CAPI_MESSAGING_PUSH_PUBLIC_MODULE
 * @{
 */

/**
 * @brief This is key string for getting extra data of App Control response.
 * @since_tizen 3.0
 * @remarks Your application could be launched in the background by push service. \n
 *			If you want to know what push operation makes your application wake up, use app_control_get_extra_data() with below key. \n
 *			In the case that your application is launched by push notification with "LAUNCH" option, you can see a value, "notification" in App Control callback. \n
 *			On the other hand, when the state of registration changed, you can find the other value, "registration_change".
 * @see app_control_get_extra_data()
 */
#define APP_CONTROL_DATA_PUSH_LAUNCH_TYPE "http://tizen.org/appcontrol/data/push/launch_type"

/**
 * @brief Enumerations of error codes for push API.
 * @since_tizen 2.3
 */
typedef enum {
	PUSH_SERVICE_ERROR_NONE = TIZEN_ERROR_NONE,   /**< Successful */
	PUSH_SERVICE_ERROR_OUT_OF_MEMORY = TIZEN_ERROR_OUT_OF_MEMORY,   /**< Out of memory */
	PUSH_SERVICE_ERROR_INVALID_PARAMETER = TIZEN_ERROR_INVALID_PARAMETER,   /**< Invalid parameter */
	PUSH_SERVICE_ERROR_NOT_CONNECTED = TIZEN_ERROR_CONNECTION,  /**< Not connected */
	PUSH_SERVICE_ERROR_NO_DATA = TIZEN_ERROR_NO_DATA,  /**< No data available */
	PUSH_SERVICE_ERROR_OPERATION_FAILED = TIZEN_ERROR_UNKNOWN, /**< Internal operation failed */
	PUSH_SERVICE_ERROR_PERMISSION_DENIED = TIZEN_ERROR_PERMISSION_DENIED, /**< No privilege to access the push service */
	PUSH_SERVICE_ERROR_NOT_SUPPORTED = TIZEN_ERROR_NOT_SUPPORTED, /**< Push service not supported */
} push_service_error_e;

/**
 * @brief Enumeration of registration states.
 * @since_tizen 2.3
 */
typedef enum {
	PUSH_SERVICE_STATE_REGISTERED,   /**< Registered */
	PUSH_SERVICE_STATE_UNREGISTERED, /**< Unregistered */
	PUSH_SERVICE_STATE_PROVISIONING_IPCHANGE, /**< To change the provisioning server IP */
	PUSH_SERVICE_STATE_PING_CHANGE,       /**< Ping interval is changing */
	PUSH_SERVICE_STATE_ERROR,       /**< Error */
} push_service_state_e;

/**
 * @brief Enumeration of result.
 * @since_tizen 2.3
 */
typedef enum {
	PUSH_SERVICE_RESULT_SUCCESS,  /**< Successful */
	PUSH_SERVICE_RESULT_TIMEOUT,  /**< Request timed out */
	PUSH_SERVICE_RESULT_SERVER_ERROR,  /**< Push server error */
	PUSH_SERVICE_RESULT_SYSTEM_ERROR,  /**< System error */
} push_service_result_e;

/**
 * @brief Connection to the push service handle.
 * @since_tizen 2.3
 */
typedef struct push_connection_s *push_service_connection_h;

/**
 * @brief Notification delivered from the push server handle.
 * @since_tizen 2.3
 */
typedef struct push_notification_s *push_service_notification_h;

/**
 * @brief Called when the registration state is refreshed.
 * @since_tizen 2.3
 * @remarks This callback will be invoked when the registration state is refreshed. \n
 *          If the registration or deregistration has succeeded, then this state callback must be called. \n
 *          In addition, the state can be changed if the push server deregisters the application.
 * @param[in] state The registration state
 * @param[in] err  The error message
 * @param[in] user_data The user data passed to this callback
 * @see push_service_connect()
 */
typedef void (*push_service_state_cb)(
		push_service_state_e state, const char *err, void *user_data);

/**
 * @brief Called to handle a notification.
 * @since_tizen 2.3
 * @param[in] noti A handle of the notification containing its payload\n
 *                         The handle is available inside this callback only.
 * @param[in] user_data The user data passed to this callback
 * @see push_service_connect()
 * @see push_service_get_notification_data()
 */
typedef void (*push_service_notify_cb)(
		push_service_notification_h noti, void *user_data);

/**
 * @brief Called with the result of a registration/deregistration.
 * @since_tizen 2.3
 * @param[in] result The registration/deregistration result
 * @param[in] msg The result message from the push server,
 *                otherwise @c NULL
 * @param[in] user_data The user data passed to this callback
 * @see push_service_register()
 * @see push_service_deregister()
 */
typedef void (*push_service_result_cb)(push_service_result_e result, const char *msg, void *user_data);

/**
 * @brief Connects to the push service and sets callback functions.
 * @since_tizen 2.3
 * @privlevel public
 * @privilege %http://tizen.org/privilege/push
 * @remarks If there is a connection between an application and the push service,\n
 *          the notify callback passes the notification upon its arrival.\n
 *          Otherwise, the push service posts a UI notification to alert users.\n
 *          The connection should be freed with push_service_disconnect() by you.
 * @param[in] push_app_id App id received from Tizen Push Server team
 * @param[in] state_callback State callback function
 * @param[in] notify_callback Notify callback function
 * @param[in] user_data User data to pass to <I>state_cb</I> and <I>notify_cb</I>
 * @param[out] connection The connection handle to the push service
 * @return @c 0 on success,
 *         otherwise a negative error value
 * @retval #PUSH_SERVICE_ERROR_NONE Successful
 * @retval #PUSH_SERVICE_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #PUSH_SERVICE_ERROR_OUT_OF_MEMORY Out of memory
 * @retval #PUSH_SERVICE_ERROR_NOT_CONNECTED Connection to the daemon failed
 * @retval #PUSH_SERVICE_ERROR_PERMISSION_DENIED No push privilege
 * @retval #PUSH_SERVICE_ERROR_NOT_SUPPORTED Not supported feature
 * @pre There is no connection to the push service for the <I>app_id</I>.
 * @post The state callback will be called to let you know the current
 *       registration state immediately.
 * @see push_service_disconnect()
 */
int push_service_connect(const char *push_app_id,
		push_service_state_cb state_callback,
		push_service_notify_cb notify_callback, void *user_data,
		push_service_connection_h *connection);

/**
 * @brief. Closes the connection and releases all its resources
 * @since_tizen 2.3
 * @remarks If you call this function in the push callback functions,\n
 *          it may cause your application to crash.\n
 *			The specific error code can be obtained using the get_last_result()\n
 *			Error codes are described in the exception section.
 * @param[in] connection The connection handle to the push service
 * @exception #PUSH_SERVICE_ERROR_NONE Successful
 * @exception #PUSH_SERVICE_ERROR_INVALID_PARAMETER Invalid parameter
 * @exception #PUSH_SERVICE_ERROR_NOT_SUPPORTED Not supported feature
 * @see push_service_connect()
 */
void push_service_disconnect(push_service_connection_h connection);

/**
 * @brief Registers an application to the push server.
 * @since_tizen 2.3
 * @param[in] connection The connection handle to the push service
 * @param[in] result_callback  Result callback function
 * @param[in] user_data  User data to pass to <I>result_cb</I>
 * @return @c 0 on success,
 *         otherwise a negative error value
 * @retval #PUSH_SERVICE_ERROR_NONE Successful
 * @retval #PUSH_SERVICE_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #PUSH_SERVICE_ERROR_OUT_OF_MEMORY Out of memory
 * @retval #PUSH_SERVICE_ERROR_NOT_CONNECTED No connection to the push service
 * @retval #PUSH_SERVICE_ERROR_OPERATION_FAILED Operation failed
 * @retval #PUSH_SERVICE_ERROR_NOT_SUPPORTED Not supported feature
 * @pre The application should be connected to the push service.
 * @post As a result, the state callback will be invoked.
 * @see push_service_deregister()
 */
int push_service_register(push_service_connection_h connection,
		push_service_result_cb result_callback, void *user_data);

/**
 * @brief Deregisters an application from the Push server.
 * @since_tizen 2.3
 * @param[in] connection The connection handle to the push service
 * @param[in] result_callback Result callback function
 * @param[in] user_data User data to pass to <I>result_cb</I>
 * @return @c 0 on success,
 *         otherwise a negative error value
 * @retval #PUSH_SERVICE_ERROR_NONE Successful
 * @retval #PUSH_SERVICE_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #PUSH_SERVICE_ERROR_OUT_OF_MEMORY Out of memory
 * @retval #PUSH_SERVICE_ERROR_NOT_CONNECTED No connection to the push service
 * @retval #PUSH_SERVICE_ERROR_OPERATION_FAILED Operation failed
 * @retval #PUSH_SERVICE_ERROR_NOT_SUPPORTED Not supported feature
 * @pre The application should be connected to the push service.
 * @post As a result, the state callback will be invoked.
 * @see push_service_register()
 */
int push_service_deregister(push_service_connection_h connection, push_service_result_cb result_callback,
		void *user_data);

/**
 * @brief Retrieves the payload data of a notification that forcibly launched the app.
 * @details When a notification arrives at the device with the "LAUNCH"\n
 *			option or a user clicks a notification in the quick panel,\n
 *          the push daemon forcibly launches the app and delivers the\n
 *          notification to the app as a bundle. This function returns\n
 *			the payload data in the notification.
 * @since_tizen 2.3
 * @privlevel public
 * @privilege %http://tizen.org/privilege/push
 * @remarks This function must be called in the app control callback function.\n
 *		   You must release the payload data using free().\n
 *		   push_service_app_control_to_notification() is preferred to this API.\n
 *		   The specific error code can be obtained using the get_last_result()\n
 *		   Error codes are described in the exception section.
 * @param[in] app_control The app control handle that is handed over in the\n
 *                        app control callback function
 * @param[in] operation The operation in the app control handle retrieved by\n
 *                        app_control_get_operation()
 * @return The payload data (appData) in the notification\n
 *         NULL if the app is not launched by a push notification.
 * @exception #PUSH_SERVICE_ERROR_NONE Successful
 * @exception #PUSH_SERVICE_ERROR_INVALID_PARAMETER Invalid parameter
 * @exception #PUSH_SERVICE_ERROR_NO_DATA	Not launched by a notification
 * @exception #PUSH_SERVICE_ERROR_OPERATION_FAILED Operation fail
 * @exception #PUSH_SERVICE_ERROR_OUT_OF_MEMORY Out of memory
 * @exception #PUSH_SERVICE_ERROR_NOT_CONNECTED Connection to the daemon failed
 * @exception #PUSH_SERVICE_ERROR_PERMISSION_DENIED No push privilege
 * @exception #PUSH_SERVICE_ERROR_NOT_SUPPORTED Not supported feature
 * @see app_control_get_operation()
 */
char *push_service_app_control_to_noti_data(app_control_h app_control, char *operation);

/**
 * @brief Retrieves the notification that forcibly launched the app.
 * @details When a notification arrives at the device with the "LAUNCH"\n
 *			option or a user clicks a notification in the quick panel,\n
 *          the push daemon forcibly launches the app and delivers the\n
 *          notification to the app as a bundle. This function returns\n
 *			the notification from the bundle.
 * @since_tizen 2.3
 * @privlevel public
 * @privilege %http://tizen.org/privilege/push
 * @remarks This function must be called in the app control callback function.\n
 *		   You must release the notification using push_service_free_notification().
 * @param[in] app_control The app control handle that is handed over in the\n
 *                        app control callback function
 * @param[in] operation The operation in the app control handle retrieved by\n
 *                        app_control_get_operation()
 * @param[out] noti	The handle of the notification that forcibly launched the app
 * @return @c 0 on success,
 *         otherwise a negative error value
 * @retval #PUSH_SERVICE_ERROR_NONE Successful
 * @retval #PUSH_SERVICE_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #PUSH_SERVICE_ERROR_NO_DATA	Not launched by a notification
 * @retval #PUSH_SERVICE_ERROR_OPERATION_FAILED Operation fail
 * @retval #PUSH_SERVICE_ERROR_OUT_OF_MEMORY Out of memory
 * @retval #PUSH_SERVICE_ERROR_NOT_CONNECTED Connection to the daemon failed
 * @retval #PUSH_SERVICE_ERROR_PERMISSION_DENIED No push privilege
 * @retval #PUSH_SERVICE_ERROR_NOT_SUPPORTED Not supported feature
 * @see push_service_free_notification()
 * @see app_control_get_operation()
 */
int push_service_app_control_to_notification(app_control_h app_control, char *operation, push_service_notification_h *noti);

/**
 * @brief Gets notification data sent by the server.
 * @since_tizen 2.3
 * @remarks You must release @a data using free().
 * @param[in] notification The notification handle
 * @param[out] data The notification data\n
 *                  Set NULL if error but <I>#PUSH_SERVICE_ERROR_INVALID_PARAMETER</I>
 * @return @c 0 on success,
 *         otherwise a negative error value
 * @retval #PUSH_SERVICE_ERROR_NONE Successful
 * @retval #PUSH_SERVICE_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #PUSH_SERVICE_ERROR_OUT_OF_MEMORY Out of memory
 * @retval #PUSH_SERVICE_ERROR_NO_DATA No data available
 * @retval #PUSH_SERVICE_ERROR_NOT_SUPPORTED Not supported feature
 * @see push_service_notify_cb()
 * @see push_service_request_unread_notification()
 */
int push_service_get_notification_data(push_service_notification_h notification, char **data);

/**
 * @brief Gets the notification message sent by the server.
 * @since_tizen 2.3
 * @remarks The <I>msg</I> must be released with free().\n
 *		   push_service_request_unread_notification() is\n
 *		   preferred to this API.
 * @param[in] notification The notification handle
 * @param[out] msg The notification message\n
 *                  Set NULL if error but <I>#PUSH_SERVICE_ERROR_INVALID_PARAMETER</I>
 * @return @c 0 on success,
 *         otherwise a negative error value
 * @retval #PUSH_SERVICE_ERROR_NONE Successful
 * @retval #PUSH_SERVICE_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #PUSH_SERVICE_ERROR_OUT_OF_MEMORY Out of memory
 * @retval #PUSH_SERVICE_ERROR_NO_DATA No data available
 * @retval #PUSH_SERVICE_ERROR_NOT_SUPPORTED Not supported feature
 * @see push_service_notify_cb()
 * @see push_service_get_unread_notification()
 */
int push_service_get_notification_message(push_service_notification_h notification,
		char **msg);

/**
 * @brief Gets the received time of the notification message.
 * @since_tizen 2.3
 * @param[in] notification The notification handle
 * @param[out] received_time The received time of the notification message \n
 *                           The @a received_time is based on UTC.
 * @return @c 0 on success,
 *         otherwise a negative error value
 * @retval #PUSH_SERVICE_ERROR_NONE Successful
 * @retval #PUSH_SERVICE_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #PUSH_SERVICE_ERROR_NO_DATA No data available
 * @retval #PUSH_SERVICE_ERROR_NOT_SUPPORTED Not supported feature
 * @see push_service_notify_cb()
 * @see push_service_request_unread_notification()
 */
int push_service_get_notification_time(push_service_notification_h notification, long long int *received_time);

/**
 * @brief Gets the sender of the notification.
 * @since_tizen 2.3
 * @remarks You must release @a sender using free().
 * @param[in] notification The notification handle
 * @param[out] sender The sender\n
 *                  Set @c NULL if error but #PUSH_SERVICE_ERROR_INVALID_PARAMETER
 * @return @c 0 on success,
 *         otherwise a negative error value
 * @retval #PUSH_SERVICE_ERROR_NONE Successful
 * @retval #PUSH_SERVICE_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #PUSH_SERVICE_ERROR_OUT_OF_MEMORY Out of memory
 * @retval #PUSH_SERVICE_ERROR_NO_DATA No data available
 * @retval #PUSH_SERVICE_ERROR_NOT_SUPPORTED Not supported feature
 * @see push_service_notify_cb()
 * @see push_service_get_unread_notification()
 */
int push_service_get_notification_sender(push_service_notification_h notification,
		char **sender);

/**
 * @brief Gets the session ID of the notification.
 * @since_tizen 2.3
 * @remarks You must release @a session_info using free().
 * @param[in] notification The notification handle
 * @param[out] session_info The session ID\n
 *                  Set @c NULL if error but #PUSH_SERVICE_ERROR_INVALID_PARAMETER
 * @return @c 0 on success,
 *         otherwise a negative error value
 * @retval #PUSH_SERVICE_ERROR_NONE Successful
 * @retval #PUSH_SERVICE_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #PUSH_SERVICE_ERROR_OUT_OF_MEMORY Out of memory
 * @retval #PUSH_SERVICE_ERROR_NO_DATA No data available
 * @retval #PUSH_SERVICE_ERROR_NOT_SUPPORTED Not supported feature
 * @see push_service_notify_cb()
 * @see push_service_request_unread_notification()
 */
int push_service_get_notification_session_info(push_service_notification_h notification,
		char **session_info);


/**
 * @brief Gets the request ID assigned by the sender.
 * @since_tizen 2.3
 * @remarks You must release @a request_id using free().
 * @param[in] notification The notification handle
 * @param[out] request_id The request ID\n
 *                  Set @c NULL if error but #PUSH_SERVICE_ERROR_INVALID_PARAMETER
 * @return @c 0 on success,
 *         otherwise a negative error value
 * @retval #PUSH_SERVICE_ERROR_NONE Successful
 * @retval #PUSH_SERVICE_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #PUSH_SERVICE_ERROR_OUT_OF_MEMORY Out of memory
 * @retval #PUSH_SERVICE_ERROR_NO_DATA No data available
 * @retval #PUSH_SERVICE_ERROR_NOT_SUPPORTED Not supported feature
 * @see push_service_notify_cb()
 * @see push_service_request_unread_notification()
 */
int push_service_get_notification_request_id(push_service_notification_h notification,
		char **request_id);

/**
 * @brief. Gets the value in the type field of the notification
 * @since_tizen 2.3
 * @param[in] notification The notification handle
 * @param[out] type The type value assigned by the sender
 * @return @c 0 on success,
 *         otherwise a negative error value
 * @retval #PUSH_SERVICE_ERROR_NONE Successful
 * @retval #PUSH_SERVICE_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #PUSH_SERVICE_ERROR_NO_DATA No data available
 * @retval #PUSH_SERVICE_ERROR_NOT_SUPPORTED Not supported feature
 * @see push_service_notify_cb()
 * @see push_service_request_unread_notification()
 */
int push_service_get_notification_type(push_service_notification_h notification, int *type);

/**
 * @brief Gets an unread notification message from the push server.
 * @details If an application receives an unread message with this method, the message is removed from the system. \n
 *          This method can be called repeatedly until it returns <I>#PUSH_SERVICE_ERROR_NO_DATA</I> \n
 *          But, this method does NOT guarantee order and reliability of notification messages. \n
 *          Some notification messages can be dropped when the system message queue is full.
 * @since_tizen 2.3
 * @remarks It is recommended to use push_service_request_unread_notification().
 * @remarks You must release @a noti using push_service_free_notification().
 * @param[in] connection The connection handle to the push service
 * @param[out] noti The notification handle
 * @return @c 0 on success,
 *         otherwise a negative error value
 * @retval #PUSH_SERVICE_ERROR_NONE Successful
 * @retval #PUSH_SERVICE_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #PUSH_SERVICE_ERROR_OUT_OF_MEMORY Out of memory
 * @retval #PUSH_SERVICE_ERROR_NO_DATA No data available
 * @retval #PUSH_SERVICE_ERROR_NOT_SUPPORTED Not supported feature
 *
 * @see push_service_get_notification_message()
 * @see push_service_get_notification_time()
 * @see push_service_get_notification_data()
 */
int push_service_get_unread_notification(push_service_connection_h connection,
		push_service_notification_h *noti);

/**
 * @brief Requests unread notification messages to the push server.
 * @details When the app wants to receive messages that arrived before it launched, this  \n
 *          method should be called. Upon receiving this request, the daemon sends messages\n
 *          stored in its DB to the app. The notify_callback() method assigned in push_service_connect()\n
 *          will be called when these messages arrive. No need to call this method multiple\n
 *          times to receive multiple messages. This method does NOT guarantee order and\n
 *          reliability of notification messages.
 * @since_tizen 2.3
 * @remarks This method is preferred to push_service_get_unread_notification().
 * @param[in] connection The connection handle to the push service
 * @return @c 0 on success,
 *         otherwise a negative error value
 * @retval #PUSH_SERVICE_ERROR_NONE Successful
 * @retval #PUSH_SERVICE_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #PUSH_SERVICE_ERROR_NOT_CONNECTED Not connected to the daemon
 * @retval #PUSH_SERVICE_ERROR_OPERATION_FAILED Error when sending the request
 * @retval #PUSH_SERVICE_ERROR_NOT_SUPPORTED Not supported feature
 *
 * @see push_service_get_unread_notification()
 * @see push_service_connect()
 */
int push_service_request_unread_notification(push_service_connection_h connection);

/**
 * @brief Gets the registration ID in the <I>#PUSH_SERVICE_STATE_REGISTERED</I> state.
 * @since_tizen 2.3
 * @remarks You must release @a reg_id using free().
 * @param[in] connection The connection handle to the push service
 * @param[out] reg_id The registration ID\n
 *                    Set NULL if error but <I>#PUSH_SERVICE_ERROR_INVALID_PARAMETER</I>
 * @return @c 0 on success,
 *         otherwise a negative error value
 * @retval #PUSH_SERVICE_ERROR_NONE Successful
 * @retval #PUSH_SERVICE_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #PUSH_SERVICE_ERROR_OUT_OF_MEMORY Out of memory
 * @retval #PUSH_SERVICE_ERROR_NO_DATA No registration ID available
 * @retval #PUSH_SERVICE_ERROR_NOT_SUPPORTED Not supported feature
 */
int push_service_get_registration_id(push_service_connection_h connection, char **reg_id);

/**
 * @brief Frees the notification handle.
 * @since_tizen 2.3
 * @remarks The specific error code can be obtained using the get_last_result()\n
 *		   Error codes are described in the exception section.
 * @param[in] noti The notification handle
 * @exception #PUSH_SERVICE_ERROR_NONE Successful
 * @exception #PUSH_SERVICE_ERROR_INVALID_PARAMETER Invalid parameter
 * @exception #PUSH_SERVICE_ERROR_NOT_SUPPORTED Not supported feature
 */
void push_service_free_notification(push_service_notification_h noti);

/**
 * @}
 */


#ifdef __cplusplus
}
#endif

#endif /* __PUSH_PUBLIC_LIB_H__ */
