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

#ifndef __VCONF_INTERNAL_DNET_KEYS_H__
#define __VCONF_INTERNAL_DNET_KEYS_H__
/**
 *
 */
#define VCONFKEY_NETWORK_STATUS  "memory/dnet/status"

enum {
	VCONFKEY_NETWORK_OFF = 0,
		VCONFKEY_NETWORK_CELLULAR,
		VCONFKEY_NETWORK_WIFI,
		VCONFKEY_NETWORK_ETHERNET,
		VCONFKEY_NETWORK_BLUETOOTH,
		VCONFKEY_NETWORK_DEFAULT_PROXY
};

/**
 *
 */
#define VCONFKEY_NETWORK_CONFIGURATION_CHANGE_IND  "memory/dnet/network_config"

/**
 *
 */
#define VCONFKEY_NETWORK_IP  "memory/dnet/ip"

/**
 * Active IPv6 address
 */
#define VCONFKEY_NETWORK_IP6  "memory/dnet/ip6"

/**
 * Active IPv4 address for tethering
 */
#define VCONFKEY_NETWORK_RNDIS_IP  "db/dnet/rndis_ip"

/**
 *
 */
#define VCONFKEY_NETWORK_PROXY  "memory/dnet/proxy"

/**
 *
 */
#define VCONFKEY_NETWORK_WIFI_STATE  "memory/dnet/wifi"

enum {
	VCONFKEY_NETWORK_WIFI_OFF,
		VCONFKEY_NETWORK_WIFI_NOT_CONNECTED,
		VCONFKEY_NETWORK_WIFI_CONNECTED
};

/**
 *
 */
#define VCONFKEY_NETWORK_CELLULAR_STATE  "memory/dnet/cellular"

enum {
	VCONFKEY_NETWORK_CELLULAR_ON = 0x00,
		VCONFKEY_NETWORK_CELLULAR_3G_OPTION_OFF,
		VCONFKEY_NETWORK_CELLULAR_ROAMING_OFF,
		VCONFKEY_NETWORK_CELLULAR_FLIGHT_MODE,
		VCONFKEY_NETWORK_CELLULAR_NO_SERVICE
};

/**
 *
 */
#define VCONFKEY_DNET_STATE  "memory/dnet/state"

enum {
	VCONFKEY_DNET_OFF = 0x00,
		VCONFKEY_DNET_NORMAL_CONNECTED,
		VCONFKEY_DNET_SECURE_CONNECTED,
		VCONFKEY_DNET_TRANSFER
};

/**
 *
 */
#define VCONFKEY_DNET_STATE2  "memory/dnet/state2"

/**
 *
 */
#define VCONFKEY_PACKET_STATE  "memory/dnet/packet_state"

enum {
	VCONFKEY_PACKET_NORMAL = 0x00,
		VCONFKEY_PACKET_RX,
		VCONFKEY_PACKET_TX,
		VCONFKEY_PACKET_RXTX,
		VCONFKEY_PACKET_MAX
};

/**
 *
 */
#define VCONFKEY_NETWORK_CELLULAR_PKT_TOTAL_RCV  "db/dnet/statistics/cellular/totalrcv"

/**
 *
 */
#define VCONFKEY_NETWORK_CELLULAR_PKT_TOTAL_RCV2  "db/dnet/statistics/cellular/totalrcv2"

/**
 *
 */
#define VCONFKEY_NETWORK_CELLULAR_PKT_TOTAL_SNT  "db/dnet/statistics/cellular/totalsnt"

/**
 *
 */
#define VCONFKEY_NETWORK_CELLULAR_PKT_TOTAL_SNT2  "db/dnet/statistics/cellular/totalsnt2"

/**
 *
 */
#define VCONFKEY_NETWORK_CELLULAR_PKT_LAST_RCV  "db/dnet/statistics/cellular/lastrcv"

/**
 *
 */
#define VCONFKEY_NETWORK_CELLULAR_PKT_LAST_RCV2  "db/dnet/statistics/cellular/lastrcv2"

/**
 *
 */
#define VCONFKEY_NETWORK_CELLULAR_PKT_LAST_SNT  "db/dnet/statistics/cellular/lastsnt"

/**
 *
 */
#define VCONFKEY_NETWORK_CELLULAR_PKT_LAST_SNT2  "db/dnet/statistics/cellular/lastsnt2"

/**
 *
 */
#define VCONFKEY_NETWORK_WIFI_PKT_TOTAL_RCV  "db/dnet/statistics/wifi/totalrcv"

/**
 *
 */
#define VCONFKEY_NETWORK_WIFI_PKT_TOTAL_SNT  "db/dnet/statistics/wifi/totalsnt"

/**
 *
 */
#define VCONFKEY_NETWORK_WIFI_PKT_LAST_RCV  "db/dnet/statistics/wifi/lastrcv"

/**
 *
 */
#define VCONFKEY_NETWORK_WIFI_PKT_LAST_SNT  "db/dnet/statistics/wifi/lastsnt"

/**
 *
 */
#define VCONFKEY_NETWORK_WIFI_LAST_POWER_STATE  "file/private/wifi/last_power_state"

/**
 *
 */
#define VCONFKEY_NETWORK_WIFI_OFF_BY_AIRPLANE  "file/private/wifi/wifi_off_by_airplane"

/**
 *
 */
#define VCONFKEY_NETWORK_WIFI_OFF_BY_EMERGENCY  "file/private/wifi/wifi_off_by_emergency"

/**
 *
 */
#define VCONFKEY_NETWORK_WIFI_OFF_BY_RESTRICTED  "file/private/wifi/wifi_off_by_restricted"

/**
 *
 */
#define VCONFKEY_NETWORK_SLEEP_POLICY  "file/private/wifi/sleep_policy"

/**
 * This key is for TV profile
 */
#define VCONFKEY_NETWORK_MAC_ADDRESS  "db/dnet/mac_address"

/**
 * Wi-Fi Network Frequency for Wi-Fi Direct
 */
#define VCONFKEY_NETWORK_WIFI_FREQUENCY  "memory/private/wifi/frequency"

/**
 * This key is for wearable profile
 */
#define VCONFKEY_NETWORK_WEARABLE_WIFI_USE  "db/private/wifi/wearable_wifi_use"

#endif /*   __VCONF_INTERNAL_DNET_KEYS_H__    */
