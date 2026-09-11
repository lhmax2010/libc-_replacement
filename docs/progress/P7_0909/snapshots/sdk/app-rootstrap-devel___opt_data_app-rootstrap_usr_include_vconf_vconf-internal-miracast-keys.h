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

#ifndef __VCONF_INTERNAL_MIRACAST_KEYS_H__
#define __VCONF_INTERNAL_MIRACAST_KEYS_H__
#ifdef USE_NONCREATED_VCONF_KEY
#warning using non-created vconf-key ----- please make sure this key is in use actually
#endif
/**
 *
 */
#define VCONFKEY_MIRACAST_WFD_SOURCE_STATUS  "memory/wifi/miracast/source_status"

enum {
	VCONFKEY_MIRACAST_WFD_SOURCE_OFF,
		VCONFKEY_MIRACAST_WFD_SOURCE_ON
};

#ifdef USE_NONCREATED_VCONF_KEY
#warning using non-created vconf-key ----- please make sure this key is in use actually
#endif
/**
 *  webkit2-efl is in use for TV profile
 */
#define VCONFKEY_WFD_WEB_VIDEO_PLAYER_STATE  "memory/wifi/miracast/web_video_player_state"

enum {
	VCONFKEY_WFD_WEB_VIDEO_PLAYER_STOP = 0,
		VCONFKEY_WFD_WEB_VIDEO_PLAYER_PLAY = 1
};

#endif /*   __VCONF_INTERNAL_MIRACAST_KEYS_H__    */
