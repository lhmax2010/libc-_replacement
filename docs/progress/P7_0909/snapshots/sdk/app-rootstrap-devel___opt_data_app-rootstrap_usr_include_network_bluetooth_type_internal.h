/*
 * Copyright (c) 2011 Samsung Electronics Co., Ltd All Rights Reserved
 *
 * Licensed under the Apache License, Version 2.0 (the License);
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 * http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an AS IS BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#ifndef __TIZEN_NETWORK_BLUETOOTH_TYPE_INTERNAL_H__
#define __TIZEN_NETWORK_BLUETOOTH_TYPE_INTERNAL_H__

 #ifdef __cplusplus
extern "C"
{
#endif /* __cplusplus */

#include <stdint.h>
#include <glib.h>

/* This variable will be added into bt_service_class_t in tizen 4.0 */
#define BT_SC_MAP_SERVICE_MASK 0x00800000 /**< MAP service class */

/* Macro for advertising interval */
#define BT_LE_ADV_INTERVAL_MIN 32 /* equals to 20msec, minimun allowed interval */
#define BT_LE_ADV_INTERVAL_MAX 16384 /* equals to 10.24sec, maximum allowed interval */

/**
 * @file bluetooth_type_internal.h
 */

#define BT_PROFILE_SERVICE_UUID_SPP		"1101"
#define BT_PROFILE_SERVICE_UUID_HSP_HS		"1108"
#define BT_PROFILE_SERVICE_UUID_HSP_AG		"1112"
#define BT_PROFILE_SERVICE_UUID_OPP		"1105"
#define BT_PROFILE_SERVICE_UUID_HFP_HF		"111e"
#define BT_PROFILE_SERVICE_UUID_HFP_AG		"111f"
#define BT_PROFILE_SERVICE_UUID_A2DP_SOURCE	"110a"
#define BT_PROFILE_SERVICE_UUID_A2DP_SINK	"110b"
#define BT_PROFILE_SERVICE_UUID_AVRCP_TARGET	"110c"
#define BT_PROFILE_SERVICE_UUID_AVRCP_CONTROL	"110f"
#define BT_PROFILE_SERVICE_UUID_PAN_PANU	"1115"
#define BT_PROFILE_SERVICE_UUID_PAN_NAP		"1116"
#define BT_PROFILE_SERVICE_UUID_PAN_GN		"1117"
#define BT_PROFILE_SERVICE_UUID_HDP_SOURCE	"1401"
#define BT_PROFILE_SERVICE_UUID_HDP_SINK	"1402"
#define BT_PROFILE_SERVICE_UUID_HID		"1124"

/**
 * @ingroup CAPI_NETWORK_BLUETOOTH_ADAPTER_LE_MODULE
 * @brief  Enumerations of the Bluetooth adapter le state.
 * @since_tizen 2.3
 */
typedef enum {
	BT_ADAPTER_LE_DISABLED = 0x00, /**< Bluetooth le is disabled */
	BT_ADAPTER_LE_ENABLED, /**< Bluetooth le is enabled */
} bt_adapter_le_state_e;

/**
 * @ingroup CAPI_NETWORK_BLUETOOTH_GATT_MODULE
 * @brief  Enumerations of the ATT error codes.
 * @since_tizen 4.0
 */
typedef enum {
	BT_ATT_ERROR_INTERNAL = -1,
	BT_ATT_ERROR_NONE =	0x00,

	/* Attribute error codes */
	BT_ATT_ERROR_INVALID_HANDLE = 0x01,
	BT_ATT_ERROR_READ_NOT_PERMITTED = 0x02,
	BT_ATT_ERROR_WRITE_NOT_PERMITTED = 0x03,
	BT_ATT_ERROR_INVALID_PDU = 0x04,
	BT_ATT_ERROR_AUTHENTICATION = 0x05,
	BT_ATT_ERROR_REQUEST_NOT_SUPPORTED = 0x06,
	BT_ATT_ERROR_INVALID_OFFSET = 0x07,
	BT_ATT_ERROR_AUTHORIZATION = 0x08,
	BT_ATT_ERROR_PREPARE_QUEUE_FULL = 0x09,
	BT_ATT_ERROR_ATTRIBUTE_NOT_FOUND = 0x0A,
	BT_ATT_ERROR_ATTRIBUTE_NOT_LONG = 0x0B,
	BT_ATT_ERROR_INSUFFICIENT_ENCRYPTION_KEY_SIZE = 0x0C,
	BT_ATT_ERROR_INVALID_ATTRIBUTE_VALUE_LEN = 0x0D,
	BT_ATT_ERROR_UNLIKELY = 0x0E,
	BT_ATT_ERROR_INSUFFICIENT_ENCRYPTION = 0x0F,
	BT_ATT_ERROR_UNSUPPORTED_GROUP_TYPE = 0x10,
	BT_ATT_ERROR_INSUFFICIENT_RESOURCES = 0x11,

	/* Common profile error codes */
	BT_ATT_ERROR_WRITE_REQUEST_REJECTED = 0xFC,
	BT_ATT_ERROR_CCCD_IMPROPERLY_CONFIGURED = 0xFD,
	BT_ATT_ERROR_PROCEDURE_ALREADY_IN_PROGRESS = 0xFE,
	BT_ATT_ERROR_OUT_OF_RANGE = 0xFF,

} bt_att_error_e;

/**
 * @ingroup CAPI_NETWORK_BLUETOOTH_OTP_MODULE
 * @brief  Enumerations of the ATT error codes specific to OTP.
 * @since_tizen 4.0
 */
typedef enum {
	/* Bluetooth OTP error codes */
	BT_OTP_ERROR_WRITE_REQUEST_REJECTED = 0x80,
	BT_OTP_ERROR_OBJECT_NOT_SELECTED = 0x81,
	BT_OTP_ERROR_CONCURRENCY_LIMIT_EXCEEDED = 0x82,
	BT_OTP_ERROR_OBJECT_NAME_EXISTS = 0x83,
} bt_otp_error_e;

/**
 * @ingroup CAPI_NETWORK_BLUETOOTH_ADAPTER_LE_MODULE
 * @brief  Called when the Bluetooth adapter le state changes.
 * @since_tizen 2.3
 *
 * @param[in]   result  The result of the adapter state changing
 * @param[in]   adapter_le_state  The adapter le state to be changed
 * @param[in]   user_data  The user data passed from the callback registration function
 * @pre Either bt_adapter_le_enable() or bt_adapter_le_disable() will invoke this callback if you register this callback using bt_adapter_le_set_state_changed_cb().
 * @see bt_adapter_le_enable()
 * @see bt_adapter_le_disable()
 * @see bt_adapter_le_set_state_changed_cb()
 * @see bt_adapter_le_unset_state_changed_cb()
 */
typedef void (*bt_adapter_le_state_changed_cb)(int result, bt_adapter_le_state_e adapter_le_state, void *user_data);

/**
 * @ingroup CAPI_NETWORK_BLUETOOTH_AVRCP_MODULE
 * @brief  Called when the delay is changed by the remote device.
 * @since_tizen 5.0
 * @param[in] delay Streaming delay in milliseconds.
 * @param[in] user_data The user data passed from the callback registration function
 * @see bt_avrcp_set_delay_changed_cb()
 * @see bt_avrcp_unset_delay_changed_cb()
 */
typedef void (*bt_avrcp_delay_changed_cb) (unsigned int delay, void *user_data);

/**
 * @ingroup CAPI_NETWORK_BLUETOOTH_AVRCP_MODULE
 * @brief  Called when the volume is changed by the remote device.
 * @since_tizen 6.0
 * @param[in] volume Volume in system value. (0 ~ 150)
 * @param[in] user_data The user data passed from the callback registration function
 * @see bt_avrcp_set_volume_changed_cb()
 * @see bt_avrcp_unset_volume_changed_cb()
 */
typedef void (*bt_audio_absolute_volume_changed_cb) (unsigned int volume, void *user_data);

/**
 * @ingroup CAPI_NETWORK_BLUETOOTH_AUDIO_MODULE
 * @brief  Called when the absolute volume control status is changed.
 * @since_tizen 6.0
 * @param[in] activated
 * @param[in] user_data The user data passed from the callback registration function
 * @see bt_avrcp_set_volume_changed_cb()
 * @see bt_avrcp_unset_volume_changed_cb()
 */
typedef void (*bt_audio_avc_status_changed_cb) (bool activated, void *user_data);

/**
 * @ingroup CAPI_NETWORK_BLUETOOTH_AUDIO_AG_MODULE
 * @brief  Enumerations for the call state
 * @since_tizen 2.3
 */
typedef enum {
	BT_AG_CALL_EVENT_IDLE = 0x00,  /**< Idle */
	BT_AG_CALL_EVENT_ANSWERED,  /**< Answered */
	BT_AG_CALL_EVENT_HELD,  /**< Held */
	BT_AG_CALL_EVENT_RETRIEVED,  /**< Retrieved */
	BT_AG_CALL_EVENT_DIALING,  /**< Dialing */
	BT_AG_CALL_EVENT_ALERTING,  /**< Alerting */
	BT_AG_CALL_EVENT_INCOMING,  /**< Incoming */
} bt_ag_call_event_e;

/**
 * @ingroup CAPI_NETWORK_BLUETOOTH_AUDIO_AG_MODULE
 * @brief  Enumerations for the call state
 * @since_tizen 2.3
 */
typedef enum {
	BT_AG_CALL_STATE_IDLE = 0x00,  /**< Idle state */
	BT_AG_CALL_STATE_ACTIVE,  /**< Active state */
	BT_AG_CALL_STATE_HELD,  /**< Held state */
	BT_AG_CALL_STATE_DIALING,  /**< Dialing state */
	BT_AG_CALL_STATE_ALERTING,  /**< Alerting state */
	BT_AG_CALL_STATE_INCOMING,  /**< Incoming state */
	BT_AG_CALL_STATE_WAITING,  /**< Waiting for connected indication event after answering an incoming call*/
} bt_ag_call_state_e;

/**
 * @ingroup CAPI_NETWORK_BLUETOOTH_OPP_MODULE
 * @brief  Enumerations for the transfer type
 * @since_tizen 3.0
 */
typedef enum {
	BT_TRANSFER_INBOUND,	    /**< Inbound Transfer Type */
	BT_TRANSFER_OUTBOUND,	    /**< Outbound Transfer Type */
} bt_opp_transfer_type_t;

/**
 * @ingroup CAPI_NETWORK_BLUETOOTH_AUDIO_MODULE
 * @brief  Enumerations of the Bluetooth A2DP role.
 * @since_tizen 4.0
 */
typedef enum {
	BT_A2DP_SOURCE,
	BT_A2DP_SINK,
} bt_audio_role_e;

/**
 * @ingroup CAPI_NETWORK_BLUETOOTH_MAP_CLIENT_MODULE
 * @brief
 * @since_tizen 3.0
 */
typedef void* bt_map_client_session_info_h;

typedef void* bt_map_client_message_object_h;

typedef struct {
	int16_t offset;
	int16_t max_count;
} bt_map_client_list_folders_filter_s;

typedef struct {
	int16_t offset;
	int16_t max_count;
	int8_t subject_length;
	char *fields;
	char *types;
	char *period_begin;
	char *period_end;
	int is_read;
	char *recipient;
	char *sender;
	int is_priority;
} bt_map_client_list_messages_filter_s;

typedef struct {
	bt_map_client_message_object_h message_object;
	char *folder;
	char *subject;
	char *timestamp;
	char *sender;
	char *sender_address;
	char *reply_to;
	char *recipient;
	char *recipient_address;
	char *type;
	int64_t size;
	int is_text;
	char *status;
	int64_t attachment_size;
	int is_priority;
	int is_read;
	int is_sent;
	int is_protected;
} bt_map_client_message_item_s;

typedef struct {
	int is_transparent;
	int is_retry;
	char *charset;
} bt_map_client_push_message_args_s;

typedef struct {
	char *file_path;
// TODO: maybe some additional fields could be supported
//	char *folder;
//	char *subject;
//	char *timestamp;
//	char *sender;
//	char *sender_address;
//	char *reply_to;
//	char *recipient;
//	char *recipient_address;
//	char *type;
//	int64_t size;
//	char *status;
//	int is_priority;
//	int is_read;
//	int is_deleted;
//	int is_sent;
//	int is_protected;
} bt_map_client_message_s;

typedef struct {
	time_t session_start_time;
	time_t session_end_time;
	uint32_t tx_time;
	uint32_t rx_time;
	uint32_t idle_time;
	uint32_t session_connected_time;
	uint32_t session_scan_time;
	GSList *atm_list;
} bt_battery_info_s;

typedef struct {
	uid_t uid;
	pid_t pid;
	uint32_t rx_bytes;
	uint32_t tx_bytes;
	uint time;
} bt_battery_app_info_s;

typedef bt_map_client_list_folders_filter_s *bt_map_client_list_folders_filter_h;

typedef bt_map_client_list_messages_filter_s *bt_map_client_list_messages_filter_h;

typedef bt_map_client_message_item_s *bt_map_client_message_item_h;

typedef bt_map_client_push_message_args_s *bt_map_client_push_message_args_h;

typedef bt_map_client_message_s *bt_map_client_message_h;

typedef void (*bt_map_client_list_folders_cb)(int result, char **folders, int count, void *user_data);

typedef void (*bt_map_client_list_filter_fields_cb)(int result, char **filter_fields, int count, void *user_data);

typedef void (*bt_map_client_list_messages_cb)(int result, bt_map_client_message_item_s *messages, int count, void *user_data);

typedef void (*bt_map_client_push_message_cb)(int result, void *user_data);

typedef void (*bt_map_client_get_message_cb)(int result, bt_map_client_message_h message, void *user_data);

/**
 * @ingroup CAPI_NETWORK_BLUETOOTH_ADAPTER_MODULE
 * @brief  Called when the connectable state changes.
 * @since_tizen 2.3
 *
 * @param[in] result The result of the connectable state changing
 * @param[in] connectable The connectable to be changed
 * @param[in] user_data The user data passed from the callback registration function
 *
 * @pre This function will be invoked when the connectable state of local Bluetooth adapter changes
 * if you register this callback using bt_adapter_set_connectable_changed_cb().
 *
 * @see bt_adapter_set_connectable()
 * @see bt_adapter_set_connectable_changed_cb()
 * @see bt_adapter_unset_connectable_changed_cb()
 */
typedef void (*bt_adapter_connectable_changed_cb)
	(int result, bool connectable, void *user_data);

/**
 * @ingroup CAPI_NETWORK_BLUETOOTH_ADAPTER_MODULE
 * @brief  Called when adapter is disabled.
 * @since_tizen5.5
 *
 * @param[in] data Bluetooth session data
 *
 * @pre This function will be invoked when Bluetooth adapter is disabled
 * if you activate the battery monitor feature using bt_adapter_init_battery_monitor
 *
 * @see bt_adapter_init_battery_monitor
 */
typedef void (*bt_adapter_disable_battery_cb)(bt_battery_info_s *data);

/**
 * @ingroup CAPI_NETWORK_BLUETOOTH_OPP_SERVER_MODULE
 * @brief  Called when the push is requested.
 * @since_tizen 2.3
 *
 * @details You must call bt_opp_server_accept() if you want to accept.
 * Otherwise, you must call bt_opp_server_reject().
 * @param[in] file  The path of file to be pushed
 * @param[in] size The file size (bytes)
 * @param[in] user_data The user data passed from the callback registration function
 * @see bt_opp_server_initialize()
 */
typedef void (*bt_opp_server_push_requested_cb)(const char *file, int size, void *user_data);

/**
 * @ingroup CAPI_NETWORK_BLUETOOTH_ADAPTER_LE_MODULE
 * @brief  Enumerations of the Bluetooth adapter le scan type.
 * @since_tizen 2.3
 */
typedef enum {
	BT_ADAPTER_LE_PASSIVE_SCAN = 0x00,
	BT_ADAPTER_LE_ACTIVE_SCAN
} bt_adapter_le_scan_type_e;

/**
* @ingroup CAPI_NETWORK_BLUETOOTH_ADAPTER_LE_MODULE
* @since_tizen 8.0
* @brief  Enumerations of the Bluetooth LE phy mask.
*/
typedef enum {
	BT_LE_1M_PHY_MASK = 0x01,
	BT_LE_2M_PHY_MASK = 0x02,
	BT_LE_CODED_PHY_MASK = 0x04
} bt_adapter_le_phy_mask_e;

/**
* @ingroup CAPI_NETWORK_BLUETOOTH_ADAPTER_LE_MODULE
* @since_tizen 8.0
* @brief  Enumerations of the preferred coding when transmitting on LE CODED PHY.
*/
typedef enum {
	PHY_OPTION_NO_PREFERRED = 0x00,
	PHY_OPTION_S2 = 0x01,
	PHY_OPTION_S8 = 0x02
} bt_adapter_le_phy_option_e;

/**
 * @ingroup CAPI_NETWORK_BLUETOOTH_ADAPTER_MODULE
 * @brief  Called when the manufacturer dat changes.
 * @since_tizen 2.3
 *
 * @param[in]   data		The manufacurer data of the Bluetooth device to be changed
 * @param[in]   len			The length of @a data
 * @param[in]   user_data	The user data passed from the callback registration function
 * @pre This function will be invoked when the manufacturer data of Bluetooth adapter changes
 * if callback is registered using bt_adapter_set_manufacturer_data_changed_cb().
 * @see bt_adapter_set_manufacturer_data()
 * @see bt_adapter_set_manufacturer_data_changed_cb()
 * @see bt_adapter_unset_manufacturer_data_changed_cb()
 */
typedef void (*bt_adapter_manufacturer_data_changed_cb) (char *data,
		int len, void *user_data);

/**
 * @internal
 * @ingroup CAPI_NETWORK_BLUETOOTH_ADAPTER_MODULE
 * @brief  Called repeatedly when you get the devices connected with specific profile.
 * @since_tizen 3.0
 *
 * @param[in]   remote_address	The address of remote device
 * @param[in]   user_data	The user data passed from the callback registration function
 * @return @c true to continue with the next iteration of the loop,
 * \n @c false to break out of the loop.
 * @pre bt_adapter_foreach_profile_connected_devices() will invoke this function.
 * @see bt_adapter_foreach_profile_connected_devices()
 */
typedef bool (*bt_adapter_profile_connected_devices_cb)(const char *remote_address, void *user_data);

/**
* @ingroup CAPI_NETWORK_BLUETOOTH_DPM_MODULE
* @brief DPM BT allowance state
* @since_tizen 3.0
*/
typedef enum {
	BT_DPM_ERROR	  = -1,   /**< bluetooth allowance error */
	BT_DPM_BT_ALLOWED,	  /**< bluetooth allowance allowed */
	BT_DPM_HANDSFREE_ONLY, /**< bluetooth allowance handsfree only */
	BT_DPM_BT_RESTRICTED,  /**< bluetooth allowance restricted */
} bt_dpm_allow_e;

/**
* @ingroup CAPI_NETWORK_BLUETOOTH_DPM_MODULE
* @brief DPM Policy status
* @since_tizen 3.0
*/
typedef enum {
	BT_DPM_STATUS_ERROR = -1,

	BT_DPM_ALLOWED	 = 0,	 /**< DPM Policy status allowed. */
	BT_DPM_RESTRICTED		 = 1,	 /**< DPM Policy status restricted. */

	BT_DPM_ENABLE			 = 1,	 /**< DPM Policy status enabled. */
	BT_DPM_DISABLE	= 0,	 /**< DPM Policy status disabled. */

	BT_DPM_FALSE	 = 0,	/**< DPM Policy status false. */
	BT_DPM_TRUE		= 1,	/**< DPM Policy status true. */
} bt_dpm_status_e;

/**
* @ingroup CAPI_NETWORK_BLUETOOTH_DPM_MODULE
* @brief DPM Profile states
* @since_tizen 3.0
*/
typedef enum {
	BT_DPM_POLICY_A2DP_PROFILE_STATE,
	BT_DPM_POLICY_AVRCP_PROFILE_STATE,
	BT_DPM_POLICY_BPP_PROFILE_STATE,
	BT_DPM_POLICY_DUN_PROFILE_STATE,
	BT_DPM_POLICY_FTP_PROFILE_STATE,
	BT_DPM_POLICY_HFP_PROFILE_STATE,
	BT_DPM_POLICY_HSP_PROFILE_STATE,
	BT_DPM_POLICY_PBAP_PROFILE_STATE,
	BT_DPM_POLICY_SAP_PROFILE_STATE,
	BT_DPM_POLICY_SPP_PROFILE_STATE,
	BT_DPM_PROFILE_NONE,
} bt_dpm_profile_e;

/**
 * @ingroup CAPI_NETWORK_BLUETOOTH_DEVICE_MODULE
 * @brief Device LE connection update structure.
 * @since_tizen 2.3
 */
typedef struct {
	unsigned int interval_min;   /**< Minimum value for the connection event interval (msec) */
	unsigned int interval_max;   /**< Maximum value for the connection event interval (msec) */
	unsigned int latency;   /**< Slave latency (msec) */
	unsigned int time_out;   /**< Supervision timeout (msec) */
} bt_le_conn_update_s;

/**
 * Structure to DPM device list
 */
typedef struct {
	int count;
	char **devices;
} bt_dpm_device_list_s;

/**
 * Structure to DPM uuid list
 */
typedef struct {
	int count;
	char **uuids;
} bt_dpm_uuids_list_s;

/**
 * @internal
 * @ingroup CAPI_NETWORK_BLUETOOTH_ADAPTER_MODULE
 * @brief Enumaration for the address type field in manufactrer specific data
 * @since_tizen 3.0
 */
typedef enum {
	ADDRESS_NONE_TYPE = 0x00,
	WI_FI_P2P_ADDRESS = 0x01,
	BLUETOOTH_ADDRESS = 0x02,
	INDICATION_ADDRESS = 0x04,
	IPV4_ADDRESS = 0x08,
	IPV6_ADDRESS = 0x10,
	UNKNOWN_ADDRESS = 0xff
} connectivity_address_t;

/**
 * @internal
 * @ingroup CAPI_NETWORK_BLUETOOTH_ADAPTER_MODULE
 * @brief Enumaration for the proximity locality type field in manufactrer specific data
 * @since_tizen 3.0
 */
typedef enum {
	NONE_TYPE = 0x00,
	PROXIMITY = 0x01,
	CO_PRESENCE = 0x02,
	UNKNOWN_TYPE = 0xff
} bt_proximity_locality_t;

/**
 * @internal
 * @ingroup CAPI_NETWORK_BLUETOOTH_ADAPTER_MODULE
 * @brief Structure of samsung specific manufacturer data
 * @since_tizen 3.0
 */
typedef struct {
	unsigned char version;
	unsigned char service_id;
	unsigned char discovery_version;
	unsigned char associated_service_id;
	bt_proximity_locality_t proximity_locality_type;
	unsigned char proximity_locality_info;
	unsigned char device_type;
	unsigned char device_icon;
	unsigned char auth_info[5];
	connectivity_address_t addr_type;
	unsigned char addr1[6];
	unsigned char addr2[6];
	unsigned char channel_info;
	unsigned char associated_service_data_len;
	unsigned char *associated_service_data_val;
	char *name;
} bt_manufacturer_data;

/**
 * @ingroup CAPI_NETWORK_BLUETOOTH_DEVICE_MODULE
 * @brief Trusted Profile enumeration.
 * @since_tizen 3.0
 *
 * @see bt_device_set_profile_trusted()
 * @see bt_device_get_profile_trusted()
 */
typedef enum {
	BT_TRUSTED_PROFILE_PBAP = 1,
	BT_TRUSTED_PROFILE_MAP,
	BT_TRUSTED_PROFILE_SAP,
	BT_TRUSTED_PROFILE_HFP_HF,
	BT_TRUSTED_PROFILE_A2DP,
	BT_TRUSTED_PROFILE_ALL = 0xFFFFFFFF,
} bt_trusted_profile_t;

/**
 * @deprecated Deprecated since 4.0. Use bt_trusted_profile_t instead.
 * @ingroup CAPI_NETWORK_BLUETOOTH_DEVICE_MODULE
 * @brief Restricted Profile enumeration.
 * @since_tizen 3.0
 *
 * @see bt_device_set_profile_restricted()
 * @see bt_device_get_profile_restricted()
 */
typedef enum {
	BT_RESTRICTED_PROFILE_HFP_HS = 1,
	BT_RESTRICTED_PROFILE_A2DP,
} bt_restricted_profile_t;

/**
 * @ingroup CAPI_NETWORK_BLUETOOTH_ADAPTER_MODULE
 * @brief Enumerations of the authentication event types.
 * @since_tizen 3.0
 *
 */
typedef enum {
	BT_AUTH_KEYBOARD_PASSKEY_DISPLAY = 0, /**< PIN display event to user for entering PIN in keyboard */
	BT_AUTH_PIN_REQUEST,                  /**< Legacy PIN or PASSKEY request event */
	BT_AUTH_PASSKEY_CONFIRM_REQUEST,      /**< PASSKEY confirmation event to match PASSKEY in remote device */
} bt_authentication_type_info_e;

/**
 * @ingroup CAPI_NETWORK_BLUETOOTH_PROXIMITY_MODULE
 * @brief Proximity Profile Role
 * @since_tizen 3.0
 */
typedef enum {
	BT_PROXIMITY_REPORTER, /**< Proximity Profile Reporter role. */
	BT_PROXIMITY_MONITOR, /**< Proximity Profile Monitor role. */
} bt_proximity_role_t;

/**
 * @ingroup CAPI_NETWORK_BLUETOOTH_PROXIMITY_MODULE
 * @brief Proximity Profile Role
 * @since_tizen 3.0
 */
typedef enum {
	BT_PROXIMITY_LINKLOSS_ALERT = 0x01, /**< Proximity profile Link Loss alert property */
	BT_PROXIMITY_IMMEDIATE_ALERT = 0x02, /**< Proximity Profile Immediate alert property */
	BT_PROXIMITY_TX_POWER = 0x04, /**< Proximity Profile Immediate alert property */
} bt_proximity_property_t;

/**
 * @ingroup CAPI_NETWORK_BLUETOOTH_DEVICE_MODULE
 * @brief  Called when Trusted Profiles is changed.
 * @since_tizen 3.0
 *
 * @param[in]   result  The result of supported profile callback
 * @param[in]   remote_address  Address of remote device
 * @param[in]   trusted_profiles  Trusted profile FLAG
 * @param[in]   user_data  The user data passed from the callback registration function
 * @see bt_device_set_trusted_profile_cb()
 * @see bt_device_unset_trusted_profile_cb()
 */
typedef void (*bt_device_trusted_profiles_cb)
	(int result, char *remote_address, int trusted_profile, bool supported, bool trusted, void *user_data);

/**
 * @deprecated Deprecated since 4.0. Use bt_gatt_client_att_mtu_info_s instead.
 * @ingroup CAPI_NETWORK_BLUETOOTH_DEVICE_MODULE
 * @brief Attribute protocol MTU change information structure.
 * @since_tizen 3.0
 *
 * @see bt_device_att_mtu_changed_cb()
 */
typedef struct {
	char *remote_address;	/**< The address of remote device */
	unsigned int mtu;		/** < MTU value */
	unsigned int status;		/** < request status*/
} bt_device_att_mtu_info_s;

/**
 * @deprecated Deprecated since 4.0. Use bt_gatt_client_att_mtu_changed_cb instead.
 * @ingroup CAPI_NETWORK_BLUETOOTH_DEVICE_MODULE
 * @brief  Called when the connection state is changed.
 * @since_tizen 3.0
 *
 * @param[in] connected The connection status: (@c true = connected, @c false = disconnected)
 * @param[in] conn_info The connection information
 * @param[in] user_data The user data passed from the callback registration function
 * @see bt_device_set_connection_state_changed_cb()
 * @see bt_device_unset_connection_state_changed_cb()
 */
typedef void (*bt_device_att_mtu_changed_cb)(int result, bt_device_att_mtu_info_s *mtu_info, void *user_data);

/**
 * @ingroup CAPI_NETWORK_BLUETOOTH_GATT_SERVER_MODULE
 * @brief  Called when the remote device enables or disables the Notification/Indication for particular characteristics.
 *
 * @details By using this callback function, server can know notification state.
 *
 * @since_tizen 6.5
 * @remarks The @a server must not be freed by application.
 * @remarks The @a gatt_handle must not be freed by application.
 *
 * @param[in] remote_address The client device address
 * @param[in] notify Indicates whether the Notification/Indication is enabled or not
 * @param[in] server The GATT server handle
 * @param[in] gatt_handle The characteristic's GATT handle to be read
 * @param[in] user_data The user data passed from the registration function
 *
 * @see bt_gatt_server_set_read_value_requested_cb()
 */
typedef void (*bt_gatt_server_characteristic_notif_state_changed_cb) (const char *remote_address, bool notify,
			bt_gatt_server_h server, bt_gatt_h gatt_handle, void *user_data);

/**
 * @ingroup CAPI_NETWORK_BLUETOOTH_GATT_SERVER_MODULE
 * @brief Attribute protocol MTU change information structure.
 * @since_tizen 5.5
 *
 * @see bt_gatt_server_att_mtu_changed_cb()
 */
typedef struct {
	char *remote_address;	/**< The address of remote device */
	unsigned int mtu;		/**< MTU value */
	unsigned int status;		/**< Request status*/
} bt_gatt_server_att_mtu_info_s;

/**
 * @ingroup CAPI_NETWORK_BLUETOOTH_GATT_SERVER_MODULE
 * @brief This callback will be invoked if remote client has requested to change the MTU.
 * @since_tizen 5.5
 *
 * @remarks The @a mtu_info must not be freed by application.
 *                @a mtu_info can be used only inside the callback.
 *                If it's needed outside, make a copy.
 *
 * @param[in] server The handle of a GATT server
 * @param[in] mtu_info The MTU information
 * @param[in] user_data The user data passed from the callback registration function
 */
typedef void (*bt_gatt_server_att_mtu_changed_cb)(bt_gatt_server_h server,
		const bt_gatt_server_att_mtu_info_s *mtu_info,
		void *user_data);

/**
 * @ingroup CAPI_NETWORK_BLUETOOTH_DEVICE_MODULE
 * @brief PHY change information structure.
 * @since_tizen 8.0
 *
 * @see bt_gatt_server_phy_update_cb()
 * @see bt_gatt_server_phy_read_cb()
 * @see bt_gatt_client_phy_update_cb()
 * @see bt_gatt_client_phy_read_cb()
 */
typedef struct {
	char *remote_address;	/**< The address of remote device */
	int tx_phy;		/**< The transmitter PHY in use */
	int rx_phy;		/**< The receiver PHY in use */
	int status; 	/**< Status of the PHY operation */
} bt_gatt_phy_info_s;

/**
 * @ingroup CAPI_NETWORK_BLUETOOTH_GATT_SERVER_MODULE
 * @brief This callback will be invoked as a result of bt_gatt_server_set_phy(), or as a result of remote device changing the PHY.
 * @since_tizen 8.0
 *
 * @remarks The @a phy_info must not be freed by application.
 *                @a phy_info can be used only inside the callback.
 *                If it's needed outside, make a copy.
 *
 * @param[in] server The handle of a GATT server
 * @param[in] phy_info The PHY information
 * @param[in] user_data The user data passed from the callback registration function
 */
typedef void (*bt_gatt_server_phy_update_cb) (bt_gatt_server_h server,
		const bt_gatt_phy_info_s *phy_info, void *user_data);

/**
 * @ingroup CAPI_NETWORK_BLUETOOTH_GATT_SERVER_MODULE
 * @brief This callback will be invoked as a result of bt_gatt_server_read_phy().
 * @since_tizen 8.0
 *
 * @remarks The @a phy_info must not be freed by application.
 *                @a phy_info can be used only inside the callback.
 *                If it's needed outside, make a copy.
 *
 * @param[in] server The handle of a GATT server
 * @param[in] phy_info The PHY information
 * @param[in] user_data The user data passed from the callback registration function
 */
typedef void (*bt_gatt_server_phy_read_cb) (bt_gatt_server_h server,
		const bt_gatt_phy_info_s *phy_info, void *user_data);

/**
 * @ingroup CAPI_NETWORK_BLUETOOTH_GATT_CLIENT_MODULE
 * @brief This callback will be invoked as a result of bt_gatt_client_set_phy(), or as a result of remote device changing the PHY.
 * @since_tizen 8.0
 *
 * @remarks The @a phy_info must not be freed by application.
 *                @a phy_info can be used only inside the callback.
 *                If it's needed outside, make a copy.
 *
 * @param[in] client The created GATT client's handle
 * @param[in] phy_info The PHY information
 * @param[in] user_data The user data passed from the callback registration function
 */
typedef void (*bt_gatt_client_phy_update_cb) (bt_gatt_client_h client,
		const bt_gatt_phy_info_s *phy_info, void *user_data);

/**
 * @ingroup CAPI_NETWORK_BLUETOOTH_GATT_CLIENT_MODULE
 * @brief This callback will be invoked as a result of bt_gatt_client_read_phy().
 * @since_tizen 8.0
 *
 * @remarks The @a phy_info must not be freed by application.
 *                @a phy_info can be used only inside the callback.
 *                If it's needed outside, make a copy.
 *
 * @param[in] client The created GATT client's handle
 * @param[in] phy_info The PHY information
 * @param[in] user_data The user data passed from the callback registration function
 */
typedef void (*bt_gatt_client_phy_read_cb) (bt_gatt_client_h client,
		const bt_gatt_phy_info_s *phy_info, void *user_data);

/**
 * @ingroup CAPI_NETWORK_BLUETOOTH_GATT_MODULE
 * @brief Called when the connection state is changed.
 *
 * @details This callback is called when the connection state is changed.
 * When you called bt_gatt_client_connect() or bt_gatt_client_disconnect(), this callback is also called with error result even though these functions fail.
 *
 * @since_tizen 7.0
 * @param[in] result The result of changing the connection state.
 * @param[in] connected The state to be changed, true means connected state, Otherwise, false.
 * @param[in] remote_address The remote_address
 * @param[in] user_data The user data passed from the callback registration function.
 *
 * @see bt_gatt_client_connect()
 * @see bt_gatt_client_disconnect()
 */
typedef void (*bt_gatt_client_connection_state_changed_cb)(int result, bool connected, const char *remote_address, void *user_data);

/**
 * @ingroup CAPI_NETWORK_BLUETOOTH_GATT_MODULE
 * @brief Called when the connection state is changed.
 *
 * @details This callback is called when the connection state is changed.
 * When you called bt_gatt_client_connect() or bt_gatt_client_disconnect(), this callback is also called with error result even though these functions fail.
 *
 * @since_tizen 7.0
 * @param[in] result The result of changing the connection state.
 * @param[in] connected The state to be changed, true means connected state, Otherwise, false.
 * @param[in] remote_address The remote_address
 * @param[in] user_data The user data passed from the callback registration function.
 *
 * @see bt_gatt_client_connect()
 * @see bt_gatt_client_disconnect()
 * @see bt_gatt_server_set_connection_state_changed_cb()
 * @see bt_gatt_server_unset_connection_state_changed_cb()
 */
typedef void(*bt_gatt_server_connection_state_changed_cb)(int result, bool connected, const char *remote_address, void *user_data);

/**
 * @ingroup CAPI_NETWORK_BLUETOOTH_PROXIMITY_MODULE
 * @brief The handle of a Proximity Monitor client handle which is associated with a remote device.
 * @since_tizen 4.0
 */
typedef void *bt_proximity_monitor_h;

/**
 * @ingroup CAPI_NETWORK_BLUETOOTH_PROXIMITY_MODULE
 * @brief The handle of a Proximity Reporter server.
 * @since_tizen 4.0
 */
typedef void *bt_proximity_reporter_h;

/**
 * @ingroup CAPI_NETWORK_BLUETOOTH_PROXIMITY_MODULE
 * @brief  Called when the Proximity profile reporter property is changed.
 * @since_tizen 4.0
 *
 * @param[in] result callback result
 * @param[in] remote_address remote device address on which property is changed
 * @param[in] service_type The property type for which the value is changed
 * @param[in] prop_value The property value changed
 * @param[in] user_data The user data passed from the callback registration function
 * @see bt_proximity_reporter_set_property_changed_cb()
 * @see bt_proximity_reporter_unset_property_changed_cb()
 */
typedef void (*bt_proximity_reporter_property_changed_cb)
		(int result, const char *remote_address, int service_type, int prop_value, void *user_data);

/**
 * @ingroup CAPI_NETWORK_BLUETOOTH_PROXIMITY_MODULE
 * @brief  Called when the Proximity monitor profile connection state is changed.
 * @since_tizen 4.0
 *
 * @param[in] result callback result
 * @param[in] remote_address remote device address to which the device is connected
 * @param[in] monitor proximity profile monitor handle
 * @param[in] connected The connection status: (@c true = connected, @c false = disconnected)
 * @param[in] supported_service supported services of proximity profile
 * @param[in] user_data The user data passed from the callback registration function
 * @see bt_proximity_monitor_set_connection_state_changed_cb()
 * @see bt_proximity_monitor_unset_connection_state_changed_cb()
 */
typedef void (*bt_proximity_monitor_connection_state_changed_cb)
		(int result, const char *remote_address, bt_proximity_monitor_h monitor, bool connected, int supported_service, void *user_data);

/**
 * @ingroup CAPI_NETWORK_BLUETOOTH_PROXIMITY_MODULE
 * @brief  Called when the Proximity reporter profile connection state is changed.
 * @since_tizen 4.0
 *
 * @param[in] result callback result
 * @param[in] remote_address remote device address to which the device is connected
 * @param[in] reporter proximity profile reporter handle
 * @param[in] connected The connection status: (@c true = connected, @c false = disconnected)
 * @param[in] supported_service supported services of proximity profile
 * @param[in] user_data The user data passed from the callback registration function
 * @see bt_proximity_reporter_set_connection_state_changed_cb()
 * @see bt_proximity_reporter_unset_connection_state_changed_cb()
 */
typedef void (*bt_proximity_reporter_connection_state_changed_cb)
		(int result, const char *remote_address, bt_proximity_reporter_h reporter, bool connected, int supported_service, void *user_data);


/**
 * @internal
 * @brief Various TDS transport states.
 * @since_tizen 3.0
 */
typedef enum {
	BT_TDS_TRANSPORT_STATE_OFF,	/**< Transport is currently in OFF state */
	BT_TDS_TRANSPORT_STATE_ON,	/**< Transport is currently in ON state */
	BT_TDS_TRANSPORT_STATE_UNAVAILABLE,	/**< Transport is temporarily unavailable */
} bt_tds_transport_state_e;

/**
 * @internal
 * @brief Various TDS transports.
 * @since_tizen 3.0
 */
typedef enum {
	BT_TDS_TRANSPORT_BT = 0x01,		/**< Transport BR-EDR */
	BT_TDS_TRANSPORT_CUSTOM,		/**< Transport custom */
	BT_TDS_TRANSPORT_WIFI_NAN,		/**< Wi-Fi Alliance Neighbor Awareness Networking */
	BT_TDS_TRANSPORT_WIFI_SVC_ADV,	/**< Wi-Fi Alliance Service Advertisement */
	/* ... */
	BT_TDS_TRANSPORT_INVALID
} bt_tds_transport_e;

/**
 * @internal
 * @brief The structure type of TDS transport data block
 * @since_tizen 3.0
 */
typedef struct {
	bt_tds_transport_e transport;
	bt_tds_transport_state_e state;
	bool is_data_complete;
	char *data;
	int length;
} tds_transport_data_s;

/**
 * @internal
 * @brief The structure type of list of TDS transport data block
 * @since_tizen 3.0
 */
typedef struct {
	int num_transport_block;            /**< Number of Transport Data Blocks */
	tds_transport_data_s **data;        /**< Array of Transport Data Block */
} bt_tds_transport_block_list_s;

/**
 * @deprecated Depricated since 5.5
 * @internal
 * @ingroup CAPI_NETWORK_BLUETOOTH_ADAPTER_MODULE
 * @brief Called when remote device requests authentication.
 * @since_tizen 3.0
 * @param[in] result
 * @param[in] auth_type
 *             typedef enum {
 *              BT_AUTH_KEYBOARD_PASSKEY_DISPLAY = 0, : PIN display event to user for entering PIN in keyboard
 *              BT_AUTH_PIN_REQUEST,                  : Legacy PIN or PASSKEY request event
 *              BT_AUTH_PASSKEY_CONFIRM_REQUEST,      : PASSKEY confirmation event to match PASSKEY in remote device
 *             } bt_authentication_type_info_e;
 * @param[in] device_name  Name of the remote device
 * @param[in] remote_addr  Remote BD address
 * @param[in] pass_key     PASSKEY string
 *            PASSKEY string is valid only if authentication types are following
 *             a/ BT_AUTH_KEYBOARD_PASSKEY_DISPLAY
 *             b/ BT_AUTH_PASSKEY_CONFIRM_REQUEST
 *            pass_key string will be invalid if authentication event is of type BT_AUTH_PIN_REQUEST
 *            as this event indicates that user MUST enter PIN or PASSKEY and perform authentication.
 *
 *            Upon receiving BT_AUTH_KEYBOARD_PASSKEY_DISPLAY event, user should enter PASSKEY in keyboard
 *            Application can also call bt_device_cancel_bonding() Upon receiving BT_AUTH_KEYBOARD_PASSKEY_DISPLAY
 *            event which will fail the on-going pairing with remote device.
 * @param[in] user_data The user data passed from the callback registration function
 * @see bt_adapter_set_authentication_req_cb()
 */
typedef void (*bt_adapter_authentication_req_cb)(int result, bt_authentication_type_info_e auth_type,
						char *device_name, char *remote_addr,
						char *pass_key, void *user_data);

/**
 * @internal
 * @ingroup CAPI_NETWORK_BLUETOOTH_ADAPTER_MODULE
 * @brief Called when remote device requests authentication and direction of pairing request (incoming or not)
 * @since_tizen 5.5
 * @param[in] incoming     Direction of pairing. It is set to true if the pairing request is incoming.
 * @param[in] auth_type    Event whose occurence has invoked this callback
 *             typedef enum {
 *              BT_AUTH_KEYBOARD_PASSKEY_DISPLAY = 0, : PIN display event to user for entering PIN in keyboard
 *              BT_AUTH_PIN_REQUEST,                  : Legacy PIN or PASSKEY request event
 *              BT_AUTH_PASSKEY_CONFIRM_REQUEST,      : PASSKEY confirmation event to match PASSKEY in remote device
 *             } bt_authentication_type_info_e;
 * @param[in] device_name  Name of the remote device
 * @param[in] remote_addr  Remote BD address
 * @param[in] pass_key     PASSKEY string
 *            PASSKEY string is valid only if authentication types are following
 *             a/ BT_AUTH_KEYBOARD_PASSKEY_DISPLAY
 *             b/ BT_AUTH_PASSKEY_CONFIRM_REQUEST
 *            pass_key string will be invalid if authentication event is of type BT_AUTH_PIN_REQUEST
 *            as this event indicates that user MUST enter PIN or PASSKEY and perform authentication.
 *
 *            Upon receiving BT_AUTH_KEYBOARD_PASSKEY_DISPLAY event, user should enter PASSKEY in keyboard
 *            Application can also call bt_device_cancel_bonding() Upon receiving BT_AUTH_KEYBOARD_PASSKEY_DISPLAY
 *            event which will fail the on-going pairing with remote device.
 * @param[in] user_data The user data passed from the callback registration function
 * @see bt_adapter_set_authentication_request_cb()
 */

typedef void (*bt_adapter_authentication_request_cb)(bool incoming, bt_authentication_type_info_e auth_type,
                                                char *device_name, char *remote_addr,
                                                char *pass_key, void *user_data);

/**
 * @ingroup CAPI_NETWORK_BLUETOOTH_AUDIO_AG_MODULE
 * @brief  Called when a XSAT vendor command is transmitted from Hands-Free.
 * @since_tizen 2.3
 *
 * @param[in] command  The XSAT vendor command transmitted from Hands-Free
 * @param[in] user_data The user data passed from the callback registration function
 * @see bt_ag_set_vendor_cmd_cb()
 * @see bt_ag_unset_vendor_cmd_cb()
 */
typedef void (*bt_ag_vendor_cmd_cb) (char *command, void *user_data);

/**
 * @internal
 * @brief The handle of a Transport Discover Service Provider.
 * @since_tizen 3.0
 */
typedef void *bt_tds_provider_h;

/**
 * @internal
 * @brief Called when remote device requests transport activation
 * @since_tizen 3.0
 * @param[in] remote_bd_addr Remote device address
 * @param[in] transport transport to be activated
 * @param[in] buf transport activation request data buffer
 * @param[in] len transport activation request data buffer length
 * @param[in] user_data The user data passed from the callback registration function
 *
 * @see bt_tds_set_transport_activation_requested_cb()
 */
typedef void (*bt_tds_activation_requested_cb)(char *remote_bd_addr,
	bt_tds_transport_e transport, unsigned char *buf, int len, void *user_data);

/**
 * @ingroup CAPI_NETWORK_BLUETOOTH_ADAPTER_LE_MODULE
 * @brief The handle of a TDS Seeker client which is associated with a remote TDS provider
 * @since_tizen 3.0
 */
typedef void *bt_tds_seeker_h;

/**
 * @internal
 * @since_tizen 3.0
 * @brief TDS Seeker profile Scan result callback containing filtered TDS service information and
 * complete LE discovery informations
 */
typedef void (*bt_tds_provider_scan_result_cb)(int result, const char *remote_address,
		bt_tds_transport_block_list_s *info, bt_adapter_le_device_scan_result_info_s *scan_info,
		void *user_data);

/**
 * @internal
 * @since_tizen 3.0
 * @brief TDS Seeker profile Connection State changed callback which is associated with a remote TDS provider
 */
typedef void (*bt_tds_seeker_connection_state_changed_cb)
		(int result, const char *remote_address, bt_tds_seeker_h seeker, bool connected, void *user_data);

/**
 * @internal
 * @since_tizen 3.0
 * @brief TDS Seeker profile complete TDS data block read callback from remote TDS provider
 */
typedef void (*bt_tds_seeker_complete_transport_data_cb)
		(int result, const char *remote_address, bt_tds_transport_block_list_s *info, void *user_data);

/**
 * @internal
 * @since_tizen 3.0
 * @brief TDS Seeker profile TDS Control Point Activation request callback which is associated with remote TDS provider
 */
typedef void (*bt_tds_control_point_activation_indication_cb)
		(int result, const char *remote_address, unsigned char *data, int data_length, void *user_data);

/**
 * @internal
 * @since_tizen 4.0
 * @brief  Called when OTP Server is Enabled or Disabled.
 */
typedef void (*bt_otp_server_state_changed_cb)(int result, bool connected);


/**
 * @ingroup CAPI_NETWORK_BLUETOOTH_ADAPTER_LE_MODULE
 * @brief The handle of a OTP client which is associated with a remote OTP Server
 * @since_tizen 4.0
 */
typedef void *bt_otp_client_h;

/**
 * @internal
 * @brief The structure type of OTP object metadata
 * @since_tizen 4.0
 */
typedef struct {
	unsigned long long id;
	char *name;
} otp_object_metadata_s;

/**
 * @internal
 * @brief The structure type of list of OTP discovered objects
 * @since_tizen 4.0
 */
typedef struct {
	int num_objects;            /**< Number of Objects Discovered */
	otp_object_metadata_s **data;	/**< Array of Discovered Object MetaData */
} bt_otp_object_list_s;

/**
 * @internal
 * @since_tizen 4.0
 * @brief OTP Client profile Connection State changed callback which is associated with a remote OTP Server
 */
typedef void (*bt_otp_client_state_changed_cb)
		(int result, const char *remote_address, bt_otp_client_h otp_client, bool connected, void *user_data);

/**
 * @internal
 * @since_tizen 4.0
 * @brief OTP Client profile remote object discovery callback
 */
typedef void (*bt_otp_client_object_discovery_cb)
		(int result, const char *remote_address, bt_otp_object_list_s *obj_list, void *user_data);

/**
 * @internal
 * @since_tizen 4.0
 * @brief OTP Client profile select object callback
 */
typedef void (*bt_otp_client_object_select_cb)
		(int result, const char *remote_address, unsigned long long obj_id, void *user_data);

/**
 * @internal
 * @since_tizen 4.0
 * @brief OTP Client profile remote object read complete callback
 */
typedef void (*bt_otp_client_read_object_complete_cb)
		(int result, const char *remote_address, char *file_path, void *user_data);

/**
 * @internal
 * @since_tizen 4.0
 * @brief OTP Client profile object create callback
 */
typedef void (*bt_otp_client_object_create_cb)
		(int result, const char *remote_address, unsigned long long obj_id, void *user_data);

/**
 * @internal
 * @since_tizen 4.0
 * @brief OTP Client profile object create callback
 */
typedef void (*bt_otp_client_object_write_cb)
		(int result, const char *remote_address, unsigned int length, void *user_data);

/**
 * @internal
 * @since_tizen 4.0
 * @brief OTP Client profile object execute callback
 */
typedef void (*bt_otp_client_object_execute_cb)
		(int result, const char *remote_address, unsigned long long obj_id, void *user_data);

/**
 * @internal
 * @since_tizen 4.0
 * @brief OTP Client profile object delete callback
 */
typedef void (*bt_otp_client_object_delete_cb)
		(int result, const char *remote_address, unsigned long long obj_id, void *user_data);

/**
 * @}
 */

/**
 * @ingroup CAPI_NETWORK_BLUETOOTH_ADAPTER_LE_MODULE
 * @brief  Enumerations of the Bluetooth advertising state for HRP.
 * @since_tizen 2.3
 */
typedef enum {
	BT_HRP_SENSOR_ADVERTISING_STOPPED = 0x00, /**< Bluetooth advertising is stopped */
	BT_HRP_SENSOR_ADVERTISING_STARTED, /**< Bluetooth advertising is started */
} bt_hrp_sensor_advertising_state_e;

/**
 * @internal tizen 4.0
 * @brief HRP Sensor Advertisement state change callback
 */

typedef void (*bt_hrp_sensor_advertising_state_changed_cb)(int result,
		bt_hrp_sensor_advertising_state_e adv_state, void *user_data);


/* Body Sensor Enumeration
*/
typedef enum {
	BT_BSL_NOTSUPPORTED = -1,
	BT_BSL_OTHER = 0,
	BT_BSL_CHEST,
	BT_BSL_WRIST,
	BT_BSL_FINGER,
	BT_BSL_HAND,
	BT_BSL_EARLOBE,
	BT_BSL_FOOT,
} bt_body_sensor_location_e;

/**
 * @internal
 * @brief The handle of a HRP Collector client which is associated with a remote HRP sensor
 * @since_tizen 4.0
 */
typedef void *bt_hrp_collector_h;

/**
 * @ingroup CAPI_NETWORK_BLUETOOTH_GATT_CLIENT_MODULE
 * @brief  Called when a value of a watched characteristic's GATT handle has been changed in HRP.
 * @since_tizen 2.3.1
 *
 * @remarks After this function is returned, a changed value is automatically \n
 * applied to @a characteristic. Before that, @a characteristic has an old value.
 *
 * @param[in] characteristic The characteristic's GATT handle of which value change is informed. It has an old value.
 * @param[in] value The new value
 * @param[in] len The length of @a value
 * @param[in] user_data The user data passed from the registering function
 *
 * @see bt_gatt_client_set_characteristic_value_changed_cb()
 */
typedef void (*bt_hrp_collector_heart_rate_value_changed_cb) (bt_gatt_client_h characteristic,
		unsigned short hr_value, void *user_data);

/**
 * @ingroup CAPI_NETWORK_BLUETOOTH_GATT_CLIENT_MODULE
 * @brief type information structure ,define what type of reequest is made.
 * @since_tizen 4.0
 *
 * @see bt_gatt_client_att_mtu_changed_cb()
 */

typedef enum {
	HEART_RATE = 1,
	BODY_SENSOR_LOCATION,
	HEART_CONTROL_POINT,
} bt_serv_char_type_t;

/**
 * @ingroup CAPI_NETWORK_BLUETOOTH_ADAPTER_LE_MODULE
 * @brief  Called when the LE advertisement callback for HRP sensor has been found.
 * @since_tizen 4.0
 *
 * @param[in] result The result of the LE scan
 * @param[in] info The information of the found LE advertisement.
 * @param[in] user_data The user data passed from the start function
 *
 *
 */
typedef void (*bt_hrp_collector_scan_result_cb)(int result,
		bt_adapter_le_device_scan_result_info_s *info, void *user_data);

/**
 * @ingroup CAPI_NETWORK_BLUETOOTH_GATT_CLIENT_MODULE
 * @brief  Called when the client request read operation on body sensor location has been completed.
 * @since_tizen 4
 *
 * @param[in] result The result of a request
 * @param[in] request_handle The requesting GATT handle
 * @param[in] type : Heart Rate Value , Body Sensor Location or Heart Control Point
 * @param[in] value : Int value corresponding to the type recieved .
 * @param[in] user_data The user data passed from the requesting function
 *
 */
typedef void (*bt_hrp_collector_bsl_read_completed_cb) (int result,
		bt_hrp_collector_h request_handle, bt_body_sensor_location_e location, void *user_data);

/**
 * @internal
 * @since_tizen 4.0
 * @brief HRP Collector profile Connection State changed callback which is associated with
 *	a remote HRP sensor.
 */
typedef void (*bt_hrp_collector_connection_state_changed_cb)
		(int result, const char *remote_address, bt_hrp_collector_h collector,
		  bool connected, void *user_data);

/**
 * @ingroup CAPI_NETWORK_BLUETOOTH_ADAPTER_LE_MODULE
 * @brief  Enumerations of the Bluetooth advertising Tx power level.
 * @since_tizen 5.5
 */
typedef enum {
	BT_ADAPTER_LE_ADVERTISING_TX_POWER_HIGH = 1,  /**< High transmission power level */
	BT_ADAPTER_LE_ADVERTISING_TX_POWER_MEDIUM = -7,  /**< Medium transmission power level */
	BT_ADAPTER_LE_ADVERTISING_TX_POWER_LOW = -15,  /**< Low transmission power level */
	BT_ADAPTER_LE_ADVERTISING_TX_POWER_ULTRA_LOW = -23  /**< Lowst transmission power level */
} bt_adapter_le_advertising_tx_power_level_e;

/* Mesh Start */
#define BT_MESH_MAX_UNPROVISIONED_DEVICE_SCAN_TIME 300
#define BT_MESH_NETWORK_NAME_STRING_MAX_LEN 100
#define BT_MESH_MAX_PUBISH_PERIOD_STEPS 0x3F
#define BT_MESH_UUID_STRING_LEN 32
#define BT_MESH_TOKEN_STRING_LEN 16

/* Intervals for retransmission of a Publish message: value * 50 msec */
#define BT_MESH_MAX_PUBISH_RETRANSMIT_INTERVAL_STEPS 0x1F

/* How many times, restransmission is allowed for a publish message */
#define BT_MESH_MAX_PUBISH_RETRANSMIT_COUNT 0x7

/**
 * @ingroup CAPI_NETWORK_BLUETOOTH_MESH_MODULE
 * @brief  Enumerations for the BT SIG Mesh Models
 * @since_tizen 6.0
 */
typedef enum {
	BT_MESH_MODEL_ID_CFG_SRV			  = 0x0000,
	BT_MESH_MODEL_ID_CFG_CLIENT			  = 0x0001,
	BT_MESH_MODEL_ID_HEALTH_SRV			  = 0x0002,
	BT_MESH_MODEL_ID_HEALTH_CLIENT			  = 0x0003,
	BT_MESH_MODEL_ID_GEN_ONOFF_SRV			  = 0x1000,
	BT_MESH_MODEL_ID_GEN_ONOFF_CLIENT		  = 0x1001,
	BT_MESH_MODEL_ID_GEN_LEVEL_SRV             	  = 0x1002,
	BT_MESH_MODEL_ID_GEN_LEVEL_CLI             	  = 0x1003,
	BT_MESH_MODEL_ID_GEN_DEF_TRANS_TIME_SRV	          = 0x1004,
	BT_MESH_MODEL_ID_GEN_DEF_TRANS_TIME_CLI		  = 0x1005,
	BT_MESH_MODEL_ID_GEN_POWER_ONOFF_SRV		  = 0x1006,
	BT_MESH_MODEL_ID_GEN_POWER_ONOFF_SETUP_SRV 	  = 0x1007,
	BT_MESH_MODEL_ID_GEN_POWER_ONOFF_CLI       	  = 0x1008,
	BT_MESH_MODEL_ID_GEN_POWER_LEVEL_SRV      	  = 0x1009,
	BT_MESH_MODEL_ID_GEN_POWER_LEVEL_SETUP_SRV	  = 0x100a,
	BT_MESH_MODEL_ID_GEN_POWER_LEVEL_CLI       	  = 0x100b,
	BT_MESH_MODEL_ID_GEN_BATTERY_SRV           	  = 0x100c,
	BT_MESH_MODEL_ID_GEN_BATTERY_CLI           	  = 0x100d,
	BT_MESH_MODEL_ID_GEN_LOCATION_SRV          	  = 0x100e,
	BT_MESH_MODEL_ID_GEN_LOCATION_SETUPSRV     	  = 0x100f,
	BT_MESH_MODEL_ID_GEN_LOCATION_CLI		  = 0x1010,
	BT_MESH_MODEL_ID_GEN_ADMIN_PROP_SRV        	  = 0x1011,
	BT_MESH_MODEL_ID_GEN_MANUFACTURER_PROP_SRV 	  = 0x1012,
	BT_MESH_MODEL_ID_GEN_USER_PROP_SRV         	  = 0x1013,
	BT_MESH_MODEL_ID_GEN_CLIENT_PROP_SRV       	  = 0x1014,
	BT_MESH_MODEL_ID_GEN_PROP_CLI              	  = 0x1015,
	BT_MESH_MODEL_ID_SENSOR_SRV                	  = 0x1100,
	BT_MESH_MODEL_ID_SENSOR_SETUP_SRV          	  = 0x1101,
	BT_MESH_MODEL_ID_SENSOR_CLI                	  = 0x1102,
	BT_MESH_MODEL_ID_TIME_SRV			  = 0x1200,
	BT_MESH_MODEL_ID_TIME_SETUP_SRV			  = 0x1201,
	BT_MESH_MODEL_ID_TIME_CLI			  = 0x1202,
	BT_MESH_MODEL_ID_SCENE_SRV			  = 0x1203,
	BT_MESH_MODEL_ID_SCENE_SETUP_SRV		  = 0x1204,
	BT_MESH_MODEL_ID_SCENE_CLI			  = 0x1205,
	BT_MESH_MODEL_ID_SCHEDULER_SRV			  = 0x1206,
	BT_MESH_MODEL_ID_SCHEDULER_SETUP_SRV		  = 0x1207,
	BT_MESH_MODEL_ID_SCHEDULER_CLI			  = 0x1208,
	BT_MESH_MODEL_ID_LIGHT_LIGHTNESS_SRV  		  = 0x1300,
	BT_MESH_MODEL_ID_LIGHT_LIGHTNESS_SETUP_SRV 	  = 0x1301,
	BT_MESH_MODEL_ID_LIGHT_LIGHTNESS_CLI       	  = 0x1302,
	BT_MESH_MODEL_ID_LIGHT_CTL_SRV             	  = 0x1303,
	BT_MESH_MODEL_ID_LIGHT_CTL_SETUP_SRV       	  = 0x1304,
	BT_MESH_MODEL_ID_LIGHT_CTL_CLI             	  = 0x1305,
	BT_MESH_MODEL_ID_LIGHT_CTL_TEMP_SRV        	  = 0x1306,
	BT_MESH_MODEL_ID_LIGHT_HSL_SRV             	  = 0x1307,
	BT_MESH_MODEL_ID_LIGHT_HSL_SETUP_SRV       	  = 0x1308,
	BT_MESH_MODEL_ID_LIGHT_HSL_CLI             	  = 0x1309,
	BT_MESH_MODEL_ID_LIGHT_HSL_HUE_SRV         	  = 0x130a,
	BT_MESH_MODEL_ID_LIGHT_HSL_SAT_SRV         	  = 0x130b,
	BT_MESH_MODEL_ID_LIGHT_XYL_SRV             	  = 0x130c,
	BT_MESH_MODEL_ID_LIGHT_XYL_SETUP_SRV       	  = 0x130d,
	BT_MESH_MODEL_ID_LIGHT_XYL_CLI             	  = 0x130e,
	BT_MESH_MODEL_ID_LIGHT_LC_SRV              	  = 0x130f,
	BT_MESH_MODEL_ID_LIGHT_LC_SETUPSRV         	  = 0x1310,
	BT_MESH_MODEL_ID_LIGHT_LC_CLI              	  = 0x1311,
	BT_MESH_MODEL_MAX		              	  = 0xFFFF,
} bt_mesh_sig_model_id_e;

/**
 * @ingroup CAPI_NETWORK_BLUETOOTH_MESH_MODULE
 * @brief  Enumerations of the Provisioning error codes.
 * @since_tizen 6.0
 */
typedef enum {
	BT_MESH_PROV_ERR_INTERNAL               = -1,
	BT_MESH_PROV_ERR_SUCCESS                = 0x00,
	BT_MESH_PROV_ERR_INVALID_PDU            = 0x01,
	BT_MESH_PROV_ERR_INVALID_FORMAT         = 0x02,
	BT_MESH_PROV_ERR_UNEXPECTED_PDU         = 0x03,
	BT_MESH_PROV_ERR_CONFIRM_FAILED         = 0x04,
	BT_MESH_PROV_ERR_INSUF_RESOURCE         = 0x05,
	BT_MESH_PROV_ERR_DECRYPT_FAILED         = 0x06,
	BT_MESH_PROV_ERR_UNEXPECTED_ERR         = 0x07,
	BT_MESH_PROV_ERR_CANT_ASSIGN_ADDR       = 0x08,
	/* Internally generated  by Stack */
	BT_MESH_PROV_ERR_TIMEOUT                = 0xFF,
} bt_mesh_prov_error_codes_e;

/**
 * @ingroup CAPI_NETWORK_BLUETOOTH_MESH_MODULE
 * @brief Enumerations of the Unprovisioned scanning  state of Provisioner device.
 * @since_tizen 6.0
 */
typedef enum {
	BT_MESH_SCANNING_STARTED, /**< Scanning is started */
	BT_MESH_SCANNING_FINISHED, /**< Scanning is finished */
	BT_MESH_SCAN_DEVICE_FOUND, /**< The remote Unprovisioned device is found */
} bt_mesh_scanning_state_e;

/**
 * @ingroup CAPI_NETWORK_BLUETOOTH_MESH_MODULE
 * @brief Enumerations of 2 octet OOB Information field,
 * per Table 3.54, Mesh profile Specification
 * @since_tizen 6.0
 */
typedef enum {
	BT_MESH_OOB_INFO_OTHER				= 0x01,
	BT_MESH_OOB_INFO_ELECTRONIC_OR_URI		= 0x02,
	BT_MESH_OOB_INFO_2D_MACHINE_READABLE_CODE	= 0x04,
	BT_MESH_OOB_INFO_BAR_CODE			= 0x08,
	BT_MESH_OOB_INFO_NEAR_FIELD_COMMUNICATION	= 0x10,
	BT_MESH_OOB_INFO_NUMBER				= 0x20,
	BT_MESH_OOB_INFO_STRING				= 0x40,
	/* bit 7 ~ 10 : RFU */
	BT_MESH_OOB_INFO_ON_BOX				= 0x800,
	BT_MESH_OOB_INFO_INSIDE_BOX			= 0x1000,
	BT_MESH_OOB_INFO_ON_PIECE_OF_PAPER		= 0x2000,
	BT_MESH_OOB_INFO_INSIDE_MANUAL			= 0x4000,
	BT_MESH_OOB_INFO_ON_DEVICE			= 0x8000,
} bt_mesh_oob_info_e;

/**
 * @ingroup CAPI_NETWORK_BLUETOOTH_MESH_MODULE
 * @brief Enumerations of 2 octet OUT OOB Actions
 * per Table 5.22, Mesh profile Specification
 * @since_tizen 6.0
 */
typedef enum {
	BT_MESH_OUT_OOB_METHOD_BLINK		= 0x01,
	BT_MESH_OUT_OOB_METHOD_BEEP		= 0x02,
	BT_MESH_OUT_OOB_METHOD_VIBRATE		= 0x04,
	BT_MESH_OUT_OOB_METHOD_NUMERIC		= 0x08,
	BT_MESH_OUT_OOB_METHOD_ALPHANUMERIC	= 0x10,
	/* bit 5 ~ 15 : RFU */
} bt_mesh_output_oob_action_e;

/**
 * @ingroup CAPI_NETWORK_BLUETOOTH_MESH_MODULE
 * @brief Enumerations of 2 octet IN OOB Actions
 * per Table 5.24, Mesh profile Specification
 * @since_tizen 6.0
 */
typedef enum {
	BT_MESH_IN_OOB_METHOD_PUSH		= 0x01,
	BT_MESH_IN_OOB_METHOD_TWIST		= 0x02,
	BT_MESH_IN_OOB_METHOD_NUMERIC		= 0x04,
	BT_MESH_IN_OOB_METHOD_ALPHANUMERIC	= 0x08,
	/* bit 4 ~ 15 : RFU */
} bt_mesh_input_oob_action_e;

/**
 * @ingroup CAPI_NETWORK_BLUETOOTH_MESH_MODULE
 * @brief Enumerations of Authentication Types(Input, Output, OOB Keys)
 * @since_tizen 6.0
 */
typedef enum {
	/*< Output authentication request types */
	BT_MESH_AUTH_ALPHANUMERIC_DISPLAY = 0,
	BT_MESH_AUTH_NUMERIC_DISPLAY,
	BT_MESH_AUTH_PUSH_COUNT_DISPLAY,
	BT_MESH_AUTH_TWIST_COUNT_DISPLAY,

	/*< Input authentication request types */
	BT_MESH_AUTH_REQ_ALPHANUMERIC_INPUT,
	BT_MESH_AUTH_REQ_NUMERIC_INPUT,
	BT_MESH_AUTH_REQ_BLINK_COUNT_INPUT,
	BT_MESH_AUTH_REQ_BEEP_COUNT_INPUT,
	BT_MESH_AUTH_REQ_VIBRATE_COUNT_INPUT,

	/**< OOB Key Inputs */
	BT_MESH_AUTH_REQ_OOB_PUBLIC_KEY_INPUT,
	BT_MESH_AUTH_REQ_OOB_STATIC_KEY_INPUT
} bt_mesh_authentication_type_e;

/**
 * @ingroup CAPI_NETWORK_BLUETOOTH_MESH_MODULE
 * @brief  Enumerations for the Step Resolution for Period of Publication message
 * @since_tizen 6.0
 */
typedef enum {
	BT_MESH_PUBLICATION_STEP_RES_100_MSEC,
	BT_MESH_PUBLICATION_STEP_RES_1_SECOND,
	BT_MESH_PUBLICATION_STEP_RES_10_SECONDS,
	BT_MESH_PUBLICATION_STEP_RES_10_MINS,
} bt_mesh_publish_resolution_e;

/**
 * @ingroup CAPI_NETWORK_BLUETOOTH_MESH_MODULE
 * @brief  Enumerations for the Model Subscription operations
 * @since_tizen 6.0
 */
typedef enum {
	BT_MESH_MODEL_SUBSCRIPTION_ADD,
	BT_MESH_MODEL_SUBSCRIPTION_DELETE,
	BT_MESH_MODEL_SUBSCRIPTION_DELETE_ALL,
	BT_MESH_MODEL_SUBSCRIPTION_OVERWRITE,
} bt_mesh_model_subscription_op_e;

/**
 * @ingroup CAPI_NETWORK_BLUETOOTH_MESH_MODULE
 * @brief  Enumerations for the Mesh features
 * per Table 4.3, Mesh profile Specification
 * @since_tizen 6.0
 */
typedef enum {
	BT_MESH_FEATURE_RELAY		= 0x01,
	BT_MESH_FEATURE_PROXY		= 0x02,
	BT_MESH_FEATURE_FRIEND		= 0x04,
	BT_MESH_FEATURE_LOWPOWER	= 0x08,
	/* bit 4 ~ 15 : RFU */
} bt_mesh_features_e;

/**
 * @ingroup CAPI_NETWORK_BLUETOOTH_MESH_MODULE
 * @brief  Enumerations for the TTL operation for a node in a network
 * @since_tizen 6.0
 */
typedef enum {
	BT_MESH_TTL_GET,
	BT_MESH_TTL_SET,
} bt_mesh_node_ttl_operation_e;


typedef enum {
	BT_MESH_NODE_KEY_ADD,
	BT_MESH_NODE_KEY_UPDATE,
	BT_MESH_NODE_KEY_DELETE,
} bt_mesh_node_key_configuration_e;

/**
 * @ingroup CAPI_NETWORK_BLUETOOTH_MESH_MODULE
 * @brief  structure of Model ID
 * @since_tizen 6.0
 */
typedef struct {
	uint16_t company_id; /** < 0xFFFF in case of BT SIG Model ID */
	uint16_t model_id;
} bt_mesh_model_id_s;

/**
 * @ingroup CAPI_NETWORK_BLUETOOTH_MESH_MODULE
 * @brief  structure of Model Publication parameters
 * @since_tizen 6.0
 */
typedef struct {
	uint8_t ttl;
	uint8_t num_steps;
	bt_mesh_publish_resolution_e per_res;
	uint8_t retrans_cnt;
	uint8_t retrans_step;
	//uint16_t pub_addr;
} bt_mesh_model_pub_params_s;

/**
 * @ingroup CAPI_NETWORK_BLUETOOTH_MESH_MODULE
 * @brief  structure of Model Message parameters
 * @since_tizen 6.0
 */
typedef struct {
	uint16_t opcode;
	char *data;
} bt_mesh_model_msg_params_s;


/**
 * @ingroup CAPI_NETWORK_BLUETOOTH_MESH_MODULE
 * @brief The handle of a BLE Mesh provisioner or a Mesh Network instance.
 * @since_tizen 6.0
 */
typedef void *bt_mesh_network_h;

/**
 * @ingroup CAPI_NETWORK_BLUETOOTH_MESH_MODULE
 * @brief The handle of a BLE Mesh Network group.
 * @since_tizen 6.0
 */
typedef void *bt_mesh_group_h;

/**
 * @ingroup CAPI_NETWORK_BLUETOOTH_MESH_MODULE
 * @brief The handle of a BLE Mesh node (Local or Remote)
 * @since_tizen 6.0
 */
typedef void *bt_mesh_node_h;

/**
 * @ingroup CAPI_NETWORK_BLUETOOTH_MESH_MODULE
 * @brief The handle of a BLE Mesh element contained in a node.
 * @since_tizen 6.0
 */
typedef void *bt_mesh_element_h;

/**
 * @ingroup CAPI_NETWORK_BLUETOOTH_MESH_MODULE
 * @brief The handle of a BLE Mesh Model (BT SIG or vendor Model)
 * @since_tizen 6.0
 */
typedef void *bt_mesh_model_h;

/**
 * @ingroup CAPI_NETWORK_BLUETOOTH_MESH_MODULE
 * @brief The handle of a BLE Mesh Network or subnetwork Key.
 * @since_tizen 6.0
 */
typedef void *bt_mesh_netkey_h;

/**
 * @ingroup CAPI_NETWORK_BLUETOOTH_MESH_MODULE
 * @brief The handle of a BLE Mesh Application key.
 * @since_tizen 6.0
 */
typedef void *bt_mesh_appkey_h;

/**
 * @ingroup CAPI_NETWORK_BLUETOOTH_MESH_MODULE
 * @brief Structure of a BLE Mesh Scan result (From a Unprovisioned Device Beacon)
 * @since_tizen 6.0
 * @see bt_mesh_network_create()
 */
typedef struct {
	char *uuid;  /**< Unprovisioned Device UUID */
	int rssi;    /**< RSSI signal strength */
	bt_mesh_oob_info_e oob_info; /**< Indicates various method via which OOB Data can be exported by remote node */
	/* TODO Currently mesh daemon does not support  URI Hash data.
	   The structure will be expanded once it is available from stack */
} bt_mesh_scan_result_s;

/**
 * @ingroup CAPI_NETWORK_BLUETOOTH_MESH_MODULE
 * @brief Structure of a BLE Mesh Provisioner Capabilities
 * @since_tizen 6.0
 * @see bt_mesh_network_set_provisioning_capabilities()
 */
typedef struct {
	bool public_oob;			/**< Indicates Public Key for ECC Key exchange, can be fetched Out Of Band,  */
	bool static_oob;			/**< Indicates Static Key for authentication, can be fetched Out-Of-Band*/
	bt_mesh_output_oob_action_e out_oob;	/**< Indicates suport for bitmap combination of OUT-OOB actions */
	bt_mesh_input_oob_action_e  in_oob;	/**< Indicates suport for bitmap combination of IN-OOB actions */
} bt_mesh_provisioner_capabilities_s;

/**
 * @ingroup CAPI_NETWORK_BLUETOOTH_MESH_MODULE
 * @brief Structure of a BLE Mesh Unprovisioned Device Scan parameters
 * @since_tizen 6.0
 * @see bt_mesh_network_unprovisioned_device_scan()
 * @see MAX_UNPROVISIONED_DEVICE_SCAN_TIME
 */
typedef struct {
	uint16_t seconds; /**< Current maximum is 300 seconds or 5 minutes */
	/* More parameters to add in future */
} bt_mesh_scan_params_s;

/**
 * @ingroup CAPI_NETWORK_BLUETOOTH_MESH_MODULE
 * @brief Structure of a BLE Mesh Node feature
 * @since_tizen 6.0
 * @see bt_mesh_node_get_features()
 * @see MAX_UNPROVISIONED_DEVICE_SCAN_TIME
 */
typedef struct {
	uint16_t cid;			/**< Contains a 16-bit company identifier assigned by the Bluetooth SIG */
	uint16_t pid;			/**< Contains a 16-bit vendor-assigned product identifier */
	uint16_t vid;			/**< Contains a 16-bit vendor-assigned product version identifier*/
	uint16_t crpl;			/**< Contains a 16-bit value representing the minimum number of replay protection list entries in a device*/
	bt_mesh_features_e features;	/**< Bit map of Mesh features supported by the node */
} bt_mesh_node_features_s;

/* Mesh Callback Signatures */
typedef void (*bt_mesh_network_create_cb)
		(int result, bt_mesh_network_h network,
			const char *network_name, void *user_data);


typedef void (*bt_mesh_network_load_cb)
		(int result, bt_mesh_network_h network, const char *token,
			const char *network_name, void *user_data);


typedef void (*bt_mesh_network_scan_unprovisioned_device_result_cb)
		(int result, bt_mesh_network_h network,
			bt_mesh_scanning_state_e state,
				bt_mesh_scan_result_s *scan_res,
					void *user_data);

typedef void (*bt_mesh_network_device_provision_cb)
		(int result, bt_mesh_network_h network,
			const char *dev_uuid, void *user_data);

typedef void (*bt_mesh_join_complete_cb)
		(int result, bt_mesh_node_h node,
			void *user_data);

typedef void (*bt_mesh_authentication_request_cb)
		(int result, bt_mesh_authentication_type_e auth_type,
			char *auth_value, void *user_data);


typedef void (*bt_mesh_network_node_reset_cb)
		(int result, bt_mesh_network_h network,
			const char *dev_uuid, void *user_data);


typedef void (*bt_mesh_node_ttl_cb)
		(int result, bt_mesh_node_h node,
			bt_mesh_node_ttl_operation_e ttl_op,
				uint8_t ttl, void *user_data);

typedef void (*bt_mesh_node_discover_status_cb)
		(int result, bt_mesh_network_h network, bt_mesh_node_h node,
			void *user_data);

typedef void (*bt_mesh_node_features_cb)
		(int result, bt_mesh_node_h node,
			bt_mesh_node_features_s *features, void *user_data);

typedef bool (*bt_mesh_node_netkey_info_cb)
		(int result, bt_mesh_node_h node, int total,
			bt_mesh_netkey_h netkey,
				uint16_t netkey_index, void *user_data);

typedef bool (*bt_mesh_node_appkey_info_cb)
		(int result, bt_mesh_node_h node, int total,
			bt_mesh_netkey_h netkey, bt_mesh_appkey_h appkey,
				uint16_t appkey_index, void *user_data);

typedef bool (*bt_mesh_network_netkey_info_cb)
		(int result, bt_mesh_network_h network,
			int total, bt_mesh_netkey_h netkey,
				uint16_t netkey_index, void *user_data);

typedef bool (*bt_mesh_appkey_info_cb)
		(int result, bt_mesh_network_h network, int total,
			bt_mesh_netkey_h netkey, bt_mesh_appkey_h appkey,
				uint16_t appkey_index, void *user_data);

typedef bool (*bt_mesh_network_device_info_cb)
		(int result, bt_mesh_network_h network, int total,
			const char *dev_uuid, uint16_t primary_unicast,
				void *user_data);

typedef bool (*bt_mesh_node_element_info_cb)
		(int result, bt_mesh_node_h node, int total,
			bt_mesh_element_h element,
				int elem_index, uint16_t element_addr,
					void *user_data);

typedef bool (*bt_mesh_element_model_info_cb)
		(int result, bt_mesh_element_h element, int total,
			bt_mesh_model_h model, bt_mesh_model_id_s *model_id,
				void *user_data);

typedef void (*bt_mesh_appkey_status_cb)
		(int result, bt_mesh_node_key_configuration_e op,
			bt_mesh_node_h node, bt_mesh_netkey_h netkey,
				bt_mesh_appkey_h appkey, void *user_data);

typedef void (*bt_mesh_netkey_status_cb)
		(int result, bt_mesh_node_key_configuration_e op, bt_mesh_node_h node,
			bt_mesh_netkey_h netkey, void *user_data);

typedef void (*bt_mesh_model_bind_cb)
		(int result, bt_mesh_model_h model, bt_mesh_appkey_h appkey,
			void *user_data);

typedef void (*bt_mesh_model_unbind_cb)
		(int result, bt_mesh_model_h model, bt_mesh_appkey_h appkey,
			void *user_data);

typedef void (*bt_mesh_model_appkey_list_cb)
		(int result, bt_mesh_model_h model, int total,
			const GSList *appkeylist,
				void *user_data);

typedef void (*bt_mesh_model_msg_cb)
		(int result, bt_mesh_element_h element, bt_mesh_appkey_h appkey,
			bt_mesh_model_msg_params_s *msg, void *user_data);

typedef void (*bt_mesh_model_subscription_op_cb)
		(int result, bt_mesh_model_subscription_op_e op,
			bt_mesh_model_h model, bt_mesh_group_h group,
				void *user_data);


typedef bool (*bt_mesh_model_subscription_list_cb)
		(int result, bt_mesh_model_h model, int total,
			const GSList *sub_addr,
				void *user_data);

typedef void (*bt_mesh_model_publication_status_cb)
		(int result, bt_mesh_model_h model, bt_mesh_group_h group,
			bt_mesh_appkey_h appkey, void *user_data);

typedef bool (*bt_mesh_network_group_info_cb)
		(int result, bt_mesh_network_h network, int total,
			bt_mesh_group_h group, void *user_data);

/**
 * @ingroup CAPI_NETWORK_BLUETOOTH_AUDIO_HF_MODULE
 * @brief Samsung XSAT Vendor dependent command.
 */
typedef struct {
	int app_id; /**< The application ID */
	char *message; /**< Command message */
} bt_hf_vendor_dep_at_cmd_s;

/**
 * @ingroup CAPI_NETWORK_BLUETOOTH_AUDIO_HF_MODULE
 * @brief Called when a vendor command event happened from Hands-Free.
 * @remarks The @a meesage of cmd must be released with free() by you.
 *
 * @param[in] cmd  The information of vendor command
 * @param[in] user_data The user data passed from the callback registration function
 *
 * @see bt_hf_set_vendor_dep_cmd_event_cb()
 * @see bt_hf_unset_vendor_dep_cmd_event_cb()
 */
typedef void (*bt_hf_vendor_dep_cmd_event_cb) (bt_hf_vendor_dep_at_cmd_s cmd, void *user_data);

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* __TIZEN_NETWORK_BLUETOOTH_TYPE_INTERNAL_H__ */
