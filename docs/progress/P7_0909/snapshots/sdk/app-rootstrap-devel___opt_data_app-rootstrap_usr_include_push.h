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

#ifndef __PUSH_LIB_H__
#define __PUSH_LIB_H__

#include <app.h>
#include <tizen_error.h>

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @file push.h
 */

/**
 * @internal
 * @addtogroup CAPI_MESSAGING_PUSH_MODULE
 * @{
 */

/**
 * @brief Enumeration of error codes for push API.
 * @since_tizen 2.3
 */
typedef enum {
	PUSH_ERROR_NONE = TIZEN_ERROR_NONE,   /**< Successful */
	PUSH_ERROR_OUT_OF_MEMORY = TIZEN_ERROR_OUT_OF_MEMORY,   /**< Out of memory */
	PUSH_ERROR_INVALID_PARAMETER = TIZEN_ERROR_INVALID_PARAMETER,   /**< Invalid parameter */
	PUSH_ERROR_NOT_CONNECTED = TIZEN_ERROR_CONNECTION,  /**< Not connected */
	PUSH_ERROR_NO_DATA = TIZEN_ERROR_NO_DATA,  /**< No data available */
	PUSH_ERROR_OPERATION_FAILED = TIZEN_ERROR_UNKNOWN, /**< Internal operation failed */
	PUSH_ERROR_PERMISSION_DENIED = TIZEN_ERROR_PERMISSION_DENIED, /**< No privilege to access the push service */
	PUSH_ERROR_NOT_SUPPORTED = TIZEN_ERROR_NOT_SUPPORTED, /**< Push not supported */
} push_error_e;

/**
 * @brief Enumeration of registration states.
 * @since_tizen 2.3
 */
typedef enum {
	PUSH_STATE_REGISTERED,   /**< Registered */
	PUSH_STATE_UNREGISTERED, /**< Unregistered */
	PUSH_STATE_PROVISIONING_IPCHANGE, /**< To change the provisioning server IP */
	PUSH_STATE_PING_CHANGE,       /**< Ping interval is changing */
	PUSH_STATE_ERROR,       /**< Error */
} push_state_e;

/**
 * @brief Enumeration of result.
 * @since_tizen 2.3
 */
typedef enum {
	PUSH_RESULT_SUCCESS,  /**< Successful */
	PUSH_RESULT_TIMEOUT,  /**< Request timed out */
	PUSH_RESULT_SERVER_ERROR,  /**< Push server error */
	PUSH_RESULT_SYSTEM_ERROR,  /**< System error */
} push_result_e;

/**
 * @brief Connection to the push service handle.
 */
typedef struct push_connection_s *push_connection_h;

/**
 * @brief Notification delivered from the push server handle.
 * @since_tizen 2.3
 */
typedef struct push_notification_s *push_notification_h;

/**
 * @brief Called when the registration state is refreshed.
 * @since_tizen 2.3
 * @remarks This callback will be invoked when the registration state is refreshed. \n
 *          If the registration or deregistration has succeeded, then this state callback must be called. \n
 *          In addition, the state can be changed if the push server deregisters the application.
 * @param[in] state The registration state
 * @param[in] err  The error message
 * @param[in] user_data The user data passed to this callback
 * @see push_connect()
 */
typedef void (*push_state_cb)(
		push_state_e state, const char *err, void *user_data);

/**
 * @brief Called to handle a notification.
 * @since_tizen 2.3
 * @param[in] notification A handle of the notification containing its payload \n
 *                         The handle is available inside this callback only.
 * @param[in] user_data The user data passed to this callback
 * @see push_connect()
 * @see push_get_notification_data()
 */
typedef void (*push_notify_cb)(
		push_notification_h noti, void *user_data);

/**
 * @brief Called with the result of a registration/deregistration.
 * @since_tizen 2.3
 * @param[in] result The registration/deregistration result
 * @param[in] msg The result message from the push server,
 *                otherwise @c NULL
 * @param[in] user_data The user data passed to this callback
 * @see push_register()
 * @see push_deregister()
 */
typedef void (*push_result_cb)(push_result_e result, const char *msg, void *user_data);

/**
 * @brief Connects to the push service and sets callback functions.
 * @since_tizen 2.3
 * @privlevel public
 * @privilege %http://tizen.org/privilege/push
 * @remarks If there is a connection between an application and the push service,
 *          the notify callback passes the notification upon its arrival.\n
 *          Otherwise, the push service posts a UI notification to alert users.\n
 *          The connection should be freed using push_disconnect().
 * @param[in] app_id The application ID
 * @param[in] state_cb The state callback function
 * @param[in] notify_cb The notify callback function
 * @param[in] user_data The user data to pass to @a state_cb and @a notify_cb
 * @param[out] connection The connection handle to the push service
 * @return @c 0 on success,
 *         otherwise a negative error value
 * @retval #PUSH_ERROR_NONE Successful
 * @retval #PUSH_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #PUSH_ERROR_OUT_OF_MEMORY Out of memory
 * @retval #PUSH_ERROR_NOT_CONNECTED Connection to the daemon failed
 * @retval #PUSH_ERROR_PERMISSION_DENIED No push privilege
 * @pre There is no connection for the @a app_id to the push service.
 * @post The state callback will be called to let you know the current
 *       registration state immediately.
 * @see push_disconnect()
 */
int push_connect(const char *push_app_id, push_state_cb state_callback,
		push_notify_cb notify_callback, void *user_data,
		push_connection_h *connection);

/**
 * @brief Closes the connection and releases all its resources.
 * @since_tizen 2.3
 * @privlevel public
 * @remarks If you call this function in the push callback functions,
 *          it may cause your application to crash.
 * @param[in] connection A connection to the push service handle
 * @see push_connect()
 */
void push_disconnect(push_connection_h connection);

/**
 * @brief Registers an application to the push server.
 * @since_tizen 2.3
 * @privlevel public
 * @param[in] connection The connection handle to the push service
 * @param[in] app_control An @ref CAPI_APP_CONTROL_MODULE handle to launch an application by\n
 * an posted UI notification
 * @param[in] result_callback  Result callback function
 * @param[in] user_data  The user data to pass to @a result_cb
 * @return @c 0 on success,
 *         otherwise a negative error value
 * @retval #PUSH_ERROR_NONE Successful
 * @retval #PUSH_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #PUSH_ERROR_OUT_OF_MEMORY Out of memory
 * @retval #PUSH_ERROR_NOT_CONNECTED No connection to the push service
 * @retval #PUSH_ERROR_OPERATION_FAILED Operation failed
 * @pre The application should be connected to the push service.
 * @post For successful result, the state callback should be invoked.
 * @see push_deregister()
 */
int push_register(push_connection_h connection, app_control_h app_control,
		push_result_cb result_callback, void *user_data);

/**
 * @brief Deregisters an application from the Push server.
 * @since_tizen 2.3
 * @privlevel public
 * @param[in] connection The connection handle to the push service
 * @param[in] result_callback Result callback function
 * @param[in] user_data The user data to pass to @a result_cb
 * @return @c 0 on success,
 *         otherwise a negative error value
 * @retval #PUSH_ERROR_NONE Successful
 * @retval #PUSH_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #PUSH_ERROR_OUT_OF_MEMORY Out of memory
 * @retval #PUSH_ERROR_NOT_CONNECTED No connection to the push service
 * @retval #PUSH_ERROR_OPERATION_FAILED Operation failed
 * @pre The application should be connected to the push service.
 * @post As a result, the state callback will be invoked.
 * @see push_register()
 */
int push_deregister(push_connection_h connection, push_result_cb result_callback,
		void *user_data);

/**
 * @brief Registers an daemon dbus call information.
 * @since_tizen 2.4
 * @privlevel platform
 * @param[in] connection The connection handle to the push service
 * @param[in] result_callback  Result callback function
 * @param[in] dbus_bus_name  The string of dbus bus name to be called
 * @param[in] dbus_object_path  The string of dbus object path name to be called
 * @param[in] dbus_interface_name  The string of dbus interface name to be called
 * @param[in] dbus_method_name  The string of dbus method name to be called
 * @param[in] user_data  The user data to pass to @a result_cb
 * @return @c 0 on success,
 *         otherwise a negative error value
 * @retval #PUSH_ERROR_NONE Successful
 * @retval #PUSH_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #PUSH_ERROR_OUT_OF_MEMORY Out of memory
 * @retval #PUSH_ERROR_NOT_CONNECTED No connection to the push service
 * @retval #PUSH_ERROR_OPERATION_FAILED Operation failed
 * @pre The application should be registered to the push service.
 * @post For successful result, you must clear dbus call infomaion with push_deregister_dbus_call_info
 *         before deregister your daemon service.
 * @see push_deregister() and push_register_dbus_call_info
 */
int push_register_dbus_call_info(struct push_connection_s *conn, push_result_cb cb,
		const char *dbus_bus_name, const char *dbus_object_path, const char *dbus_interface_name,
		const char *dbus_method_name, void *user_data);

/**
 * @brief Deregisters an daemon dbus call information.
 * @since_tizen 2.4
 * @privlevel platform
 * @param[in] connection The connection handle to the push service
 * @param[in] result_callback  Result callback function
 * @param[in] user_data  The user data to pass to @a result_cb
 * @return @c 0 on success,
 *         otherwise a negative error value
 * @retval #PUSH_ERROR_NONE Successful
 * @retval #PUSH_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #PUSH_ERROR_OUT_OF_MEMORY Out of memory
 * @retval #PUSH_ERROR_NOT_CONNECTED No connection to the push service
 * @retval #PUSH_ERROR_OPERATION_FAILED Operation failed
 * @pre The application should be connected to the push service, and dbus call info was registerd.
 * @post For successful result, no more push dbus call activated.
 * @see push_register() and push_register_dbus_call_info
 */
int push_deregister_dbus_call_info(struct push_connection_s *conn, push_result_cb cb, void *user_data);

/**
 * @brief Gets the payload data in the notification.
 * @since_tizen 2.3
 * @privlevel public
 * @remarks You must release @a data using free().
 * @param[in] notification The notification handle
 * @param[out] data The notification data\n
 *                  Set @c NULL if error but #PUSH_ERROR_INVALID_PARAMETER
 * @return @c 0 on success,
 *         otherwise a negative error value
 * @retval #PUSH_ERROR_NONE Successful
 * @retval #PUSH_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #PUSH_ERROR_OUT_OF_MEMORY Out of memory
 * @retval #PUSH_ERROR_NO_DATA No data available
 * @see push_notify_cb()
 * @see push_request_unread_notification()
 */
int push_get_notification_data(push_notification_h notification, char **data);

/**
 * @brief Gets the message in the notification.
 * @since_tizen 2.3
 * @privlevel public
 * @remarks You must release @a msg using free().
 * @param[in] notification The notification handle
 * @param[out] msg The notification message\n
 *                  Set @c NULL if error but #PUSH_ERROR_INVALID_PARAMETER
 * @return @c 0 on success,
 *         otherwise a negative error value
 * @retval #PUSH_ERROR_NONE Successful
 * @retval #PUSH_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #PUSH_ERROR_OUT_OF_MEMORY Out of memory
 * @retval #PUSH_ERROR_NO_DATA No data available
 * @see push_notify_cb()
 * @see push_request_unread_notification()
 */
int push_get_notification_message(push_notification_h notification,
		char **msg);

/**
 * @brief Gets the received time of the notification.
 * @since_tizen 2.3
 * @privlevel public
 * @param[in] notification The notification handle
 * @param[out] timestamp The timestamp information that the application server
 *						optinally added to this notification when sending it.
 *						Typically, it is the number of milliseconds from a given
 *						standard time in the server.
 * @return @c 0 on success,
 *         otherwise a negative error value
 * @retval #PUSH_ERROR_NONE Successful
 * @retval #PUSH_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #PUSH_ERROR_NO_DATA No data available
 *
 * @see push_notify_cb()
 * @see push_request_unread_notification()
 */
int push_get_notification_time(push_notification_h notification, long long int *timestamp);

/**
 * @brief Gets the sender of the notification.
 * @since_tizen 2.3
 * @privlevel public
 * @remarks You must release @a sender using free().
 * @param[in] notification The notification handle
 * @param[out] sender The sender\n
 *                  Set @c NULL if error but #PUSH_ERROR_INVALID_PARAMETER
 * @return @c 0 on success,
 *         otherwise a negative error value
 * @retval #PUSH_ERROR_NONE Successful
 * @retval #PUSH_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #PUSH_ERROR_OUT_OF_MEMORY Out of memory
 * @retval #PUSH_ERROR_NO_DATA No data available
 * @see push_notify_cb()
 * @see push_request_unread_notification()
 */
int push_get_notification_sender(push_notification_h notification,
		char **sender);

/**
 * @brief Gets the session ID of the notification.
 * @since_tizen 2.3
 * @privlevel public
 * @remarks You must release @a session_info using free().
 * @param[in] notification The notification handle
 * @param[out] session_info The session ID\n
 *                  Set @c NULL if error but #PUSH_ERROR_INVALID_PARAMETER
 * @return @c 0 on success,
 *         otherwise a negative error value
 * @retval #PUSH_ERROR_NONE Successful
 * @retval #PUSH_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #PUSH_ERROR_OUT_OF_MEMORY Out of memory
 * @retval #PUSH_ERROR_NO_DATA No data available
 * @see push_notify_cb()
 * @see push_request_unread_notification()
 */
int push_get_notification_session_info(push_notification_h notification,
		char **session_info);


/**
 * @brief Gets the request ID assigned by the sender.
 * @since_tizen 2.3
 * @privlevel public
 * @remarks You must release @a request_id using free().
 * @param[in] notification The notification handle
 * @param[out] request_id The request ID\n
 *                  Set @c NULL if error but #PUSH_ERROR_INVALID_PARAMETER
 * @return @c 0 on success,
 *         otherwise a negative error value
 * @retval #PUSH_ERROR_NONE Successful
 * @retval #PUSH_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #PUSH_ERROR_OUT_OF_MEMORY Out of memory
 * @retval #PUSH_ERROR_NO_DATA No data available
 * @see push_notify_cb()
 * @see push_request_unread_notification()
 */
int push_get_notification_request_id(push_notification_h notification,
		char **request_id);

/**
 * @brief Gets the value in the type field of the notification
 * @since_tizen 2.3
 * @privlevel public
 * @param[in] notification The notification handle
 * @param[out] type The type value assigned by the sender
 * @return @c 0 on success,
 *         otherwise a negative error value
 * @retval #PUSH_ERROR_NONE Successful
 * @retval #PUSH_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #PUSH_ERROR_NO_DATA No data available
 * @see push_notify_cb()
 * @see push_request_unread_notification()
 */
int push_get_notification_type(push_notification_h notification, int *type);

/**
 * @brief Gets an unread notification message from the push server.
 *
 * @details If an application receives an unread message with this method, the message is removed from the system.\n
 *          This method can be called repeatedly until it returns #PUSH_ERROR_NO_DATA.\n
 *          However, this method does NOT guarantee order and reliability of notification messages.\n
 *          Some notification messages can be dropped when the system message queue is full.
 * @since_tizen 2.3
 * @privlevel public
 * @remarks This method will be deprecated.
 * @remarks You must release @a noti using push_free_notification().
 *			push_request_unread_notification() is preferred to this API.
 * @param[in] connection The connection handle to the push service
 * @param[out] noti The notification handle
 * @return @c 0 on success,
 *         otherwise a negative error value
 * @retval #PUSH_ERROR_NONE Successful
 * @retval #PUSH_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #PUSH_ERROR_OUT_OF_MEMORY Out of memory
 * @retval #PUSH_ERROR_NO_DATA No data available
 *
 * @see push_get_notification_message()
 * @see push_get_notification_time()
 * @see push_get_notification_data()
 */
int push_get_unread_notification(push_connection_h connection,
		push_notification_h *noti);

/**
 * @brief Requests unread notification messages to the push server.
 * @details When the app wants to receive messages that arrived before it launced, this  \n
 *          method should be called. Upon receiving this request, the daemon sends messages\n
 *          stored in its DB to the app. The notify_callback() method assigned in push_connect()\n
 *          will be called when these messages arrive. No need to call this method multiple\n
 *          times to receive multiple messages. This method does NOT guarantee order and\n
 *          reliability of notification messages.
 * @since_tizen 2.3
 * @privlevel public
 * @remark This method is preferred to push_get_unread_notification().
 * @param[in] connection The connection handle to the push service
 * @return @c 0 on success,
 *         otherwise a negative error value
 * @retval #PUSH_ERROR_NONE Successful
 * @retval #PUSH_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #PUSH_ERROR_NOT_CONNECTED Not connected to the daemon
 * @retval #PUSH_ERROR_OPERATION_FAILED Error when sending the request
 *
 * @see push_get_unread_notification()
 * @see push_connect()
 */
int push_request_unread_notification(push_connection_h connection);

/**
 * @brief Retrieves the notification with the notification token
 * @details When the push service forcibly launches the application to\n
			deliver a notification, a unique token for the notification\n
			is generated and delivered to the application as a bundle.\n
			Using this API with this token, the application can get\n
			the notification.
 * @since_tizen 2.3
 * @privlevel public
 * @privilege %http://tizen.org/privilege/push
 * @remark This function must be called in the app control callback function.\n
 *		   You must release the notification using push_free_notification().
 * @param[in] noti_token The notification token received from the bundle
 * @param[out] noti	The handle for the notification that launched this app
 * @return @c 0 on success,
 *         otherwise a negative error value
 * @retval #PUSH_SERVICE_ERROR_NONE Successful
 * @retval #PUSH_SERVICE_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #PUSH_SERVICE_ERROR_NO_DATA	No notification for this token
 * @retval #PUSH_SERVICE_ERROR_OPERATION_FAILED Operation fail
 * @retval #PUSH_SERVICE_ERROR_OUT_OF_MEMORY Out of memory
 * @retval #PUSH_SERVICE_ERROR_NOT_CONNECTED Connection to the daemon failed
 * @retval #PUSH_SERVICE_ERROR_PERMISSION_DENIED No push privilege
 * @see push_free_notification()
 * @see app_control_get_operation()
 */
int push_get_notification_using_token(const char *noti_token, push_notification_h *noti);


/**
 * @brief Gets the registration ID in the @a #PUSH_STATE_REGISTERED state.
 * @since_tizen 2.3
 * @privlevel public
 * @remarks You must release @a reg_id using free().
 * @param[in] connection The connection handle to the push service
 * @param[out] reg_id The registration ID
 *                    Set @c NULL if error but #PUSH_ERROR_INVALID_PARAMETER
 * @return @c 0 on success,
 *         otherwise a negative error value
 * @retval #PUSH_ERROR_NONE Successful
 * @retval #PUSH_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #PUSH_ERROR_OUT_OF_MEMORY Out of memory
 * @retval #PUSH_ERROR_NO_DATA No registration ID available
 */
int push_get_registration_id(push_connection_h connection, char **reg_id);


/**
 * @brief Frees the notification handle.
 * @since_tizen 2.3
 * @privlevel public
 * @privilege %http://tizen.org/privilege/push
 * @param[in] noti The notification handle
 */
void push_free_notification(push_notification_h noti);

/**
 * @}
 */

#ifdef __cplusplus
}
#endif

#endif /* __PUSH_LIB_H__ */
