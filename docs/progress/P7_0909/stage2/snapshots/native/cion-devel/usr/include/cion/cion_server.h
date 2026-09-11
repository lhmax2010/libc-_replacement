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

#ifndef __TIZEN_APPFW_CION_SERVER_H__
#define __TIZEN_APPFW_CION_SERVER_H__

#include <cion_payload.h>
#include <cion_peer_info.h>
#include <cion_security.h>
#include <cion_connection_result.h>
#include <cion_payload_async_result.h>

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @addtogroup CAPI_CION_SERVER_MODULE
 * @{
 */

/**
 * @brief The Cion server handle.
 * @since_tizen 6.5
 */
typedef void *cion_server_h;

/**
 * @brief Called to get the connected peer information.
 * @since_tizen 6.5
 * @param[in] peer_info The Cion peer information handle \n
 *            The @a peer_info can be used only in the callback. To use outside, make a copy.
 * @param[in] user_data The user data
 * @return @c true to continue with the next iteration of the loop, otherwise @c false to break out of the loop.
 * @see cion_server_foreach_connected_peer_info()
 * @see #cion_peer_info_h
 */
typedef bool (*cion_server_connected_peer_info_cb)(
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
 * @see cion_server_add_connection_result_cb()
 * @see #cion_peer_info_h
 * @see #cion_connection_result_h
 */
typedef void (*cion_server_connection_result_cb)(
    const char *service_name, const cion_peer_info_h peer_info,
    const cion_connection_result_h result, void *user_data);

/**
 * @brief Called after sending payload asynchronously.
 * @since_tizen 6.5
 * @param[in] result The result of sending payload \n
 *            The @a result can be used only in the callback. To use outside, make a copy.
 * @param[in] user_data The user data
 * @see cion_server_send_payload_async()
 * @see #cion_payload_async_result_h
 */
typedef void (*cion_server_payload_async_result_cb)(
    const cion_payload_async_result_h result, void *user_data);

/**
 * @brief Called when data is received.
 * @since_tizen 6.5
 * @remarks The server should return reply by setting @a return_data and @a return_data_size.
 * @param[in] service_name The name of service \n
 *            The @a service_name can be used only in the callback. To use outside, make a copy.
 * @param[in] peer_info The Cion peer information handle \n
 *            The @a peer_info can be used only in the callback. To use outside, make a copy.
 * @param[in] data The data \n
 *            The @a peer_info can be used only in the callback. To use outside, make a copy.
 * @param[in] data_size The size of data
 * @param[out] return_data The data to return
 * @param[out] return_data_size The size of return data
 * @param[in] user_data The user data
 * @see cion_server_set_data_received_cb()
 * @see #cion_peer_info_h
 */
typedef void (*cion_server_data_received_cb)(const char *service_name,
    const cion_peer_info_h peer_info, const unsigned char *data,
    unsigned int data_size, unsigned char **return_data,
    unsigned int *return_data_size, void *user_data);

/**
 * @brief Called when payload is received.
 * @since_tizen 6.5
 * @param[in] service_name The name of service \n
 *            The @a service_name can be used only in the callback. To use outside, make a copy.
 * @param[in] peer_info The Cion peer information handle \n
 *            The @a peer_info can be used only in the callback. To use outside, make a copy.
 * @param[in] payload The received payload \n
 *            The @a payload can be used only in the callback. \n
 *            The @a payload should not be released.
 * @param[in] status The status of transfer
 * @param[in] user_data The user data
 * @see cion_server_add_payload_received_cb()
 * @see #cion_peer_info_h
 * @see #cion_payload_h
 * @see #cion_payload_transfer_status_e
 */
typedef void (*cion_server_payload_received_cb)(const char *service_name,
    const cion_peer_info_h peer_info, const cion_payload_h payload,
    cion_payload_transfer_status_e status,
    void *user_data);

/**
 * @brief Called when a connection is requested.
 * @since_tizen 6.5
 * @param[in] service_name The name of service \n
 *            The @a service_name can be used only in the callback. To use outside, make a copy.
 * @param[in] peer_info The Cion peer information handle \n
 *            The @a peer_info can be used only in the callback. To use outside, make a copy.
 * @param[in] user_data The user data
 * @see cion_server_listen()
 * @see #cion_peer_info_h
 */
typedef void (*cion_server_connection_request_cb)(const char *service_name,
    const cion_peer_info_h peer_info, void *user_data);

/**
 * @brief Called when a peer disconnects.
 * @since_tizen 6.5
 * @param[in] service_name The name of service \n
 *            The @a service_name can be used only in the callback. To use outside, make a copy.
 * @param[in] peer_info The Cion peer information handle \n
 *            The @a peer_info can be used only in the callback. To use outside, make a copy.
 * @param[in] user_data The user data
 * @see cion_server_add_disconnected_cb()
 * @see #cion_peer_info_h
 */
typedef void (*cion_server_disconnected_cb)(const char *service_name,
    const cion_peer_info_h peer_info, void *user_data);

/**
 * @brief Creates the Cion server handle.
 * @details The Cion server listens client requests.
 * @since_tizen 6.5
 * @remarks @a server must be released using cion_server_destroy().
 * @remarks Max length of @a service_name including the null terminator is 512.
 * @param[out] server The Cion server handle
 * @param[in] service_name The name of service
 * @param[in] display_name The display name of service
 * @param[in] security The Cion security handle
 * @return @c 0 on success, otherwise a negative error value
 * @retval #CION_ERROR_NONE Successful
 * @retval #CION_ERROR_NOT_SUPPORTED Not supported
 * @retval #CION_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #CION_ERROR_OUT_OF_MEMORY Out of memory
 * @retval #CION_ERROR_IO_ERROR IO error
 * @retval #CION_ERROR_INVALID_OPERATION Invalid operation
 * @retval #CION_ERROR_OPERATION_FAILED Operation failed
 * @see cion_server_destroy()
 * @see cion_security_create()
 * @par Sample code:
 * @code
#include <cion.h>

{
	int ret;

	cion_server_h server = NULL;

	ret = cion_server_create(&server, "myservice", "mydisplayname", NULL);
}
 * @endcode
 */
int cion_server_create(cion_server_h *server, const char *service_name,
    const char *display_name, cion_security_h security);

/**
 * @brief Destroys the Cion server handle.
 * @since_tizen 6.5
 * @param[in] server The Cion server handle.
 * @return @c 0 on success, otherwise a negative error value
 * @retval #CION_ERROR_NONE Successful
 * @retval #CION_ERROR_INVALID_PARAMETER Invalid parameter
 * @see cion_server_create()
 * @par Sample code:
 * @code
#include <cion.h>
{
    int ret;

    ret = cion_server_destroy(server);
}

 * @endcode
 */
int cion_server_destroy(cion_server_h server);

/**
 * @brief Starts listening to connection requests from clients.
 * @since_tizen 6.5
 * @privlevel public
 * @privilege %http://tizen.org/privilege/d2d.datasharing \n
 *            %http://tizen.org/privilege/internet
 * @param[in] server The Cion server handle
 * @param[in] cb The callback function
 * @param[in] user_data The user data
 * @return @c 0 on success, otherwise a negative error value
 * @retval #CION_ERROR_NONE Successful
 * @retval #CION_ERROR_PERMISSION_DENIED Permission denied
 * @retval #CION_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #CION_ERROR_ALREADY_IN_PROGRESS Already in progress
 * @see cion_server_connection_request_cb()
 * @par Sample code:
 * @code
#include <cion.h>

{
    int ret;

    ret = cion_server_listen(server, _cion_server_connection_request_cb, NULL);
}
 * @endcode
 */
int cion_server_listen(cion_server_h server,
    cion_server_connection_request_cb cb, void *user_data);

/**
 * @brief Stops listening to connection requests from clients.
 * @since_tizen 6.5
 * @param[in] server The Cion server handle
 * @return @c 0 on success, otherwise a negative error value
 * @retval #CION_ERROR_NONE Successful
 * @retval #CION_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #CION_ERROR_INVALID_OPERATION Invalid operation
 * @see cion_server_listen()
 * @par Sample code:
 * @code
#include <cion.h>

{
    int ret;

    ret = cion_server_stop(server);
}
 * @endcode
 */
int cion_server_stop(cion_server_h server);

/**
 * @brief Disconnects from a client.
 * @since_tizen 6.5
 * @param[in] server The Cion server handle
 * @param[in] peer_info The Cion peer information handle
 * @return @c 0 on success, otherwise a negative error value
 * @retval #CION_ERROR_NONE Successful
 * @retval #CION_ERROR_INVALID_PARAMETER Invalid parameter
 * @par Sample code:
 * @code
#include <cion.h>

{
    int ret;

    ret = cion_server_disconnect(server, peer_info);
}
 * @endcode
 */
int cion_server_disconnect(cion_server_h server, cion_peer_info_h peer_info);

/**
 * @brief Sends payload asynchronously.
 * @since_tizen 6.5
 * @param[in] server The Cion server handle
 * @param[in] peer_info The Cion peer information handle
 * @param[in] payload The payload to send
 * @param[in] cb The callback function for result
 * @param[in] user_data The user data
 * @return @c 0 on success, otherwise a negative error value
 * @retval #CION_ERROR_NONE Successful
 * @retval #CION_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #CION_ERROR_INVALID_OPERATION Invalid operation
 * @see cion_payload_create()
 * @see cion_server_payload_async_result_cb()
 * @par Sample code:
 * @code
#include <cion.h>

{
    int ret;

    ret = cion_server_send_payload_async(server, peer_info, payload,
            _cion_server_payload_async_result_cb, NULL);
}
 * @endcode
 */
int cion_server_send_payload_async(cion_server_h server,
    cion_peer_info_h peer_info, cion_payload_h payload,
    cion_server_payload_async_result_cb cb,
    void *user_data);

/**
 * @brief Gets connected peers information.
 * @since_tizen 6.5
 * @param[in] server The Cion server handle
 * @param[in] callback The callback function
 * @param[in] user_data The user data
 * @return @c 0 on success, otherwise a negative error value
 * @retval #CION_ERROR_NONE Successful
 * @retval #CION_ERROR_INVALID_PARAMETER Invalid parameter
 * @see cion_server_connected_peer_info_cb()
 * @par Sample code:
 * @code
#include <cion.h>

{
    int ret;

    ret = cion_server_foreach_connected_peer_info(server,
        _cion_server_connected_peer_info_cb, NULL);
}

 * @endcode
 */
int cion_server_foreach_connected_peer_info(cion_server_h server,
    cion_server_connected_peer_info_cb callback, void *user_data);

/**
 * @brief Adds callback function for connection result.
 * @since_tizen 6.5
 * @param[in] server The Cion server handle.
 * @param[in] cb The callback function
 * @param[in] user_data The user data
 * @return @c 0 on success, otherwise a negative error value
 * @retval #CION_ERROR_NONE Successful
 * @retval #CION_ERROR_INVALID_PARAMETER Invalid parameter
 * @see cion_server_connection_result_cb()
 * @see cion_server_remove_connection_result_cb()
 * @par Sample code:
 * @code
#include <cion.h>

{
    int ret;

    ret = cion_server_add_connection_result_cb(server,
            _cion_server_connection_result_cb, NULL);
}

 * @endcode
 */
int cion_server_add_connection_result_cb(cion_server_h server,
    cion_server_connection_result_cb cb,  void *user_data);

/**
 * @brief Removes callback function for connection result.
 * @since_tizen 6.5
 * @param[in] server The Cion server handle.
 * @param[in] cb The callback function
 * @return @c 0 on success, otherwise a negative error value
 * @retval #CION_ERROR_NONE Successful
 * @retval #CION_ERROR_INVALID_PARAMETER Invalid parameter
 * @see cion_server_connection_result_cb()
 * @see cion_server_add_connection_result_cb()
 * @par Sample code:
 * @code
#include <cion.h>

{
    int ret;

    ret = cion_server_remove_connection_result_cb(server,
            _cion_server_connection_result_cb);
}

 * @endcode
 */
int cion_server_remove_connection_result_cb(cion_server_h server,
    cion_server_connection_result_cb cb);

/**
 * @brief Adds callback function to receive payload.
 * @since_tizen 6.5
 * @param[in] server The Cion server
 * @param[in] cb The callback function
 * @param[in] user_data The user data
 * @return @c 0 on success, otherwise a negative error value
 * @retval #CION_ERROR_NONE Successful
 * @retval #CION_ERROR_INVALID_PARAMETER Invalid parameter
 * @see cion_server_payload_received_cb()
 * @see cion_server_remove_payload_received_cb()
 * @par Sample code:
 * @code
#include <cion.h>

{
    int ret;

    ret = cion_server_add_payload_received_cb(server,
            _cion_server_payload_received_cb, NULL);
}
 * @endcode
 */
int cion_server_add_payload_received_cb(cion_server_h server,
    cion_server_payload_received_cb cb, void *user_data);

/**
 * @brief Removes callback function to receive payload.
 * @since_tizen 6.5
 * @param[in] server The Cion server
 * @param[in] cb The callback function
 * @return @c 0 on success, otherwise a negative error value
 * @retval #CION_ERROR_NONE Successful
 * @retval #CION_ERROR_INVALID_PARAMETER Invalid parameter
 * @see cion_server_payload_received_cb()
 * @see cion_server_add_payload_received_cb()
 * @par Sample code:
 * @code
#include <cion.h>

{
    int ret;

    ret = cion_server_remove_payload_received_cb(server,
            _cion_server_payload_received_cb);
}
 * @endcode
 */
int cion_server_remove_payload_received_cb(cion_server_h server,
    cion_server_payload_received_cb cb);

/**
 * @brief Sets callback function to receive data.
 * @since_tizen 6.5
 * @param[in] server The Cion server handle
 * @param[in] cb The callback function
 * @param[in] user_data The user data
 * @return @c 0 on success, otherwise a negative error value
 * @retval #CION_ERROR_NONE Successful
 * @retval #CION_ERROR_INVALID_PARAMETER Invalid parameter
 * @see cion_server_data_received_cb()
 * @see cion_server_unset_data_received_cb()
 * @par Sample code:
 * @code
#include <cion.h>

{
    int ret;

    ret = cion_server_set_data_received_cb(server,
            _cion_server_data_received_cb, NULL);
}

 * @endcode
 */
int cion_server_set_data_received_cb(cion_server_h server,
    cion_server_data_received_cb cb, void *user_data);

/**
 * @brief Unsets callback function to receive data.
 * @since_tizen 6.5
 * @param[in] server The Cion server handle
 * @return @c 0 on success, otherwise a negative error value
 * @retval #CION_ERROR_NONE Successful
 * @retval #CION_ERROR_INVALID_PARAMETER Invalid parameter
 * @see cion_server_data_received_cb()
 * @see cion_server_set_data_received_cb()
 * @par Sample code:
 * @code
#include <cion.h>

{
    int ret;

    ret = cion_server_unset_data_received_cb(server);
}

 * @endcode
 */
int cion_server_unset_data_received_cb(cion_server_h server);

/**
 * @brief Adds callback function for disconnection event.
 * @since_tizen 6.5
 * @param[in] server The Cion server handle
 * @param[in] cb The callback function
 * @param[in] user_data The user data
 * @return @c 0 on success, otherwise a negative error value
 * @retval #CION_ERROR_NONE Successful
 * @retval #CION_ERROR_INVALID_PARAMETER Invalid parameter
 * @see cion_server_disconnected_cb()
 * @see cion_server_remove_disconnected_cb()
 * @par Sample code:
 * @code
#include <cion.h>

{
    int ret;

    ret = cion_server_add_disconnected_cb(server,
            _cion_server_disconnected_cb, NULL);
}
 * @endcode
 */
int cion_server_add_disconnected_cb(cion_server_h server,
    cion_server_disconnected_cb cb, void *user_data);

/**
 * @brief Removes callback function for disconnection event.
 * @since_tizen 6.5
 * @param[in] server The Cion server handle
 * @param[in] cb The callback function
 * @return @c 0 on success, otherwise a negative error value
 * @retval #CION_ERROR_NONE Successful
 * @retval #CION_ERROR_INVALID_PARAMETER Invalid parameter
 * @see cion_server_disconnected_cb()
 * @see cion_server_add_disconnected_cb()
 * @par Sample code:
 * @code
#include <cion.h>

{
    int ret;

    ret = cion_server_remove_disconnected_cb(server,
            _cion_server_disconnected_cb);
}
 * @endcode
 */
int cion_server_remove_disconnected_cb(cion_server_h server,
    cion_server_disconnected_cb cb);

/**
 * @brief Accepts the connection request from a peer.
 * @since_tizen 6.5
 * @param[in] server The Cion server handle
 * @param[in] peer_info The Cion peer information handle
 * @return @c 0 on success, otherwise a negative error value
 * @retval #CION_ERROR_NONE Successful
 * @retval #CION_ERROR_INVALID_PARAMETER Invalid parameter
 * @par Sample code:
 * @code
#include <cion.h>

{
    int ret;

    ret = cion_server_accept(server, peer_info);
}
 * @endcode
 */
int cion_server_accept(cion_server_h server, const cion_peer_info_h peer_info);

/**
 * @brief Rejects the connection request from a peer.
 * @since_tizen 6.5
 * @param[in] server The Cion server handle
 * @param[in] peer_info The Cion peer information handle
 * @param[in] reason The reason of reject
 * @return @c 0 on success, otherwise a negative error value
 * @retval #CION_ERROR_NONE Successful
 * @retval #CION_ERROR_INVALID_PARAMETER Invalid parameter
 * @par Sample code:
 * @code
#include <cion.h>

{
    int ret;

    ret = cion_server_reject(server, peer_info, "NOT_ALLOWED");
}
 * @endcode
 */
int cion_server_reject(cion_server_h server, const cion_peer_info_h peer_info,
        const char *reason);

/**
 * @brief Sets the server display name.
 * @details The display name is a customized name for server.
 * @since_tizen 6.5
 * @param[in] server The Cion server handle
 * @param[in] display_name The display name
 * @return @c 0 on success, otherwise a negative error value
 * @retval #CION_ERROR_NONE Successful
 * @retval #CION_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #CION_ERROR_OUT_OF_MEMORY Out of memory
 * @retval #CION_ERROR_IO_ERROR IO error
 * @par Sample code:
 * @code
#include <cion.h>

{
    int ret;

    ret = cion_server_set_display_name(server, "MyDPName");
}
 * @endcode
 */
int cion_server_set_display_name(cion_server_h server,
        const char *display_name);

 /**
 * @brief Sets on-demand launch state.
 * @details If the on-demand launch state is enabled, the server is launched on demand
 *          when the request comes.
 * @since_tizen 6.5
 * @privlevel public
 * @privilege %http://tizen.org/privilege/d2d.remotelaunch
 * @param[in] server The Cion server handle
 * @param[in] enabled The enable state
 * @return @c 0 on success, otherwise a negative error value
 * @retval #CION_ERROR_NONE Successful
 * @retval #CION_ERROR_PERMISSION_DENIED Permission denied
 * @retval #CION_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #CION_ERROR_OUT_OF_MEMORY Out of memory
 * @retval #CION_ERROR_IO_ERROR IO error
 * @par Sample code:
 * @code
#include <cion.h>

{
    int ret;

    ret = cion_server_set_on_demand_launch_enabled(server, true);
}

 * @endcode
 */
int cion_server_set_on_demand_launch_enabled(cion_server_h server, bool enabled);

/**
 * @}
 */

#ifdef __cplusplus
}
#endif

#endif  /*__TIZEN_APPFW_CION_SERVER_H__ */
