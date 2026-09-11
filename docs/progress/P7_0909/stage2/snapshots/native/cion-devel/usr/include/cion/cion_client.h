/*
 * Copyright (c) 2021 Samsung Electronics Co., Ltd.
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

#ifndef __TIZEN_APPFW_CION_CLIENT_H__
#define __TIZEN_APPFW_CION_CLIENT_H__

#include <cion_payload.h>
#include <cion_peer_info.h>
#include <cion_security.h>
#include <cion_connection_result.h>
#include <cion_payload_async_result.h>

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @addtogroup CAPI_CION_CLIENT_MODULE
 * @{
 */

/**
 * @brief The Cion client handle.
 * @since_tizen 6.5
 */
typedef void *cion_client_h;

/**
 * @brief Called when the server is discovered.
 * @since_tizen 6.5
 * @param[in] service_name The name of service \n
 *            The @a service_name can be used only in the callback. To use outside, make a copy.
 * @param[in] peer_info The Cion peer information handle \n
 *            The @a peer_info can be used only in the callback. To use outside, make a copy.
 * @param[in] user_data The user data
 * @see cion_client_try_discovery()
 * @see #cion_peer_info_h
 */
typedef void (*cion_client_server_discovered_cb)(const char *service_name,
    const cion_peer_info_h peer_info, void *user_data);

/**
 * @brief Called when the connection is accepted or rejected.
 * @since_tizen 6.5
 * @param[in] service_name The name of service \n
 *            The @a service_name can be used only in the callback. To use outside, make a copy.
 * @param[in] peer_info The Cion peer information handle \n
 *            The @a peer_info can be used only in the callback. To use outside, make a copy.
 * @param[in] result The handle for connection result \n
 *            The @a result can be used only in the callback. To use outside, make a copy.
 * @param[in] user_data The user data
 * @see cion_client_add_connection_result_cb()
 * @see #cion_peer_info_h
 * @see #cion_connection_result_h
 */
typedef void (*cion_client_connection_result_cb)(
    const char *service_name, const cion_peer_info_h peer_info,
    const cion_connection_result_h result, void *user_data);

/**
 * @brief Called after sending payload asynchronously.
 * @since_tizen 6.5
 * @param[in] result The result of sending payload \n
 *            The @a result can be used only in the callback. To use outside, make a copy.
 * @param[in] user_data The user data
 * @see cion_client_send_payload_async()
 * @see #cion_payload_async_result_h
 */
typedef void (*cion_client_payload_async_result_cb)(
    const cion_payload_async_result_h result, void *user_data);

/**
 * @brief Called when the payload is received.
 * @since_tizen 6.5
 * @param[in] service_name The name of service \n
 *            The @a service_name is the name of connected service that sent the payload.
 *            The @a service_name can be used only in the callback. To use outside, make a copy.
 * @param[in] peer_info The Cion peer information handle \n
 *            The @a peer_info can be used only in the callback. To use outside, make a copy.
 * @param[in] payload The received payload \n
 *            The @a payload can be used only in the callback. \n
 *            The @a payload should not be released.
 * @param[in] status The status of transfer
 * @param[in] user_data The user data
 * @see cion_client_add_payload_received_cb()
 * @see #cion_peer_info_h
 * @see #cion_payload_h
 * @see #cion_payload_transfer_status_e
 */
typedef void (*cion_client_payload_received_cb)(const char *service_name,
    const cion_peer_info_h peer_info, const cion_payload_h payload,
    cion_payload_transfer_status_e status,
    void *user_data);

/**
 * @brief Called when the connection is disconnected.
 * @since_tizen 6.5
 * @param[in] service_name The name of service \n
 *            The @a service_name can be used only in the callback. To use outside, make a copy.
 * @param[in] peer_info The cion peer information handle \n
 *            The @a peer_info can be used only in the callback. To use outside, make a copy.
 * @param[in] user_data The user data
 * @see cion_client_add_disconnected_cb()
 * @see #cion_peer_info_h
 */
typedef void (*cion_client_disconnected_cb)(const char *service_name,
    const cion_peer_info_h peer_info, void *user_data);

/**
 * @brief Creates a Cion client handle.
 * @details The Cion client requests connection to server.
 * @since_tizen 6.5
 * @remarks @a client must be released using cion_client_destroy().
 * @remarks Max length of @a service_name including the null terminator is 512.
 * @param[out] client The Cion client handle
 * @param[in] service_name The name of service
 * @param[in] security The Cion security handle
 * @return @c 0 on success, otherwise a negative error value
 * @retval #CION_ERROR_NONE Successful
 * @retval #CION_ERROR_NOT_SUPPORTED Not supported
 * @retval #CION_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #CION_ERROR_OUT_OF_MEMORY Out of memory
 * @retval #CION_ERROR_INVALID_OPERATION Invalid operation
 * @retval #CION_ERROR_OPERATION_FAILED Operation failed
 * @see cion_server_destroy()
 * @see cion_security_create()
 * @par Sample code:
 * @code
#include <cion.h>

{
	int ret;
	cion_client_h client = NULL;

	ret = cion_client_create(&client, "myclient", NULL);
}
 * @endcode
 */
int cion_client_create(cion_client_h *client, const char *service_name,
    cion_security_h security);

/**
 * @brief Destroys the Cion client handle.
 * @since_tizen 6.5
 * @param[in] client The Cion client handle
 * @return @c 0 on success, otherwise a negative error value
 * @retval #CION_ERROR_NONE Successful
 * @retval #CION_ERROR_INVALID_PARAMETER Invalid parameter
 * @see cion_security_create()
 * @par Sample code:
 * @code
#include <cion.h>

{
	int ret;

	ret = cion_client_destroy(client);
}
 * @endcode
 */
int cion_client_destroy(cion_client_h client);

/**
 * @brief Tries to discover a Cion server.
 * @since_tizen 6.5
 * @privlevel public
 * @privilege %http://tizen.org/privilege/d2d.datasharing \n
 *            %http://tizen.org/privilege/internet
 * @remarks Tries to discover the server with service name that the client handle has.
 * @param[in] client The Cion client handle
 * @param[in] cb The callback function
 * @param[in] user_data The user data
 * @return @c 0 on success, otherwise a negative error value
 * @retval #CION_ERROR_NONE Successful
 * @retval #CION_ERROR_PERMISSION_DENIED Permission denied
 * @retval #CION_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #CION_ERROR_ALREADY_IN_PROGRESS Already in progress
 * @retval #CION_ERROR_IO_ERROR IO error
 * @see cion_client_server_discovered_cb()
 * @par Sample code:
 * @code
#include <cion.h>

{
	int ret;

	ret = cion_client_try_discovery(client,
            _cion_client_server_discovered_cb, NULL);
}
 * @endcode
 */
int cion_client_try_discovery(cion_client_h client,
    cion_client_server_discovered_cb cb, void *user_data);

/**
 * @brief Stops to discovery Cion server.
 * @since_tizen 6.5
 * @param[in] client The Cion client handle
 * @return @c 0 on success, otherwise a negative error value
 * @retval #CION_ERROR_NONE Successful
 * @retval #CION_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #CION_ERROR_INVALID_OPERATION Invalid
 * @par Sample code:
 * @code
#include <cion.h>

{
	int ret;

	ret = cion_client_stop_discovery(client);
}
 * @endcode
 */
int cion_client_stop_discovery(cion_client_h client);

/**
 * @brief Tries to connect to the Cion server.
 * @since_tizen 6.5
 * @privlevel public
 * @privilege %http://tizen.org/privilege/d2d.datasharing \n
 *            %http://tizen.org/privilege/internet
 * @param[in] client The Cion client handle
 * @param[in] peer_info The Cion peer information handle
 * @return @c 0 on success, otherwise a negative error value
 * @retval #CION_ERROR_NONE Successful
 * @retval #CION_ERROR_PERMISSION_DENIED Permission denied
 * @retval #CION_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #CION_ERROR_INVALID_OPERATION Invalid operation
 * @retval #CION_ERROR_IO_ERROR IO error
 * @par Sample code:
 * @code
#include <cion.h>

{
	int ret;

	ret = cion_client_connect(client, peer_info);
}
 * @endcode
 */
int cion_client_connect(cion_client_h client, const cion_peer_info_h peer_info);

/**
 * @brief Disconnects from a server.
 * @since_tizen 6.5
 * @param[in] client The Cion client handle
 * @return @c 0 on success, otherwise a negative error value
 * @retval #CION_ERROR_NONE Successful
 * @retval #CION_ERROR_INVALID_PARAMETER Invalid parameter
 * @par Sample code:
 * @code
#include <cion.h>

{
	int ret;

	ret = cion_client_disconnect(client);
}
 * @endcode
 */
int cion_client_disconnect(cion_client_h client);

/**
 * @brief Sends the data.
 * @since_tizen 6.5
 * @remarks @a return_data must be released using free().
 * @param[in] client The Cion client handle
 * @param[in] data The data to send
 * @param[in] data_size The size of data
 * @param[in] timeout The time(milliseconds) to check time out
 * @param[out] return_data The returned data
 * @param[out] return_data_size The size of returned data
 * @return @c 0 on success, otherwise a negative error value
 * @retval #CION_ERROR_NONE Successful
 * @retval #CION_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #CION_ERROR_IO_ERROR IO error
 * @retval #CION_ERROR_TIMED_OUT Timed out
 * @retval #CION_ERROR_INVALID_OPERATION Invalid operation
 * @retval #CION_ERROR_OUT_OF_MEMORY Out of memory
 * @par Sample code:
 * @code
#include <cion.h>

{
	int ret;
    unsigned char *return_data = NULL,
    unsigned int return_size;

	ret = cion_client_send_data(client, "senddata", 8, 10,
            &return_data, &return_size);
}
 * @endcode
 */
int cion_client_send_data(cion_client_h client, unsigned char *data,
    unsigned int data_size, int timeout, unsigned char **return_data,
    unsigned int *return_data_size);

/**
 * @brief Sends the payload asynchronously.
 * @since_tizen 6.5
 * @param[in] client The Cion client handle
 * @param[in] payload The payload handle
 * @param[in] cb The callback function
 * @param[in] user_data The user data
 * @return @c 0 on success, otherwise a negative error value
 * @retval #CION_ERROR_NONE Successful
 * @retval #CION_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #CION_ERROR_INVALID_OPERATION Invalid Operation
 * @see cion_payload_create()
 * @see cion_client_payload_async_result_cb()
 * @par Sample code:
 * @code
#include <cion.h>

{
	int ret;

	ret = cion_client_send_payload_async(client, payload,
            _cion_client_payload_async_result_cb, NULL);
}
 * @endcode
 */
int cion_client_send_payload_async(cion_client_h client,
    cion_payload_h payload, cion_client_payload_async_result_cb cb,
    void *user_data);

/**
 * @brief Adds callback function for connection result.
 * @since_tizen 6.5
 * @param[in] client The Cion client handle
 * @param[in] cb The callback function
 * @param[in] user_data The user data
 * @return @c 0 on success, otherwise a negative error value
 * @retval #CION_ERROR_NONE Successful
 * @retval #CION_ERROR_INVALID_PARAMETER Invalid parameter
 * @see cion_client_connection_result_cb()
 * @see cion_client_remove_connection_result_cb()
 * @par Sample code:
 * @code
#include <cion.h>

{
    int ret;

    ret = cion_client_add_connection_result_cb(client,
            _cion_client_connection_result_cb, NULL);
}
 * @endcode
 */
int cion_client_add_connection_result_cb(cion_client_h client,
    cion_client_connection_result_cb cb, void *user_data);

/**
 * @brief Removes callback function for connection result.
 * @since_tizen 6.5
 * @param[in] client The Cion client handle
 * @param[in] cb The callback function
 * @return @c 0 on success, otherwise a negative error value
 * @retval #CION_ERROR_NONE Successful
 * @retval #CION_ERROR_INVALID_PARAMETER Invalid parameter
 * @see cion_client_connection_result_cb()
 * @see cion_client_add_connection_result_cb()
 * @par Sample code:
 * @code
#include <cion.h>

{
    int ret;

    ret = cion_client_remove_connection_result_cb(client,
            _cion_client_connection_result_cb);
}
 * @endcode
 */
int cion_client_remove_connection_result_cb(cion_client_h client,
    cion_client_connection_result_cb cb);

/**
 * @brief Adds callback function to receive payload.
 * @since_tizen 6.5
 * @param[in] client The Cion client handle
 * @param[in] cb The callback function
 * @param[in] user_data The user data
 * @return @c 0 on success, otherwise a negative error value
 * @retval #CION_ERROR_NONE Successful
 * @retval #CION_ERROR_INVALID_PARAMETER Invalid parameter
 * @see cion_client_payload_received_cb()
 * @see cion_client_remove_payload_received_cb()
 * @par Sample code:
 * @code
#include <cion.h>

{
    int ret;

    ret = cion_client_add_payload_received_cb(client,
            _cion_client_payload_received_cb, NULL);
}
 * @endcode
 */
int cion_client_add_payload_received_cb(cion_client_h client,
    cion_client_payload_received_cb cb, void *user_data);

/**
 * @brief Removes callback function to receive payload.
 * @since_tizen 6.5
 * @param[in] client The Cion client handle
 * @param[in] cb The callback function
 * @return @c 0 on success, otherwise a negative error value
 * @retval #CION_ERROR_NONE Successful
 * @retval #CION_ERROR_INVALID_PARAMETER Invalid parameter
 * @see cion_client_payload_received_cb()
 * @see cion_client_add_payload_received_cb()
 * @par Sample code:
 * @code
#include <cion.h>

{
    int ret;

    ret = cion_client_remove_payload_received_cb(client,
            _cion_client_payload_received_cb);
}
 * @endcode
 */
int cion_client_remove_payload_received_cb(cion_client_h client,
    cion_client_payload_received_cb cb);

/**
 * @brief Adds callback function for disconnection event.
 * @since_tizen 6.5
 * @param[in] client The Cion client handle
 * @param[in] cb The callback function
 * @param[in] user_data The user data
 * @return @c 0 on success, otherwise a negative error value
 * @retval #CION_ERROR_NONE Successful
 * @retval #CION_ERROR_INVALID_PARAMETER Invalid parameter
 * @see cion_client_disconnected_cb()
 * @see cion_client_remove_disconnected_cb()
 * @par Sample code:
 * @code
#include <cion.h>

{
    int ret;

    ret = cion_client_add_disconnected_cb(client,
            _cion_client_disconnected_cb, NULL);
}
 * @endcode
 */
int cion_client_add_disconnected_cb(cion_client_h client,
    cion_client_disconnected_cb cb, void *user_data);

/**
 * @brief Removes callback function for disconnection event.
 * @since_tizen 6.5
 * @param[in] client The Cion client handle
 * @param[in] cb The callback function
 * @return @c 0 on success, otherwise a negative error value
 * @retval #CION_ERROR_NONE Successful
 * @retval #CION_ERROR_INVALID_PARAMETER Invalid parameter
 * @see cion_client_disconnected_cb()
 * @see cion_client_add_disconnected_cb()
 * @par Sample code:
 * @code
#include <cion.h>

{
    int ret;

    ret = cion_client_remove_disconnected_cb(client,
            _cion_client_disconnected_cb);
}
 * @endcode
 */
int cion_client_remove_disconnected_cb(cion_client_h client,
    cion_client_disconnected_cb cb);

/**
 * @}
 */

#ifdef __cplusplus
}
#endif

#endif  /* __TIZEN_APPFW_CION_CLIENT_H__ */
