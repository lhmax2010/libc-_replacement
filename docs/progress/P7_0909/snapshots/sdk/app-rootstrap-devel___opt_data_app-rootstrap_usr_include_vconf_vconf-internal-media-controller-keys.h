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

#ifndef __VCONF_INTERNAL_MEDIA_CONTROLLER_KEYS_H__
#define __VCONF_INTERNAL_MEDIA_CONTROLLER_KEYS_H__
/**
 *  app id of the latest application using media-controller server
 */
#define VCONFKEY_MEDIA_CONTROLLER_LATEST_SERVER_NAME  "db/media-controller/latest_server/name"

/**
 *  server state of the latest application using media-controller server
 */
#define VCONFKEY_MEDIA_CONTROLLER_LATEST_SERVER_STATE  "db/media-controller/latest_server/state"

enum {
	VCONFKEY_MEDIA_CONTROLLER_SERVER_STATE_NONE = 0x00,
		VCONFKEY_MEDIA_CONTROLLER_SERVER_STATE_ACTIVATED,
		VCONFKEY_MEDIA_CONTROLLER_SERVER_STATE_DEACTIVATED
};

/**
 *  playback state of the latest application using media-controller server
 */
#define VCONFKEY_MEDIA_CONTROLLER_LATEST_SERVER_PLAYBACK_STATE  "db/media-controller/latest_server/playback_state"

enum {
	VCONFKEY_MEDIA_CONTROLLER_PLAYBACK_STATE_NONE = 0x00,
		VCONFKEY_MEDIA_CONTROLLER_PLAYBACK_STATE_PLAYING,
		VCONFKEY_MEDIA_CONTROLLER_PLAYBACK_STATE_PAUSED,
		VCONFKEY_MEDIA_CONTROLLER_PLAYBACK_STATE_STOPPED,
		VCONFKEY_MEDIA_CONTROLLER_PLAYBACK_STATE_MOVING_TO_NEXT = 0x08,
		VCONFKEY_MEDIA_CONTROLLER_PLAYBACK_STATE_MOVING_TO_PREVIOUS,
		VCONFKEY_MEDIA_CONTROLLER_PLAYBACK_STATE_FAST_FORWARDING,
		VCONFKEY_MEDIA_CONTROLLER_PLAYBACK_STATE_REWINDING
};

#endif /*   __VCONF_INTERNAL_MEDIA_CONTROLLER_KEYS_H__    */
