/*
 * vconf-internal-keys
 *
 * Copyright (c) 2000 - 2011 Samsung Electronics Co., Ltd. All rights reserved.
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

#ifndef __VCONF_INTERNAL_WIFI_KEYS_H__
#define __VCONF_INTERNAL_WIFI_KEYS_H__
/**
 *
 */
#define VCONFKEY_WIFI_STATE  "memory/wifi/state"

enum {
	VCONFKEY_WIFI_OFF = 0x00,
		VCONFKEY_WIFI_UNCONNECTED,
		VCONFKEY_WIFI_CONNECTED,
		VCONFKEY_WIFI_TRANSFER,
		VCONFKEY_WIFI_STATE_MAX
};

/**
 *
 */
#define VCONFKEY_WIFI_TRANSFER_STATE  "memory/wifi/transfer_state"

enum {
	VCONFKEY_WIFI_TRANSFER_STATE_NONE = 0x00,
		VCONFKEY_WIFI_TRANSFER_STATE_TX,
		VCONFKEY_WIFI_TRANSFER_STATE_RX,
		VCONFKEY_WIFI_TRANSFER_STATE_TXRX
};

/**
 *
 */
#define VCONFKEY_WIFI_STRENGTH  "memory/wifi/strength"

enum {
	VCONFKEY_WIFI_STRENGTH_MIN = 0,
		VCONFKEY_WIFI_STRENGTH_MAX = 4
};

/**
 *
 */
#define VCONFKEY_WIFI_QS_EXIT  "memory/wifi/wifi_qs_exit"

enum {
	VCONFKEY_WIFI_QS_CANCEL = 0x00,
		VCONFKEY_WIFI_QS_WIFI_CONNECTED,
		VCONFKEY_WIFI_QS_3G
};

/**
 *
 */
#define VCONFKEY_WIFI_ENABLE_QS  "db/wifi/enable_quick_start"

enum {
	VCONFKEY_WIFI_QS_DISABLE = 0x00,
		VCONFKEY_WIFI_QS_ENABLE = 0x01
};

/**
 *
 */
#define VCONFKEY_WIFI_CONNECTED_AP_NAME  "memory/wifi/connected_ap_name"

/**
 *
 */
#define VCONFKEY_WIFI_UG_RUN_STATE  "memory/wifi/ug_run_state"

enum {
	VCONFKEY_WIFI_UG_RUN_STATE_NUL = 0x00,
		VCONFKEY_WIFI_UG_RUN_STATE_ON_FOREGROUND,
		VCONFKEY_WIFI_UG_RUN_STATE_ON_BACKGROUND,
		VCONFKEY_WIFI_UG_RUN_STATE_OFF,
		VCONFKEY_WIFI_UG_RUN_STATE_MAX
};

/**
 *
 */
#define VCONFKEY_WIFI_BSSID_ADDRESS  "db/wifi/bssid_address"

/**
 *
 */
#define VCONFKEY_WIFI_BGSCAN_MODE  "memory/wifi/bgscan_mode"

enum {
	VCONFKEY_WIFI_BGSCAN_MODE_PERIODIC = 0x00,
		VCONFKEY_WIFI_BGSCAN_MODE_EXPONENTIAL
};

#ifdef USE_NONCREATED_VCONF_KEY
#warning using non-created vconf-key ----- please make sure this key is in use actually
#endif
/**
 *
 */
#define VCONFKEY_WIFI_DIRECT_TRANSFER_STATE  "memory/wifi_direct/transfer_state"

enum {
	VCONFKEY_WIFI_DIRECT_TRANSFER_START = 0,
		VCONFKEY_WIFI_DIRECT_TRANSFER_FAIL,
		VCONFKEY_WIFI_DIRECT_TRANSFER_FINISH
};

#ifdef USE_NONCREATED_VCONF_KEY
#warning using non-created vconf-key ----- please make sure this key is in use actually
#endif
/**
 *
 */
#define VCONFKEY_WIFI_DIRECT_SENDING_STATE  "memory/wifi_direct/share/sending_state"

enum {
	VCONFKEY_WIFI_DIRECT_SENDING_IDLE = 0,
		VCONFKEY_WIFI_DIRECT_SENDING_IN_PROGRESS
};

#ifdef USE_NONCREATED_VCONF_KEY
#warning using non-created vconf-key ----- please make sure this key is in use actually
#endif
/**
 *
 */
#define VCONFKEY_WIFI_DIRECT_RECEIVING_STATE  "memory/wifi_direct/share/receiving_state"

enum {
	VCONFKEY_WIFI_DIRECT_RECEIVING_IDLE = 0,
		VCONFKEY_WIFI_DIRECT_RECEIVING_IN_PROGRESS
};

/**
 *
 */
#define VCONFKEY_WIFI_DIRECT_DHCP_IP_LEASE  "memory/private/wifi_direct_manager/dhcp_ip_lease"

/**
 *  error took place
 */
#define VCONFKEY_WIFI_DIRECT_DHCPC_SERVER_IP  "memory/private/wifi_direct_manager/dhcpc_server_ip"

/**
 *  error took place
 */
#define VCONFKEY_WIFI_DIRECT_P2P_GATEWAY  "memory/private/wifi_direct_manager/p2p_gateway"

/**
 *  error took place
 */
#define VCONFKEY_WIFI_DIRECT_P2P_IFNAME  "memory/private/wifi_direct_manager/p2p_ifname"

/**
 *  error took place
 */
#define VCONFKEY_WIFI_DIRECT_P2P_LOCAL_IP  "memory/private/wifi_direct_manager/p2p_local_ip"

/**
 *
 */
#define VCONFKEY_WIFI_DIRECT_P2P_SUBNET_MASK  "memory/private/wifi_direct_manager/p2p_subnet_mask"

/**
 *
 */
#define VCONFKEY_WIFI_DIRECT_STATE  "memory/wifi_direct/state"

enum {
	VCONFKEY_WIFI_DIRECT_DEACTIVATED = 0,
		VCONFKEY_WIFI_DIRECT_ACTIVATED,
		VCONFKEY_WIFI_DIRECT_DISCOVERING,
		VCONFKEY_WIFI_DIRECT_CONNECTED,
		VCONFKEY_WIFI_DIRECT_GROUP_OWNER,
		VCONFKEY_WIFI_DIRECT_DEACTIVATING,
		VCONFKEY_WIFI_DIRECT_ACTIVATING,
		VCONFKEY_WIFI_DIRECT_CONNECTING,
		VCONFKEY_WIFI_DIRECT_DISCONNECTING
};

#ifdef USE_NONCREATED_VCONF_KEY
#warning using non-created vconf-key ----- please make sure this key is in use actually
#endif
/**
 *
 */
#define VCONFKEY_ALLSHARE_CAST_STATE  "memory/allshare_cast/state"

enum {
	VCONFKEY_ALLSHARECAST_NOT_CONNECTED = 0,
		VCONFKEY_ALLSHARECAST_CONNECTED
};

#ifdef USE_NONCREATED_VCONF_KEY
#warning using non-created vconf-key ----- please make sure this key is in use actually
#endif
/**
 *
 */
#define VCONFKEY_SCREEN_MIRRORING_STATE  "memory/screen_mirroring/status"

enum {
	VCONFKEY_SCREEN_MIRRORING_DEACTIVATED = 0,
		VCONFKEY_SCREEN_MIRRORING_ACTIVATED,
		VCONFKEY_SCREEN_MIRRORING_CONNECTED
};

/**
 *
 */
#define VCONFKEY_WIFI_NETWORK_BONDING  "file/private/wifi/network_bonding"

/**
 *
 */
#define VCONFKEY_WIFI_DEVICE_STATUS_UEVENT  "memory/wifi/device/status_uevent"

enum {
	VCONFKEY_WIFI_DEVICE_STATUS_DETACHED = 0x00,
		VCONFKEY_WIFI_DEVICE_STATUS_ATTACHED
};

/**
 *
 */
#define VCONFKEY_WIFI_INTERFACE_NAME  "memory/wifi/device/ifname"

/**
 *
 */
#define VCONFKEY_WIFI_SORT_BY  "file/private/wifi/sort_by"

#endif /*   __VCONF_INTERNAL_WIFI_KEYS_H__    */
