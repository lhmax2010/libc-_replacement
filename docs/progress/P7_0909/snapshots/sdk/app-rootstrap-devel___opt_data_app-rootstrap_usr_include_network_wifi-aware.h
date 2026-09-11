/*
 * Copyright (c) 2020 Samsung Electronics Co., Ltd. All rights reserved.
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

#ifndef __TIZEN_NET_WIFI_AWARE_H__
#define __TIZEN_NET_WIFI_AWARE_H__

#include <tizen.h>

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @file wifi-aware.h
 */

/**
 * @addtogroup CAPI_NETWORK_WIFI_AWARE_MODULE
 * @{
 */

#ifndef TIZEN_ERROR_WIFI_AWARE
#define TIZEN_ERROR_WIFI_AWARE -0x03050000
#endif

/**
 * @brief Enumeration for Wi-Fi Aware error code.
 * @since_tizen 9.0
 */
typedef enum {
	/**
	 * Successful
	 */
	WIFI_AWARE_ERROR_NONE					= TIZEN_ERROR_NONE,
	/**
	 * Operation not permitted(1)
	 */
	WIFI_AWARE_ERROR_NOT_PERMITTED			= TIZEN_ERROR_NOT_PERMITTED,
	/**
	 * Out of memory(12)
	 */
	WIFI_AWARE_ERROR_OUT_OF_MEMORY			= TIZEN_ERROR_OUT_OF_MEMORY,
	/**
	 * Permission denied(13)
	 */
	WIFI_AWARE_ERROR_PERMISSION_DENIED		= TIZEN_ERROR_PERMISSION_DENIED,
	/**
	 * Device or resource busy(16)
	 */
	WIFI_AWARE_ERROR_RESOURCE_BUSY			= TIZEN_ERROR_RESOURCE_BUSY,
	/**
	 * Invalid function parameter(22)
	 */
	WIFI_AWARE_ERROR_INVALID_PARAMETER		= TIZEN_ERROR_INVALID_PARAMETER,
	/**
	 * Invalid operation(38)
	 */
	WIFI_AWARE_ERROR_INVALID_OPERATION = TIZEN_ERROR_INVALID_OPERATION,
	/**
	 * Connection timed out(110)
	 */
	// WIFI_AWARE_ERROR_CONNECTION_TIME_OUT   = TIZEN_ERROR_CONNECTION_TIME_OUT,
	/**
	 * Now in progress(115)
	 */
	WIFI_AWARE_ERROR_NOW_IN_PROGRESS		= TIZEN_ERROR_NOW_IN_PROGRESS,
	/**
	 * Not supported
	 */
	WIFI_AWARE_ERROR_NOT_SUPPORTED			= TIZEN_ERROR_NOT_SUPPORTED,
	/**
	 * Not initialized
	 */
	WIFI_AWARE_ERROR_NOT_INITIALIZED		= TIZEN_ERROR_WIFI_AWARE|0x01,
	/**
	 * Already initialized
	 */
	WIFI_AWARE_ERROR_ALREADY_INITIALIZED		= TIZEN_ERROR_WIFI_AWARE|0x02,
	/**
	 * Already enabled
	 */
	WIFI_AWARE_ERROR_ALREADY_ENABLED		= TIZEN_ERROR_WIFI_AWARE|0x03,
	/**
	 * Operation failed
	 */
	WIFI_AWARE_ERROR_OPERATION_FAILED		= TIZEN_ERROR_WIFI_AWARE|0x04,
	/**
	 * Rejected by peer
	 */
	WIFI_AWARE_ERROR_REJECTED_BY_PEER		= TIZEN_ERROR_WIFI_AWARE|0x05,
	/**
	 * Wi-Fi Interface is down
	 */
	WIFI_AWARE_ERROR_INTERFACE_DOWN			= TIZEN_ERROR_WIFI_AWARE|0x06,
} wifi_aware_error_e;

/**
 * @brief Enumeration for Wi-Fi Aware termination reason.
 * @since_tizen 9.0
 */
typedef enum {
	WIFI_AWARE_TERMINATION_REASON_UNKNOWN = 0, /**< Unknown reason */
} wifi_aware_termination_reason_e;

/**
 * @brief Enumeration for Wi-Fi Aware session type.
 * @since_tizen 9.0
 */
typedef enum {
	WIFI_AWARE_SESSION_PUBLISH = 0, /**< Publish type */
	WIFI_AWARE_SESSION_SUBSCRIBE, /**< Subscribe type */
} wifi_aware_session_type_e;

/**
 * @}
 */

/**
 * @addtogroup CAPI_NETWORK_WIFI_AWARE_PUBLISH_MODULE
 * @{
 */

/**
 * @brief Enumeration for Wi-Fi Aware publish type.
 * @since_tizen 9.0
 */
typedef enum {
	WIFI_AWARE_PUBLISH_TYPE_UNSOLICITED = 0, /**< Unsolicited type */
	WIFI_AWARE_PUBLISH_TYPE_SOLICITED, /**< Solicited type */
} wifi_aware_publish_type_e;

/**
 * @}
 */

/**
 * @addtogroup CAPI_NETWORK_WIFI_AWARE_SUBSCRIBE_MODULE
 * @{
 */

/**
 * @brief Enumeration for Wi-Fi Aware subscribe type.
 * @since_tizen 9.0
 */
typedef enum {
	WIFI_AWARE_SUBSCRIBE_TYPE_PASSIVE = 0,  /**< Passive type */
	WIFI_AWARE_SUBSCRIBE_TYPE_ACTIVE,   /**< Active type */
} wifi_aware_subscribe_type_e;

/**
 * @}
 */

/**
 * @addtogroup CAPI_NETWORK_WIFI_AWARE_MODULE
 * @{
 */

/**
 * @brief Wi-Fi Aware Session.
 * @since_tizen 9.0
 */
typedef void *wifi_aware_session_h;

/**
 * @brief Handle for Wi-Fi Aware Publish configuration.
 * @since_tizen 9.0
 */
typedef void *wifi_aware_publish_h;

/**
 * @brief Handle for Wi-Fi Aware Subscribe configuration.
 * @since_tizen 9.0
 */
typedef void *wifi_aware_subscribe_h;

/**
 * @brief Handle for Wi-Fi Aware peer.
 * @since_tizen 9.0
 */
typedef void *wifi_aware_peer_h;

/**
 * @brief Handle for Wi-Fi Aware Data Path.
 * @since_tizen 9.0
 */
typedef void *wifi_aware_data_path_h;


/**
 * @brief Initializes Wi-Fi Aware.
 * @since_tizen 9.0
 * @return 0 on success, otherwise a negative error value
 * @retval #WIFI_AWARE_ERROR_NONE                   Successful
 * @retval #WIFI_AWARE_ERROR_NOT_SUPPORTED          Not supported
 * @retval #WIFI_AWARE_ERROR_ALREADY_INITIALIZED    Already initialized
 * @retval #WIFI_AWARE_ERROR_OPERATION_FAILED       Operation failed
 * @retval #WIFI_AWARE_ERROR_OUT_OF_MEMORY          Out of memory
 * @see wifi_aware_deinitialize()
 */
int wifi_aware_initialize(void);

/**
 * @brief Deinitializes Wi-Fi Aware.
 * @since_tizen 9.0
 * @return 0 on success, otherwise a negative error value
 * @retval #WIFI_AWARE_ERROR_NONE                   Successful
 * @retval #WIFI_AWARE_ERROR_NOT_SUPPORTED          Not supported
 * @retval #WIFI_AWARE_ERROR_NOT_INITIALIZED        Not initialized
 * @see wifi_aware_initialize()
 */
int wifi_aware_deinitialize(void);

/**
 * @brief Called when Wi-Fi Aware is enabled.
 * @details The following error codes can be received: \n
 *              #WIFI_AWARE_ERROR_NONE                   Successful \n
 *              #WIFI_AWARE_ERROR_NOT_SUPPORTED          Not supported \n
 *              #WIFI_AWARE_ERROR_PERMISSION_DENIED      Permission denied \n
 *              #WIFI_AWARE_ERROR_INVALID_PARAMETER      Invalid function parameter \n
 *              #WIFI_AWARE_ERROR_INVALID_OPERATION      Invalid operation \n
 *              #WIFI_AWARE_ERROR_OPERATION_FAILED       Operation failed
 * @since_tizen 9.0
 * @param[in] error The error code
 * @param[in] user_data The user data passed from the request function
 * @see wifi_aware_enable()
 */
typedef void(*wifi_aware_enabled_cb)(wifi_aware_error_e error, void *user_data);

/**
 * @brief Enables Wi-Fi Aware functions.
 * @since_tizen 9.0
 * @privlevel public
 * @privilege %http://tizen.org/privilege/network.set
 * @remarks If the enable request is successfully done, the application can get the MAC address.
 * @param[in] callback The result of enable request is asynchronously delivered
 * @param[in] user_data The user data passed to the callback function
 * @return 0 on success, otherwise a negative error value
 * @retval #WIFI_AWARE_ERROR_NONE                   Successful
 * @retval #WIFI_AWARE_ERROR_NOT_SUPPORTED          Not supported
 * @retval #WIFI_AWARE_ERROR_PERMISSION_DENIED      Permission denied
 * @retval #WIFI_AWARE_ERROR_NOT_INITIALIZED        Not initialized
 * @retval #WIFI_AWARE_ERROR_ALREADY_ENABLED        Already enabled
 * @retval #WIFI_AWARE_ERROR_OPERATION_FAILED       Operation failed
 * @retval #WIFI_AWARE_ERROR_OUT_OF_MEMORY          Out of memory
 * @see wifi_aware_disable()
 */
int wifi_aware_enable(wifi_aware_enabled_cb callback, void *user_data);

/**
 * @brief Called when Wi-Fi Aware is disabled.
 * @details The following error codes can be received: \n
 * 		#WIFI_AWARE_ERROR_NONE			Successful \n
 *		#WIFI_AWARE_ERROR_NOT_SUPPORTED		Not supported \n
 *		#WIFI_AWARE_ERROR_PERMISSION_DENIED	Permission denied \n
 *		#WIFI_AWARE_ERROR_INVALID_PARAMETER	Invalid function parameter \n
 *		#WIFI_AWARE_ERROR_INVALID_OPERATION	Invalid operation \n
 *		#WIFI_AWARE_ERROR_OPERATION_FAILED	Operation failed
 * @since_tizen 9.0
 * @param[in] error The error code
 * @param[in] user_data The user data passed from the request function
 * @see wifi_aware_disable()
 */
typedef void(*wifi_aware_disabled_cb)(wifi_aware_error_e error, void *user_data);

/**
 * @brief Disables Wi-Fi Aware functions.
 * @since_tizen 9.0
 * @privlevel public
 * @privilege %http://tizen.org/privilege/network.set
 * @param[in] callback The result of disable request is asynchronously delivered
 * @param[in] user_data The user data passed to the callback function
 * @return 0 on success, otherwise a negative error value
 * @retval #WIFI_AWARE_ERROR_NONE                   Successful
 * @retval #WIFI_AWARE_ERROR_NOT_SUPPORTED          Not supported
 * @retval #WIFI_AWARE_ERROR_PERMISSION_DENIED      Permission denied
 * @retval #WIFI_AWARE_ERROR_NOT_INITIALIZED        Not initialized
 * @retval #WIFI_AWARE_ERROR_INVALID_OPERATION      Invalid operation
 * @retval #WIFI_AWARE_ERROR_OPERATION_FAILED       Operation failed
 * @see wifi_aware_enable()
 */
int wifi_aware_disable(wifi_aware_disabled_cb callback, void *user_data);

/**
 * @brief Creates the Wi-Fi Aware Session.
 * @since_tizen 9.0
 * @remarks The @a session should be released using wifi_aware_session_destroy().
 * @param[in] session_type The type of session
 * @param[out] session The Wi-Fi Aware Session handle that is newly created on success
 * @return 0 on success, otherwise a negative error value
 * @retval #WIFI_AWARE_ERROR_NONE                   Successful
 * @retval #WIFI_AWARE_ERROR_NOT_SUPPORTED          Not supported
 * @retval #WIFI_AWARE_ERROR_INVALID_PARAMETER      Invalid function parameter
 * @retval #WIFI_AWARE_ERROR_NOT_INITIALIZED        Not initialized
 * @retval #WIFI_AWARE_ERROR_OUT_OF_MEMORY          Out of memory
 *
 * @par Here is an example of the usage:
 * @code
 *
 * 	int ret = 0;
 * 	wifi_aware_session_h wifi_aware_session = NULL;
 *
 * 	ret = wifi_aware_session_create(WIFI_AWARE_SESSION_PUBLISH, &wifi_aware_session);
 * 	if (ret != WIFI_AWARE_ERROR_NONE) {
 * 		printf("Failed to create session\n");
 * 		return;
 * 	}
 *
 * 	// Do stuff here for created session
 *
 * 	wifi_aware_session_destroy(wifi_aware_session);
 *
 * @endcode
 * @see wifi_aware_session_destroy()
 */
int wifi_aware_session_create(wifi_aware_session_type_e session_type, wifi_aware_session_h *session);

/**
 * @brief Destroys the Wi-Fi Aware Session.
 * @since_tizen 9.0
 * @param[in] session The Wi-Fi Aware Session handle
 * @return 0 on success, otherwise a negative error value
 * @retval #WIFI_AWARE_ERROR_NONE                   Successful
 * @retval #WIFI_AWARE_ERROR_NOT_SUPPORTED          Not supported
 * @retval #WIFI_AWARE_ERROR_INVALID_PARAMETER      Invalid function parameter
 * @retval #WIFI_AWARE_ERROR_NOT_INITIALIZED        Not initialized
 * @see wifi_aware_session_create()
 */
int wifi_aware_session_destroy(wifi_aware_session_h session);

/**
 * @brief Called when the Wi-Fi Aware Session has been lost.
 * @since_tizen 9.0
 * @remarks The @a session should not be released.
 * @param[in] session   The Wi-Fi Aware Session handle
 * @param[in] reason    The reason of termination
 * @param[in] user_data The user data passed from request function
 * @see wifi_aware_session_set_terminated_cb()
 * @see wifi_aware_session_unset_terminated_cb()
 */
typedef void(*wifi_aware_session_terminated_cb)(wifi_aware_session_h session,
		wifi_aware_termination_reason_e reason, void *user_data);

/**
 * @brief Sets a callback function to notify session is terminated.
 * @since_tizen 9.0
 * @param[in] session   The Wi-Fi Aware Session
 * @param[in] callback  The callback function to register
 * @param[in] user_data The user data to be passed to the callback function
 * @return 0 on success, otherwise a negative error value
 * @retval #WIFI_AWARE_ERROR_NONE                   Successful
 * @retval #WIFI_AWARE_ERROR_NOT_SUPPORTED          Not supported
 * @retval #WIFI_AWARE_ERROR_INVALID_PARAMETER      Invalid function parameter
 * @retval #WIFI_AWARE_ERROR_NOT_INITIALIZED        Not initialized
 * @see wifi_aware_session_unset_terminated_cb()
 */
int wifi_aware_session_set_terminated_cb(wifi_aware_session_h session,
		wifi_aware_session_terminated_cb callback, void *user_data);

/**
 * @brief Unsets the session terminated callback function.
 * @since_tizen 9.0
 * @param[in] session The Wi-Fi Aware Session
 * @return 0 on success, otherwise a negative error value
 * @retval #WIFI_AWARE_ERROR_NONE                   Successful
 * @retval #WIFI_AWARE_ERROR_NOT_SUPPORTED          Not supported
 * @retval #WIFI_AWARE_ERROR_INVALID_PARAMETER      Invalid function parameter
 * @retval #WIFI_AWARE_ERROR_NOT_INITIALIZED        Not initialized
 * @see wifi_aware_session_set_terminated_cb()
 */
int wifi_aware_session_unset_terminated_cb(wifi_aware_session_h session);

/**
 * @brief Stops a Wi-Fi Aware session.
 * @since_tizen 9.0
 * @privlevel public
 * @privilege %http://tizen.org/privilege/network.set
 * @remarks The function of the session, Publish or Subscribe, will be cancelled.
 * @param[in] session The Wi-Fi Aware Session handle
 * @return 0 on success, otherwise a negative error value
 * @retval #WIFI_AWARE_ERROR_NONE                   Successful
 * @retval #WIFI_AWARE_ERROR_NOT_SUPPORTED          Not supported
 * @retval #WIFI_AWARE_ERROR_PERMISSION_DENIED      Permission denied
 * @retval #WIFI_AWARE_ERROR_INVALID_PARAMETER      Invalid function parameter
 * @retval #WIFI_AWARE_ERROR_NOT_INITIALIZED        Not initialized
 * @retval #WIFI_AWARE_ERROR_INVALID_OPERATION      Invalid operation
 * @retval #WIFI_AWARE_ERROR_OPERATION_FAILED       Operation failed
 * @see wifi_aware_session_publish()
 * @see wifi_aware_session_subscribe()
 */
int wifi_aware_session_stop(wifi_aware_session_h session);

/**
 * @brief Called when publish request is finished.
 * @details The following error codes can be received: \n
 *              #WIFI_AWARE_ERROR_NONE                   Successful \n
 *              #WIFI_AWARE_ERROR_NOT_SUPPORTED          Not supported \n
 *              #WIFI_AWARE_ERROR_PERMISSION_DENIED      Permission denied \n
 *              #WIFI_AWARE_ERROR_INVALID_PARAMETER      Invalid function parameter \n
 *              #WIFI_AWARE_ERROR_INVALID_OPERATION      Invalid operation \n
 *              #WIFI_AWARE_ERROR_OPERATION_FAILED       Operation failed
 * @since_tizen 9.0
 * @remarks The @a session should not be released.
 * @param[in] session   The Wi-Fi Aware Session handle
 * @param[in] error     The error code
 * @param[in] user_data The user data passed from request function
 * @see wifi_aware_session_publish()
 * @see wifi_aware_session_update_publish()
 */
typedef void(*wifi_aware_published_cb)(wifi_aware_session_h session,
		wifi_aware_error_e error, void *user_data);

/**
 * @brief Publishes a Wi-Fi Aware service so that neighboring devices can find it.
 * @since_tizen 9.0
 * @privlevel public
 * @privilege %http://tizen.org/privilege/network.set
 * @param[in] session  The Wi-Fi Aware Session handle
 * @param[in] publish   The Wi-Fi Aware Publish handle
 * @param[in] callback  The callback which gets called when a service is published
 * @param[in] user_data The user data passed to the callback function
 * @return 0 on success, otherwise a negative error value
 * @retval #WIFI_AWARE_ERROR_NONE                   Successful
 * @retval #WIFI_AWARE_ERROR_NOT_SUPPORTED          Not supported
 * @retval #WIFI_AWARE_ERROR_PERMISSION_DENIED      Permission denied
 * @retval #WIFI_AWARE_ERROR_INVALID_PARAMETER      Invalid function parameter
 * @retval #WIFI_AWARE_ERROR_NOT_INITIALIZED        Not initialized
 * @retval #WIFI_AWARE_ERROR_INVALID_OPERATION      Invalid operation
 * @retval #WIFI_AWARE_ERROR_OPERATION_FAILED       Operation failed
 * @see wifi_aware_session_stop()
 */
int wifi_aware_session_publish(wifi_aware_session_h session,
		wifi_aware_publish_h publish,
		wifi_aware_published_cb callback,
		void *user_data);

/**
 * @brief Updates the configuration for a Wi-Fi Aware service that has already been published.
 * @since_tizen 9.0
 * @privlevel public
 * @privilege %http://tizen.org/privilege/network.set
 * @param[in] session  The Wi-Fi Aware Session handle
 * @param[in] publish   The Wi-Fi Aware Publish handle
 * @param[in] callback  The callback which gets called when a service is updated
 * @param[in] user_data The user data passed to the callback function
 * @return 0 on success, otherwise a negative error value
 * @retval #WIFI_AWARE_ERROR_NONE                   Successful
 * @retval #WIFI_AWARE_ERROR_NOT_SUPPORTED          Not supported
 * @retval #WIFI_AWARE_ERROR_PERMISSION_DENIED      Permission denied
 * @retval #WIFI_AWARE_ERROR_INVALID_PARAMETER      Invalid function parameter
 * @retval #WIFI_AWARE_ERROR_NOT_INITIALIZED        Not initialized
 * @retval #WIFI_AWARE_ERROR_INVALID_OPERATION      Invalid operation
 * @retval #WIFI_AWARE_ERROR_OPERATION_FAILED       Operation failed
 * @see wifi_aware_session_publish()
 */
int wifi_aware_session_update_publish(wifi_aware_session_h session,
		wifi_aware_publish_h publish,
		wifi_aware_published_cb callback, void *user_data);

/**
 * @brief Called when Subscribe request is finished.
 * @details The following error codes can be received: \n
 *              #WIFI_AWARE_ERROR_NONE                   Successful \n
 *              #WIFI_AWARE_ERROR_NOT_SUPPORTED          Not supported \n
 *              #WIFI_AWARE_ERROR_PERMISSION_DENIED      Permission denied \n
 *              #WIFI_AWARE_ERROR_INVALID_PARAMETER      Invalid function parameter \n
 *              #WIFI_AWARE_ERROR_INVALID_OPERATION      Invalid operation \n
 *              #WIFI_AWARE_ERROR_OPERATION_FAILED       Operation failed
 * @since_tizen 9.0
 * @remarks The @a session should not be released.
 * @param[in] session   The Wi-Fi Aware Session handle
 * @param[in] error     The error code
 * @param[in] user_data The user data passed from request function
 * @see wifi_aware_session_subscribe()
 */
typedef void(*wifi_aware_subscribed_cb)(wifi_aware_session_h session,
		wifi_aware_error_e error, void *user_data);

/**
 * @brief Starts to find Wi-Fi Aware services.
 * @since_tizen 9.0
 * @privlevel public
 * @privilege %http://tizen.org/privilege/network.set
 * @param[in] session   The Wi-Fi Aware Session handle
 * @param[in] subscribe The Wi-Fi Aware Subscribe handle
 * @param[in] callback  The callback which get called when request is finished
 * @param[in] user_data The user data passed to the callback function
 * @return 0 on success, otherwise a negative error value
 * @retval #WIFI_AWARE_ERROR_NONE                   Successful
 * @retval #WIFI_AWARE_ERROR_NOT_SUPPORTED          Not supported
 * @retval #WIFI_AWARE_ERROR_PERMISSION_DENIED      Permission denied
 * @retval #WIFI_AWARE_ERROR_INVALID_PARAMETER      Invalid function parameter
 * @retval #WIFI_AWARE_ERROR_NOT_INITIALIZED        Not initialized
 * @retval #WIFI_AWARE_ERROR_INVALID_OPERATION      Invalid operation
 * @retval #WIFI_AWARE_ERROR_OPERATION_FAILED       Operation failed
 * @see wifi_aware_session_stop()
 */
int wifi_aware_session_subscribe(wifi_aware_session_h session,
		wifi_aware_subscribe_h subscribe,
		wifi_aware_subscribed_cb callback, void *user_data);

/**
 * @brief Updates the configuration for a Wi-Fi Aware service that has already been subscribed.
 * @since_tizen 9.0
 * @privlevel public
 * @privilege %http://tizen.org/privilege/network.set
 * @param[in] session   The Wi-Fi Aware Session handle
 * @param[in] subscribe The Wi-Fi Aware Subscribe handle
 * @param[in] callback  The callback which gets called when service is updated
 * @param[in] user_data The user data passed to the callback function
 * @return 0 on success, otherwise a negative error value
 * @retval #WIFI_AWARE_ERROR_NONE                   Successful
 * @retval #WIFI_AWARE_ERROR_NOT_SUPPORTED          Not supported
 * @retval #WIFI_AWARE_ERROR_PERMISSION_DENIED      Permission denied
 * @retval #WIFI_AWARE_ERROR_INVALID_PARAMETER      Invalid function parameter
 * @retval #WIFI_AWARE_ERROR_NOT_INITIALIZED        Not initialized
 * @retval #WIFI_AWARE_ERROR_INVALID_OPERATION      Invalid operation
 * @retval #WIFI_AWARE_ERROR_OPERATION_FAILED       Operation failed
 * @see wifi_aware_session_subscribe()
 */
int wifi_aware_session_update_subscribe(wifi_aware_session_h session,
		wifi_aware_subscribe_h subscribe,
		wifi_aware_subscribed_cb callback, void *user_data);

/**
 * @brief Called whenever a matched service for the subscribe configurations is found.
 * @since_tizen 9.0
 * @remarks If the publisher doesn't enabled ranging or the subscriber doesn't set both max and min distance,\n
 *         @a distance would be negative value.\n
 *         The @a session and @a peer should not be released.
 * @param[in] session   The Wi-Fi Aware Session handle
 * @param[in] peer      The found Peer information
 * @param[in] service_specific_info     Sequence of octets which were received from published service
 * @param[in] service_specific_info_len The length of @a service_specific_info
 * @param[in] distance The distance to the peer
 * @param[in] user_data The user data passed to the callback function
 * @see wifi_aware_session_set_service_discovered_cb()
 * @see wifi_aware_session_unset_service_discovered_cb()
 */
typedef void(*wifi_aware_service_discovered_cb)(wifi_aware_session_h session,
		wifi_aware_peer_h peer,
		const unsigned char *service_specific_info, size_t service_specific_info_len,
		int distance,
		void *user_data);

/**
 * @brief Sets a callback function to notify a service is discovered.
 * @since_tizen 9.0
 * @param[in] session   The Wi-Fi Aware Session handle
 * @param[in] callback  The callback function to register
 * @param[in] user_data The user data to be passed to the callback function
 * @return 0 on success, otherwise a negative error value
 * @retval #WIFI_AWARE_ERROR_NONE                   Successful
 * @retval #WIFI_AWARE_ERROR_NOT_SUPPORTED          Not supported
 * @retval #WIFI_AWARE_ERROR_INVALID_PARAMETER      Invalid function parameter
 * @retval #WIFI_AWARE_ERROR_NOT_INITIALIZED        Not initialized
 * @see wifi_aware_session_unset_service_discovered_cb()
 */
int wifi_aware_session_set_service_discovered_cb(wifi_aware_session_h session,
		wifi_aware_service_discovered_cb callback, void *user_data);

/**
 * @brief Unsets the service discovered callback function.
 * @since_tizen 9.0
 * @param[in] session The Wi-Fi Aware Session handle
 * @return 0 on success, otherwise a negative error value
 * @retval #WIFI_AWARE_ERROR_NONE                   Successful
 * @retval #WIFI_AWARE_ERROR_NOT_SUPPORTED          Not supported
 * @retval #WIFI_AWARE_ERROR_INVALID_PARAMETER      Invalid function parameter
 * @retval #WIFI_AWARE_ERROR_NOT_INITIALIZED        Not initialized
 * @see wifi_aware_session_set_service_discovered_cb()
 */
int wifi_aware_session_unset_service_discovered_cb(wifi_aware_session_h session);

/**
 * @brief Called when a message is sent to a peer.
 * @details The following error codes can be received: \n
 *              #WIFI_AWARE_ERROR_NONE                   Successful \n
 *              #WIFI_AWARE_ERROR_NOT_SUPPORTED          Not supported \n
 *              #WIFI_AWARE_ERROR_PERMISSION_DENIED      Permission denied \n
 *              #WIFI_AWARE_ERROR_INVALID_PARAMETER      Invalid function parameter \n
 *              #WIFI_AWARE_ERROR_INVALID_OPERATION      Invalid operation \n
 *              #WIFI_AWARE_ERROR_OPERATION_FAILED       Operation failed
 * @since_tizen 9.0
 * @remarks The @a session should not be released.
 * @param[in] session   The Wi-Fi Aware Session handle
 * @param[in] error     The error code
 * @param[in] user_data The user data passed from request function
 */
typedef void(*wifi_aware_send_message_result_cb)(wifi_aware_session_h session,
		wifi_aware_error_e error, void *user_data);

/**
 * @brief Sends the message to a peer.
 * @since_tizen 9.0
 * @privlevel public
 * @privilege %http://tizen.org/privilege/network.set
 * @param[in] session   The Wi-Fi Aware Session handle
 * @param[in] peer      The peer who gets the message
 * @param[in] message   The message sent to a peer
 * @param[in] len       The length of the message
 * @param[in] callback  The callback function
 * @param[in] user_data The user data passed to the callback function
 * @return 0 on success, otherwise a negative error value
 * @retval #WIFI_AWARE_ERROR_NONE                   Successful
 * @retval #WIFI_AWARE_ERROR_NOT_SUPPORTED          Not supported
 * @retval #WIFI_AWARE_ERROR_PERMISSION_DENIED      Permission denied
 * @retval #WIFI_AWARE_ERROR_INVALID_PARAMETER      Invalid function parameter
 * @retval #WIFI_AWARE_ERROR_NOT_INITIALIZED        Not initialized
 * @retval #WIFI_AWARE_ERROR_INVALID_OPERATION      Invalid operation
 * @retval #WIFI_AWARE_ERROR_OPERATION_FAILED       Operation failed
 * @retval #WIFI_AWARE_ERROR_OUT_OF_MEMORY          Out of memory
 */
int wifi_aware_session_send_message(
		wifi_aware_session_h session,
		wifi_aware_peer_h peer,
		const unsigned char *message, size_t len,
		wifi_aware_send_message_result_cb callback, void *user_data);

/**
 * @brief Called whenever receiving a message from a peer.
 * @since_tizen 9.0
 * @remarks The @a session and @a peer should not be released.
 * @param[in] session   The Wi-Fi Aware Session handle
 * @param[in] peer      The peer who sent the message
 * @param[in] message   The message which is sent
 * @param[in] len       The length of the message
 * @param[in] user_data The user data passed to the callback function
 * @see wifi_aware_session_set_message_received_cb()
 * @see wifi_aware_session_unset_message_received_cb()
 */
typedef void(*wifi_aware_message_received_cb)(
		wifi_aware_session_h session,
		wifi_aware_peer_h peer,
		const unsigned char *message, size_t len, void *user_data);

/**
 * @brief Sets a callback function to deliver received message from a peer.
 * @since_tizen 9.0
 * @param[in] session   The Wi-Fi Aware Session handle
 * @param[in] callback  The callback function which gets called
 * @param[in] user_data The user data passed to @a callback
 * @return 0 on success, otherwise a negative error value
 * @retval #WIFI_AWARE_ERROR_NONE                   Successful
 * @retval #WIFI_AWARE_ERROR_NOT_SUPPORTED          Not supported
 * @retval #WIFI_AWARE_ERROR_INVALID_PARAMETER      Invalid function parameter
 * @retval #WIFI_AWARE_ERROR_NOT_INITIALIZED        Not initialized
 * @see wifi_aware_session_unset_message_received_cb()
 */
int wifi_aware_session_set_message_received_cb(wifi_aware_session_h session,
		wifi_aware_message_received_cb callback, void *user_data);

/**
 * @brief Unsets a callback function for received message.
 * @since_tizen 9.0
 * @param[in] session The Wi-Fi Aware Session handle
 * @return 0 on success, otherwise a negative error value
 * @retval #WIFI_AWARE_ERROR_NONE                   Successful
 * @retval #WIFI_AWARE_ERROR_NOT_SUPPORTED          Not supported
 * @retval #WIFI_AWARE_ERROR_INVALID_PARAMETER      Invalid function parameter
 * @retval #WIFI_AWARE_ERROR_NOT_INITIALIZED        Not initialized
 * @see wifi_aware_session_set_message_received_cb()
 */
int wifi_aware_session_unset_message_received_cb(wifi_aware_session_h session);

/**
 * @brief Copies a Peer handle.
 * @since_tizen 9.0
 * @remarks The @a cloned should be released using wifi_aware_peer_destroy().
 * @param[out] cloned The Peer handle copied from @a origin
 * @param[in] origin  The origin Peer handle
 * @return 0 on success, otherwise a negative error value
 * @retval #WIFI_AWARE_ERROR_NONE                   Successful
 * @retval #WIFI_AWARE_ERROR_NOT_SUPPORTED          Not supported
 * @retval #WIFI_AWARE_ERROR_INVALID_PARAMETER      Invalid function parameter
 * @retval #WIFI_AWARE_ERROR_NOT_INITIALIZED        Not initialized
 * @retval #WIFI_AWARE_ERROR_OUT_OF_MEMORY          Out of memory
 *
 * @par Here is an example of the usage:
 * @code
 *
 *	wifi_aware_peer_h cloned_peer;
 * 	int ret = 0;
 *
 * 	ret = wifi_aware_peer_clone(&cloned_peer, src_peer);
 * 	if (ret != WIFI_AWARE_ERROR_NONE) {
 * 		printf("Failed to clone the peer\n");
 * 		return;
 * 	}
 *
 * 	// Do stuff here for cloned peer
 *
 * 	wifi_aware_peer_destroy(cloned_peer);
 *
 * @endcode
 * @see wifi_aware_peer_destroy()
 */
int wifi_aware_peer_clone(wifi_aware_peer_h *cloned, wifi_aware_peer_h origin);

/**
 * @brief Destroys a Peer handle.
 * @since_tizen 9.0
 * @param[in] peer The Peer handle
 * @return 0 on success, otherwise a negative error value
 * @retval #WIFI_AWARE_ERROR_NONE                   Successful
 * @retval #WIFI_AWARE_ERROR_NOT_SUPPORTED          Not supported
 * @retval #WIFI_AWARE_ERROR_INVALID_PARAMETER      Invalid function parameter
 * @retval #WIFI_AWARE_ERROR_NOT_INITIALIZED        Not initialized
 * @see wifi_aware_peer_clone()
 */
int wifi_aware_peer_destroy(wifi_aware_peer_h peer);

/**
 * @brief Gets the MAC address (NAN Management Interface) of Wi-Fi Aware peer.
 * @since_tizen 9.0
 * @remarks The @a mac should be freed using free().
 * @param[in] peer The Peer handle
 * @param[out] mac The MAC address of the peer
 * @return 0 on success, otherwise a negative error value
 * @retval #WIFI_AWARE_ERROR_NONE                   Successful
 * @retval #WIFI_AWARE_ERROR_NOT_SUPPORTED          Not supported
 * @retval #WIFI_AWARE_ERROR_INVALID_PARAMETER      Invalid function parameter
 * @retval #WIFI_AWARE_ERROR_NOT_INITIALIZED        Not initialized
 * @retval #WIFI_AWARE_ERROR_OUT_OF_MEMORY          Out of memory
 *
 * @par Here is an example of the usage:
 * @code
 *
 *	peer_info *info = (peer_info *)data;
 *	unsigned char *mac = NULL;
 * 	int ret = 0;
 *
 *	ret = wifi_aware_peer_get_mac(info->peer, &mac);
 *	if (ret != WIFI_AWARE_ERROR_NONE) {
 * 		printf("Failed to get mac of the peer\n");
 * 		return;
 * 	}
 *
 * // Do stuff here for the mac
 *
 *	free(mac);
 *
 * @endcode
 */
int wifi_aware_peer_get_mac(wifi_aware_peer_h peer, unsigned char **mac);

/**
 * @brief Enumeration for Wi-Fi Aware security type.
 * @since_tizen 9.0
 */
typedef enum {
	WIFI_AWARE_SECURITY_TYPE_OPEN, /**< Wi-Fi Aware security type: Open */
	WIFI_AWARE_SECURITY_TYPE_PSK, /**< Wi-Fi Aware security type: PSK */
	WIFI_AWARE_SECURITY_TYPE_PMK, /**< Wi-Fi Aware security type: PMK */
} wifi_aware_security_type_e;

/**
 * @brief Creates a handle to be used for Wi-Fi Aware Data Path.
 * @since_tizen 9.0
 * @remarks The @a data_path should be released using wifi_aware_data_path_destroy().
 * @param[in] session    The Wi-Fi Aware Session handle
 * @param[in] peer       The Peer handle
 * @param[out] data_path The Data Path handle
 * @return 0 on success, otherwise a negative error value
 * @retval #WIFI_AWARE_ERROR_NONE                   Successful
 * @retval #WIFI_AWARE_ERROR_NOT_SUPPORTED          Not supported
 * @retval #WIFI_AWARE_ERROR_INVALID_PARAMETER      Invalid function parameter
 * @retval #WIFI_AWARE_ERROR_NOT_INITIALIZED        Not initialized
 * @retval #WIFI_AWARE_ERROR_OUT_OF_MEMORY          Out of memory
 *
 * @par Here is an example of the usage:
 * @code
 *
 *	wifi_aware_data_path_h data_path;
 *	int ret = 0;
 *
 *	ret = wifi_aware_data_path_create(g_session, g_peer, &data_path);
 *	if (ret != WIFI_AWARE_ERROR_NONE) {
 * 		printf("Failed to create data path\n");
 * 		return;
 * 	}
 *
 * // Do stuff here for the data path
 *
 *	wifi_aware_data_path_destroy(data_path);
 *
 * @endcode
 * @see wifi_aware_data_path_destroy()
 */
int wifi_aware_data_path_create(wifi_aware_session_h session, wifi_aware_peer_h peer,
		wifi_aware_data_path_h *data_path);

/**
 * @brief Sets a security type for Wi-Fi Aware Data Path.
 * @since_tizen 9.0
 * @param[in] data_path     The Data Path handle
 * @param[in] security_type The security type
 * @return 0 on success, otherwise a negative error value
 * @retval #WIFI_AWARE_ERROR_NONE                   Successful
 * @retval #WIFI_AWARE_ERROR_NOT_SUPPORTED          Not supported
 * @retval #WIFI_AWARE_ERROR_INVALID_PARAMETER      Invalid function parameter
 * @retval #WIFI_AWARE_ERROR_NOT_INITIALIZED        Not initialized
 * @see wifi_aware_data_path_create()
 */
int wifi_aware_data_path_set_security(wifi_aware_data_path_h data_path, wifi_aware_security_type_e security_type);

/**
 * @brief Sets a passphrase for Wi-Fi Aware Data Path.
 * @since_tizen 9.0
 * @remarks The @a key can be used only when secure type is #WIFI_AWARE_SECURITY_TYPE_PSK.
 * @param[in] data_path The Data Path handle
 * @param[in] key       The passphrase
 * @return 0 on success, otherwise a negative error value
 * @retval #WIFI_AWARE_ERROR_NONE                   Successful
 * @retval #WIFI_AWARE_ERROR_NOT_SUPPORTED          Not supported
 * @retval #WIFI_AWARE_ERROR_INVALID_PARAMETER      Invalid function parameter
 * @retval #WIFI_AWARE_ERROR_NOT_INITIALIZED        Not initialized
 * @retval #WIFI_AWARE_ERROR_INVALID_OPERATION      Invalid operation
 * @see wifi_aware_data_path_create()
 */
int wifi_aware_data_path_set_psk(wifi_aware_data_path_h data_path, const char *key);

/**
 * @brief Sets a PMK for Wi-Fi Aware Data Path.
 * @since_tizen 9.0
 * @remarks The @a key can be used only when security type is #WIFI_AWARE_SECURITY_TYPE_PMK.
 * @param[in] data_path The Data Path handle
 * @param[in] key        Pairwise Master Key. Its length shall be of length 32.
 * @return 0 on success, otherwise a negative error value
 * @retval #WIFI_AWARE_ERROR_NONE                   Successful
 * @retval #WIFI_AWARE_ERROR_NOT_SUPPORTED          Not supported
 * @retval #WIFI_AWARE_ERROR_INVALID_PARAMETER      Invalid function parameter
 * @retval #WIFI_AWARE_ERROR_NOT_INITIALIZED        Not initialized
 * @retval #WIFI_AWARE_ERROR_INVALID_OPERATION      Invalid operation
 * @see wifi_aware_data_path_create()
 */
int wifi_aware_data_path_set_pmk(wifi_aware_data_path_h data_path, const unsigned char *key);

/**
 * @brief Sets a port for Wi-Fi Aware Data Path.
 * @since_tizen 9.0
 * @remarks The @a port cannot be exceed 65535.
 * @param[in] data_path The Data Path handle
 * @param[in] port      The port
 * @return 0 on success, otherwise a negative error value
 * @retval #WIFI_AWARE_ERROR_NONE                   Successful
 * @retval #WIFI_AWARE_ERROR_NOT_SUPPORTED          Not supported
 * @retval #WIFI_AWARE_ERROR_INVALID_PARAMETER      Invalid function parameter
 * @retval #WIFI_AWARE_ERROR_NOT_INITIALIZED        Not initialized
 * @see wifi_aware_data_path_create()
 */
int wifi_aware_data_path_set_port(wifi_aware_data_path_h data_path, int port);

/**
 * @brief Destroys a handle for Wi-Fi Aware Data Path.
 * @since_tizen 9.0
 * @param[in] data_path The Data Path handle
 * @return 0 on success, otherwise a negative error value
 * @retval #WIFI_AWARE_ERROR_NONE                   Successful
 * @retval #WIFI_AWARE_ERROR_NOT_SUPPORTED          Not supported
 * @retval #WIFI_AWARE_ERROR_INVALID_PARAMETER      Invalid function parameter
 * @retval #WIFI_AWARE_ERROR_NOT_INITIALIZED        Not initialized
 * @see wifi_aware_data_path_create()
 */
int wifi_aware_data_path_destroy(wifi_aware_data_path_h data_path);

/**
 * @brief Called when Data Path open request is done.
 * @details The following error codes can be received: \n
 *              #WIFI_AWARE_ERROR_NONE                   Successful \n
 *              #WIFI_AWARE_ERROR_NOT_SUPPORTED          Not supported \n
 *              #WIFI_AWARE_ERROR_PERMISSION_DENIED      Permission denied \n
 *              #WIFI_AWARE_ERROR_INVALID_PARAMETER      Invalid function parameter \n
 *              #WIFI_AWARE_ERROR_INVALID_OPERATION      Invalid operation \n
 *              #WIFI_AWARE_ERROR_OPERATION_FAILED       Operation failed
 * @since_tizen 9.0
 * @remarks The @a data_path should not be released.
 * @param[in] data_path The Data Path handle
 * @param[in] error     The result of the open request
 * @param[in] user_data The user data passed from request function
 * @see wifi_aware_data_path_open()
 */
typedef void(*wifi_aware_data_path_open_cb)(wifi_aware_data_path_h data_path, wifi_aware_error_e error,
		void *user_data);

/**
 * @brief Requests to open a Wi-Fi Aware Data Path.
 * @since_tizen 9.0
 * @privlevel public
 * @privilege %http://tizen.org/privilege/network.set
 * @remarks The @a callback will be always invoked whether the open request succeeds or fails.
 * @param[in] data_path The Data Path handle
 * @param[in] callback  The callback function
 * @param[in] user_data The user data passed to the callback function
 * @return 0 on success, otherwise a negative error value
 * @retval #WIFI_AWARE_ERROR_NONE                   Successful
 * @retval #WIFI_AWARE_ERROR_NOT_SUPPORTED          Not supported
 * @retval #WIFI_AWARE_ERROR_PERMISSION_DENIED      Permission denied
 * @retval #WIFI_AWARE_ERROR_INVALID_PARAMETER      Invalid function parameter
 * @retval #WIFI_AWARE_ERROR_NOT_INITIALIZED        Not initialized
 * @retval #WIFI_AWARE_ERROR_INVALID_OPERATION      Invalid operation
 * @retval #WIFI_AWARE_ERROR_OPERATION_FAILED       Operation failed
 * @see wifi_aware_data_path_close()
 */
int wifi_aware_data_path_open(wifi_aware_data_path_h data_path,
		wifi_aware_data_path_open_cb callback, void *user_data);

/**
 * @brief Closes a Wi-Fi Aware Data Path.
 * @since_tizen 9.0
 * @privlevel public
 * @privilege %http://tizen.org/privilege/network.set
 * @param[in] data_path The Data Path handle
 * @return 0 on success, otherwise a negative error value
 * @retval #WIFI_AWARE_ERROR_NONE                   Successful
 * @retval #WIFI_AWARE_ERROR_NOT_SUPPORTED          Not supported
 * @retval #WIFI_AWARE_ERROR_PERMISSION_DENIED      Permission denied
 * @retval #WIFI_AWARE_ERROR_INVALID_PARAMETER      Invalid function parameter
 * @retval #WIFI_AWARE_ERROR_NOT_INITIALIZED        Not initialized
 * @retval #WIFI_AWARE_ERROR_INVALID_OPERATION      Invalid operation
 * @retval #WIFI_AWARE_ERROR_OPERATION_FAILED       Operation failed
 * @see wifi_aware_data_path_open()
 */
int wifi_aware_data_path_close(wifi_aware_data_path_h data_path);

/**
 * @brief Gets a port of a peer.
 * @since_tizen 9.0
 * @param[in] data_path The Data Path handle
 * @param[out] port     The port
 * @return 0 on success, otherwise a negative error value
 * @retval #WIFI_AWARE_ERROR_NONE                   Successful
 * @retval #WIFI_AWARE_ERROR_NOT_SUPPORTED          Not supported
 * @retval #WIFI_AWARE_ERROR_INVALID_PARAMETER      Invalid function parameter
 * @retval #WIFI_AWARE_ERROR_NOT_INITIALIZED        Not initialized
 * @retval #WIFI_AWARE_ERROR_INVALID_OPERATION      Invalid operation
 */
int wifi_aware_data_path_get_peer_port(wifi_aware_data_path_h data_path, int* port);

/**
 * @brief Gets an IPv6 address of a peer.
 * @since_tizen 9.0
 * @remarks The @a ipv6_address should be freed by using free().
 * @param[in] data_path     The Data Path handle
 * @param[out] ipv6_address The IPv6 address of a peer
 * @return 0 on success, otherwise a negative error value
 * @retval #WIFI_AWARE_ERROR_NONE                   Successful
 * @retval #WIFI_AWARE_ERROR_NOT_SUPPORTED          Not supported
 * @retval #WIFI_AWARE_ERROR_INVALID_PARAMETER      Invalid function parameter
 * @retval #WIFI_AWARE_ERROR_NOT_INITIALIZED        Not initialized
 * @retval #WIFI_AWARE_ERROR_INVALID_OPERATION      Invalid operation
 *
 * @par Here is an example of the usage:
 * @code
 *
 *	int ret = 0;
 *	char *ipv6 = NULL;
 *
 *	ret = wifi_aware_data_path_get_peer_ipv6_address(g_data_path, &ipv6);
 *	if (ret != WIFI_AWARE_ERROR_NONE) {
 * 		printf("Failed to get IPv6 address of the peer\n");
 * 		return;
 * 	}
 *
 * 	// Do stuff here for the IPv6 address of the peer
 *
 *	free(ipv6);
 *
 * @endcode
 */
int wifi_aware_data_path_get_peer_ipv6_address(wifi_aware_data_path_h data_path, char **ipv6_address);

/**
 * @brief Gets Interface name for Wi-Fi Aware Data Path.
 * @since_tizen 9.0
 * @remarks The @a interface should be freed by using free().
 * @param[in] data_path  The Data Path handle
 * @param[out] interface The interface name
 * @return 0 on success, otherwise a negative error value
 * @retval #WIFI_AWARE_ERROR_NONE                   Successful
 * @retval #WIFI_AWARE_ERROR_NOT_SUPPORTED          Not supported
 * @retval #WIFI_AWARE_ERROR_INVALID_PARAMETER      Invalid function parameter
 * @retval #WIFI_AWARE_ERROR_NOT_INITIALIZED        Not initialized
 * @retval #WIFI_AWARE_ERROR_INVALID_OPERATION      Invalid operation
 *
 * @par Here is an example of the usage:
 * @code
 *
 *	int ret = 0;
 *	char *interface = NULL;
 *
 *	ret = wifi_aware_data_path_get_interface(g_data_path, &interface);
 *	if (ret != WIFI_AWARE_ERROR_NONE) {
 * 		printf("Failed to get interface name\n");
 * 		return;
 * 	}
 *
 * 	// Do stuff here for the interface name
 *
 *	free(interface);
 *
 * @endcode
 */
int wifi_aware_data_path_get_interface(wifi_aware_data_path_h data_path, char **interface);

/**
 * @brief Called when the Wi-Fi Aware Data Path has been lost.
 * @since_tizen 9.0
 * @remarks The @a data_path should not be released.
 * @param[in] data_path The terminated Data Path handle
 * @param[in] reason    The reason of termination
 * @param[in] user_data The user data passed from request function
 * @see wifi_aware_data_path_set_terminated_cb()
 * @see wifi_aware_data_path_unset_terminated_cb()
 */
typedef void(*wifi_aware_data_path_terminated_cb)(wifi_aware_data_path_h data_path, wifi_aware_termination_reason_e reason, void *user_data);

/**
 * @brief Sets a callback function to notify a Data Path termination.
 * @since_tizen 9.0
 * @param[in] data_path The Data Path handle
 * @param[in] callback  The callback function
 * @param[in] user_data The user data passed to the callback function
 * @return 0 on success, otherwise a negative error value
 * @retval #WIFI_AWARE_ERROR_NONE                   Successful
 * @retval #WIFI_AWARE_ERROR_NOT_SUPPORTED          Not supported
 * @retval #WIFI_AWARE_ERROR_INVALID_PARAMETER      Invalid function parameter
 * @retval #WIFI_AWARE_ERROR_NOT_INITIALIZED        Not initialized
 * @see wifi_aware_data_path_unset_terminated_cb()
 */
int wifi_aware_data_path_set_terminated_cb(wifi_aware_data_path_h data_path, wifi_aware_data_path_terminated_cb callback, void *user_data);

/**
 * @brief Unsets the Data Path terminated callback function.
 * @since_tizen 9.0
 * @param[in] data_path The Data Path handle
 * @return 0 on success, otherwise a negative error value
 * @retval #WIFI_AWARE_ERROR_NONE                   Successful
 * @retval #WIFI_AWARE_ERROR_NOT_SUPPORTED          Not supported
 * @retval #WIFI_AWARE_ERROR_INVALID_PARAMETER      Invalid function parameter
 * @retval #WIFI_AWARE_ERROR_NOT_INITIALIZED        Not initialized
 * @see wifi_aware_data_path_set_terminated_cb()
 */
int wifi_aware_data_path_unset_terminated_cb(wifi_aware_data_path_h data_path);

/**
 * @}
 */

/**
 * @addtogroup CAPI_NETWORK_WIFI_AWARE_PUBLISH_MODULE
 * @{
 */

/**
 * @brief Creates a Publish request.
 * @since_tizen 9.0
 * @remarks The @a publish should be released using wifi_aware_publish_destroy().
 * @param[out] publish The Wi-Fi Aware Publish handle.
 * @return 0 on success, otherwise a negative error value
 * @retval #WIFI_AWARE_ERROR_NONE                   Successful
 * @retval #WIFI_AWARE_ERROR_NOT_SUPPORTED          Not supported
 * @retval #WIFI_AWARE_ERROR_INVALID_PARAMETER      Invalid function parameter
 * @retval #WIFI_AWARE_ERROR_NOT_INITIALIZED        Not initialized
 * @retval #WIFI_AWARE_ERROR_OUT_OF_MEMORY          Out of memory
 *
 * @par Here is an example of the usage:
 * @code
 *
 * 	int ret = 0;
 * 	wifi_aware_session_h wifi_aware_session = NULL;
 * 	static wifi_aware_publish_h publish_handle = NULL;
 *
 * 	ret = wifi_aware_session_create(WIFI_AWARE_SESSION_PUBLISH, &wifi_aware_session);
 * 	if (ret != WIFI_AWARE_ERROR_NONE) {
 * 		printf("Failed to create session\n");
 * 		return;
 * 	}
 *
 * 	ret = wifi_aware_publish_create(&publish_handle);
 * 	if (ret != WIFI_AWARE_ERROR_NONE) {
 * 		printf("Failed to create publish request\n");
 * 		return;
 * 	}
 *
 * 	printf("Publish request is created Successfully\n");
 *
 * 	wifi_aware_publish_destroy(publish_handle);
 * 	wifi_aware_session_destroy(wifi_aware_session);
 *
 * @endcode
 * @see wifi_aware_publish_destroy()
 */
int wifi_aware_publish_create(wifi_aware_publish_h *publish);

/**
 * @brief Destroys a Publish request.
 * @since_tizen 9.0
 * @param[in] publish The Wi-Fi Aware Publish handle.
 * @return 0 on success, otherwise a negative error value
 * @retval #WIFI_AWARE_ERROR_NONE                   Successful
 * @retval #WIFI_AWARE_ERROR_NOT_SUPPORTED          Not supported
 * @retval #WIFI_AWARE_ERROR_INVALID_PARAMETER      Invalid function parameter
 * @retval #WIFI_AWARE_ERROR_NOT_INITIALIZED        Not initialized
 * @see wifi_aware_publish_create()
 */
int wifi_aware_publish_destroy(wifi_aware_publish_h publish);

/**
 * @brief Sets TTL (Time To Live) for Publish session.
 * @since_tizen 9.0
 * @param[in] publish The Wi-Fi Aware Publish handle.
 * @param[in] ttl The lifetime(seconds) of publish session. 0 means forever until canceled.
 * @return 0 on success, otherwise a negative error value
 * @retval #WIFI_AWARE_ERROR_NONE                   Successful
 * @retval #WIFI_AWARE_ERROR_NOT_SUPPORTED          Not supported
 * @retval #WIFI_AWARE_ERROR_INVALID_PARAMETER      Invalid function parameter
 * @retval #WIFI_AWARE_ERROR_NOT_INITIALIZED        Not initialized
 * @see wifi_aware_publish_create()
 */
int wifi_aware_publish_set_ttl(wifi_aware_publish_h publish, unsigned short ttl);

/**
 * @brief Sets the type of the service as solicited or unsolicited.
 * @since_tizen 9.0
 * @param[in] publish The Wi-Fi Aware Publish handle
 * @param[in] publish_type The Publish type
 * @return 0 on success, otherwise a negative error value
 * @retval #WIFI_AWARE_ERROR_NONE                   Successful
 * @retval #WIFI_AWARE_ERROR_NOT_SUPPORTED          Not supported
 * @retval #WIFI_AWARE_ERROR_INVALID_PARAMETER      Invalid function parameter
 * @retval #WIFI_AWARE_ERROR_NOT_INITIALIZED        Not initialized
 * @see wifi_aware_publish_create()
 */
int wifi_aware_publish_set_type(wifi_aware_publish_h publish, wifi_aware_publish_type_e publish_type);

/**
 * @brief Sets the name of the service to publish.
 * @since_tizen 9.0
 * @remarks The length should be less than or equal to 255.
 * @param[in] publish The Wi-Fi Aware Publish handle
 * @param[in] service_name UTF-8 name which identifies the service
 * @return 0 on success, otherwise a negative error value
 * @retval #WIFI_AWARE_ERROR_NONE                   Successful
 * @retval #WIFI_AWARE_ERROR_NOT_SUPPORTED          Not supported
 * @retval #WIFI_AWARE_ERROR_INVALID_PARAMETER      Invalid function parameter
 * @retval #WIFI_AWARE_ERROR_NOT_INITIALIZED        Not initialized
 * @see wifi_aware_publish_create()
 */
int wifi_aware_publish_set_service_name(wifi_aware_publish_h publish, const char *service_name);

/**
 * @brief Sets the specific information for the service to publish.
 * @since_tizen 9.0
 * @param[in] publish The Wi-Fi Aware Publish handle
 * @param[in] service_specific_info The Sequence of values that are conveyed in the Publish message
 * @param[in] len The length of service specific information
 * @return 0 on success, otherwise a negative error value
 * @retval #WIFI_AWARE_ERROR_NONE                   Successful
 * @retval #WIFI_AWARE_ERROR_NOT_SUPPORTED          Not supported
 * @retval #WIFI_AWARE_ERROR_INVALID_PARAMETER      Invalid function parameter
 * @retval #WIFI_AWARE_ERROR_NOT_INITIALIZED        Not initialized
 * @see wifi_aware_publish_create()
 */
int wifi_aware_publish_set_service_specific_info(wifi_aware_publish_h publish, const unsigned char *service_specific_info, size_t len);

/**
 * @brief Sets the match filter of the service to publish.
 * @since_tizen 9.0
 * @remarks The match filter is used to decide whether accepting a subscribe request or not.
 * @param[in] publish The Wi-Fi Aware Publish handle
 * @param[in] match_filter An ordered sequence of <length, value> pairs
 * @param[in] len The whole length of match_filter
 * @return 0 on success, otherwise a negative error value
 * @retval #WIFI_AWARE_ERROR_NONE                   Successful
 * @retval #WIFI_AWARE_ERROR_NOT_SUPPORTED          Not supported
 * @retval #WIFI_AWARE_ERROR_INVALID_PARAMETER      Invalid function parameter
 * @retval #WIFI_AWARE_ERROR_NOT_INITIALIZED        Not initialized
 * @see wifi_aware_publish_create()
 */
int wifi_aware_publish_set_match_filter(wifi_aware_publish_h publish, const unsigned char *match_filter, size_t len);

/**
 * @brief Sets whether the ranging function is supported or not.
 * @since_tizen 9.0
 * @param[in] publish The Wi-Fi Aware Publish handle
 * @param[in] enable @c true if ranging is enabled,
 *                   otherwise @c false if ranging is disabled.
 * @return 0 on success, otherwise a negative error value
 * @retval #WIFI_AWARE_ERROR_NONE                   Successful
 * @retval #WIFI_AWARE_ERROR_NOT_SUPPORTED          Not supported
 * @retval #WIFI_AWARE_ERROR_INVALID_PARAMETER      Invalid function parameter
 * @retval #WIFI_AWARE_ERROR_NOT_INITIALIZED        Not initialized
 * @see wifi_aware_publish_create()
 */
int wifi_aware_publish_enable_ranging(wifi_aware_publish_h publish, bool enable);

/**
 * @}
 */

/**
 * @addtogroup CAPI_NETWORK_WIFI_AWARE_SUBSCRIBE_MODULE
 * @{
 */

/**
 * @brief Creates a Subscribe request.
 * @since_tizen 9.0
 * @remarks The @a subscribe should be released using wifi_aware_subscribe_destroy().
 * @param[out] subscribe The Wi-Fi Aware Subscribe handle.
 * @return 0 on success, otherwise a negative error value
 * @retval #WIFI_AWARE_ERROR_NONE                   Successful
 * @retval #WIFI_AWARE_ERROR_NOT_SUPPORTED          Not supported
 * @retval #WIFI_AWARE_ERROR_INVALID_PARAMETER      Invalid function parameter
 * @retval #WIFI_AWARE_ERROR_NOT_INITIALIZED        Not initialized
 * @retval #WIFI_AWARE_ERROR_OUT_OF_MEMORY          Out of memory
 *
 * @par Here is an example of the usage:
 * @code
 *
 * 	int ret = 0;
 * 	wifi_aware_session_h wifi_aware_session = NULL;
 * 	wifi_aware_subscribe_h subscribe_handle = NULL;
 *
 * 	ret = wifi_aware_session_create(WIFI_AWARE_SESSION_SUBSCRIBE, &wifi_aware_session);
 * 	if (ret != WIFI_AWARE_ERROR_NONE) {
 * 		printf("Failed to create session\n");
 * 		return;
 * 	}
 *
 * 	ret = wifi_aware_subscribe_create(&subscribe_handle);
 * 	if (ret != WIFI_AWARE_ERROR_NONE) {
 * 		printf("Failed to create subscribe handle\n");
 * 		return;
 * 	}
 *
 * 	// Setup and use the subscribe handle
 *
 * 	wifi_aware_subscribe_destroy(subscribe_handle);
 * 	wifi_aware_session_destroy(wifi_aware_session);
 *
 * @endcode
 * @see wifi_aware_subscribe_destroy()
 */
int wifi_aware_subscribe_create(wifi_aware_subscribe_h *subscribe);

/**
 * @brief Destroys a Subscribe request.
 * @since_tizen 9.0
 * @param[in] subscribe The Wi-Fi Aware Subscribe handle.
 * @return 0 on success, otherwise a negative error value
 * @retval #WIFI_AWARE_ERROR_NONE                   Successful
 * @retval #WIFI_AWARE_ERROR_NOT_SUPPORTED          Not supported
 * @retval #WIFI_AWARE_ERROR_INVALID_PARAMETER      Invalid function parameter
 * @retval #WIFI_AWARE_ERROR_NOT_INITIALIZED        Not initialized
 * @see wifi_aware_subscribe_create()
 */
int wifi_aware_subscribe_destroy(wifi_aware_subscribe_h subscribe);

/**
 * @brief Sets TTL (Time To Live) for Subscribe session.
 * @since_tizen 9.0
 * @param[in] subscribe The Wi-Fi Aware Subscribe handle
 * @param[in] ttl The lifetime(seconds) of subscribe session.\n
 *                0 means forever until wifi_aware_service_discovered_cb().
 * @return 0 on success, otherwise a negative error value
 * @retval #WIFI_AWARE_ERROR_NONE                   Successful
 * @retval #WIFI_AWARE_ERROR_NOT_SUPPORTED          Not supported
 * @retval #WIFI_AWARE_ERROR_INVALID_PARAMETER      Invalid function parameter
 * @retval #WIFI_AWARE_ERROR_NOT_INITIALIZED        Not initialized
 * @see wifi_aware_subscribe_create()
 */
int wifi_aware_subscribe_set_ttl(wifi_aware_subscribe_h subscribe, unsigned short ttl);

/**
 * @brief Sets the type of the service as passive or active.
 * @since_tizen 9.0
 * @param[in] subscribe The Wi-Fi Aware Subscribe handle
 * @param[in] subscribe_type The Subscribe type
 * @return 0 on success, otherwise a negative error value
 * @retval #WIFI_AWARE_ERROR_NONE                   Successful
 * @retval #WIFI_AWARE_ERROR_NOT_SUPPORTED          Not supported
 * @retval #WIFI_AWARE_ERROR_INVALID_PARAMETER      Invalid function parameter
 * @retval #WIFI_AWARE_ERROR_NOT_INITIALIZED        Not initialized
 * @see wifi_aware_subscribe_create()
 */
int wifi_aware_subscribe_set_type(wifi_aware_subscribe_h subscribe, wifi_aware_subscribe_type_e subscribe_type);

/**
 * @brief Sets the name of the service to subscribe.
 * @since_tizen 9.0
 * @remarks The length should be less than or equal to 255
 * @param[in] subscribe The Wi-Fi Aware Subscribe handle
 * @param[in] service_name UTF-8 name which identifies the service
 * @return 0 on success, otherwise a negative error value
 * @retval #WIFI_AWARE_ERROR_NONE                   Successful
 * @retval #WIFI_AWARE_ERROR_NOT_SUPPORTED          Not supported
 * @retval #WIFI_AWARE_ERROR_INVALID_PARAMETER      Invalid function parameter
 * @retval #WIFI_AWARE_ERROR_NOT_INITIALIZED        Not initialized
 * @see wifi_aware_subscribe_create()
 */
int wifi_aware_subscribe_set_service_name(wifi_aware_subscribe_h subscribe, const char *service_name);

/**
 * @brief Sets the specific information for the service to subscribe.
 * @since_tizen 9.0
 * @param[in] subscribe The Wi-Fi Aware Subscribe handle
 * @param[in] service_specific_info The Sequence of values that are conveyed in the Subscribe message
 * @param[in] len The length of service specific information
 * @return 0 on success, otherwise a negative error value
 * @retval #WIFI_AWARE_ERROR_NONE                   Successful
 * @retval #WIFI_AWARE_ERROR_NOT_SUPPORTED          Not supported
 * @retval #WIFI_AWARE_ERROR_INVALID_PARAMETER      Invalid function parameter
 * @retval #WIFI_AWARE_ERROR_NOT_INITIALIZED        Not initialized
 * @see wifi_aware_subscribe_create()
 */
int wifi_aware_subscribe_set_service_specific_info(wifi_aware_subscribe_h subscribe, const unsigned char *service_specific_info, size_t len);

/**
 * @brief Sets the match filter of the service to subscribe.
 * @since_tizen 9.0
 * @remarks The match filter is used to decide whether accepting a subscribe request or not.
 * @param[in] subscribe The Wi-Fi Aware Subscribe handle
 * @param[in] match_filter An ordered sequence of <length, value> pairs
 * @param[in] len The whole length of match_filter
 * @return 0 on success, otherwise a negative error value
 * @retval #WIFI_AWARE_ERROR_NONE                   Successful
 * @retval #WIFI_AWARE_ERROR_NOT_SUPPORTED          Not supported
 * @retval #WIFI_AWARE_ERROR_INVALID_PARAMETER      Invalid function parameter
 * @retval #WIFI_AWARE_ERROR_NOT_INITIALIZED        Not initialized
 * @see wifi_aware_subscribe_create()
 */
int wifi_aware_subscribe_set_match_filter(wifi_aware_subscribe_h subscribe, const unsigned char *match_filter, size_t len);

/**
 * @brief Sets the maximum distance.
 * @since_tizen 9.0
 * @remarks Only for the services within the @a distance are searched.
 * @param[in] subscribe The Wi-Fi Aware Subscribe handle
 * @param[in] distance The maximum distance
 * @return 0 on success, otherwise a negative error value
 * @retval #WIFI_AWARE_ERROR_NONE                   Successful
 * @retval #WIFI_AWARE_ERROR_NOT_SUPPORTED          Not supported
 * @retval #WIFI_AWARE_ERROR_INVALID_PARAMETER      Invalid function parameter
 * @retval #WIFI_AWARE_ERROR_NOT_INITIALIZED        Not initialized
 * @see wifi_aware_subscribe_create()
 */
int wifi_aware_subscribe_set_max_distance(wifi_aware_subscribe_h subscribe, int distance);

/**
 * @brief Sets the minimum distance.
 * @since_tizen 9.0
 * @remarks Only for the services at least at the @a distance are searched.
 * @param[in] subscribe The Wi-Fi Aware Subscribe handle
 * @param[in] distance The minimum distance
 * @return 0 on success, otherwise a negative error value
 * @retval #WIFI_AWARE_ERROR_NONE                   Successful
 * @retval #WIFI_AWARE_ERROR_NOT_SUPPORTED          Not supported
 * @retval #WIFI_AWARE_ERROR_INVALID_PARAMETER      Invalid function parameter
 * @retval #WIFI_AWARE_ERROR_NOT_INITIALIZED        Not initialized
 * @see wifi_aware_subscribe_create()
 */
int wifi_aware_subscribe_set_min_distance(wifi_aware_subscribe_h subscribe, int distance);

/**
 * @}
 */

#ifdef __cplusplus
}
#endif

#endif /* __TIZEN_NET_WIFI_AWARE_H__ */

