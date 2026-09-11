/*
 * Copyright (c) 2018 - 2021 Samsung Electronics Co., Ltd All Rights Reserved
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

#ifndef __TIZEN_APPFW_RPC_PORT_INCLUDE_H__
#define __TIZEN_APPFW_RPC_PORT_INCLUDE_H__

#ifdef __cplusplus
extern "C" {
#endif

#include <stdbool.h>
#include <tizen_error.h>

/**
 * @addtogroup CAPI_RPC_PORT_MODULE
 * @{
 */


/**
 * @brief Enumeration for error codes of a rpc port.
 * @since_tizen 4.0
 */
typedef enum {
	RPC_PORT_ERROR_NONE = TIZEN_ERROR_NONE, /**< Successful */
	RPC_PORT_ERROR_IO_ERROR = TIZEN_ERROR_IO_ERROR, /**< Internal I/O error */
	RPC_PORT_ERROR_OUT_OF_MEMORY = TIZEN_ERROR_OUT_OF_MEMORY, /**< Out of memory */
	RPC_PORT_ERROR_INVALID_PARAMETER = TIZEN_ERROR_INVALID_PARAMETER, /**< Invalid parameter */
	RPC_PORT_ERROR_PERMISSION_DENIED = TIZEN_ERROR_PERMISSION_DENIED, /**< Permission denied */
} rpc_port_error_e;

/**
 * @brief Enumeration for types of communication channels.
 * @since_tizen 4.0
 */
typedef enum {
	RPC_PORT_PORT_MAIN, /**< Main channel */
	RPC_PORT_PORT_CALLBACK, /**< The channel for callbacks */
} rpc_port_port_type_e;

/* Common */

/**
 * @brief The rpc port handle.
 * @since_tizen 4.0
 */
typedef void *rpc_port_h;

/**
 * @brief Reads data from an RPC port.
 * @since_tizen 4.0
 *
 * @param[in] h The rpc port handle
 * @param[out] buf Buffer for reading data
 * @param[in] size Size for reading data
 *
 * @return @c 0 on success,
 *          otherwise a negative error value
 * @retval #RPC_PORT_ERROR_NONE Successful
 * @retval #RPC_PORT_ERROR_INVALID_PARAMETER The specified @a h is NULL
 * @retval #RPC_PORT_ERROR_IO_ERROR Internal I/O error
 * @see rpc_port_write()
 */
int rpc_port_read(rpc_port_h h, void *buf, unsigned int size);

/**
 * @brief Writes data to an RPC port.
 * @since_tizen 4.0
 *
 * @param[in] h The rpc port handle
 * @param[in] buf Buffer for writing data
 * @param[in] size Size for writing data
 *
 * @return @c 0 on success,
 *          otherwise a negative error value
 * @retval #RPC_PORT_ERROR_NONE Successful
 * @retval #RPC_PORT_ERROR_INVALID_PARAMETER The specified @a h is NULL
 * @retval #RPC_PORT_ERROR_IO_ERROR Internal I/O error
 * @see rpc_port_read()
 */
int rpc_port_write(rpc_port_h h, const void *buf, unsigned int size);


/* Proxy */

/**
 * @brief Called when the proxy is connected.
 * @details The function is called when the proxy is connected with stub by port.
 * @since_tizen 4.0
 * @param[in] receiver The target stub app id
 * @param[in] port_name The name of the port
 * @param[in] port The rpc port handle for reading and writing
 * @param[in] user_data The user data passed from the register function
 */
typedef void (*rpc_port_proxy_connected_event_cb)(const char *receiver,
		const char *port_name, rpc_port_h port, void *user_data);

/**
 * @brief Called when the proxy is disconnected.
 * @details The function is called when the proxy is disconnected from stub.
 * @since_tizen 4.0
 * @param[in] receiver The target stub app id
 * @param[in] port_name The name of the port
 * @param[in] user_data The user data passed from the register function
 */
typedef void (*rpc_port_proxy_disconnected_event_cb)(const char *receiver,
		const char *port_name, void *user_data);

/**
 * @brief Called when the proxy is rejected.
 * @details The function is called when the proxy is rejected to connect stub.
 * @since_tizen 4.0
 * @param[in] receiver The target stub app id
 * @param[in] port_name The name of the port
 * @param[in] user_data The user data passed from the register function
 */
typedef void (*rpc_port_proxy_rejected_event_cb)(const char *receiver,
		const char *port_name, void *user_data);

/**
 * @brief Called when the proxy received data.
 * @details The function is called when the proxy received data from stub.
 * @since_tizen 4.0
 * @param[in] receiver The target stub app id
 * @param[in] port_name The name of the port
 * @param[in] user_data The user data passed from the register function
 */
typedef void (*rpc_port_proxy_received_event_cb)(const char *receiver,
		const char *port_name, void *user_data);

/**
 * @brief The rpc port proxy handle.
 * @since_tizen 4.0
 */
typedef void *rpc_port_proxy_h;

/**
 * @brief Creates a rpc port proxy handle.
 * @since_tizen 4.0
 * @remarks You must release @a h using rpc_port_proxy_destroy().
 * @param[out] h The rpc port proxy handle that is newly created
 * @return @c 0 on success,
 *         otherwise a negative error value
 * @retval #RPC_PORT_ERROR_NONE Successful
 * @retval #RPC_PORT_ERROR_INVALID_PARAMETER The specified @a h is NULL
 * @see rpc_port_proxy_destroy()
 */
int rpc_port_proxy_create(rpc_port_proxy_h *h);

/**
 * @brief Destroys a rpc port proxy handle.
 * @since_tizen 4.0
 * @param[in] h The rpc port proxy handle
 * @return @c 0 on success,
 *         otherwise a negative error value
 * @retval #RPC_PORT_ERROR_NONE Successful
 * @retval #RPC_PORT_ERROR_INVALID_PARAMETER The specified @a h is NULL
 * @see rpc_port_proxy_create()
 */
int rpc_port_proxy_destroy(rpc_port_proxy_h h);

/**
 * @brief Connects to @a port of @a appid.
 * @details To send and receive data, the proxy should connect to port of stub
 * @since_tizen 4.0
 * @privlevel public
 * @privilege %http://tizen.org/privilege/appmanager.launch \n
 *            %http://tizen.org/privilege/datasharing
 * @remarks If you want to use this function, you must add privileges.
 * @param[in] h The rpc port proxy handle
 * @param[in] appid The target stub appid
 * @param[in] port The name of rpc port
 * @return @c 0 on success,
 *         otherwise a negative error value
 * @retval #RPC_PORT_ERROR_NONE Successful
 * @retval #RPC_PORT_ERROR_PERMISSION_DENIED Permission denied
 * @retval #RPC_PORT_ERROR_INVALID_PARAMETER The specified @a h is NULL
 * @retval #RPC_PORT_ERROR_IO_ERROR Internal I/O error
 */
int rpc_port_proxy_connect(rpc_port_proxy_h h, const char *appid,
		const char *port);

/**
 * @brief Adds a proxy connected callback.
 * @since_tizen 4.0
 * @param[in] h The rpc port proxy handle
 * @param[in] cb The callback function to be called when proxy is connected
 * @param[in] user_data The user data to be passed to
 *                 the rpc_port_proxy_connected_event_cb() function
 * @return @c 0 on success,
 *         otherwise a negative error value
 * @retval #RPC_PORT_ERROR_NONE Successful
 * @retval #RPC_PORT_ERROR_INVALID_PARAMETER The specified @a h is NULL
 */
int rpc_port_proxy_add_connected_event_cb(rpc_port_proxy_h h,
		rpc_port_proxy_connected_event_cb cb, void *user_data);

/**
 * @brief Adds a proxy disconnected callback.
 * @since_tizen 4.0
 * @param[in] h The rpc port proxy handle
 * @param[in] cb The callback function to be called when proxy is disconnected
 * @param[in] user_data The user data to be passed to
 *                 the rpc_port_proxy_disconnected_event_cb() function
 * @return @c 0 on success,
 *         otherwise a negative error value
 * @retval #RPC_PORT_ERROR_NONE Successful
 * @retval #RPC_PORT_ERROR_INVALID_PARAMETER The specified @a h is NULL
 */
int rpc_port_proxy_add_disconnected_event_cb(rpc_port_proxy_h h,
		rpc_port_proxy_disconnected_event_cb cb, void *user_data);

/**
 * @brief Adds a proxy rejected callback.
 * @since_tizen 4.0
 * @param[in] h The rpc port proxy handle
 * @param[in] cb The callback function to be called when proxy is rejected
 * @param[in] user_data The user data to be passed to
 *                 the rpc_port_proxy_rejected_event_cb() function
 * @return @c 0 on success,
 *         otherwise a negative error value
 * @retval #RPC_PORT_ERROR_NONE Successful
 * @retval #RPC_PORT_ERROR_INVALID_PARAMETER The specified @a h is NULL
 */
int rpc_port_proxy_add_rejected_event_cb(rpc_port_proxy_h h,
		rpc_port_proxy_rejected_event_cb cb, void *user_data);

/**
 * @brief Adds a proxy received callback.
 * @since_tizen 4.0
 * @param[in] h The rpc port proxy handle
 * @param[in] cb The callback function to be called when proxy received data
 * @param[in] user_data The user data to be passed to
 *                 the rpc_port_proxy_received_event_cb() function
 * @return @c 0 on success,
 *         otherwise a negative error value
 * @retval #RPC_PORT_ERROR_NONE Successful
 * @retval #RPC_PORT_ERROR_INVALID_PARAMETER The specified @a h is NULL
 */
int rpc_port_proxy_add_received_event_cb(rpc_port_proxy_h h,
		rpc_port_proxy_received_event_cb cb, void *user_data);

/**
 * @brief Gets a port from proxy handle.
 * @since_tizen 4.0
 * @remarks This handle @a port will not be valid if the proxy was disconnected or destroyed.
 * @param[in] h The rpc port proxy handle
 * @param[in] type The type of port
 * @param[out] port The port to communicate
 * @return @c 0 on success,
 *         otherwise a negative error value
 * @retval #RPC_PORT_ERROR_NONE Successful
 * @retval #RPC_PORT_ERROR_INVALID_PARAMETER The specified @a h is NULL
 * @retval #RPC_PORT_ERROR_IO_ERROR No available ports
 * @see rpc_port_write()
 * @see rpc_port_read()
 * @see rpc_port_parcel_create_from_port()
 * @see rpc_port_parcel_send();
 */
int rpc_port_proxy_get_port(rpc_port_proxy_h h, rpc_port_port_type_e type,
		rpc_port_h *port);

/* Stub */

/**
 * @brief The rpc port stub handle.
 * @since_tizen 4.0
 */
typedef void *rpc_port_stub_h;

/**
 * @brief Called when the proxy is connected with stub.
 * @details The function is called when the proxy is connected with stub.
 *          When a proxy connects to stub several times with new port,
 *          you can handle each request by using @a instance.
 * @since_tizen 4.0
 * @param[in] sender The target proxy app id
 * @param[in] instance The information of the request
 * @param[in] user_data The user data passed from the register function
 */
typedef void (*rpc_port_stub_connected_event_cb)(const char *sender,
		const char *instance, void *user_data);

/**
 * @brief Called when the proxy is disconnected from stub.
 * @details The function is called when the proxy is disconnected from stub.
 *          When a proxy is disconnected, you can check the request
 *          by using @a instance.
 * @since_tizen 4.0
 * @param[in] sender The target proxy app id
 * @param[in] instance The information of the request
 * @param[in] user_data The user data passed from the register function
 */
typedef void (*rpc_port_stub_disconnected_event_cb)(const char *sender,
		const char *instance, void *user_data);

/**
 * @brief Called when the stub received data from proxy.
 * @details The function is called when the stub received data from stub.
 *          When a stub received data from several ports, you can handle
 *          each request by using @a instance. If the function returns non zero
 *          value, the stub is disconnected.
 * @since_tizen 4.0
 * @param[in] sender The target proxy app id
 * @param[in] instance The information of the request
 * @param[in] port The rpc port handle for reading and writing
 * @param[in] user_data The user data passed from the register function
 * @return @c zero to continue receive data with the sender,
 *         otherwise @c nonzero to disconnect from the port
 */
typedef int (*rpc_port_stub_received_event_cb)(const char *sender,
		const char *instance, rpc_port_h port, void *user_data);

/**
 * @brief Creates a rpc port stub handle.
 * @since_tizen 4.0
 * @remarks You must release @a h using rpc_port_stub_destroy().
 * @param[out] h The rpc port stub handle that is newly created
 * @param[in] port_name The name of the port which want to listen
 * @return @c 0 on success,
 *         otherwise a negative error value
 * @retval #RPC_PORT_ERROR_NONE Successful
 * @retval #RPC_PORT_ERROR_INVALID_PARAMETER The specified @a h is NULL
 * @see rpc_port_stub_destroy()
 */
int rpc_port_stub_create(rpc_port_stub_h *h, const char *port_name);

/**
 * @brief Destroys a rpc port stub handle.
 * @since_tizen 4.0
 * @param[in] h The rpc port stub handle
 * @return @c 0 on success,
 *         otherwise a negative error value
 * @retval #RPC_PORT_ERROR_NONE Successful
 * @retval #RPC_PORT_ERROR_INVALID_PARAMETER The specified @a h is NULL
 * @see rpc_port_stub_create()
 */
int rpc_port_stub_destroy(rpc_port_stub_h h);

/**
 * @brief Listens to the requests for connections.
 * @details The stub listens requests to connect by port
 * @since_tizen 4.0
 * @param[in] h The rpc port stub handle
 * @return @c 0 on success,
 *         otherwise a negative error value
 * @retval #RPC_PORT_ERROR_NONE Successful
 * @retval #RPC_PORT_ERROR_INVALID_PARAMETER The specified @a h is NULL
 * @retval #RPC_PORT_ERROR_IO_ERROR Internal I/O error
 */
int rpc_port_stub_listen(rpc_port_stub_h h);

/**
 * @brief Adds a privilege to the stub.
 * @details The stub can control access to the port using tizen privilege.
 *          It allows connections only if the proxy which have the privileges.
 * @since_tizen 4.0
 * @param[in] h The rpc port stub handle
 * @param[in] privilege The privilege to access this stub
 * @return @c 0 on success,
 *         otherwise a negative error value
 * @retval #RPC_PORT_ERROR_NONE Successful
 * @retval #RPC_PORT_ERROR_INVALID_PARAMETER The specified @a h is NULL
 */
int rpc_port_stub_add_privilege(rpc_port_stub_h h, const char *privilege);

/**
 * @brief Sets trusted to the stub.
 * @details The stub can control access to the port using tizen certificate.
 *          It allows connections only if the proxy is signed with the same
 *          certificate.
 * @since_tizen 4.0
 * @param[in] h The rpc port stub handle
 * @param[in] trusted Whether stub allows only trusted proxy or not
 * @return @c 0 on success,
 *         otherwise a negative error value
 * @retval #RPC_PORT_ERROR_NONE Successful
 * @retval #RPC_PORT_ERROR_INVALID_PARAMETER The specified @a h is NULL
 */
int rpc_port_stub_set_trusted(rpc_port_stub_h h, const bool trusted);

/**
 * @brief Adds a stub connected callback.
 * @since_tizen 4.0
 * @param[in] h The rpc stub stub handle
 * @param[in] cb The callback function to be called when proxy is connected
 *               with the stub
 * @param[in] user_data The user data to be passed to
 *                 the rpc_port_stub_connected_event_cb() function
 * @return @c 0 on success,
 *         otherwise a negative error value
 * @retval #RPC_PORT_ERROR_NONE Successful
 * @retval #RPC_PORT_ERROR_INVALID_PARAMETER The specified @a h is NULL
 */
int rpc_port_stub_add_connected_event_cb(rpc_port_stub_h h,
		rpc_port_stub_connected_event_cb cb, void *user_data);

/**
 * @brief Adds a stub disconnected callback.
 * @since_tizen 4.0
 * @param[in] h The rpc port stub handle
 * @param[in] cb The callback function to be called when proxy is disconnected
 *               with the stub
 * @param[in] user_data The user data to be passed to
 *                 the rpc_port_stub_disconnected_event_cb() function
 * @return @c 0 on success,
 *         otherwise a negative error value
 * @retval #RPC_PORT_ERROR_NONE Successful
 * @retval #RPC_PORT_ERROR_INVALID_PARAMETER The specified @a h is NULL
 */
int rpc_port_stub_add_disconnected_event_cb(rpc_port_stub_h h,
		rpc_port_stub_disconnected_event_cb cb, void *user_data);

/**
 * @brief Adds a stub received callback.
 * @since_tizen 4.0
 * @param[in] h The rpc port stub handle
 * @param[in] cb The callback function to be called when stub received data
 * @param[in] user_data The user data to be passed to
 *                 the rpc_port_stub_received_event_cb() function
 * @return @c 0 on success,
 *         otherwise a negative error value
 * @retval #RPC_PORT_ERROR_NONE Successful
 * @retval #RPC_PORT_ERROR_INVALID_PARAMETER The specified @a h is NULL
 */
int rpc_port_stub_add_received_event_cb(rpc_port_stub_h h,
		rpc_port_stub_received_event_cb cb, void *user_data);

/**
 * @brief Gets a port from stub handle.
 * @since_tizen 4.0
 * @remarks This handle @a port will not be valid if the instance of the stub was disconnected or destroyed.
 * @param[in] h The rpc port stub handle
 * @param[in] type The type of port
 * @param[in] instance The ID of the instance which is connected
 * @param[out] port The port to communicate
 * @return @c 0 on success,
 *         otherwise a negative error value
 * @retval #RPC_PORT_ERROR_NONE Successful
 * @retval #RPC_PORT_ERROR_INVALID_PARAMETER The specified @a h is NULL
 * @retval #RPC_PORT_ERROR_IO_ERROR No available ports
 * @see rpc_port_write()
 * @see rpc_port_read()
 * @see rpc_port_parcel_create_from_port()
 * @see rpc_port_parcel_send();
 */
int rpc_port_stub_get_port(rpc_port_stub_h h, rpc_port_port_type_e type,
		const char *instance, rpc_port_h *port);

/**
 * @brief Connects to @a port_name of @a appid synchronously.
 * @details To send and receive data, the proxy should connect to port of stub.
 * @since_tizen 6.0
 * @privlevel public
 * @privilege %http://tizen.org/privilege/appmanager.launch \n
 *            %http://tizen.org/privilege/datasharing
 * @param[in] h The rpc port proxy handle
 * @param[in] appid The application ID of the target stub
 * @param[in] port_name The name of rpc port
 * @return @c 0 on success,
 *         otherwise a negative error value
 * @retval #RPC_PORT_ERROR_NONE Successful
 * @retval #RPC_PORT_ERROR_PERMISSION_DENIED Permission denied
 * @retval #RPC_PORT_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #RPC_PORT_ERROR_IO_ERROR I/O error
 */
int rpc_port_proxy_connect_sync(rpc_port_proxy_h h, const char *appid,
                const char *port_name);

/**
 * @brief Sets the paths of private sharing files.
 * @details If all added paths are under the caller application's data path which can be obtained by calling app_get_data_path() function,
 *          those will be shared to the target application. Platform will grant a temporary permission to the target application for those files and revoke it when the target application is terminated or rpc_port_unset_private_sharing() is called.
 *          Paths should be regular files. The target application can just read them.
 *          Note that the target application doesn't have read permission of the directory that is obtained by caller's app_get_data_path(),
 *          You should open the file path with read only mode directly. For example, access() call to the file path will return error because access() needs the read permission of the directory.
 *          The target application can call open() with O_RDONLY mode for the passed file path, because platform grants read permission to the passed file path.
 * @since_tizen 6.0
 * @param[in] port The rpc port handle
 * @param[in] paths The array of paths of files
 * @param[in] size The size of the array of the paths
 * @return @c 0 on success,
 *         otherwise a negative error value
 * @retval #RPC_PORT_ERROR_NONE Successful
 * @retval #RPC_PORT_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #RPC_PORT_ERROR_IO_ERROR I/O error
 *
 * @see rpc_port_unset_private_sharing()
 */
int rpc_port_set_private_sharing_array(rpc_port_h port, const char *paths[],
                unsigned int size);

/**
 * @brief Sets the path of the private sharing file.
 * @details See the description of rpc_port_set_private_sharing_array() for details.
 * @since_tizen 6.0
 * @param[in] port The rpc port handle
 * @param[in] path The path of the file
 * @return @c 0 on success,
 *         otherwise a negative error value
 * @retval #RPC_PORT_ERROR_NONE Successful
 * @retval #RPC_PORT_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #RPC_PORT_ERROR_IO_ERROR I/O error
 *
 * @see rpc_port_set_private_sharing_array()
 * @see rpc_port_unset_private_sharing()
 */
int rpc_port_set_private_sharing(rpc_port_h port, const char *path);

/**
 * @brief Unsets the private sharing.
 * @since_tizen 6.0
 * @param[in] port The rpc port handle
 * @return @c 0 on success,
 *         otherwise a negative error value
 * @retval #RPC_PORT_ERROR_NONE Successful
 * @retval #RPC_PORT_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #RPC_PORT_ERROR_IO_ERROR I/O error
 *
 * @see rpc_port_set_private_sharing_array()
 * @see rpc_port_set_private_sharing()
 */
int rpc_port_unset_private_sharing(rpc_port_h port);

/**
 * @brief Disconnects the port.
 * @since_tizen 6.5
 * @param[in] port The rpc port handle
 * @return @c 0 on success,
 *         otherwise a negative error value
 * @retval #RPC_PORT_ERROR_NONE Successful
 * @retval #RPC_PORT_ERROR_INVALID_PARAMETER Invalid parameter
 */
int rpc_port_disconnect(rpc_port_h port);

/**
 * @}
 */

#ifdef __cplusplus
}
#endif

#endif /* __TIZEN_APPFW_RPC_PORT_INCLUDE_H__ */
