/*
 * Copyright (c) 2017 - 2021 Samsung Electronics Co., Ltd All Rights Reserved
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

#ifndef __TIZEN_APPFW_RPC_PORT_INTERNAL_INCLUDE_H__
#define __TIZEN_APPFW_RPC_PORT_INTERNAL_INCLUDE_H__

#include <bundle.h>
#include <rpc-port.h>
#include <sys/types.h>
#include <unistd.h>

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @brief Sets the target user ID.
 * @since_tizen 6.5
 *
 * @param[in]   target_uid      The target user ID
 */
void rpc_port_set_target_uid(uid_t target_uid);

/**
 * @brief Gets the target user ID.
 * @since_tizen 6.5
 *
 * @return @c the target user ID
 */
uid_t rpc_port_get_target_uid(void);

/**
 * @brief Registers the process information.
 * @since_tizen 6.5
 * @remarks This function is only for a daemon process.
 * @param[in]   proc_name       The process name
 * @param[in]   extra           The extra data
 * @return @c 0 on success,
 *         otherwise a negative error value
 * @see rpc_port_deregister_proc_info()
 */
int rpc_port_register_proc_info(const char *proc_name, bundle *extra);

/**
 * @brief Dergisters the process information.
 * @since_tizen 6.5
 * @return @c 0 on success,
 *         otherwise a negative error value
 * @see rpc_port_register_proc_info()
 */
int rpc_port_deregister_proc_info(void);

/**
 * @brief Gets the pid and uid of the connected rpc port.
 * @since_tizen 8.0
 * @param[in] h The rpc port handle
 * @param[out] pid Process ID
 * @param[out] uid User ID
 * @return @c 0 on success,
 *         otherwise a negative error value
 */
int rpc_port_get_peer_info(rpc_port_h h, pid_t *pid, uid_t *uid);

/**
 * @brief Gets the read fd number of the connected rpc port.
 * @since_tizen 10.0
 * @param[in] h The rpc port handle
 * @param[out] fd Read fd number
 * @return @c 0 on success,
 *         otherwise a negative error value
 */
int rpc_port_get_read_fd(rpc_port_h h, int *fd);

/**
 * @brief Checks whether the pending request exists or not.
 * @since_tizen 9.0
 * @param[in] h The rpc port stub handle
 * @param[out] has_request The flag, if true, the pending request exists
 * @return @c 0 on success,
 *         otherwise a negative error value
 * @retval #RPC_PORT_ERROR_NONE Successful
 * @retval #RPC_PORT_ERROR_INVALID_PARAMETER Invalid parameter
 */
int rpc_port_stub_has_pending_request(rpc_port_stub_h h, bool *has_request);

/**
 * @brief Sets the timeout for the rpc port.
 * @since_tizen 9.0
 * @param[in] timeout The timeout in milliseconds
 * @return @c 0 on success,
 *         otherwise a negative error value
 * @retval #RPC_PORT_ERROR_NONE Successful
 * @retval #RPC_PORT_ERROR_INVALID_PARAMETER Invalid parameter
 * @see rpc_port_get_timeout()
 */
int rpc_port_set_timeout(int timeout);

/**
 * @brief Gets the timeout of the rpc port.
 * @since_tizen 9.0
 * @return The timeout in milliseconds
 * @see rpc_port_set_timeout()
 */
int rpc_port_get_timeout(void);

/**
 * @brief Sets the communication domain as INET for the rpc port stub.
 * @since_tizen 11.0
 * @return @c 0 on success,
 *         otherwise a negative error value
 */
int rpc_port_stub_set_domain_inet(rpc_port_stub_h h);

/**
 * @brief Sets the communication domain as INET for the rpc port proxy.
 * @since_tizen 11.0
 * @return @c 0 on success,
 *         otherwise a negative error value
 */
int rpc_port_proxy_set_domain_inet(rpc_port_proxy_h h);

/**
 * @brief Connects to the target stub using TCP/IP protocol asynchronously.
 * @details To send and receive data, the proxy should connect to port of stub.
 * @since_tizen 11.0
 * @privlevel public
 * @privilege %http://tizen.org/privilege/appmanager.launch \n
 *            %http://tizen.org/privilege/datasharing
 * @param[in] h The rpc port proxy handle
 * @param[in] ip The IP address of the target stub
 * @param[in] appid The application ID of the target stub
 * @param[in] port The name of rpc port
 * @return @c 0 on success,
 *         otherwise a negative error value
 * @retval #RPC_PORT_ERROR_NONE Successful
 * @retval #RPC_PORT_ERROR_PERMISSION_DENIED Permission denied
 * @retval #RPC_PORT_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #RPC_PORT_ERROR_IO_ERROR I/O error
 */
int rpc_port_proxy_tcp_connect(rpc_port_proxy_h h, const char *ip,
                               const char *appid, const char *port);

/**
 * @brief Connects to the target stub using TCP/IP protocol synchronously.
 * @details To send and receive data, the proxy should connect to port of stub.
 * @since_tizen 11.0
 * @privlevel public
 * @privilege %http://tizen.org/privilege/appmanager.launch \n
 *            %http://tizen.org/privilege/datasharing
 * @param[in] h The rpc port proxy handle
 * @param[in] ip The IP address of the target stub
 * @param[in] appid The application ID of the target stub
 * @param[in] port The name of rpc port
 * @return @c 0 on success,
 *         otherwise a negative error value
 * @retval #RPC_PORT_ERROR_NONE Successful
 * @retval #RPC_PORT_ERROR_PERMISSION_DENIED Permission denied
 * @retval #RPC_PORT_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #RPC_PORT_ERROR_IO_ERROR I/O error
 */
int rpc_port_proxy_tcp_connect_sync(rpc_port_proxy_h h, const char *ip,
                                    const char *appid, const char *port);

/**
 * @brief Sets the client TLS certificate and key for authentication.
 * @since_tizen 11.0
 * @param[in] h The rpc port proxy handle
 * @param[in] cert_path Path to the client certificate file (PEM format)
 * @param[in] key_path Path to the client private key file (PEM format)
 * @return @c 0 on success,
 *         otherwise a negative error value
 * @retval #RPC_PORT_ERROR_NONE Successful
 * @retval #RPC_PORT_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #RPC_PORT_ERROR_IO_ERROR Failed to read certificate/key files
 */
int rpc_port_proxy_set_tls_client_certificate(rpc_port_proxy_h h,
                                             const char *cert_path,
                                             const char *key_path);

/**
 * @brief Sets the CA certificate for server verification.
 * @since_tizen 11.0
 * @param[in] h The rpc port proxy handle
 * @param[in] ca_path Path to the CA certificate file (PEM format)
 * @return @c 0 on success,
 *         otherwise a negative error value
 * @retval #RPC_PORT_ERROR_NONE Successful
 * @retval #RPC_PORT_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #RPC_PORT_ERROR_IO_ERROR Failed to read CA certificate
 */
int rpc_port_proxy_set_tls_ca_certificate(rpc_port_proxy_h h,
                                         const char *ca_path);

/**
 * @brief Sets the server TLS certificate and key.
 * @since_tizen 11.0
 * @param[in] h The rpc port stub handle
 * @param[in] cert_path Path to the server certificate file (PEM format)
 * @param[in] key_path Path to the server private key file (PEM format)
 * @return @c 0 on success,
 *         otherwise a negative error value
 * @retval #RPC_PORT_ERROR_NONE Successful
 * @retval #RPC_PORT_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #RPC_PORT_ERROR_IO_ERROR Failed to read certificate/key files
 */
int rpc_port_stub_set_tls_certificate(rpc_port_stub_h h,
                                     const char *cert_path,
                                     const char *key_path);

/**
 * @brief Sets the CA certificate for client verification.
 * @since_tizen 11.0
 * @param[in] h The rpc port stub handle
 * @param[in] ca_path Path to the CA certificate file (PEM format)
 * @return @c 0 on success,
 *         otherwise a negative error value
 * @retval #RPC_PORT_ERROR_NONE Successful
 * @retval #RPC_PORT_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #RPC_PORT_ERROR_IO_ERROR Failed to read CA certificate
 */
int rpc_port_stub_set_tls_ca_certificate(rpc_port_stub_h h,
                                         const char *ca_path);

/**
 * @brief Checks if the given instance has the specified privilege.
 * @since_tizen 11.0
 * @param[in] h The rpc port stub handle
 * @param[in] instance The instance name
 * @param[in] privilege The privilege name
 * @param[out] has_privilege @c true if the instance has the privilege, @c false otherwise
 * @return @c 0 on success,
 *         otherwise a negative error value
 * @retval #RPC_PORT_ERROR_NONE Successful
 * @retval #RPC_PORT_ERROR_INVALID_PARAMETER Invalid parameter
 */
int rpc_port_stub_has_privilege(rpc_port_stub_h h, const char *instance, const char *privilege, bool *has_privilege);

/**
 * @brief Checks if the given appid has the specified privilege.
 * @since_tizen 11.0
 * @param[in] h The rpc port stub handle
 * @param[in] appid The App ID
 * @param[in] privilege The privilege name
 * @param[out] has_privilege @c true if the instance has the privilege, @c false otherwise
 * @return @c 0 on success,
 *         otherwise a negative error value
 * @retval #RPC_PORT_ERROR_NONE Successful
 * @retval #RPC_PORT_ERROR_INVALID_PARAMETER Invalid parameter
 */
int rpc_port_stub_has_privilege_local(rpc_port_stub_h h, const char *appid, const char *privilege, bool *has_privilege);


typedef void *rpc_port_discovery_h;

/**
 * @brief Discovery callback function type.
 * @since_tizen 11.0
 * @param[in] service_name The name of the discovered service
 * @param[in] tidl_port The TIDL port name
 * @param[in] app_id The application ID
 * @param[in] ip The IP address of the service
 * @param[in] user_data User data
 */
typedef void (*rpc_port_discovery_cb)(const char *service_name, const char *tidl_port, const char* app_id, const char *ip, void *user_data);

/**
 * @brief Creates a discovery handle.
 * @since_tizen 11.0
 * @param[out] h The discovery handle to be created
 * @return @c 0 on success,
 *         otherwise a negative error value
 * @retval #RPC_PORT_ERROR_NONE Successful
 * @retval #RPC_PORT_ERROR_INVALID_PARAMETER Invalid parameter
 */
int rpc_port_discovery_create(rpc_port_discovery_h *h);

/**
 * @brief Destroys a discovery handle.
 * @since_tizen 11.0
 * @param[in] h The discovery handle to be destroyed
 * @return @c 0 on success,
 *         otherwise a negative error value
 * @retval #RPC_PORT_ERROR_NONE Successful
 * @retval #RPC_PORT_ERROR_INVALID_PARAMETER Invalid parameter
 */
int rpc_port_discovery_destroy(rpc_port_discovery_h h);

/**
 * @brief Starts finding services.
 * @since_tizen 11.0
 * @param[in] h The discovery handle
 * @param[in] cb The callback function to be called when a service is found
 * @param[in] user_data User data to be passed to the callback function
 * @return @c 0 on success,
 *         otherwise a negative error value
 * @retval #RPC_PORT_ERROR_NONE Successful
 * @retval #RPC_PORT_ERROR_INVALID_PARAMETER Invalid parameter
 */
int rpc_port_discovery_start_finding(rpc_port_discovery_h h, rpc_port_discovery_cb cb, void *user_data);

/**
 * @brief Stops finding services.
 * @since_tizen 11.0
 * @param[in] h The discovery handle
 * @return @c 0 on success,
 *         otherwise a negative error value
 * @retval #RPC_PORT_ERROR_NONE Successful
 * @retval #RPC_PORT_ERROR_INVALID_PARAMETER Invalid parameter
 */
int rpc_port_discovery_stop_finding(rpc_port_discovery_h h);

/**
 * @brief Registers a service for discovery.
 * @since_tizen 11.0
 * @param[in] h The discovery handle
 * @param[in] service_name The name of the service to register
 * @param[in] tidl_port The TIDL port name
 * @param[in] app_id The application ID
 * @return @c 0 on success,
 *         otherwise a negative error value
 * @retval #RPC_PORT_ERROR_NONE Successful
 * @retval #RPC_PORT_ERROR_INVALID_PARAMETER Invalid parameter
 */
int rpc_port_discovery_register_service(rpc_port_discovery_h h, const char *service_name, const char *tidl_port, const char *app_id);

#ifdef __cplusplus
}
#endif

#endif /* __TIZEN_APPFW_RPC_PORT_INTERNAL_INCLUDE_H__ */
