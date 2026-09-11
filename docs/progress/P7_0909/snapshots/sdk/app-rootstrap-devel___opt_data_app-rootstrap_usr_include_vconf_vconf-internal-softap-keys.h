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

#ifndef __VCONF_INTERNAL_SOFTAP_KEYS_H__
#define __VCONF_INTERNAL_SOFTAP_KEYS_H__
/**
 *  To notify clients which are connected through softap
 */
#define VCONFKEY_SOFTAP_CONNECTED_DEVICE  "memory/softap/connected_device"

/**
 *  To notify softap security type
 */
#define VCONFKEY_SOFTAP_SECURITY  "db/softap/security"

enum {
	VCONFKEY_SOFTAP_SECURITY_OFF  = 0x00,
		VCONFKEY_SOFTAP_SECURITY_ON
};

/**
 *  To notify softap visibility mode(visible/invisible)
 */
#define VCONFKEY_SOFTAP_HIDE  "db/softap/hide"

enum {
	VCONFKEY_SOFTAP_HIDE_OFF  = 0x00,
		VCONFKEY_SOFTAP_HIDE_ON
};

/**
 *  To notify softap channel
 */
#define VCONFKEY_SOFTAP_CHANNEL  "db/softap/channel"

/**
 *  To notify softap wireless mode
 */
#define VCONFKEY_SOFTAP_MODE  "db/softap/mode"

enum {
	VCONFKEY_SOFTAP_MODE_B = 0x00,
		VCONFKEY_SOFTAP_MODE_G = 0x01,
		VCONFKEY_SOFTAP_MODE_A = 0x02,
		VCONFKEY_SOFTAP_MODE_AD = 0x03
};

/**
 *  Softap device's SSID
 */
#define VCONFKEY_SOFTAP_SSID  "memory/private/softap/ssid"

/**
 *  Softap key
 */
#define VCONFKEY_SOFTAP_KEY  "memory/private/softap/key"

/**
 *  Softap key
 */
#define VCONFKEY_SOFTAP_VSIE  "memory/private/softap/vsie"

#endif /*   __VCONF_INTERNAL_SOFTAP_KEYS_H__    */
