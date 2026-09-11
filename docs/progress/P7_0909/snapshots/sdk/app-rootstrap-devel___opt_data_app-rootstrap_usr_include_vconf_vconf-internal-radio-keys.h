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

#ifndef __VCONF_INTERNAL_RADIO_KEYS_H__
#define __VCONF_INTERNAL_RADIO_KEYS_H__
/**
 *  fmradio status for system wide
 */
#define VCONFKEY_RADIO_STATUS  "db/radio/status"

enum {
	VCONFKEY_RADIO_STATUS_OFF = 0x0000,
		VCONFKEY_RADIO_STATUS_ON = 0x0001
};

#endif /*   __VCONF_INTERNAL_RADIO_KEYS_H__    */
