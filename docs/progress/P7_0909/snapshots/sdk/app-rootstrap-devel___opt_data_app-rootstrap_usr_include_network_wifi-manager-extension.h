/*
 * Copyright (c) 2012-2013 Samsung Electronics Co., Ltd All Rights Reserved
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

#ifndef __TIZEN_WIFI_MANAGER_EXTENSION_H__
#define __TIZEN_WIFI_MANAGER_EXTENSION_H__

#include "wifi-manager.h"

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @file wifi-manager-extension.h
 */


/**
 * @brief Enumeration for the Wi-Fi autoscan mode.
 * @since_tizen 4.0
 */
typedef enum {
	/**
	 * Auto scan interval is increased exponentially like 4, 8, 16, ...128secs
	 */
	WIFI_MANAGER_AUTOSCAN_MODE_EXPONENTIAL = 0x00,
	/**
	 * Auto scan interval is fixed with 10secs(for mobile) / 15secs(for wearable)
	 */
	WIFI_MANAGER_AUTOSCAN_MODE_PERIODIC
} wifi_manager_autoscan_mode_e;

/**
 * @brief Enumeration for the Wi-Fi Connection Mode.
 * @since_tizen 5.0
 */
typedef enum {
	WIFI_MANAGER_CONNECTION_MODE_UNKNOWN,     /**< Unknown */
	WIFI_MANAGER_CONNECTION_MODE_80211_B,    /**< IEEE 802.11b 2.4 Ghz */
	WIFI_MANAGER_CONNECTION_MODE_80211_G,    /**< IEEE 802.11g 2.4 Ghz */
	WIFI_MANAGER_CONNECTION_MODE_80211_N,    /**< IEEE 802.11n 2.4/5 Ghz */
	WIFI_MANAGER_CONNECTION_MODE_80211_A,    /**< IEEE 802.11a 5 Ghz */
	WIFI_MANAGER_CONNECTION_MODE_80211_AC,   /**< IEEE 802.11ac 5 Ghz */
	WIFI_MANAGER_CONNECTION_MODE_80211_AX,   /**< IEEE 802.11ax 5 Ghz */
	WIFI_MANAGER_CONNECTION_MODE_80211_BE,   /**< IEEE 802.11be */
} wifi_manager_connection_mode_e;

/**
 * @brief Enumeration for the Wi-Fi Mac Address Policy.
 * @since_tizen 6.5
 */
typedef enum {
	WIFI_MANAGER_MAC_POLICY_FIXED_MAC,              /**< Fixed Mac */
	WIFI_MANAGER_MAC_POLICY_RANDOM_OUI_RANDOM_MAC,  /**< Random OUI random Mac */
	WIFI_MANAGER_MAC_POLICY_FIXED_OUI_RANDOM_MAC,   /**< Fixed OUI random Mac */
} wifi_manager_mac_policy_e;

/**
 * @brief Enumeration for the Wi-Fi Roemaing State.
 * @since_tizen 7.0
 */
typedef enum {
	WIFI_MANAGER_ROAM_UNKNOWN,          /**< Unknown */
	WIFI_MANAGER_ROAM_SCAN_REQUIRED,    /**< The SNR and signal of the connected AP are weakened */
	WIFI_MANAGER_ROAM_STARTED,          /**< Wi-Fi Roaming is started */
	WIFI_MANAGER_ROAM_FAILURE,          /**< Wi-Fi Roaming failed */
	WIFI_MANAGER_ROAM_SUCCESS,          /**< Wi-Fi Roaming is successful. */
} wifi_manager_roam_e;

/**
 * @brief Enumeration for the Wi-Fi DHCP state.
 * @since_tizen 7.5
 */
typedef enum {
	WIFI_MANAGER_DHCP_STATE_UNKNOWN = 0,  /* Unknown state */
	WIFI_MANAGER_DHCP_STATE_STARTED = 1,  /* DHCP is running */
	WIFI_MANAGER_DHCP_STATE_FINISHED = 2, /* DHCP is finished */
} wifi_manager_dhcp_state_e;

/**
 * @brief Enumeration for the Wi-Fi Power-save State.
 * @since_tizen 7.5
 */
typedef enum {
	WIFI_MANAGER_PS_OFF = 0x00,
	WIFI_MANAGER_PS_ON,
} wifi_manager_power_save_state_e;

/**
 * @brief Enumeration for the Wi-Fi Power-save Mode.
 * @since_tizen 7.5
 */
typedef enum {
	WIFI_MANAGER_PS_MODE_DEFAULT_OFF = 0x00,
	WIFI_MANAGER_PS_MODE_DEFAULT_ON,
	WIFI_MANAGER_PS_MODE_DYNAMIC,
} wifi_manager_power_save_mode_e;

/* Wi-Fi band to scan */
typedef enum {
	WIFI_MANAGER_BAND_SELECTION_ALL = 0x00,
	WIFI_MANAGER_BAND_SELECTION_2_4GHZ,
	WIFI_MANAGER_BAND_SELECTION_5GHZ,
	WIFI_MANAGER_BAND_SELECTION_6GHZ,
} wifi_manager_band_selection_e;

/**
 * @brief The Wi-Fi netlink scan handle.
 * @since_tizen 5.0
 */
typedef void *wifi_manager_netlink_scan_h;

/**
* @}
*/

/**
* @addtogroup CAPI_NETWORK_WIFI_MANAGER_MANAGEMENT_MODULE
* @{
*/

/**
 * @brief Called for each found available interface.
 * @since_tizen 6.0
 * @param[in]  ifname       The interface name
 * @param[in]  user_data    The user data passed from the request function
 * @return  @c true to continue with the next iteration of the loop, \n
 *     otherwise @c false to break out of the loop
 * @pre  wifi_manager_foreach_available_interface() will invoke this callback.
 * @see  wifi_manager_foreach_available_interface()
 */
typedef bool(*wifi_manager_interface_cb)(char *ifname, void *user_data);

/**
 * @brief Initializes Wi-Fi with interface name.
 * @since_tizen 6.0
 * @privlevel public
 * @privilege %http://tizen.org/privilege/network.get
 * @remarks You must release @a wifi using wifi_manager_deinitialize().
 * @param[out] wifi        The Wi-Fi handle
 * @param[in]  ifname      The interface name
 * @return @c 0 on success, otherwise negative error value
 * @retval #WIFI_MANAGER_ERROR_NONE                 Successful
 * @retval #WIFI_MANAGER_ERROR_ALREADY_INITIALIZED  Already initialized
 * @retval #WIFI_MANAGER_ERROR_INVALID_PARAMETER    Invalid parameter
 * @retval #WIFI_MANAGER_ERROR_INVALID_OPERATION    Invalid operation
 * @retval #WIFI_MANAGER_ERROR_OPERATION_FAILED     Operation failed
 * @retval #WIFI_MANAGER_ERROR_PERMISSION_DENIED    Permission Denied
 * @retval #WIFI_MANAGER_ERROR_OUT_OF_MEMORY        Out of memory
 * @retval #WIFI_MANAGER_ERROR_NOT_SUPPORTED        Not supported
 */
int wifi_manager_initialize_with_interface_name(wifi_manager_h *wifi,
		const char *ifname);

/**
 * @brief Gets a list of available interfaces.
 * @since_tizen 6.0
 * @privlevel public
 * @privilege %http://tizen.org/privilege/network.get
 * @param[in] wifi            The Wi-Fi handle
 * @param[in] callback        The callback to be called
 * @param[in] user_data       The user data passed to the callback function
 * @return 0 on success, otherwise negative error value
 * @retval #WIFI_MANAGER_ERROR_NONE                 Successful
 * @retval #WIFI_MANAGER_ERROR_NOT_INITIALIZED      Not initialized
 * @retval #WIFI_MANAGER_ERROR_INVALID_PARAMETER    Invalid parameter
 * @retval #WIFI_MANAGER_ERROR_OPERATION_FAILED     Operation failed
 * @retval #WIFI_MANAGER_ERROR_PERMISSION_DENIED    Permission Denied
 * @retval #WIFI_MANAGER_ERROR_NOT_SUPPORTED        Not supported
 * @post This function invokes wifi_manager_found_ap_cb().
 */
int wifi_manager_foreach_available_interface(wifi_manager_h wifi,
		wifi_manager_interface_cb callback, void *user_data);

/**
 * @brief Gets the result of the scan in all interfaces.
 * @since_tizen 6.0
 * @privlevel public
 * @privilege %http://tizen.org/privilege/network.get
 * @param[in] wifi            The Wi-Fi handle
 * @param[in] callback        The callback to be called
 * @param[in] user_data       The user data passed to the callback function
 * @return 0 on success, otherwise negative error value
 * @retval #WIFI_MANAGER_ERROR_NONE                 Successful
 * @retval #WIFI_MANAGER_ERROR_NOT_INITIALIZED      Not initialized
 * @retval #WIFI_MANAGER_ERROR_INVALID_PARAMETER    Invalid parameter
 * @retval #WIFI_MANAGER_ERROR_OPERATION_FAILED     Operation failed
 * @retval #WIFI_MANAGER_ERROR_PERMISSION_DENIED    Permission Denied
 * @retval #WIFI_MANAGER_ERROR_NOT_SUPPORTED        Not supported
 * @post This function invokes wifi_manager_found_ap_cb().
 */
int wifi_manager_foreach_found_ap_in_all_interfaces(wifi_manager_h wifi,
		wifi_manager_found_ap_cb callback, void *user_data);

/**
 * @brief Gets configurations of an access point in all interfaces.
 * @since_tizen 6.0
 * @privlevel public
 * @privilege %http://tizen.org/privilege/network.profile
 *
 * @param[in] wifi              The Wi-Fi handle
 * @param[in] callback          The callback to be called
 * @param[in] user_data         The user data passed to the callback function
 *
 * @return 0 on success, otherwise negative error value
 * @retval #WIFI_MANAGER_ERROR_NONE                 Successful
 * @retval #WIFI_MANAGER_ERROR_NOT_INITIALIZED      Not initialized
 * @retval #WIFI_MANAGER_ERROR_INVALID_OPERATION    Invalid operation
 * @retval #WIFI_MANAGER_ERROR_INVALID_PARAMETER    Invalid parameter
 * @retval #WIFI_MANAGER_ERROR_PERMISSION_DENIED    Permission denied
 * @retval #WIFI_MANAGER_ERROR_NOT_SUPPORTED        Not supported
 * @pre This function needs wifi_manager_initialize() before use.
 * @post This function invokes wifi_manager_config_list_cb().
 */
int wifi_manager_config_foreach_configuration_in_all_interfaces(wifi_manager_h wifi,
			wifi_manager_config_list_cb callback, void *user_data);

/**
 * @brief Gets access point passphrase from configuration.
 * @since_tizen 9.0
 * @privlevel public
 * @privilege %http://tizen.org/privilege/wifi.secureinfo
 * @param[in] wifi          The Wi-Fi handle
 * @param[in] config        The access point configuration handle
 * @param[out] passphrase   The passphrase of access point
 *
 * @return 0 on success, otherwise negative error value.
 * @retval #WIFI_MANAGER_ERROR_NONE  Successful
 * @retval #WIFI_MANAGER_ERROR_INVALID_PARAMETER    Invalid parameter
 * @retval #WIFI_MANAGER_ERROR_OPERATION_FAILED     Operation failed
 * @retval #WIFI_MANAGER_ERROR_PERMISSION_DENIED    Permission Denied
 * @retval #WIFI_MANAGER_ERROR_NOT_SUPPORTED        Not supported
 */
int wifi_manager_config_get_passphrase(wifi_manager_h wifi,
		wifi_manager_config_h config, char **passphrase);

/**
 * @brief Reset all WiFi configurations
 * @since_tizen 7.5
 * @privlevel public
 * @privilege %http://tizen.org/privilege/network.profile
 * @param[in] wifi           The Wi-Fi handle
 *
 * @return 0 on success, otherwise negative error value.
 * @retval #WIFI_MANAGER_ERROR_NONE  Successful
 * @retval #WIFI_MANAGER_ERROR_INVALID_PARAMETER    Invalid parameter
 * @retval #WIFI_MANAGER_ERROR_INVALID_OPERATION    Invalid operation
 * @retval #WIFI_MANAGER_ERROR_NOT_SUPPORTED        Not supported
 */
int wifi_manager_config_reset_configurations(wifi_manager_h wifi);

/**
 * @brief Enables or disables auto-scanning
 * @details If auto-scanning is disabled, then background scan and wps scan don't work.
 * By default, the auto-scanning is enabled automatically until disabling auto-scanning.
 * Don't forget to enable again when you finish your works with disabled auto-scanning.
 * @since_tizen 4.0
 *
 * @param[in] wifi           The Wi-Fi handle
 * @param[in] autoscan       true to enable and false to disable
 *
 * @return 0 on success, otherwise negative error value
 * @retval #WIFI_MANAGER_ERROR_NONE                  Successful
 * @retval #WIFI_MANAGER_ERROR_INVALID_OPERATION     Invalid operation
 * @retval #WIFI_MANAGER_ERROR_INVALID_PARAMETER     Invalid parameter
 * @retval #WIFI_MANAGER_ERROR_OPERATION_FAILED      Operation failed
 * @retval #WIFI_MANAGER_ERROR_PERMISSION_DENIED     Permission Denied
 * @retval #WIFI_MANAGER_ERROR_NOT_SUPPORTED         Not supported
 * @pre This API needs wifi_manager_initialize() before use.
 */
int wifi_manager_set_autoscan(wifi_manager_h wifi, bool autoscan);

/**
 * @brief Gets the autoscan state.
 * @since_tizen 4.0
 *
 * @param[in] wifi           The Wi-Fi handle
 * @param[out] autoscan      The autoscan state(enable or disable)
 *
 * @return 0 on success, otherwise negative error value
 * @retval #WIFI_MANAGER_ERROR_NONE                 Successful
 * @retval #WIFI_MANAGER_ERROR_INVALID_PARAMETER    Invalid parameter
 * @retval #WIFI_MANAGER_ERROR_OPERATION_FAILED     Operation failed
 * @retval #WIFI_MANAGER_ERROR_PERMISSION_DENIED    Permission Denied
 * @retval #WIFI_MANAGER_ERROR_NOT_SUPPORTED        Not supported
 */
int wifi_manager_get_autoscan(wifi_manager_h wifi, bool *autoscan);

/**
 * @brief Sets the mode of autoscan.
 * @since_tizen 4.0
 *
 * @param[in] wifi           The Wi-Fi handle
 * @param[in] mode           The auto scan mode
 *
 * @return 0 on success, otherwise negative error value
 * @retval #WIFI_MANAGER_ERROR_NONE                  Successful
 * @retval #WIFI_MANAGER_ERROR_INVALID_PARAMETER     Invalid parameter
 * @retval #WIFI_MANAGER_ERROR_INVALID_OPERATION     Invalid operation
 * @retval #WIFI_MANAGER_ERROR_PERMISSION_DENIED     Permission Denied
 * @retval #WIFI_MANAGER_ERROR_NOT_SUPPORTED         Not supported
 * @pre This API needs wifi_manager_initialize() before use.
 */
int wifi_manager_set_autoscan_mode(wifi_manager_h wifi,
		wifi_manager_autoscan_mode_e mode);

/**
 * @brief Gets the autoscan mode.
 * @since_tizen 4.0
 *
 * @param[in] wifi           The Wi-Fi handle
 * @param[out] mode          The mode of autoscan
 *
 * @return 0 on success, otherwise negative error value
 * @retval #WIFI_MANAGER_ERROR_NONE                 Successful
 * @retval #WIFI_MANAGER_ERROR_INVALID_PARAMETER    Invalid parameter
 * @retval #WIFI_MANAGER_ERROR_OPERATION_FAILED     Operation failed
 * @retval #WIFI_MANAGER_ERROR_PERMISSION_DENIED    Permission Denied
 * @retval #WIFI_MANAGER_ERROR_NOT_SUPPORTED        Not supported
 */
int wifi_manager_get_autoscan_mode(wifi_manager_h wifi,
		wifi_manager_autoscan_mode_e *mode);

/**
 * @brief Sets the interval of autoscan.
 * @since_tizen 7.5
 * @privlevel public
 * @privilege %http://tizen.org/privilege/network.set
 *
 * @param[in] wifi           The Wi-Fi handle
 * @param[in] mode           The auto scan mode
 * @param[in] interval       The interval of autoscan
 *
 * @return 0 on success, otherwise negative error value
 * @retval #WIFI_MANAGER_ERROR_NONE                  Successful
 * @retval #WIFI_MANAGER_ERROR_NOT_INITIALIZED       Not initialized
 * @retval #WIFI_MANAGER_ERROR_INVALID_PARAMETER     Invalid parameter
 * @retval #WIFI_MANAGER_ERROR_INVALID_OPERATION     Invalid operation
 * @retval #WIFI_MANAGER_ERROR_PERMISSION_DENIED     Permission Denied
 * @retval #WIFI_MANAGER_ERROR_NOT_SUPPORTED         Not supported
 * @pre This API needs wifi_manager_initialize() before use.
 */
int wifi_manager_set_autoscan_interval(wifi_manager_h wifi,
		wifi_manager_autoscan_mode_e mode, int interval);

/**
 * @brief Gets the autoscan interval.
 * @since_tizen 7.5
 * @privlevel public
 * @privilege %http://tizen.org/privilege/network.get
 *
 * @param[in] wifi           The Wi-Fi handle
 * @param[in] mode           The auto scan mode
 * @param[out] interval      The interval of autoscan
 *
 * @return 0 on success, otherwise negative error value
 * @retval #WIFI_MANAGER_ERROR_NONE                 Successful
 * @retval #WIFI_MANAGER_ERROR_NOT_INITIALIZED      Not initialized
 * @retval #WIFI_MANAGER_ERROR_INVALID_PARAMETER    Invalid parameter
 * @retval #WIFI_MANAGER_ERROR_OPERATION_FAILED     Operation failed
 * @retval #WIFI_MANAGER_ERROR_PERMISSION_DENIED    Permission Denied
 * @retval #WIFI_MANAGER_ERROR_NOT_SUPPORTED        Not supported
 */
int wifi_manager_get_autoscan_interval(wifi_manager_h wifi,
		wifi_manager_autoscan_mode_e mode, int *interval);

/**
* @brief Set BSSID to connman to be connected
* @since_tizen 5.0
* @param[in] wifi               The Wi-Fi handle
* @param[in] bssid              The BSSID to be connected
*
* @return 0 on success, otherwise negative error value.
* @retval #WIFI_MANAGER_ERROR_NONE  Successful
* @retval #WIFI_MANAGER_ERROR_INVALID_PARAMETER    Invalid parameter
* @retval #WIFI_MANAGER_ERROR_OPERATION_FAILED     Operation failed
* @retval #WIFI_MANAGER_ERROR_NOT_SUPPORTED        Not supported
*/
int wifi_manager_set_bssid(wifi_manager_h wifi, char *bssid);

/**
 * @brief Sets auto connection mode(enable or disable) for all Wi-Fi profile.
 * @since_tizen 4.0
 *
 * @param[in] wifi           The Wi-Fi handle
 * @param[in] connect_mode   The mode of auto connection
 *
 * @return 0 on success, otherwise negative error value.
 * @retval #WIFI_MANAGER_ERROR_NONE  Successful
 * @retval #WIFI_MANAGER_ERROR_INVALID_PARAMETER    Invalid parameter
 * @retval #WIFI_MANAGER_ERROR_INVALID_OPERATION    Invalid operation
 * @retval #WIFI_MANAGER_ERROR_NOT_SUPPORTED        Not supported
 */
int wifi_manager_set_auto_connect(wifi_manager_h wifi, int connect_mode);

/**
 * @brief Gets auto connection mode(enable or disable) for all Wi-Fi profile.
 * @since_tizen 4.0
 *
 * @param[in] wifi           The Wi-Fi handle
 * @param[out] connect_mode  The mode of auto connection
 *
 * @return 0 on success, otherwise negative error value.
 * @retval #WIFI_MANAGER_ERROR_NONE  Successful
 * @retval #WIFI_MANAGER_ERROR_INVALID_PARAMETER    Invalid parameter
 * @retval #WIFI_MANAGER_ERROR_INVALID_OPERATION    Invalid operation
 * @retval #WIFI_MANAGER_ERROR_NOT_SUPPORTED        Not supported
 */
int wifi_manager_get_auto_connect(wifi_manager_h wifi, int *connect_mode);

/**
 * @brief Enables or disables passpoint.
 * @since_tizen 5.0
 *
 * @param[in] wifi           The Wi-Fi handle
 * @param[in] passpoint      true to enable and false to disable
 *
 * @return 0 on success, otherwise negative error value.
 * @retval #WIFI_MANAGER_ERROR_NONE  Successful
 * @retval #WIFI_MANAGER_ERROR_INVALID_PARAMETER    Invalid parameter
 * @retval #WIFI_MANAGER_ERROR_INVALID_OPERATION    Invalid operation
 * @retval #WIFI_MANAGER_ERROR_NOT_SUPPORTED        Not supported
 */
int wifi_manager_set_passpoint(wifi_manager_h wifi, int passpoint);

/**
 * @brief Gets the passpoint state.
 * @since_tizen 5.0
 *
 * @param[in] wifi           The Wi-Fi handle
 * @param[out] passpoint     The passpoint state(enable or disable)
 *
 * @return 0 on success, otherwise negative error value.
 * @retval #WIFI_MANAGER_ERROR_NONE  Successful
 * @retval #WIFI_MANAGER_ERROR_INVALID_PARAMETER    Invalid parameter
 * @retval #WIFI_MANAGER_ERROR_INVALID_OPERATION    Invalid operation
 * @retval #WIFI_MANAGER_ERROR_NOT_SUPPORTED        Not supported
 */
int wifi_manager_get_passpoint(wifi_manager_h wifi, int *passpoint);

/**
 * @brief Sets enables or disables auto-connection of the AP.
 * @details If an AP is connected, then connection information will be stored.
 * By default, the stored AP can be connected automatically when it found, until disabling auto-connection.
 * @since_tizen 5.0
 *
 * @param[in] ap             The access point handle
 * @param[in] autoconnect    true when enabled and false when disabled
 *
 * @return 0 on success, otherwise negative error value
 * @retval #WIFI_MANAGER_ERROR_NONE                 Successful
 * @retval #WIFI_MANAGER_ERROR_INVALID_OPERATION    Invalid operation
 * @retval #WIFI_MANAGER_ERROR_INVALID_PARAMETER    Invalid parameter
 * @retval #WIFI_MANAGER_ERROR_OPERATION_FAILED     Operation failed
 * @retval #WIFI_MANAGER_ERROR_PERMISSION_DENIED    Permission Denied
 * @retval #WIFI_MANAGER_ERROR_NOT_SUPPORTED        Not supported
 * @pre This API needs wifi_manager_initialize() and wifi_manager_activate() before use.
 */
int wifi_manager_ap_set_auto_connect(wifi_manager_ap_h ap, bool autoconnect);

/**
 * @brief Gets enables or disables auto-connection of the AP.
 * @details If an AP is connected, then connection information will be stored.
 * By default, the stored AP can be connected automatically when it found, until disabling auto-connection.
 * @since_tizen 5.0
 *
 * @param[in] ap             The access point handle
 * @param[out] autoconnect   true when enabled and false when disabled
 *
 * @return 0 on success, otherwise negative error value
 * @retval #WIFI_MANAGER_ERROR_NONE                 Successful
 * @retval #WIFI_MANAGER_ERROR_INVALID_OPERATION    Invalid operation
 * @retval #WIFI_MANAGER_ERROR_INVALID_PARAMETER    Invalid parameter
 * @retval #WIFI_MANAGER_ERROR_OPERATION_FAILED     Operation failed
 * @retval #WIFI_MANAGER_ERROR_PERMISSION_DENIED    Permission Denied
 * @retval #WIFI_MANAGER_ERROR_NOT_SUPPORTED        Not supported
 * @pre This API needs wifi_manager_initialize() and wifi_manager_activate() before use.
 */
int wifi_manager_ap_get_auto_connect(wifi_manager_ap_h ap, bool *autoconnect);


/**
 * @brief Changes the IP conflict detection period in seconds.
 * @since_tizen 5.0
 *
 * @param[in] wifi           The Wi-Fi handle
 * @param[in] initial_time   The period time in seconds of the ARP detect ping
 *
 * @return @c 0 on success, otherwise negative error value
 * @retval #WIFI_MANAGER_ERROR_NONE                 Successful
 * @retval #WIFI_MANAGER_ERROR_INVALID_PARAMETER    Invalid parameter
 * @retval #WIFI_MANAGER_ERROR_INVALID_OPERATION    Invalid operation
 * @retval #WIFI_MANAGER_ERROR_PERMISSION_DENIED    Permission denied
 * @retval #WIFI_MANAGER_ERROR_NOT_SUPPORTED        Not supported
 */
int wifi_manager_set_ip_conflict_period(wifi_manager_h wifi, unsigned int initial_time);

/**
 * @brief Gets the IP conflict detection period in seconds.
 * @since_tizen 5.0
 *
 * @param[in] wifi            The Wi-Fi handle
 * @param[out] initial_time   The period time in seconds of the ARP detect ping
 *
 * @return 0 on success, otherwise negative error value
 * @retval #WIFI_MANAGER_ERROR_NONE                 Successful
 * @retval #WIFI_MANAGER_ERROR_INVALID_PARAMETER    Invalid parameter
 * @retval #WIFI_MANAGER_ERROR_OPERATION_FAILED     Operation failed
 * @retval #WIFI_MANAGER_ERROR_NOT_SUPPORTED        Not supported
 */
int wifi_manager_get_ip_conflict_period(wifi_manager_h wifi, unsigned int *initial_time);

/**
 * @brief Called when the Netlink scan is finished.
 * @since_tizen 5.0
 * @param[in] error_code    The error code
 * @param[in] user_data     The user data passed from the callback registration function
 * @see wifi_manager_netlink_scan()
 */
typedef void(*wifi_manager_netlink_scan_finished_cb)(wifi_manager_error_e error_code, void *user_data);

/**
 * @brief Gets the result of the netlink scan (i.e.BSSID, ESSID, Frequency, RSSI, VSIE).
 * @details If tethering is enabled in device then wlan interface is available but
 * wifi module is in deactivated state and wpa-supplicant service is not active. So
 * in this case to search available APs wifi_manager_netlink_scan() API will be used.
 * @since_tizen 5.0
 * @privlevel public
 * @privilege %http://tizen.org/privilege/network.get
 * @param[in] wifi            The Wi-Fi handle
 * @param[in] callback        The callback to be called
 * @param[in] user_data       The user data passed to the callback function
 * @return 0 on success, otherwise negative error value
 * @retval #WIFI_MANAGER_ERROR_NONE                 Successful
 * @retval #WIFI_MANAGER_ERROR_INVALID_PARAMETER    Invalid parameter
 * @retval #WIFI_MANAGER_ERROR_OPERATION_FAILED     Operation failed
 * @retval #WIFI_MANAGER_ERROR_NOT_SUPPORTED        Not supported
 * @post This function invokes wifi_manager_found_ap_cb().
 * @see wifi_manager_netlink_scan()
 */
int wifi_manager_foreach_found_netlink_scan_ap(wifi_manager_h wifi,
				wifi_manager_found_ap_cb callback, void *user_data);

/**
 * @brief Starts netlink scan asynchronously.
 * @details If tethering is enabled in device then wlan interface is available but
 * wifi module is in deactivated state and wpa-supplicant service is not active. So
 * in this case to search available APs use wifi_manager_netlink_scan() API.
 * @since_tizen 5.0
 * @privlevel public
 * @privilege %http://tizen.org/privilege/network.set \n
 *            %http://tizen.org/privilege/network.get
 * @remarks This function needs both privileges.
 * @param[in] wifi            The Wi-Fi handle
 * @param[in] netlink_scan	The Wi-Fi netlink scan handle
 * @param[in] callback        The callback function to be called
 * @param[in] user_data       The user data passed to the callback function
 * @return 0 on success, otherwise negative error value
 * @retval #WIFI_MANAGER_ERROR_NONE                 Successful
 * @retval #WIFI_MANAGER_ERROR_INVALID_PARAMETER    Invalid parameter
 * @retval #WIFI_MANAGER_ERROR_INVALID_OPERATION    Invalid operation
 * @retval #WIFI_MANAGER_ERROR_OPERATION_FAILED     Operation failed
 * @retval #WIFI_MANAGER_ERROR_PERMISSION_DENIED    Permission Denied
 * @retval #WIFI_MANAGER_ERROR_NOT_SUPPORTED        Not supported
 * @post This function invokes wifi_manager_netlink_scan_finished_cb().
 */
int wifi_manager_netlink_scan(wifi_manager_h wifi,
		wifi_manager_netlink_scan_h netlink_scan,
		wifi_manager_netlink_scan_finished_cb callback, void *user_data);

/**
 * @brief Creates a Wi-Fi netlink scan handle.
 * @since_tizen 5.0
 * @remarks You must release @a netlink_scan using wifi_manager_netlink_scan_destroy().
 * @param[in] wifi		The Wi-Fi handle
 * @param[out] netlink_scan	The Wi-Fi netlink scan handle
 *
 * @return 0 on success, otherwise negative error value
 * @retval #WIFI_MANAGER_ERROR_NONE                 Successful
 * @retval #WIFI_MANAGER_ERROR_INVALID_PARAMETER    Invalid parameter
 * @retval #WIFI_MANAGER_ERROR_OUT_OF_MEMORY        Out of memory
 * @retval #WIFI_MANAGER_ERROR_NOT_SUPPORTED        Not supported
 * @pre This function needs a wifi_manager_initialize() call before use.
 * @see wifi_manager_netlink_scan_create()
 */
int wifi_manager_netlink_scan_create(wifi_manager_h wifi,
		wifi_manager_netlink_scan_h *netlink_scan);
/**
 * @brief Destroys a Wi-Fi netlink scan handle.
 * @since_tizen 5.0
 * @param[in] wifi		The Wi-Fi handle
 * @param[in] netlink_scan	The Wi-Fi netlink scan handle
 *
 * @return 0 on success, otherwise negative error value
 * @retval #WIFI_MANAGER_ERROR_NONE                 Successful
 * @retval #WIFI_MANAGER_ERROR_INVALID_PARAMETER    Invalid parameter
 * @retval #WIFI_MANAGER_ERROR_NOT_SUPPORTED        Not supported
 * @pre This function needs a wifi_manager_initialize() call before use.
 * @see wifi_manager_netlink_scan_destroy()
 */
int wifi_manager_netlink_scan_destroy(wifi_manager_h wifi,
		wifi_manager_netlink_scan_h netlink_scan);
/**
 * @brief Sets the SSID of netlink specific AP scan.
 * @since_tizen 5.0
 * @param[in] netlink_scan	The Wi-Fi netlink scan handle
 * @param[in] essid		The SSID of specific AP scan
 *
 * @return 0 on success, otherwise negative error value
 * @retval #WIFI_MANAGER_ERROR_NONE                 Successful
 * @retval #WIFI_MANAGER_ERROR_INVALID_PARAMETER    Invalid parameter
 * @retval #WIFI_MANAGER_ERROR_NOT_SUPPORTED        Not supported
 * @see wifi_manager_netlink_scan_set_ssid()
 */
int wifi_manager_netlink_scan_set_ssid(wifi_manager_netlink_scan_h netlink_scan,
		const char *essid);
/**
 * @brief Sets the VSIE for netlink scan.
 * @since_tizen 5.0
 * @param[in] netlink_scan	The Wi-Fi netlink scan handle
 * @param[in] vsie		The vendor specific information
 *
 * @return 0 on success, otherwise negative error value
 * @retval #WIFI_MANAGER_ERROR_NONE                 Successful
 * @retval #WIFI_MANAGER_ERROR_INVALID_PARAMETER    Invalid parameter
 * @retval #WIFI_MANAGER_ERROR_NOT_SUPPORTED        Not supported
 * @see wifi_manager_netlink_scan_set_vsie()
 */
int wifi_manager_netlink_scan_set_vsie(wifi_manager_netlink_scan_h netlink_scan,
		const char *vsie);

/**
 * @brief Initializes Wi-Fi in C# API.
 * @since_tizen 5.0
 * @privlevel public
 * @privilege %http://tizen.org/privilege/network.get
 * @remarks You must release @a wifi using wifi_manager_deinitialize_cs().
 * @param[in]  tid         TID in C#
 * @param[out] wifi        The Wi-Fi handle
 * @return @c 0 on success, otherwise negative error value
 * @retval #WIFI_MANAGER_ERROR_NONE                 Successful
 * @retval #WIFI_MANAGER_ERROR_ALREADY_INITIALIZED  Already initialized
 * @retval #WIFI_MANAGER_ERROR_INVALID_PARAMETER    Invalid parameter
 * @retval #WIFI_MANAGER_ERROR_INVALID_OPERATION    Invalid operation
 * @retval #WIFI_MANAGER_ERROR_OPERATION_FAILED     Operation failed
 * @retval #WIFI_MANAGER_ERROR_PERMISSION_DENIED    Permission Denied
 * @retval #WIFI_MANAGER_ERROR_OUT_OF_MEMORY        Out of memory
 * @retval #WIFI_MANAGER_ERROR_NOT_SUPPORTED        Not supported
 */
int wifi_manager_initialize_cs(int tid, wifi_manager_h *wifi);

/**
 * @brief Deinitializes Wi-Fi in C# API.
 * @since_tizen 5.0
 * @param[in] tid         TID in C#
 * @param[in] wifi        The Wi-Fi handle
 * @return 0 on success, otherwise negative error value
 * @retval #WIFI_MANAGER_ERROR_NONE               Successful
 * @retval #WIFI_MANAGER_ERROR_INVALID_PARAMETER  Invalid parameter
 * @retval #WIFI_MANAGER_ERROR_INVALID_OPERATION  Invalid operation
 * @retval #WIFI_MANAGER_ERROR_OPERATION_FAILED   Operation failed
 * @retval #WIFI_MANAGER_ERROR_NOT_SUPPORTED      Not supported
 */
int wifi_manager_deinitialize_cs(int tid, wifi_manager_h wifi);

/**
 * @brief Gets IEEE 802.11 Connection mode of connected network.
 * @since_tizen 5.0
 * @param[in] wifi     Wi-Fi handle
 * @param[out] mode    connection mode
 * @return 0 on success, otherwise negative error value
 * @retval #WIFI_MANAGER_ERROR_NONE                 Successful
 * @retval #WIFI_MANAGER_ERROR_NO_CONNECTION        Not Connected
 * @retval #WIFI_MANAGER_ERROR_NOT_INITIALIZED      Not initialized
 * @retval #WIFI_MANAGER_ERROR_INVALID_PARAMETER    Invalid parameter
 * @retval #WIFI_MANAGER_ERROR_NOT_SUPPORTED        Not supported
 */
int wifi_manager_get_connection_mode(wifi_manager_h wifi,
		wifi_manager_connection_mode_e *mode);

/**
 * @brief Gets the IEEE 802.11 connection mode supported by AP.
 * @since_tizen 6.0
 * @param[in] ap The access point handle
 * @param[out] mode The Wi-Fi connection mode.
 * @return 0 on success, otherwise negative error value
 * @retval #WIFI_MANAGER_ERROR_NONE                 Successful
 * @retval #WIFI_MANAGER_ERROR_INVALID_PARAMETER    Invalid parameter
 * @retval #WIFI_MANAGER_ERROR_OPERATION_FAILED		Operation Failed
 * @retval #WIFI_MANAGER_ERROR_NOT_SUPPORTED        Not supported
 */
int wifi_manager_ap_get_connection_mode(wifi_manager_ap_h ap, wifi_manager_connection_mode_e *mode);

/**
 * @brief Sets MAC address policy for connection operations.
 * @since_tizen 6.5
 * @privlevel public
 * @privilege %http://tizen.org/privilege/network.set
 * @param[in] wifi The Wi-Fi handle.
 * @param[in] policy The mac address policy.
 * @return 0 on success, otherwise negative error value
 * @retval #WIFI_MANAGER_ERROR_NONE                 Successful
 * @retval #WIFI_MANAGER_ERROR_INVALID_PARAMETER    Invalid parameter
 * @retval #WIFI_MANAGER_ERROR_OPERATION_FAILED		Operation Failed
 * @retval #WIFI_MANAGER_ERROR_NOT_SUPPORTED        Not supported
 * @retval #WIFI_MANAGER_ERROR_PERMISSION_DENIED    Permission Denied
 */
int wifi_manager_set_mac_policy(wifi_manager_h wifi, wifi_manager_mac_policy_e policy);

/**
 * @brief Gets MAC address policy for connection operations.
 * @since_tizen 6.5
 * @privlevel public
 * @privilege %http://tizen.org/privilege/network.get
 * @param[in] wifi The Wi-Fi handle.
 * @param[out] policy The mac address policy.
 * @return 0 on success, otherwise negative error value
 * @retval #WIFI_MANAGER_ERROR_NONE                 Successful
 * @retval #WIFI_MANAGER_ERROR_INVALID_PARAMETER    Invalid parameter
 * @retval #WIFI_MANAGER_ERROR_OPERATION_FAILED		Operation Failed
 * @retval #WIFI_MANAGER_ERROR_NOT_SUPPORTED        Not supported
 * @retval #WIFI_MANAGER_ERROR_PERMISSION_DENIED    Permission Denied
 */
int wifi_manager_get_mac_policy(wifi_manager_h wifi, wifi_manager_mac_policy_e *policy);

/**
 * @brief Sets MAC address policy for pre-association operations (scanning, ANQP).
 * @since_tizen 6.5
 * @privlevel public
 * @privilege %http://tizen.org/privilege/network.set
 * @param[in] wifi The Wi-Fi handle.
 * @param[in] policy The mac address policy.
 * @return 0 on success, otherwise negative error value
 * @retval #WIFI_MANAGER_ERROR_NONE                 Successful
 * @retval #WIFI_MANAGER_ERROR_INVALID_PARAMETER    Invalid parameter
 * @retval #WIFI_MANAGER_ERROR_OPERATION_FAILED		Operation Failed
 * @retval #WIFI_MANAGER_ERROR_NOT_SUPPORTED        Not supported
 * @retval #WIFI_MANAGER_ERROR_PERMISSION_DENIED    Permission Denied
 */
int wifi_manager_set_preassoc_mac_policy(wifi_manager_h wifi, wifi_manager_mac_policy_e policy);

/**
 * @brief Gets MAC address policy for pre-association operations (scanning, ANQP).
 * @since_tizen 6.5
 * @privlevel public
 * @privilege %http://tizen.org/privilege/network.get
 * @param[in] wifi The Wi-Fi handle.
 * @param[out] policy The mac address policy.
 * @return 0 on success, otherwise negative error value
 * @retval #WIFI_MANAGER_ERROR_NONE                 Successful
 * @retval #WIFI_MANAGER_ERROR_INVALID_PARAMETER    Invalid parameter
 * @retval #WIFI_MANAGER_ERROR_OPERATION_FAILED		Operation Failed
 * @retval #WIFI_MANAGER_ERROR_NOT_SUPPORTED        Not supported
 * @retval #WIFI_MANAGER_ERROR_PERMISSION_DENIED    Permission Denied
 */
int wifi_manager_get_preassoc_mac_policy(wifi_manager_h wifi, wifi_manager_mac_policy_e *policy);

/**
 * @brief Sets lifetime of random mac_address after which new random mac is used.
 * @since_tizen 6.5
 * @privlevel public
 * @privilege %http://tizen.org/privilege/network.set
 * @param[in] wifi The Wi-Fi handle.
 * @param[in] lifetime The lifetime of random mac address.
 * @return 0 on success, otherwise negative error value
 * @retval #WIFI_MANAGER_ERROR_NONE                 Successful
 * @retval #WIFI_MANAGER_ERROR_INVALID_PARAMETER    Invalid parameter
 * @retval #WIFI_MANAGER_ERROR_OPERATION_FAILED		Operation Failed
 * @retval #WIFI_MANAGER_ERROR_NOT_SUPPORTED        Not supported
 * @retval #WIFI_MANAGER_ERROR_PERMISSION_DENIED    Permission Denied
 */
int wifi_manager_set_random_mac_lifetime(wifi_manager_h wifi, unsigned int lifetime);

/**
 * @brief Gets lifetime of random mac_address after which new random mac is used.
 * @since_tizen 6.5
 * @privlevel public
 * @privilege %http://tizen.org/privilege/network.get
 * @param[in] wifi The Wi-Fi handle.
 * @param[out] lifetime The lifetime of random mac address.
 * @return 0 on success, otherwise negative error value
 * @retval #WIFI_MANAGER_ERROR_NONE                 Successful
 * @retval #WIFI_MANAGER_ERROR_INVALID_PARAMETER    Invalid parameter
 * @retval #WIFI_MANAGER_ERROR_OPERATION_FAILED		Operation Failed
 * @retval #WIFI_MANAGER_ERROR_NOT_SUPPORTED        Not supported
 * @retval #WIFI_MANAGER_ERROR_PERMISSION_DENIED    Permission Denied
 */
int wifi_manager_get_random_mac_lifetime(wifi_manager_h wifi, unsigned int *lifetime);

/**
 * @brief Sets the raw country code.
 * @since_tizen 6.5
 * @privlevel public
 * @privilege %http://tizen.org/privilege/network.set
 * @param[in] wifi The Wi-Fi handle.
 * @param[in] country The ISO/IEC alpha2 country code.
 * @return 0 on success, otherwise negative error value
 * @retval #WIFI_MANAGER_ERROR_NONE                 Successful
 * @retval #WIFI_MANAGER_ERROR_NOT_SUPPORTED        Not supported
 * @retval #WIFI_MANAGER_ERROR_NOT_INITIALIZED      Not initialized
 * @retval #WIFI_MANAGER_ERROR_INVALID_PARAMETER    Invalid parameter
 * @retval #WIFI_MANAGER_ERROR_OPERATION_FAILED     Operation Failed
 * @retval #WIFI_MANAGER_ERROR_PERMISSION_DENIED    Permission Denied
 */
int wifi_manager_set_country_code(wifi_manager_h wifi, const char *country);

/**
 * @brief Gets the raw country code.
 * @since_tizen 6.5
 * @privlevel public
 * @privilege %http://tizen.org/privilege/network.get
 * @remarks You must release @a country_code using free().
 * @param[in] wifi The Wi-Fi handle.
 * @param[out] country_code The ISO/IEC alpha2 country code.
 * @return 0 on success, otherwise negative error value
 * @retval #WIFI_MANAGER_ERROR_NONE                 Successful
 * @retval #WIFI_MANAGER_ERROR_NOT_SUPPORTED        Not supported
 * @retval #WIFI_MANAGER_ERROR_NOT_INITIALIZED      Not initialized
 * @retval #WIFI_MANAGER_ERROR_INVALID_PARAMETER    Invalid parameter
 * @retval #WIFI_MANAGER_ERROR_OPERATION_FAILED     Operation Failed
 * @retval #WIFI_MANAGER_ERROR_PERMISSION_DENIED    Permission Denied
 */
int wifi_manager_get_country_code(wifi_manager_h wifi, char **country_code);

/**
 * @brief Gets the Wi-Fi Vendor Specific Information Elements (VSIE) from DHCP ack packet of the connected network.
 * @since_tizen 9.0
 * @privlevel public
 * @privilege %http://tizen.org/privilege/network.get
 * @remarks @a vsie_str must be released with free().
 *
 * @param[in]  wifi          The Wi-Fi handle
 * @param[out] vsie_str      The DHCP VSIE data. If the connection is established but there is no VSIE data, this parameter will be set to NULL.
 *
 * @return 0 on success, otherwise negative error value
 * @retval #WIFI_MANAGER_ERROR_NONE                 Successful
 * @retval #WIFI_MANAGER_ERROR_INVALID_PARAMETER    Invalid parameter
 * @retval #WIFI_MANAGER_ERROR_NOT_SUPPORTED        Not supported
 * @retval #WIFI_MANAGER_ERROR_NO_CONNECTION        Not Connected
 * @retval #WIFI_MANAGER_ERROR_OPERATION_FAILED     Operation Failed
 * @retval #WIFI_MANAGER_ERROR_PERMISSION_DENIED    Permission Denied
 */
int wifi_manager_get_dhcp_vsie(wifi_manager_h wifi, char **vsie_str);

/**
 * @brief Sets the Wi-Fi band to scan.
 * @since_tizen 9.0
 *
 * @param[in] wifi           The Wi-Fi handle
 * @param[in] scan_band      The Wi-Fi band to scan
 *
 * @return 0 on success, otherwise negative error value
 * @retval #WIFI_MANAGER_ERROR_NONE                  Successful
 * @retval #WIFI_MANAGER_ERROR_NOT_SUPPORTED         Not supported
 * @retval #WIFI_MANAGER_ERROR_PERMISSION_DENIED     Permission Denied
 * @retval #WIFI_MANAGER_ERROR_INVALID_PARAMETER     Invalid parameter
 * @retval #WIFI_MANAGER_ERROR_NOT_INITIALIZED       Not initialized
 * @retval #WIFI_MANAGER_ERROR_OPERATION_FAILED      Operation failed
 * @pre This API needs wifi_manager_initialize() before use.
 */
int wifi_manager_set_scan_band(wifi_manager_h wifi, wifi_manager_band_selection_e scan_band);

/**
 * @brief Gets the Wi-Fi band to scan.
 * @since_tizen 9.0
 *
 * @param[in] wifi           The Wi-Fi handle
 * @param[out] scan_band     The Wi-Fi band to scan
 *
 * @return 0 on success, otherwise negative error value
 * @retval #WIFI_MANAGER_ERROR_NONE                  Successful
 * @retval #WIFI_MANAGER_ERROR_NOT_SUPPORTED         Not supported
 * @retval #WIFI_MANAGER_ERROR_PERMISSION_DENIED     Permission Denied
 * @retval #WIFI_MANAGER_ERROR_INVALID_PARAMETER     Invalid parameter
 * @retval #WIFI_MANAGER_ERROR_NOT_INITIALIZED       Not initialized
 * @retval #WIFI_MANAGER_ERROR_OPERATION_FAILED      Operation failed
 */
int wifi_manager_get_scan_band(wifi_manager_h wifi, wifi_manager_band_selection_e *scan_band);

/**
 * @brief Called when the Roaming state is changed.
 * @since_tizen 7.0
 * @param[in] state         The Wi-Fi roaming state
 * @param[in] cur_bssid     The MAC address of connected AP
 * @param[in] dst_bssid     The MAC address of AP to roam
 * @param[in] user_data     The user data passed from the callback registration function
 * @see wifi_manager_set_roaming_cb()
 * @see wifi_manager_unset_roaming_cb()
 */
typedef void(*wifi_manager_roaming_state_changed_cb)(wifi_manager_roam_e state,
							char *cur_bssid, char *dst_bssid, void *user_data);

/**
 * @brief Registers the callback called when the Roaming state is changed.
 * @since_tizen 7.0
 * @param[in] wifi            The Wi-Fi handle
 * @param[in] callback        The callback function to be called
 * @param[in] user_data       The user data passed to the callback function
 * @return 0 on success, otherwise negative error value
 * @retval #WIFI_MANAGER_ERROR_NONE                 Successful
 * @retval #WIFI_MANAGER_ERROR_NOT_INITIALIZED      Not initialized
 * @retval #WIFI_MANAGER_ERROR_INVALID_OPERATION    Invalid operation
 * @retval #WIFI_MANAGER_ERROR_INVALID_PARAMETER    Invalid parameter
 * @retval #WIFI_MANAGER_ERROR_NOT_SUPPORTED        Not supported
 */
int wifi_manager_set_roaming_cb(wifi_manager_h wifi,
		wifi_manager_roaming_state_changed_cb callback, void *user_data);

/**
 * @brief Unregisters the callback called when the Roaming state is changed.
 * @since_tizen 7.0
 * @param[in] wifi            The Wi-Fi handle
 * @return 0 on success, otherwise negative error value
 * @retval #WIFI_MANAGER_ERROR_NONE                 Successful
 * @retval #WIFI_MANAGER_ERROR_NOT_INITIALIZED      Not initialized
 * @retval #WIFI_MANAGER_ERROR_INVALID_OPERATION    Invalid operation
 * @retval #WIFI_MANAGER_ERROR_INVALID_PARAMETER    Invalid parameter
 * @retval #WIFI_MANAGER_ERROR_NOT_SUPPORTED        Not supported
 */
int wifi_manager_unset_roaming_cb(wifi_manager_h wifi);

/**
 * @brief Called when the DHCP state is changed.
 * @details The following error codes can be received: \n
 *              #WIFI_MANAGER_ERROR_NONE                 Successful \n
 *              #WIFI_MANAGER_ERROR_DHCP_FAILED          DHCP failed \n
 * @since_tizen 7.5
 * @param[in] state         The DHCP state
 * @param[in] error_code    The error code
 * @param[in] user_data     The user data passed from the callback registration function
 * @see wifi_manager_set_dhcp_state_changed_cb()
 * @see wifi_manager_unset_dhcp_state_changed_cb()
 */
typedef void(*wifi_manager_dhcp_event_cb)(wifi_manager_dhcp_state_e state,
							wifi_manager_error_e error_code, void *user_data);

/**
 * @brief set DHCP event callback
 * @since_tizen 7.5
 * @privlevel public
 * @privilege %http://tizen.org/privilege/network.set
 *
 * @param[in] wifi            The Wi-Fi handle
 * @param[in] callback        The callback function to be called
 * @param[in] user_data       The user data passed to the callback function
 *
 * @return 0 on success, otherwise negative error value
 * @retval #WIFI_MANAGER_ERROR_NONE                  Successful
 * @retval #WIFI_MANAGER_ERROR_INVALID_OPERATION     Invalid operation
 * @retval #WIFI_MANAGER_ERROR_INVALID_PARAMETER     Invalid parameter
 * @retval #WIFI_MANAGER_ERROR_OPERATION_FAILED      Operation failed
 * @retval #WIFI_MANAGER_ERROR_PERMISSION_DENIED     Permission Denied
 * @retval #WIFI_MANAGER_ERROR_NOT_SUPPORTED         Not supported
 */
int wifi_manager_set_dhcp_state_changed_cb(wifi_manager_h wifi, wifi_manager_dhcp_event_cb callback, void *user_data);

/**
 * @brief unset DHCP event callback
 * @since_tizen 7.5
 * @privlevel public
 * @privilege %http://tizen.org/privilege/network.set
 *
 * @param[in] wifi            The Wi-Fi handle
 *
 * @return 0 on success, otherwise negative error value
 * @retval #WIFI_MANAGER_ERROR_NONE                  Successful
 * @retval #WIFI_MANAGER_ERROR_INVALID_OPERATION     Invalid operation
 * @retval #WIFI_MANAGER_ERROR_INVALID_PARAMETER     Invalid parameter
 * @retval #WIFI_MANAGER_ERROR_OPERATION_FAILED      Operation failed
 * @retval #WIFI_MANAGER_ERROR_PERMISSION_DENIED     Permission Denied
 * @retval #WIFI_MANAGER_ERROR_NOT_SUPPORTED         Not supported
 */
int wifi_manager_unset_dhcp_state_changed_cb(wifi_manager_h wifi);

/**
 * @brief Gets power-save state.
 * @since_tizen 7.5
 * @privlevel public
 * @privilege %http://tizen.org/privilege/network.get
 *
 * @param[in]  wifi          The Wi-Fi handle
 * @param[out] ps_state      @c WIFI_MANAGER_PS_OFF if power-save state is disabled
 *                           @c WIFI_MANAGER_PS_ON  if power-save state is enabled
 *
 * @return 0 on success, otherwise negative error value
 * @retval #WIFI_MANAGER_ERROR_NONE                 Successful
 * @retval #WIFI_MANAGER_ERROR_INVALID_OPERATION    Invalid operation
 * @retval #WIFI_MANAGER_ERROR_INVALID_PARAMETER    Invalid parameter
 * @retval #WIFI_MANAGER_ERROR_OPERATION_FAILED     Operation failed
 * @retval #WIFI_MANAGER_ERROR_PERMISSION_DENIED    Permission Denied
 * @retval #WIFI_MANAGER_ERROR_NOT_SUPPORTED        Not supported
 */
int wifi_manager_get_power_save_state(wifi_manager_h wifi, wifi_manager_power_save_state_e *ps_state);

/**
 * @brief Sets power-save state.
 * @since_tizen 7.5
 * @privlevel public
 * @privilege %http://tizen.org/privilege/network.set
 *
 * @param[in]  wifi          The Wi-Fi handle
 * @param[in]  ps_state      @c WIFI_MANAGER_PS_OFF to disable power-save
 *                           @c WIFI_MANAGER_PS_ON  to enable power-save
 *
 * @return 0 on success, otherwise negative error value
 * @retval #WIFI_MANAGER_ERROR_NONE                 Successful
 * @retval #WIFI_MANAGER_ERROR_INVALID_OPERATION    Invalid operation
 * @retval #WIFI_MANAGER_ERROR_INVALID_PARAMETER    Invalid parameter
 * @retval #WIFI_MANAGER_ERROR_OPERATION_FAILED     Operation failed
 * @retval #WIFI_MANAGER_ERROR_PERMISSION_DENIED    Permission Denied
 * @retval #WIFI_MANAGER_ERROR_NOT_SUPPORTED        Not supported
 */
int wifi_manager_set_power_save_state(wifi_manager_h wifi, wifi_manager_power_save_state_e ps_state);

/**
 * @brief Gets power-save mode.
 * @since_tizen 7.5
 * @privlevel public
 * @privilege %http://tizen.org/privilege/network.get
 *
 * @param[in]  wifi          The Wi-Fi handle
 * @param[out] ps_mode       @c WIFI_MANAGER_PS_MODE_DEFAULT_OFF  if power-save mode is default-off
 *                           @c WIFI_MANAGER_PS_MODE_DEFAULT_ON   if power-save mode is default-on
 *                           @c WIFI_MANAGER_PS_MODE_DYNAMIC      if power-save mode is dynamic
 *
 * @return 0 on success, otherwise negative error value
 * @retval #WIFI_MANAGER_ERROR_NONE                 Successful
 * @retval #WIFI_MANAGER_ERROR_INVALID_OPERATION    Invalid operation
 * @retval #WIFI_MANAGER_ERROR_INVALID_PARAMETER    Invalid parameter
 * @retval #WIFI_MANAGER_ERROR_OPERATION_FAILED     Operation failed
 * @retval #WIFI_MANAGER_ERROR_PERMISSION_DENIED    Permission Denied
 * @retval #WIFI_MANAGER_ERROR_NOT_SUPPORTED        Not supported
 */
int wifi_manager_get_power_save_mode(wifi_manager_h wifi, wifi_manager_power_save_mode_e *ps_mode);

/**
 * @brief Sets power-save mode.
 * @since_tizen 7.5
 * @privlevel public
 * @privilege %http://tizen.org/privilege/network.set
 *
 * @param[in]  wifi          The Wi-Fi handle
 * @param[in]  ps_mode       @c WIFI_MANAGER_PS_MODE_DEFAULT_OFF   set power-save mode to default-off
 *                           @c WIFI_MANAGER_PS_MODE_DEFAULT_ON    set power-save mode to default-on
 *                           @c WIFI_MANAGER_PS_MODE_DYNAMIC       set power-save mode to dynamic
 *
 * @return 0 on success, otherwise negative error value
 * @retval #WIFI_MANAGER_ERROR_NONE                 Successful
 * @retval #WIFI_MANAGER_ERROR_INVALID_OPERATION    Invalid operation
 * @retval #WIFI_MANAGER_ERROR_INVALID_PARAMETER    Invalid parameter
 * @retval #WIFI_MANAGER_ERROR_OPERATION_FAILED     Operation failed
 * @retval #WIFI_MANAGER_ERROR_PERMISSION_DENIED    Permission Denied
 * @retval #WIFI_MANAGER_ERROR_NOT_SUPPORTED        Not supported
 */
int wifi_manager_set_power_save_mode(wifi_manager_h wifi, wifi_manager_power_save_mode_e ps_mode);

/**
 * @}
 */


/**
 * @}
 */

#ifdef __cplusplus
}
#endif

#endif /* __TIZEN_WIFI_MANAGER_EXTENSION_H__ */
