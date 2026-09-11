/*
* Copyright (c) 2011 Samsung Electronics Co., Ltd All Rights Reserved
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

#ifndef __TIZEN_NETWORK_SOFTAP_H__
#define __TIZEN_NETWORK_SOFTAP_H__

#include <tizen.h>
#include <time.h>

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @file softap.h
 */

/**
 * @addtogroup CAPI_NETWORK_SOFTAP_MANAGER_MODULE
 * @{
 */

/**
 * @brief The SoftAP handle.
 * @since_tizen 5.0
 */
typedef void * softap_h;

/**
 * @brief Enumeration for the SoftAP.
 * @since_tizen 5.0
 */
typedef enum {
	SOFTAP_ERROR_NONE = TIZEN_ERROR_NONE,                           /**< Successful */
	SOFTAP_ERROR_NOT_PERMITTED = TIZEN_ERROR_NOT_PERMITTED,         /**< Operation not permitted */
	SOFTAP_ERROR_INVALID_PARAMETER = TIZEN_ERROR_INVALID_PARAMETER, /**< Invalid parameter */
	SOFTAP_ERROR_OUT_OF_MEMORY = TIZEN_ERROR_OUT_OF_MEMORY,         /**< Out of memory */
	SOFTAP_ERROR_RESOURCE_BUSY = TIZEN_ERROR_RESOURCE_BUSY,         /**< Resource busy */
	SOFTAP_ERROR_OPERATION_FAILED = TIZEN_ERROR_SOFTAP | 0x01,      /**< Operation failed */
	SOFTAP_ERROR_ALREADY_ENABLED = TIZEN_ERROR_SOFTAP | 0x02,       /**< Already enabled */
	SOFTAP_ERROR_INVALID_OPERATION = TIZEN_ERROR_INVALID_OPERATION, /**< Invalid operation */
	SOFTAP_ERROR_NOT_SUPPORTED = TIZEN_ERROR_NOT_SUPPORTED,         /**< API is not supported */
	SOFTAP_ERROR_PERMISSION_DENIED = TIZEN_ERROR_PERMISSION_DENIED, /**< Permission denied */
} softap_error_e;

/**
 * @brief Enumeration for the cause of disabling the SoftAP.
 * @since_tizen 5.0
 */
typedef enum {
	SOFTAP_DISABLED_BY_FLIGHT_MODE = 0,  /**< Disabled due to flight mode */
	SOFTAP_DISABLED_BY_LOW_BATTERY,  /**< Disabled due to low battery */
	SOFTAP_DISABLED_BY_NETWORK_CLOSE,  /**< Disabled due to pdp network close */
	SOFTAP_DISABLED_BY_TIMEOUT,  /**< Disabled due to timeout */
	SOFTAP_DISABLED_BY_OTHERS,  /**< Disabled by other apps */
	SOFTAP_DISABLED_BY_REQUEST,  /**< Disabled by your request */
	SOFTAP_DISABLED_BY_WIFI_ON,  /**< Disabled due to Wi-Fi on */
} softap_disabled_cause_e;

/**
 * @}
 */


/**
 * @addtogroup CAPI_NETWORK_SOFTAP_MANAGER_MODULE
 * @{
 */

/**
 * @brief Enumeration for the SoftAP security.
 * @since_tizen 5.0
 */
typedef enum {
	SOFTAP_SECURITY_TYPE_NONE = 0,  /**< No Security type */
	SOFTAP_SECURITY_TYPE_WPA2_PSK,  /**< WPA2_PSK */
	SOFTAP_SECURITY_TYPE_WPS,  /**< WPS */
} softap_security_type_e;

/**
 * @brief Enumeration for the SoftAP HW mode.
 * @since_tizen 5.0
 */
typedef enum {
	SOFTAP_WIRELESS_MODE_B = 0,      /**< IEEE 802.11b */
	SOFTAP_WIRELESS_MODE_G,      /**< IEEE 802.11g */
	SOFTAP_WIRELESS_MODE_A,  /**< IEEE 802.11a */
	SOFTAP_WIRELESS_MODE_AD,     /**< IEEE 802.11ad */
} softap_wireless_mode_e;

/**
 * @}
 */


/**
 * @addtogroup CAPI_NETWORK_SOFTAP_CLIENT_MODULE
 * @{
 */

/**
 * @brief The SoftAP client handle.
 * @since_tizen 5.0
 */
typedef void * softap_client_h;

/**
 * @brief Enumeration for address family.
 * @since_tizen 5.0
 */
typedef enum {
	SOFTAP_ADDRESS_FAMILY_IPV4 = 0,  /**< IPv4 Address type */
	SOFTAP_ADDRESS_FAMILY_IPV6,  /**< IPv6 Address type */
} softap_address_family_e;

/**
 * @}
 */


/**
 * @addtogroup CAPI_NETWORK_SOFTAP_MANAGER_MODULE
 * @{
 */

/**
 * @brief Called when the SoftAP is enabled.
 * @details The following error codes can be received: \n
 *          #SOFTAP_ERROR_NONE:              Success \n
 *          #SOFTAP_ERROR_NOT_PERMITTED:     Operation not permitted \n
 *          #SOFTAP_ERROR_INVALID_PARAMETER: Invalid parameter \n
 *          #SOFTAP_ERROR_OUT_OF_MEMORY:     Out of memory \n
 *          #SOFTAP_ERROR_OPERATION_FAILED:  Operation failed \n
 *          #SOFTAP_ERROR_ALREADY_ENABLED:   Already enabled \n
 * @since_tizen 5.0
 * @param[in]  result  The result of enabling the SoftAP
 * @param[in]  is_requested  Indicates whether this change is requested
 * @param[in]  user_data  The user data passed from softap_set_enabled_cb()
 * @pre  If you register callback function using softap_set_enabled_cb(), this will be invoked when the SoftAP is enabled.
 * @see  softap_enable()
 * @see  softap_unset_enabled_cb()
 */
typedef void (*softap_enabled_cb)(softap_error_e result, bool is_requested, void *user_data);

/**
 * @brief Called when the SoftAP is disabled.
 * @details The following error codes can be received: \n
 *          #SOFTAP_ERROR_NONE:              Success \n
 *          #SOFTAP_ERROR_INVALID_PARAMETER: Invalid parameter \n
 *          #SOFTAP_ERROR_OUT_OF_MEMORY:     Out of memory \n
 *          #SOFTAP_ERROR_OPERATION_FAILED:  Operation failed \n
 * @since_tizen 5.0
 * @param[in]  result  The result of disabling the SoftAP
 * @param[in]  cause  The cause of disabling
 * @param[in]  user_data  The user data passed from softap_set_disabled_cb()
 * @pre  If you register callback function using softap_set_disabled_cb(), this will be invoked when the SoftAP is disabled.
 * @see  softap_set_disabled_cb()
 * @see  softap_unset_disabled_cb()
 */
typedef void (*softap_disabled_cb)(softap_error_e result, softap_disabled_cause_e cause, void *user_data);

/**
 * @brief Called when the connection state is changed.
 * @since_tizen 5.0
 * @remarks The @a client should not be released by the application and it is valid only in this function.\n
 *          In order to use it outside this function, a user must copy the client with softap_client_clone().
 * @param[in]  client  The client of which connection state is changed
 * @param[in]  opened  @c true when connection is opened, otherwise false
 * @param[in]  user_data  The user data passed from softap_set_client_connection_state_changed_cb()
 * @pre  If you register callback function using softap_set_client_connection_state_changed_cb(), this will be invoked when the connection state is changed.
 * @see  softap_set_client_connection_state_changed_cb()
 * @see  softap_unset_client_connection_state_changed_cb()
 */
typedef void (*softap_client_connection_state_changed_cb)(softap_client_h client, bool opened, void *user_data);

/**
 * @brief Called when you get the connected client repeatedly.
 * @since_tizen 5.0
 * @remarks The @a client should not be released by the application and it is valid only in this function.\n
 *          In order to use the client outside this function, a user must copy the client with softap_client_clone().
 * @param[in]  client  The connected client
 * @param[in]  user_data  The user data passed from the request function
 * @return  @c true to continue with the next iteration of the loop, \n @c false to break out of the loop
 * @pre  softap_foreach_connected_clients() will invoke this callback.
 * @see  softap_foreach_connected_clients()
 */
typedef bool(*softap_connected_client_cb)(softap_client_h client, void *user_data);

/**
 * @brief Called when the security type of SoftAP is changed.
 * @since_tizen 5.0
 * @param[in]  changed_type  The changed security type
 * @param[in]  user_data  The user data passed from the register function
 * @see  softap_set_security_type_changed_cb()
 * @see  softap_unset_security_type_changed_cb()
 */
typedef void (*softap_security_type_changed_cb)(softap_security_type_e changed_type, void *user_data);

/**
 * @brief Called when the visibility of SSID is changed.
 * @since_tizen 5.0
 * @param[in]  changed_visible  The changed visibility of SSID
 * @param[in]  user_data  The user data passed from the register function
 * @see  softap_set_ssid_visibility_changed_cb()
 * @see  softap_unset_ssid_visibility_changed_cb()
 */
typedef void (*softap_ssid_visibility_changed_cb)(bool changed_visible, void *user_data);

/**
 * @brief Called when the passphrase is changed.
 * @since_tizen 5.0
 * @param[in]  user_data  The user data passed from the register function
 * @see  softap_set_passphrase_changed_cb()
 * @see  softap_unset_passphrase_changed_cb()
 */
typedef void (*softap_passphrase_changed_cb)(void *user_data);

/**
 * @brief Called when SoftAP settings are reloaded.
 * @details The following error codes can be received: \n
 *          #SOFTAP_ERROR_NONE:              Success \n
 *          #SOFTAP_ERROR_NOT_PERMITTED:     Operation not permitted \n
 *          #SOFTAP_ERROR_INVALID_PARAMETER: Invalid parameter \n
 *          #SOFTAP_ERROR_OUT_OF_MEMORY:     Out of memory \n
 *          #SOFTAP_ERROR_OPERATION_FAILED:  Operation failed \n
 * @since_tizen 5.0
 * @param[in]  result  The result of reloading the settings
 * @param[in]  user_data  The user data passed from the request function
 * @pre  softap_reload_settings() will invoke this callback.
 */
typedef void (*softap_settings_reloaded_cb)(softap_error_e result, void *user_data);

/**
 * @brief Creates the handle for SoftAP.
 * @since_tizen 5.0
 * @privlevel public
 * @privilege %http://tizen.org/privilege/softap
 * @remarks The @a softap should be released using softap_destroy().
 * @param[out]  softap A handle of a new mobile ap handle on success
 * @return  0 on success, otherwise a negative error value
 * @retval  #SOFTAP_ERROR_NONE              Successful
 * @retval  #SOFTAP_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval  #SOFTAP_ERROR_OUT_OF_MEMORY  Out of memory
 * @retval  #SOFTAP_ERROR_NOT_SUPPORTED     API is not supported
 * @retval  #SOFTAP_ERROR_PERMISSION_DENIED Permission denied
 * @see  softap_destroy()
 */
int softap_create(softap_h *softap);

/**
 * @brief Destroys the handle for SoftAP.
 * @since_tizen 5.0
 * @privlevel public
 * @privilege %http://tizen.org/privilege/softap
 * @param[in]  softap  The SoftAP handle
 * @return  0 on success, otherwise a negative error value
 * @retval  #SOFTAP_ERROR_NONE              Successful
 * @retval  #SOFTAP_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval  #SOFTAP_ERROR_NOT_SUPPORTED     API is not supported
 * @retval  #SOFTAP_ERROR_PERMISSION_DENIED Permission denied
 * @see  softap_create()
 */
int softap_destroy(softap_h softap);

/**
 * @platform
 * @brief Enables the SoftAP, asynchronously.
 * @details This function allows you to activate the SoftAP functionality on the device.
 * It's important to ensure that the required privileges have been obtained before invoking this function, as mentioned in the precondition.
 * Upon successful execution, the SoftAP will be enabled, allowing nearby devices to establish wireless connections.
 * Depending on the device, the user might need to manually turn off the current Wi-Fi connection before enabling the SoftAP.
 * @since_tizen 5.0
 * @privlevel platform
 * @privilege %http://tizen.org/privilege/softap.admin
 * @param[in]  softap  The SoftAP handle
 * @return 0 on success, otherwise negative error value
 * @retval  #SOFTAP_ERROR_NONE              Successful
 * @retval  #SOFTAP_ERROR_NOT_PERMITTED  Operation not permitted
 * @retval  #SOFTAP_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval  #SOFTAP_ERROR_INVALID_OPERATION  Invalid operation
 * @retval  #SOFTAP_ERROR_RESOURCE_BUSY Device or resource busy
 * @retval  #SOFTAP_ERROR_NOT_SUPPORTED     API is not supported
 * @retval  #SOFTAP_ERROR_PERMISSION_DENIED Permission denied
 * @post softap_enabled_cb() will be invoked.
 * @see  softap_is_enabled()
 * @see  softap_disable()
 */
int softap_enable(softap_h softap);
/**
 * @platform
 * @brief Disables the SoftAP, asynchronously.
 * @details This function enables you to deactivate the SoftAP functionality on the device.
 * Upon successful execution, the SoftAP will be disabled, preventing further wireless connections from being established.
 * @since_tizen 5.0
 * @privlevel platform
 * @privilege %http://tizen.org/privilege/softap.admin
 * @param[in]  softap  The SoftAP handle
 * @return 0 on success, otherwise negative error value
 * @retval  #SOFTAP_ERROR_NONE              Successful
 * @retval  #SOFTAP_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval  #SOFTAP_ERROR_INVALID_OPERATION  Invalid operation
 * @retval  #SOFTAP_ERROR_NOT_SUPPORTED     API is not supported
 * @retval  #SOFTAP_ERROR_PERMISSION_DENIED Permission denied
 * @post softap_disabled_cb() will be invoked.
 * @see  softap_is_enabled()
 * @see  softap_enable()
 */
int softap_disable(softap_h softap);

/**
 * @brief Checks whether the SoftAP is enabled or not.
 * @since_tizen 5.0
 * @privlevel public
 * @privilege %http://tizen.org/privilege/softap
 * @param[in]  softap  The SoftAP handle
 * @param[out] enable  @c true if SoftAP is enabled, \n @c false if SoftAP is disabled
 * @return 0 on success, otherwise negative error value
 * @retval  #SOFTAP_ERROR_NONE              Successful
 * @retval  #SOFTAP_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval  #SOFTAP_ERROR_OPERATION_FAILED  Operation failed
 * @retval  #SOFTAP_ERROR_NOT_SUPPORTED     API is not supported
 * @retval  #SOFTAP_ERROR_PERMISSION_DENIED Permission denied
 */
int softap_is_enabled(softap_h softap, bool *enable);

/**
 * @brief Gets the Media Access Control (MAC) address of the local device in the format "FC:A1:3E:D6:B1:B1".
 * @details This function allows you to obtain the MAC address of the local device.
 * Remember that the SoftAP must be enabled in order to use this function.
 * You can verify the SoftAP's activation state using the 'softap_is_enabled' function.
 * Upon successful execution, the MAC address will be returned in the form of a string,
 * which should be freed using the 'free' function once it's no longer needed.
 * @since_tizen 5.0
 * @privlevel public
 * @privilege %http://tizen.org/privilege/softap
 * @remarks The @a mac_address should be released using free().
 * @param[in]  softap  The SoftAP handle
 * @param[out]  mac_address  The MAC address
 * @return  0 on success, otherwise a negative error value
 * @retval  #SOFTAP_ERROR_NONE              Successful
 * @retval  #SOFTAP_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval  #SOFTAP_ERROR_OUT_OF_MEMORY     Out of memory
 * @retval  #SOFTAP_ERROR_OPERATION_FAILED  Operation failed
 * @retval  #SOFTAP_ERROR_INVALID_OPERATION Invalid operation
 * @retval  #SOFTAP_ERROR_NOT_SUPPORTED     API is not supported
 * @retval  #SOFTAP_ERROR_PERMISSION_DENIED Permission denied
 * @pre  The SoftAP must be enabled.
 * @see  softap_is_enabled()
 * @see  softap_enable()
 */
int softap_get_mac_address(softap_h softap, char **mac_address);

/**
 * @brief Gets the name of network interface (e.g. wlan0).
 * @details This function enables you to retrieve the name of the network interface associated with the SoftAP functionality.
 * Remember that the SoftAP must be enabled in order to use this function.
 * You can verify the SoftAP's activation state using the 'softap_is_enabled' function.
 * Upon successful execution, the name of the network interface will be returned as a string,
 * which should be freed using the 'free' function once it's no longer needed.
 * @since_tizen 5.0
 * @privlevel public
 * @privilege %http://tizen.org/privilege/softap
 * @remarks The @a interface_name should be released using free().
 * @param[in]  softap  The SoftAP handle
 * @param[out]  interface_name  The name of the network interface
 * @return 0 on success, otherwise negative error value
 * @retval  #SOFTAP_ERROR_NONE              Successful
 * @retval  #SOFTAP_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval  #SOFTAP_ERROR_OUT_OF_MEMORY  Out of memory
 * @retval  #SOFTAP_ERROR_OPERATION_FAILED  Operation failed
 * @retval  #SOFTAP_ERROR_INVALID_OPERATION  Invalid operation
 * @retval  #SOFTAP_ERROR_NOT_SUPPORTED     API is not supported
 * @retval  #SOFTAP_ERROR_PERMISSION_DENIED Permission denied
 * @pre  The SoftAP must be enabled.
 * @see  softap_is_enabled()
 * @see  softap_enable()
 */
int softap_get_network_interface_name(softap_h softap, char **interface_name);

/**
 * @brief Sets the local IP address.
 * @since_tizen 5.0
 * @privlevel public
 * @privilege %http://tizen.org/privilege/softap
 * @remarks This change is applied next time SoftAP is enabled.
 *          You can use softap_enable() or softap_reload_settings() to enable SoftAP.
 * @param[in]  softap  The SoftAP handle
 * @param[in]  address_family  The address family of IP address (currently only #SOFTAP_ADDRESS_FAMILY_IPV4 is supported)
 * @param[in]  ip_address  The local IP address
 * @return 0 on success, otherwise negative error value
 * @retval  #SOFTAP_ERROR_NONE              Successful
 * @retval  #SOFTAP_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval  #SOFTAP_ERROR_NOT_SUPPORTED     API is not supported
 * @retval  #SOFTAP_ERROR_PERMISSION_DENIED Permission denied
 */
int softap_set_ip_address(softap_h softap, softap_address_family_e address_family, const char *ip_address);

/**
 * @brief Gets the local IP address.
 * @details This function retrieves the IP address of the SoftAP.
 * Remember that the SoftAP must be enabled in order to use this function.
 * You can verify the SoftAP's activation state using the 'softap_is_enabled' function.
 * Upon successful execution, the IP address will be returned as a string,
 * which should be freed using the 'free' function once it's no longer needed.
 * @since_tizen 5.0
 * @privlevel public
 * @privilege %http://tizen.org/privilege/softap
 * @remarks The @a ip_address should be released using free().
 * @param[in]  softap  The SoftAP handle
 * @param[in]  address_family  The address family of IP address (currently only #SOFTAP_ADDRESS_FAMILY_IPV4 is supported)
 * @param[out]  ip_address  The local IP address
 * @return 0 on success, otherwise negative error value
 * @retval  #SOFTAP_ERROR_NONE              Successful
 * @retval  #SOFTAP_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval  #SOFTAP_ERROR_OUT_OF_MEMORY  Out of memory
 * @retval  #SOFTAP_ERROR_OPERATION_FAILED  Operation failed
 * @retval  #SOFTAP_ERROR_INVALID_OPERATION  Invalid operation
 * @retval  #SOFTAP_ERROR_NOT_SUPPORTED     API is not supported
 * @retval  #SOFTAP_ERROR_PERMISSION_DENIED Permission denied
 * @pre  The SoftAP must be enabled.
 * @see  softap_is_enabled()
 * @see  softap_enable()
 */
int softap_get_ip_address(softap_h softap, softap_address_family_e address_family, char **ip_address);

/**
 * @brief Gets the Gateway address.
 * @details This function retrieves the gateway address of the SoftAP.
 * Remember that the SoftAP must be enabled in order to use this function.
 * You can verify the SoftAP's activation state using the 'softap_is_enabled' function.
 * Upon successful execution, the gateway address will be returned as a string,
 * which should be freed using the 'free' function once it's no longer needed.
 * @since_tizen 5.0
 * @privlevel public
 * @privilege %http://tizen.org/privilege/softap
 * @remarks The @a gateway_address should be released using free().
 * @param[in]  softap  The SoftAP handle
 * @param[in]  address_family  The address family of IP address (currently only #SOFTAP_ADDRESS_FAMILY_IPV4 is supported)
 * @param[out]  gateway_address  Gateway address
 * @return 0 on success, otherwise negative error value
 * @retval  #SOFTAP_ERROR_NONE              Successful
 * @retval  #SOFTAP_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval  #SOFTAP_ERROR_OUT_OF_MEMORY  Out of memory
 * @retval  #SOFTAP_ERROR_OPERATION_FAILED  Operation failed
 * @retval  #SOFTAP_ERROR_INVALID_OPERATION  Invalid operation
 * @retval  #SOFTAP_ERROR_NOT_SUPPORTED     API is not supported
 * @retval  #SOFTAP_ERROR_PERMISSION_DENIED Permission denied
 * @pre  The SoftAP must be enabled.
 * @see  softap_is_enabled()
 * @see  softap_enable()
 */
int softap_get_gateway_address(softap_h softap, softap_address_family_e address_family, char **gateway_address);

/**
 * @brief Gets the Subnet Mask.
 * @details This function retrieves the subnet mask of the SoftAP.
 * Remember that the SoftAP must be enabled in order to use this function.
 * You can verify the SoftAP's activation state using the 'softap_is_enabled' function.
 * Upon successful execution, the subnet mask will be returned as a string,
 * which should be freed using the 'free' function once it's no longer needed.
 * @since_tizen 5.0
 * @privlevel public
 * @privilege %http://tizen.org/privilege/softap
 * @remarks The @a subnet_mask should be released using free().
 * @param[in]  softap  The SoftAP handle
 * @param[in]  address_family  The address family of IP address (currently only #SOFTAP_ADDRESS_FAMILY_IPV4 is supported)
 * @param[out]  subnet_mask  Subnet mask
 * @return 0 on success, otherwise negative error value
 * @retval  #SOFTAP_ERROR_NONE              Successful
 * @retval  #SOFTAP_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval  #SOFTAP_ERROR_OUT_OF_MEMORY  Out of memory
 * @retval  #SOFTAP_ERROR_OPERATION_FAILED  Operation failed
 * @retval  #SOFTAP_ERROR_INVALID_OPERATION  Invalid operation
 * @retval  #SOFTAP_ERROR_NOT_SUPPORTED     API is not supported
 * @retval  #SOFTAP_ERROR_PERMISSION_DENIED Permission denied
 * @pre  The SoftAP must be enabled.
 * @see  softap_is_enabled()
 * @see  softap_enable()
 */
int softap_get_subnet_mask(softap_h softap, softap_address_family_e address_family, char **subnet_mask);

/**
 * @brief Gets the clients which are connected.
 * @details This function allows you to enumerate the clients that are currently connected to the SoftAP.
 * By providing the appropriate SoftAP handle as the 'softap' argument and specifying the callback function and user data, you can receive information about each connected client.
 * Upon successful execution, the callback function will be called for each connected client, giving you the opportunity to perform actions based on the received client information.
 * @since_tizen 5.0
 * @privlevel public
 * @privilege %http://tizen.org/privilege/softap
 * @param[in]  softap  The SoftAP handle
 * @param[in]  callback  The callback function to invoke
 * @param[in]  user_data  The user data to be passed to the callback function
 * @return 0 on success, otherwise negative error value
 * @retval  #SOFTAP_ERROR_NONE              Successful
 * @retval  #SOFTAP_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval  #SOFTAP_ERROR_INVALID_OPERATION  Invalid operation
 * @retval  #SOFTAP_ERROR_OPERATION_FAILED  Operation failed
 * @retval  #SOFTAP_ERROR_NOT_SUPPORTED     API is not supported
 * @retval  #SOFTAP_ERROR_PERMISSION_DENIED Permission denied
 * @pre  The SoftAP must be enabled.
 * @see  softap_is_enabled()
 * @see  softap_enable()
 */
int softap_foreach_connected_clients(softap_h softap, softap_connected_client_cb callback, void *user_data);

/**
 * @brief Sets a callback function to be called when the SoftAP is enabled.
 * @details This function allows you to associate a callback function with the SoftAP handle, which will be invoked when the SoftAP is successfully enabled or when an error occurs during the enabling process. 
 * By providing the appropriate SoftAP handle as the 'softap' argument and specifying the callback function and user data, you can receive notification regarding the SoftAP's activation status.
 * Upon successful execution, the callback function will be registered, ensuring that your application receives timely updates regarding the SoftAP's enabling process.
 * @since_tizen 5.0
 * @privlevel public
 * @privilege %http://tizen.org/privilege/softap
 * @param[in]  softap The SoftAP handle
 * @param[in]  callback  The callback function to invoke
 * @param[in]  user_data  The user data to be passed to the callback function
 * @return 0 on success, otherwise negative error value
 * @retval  #SOFTAP_ERROR_NONE              Successful
 * @retval  #SOFTAP_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval  #SOFTAP_ERROR_NOT_SUPPORTED     API is not supported
 * @retval  #SOFTAP_ERROR_PERMISSION_DENIED Permission denied
 * @see  softap_unset_enabled_cb()
 * @see  softap_enable()
 */
int softap_set_enabled_cb(softap_h softap, softap_enabled_cb callback, void *user_data);

/**
 * @brief Unsets the callback function, which is called when SoftAP is enabled.
 * @details This function enables you to unregister the callback function from the SoftAP handle, preventing it from receiving notifications regarding the SoftAP's activation status.
 * By providing the appropriate SoftAP handle as the'softap' argument, you can initiate the unregistration process.
 * @since_tizen 5.0
 * @privlevel public
 * @privilege %http://tizen.org/privilege/softap
 * @param[in]  softap  The SoftAP handle
 * @return 0 on success, otherwise negative error value
 * @retval  #SOFTAP_ERROR_NONE              Successful
 * @retval  #SOFTAP_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval  #SOFTAP_ERROR_NOT_SUPPORTED     API is not supported
 * @retval  #SOFTAP_ERROR_PERMISSION_DENIED Permission denied
 * @see  softap_set_enabled_cb()
 */
int softap_unset_enabled_cb(softap_h softap);

/**
 * @brief Sets the callback function called when SoftAP is disabled.
 * @details This function allows you to associate a callback function with the SoftAP handle, which will be invoked when the SoftAP is successfully disabled.
 * By providing the appropriate SoftAP handle as the'softap' argument and specifying the callback function and user data, you can receive notification regarding the SoftAP's deactivation status.
 * Upon successful execution, the callback function will be registered, ensuring that your application receives timely updates regarding the SoftAP's disabling process.
 * @since_tizen 5.0
 * @privlevel public
 * @privilege %http://tizen.org/privilege/softap
 * @param[in]  softap  The SoftAP handle
 * @param[in]  callback  The callback function to invoke
 * @param[in]  user_data  The user data to be passed to the callback function
 * @return 0 on success, otherwise negative error value
 * @retval  #SOFTAP_ERROR_NONE              Successful
 * @retval  #SOFTAP_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval  #SOFTAP_ERROR_NOT_SUPPORTED     API is not supported
 * @retval  #SOFTAP_ERROR_PERMISSION_DENIED Permission denied
 * @see  softap_unset_disabled_cb()
 */
int softap_set_disabled_cb(softap_h softap, softap_disabled_cb callback, void *user_data);

/**
 * @brief Unsets the callback function, which is called when SoftAP is disabled.
 * @details This function enables you to dissociate the callback function from the SoftAP handle,
 * preventing it from receiving notifications regarding the SoftAP's deactivation status.
 * By providing the appropriate SoftAP handle as the'softap' argument, you can initiate the unregistration process.
 * Upon successful execution, the callback function will be unregistered, allowing your application to focus solely on other aspects of the SoftAP functionality.
 * @since_tizen 5.0
 * @privlevel public
 * @privilege %http://tizen.org/privilege/softap
 * @param[in]  softap  The SoftAP handle
 * @return 0 on success, otherwise negative error value
 * @retval  #SOFTAP_ERROR_NONE              Successful
 * @retval  #SOFTAP_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval  #SOFTAP_ERROR_NOT_SUPPORTED     API is not supported
 * @retval  #SOFTAP_ERROR_PERMISSION_DENIED Permission denied
 * @see  softap_set_disabled_cb()
 */
int softap_unset_disabled_cb(softap_h softap);

/**
 * @brief Sets the callback function, which is called when the state of connection is changed.
 * @since_tizen 5.0
 * @privlevel public
 * @privilege %http://tizen.org/privilege/softap
 * @param[in]  softap  The SoftAP handle
 * @param[in]  callback  The callback function to invoke
 * @param[in]  user_data  The user data to be passed to the callback function
 * @return 0 on success, otherwise negative error value
 * @retval  #SOFTAP_ERROR_NONE              Successful
 * @retval  #SOFTAP_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval  #SOFTAP_ERROR_NOT_SUPPORTED     API is not supported
 * @retval  #SOFTAP_ERROR_PERMISSION_DENIED Permission denied
 * @see  softap_unset_client_connection_state_changed_cb()
 */
int softap_set_client_connection_state_changed_cb(softap_h softap, softap_client_connection_state_changed_cb callback, void *user_data);

/**
 * @brief Unsets the callback function, which is called when the state of connection is changed.
 * @since_tizen 5.0
 * @privlevel public
 * @privilege %http://tizen.org/privilege/softap
 * @param[in]  softap  The SoftAP handle
 * @return 0 on success, otherwise negative error value
 * @retval  #SOFTAP_ERROR_NONE              Successful
 * @retval  #SOFTAP_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval  #SOFTAP_ERROR_NOT_SUPPORTED     API is not supported
 * @retval  #SOFTAP_ERROR_PERMISSION_DENIED Permission denied
 * @see  softap_set_client_connection_state_changed_cb()
 */
int softap_unset_client_connection_state_changed_cb(softap_h softap);

/**
 * @brief Sets the callback function, which is called when the security type of SoftAP is changed.
 * @since_tizen 5.0
 * @privlevel public
 * @privilege %http://tizen.org/privilege/softap
 * @param[in]  softap  The SoftAP handle
 * @param[in]  callback  The callback function to invoke
 * @param[in]  user_data  The user data to be passed to the callback function
 * @return 0 on success, otherwise negative error value
 * @retval  #SOFTAP_ERROR_NONE              Successful
 * @retval  #SOFTAP_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval  #SOFTAP_ERROR_NOT_SUPPORTED     API is not supported
 * @retval  #SOFTAP_ERROR_PERMISSION_DENIED Permission denied
 * @see  softap_unset_security_type_changed_cb()
 */
int softap_set_security_type_changed_cb(softap_h softap, softap_security_type_changed_cb callback, void *user_data);

/**
 * @brief Unsets the callback function, which is called when the security type of SoftAP is changed.
 * @since_tizen 5.0
 * @privlevel public
 * @privilege %http://tizen.org/privilege/softap
 * @param[in]  softap  The SoftAP handle
 * @return 0 on success, otherwise negative error value
 * @retval  #SOFTAP_ERROR_NONE              Successful
 * @retval  #SOFTAP_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval  #SOFTAP_ERROR_NOT_SUPPORTED     API is not supported
 * @retval  #SOFTAP_ERROR_PERMISSION_DENIED Permission denied
 * @see  softap_set_security_type_changed_cb()
 */
int softap_unset_security_type_changed_cb(softap_h softap);

/**
 * @brief Sets the callback function , which is called when the visibility of SSID is changed.
 * @since_tizen 5.0
 * @privlevel public
 * @privilege %http://tizen.org/privilege/softap
 * @param[in]  softap  The SoftAP handle
 * @param[in]  callback  The callback function to invoke
 * @param[in]  user_data  The user data to be passed to the callback function
 * @return 0 on success, otherwise negative error value
 * @retval  #SOFTAP_ERROR_NONE              Successful
 * @retval  #SOFTAP_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval  #SOFTAP_ERROR_NOT_SUPPORTED     API is not supported
 * @retval  #SOFTAP_ERROR_PERMISSION_DENIED Permission denied
 * @see  softap_unset_ssid_visibility_changed_cb()
 */
int softap_set_ssid_visibility_changed_cb(softap_h softap, softap_ssid_visibility_changed_cb callback, void *user_data);

/**
 * @brief Unsets the callback function, which is called when the visibility of SSID is changed.
 * @since_tizen 5.0
 * @privlevel public
 * @privilege %http://tizen.org/privilege/softap
 * @param[in]  softap  The SoftAP handle
 * @return  0 on success, otherwise a negative error value
 * @retval  #SOFTAP_ERROR_NONE              Successful
 * @retval  #SOFTAP_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval  #SOFTAP_ERROR_NOT_SUPPORTED     API is not supported
 * @retval  #SOFTAP_ERROR_PERMISSION_DENIED Permission denied
 * @see  softap_set_ssid_visibility_changed_cb()
 */
int softap_unset_ssid_visibility_changed_cb(softap_h softap);

/**
 * @brief Sets the callback function, which is called when the passphrase of SoftAP is changed.
 * @since_tizen 5.0
 * @privlevel public
 * @privilege %http://tizen.org/privilege/softap
 * @param[in]  softap  The SoftAP handle
 * @param[in]  callback  The callback function to invoke
 * @param[in]  user_data  The user data to be passed to the callback function
 * @return 0 on success, otherwise negative error value
 * @retval  #SOFTAP_ERROR_NONE              Successful
 * @retval  #SOFTAP_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval  #SOFTAP_ERROR_NOT_SUPPORTED     API is not supported
 * @retval  #SOFTAP_ERROR_PERMISSION_DENIED Permission denied
 * @see  softap_unset_passphrase_changed_cb()
 */
int softap_set_passphrase_changed_cb(softap_h softap, softap_passphrase_changed_cb callback, void *user_data);

/**
 * @brief Unsets the callback function, which is called when the passphrase of SoftAP is changed.
 * @since_tizen 5.0
 * @privlevel public
 * @privilege %http://tizen.org/privilege/softap
 * @param[in]  softap  The SoftAP handle
 * @return  0 on success, otherwise a negative error value
 * @retval  #SOFTAP_ERROR_NONE              Successful
 * @retval  #SOFTAP_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval  #SOFTAP_ERROR_NOT_SUPPORTED     API is not supported
 * @retval  #SOFTAP_ERROR_PERMISSION_DENIED Permission denied
 * @see  softap_set_passphrase_changed_cb()
 */
int softap_unset_passphrase_changed_cb(softap_h softap);

/**
 * @}
 */

/**
 * @addtogroup CAPI_NETWORK_SOFTAP_MANAGER_MODULE
 * @{
 */

/**
 * @brief Sets the security type of SoftAP.
 * @details If security type is not set, WPA2_PSK is used.
 * @since_tizen 5.0
 * @privlevel public
 * @privilege %http://tizen.org/privilege/softap
 * @remarks This change is applied next time SoftAP is enabled. \
 *          You can use softap_enable() or softap_reload_settings() to enable SoftAP.
 * @param[in]  softap  The SoftAP handle
 * @param[in]  type  The security type
 * @return 0 on success, otherwise negative error value
 * @retval  #SOFTAP_ERROR_NONE              Successful
 * @retval  #SOFTAP_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval  #SOFTAP_ERROR_OPERATION_FAILED  Operation failed
 * @retval  #SOFTAP_ERROR_NOT_SUPPORTED     API is not supported
 * @retval  #SOFTAP_ERROR_PERMISSION_DENIED Permission denied
 * @see  softap_get_security_type()
 */
int softap_set_security_type(softap_h softap, softap_security_type_e type);

/**
 * @brief Gets the security type of SoftAP.
 * @details If security type is not set, WPA2_PSK is used.
 * @since_tizen 5.0
 * @privlevel public
 * @privilege %http://tizen.org/privilege/softap
 * @param[in]  softap  The SoftAP handle
 * @param[out]  type  The security type
 * @return 0 on success, otherwise negative error value
 * @retval  #SOFTAP_ERROR_NONE              Successful
 * @retval  #SOFTAP_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval  #SOFTAP_ERROR_NOT_SUPPORTED     API is not supported
 * @retval  #SOFTAP_ERROR_PERMISSION_DENIED Permission denied
 * @see  softap_set_security_type()
 */
int softap_get_security_type(softap_h softap, softap_security_type_e *type);

/**
 * @brief Sets the SSID (service set identifier) for SoftAP.
 * @details The SSID cannot exceed 32 bytes. If SSID is not set, device name is used as SSID.
 * @since_tizen 5.0
 * @privlevel public
 * @privilege %http://tizen.org/privilege/softap
 * @remarks This change is applied next time SoftAP is enabled. \n
 *          You can use softap_enable() or softap_reload_settings() to enable SoftAP.
 * @param[in]  softap  The SoftAP handle
 * @param[in]  ssid  The SSID
 * @return 0 on success, otherwise negative error value
 * @retval  #SOFTAP_ERROR_NONE              Successful
 * @retval  #SOFTAP_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval  #SOFTAP_ERROR_OUT_OF_MEMORY  Out of memory
 * @retval  #SOFTAP_ERROR_NOT_SUPPORTED     API is not supported
 * @retval  #SOFTAP_ERROR_PERMISSION_DENIED Permission denied
 * @see  softap_get_ssid()
 */
int softap_set_ssid(softap_h softap, const char *ssid);

/**
 * @brief Gets the SSID (service set identifier) for SoftAP.
 * @details If SSID is not set, Device name is used as SSID.
 * @since_tizen 5.0
 * @privlevel public
 * @privilege %http://tizen.org/privilege/softap
 * @remarks The @a ssid should be released using free().
 * @param[in]  softap  The SoftAP handle
 * @param[out]  ssid  The SSID
 * @return 0 on success, otherwise negative error value
 * @retval  #SOFTAP_ERROR_NONE              Successful
 * @retval  #SOFTAP_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval  #SOFTAP_ERROR_OUT_OF_MEMORY  Out of memory
 * @retval  #SOFTAP_ERROR_NOT_SUPPORTED     API is not supported
 * @retval  #SOFTAP_ERROR_PERMISSION_DENIED Permission denied
 * @see  softap_set_ssid()
 */
int softap_get_ssid(softap_h softap, char **ssid);

/**
 * @brief Sets the visibility of SSID (service set identifier) for SoftAP.
 * @details If you set the visibility to invisible, then the SSID of this device is hidden and Wi-Fi scan won't find your device.
 * @details By default visibility is set to @c true.
 * @since_tizen 5.0
 * @privlevel public
 * @privilege %http://tizen.org/privilege/softap
 * @remarks This change is applied next time SoftAP is enabled. \n
 *          You can use softap_enable() or softap_reload_settings() to enable SoftAP.
 * @param[in]  softap  The SoftAP handle
 * @param[in]  visible  The visibility of SSID: (@c true = visible, @c false = invisible)
 * @return 0 on success, otherwise negative error value
 * @retval  #SOFTAP_ERROR_NONE              Successful
 * @retval  #SOFTAP_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval  #SOFTAP_ERROR_OPERATION_FAILED  Operation failed
 * @retval  #SOFTAP_ERROR_NOT_SUPPORTED     API is not supported
 * @retval  #SOFTAP_ERROR_PERMISSION_DENIED Permission denied
 * @see  softap_get_ssid_visibility()
 */
int softap_set_ssid_visibility(softap_h softap, bool visible);

/**
 * @brief Gets the visibility of SSID (service set identifier) for SoftAP.
 * @details If the visibility is set to invisible, then the SSID of this device is hidden and Wi-Fi scan won't find your device.
 * @since_tizen 5.0
 * @privlevel public
 * @privilege %http://tizen.org/privilege/softap
 * @param[in]  softap  The SoftAP handle
 * @param[out]  visible  The visibility of SSID: (@c true = visible, @c false = invisible)
 * @return 0 on success, otherwise negative error value
 * @retval  #SOFTAP_ERROR_NONE              Successful
 * @retval  #SOFTAP_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval  #SOFTAP_ERROR_NOT_SUPPORTED     API is not supported
 * @retval  #SOFTAP_ERROR_PERMISSION_DENIED Permission denied
 * @see  softap_set_ssid_visibility()
 */
int softap_get_ssid_visibility(softap_h softap, bool *visible);

/**
 * @brief Sets the passphrase for SoftAP.
 * @details If the passphrase is not set, random string of 8 characters is used.
 * @since_tizen 5.0
 * @privlevel public
 * @privilege %http://tizen.org/privilege/softap
 * @remarks This change is applied next time SoftAP is enabled. \n
 *          You can use softap_enable() or softap_reload_settings() to enable SoftAP.
 * @param[in]  softap  The SoftAP handle
 * @param[in]  passphrase  The passphrase
 * @return 0 on success, otherwise negative error value
 * @retval  #SOFTAP_ERROR_NONE              Successful
 * @retval  #SOFTAP_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval  #SOFTAP_ERROR_NOT_SUPPORTED     API is not supported
 * @retval  #SOFTAP_ERROR_PERMISSION_DENIED Permission denied
 * @see  softap_get_passphrase()
 */
int softap_set_passphrase(softap_h softap, const char *passphrase);

/**
 * @brief Gets the passphrase for SoftAP.
 * @details If the passphrase is not set, random string of 8 characters will be used.
 * @since_tizen 5.0
 * @privlevel public
 * @privilege %http://tizen.org/privilege/softap
 * @remarks The @a passphrase should be released using free().
 * @param[in]  softap  The SoftAP handle
 * @param[out]  passphrase  The passphrase
 * @return 0 on success, otherwise negative error value
 * @retval  #SOFTAP_ERROR_NONE              Successful
 * @retval  #SOFTAP_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval  #SOFTAP_ERROR_OUT_OF_MEMORY  Out of memory
 * @retval  #SOFTAP_ERROR_NOT_SUPPORTED     API is not supported
 * @retval  #SOFTAP_ERROR_PERMISSION_DENIED Permission denied
 * @see  softap_set_passphrase()
 */
int softap_get_passphrase(softap_h softap, char **passphrase);

/**
 * @platform
 * @brief Reloads the settings (SSID / Passphrase / Security type / SSID visibility) for SoftAP.
 * @since_tizen 5.0
 * @privlevel platform
 * @privilege %http://tizen.org/privilege/softap.admin
 * @remarks Devices connected via MobileAP will be disconnected when the settings are reloaded.
 * @param[in]  softap  The SoftAP handle
 * @param[in]  callback  The callback function to invoke
 * @param[in]  user_data  The user data to be passed to the callback function
 * @return 0 on success, otherwise negative error value
 * @retval  #SOFTAP_ERROR_NONE              Successful
 * @retval  #SOFTAP_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval  #SOFTAP_ERROR_OPERATION_FAILED  Operation failed
 * @retval  #SOFTAP_ERROR_RESOURCE_BUSY     Device or resource busy
 * @retval  #SOFTAP_ERROR_NOT_SUPPORTED     API is not supported
 * @retval  #SOFTAP_ERROR_PERMISSION_DENIED Permission denied
 */
int softap_reload_settings(softap_h softap, softap_settings_reloaded_cb callback, void *user_data);

/**
 * @brief Pushes the WPS button to connect with SoftAP client(WPS PBC).
 * @since_tizen 5.0
 * @privlevel public
 * @privilege %http://tizen.org/privilege/softap
 * @remarks The WPS button should be pushed when client tries to connect with SoftAP by using WPS PBC.
 * @param[in]  softap  The SoftAP handle
 * @return 0 on success, otherwise negative error value
 * @retval  #SOFTAP_ERROR_NONE              Successful
 * @retval  #SOFTAP_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval  #SOFTAP_ERROR_OPERATION_FAILED  Operation failed
 * @retval  #SOFTAP_ERROR_NOT_SUPPORTED     API is not supported
 * @retval  #SOFTAP_ERROR_PERMISSION_DENIED Permission denied
 */
int softap_push_wps_button(softap_h softap);

/**
 * @brief Sets the WPS PIN to connect with SoftAP client(WPS PIN).
 * @since_tizen 5.0
 * @privlevel public
 * @privilege %http://tizen.org/privilege/softap
 * @remarks The WPS PIN should be inserted when client tries to connect with SoftAP by using WPS PIN.
 * @param[in]  softap  The SoftAP handle
 * @param[in]  wps_pin  The WPS PIN
 * @return 0 on success, otherwise negative error value
 * @retval  #SOFTAP_ERROR_NONE              Successful
 * @retval  #SOFTAP_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval  #SOFTAP_ERROR_OPERATION_FAILED  Operation failed
 * @retval  #SOFTAP_ERROR_NOT_SUPPORTED     API is not supported
 * @retval  #SOFTAP_ERROR_PERMISSION_DENIED Permission denied
 */
int softap_set_wps_pin(softap_h softap, const char *wps_pin);

/**
 * @brief Sets vendor specific elements for Beacon and Probe Response frames.
 * @since_tizen 5.0
 * @privlevel public
 * @privilege %http://tizen.org/privilege/softap
 * @remarks The format for vendor parameter should be a hex dump of the raw information elements. \n
 *          This change is applied next time SoftAP is enabled. \n
 *          You can use softap_enable() or softap_reload_settings() to enable SoftAP.
 * @param[in]  softap  The SoftAP handle
 * @param[in]  vendor_element  The vendor specific elements
 * @return 0 on success, otherwise negative error value
 * @retval  #SOFTAP_ERROR_NONE              Successful
 * @retval  #SOFTAP_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval  #SOFTAP_ERROR_NOT_SUPPORTED     API is not supported
 * @retval  #SOFTAP_ERROR_PERMISSION_DENIED Permission denied
 * @see  softap_get_vendor_element()
 */
int softap_set_vendor_element(softap_h softap, const char *vendor_element);

/**
 * @brief Gets vendor specific elements for Beacon and Probe Response frames.
 * @since_tizen 5.0
 * @privlevel public
 * @privilege %http://tizen.org/privilege/softap
 * @remarks The format for vendor parameter should be a hex dump of the raw information elements. \n
 *           The @a vendor_element should be released using free().
 * @param[in]  softap  The SoftAP handle
 * @param[out] vendor_element  The vendor specific elements
 * @return 0 on success, otherwise negative error value
 * @retval  #SOFTAP_ERROR_NONE              Successful
 * @retval  #SOFTAP_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval  #SOFTAP_ERROR_NOT_SUPPORTED     API is not supported
 * @retval  #SOFTAP_ERROR_PERMISSION_DENIED Permission denied
 * @see  softap_set_vendor_element()
 */
int softap_get_vendor_element(softap_h softap, char **vendor_element);

/**
 * @brief Sets the channel for SoftAP.
 * @details If channel is not set, channel 1 is used as default.
 * @since_tizen 5.0
 * @privlevel public
 * @privilege %http://tizen.org/privilege/softap
 * @remarks This change is applied next time SoftAP is enabled.\n
 *          You can use softap_enable() or softap_reload_settings() to enable SoftAP.\n
 *          If @a channel is 0, the channel can be selected automatically at run time.\n
 *          (ACS: Automatic Channel Selection)
 * @param[in]  softap   The SoftAP handle
 * @param[in]  channel  The channel number
 * @return 0 on success, otherwise negative error value
 * @retval  #SOFTAP_ERROR_NONE              Successful
 * @retval  #SOFTAP_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval  #SOFTAP_ERROR_NOT_SUPPORTED     API is not supported
 * @retval  #SOFTAP_ERROR_PERMISSION_DENIED Permission denied
 * @see  softap_get_channel()
 */
int softap_set_channel(softap_h softap, int channel);

/**
 * @brief Gets the channel for SoftAP.
 * @since_tizen 5.0
 * @privlevel public
 * @privilege %http://tizen.org/privilege/softap
 * @param[in]   softap   The SoftAP handle
 * @param[out]  channel  The channel number
 * @return 0 on success, otherwise negative error value
 * @retval  #SOFTAP_ERROR_NONE              Successful
 * @retval  #SOFTAP_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval  #SOFTAP_ERROR_NOT_SUPPORTED     API is not supported
 * @retval  #SOFTAP_ERROR_PERMISSION_DENIED Permission denied
 * @see  softap_set_channel()
 */
int softap_get_channel(softap_h softap, int *channel);

/**
 * @brief Enables the DHCP server.
 * @details Enable/disable the DHCP server.
 * @since_tizen 5.0
 * @privlevel public
 * @privilege %http://tizen.org/privilege/softap
 * @param[in]  softap   The SoftAP handle
 * @return 0 on success, otherwise negative error value.
 * @retval  #SOFTAP_ERROR_NONE              Successful
 * @retval  #SOFTAP_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval  #SOFTAP_ERROR_NOT_SUPPORTED     API is not supported
 * @retval  #SOFTAP_ERROR_PERMISSION_DENIED Permission denied
 * @see  softap_disable_dhcp()
 */
int softap_enable_dhcp(softap_h softap);

/**
 * @brief Disables the DHCP server.
 * @details Enable/disable the DHCP server.
 * @since_tizen 5.0
 * @privlevel public
 * @privilege %http://tizen.org/privilege/softap
 * @param[in]  softap   The SoftAP handle
 * @return 0 on success, otherwise negative error value.
 * @retval  #SOFTAP_ERROR_NONE              Successful
 * @retval  #SOFTAP_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval  #SOFTAP_ERROR_NOT_SUPPORTED     API is not supported
 * @retval  #SOFTAP_ERROR_PERMISSION_DENIED Permission denied
 * @see  softap_enable_dhcp()
 * @see  softap_enable_dhcp_with_range()
 */
int softap_disable_dhcp(softap_h softap);

/**
 * @brief Checks whether the DHCP is enabled or not.
 * @since_tizen 5.0
 * @privlevel public
 * @privilege %http://tizen.org/privilege/softap
 * @param[in]  softap  The SoftAP handle
 * @param[out] dhcp_enabled  @c true if DHCP is enabled, \n @c false if DHCP is disabled
 * @return  0 on success, otherwise a negative error value
 * @retval  #SOFTAP_ERROR_NONE              Successful
 * @retval  #SOFTAP_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval  #SOFTAP_ERROR_PERMISSION_DENIED Permission denied
 **/
int softap_is_dhcp_enabled(softap_h softap, bool *dhcp_enabled);

/**
 * @brief Enables the DHCP server with the address range.
 * @details Enable the DHCP server with the address range.
 * @since_tizen 5.0
 * @privlevel public
 * @privilege %http://tizen.org/privilege/softap
 * @param[in]  softap  The handle of softap
 * @param[in]  rangestart Start address range
 * @param[in]  rangestop  End address range
 * @return 0 on success, otherwise negative error value.
 * @retval  #SOFTAP_ERROR_NONE              Successful
 * @retval  #SOFTAP_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval  #SOFTAP_ERROR_NOT_SUPPORTED     API is not supported
 * @retval  #SOFTAP_ERROR_PERMISSION_DENIED Permission denied
 * @see  softap_disable_dhcp()
 **/
int softap_enable_dhcp_with_range(softap_h softap, char *rangestart, char *rangestop);

/**
 * @brief Sets the wireless mode for SoftAP.
 * @details If wireless mode is not set, IEEE 802.11g is used as default.
 * @since_tizen 5.0
 * @privlevel public
 * @privilege %http://tizen.org/privilege/softap
 * @param[in]  softap   The SoftAP handle
 * @param[in]  mode     The mode
 * @return 0 on success, otherwise negative error value
 * @retval  #SOFTAP_ERROR_NONE              Successful
 * @retval  #SOFTAP_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval  #SOFTAP_ERROR_NOT_SUPPORTED     API is not supported
 * @retval  #SOFTAP_ERROR_PERMISSION_DENIED Permission denied
 * @see  softap_get_mode()
 */
int softap_set_mode(softap_h softap, softap_wireless_mode_e mode);

/**
 * @brief Gets the wireless mode for SoftAP.
 * @since_tizen 5.0
 * @privlevel public
 * @privilege %http://tizen.org/privilege/softap
 * @param[in]  softap   The SoftAP handle
 * @param[out] mode     The wireless mode
 * @return 0 on success, otherwise negative error value
 * @retval  #SOFTAP_ERROR_NONE              Successful
 * @retval  #SOFTAP_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval  #SOFTAP_ERROR_NOT_SUPPORTED     API is not supported
 * @retval  #SOFTAP_ERROR_PERMISSION_DENIED Permission denied
 * @see  softap_set_mode()
 */
int softap_get_mode(softap_h softap, softap_wireless_mode_e *mode);

/**
 * @}
 */


/**
 * @addtogroup CAPI_NETWORK_SOFTAP_CLIENT_MODULE
 * @{
 */

/**
 * @brief Clones the handle of a client.
 * @since_tizen 5.0
 * @privlevel public
 * @privilege %http://tizen.org/privilege/softap
 * @remarks The @a dest should be released using softap_client_destroy().
 * @param[out]  dest  The cloned client handle
 * @param[in]  origin  The origin client handle
 * @return  0 on success, otherwise a negative error value
 * @retval  #SOFTAP_ERROR_NONE              Successful
 * @retval  #SOFTAP_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval  #SOFTAP_ERROR_OUT_OF_MEMORY  Out of memory
 * @retval  #SOFTAP_ERROR_NOT_SUPPORTED     API is not supported
 * @retval  #SOFTAP_ERROR_PERMISSION_DENIED Permission denied
 * @see  softap_client_destroy()
 */
int softap_client_clone(softap_client_h *dest, softap_client_h origin);

/**
 * @brief Destroys the handle of a client.
 * @since_tizen 5.0
 * @privlevel public
 * @privilege %http://tizen.org/privilege/softap
 * @param[in]  client  The client handle
 * @return  0 on success, otherwise a negative error value
 * @retval  #SOFTAP_ERROR_NONE              Successful
 * @retval  #SOFTAP_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval  #SOFTAP_ERROR_NOT_SUPPORTED     API is not supported
 * @retval  #SOFTAP_ERROR_PERMISSION_DENIED Permission denied
 * @see  softap_client_clone()
 */
int softap_client_destroy(softap_client_h client);

/**
 * @brief Gets the name of a client.
 * @since_tizen 5.0
 * @privlevel public
 * @privilege %http://tizen.org/privilege/softap
 * @remarks The @a name should be released using free().
 * @param[in]  client  The client handle
 * @param[out]  name  The name of the client
 * @return  0 on success, otherwise a negative error value
 * @retval  #SOFTAP_ERROR_NONE              Successful
 * @retval  #SOFTAP_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval  #SOFTAP_ERROR_OUT_OF_MEMORY  Out of memory
 * @retval  #SOFTAP_ERROR_NOT_SUPPORTED     API is not supported
 * @retval  #SOFTAP_ERROR_PERMISSION_DENIED Permission denied
 * @see  softap_client_connection_state_changed_cb()
 */
int softap_client_get_name(softap_client_h client, char **name);

/**
 * @brief Gets the IP address of a client.
 * @since_tizen 5.0
 * @privlevel public
 * @privilege %http://tizen.org/privilege/softap
 * @remarks The @a ip_address should be released using free().
 * @param[in]  client  The client handle
 * @param[in]  address_family  The address family of IP address. Currently, #SOFTAP_ADDRESS_FAMILY_IPV4 is only supported
 * @param[out]  ip_address  The IP address
 * @return  0 on success, otherwise a negative error value
 * @retval  #SOFTAP_ERROR_NONE              Successful
 * @retval  #SOFTAP_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval  #SOFTAP_ERROR_OUT_OF_MEMORY  Out of memory
 * @retval  #SOFTAP_ERROR_NOT_SUPPORTED     API is not supported
 * @retval  #SOFTAP_ERROR_PERMISSION_DENIED Permission denied
 * @see  softap_client_connection_state_changed_cb()
 */
int softap_client_get_ip_address(softap_client_h client, softap_address_family_e address_family, char **ip_address);

/**
 * @brief Gets the MAC address of a client.
 * @since_tizen 5.0
 * @privlevel public
 * @privilege %http://tizen.org/privilege/softap
 * @remarks The @a mac_address should be released using free().
 * @param[in]  client  The client handle
 * @param[out]  mac_address  The MAC address
 * @return  0 on success, otherwise a negative error value
 * @retval  #SOFTAP_ERROR_NONE              Successful
 * @retval  #SOFTAP_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval  #SOFTAP_ERROR_OUT_OF_MEMORY  Out of memory
 * @retval  #SOFTAP_ERROR_NOT_SUPPORTED     API is not supported
 * @retval  #SOFTAP_ERROR_PERMISSION_DENIED Permission denied
 * @see  softap_client_connection_state_changed_cb()
 */
int softap_client_get_mac_address(softap_client_h client, char **mac_address);

/**
 * @brief Gets the connection time of a client.
 * @since_tizen 5.0
 * @privlevel public
 * @privilege %http://tizen.org/privilege/softap
 * @param[in]  client    The client handle
 * @param[out] timestamp The connected time of the client
 * @return  0 on success, otherwise a negative error value
 * @retval  #SOFTAP_ERROR_NONE              Successful
 * @retval  #SOFTAP_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval  #SOFTAP_ERROR_NOT_SUPPORTED     API is not supported
 * @retval  #SOFTAP_ERROR_PERMISSION_DENIED Permission denied
 * @see  softap_client_connection_state_changed_cb()
 */
int softap_client_get_time(softap_client_h client, time_t *timestamp);

/**
 * @}
 */

#ifdef __cplusplus
}
#endif

#endif /* __TIZEN_NETWORK_SOFTAP_H__ */


