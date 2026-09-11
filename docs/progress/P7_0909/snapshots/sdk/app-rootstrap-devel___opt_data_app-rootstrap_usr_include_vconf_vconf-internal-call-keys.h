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

#ifndef __VCONF_INTERNAL_CALL_KEYS_H__
#define __VCONF_INTERNAL_CALL_KEYS_H__
/**
 * Will be repalced metheod which is not using vconf key
 */
#define VCONFKEY_CALL_STATE  "memory/call/state"

enum {
	VCONFKEY_CALL_OFF = 0x00,
		VCONFKEY_CALL_VOICE_CONNECTING,
		VCONFKEY_CALL_VOICE_ACTIVE,
		VCONFKEY_CALL_VIDEO_CONNECTING,
		VCONFKEY_CALL_VIDEO_ACTIVE,
		VCONFKEY_CALL_STATE_MAX
};

#endif /*   __VCONF_INTERNAL_CALL_KEYS_H__    */
