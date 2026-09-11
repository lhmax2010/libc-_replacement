/*
 * Copyright (c) 2018 Samsung Electronics Co., Ltd. All rights reserved.
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

#ifndef __TIZEN_NET_INM_H__
#define __TIZEN_NET_INM_H__

#include <tizen.h>

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @file inm.h
 */

/**
 * @addtogroup CAPI_NETWORK_INM_MODULE
 * @{
 */


/**
 * @brief The inm handle.
 * @since_tizen 5.0
*/
typedef void* inm_h;

/**
* @brief The connection handle.
* @since_tizen 5.0
*/
typedef void* inm_connection_h;

/**
* @brief The connections iterator handle.
* @since_tizen 5.0
*/
typedef void* inm_connection_iterator_h;

/**
* @brief The link handle.
* @since_tizen 5.5
*/
typedef void* inm_link_h;

/**
* @brief The link address handle.
* @since_tizen 5.5
*/
typedef void* inm_link_address_h;

/**
* @brief The link route table handle.
* @since_tizen 5.5
*/
typedef void* inm_link_route_h;

/**
 * @brief Enumeration for Intelligent Network Monitoring (INM) error code.
 * @since_tizen 5.0
 */
typedef enum {
	/**
	 * Successful
	 */
	INM_ERROR_NONE                  = TIZEN_ERROR_NONE,
	/**
	 * Operation not permitted(1)
	 */
	INM_ERROR_NOT_PERMITTED         = TIZEN_ERROR_NOT_PERMITTED,
	/**
	 * Out of memory(12)
	 */
	INM_ERROR_OUT_OF_MEMORY         = TIZEN_ERROR_OUT_OF_MEMORY,
	/**
	 * Permission denied(13)
	 */
	INM_ERROR_PERMISSION_DENIED     = TIZEN_ERROR_PERMISSION_DENIED,
	/**
	 * Device or resource busy(16)
	 */
	INM_ERROR_RESOURCE_BUSY         = TIZEN_ERROR_RESOURCE_BUSY,
	/**
	 * Invalid function parameter(22)
	 */
	INM_ERROR_INVALID_PARAMETER     = TIZEN_ERROR_INVALID_PARAMETER,
	/**
	 * Connection timed out(110)
	 */
	INM_ERROR_CONNECTION_TIME_OUT   = TIZEN_ERROR_CONNECTION_TIME_OUT,
	/**
	 * Now in progress(115)
	 */
	INM_ERROR_NOW_IN_PROGRESS       = TIZEN_ERROR_NOW_IN_PROGRESS,
	/**
	 * Not supported
	 */
	INM_ERROR_NOT_SUPPORTED         = TIZEN_ERROR_NOT_SUPPORTED,
	/**
	 * Not initialized
	 */
	INM_ERROR_NOT_INITIALIZED       = TIZEN_ERROR_INM|0x01,
	/**
	 * Already initialized
	 */
	INM_ERROR_ALREADY_INITIALIZED   = TIZEN_ERROR_INM|0x02,
	/**
	 * Operation failed
	 */
	INM_ERROR_OPERATION_FAILED      = TIZEN_ERROR_INM|0x03,
	/**
	 * Data not found
	 */
	INM_ERROR_DATA_NOT_FOUND        = TIZEN_ERROR_INM|0x04,
} inm_error_e;

/**
 * @}
 */


/**
 * @addtogroup CAPI_NETWORK_INM_SPECIFIC_MODULE
 * @{
 */

/**
 * @brief Enumeration for the Ethernet cable state.
 * @since_tizen 5.0
 */
typedef enum {
	INM_ETHERNET_CABLE_STATE_DETACHED = 0,  /**< Ethernet cable is detached */
	INM_ETHERNET_CABLE_STATE_ATTACHED = 1,  /**< Ethernet cable is attached */
} inm_ethernet_cable_state_e;


/**
 * @brief Enumeration for the Wi-Fi Module state.
 * @since_tizen 5.0
 */
typedef enum {
	INM_WIFI_MODULE_STATE_DETACHED = 0,  /**< Wi-Fi Module is detached */
	INM_WIFI_MODULE_STATE_ATTACHED = 1,  /**< Wi-Fi Module is attached */
} inm_wifi_module_state_e;

/**
 * @brief Enumeration for Gateway state.
 * @since_tizen 9.0
 */
typedef enum {
	INM_GATEWAY_STATE_INVALID = 0,  /**< Gateway Check Failed */
	INM_GATEWAY_STATE_VALID = 1,    /**< Gateway Check Succeeded */
} inm_gateway_state_e;

/**
 * @brief Enumeration for DNS state.
 * @since_tizen 9.0
 */
typedef enum {
	INM_DNS_STATE_INVALID = 0,  /**< DNS Check Failed */
	INM_DNS_STATE_VALID = 1,    /**< DNS Check Succeeded */
} inm_dns_state_e;

/**
 * @brief Enumeration for the state of the IP conflict.
 * @since_tizen 5.0
 */
typedef enum {
	INM_IP_CONFLICT_STATE_UNKNOWN = 0,                /**< Unknown state */
	INM_IP_CONFLICT_STATE_CONFLICT_NOT_DETECTED = 1,  /**< Resolved state */
	INM_IP_CONFLICT_STATE_CONFLICT_DETECTED = 2,      /**< Conflict state */
} inm_ip_conflict_state_e;

/**
 * @brief Enumeration for connection type.
 * @since_tizen 5.0
 */
typedef enum {
	INM_CONNECTION_TYPE_DISCONNECTED = 0,  /**< Disconnected */
	INM_CONNECTION_TYPE_WIFI = 1,          /**< Wi-Fi type */
	INM_CONNECTION_TYPE_CELLULAR = 2,      /**< Cellular type */
	INM_CONNECTION_TYPE_ETHERNET = 3,      /**< Ethernet type */
	INM_CONNECTION_TYPE_BT = 4,            /**< Bluetooth type */
	INM_CONNECTION_TYPE_NET_PROXY,         /**< Proxy type for internet connection */
} inm_connection_type_e;

/**
 * @brief Enumeration for statistics type.
 * @since_tizen 5.0
 */
typedef enum {
	INM_STATISTICS_TYPE_LAST_RECEIVED_DATA = 0,   /**< Last received data */
	INM_STATISTICS_TYPE_LAST_SENT_DATA = 1,       /**< Last sent data */
	INM_STATISTICS_TYPE_TOTAL_RECEIVED_DATA = 2,  /**< Total received data */
	INM_STATISTICS_TYPE_TOTAL_SENT_DATA = 3,      /**< Total sent data */
} inm_statistics_type_e;

/**
 * @brief Enumeration for congestion level.
 * @since_tizen 5.0
 */
typedef enum {
	INM_CONGESTION_LEVEL_VERY_HIGH = 0,  /**< Very high TCP congestion */
	INM_CONGESTION_LEVEL_HIGH = 1,       /**< High TCP congestion */
	INM_CONGESTION_LEVEL_MEDIUM = 2,     /**< Medium TCP congestion */
	INM_CONGESTION_LEVEL_LOW = 3,        /**< Low TCP congestion */
} inm_congestion_level_e;

/**
 * @}
 */


/**
 * @addtogroup CAPI_NETWORK_INM_GENERAL_MODULE
 * @{
 */

/**
 * @brief Enumeration for cellular network state.
 * @since_tizen 5.0
 */
typedef enum {
	INM_CELLULAR_STATE_DEACTIVATED,   /**< Cellular is deactivated */
	INM_CELLULAR_STATE_DISCONNECTED,  /**< Disconnected */
	INM_CELLULAR_STATE_CONNECTED,     /**< Connected */
} inm_cellular_state_e;

/**
 * @brief Enumeration for Wi-Fi state.
 * @since_tizen 5.0
 */
typedef enum {
	INM_WIFI_STATE_DEACTIVATED,   /**< Wi-Fi is deactivated */
	INM_WIFI_STATE_DISCONNECTED,  /**< Disconnected */
	INM_WIFI_STATE_CONNECTED,     /**< Connected */
} inm_wifi_state_e;

/**
 * @brief Enumeration for ethernet state.
 * @since_tizen 5.0
 */
typedef enum {
	INM_ETHERNET_STATE_DEACTIVATED,   /**< There is no Ethernet profile to open */
	INM_ETHERNET_STATE_DISCONNECTED,  /**< Disconnected */
	INM_ETHERNET_STATE_CONNECTED,     /**< Connected */
} inm_ethernet_state_e;

/**
 * @brief Enumeration for URL list type.
 * @since_tizen 5.5
 */
typedef enum {
	INM_URL_LIST_TYPE_DEFAULT,  /**< Default test URLs */
	INM_URL_LIST_TYPE_USER,     /**< User registered test URLs */
} inm_url_list_type_e;

/**
 * @brief Enumeration for result of checking URL callback.
 * @since_tizen 5.5
 */
typedef enum {
    INM_REACHABLE_URL_CHECK_RESULT_SUCCESS,               /**< URL accessed successfully */
    INM_REACHABLE_URL_CHECK_RESULT_ERROR_MALFORMED_URL,   /**< 'URL is malformed' error */
    INM_REACHABLE_URL_CHECK_RESULT_ERROR_DNS_RESOLVE,     /**< 'URL couldn't be resolved' error */
    INM_REACHABLE_URL_CHECK_RESULT_ERROR_TCP_CONNECT,     /**< 'TCP connection' error */
    INM_REACHABLE_URL_CHECK_RESULT_ERROR_SSL,             /**< SSL error */
    INM_REACHABLE_URL_CHECK_RESULT_ERROR_HTTP,            /**< HTTP error */
    INM_REACHABLE_URL_CHECK_RESULT_ERROR_FILE_NOT_FOUND,  /**< 'Remote file not found' error */
    INM_REACHABLE_URL_CHECK_RESULT_ERROR_UNKNOWN = 255,   /**< Unknown error */
} inm_reachable_urls_check_result_e;

/**
 * @}
 */


/**
 * @addtogroup CAPI_NETWORK_INM_CONNECTION_MODULE
 * @{
 */

/**
 * @brief Enumeration for connection state type.
 * @since_tizen 5.0
 */
typedef enum {
	INM_CONNECTION_STATE_DISCONNECTED,   /**< Disconnected state */
	INM_CONNECTION_STATE_ASSOCIATION,    /**< Association state */
	INM_CONNECTION_STATE_CONFIGURATION,  /**< Configuration state */
	INM_CONNECTION_STATE_CONNECTED,      /**< Connected state */
} inm_connection_state_e;

/**
 * @brief Enumeration for address family.
 * @since_tizen 5.0
*/
typedef enum {
	INM_ADDRESS_FAMILY_IPV4,  /**< IPV4 Address type */
	INM_ADDRESS_FAMILY_IPV6,  /**< IPV6 Address type */
} inm_address_family_e;

/**
 * @brief Enumeration for IP configuration type.
 * @since_tizen 5.0
 */
typedef enum {
	/**
	 * Not defined
	 */
	INM_IP_CONFIG_TYPE_NONE = 0,

	/**
	 * Manual IP configuration
	 */
	INM_IP_CONFIG_TYPE_STATIC  = 1,

	/**
	 * Config IP using DHCP client
	 */
	INM_IP_CONFIG_TYPE_DYNAMIC = 2,

	/**
	 * Config IP from Auto IP pool (169.254/16). Later with DHCP client, if available
	 */
	INM_IP_CONFIG_TYPE_AUTO = 3,

	/**
	 * Indicates an IP address that can not be modified
	 */
	INM_IP_CONFIG_TYPE_FIXED = 4,

} inm_ip_config_type_e;

/**
 * @brief Enumeration for proxy method type.
 * @since_tizen 5.0
 */
typedef enum {
	/**
	 * Direct connection
	 */
	INM_PROXY_TYPE_DIRECT = 0,

	/**
	 * Auto configuration (use PAC file). If URL property is not set,
	 * DHCP/WPAD auto-discover will be tried
	 */
	INM_PROXY_TYPE_AUTO = 1,

	/**
	 * Manual configuration
	 */
	INM_PROXY_TYPE_MANUAL  = 2,

} inm_proxy_type_e;

/**
 * @brief Enumeration for DNS configuration type.
 * @since_tizen 5.0
 */
typedef enum {
	INM_DNS_CONFIG_TYPE_NONE,     /**< Not defined */
	INM_DNS_CONFIG_TYPE_STATIC,   /**< Manual DNS configuration */
	INM_DNS_CONFIG_TYPE_DYNAMIC,  /**< Config DNS using DHCP client */
} inm_dns_config_type_e;

/**
 * @}
 */


/**
 * @addtogroup CAPI_NETWORK_INM_WIFI_MODULE
 * @{
 */

/**
 * @brief Enumeration for the wifi scanning state.
 * @since_tizen 5.0
 */
typedef enum {
	INM_WIFI_SCAN_STATE_NOT_SCANNING,  /**< Scan is not running */
	INM_WIFI_SCAN_STATE_SCANNING,      /**< Scan is in progress */
} inm_wifi_scan_state_e;

/**
 * @}
 */


/**
 * @addtogroup CAPI_NETWORK_INM_WIFI_AP_MODULE
 * @{
 */

/**
 * @brief Enumeration for the RSSI level.
 * @since_tizen 5.0
 */
typedef enum {
	INM_WIFI_RSSI_LEVEL_0,  /**< No signal */
	INM_WIFI_RSSI_LEVEL_1,  /**< Very weak signal ~ -83dBm */
	INM_WIFI_RSSI_LEVEL_2,  /**< Weak signal -82 ~ -75dBm */
	INM_WIFI_RSSI_LEVEL_3,  /**< Strong signal -74 ~ -64dBm */
	INM_WIFI_RSSI_LEVEL_4,  /**< Very strong signal -63dBm ~ */
} inm_wifi_rssi_level_e;

/**
 * @brief Enumeration for Wi-Fi security type.
 * @details The following security modes are used in infrastructure and ad-hoc mode.
 * For now all EAP security mechanisms are provided only in infrastructure mode.
 *
 * @since_tizen 5.0
 */
typedef enum {
	INM_WIFI_SECURITY_TYPE_NONE,      /**< Security disabled */
	INM_WIFI_SECURITY_TYPE_WEP,       /**< WEP */
	INM_WIFI_SECURITY_TYPE_WPA_PSK,   /**< WPA-PSK */
	INM_WIFI_SECURITY_TYPE_WPA2_PSK,  /**< WPA2-PSK */
	INM_WIFI_SECURITY_TYPE_EAP,       /**< EAP */
	INM_WIFI_SECURITY_TYPE_FT_PSK,    /**< FT-PSK */
} inm_wifi_security_type_e;

/**
 * @brief Enumeration for Wi-Fi encryption type.
 * @details The following encryption modes are used in infrastructure and ad-hoc mode.
 * @since_tizen 5.0
 */
typedef enum {
	INM_WIFI_ENCRYPTION_TYPE_NONE,            /**< Encryption disabled */
	INM_WIFI_ENCRYPTION_TYPE_WEP,             /**< WEP */
	INM_WIFI_ENCRYPTION_TYPE_TKIP,            /**< TKIP */
	INM_WIFI_ENCRYPTION_TYPE_AES,             /**< AES */
	INM_WIFI_ENCRYPTION_TYPE_TKIP_AES_MIXED,  /**< TKIP and AES are both supported */
} inm_wifi_encryption_type_e;

/**
 * @brief Enumeration for Wi-Fi disconnect reason, provided by the supplicant.
 * @since_tizen 5.0
 */
typedef enum {
	/**
	 * Locally Generate Disconnect from user side
	 */
	INM_WIFI_REASON_LOCAL_GENERATE_FROM_USER = -3,

	/**
	 * Unspecified reason
	 */
	INM_WIFI_REASON_UNSPECIFIED = 1,

	/**
	 * Previous authentication no longer valid
	 */
	INM_WIFI_REASON_PREV_AUTH_NOT_VALID = 2,

	/**
	 * Deauthenticated because sending STA is leaving (or has left) IBSS or ESS
	 */
	INM_WIFI_REASON_DEAUTH_LEAVING = 3,

	/**
	 * Disassociated due to inactivity
	 */
	INM_WIFI_REASON_DISASSOC_DUE_TO_INACTIVITY = 4,

	/**
	 * Disassociated because AP is unable to handle all currently associated STAs
	 */
	INM_WIFI_REASON_DISASSOC_AP_BUSY = 5,

	/**
	 * Class 2 frame received from nonauthenticated STA
	 */
	INM_WIFI_REASON_CLASS2_FRAME_FROM_NONAUTH_STA = 6,

	/**
	 * Class 3 frame received from nonassociated STA
	 */
	INM_WIFI_REASON_CLASS3_FRAME_FROM_NONASSOC_STA = 7,

	/**
	 * Disassociated because sending STA is leaving (or has left) BSS
	 */
	INM_WIFI_REASON_DISASSOC_STA_HAS_LEFT = 8,

	/**
	 * STA requesting (re)association is not authenticated with responding STA
	 */
	INM_WIFI_REASON_STA_REQ_ASSOC_WITHOUT_AUTH = 9,

	/**
	 * Disassociated because the information in the Power Capability
	 * element is unacceptable
	 */
	INM_WIFI_REASON_PWR_CAPABILITY_NOT_VALID = 10,

	/**
	 * Disassociated because the information in the Supported Channels
	 * element is unacceptable
	 */
	INM_WIFI_REASON_SUPPORTED_CHANNEL_NOT_VALID = 11,

	/**
	 * Invalid element i.e., an element defined in this standard for which the
	 * content does not meet the specifications in Clause 8
	 */
	INM_WIFI_REASON_INVALID_IE = 13,

	/**
	 * Message Integrity Code (MIC) failure
	 */
	INM_WIFI_REASON_MICHAEL_MIC_FAILURE = 14,

	/**
	 * 4-Way Handshake timeout
	 */
	INM_WIFI_REASON_4WAY_HANDSHAKE_TIMEOUT = 15,

	/**
	 * Group Key Handshake timeout
	 */
	INM_WIFI_REASON_GROUP_KEY_UPDATE_TIMEOUT = 16,

	/**
	 * Element in 4-Way Handshake different from (Re)Association Request/Probe
	 * Response/Beacon frame
	 */
	INM_WIFI_REASON_IE_IN_4WAY_DIFFERS = 17,

	/**
	 * Invalid group cipher
	 */
	INM_WIFI_REASON_GROUP_CIPHER_NOT_VALID = 18,

	/**
	 * Invalid pairwise cipher
	 */
	INM_WIFI_REASON_PAIRWISE_CIPHER_NOT_VALID = 19,

	/**
	 * Invalid AKMP
	 */
	INM_WIFI_REASON_AKMP_NOT_VALID = 20,

	/**
	 * Unsupported RSNE version
	 */
	INM_WIFI_REASON_UNSUPPORTED_RSN_IE_VERSION = 21,

	/**
	 * Invalid RSNE capabilities
	 */
	INM_WIFI_REASON_INVALID_RSN_IE_CAPAB = 22,

	/**
	 * IEEE 802.1X authentication failed
	 */
	INM_WIFI_REASON_IEEE_802_1X_AUTH_FAILED = 23,

	/**
	 * Cipher suite rejected because of the security policy
	 */
	INM_WIFI_REASON_CIPHER_SUITE_REJECTED = 24,

	/**
	 * TDLS direct-link teardown due to TDLS peer STA unreachable via the
	 * TDLS direct link
	 */
	INM_WIFI_REASON_TDLS_TEARDOWN_UNREACHABLE = 25,

	/**
	 * TDLS direct-link teardown for unspecified reason
	 */
	INM_WIFI_REASON_TDLS_TEARDOWN_UNSPECIFIED = 26,

	/**
	 * Disassociated because excessive number of frames need to be acknowledged,
	 * but are not acknowledged due to AP transmissions and/or poor channel conditions
	 */
	INM_WIFI_REASON_DISASSOC_LOW_ACK = 34,

	/**
	 * SME cancels the mesh peering instance with the reason other than reaching
	 * the maximum number of peer mesh STAs
	 */
	INM_WIFI_REASON_MESH_PEERING_CANCELLED = 52,

	/**
	 * The mesh STA has reached the supported maximum number of peer mesh STAs
	 */
	INM_WIFI_REASON_MESH_MAX_PEERS = 53,

	/**
	 * The received information violates the Mesh Configuration policy
	 * configured in the mesh STA profile
	 */
	INM_WIFI_REASON_MESH_CONFIG_POLICY_VIOLATION = 54,

	/**
	 * The mesh STA has received a Mesh Peering Close message requesting
	 * to close the mesh peering
	 */
	INM_WIFI_REASON_MESH_CLOSE_RCVD = 55,

	/**
	 * The mesh STA has resent dot11MeshMaxRetries Mesh Peering Open messages,
	 * without receiving a Mesh Peering Confirm message
	 */
	INM_WIFI_REASON_MESH_MAX_RETRIES = 56,

	/**
	 * The confirmTimer for the mesh peering instance times out
	 */
	INM_WIFI_REASON_MESH_CONFIRM_TIMEOUT = 57,

	/**
	 * The mesh STA fails to unwrap the GTK or the values in the wrapped
	 * contents do not match
	 */
	INM_WIFI_REASON_MESH_INVALID_GTK = 58,

	/**
	 * The mesh STA receives inconsistent information about the mesh parameters
	 * between Mesh Peering Management frames
	 */
	INM_WIFI_REASON_MESH_INCONSISTENT_PARAMS = 59,

	/**
	 * The mesh STA does not have proxy information for this external destination
	 */
	INM_WIFI_REASON_MESH_INVALID_SECURITY_CAP = 60,

} inm_wifi_disconnect_reason_e;

/**
 * @brief Enumeration for Wi-Fi Association Status code, provided by the supplicant.
 * @details The Wi-Fi Standard Reference : Status codes (IEEE 802.11-2007, 7.3.1.9, Table 7-23).
 * @since_tizen 5.0
 */
typedef enum {
	/**
	 * Successful
	 */
	INM_WLAN_STATUS_SUCCESS = 0,

	/**
	 * Unspecified failure
	 */
	INM_WLAN_STATUS_UNSPECIFIED_FAILURE = 1,

	/**
	 * Responding STA does not support the specified authentication
	 * algorithm
	 */
	INM_WLAN_STATUS_NOT_SUPPORTED_AUTH_ALG = 13,

	/**
	 * Association denied because AP is unable to handle additional
	 * associated STAs
	 */
	INM_WLAN_STATUS_AP_UNABLE_TO_HANDLE_NEW_STA = 17,

	/**
	 * Association denied because the requesting STA does not support
	 * HT feature
	 */
	INM_WLAN_STATUS_ASSOC_DENIED_NO_HT = 27,

	/**
	 * Association request rejected temporarily
	 */
	INM_WLAN_STATUS_ASSOC_REJECTED_TEMPORARILY = 30,

	/**
	 * Robust management frame policy violation
	 */
	INM_WLAN_STATUS_ROBUST_MGMT_FRAME_POLICY_VIOLATION = 31,

	/**
	 * Invalid element
	 */
	INM_WLAN_STATUS_INVALID_IE = 40,

	/**
	 * Invalid group cipher
	 */
	INM_WLAN_STATUS_GROUP_CIPHER_NOT_VALID = 41,

	/**
	 * Invalid pairwise cipher
	 */
	INM_WLAN_STATUS_PAIRWISE_CIPHER_NOT_VALID = 42,

	/**
	 * Invalid AKMP
	 */
	INM_WLAN_STATUS_AKMP_NOT_VALID = 43,

	/**
	 * Cipher suite rejected because of security policy
	 */
	INM_WLAN_STATUS_CIPHER_REJECTED_PER_POLICY = 46,

	/**
	 * Association denied because the Listen interval is too large
	 */
	INM_WLAN_STATUS_ASSOC_DENIED_LISTEN_INT_TOO_LARGE = 51,

	/**
	 * Invalid pairwise master key identifier (PKMID)
	 */
	INM_WLAN_STATUS_INVALID_PMKID = 53,

	/**
	 * Invalid MDE
	 */
	INM_WLAN_STATUS_INVALID_MDIE = 54,

	/**
	 * Invalid FTE
	 */
	INM_WLAN_STATUS_INVALID_FTIE = 55,

	/**
	 * Association denied because the requesting STA does not support
	 * VHT feature
	 */
	INM_WLAN_STATUS_ASSOC_DENIED_NO_VHT = 104,
} inm_wifi_assoc_status_code_e;

/**
 * @}
 */

/**
 * @addtogroup CAPI_NETWORK_INM_LINK_MODULE
 * @{
 */

/**
 * @brief Enumeration for link flag.
 * @since_tizen 5.5
*/
typedef enum {

	/**
	 * Interface is up
	 */
	INM_LINK_FLAG_UP = 0x1,

	/**
	 * Broadcast address is valid
	 */
	INM_LINK_FLAG_BROADCAST = 0x2,

	/**
	 * Debugging
	 */
	INM_LINK_FLAG_DEBUG = 0x4,

	/**
	 * link is a loopback
	 */
	INM_LINK_FLAG_LOOPBACK = 0x8,

	/**
	 * Interface is has p-p link
	 */
	INM_LINK_FLAG_P2P = 0x10,

	/**
	 * Avoid use of trailers
	 */
	INM_LINK_FLAG_NOTRAILERS = 0x20,

	/**
	 * Interface RFC2863 OPER_UP
	 */
	INM_LINK_FLAG_RUNNING = 0x40,

	/**
	 * No ARP protocol
	 */
	INM_LINK_FLAG_NOARP = 0x80,

	/**
	 * Receive all packets
	 */
	INM_LINK_FLAG_PROMISC = 0x100,

	/**
	 * Receive all multicast packets
	 */
	INM_LINK_FLAG_ALLMULTI = 0x200,

	/**
	 * Supports multicast
	 */
	INM_LINK_FLAG_MULTICAST = 0x1000,

	/**
	 *  Dialup device with changing addresses
	 */
	INM_LINK_FLAG_DYNAMIC = 0x8000,

	/**
	 * Driver signals L1 up
	 */
	INM_LINK_FLAG_LOWER_UP = 0x10000,

	/**
	 * Driver signals dormant
	 */
	INM_LINK_FLAG_DORMANT = 0x20000,
} inm_link_flag_e;

/**
 * @brief Enumeration for link RFC 2863 operation status.
 * @since_tizen 5.5
*/
typedef enum {
	INM_LINK_OPER_UNKNOWN,         /**< UNKNOWN */
	INM_LINK_OPER_NOT_PRESENT,      /**< NOT PRESENT */
	INM_LINK_OPER_DOWN,            /**< DOWN */
	INM_LINK_OPER_LOWER_LAYER_DOWN,  /**< LOWER LAYER DOWN */
	INM_LINK_OPER_TESTING,         /**< TESTING */
	INM_LINK_OPER_DORMANT,         /**< DORMANT */
	INM_LINK_OPER_UP,              /**< UP */
} inm_link_oper_state_e;

/**
 * @brief Enumeration for link scope.
 * @since_tizen 5.5
*/
typedef enum {
	/**
	 * Scope nowhere
	 */
	INM_LINK_SCOPE_NOWHERE,

	/**
	 * A route has host scope when it leads to
	 * a destination address on the local host
	 */
	INM_LINK_SCOPE_HOST,

	/**
	 * A route has host scope when it leads to
	 * a destination address on the local network
	 */
	INM_LINK_SCOPE_LINK,

	/**
	 * Valid only within this site (IPv6)
	 */
	INM_LINK_SCOPE_SITE,

	/**
	 * A route has universe scope when it leads to
	 * addresses more than one hop away
	 */
	INM_LINK_SCOPE_UNIVERSE,
} inm_link_scope_e;

/**
 * @brief Enumeration for link route scope.
 * @since_tizen 5.5
*/
typedef enum {
	/**
	 * Unspecified
	 */
	INM_LINK_ROUTE_TYPE_UNSPEC,

	/**
	 * Gateway or direct
	 */
	INM_LINK_ROUTE_TYPE_UNICAST,

	/**
	 * Accept locally
	 */
	INM_LINK_ROUTE_TYPE_LOCAL,

	/**
	 * Accept locally as broadcast, send as broadcast
	 */
	INM_LINK_ROUTE_TYPE_BROADCAST,

	/**
	 * Accept locally as broadcast, but send as unicast
	 */
	INM_LINK_ROUTE_TYPE_ANYCAST,

	/**
	 * Multicast
	 */
	INM_LINK_ROUTE_TYPE_MULTICAST,

	/**
	 * Drop
	 */
	INM_LINK_ROUTE_TYPE_BLACKHOLE,

	/**
	 * Destination is unreachable
	 */
	INM_LINK_ROUTE_TYPE_UNREACHABLE,

	/**
	 * Administratively prohibited
	 */
	INM_LINK_ROUTE_TYPE_PROHIBIT,

	/**
	 * Not in this table
	 */
	INM_LINK_ROUTE_TYPE_THROW,

	/**
	 * Translate this address
	 */
	INM_LINK_ROUTE_TYPE_NAT,

	/**
	 * Use external resolver
	 */
	INM_LINK_ROUTE_TYPE_XRESOLVE,
} inm_link_route_type_e;

/**
 * @}
 */


/**
 * @addtogroup CAPI_NETWORK_INM_MODULE
 * @{
 */

/**
 * @brief Initializes INM.
 * @since_tizen 5.0
 * @privlevel public
 * @privilege %http://tizen.org/privilege/network.get
 * @remarks You must release @a inm using inm_deinitialize().
 * @param[out] inm  The INM handle
 * @return @c 0 on success, otherwise negative error value
 * @retval #INM_ERROR_NONE                 Successful
 * @retval #INM_ERROR_OUT_OF_MEMORY        Out of memory
 * @retval #INM_ERROR_PERMISSION_DENIED    Permission Denied
 * @retval #INM_ERROR_INVALID_PARAMETER    Invalid parameter
 * @retval #INM_ERROR_NOT_SUPPORTED        Not supported
 * @retval #INM_ERROR_ALREADY_INITIALIZED  Already initialized
 * @retval #INM_ERROR_OPERATION_FAILED     Operation failed
 */
int inm_initialize(inm_h *inm);

/**
 * @brief Deinitializes INM.
 * @since_tizen 5.0
 * @param[out] inm  The INM handle
 * @return 0 on success, otherwise negative error value
 * @retval #INM_ERROR_NONE               Successful
 * @retval #INM_ERROR_INVALID_PARAMETER  Invalid parameter
 * @retval #INM_ERROR_NOT_SUPPORTED      Not supported
 * @retval #INM_ERROR_NOT_INITIALIZED    Not initialized
 * @retval #INM_ERROR_OPERATION_FAILED   Operation failed
 */
int inm_deinitialize(inm_h inm);

/**
 * @}
 */


/**
 * @addtogroup CAPI_NETWORK_INM_SPECIFIC_MODULE
 * @{
 */

/**
 * @brief Called when the Ethernet cable state is changed.
 * @since_tizen 5.0
 * @param[in] state      The Ethernet cable state
 * @param[in] user_data  The user data passed from the callback registration function
 * @see inm_set_ethernet_cable_state_changed_cb()
 * @see inm_unset_ethernet_cable_state_changed_cb()
 */
typedef void(*inm_ethernet_cable_state_changed_cb)(inm_ethernet_cable_state_e state,
		void *user_data);

/**
 * @brief Called when the Wi-Fi Module state is changed.
 * @since_tizen 5.0
 * @param[in] state      The Wi-Fi Module state
 * @param[in] user_data  The user data passed from the callback registration function
 * @see inm_set_wifi_module_state_changed_cb()
 * @see inm_unset_wifi_module_state_changed_cb()
 */
typedef void(*inm_wifi_module_state_changed_cb)(inm_wifi_module_state_e state,
		void *user_data);

/**
 * @brief Called when the IP conflict state is changed.
 * @since_tizen 5.0
 * @remarks @a if_name and @a ip should not be freed. @a if_name and @a ip is available only in the callback. To use
 *          outside the callback, make a copy.
 * @param[in] if_name    The destination interface name causing conflict
 * @param[in] ip         The local IP address causing conflict
 * @param[in] state      The current state
 * @param[in] user_data  The user data passed from the callback registration function
 * @see inm_set_ip_conflict_cb()
 * @see inm_unset_ip_conflict_cb()
 */
typedef void(*inm_ip_conflict_cb)(char *if_name, char *ip, inm_ip_conflict_state_e state, void *user_data);

/**
 * @brief Called when the TCP congestion level is updated.
 * @since_tizen 5.0
 * @param[in] level      The current TCP congestion level
 * @param[in] user_data  The user data passed from the callback registration function
 * @see inm_set_congestion_level_cb()
 * @see inm_unset_congestion_level_cb()
 */
typedef void(*inm_congestion_level_cb)(inm_congestion_level_e level, void *user_data);

/**
 * @brief Called when the retry TX rate is updated.
 * @since_tizen 5.0
 * @param[in] rate       The current TCP TX retry rate
 * @param[in] user_data  The user data passed from the callback registration function
 * @see inm_set_retry_tx_rate_cb()
 * @see inm_unset_retry_tx_rate_cb()
 */
typedef void(*inm_retry_tx_rate_cb)(int rate, void *user_data);

/**
 * @brief Called when the channel interference is updated.
 * @since_tizen 5.0
 * @param[in] freq          The frequency of wireless network
 * @param[in] channel_intf  The current channel interference
 * @param[in] user_data     The user data passed from the callback registration function
 * @see inm_set_channel_interference_cb()
 * @see inm_unset_channel_interference_cb()
 */
typedef void(*inm_channel_interference_cb)(int freq, double channel_intf, void *user_data);

/**
 * @brief Called when the Gateway state is changed.
 * @since_tizen 9.0
 * @remarks @a gateway_ip should not be freed. @a gateway_ip is available only in the callback. To use
 *          outside the callback, make a copy.
 * @param[in] found      @c true if Gateway IP is found, @c false if Gateway IP not found
 * @param[in] gateway_ip The gateway IP address found on a local network
 * @param[in] user_data  The user data passed from the callback registration function
 * @see inm_set_default_gateway_state_changed_cb()
 * @see inm_unset_default_gateway_state_changed_cb()
 */
typedef void (*inm_default_gateway_state_changed_cb) (bool found, const char *gateway_ip, void *user_data);

/**
 * @brief Called when the DNS state is changed.
 * @since_tizen 9.0
 * @param[in] found      @c true if DNS lookup succeeded, @c false if DNS lookup failed
 * @param[in] user_data  The user data passed from the callback registration function
 * @see inm_set_default_dns_state_changed_cb()
 * @see inm_unset_default_dns_state_changed_cb()
 */
typedef void (*inm_default_dns_state_changed_cb) (bool found, void *user_data);

/**
 * @brief Called after inm_http_online_check() is completed.
 * @since_tizen 9.0
 * @param[in] status        The status of online check is succeed or not
 * @param[in] user_data     The user data passed from inm_http_online_check()
 * @pre inm_http_online_check() will invoke this callback function.
 * @see inm_http_online_check()
 */
typedef void (*inm_default_http_check_finished_cb) (bool status, void *user_data);
/**
 * @brief Sets a callback called when the Ethernet cable state is changed.
 * @details This function allows you to register a callback function that will be triggered
 *          whenever there is a change in the Ethernet cable connection state.
 *          It provides useful information regarding the current Ethernet cable state through the provided callback function.
 * @since_tizen 5.0
 * @remarks To remove a previously registered callback, use inm_unset_ethernet_cable_state_changed_cb().
 * @param[in] inm        The monitor handle
 * @param[in] callback   The callback function to be called
 * @param[in] user_data  The user data passed to the callback function
 * @return @c 0 on success, otherwise negative error value
 * @retval #INM_ERROR_NONE               Successful
 * @retval #INM_ERROR_INVALID_PARAMETER  Invalid parameter
 * @retval #INM_ERROR_NOT_SUPPORTED      Not supported
 * @retval #INM_ERROR_NOT_INITIALIZED    Not initialized
 * @retval #INM_ERROR_OPERATION_FAILED   Operation failed
 * @see inm_ethernet_cable_state_changed_cb()
 * @see inm_unset_ethernet_cable_state_changed_cb()
 */
int inm_set_ethernet_cable_state_changed_cb(inm_h inm,
		inm_ethernet_cable_state_changed_cb callback, void *user_data);

/**
 * @brief Unsets the callback called when the Ethernet cable state is changed.
 * @details This function unregisters a previously registered callback function used to
 *          receive notifications about Ethernet cable state changes.
 *          By calling this function, you effectively disable further calls to the associated callback function.
 * @since_tizen 5.0
 * @param[in] inm  The monitor handle
 * @return @c 0 on success, otherwise negative error value
 * @retval #INM_ERROR_NONE               Successful
 * @retval #INM_ERROR_INVALID_PARAMETER  Invalid parameter
 * @retval #INM_ERROR_NOT_SUPPORTED      Not supported
 * @retval #INM_ERROR_NOT_INITIALIZED    Not initialized
 * @retval #INM_ERROR_OPERATION_FAILED   Operation failed
 * @see inm_ethernet_cable_state_changed_cb()
 * @see inm_set_ethernet_cable_state_changed_cb()
 */
int inm_unset_ethernet_cable_state_changed_cb(inm_h inm);

/**
 * @brief Gets the Ethernet cable state.
 * @details This function retrieves the current state of the Ethernet cable connection. Depending on the returned state,
 *      	you can determine whether the Ethernet cable is attached or detached.
 * @since_tizen 5.0
 * @privlevel public
 * @privilege %http://tizen.org/privilege/network.get
 * @param[in] inm     The monitor handle
 * @param[out] state  The Ethernet cable state
 * @return @c 0 on success, otherwise negative error value
 * @retval #INM_ERROR_NONE               Successful
 * @retval #INM_ERROR_PERMISSION_DENIED  Permission Denied
 * @retval #INM_ERROR_INVALID_PARAMETER  Invalid parameter
 * @retval #INM_ERROR_NOT_SUPPORTED      Not supported
 * @retval #INM_ERROR_NOT_INITIALIZED    Not initialized
 * @retval #INM_ERROR_OPERATION_FAILED   Operation failed
 */
int inm_get_ethernet_cable_state(inm_h inm, inm_ethernet_cable_state_e *state);

/**
 * @brief Sets a callback called when the Wi-Fi Module state is changed.
 * @details This function allows you to register a callback function that will be triggered
 *          whenever there is a change in the Wi-Fi module state.
 *          It provides useful information regarding the current Wi-Fi module state through the provided callback function.
 * @since_tizen 5.0
 * @param[in] inm        The monitor handle
 * @param[in] callback   The callback function to be called
 * @param[in] user_data  The user data passed to the callback function
 * @return @c 0 on success, otherwise negative error value
 * @retval #INM_ERROR_NONE               Successful
 * @retval #INM_ERROR_INVALID_PARAMETER  Invalid parameter
 * @retval #INM_ERROR_NOT_SUPPORTED      Not supported
 * @retval #INM_ERROR_NOT_INITIALIZED    Not initialized
 * @retval #INM_ERROR_OPERATION_FAILED   Operation failed
 * @see inm_wifi_module_state_changed_cb()
 * @see inm_unset_wifi_module_state_changed_cb()
*/
int inm_set_wifi_module_state_changed_cb(inm_h inm,
		inm_wifi_module_state_changed_cb callback, void *user_data);

/**
 * @brief Unsets the callback called when the Wi-Fi Module state is changed.
 * @details This function unregisters a previously registered callback function used to
 *          receive notifications about Wi-Fi module state changes. By calling this function,
 *          you effectively disable further calls to the associated callback function.
 * @since_tizen 5.0
 * @param[in] inm  The monitor handle
 * @return @c 0 on success, otherwise negative error value
 * @retval #INM_ERROR_NONE               Successful
 * @retval #INM_ERROR_INVALID_PARAMETER  Invalid parameter
 * @retval #INM_ERROR_NOT_SUPPORTED      Not supported
 * @retval #INM_ERROR_NOT_INITIALIZED    Not initialized
 * @retval #INM_ERROR_OPERATION_FAILED   Operation failed
 * @see inm_wifi_module_state_changed_cb()
 * @see inm_set_wifi_module_state_changed_cb()
 */
int inm_unset_wifi_module_state_changed_cb(inm_h inm);

/**
 * @brief Gets the Wi-Fi Module state.
 * @details This function retrieves the current state of the wifi module. Depending on the returned state,
 *          you can determine whether the wifi module is attached or detached.
 * @since_tizen 5.0
 * @param[in] inm     The monitor handle
 * @param[out] state  The Wi-Fi Module state
 * @return @c 0 on success, otherwise negative error value
 * @retval #INM_ERROR_NONE               Successful
 * @retval #INM_ERROR_INVALID_PARAMETER  Invalid parameter
 * @retval #INM_ERROR_NOT_SUPPORTED      Not supported
 * @retval #INM_ERROR_NOT_INITIALIZED    Not initialized
 * @retval #INM_ERROR_OPERATION_FAILED   Operation failed
 */
int inm_get_wifi_module_state(inm_h inm, inm_wifi_module_state_e *state);

/**
 * @brief Sets a callback called when the Gateway state is changed.
 * @details This function allows you to register a callback function that will be triggered
 *          whenever there is a change in the default gateway state.
 *          It provides useful information regarding the current default gateway state through the provided callback function.
 * @since_tizen 9.0
 * @param[in] inm       The monitor handle
 * @param[in] callback  The callback function to be called
 * @param[in] user_data The user data passed to the callback function
 * @return @c 0 on success, otherwise negative error value
 * @retval #INM_ERROR_NONE              Successful
 * @retval #INM_ERROR_NOT_SUPPORTED     Not supported
 * @retval #INM_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #INM_ERROR_NOT_INITIALIZED   Not initialized
 */
int inm_set_default_gateway_state_changed_cb(inm_h inm,
		inm_default_gateway_state_changed_cb callback, void *user_data);

/**
 * @brief Unsets the callback called when the Gateway state is changed.
 * @details This function unregisters a previously registered callback function used to
 *          receive notifications about default gateway state changes. By calling this function,
 *          you effectively disable further calls to the associated callback function.
 * @since_tizen 9.0
 * @param[in] inm  The monitor handle
 * @return @c 0 on success, otherwise negative error value
 * @retval #INM_ERROR_NONE              Successful
 * @retval #INM_ERROR_NOT_SUPPORTED     Not supported
 * @retval #INM_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #INM_ERROR_NOT_INITIALIZED   Not initialized
 * @see inm_default_gateway_state_changed_cb()
 * @see inm_set_default_gateway_state_changed_cb()
 */
int inm_unset_default_gateway_state_changed_cb(inm_h inm);

/**
 * @brief Gets the Gateway state.
 * @details This function retrieves the current state of the default gateway.
 *          Depending on the returned state, you can determine whether the default gateway is invaild or vaild.
 * @since_tizen 9.0
 * @privlevel public
 * @privilege %http://tizen.org/privilege/network.get
 * @param[in] inm     The monitor handle
 * @param[out] state  The Gateway state
 * @return @c 0 on success, otherwise negative error value
 * @retval #INM_ERROR_NONE               Successful
 * @retval #INM_ERROR_NOT_SUPPORTED      Not supported
 * @retval #INM_ERROR_PERMISSION_DENIED  Permission Denied
 * @retval #INM_ERROR_INVALID_PARAMETER  Invalid parameter
 * @retval #INM_ERROR_NOT_INITIALIZED    Not initialized
 * @retval #INM_ERROR_OPERATION_FAILED   Operation failed
 * @retval #INM_ERROR_OUT_OF_MEMORY      Out of memory
 * @retval #INM_ERROR_NOW_IN_PROGRESS    Now in progress
 */
int inm_get_gateway_state(inm_h inm, inm_gateway_state_e *state);

/**
 * @brief Sets a callback called when the DNS state is changed.
 * @details This function allows you to register a callback function that will be triggered
 *          whenever there is a change in the default DNS state.
 *          It provides useful information regarding the current default DNS state through the provided callback function.
 * @since_tizen 9.0
 * @param[in] inm       The monitor handle
 * @param[in] callback  The callback function to be called
 * @param[in] user_data The user data passed to the callback function
 * @return @c 0 on success, otherwise negative error value
 * @retval #INM_ERROR_NONE              Successful
 * @retval #INM_ERROR_NOT_SUPPORTED     Not supported
 * @retval #INM_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #INM_ERROR_NOT_INITIALIZED   Not initialized
 */
int inm_set_default_dns_state_changed_cb(inm_h inm,
		inm_default_dns_state_changed_cb callback, void *user_data);

/**
 * @brief Unsets the callback called when the DNS state is changed.
 * @details This function unregisters a previously registered callback function used to
 *          receive notifications about default DNS state changes. By calling this function,
 *          you effectively disable further calls to the associated callback function.
 * @since_tizen 9.0
 * @param[in] inm  The monitor handle
 * @return @c 0 on success, otherwise negative error value
 * @retval #INM_ERROR_NONE              Successful
 * @retval #INM_ERROR_NOT_SUPPORTED     Not supported
 * @retval #INM_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #INM_ERROR_NOT_INITIALIZED   Not initialized
 * @see inm_default_dns_state_changed_cb()
 * @see inm_set_default_dns_state_changed_cb()
 */
int inm_unset_default_dns_state_changed_cb(inm_h inm);

/**
 * @brief Gets the DNS state.
 * @details This function retrieves the current state of the default DNS server
 *          Depending on the returned state, you can determine whether the default DNS server is invaild or vaild.
 * @since_tizen 9.0
 * @privlevel public
 * @privilege %http://tizen.org/privilege/network.get
 * @param[in] inm     The monitor handle
 * @param[out] state  The DNS state
 * @return @c 0 on success, otherwise negative error value
 * @retval #INM_ERROR_NONE               Successful
 * @retval #INM_ERROR_NOT_SUPPORTED      Not supported
 * @retval #INM_ERROR_PERMISSION_DENIED  Permission Denied
 * @retval #INM_ERROR_INVALID_PARAMETER  Invalid parameter
 * @retval #INM_ERROR_NOT_INITIALIZED    Not initialized
 * @retval #INM_ERROR_OPERATION_FAILED   Operation failed
 * @retval #INM_ERROR_OUT_OF_MEMORY      Out of memory
 * @retval #INM_ERROR_NOW_IN_PROGRESS    Now in progress
 */
int inm_get_dns_state(inm_h inm, inm_dns_state_e *state);

/**
 * @brief Initiates the one-time Online HTTP check.
 * @details This function initiates a one-time HTTP connectivity test over the Internet.
 *          Upon completion, it invokes the provided callback function with the status of connection.
 * @since_tizen 9.0
 * @privlevel public
 * @privilege %http://tizen.org/privilege/network.get
 * @param[in] inm       The monitor handle
 * @param[in] callback  The callback function to be called
 * @param[in] user_data The user data passed to the callback function
 * @return @c 0 on success, otherwise negative error value
 * @retval #INM_ERROR_NONE               Successful
 * @retval #INM_ERROR_NOT_SUPPORTED      Not supported
 * @retval #INM_ERROR_PERMISSION_DENIED  Permission Denied
 * @retval #INM_ERROR_INVALID_PARAMETER  Invalid parameter
 * @retval #INM_ERROR_NOT_INITIALIZED    Not initialized
 * @retval #INM_ERROR_OPERATION_FAILED   Operation failed
 * @retval #INM_ERROR_OUT_OF_MEMORY      Out of memory
 * @retval #INM_ERROR_NOW_IN_PROGRESS    Now in progress
 */
int inm_http_online_check(inm_h inm, inm_default_http_check_finished_cb callback,
			void *user_data);

/**
 * @brief Sets the callback called when IP conflict state is changed.
 * @since_tizen 5.0
 * @param[in] inm        The monitoring handle
 * @param[in] callback   The callback function to be called
 * @param[in] user_data  The user data passed to the callback function
 * @return 0 on success, otherwise negative error value
 * @retval #INM_ERROR_NONE               Successful
 * @retval #INM_ERROR_INVALID_PARAMETER  Invalid parameter
 * @retval #INM_ERROR_NOT_SUPPORTED      Not supported
 * @retval #INM_ERROR_NOT_INITIALIZED    Not initialized
 * @retval #INM_ERROR_OPERATION_FAILED   Operation failed
 */
int inm_set_ip_conflict_cb(inm_h inm,
		inm_ip_conflict_cb callback, void *user_data);

/**
 * @brief Unsets the callback called when IP conflict state is changed.
 * @since_tizen 5.0
 * @param[in] inm  The monitoring handle
 * @return 0 on success, otherwise negative error value
 * @retval #INM_ERROR_NONE               Successful
 * @retval #INM_ERROR_INVALID_PARAMETER  Invalid parameter
 * @retval #INM_ERROR_NOT_SUPPORTED      Not supported
 * @retval #INM_ERROR_NOT_INITIALIZED    Not initialized
 * @retval #INM_ERROR_OPERATION_FAILED   Operation failed
 */
int inm_unset_ip_conflict_cb(inm_h inm);

/**
 * @brief Checks whether IP conflict detection is enabled.
 * @details This function checks whether IP address conflict detection is currently enabled or disabled within the system.
 * @since_tizen 5.0
 * @privlevel public
 * @privilege %http://tizen.org/privilege/network.get
 * @param[in] inm     The monitoring handle
 * @param[out] state  @c true if IP conflict detection is enabled,
 *             otherwise @c false if IP conflict detection is disabled.
 * @return 0 on success, otherwise negative error value
 * @retval #INM_ERROR_NONE               Successful
 * @retval #INM_ERROR_PERMISSION_DENIED  Permission Denied
 * @retval #INM_ERROR_INVALID_PARAMETER  Invalid parameter
 * @retval #INM_ERROR_NOT_SUPPORTED      Not supported
 * @retval #INM_ERROR_NOT_INITIALIZED    Not initialized
 * @retval #INM_ERROR_OPERATION_FAILED   Operation failed
 */
int inm_ip_conflict_detect_is_enabled(inm_h inm, bool *state);

/**
 * @brief Gets the state of the IP conflict.
 * @details This function obtains the current state of IP address conflicts within the system.
 *          Based on the returned state, you can determine the status of the IP conflict.
 * @since_tizen 5.0
 * @privlevel public
 * @privilege %http://tizen.org/privilege/network.get
 * @param[in] inm     The monitoring handle
 * @param[out] state  The current state of IP conflict
 * @return 0 on success, otherwise negative error value
 * @retval #INM_ERROR_NONE               Successful
 * @retval #INM_ERROR_PERMISSION_DENIED  Permission Denied
 * @retval #INM_ERROR_INVALID_PARAMETER  Invalid parameter
 * @retval #INM_ERROR_NOT_SUPPORTED      Not supported
 * @retval #INM_ERROR_NOT_INITIALIZED    Not initialized
 * @retval #INM_ERROR_OPERATION_FAILED   Operation failed
 */
int inm_get_ip_conflict_state(inm_h inm,
		inm_ip_conflict_state_e *state);

/**
 * @brief Gets the statistics information.
 * @details This function retrieves specific statistical information regarding network usage
 *          depending on the parameters provided. Currently only WiFi (#INM_CONNECTION_TYPE_WIFI)
 *          and Cellular (#INM_CONNECTION_TYPE_CELLULAR) connections are supported.
 * @since_tizen 5.0
 * @privlevel public
 * @privilege %http://tizen.org/privilege/network.get
 * @param[in] inm              The monitor handle
 * @param[in] connection_type  The type of connection \n
 *            Only #INM_CONNECTION_TYPE_WIFI and #INM_CONNECTION_TYPE_CELLULAR are supported
 * @param[in] statistics_type  The type of statistics
 * @param[out] size            The received data size of the last cellular packet data connection (bytes)
 * @return @c 0 on success,
 *         otherwise a negative error value
 * @retval #INM_ERROR_NONE               Successful
 * @retval #INM_ERROR_PERMISSION_DENIED  Permission Denied
 * @retval #INM_ERROR_INVALID_PARAMETER  Invalid parameter
 * @retval #INM_ERROR_NOT_SUPPORTED      Not supported
 * @retval #INM_ERROR_NOT_INITIALIZED    Not initialized
 * @retval #INM_ERROR_OPERATION_FAILED   Operation failed
 */
int inm_get_statistics(inm_h inm, inm_connection_type_e connection_type,
		inm_statistics_type_e statistics_type, unsigned long long* size);

/**
 * @brief Resets the statistics information.
 * @since_tizen 5.0
 * @privlevel public
 * @privilege %http://tizen.org/privilege/network.set
 * @param[in] inm              The monitor handle
 * @param[in] connection_type  The type of connection \n
 *            Only #INM_CONNECTION_TYPE_WIFI and #INM_CONNECTION_TYPE_CELLULAR are supported
 * @param[in] statistics_type  The type of statistics
 * @return @c 0 on success,
 *         otherwise a negative error value
 * @retval #INM_ERROR_NONE               Successful
 * @retval #INM_ERROR_PERMISSION_DENIED  Permission Denied
 * @retval #INM_ERROR_INVALID_PARAMETER  Invalid parameter
 * @retval #INM_ERROR_NOT_SUPPORTED      Not supported
 * @retval #INM_ERROR_NOT_INITIALIZED    Not initialized
 * @retval #INM_ERROR_OPERATION_FAILED   Operation failed
 */
int inm_reset_statistics(inm_h inm, inm_connection_type_e connection_type,
		inm_statistics_type_e statistics_type);

/**
 * @brief Sets the callback called when the TCP congestion level is updated.
 * @since_tizen 5.0
 * @param[in] inm        The monitoring handle
 * @param[in] callback   The callback function to be called
 * @param[in] user_data  The user data passed to the callback function
 * @return 0 on success, otherwise negative error value
 * @retval #INM_ERROR_NONE               Successful
 * @retval #INM_ERROR_INVALID_PARAMETER  Invalid parameter
 * @retval #INM_ERROR_NOT_SUPPORTED      Not supported
 * @retval #INM_ERROR_NOT_INITIALIZED    Not initialized
 * @retval #INM_ERROR_OPERATION_FAILED   Operation failed
 */
int inm_set_congestion_level_cb(inm_h inm,
		inm_congestion_level_cb callback, void *user_data);

/**
 * @brief Unsets the callback called when the TCP congestion level is updated.
 * @since_tizen 5.0
 * @param[in] inm  The monitoring handle
 * @return 0 on success, otherwise negative error value
 * @retval #INM_ERROR_NONE               Successful
 * @retval #INM_ERROR_INVALID_PARAMETER  Invalid parameter
 * @retval #INM_ERROR_NOT_SUPPORTED      Not supported
 * @retval #INM_ERROR_NOT_INITIALIZED    Not initialized
 * @retval #INM_ERROR_OPERATION_FAILED   Operation failed
 */
int inm_unset_congestion_level_cb(inm_h inm);

/**
 * @brief Gets the TCP congestion level.
 * @since_tizen 5.0
 * @privlevel public
 * @privilege %http://tizen.org/privilege/network.get
 * @param[in] inm     The monitoring handle
 * @param[out] level  The current TCP congestion level
 * @return 0 on success, otherwise negative error value
 * @retval #INM_ERROR_NONE               Successful
 * @retval #INM_ERROR_PERMISSION_DENIED  Permission Denied
 * @retval #INM_ERROR_INVALID_PARAMETER  Invalid parameter
 * @retval #INM_ERROR_NOT_SUPPORTED      Not supported
 * @retval #INM_ERROR_NOT_INITIALIZED    Not initialized
 * @retval #INM_ERROR_OPERATION_FAILED   Operation failed
 */
int inm_get_congestion_level(inm_h inm,
		inm_congestion_level_e *level);

/**
 * @brief Sets the callback called the TCP TX retry rate is updated.
 * @since_tizen 5.0
 * @param[in] inm        The monitoring handle
 * @param[in] callback   The callback function to be called
 * @param[in] user_data  The user data passed to the callback function
 * @return 0 on success, otherwise negative error value
 * @retval #INM_ERROR_NONE               Successful
 * @retval #INM_ERROR_INVALID_PARAMETER  Invalid parameter
 * @retval #INM_ERROR_NOT_SUPPORTED      Not supported
 * @retval #INM_ERROR_NOT_INITIALIZED    Not initialized
 * @retval #INM_ERROR_OPERATION_FAILED   Operation failed
 */
int inm_set_retry_tx_rate_cb(inm_h inm,
		inm_retry_tx_rate_cb callback, void *user_data);

/**
 * @brief Unsets the callback called when the TCP TX retry rate is updated.
 * @since_tizen 5.0
 * @param[in] inm  The monitoring handle
 * @return 0 on success, otherwise negative error value
 * @retval #INM_ERROR_NONE               Successful
 * @retval #INM_ERROR_INVALID_PARAMETER  Invalid parameter
 * @retval #INM_ERROR_NOT_SUPPORTED      Not supported
 * @retval #INM_ERROR_NOT_INITIALIZED    Not initialized
 * @retval #INM_ERROR_OPERATION_FAILED   Operation failed
 */
int inm_unset_retry_tx_rate_cb(inm_h inm);

/**
 * @brief Gets the TCP TX retry rate.
 * @since_tizen 5.0
 * @privlevel public
 * @privilege %http://tizen.org/privilege/network.get
 * @param[in] inm     The monitoring handle
 * @param[out] rate   The current TCP TX retry rate
 * @return 0 on success, otherwise negative error value
 * @retval #INM_ERROR_NONE               Successful
 * @retval #INM_ERROR_PERMISSION_DENIED  Permission Denied
 * @retval #INM_ERROR_INVALID_PARAMETER  Invalid parameter
 * @retval #INM_ERROR_NOT_SUPPORTED      Not supported
 * @retval #INM_ERROR_NOT_INITIALIZED    Not initialized
 * @retval #INM_ERROR_OPERATION_FAILED   Operation failed
 */
int inm_get_retry_tx_rate(inm_h inm,
		int *rate);

/**
 * @brief Sets the callback to be called when the channel interference is updated.
 * @since_tizen 5.0
 * @param[in] inm        The monitoring handle
 * @param[in] callback   The callback function to be called
 * @param[in] user_data  The user data passed to the callback function
 * @return 0 on success, otherwise negative error value
 * @retval #INM_ERROR_NONE               Successful
 * @retval #INM_ERROR_INVALID_PARAMETER  Invalid parameter
 * @retval #INM_ERROR_NOT_SUPPORTED      Not supported
 * @retval #INM_ERROR_NOT_INITIALIZED    Not initialized
 * @retval #INM_ERROR_OPERATION_FAILED   Operation failed
 */
int inm_set_channel_interference_cb(inm_h inm,
		inm_channel_interference_cb callback, void *user_data);

/**
 * @brief Unsets the callback called when the channel interference is updated.
 * @since_tizen 5.0
 * @param[in] inm  The monitoring handle
 * @return 0 on success, otherwise negative error value
 * @retval #INM_ERROR_NONE               Successful
 * @retval #INM_ERROR_INVALID_PARAMETER  Invalid parameter
 * @retval #INM_ERROR_NOT_SUPPORTED      Not supported
 * @retval #INM_ERROR_NOT_INITIALIZED    Not initialized
 * @retval #INM_ERROR_OPERATION_FAILED   Operation failed
 */
int inm_unset_channel_interference_cb(inm_h inm);

/**
 * @brief Gets the channel interference of the wireless network.
 * @since_tizen 5.0
 * @privlevel public
 * @privilege %http://tizen.org/privilege/network.get
 * @param[in] inm        The monitoring handle
 * @param[out] ch_intf   The current channel interference
 * @return 0 on success, otherwise negative error value
 * @retval #INM_ERROR_NONE               Successful
 * @retval #INM_ERROR_PERMISSION_DENIED  Permission Denied
 * @retval #INM_ERROR_INVALID_PARAMETER  Invalid parameter
 * @retval #INM_ERROR_NOT_SUPPORTED      Not supported
 * @retval #INM_ERROR_NOT_INITIALIZED    Not initialized
 * @retval #INM_ERROR_OPERATION_FAILED   Operation failed
 */
int inm_get_channel_interference(inm_h inm, double *ch_intf);

/**
 * @}
 */


/**
 * @addtogroup CAPI_NETWORK_INM_GENERAL_MODULE
 * @{
 */

/**
 * @brief Called when the cellular state is changed.
 * @since_tizen 5.0
 * @param[in] state      The cellular state
 * @param[in] user_data  The user data passed from the callback registration function
 * @see inm_set_cellular_state_changed_cb()
 * @see inm_unset_cellular_state_changed_cb()
 */
typedef void(*inm_cellular_state_changed_cb)(inm_cellular_state_e state,
		void *user_data);

/**
 * @brief Sets a callback called when the cellular state is changed.
 * @since_tizen 5.0
 * @param[in] inm        The monitor handle
 * @param[in] callback   The callback function to be called
 * @param[in] user_data  The user data passed to the callback function
 * @return @c 0 on success, otherwise negative error value
 * @retval #INM_ERROR_NONE               Successful
 * @retval #INM_ERROR_INVALID_PARAMETER  Invalid parameter
 * @retval #INM_ERROR_NOT_SUPPORTED      Not supported
 * @retval #INM_ERROR_NOT_INITIALIZED    Not initialized
 * @retval #INM_ERROR_OPERATION_FAILED   Operation failed
 * @see inm_cellular_state_changed_cb()
 * @see inm_unset_cellular_state_changed_cb()
 */
int inm_set_cellular_state_changed_cb(inm_h inm,
		inm_cellular_state_changed_cb callback, void *user_data);

/**
 * @brief Unsets the callback called when the cellular state is changed.
 * @since_tizen 5.0
 * @param[in] inm  The monitor handle
 * @return @c 0 on success, otherwise negative error value
 * @retval #INM_ERROR_NONE               Successful
 * @retval #INM_ERROR_INVALID_PARAMETER  Invalid parameter
 * @retval #INM_ERROR_NOT_SUPPORTED      Not supported
 * @retval #INM_ERROR_NOT_INITIALIZED    Not initialized
 * @retval #INM_ERROR_OPERATION_FAILED   Operation failed
 * @see inm_cellular_state_changed_cb()
 * @see inm_set_cellular_state_changed_cb()
 */
int inm_unset_cellular_state_changed_cb(inm_h inm);

/**
 * @brief Gets the state of the cellular connection.
 * @details The returned state is for the cellular connection state.
 * @since_tizen 5.0
 * @param[in] inm     The inm handle
 * @param[out] state  The state of the cellular connection
 * @return @c 0 on success,
 *         otherwise a negative error value
 * @retval #INM_ERROR_NONE               Successful
 * @retval #INM_ERROR_INVALID_PARAMETER  Invalid parameter
 * @retval #INM_ERROR_NOT_SUPPORTED      Not supported
 * @retval #INM_ERROR_NOT_INITIALIZED    Not initialized
 * @retval #INM_ERROR_OPERATION_FAILED   Operation failed
 */
int inm_get_cellular_state(inm_h inm, inm_cellular_state_e* state);

/**
 * @brief Called when the Wi-Fi state is changed.
 * @since_tizen 5.0
 * @param[in] state      The Wi-Fi state
 * @param[in] user_data  The user data passed from the callback registration function
 * @see inm_set_wifi_state_changed_cb()
 * @see inm_unset_wifi_state_changed_cb()
 */
typedef void(*inm_wifi_state_changed_cb)(inm_wifi_state_e state,
		void *user_data);

/**
 * @brief Sets a callback called when the Wi-Fi state is changed.
 * @since_tizen 5.0
 * @param[in] inm        The monitor handle
 * @param[in] callback   The callback function to be called
 * @param[in] user_data  The user data passed to the callback function
 * @return @c 0 on success, otherwise negative error value
 * @retval #INM_ERROR_NONE               Successful
 * @retval #INM_ERROR_INVALID_PARAMETER  Invalid parameter
 * @retval #INM_ERROR_NOT_SUPPORTED      Not supported
 * @retval #INM_ERROR_NOT_INITIALIZED    Not initialized
 * @retval #INM_ERROR_OPERATION_FAILED   Operation failed
 * @see inm_wifi_state_changed_cb()
 * @see inm_unset_wifi_state_changed_cb()
 */
int inm_set_wifi_state_changed_cb(inm_h inm,
		inm_wifi_state_changed_cb callback, void *user_data);

/**
 * @brief Unsets the callback called when the Wi-Fi state is changed.
 * @since_tizen 5.0
 * @param[in] inm  The monitor handle
 * @return @c 0 on success, otherwise negative error value
 * @retval #INM_ERROR_NONE               Successful
 * @retval #INM_ERROR_INVALID_PARAMETER  Invalid parameter
 * @retval #INM_ERROR_NOT_SUPPORTED      Not supported
 * @retval #INM_ERROR_NOT_INITIALIZED    Not initialized
 * @retval #INM_ERROR_OPERATION_FAILED   Operation failed
 * @see inm_wifi_state_changed_cb()
 * @see inm_set_wifi_state_changed_cb()
 */
int inm_unset_wifi_state_changed_cb(inm_h inm);

/**
 * @brief Gets the state of the Wi-Fi.
 * @details The returned state is for the Wi-Fi connection state.
 * @since_tizen 5.0
 * @privlevel public
 * @privilege %http://tizen.org/privilege/network.get
 * @param[in] inm     The inm handle
 * @param[out] state  The state of Wi-Fi connection
 * @return @c 0 on success,
 *         otherwise a negative error value
 * @retval #INM_ERROR_NONE               Successful
 * @retval #INM_ERROR_PERMISSION_DENIED  Permission Denied
 * @retval #INM_ERROR_INVALID_PARAMETER  Invalid parameter
 * @retval #INM_ERROR_NOT_SUPPORTED      Not supported
 * @retval #INM_ERROR_NOT_INITIALIZED    Not initialized
 * @retval #INM_ERROR_OPERATION_FAILED   Operation failed
 */
int inm_get_wifi_state(inm_h inm, inm_wifi_state_e* state);

/**
 * @brief Called when the ethernet state is changed.
 * @since_tizen 5.0
 * @param[in] state      The ethernet state
 * @param[in] user_data  The user data passed from the callback registration function
 * @see inm_set_ethernet_state_changed_cb()
 * @see inm_unset_ethernet_state_changed_cb()
 */
typedef void(*inm_ethernet_state_changed_cb)(inm_ethernet_state_e state,
		void *user_data);

/**
 * @brief Sets a callback called when the ethernet state is changed.
 * @since_tizen 5.0
 * @param[in] inm        The monitor handle
 * @param[in] callback   The callback function to be called
 * @param[in] user_data  The user data passed to the callback function
 * @return @c 0 on success, otherwise negative error value
 * @retval #INM_ERROR_NONE               Successful
 * @retval #INM_ERROR_INVALID_PARAMETER  Invalid parameter
 * @retval #INM_ERROR_NOT_SUPPORTED      Not supported
 * @retval #INM_ERROR_NOT_INITIALIZED    Not initialized
 * @retval #INM_ERROR_OPERATION_FAILED   Operation failed
 * @see inm_ethernet_state_changed_cb()
 * @see inm_unset_ethernet_state_changed_cb()
 */
int inm_set_ethernet_state_changed_cb(inm_h inm,
		inm_ethernet_state_changed_cb callback, void *user_data);

/**
 * @brief Unsets the callback called when the ethernet state is changed.
 * @since_tizen 5.0
 * @param[in] inm  The monitor handle
 * @return @c 0 on success, otherwise negative error value
 * @retval #INM_ERROR_NONE               Successful
 * @retval #INM_ERROR_INVALID_PARAMETER  Invalid parameter
 * @retval #INM_ERROR_NOT_SUPPORTED      Not supported
 * @retval #INM_ERROR_NOT_INITIALIZED    Not initialized
 * @retval #INM_ERROR_OPERATION_FAILED   Operation failed
 * @see inm_ethernet_state_changed_cb()
 * @see inm_set_ethernet_state_changed_cb()
 */
int inm_unset_ethernet_state_changed_cb(inm_h inm);

/**
 * @brief Gets the state of the ethernet.
 * @details The returned state is for the ethernet connection state.
 * @since_tizen 5.0
 * @privlevel public
 * @privilege %http://tizen.org/privilege/network.get
 * @param[in] inm     The inm handle
 * @param[out] state  The state of ethernet connection
 * @return @c 0 on success,
 *         otherwise a negative error value
 * @retval #INM_ERROR_NONE               Successful
 * @retval #INM_ERROR_PERMISSION_DENIED  Permission Denied
 * @retval #INM_ERROR_INVALID_PARAMETER  Invalid parameter
 * @retval #INM_ERROR_NOT_SUPPORTED      Not supported
 * @retval #INM_ERROR_NOT_INITIALIZED    Not initialized
 * @retval #INM_ERROR_OPERATION_FAILED   Operation failed
 */
int inm_get_ethernet_state(inm_h inm, inm_ethernet_state_e* state);

/**
 * @brief Gets the handle of the default connection.
 * @since_tizen 5.0
 * @privlevel public
 * @privilege %http://tizen.org/privilege/network.get
 * @remarks You must release @a connection using inm_connection_destroy().
 * @param[in] inm          The inm handle
 * @param[out] connection  The connection handle
 * @return @c 0 on success,
 *         otherwise a negative error value
 * @retval #INM_ERROR_NONE               Successful
 * @retval #INM_ERROR_OUT_OF_MEMORY      Out of memory
 * @retval #INM_ERROR_PERMISSION_DENIED  Permission Denied
 * @retval #INM_ERROR_INVALID_PARAMETER  Invalid parameter
 * @retval #INM_ERROR_NOT_SUPPORTED      Not supported
 * @retval #INM_ERROR_NOT_INITIALIZED    Not initialized
 * @retval #INM_ERROR_OPERATION_FAILED   Operation failed
 */
int inm_get_current_connection(inm_h inm, inm_connection_h* connection);

/**
 * @brief Destroys a connection handle.
 * @since_tizen 5.0
 * @param[in] connection  The connection handle
 * @return @c 0 on success,
 *         otherwise a negative error value
 * @retval #INM_ERROR_NONE               Successful
 * @retval #INM_ERROR_INVALID_PARAMETER  Invalid parameter
 * @retval #INM_ERROR_NOT_SUPPORTED      Not supported
 * @retval #INM_ERROR_NOT_INITIALIZED    Not initialized
 * @retval #INM_ERROR_OPERATION_FAILED   Operation failed
 * @see inm_get_current_connection()
 */
int inm_connection_destroy(inm_connection_h* connection);

/**
 * @brief Clones a connection handle.
 * @details This function creates a new handle representing the same underlying connection as the original one.
 *          The newly created handle can be used independently from the original one.
 *          However, remember to destroy both handles once they're no longer needed.
 * @since_tizen 5.0
 * @remarks You must release @a cloned connection using inm_connection_destroy() afterwards.
 * @param[out] cloned  The handle of the cloned connection
 * @param[in] origin   The handle of the origin connection
 * @return @c 0 on success,
 *         otherwise a negative error value
 * @retval #INM_ERROR_NONE               Successful
 * @retval #INM_ERROR_OUT_OF_MEMORY      Out of memory
 * @retval #INM_ERROR_INVALID_PARAMETER  Invalid parameter
 * @retval #INM_ERROR_NOT_SUPPORTED      Not supported
 * @retval #INM_ERROR_NOT_INITIALIZED    Not initialized
 * @retval #INM_ERROR_OPERATION_FAILED   Operation failed
 * @see inm_connection_destroy()
 */
int inm_connection_clone(inm_connection_h *cloned,
		inm_connection_h origin);

/**
 * @brief Gets a connections iterator.
 * @details This function returns an iterator object that can be used to iterate
 *          through all currently active network connections. Remember to free
 *          the returned iterator using inm_destroy_connection_iterator() when done.
 * @since_tizen 5.0
 * @privlevel public
 * @privilege %http://tizen.org/privilege/network.get
 * @remarks You must release @a connection_iterator using inm_destroy_connection_iterator().
 * @param[in] inm                   The inm handle
 * @param[out] connection_iterator  The iterator of connection
 * @return @c 0 on success,
 *         otherwise a negative error value
 * @retval #INM_ERROR_NONE               Successful
 * @retval #INM_ERROR_OUT_OF_MEMORY      Out of memory
 * @retval #INM_ERROR_PERMISSION_DENIED  Permission Denied
 * @retval #INM_ERROR_INVALID_PARAMETER  Invalid parameter
 * @retval #INM_ERROR_NOT_SUPPORTED      Not supported
 * @retval #INM_ERROR_NOT_INITIALIZED    Not initialized
 * @retval #INM_ERROR_OPERATION_FAILED   Operation failed
 */
int inm_get_connection_iterator(inm_h inm,
		inm_connection_iterator_h* connection_iterator);

/**
 * @brief Moves the connection iterator to the next position and gets a connection handle.
 * @since_tizen 5.0
 * @remarks After the iterator is created, its inner cursor is placed before the first element.
 *          The first call to inm_connection_iterator_next() gets the first element.
 *          You must release @a connection using inm_connection_destroy().
 * @param[in] connection_iterator  The connection iterator
 * @param[out] connection          The connection handle
 * @return @c 0 on success,
 *         otherwise a negative error value
 * @retval #INM_ERROR_NONE               Successful
 * @retval #INM_ERROR_INVALID_PARAMETER  Invalid parameter
 * @retval #INM_ERROR_NOT_SUPPORTED      Not supported
 * @retval #INM_ERROR_NOT_INITIALIZED    Not initialized
 * @retval #INM_ERROR_OPERATION_FAILED   Operation failed
 */
int inm_connection_iterator_next(inm_connection_iterator_h connection_iterator,
		inm_connection_h* connection);

/**
 * @brief Destroys a connections iterator.
 * @since_tizen 5.0
 * @param[in] connection_iterator  The connection iterator
 * @return @c 0 on success,
 *         otherwise a negative error value
 * @retval #INM_ERROR_NONE               Successful
 * @retval #INM_ERROR_INVALID_PARAMETER  Invalid parameter
 * @retval #INM_ERROR_NOT_SUPPORTED      Not supported
 * @retval #INM_ERROR_NOT_INITIALIZED    Not initialized
 */
int inm_destroy_connection_iterator(inm_connection_iterator_h connection_iterator);

/**
 * @brief Called with a link handle.
 * @since_tizen 5.5
 * @remarks  If @a link is needed outside the callback, a copy should be
 *           made. @a link will be freed automatically after the execution
 *           of this callback.
 * @param[in] link       The link handle
 * @param[in] user_data  The user data passed from foreach function
 * @return @c true to continue with the next iteration of the loop,
 *         @c false to break out of the loop
 * @see inm_foreach_link()
 * @see inm_link_clone()
 */
typedef bool (*inm_link_cb) (inm_link_h link, void *user_data);

/**
 * @brief Gets handles of the link.
 * @since_tizen 5.5
 * @privlevel public
 * @privilege %http://tizen.org/privilege/network.get
 * @param[in] inm        The inm handle
 * @param[in] callback   The callback function to invoke
 * @param[in] user_data  The user data to be passed to the callback function
 * @return @c 0 on success,
 *         otherwise a negative error value
 * @retval #INM_ERROR_NONE               Successful
 * @retval #INM_ERROR_OUT_OF_MEMORY      Out of memory
 * @retval #INM_ERROR_PERMISSION_DENIED  Permission Denied
 * @retval #INM_ERROR_INVALID_PARAMETER  Invalid parameter
 * @retval #INM_ERROR_NOT_SUPPORTED      Not supported
 * @retval #INM_ERROR_NOT_INITIALIZED    Not initialized
 * @retval #INM_ERROR_OPERATION_FAILED   Operation failed
 * @see inm_link_clone()
 */
int inm_foreach_link(inm_h inm,
		inm_link_cb callback, void *user_data);

/**
 * @brief Refreshes link information held by a handle.
 * @since_tizen 5.5
 * @privlevel public
 * @privilege %http://tizen.org/privilege/network.get
 * @param[in] inm  The inm handle
 * @return @c 0 on success,
 *         otherwise a negative error value
 * @retval #INM_ERROR_NONE               Successful
 * @retval #INM_ERROR_OUT_OF_MEMORY      Out of memory
 * @retval #INM_ERROR_PERMISSION_DENIED  Permission Denied
 * @retval #INM_ERROR_INVALID_PARAMETER  Invalid parameter
 * @retval #INM_ERROR_NOT_SUPPORTED      Not supported
 * @retval #INM_ERROR_NOT_INITIALIZED    Not initialized
 * @retval #INM_ERROR_OPERATION_FAILED   Operation failed
 * @see inm_foreach_link()
 */
int inm_refresh_links(inm_h inm);

/**
 * @brief Sets the ARP packet interval which is used to find out the IP address of the device connected to the local network.
 * @since_tizen 5.5
 * @privlevel public
 * @privilege %http://tizen.org/privilege/network.get
 * @param[in] inm  The monitoring handle
 * @param[in] seconds  The ARP packet interval value in seconds
 * @return @c 0 on success,
 *         otherwise a negative error value
 * @retval #INM_ERROR_NONE               Successful
 * @retval #INM_ERROR_PERMISSION_DENIED  Permission Denied
 * @retval #INM_ERROR_INVALID_PARAMETER  Invalid parameter
 * @retval #INM_ERROR_NOT_SUPPORTED      Not supported
 * @retval #INM_ERROR_NOT_INITIALIZED    Not initialized
 * @retval #INM_ERROR_OPERATION_FAILED   Operation failed
 * @retval #INM_ERROR_NOW_IN_PROGRESS    Now in progress
 * @see inm_arp_request_start()
 * @see inm_arp_request_stop()
 * @see inm_default_gateway_start_checking()
 * @see inm_default_gateway_stop_checking()
 */
int inm_arp_request_set_packet_interval(inm_h inm, int seconds);

/**
 * @brief Gets the ARP packet interval which is used to find out the IP address of the device connected to the local network.
 * @since_tizen 5.5
 * @privlevel public
 * @privilege %http://tizen.org/privilege/network.get
 * @param[in] inm  The monitoring handle
 * @param[out] seconds  The ARP packet interval value in seconds
 * @return @c 0 on success,
 *         otherwise a negative error value
 * @retval #INM_ERROR_NONE               Successful
 * @retval #INM_ERROR_PERMISSION_DENIED  Permission Denied
 * @retval #INM_ERROR_INVALID_PARAMETER  Invalid parameter
 * @retval #INM_ERROR_NOT_SUPPORTED      Not supported
 * @retval #INM_ERROR_NOT_INITIALIZED    Not initialized
 * @retval #INM_ERROR_OPERATION_FAILED   Operation failed
 * @see inm_arp_request_start()
 * @see inm_arp_request_stop()
 * @see inm_default_gateway_start_checking()
 * @see inm_default_gateway_stop_checking()
 */
int inm_arp_request_get_packet_interval(inm_h inm, int *seconds);

/**
 * @brief Called when the IP is found in the local network.
 * @since_tizen 5.5
 * @remarks @a ip should not be freed. @a ip is available only in the callback.
 *          To use outside the callback, make a copy.
 * @param[in] found      @c true if IP is found, @c false if ip IP not found
 * @param[in] ip         The IP address found on a local network
 * @param[in] user_data  The user data passed from the callback registration function
 * @see inm_arp_request_start()
 * @see inm_arp_request_stop()
 */
typedef void (*inm_arp_request_ip_found_cb) (bool found, const char *ip, void *user_data);

/**
 * @brief Starts to send ARP packets to find a @a target_ip.
 * @details User should set callback to get results by inm_arp_request_ip_found_cb()
 * @since_tizen 5.5
 * @privlevel public
 * @privilege %http://tizen.org/privilege/network.get
 * @param[in] inm        The monitoring handle
 * @param[in] target_ip  IP address to find
 * @param[in] callback   The callback function to be called
 * @param[in] user_data  The user data passed to the callback function
 * @return 0 on success, otherwise negative error value
 * @retval #INM_ERROR_NONE               Successful
 * @retval #INM_ERROR_PERMISSION_DENIED  Permission Denied
 * @retval #INM_ERROR_INVALID_PARAMETER  Invalid parameter
 * @retval #INM_ERROR_NOT_SUPPORTED      Not supported
 * @retval #INM_ERROR_NOT_INITIALIZED    Not initialized
 * @retval #INM_ERROR_OPERATION_FAILED   Operation failed
 * @retval #INM_ERROR_NOW_IN_PROGRESS    Now in progress
 */
int inm_arp_request_start(inm_h inm, const char *target_ip,
		inm_arp_request_ip_found_cb callback, void *user_data);

/**
 * @brief Stops sending ARP packets.
 * @since_tizen 5.5
 * @privlevel public
 * @privilege %http://tizen.org/privilege/network.get
 * @param[in] inm        The monitoring handle
 * @param[in] target_ip  IP address to stop finding
 * @return 0 on success, otherwise negative error value
 * @retval #INM_ERROR_NONE               Successful
 * @retval #INM_ERROR_PERMISSION_DENIED  Permission Denied
 * @retval #INM_ERROR_INVALID_PARAMETER  Invalid parameter
 * @retval #INM_ERROR_NOT_SUPPORTED      Not supported
 * @retval #INM_ERROR_NOT_INITIALIZED    Not initialized
 * @retval #INM_ERROR_OPERATION_FAILED   Operation failed
 * @retval #INM_ERROR_NOW_IN_PROGRESS    Now in progress
 */
int inm_arp_request_stop(inm_h inm, const char *target_ip);

/**
 * @brief Called when the gateway IP is found in a local network
 *        by sending ARP packets.
 * @since_tizen 5.5
 * @remarks @a gateway_ip should not be freed. @a gateway_ip is available
 *          only in the callback. To use outside the callback, make a copy.
 * @param[in] found      @c true if gateway IP address is found, @c false
 *                       if gateway IP address isn't found after timeout
 * @param[in] gateway_ip The gateway IP address found on a local network
 * @param[in] user_data  The user data passed from the callback registration function
 * @see inm_default_gateway_start_checking()
 * @see inm_default_gateway_stop_checking()
 */
typedef void (*inm_default_gateway_found_cb) (bool found, const char *gateway_ip, void *user_data);

/**
 * @brief Starts to send ARP packets to find a gateway during @a timeout, in seconds.
 * @since_tizen 5.5
 * @privlevel public
 * @privilege %http://tizen.org/privilege/network.get
 * @param[in] inm        The monitoring handle
 * @param[in] timeout    Specifies the duration of discovery period,
 *                       in seconds. If @c 0, then there is no limit
 *                       on how long the discovery takes.
 * @param[in] callback   The callback function to be called
 * @param[in] user_data  The user data passed to the callback function
 * @return 0 on success, otherwise negative error value
 * @retval #INM_ERROR_NONE               Successful
 * @retval #INM_ERROR_PERMISSION_DENIED  Permission Denied
 * @retval #INM_ERROR_INVALID_PARAMETER  Invalid parameter
 * @retval #INM_ERROR_NOT_SUPPORTED      Not supported
 * @retval #INM_ERROR_NOT_INITIALIZED    Not initialized
 * @retval #INM_ERROR_OPERATION_FAILED   Operation failed
 * @retval #INM_ERROR_NOW_IN_PROGRESS    Now in progress
 */
int inm_default_gateway_start_checking(inm_h inm, int timeout,
		inm_default_gateway_found_cb callback,
		void *user_data);

/**
 * @brief Stops sending ARP packets to check default gateway.
 * @since_tizen 5.5
 * @privlevel public
 * @privilege %http://tizen.org/privilege/network.get
 * @param[in] inm        The monitoring handle
 * @return 0 on success, otherwise negative error value
 * @retval #INM_ERROR_NONE               Successful
 * @retval #INM_ERROR_PERMISSION_DENIED  Permission Denied
 * @retval #INM_ERROR_INVALID_PARAMETER  Invalid parameter
 * @retval #INM_ERROR_NOT_SUPPORTED      Not supported
 * @retval #INM_ERROR_NOT_INITIALIZED    Not initialized
 * @retval #INM_ERROR_OPERATION_FAILED   Operation failed
 * @retval #INM_ERROR_NOW_IN_PROGRESS    Now in progress
 */
int inm_default_gateway_stop_checking(inm_h inm);

/**
 * @brief Called when the DNS lookup result is received.
 * @since_tizen 5.5
 * @param[in] found      @c true if DNS lookup succeeded, @c false
 *                       if DNS lookup failed
 * @param[in] user_data  The user data passed from the callback registration function
 * @see inm_default_dns_lookup_check()
 */
typedef void (*inm_default_dns_lookup_result_cb) (bool found, void *user_data);

/**
 * @brief Checks DNS lookup of the target to check whether DNS server is valid or not.
 * @since_tizen 5.5
 * @privlevel public
 * @privilege %http://tizen.org/privilege/network.get
 * @param[in] inm        The monitoring handle
 * @param[in] callback   The callback function to be called
 * @param[in] user_data  The user data passed to the callback function
 * @return 0 on success, otherwise negative error value
 * @retval #INM_ERROR_NONE               Successful
 * @retval #INM_ERROR_PERMISSION_DENIED  Permission Denied
 * @retval #INM_ERROR_INVALID_PARAMETER  Invalid parameter
 * @retval #INM_ERROR_NOT_SUPPORTED      Not supported
 * @retval #INM_ERROR_NOT_INITIALIZED    Not initialized
 * @retval #INM_ERROR_OPERATION_FAILED   Operation failed
 * @retval #INM_ERROR_NOW_IN_PROGRESS    Now in progress
 */
int inm_default_dns_lookup_check(inm_h inm,
		inm_default_dns_lookup_result_cb callback,
		void *user_data);

/**
 * @brief Adds URL to check reachability by inm_reachable_urls_start_checking()
 *        with type #INM_URL_LIST_TYPE_USER.
 * @details This function adds the provided url to the list of URLs being checked for connectivity.
 *          The checking operation itself needs to be started separately via inm_reachable_urls_start_checking()
 * @since_tizen 5.5
 * @privlevel public
 * @privilege %http://tizen.org/privilege/network.get
 * @param[in] inm  The monitoring handle
 * @param[in] url  The URL to check reachability
 * @return @c 0 on success,
 *         otherwise a negative error value
 * @retval #INM_ERROR_NONE               Successful
 * @retval #INM_ERROR_PERMISSION_DENIED  Permission Denied
 * @retval #INM_ERROR_INVALID_PARAMETER  Invalid parameter
 * @retval #INM_ERROR_NOT_SUPPORTED      Not supported
 * @retval #INM_ERROR_NOT_INITIALIZED    Not initialized
 * @retval #INM_ERROR_OPERATION_FAILED   Operation failed
 * @see inm_reachable_urls_start_checking()
 * @see inm_reachable_urls_stop_checking()
 */
int inm_reachable_urls_add_url_to_check(inm_h inm, const char *url);

/**
 * @brief Removes URL to check reachability by inm_reachable_urls_start_checking()
 *        with type #INM_URL_LIST_TYPE_USER.
 * @since_tizen 5.5
 * @privlevel public
 * @privilege %http://tizen.org/privilege/network.get
 * @param[in] inm  The monitoring handle
 * @param[in] url  The URL to check reachability
 * @return @c 0 on success,
 *         otherwise a negative error value
 * @retval #INM_ERROR_NONE               Successful
 * @retval #INM_ERROR_PERMISSION_DENIED  Permission Denied
 * @retval #INM_ERROR_INVALID_PARAMETER  Invalid parameter
 * @retval #INM_ERROR_NOT_SUPPORTED      Not supported
 * @retval #INM_ERROR_NOT_INITIALIZED    Not initialized
 * @retval #INM_ERROR_OPERATION_FAILED   Operation failed
 * @see inm_reachable_urls_start_checking()
 * @see inm_reachable_urls_stop_checking()
 */
int inm_reachable_urls_remove_url_to_check(inm_h inm, const char *url);

/**
 * @brief Called when the URL responds to HTTP GET generated by curl.
 * @since_tizen 5.5
 * @remarks @a url should not be freed. @a url is available only in the callback.
 *          To use outside the callback, make a copy.
 * @param[in] result     The enumeration value to describe the result.
 * @param[in] url        The URL responses to HTTP GET generated by curl
 * @param[in] user_data  The user data passed from the callback registration function
 * @see inm_reachable_urls_start_checking()
 * @see inm_reachable_urls_stop_checking()
 * @see inm_reachable_urls_is_check_running()
 */
typedef void (*inm_reachable_urls_check_result_cb) (
		inm_reachable_urls_check_result_e result,
		const char *url,
		void *user_data);

/**
 * @brief Starts to send HTTP GET requests with curl to check reachability of URLs of the given type.
 * @details This function just checks if URLs in the list can be reachable once.
 *          If you want to check again, you need to call this function again
 *          with the target URL list.
 * @since_tizen 5.5
 * @privlevel public
 * @privilege %http://tizen.org/privilege/network.get
 * @param[in] inm        The monitoring handle
 * @param[in] type       URL type to check. For #INM_URL_LIST_TYPE_DEFAULT,
 *                       the platform checks the reachability of "tizen.org".
 *                       For #INM_URL_LIST_TYPE_USER, the platform checks
 *                       the reachability of URLs added by inm_add_url_to_check()
 * @param[in] callback   The callback function to be called
 * @param[in] user_data  The user data passed to the callback function
 * @return 0 on success, otherwise negative error value
 * @retval #INM_ERROR_NONE               Successful
 * @retval #INM_ERROR_PERMISSION_DENIED  Permission Denied
 * @retval #INM_ERROR_INVALID_PARAMETER  Invalid parameter
 * @retval #INM_ERROR_NOT_SUPPORTED      Not supported
 * @retval #INM_ERROR_NOT_INITIALIZED    Not initialized
 * @retval #INM_ERROR_OPERATION_FAILED   Operation failed
 * @retval #INM_ERROR_NOW_IN_PROGRESS    Now in progress
 */
int inm_reachable_urls_start_checking(inm_h inm,
		inm_url_list_type_e type,
		inm_reachable_urls_check_result_cb callback,
		void *user_data);

/**
 * @brief Stops sending HTTP GET requests with curl to check reachability of URLs of the given type.
 * @since_tizen 5.5
 * @privlevel public
 * @privilege %http://tizen.org/privilege/network.get
 * @param[in] inm        The monitoring handle
 * @param[in] type       The URL type to check
 * @return 0 on success, otherwise negative error value
 * @retval #INM_ERROR_NONE               Successful
 * @retval #INM_ERROR_PERMISSION_DENIED  Permission Denied
 * @retval #INM_ERROR_INVALID_PARAMETER  Invalid parameter
 * @retval #INM_ERROR_NOT_SUPPORTED      Not supported
 * @retval #INM_ERROR_NOT_INITIALIZED    Not initialized
 * @retval #INM_ERROR_OPERATION_FAILED   Operation failed
 */
int inm_reachable_urls_stop_checking(inm_h inm, inm_url_list_type_e type);

/**
 * @brief Gets whether the reachable URLs check is running or not.
 * @since_tizen 5.5
 * @privlevel public
 * @privilege %http://tizen.org/privilege/network.get
 * @param[in] inm         The monitoring handle
 * @param[in] type        URL type to check
 * @param[out] is_running  checking URL is running or not
 * @return 0 on success, otherwise negative error value
 * @retval #INM_ERROR_NONE               Successful
 * @retval #INM_ERROR_PERMISSION_DENIED  Permission Denied
 * @retval #INM_ERROR_INVALID_PARAMETER  Invalid parameter
 * @retval #INM_ERROR_NOT_SUPPORTED      Not supported
 * @retval #INM_ERROR_NOT_INITIALIZED    Not initialized
 * @retval #INM_ERROR_OPERATION_FAILED   Operation failed
 */
int inm_reachable_urls_is_check_running(inm_h inm, inm_url_list_type_e type, bool *is_running);

/**
 * @}
 */

/**
 * @addtogroup CAPI_NETWORK_INM_CONNECTION_MODULE
 * @{
 */

/**
 * @brief Gets the connection ID.
 * @details Two different connections can have the same name.
 *          So, you must use this function instead of inm_connection_get_name()
 *          if you want to get the unique identification.
 * @since_tizen 5.0
 * @remarks You must release @a id using free().
 * @param[in] connection  The connection handle
 * @param[out] id         The ID of the connection
 * @return @c 0 on success,
 *         otherwise a negative error value
 * @retval #INM_ERROR_NONE               Successful
 * @retval #INM_ERROR_OUT_OF_MEMORY      Out of memory
 * @retval #INM_ERROR_INVALID_PARAMETER  Invalid parameter
 * @retval #INM_ERROR_NOT_SUPPORTED      Not supported
 * @retval #INM_ERROR_NOT_INITIALIZED    Not initialized
 * @retval #INM_ERROR_OPERATION_FAILED   Operation failed
 * @see inm_connection_get_name()
 */
int inm_connection_get_id(inm_connection_h connection, char **id);

/**
 * @brief Gets the name associated with the given network connection.
 * @since_tizen 5.0
 * @remarks You must release @a name using free().
 * @param[in] connection  The connection handle
 * @param[out] name       The name of the connection
 * @return @c 0 on success,
 *         otherwise a negative error value
 * @retval #INM_ERROR_NONE               Successful
 * @retval #INM_ERROR_OUT_OF_MEMORY      Out of memory
 * @retval #INM_ERROR_INVALID_PARAMETER  Invalid parameter
 * @retval #INM_ERROR_NOT_SUPPORTED      Not supported
 * @retval #INM_ERROR_NOT_INITIALIZED    Not initialized
 * @retval #INM_ERROR_OPERATION_FAILED   Operation failed
 * @see inm_connection_get_id()
 */
int inm_connection_get_name(inm_connection_h connection, char **name);

/**
 * @brief Gets the network type associated with the given network connection.
 * @since_tizen 5.0
 * @param[in] connection  The connection handle
 * @param[out] type       The type of the connection
 * @return @c 0 on success,
 *         otherwise a negative error value
 * @retval #INM_ERROR_NONE               Successful
 * @retval #INM_ERROR_INVALID_PARAMETER  Invalid parameter
 * @retval #INM_ERROR_NOT_SUPPORTED      Not supported
 * @retval #INM_ERROR_NOT_INITIALIZED    Not initialized
 * @retval #INM_ERROR_OPERATION_FAILED   Operation failed
 */
int inm_connection_get_type(inm_connection_h connection, inm_connection_type_e *type);

/**
 * @brief Gets the name of the network interface associated with the given network connection, e.g. eth0 and pdp0.
 * @since_tizen 5.0
 * @remarks You must release @a interface_name using free().
 * @param[in] connection       The connection handle
 * @param[out] interface_name  The name of the network interface
 * @return @c 0 on success,
 *         otherwise a negative error value
 * @retval #INM_ERROR_NONE               Successful
 * @retval #INM_ERROR_OUT_OF_MEMORY      Out of memory
 * @retval #INM_ERROR_INVALID_PARAMETER  Invalid parameter
 * @retval #INM_ERROR_NOT_SUPPORTED      Not supported
 * @retval #INM_ERROR_NOT_INITIALIZED    Not initialized
 * @retval #INM_ERROR_OPERATION_FAILED   Operation failed
 */
int inm_connection_get_network_interface_name(inm_connection_h connection,
		char **interface_name);

/**
 * @brief Refreshes the connection information.
 * @since_tizen 5.0
 * @privlevel public
 * @privilege %http://tizen.org/privilege/network.get
 * @remarks You should call this function in order to get the current information
 *          because the connection information can be changed.
 * @param[in] connection  The connection handle
 * @return @c 0 on success,
 *         otherwise a negative error value
 * @retval #INM_ERROR_NONE               Successful
 * @retval #INM_ERROR_OUT_OF_MEMORY      Out of memory
 * @retval #INM_ERROR_PERMISSION_DENIED  Permission Denied
 * @retval #INM_ERROR_INVALID_PARAMETER  Invalid parameter
 * @retval #INM_ERROR_NOT_SUPPORTED      Not supported
 * @retval #INM_ERROR_NOT_INITIALIZED    Not initialized
 * @retval #INM_ERROR_OPERATION_FAILED   Operation failed
 */
int inm_connection_refresh(inm_connection_h connection);

/**
 * @brief Gets the network state associated with the given network connection.
 * @since_tizen 5.0
 * @param[in] connection  The connection handle
 * @param[out] state      The state of the connection
 * @return @c 0 on success,
 *         otherwise a negative error value
 * @retval #INM_ERROR_NONE               Successful
 * @retval #INM_ERROR_INVALID_PARAMETER  Invalid parameter
 * @retval #INM_ERROR_NOT_SUPPORTED      Not supported
 * @retval #INM_ERROR_NOT_INITIALIZED    Not initialized
 * @retval #INM_ERROR_OPERATION_FAILED   Operation failed
 */
int inm_connection_get_state(inm_connection_h connection,
		inm_connection_state_e *state);

/**
 * @brief Gets the IPv6 network state associated with the given network connection.
 * @since_tizen 5.0
 * @param[in] connection  The connection handle
 * @param[out] state      The connection state
 * @return 0 on success, otherwise negative error value
 * @retval #INM_ERROR_NONE               Successful
 * @retval #INM_ERROR_INVALID_PARAMETER  Invalid parameter
 * @retval #INM_ERROR_NOT_SUPPORTED      Not supported
 * @retval #INM_ERROR_NOT_INITIALIZED    Not initialized
 * @retval #INM_ERROR_OPERATION_FAILED   Operation failed
 */
int inm_connection_get_ipv6_state(inm_connection_h connection,
		inm_connection_state_e *state);

/**
 * @brief Gets the IP config type associated with the given network connection and address family.
 * @since_tizen 5.0
 * @param[in] connection      The connection handle
 * @param[in] address_family  The address family
 * @param[out] type           The type of the IP config
 * @return @c 0 on success,
 *         otherwise a negative error value
 * @retval #INM_ERROR_NONE               Successful
 * @retval #INM_ERROR_INVALID_PARAMETER  Invalid parameter
 * @retval #INM_ERROR_NOT_SUPPORTED      Not supported
 * @retval #INM_ERROR_NOT_INITIALIZED    Not initialized
 * @retval #INM_ERROR_OPERATION_FAILED   Operation failed
 */
int inm_connection_get_ip_config_type(inm_connection_h connection,
		inm_address_family_e address_family, inm_ip_config_type_e *type);

/**
 * @brief Gets the IP address associated with the given network connection and address family.
 * @since_tizen 5.0
 * @remarks You must release @a ip_address using free().
 * @param[in] connection      The connection handle
 * @param[in] address_family  The address family
 * @param[out] ip_address     The IP address
 * @return @c 0 on success,
 *         otherwise a negative error value
 * @retval #INM_ERROR_NONE               Successful
 * @retval #INM_ERROR_OUT_OF_MEMORY      Out of memory
 * @retval #INM_ERROR_INVALID_PARAMETER  Invalid parameter
 * @retval #INM_ERROR_NOT_SUPPORTED      Not supported
 * @retval #INM_ERROR_NOT_INITIALIZED    Not initialized
 * @retval #INM_ERROR_OPERATION_FAILED   Operation failed
 */
int inm_connection_get_ip_address(inm_connection_h connection,
		inm_address_family_e address_family, char **ip_address);

/**
 * @brief Gets the Subnet Mask associated with the given network connection and address family..
 * @since_tizen 5.0
 * @remarks You must release @a subnet_mask using free().
 *          This function is supported only for IPv4 address family.
 * @param[in] connection           The connection handle
 * @param[in] address_family  The address family
 * @param[out] subnet_mask    The subnet mask
 * @return @c 0 on success,
 *         otherwise a negative error value
 * @retval #INM_ERROR_NONE               Successful
 * @retval #INM_ERROR_OUT_OF_MEMORY      Out of memory
 * @retval #INM_ERROR_INVALID_PARAMETER  Invalid parameter
 * @retval #INM_ERROR_NOT_SUPPORTED      Not supported
 * @retval #INM_ERROR_NOT_INITIALIZED    Not initialized
 * @retval #INM_ERROR_OPERATION_FAILED   Operation failed
 */
int inm_connection_get_subnet_mask(inm_connection_h connection,
		inm_address_family_e address_family, char **subnet_mask);

/**
 * @brief Gets the Gateway address associated with the given network connection and address family..
 * @since_tizen 5.0
 * @remarks You must release @a gateway_address using free().
 * @param[in] connection           The connection handle
 * @param[in] address_family    The address family
 * @param[out] gateway_address  The gateway address
 * @return @c 0 on success,
 *         otherwise a negative error value
 * @retval #INM_ERROR_NONE               Successful
 * @retval #INM_ERROR_OUT_OF_MEMORY      Out of memory
 * @retval #INM_ERROR_INVALID_PARAMETER  Invalid parameter
 * @retval #INM_ERROR_NOT_SUPPORTED      Not supported
 * @retval #INM_ERROR_NOT_INITIALIZED    Not initialized
 * @retval #INM_ERROR_OPERATION_FAILED   Operation failed
 */
int inm_connection_get_gateway_address(inm_connection_h connection,
		inm_address_family_e address_family, char **gateway_address);

/**
 * @brief Gets the DHCP Server address associated with the given network connection and address family..
 * @since_tizen 5.0
 * @remarks You must release @a dhcp_server using free().
 *          This function is supported only for IPv4 address family.
 * @param[in] connection      The connection handle
 * @param[in] address_family  The address family
 * @param[out] dhcp_server    The DHCP Server address
 * @return @c 0 on success, otherwise a negative error value
 * @retval #INM_ERROR_NONE               Successful
 * @retval #INM_ERROR_OUT_OF_MEMORY      Out of memory
 * @retval #INM_ERROR_INVALID_PARAMETER  Invalid parameter
 * @retval #INM_ERROR_NOT_SUPPORTED      Not supported
 * @retval #INM_ERROR_NOT_INITIALIZED    Not initialized
 * @retval #INM_ERROR_OPERATION_FAILED   Operation failed
 */
int inm_connection_get_dhcp_server_address(inm_connection_h connection,
		inm_address_family_e address_family, char **dhcp_server);

/**
 * @brief Gets the DHCP lease duration associated with the given network connection and address family..
 * @since_tizen 5.0
 * @remarks This function is supported only for IPv4 address family.
 * @param[in] connection            The connection handle
 * @param[in] address_family        The address family
 * @param[out] dhcp_lease_duration  The DHCP lease duration in seconds
 * @return @c 0 on success, otherwise a negative error value
 * @retval #INM_ERROR_NONE               Successful
 * @retval #INM_ERROR_INVALID_PARAMETER  Invalid parameter
 * @retval #INM_ERROR_NOT_SUPPORTED      Not supported
 * @retval #INM_ERROR_NOT_INITIALIZED    Not initialized
 * @retval #INM_ERROR_OPERATION_FAILED   Operation failed
 */
int inm_connection_get_dhcp_lease_duration(inm_connection_h connection,
		inm_address_family_e address_family, int *dhcp_lease_duration);

/**
 * @brief Gets the DNS address associated with the given network connection and address family..
 * @since_tizen 5.0
 * @remarks The allowance of the DNS address is @c 2. You must release @a dns_address using free().
 * @param[in] connection      The connection handle
 * @param[in] order           The order of DNS address \n
 *            It starts from 1, which means first DNS address
 * @param[in] address_family  The address family
 * @param[out] dns_address    The DNS address
 * @return @c 0 on success,
 *         otherwise a negative error value
 * @retval #INM_ERROR_NONE               Successful
 * @retval #INM_ERROR_OUT_OF_MEMORY      Out of memory
 * @retval #INM_ERROR_INVALID_PARAMETER  Invalid parameter
 * @retval #INM_ERROR_NOT_SUPPORTED      Not supported
 * @retval #INM_ERROR_NOT_INITIALIZED    Not initialized
 * @retval #INM_ERROR_OPERATION_FAILED   Operation failed
 */
int inm_connection_get_dns_address(inm_connection_h connection,
		int order, inm_address_family_e address_family, char **dns_address);

/**
 * @brief Gets the Proxy type associated with the given network connection.
 * @since_tizen 5.0
 * @param[in] connection  The connection handle
 * @param[out] type       The type of the proxy
 * @return @c 0 on success,
 *         otherwise a negative error value
 * @retval #INM_ERROR_NONE               Successful
 * @retval #INM_ERROR_INVALID_PARAMETER  Invalid parameter
 * @retval #INM_ERROR_NOT_SUPPORTED      Not supported
 * @retval #INM_ERROR_NOT_INITIALIZED    Not initialized
 * @retval #INM_ERROR_OPERATION_FAILED   Operation failed
 */
int inm_connection_get_proxy_type(inm_connection_h connection,
		inm_proxy_type_e *type);

/**
 * @brief Gets the Proxy address associated with the given network connection and address family.
 * @since_tizen 5.0
 * @remarks You must release @a proxy_address using free().
 * @param[in] connection      The connection handle
 * @param[in] address_family  The address family
 * @param[out] proxy_address  The proxy address
 * @return @c 0 on success,
 *         otherwise a negative error value
 * @retval #INM_ERROR_NONE               Successful
 * @retval #INM_ERROR_OUT_OF_MEMORY      Out of memory
 * @retval #INM_ERROR_INVALID_PARAMETER  Invalid parameter
 * @retval #INM_ERROR_NOT_SUPPORTED      Not supported
 * @retval #INM_ERROR_NOT_INITIALIZED    Not initialized
 * @retval #INM_ERROR_OPERATION_FAILED   Operation failed
 */
int inm_connection_get_proxy_address(inm_connection_h connection,
		inm_address_family_e address_family, char **proxy_address);

/**
 * @brief Gets the DNS config type associated with the given network connection and address family.
 * @since_tizen 5.0
 * @param[in] connection      The connection handle
 * @param[in] address_family  The address family
 * @param[out] type           The DNS config type
 * @return @c 0 on success,
 *         otherwise a negative error value
 * @retval #INM_ERROR_NONE               Successful
 * @retval #INM_ERROR_INVALID_PARAMETER  Invalid parameter
 * @retval #INM_ERROR_NOT_SUPPORTED      Not supported
 * @retval #INM_ERROR_NOT_INITIALIZED    Not initialized
 * @retval #INM_ERROR_OPERATION_FAILED   Operation failed
 */
int inm_connection_get_dns_config_type(inm_connection_h connection,
		inm_address_family_e address_family, inm_dns_config_type_e *type);

/**
 * @brief Gets the network prefix length associated with the given network connection and address family.
 * @since_tizen 5.0
 * @param[in] connection      The connection handle
 * @param[in] address_family  The address family
 * @param[out] prefix_len     The network prefix length \n
 *                            In case of IPv4, it means netmask length
 *                            (also called a prefix, e.g. 8, 16, 24, 32)
 * @return @c 0 on success,
 *         otherwise a negative error value
 * @retval #INM_ERROR_NONE               Successful
 * @retval #INM_ERROR_INVALID_PARAMETER  Invalid parameter
 * @retval #INM_ERROR_NOT_SUPPORTED      Not supported
 * @retval #INM_ERROR_NOT_INITIALIZED    Not initialized
 * @retval #INM_ERROR_OPERATION_FAILED   Operation failed
 */
int inm_connection_get_prefix_length(inm_connection_h connection,
		inm_address_family_e address_family, int *prefix_len);

/**
 * @brief Called when the state of the connection is changed.
 * @since_tizen 5.0
 * @param[in] state      The state
 * @param[in] user_data  The user data passed from the callback registration function
 * @see inm_connection_set_state_changed_cb()
 * @see inm_connection_unset_state_changed_cb()
 */
typedef void(*connection_state_changed_cb)(inm_connection_state_e state,
		void *user_data);

/**
 * @brief Sets the callback that is called when the state of connection is changed.
 * @since_tizen 5.0
 * @param[in] connection  The connection handle
 * @param[in] callback    The callback function to be called
 * @param[in] user_data   The user data passed to the callback function
 * @return @c 0 on success,
 *         otherwise a negative error value
 * @retval #INM_ERROR_NONE               Successful
 * @retval #INM_ERROR_INVALID_PARAMETER  Invalid parameter
 * @retval #INM_ERROR_NOT_SUPPORTED      Not supported
 * @retval #INM_ERROR_NOT_INITIALIZED    Not initialized
 * @retval #INM_ERROR_OPERATION_FAILED   Operation failed
 * @post connection_opened_cb() is invoked when the state of connection is changed.
 * @see connection_state_changed_cb()
 * @see inm_connection_unset_state_changed_cb()
 */
int inm_connection_set_state_changed_cb(inm_connection_h connection,
		connection_state_changed_cb callback, void *user_data);

/**
 * @brief Unsets the callback that is called when the state of connection is changed.
 * @since_tizen 5.0
 * @param[in] connection  The connection handle
 * @return @c 0 on success,
 *         otherwise a negative error value
 * @retval #INM_ERROR_NONE               Successful
 * @retval #INM_ERROR_INVALID_PARAMETER  Invalid parameter
 * @retval #INM_ERROR_NOT_SUPPORTED      Not supported
 * @retval #INM_ERROR_NOT_INITIALIZED    Not initialized
 * @retval #INM_ERROR_OPERATION_FAILED   Operation failed
 * @see connection_state_changed_cb()
 * @see inm_connection_set_state_changed_cb()
 */
int inm_connection_unset_state_changed_cb(inm_connection_h connection);

/**
 * @brief Gets the link handle for this connection.
 * @details This function retrieves the link handle associated with the given connection handle.
 *          The link handle represents the underlying network interface that is used for the connection.
 * @since_tizen 5.5
 * @remarks You must release @a link using inm_link_destroy().
 * @param[in] connection       The connection handle
 * @param[out] link            The link of the connection handle
 * @return @c 0 on success,
 *         otherwise a negative error value
 * @retval #INM_ERROR_NONE               Successful
 * @retval #INM_ERROR_OUT_OF_MEMORY      Out of memory
 * @retval #INM_ERROR_INVALID_PARAMETER  Invalid parameter
 * @retval #INM_ERROR_NOT_SUPPORTED      Not supported
 * @retval #INM_ERROR_NOT_INITIALIZED    Not initialized
 * @retval #INM_ERROR_OPERATION_FAILED   Operation failed
 * @see inm_link_clone()
 */
int inm_connection_get_link(inm_connection_h connection,
		inm_link_h *link);

/**
 * @}
 */


/**
 * @addtogroup CAPI_NETWORK_INM_WIFI_MODULE
 * @{
 */

/**
 * @brief Called when the scanning state is changed.
 * @since_tizen 5.0
 * @param[in] state      The wifi scanning state
 * @param[in] user_data  The user data passed from the callback registration function
 * @see inm_wifi_set_scan_state_changed_cb()
 * @see inm_wifi_unset_scan_state_changed_cb()
 */
typedef void(*inm_wifi_scan_state_changed_cb)(inm_wifi_scan_state_e state, void *user_data);

/**
 * @brief Sets the callback called when the scanning state is changed.
 * @since_tizen 5.0
 * @param[in] inm        The inm handle
 * @param[in] callback   The callback function to be called
 * @param[in] user_data  The user data passed to the callback function
 * @return 0 on success, otherwise negative error value
 * @retval #INM_ERROR_NONE               Successful
 * @retval #INM_ERROR_INVALID_PARAMETER  Invalid parameter
 * @retval #INM_ERROR_NOT_SUPPORTED      Not supported
 * @retval #INM_ERROR_NOT_INITIALIZED    Not initialized
 * @retval #INM_ERROR_OPERATION_FAILED   Operation failed
 */
int inm_wifi_set_scan_state_changed_cb(inm_h inm,
		inm_wifi_scan_state_changed_cb callback, void *user_data);

/**
 * @brief Unsets the callback called when the scanning state is changed.
 * @since_tizen 5.0
 * @param[in] inm  The inm handle
 * @return 0 on success, otherwise negative error value
 * @retval #INM_ERROR_NONE               Successful
 * @retval #INM_ERROR_INVALID_PARAMETER  Invalid parameter
 * @retval #INM_ERROR_NOT_SUPPORTED      Not supported
 * @retval #INM_ERROR_NOT_INITIALIZED    Not initialized
 * @retval #INM_ERROR_OPERATION_FAILED   Operation failed
 */
int inm_wifi_unset_scan_state_changed_cb(inm_h inm);

/**
 * @brief Gets the Wi-Fi scan state.
 * @since_tizen 5.0
 * @param[in] inm    The inm handle
 * @param[in] state  The Wi-Fi scan state
 * @return 0 on success, otherwise negative error value
 * @retval #INM_ERROR_NONE               Successful
 * @retval #INM_ERROR_INVALID_PARAMETER  Invalid parameter
 * @retval #INM_ERROR_NOT_SUPPORTED      Not supported
 * @retval #INM_ERROR_NOT_INITIALIZED    Not initialized
 * @retval #INM_ERROR_OPERATION_FAILED   Operation failed
 */
int inm_wifi_get_scan_state(inm_h inm, inm_wifi_scan_state_e *state);

/**
 * @brief Gets the handle of the connected access point.
 * @since_tizen 5.0
 * @privlevel public
 * @privilege %http://tizen.org/privilege/network.get
 * @remarks You must release @a ap using inm_connection_destroy().
 * @param[in] inm             The inm handle
 * @param[out] ap             The access point handle
 * @return 0 on success, otherwise negative error value
 * @retval #INM_ERROR_NONE               Successful
 * @retval #INM_ERROR_OUT_OF_MEMORY      Out of memory
 * @retval #INM_ERROR_PERMISSION_DENIED  Permission Denied
 * @retval #INM_ERROR_INVALID_PARAMETER  Invalid parameter
 * @retval #INM_ERROR_NOT_SUPPORTED      Not supported
 * @retval #INM_ERROR_NOT_INITIALIZED    Not initialized
 * @retval #INM_ERROR_OPERATION_FAILED   Operation failed
 */
int inm_wifi_get_connected_ap(inm_h inm, inm_connection_h *ap);

/**
 * @brief Called for each found access point.
 * @since_tizen 5.0
 * @remarks @a ap should not be freed. @a ap is valid only in this function.
 * In order to use @a ap outside this function, you must copy the ap with inm_connection_clone().
 * @param[in]  ap         The access point handle
 * @param[in]  user_data  The user data passed from the request function
 * @return  @c true to continue with the next iteration of the loop, \n
 *     otherwise @c false to break out of the loop
 * @pre  inm_wifi_foreach_found_ap() will invoke this callback.
 * @see  inm_wifi_foreach_found_ap()
 */
typedef bool (*inm_wifi_found_ap_cb)(inm_connection_h ap, void *user_data);

/**
 * @brief Gets the result of the scan.
 * @details This function retrieves the list of found access points (APs) after a WiFi scan.
 *          It calls the provided callback function for each found AP, passing the AP information and user data.
 * @since_tizen 5.0
 * @privlevel public
 * @privilege %http://tizen.org/privilege/network.get
 * @param[in] inm        The inm handle
 * @param[in] callback   The callback to be called
 * @param[in] user_data  The user data passed to the callback function
 * @return 0 on success, otherwise negative error value
 * @retval #INM_ERROR_NONE               Successful
 * @retval #INM_ERROR_OUT_OF_MEMORY      Out of memory
 * @retval #INM_ERROR_PERMISSION_DENIED  Permission Denied
 * @retval #INM_ERROR_INVALID_PARAMETER  Invalid parameter
 * @retval #INM_ERROR_NOT_SUPPORTED      Not supported
 * @retval #INM_ERROR_NOT_INITIALIZED    Not initialized
 * @retval #INM_ERROR_OPERATION_FAILED   Operation failed
 * @post This function invokes inm_wifi_found_ap_cb().
 */
int inm_wifi_foreach_found_ap(inm_h inm, inm_wifi_found_ap_cb callback, void *user_data);

/**
 * @}
 */


/**
 * @addtogroup CAPI_NETWORK_INM_WIFI_AP_MODULE
 * @{
 */

/**
 * @brief Gets ESSID (Extended Service Set Identifier).
 * @since_tizen 5.0
 * @remarks You must release @a essid using free().
 * @param[in] ap      The access point handle
 * @param[out] essid  The ESSID
 * @return 0 on success, otherwise negative error value
 * @retval #INM_ERROR_NONE               Successful
 * @retval #INM_ERROR_OUT_OF_MEMORY      Out of memory
 * @retval #INM_ERROR_INVALID_PARAMETER  Invalid parameter
 * @retval #INM_ERROR_NOT_SUPPORTED      Not supported
 * @retval #INM_ERROR_OPERATION_FAILED   Operation failed
 */
int inm_wifi_ap_get_essid(inm_connection_h ap, char **essid);

/**
 * @brief Gets raw SSID (Service Set Identifier).
 * @since_tizen 5.0
 * @remarks You must release @a ssid using free().
 * @param[in] ap         The access point handle
 * @param[out] ssid      The raw SSID bytes
 * @param[out] ssid_len  The raw SSID length
 * @return 0 on success, otherwise negative error value
 * @retval #INM_ERROR_NONE               Successful
 * @retval #INM_ERROR_OUT_OF_MEMORY      Out of memory
 * @retval #INM_ERROR_INVALID_PARAMETER  Invalid parameter
 * @retval #INM_ERROR_NOT_SUPPORTED      Not supported
 * @retval #INM_ERROR_OPERATION_FAILED   Operation failed
 */
int inm_wifi_ap_get_raw_ssid(inm_connection_h ap, char **ssid, int *ssid_len);

/**
 * @brief Gets BSSID (Basic Service Set Identifier).
 * @since_tizen 5.0
 * @remarks You must release @a bssid using free().
 * @param[in] ap      The access point handle
 * @param[out] bssid  The BSSID
 * @return 0 on success, otherwise negative error value
 * @retval #INM_ERROR_NONE               Successful
 * @retval #INM_ERROR_OUT_OF_MEMORY      Out of memory
 * @retval #INM_ERROR_INVALID_PARAMETER  Invalid parameter
 * @retval #INM_ERROR_NOT_SUPPORTED      Not supported
 * @retval #INM_ERROR_OPERATION_FAILED   Operation failed
 */
int inm_wifi_ap_get_bssid(inm_connection_h ap, char **bssid);

/**
 * @brief Gets the RSSI.
 * @since_tizen 5.0
 * @param[in] ap     The access point handle
 * @param[out] rssi  The RSSI value (in dBm)
 * @return 0 on success, otherwise negative error value
 * @retval #INM_ERROR_NONE               Successful
 * @retval #INM_ERROR_INVALID_PARAMETER  Invalid parameter
 * @retval #INM_ERROR_NOT_SUPPORTED      Not supported
 * @retval #INM_ERROR_OPERATION_FAILED   Operation failed
 */
int inm_wifi_ap_get_rssi(inm_connection_h ap, int *rssi);

/**
 * @brief Gets the RSSI level.
 * @since_tizen 5.0
 * @param[in] ap           The access point handle
 * @param[out] rssi_level  The RSSI level
 * @return 0 on success, otherwise negative error value
 * @retval #INM_ERROR_NONE               Successful
 * @retval #INM_ERROR_INVALID_PARAMETER  Invalid parameter
 * @retval #INM_ERROR_NOT_SUPPORTED      Not supported
 * @retval #INM_ERROR_OPERATION_FAILED   Operation failed
 */
int inm_wifi_ap_get_rssi_level(inm_connection_h ap, inm_wifi_rssi_level_e *rssi_level);

/**
 * @brief Gets the frequency band (MHz).
 * @since_tizen 5.0
 * @param[in] ap          The access point handle
 * @param[out] frequency  The frequency
 * @return 0 on success, otherwise negative error value
 * @retval #INM_ERROR_NONE               Successful
 * @retval #INM_ERROR_INVALID_PARAMETER  Invalid parameter
 * @retval #INM_ERROR_NOT_SUPPORTED      Not supported
 * @retval #INM_ERROR_OPERATION_FAILED   Operation failed
 */
int inm_wifi_ap_get_frequency(inm_connection_h ap, int *frequency);

/**
 * @brief Gets the max speed (Mbps).
 * @since_tizen 5.0
 * @param[in] ap          The access point handle
 * @param[out] max_speed  The max speed
 * @return 0 on success, otherwise negative error value
 * @retval #INM_ERROR_NONE               Successful
 * @retval #INM_ERROR_INVALID_PARAMETER  Invalid parameter
 * @retval #INM_ERROR_NOT_SUPPORTED      Not supported
 * @retval #INM_ERROR_OPERATION_FAILED   Operation failed
 */
int inm_wifi_ap_get_max_speed(inm_connection_h ap, int *max_speed);

/**
 * @brief Checks whether the access point is favorite or not.
 * @since_tizen 5.0
 * @param[in] ap         The access point handle
 * @param[out] favorite  @c true if access point is favorite,
 *             otherwise @c false if access point is not favorite
 * @return 0 on success, otherwise negative error value
 * @retval #INM_ERROR_NONE               Successful
 * @retval #INM_ERROR_INVALID_PARAMETER  Invalid parameter
 * @retval #INM_ERROR_NOT_SUPPORTED      Not supported
 * @retval #INM_ERROR_OPERATION_FAILED   Operation failed
 */
int inm_wifi_ap_is_favorite(inm_connection_h ap, bool *favorite);

/**
 * @brief Checks whether the access point is a Passpoint or not.
 * @since_tizen 5.0
 * @param[in] ap          The access point handle
 * @param[out] passpoint  @c true if access point is a Passpoint,
 *                        @c false if access point is not a Passpoint.
 * @return 0 on success, otherwise negative error value
 * @retval #INM_ERROR_NONE               Successful
 * @retval #INM_ERROR_INVALID_PARAMETER  Invalid parameter
 * @retval #INM_ERROR_NOT_SUPPORTED      Not supported
 * @retval #INM_ERROR_OPERATION_FAILED   Operation failed
 */
int inm_wifi_ap_is_passpoint(inm_connection_h ap, bool *passpoint);

/**
 * @brief Gets the Wi-Fi security mode.
 * @since_tizen 5.0
 * @param[in] ap     The access point handle
 * @param[out] type  The type of Wi-Fi security
 * @return 0 on success, otherwise negative error value
 * @retval #INM_ERROR_NONE               Successful
 * @retval #INM_ERROR_INVALID_PARAMETER  Invalid parameter
 * @retval #INM_ERROR_NOT_SUPPORTED      Not supported
 * @retval #INM_ERROR_OPERATION_FAILED   Operation failed
 */
int inm_wifi_ap_get_security_type(inm_connection_h ap, inm_wifi_security_type_e *type);

/**
 * @brief Gets the Wi-Fi encryption type.
 * @since_tizen 5.0
 * @param[in] ap     The access point handle
 * @param[out] type  The type of Wi-Fi encryption
 * @return 0 on success, otherwise negative error value
 * @retval #INM_ERROR_NONE               Successful
 * @retval #INM_ERROR_INVALID_PARAMETER  Invalid parameter
 * @retval #INM_ERROR_NOT_SUPPORTED      Not supported
 * @retval #INM_ERROR_OPERATION_FAILED   Operation failed
 */
int inm_wifi_ap_get_encryption_type(inm_connection_h ap, inm_wifi_encryption_type_e *type);

/**
 * @brief Checks whether the passphrase is required or not.
 * @since_tizen 5.0
 * @remarks This function is not valid if security type is #INM_WIFI_SECURITY_TYPE_EAP.
 * @param[in] ap         The access point handle
 * @param[out] required  @c true if passphrase is required,
 *             @c false if passphrase is not required.
 * @return 0 on success, otherwise negative error value
 * @retval #INM_ERROR_NONE               Successful
 * @retval #INM_ERROR_INVALID_PARAMETER  Invalid parameter
 * @retval #INM_ERROR_NOT_SUPPORTED      Not supported
 * @retval #INM_ERROR_OPERATION_FAILED   Operation failed
 */
int inm_wifi_ap_is_passphrase_required(inm_connection_h ap, bool *required);

/**
 * @brief Checks whether the WPS (Wi-Fi Protected Setup) is supported or not.
 * @since_tizen 5.0
 * @param[in] ap          The access point handle
 * @param[out] supported  @c true if WPS is supported,
 *              otherwise @c false is WPS is not supported.
 * @return 0 on success, otherwise negative error value
 * @retval #INM_ERROR_NONE               Successful
 * @retval #INM_ERROR_INVALID_PARAMETER  Invalid parameter
 * @retval #INM_ERROR_NOT_SUPPORTED      Not supported
 * @retval #INM_ERROR_OPERATION_FAILED   Operation failed
 */
int inm_wifi_ap_is_wps_supported(inm_connection_h ap, bool *supported);

/**
 * @brief Gets the Wi-Fi disconnect reason from the supplicant.
 * @since_tizen 5.0
 * @param[in] ap                  The access point handle
 * @param[out] disconnect_reason  The supplicant disconnect reason
 * @return 0 on success, otherwise negative error value
 * @retval #INM_ERROR_NONE               Successful
 * @retval #INM_ERROR_INVALID_PARAMETER  Invalid parameter
 * @retval #INM_ERROR_NOT_SUPPORTED      Not supported
 * @retval #INM_ERROR_OPERATION_FAILED   Operation failed
 */
int inm_wifi_ap_get_disconnect_reason(inm_connection_h ap,
							inm_wifi_disconnect_reason_e *disconnect_reason);

/**
 * @brief Gets the Wi-Fi Association Status Code from the supplicant.
 * @since_tizen 5.0
 * @param[in] ap            The access point handle
 * @param[out] status_code  The supplicant Wi-Fi association status code
 * @return 0 on success, otherwise negative error value
 * @retval #INM_ERROR_NONE               Successful
 * @retval #INM_ERROR_INVALID_PARAMETER  Invalid parameter
 * @retval #INM_ERROR_NOT_SUPPORTED      Not supported
 * @retval #INM_ERROR_OPERATION_FAILED   Operation failed
 */
int inm_wifi_ap_get_assoc_status_code(inm_connection_h ap,
							inm_wifi_assoc_status_code_e *status_code);
/**
 * @brief Called with VSIE data and length of VSIE.
 * @since_tizen 5.0
 * @remarks  If @a vsie is needed outside the callback, a copy should be
 *           made. @a vsie will be freed automatically after the execution
 *           of this callback.
 * @param[in] vsie             The vendor specific data
 * @param[in] length         The length of vendor specific data
 * @param[in] user_data        The user data passed from the foreach function
 * @return  @c true to continue with the next iteration of the loop, \n
 *          @c false to break out of the loop
 * @pre  inm_wifi_ap_foreach_vsie() will invoke this callback.
 * @see  inm_wifi_ap_foreach_vsie()
 */
typedef bool(*inm_wifi_ap_vsie_cb)(unsigned char *vsie, int length, void *user_data);

/**
 * @brief Gets all VSIE of AP.
 * @since_tizen 5.0
 * @param[in] ap         The access point handle
 * @param[in] callback   The callback to be called for each VSIE of AP
 * @param[in] user_data  The user data passed to the callback function
 * @return 0 on success, otherwise negative error value.
 * @retval #INM_ERROR_NONE               Successful
 * @retval #INM_ERROR_OUT_OF_MEMORY      Out of memory
 * @retval #INM_ERROR_INVALID_PARAMETER  Invalid parameter
 * @retval #INM_ERROR_NOT_SUPPORTED      Not supported
 * @retval #INM_ERROR_OPERATION_FAILED   Operation failed
 */
int inm_wifi_ap_foreach_vsie(inm_connection_h ap,
		inm_wifi_ap_vsie_cb callback, void *user_data);

/**
 * @}
 */

/**
 * @addtogroup CAPI_NETWORK_INM_LINK_MODULE
 * @{
 */

/**
 * @brief Destroys a link handle.
 * @since_tizen 5.5
 * @param[out] link  The link handle
 * @return @c 0 on success,
 *         otherwise a negative error value
 * @retval #INM_ERROR_NONE               Successful
 * @retval #INM_ERROR_INVALID_PARAMETER  Invalid parameter
 * @retval #INM_ERROR_NOT_SUPPORTED      Not supported
 * @retval #INM_ERROR_NOT_INITIALIZED    Not initialized
 * @retval #INM_ERROR_OPERATION_FAILED   Operation failed
 * @see inm_link_clone()
 * @see inm_foreach_link()
 */
int inm_link_destroy(inm_link_h link);

/**
 * @brief Clones a link handle.
 * @since_tizen 5.5
 * @remarks You must release @a cloned link using inm_link_destroy().
 * @param[out] cloned  The handle of the cloned link
 * @param[in] origin   The handle of the origin link
 * @return @c 0 on success,
 *         otherwise a negative error value
 * @retval #INM_ERROR_NONE               Successful
 * @retval #INM_ERROR_OUT_OF_MEMORY      Out of memory
 * @retval #INM_ERROR_INVALID_PARAMETER  Invalid parameter
 * @retval #INM_ERROR_NOT_SUPPORTED      Not supported
 * @retval #INM_ERROR_NOT_INITIALIZED    Not initialized
 * @retval #INM_ERROR_OPERATION_FAILED   Operation failed
 * @see inm_link_destroy()
 */
int inm_link_clone(inm_link_h *cloned,
		inm_link_h origin);

/**
 * @brief Gets the link interface name.
 * @since_tizen 5.5
 * @remarks You must release @a interface_name using free().
 * @param[in] link             The link handle
 * @param[out] interface_name  The interface name of the link
 * @return @c 0 on success,
 *         otherwise a negative error value
 * @retval #INM_ERROR_NONE               Successful
 * @retval #INM_ERROR_OUT_OF_MEMORY      Out of memory
 * @retval #INM_ERROR_INVALID_PARAMETER  Invalid parameter
 * @retval #INM_ERROR_NOT_SUPPORTED      Not supported
 * @retval #INM_ERROR_NOT_INITIALIZED    Not initialized
 * @retval #INM_ERROR_OPERATION_FAILED   Operation failed
 * @see inm_foreach_link()
 */
int inm_link_get_interface_name(inm_link_h link, char **interface_name);

/**
 * @brief Gets the link flags.
 * @since_tizen 5.5
 * @param[in] link    The link handle
 * @param[out] flags The flags, values of #inm_link_flag_e combined with bitwise 'or'
 * @return @c 0 on success,
 *         otherwise a negative error value
 * @retval #INM_ERROR_NONE               Successful
 * @retval #INM_ERROR_INVALID_PARAMETER  Invalid parameter
 * @retval #INM_ERROR_NOT_SUPPORTED      Not supported
 * @retval #INM_ERROR_NOT_INITIALIZED    Not initialized
 * @retval #INM_ERROR_OPERATION_FAILED   Operation failed
 * @see inm_foreach_link()
 */
int inm_link_get_flags(inm_link_h link, int *flags);

/**
 * @brief Gets the link operation status.
 * @since_tizen 5.5
 * @param[in] link              The link handle
 * @param[out] operation_state  The operation status of the link
 * @return @c 0 on success,
 *         otherwise a negative error value
 * @retval #INM_ERROR_NONE               Successful
 * @retval #INM_ERROR_INVALID_PARAMETER  Invalid parameter
 * @retval #INM_ERROR_NOT_SUPPORTED      Not supported
 * @retval #INM_ERROR_NOT_INITIALIZED    Not initialized
 * @retval #INM_ERROR_OPERATION_FAILED   Operation failed
 * @see inm_foreach_link()
 */
int inm_link_get_operation_state(inm_link_h link,
		inm_link_oper_state_e *operation_state);

/**
 * @brief Gets the bytes received by the link.
 * @since_tizen 5.5
 * @param[in] link    The link handle
 * @param[out] bytes  The bytes received by the link
 * @return @c 0 on success,
 *         otherwise a negative error value
 * @retval #INM_ERROR_NONE               Successful
 * @retval #INM_ERROR_INVALID_PARAMETER  Invalid parameter
 * @retval #INM_ERROR_NOT_SUPPORTED      Not supported
 * @retval #INM_ERROR_NOT_INITIALIZED    Not initialized
 * @retval #INM_ERROR_OPERATION_FAILED   Operation failed
 * @see inm_foreach_link()
 */
int inm_link_get_received_bytes(inm_link_h link, unsigned long long *bytes);

/**
 * @brief Gets the bytes sent by the link.
 * @since_tizen 5.5
 * @param[in] link    The link handle
 * @param[out] bytes  The bytes sent by the link
 * @return @c 0 on success,
 *         otherwise a negative error value
 * @retval #INM_ERROR_NONE               Successful
 * @retval #INM_ERROR_INVALID_PARAMETER  Invalid parameter
 * @retval #INM_ERROR_NOT_SUPPORTED      Not supported
 * @retval #INM_ERROR_NOT_INITIALIZED    Not initialized
 * @retval #INM_ERROR_OPERATION_FAILED   Operation failed
 * @see inm_foreach_link()
 */
int inm_link_get_sent_bytes(inm_link_h link, unsigned long long *bytes);

/**
 * @brief Called with handle of the link address.
 * @since_tizen 5.5
 * @remarks  If @a address is needed outside the callback, a copy should be
 *           made. @a address will be freed automatically after the execution
 *           of this callback.
 * @param[in] address    The link address handle
 * @param[in] user_data  The user data passed from the foreach function
 * @return  @c true to continue with the next iteration of the loop, \n
 *          @c false to break out of the loop
 * @pre  inm_link_foreach_address() will invoke this callback.
 * @see  inm_link_foreach_address()
 */
typedef bool(*inm_link_address_cb)(inm_link_address_h address, void *user_data);

/**
 * @brief Gets all addresses of the link.
 * @since_tizen 5.5
 * @param[in] link       The link handle
 * @param[in] callback   The callback to be called for each address of link
 * @param[in] user_data  The user data passed to the callback function
 * @return 0 on success, otherwise negative error value.
 * @retval #INM_ERROR_NONE               Successful
 * @retval #INM_ERROR_OUT_OF_MEMORY      Out of memory
 * @retval #INM_ERROR_INVALID_PARAMETER  Invalid parameter
 * @retval #INM_ERROR_NOT_SUPPORTED      Not supported
 * @retval #INM_ERROR_OPERATION_FAILED   Operation failed
 */
int inm_link_foreach_address(inm_link_h link,
		inm_link_address_cb callback, void *user_data);

/**
 * @brief Destroys a link address handle.
 * @since_tizen 5.5
 * @param[in] address  The link address handle
 * @return @c 0 on success,
 *         otherwise a negative error value
 * @retval #INM_ERROR_NONE               Successful
 * @retval #INM_ERROR_INVALID_PARAMETER  Invalid parameter
 * @retval #INM_ERROR_NOT_SUPPORTED      Not supported
 * @retval #INM_ERROR_NOT_INITIALIZED    Not initialized
 * @retval #INM_ERROR_OPERATION_FAILED   Operation failed
 * @see inm_link_address_clone()
 * @see inm_link_foreach_address()
 */
int inm_link_address_destroy(inm_link_address_h address);

/**
 * @brief Clones a link address handle.
 * @since_tizen 5.5
 * @remarks You must release @a cloned link address using inm_link_address_destroy().
 * @param[out] cloned  The handle of the cloned link address
 * @param[in] origin   The handle of the origin link address
 * @return @c 0 on success,
 *         otherwise a negative error value
 * @retval #INM_ERROR_NONE               Successful
 * @retval #INM_ERROR_OUT_OF_MEMORY      Out of memory
 * @retval #INM_ERROR_INVALID_PARAMETER  Invalid parameter
 * @retval #INM_ERROR_NOT_SUPPORTED      Not supported
 * @retval #INM_ERROR_NOT_INITIALIZED    Not initialized
 * @retval #INM_ERROR_OPERATION_FAILED   Operation failed
 * @see inm_link_address_destroy()
 */
int inm_link_address_clone(inm_link_address_h *cloned,
		inm_link_address_h origin);

/**
 * @brief Gets the address family.
 * @since_tizen 5.5
 * @param[in] address  The address handle
 * @param[out] family  The address family
 * @return @c 0 on success,
 *         otherwise a negative error value
 * @retval #INM_ERROR_NONE               Successful
 * @retval #INM_ERROR_INVALID_PARAMETER  Invalid parameter
 * @retval #INM_ERROR_NOT_SUPPORTED      Not supported
 * @retval #INM_ERROR_NOT_INITIALIZED    Not initialized
 * @retval #INM_ERROR_OPERATION_FAILED   Operation failed
 * @see inm_link_foreach_address()
 */
int inm_link_address_get_family(inm_link_address_h address,
		inm_address_family_e *family);

/**
 * @brief Gets the address prefix length.
 * @since_tizen 5.5
 * @param[in] address         The address handle
 * @param[out] prefix_length  The prefix length of address
 * @return @c 0 on success,
 *         otherwise a negative error value
 * @retval #INM_ERROR_NONE               Successful
 * @retval #INM_ERROR_INVALID_PARAMETER  Invalid parameter
 * @retval #INM_ERROR_NOT_SUPPORTED      Not supported
 * @retval #INM_ERROR_NOT_INITIALIZED    Not initialized
 * @retval #INM_ERROR_OPERATION_FAILED   Operation failed
 * @see inm_link_foreach_address()
 */
int inm_link_address_get_prefix_length(inm_link_address_h address,
		int *prefix_length);

/**
 * @brief Gets the address scope.
 * @since_tizen 5.5
 * @param[in] address  The address handle
 * @param[out] scope   The address scope
 * @return @c 0 on success,
 *         otherwise a negative error value
 * @retval #INM_ERROR_NONE               Successful
 * @retval #INM_ERROR_INVALID_PARAMETER  Invalid parameter
 * @retval #INM_ERROR_NOT_SUPPORTED      Not supported
 * @retval #INM_ERROR_NOT_INITIALIZED    Not initialized
 * @retval #INM_ERROR_OPERATION_FAILED   Operation failed
 * @see inm_link_foreach_address()
 */
int inm_link_address_get_scope(inm_link_address_h address,
		inm_link_scope_e *scope);

/**
 * @brief Gets the address string.
 * @since_tizen 5.5
 * @remarks You must release @a address_string using free().
 * @param[in] address          The address handle
 * @param[out] address_string  The address string
 * @return @c 0 on success,
 *         otherwise a negative error value
 * @retval #INM_ERROR_NONE               Successful
 * @retval #INM_ERROR_OUT_OF_MEMORY      Out of memory
 * @retval #INM_ERROR_INVALID_PARAMETER  Invalid parameter
 * @retval #INM_ERROR_NOT_SUPPORTED      Not supported
 * @retval #INM_ERROR_NOT_INITIALIZED    Not initialized
 * @retval #INM_ERROR_OPERATION_FAILED   Operation failed
 * @retval #INM_ERROR_DATA_NOT_FOUND     Data not found
 * @see inm_link_foreach_address()
 */
int inm_link_address_get_string(inm_link_address_h address,
		char **address_string);

/**
 * @brief Called with handle of the link route table.
 * @since_tizen 5.0
 * @remarks  If @a route is needed outside the callback, a copy should be
 *           made. @a route will be freed automatically after the execution
 *           of this callback.
 * @param[in] route      The link route handle
 * @param[in] user_data  The user data passed from the foreach function
 * @return  @c true to continue with the next iteration of the loop, \n
 *          @c false to break out of the loop
 * @pre  inm_link_foreach_route() will invoke this callback.
 * @see  inm_link_foreach_route()
 */
typedef bool(*inm_link_route_cb)(inm_link_route_h route, void *user_data);

/**
 * @brief Gets all route tables of the link.
 * @since_tizen 5.0
 * @param[in] link       The link handle
 * @param[in] callback   The callback to be called for each route table of link
 * @param[in] user_data  The user data passed to the callback function
 * @return 0 on success, otherwise negative error value.
 * @retval #INM_ERROR_NONE               Successful
 * @retval #INM_ERROR_OUT_OF_MEMORY      Out of memory
 * @retval #INM_ERROR_INVALID_PARAMETER  Invalid parameter
 * @retval #INM_ERROR_NOT_SUPPORTED      Not supported
 * @retval #INM_ERROR_OPERATION_FAILED   Operation failed
 */
int inm_link_foreach_route(inm_link_h link,
		inm_link_route_cb callback, void *user_data);

/**
 * @brief Destroys a link route handle.
 * @since_tizen 5.5
 * @param[in] route  The link route handle
 * @return @c 0 on success,
 *         otherwise a negative error value
 * @retval #INM_ERROR_NONE               Successful
 * @retval #INM_ERROR_INVALID_PARAMETER  Invalid parameter
 * @retval #INM_ERROR_NOT_SUPPORTED      Not supported
 * @retval #INM_ERROR_NOT_INITIALIZED    Not initialized
 * @retval #INM_ERROR_OPERATION_FAILED   Operation failed
 * @see inm_link_route_clone()
 * @see inm_link_foreach_route()
 */
int inm_link_route_destroy(inm_link_route_h route);

/**
 * @brief Clones a link route handle.
 * @since_tizen 5.5
 * @remarks You must release @a cloned link route using inm_link_route_destroy().
 * @param[out] cloned  The handle of the cloned link route
 * @param[in] origin   The handle of the origin link route
 * @return @c 0 on success,
 *         otherwise a negative error value
 * @retval #INM_ERROR_NONE               Successful
 * @retval #INM_ERROR_OUT_OF_MEMORY      Out of memory
 * @retval #INM_ERROR_INVALID_PARAMETER  Invalid parameter
 * @retval #INM_ERROR_NOT_SUPPORTED      Not supported
 * @retval #INM_ERROR_NOT_INITIALIZED    Not initialized
 * @retval #INM_ERROR_OPERATION_FAILED   Operation failed
 * @see inm_link_route_destroy()
 */
int inm_link_route_clone(inm_link_route_h *cloned,
		inm_link_route_h origin);

/**
 * @brief Gets the route destination.
 * @since_tizen 5.5
 * @remarks You must release @a destination using free().
 * @param[in] route         The route handle
 * @param[out] destination  The route destination
 * @return @c 0 on success,
 *         otherwise a negative error value
 * @retval #INM_ERROR_NONE               Successful
 * @retval #INM_ERROR_OUT_OF_MEMORY      Out of memory
 * @retval #INM_ERROR_INVALID_PARAMETER  Invalid parameter
 * @retval #INM_ERROR_NOT_SUPPORTED      Not supported
 * @retval #INM_ERROR_NOT_INITIALIZED    Not initialized
 * @retval #INM_ERROR_OPERATION_FAILED   Operation failed
 * @retval #INM_ERROR_DATA_NOT_FOUND     Data not found
 * @see inm_link_foreach_route()
 */
int inm_link_route_get_destination(inm_link_route_h route,
		char **destination);

/**
 * @brief Gets the route gateway.
 * @since_tizen 5.5
 * @remarks You must release @a gateway using free().
 * @param[in] route     The route handle
 * @param[out] gateway  The route gateway
 * @return @c 0 on success,
 *         otherwise a negative error value
 * @retval #INM_ERROR_NONE               Successful
 * @retval #INM_ERROR_OUT_OF_MEMORY      Out of memory
 * @retval #INM_ERROR_INVALID_PARAMETER  Invalid parameter
 * @retval #INM_ERROR_NOT_SUPPORTED      Not supported
 * @retval #INM_ERROR_NOT_INITIALIZED    Not initialized
 * @retval #INM_ERROR_OPERATION_FAILED   Operation failed
 * @retval #INM_ERROR_DATA_NOT_FOUND     Data not found
 * @see inm_link_foreach_route()
 */
int inm_link_route_get_gateway(inm_link_route_h route,
		char **gateway);

/**
 * @brief Gets the route interface.
 * @since_tizen 5.5
 * @remarks You must release @a interface using free().
 * @param[in] route       The route handle
 * @param[out] interface  The route interface
 * @return @c 0 on success,
 *         otherwise a negative error value
 * @retval #INM_ERROR_NONE               Successful
 * @retval #INM_ERROR_OUT_OF_MEMORY      Out of memory
 * @retval #INM_ERROR_INVALID_PARAMETER  Invalid parameter
 * @retval #INM_ERROR_NOT_SUPPORTED      Not supported
 * @retval #INM_ERROR_NOT_INITIALIZED    Not initialized
 * @retval #INM_ERROR_OPERATION_FAILED   Operation failed
 * @retval #INM_ERROR_DATA_NOT_FOUND     Data not found
 * @see inm_link_foreach_route()
 */
int inm_link_route_get_interface(inm_link_route_h route,
		char **interface);

/**
 * @brief Checks whether the route is default or not.
 * @since_tizen 5.5
 * @param[in] route        The route handle
 * @param[out] is_default  @c true if route is default,
 *               otherwise @c false if route is not default
 * @return @c 0 on success,
 *         otherwise a negative error value
 * @retval #INM_ERROR_NONE               Successful
 * @retval #INM_ERROR_INVALID_PARAMETER  Invalid parameter
 * @retval #INM_ERROR_NOT_SUPPORTED      Not supported
 * @retval #INM_ERROR_NOT_INITIALIZED    Not initialized
 * @retval #INM_ERROR_OPERATION_FAILED   Operation failed
 * @see inm_link_foreach_route()
 */
int inm_link_route_is_default(inm_link_route_h route,
		bool *is_default);

/**
 * @brief Gets the route type.
 * @since_tizen 5.5
 * @param[in] route  The route handle
 * @param[out] type  The route type
 * @return @c 0 on success,
 *         otherwise a negative error value
 * @retval #INM_ERROR_NONE               Successful
 * @retval #INM_ERROR_INVALID_PARAMETER  Invalid parameter
 * @retval #INM_ERROR_NOT_SUPPORTED      Not supported
 * @retval #INM_ERROR_NOT_INITIALIZED    Not initialized
 * @retval #INM_ERROR_OPERATION_FAILED   Operation failed
 * @see inm_link_foreach_route()
 */
int inm_link_route_get_type(inm_link_route_h route,
		inm_link_route_type_e *type);

/**
 * @}
 */

#ifdef __cplusplus
}
#endif

#endif /* __TIZEN_NET_INM_H__ */

