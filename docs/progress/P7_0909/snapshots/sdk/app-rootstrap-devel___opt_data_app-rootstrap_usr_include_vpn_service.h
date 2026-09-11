/*
* Copyright (c) 2016 Samsung Electronics Co., Ltd All Rights Reserved
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

#ifndef __TIZEN_VPN_SERVICE_H__
#define __TIZEN_VPN_SERVICE_H__

#include <tizen.h>

#ifdef __cplusplus
extern "C" {
#endif // __cplusplus

/**
 * @file vpn_service.h
 */

/**
 * @addtogroup	CAPI_NETWORK_VPN_SERVICE_MODULE
 * @{
 */

/**
  * @brief   Enumeration for VPN service error types.
  * @details Indicate formats of error type field
  */
typedef enum {
	VPNSVC_ERROR_NONE = TIZEN_ERROR_NONE,				/**< Successful */
	VPNSVC_ERROR_INVALID_PARAMETER = TIZEN_ERROR_INVALID_PARAMETER,	/**< Invalid parameter */
	VPNSVC_ERROR_OUT_OF_MEMORY = TIZEN_ERROR_OUT_OF_MEMORY,		/**< Out of memory */
	VPNSVC_ERROR_PERMISSION_DENIED = TIZEN_ERROR_PERMISSION_DENIED,	/**< Permission denied */
	VPNSVC_ERROR_NO_SUCH_FILE = TIZEN_ERROR_NO_SUCH_FILE,		/**< No such file or directory */
	VPNSVC_ERROR_IO_ERROR = TIZEN_ERROR_IO_ERROR,			/**< IO error */
	VPNSVC_ERROR_TIMEOUT = TIZEN_ERROR_TIMED_OUT,			/**< Time out error or no answer */
	VPNSVC_ERROR_IPC_FAILED = TIZEN_ERROR_VPNSVC | 0x02,		/**< Failed to communicate with server */
	VPNSVC_ERROR_NOT_SUPPORTED = TIZEN_ERROR_NOT_SUPPORTED		/**< Not Supported */
} vpnsvc_error_e;


/**
  * @brief   The VPN interface handle.
  * @details This handle can be obtained by calling vpnsvc_init() and destroyed by calling vpnsvc_deinit().
  * @since_tizen 3.0
  * @see vpnsvc_init()
  * @see vpnsvc_deinit()
  */
typedef void* vpnsvc_h;

/**
 * @brief  Initializes VPN interface.
 * @details You should call vpnsvc_get_iface_name() for checking the actual initialized VPN interface name. (In case of duplicated interface name)
 * @since_tizen 3.0
 * @privlevel public
 * @privilege %http://tizen.org/privilege/vpnservice \n
 *            %http://tizen.org/privilege/internet
 * @remarks The @a handle should be released using vpnsvc_deinit().
 * @param[in] iface_name The VPN interface name
 * @param[out] handle  The VPN interface handle
 * @return 0 on success. otherwise, a negative error value.
 * @retval #VPNSVC_ERROR_NONE                  Success
 * @retval #VPNSVC_ERROR_INVALID_PARAMETER     Invalid parameter
 * @retval #VPNSVC_ERROR_IO_ERROR              I/O Error (e.g. socket I/O error)
 * @retval #VPNSVC_ERROR_IPC_FAILED            Cannot connect to service daemon
 * @retval #VPNSVC_ERROR_PERMISSION_DENIED     Permission Denied
 * @retval #VPNSVC_ERROR_NOT_SUPPORTED         Not Supported
 * @post Please call vpnsvc_deinit() if you want to de-initialize VPN interface.
 * @post Please call vpnsvc_get_iface_fd() if you want to know the fd(file descriptor) of VPN interface.
 * @post Please call vpnsvc_get_iface_index() if you want to know the index of VPN interface.
 * @post Please call vpnsvc_get_iface_name() if you want to know the name of VPN interface.
 * @see vpnsvc_deinit()
 * @see vpnsvc_get_iface_fd()
 * @see vpnsvc_get_iface_index()
 * @see vpnsvc_get_iface_name()
 */
int vpnsvc_init(const char* iface_name, vpnsvc_h *handle);

/**
 * @brief De-Initializes VPN interface.
 * @since_tizen 3.0
 * @privlevel public
 * @privilege %http://tizen.org/privilege/vpnservice \n
 *            %http://tizen.org/privilege/internet
 * @param[in] handle The VPN interface handle
 * @return 0 on success. otherwise, a negative error value.
 * @retval #VPNSVC_ERROR_NONE                  Success
 * @retval #VPNSVC_ERROR_INVALID_PARAMETER     Invalid parameter
 * @retval #VPNSVC_ERROR_IPC_FAILED            Cannot connect to service daemon
 * @retval #VPNSVC_ERROR_PERMISSION_DENIED     Permission Denied
 * @retval #VPNSVC_ERROR_NOT_SUPPORTED         Not Supported
 * @pre Before calling this function, VPN interface should be initialized already.
 * @see vpnsvc_init()
 */
int vpnsvc_deinit(vpnsvc_h handle);

/**
 * @brief Protect a socket from VPN connections.
 * @details This function allows you to bypass the VPN tunnel
 * and send data directly to the underlying network using the specified socket.
 * It can be useful when you need to access local resources or services that are not available through the VPN connection.
 * @remarks @a socket_fd should refer to an already open socket file descriptor. \n
 *          If @a socket_fd cannot be bind to @a iface_name, #VPNSVC_ERROR_IO_ERROR occurs.
 * @since_tizen 3.0
 * @privlevel public
 * @privilege %http://tizen.org/privilege/vpnservice \n
 *            %http://tizen.org/privilege/internet
 * @param[in] handle    The VPN interface handle
 * @param[in] socket_fd The opened socket file descriptor
 * @param[in] iface_name  The network interface name (e.g., interface name such as eth0, ppp0, etc) through which the VPN is working
 * @return 0 on success. otherwise, a negative error value.
 * @retval #VPNSVC_ERROR_NONE                  Success
 * @retval #VPNSVC_ERROR_INVALID_PARAMETER     Invalid parameter
 * @retval #VPNSVC_ERROR_IO_ERROR              I/O Error (e.g. socket I/O error)
 * @retval #VPNSVC_ERROR_IPC_FAILED            Cannot connect to service daemon
 * @retval #VPNSVC_ERROR_PERMISSION_DENIED     Permission Denied
 * @retval #VPNSVC_ERROR_NOT_SUPPORTED         Not Supported
 */
int vpnsvc_protect(vpnsvc_h handle, int socket_fd, const char* iface_name);

/**
 * @brief Waits for the read event on VPN interface descriptor, but no more than the indicated timeout in milliseconds.
 * @details This function can block until there is data available for reading from the VPN interface, or the timeout expires.
 * It's important to set a reasonable timeout value to avoid unnecessary delays in your application's response time.
 * @since_tizen 3.0
 * @param[in] handle      The VPN interface handle
 * @param[in] timeout_ms  The value of timeout (milliseconds)
 * @return 0 on success. otherwise, a negative error value.
 * @retval #VPNSVC_ERROR_NONE                  Success
 * @retval #VPNSVC_ERROR_INVALID_PARAMETER     Invalid parameter
 * @retval #VPNSVC_ERROR_IO_ERROR              I/O Error (e.g. socket I/O error)
 * @retval #VPNSVC_ERROR_TIMEOUT               Timeout (no data received within the specified timeout)
 * @retval #VPNSVC_ERROR_NOT_SUPPORTED         Not Supported
 * @pre The VPN interface should be initialized already.
 * @see vpnsvc_init()
 */
int vpnsvc_read(vpnsvc_h handle, int timeout_ms);

/**
 * @brief Writes the data supplied into the VPN interface.
 * @details This function allows you to transfer data to the VPN interface by passing @a data containing the desired information and its corresponding @a size.
 * @since_tizen 3.0
 * @param[in] handle The VPN interface handle
 * @param[in] data   Data writing to VPN interface
 * @param[in] size   The size of data
 * @return On success, the number of bytes written is returned (zero indicates nothing was written). Otherwise, a negative error value.
 * @retval #VPNSVC_ERROR_NONE                  Success
 * @retval #VPNSVC_ERROR_INVALID_PARAMETER     Invalid parameter
 * @retval #VPNSVC_ERROR_NOT_SUPPORTED         Not Supported
 * @retval In case of negative error, please refer to standard posix write API's error code.
 * @pre The VPN interface should be initialized already.
 * @see vpnsvc_init()
 */
int vpnsvc_write(vpnsvc_h handle, const char* data, size_t size);

/**
 * @brief Blocks all traffics except specified allowing networks.
 * @details This function enables you to restrict network access by blocking all traffic except for the specified allowing networks.
 * You can define the destination addresses of the routes for both the VPN interface and the original interface, along with their respective prefix lengths.
 * By setting @a num_allow_routes_vpn @a num_allow_routes_orig, you can specify up to 255 allowing networks for each interface.
 * Once applied, this function ensures that only the defined allowing networks are accessible while all other traffic remains blocked.
 * @since_tizen 3.0
 * @privlevel public
 * @privilege %http://tizen.org/privilege/vpnservice \n
 *            %http://tizen.org/privilege/internet
 * @param[in] handle                  The VPN interface handle
 * @param[in] routes_dest_vpn_addr    Destination address of the routes, the list of allowing networks over VPN interface (e.g., VPN interface such as tun0, etc).
 * @param[in] routes_vpn_prefix       The prefix of VPN interface, netmask length (also called a prefix, e.g. 8, 16, 24, 32).
 * @param[in] num_allow_routes_vpn    The number of allowing networks over VPN interface. Up to 255 addresses can be allowed.
 * @param[in] routes_dest_orig_addr   Destination address of the routes, the list of allowing networks over the original interface (e.g., original interface such as eth0, wlan0, etc).
 * @param[in] routes_orig_prefix      The prefix of Original interface, netmask length (also called a prefix, e.g. 8, 16, 24, 32).
 * @param[in] num_allow_routes_orig   The number of allowing networks over the original interface. Up to 255addresses can be allowed.
 * @return 0 on success. otherwise, a negative error value.
 * @retval #VPNSVC_ERROR_NONE                  Success
 * @retval #VPNSVC_ERROR_INVALID_PARAMETER     Invalid parameter
 * @retval #VPNSVC_ERROR_IPC_FAILED            Cannot connect to service daemon
 * @retval #VPNSVC_ERROR_PERMISSION_DENIED     Permission Denied
 * @retval #VPNSVC_ERROR_NOT_SUPPORTED         Not Supported
 * @post Please call vpnsvc_unblock_networks() if you want to allow all traffics.
 * @see vpnsvc_unblock_networks()
 */
int vpnsvc_block_networks(vpnsvc_h handle,
		char *routes_dest_vpn_addr[],
		int routes_vpn_prefix[],
		size_t num_allow_routes_vpn,
		char *routes_dest_orig_addr[],
		int routes_orig_prefix[],
		size_t num_allow_routes_orig);

/**
 * @brief Removes any restrictions imposed by vpnsvc_block_networks().
 * @details This function serves as the counterpart to vpnsvc_block_networks(), enabling you to remove any previously applied restrictions on network access.
 * By calling this function with the appropriate VPN interface handle, you can effectively lift the limitations imposed by the previous blocking rules and restore full network connectivity.
 * @since_tizen 3.0
 * @privlevel public
 * @privilege %http://tizen.org/privilege/vpnservice \n
 *            %http://tizen.org/privilege/internet
 * @param[in] handle The VPN interface handle
 * @return 0 on success. otherwise, a negative error value.
 * @retval #VPNSVC_ERROR_NONE                  Success
 * @retval #VPNSVC_ERROR_INVALID_PARAMETER     Invalid parameter
 * @retval #VPNSVC_ERROR_IPC_FAILED            Cannot connect to service daemon
 * @retval #VPNSVC_ERROR_PERMISSION_DENIED     Permission Denied
 * @retval #VPNSVC_ERROR_NOT_SUPPORTED         Not Supported
 * @see vpnsvc_block_networks()
 */
int vpnsvc_unblock_networks(vpnsvc_h handle);

/**
 * @brief Updates the VPN interface settings including Local IP, Remote IP, and MTU values.
 * @details This function allows you to modify various settings related to the VPN interface, such as the local IP address, remote IP address, and MTU value.
 * By providing the appropriate arguments, you can update these configurations accordingly.
 * @since_tizen 3.0
 * @privlevel public
 * @privilege %http://tizen.org/privilege/vpnservice
 * @param[in] handle The VPN interface handle
 * @return 0 on success. Otherwise, a negative error value.
 * @retval #VPNSVC_ERROR_NONE                  Success
 * @retval #VPNSVC_ERROR_INVALID_PARAMETER     Invalid parameter
 * @retval #VPNSVC_ERROR_IPC_FAILED            Cannot connect to service daemon
 * @retval #VPNSVC_ERROR_PERMISSION_DENIED     Permission Denied
 * @retval #VPNSVC_ERROR_NOT_SUPPORTED         Not Supported
 * @pre The VPN interface should be initialized already.
 * @see vpnsvc_init()
 * @see vpnsvc_set_local_ip_address()
 * @see vpnsvc_set_remote_ip_address()
 * @see vpnsvc_set_mtu()
 */
int vpnsvc_update_settings(vpnsvc_h handle);

/**
 * @brief Gets the fd of the VPN interface.
 * @since_tizen 3.0
 * @param[in] handle The VPN interface handle
 * @param[out] iface_fd The vpn interface fd
 * @return The fd value of VPN interface. Otherwise, a negative error value.
 * @retval #VPNSVC_ERROR_NONE                  Success
 * @retval #VPNSVC_ERROR_INVALID_PARAMETER     Invalid parameter
 * @retval #VPNSVC_ERROR_NOT_SUPPORTED         Not Supported
 */
int vpnsvc_get_iface_fd(vpnsvc_h handle, int* iface_fd);

/**
 * @brief Gets the index of VPN interface.
 * @since_tizen 3.0
 * @param[in] handle The VPN interface handle
 * @param[out] iface_index The VPN interface index
 * @return The index of the VPN interface. otherwise, a negative error value.
 * @retval #VPNSVC_ERROR_NONE                  Success
 * @retval #VPNSVC_ERROR_INVALID_PARAMETER     Invalid parameter
 * @retval #VPNSVC_ERROR_NOT_SUPPORTED         Not Supported
 * @pre Before calling this function, VPN interface should be initialized already.
 * @see vpnsvc_init()
 */
int vpnsvc_get_iface_index(vpnsvc_h handle, int* iface_index);

/**
 * @brief Gets the name of VPN interface.
 * @since_tizen 3.0
 * @remarks The @a iface_name should be released using free()
 * @param[in] handle    The VPN interface handle
 * @param[out] iface_name The name of VPN interface name
 * @return 0 on success. Otherwise, a negative error value.
 * @retval #VPNSVC_ERROR_NONE                  Success
 * @retval #VPNSVC_ERROR_INVALID_PARAMETER     Invalid parameter
 * @retval #VPNSVC_ERROR_NOT_SUPPORTED         Not Supported
 * @pre Before calling this function, VPN interface should be initialized already.
 * @see vpnsvc_init()
 */
int vpnsvc_get_iface_name(vpnsvc_h handle, char** iface_name);

/**
 * @brief Sets the Maximum Transmission Unit (MTU) of the VPN interface.
 * @details This function allows you to adjust the maximum transmission unit (MTU) value for the VPN interface.
 * By providing the appropriate @a mtu, you can customize the size of packets that can be transmitted over the VPN link.
 * It's crucial to ensure that the VPN interface has been initialized correctly before invoking this function, as mentioned in the precondition.
 * Moreover, remember to call the vpnsvc_update_settings() after modifying the MTU value in order for the change to take effect.
 * @since_tizen 3.0
 * @remarks vpnsvc_update_settings() should be invoked after setting MTU.
 * @param[in] handle The VPN interface handle
 * @param[in] mtu    The MTU (Maximum Transmission Unit) value to be set for VPN interface. Default MTU size is 1500.
 * @return 0 on success. Otherwise, a negative error value.
 * @retval #VPNSVC_ERROR_NONE                  Success
 * @retval #VPNSVC_ERROR_INVALID_PARAMETER     Invalid parameter
 * @retval #VPNSVC_ERROR_IO_ERROR              I/O Error (e.g. socket I/O error)
 * @retval #VPNSVC_ERROR_NOT_SUPPORTED         Not Supported
 * @pre Before calling this function, VPN interface should be initialized already.
 * @see vpnsvc_init()
 * @see vpnsvc_update_settings()
 */
int vpnsvc_set_mtu(vpnsvc_h handle, int mtu);

/**
 * @brief Sets blocking mode of the file descriptor of VPN interface.
 * @details This function allows you to control the blocking behavior of the VPN interface's file descriptor.
 * By setting the @a blocking to @c true, the file descriptor will operate in blocking mode, meaning that system calls such as read or write will block until they complete successfully.
 * Alternatively, setting @a blocking to @c false will enable non-blocking mode, where system calls will return immediately regardless of their completion status.
 * @since_tizen 3.0
 * @param[in] handle    The VPN interface handle
 * @param[in] blocking  The blocking mode flag; True = BLOCKING, False = NON_BLOCKING (Default : BLOCKING)
 * @return 0 on success. Otherwise, a negative error value.
 * @retval #VPNSVC_ERROR_NONE                  Success
 * @retval #VPNSVC_ERROR_INVALID_PARAMETER     Invalid parameter
 * @retval #VPNSVC_ERROR_IO_ERROR              Failed to set the blocking flags
 * @retval #VPNSVC_ERROR_NOT_SUPPORTED         Not Supported
 * @pre Before calling this function, VPN interface should be initialized already.
 * @see vpnsvc_init()
 */
int vpnsvc_set_blocking(vpnsvc_h handle, bool blocking);

/**
 * @brief Sets the session name for the VPN.
 * @details A VPN session refers to the continuous and logical connection state during which a user or device connects to and interacts with a VPN network.\n
 * This function allows you can set a name for this logical connection. It will be displayed in system-managed dialogs and notifications.
 * @since_tizen 3.0
 * @param[in] handle       The VPN interface handle
 * @param[in] session      The Session Name
 * @return 0 on success. Otherwise, a negative error value.
 * @retval #VPNSVC_ERROR_NONE                  Success
 * @retval #VPNSVC_ERROR_INVALID_PARAMETER     Invalid parameter
 * @retval #VPNSVC_ERROR_NOT_SUPPORTED         Not Supported
 * @pre Before calling this function, VPN interface should be initialized already.
 * @see vpnsvc_init()
 */
int vpnsvc_set_session(vpnsvc_h handle, const char* session);

/**
 * @brief Gets the session name for the VPN.
 * @since_tizen 3.0
 * @remarks The @a session should be released using free()
 * @param[in] handle   The VPN interface handle
 * @param[out] session The Session Name returned
 * @return 0 on success. Otherwise, a negative error value.
 * @retval #VPNSVC_ERROR_NONE                  Success
 * @retval #VPNSVC_ERROR_INVALID_PARAMETER     Invalid parameter
 * @retval #VPNSVC_ERROR_NOT_SUPPORTED         Not Supported
 * @pre Before calling this function, VPN interface should be initialized already.
 * @see vpnsvc_init()
 */
int vpnsvc_get_session(vpnsvc_h handle, char** session);

/**
 * @brief Sets the VPN client IP address.
 * @details This function allows you to configure the VPN client IP address by providing the appropriate @a local_ip.
 * Remember to call vpnsvc_update_settings() after setting the local IP address in order for the change to take effect.
 * @since_tizen 3.0
 * @remarks The local address should be set before enabling VPN interface. \n
 *			vpnsvc_update_settings() should be invoked after setting local IP address.
 * @param[in] handle		The VPN interface handle
 * @param[in] local_ip		The local (VPN client) IP address (IPv4 only)

 * @return 0 on success. Otherwise, a negative error value.
 * @retval #VPNSVC_ERROR_NONE                  Success
 * @retval #VPNSVC_ERROR_INVALID_PARAMETER     Invalid parameter
 * @retval #VPNSVC_ERROR_IO_ERROR              I/O Error (e.g. socket I/O error)
 * @retval #VPNSVC_ERROR_NOT_SUPPORTED         Not Supported
 * @pre The VPN interface should be initialized already.
 * @see vpnsvc_init()
 * @see vpnsvc_update_settings()
 */
int vpnsvc_set_local_ip_address(vpnsvc_h handle, const char *local_ip);

/**
 * @brief Sets the VPN server IP address.
 * @details This function allows you to configure the VPN server IP address by providing the appropriate @a remote_ip.
 * Remember to call vpnsvc_update_settings() after setting the remote IP address in order for the change to take effect.
 * @since_tizen 3.0
 * @remarks The remote address should be set before enabling VPN interface. \n
 *			vpnsvc_update_settings() should be invoked after setting remote IP address.
 * @param[in] handle		The VPN interface handle
 * @param[in] remote_ip		The remote (VPN server) IP address (IPv4 only)
 * @return 0 on success. Otherwise, a negative error value.
 * @retval #VPNSVC_ERROR_NONE                  Success
 * @retval #VPNSVC_ERROR_INVALID_PARAMETER     Invalid parameter
 * @retval #VPNSVC_ERROR_IO_ERROR              I/O Error (e.g. socket I/O error)
 * @retval #VPNSVC_ERROR_NOT_SUPPORTED         Not Supported
 * @pre The VPN interface should be initialized already.
 * @see vpnsvc_init()
 * @see vpnsvc_update_settings()
 */
int vpnsvc_set_remote_ip_address(vpnsvc_h handle, const char *remote_ip);

/**
 * @brief Adds the route address.
 * @details This function allows you to add a new route address to the VPN interface by providing the appropriate @a route_address @a prefix.
 * Upon successful execution, the specified route address will be added to the routing table, facilitating network communication via the VPN interface.
 * @since_tizen 3.0
 * @privlevel public
 * @privilege %http://tizen.org/privilege/vpnservice
 * @remarks The route address should be set after enabling VPN interface.
 * @param[in] handle		The VPN interface handle
 * @param[in] route_address	Destination address of the routes (IPv4 only)
 * @param[in] prefix		The prefix of routes, netmask length (also called a prefix, e.g. 8, 16, 24, 32)
 * @return 0 on success. Otherwise, a negative error value.
 * @retval #VPNSVC_ERROR_NONE                  Success
 * @retval #VPNSVC_ERROR_INVALID_PARAMETER     Invalid parameter
 * @retval #VPNSVC_ERROR_IO_ERROR              I/O Error (e.g. socket I/O error)
 * @retval #VPNSVC_ERROR_NOT_SUPPORTED         Not Supported
 * @pre The VPN interface should be initialized already.
 * @see vpnsvc_init()
 * @see vpnsvc_remove_route()
 */
int vpnsvc_add_route(vpnsvc_h handle, const char *route_address, int prefix);

/**
 * @brief Removes the route address.
 * @details This function enables you to delete a specific route address from the VPN interface by passing @a route_address and @a prefix arguments.
 * Upon successful execution, the specified route address will be removed from the routing table, altering the network communication paths accordingly.
 * @since_tizen 3.0
 * @privlevel public
 * @privilege %http://tizen.org/privilege/vpnservice
 * @param[in] handle			The VPN interface handle
 * @param[in] route_address		Destination address of the routes (IPv4 only)
 * @param[in] prefix		The prefix of routes, netmask length (also called a prefix, e.g. 8, 16, 24, 32)
 * @return 0 on success. Otherwise, a negative error value.
 * @retval #VPNSVC_ERROR_NONE                  Success
 * @retval #VPNSVC_ERROR_INVALID_PARAMETER     Invalid parameter
 * @retval #VPNSVC_ERROR_IO_ERROR              I/O Error (e.g. socket I/O error)
 * @retval #VPNSVC_ERROR_NOT_SUPPORTED         Not Supported
 * @pre The VPN interface should be initialized already.
 * @see vpnsvc_init()
 * @see vpnsvc_add_route()
 */
int vpnsvc_remove_route(vpnsvc_h handle, const char *route_address, int prefix);

/**
 * @brief Adds the DNS server name.
 * @details This function allows you to add a new DNS server address to the VPN interface by providing the appropriate @a dns_server.
 * Upon successful execution, the specified DNS server address will be registered, enabling efficient domain name resolution via the VPN interface.
 * @since_tizen 3.0
 * @privlevel public
 * @privilege %http://tizen.org/privilege/vpnservice
 * @remarks The dns address should be set after enabling VPN interface.
 * @param[in] handle		The VPN interface handle
 * @param[in] dns_server	The DNS server address (IPv4 only)
 * @return 0 on success. Otherwise, a negative error value.
 * @retval #VPNSVC_ERROR_NONE                  Success
 * @retval #VPNSVC_ERROR_INVALID_PARAMETER     Invalid parameter
 * @retval #VPNSVC_ERROR_IO_ERROR              I/O Error (e.g. socket I/O error)
 * @retval #VPNSVC_ERROR_NOT_SUPPORTED         Not Supported
 * @pre The VPN interface should be initialized already.
 * @see vpnsvc_init()
 */
int vpnsvc_add_dns_server(vpnsvc_h handle, const char *dns_server);

/**
 * @brief Removes the DNS server name.
 * @details This function enables you to delete a specific DNS server address from the VPN interface by passing @a dns_server.
 * Upon successful execution, the specified DNS server address will be deregistered, modifying the domain name resolution process accordingly.
 * @since_tizen 3.0
 * @privlevel public
 * @privilege %http://tizen.org/privilege/vpnservice
 * @param[in] handle		The VPN interface handle
 * @param[in] dns_server	The DNS server address (IPv4 only)
 * @return 0 on success. Otherwise, a negative error value.
 * @retval #VPNSVC_ERROR_NONE                  Success
 * @retval #VPNSVC_ERROR_INVALID_PARAMETER     Invalid parameter
 * @retval #VPNSVC_ERROR_IO_ERROR              I/O Error (e.g. socket I/O error)
 * @retval #VPNSVC_ERROR_NOT_SUPPORTED         Not Supported
 * @pre The VPN interface should be initialized already.
 * @see vpnsvc_init()
 */
int vpnsvc_remove_dns_server(vpnsvc_h handle, const char *dns_server);

/**
* @}
*/

#ifdef __cplusplus
}
#endif	// __cplusplus

#endif /* __TIZEN_CAPI_VPN_SERVICE_H__ */
