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

#ifndef __VCONF_INTERNAL_CAMERA_KEYS_H__
#define __VCONF_INTERNAL_CAMERA_KEYS_H__
/**
 *  camera state checker for system wide
 */
#define VCONFKEY_CAMERA_STATE  "memory/camera/state"

enum {
	VCONFKEY_CAMERA_STATE_NULL,
		VCONFKEY_CAMERA_STATE_OPEN,
		VCONFKEY_CAMERA_STATE_PREVIEW,
		VCONFKEY_CAMERA_STATE_RECORDING,
		VCONFKEY_CAMERA_STATE_RECORDING_PAUSE
};

/**
 *  camera shutter sound policy while capturing and recording mode according to country policy
 */
#define VCONFKEY_CAMERA_SHUTTER_SOUND_POLICY  "file/camera/shutter_sound_policy"

enum {
	VCONFKEY_CAMERA_SHUTTER_SOUND_POLICY_OFF,
		VCONFKEY_CAMERA_SHUTTER_SOUND_POLICY_ON
};

/**
 *  recorder state checker for system wide
 */
#define VCONFKEY_RECORDER_STATE  "memory/recorder/state"

enum {
	VCONFKEY_RECORDER_STATE_NULL,
		VCONFKEY_RECORDER_STATE_CREATED,
		VCONFKEY_RECORDER_STATE_RECORDING,
		VCONFKEY_RECORDER_STATE_RECORDING_PAUSE
};

/**
 *  pid number checker for applications from product team requirement
 */
#define VCONFKEY_CAMERA_PID  "memory/camera/pid"

/**
 *  flash state checker for application from flash api team and product team requirement
 */
#define VCONFKEY_CAMERA_FLASH_STATE  "memory/camera/flash_state"

enum {
	VCONFKEY_CAMERA_FLASH_STATE_OFF,
		VCONFKEY_CAMERA_FLASH_STATE_ON
};

/**
 *  pid number checker for applications from product team requirement
 */
#define VCONFKEY_RECORDER_PID  "memory/recorder/pid"

#endif /*   __VCONF_INTERNAL_CAMERA_KEYS_H__    */
