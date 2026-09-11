/*
 * libmm-sound
 *
 * Copyright (c) 2000 - 2011 Samsung Electronics Co., Ltd. All rights reserved.
 *
 * Contact: Seungbae Shin <seungbae.shin@samsung.com>
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

/**
 * @file		mm_sound.h
 * @brief		Application interface library for sound module.
 * @date
 * @version		Release
 *
 * Application interface library for sound module.
 */

#ifndef	__MM_SOUND_PRIVATE_H__
#define	__MM_SOUND_PRIVATE_H__

#include <mm_types.h>
#include <mm_error.h>
#include <mm_sound.h>

#ifdef __cplusplus
	extern "C" {
#endif

/**
	@internal
	@addtogroup SOUND_PRIVATE
	@{

*/

typedef struct {
	const char *filename;					/**< filename to play */
	int volume;								/**< relative volume level */
	int loop;								/**< loop count */
	mm_sound_stop_callback_func callback;	/**< callback function when playing is terminated */
	void *data;								/**< user data to callback */
	int volume_config;						/**< volume type & volume gain */
} MMSoundPlayParam;

/**
 * This function is to play key sound.
 *
 * @param	filename		[in] keytone filename to play
 * @param	volume_config	[in] volume type & volume gain
 *
 * @return	This function returns MM_ERROR_NONE on success, or negative value
 *			with error code.
 *
 * @remark	This function provides low latency sound play (such as dialer keytone)
 *			using fixed spec of wave file (44100Hz, mono channel)
 * @see		mm_sound_stop_keysound()
 * @see		volume_type_t volume_gain_t
 */
int mm_sound_play_keysound(const char *filename, int volume_config);

/**
 * This function is to play key sound with a priority.
 *
 * @param	filename		[in] keytone filename to play
 * @param	volume_config	[in] volume type & volume gain
 * @param	priority		[in] priority lower as a higher priority, -1 for no priority (= don't care)
 *
 * @return	This function returns MM_ERROR_NONE on success, or negative value
 *			with error code.
 *
 * @remark	This function provides low latency sound play with a given priority
 * @see		mm_sound_stop_keysound()
 * @see		volume_type_t volume_gain_t
 */
int mm_sound_play_keysound_priority(const char *filename, int volume_config, int priority);

/**
 * This function is to stop key sound.
 *
 * @param	filename		[in] keytone filename to stop (can be null if whole exist keytones need stops)
 *
 * @return	This function returns MM_ERROR_NONE on success, or negative value
 *			with error code.
 *
 * @see		mm_sound_play_keysound()
 */
int mm_sound_stop_keysound(const char *filename);

int mm_sound_boot_ready(int timeout_sec);

int mm_sound_boot_play_sound(char* path);

/**
	@}
 */

#ifdef __cplusplus
}
#endif

#endif	/* __MM_SOUND_H__ */

