/*
 * Copyright (c) 2011-2013 Samsung Electronics Co., Ltd All Rights Reserved
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


#ifndef __TIZEN_NETWORK_NET_CONNECTION_H__
#define __TIZEN_NETWORK_NET_CONNECTION_H__


#include <connection_profile.h>


#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */


/**
 * @file net_connection.h
 */


/**
 * @addtogroup CAPI_NETWORK_CONNECTION_MANAGER_MODULE
 * @{
 */


/**
 * @brief The connection handle.
 * @since_tizen 2.3
*/
typedef void* connection_h;


/**
 * @brief The profiles iterator handle.
 * @since_tizen 2.3
*/
typedef void* connection_profile_iterator_h;


/**
 * @brief Enumeration for connection type.
 * @since_tizen 2.3
 */
typedef enum {
	CONNECTION_TYPE_DISCONNECTED = 0,  /**< Disconnected */
	CONNECTION_TYPE_WIFI = 1,          /**< Wi-Fi type */
	CONNECTION_TYPE_CELLULAR = 2,      /**< Cellular type */
	CONNECTION_TYPE_ETHERNET = 3,      /**< Ethernet type */
	CONNECTION_TYPE_BT = 4,            /**< Bluetooth type */
	CONNECTION_TYPE_NET_PROXY,         /**< Proxy type for Internet connection (Since 3.0) */
} connection_type_e;


/**
 * @brief Enumeration for cellular network state.
 * @since_tizen 2.3
 */
typedef enum {
	CONNECTION_CELLULAR_STATE_OUT_OF_SERVICE = 0,      /**< Out of service */
	CONNECTION_CELLULAR_STATE_FLIGHT_MODE = 1,         /**< Flight mode */
	CONNECTION_CELLULAR_STATE_ROAMING_OFF = 2,         /**< Roaming is turned off */
	CONNECTION_CELLULAR_STATE_CALL_ONLY_AVAILABLE = 3, /**< Call is only available */
	CONNECTION_CELLULAR_STATE_AVAILABLE = 4,           /**< Available but not connected yet */
	CONNECTION_CELLULAR_STATE_CONNECTED = 5,           /**< Connected */
} connection_cellular_state_e;


/**
 * @brief Enumeration for Wi-Fi state.
 * @since_tizen 2.3
 */
typedef enum {
	CONNECTION_WIFI_STATE_DEACTIVATED = 0,   /**< Wi-Fi is deactivated */
	CONNECTION_WIFI_STATE_DISCONNECTED = 1,  /**< Disconnected */
	CONNECTION_WIFI_STATE_CONNECTED = 2,     /**< Connected */
} connection_wifi_state_e;


/**
 * @brief Enumeration for ethernet state.
 * @since_tizen 2.4
 */
typedef enum {
	CONNECTION_ETHERNET_STATE_DEACTIVATED = 0,  /**< There is no Ethernet profile to open */
	CONNECTION_ETHERNET_STATE_DISCONNECTED = 1, /**< Disconnected */
	CONNECTION_ETHERNET_STATE_CONNECTED = 2,    /**< Connected */
} connection_ethernet_state_e;


/**
 * @brief Enumeration for Bluetooth state.
 * @since_tizen 2.3
 */
typedef enum {
	CONNECTION_BT_STATE_DEACTIVATED = 0,   /**< There is no Bluetooth profile to open */
	CONNECTION_BT_STATE_DISCONNECTED = 1,  /**< Disconnected */
	CONNECTION_BT_STATE_CONNECTED = 2,     /**< Connected */
} connection_bt_state_e;


/**
 * @brief Enumeration for connection iterator type.
 * @since_tizen 2.3
 */
typedef enum {
	CONNECTION_ITERATOR_TYPE_REGISTERED = 0, /**< The iterator of the registered profile */
	CONNECTION_ITERATOR_TYPE_CONNECTED = 1,  /**< The iterator of the connected profile */
	CONNECTION_ITERATOR_TYPE_DEFAULT = 2,    /**< The iterator of the default profile */
} connection_iterator_type_e;


/**
 * @brief Enumeration for reset profile type.
 * @since_tizen 2.3
*/
typedef enum {
	/**
	 * Initialized with the default profile defined by csc
	 */
	CONNECTION_RESET_DEFAULT_PROFILE = 0,

	/**
	 * Remove all profiles
	 */
	CONNECTION_RESET_CLEAR_PROFILE = 1,

} connection_reset_option_e;


/**
 * @brief Enumeration for the attached or detached state of ethernet cable.
 * @since_tizen 2.4
 */
typedef enum {
	CONNECTION_ETHERNET_CABLE_DETACHED = 0,   /**< Ethernet cable is detached */
	CONNECTION_ETHERNET_CABLE_ATTACHED = 1,   /**< Ethernet cable is attached */
} connection_ethernet_cable_state_e;

/**
 * @brief Enumeration for the DHCP state.
 * @since_tizen 8.0
 */
typedef enum {
	CONNECTION_DHCP_STATE_UNKNOWN = 0,  /**< Unknown state */
	CONNECTION_DHCP_STATE_STARTED = 1,  /**< DHCP is running */
	CONNECTION_DHCP_STATE_FINISHED = 2, /**< DHCP is finished */
} connection_dhcp_state_e;

/**
 * @brief Enumeration for connection errors.
 * @since_tizen 2.3
 */
typedef enum {
	/**
	 * Successful
	 */
	CONNECTION_ERROR_NONE = TIZEN_ERROR_NONE,

	/**
	 * Invalid parameter
	 */
	CONNECTION_ERROR_INVALID_PARAMETER = TIZEN_ERROR_INVALID_PARAMETER,

	/**
	 * Out of memory error
	 */
	CONNECTION_ERROR_OUT_OF_MEMORY = TIZEN_ERROR_OUT_OF_MEMORY,

	/**
	 * Invalid operation
	 */
	CONNECTION_ERROR_INVALID_OPERATION = TIZEN_ERROR_INVALID_OPERATION,

	/**
	 * Address family not supported
	 */
	CONNECTION_ERROR_ADDRESS_FAMILY_NOT_SUPPORTED = TIZEN_ERROR_ADDRESS_FAMILY_NOT_SUPPORTED,

	/**
	 * Now inprogress
	 */
	CONNECTION_ERROR_NOW_IN_PROGRESS = TIZEN_ERROR_NOW_IN_PROGRESS,

	/**
	 * Permission denied
	 */
	CONNECTION_ERROR_PERMISSION_DENIED = TIZEN_ERROR_PERMISSION_DENIED,

	/**
	 * Not supported
	 */
	CONNECTION_ERROR_NOT_SUPPORTED = TIZEN_ERROR_NOT_SUPPORTED,

	/**
	 * Operation failed
	 */
	CONNECTION_ERROR_OPERATION_FAILED = TIZEN_ERROR_CONNECTION|0x0401,

	/**
	 * End of iteration
	 */
	CONNECTION_ERROR_ITERATOR_END = TIZEN_ERROR_CONNECTION|0x0402,

	/**
	 * There is no connection
	 */
	CONNECTION_ERROR_NO_CONNECTION = TIZEN_ERROR_CONNECTION|0x0403,

	/**
	 * Already exists
	 */
	CONNECTION_ERROR_ALREADY_EXISTS = TIZEN_ERROR_CONNECTION|0x0404,

	/**
	 * Operation is aborted
	 */
	CONNECTION_ERROR_OPERATION_ABORTED = TIZEN_ERROR_CONNECTION|0x0405,

	/**
	 * DHCP failed
	 */
	CONNECTION_ERROR_DHCP_FAILED = TIZEN_ERROR_CONNECTION|0x0406,

	/**
	 * Invalid key
	 */
	CONNECTION_ERROR_INVALID_KEY = TIZEN_ERROR_CONNECTION|0x0407,

	/**
	 * No reply
	 */
	CONNECTION_ERROR_NO_REPLY = TIZEN_ERROR_CONNECTION|0x0408,

	/**
	 * Already initialized (Since 5.0)
	 */
	CONNECTION_ERROR_ALREADY_INITIALIZED = TIZEN_ERROR_CONNECTION|0x0409,

	/**
	 * Not initialized (Since 5.0)
	 */
	CONNECTION_ERROR_NOT_INITIALIZED = TIZEN_ERROR_CONNECTION|0x040A,

} connection_error_e;

/**
 * @}
*/


/**
 * @addtogroup CAPI_NETWORK_CONNECTION_STATISTICS_MODULE
 * @{
*/


/**
 * @brief Enumeration for statistics type.
 * @since_tizen 2.3
 */
typedef enum {
	CONNECTION_STATISTICS_TYPE_LAST_RECEIVED_DATA = 0,  /**< Last received data */
	CONNECTION_STATISTICS_TYPE_LAST_SENT_DATA = 1,      /**< Last sent data */
	CONNECTION_STATISTICS_TYPE_TOTAL_RECEIVED_DATA = 2, /**< Total received data */
	CONNECTION_STATISTICS_TYPE_TOTAL_SENT_DATA = 3,     /**< Total sent data */
} connection_statistics_type_e;


/**
 * @}
*/


/**
 * @addtogroup CAPI_NETWORK_CONNECTION_MANAGER_MODULE
 * @{
*/


/**
 * @brief Creates a handle for managing data connections.
 * @details If you do not use this function and use other functions,
 *          you will get the #CONNECTION_ERROR_NOT_INITIALIZED error.
 *          If you put an invalid handle, you will get the #CONNECTION_ERROR_INVALID_PARAMETER error.
 * @since_tizen 2.3
 * @privlevel public
 * @privilege %http://tizen.org/privilege/network.get
 * @remarks You must release @a connection using connection_destroy().
 * @param[out] connection       The connection handle
 * @return @c 0 on success,
 *         otherwise a negative error value
 * @retval #CONNECTION_ERROR_NONE                  Successful
 * @retval #CONNECTION_ERROR_ALREADY_INITIALIZED   Already initialized
 * @retval #CONNECTION_ERROR_INVALID_PARAMETER     Invalid parameter
 * @retval #CONNECTION_ERROR_OUT_OF_MEMORY         Out of memory
 * @retval #CONNECTION_ERROR_PERMISSION_DENIED     Permission denied
 * @retval #CONNECTION_ERROR_NOT_SUPPORTED         Not supported
 * @see connection_destroy()
 */
int connection_create(connection_h* connection);


/**
 * @brief Destroys the connection handle.
 * @details This function releases all resources allocated for the given connection handle. \n
 *          should be called when the connection is no longer needed to prevent memory leaks and other potential issues.
 * @since_tizen 2.3
 * @param[in] connection        The connection handle
 * @return @c 0 on success,
 *         otherwise a negative error value
 * @retval #CONNECTION_ERROR_NONE                  Successful
 * @retval #CONNECTION_ERROR_NOT_INITIALIZED       Not initialized
 * @retval #CONNECTION_ERROR_INVALID_PARAMETER     Invalid parameter
 * @retval #CONNECTION_ERROR_NOT_SUPPORTED         Not supported
 * @see connection_create()
 */
int connection_destroy(connection_h connection);


/**
 * @brief Called when the type of a connection is changed.
 * @details This callback function is invoked when the type of the network connection changes. \n
 *          It provides information about the connection type changes and allows you to handle the change accordingly.
 * @since_tizen 2.3
 * @param[in] type              The type of the current network connection
 * @param[in] user_data         The user data passed from the callback registration function
 * @see connection_set_type_changed_cb()
 * @see connection_unset_type_changed_cb()
 */
typedef void(*connection_type_changed_cb)(connection_type_e type, void* user_data);


/**
 * @brief Called when the address is changed.
 * @details This callback function is called when the IP address for IPv4 or IPv6 changes. \n
 *          The addresses provided within this callback are valid only during the execution of the callback. \n
 *          If you need to use these addresses outside of the callback, you must make a copy of them.
 * @since_tizen 2.3
 * @remarks @a ipv4_address @a ipv6_address should not be freed.
 *          @a ipv4_address @a ipv6_address is available only in the callback.
 *          To use outside the callback, make a copy.
 * @param[in] ipv4_address      The IP address for IPv4
 * @param[in] ipv6_address      The IP address for IPv6
 * @param[in] user_data         The user data passed from the callback registration function
 * @see connection_set_ip_address_changed_cb()
 * @see connection_unset_ip_address_changed_cb()
 * @see connection_set_proxy_address_changed_cb()
 * @see connection_unset_proxy_address_changed_cb()
 */
typedef void(*connection_address_changed_cb)(const char* ipv4_address,
		const char* ipv6_address, void* user_data);


/**
 * @brief Called when connection_set_default_cellular_service_profile_async() finishes.
 * @details This callback function is invoked after the asynchronous operation of \n
 *          setting a default cellular service profile is completed. \n
 *          It provides the result of the operation and any user data that was passed \n
 *          during the invocation of connection_set_default_cellular_service_profile_async().
 * @since_tizen 2.3
 * @param[in] result            The result
 * @param[in] user_data         The user data passed from connection_open_profile()
 * @pre connection_set_default_cellular_service_profile_async() will invoke this callback function.
 * @see connection_set_default_cellular_service_profile_async()
*/
typedef void(*connection_set_default_cb)(connection_error_e result, void* user_data);

/**
 * @brief Called with an IPv6 address.
 * @details This callback function is invoked when iterating through the list of IPv6 addresses associated with a network interface. \n
 *          It provides each IPv6 address as a character string and allows the caller to perform custom operations on each address.
 * @since_tizen 4.0
 * @remarks   If @a ipv6_address is needed outside the callback, a copy should be made. \n
 *            @a ipv6_address will be freed automatically after the execution of this callback.
 * @param[in]  ipv6_address     The IPv6 address
 * @param[in]  user_data        The user data passed from the foreach function
 * @return  @c true to continue with the next iteration of the loop, \n
 *          @c false to break out of the loop
 * @pre  connection_foreach_ipv6_address() will invoke this callback.
 * @see  connection_foreach_ipv6_address()
 */
typedef bool(*connection_ipv6_address_cb)(char *ipv6_address, void *user_data);

/**
 * @brief Called when the DHCP state is changed.
 * @details This callback function is invoked when the DHCP state changes for a given network interface. \n
 *          It provides information about DHCP state changes, the network interface associated with the state change, \n
 *          and the result of the DHCP operation.
 * @since_tizen 8.0
 * @param[in]  state            The DHCP state
 * @param[in]  interface_name   The interface name with DHCP state change
 * @param[in]  result           The result of DHCP operation
 * @param[in]  user_data        The user data passed from the callback registration function
 * @see  connection_set_dhcp_state_changed_cb()
 * @see  connection_unset_dhcp_state_changed_cb()
 */
typedef void(*connection_dhcp_state_changed_cb)(connection_dhcp_state_e state,
            const char *interface_name, connection_error_e result, void *user_data);

/**
 * @brief Gets the type of the current profile for data connection.
 * @details This function retrieves the type of the network currently being used for data connection. \n
 *          It returns an integer value representing the type of the network.
 * @since_tizen 2.3
 * @param[in] connection        The connection handle
 * @param[out] type             The type of the network
 * @return @c 0 on success,
 *         otherwise a negative error value
 * @retval #CONNECTION_ERROR_NONE                  Successful
 * @retval #CONNECTION_ERROR_NOT_INITIALIZED       Not initialized
 * @retval #CONNECTION_ERROR_INVALID_PARAMETER     Invalid parameter
 * @retval #CONNECTION_ERROR_OPERATION_FAILED      Operation failed
 * @retval #CONNECTION_ERROR_NOT_SUPPORTED         Not supported
 */
int connection_get_type(connection_h connection, connection_type_e* type);


/**
 * @brief Gets the IP address of the current connection.
 * @details This function retrieves the IP address associated with the network currently being used for data connection and address family. \n
 *          The retrieved IP address should be released using the free() function.
 * @since_tizen 2.3
 * @remarks You must release @a ip_address using free().
 * @param[in] connection        The connection handle
 * @param[in] address_family    The address family
 * @param[out] ip_address       The pointer to the IP address string
 * @return @c 0 on success,
 *         otherwise a negative error value
 * @retval #CONNECTION_ERROR_NONE                           Successful
 * @retval #CONNECTION_ERROR_NOT_INITIALIZED                Not initialized
 * @retval #CONNECTION_ERROR_INVALID_PARAMETER              Invalid parameter
 * @retval #CONNECTION_ERROR_OPERATION_FAILED               Operation failed
 * @retval #CONNECTION_ERROR_ADDRESS_FAMILY_NOT_SUPPORTED   Not supported address family
 * @retval #CONNECTION_ERROR_NOT_SUPPORTED         Not supported
 */
int connection_get_ip_address(connection_h connection,
		connection_address_family_e address_family, char** ip_address);


/**
 * @brief Gets the proxy address of the current connection.
 * @details This function retrieves the proxy address for the specified address family associated with the network currently being used for data connection. \n
 *          The retrieved proxy address should be released using free() after use.
 * @since_tizen 2.3
 * @remarks You must release @a proxy using free().
 * @param[in] connection        The connection handle
 * @param[in] address_family    The address family
 * @param[out] proxy            The proxy address
 * @return @c 0 on success,
 *         otherwise a negative error value
 * @retval #CONNECTION_ERROR_NONE                           Successful
 * @retval #CONNECTION_ERROR_NOT_INITIALIZED                Not initialized
 * @retval #CONNECTION_ERROR_INVALID_PARAMETER              Invalid parameter
 * @retval #CONNECTION_ERROR_OPERATION_FAILED               Operation failed
 * @retval #CONNECTION_ERROR_ADDRESS_FAMILY_NOT_SUPPORTED   Not supported address family
 * @retval #CONNECTION_ERROR_NOT_SUPPORTED         Not supported
 */
int connection_get_proxy(connection_h connection,
		connection_address_family_e address_family, char** proxy);


/**
 * @brief Gets the MAC address of the Wi-Fi or ethernet.
 * @details This function retrieves the MAC address of the specified network connection type (Wi-Fi or Ethernet). \n
 *          The caller is responsible for releasing the memory allocated for the MAC address using free().
 * @since_tizen 2.4
 * @remarks @a mac_addr must be released with free() by you.
 * @param[in] connection        The handle of the connection
 * @param[in] type              The type of current network connection
 * @param[out] mac_addr         The MAC address
 * @return @c 0 on success,
 *         otherwise a negative error value
 * @retval #CONNECTION_ERROR_NONE                  Successful
 * @retval #CONNECTION_ERROR_NOT_INITIALIZED       Not initialized
 * @retval #CONNECTION_ERROR_INVALID_PARAMETER     Invalid parameter
 * @retval #CONNECTION_ERROR_INVALID_OPERATION     Invalid operation
 * @retval #CONNECTION_ERROR_OPERATION_FAILED      Operation failed
 * @retval #CONNECTION_ERROR_NOT_SUPPORTED         Not supported
 * @retval #CONNECTION_ERROR_OUT_OF_MEMORY         Out of memory
 */
int connection_get_mac_address(connection_h connection,
		connection_type_e type, char** mac_addr);


/**
 * @brief Gets if the current connection is metered.
 * @details This function retrieves information about whether the current network connection is metered or not. \n
 *          A metered connection is typically one where data usage may be limited or charged for, such as a mobile network.
 * @since_tizen 4.0
 * @privlevel public
 * @privilege %http://tizen.org/privilege/network.get
 * @param[in] connection        The connection handle
 * @param[out] is_metered       The value indicating whether it is metered
 * @return @c 0 on success,
 *         otherwise a negative error value
 * @retval #CONNECTION_ERROR_NONE                  Successful
 * @retval #CONNECTION_ERROR_NOT_INITIALIZED       Not initialized
 * @retval #CONNECTION_ERROR_INVALID_PARAMETER     Invalid parameter
 * @retval #CONNECTION_ERROR_OPERATION_FAILED      Operation failed
 * @retval #CONNECTION_ERROR_PERMISSION_DENIED     Permission denied
 * @retval #CONNECTION_ERROR_NOT_SUPPORTED         Not supported
 */
int connection_is_metered_network(connection_h connection, bool* is_metered);


/**
 * @brief Gets the state of cellular connection.
 * @details This function retrieves the current state of the cellular connection. \n
 *          The state is returned as an enumeration value of the `connection_cellular_state_e` type.
 * @since_tizen 2.3
 * @param[in] connection        The connection handle
 * @param[out] state            The state of the cellular connection
 * @return @c 0 on success,
 *         otherwise a negative error value
 * @retval #CONNECTION_ERROR_NONE                  Successful
 * @retval #CONNECTION_ERROR_NOT_INITIALIZED       Not initialized
 * @retval #CONNECTION_ERROR_INVALID_PARAMETER     Invalid parameter
 * @retval #CONNECTION_ERROR_OPERATION_FAILED      Operation failed
 * @retval #CONNECTION_ERROR_NOT_SUPPORTED         Not supported
 */
int connection_get_cellular_state(connection_h connection, connection_cellular_state_e* state);


/**
 * @brief Gets the state of the Wi-Fi.
 * @details This function retrieves the current state of the Wi-Fi connection. \n
 *          This information can be used to determine the status of the Wi-Fi network connection.
 * @since_tizen 2.3
 * @privlevel public
 * @privilege %http://tizen.org/privilege/network.get
 * @param[in] connection        The connection handle
 * @param[out] state            The state of Wi-Fi connection
 * @return @c 0 on success,
 *         otherwise a negative error value
 * @retval #CONNECTION_ERROR_NONE                  Successful
 * @retval #CONNECTION_ERROR_NOT_INITIALIZED       Not initialized
 * @retval #CONNECTION_ERROR_INVALID_PARAMETER     Invalid parameter
 * @retval #CONNECTION_ERROR_OPERATION_FAILED      Operation failed
 * @retval #CONNECTION_ERROR_PERMISSION_DENIED     Permission denied
 * @retval #CONNECTION_ERROR_NOT_SUPPORTED         Not supported
 */
int connection_get_wifi_state(connection_h connection, connection_wifi_state_e* state);


/**
 * @brief Gets the state of the Ethernet.
 * @details This function retrieves the current state of the Ethernet connection.
 * @since_tizen 2.4
 * @privlevel public
 * @privilege %http://tizen.org/privilege/network.get
 * @param[in] connection        The connection handle
 * @param[out] state            The state of Ethernet connection
 * @return @c 0 on success,
 *         otherwise a negative error value
 * @retval #CONNECTION_ERROR_NONE                  Successful
 * @retval #CONNECTION_ERROR_NOT_INITIALIZED       Not initialized
 * @retval #CONNECTION_ERROR_INVALID_PARAMETER     Invalid parameter
 * @retval #CONNECTION_ERROR_OPERATION_FAILED      Operation failed
 * @retval #CONNECTION_ERROR_PERMISSION_DENIED     Permission denied
 * @retval #CONNECTION_ERROR_NOT_SUPPORTED         Not supported
 */
int connection_get_ethernet_state(connection_h connection, connection_ethernet_state_e* state);


/**
 * @brief Called when the Internet state of the current connection is changed.
 * @details This callback function is invoked when the Internet state of the current connection changes. \n
 *          It provides information about the new state of the connection and allows you to handle any necessary actions based on the updated state.
 * @since_tizen 5.5
 * @param[in] state             The Internet state of current connection
 * @param[in] user_data         The user data passed to callback registration function
 */
typedef void(*connection_internet_state_changed_cb)(
		connection_internet_state_e state, void* user_data);

/**
 * @brief Sets the callback that is called when the Internet availability over the current connection is changed.
 * @details This function sets a callback function to receive notifications about changes in the Internet availability status of the connection. \n
 *          When the Internet availability state changes, the provided callback function will be invoked with the new state \n
 *          and the user data supplied during the registration.
 * @since_tizen 5.5
 * @param[in] connection        The connection handle
 * @param[in] callback          The callback function to be called
 * @param[in] user_data         The user data passed to the callback function
 * @return @c 0 on success,
 *         otherwise a negative error value
 * @retval #CONNECTION_ERROR_NONE                  Successful
 * @retval #CONNECTION_ERROR_NOT_INITIALIZED       Not initialized
 * @retval #CONNECTION_ERROR_INVALID_PARAMETER     Invalid parameter
 * @retval #CONNECTION_ERROR_OPERATION_FAILED      Operation failed
 * @retval #CONNECTION_ERROR_NOT_SUPPORTED         Not supported
 */
int connection_set_internet_state_changed_cb(connection_h connection,
		connection_internet_state_changed_cb callback, void *user_data);


/**
 * @brief Unsets the callback that is called when the Internet state of the current connection is changed.
 * @details This function removes the previously set callback for monitoring changes in the Internet state of the connection. \n
 *          After calling this function, the callback will no longer be invoked when the Internet state changes.
 * @since_tizen 5.5
 * @param[in] connection        The connection handle
 * @return @c 0 on success,
 *         otherwise a negative error value
 * @retval #CONNECTION_ERROR_NONE                  Successful
 * @retval #CONNECTION_ERROR_NOT_INITIALIZED       Not initialized
 * @retval #CONNECTION_ERROR_INVALID_PARAMETER     Invalid parameter
 * @retval #CONNECTION_ERROR_OPERATION_FAILED      Operation failed
 * @retval #CONNECTION_ERROR_NOT_SUPPORTED         Not supported
 */
int connection_unset_internet_state_changed_cb(connection_h connection);

/**
 * @brief Checks for ethernet cable is attached or not.
 * @details This function retrieves the current state of the ethernet cable connected to the device. \n
 *          The possible states include whether the cable is attached or detached.
 * @since_tizen 2.4
 * @privlevel public
 * @privilege %http://tizen.org/privilege/network.get
 * @param[in] connection        The handle of the connection
 * @param[in] state             The state of ethernet cable
 * @return @c 0 on success,
 *         otherwise a negative error value
 * @retval #CONNECTION_ERROR_NONE                  Successful
 * @retval #CONNECTION_ERROR_NOT_INITIALIZED       Not initialized
 * @retval #CONNECTION_ERROR_INVALID_PARAMETER     Invalid parameter
 * @retval #CONNECTION_ERROR_OPERATION_FAILED      Operation failed
 * @retval #CONNECTION_ERROR_NOT_SUPPORTED         Not supported
 * @retval #CONNECTION_ERROR_PERMISSION_DENIED     Permission denied
 */
int connection_get_ethernet_cable_state(connection_h connection,
		connection_ethernet_cable_state_e *state);


/**
 * @brief Gets the DHCP state of the interface.
 * @details This function retrieves the DHCP state of the network currently being used for data connection. \n
 *          The DHCP state indicates whether the interface has started to obtained an IP address via DHCP or not.
 * @since_tizen 8.0
 * @privlevel public
 * @privilege %http://tizen.org/privilege/network.get
 * @param[in] connection        The connection handle
 * @param[in] interface_name    The interface name
 * @param[out] state            The DHCP state of the interface
 * @return @c 0 on success,
 *         otherwise a negative error value
 * @retval #CONNECTION_ERROR_NONE                  Successful
 * @retval #CONNECTION_ERROR_INVALID_PARAMETER     Invalid parameter
 * @retval #CONNECTION_ERROR_OPERATION_FAILED      Operation failed
 * @retval #CONNECTION_ERROR_PERMISSION_DENIED     Permission denied
 * @retval #CONNECTION_ERROR_NOT_SUPPORTED         Not supported
 */
int connection_get_dhcp_state(connection_h connection,
		const char *interface_name, connection_dhcp_state_e *state);


/**
 * @deprecated Deprecated since 4.0. Use connection_ethernet_cable_state_changed_cb() instead.
 * @brief Called when ethernet cable is plugged [in/out].
 * @since_tizen 2.4
 * @param[in] state             The state of ethernet cable
 * @param[in] user_data         The user data passed to callback registration function
 */
typedef void(*connection_ethernet_cable_state_chaged_cb)
		(connection_ethernet_cable_state_e state, void* user_data);


/**
 * @deprecated Deprecated since 4.0. Use connection_set_ethernet_cable_state_changed_cb() instead.
 * @brief Sets callback for ethernet cable is plugged [in/out] event.
 * @since_tizen 2.4
 * @param[in] connection        The connection handle
 * @param[in] callback          The callback function to be called
 * @param[in] user_data         The user data passed to the callback function
 * @return @c 0 on success,
 *         otherwise a negative error value
 * @retval #CONNECTION_ERROR_NONE                  Successful
 * @retval #CONNECTION_ERROR_NOT_INITIALIZED       Not initialized
 * @retval #CONNECTION_ERROR_INVALID_PARAMETER     Invalid parameter
 * @retval #CONNECTION_ERROR_OPERATION_FAILED      Operation failed
 * @retval #CONNECTION_ERROR_NOT_SUPPORTED         Not supported
 */
int connection_set_ethernet_cable_state_chaged_cb(connection_h connection,
		connection_ethernet_cable_state_chaged_cb callback, void *user_data) TIZEN_DEPRECATED_API;


/**
 * @deprecated Deprecated since 4.0. Use connection_unset_ethernet_cable_state_changed_cb() instead.
 * @brief Unsets callback for ethernet cable is plugged [in/out] event.
 * @since_tizen 2.4
 * @param[in] connection        The connection handle
 * @return @c 0 on success,
 *         otherwise a negative error value
 * @retval #CONNECTION_ERROR_NONE                  Successful
 * @retval #CONNECTION_ERROR_NOT_INITIALIZED       Not initialized
 * @retval #CONNECTION_ERROR_INVALID_PARAMETER     Invalid parameter
 * @retval #CONNECTION_ERROR_OPERATION_FAILED      Operation failed
 * @retval #CONNECTION_ERROR_NOT_SUPPORTED         Not supported
 */
int connection_unset_ethernet_cable_state_chaged_cb(connection_h connection) TIZEN_DEPRECATED_API;

/**
 * @brief Called when ethernet cable is plugged [in/out].
 * @details This callback function is invoked when an Ethernet cable is either plugged in or unplugged.
 * @since_tizen 4.0
 * @param[in] state             The state of ethernet cable
 * @param[in] user_data         The user data passed to callback registration function
 */
typedef void(*connection_ethernet_cable_state_changed_cb)(
		connection_ethernet_cable_state_e state, void* user_data);


/**
 * @brief Sets callback for ethernet cable is plugged [in/out] event.
 * @details This function registers a callback function to be invoked when the state of the ethernet cable changes. \n
 *          It allows you to monitor whether the ethernet cable is plugged in or out.
 * @since_tizen 4.0
 * @param[in] connection        The connection handle
 * @param[in] callback          The callback function to be called
 * @param[in] user_data         The user data passed to the callback function
 * @return @c 0 on success,
 *         otherwise a negative error value
 * @retval #CONNECTION_ERROR_NONE                  Successful
 * @retval #CONNECTION_ERROR_NOT_INITIALIZED       Not initialized
 * @retval #CONNECTION_ERROR_INVALID_PARAMETER     Invalid parameter
 * @retval #CONNECTION_ERROR_OPERATION_FAILED      Operation failed
 * @retval #CONNECTION_ERROR_NOT_SUPPORTED         Not supported
 */
int connection_set_ethernet_cable_state_changed_cb(connection_h connection,
		connection_ethernet_cable_state_changed_cb callback, void *user_data);


/**
 * @brief Unsets callback for ethernet cable is plugged [in/out] event.
 * @details This function removes the callback that was previously set to receive notifications when the ethernet cable is plugged in or out.
 * @since_tizen 4.0
 * @param[in] connection        The connection handle
 * @return @c 0 on success,
 *         otherwise a negative error value
 * @retval #CONNECTION_ERROR_NONE                  Successful
 * @retval #CONNECTION_ERROR_NOT_INITIALIZED       Not initialized
 * @retval #CONNECTION_ERROR_INVALID_PARAMETER     Invalid parameter
 * @retval #CONNECTION_ERROR_OPERATION_FAILED      Operation failed
 * @retval #CONNECTION_ERROR_NOT_SUPPORTED         Not supported
 */
int connection_unset_ethernet_cable_state_changed_cb(connection_h connection);


/**
 * @brief Gets the state of the Bluetooth.
 * @details This function retrieves the current state of the Bluetooth connection. \n
 *          Possible states include disconnected, connected, and deactivated.
 * @since_tizen 2.3
 * @privlevel public
 * @privilege %http://tizen.org/privilege/network.get
 * @param[in] connection        The connection handle
 * @param[out] state            The state of the Bluetooth connection
 * @return @c 0 on success,
 *         otherwise a negative error value
 * @retval #CONNECTION_ERROR_NONE                  Successful
 * @retval #CONNECTION_ERROR_NOT_INITIALIZED       Not initialized
 * @retval #CONNECTION_ERROR_INVALID_PARAMETER     Invalid parameter
 * @retval #CONNECTION_ERROR_OPERATION_FAILED      Operation failed
 * @retval #CONNECTION_ERROR_PERMISSION_DENIED     Permission denied
 * @retval #CONNECTION_ERROR_NOT_SUPPORTED         Not supported
 */
int connection_get_bt_state(connection_h connection, connection_bt_state_e* state);


/**
 * @brief Sets the callback that is called when the type of the current connection is changed.
 * @details This function registers a callback function to be invoked when the type of the current network connection changes. \n
 *          The registered callback will be called with user data provided during registration whenever the connection type changes.
 * @since_tizen 2.3
 * @param[in] connection        The connection handle
 * @param[in] callback          The callback function to be called
 * @param[in] user_data         The user data passed to the callback function
 * @return @c 0 on success,
 *         otherwise a negative error value
 * @retval #CONNECTION_ERROR_NONE                  Successful
 * @retval #CONNECTION_ERROR_NOT_INITIALIZED       Not initialized
 * @retval #CONNECTION_ERROR_INVALID_PARAMETER     Invalid parameter
 * @retval #CONNECTION_ERROR_OPERATION_FAILED      Operation failed
 * @retval #CONNECTION_ERROR_NOT_SUPPORTED         Not supported
 */
int connection_set_type_changed_cb(connection_h connection,
		connection_type_changed_cb callback, void* user_data);


/**
 * @brief Unsets the callback that is called when the type of current connection is changed.
 * @details This function removes the callback function that was previously registered using connection_set_type_changed_cb(). \n
 *          After calling this function, the connection_type_changed_cb callback will no longer be called when the connection type changes.
 * @since_tizen 2.3
 * @param[in] connection        The connection handle
 * @return @c 0 on success,
 *         otherwise a negative error value
 * @retval #CONNECTION_ERROR_NONE                  Successful
 * @retval #CONNECTION_ERROR_NOT_INITIALIZED       Not initialized
 * @retval #CONNECTION_ERROR_INVALID_PARAMETER     Invalid parameter
 * @retval #CONNECTION_ERROR_OPERATION_FAILED      Operation failed
 * @retval #CONNECTION_ERROR_NOT_SUPPORTED         Not supported
 */
int connection_unset_type_changed_cb(connection_h connection);


/**
 * @brief Sets the callback that is called when the IP address is changed.
 * @details This function registers a callback function to be invoked when the IP address of the connection changes.
 * @since_tizen 2.3
 * @param[in] connection        The connection handle
 * @param[in] callback          The callback function to be called
 * @param[in] user_data         The user data passed to the callback function
 * @return @c 0 on success,
 *         otherwise a negative error value
 * @retval #CONNECTION_ERROR_NONE                  Successful
 * @retval #CONNECTION_ERROR_NOT_INITIALIZED       Not initialized
 * @retval #CONNECTION_ERROR_INVALID_PARAMETER     Invalid parameter
 * @retval #CONNECTION_ERROR_OPERATION_FAILED      Operation failed
 * @retval #CONNECTION_ERROR_NOT_SUPPORTED         Not supported
 */
int connection_set_ip_address_changed_cb(connection_h connection,
		connection_address_changed_cb callback, void* user_data);


/**
 * @brief Unsets the callback that is called when the IP address is changed.
 * @details This function removes the callback function that was previously set to be called when the IP address changes for the connection. \n
 *          After calling this function, no further notifications will be sent about IP address changes.
 * @since_tizen 2.3
 * @param[in] connection        The connection handle
 * @return @c 0 on success,
 *         otherwise a negative error value
 * @retval #CONNECTION_ERROR_NONE                  Successful
 * @retval #CONNECTION_ERROR_NOT_INITIALIZED       Not initialized
 * @retval #CONNECTION_ERROR_INVALID_PARAMETER     Invalid parameter
 * @retval #CONNECTION_ERROR_OPERATION_FAILED      Operation failed
 * @retval #CONNECTION_ERROR_NOT_SUPPORTED         Not supported
 */
int connection_unset_ip_address_changed_cb(connection_h connection);


/**
 * @brief Sets the callback that is called when the proxy address is changed.
 * @details This function registers a callback function to be invoked when the proxy address changes for the connection.
 * @since_tizen 2.3
 * @param[in] connection        The connection handle
 * @param[in] callback          The callback function to be called
 * @param[in] user_data         The user data passed to the callback function
 * @return @c 0 on success,
 *         otherwise a negative error value
 * @retval #CONNECTION_ERROR_NONE                  Successful
 * @retval #CONNECTION_ERROR_NOT_INITIALIZED       Not initialized
 * @retval #CONNECTION_ERROR_INVALID_PARAMETER     Invalid parameter
 * @retval #CONNECTION_ERROR_OPERATION_FAILED      Operation failed
 * @retval #CONNECTION_ERROR_NOT_SUPPORTED         Not supported
 */
int connection_set_proxy_address_changed_cb(connection_h connection,
		connection_address_changed_cb callback, void* user_data);


/**
 * @brief Unsets the callback that is called when the proxy address is changed.
 * @details This function removes the callback function that was previously set using connection_set_proxy_address_changed_cb(). \n
 *          After calling this function, the proxy address change events will no longer be delivered to the application.
 * @since_tizen 2.3
 * @param[in] connection        The connection handle
 * @return @c 0 on success,
 *         otherwise a negative error value
 * @retval #CONNECTION_ERROR_NONE                  Successful
 * @retval #CONNECTION_ERROR_NOT_INITIALIZED       Not initialized
 * @retval #CONNECTION_ERROR_INVALID_PARAMETER     Invalid parameter
 * @retval #CONNECTION_ERROR_OPERATION_FAILED      Operation failed
 * @retval #CONNECTION_ERROR_NOT_SUPPORTED         Not supported
 */
int connection_unset_proxy_address_changed_cb(connection_h connection);


/**
 * @brief Sets the callback that is called when the DHCP state is changed.
 * @details This function registers a callback function to receive notifications whenever the DHCP state changes for the connection. \n
 *          When the DHCP state changes, the registered callback function will be invoked with the new DHCP state and the provided user data.
 * @since_tizen 8.0
 * @param[in] connection        The connection handle
 * @param[in] callback          The callback function to be called
 * @param[in] user_data         The user data passed to the callback function
 * @return @c 0 on success,
 *         otherwise a negative error value
 * @retval #CONNECTION_ERROR_NONE                  Successful
 * @retval #CONNECTION_ERROR_NOT_INITIALIZED       Not initialized
 * @retval #CONNECTION_ERROR_INVALID_PARAMETER     Invalid parameter
 * @retval #CONNECTION_ERROR_OPERATION_FAILED      Operation failed
 * @retval #CONNECTION_ERROR_NOT_SUPPORTED         Not supported
 */
int connection_set_dhcp_state_changed_cb(connection_h connection,
		connection_dhcp_state_changed_cb callback, void *user_data);


/**
 * @brief Unsets the callback that is called when the DHCP state is changed.
 * @details This function removes the callback function that was previously registered to be invoked when the DHCP state changes.
 * @since_tizen 8.0
 * @param[in] connection        The connection handle
 * @return @c 0 on success,
 *         otherwise a negative error value
 * @retval #CONNECTION_ERROR_NONE                  Successful
 * @retval #CONNECTION_ERROR_NOT_INITIALIZED       Not initialized
 * @retval #CONNECTION_ERROR_INVALID_PARAMETER     Invalid parameter
 * @retval #CONNECTION_ERROR_OPERATION_FAILED      Operation failed
 * @retval #CONNECTION_ERROR_NOT_SUPPORTED         Not supported
 */
int connection_unset_dhcp_state_changed_cb(connection_h connection);


/**
 * @brief Adds a new profile which is created by connection_profile_create().
 * @details This function adds a new cellular profile to the profile list to be used for cellular connection. \n
 *          The profile must be created using connection_profile_create() and should be of the cellular type. \n
 *          The caller must have the necessary privileges to perform this operation.
 * @since_tizen 2.3
 * @privlevel public
 * @privilege %http://tizen.org/privilege/network.profile
 * @remarks You can only add a profile of the cellular type.
 * @param[in] connection        The connection handle
 * @param[in] profile           The profile handle
 * @return @c 0 on success,
 *         otherwise a negative error value
 * @retval #CONNECTION_ERROR_NONE                  Successful
 * @retval #CONNECTION_ERROR_NOT_INITIALIZED       Not initialized
 * @retval #CONNECTION_ERROR_INVALID_PARAMETER     Invalid parameter
 * @retval #CONNECTION_ERROR_OPERATION_FAILED      Operation failed
 * @retval #CONNECTION_ERROR_PERMISSION_DENIED     Permission denied
 * @retval #CONNECTION_ERROR_NOT_SUPPORTED         Not supported
 */
int connection_add_profile(connection_h connection, connection_profile_h profile);


/**
 * @brief Removes an existing profile.
 * @details This function removes the specified profile from the profile list.
 * @since_tizen 2.3
 * @privlevel public
 * @privilege %http://tizen.org/privilege/network.profile \n
 *            %http://tizen.org/privilege/network.get
 * @remarks This function needs both privileges.
 * @param[in] connection        The connection handle
 * @param[in] profile           The profile handle
 * @return @c 0 on success,
 *         otherwise a negative error value
 * @retval #CONNECTION_ERROR_NONE                  Successful
 * @retval #CONNECTION_ERROR_NOT_INITIALIZED       Not initialized
 * @retval #CONNECTION_ERROR_INVALID_PARAMETER     Invalid parameter
 * @retval #CONNECTION_ERROR_OPERATION_FAILED      Operation failed
 * @retval #CONNECTION_ERROR_PERMISSION_DENIED     Permission denied
 * @retval #CONNECTION_ERROR_NOT_SUPPORTED         Not supported
 */
int connection_remove_profile(connection_h connection, connection_profile_h profile);


/**
 * @brief Updates an existing profile.
 * @details When a profile is changed, these changes will be not applied to the
 *          Connection Manager immediately.
 *          When you call this function, your changes affect the Connection Manager
 *          and the existing profile is updated.
 *          In addition, the existing profile will be updated if you call connection_open_profile().
 * @since_tizen 2.3
 * @privlevel public
 * @privilege %http://tizen.org/privilege/network.profile \n
 *            %http://tizen.org/privilege/network.get
 * @remarks This function needs both privileges.
 * @param[in] connection        The connection handle
 * @param[in] profile           The profile handle
 * @return @c 0 on success,
 *         otherwise a negative error value
 * @retval #CONNECTION_ERROR_NONE                  Successful
 * @retval #CONNECTION_ERROR_NOT_INITIALIZED       Not initialized
 * @retval #CONNECTION_ERROR_INVALID_PARAMETER     Invalid parameter
 * @retval #CONNECTION_ERROR_OPERATION_FAILED      Operation failed
 * @retval #CONNECTION_ERROR_PERMISSION_DENIED     Permission denied
 * @retval #CONNECTION_ERROR_NOT_SUPPORTED         Not supported
 * @see connection_open_profile()
 */
int connection_update_profile(connection_h connection, connection_profile_h profile);


/**
 * @brief Gets a profiles iterator.
 * @details This function retrieves an iterator to enumerate all the profiles of a given type from the profile list. \n
 *          The caller is responsible for releasing the iterator using the connection_destroy_profile_iterator() function after it is no longer needed.
 * @since_tizen 2.3
 * @privlevel public
 * @privilege %http://tizen.org/privilege/network.get
 * @remarks You must release @a profile_iterator using connection_destroy_profile_iterator().
 * @param[in] connection        The connection handle
 * @param[in] type              The type of the connection iterator
 * @param[out] profile_iterator The iterator of profile
 * @return @c 0 on success,
 *         otherwise a negative error value
 * @retval #CONNECTION_ERROR_NONE                  Successful
 * @retval #CONNECTION_ERROR_NOT_INITIALIZED       Not initialized
 * @retval #CONNECTION_ERROR_INVALID_PARAMETER     Invalid parameter
 * @retval #CONNECTION_ERROR_OUT_OF_MEMORY         Out of memory
 * @retval #CONNECTION_ERROR_OPERATION_FAILED      Operation failed
 * @retval #CONNECTION_ERROR_PERMISSION_DENIED     Permission denied
 * @retval #CONNECTION_ERROR_NOT_SUPPORTED         Not supported
 */
int connection_get_profile_iterator(connection_h connection,
		connection_iterator_type_e type, connection_profile_iterator_h* profile_iterator);


/**
 * @brief Moves the profile iterator to the next position and gets a profile handle.
 * @details This function moves the given profile iterator to the next position and retrieves the profile handle at that position. \n
 *          If the end of the iteration is reached, the function returns CONNECTION_ERROR_ITERATOR_END. \n
 *          The retrieved profile handle will be automatically destroyed when the profile iterator is destroyed. \n
 *          To continue using the profile handle after the iterator is destroyed, make a copy of it.
 * @since_tizen 2.3
 * @remarks The @a profile will be destroyed automatically when the profile iterator is destroyed.
 *          Make a copy if you want to keep using it.
 * @param[in] profile_iterator  The iterator of profile
 * @param[out] profile          The profile handle
 * @return @c 0 on success,
 *         otherwise a negative error value
 * @retval #CONNECTION_ERROR_NONE                  Successful
 * @retval #CONNECTION_ERROR_NOT_INITIALIZED       Not initialized
 * @retval #CONNECTION_ERROR_INVALID_PARAMETER     Invalid parameter
 * @retval #CONNECTION_ERROR_ITERATOR_END          End of iteration
 * @retval #CONNECTION_ERROR_NOT_SUPPORTED         Not supported
 */
int connection_profile_iterator_next(connection_profile_iterator_h profile_iterator,
		connection_profile_h* profile);


/**
 * @brief Checks whether the next element of a profile iterator exists or not.
 * @details This function checks if there is another profile available to iterate over using the given profile iterator handle. \n
 *          If the next element exists, it returns true; otherwise, it returns false. \n
 *          The specific error code can be obtained using the get_last_result() method. \n
 *          Error codes are described in Exception section.
 * @since_tizen 2.3
 * @remarks The specific error code can be obtained using the get_last_result() method.
 *          Error codes are described in Exception section.
 * @param[in] profile_iterator  The iterator of profile
 * @return @c true if next element exists,
 *         otherwise @c false if next element doesn't exist
 * @exception #CONNECTION_ERROR_NONE                  Successful
 * @retval #CONNECTION_ERROR_NOT_INITIALIZED          Not initialized
 * @exception #CONNECTION_ERROR_NOT_SUPPORTED         Not supported
 * @see get_last_result()
 */
bool connection_profile_iterator_has_next(connection_profile_iterator_h profile_iterator);


/**
 * @brief Destroys a profiles iterator.
 * @details This function destroys the given profile iterator handle and releases all its resources. \n
 *          After calling this function, the profile iterator handle becomes invalid, and you cannot use it to perform any operations.
 * @since_tizen 2.3
 * @param[in] profile_iterator  The iterator of the profile
 * @return @c 0 on success,
 *         otherwise a negative error value
 * @retval #CONNECTION_ERROR_NONE                  Successful
 * @retval #CONNECTION_ERROR_NOT_INITIALIZED       Not initialized
 * @retval #CONNECTION_ERROR_INVALID_PARAMETER     Invalid parameter
 * @retval #CONNECTION_ERROR_NOT_SUPPORTED         Not supported
 */
int connection_destroy_profile_iterator(connection_profile_iterator_h profile_iterator);


/**
 * @brief Gets the name of the default profile.
 * @details This function retrieves the default network profile currently in use by the system.
 * @since_tizen 2.3
 * @privlevel public
 * @privilege %http://tizen.org/privilege/network.get
 * @remarks You must release @a profile using connection_profile_destroy().
 * @param[in] connection        The connection handle
 * @param[out] profile          The profile handle
 * @return @c 0 on success,
 *         otherwise a negative error value
 * @retval #CONNECTION_ERROR_NONE                  Successful
 * @retval #CONNECTION_ERROR_NOT_INITIALIZED       Not initialized
 * @retval #CONNECTION_ERROR_INVALID_PARAMETER     Invalid parameter
 * @retval #CONNECTION_ERROR_OUT_OF_MEMORY         Out of memory
 * @retval #CONNECTION_ERROR_OPERATION_FAILED      Operation failed
 * @retval #CONNECTION_ERROR_NO_CONNECTION         There is no connection
 * @retval #CONNECTION_ERROR_PERMISSION_DENIED     Permission denied
 * @retval #CONNECTION_ERROR_NOT_SUPPORTED         Not supported
 */
int connection_get_current_profile(connection_h connection, connection_profile_h* profile);


/**
 * @brief Gets the default profile which provides the given cellular service.
 * @details This function retrieves the default profile that provides the specified cellular service type. \n
 *          The caller must release the returned profile handle using connection_profile_destroy(). \n
 * @since_tizen 2.3
 * @privlevel public
 * @privilege %http://tizen.org/privilege/network.get
 * @remarks You must release @a profile using connection_profile_destroy().
 * @param[in] connection        The connection handle
 * @param[in] type              The type of cellular service \n
 *            #CONNECTION_CELLULAR_SERVICE_TYPE_APPLICATION is not permitted
 * @param[out] profile          The profile handle
 * @return @c 0 on success,
 *         otherwise a negative error value
 * @retval #CONNECTION_ERROR_NONE                  Successful
 * @retval #CONNECTION_ERROR_NOT_INITIALIZED       Not initialized
 * @retval #CONNECTION_ERROR_INVALID_PARAMETER     Invalid parameter
 * @retval #CONNECTION_ERROR_OUT_OF_MEMORY         Out of memory
 * @retval #CONNECTION_ERROR_OPERATION_FAILED      Operation failed
 * @retval #CONNECTION_ERROR_PERMISSION_DENIED     Permission denied
 * @retval #CONNECTION_ERROR_NOT_SUPPORTED         Not supported
 */
int connection_get_default_cellular_service_profile(connection_h connection,
		connection_cellular_service_type_e type, connection_profile_h* profile);


/**
 * @brief Sets the default profile which provides the given cellular service.
 * @details This function sets the default profile for the specified cellular service type. \n
 *          It requires both network.profile and network.get privileges. \n
 *          The function only permits #CONNECTION_CELLULAR_SERVICE_TYPE_INTERNET and #CONNECTION_CELLULAR_SERVICE_TYPE_PREPAID_INTERNET as valid service types. \n
 *          If successful, it returns #CONNECTION_ERROR_NONE; otherwise, it returns an appropriate error code.
 * @since_tizen 2.3
 * @privlevel public
 * @privilege %http://tizen.org/privilege/network.profile \n
 *            %http://tizen.org/privilege/network.get
 * @remarks This function needs both privileges.
 * @param[in] connection        The connection handle
 * @param[in] type              The type of cellular service \n
 *            Only #CONNECTION_CELLULAR_SERVICE_TYPE_INTERNET and
 *            #CONNECTION_CELLULAR_SERVICE_TYPE_PREPAID_INTERNET are permitted
 * @param[in] profile           The profile handle
 * @return @c 0 on success,
 *         otherwise a negative error value
 * @retval #CONNECTION_ERROR_NONE                  Successful
 * @retval #CONNECTION_ERROR_NOT_INITIALIZED       Not initialized
 * @retval #CONNECTION_ERROR_INVALID_PARAMETER     Invalid parameter
 * @retval #CONNECTION_ERROR_OPERATION_FAILED      Operation failed
 * @retval #CONNECTION_ERROR_PERMISSION_DENIED     Permission denied
 * @retval #CONNECTION_ERROR_NOT_SUPPORTED         Not supported
 */
int connection_set_default_cellular_service_profile(connection_h connection,
		connection_cellular_service_type_e type, connection_profile_h profile);


/**
 * @brief Sets the default profile which provides the given cellular service, asynchronously.
 * @details This function sets the default profile for a specified cellular service type asynchronously. \n
 *          It requires both network.profile and network.get privileges. \n
 *          Only #CONNECTION_CELLULAR_SERVICE_TYPE_INTERNET and #CONNECTION_CELLULAR_SERVICE_TYPE_PREPAID_INTERNET are allowed as the service type. \n
 *          The function returns an integer value indicating the result of the operation. \n
 *          If successful, it returns #CONNECTION_ERROR_NONE; otherwise, it returns one of the other error codes.
 * @since_tizen 2.3
 * @privlevel public
 * @privilege %http://tizen.org/privilege/network.profile \n
 *	          %http://tizen.org/privilege/network.get
 * @remarks This function needs both privileges.
 * @param[in] connection        The connection handle
 * @param[in] type              The type of cellular service \n
 *            Only #CONNECTION_CELLULAR_SERVICE_TYPE_INTERNET and
 *            #CONNECTION_CELLULAR_SERVICE_TYPE_PREPAID_INTERNET are permitted
 * @param[in] profile The profile handle
 * @param[in] callback The callback function to be called
 * @param[in] user_data The user data passed to the callback function
 * @return @c 0 on success,
 *         otherwise a negative error value
 * @retval #CONNECTION_ERROR_NONE                  Successful
 * @retval #CONNECTION_ERROR_NOT_INITIALIZED       Not initialized
 * @retval #CONNECTION_ERROR_INVALID_PARAMETER     Invalid parameter
 * @retval #CONNECTION_ERROR_OPERATION_FAILED      Operation failed
 * @retval #CONNECTION_ERROR_PERMISSION_DENIED     Permission denied
 * @retval #CONNECTION_ERROR_NOT_SUPPORTED         Not supported
 */
int connection_set_default_cellular_service_profile_async(connection_h connection,
		connection_cellular_service_type_e type, connection_profile_h profile,
		connection_set_default_cb callback, void* user_data);


/**
 * @brief Called after connection_open_profile() is finished.
 * @details This callback function is invoked when the asynchronous operation of opening a network profile is completed. \n
 *          It provides the result of the operation and any user data that was passed during the call to connection_open_profile(). \n
 *          The application can use this callback to handle the result of the profile opening process.
 * @since_tizen 2.3
 * @param[in] result            The result
 * @param[in] user_data         The user data passed from connection_open_profile()
 * @pre connection_open_profile() will invoke this callback function.
 * @see connection_open_profile()
*/
typedef void(*connection_opened_cb)(connection_error_e result, void* user_data);


/**
 * @brief Called after connection_close_profile() is finished.
 * @details This callback function is invoked when the connection_close_profile() function has completed its execution. \n
 *          It receives the result of the operation and any user data that was passed to the connection_close_profile() function.
 * @since_tizen 2.3
 * @param[in] result            The result
 * @param[in] user_data         The user data passed from connection_close_profile()
 * @pre connection_close_profile() will invoke this callback function.
 * @see connection_close_profile()
*/
typedef void(*connection_closed_cb)(connection_error_e result, void* user_data);


/**
 * @brief Called after connection_reset_profile() is finished.
 * @details This callback function is invoked when the connection_reset_profile() function completes its operation. \n
 *          It receives the result of the operation and the user data that was passed to the connection_reset_profile() function.
 * @since_tizen 2.3
 * @param[in] result            The result
 * @param[in] user_data         The user data passed from connection_reset_profile()
 * @pre connection_reset_profile() will invoke this callback function.
 * @see connection_reset_profile()
*/
typedef void(*connection_reset_cb)(connection_error_e result, void* user_data);


/**
 * @brief Opens a connection of profile, asynchronously.
 * @details This function opens a network connection using the specified profile handle in an asynchronous manner.
 * @since_tizen 2.3
 * @privlevel public
 * @privilege %http://tizen.org/privilege/network.set \n
 *            %http://tizen.org/privilege/network.get
 * @remarks This function needs both privileges.
 * @param[in] connection        The connection handle
 * @param[in] profile           The profile handle
 * @param[in] callback          The callback function to be called
 * @param[in] user_data         The user data passed to the callback function
 * @return @c 0 on success,
 *         otherwise a negative error value
 * @retval #CONNECTION_ERROR_NONE                  Successful
 * @retval #CONNECTION_ERROR_NOT_INITIALIZED       Not initialized
 * @retval #CONNECTION_ERROR_INVALID_PARAMETER     Invalid parameter
 * @retval #CONNECTION_ERROR_OPERATION_FAILED      Operation failed
 * @retval #CONNECTION_ERROR_PERMISSION_DENIED     Permission denied
 * @retval #CONNECTION_ERROR_NOT_SUPPORTED         Not supported
 * @post connection_opened_cb() will be invoked.
 * @see connection_opened_cb()
 * @see connection_close_profile()
 * @see connection_profile_set_state_changed_cb()
 * @see connection_profile_unset_state_changed_cb()
 * @see connection_profile_state_changed_cb()
 */
int connection_open_profile(connection_h connection, connection_profile_h profile,
		connection_opened_cb callback, void* user_data);


/**
 * @brief Closes a connection of profile.
 * @details This function closes the connection of a specified profile and releases all resources associated with it. \n
 *          After closing the profile, the corresponding callback function is invoked to notify the completion of the operation.
 * @since_tizen 2.3
 * @privlevel public
 * @privilege %http://tizen.org/privilege/network.set
 * @param[in] connection        The connection handle
 * @param[in] profile           The profile handle
 * @param[in] callback          The callback function to be called
 * @param[in] user_data         The user data passed to the callback function
 * @return @c 0 on success,
 *         otherwise a negative error value
 * @retval #CONNECTION_ERROR_NONE                  Successful
 * @retval #CONNECTION_ERROR_NOT_INITIALIZED       Not initialized
 * @retval #CONNECTION_ERROR_INVALID_PARAMETER     Invalid parameter
 * @retval #CONNECTION_ERROR_OPERATION_FAILED      Operation failed
 * @retval #CONNECTION_ERROR_PERMISSION_DENIED     Permission denied
 * @retval #CONNECTION_ERROR_NOT_SUPPORTED         Not supported
 * @post connection_closed_cb() will be invoked.
 * @see connection_closed_cb()
 * @see connection_open_profile()
 * @see connection_profile_set_state_changed_cb()
 * @see connection_profile_unset_state_changed_cb()
 * @see connection_profile_state_changed_cb()
 */
int connection_close_profile(connection_h connection, connection_profile_h profile,
		connection_closed_cb callback, void* user_data);


/**
 * @brief Resets the cellular profile.
 * @details This function resets the cellular profiles. \n
 *          It requires both network.profile and network.get privileges. \n
 *          The type of reset can be specified using the 'type' parameter, \n
 *          and the subscriber identity module ID to reset can be provided using the 'id' parameter. \n
 *          The callback function and user data are used for asynchronous operation handling.
 * @since_tizen 2.3
 * @privlevel public
 * @privilege %http://tizen.org/privilege/network.profile \n
 *	          %http://tizen.org/privilege/network.get
 * @remarks This function needs both privileges.
 * @param[in] connection        The connection handle
 * @param[in] type              The type of reset
 * @param[in] id                The subscriber identity module ID to reset
 *                              (The sim index starts from 0.)
 * @param[in] callback          The callback function to be called
 * @param[in] user_data         The user data passed to the callback function
 * @return @c 0 on success,
 *         otherwise a negative error value
 * @retval #CONNECTION_ERROR_NONE                  Successful
 * @retval #CONNECTION_ERROR_NOT_INITIALIZED       Not initialized
 * @retval #CONNECTION_ERROR_INVALID_PARAMETER     Invalid parameter
 * @retval #CONNECTION_ERROR_OPERATION_FAILED      Operation failed
 * @retval #CONNECTION_ERROR_PERMISSION_DENIED     Permission denied
 * @retval #CONNECTION_ERROR_NOT_SUPPORTED         Not supported
 * @post connection_reset_cb() will be invoked.
*/
int connection_reset_profile(connection_h connection, connection_reset_option_e type,
		int id, connection_reset_cb callback, void *user_data);


/**
 * @platform
 * @deprecated Deprecated since 4.0. Use connection_add_route_entry() instead.
 * @brief Adds a IPv4 route to the routing table.
 * @details You can get the @a interface_name from
 *          connection_profile_get_network_interface_name() of opened profile.
 * @since_tizen 2.3
 * @privlevel partner
 * @privilege %http://tizen.org/privilege/network.route
 * @remarks Since 6.0, the required privilege and level of this function has changed.
 * @param[in] connection        The connection handle
 * @param[in] interface_name    The name of network interface
 * @param[in] host_address      The IP address of the host
 * @return @c 0 on success,
 *         otherwise a negative error value
 * @retval #CONNECTION_ERROR_NONE                  Successful
 * @retval #CONNECTION_ERROR_NOT_INITIALIZED       Not initialized
 * @retval #CONNECTION_ERROR_INVALID_PARAMETER     Invalid parameter
 * @retval #CONNECTION_ERROR_ALREADY_EXISTS        Already exists
 * @retval #CONNECTION_ERROR_OPERATION_FAILED      Operation failed
 * @retval #CONNECTION_ERROR_PERMISSION_DENIED     Permission denied
 * @retval #CONNECTION_ERROR_NOT_SUPPORTED         Not supported
 * @see connection_profile_get_network_interface_name()
 */
int connection_add_route(connection_h connection, const char* interface_name,
		const char* host_address) TIZEN_DEPRECATED_API;


/**
 * @platform
 * @deprecated Deprecated since 4.0. Use connection_remove_route_entry() instead.
 * @brief Removes a IPv4 route from the routing table.
 * @details You can get the @a interface_name from
 *          connection_profile_get_network_interface_name() of opened profile.
 * @since_tizen 2.3
 * @privlevel partner
 * @privilege %http://tizen.org/privilege/network.route
 * @remarks Since 6.0, the required privilege and level of this function has changed.
 * @param[in] connection        The connection handle
 * @param[in] interface_name    The name of network interface
 * @param[in] host_address      The IP address of the host
 * @return @c 0 on success,
 *         otherwise a negative error value
 * @retval #CONNECTION_ERROR_NONE                  Successful
 * @retval #CONNECTION_ERROR_NOT_INITIALIZED       Not initialized
 * @retval #CONNECTION_ERROR_INVALID_PARAMETER     Invalid parameter
 * @retval #CONNECTION_ERROR_OPERATION_FAILED      Operation failed
 * @retval #CONNECTION_ERROR_PERMISSION_DENIED     Permission denied
 * @retval #CONNECTION_ERROR_NOT_SUPPORTED         Not supported
 * @see connection_profile_get_network_interface_name()
 */
int connection_remove_route(connection_h connection, const char* interface_name,
		const char* host_address) TIZEN_DEPRECATED_API;


/**
 * @platform
 * @deprecated Deprecated since 4.0. Use connection_add_route_entry() instead.
 * @brief Adds a IPv6 route to the routing table.
 * @details You can get the @a interface_name from
 *          connection_profile_get_network_interface_name() of opened profile.
 * @since_tizen 2.3.1
 * @privlevel partner
 * @privilege %http://tizen.org/privilege/network.route
 * @remarks Since 6.0, the required privilege and level of this function has changed.
 * @param[in] connection        The connection handle
 * @param[in] interface_name    The name of network interface
 * @param[in] host_address      The IP address of the host
 * @param[in] gateway           The gateway address
 * @return @c 0 on success,
 *         otherwise a negative error value
 * @retval #CONNECTION_ERROR_NONE                  Successful
 * @retval #CONNECTION_ERROR_NOT_INITIALIZED       Not initialized
 * @retval #CONNECTION_ERROR_INVALID_PARAMETER     Invalid parameter
 * @retval #CONNECTION_ERROR_ALREADY_EXISTS        Already exists
 * @retval #CONNECTION_ERROR_OPERATION_FAILED      Operation failed
 * @retval #CONNECTION_ERROR_PERMISSION_DENIED     Permission denied
 * @retval #CONNECTION_ERROR_NOT_SUPPORTED         Not supported
 * @see connection_profile_get_network_interface_name()
 */
int connection_add_route_ipv6(connection_h connection, const char *interface_name,
		const char *host_address, const char * gateway) TIZEN_DEPRECATED_API;


/**
 * @platform
 * @deprecated Deprecated since 4.0. Use connection_remove_route_entry() instead.
 * @brief Removes a IPV6 route from the routing table.
 * @details You can get the @a interface_name from
 *          connection_profile_get_network_interface_name() of opened profile.
 * @since_tizen 2.3.1
 * @privlevel partner
 * @privilege %http://tizen.org/privilege/network.route
 * @remarks Since 6.0, the required privilege and level of this function has changed.
 * @param[in] connection        The connection handle
 * @param[in] interface_name    The name of network interface
 * @param[in] host_address      The IP address of the host
 * @param[in] gateway           The gateway address
 * @return @c 0 on success,
 *         otherwise a negative error value
 * @retval #CONNECTION_ERROR_NONE                  Successful
 * @retval #CONNECTION_ERROR_NOT_INITIALIZED       Not initialized
 * @retval #CONNECTION_ERROR_INVALID_PARAMETER     Invalid parameter
 * @retval #CONNECTION_ERROR_OPERATION_FAILED      Operation failed
 * @retval #CONNECTION_ERROR_PERMISSION_DENIED     Permission denied
 * @retval #CONNECTION_ERROR_NOT_SUPPORTED         Not supported
 * @see connection_profile_get_network_interface_name()
 */
int connection_remove_route_ipv6(connection_h connection, const char *interface_name,
		const char *host_address, const char * gateway) TIZEN_DEPRECATED_API;

/**
 * @platform
 * @brief Adds a route to the routing table.
 * @details You can get the @a interface_name from \n
 *          connection_profile_get_network_interface_name() of opened profile.
 * @since_tizen 4.0
 * @privlevel partner
 * @privilege %http://tizen.org/privilege/network.route
 * @remarks Since 6.0, the required privilege and level of this function has changed.
 * @param[in] connection        The connection handle
 * @param[in] address_family    The address family
 * @param[in] interface_name    The name of network interface
 * @param[in] host_address      The IP address of the host (e.g., single IP address such as
 *            163.152.10.2,192.168.1.2), group's IP address (e.g., 163.152.10.0, 192.168.0.0)
 *            is not allowed
 * @param[in] gateway           The gateway address
 * @return @c 0 on success,
 *         otherwise a negative error value
 * @retval #CONNECTION_ERROR_NONE                  Successful
 * @retval #CONNECTION_ERROR_NOT_INITIALIZED       Not initialized
 * @retval #CONNECTION_ERROR_INVALID_PARAMETER     Invalid parameter
 * @retval #CONNECTION_ERROR_ALREADY_EXISTS        Already exists
 * @retval #CONNECTION_ERROR_OPERATION_FAILED      Operation failed
 * @retval #CONNECTION_ERROR_PERMISSION_DENIED     Permission denied
 * @retval #CONNECTION_ERROR_NOT_SUPPORTED         Not supported
 * @see connection_profile_get_network_interface_name()
 */
int connection_add_route_entry(connection_h connection,
		connection_address_family_e address_family, const char *interface_name,
		const char *host_address, const char *gateway);

/**
 * @platform
 * @brief Removes a route from the routing table.
 * @details You can get the @a interface_name from \n
 *          connection_profile_get_network_interface_name() of opened profile.
 * @since_tizen 4.0
 * @privlevel partner
 * @privilege %http://tizen.org/privilege/network.route
 * @remarks Since 6.0, the required privilege and level of this function has changed.
 * @param[in] connection        The connection handle
 * @param[in] address_family    The address family
 * @param[in] interface_name    The name of network interface
 * @param[in] host_address      The IP address of the host (e.g., single IP address such as
 *            163.152.10.2,192.168.1.2), group's IP address (e.g., 163.152.10.0, 192.168.0.0)
 *            is not allowed
 * @param[in] gateway           The gateway address
 * @return @c 0 on success,
 *         otherwise a negative error value
 * @retval #CONNECTION_ERROR_NONE                  Successful
 * @retval #CONNECTION_ERROR_NOT_INITIALIZED       Not initialized
 * @retval #CONNECTION_ERROR_INVALID_PARAMETER     Invalid parameter
 * @retval #CONNECTION_ERROR_OPERATION_FAILED      Operation failed
 * @retval #CONNECTION_ERROR_PERMISSION_DENIED     Permission denied
 * @retval #CONNECTION_ERROR_NOT_SUPPORTED         Not supported
 * @see connection_profile_get_network_interface_name()
 */
int connection_remove_route_entry(connection_h connection,
		connection_address_family_e address_family, const char *interface_name,
		const char *host_address, const char *gateway);

/**
 * @brief Gets all IPv6 addresses assigned to the network interface.
 * @details This function retrieves all IPv6 addresses associated with a specified network interface and invokes a provided callback function for each address found.
 * @since_tizen 4.0
 * @param[in] connection       The connection handle
 * @param[in] connection_type  The connection type
 * @param[in] callback         The callback to be called for each IPv6 address
 * @param[in] user_data        The user data passed to the callback function
 * @return 0 on success, otherwise negative error value.
 * @retval #CONNECTION_ERROR_NONE               Successful
 * @retval #CONNECTION_ERROR_NOT_INITIALIZED       Not initialized
 * @retval #CONNECTION_ERROR_INVALID_PARAMETER  Invalid parameter
 * @retval #CONNECTION_ERROR_OPERATION_FAILED   Operation failed
 * @retval #CONNECTION_ERROR_NOT_SUPPORTED      Not supported
 */
int connection_foreach_ipv6_address(connection_h connection, connection_type_e connection_type,
		connection_ipv6_address_cb callback, void *user_data);

/**
 * @}
*/


/**
 * @addtogroup CAPI_NETWORK_CONNECTION_STATISTICS_MODULE
 * @{
*/


/**
 * @brief Gets the statistics information.
 * @details This function retrieves the specified statistics information for the given connection type. \n
 *          It supports only #CONNECTION_TYPE_WIFI and #CONNECTION_TYPE_CELLULAR types.
 * @since_tizen 2.3
 * @privlevel public
 * @privilege %http://tizen.org/privilege/network.get
 * @param[in] connection The connection handle
 * @param[in] connection_type   The type of connection \n
 *            Only #CONNECTION_TYPE_WIFI and #CONNECTION_TYPE_CELLULAR are supported
 * @param[in] statistics_type   The type of statistics
 * @param[out] size             The received data size of the last cellular packet data connection (bytes)
 * @return @c 0 on success,
 *         otherwise a negative error value
 * @retval #CONNECTION_ERROR_NONE                  Successful
 * @retval #CONNECTION_ERROR_NOT_INITIALIZED       Not initialized
 * @retval #CONNECTION_ERROR_INVALID_PARAMETER     Invalid parameter
 * @retval #CONNECTION_ERROR_OPERATION_FAILED      Operation failed
 * @retval #CONNECTION_ERROR_PERMISSION_DENIED     Permission denied
 * @retval #CONNECTION_ERROR_NOT_SUPPORTED         Not supported
 */
int connection_get_statistics(connection_h connection, connection_type_e connection_type,
		connection_statistics_type_e statistics_type, long long* size);


/**
 * @brief Resets the statistics information.
 * @details This function resets the specified statistics information for a given connection type. \n
 *          Supported connection types include #CONNECTION_TYPE_WIFI and #CONNECTION_TYPE_CELLULAR.
 * @since_tizen 2.3
 * @privlevel public
 * @privilege %http://tizen.org/privilege/network.set
 * @param[in] connection        The connection handle
 * @param[in] connection_type   The type of connection \n
 *            Only #CONNECTION_TYPE_WIFI and #CONNECTION_TYPE_CELLULAR are supported
 * @param[in] statistics_type   The type of statistics
 * @return @c 0 on success,
 *         otherwise a negative error value
 * @retval #CONNECTION_ERROR_NONE                  Successful
 * @retval #CONNECTION_ERROR_NOT_INITIALIZED       Not initialized
 * @retval #CONNECTION_ERROR_INVALID_PARAMETER     Invalid parameter
 * @retval #CONNECTION_ERROR_OPERATION_FAILED      Operation failed
 * @retval #CONNECTION_ERROR_PERMISSION_DENIED     Permission denied
 * @retval #CONNECTION_ERROR_NOT_SUPPORTED         Not supported
 */
int connection_reset_statistics(connection_h connection, connection_type_e connection_type,
		connection_statistics_type_e statistics_type);
/**
 * @}
*/


#ifdef __cplusplus
}
#endif /* __cplusplus */


#endif /* __TIZEN_NETWORK_NET_CONNECTION_H__ */
