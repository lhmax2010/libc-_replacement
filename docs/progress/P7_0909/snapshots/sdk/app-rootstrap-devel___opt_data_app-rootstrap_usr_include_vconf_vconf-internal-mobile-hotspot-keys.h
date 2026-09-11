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

#ifndef __VCONF_INTERNAL_MOBILE_HOTSPOT_KEYS_H__
#define __VCONF_INTERNAL_MOBILE_HOTSPOT_KEYS_H__
/**
 *  To notify which tethering type is enabled
 */
#define VCONFKEY_MOBILE_HOTSPOT_MODE  "memory/mobile_hotspot/mode"

enum {
	VCONFKEY_MOBILE_HOTSPOT_MODE_NONE      = 0x00,
		VCONFKEY_MOBILE_HOTSPOT_MODE_WIFI      = 0x01,
		VCONFKEY_MOBILE_HOTSPOT_MODE_USB       = 0x02,
		VCONFKEY_MOBILE_HOTSPOT_MODE_BT        = 0x04,
		VCONFKEY_MOBILE_HOTSPOT_MODE_WIFI_AP   = 0x08,
		VCONFKEY_MOBILE_HOTSPOT_MODE_P2P       = 0x10
};

/**
 *  To notify clients which are connected through tethering
 */
#define VCONFKEY_MOBILE_HOTSPOT_CONNECTED_DEVICE  "memory/mobile_hotspot/connected_device"

/**
 *  To notify tethering security type
 */
#define VCONFKEY_MOBILE_HOTSPOT_SECURITY  "db/mobile_hotspot/security"

enum {
	VCONFKEY_MOBILE_AP_SECURITY_OFF  = 0x00,
		VCONFKEY_MOBILE_AP_SECURITY_ON
};

/**
 *  To notify tethering visibility mode(visible/invisible)
 */
#define VCONFKEY_MOBILE_HOTSPOT_HIDE  "db/mobile_hotspot/hide"

enum {
	VCONFKEY_MOBILE_AP_HIDE_OFF  = 0x00,
		VCONFKEY_MOBILE_AP_HIDE_ON
};

/**
 *  Thethering device's SSID
 */
#define VCONFKEY_MOBILE_HOTSPOT_SSID  "memory/private/mobileap-agent/ssid"

/**
 *  To check previous wifi status
 */
#define VCONFKEY_MOBILE_HOTSPOT_PREV_WIFI_STATUS  "db/private/libug-setting-mobileap-efl/prev_wifi_status"

/**
 *
 */
#define VCONFKEY_MOBILE_HOTSPOT_IS_DEVICE_RENAME_LOCAL  "file/private/libug-setting-mobileap-efl/is_device_rename_local"

/**
 *  Wi-fi popup check-box status for enabling Wi-Fi tethering
 */
#define VCONFKEY_MOBILE_HOTSPOT_WIFI_POPUP_CHECK_BOX_STATUS  "file/private/libug-setting-mobileap-efl/wifi_popup_check_box_status"

/**
 *  BT popup check-box status for enabling BT tethering
 */
#define VCONFKEY_MOBILE_HOTSPOT_BT_POPUP_CHECK_BOX_STATUS  "file/private/libug-setting-mobileap-efl/bt_popup_check_box_status"

/**
 *  USB popup check-box status for enabling USB tethering
 */
#define VCONFKEY_MOBILE_HOTSPOT_USB_POPUP_CHECK_BOX_STATUS  "file/private/libug-setting-mobileap-efl/usb_popup_check_box_status"

/**
 *  Connect USB poup status
 */
#define VCONFKEY_MOBILE_HOTSPOT_TRYING_USB_TETHERING  "memory/private/libug-setting-mobileap-efl/trying_usb_tethering"

/**
 *
 */
#define VCONFKEY_MOBILE_HOTSPOT_WIFI_STATE  "memory/mobile_hotspot/wifi_state"

enum {
	VCONFKEY_MOBILE_HOTSPOT_WIFI_OFF = 0x00,
		VCONFKEY_MOBILE_HOTSPOT_WIFI_PENDING_ON = 0x01
};

#endif /*   __VCONF_INTERNAL_MOBILE_HOTSPOT_KEYS_H__    */
