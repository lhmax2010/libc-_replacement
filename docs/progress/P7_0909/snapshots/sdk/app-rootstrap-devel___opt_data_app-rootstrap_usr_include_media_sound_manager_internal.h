/*
* Copyright (c) 2015 - 2018 Samsung Electronics Co., Ltd All Rights Reserved
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
*/

#ifndef __TIZEN_MEDIA_SOUND_MANAGER_INTERNAL_H__
#define __TIZEN_MEDIA_SOUND_MANAGER_INTERNAL_H__

#ifdef __cplusplus
extern "C"
{
#endif

/**
 * @file sound_manager_internal.h
 * @brief This file contains the Sound Manager Internal API.
 */

/**
 * @addtogroup CAPI_MEDIA_SOUND_MANAGER_MODULE
 * @{
 */

#define SOUND_TYPE_NUM                SOUND_TYPE_VOICE + 1
#define SOUND_TYPE_INTERNAL_NUM       SOUND_TYPE_BIXBY + 1

/* For in-house user, below symbols still can be used with internal header. */
#define SOUND_DEVICE_STATE_DEPRECATED_DEACTIVATED_MASK    0x1000  /**< Mask for deactivated devices, the same value with SOUND_DEVICE_STATE_DEACTIVATED_MASK deprecated since 4.0, removed since 7.0 */
#define SOUND_DEVICE_STATE_DEPRECATED_ACTIVATED_MASK      0x2000  /**< Mask for activated devices, the same value with SOUND_DEVICE_STATE_ACTIVATED_MASK deprecated since 4.0, removed since 7.0 */

/**
 * @internal
 * @brief Virtual sound stream handle.
 * @since_tizen 3.0
 */
typedef struct virtual_sound_stream_s* virtual_sound_stream_h;

/**
 * @internal
 * @brief Enumeration for Native API.
 * @since_tizen 3.0
 */
typedef enum {
	NATIVE_API_SOUND_MANAGER,   /**< Sound-manager Native API */
	NATIVE_API_PLAYER,          /**< Player Native API */
	NATIVE_API_WAV_PLAYER,      /**< Wav-player Native API */
	NATIVE_API_TONE_PLAYER,     /**< Tone-player Native API */
	NATIVE_API_AUDIO_IO,        /**< Audio-io Native API */
	NATIVE_API_RECORDER,        /**< Recorder Native API */
	NATIVE_API_WEBRTC,          /**< WebRTC Native API */
} native_api_e;

/**
 * @internal
 * @brief Enumeration for sound stream type for internal.
 * @since_tizen 3.0
 */
typedef enum {
	SOUND_STREAM_TYPE_RINGTONE_CALL = 100,        /**< Sound stream type for ringtone for call */
	SOUND_STREAM_TYPE_RINGBACKTONE_CALL,          /**< Sound stream type for ringback tone for call */
	SOUND_STREAM_TYPE_VOICE_CALL,                 /**< Sound stream type for voice-call */
	SOUND_STREAM_TYPE_VIDEO_CALL,                 /**< Sound stream type for video-call */
	SOUND_STREAM_TYPE_RADIO,                      /**< Sound stream type for radio */
	SOUND_STREAM_TYPE_LOOPBACK,                   /**< Sound stream type for loopback */
	SOUND_STREAM_TYPE_LOOPBACK_MIRRORING,         /**< Sound stream type for loopback-mirroring */
	SOUND_STREAM_TYPE_SOLO,                       /**< Sound stream type for solo */
	SOUND_STREAM_TYPE_VOICE_RECOGNITION_SERVICE,  /**< Sound stream type for voice recognition by service (Since 4.0) */
	SOUND_STREAM_TYPE_MEDIA_COMPRESSED,           /**< Sound stream type for compressed media (Since 4.0) */
	SOUND_STREAM_TYPE_MEDIA_NETWORK_SOURCE,       /**< Sound stream type for network source media (Since 5.5) */
} sound_stream_type_internal_e;

/**
 * @brief Enumeration for sound type for internal.
 * @since_tizen 6.0
 */
typedef enum {
	SOUND_TYPE_BIXBY,          /**< Sound type for Bixby */
} sound_type_internal_e;

/**
 * @internal
 * @brief Enumeration for sound filters.
 * @since_tizen 4.0
 */
typedef enum {
	SOUND_FILTER_LOW_PASS = 0,
	SOUND_FILTER_HIGH_PASS,
	SOUND_FILTER_DELAY,
	SOUND_FILTER_SOUNDALIVE,
} sound_filter_e;

/**
 * @internal
 * @brief Enumeration for filter preset.
 * @since_tizen 4.0
 */
typedef enum {
	SOUND_FILTER_PRESET_LOW_PASS_NONE = 0,       /**< Filter preset for SOUND_FILTER_LOW_PASS none */
	SOUND_FILTER_PRESET_LOW_PASS_UNDER_240HZ,    /**< Filter preset for SOUND_FILTER_LOW_PASS under 240hz */
	SOUND_FILTER_PRESET_LOW_PASS_UNDER_480HZ,    /**< Filter preset for SOUND_FILTER_LOW_PASS under 480hz */
	SOUND_FILTER_PRESET_HIGH_PASS_NONE,          /**< Filter preset for SOUND_FILTER_HIGH_PASS none */
	SOUND_FILTER_PRESET_HIGH_PASS_OVER_240HZ,    /**< Filter preset for SOUND_FILTER_HIGH_PASS over 240hz */
	SOUND_FILTER_PRESET_HIGH_PASS_OVER_480HZ,    /**< Filter preset for SOUND_FILTER_HIGH_PASS over 480hz */
	SOUND_FILTER_PRESET_DELAY_NONE,              /**< Filter preset for SOUND_FILTER_DELAY none */
	SOUND_FILTER_PRESET_DELAY_1SEC,              /**< Filter preset for SOUND_FILTER_DELAY 1 second */
	SOUND_FILTER_PRESET_DELAY_2SEC,              /**< Filter preset for SOUND_FILTER_DELAY 2 seconds */
	SOUND_FILTER_PRESET_SOUNDALIVE_NORMAL,       /**< Filter preset for SOUND_FILTER_SOUNDALIVE normal */
	SOUND_FILTER_PRESET_SOUNDALIVE_TUBE,         /**< Filter preset for SOUND_FILTER_SOUNDALIVE tube */
	SOUND_FILTER_PRESET_SOUNDALIVE_VIRT71,       /**< Filter preset for SOUND_FILTER_SOUNDALIVE virtual7.1 */
	SOUND_FILTER_PRESET_SOUNDALIVE_STUDIO,       /**< Filter preset for SOUND_FILTER_SOUNDALIVE studio */
	SOUND_FILTER_PRESET_SOUNDALIVE_CLUB,         /**< Filter preset for SOUND_FILTER_SOUNDALIVE club */
	SOUND_FILTER_PRESET_SOUNDALIVE_CONCERT_HALL, /**< Filter preset for SOUND_FILTER_SOUNDALIVE concert hall */
} sound_filter_preset_e;

/**
 * @internal
 * @brief Enumeration for rpi playback route
 * @since_tizen 6.5
 */
typedef enum {
	SOUND_RPI_PLAYBACK_AUTO,      /**< NOT IN USE */
	SOUND_RPI_PLAYBACK_HEADPHONE, /**< Headphone (default) */
	SOUND_RPI_PLAYBACK_HDMI1,     /**< HDMI1 */
	SOUND_RPI_PLAYBACK_HDMI2,     /**< HDMI2 (only for RPI4) */
} sound_rpi_playback_route_type;

/**
* @brief Enumeration for sound effects.
* @since_tizen 8.0
*/
typedef enum {
	SOUND_EFFECT_NOISE_SUPPRESSION_DOORBELL           = 0x0100, /**< Noise suppression for doorbell */
	SOUND_EFFECT_NOISE_SUPPRESSION_VOICE_RECOGNITION   = 0x0200, /**< Noise suppression for voice recognition */
} sound_effect_method_internal_e;

/**
 * @internal
 * @brief Gets the maximum master volume level.
 * @since_tizen 3.0
 * @param[out]	max_level	The maximum volume level
 * @return @c 0 on success,
 *         otherwise a negative error value
 * @retval #SOUND_MANAGER_ERROR_NONE Success
 * @retval #SOUND_MANAGER_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #SOUND_MANAGER_ERROR_INTERNAL Internal error inside the sound system
 * @see sound_manager_set_master_volume()
 * @see sound_manager_get_master_volume()
 */
int sound_manager_get_max_master_volume(int *max_level);

/**
 * @internal
 * @brief Sets the master volume level.
 * @since_tizen 3.0
 * @param[in]	level	The volume level to be set
 * @return @c 0 on success,
 *         otherwise a negative error value
 * @retval #SOUND_MANAGER_ERROR_NONE Success
 * @retval #SOUND_MANAGER_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #SOUND_MANAGER_ERROR_INTERNAL Internal error inside the sound system
 * @see sound_manager_get_max_master_volume()
 * @see sound_manager_get_master_volume()
 */
int sound_manager_set_master_volume(int level);

/**
 * @internal
 * @brief Gets the master volume level.
 * @since_tizen 3.0
 * @param[out]	level	The current master volume level
 * @return @c 0 on success,
 *         otherwise a negative error value
 * @retval #SOUND_MANAGER_ERROR_NONE Success
 * @retval #SOUND_MANAGER_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #SOUND_MANAGER_ERROR_INTERNAL Internal error inside the sound system
 * @see sound_manager_get_max_master_volume()
 * @see sound_manager_set_master_volume()
 */
int sound_manager_get_master_volume(int *level);

/**
 * @internal
 * @brief Sets the mute specified for a particular sound type.
 * @since_tizen 5.5
 * @privlevel public
 * @privilege %http://tizen.org/privilege/volume.set
 * @param[in]		type The sound type
 * @param[in]		mute	The mute state to be set: (@c true = mute, @c false = unmute)
 * @return @c 0 on success,
 *         otherwise a negative error value
 * @retval #SOUND_MANAGER_ERROR_NONE Success
 * @retval #SOUND_MANAGER_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #SOUND_MANAGER_ERROR_PERMISSION_DENIED Permission denied
 * @retval #SOUND_MANAGER_ERROR_INTERNAL Internal error inside the sound system
 * @see sound_manager_get_mute()
 */
int sound_manager_set_mute(sound_type_e type, bool mute);

/**
 * @internal
 * @brief Gets the mute specified for a particular sound type.
 * @since_tizen 5.5
 * @param[in]		type The sound type
 * @param[out]	muted	The current mute state: (@c true = muted, @c false = unmuted)
 * @return @c 0 on success,
 *         otherwise a negative error value
 * @retval #SOUND_MANAGER_ERROR_NONE Success
 * @retval #SOUND_MANAGER_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #SOUND_MANAGER_ERROR_INTERNAL Internal error inside the sound system
 * @see sound_manager_set_mute()
 */
int sound_manager_get_mute(sound_type_e type, bool *muted);

/**
 * @brief Called when the volume has changed.
 * @since_tizen 6.0
 * @param[in]   type	The internal sound type
 * @param[in]   level	The new volume level
 * @param[in]   user_data	The user data passed from the callback registration function
 * @pre sound_manager_set_volume_internal() will invoke this callback if you register it using sound_manager_add_volume_changed_cb_internal().
 * @see sound_manager_add_volume_changed_cb_internal()
 * @see sound_manager_remove_volume_changed_cb_internal()
 */
typedef void (*sound_manager_volume_changed_cb_internal) (sound_type_internal_e type, unsigned int level, void *user_data);

/**
 * @brief Gets the maximum volume level supported for a particular sound type.
 * @since_tizen 6.0
 * @param[in]		type The internal sound type
 * @param[out]	max	The maximum volume level
 * @return @c 0 on success,
 *         otherwise a negative error value
 * @retval #SOUND_MANAGER_ERROR_NONE Success
 * @retval #SOUND_MANAGER_ERROR_INVALID_PARAMETER Invalid parameter
 * @see sound_manager_set_volume_internal()
 * @see sound_manager_get_volume_internal()
 */
int sound_manager_get_max_volume_internal(sound_type_internal_e type, int *max);

/**
 * @brief Sets the volume level specified for a particular sound type.
 * @since_tizen 6.0
 * @privlevel public
 * @privilege %http://tizen.org/privilege/volume.set
 * @param[in]		type The internal sound type
 * @param[in]		level	The volume level to be set
 * @return @c 0 on success,
 *         otherwise a negative error value
 * @retval #SOUND_MANAGER_ERROR_NONE Success
 * @retval #SOUND_MANAGER_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #SOUND_MANAGER_ERROR_PERMISSION_DENIED Permission denied
 * @retval #SOUND_MANAGER_ERROR_INTERNAL Internal error inside the sound system
 * @see sound_manager_get_max_volume_internal()
 * @see sound_manager_get_volume_internal()
 */
int sound_manager_set_volume_internal(sound_type_internal_e type, int level);

/**
 * @brief Gets the volume level specified for a particular sound type.
 * @since_tizen 6.0
 * @param[in]		type The internal sound type
 * @param[out]	level	The current volume level
 * @return @c 0 on success,
 *         otherwise a negative error value
 * @retval #SOUND_MANAGER_ERROR_NONE Success
 * @retval #SOUND_MANAGER_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #SOUND_MANAGER_ERROR_INTERNAL Internal error inside the sound system
 * @see sound_manager_get_max_volume_internal()
 * @see sound_manager_set_volume_internal()
 */
int sound_manager_get_volume_internal(sound_type_internal_e type, int *level);

/**
 * @brief Gets the type of the sound being currently played.
 * @since_tizen 6.0
 * @param[out]	type The internal sound type
 * @return @c 0 on success,
 *         otherwise a negative error value
 * @retval #SOUND_MANAGER_ERROR_NONE Success
 * @retval #SOUND_MANAGER_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #SOUND_MANAGER_ERROR_NO_PLAYING_SOUND No playing sound
 * @retval #SOUND_MANAGER_ERROR_INTERNAL Internal error inside the sound system
 */
int sound_manager_get_current_sound_type_internal(sound_type_internal_e *type);

/**
 * @brief Adds a callback function to be invoked when the volume level is changed.
 * @since_tizen 6.0
 * @param[in]	callback	Callback function to indicate change in volume
 * @param[in]	user_data	The user data to be passed to the callback function
 * @param[out]	id	The callback id
 * @return @c 0 on success,
 *         otherwise a negative error value
 * @retval #SOUND_MANAGER_ERROR_NONE Success
 * @retval #SOUND_MANAGER_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #SOUND_MANAGER_ERROR_INTERNAL Internal error inside the sound system
 * @post sound_manager_volume_changed_cb_internal() will be invoked.
 * @see sound_manager_remove_volume_changed_cb_internal()
 * @see sound_manager_volume_changed_cb_internal()
 */
int sound_manager_add_volume_changed_cb_internal(sound_manager_volume_changed_cb_internal callback, void *user_data, int *id);

/**
 * @brief Removes the volume change callback.
 * @since_tizen 6.0
 * @param[in]	id	The id of the callback to remove
 * @return 0 on success, otherwise a negative error value
 * @retval #SOUND_MANAGER_ERROR_NONE Success
 * @retval #SOUND_MANAGER_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #SOUND_MANAGER_ERROR_INTERNAL Internal error inside the sound system
 * @see sound_manager_add_volume_changed_cb_internal()
 */
int sound_manager_remove_volume_changed_cb_internal(int id);

/**
 * @internal
 * @brief Creates a handle for stream information.
 * @since_tizen 3.0
 *
 * @remarks	Do not call this API within sound_stream_focus_state_changed_cb() and sound_stream_focus_state_watch_cb(),\n
 *	otherwise SOUND_MANAGER_ERROR_INVALID_OPERATION will be returned.\n
 *	To apply the stream policy according to this stream information, this handle should be passed to other APIs\n
 *	related to playback or recording.(e.g. player, wav-player, audio-io, etc.)
 *
 * @param[in]	stream_type	The type of stream for internal usage
 * @param[in]	callback	The focus state change callback function
 * @param[in]	user_data	The user data to be passed to the callback function
 * @param[out]	stream_info	The handle of stream information
 * @return @c 0 on success,
 *         otherwise a negative error value
 * @retval #SOUND_MANAGER_ERROR_NONE Success
 * @retval #SOUND_MANAGER_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #SOUND_MANAGER_ERROR_INVALID_OPERATION Invalid operation
 * @retval #SOUND_MANAGER_ERROR_INTERNAL Internal error inside the sound system
 * @see sound_manager_destroy_stream_information()
 * @see sound_manager_add_device_for_stream_routing()
 * @see sound_manager_remove_device_for_stream_routing()
 * @see sound_manager_remove_all_devices_for_stream_routing()
 * @see sound_manager_apply_stream_routing()
 * @see sound_manager_acquire_focus()
 * @see sound_manager_release_focus()
 * @see sound_manager_get_focus_state()
 */
int sound_manager_create_stream_information_internal(sound_stream_type_internal_e stream_type, sound_stream_focus_state_changed_cb callback, void *user_data, sound_stream_info_h *stream_info);

/**
 * @internal
 * @brief Sets the stream routing option.
 * @since_tizen 3.0
 *
 * @remarks	If the stream has not been made yet, this setting will be applied when the stream starts to play.
 *
 * @param[in]	stream_info	The handle of stream information
 * @param[in]	name	The name of option
 * @param[in]	value	The value of option
 * @return @c 0 on success,
 *         otherwise a negative error value
 * @retval #SOUND_MANAGER_ERROR_NONE Success
 * @retval #SOUND_MANAGER_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #SOUND_MANAGER_ERROR_INVALID_STATE Invalid state
 * @retval #SOUND_MANAGER_ERROR_INTERNAL Internal error inside the sound system
 * @pre Call sound_manager_create_stream_information() before calling this function.
 * @see sound_manager_create_stream_information()
 * @see sound_manager_create_stream_information_internal()
 * @see sound_manager_destroy_stream_information()
 */
int sound_manager_set_stream_routing_option(sound_stream_info_h stream_info, const char *name, int value);

/**
 * @internal
 * @brief Queries if this stream information handle is available for the API.
 * @since_tizen 3.0
 * @param[in]	stream_info	The handle of stream information
 * @param[in]	api_name	The native API name
 * @param[out]	is_available	If @c true the api_name is available, @c false the api_name is not available for this strema_info
 * @return @c 0 on success,
 *         otherwise a negative error value
 * @retval #SOUND_MANAGER_ERROR_NONE Success
 * @retval #SOUND_MANAGER_ERROR_INVALID_PARAMETER Invalid parameter
 * @see sound_manager_create_stream_information()
 * @see sound_manager_destroy_stream_information()
 */
int sound_manager_is_available_stream_information(sound_stream_info_h stream_info, native_api_e api_name, bool *is_available);

/**
 * @internal
 * @brief Gets stream type from the stream information handle.
 * @since_tizen 3.0
 * @param[in]	stream_info	The handle of stream information
 * @param[out]	type	The stream type
 * @return @c 0 on success,
 *         otherwise a negative error value
 * @retval #SOUND_MANAGER_ERROR_NONE Success
 * @retval #SOUND_MANAGER_ERROR_INVALID_PARAMETER Invalid parameter
 * @see sound_manager_create_stream_information()
 * @see sound_manager_destroy_stream_information()
 */
int sound_manager_get_type_from_stream_information(sound_stream_info_h stream_info, char **type);

/**
 * @internal
 * @brief Gets the index of the stream information handle.
 * @since_tizen 3.0
 * @param[in]	stream_info	The handle of stream information
 * @param[out]	index	The unique index
 *
 * @return @c 0 on success,
 *         otherwise a negative error value
 * @retval #SOUND_MANAGER_ERROR_NONE Success
 * @retval #SOUND_MANAGER_ERROR_INVALID_PARAMETER Invalid parameter
 * @see sound_manager_create_stream_information()
 * @see sound_manager_destroy_stream_information()
 */
int sound_manager_get_index_from_stream_information(sound_stream_info_h stream_info, int *index);

/**
 * @internal
 * @brief Adds the device id to the stream information for the stream routing.
 * @since_tizen 4.0
 *
 * @remarks	Use sound_manager_get_device_list(), sound_manager_get_next_device() and sound_manager_get_device_id()\n
 *	to get the device id.\n
 *	#SOUND_MANAGER_ERROR_POLICY could be returned according to the stream type of the @a stream_info.\n
 *	The available types of the @a stream_info for this function are #SOUND_STREAM_TYPE_VOIP and #SOUND_STREAM_TYPE_MEDIA_EXTERNAL_ONLY.
 *
 * @param[in]	stream_info	The handle of stream information
 * @param[in]	device_id	The device id
 * @return @c 0 on success,
 *         otherwise a negative error value
 * @retval #SOUND_MANAGER_ERROR_NONE Success
 * @retval #SOUND_MANAGER_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #SOUND_MANAGER_ERROR_POLICY Noncompliance with the sound system policy
 * @pre Call sound_manager_create_stream_information() before calling this function.
 * @post You can apply this setting by calling sound_manager_apply_stream_routing().
 * @see sound_manager_create_stream_information()
 * @see sound_manager_destroy_stream_information()
 * @see sound_manager_remove_device_id_for_stream_routing()
 * @see sound_manager_remove_all_devices_for_stream_routing()
 * @see sound_manager_apply_stream_routing()
 */
int sound_manager_add_device_id_for_stream_routing(sound_stream_info_h stream_info, int device_id);

/**
 * @internal
 * @brief Removes the device id to the stream information for the stream routing.
 * @since_tizen 4.0
 *
 * @remarks	Use sound_manager_get_device_list(), sound_manager_get_next_device() and sound_manager_get_device_id()\n
 *	to get the device id.\n
 *
 * @param[in]	stream_info	The handle of stream information
 * @param[in]	device_id	The device id
 * @return @c 0 on success,
 *         otherwise a negative error value
 * @retval #SOUND_MANAGER_ERROR_NONE Success
 * @retval #SOUND_MANAGER_ERROR_INVALID_PARAMETER Invalid parameter
 * @pre Call sound_manager_create_stream_information() and sound_manager_add_device_for_stream_routing() before calling this function.
 * @post You can apply this setting by calling sound_manager_apply_stream_routing().
 * @see sound_manager_create_stream_information()
 * @see sound_manager_destroy_stream_information()
 * @see sound_manager_add_device_id_for_stream_routing()
 * @see sound_manager_remove_all_devices_for_stream_routing()
 * @see sound_manager_apply_stream_routing()
 */
int sound_manager_remove_device_id_for_stream_routing(sound_stream_info_h stream_info, int device_id);

/**
 * @internal
 * @brief Sets the preferred built-in device id for the stream routing.
 * @since_tizen 5.5
 *
 * @remarks	This function is to set a specific built-in device id when the system has multiple devices of the same built-in device type.
 *	When there's only one device for a built-in device type in the system, nothing will happen even if this function succeeds in operation.
 *	If the device type of @a device_id is not supported by @a stream_info, #SOUND_MANAGER_ERROR_POLICY will be returned.
 *
 * @param[in]	stream_info	The handle of stream information
 * @param[in]	io_direction	The IO direction of the device
 * @param[in]	device_id	The preferred device id (this can be #SOUND_MANAGER_STREAM_NO_PREFERRED_DEVICE to unset)
 * @return @c 0 on success,
 *         otherwise a negative error value
 * @retval #SOUND_MANAGER_ERROR_NONE Success
 * @retval #SOUND_MANAGER_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #SOUND_MANAGER_ERROR_POLICY Noncompliance with the sound system policy
 * @retval #SOUND_MANAGER_ERROR_INTERNAL Internal error inside the sound system
 * @pre Call sound_manager_create_stream_information() before calling this function.
 * @pre Get a device by calling sound_manager_get_device_list() and sound_manager_get_next_device().
 * @pre Get a device id by calling sound_manager_get_device_id().
 * @post You can get preferred devices by calling sound_manager_get_stream_preferred_device().
 * @post Call sound_manager_free_device_list() to free the devices.
 * @see sound_manager_get_stream_preferred_device()
 * @see sound_manager_create_stream_information()
 * @see sound_manager_destroy_stream_information()
 * @see sound_manager_get_device_list()
 * @see sound_manager_get_next_device()
 * @see sound_manager_get_device_id()
 * @see sound_manager_free_device_list()
 */
int sound_manager_set_stream_preferred_device_id(sound_stream_info_h stream_info, sound_device_io_direction_e io_direction, int device_id);

/**
 * @internal
 * @brief Definition for the value indicating that the preemptive device for the specific stream type was not set.
 * @since_tizen 5.5
 * @see sound_manager_get_stream_preemptive_device()
 */
#define SOUND_MANAGER_STREAM_NO_PREEMPTIVE_DEVICE    0

/**
 * @internal
 * @brief Sets the preemptive device for the stream routing.
 * @since_tizen 5.5
 *
 * @remarks	This function is to set a device id to streams made by the specific stream type.
 *	The available stream types for this function are #SOUND_STREAM_TYPE_MEDIA, #SOUND_STREAM_TYPE_SYSTEM,\n
 *	#SOUND_STREAM_TYPE_VOICE_INFORMATION and #SOUND_STREAM_TYPE_VOICE_RECOGNITION. Otherwise, #SOUND_MANAGER_ERROR_POLICY will be returned.
 *
 * @param[in]	stream_type	The type of stream
 * @param[in]	direction	The IO direction of the device
 * @param[in]	device_id	The preemptive device id (this can be #SOUND_MANAGER_STREAM_NO_PREEMPTIVE_DEVICE to unset)
 * @return @c 0 on success,
 *         otherwise a negative error value
 * @retval #SOUND_MANAGER_ERROR_NONE Success
 * @retval #SOUND_MANAGER_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #SOUND_MANAGER_ERROR_POLICY Noncompliance with the sound system policy
 * @retval #SOUND_MANAGER_ERROR_INTERNAL Internal error inside the sound system
 * @pre Get a device by calling sound_manager_get_device_list() and sound_manager_get_next_device().
 * @pre Get a device id by calling sound_manager_get_device_id().
 * @post You can get preemptive devices by calling sound_manager_get_stream_preemptive_device().
 * @post Call sound_manager_free_device_list() to free the devices.
 * @see sound_manager_get_stream_preemptive_device()
 * @see sound_manager_get_device_list()
 * @see sound_manager_get_next_device()
 * @see sound_manager_get_device_id()
 * @see sound_manager_free_device_list()
 */
int sound_manager_set_stream_preemptive_device(sound_stream_type_e stream_type, sound_device_io_direction_e direction, int device_id);

/**
 * @internal
 * @brief Gets the preemptive device for the stream routing.
 * @since_tizen 5.5
 *
 * @remarks	This function is to return the preemptive device id per each direction if it has been set.
 *	If there is no preemptive device for a given direction, the corresponding output value will be set to
 *	#SOUND_MANAGER_STREAM_NO_PREEMPTIVE_DEVICE.
 *	You can get a device handle of the id value by using sound_manager_get_device_list(),
 *	sound_manager_get_next_device() and sound_manager_get_device_id().
 *
 * @param[in]	stream_type	The type of stream
 * @param[out]	in_device_id	The preemptive input device id
 * @param[out]	out_device_id	The preemptive output device id
 * @return @c 0 on success,
 *         otherwise a negative error value
 * @retval #SOUND_MANAGER_ERROR_NONE Success
 * @retval #SOUND_MANAGER_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #SOUND_MANAGER_ERROR_INTERNAL Internal error inside the sound system
 * @see sound_manager_set_stream_preemptive_device()
 * @see sound_manager_get_device_list()
 * @see sound_manager_get_next_device()
 * @see sound_manager_get_device_id()
 * @see sound_manager_free_device_list()
 */
int sound_manager_get_stream_preemptive_device(sound_stream_type_e stream_type, int *in_device_id, int *out_device_id);

/**
 * @internal
 * @brief Checks if the device is running.
 * @since_tizen 5.0
 * @param[in]	device_id	The device id
 * @param[out]	is_running	Whether the device is running or not: (@c true = running, @c false = not running)
 * @return @c 0 on success,
 *         otherwise a negative error value
 * @retval #SOUND_MANAGER_ERROR_NONE Success
 * @retval #SOUND_MANAGER_ERROR_INVALID_PARAMETER Invalid parameter
 * @see sound_manager_get_device_list()
 * @see sound_manager_get_next_device()
 * @see sound_manager_get_prev_device()
 * @see sound_manager_get_device_id()
 * @see sound_manager_free_device_list()
 */
int sound_manager_is_device_running_by_id(int device_id, bool *is_running);

/**
 * @internal
 * @brief Gets the device's supported sample formats.
 * @since_tizen 5.0
 *
 * @remarks	@a device_id should be #SOUND_DEVICE_USB_AUDIO type and an output device,\n
 *	otherwise #SOUND_MANAGER_ERROR_INVALID_OPERATION will be returned.\n
 *	Use sound_manager_get_device_io_direction() to check if it is an output device or not.\n
 *	The @a formats should be released using free().
 *
 * @param[in]	device_id	The device id
 * @param[out]	formats	The supported sample format list
 * @param[out]	num_of_elems	The number of elements in the sample format list
 * @return @c 0 on success,
 *         otherwise a negative error value
 * @retval #SOUND_MANAGER_ERROR_NONE Success
 * @retval #SOUND_MANAGER_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #SOUND_MANAGER_ERROR_INVALID_OPERATION Invalid operation
 * @retval #SOUND_MANAGER_ERROR_INTERNAL Internal error inside the sound system
 * @see sound_manager_set_sample_format_by_id()
 * @see sound_manager_get_sample_format_by_id()
 */
int sound_manager_get_supported_sample_formats_by_id(int device_id, sound_sample_format_e **formats, unsigned int *num_of_elems);

/**
 * @internal
 * @brief Sets the device's sample format.
 * @since_tizen 5.0
 *
 * @remarks	@a device_id should be #SOUND_DEVICE_USB_AUDIO type and an output device.\n
 *	@a format should be one of the values received from sound_manager_get_supported_sample_formats_by_id(),\n
 *	otherwise #SOUND_MANAGER_ERROR_INVALID_OPERATION will be returned.\n
 *	Use sound_manager_get_device_io_direction() to check if it is an output device or not.
 *
 * @param[in]	device_id	The device id
 * @param[in]	format	The sample format
 * @return @c 0 on success,
 *         otherwise a negative error value
 * @retval #SOUND_MANAGER_ERROR_NONE Success
 * @retval #SOUND_MANAGER_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #SOUND_MANAGER_ERROR_INVALID_OPERATION Invalid operation
 * @retval #SOUND_MANAGER_ERROR_INTERNAL Internal error inside the sound system
 * @see sound_manager_get_supported_sample_formats_by_id()
 * @see sound_manager_get_sample_format_by_id()
 */
int sound_manager_set_sample_format_by_id(int device_id, sound_sample_format_e format);

/**
 * @internal
 * @brief Gets the device's sample format.
 * @since_tizen 5.0
 *
 * @remarks	@a device_id should be #SOUND_DEVICE_USB_AUDIO type and an output device,\n
 *	otherwise #SOUND_MANAGER_ERROR_INVALID_OPERATION will be returned.\n
 *	Use sound_manager_get_device_io_direction() to check if it is an output device or not.
 *
 * @param[in]	device_id	The device id
 * @param[out]	format	The sample format
 * @return @c 0 on success,
 *         otherwise a negative error value
 * @retval #SOUND_MANAGER_ERROR_NONE Success
 * @retval #SOUND_MANAGER_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #SOUND_MANAGER_ERROR_INVALID_OPERATION Invalid operation
 * @retval #SOUND_MANAGER_ERROR_INTERNAL Internal error inside the sound system
 * @see sound_manager_get_supported_sample_formats_by_id()
 * @see sound_manager_set_sample_format_by_id()
 */
int sound_manager_get_sample_format_by_id(int device_id, sound_sample_format_e *format);

/**
 * @internal
 * @brief Gets the device's supported sample rates.
 * @since_tizen 5.0
 *
 * @remarks	@a device_id should be #SOUND_DEVICE_USB_AUDIO type and an output device,\n
 *	otherwise #SOUND_MANAGER_ERROR_INVALID_OPERATION will be returned.\n
 *	Use sound_manager_get_device_io_direction() to check if it is an output device or not.\n
 *	The @a rates should be released using free().
 *
 * @param[in]	device_id	The device id
 * @param[out]	rates	The supported sample rate list
 * @param[out]	num_of_elems	The number of elements in the sample rate list
 * @return @c 0 on success,
 *         otherwise a negative error value
 * @retval #SOUND_MANAGER_ERROR_NONE Success
 * @retval #SOUND_MANAGER_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #SOUND_MANAGER_ERROR_INVALID_OPERATION Invalid operation
 * @retval #SOUND_MANAGER_ERROR_INTERNAL Internal error inside the sound system
 * @see sound_manager_set_sample_rate_by_id()
 * @see sound_manager_get_sample_rate_by_id()
 */
int sound_manager_get_supported_sample_rates_by_id(int device_id, sound_sample_rate_e **rates, unsigned int *num_of_elems);

/**
 * @internal
 * @brief Sets the device's sample rate.
 * @since_tizen 5.0
 *
 * @remarks	@a device_id should be #SOUND_DEVICE_USB_AUDIO type and an output device.\n
 *	@a rate should be one of the values received from sound_manager_get_supported_sample_rates(),\n
 *	otherwise #SOUND_MANAGER_ERROR_INVALID_OPERATION will be returned.\n
 *	Use sound_manager_get_device_io_direction() to check if it is an output device or not.
 *
 * @param[in]	device_id	The device id
 * @param[in]	rate	The sample rate
 * @return @c 0 on success,
 *         otherwise a negative error value
 * @retval #SOUND_MANAGER_ERROR_NONE Success
 * @retval #SOUND_MANAGER_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #SOUND_MANAGER_ERROR_INVALID_OPERATION Invalid operation
 * @retval #SOUND_MANAGER_ERROR_INTERNAL Internal error inside the sound system
 * @see sound_manager_get_supported_sample_rates_by_id()
 * @see sound_manager_get_sample_rate_by_id()
 */
int sound_manager_set_sample_rate_by_id(int device_id, sound_sample_rate_e rate);

/**
 * @internal
 * @brief Gets the device's sample rate.
 * @since_tizen 5.0
 *
 * @remarks	@a device_id should be #SOUND_DEVICE_USB_AUDIO type and an output device,\n
 *	otherwise #SOUND_MANAGER_ERROR_INVALID_OPERATION will be returned.\n
 *	Use sound_manager_get_device_io_direction() to check if it is an output device or not.
 *
 * @param[in]	device_id	The device id
 * @param[out]	rate	The sample rate
 * @return @c 0 on success,
 *         otherwise a negative error value
 * @retval #SOUND_MANAGER_ERROR_NONE Success
 * @retval #SOUND_MANAGER_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #SOUND_MANAGER_ERROR_INVALID_OPERATION Invalid operation
 * @retval #SOUND_MANAGER_ERROR_INTERNAL Internal error inside the sound system
 * @see sound_manager_get_supported_sample_rates_by_id()
 * @see sound_manager_set_sample_rate_by_id()
 */
int sound_manager_get_sample_rate_by_id(int device_id, sound_sample_rate_e *rate);

/**
 * @internal
 * @brief Sets the device's 'avoid resampling' property.
 * @since_tizen 5.0
 *
 * @remarks	@a device_id should be #SOUND_DEVICE_USB_AUDIO type and an output device,\n
 *	otherwise #SOUND_MANAGER_ERROR_INVALID_OPERATION will be returned.\n
 *	Use sound_manager_get_device_io_direction() to check if it is an output device or not.\n
 *	The 'avoid resampling' property is not enabled as default. With this enabled, the device will\n
 *	use the first stream's original sample format and rate without resampling if supported.
 *
 * @param[in]	device_id	The device id
 * @param[in]	enable	The 'avoid resampling' property value to set: (@c true = enable, @c false = disable)
 * @return @c 0 on success,
 *         otherwise a negative error value
 * @retval #SOUND_MANAGER_ERROR_NONE Success
 * @retval #SOUND_MANAGER_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #SOUND_MANAGER_ERROR_INVALID_OPERATION Invalid operation
 * @retval #SOUND_MANAGER_ERROR_INTERNAL Internal error inside the sound system
 * @see sound_manager_get_avoid_resampling_by_id()
 */
int sound_manager_set_avoid_resampling_by_id(int device_id, bool enable);

/**
 * @internal
 * @brief Gets the device's 'avoid resampling' property.
 * @since_tizen 5.0
 *
 * @remarks	@a device_id should be #SOUND_DEVICE_USB_AUDIO type and an output device,\n
 *	otherwise #SOUND_MANAGER_ERROR_INVALID_OPERATION will be returned.\n
 *	Use sound_manager_get_device_io_direction() to check if it is an output device or not.\n
 *	The default value of 'avoid resampling' is false.
 *
 * @param[in]	device_id	The device id
 * @param[in]	enabled	The value of 'avoid resampling' property: (@c true = enabled, @c false = disabled)
 * @return @c 0 on success,
 *         otherwise a negative error value
 * @retval #SOUND_MANAGER_ERROR_NONE Success
 * @retval #SOUND_MANAGER_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #SOUND_MANAGER_ERROR_INVALID_OPERATION Invalid operation
 * @retval #SOUND_MANAGER_ERROR_INTERNAL Internal error inside the sound system
 * @see sound_manager_set_avoid_resampling_by_id()
 */
int sound_manager_get_avoid_resampling_by_id(int device_id, bool *enabled);

/**
 * @internal
 * @brief Sets the restriction of stream type only for media.
 * @since_tizen 5.0
 *
 * @remarks	@a device_id should be #SOUND_DEVICE_USB_AUDIO type and an output device,\n
 *	otherwise #SOUND_MANAGER_ERROR_INVALID_OPERATION will be returned.\n
 *	Use sound_manager_get_device_io_direction() to check if it is an output device or not.\n
 *	This property is not enabled as default. With this enabled, no other stream types\n
 *	except #SOUND_STREAM_TYPE_MEDIA is not allowed to @a device.
 *
 * @param[in]	device_id	The device id
 * @param[in]	enable	The 'media stream only' property value to set: (@c true = enable, @c false = disable)
 * @return @c 0 on success,
 *         otherwise a negative error value
 * @retval #SOUND_MANAGER_ERROR_NONE Success
 * @retval #SOUND_MANAGER_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #SOUND_MANAGER_ERROR_INVALID_OPERATION Invalid operation
 * @retval #SOUND_MANAGER_ERROR_INTERNAL Internal error inside the sound system
 * @see sound_manager_get_media_stream_only_by_id()
 */
int sound_manager_set_media_stream_only_by_id(int device_id, bool enable);

/**
 * @internal
 * @brief Gets the restriction of stream type only for media.
 * @since_tizen 5.0
 *
 * @remarks	@a device_id should be #SOUND_DEVICE_USB_AUDIO type and an output device,\n
 *	otherwise #SOUND_MANAGER_ERROR_INVALID_OPERATION will be returned.\n
 *	Use sound_manager_get_device_io_direction() to check if it is an output device or not.\n
 *	The property is not enabled as default.
 *
 * @param[in]	device_id	The device id
 * @param[out]	enabled	The value of 'media stream only' property: (@c true = enabled, @c false = disabled)
 * @return @c 0 on success,
 *         otherwise a negative error value
 * @retval #SOUND_MANAGER_ERROR_NONE Success
 * @retval #SOUND_MANAGER_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #SOUND_MANAGER_ERROR_INVALID_OPERATION Invalid operation
 * @retval #SOUND_MANAGER_ERROR_INTERNAL Internal error inside the sound system
 * @see sound_manager_set_media_stream_only_by_id()
 */
int sound_manager_get_media_stream_only_by_id(int device_id, bool *enabled);

/**
 * @internal
 * @brief Creates a virtual stream handle.
 * @since_tizen 3.0
 * @param[in]	stream_info	The handle of stream information
 * @param[out]	virtual_stream	The handle of virtual stream
 * @return @c 0 on success,
 *         otherwise a negative error value
 * @retval #SOUND_MANAGER_ERROR_NONE Success
 * @retval #SOUND_MANAGER_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #SOUND_MANAGER_ERROR_INTERNAL Internal error inside the sound system
 * @retval #SOUND_MANAGER_ERROR_NOT_SUPPORTED Not supported
 * @see sound_manager_create_stream_information()
 * @see sound_manager_destroy_stream_information()
 * @see sound_manager_destroy_virtual_stream()
 * @see sound_manager_start_virtual_stream()
 * @see sound_manager_stop_virtual_stream()
 */
int sound_manager_create_virtual_stream(sound_stream_info_h stream_info, virtual_sound_stream_h *virtual_stream);

/**
 * @internal
 * @brief Destroys the virtual stream handle.
 * @since_tizen 3.0
 * @param[in]	virtual_stream	The handle of virtual stream
 * @return @c 0 on success,
 *         otherwise a negative error value
 * @retval #SOUND_MANAGER_ERROR_NONE Success
 * @retval #SOUND_MANAGER_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #SOUND_MANAGER_ERROR_INVALID_STATE Invalid state
 * @see sound_manager_create_virtual_stream()
 * @see sound_manager_start_virtual_stream()
 * @see sound_manager_stop_virtual_stream()
 */
int sound_manager_destroy_virtual_stream(virtual_sound_stream_h virtual_stream);

/**
 * @internal
 * @brief Starts the virtual stream.
 * @since_tizen 3.0
 * @param[in]	virtual_stream	The handle of virtual stream
 * @return @c 0 on success,
 *         otherwise a negative error value
 * @retval #SOUND_MANAGER_ERROR_NONE Success
 * @retval #SOUND_MANAGER_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #SOUND_MANAGER_ERROR_INVALID_STATE Invalid state
 * @retval #SOUND_MANAGER_ERROR_INTERNAL Internal error inside the sound system
 * @see sound_manager_create_virtual_stream()
 * @see sound_manager_destroy_virtual_stream()
 * @see sound_manager_stop_virtual_stream()
 */
int sound_manager_start_virtual_stream(virtual_sound_stream_h virtual_stream);

/**
 * @internal
 * @brief Stops the virtual stream.
 * @since_tizen 3.0
 * @param[in]	virtual_stream	The handle of virtual stream
 * @return @c 0 on success,
 *         otherwise a negative error value
 * @retval #SOUND_MANAGER_ERROR_NONE Success
 * @retval #SOUND_MANAGER_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #SOUND_MANAGER_ERROR_INVALID_STATE Invalid state
 * @see sound_manager_create_virtual_stream()
 * @see sound_manager_destroy_virtual_stream()
 * @see sound_manager_start_virtual_stream()
 */
int sound_manager_stop_virtual_stream(virtual_sound_stream_h virtual_stream);

/**
 * @internal
 * @brief Sets the virtual stream volume.
 * @since_tizen 5.5
 * @param[in]	virtual_stream	The handle of virtual stream
 * @param[in]	ratio The volume ratio to be set (Min.:0.0 ~ Max.:1.0, default:1.0)
 * @return @c 0 on success,
 *         otherwise a negative error value
 * @retval #SOUND_MANAGER_ERROR_NONE Success
 * @retval #SOUND_MANAGER_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #SOUND_MANAGER_ERROR_INVALID_STATE Invalid state
 * @retval #SOUND_MANAGER_ERROR_INTERNAL Internal error inside the sound system
 * @see sound_manager_create_virtual_stream()
 * @see sound_manager_destroy_virtual_stream()
 * @see sound_manager_start_virtual_stream()
 */
int sound_manager_set_virtual_stream_volume(virtual_sound_stream_h virtual_stream, double ratio);

/**
 * @internal
 * @brief Set sound filter and apply to audio streams given selected stream type.
 * @since_tizen 4.0
 * @param[in]	stream_type		stream type to apply
 * @param[in]	filter			sound filter to apply
 * @return @c 0 on success,
 *         otherwise a negative error value
 * @retval #SOUND_MANAGER_ERROR_NONE Success
 * @retval #SOUND_MANAGER_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #SOUND_MANAGER_ERROR_INTERNAL Internal error inside the sound system
 * @retval #SOUND_MANAGER_ERROR_NOT_SUPPORTED Not supported
 * @see sound_manager_unset_filter()
 */
int sound_manager_set_filter(sound_stream_type_e stream_type, sound_filter_e filter);

/**
 * @internal
 * @brief Unset sound filter and remove from audio streams given selected stream type.
 * @since_tizen 4.0
 * @param[in]	stream_type		stream type to remove
 * @return @c 0 on success,
 *         otherwise a negative error value
 * @retval #SOUND_MANAGER_ERROR_NONE Success
 * @retval #SOUND_MANAGER_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #SOUND_MANAGER_ERROR_INTERNAL Internal error inside the sound system
 * @retval #SOUND_MANAGER_ERROR_NOT_SUPPORTED Not supported
 * @see sound_manager_set_filter()
 */
int sound_manager_unset_filter(sound_stream_type_e stream_type);

/**
 * @internal
 * @brief Apply preset configuration to the filter according to selected stream type.
 * @since_tizen 4.0
 * @param[in]	stream_type		given stream type
 * @param[in]	filter			given sound filter
 * @param[in]	preset			filter preset to apply
 * @return @c 0 on success,
 *         otherwise a negative error value
 * @retval #SOUND_MANAGER_ERROR_NONE Success
 * @retval #SOUND_MANAGER_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #SOUND_MANAGER_ERROR_INTERNAL Internal error inside the sound system
 * @retval #SOUND_MANAGER_ERROR_NOT_SUPPORTED Not supported
 * @see sound_manager_set_filter()
 */
int sound_manager_set_filter_preset(sound_stream_type_e stream_type, sound_filter_e filter, sound_filter_preset_e preset);

/**
 * @internal
 * @brief Gets the vendor id of the device.
 * @since_tizen 4.0
 *
 * @remarks	It works only with USB audio device. Otherwise, #SOUND_MANAGER_ERROR_NOT_SUPPORTED will be returned.\n
 *
 * @param[in]	device	The device item
 * @param[out]	vendor_id	The vendor id of the device
 * @return @c 0 on success,
 *         otherwise a negative error value
 * @retval #SOUND_MANAGER_ERROR_NONE Success
 * @retval #SOUND_MANAGER_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #SOUND_MANAGER_ERROR_NOT_SUPPORTED Not supported
 * @see sound_manager_get_device_list()
 * @see sound_manager_get_next_device()
 * @see sound_manager_get_prev_device()
 * @see sound_manager_get_device_type()
 * @see sound_manager_get_device_io_direction()
 * @see sound_manager_get_device_name()
 * @see sound_manager_get_device_id()
 * @see sound_manager_get_device_product_id()
 * @see sound_manager_free_device_list()
 */
int sound_manager_get_device_vendor_id(sound_device_h device, int *vendor_id);

/**
 * @internal
 * @brief Gets the product id of the device.
 * @since_tizen 4.0
 *
 * @remarks	It works only with USB audio device. Otherwise, #SOUND_MANAGER_ERROR_NOT_SUPPORTED will be returned.\n
 *
 * @param[in]	device	The device item
 * @param[out]	product_id	The product id of the device
 * @return @c 0 on success,
 *         otherwise a negative error value
 * @retval #SOUND_MANAGER_ERROR_NONE Success
 * @retval #SOUND_MANAGER_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #SOUND_MANAGER_ERROR_NOT_SUPPORTED Not supported
 * @see sound_manager_get_device_list()
 * @see sound_manager_get_next_device()
 * @see sound_manager_get_prev_device()
 * @see sound_manager_get_device_type()
 * @see sound_manager_get_device_io_direction()
 * @see sound_manager_get_device_name()
 * @see sound_manager_get_device_id()
 * @see sound_manager_get_device_vendor_id()
 * @see sound_manager_free_device_list()
 */
int sound_manager_get_device_product_id(sound_device_h device, int *product_id);

/**
 * @internal
 * @brief Checks if the stream information is using the device.
 * @since_tizen 5.5
 *
 * @param[in]	stream_info	The handle of stream information
 * @param[in]	device_id	The device id
 * @param[out]	is_on	Whether the stream info is using the device or not: (@c true = use, @c false = not use)
 * @return @c 0 on success,
 *         otherwise a negative error value
 * @retval #SOUND_MANAGER_ERROR_NONE Success
 * @retval #SOUND_MANAGER_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #SOUND_MANAGER_ERROR_INTERNAL Internal error inside the sound system
 * @see sound_manager_create_stream_information()
 * @see sound_manager_destroy_stream_information()
 * @see sound_manager_get_device_list()
 * @see sound_manager_get_next_device()
 * @see sound_manager_get_prev_device()
 * @see sound_manager_get_device_id()
 * @see sound_manager_get_device_name()
 * @see sound_manager_free_device_list()
 */
int sound_manager_is_stream_on_device_by_id(sound_stream_info_h stream_info, int device_id, bool *is_on);

/**
 * @internal
 * @brief Sets ACM master mode.
 * @since_tizen 5.5
 *
 * @remarks	The master mode is not enabled as default.
 *
 * @param[in]	enable	The master mode to be set: (@c true = master, @c false = slave)
 * @return @c 0 on success,
 *         otherwise a negative error value
 * @retval #SOUND_MANAGER_ERROR_NONE Success
 * @retval #SOUND_MANAGER_ERROR_INTERNAL Internal error inside the sound system
 */
int sound_manager_set_acm_master_mode(bool enable);

/**
 * @internal
 * @brief Enumeration for stream type.
 * @since_tizen 6.0
 * @remarks These values can be combined with bitwise 'or'.
 * @see sound_manager_get_latest_stream_pid()
 */
typedef enum {
	STREAM_TYPE_MEDIA             = 0x0001,
	STREAM_TYPE_SYSTEM            = 0x0002,
	STREAM_TYPE_ALARM             = 0x0004,
	STREAM_TYPE_NOTIFICATION      = 0x0008,
	STREAM_TYPE_EMERGENCY         = 0x0010,
	STREAM_TYPE_VOICE_INFORMATION = 0x0020
} stream_type_e;

/**
 * @internal
 * @brief Gets the PID of the latest stream among the streams of specific types.
 * @since_tizen 6.0
 *
 * @remarks	If there is no satisfied stream in the system, it'll return #SOUND_MANAGER_ERROR_NO_DATA.\n
 *
 * @param[in]	stream_type	The requested stream type that should be searched, values of #stream_type_e combined with bitwise 'or'
 * @param[out]	pid	The PID of the latest stream of the requested stream types among all the streams which are alive.
 * @return @c 0 on success,
 *         otherwise a negative error value
 * @retval #SOUND_MANAGER_ERROR_NONE Success
 * @retval #SOUND_MANAGER_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #SOUND_MANAGER_ERROR_INVALID_OPERATION Invalid operation
 * @retval #SOUND_MANAGER_ERROR_NO_DATA No data
 */
int sound_manager_get_latest_stream_pid(int stream_type, unsigned int *pid);

/**
 * @internal
 * @brief Called when the remote client is connected or disconnected.
 * @since_tizen 6.0
 * @remarks This callback indicates that the new remote client is connected or existing remote client is disconnected. \n
            The parameters are the identification of the remote client such as unique id, name, description, and recording or not, \n
            which can help the user to make a decision to allow access from the remote client using sound_manager_set_remote_permission().
 * @param[in]	id	The id of the remote client
 * @param[in]	name	The name of the remote client
 * @param[in]	is_recording	The flag indicating whether it's recording client or not
 * @param[in]	description	The brief description of the remote client
 * @param[in]	is_connected	The state of remote client connection: (@c true = connected, @c false = disconnected)
 * @param[in]	user_data	The user data passed from the callback registration function
 * @pre You should register this callback using sound_manager_publish_local_device().
 * @see sound_manager_publish_local_device()
 * @see sound_manager_unpublish_local_device()
 * @see sound_manager_set_allow_remote_access()
 */
typedef void (*sound_manager_remote_client_connected_cb) (int id, const char *name, bool is_recording,
                                                          const char *description, bool is_connected, void *user_data);

/**
 * @internal
 * @brief Publishes device's built-in audio devices to the local network.
 * @since_tizen 6.0
 * @privlevel public
 * @privilege %http://tizen.org/privilege/internet
 * @remarks This function publishes the device's built-in audio devices such as speakers and microphones to the local network. \n
 *          The registered callback function will be invoked when the remote clients discover published audio devices and the connection established.
 * @param[in]	callback	The remote client connection state changed callback function
 * @param[in]	user_data	The user data to be passed to the callback function
 * @return @c 0 on success,
 *         otherwise a negative error value
 * @retval #SOUND_MANAGER_ERROR_NONE Success
 * @retval #SOUND_MANAGER_ERROR_NOT_SUPPORTED Not supported
 * @retval #SOUND_MANAGER_ERROR_PERMISSION_DENIED Permission denied
 * @retval #SOUND_MANAGER_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #SOUND_MANAGER_ERROR_INVALID_OPERATION Invalid operation
 * @retval #SOUND_MANAGER_ERROR_INTERNAL Internal error inside the sound system
 * @post sound_manager_remote_client_connected_cb() will be invoked.
 * @see sound_manager_unpublish_local_device()
 * @see sound_manager_set_remote_permission()
 */
int sound_manager_publish_local_device(sound_manager_remote_client_connected_cb callback, void *user_data);

/**
 * @internal
 * @brief Unpublishes device's built-in audio devices to local network.
 * @since_tizen 6.0
 * @privlevel public
 * @privilege %http://tizen.org/privilege/internet
 * @remarks This function unpublishes the device's built-in audio devices to the local network.
 * @return @c 0 on success,
 *         otherwise a negative error value
 * @retval #SOUND_MANAGER_ERROR_NONE Success
 * @retval #SOUND_MANAGER_ERROR_NOT_SUPPORTED Not supported
 * @retval #SOUND_MANAGER_ERROR_PERMISSION_DENIED Permission denied
 * @retval #SOUND_MANAGER_ERROR_INVALID_OPERATION Invalid operation
 * @retval #SOUND_MANAGER_ERROR_INTERNAL Internal error inside the sound system
 * @pre You should have already published using sound_manager_publish_local_device().
 * @see sound_manager_publish_local_device()
 * @see sound_manager_set_remote_permission()
 */
int sound_manager_unpublish_local_device(void);

/**
 * @internal
 * @brief Sets the permission of accessing the local device from the remote client.
 * @since_tizen 6.0
 * @privlevel public
 * @privilege %http://tizen.org/privilege/internet
 * @param[in]	id The remote client id
 * @param[in]	allowed	The permission to allow accessing from the remote client.
 * @return @c 0 on success,
 *         otherwise a negative error value
 * @retval #SOUND_MANAGER_ERROR_NONE Success
 * @retval #SOUND_MANAGER_ERROR_NOT_SUPPORTED Not supported
 * @retval #SOUND_MANAGER_ERROR_PERMISSION_DENIED Permission denied
 * @retval #SOUND_MANAGER_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #SOUND_MANAGER_ERROR_INVALID_OPERATION Invalid operation
 * @retval #SOUND_MANAGER_ERROR_INTERNAL Internal error inside the sound system
 * @see sound_manager_publish_local_device()
 * @see sound_manager_unpublish_local_device()
 */
int sound_manager_set_remote_permission(int id, bool allowed);

/**
 * @internal
 * @brief Starts discovering of published remote devices on the local network.
 * @since_tizen 6.0
 * @privlevel public
 * @privilege %http://tizen.org/privilege/internet
 * @return @c 0 on success,
 *         otherwise a negative error value
 * @retval #SOUND_MANAGER_ERROR_NONE Success
 * @retval #SOUND_MANAGER_ERROR_NOT_SUPPORTED Not supported
 * @retval #SOUND_MANAGER_ERROR_PERMISSION_DENIED Permission denied
 * @retval #SOUND_MANAGER_ERROR_INVALID_OPERATION Invalid operation
 * @retval #SOUND_MANAGER_ERROR_INTERNAL Internal error inside the sound system
 * @see sound_manager_stop_discover_remote_device()
 */
int sound_manager_start_discover_remote_device(void);

/**
 * @internal
 * @brief Stops discovering of published remote devices on the local network.
 * @since_tizen 6.0
 * @privlevel public
 * @privilege %http://tizen.org/privilege/internet
 * @return @c 0 on success,
 *         otherwise a negative error value
 * @retval #SOUND_MANAGER_ERROR_NONE Success
 * @retval #SOUND_MANAGER_ERROR_NOT_SUPPORTED Not supported
 * @retval #SOUND_MANAGER_ERROR_PERMISSION_DENIED Permission denied
 * @retval #SOUND_MANAGER_ERROR_INVALID_OPERATION Invalid operation
 * @retval #SOUND_MANAGER_ERROR_INTERNAL Internal error inside the sound system
 * @pre You should start discovering by sound_manager_start_discover_remote_device().
 * @see sound_manager_start_discover_remote_device()
 */
int sound_manager_stop_discover_remote_device(void);

/**
 * @internal
 * @brief Set force hdmi route
 * @since_tizen 6.5
 * @param[in]	type The playback route type for rpi
 * @return @c 0 on success,
 *         otherwise a negative error value
 * @retval #SOUND_MANAGER_ERROR_NONE Success
 * @retval #SOUND_MANAGER_ERROR_INTERNAL Internal error inside the sound system
 * @retval #SOUND_MANAGER_ERROR_INVALID_OPERATION Invalid operation
 * @see sound_manager_get_rpi_playback_route()
 */
int sound_manager_set_rpi_playback_route(sound_rpi_playback_route_type type);

/**
 * @internal
 * @brief Get force hdmi route status
 * @since_tizen 6.5
 * @remarks The default value is #SOUND_RPI_PLAYBACK_HEADPHONE.
 * @param[out]	type The current playback route type of rpi
 * @return @c 0 on success,
 *         otherwise a negative error value
 * @retval #SOUND_MANAGER_ERROR_NONE Success
 * @retval #SOUND_MANAGER_ERROR_INTERNAL Internal error inside the sound system
 * @retval #SOUND_MANAGER_ERROR_INVALID_OPERATION Invalid operation
 * @see sound_manager_set_rpi_playback_route()
 */
int sound_manager_get_rpi_playback_route(sound_rpi_playback_route_type *type);


/**
 * @internal
 * @brief Sets the host volume level specified for a particular sound type.
 * @since_tizen 7.0
 * @privlevel public
 * @privilege %http://tizen.org/privilege/volume.set
 * @param[in]	type	The sound type
 * @param[in]	volume	The volume level to be set
 * @return @c 0 on success,
 *         otherwise a negative error value
 * @retval #SOUND_MANAGER_ERROR_NONE Success
 * @retval #SOUND_MANAGER_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #SOUND_MANAGER_ERROR_PERMISSION_DENIED Permission denied
 * @retval #SOUND_MANAGER_ERROR_INTERNAL Internal error inside the sound system
 */
int sound_manager_set_host_volume(sound_type_e type, int volume);

/**
 * @internal
 * @brief Gets the host volume level specified for a particular sound type.
 * @since_tizen 7.0
 * @param[in]	type	The sound type
 * @param[out]	volume	The current volume level
 * @return @c 0 on success,
 *         otherwise a negative error value
 * @retval #SOUND_MANAGER_ERROR_NONE Success
 * @retval #SOUND_MANAGER_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #SOUND_MANAGER_ERROR_INTERNAL Internal error inside the sound system
 */
int sound_manager_get_host_volume(sound_type_e type, int *volume);

/**
 * @internal
 * @brief Gets the sample format of the device.
 * @since_tizen 7.0
 *
 * @param[in]	device	The device item
 * @param[out]	format	The sample format of the device
 * @return @c 0 on success,
 *         otherwise a negative error value
 * @retval #SOUND_MANAGER_ERROR_NONE Success
 * @retval #SOUND_MANAGER_ERROR_INVALID_PARAMETER Invalid parameter
 * @see sound_manager_get_device_list()
 * @see sound_manager_get_next_device()
 * @see sound_manager_get_prev_device()
 * @see sound_manager_get_device_type()
 * @see sound_manager_get_device_io_direction()
 * @see sound_manager_get_device_name()
 * @see sound_manager_get_device_id()
 * @see sound_manager_get_device_sample_rate()
 * @see sound_manager_get_device_channels()
 * @see sound_manager_free_device_list()
 */
int sound_manager_get_device_sample_format(sound_device_h device, sound_sample_format_e *format);

/**
 * @internal
 * @brief Gets the sample rate of the device.
 * @since_tizen 7.0
 *
 * @param[in]	device	The device item
 * @param[out]	samplerate	The sample rate of the device
 * @return @c 0 on success,
 *         otherwise a negative error value
 * @retval #SOUND_MANAGER_ERROR_NONE Success
 * @retval #SOUND_MANAGER_ERROR_INVALID_PARAMETER Invalid parameter
 * @see sound_manager_get_device_list()
 * @see sound_manager_get_next_device()
 * @see sound_manager_get_prev_device()
 * @see sound_manager_get_device_type()
 * @see sound_manager_get_device_io_direction()
 * @see sound_manager_get_device_name()
 * @see sound_manager_get_device_id()
 * @see sound_manager_get_device_sample_format()
 * @see sound_manager_get_device_channels()
 * @see sound_manager_free_device_list()
 */
int sound_manager_get_device_sample_rate(sound_device_h device, sound_sample_rate_e *samplerate);

/**
 * @internal
 * @brief Gets the number of channels of the device.
 * @since_tizen 7.0
 *
 * @param[in]	device	The device item
 * @param[out]	channels	The number of the channels of the device
 * @return @c 0 on success,
 *         otherwise a negative error value
 * @retval #SOUND_MANAGER_ERROR_NONE Success
 * @retval #SOUND_MANAGER_ERROR_INVALID_PARAMETER Invalid parameter
 * @see sound_manager_get_device_list()
 * @see sound_manager_get_next_device()
 * @see sound_manager_get_prev_device()
 * @see sound_manager_get_device_type()
 * @see sound_manager_get_device_io_direction()
 * @see sound_manager_get_device_name()
 * @see sound_manager_get_device_id()
 * @see sound_manager_get_device_sample_format()
 * @see sound_manager_get_device_sample_rate()
 * @see sound_manager_free_device_list()
 */
int sound_manager_get_device_channels(sound_device_h device, int *channels);

/**
 * @internal
 * @brief Gets the current device id for media playback stream.
 * @since_tizen 7.5
 *
 * @param[out]	device_id	The output device id that a media playback stream can go out
 * @return @c 0 on success,
 *         otherwise a negative error value
 * @retval #SOUND_MANAGER_ERROR_NONE Success
 * @retval #SOUND_MANAGER_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #SOUND_MANAGER_ERROR_NO_DATA No data
 * @see sound_manager_get_current_media_playback_device_type()
 */
int sound_manager_get_current_media_playback_device_id(int *device_id);

/**
 * @internal
 * @brief Sets sound effect methods that must need a reference device such as acoustic echo cancellation.
 * @since_tizen 8.0
 * @param[in]	stream_info	The handle of stream information
 * @param[in]	method	The method of #sound_effect_method_with_reference_e
 * @param[in]	device_id	The reference device id
 * @return @c 0 on success,
 *         otherwise a negative error value
 * @retval #SOUND_MANAGER_ERROR_NONE Success
 * @retval #SOUND_MANAGER_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #SOUND_MANAGER_ERROR_POLICY Noncompliance with the sound system policy
 * @pre Get a device by calling sound_manager_get_device_list() and sound_manager_get_next_device().
 * @see sound_manager_create_stream_information()
 * @see sound_manager_set_effect_method()
 * @see sound_manager_get_effect_method_with_reference()
 */
int sound_manager_set_effect_method_with_reference_by_id(sound_stream_info_h stream_info, sound_effect_method_with_reference_e method, int device_id);

/**
 * @internal
 * @brief Gets the PIDs of streams that are currently recording.
 * @since_tizen 11.0
 *
 * @remarks    This function retrieves the PIDs of all streams that are currently
 *             recording from a running source. If there are multiple source-outputs,
 *             multiple PIDs will be returned.
 *             Duplicate PIDs are not included in the returned array.
 *             The @a pids should be released using free().
 *
 * @param[out]	pids	The array of PIDs of recording streams
 * @param[out]	num	The number of PIDs in the array
 * @return @c 0 on success,
 *         otherwise a negative error value
 * @retval #SOUND_MANAGER_ERROR_NONE Success
 * @retval #SOUND_MANAGER_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #SOUND_MANAGER_ERROR_NO_DATA No recording stream
 * @retval #SOUND_MANAGER_ERROR_INTERNAL Internal error inside the sound system
 */
int sound_manager_get_recording_stream_pids(unsigned int **pids, unsigned int *num);

/**
 * @}
 */

#ifdef __cplusplus
}
#endif

#endif /* __TIZEN_MEDIA_SOUND_MANAGER_INTERNAL_H__ */
