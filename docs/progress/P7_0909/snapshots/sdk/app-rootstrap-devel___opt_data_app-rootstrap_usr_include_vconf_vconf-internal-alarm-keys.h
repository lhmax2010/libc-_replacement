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

#ifndef __VCONF_INTERNAL_ALARM_KEYS_H__
#define __VCONF_INTERNAL_ALARM_KEYS_H__
#ifdef USE_NONCREATED_VCONF_KEY
#warning using non-created vconf-key ----- please make sure this key is in use actually
#endif
/**
 *
 */
#define VCONFKEY_ALARM_STATE  "db/alarm/state"

#ifdef USE_NONCREATED_VCONF_KEY
#warning using non-created vconf-key ----- please make sure this key is in use actually
#endif
/**
 *
 */
#define VCONFKEY_ALARM_RINGING  "memory/alarm/ringing"

#ifdef USE_NONCREATED_VCONF_KEY
#warning using non-created vconf-key ----- please make sure this key is in use actually
#endif
/**
 *
 */
#define VCONFKEY_ALARM_VOLUME_POPUP_ENABLE  "memory/alarm/volume_popup_enable"

/**
 *
 */
#define VCONFKEY_ALARM_EXPIRE_MODE  "db/alarm/alarm_expire_mode"

enum {
	ALARM_EXPIRE_MODE_NORMAL = 0,
		ALARM_EXPIRE_MODE_LCD_OFF
};

#endif /*   __VCONF_INTERNAL_ALARM_KEYS_H__    */
