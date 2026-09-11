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

#ifndef __VCONF_INTERNAL_PM_KEYS_H__
#define __VCONF_INTERNAL_PM_KEYS_H__
/**
 * public vconf key (device)
 */
#define VCONFKEY_PM_STATE  "memory/pm/state"

enum {
	VCONFKEY_PM_STATE_NORMAL = 1,
		VCONFKEY_PM_STATE_LCDDIM,
		VCONFKEY_PM_STATE_LCDOFF,
		VCONFKEY_PM_STATE_SLEEP
};

/**
 *
 */
#define VCONFKEY_PM_CUSTOM_BRIGHTNESS_STATUS  "memory/pm/custom_brightness_status"

enum {
	VCONFKEY_PM_CUSTOM_BRIGHTNESS_OFF = 0,
		VCONFKEY_PM_CUSTOM_BRIGHTNESS_ON
};

/**
 *
 */
#define VCONFKEY_PM_CAMERA_STATUS  "memory/pm/camera_status"

enum {
	VCONFKEY_PM_CAMERA_OFF = 0,
		VCONFKEY_PM_CAMERA_ON
};

/**
 *
 */
#define VCONFKEY_PM_BATTERY_TIMETOEMPTY  "memory/pm/battery_timetoempty"

/**
 *
 */
#define VCONFKEY_PM_BATTERY_TIMETOFULL  "memory/pm/battery_timetofull"

/**
 *
 */
#define VCONFKEY_PM_BRIGHTNESS_CHANGED_IN_LPM  "memory/pm/brt_changed_lpm"

/**
 *
 */
#define VCONFKEY_PM_LCDOFF_SOURCE  "memory/pm/lcdoff_source"

enum {
	VCONFKEY_PM_LCDOFF_BY_POWERKEY = 1,
		VCONFKEY_PM_LCDOFF_BY_TIMEOUT
};

/**
 *
 */
#define VCONFKEY_PM_KEY_IGNORE  "memory/pm/key_ignore"

enum {
	VCONFKEY_PM_KEY_LOCK    = 0x01,
		VCONFKEY_PM_KEY_STARTER = 0x02,
		VCONFKEY_PM_KEY_EXTRA1  = 0x04,
		VCONFKEY_PM_KEY_EXTRA2  = 0x08
};

/**
 *
 */
#define VCONFKEY_PM_CURRENT_BRIGHTNESS  "memory/pm/current_brt"

/**
 *
 */
#define VCONFKEY_PM_SIP_STATUS  "memory/pm/sip_status"

enum {
	VCONFKEY_PM_SIP_OFF = 0,
		VCONFKEY_PM_SIP_ON
};

#endif /*   __VCONF_INTERNAL_PM_KEYS_H__    */
