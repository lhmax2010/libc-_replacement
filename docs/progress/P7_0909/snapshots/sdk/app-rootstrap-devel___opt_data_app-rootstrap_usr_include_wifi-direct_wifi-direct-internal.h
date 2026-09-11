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

#ifndef __WIFI_DIRECT_INTERNAL_H__
#define __WIFI_DIRECT_INTERNAL_H__

/**
 * @file wifi-direct-internal.h
 * @brief This file declares has API of the wifi-direct library
 */

#include "wifi-direct.h"

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @brief Called when you get the supported channels repeatedly.
 * @since_tizen 6.0
 * @param[in] channel       The supported channel
 * @param[in] user_data  The user data passed from the request function
 * @return @c true to continue with the next iteration of the loop, \n
 *         @c false to break out of the loop
 * @pre wifi_direct_foreach_supported_channel() will invoke this callback.
 * @see wifi_direct_foreach_supported_channel()
 */
typedef bool(*wifi_direct_supported_channel_cb)(unsigned int channel, void *user_data);

/**
 * @brief Rejects the connection request from other device now in progress.
 * @since_tizen 2.3
 * @privlevel public
 * @privilege http://tizen.org/privilege/wifidirect
 * @param[in] mac_address  The MAC address of rejected device
 * @retval #WIFI_DIRECT_ERROR_NONE  Successful
 * @retval #WIFI_DIRECT_ERROR_OPERATION_FAILED  Operation failed
 * @retval #WIFI_DIRECT_ERROR_COMMUNICATION_FAILED  Communication failed
 * @retval #WIFI_DIRECT_ERROR_PERMISSION_DENIED  Permission denied
 * @retval #WIFI_DIRECT_ERROR_NOT_PERMITTED  Operation not permitted
 * @retval #WIFI_DIRECT_ERROR_NOT_SUPPORTED  Not supported
 * @retval #WIFI_DIRECT_ERROR_NOT_INITIALIZED  Not initialized
 * @retval #WIFI_DIRECT_ERROR_RESOURCE_BUSY  Device or resource busy
 */
int wifi_direct_reject_connection(char *mac_address);

/**
 * @brief Accepts a connection requested from PEER.
 * @since_tizen 2.3
 * @privlevel public
 * @privilege http://tizen.org/privilege/wifidirect
 * @param[in] mac_address  MAC Address of the PEER.
 * @return 0 on success, otherwise a negative error value.
 * @retval #WIFI_DIRECT_ERROR_NONE  Successful
 * @retval #WIFI_DIRECT_ERROR_INVALID_PARAMETER  Invalid parameter
 * @retval #WIFI_DIRECT_ERROR_OPERATION_FAILED  Operation failed
 * @retval #WIFI_DIRECT_ERROR_COMMUNICATION_FAILED  Communication failed
 * @retval #WIFI_DIRECT_ERROR_PERMISSION_DENIED  Permission denied
 * @retval #WIFI_DIRECT_ERROR_NOT_PERMITTED  Operation not permitted
 * @retval #WIFI_DIRECT_ERROR_NOT_SUPPORTED  Not supported
 * @retval #WIFI_DIRECT_ERROR_NOT_INITIALIZED  Not initialized
 * @retval #WIFI_DIRECT_ERROR_RESOURCE_BUSY  Device or resource busy
 * @pre Wi-Fi Direct service must be initialized by wifi_direct_initialize().
 * @pre Wi-Fi Direct service must be activated by wifi_direct_activate().
 * @see wifi_direct_initialize()
 * @see wifi_direct_activate()
 * @see wifi_direct_reject_connection()
 */
int wifi_direct_accept_connection(char *mac_address);

/**
 * @brief Gets the list of supported channels
 * @since_tizen 6.0
 * @privlevel public
 * @privilege %http://tizen.org/privilege/wifidirect
 * @param[in] callback   The callback function which is invoked at success
 * @param[in] user_data  The user data to be passed to the callback function
 * @return @c 0 on success,
 *         otherwise a negative error value
 * @retval #WIFI_DIRECT_ERROR_NONE                  Successful
 * @retval #WIFI_DIRECT_ERROR_INVALID_PARAMETER     Invalid parameter
 * @retval #WIFI_DIRECT_ERROR_OPERATION_FAILED      Operation failed
 * @retval #WIFI_DIRECT_ERROR_COMMUNICATION_FAILED  Communication failed
 * @retval #WIFI_DIRECT_ERROR_PERMISSION_DENIED     Permission denied
 * @retval #WIFI_DIRECT_ERROR_NOT_PERMITTED         Operation not permitted
 * @retval #WIFI_DIRECT_ERROR_NOT_SUPPORTED         Not supported
 * @retval #WIFI_DIRECT_ERROR_NOT_INITIALIZED       Not initialized
 * @retval #WIFI_DIRECT_ERROR_RESOURCE_BUSY         Device or resource busy
 * @pre Wi-Fi Direct service must be activated by wifi_direct_activate().
 * @see wifi_direct_activate()
 * @see wifi_direct_supported_channel_cb()
 *
 *
 * Here is an example of the usage:
 * @code
 * #include <stdio.h>
 * #include <wifi-direct.h>
 * #include <wifi-direct-internal.h>
 *
 *
 * bool callback(unsigned int channel, void *user_data)
 * {
 * 	printf("Channel [%u] is supported\n", channel);
 * 	return true;
 * }
 *
 * int function(void)
 * {
 * 	int ret;
 *
 * 	ret = wifi_direct_foreach_supported_channel(callback, NULL);
 *
 * 	if (ret != WIFI_DIRECT_ERROR_NONE) {
 * 		printf("Failed to get supported channels\n");
 * 		return -1;
 * 	}
 *
 * 	return 0;
 * }
 *
 * int main()
 * {
 * 	// Initialize Wi-Fi Direct.
 * 	// Activate Wi-Fi Direct.
 *
 * 	function();
 *
 * 	// App must clean up Wi-Fi Direct before exiting
 *
 * 	// Deactivate Wi-Fi Direct
 * 	// Deinitialize Wi-Fi Direct
 * 	return 0;
 * }
 * @endcode
 */
int wifi_direct_foreach_supported_channel(wifi_direct_supported_channel_cb callback, void *user_data);

/**
 * @brief Sets the group owner IP address for Wi-Fi Direct.
 * @param[in] go_ip  IP address of the group owner to be set.
 * @return 0 on success, otherwise a negative error value.
 * @retval #WIFI_DIRECT_ERROR_NONE  Successful
 * @retval #WIFI_DIRECT_ERROR_INVALID_PARAMETER  Invalid parameter (e.g., NULL or invalid IP address)
 * @retval #WIFI_DIRECT_ERROR_NOT_SUPPORTED  Wi-Fi Direct feature not supported
 * @retval #WIFI_DIRECT_ERROR_NOT_INITIALIZED  Service not initialized
 * @retval #WIFI_DIRECT_ERROR_COMMUNICATION_FAILED  Communication error with DBus
 * @retval #WIFI_DIRECT_ERROR_OPERATION_FAILED  Operation failed
 * @retval #WIFI_DIRECT_ERROR_PERMISSION_DENIED  Permission denied
 * @retval #WIFI_DIRECT_ERROR_NOT_PERMITTED  Operation not permitted
 * @retval #WIFI_DIRECT_ERROR_RESOURCE_BUSY  Device or resource busy
 * @pre Wi-Fi Direct service must be initialized by \ref wifi_direct_initialize().
 * @see wifi_direct_initialize()
 *
 * Here is the example of the usage:
 * @code
 * #include <stdio.h>
 * #include <wifi-direct.h>
 * #include <wifi-direct-internal.h>
 *
 * int function(void)
 * {
 *   int ret;
 *
 *   ret = wifi_direct_set_group_owner_ip("192.168.50.1");
 *
 *   if (ret != WIFI_DIRECT_ERROR_NONE) {
 *      printf("Failed to set EAPOL IP range");
 *      return -1;
 *   }
 *
 *   return 0;
 * }
 *
 * int main()
 * {
 *   // Initialize Wi-Fi Direct.
 *
 *   function();
 *
 *   // Activate Wi-Fi Direct.
 *
 *   ....
 *
 *   // App must clean up Wi-Fi Direct before exiting
 *   // Deactivate Wi-Fi Direct
 *   // Deinitialize Wi-Fi Direct
 *
 * }
 */
int wifi_direct_set_group_owner_ip(const char *go_ip);

/**
 * @brief Sets the DHCP IP address range for the Wi-Fi Direct group owner.
 * @param[in] go_ip          IP address of the group owner.
 * @param[in] dhcp_start_range  Start IP address of the DHCP range.
 * @param[in] dhcp_end_range   End IP address of the DHCP range.
 * @return 0 on success, otherwise a negative error value.
 * @retval #WIFI_DIRECT_ERROR_NONE  Successful
 * @retval #WIFI_DIRECT_ERROR_INVALID_PARAMETER  Invalid parameter (e.g., NULL, invalid IP, subnet mismatch, or invalid range)
 * @retval #WIFI_DIRECT_ERROR_NOT_SUPPORTED  Wi-Fi Direct feature not supported
 * @retval #WIFI_DIRECT_ERROR_NOT_INITIALIZED  Service not initialized
 * @retval #WIFI_DIRECT_ERROR_COMMUNICATION_FAILED  Communication error with DBus
 * @retval #WIFI_DIRECT_ERROR_OPERATION_FAILED  Operation failed
 * @retval #WIFI_DIRECT_ERROR_PERMISSION_DENIED  Permission denied
 * @retval #WIFI_DIRECT_ERROR_NOT_PERMITTED  Operation not permitted
 * @retval #WIFI_DIRECT_ERROR_RESOURCE_BUSY  Device or resource busy
 * @pre Wi-Fi Direct service must be initialized by \ref wifi_direct_initialize().
 * @see wifi_direct_initialize()
 *
 * Here is the example of the usage:
 * @code
 * #include <stdio.h>
 * #include <wifi-direct.h>
 * #include <wifi-direct-internal.h>
 *
 * int function(void)
 * {
 *   int ret;
 *
 *   ret = wifi_direct_set_dhcp_ip_range("192.168.49.1", "192.168.49.40", "192.168.49.60");
 *
 *   if (ret != WIFI_DIRECT_ERROR_NONE) {
 *      printf("Failed to set DHCP IP range");
 *      return -1;
 *   }
 *
 *   return 0;
 * }
 *
 * int main()
 * {
 *   // Initialize Wi-Fi Direct.
 *
 *   function();
 *
 *   // Activate Wi-Fi Direct.
 *
 *   ....
 *
 *   // App must clean up Wi-Fi Direct before exiting
 *   // Deactivate Wi-Fi Direct
 *   // Deinitialize Wi-Fi Direct
 *
 * }
 */
int wifi_direct_set_dhcpd_ip_range(const char *go_ip, const char *dhcp_start_range, const char *dhcp_end_range);

/**
 * @brief Sets the EAPOL IP address range for the Wi-Fi Direct group owner.
 * @param[in] go_ip             IP address of the group owner.
 * @param[in] eapol_start_range Start IP address of the EAPOL range.
 * @param[in] eapol_end_range   End IP address of the EAPOL range.
 * @return 0 on success, otherwise a negative error value.
 * @retval #WIFI_DIRECT_ERROR_NONE  Successful
 * @retval #WIFI_DIRECT_ERROR_INVALID_PARAMETER  Invalid parameter (e.g., NULL, invalid IP, subnet mismatch, or invalid range)
 * @retval #WIFI_DIRECT_ERROR_NOT_SUPPORTED  Wi-Fi Direct feature not supported
 * @retval #WIFI_DIRECT_ERROR_NOT_INITIALIZED  Service not initialized
 * @retval #WIFI_DIRECT_ERROR_COMMUNICATION_FAILED  Communication error with DBus
 * @retval #WIFI_DIRECT_ERROR_OPERATION_FAILED  Operation failed
 * @retval #WIFI_DIRECT_ERROR_PERMISSION_DENIED  Permission denied
 * @retval #WIFI_DIRECT_ERROR_NOT_PERMITTED  Operation not permitted
 * @retval #WIFI_DIRECT_ERROR_RESOURCE_BUSY  Device or resource busy
 * @pre Wi-Fi Direct service must be initialized by \ref wifi_direct_initialize().
 * @see wifi_direct_initialize()
 *
 * Here is the example of the usage:
 * @code
 * #include <stdio.h>
 * #include <wifi-direct.h>
 * #include <wifi-direct-internal.h>
 *
 * int function(void)
 * {
 *   int ret;
 *
 *   ret = wifi_direct_set_eapol_ip_range("192.168.49.1", "192.168.49.10", "192.168.49.30");
 *
 *   if (ret != WIFI_DIRECT_ERROR_NONE) {
 *      printf("Failed to set EAPOL IP range");
 *      return -1;
 *   }
 *
 *   return 0;
 * }
 *
 * int main()
 * {
 *   // Initialize Wi-Fi Direct.
 *
 *   function();
 *
 *   // Activate Wi-Fi Direct.
 *
 *   ....
 *
 *   // App must clean up Wi-Fi Direct before exiting
 *   // Deactivate Wi-Fi Direct
 *   // Deinitialize Wi-Fi Direct
 *
 * }
 */
int wifi_direct_set_eapol_ip_range(const char *go_ip, const char *eapol_start_range, const char *eapol_end_range);

/**
 * @brief Add DHCP vendor information.
 * @since_tizen 10.0
 * @privlevel public
 * @privilege http://tizen.org/privilege/wifidirect
 * @param[in] vendor_info The vendor information to add.
 * @retval #WIFI_DIRECT_ERROR_NONE  Successful
 * @retval #WIFI_DIRECT_ERROR_INVALID_PARAMETER  Invalid parameter
 * @retval #WIFI_DIRECT_ERROR_OPERATION_FAILED  Operation failed
 * @retval #WIFI_DIRECT_ERROR_NOT_PERMITTED  Operation not permitted
 * @retval #WIFI_DIRECT_ERROR_NOT_SUPPORTED  Not supported
 * @retval #WIFI_DIRECT_ERROR_NOT_INITIALIZED  Not initialized
 */
int wifi_direct_add_dhcp_vendor_info(const char *vendor_info);

#ifdef __cplusplus
}
#endif
#endif							/* __WIFI_DIRECT_INTERNAL_H_ */
