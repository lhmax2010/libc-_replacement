/*
* Copyright (c) 2011 Samsung Electronics Co., Ltd All Rights Reserved
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

#ifndef __TIZEN_MEDIA_SOUND_MANAGER_H__
#define __TIZEN_MEDIA_SOUND_MANAGER_H__

#include <tizen.h>

#ifdef __cplusplus
extern "C"
{
#endif

/**
 * @file sound_manager.h
 * @brief This file contains the Sound Manager API.
 */

/**
 * @addtogroup CAPI_MEDIA_SOUND_MANAGER_MODULE
 * @{
 */

/**
 * @brief Enumeration for sound manager's error codes.
 * @since_tizen 2.3
 */
typedef enum {
	SOUND_MANAGER_ERROR_NONE              = TIZEN_ERROR_NONE,                    /**< Successful */
	SOUND_MANAGER_ERROR_OUT_OF_MEMORY     = TIZEN_ERROR_OUT_OF_MEMORY,           /**< Out of memory */
	SOUND_MANAGER_ERROR_INVALID_PARAMETER = TIZEN_ERROR_INVALID_PARAMETER,       /**< Invalid parameter */
	SOUND_MANAGER_ERROR_INVALID_OPERATION = TIZEN_ERROR_INVALID_OPERATION,       /**< Invalid operation */
	SOUND_MANAGER_ERROR_PERMISSION_DENIED = TIZEN_ERROR_PERMISSION_DENIED,       /**< Permission denied */
	SOUND_MANAGER_ERROR_NOT_SUPPORTED     = TIZEN_ERROR_NOT_SUPPORTED,           /**< Not supported */
	SOUND_MANAGER_ERROR_NO_DATA           = TIZEN_ERROR_NO_DATA,                 /**< No data */
	SOUND_MANAGER_ERROR_INTERNAL          = TIZEN_ERROR_SOUND_MANAGER | 01,      /**< Internal error inside the sound system */
	SOUND_MANAGER_ERROR_POLICY            = TIZEN_ERROR_SOUND_MANAGER | 02,      /**< Noncompliance with the sound system policy */
	SOUND_MANAGER_ERROR_NO_PLAYING_SOUND  = TIZEN_ERROR_SOUND_MANAGER | 03,      /**< No playing sound */
	SOUND_MANAGER_ERROR_INVALID_STATE     = TIZEN_ERROR_SOUND_MANAGER | 04,      /**< Invalid state (Since 3.0) */
} sound_manager_error_e;

/**
 * @addtogroup CAPI_MEDIA_SOUND_MANAGER_VOLUME_MODULE
 * @{
 */

/**
 * @brief Enumeration for sound type.
 * @since_tizen 2.3
 */
typedef enum {
	SOUND_TYPE_SYSTEM,          /**< Sound type for system */
	SOUND_TYPE_NOTIFICATION,    /**< Sound type for notifications */
	SOUND_TYPE_ALARM,           /**< Sound type for alarm */
	SOUND_TYPE_RINGTONE,        /**< Sound type for ringtones (Since 2.4) */
	SOUND_TYPE_MEDIA,           /**< Sound type for media */
	SOUND_TYPE_CALL,            /**< Sound type for call (Since 2.4) */
	SOUND_TYPE_VOIP,            /**< Sound type for voip (Since 2.4) */
	SOUND_TYPE_VOICE,           /**< Sound type for voice */
} sound_type_e;

/**
 * @brief Sound stream ducking handle.
 * @since_tizen 5.5
 */
typedef struct sound_stream_ducking_s* sound_stream_ducking_h;

/**
 * @}
 */

/**
 * @addtogroup CAPI_MEDIA_SOUND_MANAGER_STREAM_POLICY_MODULE
 * @{
 */

/**
 * @brief Sound stream information handle.
 * @since_tizen 3.0
 */
typedef struct sound_stream_info_s* sound_stream_info_h;

/**
 * @brief Enumeration for sound stream type.
 * @since_tizen 3.0
 */
typedef enum {
	SOUND_STREAM_TYPE_MEDIA,              /**< Sound stream type for media */
	SOUND_STREAM_TYPE_SYSTEM,             /**< Sound stream type for system */
	SOUND_STREAM_TYPE_ALARM,              /**< Sound stream type for alarm */
	SOUND_STREAM_TYPE_NOTIFICATION,       /**< Sound stream type for notification */
	SOUND_STREAM_TYPE_EMERGENCY,          /**< Sound stream type for emergency */
	SOUND_STREAM_TYPE_VOICE_INFORMATION,  /**< Sound stream type for voice information */
	SOUND_STREAM_TYPE_VOICE_RECOGNITION,  /**< Sound stream type for voice recognition */
	SOUND_STREAM_TYPE_RINGTONE_VOIP,      /**< Sound stream type for ringtone for VoIP */
	SOUND_STREAM_TYPE_VOIP,               /**< Sound stream type for VoIP */
	SOUND_STREAM_TYPE_MEDIA_EXTERNAL_ONLY,/**< Sound stream type for media only for external devices */
} sound_stream_type_e;

/**
 * @brief Enumeration for change reason of sound stream focus state.
 * @since_tizen 3.0
 */
typedef enum {
	SOUND_STREAM_FOCUS_CHANGED_BY_MEDIA,              /**< Changed by the stream type for media */
	SOUND_STREAM_FOCUS_CHANGED_BY_SYSTEM,             /**< Changed by the stream type for system */
	SOUND_STREAM_FOCUS_CHANGED_BY_ALARM,              /**< Changed by the stream type for alarm */
	SOUND_STREAM_FOCUS_CHANGED_BY_NOTIFICATION,       /**< Changed by the stream type for notification */
	SOUND_STREAM_FOCUS_CHANGED_BY_EMERGENCY,          /**< Changed by the stream type for emergency */
	SOUND_STREAM_FOCUS_CHANGED_BY_VOICE_INFORMATION,  /**< Changed by the stream type for voice information */
	SOUND_STREAM_FOCUS_CHANGED_BY_VOICE_RECOGNITION,  /**< Changed by the stream type for voice recognition */
	SOUND_STREAM_FOCUS_CHANGED_BY_RINGTONE,           /**< Changed by the stream type for ringtone */
	SOUND_STREAM_FOCUS_CHANGED_BY_VOIP,               /**< Changed by the stream type for VoIP */
	SOUND_STREAM_FOCUS_CHANGED_BY_CALL,               /**< Changed by the stream type for voice-call or video-call */
	SOUND_STREAM_FOCUS_CHANGED_BY_MEDIA_EXTERNAL_ONLY,/**< Changed by the stream type for media only for external devices */
} sound_stream_focus_change_reason_e;

/**
 * @brief Enumeration for sound stream focus mask.
 * @since_tizen 3.0
 */
typedef enum {
	SOUND_STREAM_FOCUS_FOR_PLAYBACK    = 0x0001,   /**< Mask for playback focus */
	SOUND_STREAM_FOCUS_FOR_RECORDING   = 0x0002,   /**< Mask for recording focus */
	SOUND_STREAM_FOCUS_FOR_BOTH        = 0x0003,   /**< Mask for both playback and recording focus */
} sound_stream_focus_mask_e;

/**
 * @brief Enumeration for sound stream focus state.
 * @since_tizen 3.0
 */
typedef enum {
	SOUND_STREAM_FOCUS_STATE_RELEASED,   /**< Focus state for release */
	SOUND_STREAM_FOCUS_STATE_ACQUIRED,   /**< Focus state for acquisition */
} sound_stream_focus_state_e;

/**
 * @brief Enumeration for sound behavior.
 * @since_tizen 3.0
 * @remarks These values can be combined with bitwise 'or'.
 * @see sound_manager_get_current_playback_focus()
 * @see sound_manager_get_current_recording_focus()
 */
typedef enum {
	SOUND_BEHAVIOR_NONE          = 0x0000,   /**< None (default) */
	SOUND_BEHAVIOR_NO_RESUME     = 0x0001,   /**< No resumption */
	SOUND_BEHAVIOR_FADING        = 0x0002,   /**< Fading effect */
} sound_behavior_e;

/**
 * @deprecated Deprecated since 11.0.
 * @brief Definition for the value indicating that the preferred device for the stream information was not set.
 * @since_tizen 5.5
 */
#define SOUND_MANAGER_STREAM_NO_PREFERRED_DEVICE    0

/**
 * @}
 */

/**
 * @addtogroup CAPI_MEDIA_SOUND_MANAGER_DEVICE_MODULE
 * @{
 */

/**
 * @brief Sound device handle.
 * @since_tizen 2.3
 */
typedef void* sound_device_h;

/**
 * @brief Sound device list handle.
 * @since_tizen 2.3
 */
typedef void* sound_device_list_h;

/**
* @brief Enumeration for sound device type.
* @since_tizen 2.3
*/
typedef enum {
	SOUND_DEVICE_BUILTIN_SPEAKER,   /**< Built-in speaker */
	SOUND_DEVICE_BUILTIN_RECEIVER,  /**< Built-in receiver */
	SOUND_DEVICE_BUILTIN_MIC,       /**< Built-in mic */
	SOUND_DEVICE_AUDIO_JACK,        /**< Audio jack that can be connected to wired accessory such as headphone, headset, and so on */
	SOUND_DEVICE_BLUETOOTH_MEDIA,   /**< Bluetooth device representing media (A2DP) profile (Since 3.0) */
	SOUND_DEVICE_HDMI,              /**< HDMI */
	SOUND_DEVICE_FORWARDING,        /**< Device for forwarding (Since 3.0) */
	SOUND_DEVICE_USB_AUDIO,         /**< USB Audio */
	SOUND_DEVICE_BLUETOOTH_VOICE,   /**< Bluetooth device representing voice (SCO) profile (Since 3.0) */
	SOUND_DEVICE_NETWORK,           /**< Network (Since 5.5) */
} sound_device_type_e;

/**
* @brief Enumeration for sound device direction.
* @since_tizen 2.3
*/
typedef enum {
	SOUND_DEVICE_IO_DIRECTION_IN,   /**< Input device */
	SOUND_DEVICE_IO_DIRECTION_OUT,  /**< Output device */
	SOUND_DEVICE_IO_DIRECTION_BOTH, /**< Input/output device (both directions are available) */
} sound_device_io_direction_e;


/**
* @brief Enumeration for sound device mask.
* @since_tizen 2.3
*/
typedef enum {
	SOUND_DEVICE_IO_DIRECTION_IN_MASK      = 0x0001,  /**< Mask for input devices */
	SOUND_DEVICE_IO_DIRECTION_OUT_MASK     = 0x0002,  /**< Mask for output devices */
	SOUND_DEVICE_IO_DIRECTION_BOTH_MASK    = 0x0004,  /**< Mask for input/output devices (both directions are available) */
	SOUND_DEVICE_TYPE_INTERNAL_MASK        = 0x0010,  /**< Mask for built-in devices. @deprecated Deprecated since 11.0 */
	SOUND_DEVICE_TYPE_EXTERNAL_MASK        = 0x0020,  /**< Mask for external devices. @deprecated Deprecated since 11.0 */
	SOUND_DEVICE_ALL_MASK                  = 0xFFFF,  /**< Mask for all devices */
} sound_device_mask_e;

/**
* @deprecated Deprecated since 11.0.
* @brief Enumeration for sound sample format.
* @since_tizen 5.0
*/
typedef enum {
	SOUND_SAMPLE_FORMAT_U8,         /**< Unsigned 8 bit samples */
	SOUND_SAMPLE_FORMAT_S16_LE,     /**< Signed 16 bit samples */
	SOUND_SAMPLE_FORMAT_S24_LE,     /**< Signed 24 bit samples */
	SOUND_SAMPLE_FORMAT_S24_32_LE,  /**< Signed 24 bit(packed in 32 bit) samples */
} sound_sample_format_e;

/**
* @deprecated Deprecated since 11.0.
* @brief Enumeration for sound sample rate.
* @since_tizen 5.0
*/
typedef enum {
	SOUND_SAMPLE_RATE_8000,    /**< 8000 Hz sample rate */
	SOUND_SAMPLE_RATE_16000,   /**< 16000 Hz sample rate */
	SOUND_SAMPLE_RATE_22050,   /**< 22050 Hz sample rate */
	SOUND_SAMPLE_RATE_44100,   /**< 44100 Hz sample rate */
	SOUND_SAMPLE_RATE_48000,   /**< 48000 Hz sample rate */
	SOUND_SAMPLE_RATE_88200,   /**< 88200 Hz sample rate */
	SOUND_SAMPLE_RATE_96000,   /**< 96000 Hz sample rate */
	SOUND_SAMPLE_RATE_192000,  /**< 192000 Hz sample rate */
} sound_sample_rate_e;

/**
 * @}
 */

/**
 * @addtogroup CAPI_MEDIA_SOUND_MANAGER_STREAM_POLICY_MODULE
 * @{
 */

/**
* @brief Enumeration for sound effects that need a reference device.
* @since_tizen 8.0
*/
typedef enum {
	SOUND_EFFECT_REFERENCE_COPY              = 0x0001, /**< Including reference source */
	SOUND_EFFECT_ACOUSTIC_ECHO_CANCEL_SPEEX  = 0x0002, /**< Acoustic echo cancel with speex */
	SOUND_EFFECT_ACOUSTIC_ECHO_CANCEL_WEBRTC = 0x0004, /**< Acoustic echo cancel with webrtc */
} sound_effect_method_with_reference_e;

/**
* @brief Enumeration for sound effects.
* @since_tizen 8.0
*/
typedef enum {
	SOUND_EFFECT_NOISE_SUPPRESSION_VOIP          = 0x0001, /**< Noise suppression for voice call */
	SOUND_EFFECT_AUTOMATIC_GAIN_CONTROL_CAPTURE  = 0x0002, /**< Auto Gain Control for normal capturing */
} sound_effect_method_e;

/**
 * @}
 */

/**
 * @addtogroup CAPI_MEDIA_SOUND_MANAGER_VOLUME_MODULE
 * @{
 */

/**
 * @brief Called when the system volume has changed.
 * @since_tizen 2.3
 * @param[in]   type	The sound type of the changed volume
 * @param[in]   volume	The new volume value
 * @param[in]   user_data	The user data passed from the callback registration function
 * @pre sound_manager_set_volume() will invoke this callback if you register it using sound_manager_add_volume_changed_cb().
 * @see sound_manager_add_volume_changed_cb()
 * @see sound_manager_remove_volume_changed_cb()
 */
typedef void (*sound_manager_volume_changed_cb) (sound_type_e type, unsigned int volume, void *user_data);

/**
 * @brief Called when the ducking activation or deactivation is finished.
 * @since_tizen 5.5
 *
 * @remarks	This function is invoked by the internal thread of the sound manager.
 *	Therefore it is recommended not to call functions which update UI in this callback.
 * @remarks	@a stream_ducking is the same handle as the parameter in sound_manager_create_stream_ducking() that registers this callback,
 *	so the handle should not be released in this callback.
 *
 * @param[in]   stream_ducking	The stream ducking handle
 * @param[in]   is_ducked	The flag whether it's ducked or not
 * @param[in]   user_data	The user data passed from the callback registration function
 * @pre You should register this callback when sound_manager_create_stream_ducking() is called.
 * @see sound_manager_create_stream_ducking()
 */
typedef void (*sound_stream_ducking_state_changed_cb) (sound_stream_ducking_h stream_ducking, bool is_ducked, void *user_data);

/**
 * @}
 */

/**
 * @addtogroup CAPI_MEDIA_SOUND_MANAGER_STREAM_POLICY_MODULE
 * @{
 */

/**
 * @brief Called when the state of focus that belongs to the stream_info is changed.
 * @since_tizen 3.0
 *
 * @remarks	This function is issued in the internal thread of the sound manager.\n
 *	Therefore it is recommended not to call UI update function in this function.
 *
 * @param[in]   stream_info	The handle of stream information
 * @param[in]   focus_mask	The changed focus mask
 * @param[in]   focus_state	The changed focus state
 * @param[in]   reason		The reason for state change of the focus
 * @param[in]   sound_behavior	The requested sound behavior that should be followed, values of #sound_behavior_e combined with bitwise 'or'
 * @param[in]   extra_info	The extra information
 * @param[in]   user_data	The user data passed from the callback registration function
 * @pre You should register this callback using sound_manager_create_stream_information().
 * @see sound_manager_create_stream_information()
 * @see sound_manager_destroy_stream_information()
 * @see sound_manager_get_focus_state()
 */
typedef void (*sound_stream_focus_state_changed_cb) (sound_stream_info_h stream_info,
													sound_stream_focus_mask_e focus_mask,
													sound_stream_focus_state_e focus_state,
													sound_stream_focus_change_reason_e reason,
													int sound_behavior,
													const char *extra_info,
													void *user_data);

/**
 * @brief Called when the focus state for each sound stream type is changed regardless of the process.
 * @since_tizen 3.0
 *
 * @remarks	This function is invoked by the internal thread of the sound manager.\n
 *	Therefore it is recommended not to call functions which update the UI from this callback.
 *
 * @param[in]   id		The focus state change watch cb id
 * @param[in]   focus_mask	The changed focus mask
 * @param[in]   focus_state	The changed focus state
 * @param[in]   reason		The reason for state change of the focus
 * @param[in]   extra_info	The extra information
 * @param[in]   user_data	The user data passed from the callback registration function
 * @pre You should register this callback using sound_manager_add_focus_state_watch_cb().
 * @see sound_manager_add_focus_state_watch_cb()
 * @see sound_manager_remove_focus_state_watch_cb()
 */
typedef void (*sound_stream_focus_state_watch_cb) (int id, sound_stream_focus_mask_e focus_mask, sound_stream_focus_state_e focus_state, sound_stream_focus_change_reason_e reason, const char *extra_info, void *user_data);

/**
 * @}
 */

/**
 * @addtogroup CAPI_MEDIA_SOUND_MANAGER_DEVICE_MODULE
 * @{
 */

/**
 * @brief Called when the connection state of a sound device was changed.
 * @since_tizen 3.0
 *
 * @remarks @a device is freed by the platform after this callback exits.
 *
 * @param[in]   device	The sound_device
 * @param[in]   is_connected	The state of device connection: (@c true = connected, @c false = disconnected)
 * @param[in]   user_data	The user data passed from the callback registration function
 * @pre You should add this callback using sound_manager_add_device_connection_changed_cb().
 * @see sound_manager_add_device_connection_changed_cb()
 * @see sound_manager_remove_device_connection_changed_cb()
 */
typedef void (*sound_device_connection_changed_cb) (sound_device_h device, bool is_connected, void *user_data);

/**
 * @brief Called when the state of a sound device was changed.
 * @since_tizen 5.0
 *
 * @remarks @a device is freed by the platform after this callback exits.
 *
 * @param[in]   device	The sound_device
 * @param[in]   is_running	The state of the device: (@c true = running, @c false = not running)
 * @param[in]   user_data	The user data passed from the callback registration function
 * @pre You should add this callback using sound_manager_add_device_running_changed_cb().
 * @see sound_manager_add_device_running_changed_cb()
 * @see sound_manager_remove_device_running_changed_cb()
 */
typedef void (*sound_device_running_changed_cb) (sound_device_h device, bool is_running, void *user_data);

/**
 * @}
 */

/**
 * @addtogroup CAPI_MEDIA_SOUND_MANAGER_VOLUME_MODULE
 * @{
 */

/**
 * @brief Gets the maximum volume level supported for a particular sound type.
 * @since_tizen 2.3
 * @param[in]		type The sound type
 * @param[out]	max	The maximum volume level
 * @return @c 0 on success,
 *         otherwise a negative error value
 * @retval #SOUND_MANAGER_ERROR_NONE Success
 * @retval #SOUND_MANAGER_ERROR_INVALID_PARAMETER Invalid parameter
 * @see sound_manager_set_volume()
 * @see sound_manager_get_volume()
 */
int sound_manager_get_max_volume(sound_type_e type, int *max);

/**
 * @brief Sets the volume level specified for a particular sound type.
 * @since_tizen 2.3
 * @privlevel public
 * @privilege %http://tizen.org/privilege/volume.set
 * @param[in]		type The sound type
 * @param[in]		volume	The volume level to be set
 * @return @c 0 on success,
 *         otherwise a negative error value
 * @retval #SOUND_MANAGER_ERROR_NONE Success
 * @retval #SOUND_MANAGER_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #SOUND_MANAGER_ERROR_PERMISSION_DENIED Permission denied
 * @retval #SOUND_MANAGER_ERROR_INTERNAL Internal error inside the sound system
 * @see sound_manager_get_max_volume()
 * @see sound_manager_get_volume()
 */
int sound_manager_set_volume(sound_type_e type, int volume);

/**
 * @brief Gets the volume level specified for a particular sound type.
 * @since_tizen 2.3
 * @param[in]		type The sound type
 * @param[out]	volume	The current volume level
 * @return @c 0 on success,
 *         otherwise a negative error value
 * @retval #SOUND_MANAGER_ERROR_NONE Success
 * @retval #SOUND_MANAGER_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #SOUND_MANAGER_ERROR_INTERNAL Internal error inside the sound system
 * @see sound_manager_get_max_volume()
 * @see sound_manager_set_volume()
 */
int sound_manager_get_volume(sound_type_e type, int *volume);

/**
 * @brief Gets the type of the sound being currently played.
 * @since_tizen 2.3
 * @param[out]	type The current sound type
 * @return @c 0 on success,
 *         otherwise a negative error value
 * @retval #SOUND_MANAGER_ERROR_NONE Success
 * @retval #SOUND_MANAGER_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #SOUND_MANAGER_ERROR_NO_PLAYING_SOUND No playing sound
 * @retval #SOUND_MANAGER_ERROR_INTERNAL Internal error inside the sound system
 */
int sound_manager_get_current_sound_type(sound_type_e *type);

/**
 * @brief Adds a callback function to be invoked when the volume level is changed.
 * @since_tizen 3.0
 * @param[in]	callback	Callback function to indicate change in volume
 * @param[in]	user_data	The user data to be passed to the callback function
 * @param[out]	id	The callback id
 * @return @c 0 on success,
 *         otherwise a negative error value
 * @retval #SOUND_MANAGER_ERROR_NONE Success
 * @retval #SOUND_MANAGER_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #SOUND_MANAGER_ERROR_INTERNAL Internal error inside the sound system
 * @post sound_manager_volume_changed_cb() will be invoked.
 * @see sound_manager_remove_volume_changed_cb()
 * @see sound_manager_volume_changed_cb()
 */
int sound_manager_add_volume_changed_cb(sound_manager_volume_changed_cb callback, void *user_data, int *id);

/**
 * @brief Removes the volume change callback.
 * @since_tizen 3.0
 * @param[in]	id	The id of the callback to remove
 * @return 0 on success, otherwise a negative error value
 * @retval #SOUND_MANAGER_ERROR_NONE Success
 * @retval #SOUND_MANAGER_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #SOUND_MANAGER_ERROR_INTERNAL Internal error inside the sound system
 * @see sound_manager_add_volume_changed_cb()
 */
int sound_manager_remove_volume_changed_cb(int id);

/**
 * @brief Creates a handle for stream ducking.
 * @since_tizen 5.5
 *
 * @remarks @a stream_ducking should be released using sound_manager_destroy_stream_ducking().
 *
 * @param[in]	target_stream	The type of target stream
 * @param[in]	callback		The callback function called when ducking state is changed (optional, this can be NULL)
 * @param[in]	user_data		The user data to be passed to the callback function (optional, this can be NULL)
 * @param[out]	stream_ducking	The handle of stream ducking
 * @return @c 0 on success,
 *         otherwise a negative error value
 * @retval #SOUND_MANAGER_ERROR_NONE Success
 * @retval #SOUND_MANAGER_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #SOUND_MANAGER_ERROR_INTERNAL Internal error inside the sound system
 * @see sound_manager_destroy_stream_ducking()
 * @see sound_manager_is_ducked()
 * @see sound_manager_activate_ducking()
 * @see sound_manager_deactivate_ducking()
 */
int sound_manager_create_stream_ducking(sound_stream_type_e target_stream, sound_stream_ducking_state_changed_cb callback, void *user_data, sound_stream_ducking_h *stream_ducking);

/**
 * @brief Checks if the stream is ducked.
 * @since_tizen 5.5
 *
 * @param[in]	stream_ducking	The handle of stream ducking
 * @param[out]	is_ducked		Whether the stream is ducked or not. @c true if ducked, @c false otherwise.
 * @return @c 0 on success,
 *         otherwise a negative error value
 * @retval #SOUND_MANAGER_ERROR_NONE Success
 * @retval #SOUND_MANAGER_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #SOUND_MANAGER_ERROR_INTERNAL Internal error inside the sound system
 * @retval #SOUND_MANAGER_ERROR_INVALID_STATE Invalid state
 * @see sound_manager_create_stream_ducking()
 * @see sound_manager_destroy_stream_ducking()
 * @see sound_manager_activate_ducking()
 * @see sound_manager_deactivate_ducking()
 */
int sound_manager_is_ducked(sound_stream_ducking_h stream_ducking, bool *is_ducked);

/**
 * @brief Activates ducking, asynchronously.
 * @since_tizen 5.5
 * @privlevel public
 * @privilege %http://tizen.org/privilege/volume.set
 * @remarks If ducking is activated successfully, the volume of all sound streams
 *	matched with the target_stream that set in sound_manager_create_stream_ducking()
 *	is decreased by @a ratio for @a duration.
 *	The change applies system-wide.
 * @param[in]	stream_ducking	The handle of stream ducking
 * @param[in]	duration		The duration for ducking (msec, 0 <= duration <= 3000)
 * @param[in]	ratio			The volume ratio when ducked (0.0 <= ratio < 1.0)
 * @return @c 0 on success,
 *         otherwise a negative error value
 * @retval #SOUND_MANAGER_ERROR_NONE Success
 * @retval #SOUND_MANAGER_ERROR_PERMISSION_DENIED Permission denied
 * @retval #SOUND_MANAGER_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #SOUND_MANAGER_ERROR_INVALID_STATE Invalid state
 * @retval #SOUND_MANAGER_ERROR_INTERNAL Internal error inside the sound system
 * @pre The stream should be unducked before calling this function,
 *      otherwise, #SOUND_MANAGER_ERROR_INVALID_STATE is returned.
 * @post sound_stream_ducking_state_changed_cb() will be invoked.
 * @see sound_manager_create_stream_ducking()
 * @see sound_manager_destroy_stream_ducking()
 * @see sound_manager_is_ducked()
 * @see sound_manager_deactivate_ducking()
 */
int sound_manager_activate_ducking(sound_stream_ducking_h stream_ducking, unsigned int duration, double ratio);

/**
 * @brief Deactivates ducking, asynchronously.
 * @since_tizen 5.5
 * @privlevel public
 * @privilege %http://tizen.org/privilege/volume.set
 * @param[in]	stream_ducking	The handle of stream ducking
 * @return @c 0 on success,
 *         otherwise a negative error value
 * @retval #SOUND_MANAGER_ERROR_NONE Success
 * @retval #SOUND_MANAGER_ERROR_PERMISSION_DENIED Permission denied
 * @retval #SOUND_MANAGER_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #SOUND_MANAGER_ERROR_INVALID_STATE Invalid state
 * @retval #SOUND_MANAGER_ERROR_INTERNAL Internal error inside the sound system
 * @pre The stream should be ducked before calling this function,
 *      otherwise, #SOUND_MANAGER_ERROR_INVALID_STATE is returned.
 * @post sound_stream_ducking_state_changed_cb() will be invoked.
 * @see sound_manager_create_stream_ducking()
 * @see sound_manager_destroy_stream_ducking()
 * @see sound_manager_is_ducked()
 * @see sound_manager_activate_ducking()
 */
int sound_manager_deactivate_ducking(sound_stream_ducking_h stream_ducking);

/**
 * @brief Destroys the handle for stream ducking.
 * @since_tizen 5.5
 *
 * @param[in]	stream_ducking	The handle of stream ducking
 * @return @c 0 on success,
 *         otherwise a negative error value
 * @retval #SOUND_MANAGER_ERROR_NONE Success
 * @retval #SOUND_MANAGER_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #SOUND_MANAGER_ERROR_INTERNAL Internal error inside the sound system
 * @retval #SOUND_MANAGER_ERROR_INVALID_STATE Invalid state
 * @pre The stream should be unducked before calling this function,
 *      otherwise, #SOUND_MANAGER_ERROR_INVALID_STATE is returned.
 * @see sound_manager_create_stream_ducking()
 * @see sound_manager_is_ducked()
 * @see sound_manager_activate_ducking()
 * @see sound_manager_deactivate_ducking()
 */
int sound_manager_destroy_stream_ducking(sound_stream_ducking_h stream_ducking);

/**
 * @}
 */

/**
 * @addtogroup CAPI_MEDIA_SOUND_MANAGER_STREAM_POLICY_MODULE
 * @{
 */

/**
 * @brief Creates a handle for stream information.
 * @since_tizen 3.0
 *
 * @remarks	The registered callback is issued in the internal thread of the sound manager.\n
 *	Do not call this function within sound_stream_focus_state_changed_cb() or sound_stream_focus_state_watch_cb(),\n
 *	otherwise #SOUND_MANAGER_ERROR_INVALID_OPERATION will be returned.\n
 *	To apply the stream policy according to this stream information, this handle should be passed to other APIs\n
 *	related to playback or recording.(e.g. player, wav-player, audio-io, etc.)
 *
 * @param[in]	stream_type	The type of stream
 * @param[in]	callback	The focus state change callback function (optional, this can be NULL)
 * @param[in]	user_data	The user data to be passed to the callback function (optional, this can be NULL)
 * @param[out]	stream_info	The handle of stream information
 * @return @c 0 on success,
 *         otherwise a negative error value
 * @retval #SOUND_MANAGER_ERROR_NONE Success
 * @retval #SOUND_MANAGER_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #SOUND_MANAGER_ERROR_INVALID_OPERATION Invalid operation
 * @retval #SOUND_MANAGER_ERROR_NOT_SUPPORTED Not supported
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
int sound_manager_create_stream_information(sound_stream_type_e stream_type, sound_stream_focus_state_changed_cb callback, void *user_data, sound_stream_info_h *stream_info);

/**
 * @brief Destroys the handle for stream information.
 * @since_tizen 3.0
 *
 * @remarks	Do not call this function within sound_stream_focus_state_changed_cb() or sound_stream_focus_state_watch_cb(),\n
 *	otherwise #SOUND_MANAGER_ERROR_INVALID_OPERATION will be returned.
 *
 * @param[in]	stream_info	The handle of stream information
 * @return @c 0 on success,
 *         otherwise a negative error value
 * @retval #SOUND_MANAGER_ERROR_NONE Success
 * @retval #SOUND_MANAGER_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #SOUND_MANAGER_ERROR_INVALID_OPERATION Invalid operation
 * @retval #SOUND_MANAGER_ERROR_INTERNAL Internal error inside the sound system
 * @see sound_manager_create_stream_information()
 * @see sound_manager_add_device_for_stream_routing()
 * @see sound_manager_remove_device_for_stream_routing()
 * @see sound_manager_remove_all_devices_for_stream_routing()
 * @see sound_manager_apply_stream_routing()
 * @see sound_manager_acquire_focus()
 * @see sound_manager_release_focus()
 * @see sound_manager_get_focus_state()
 */
int sound_manager_destroy_stream_information(sound_stream_info_h stream_info);

/**
 * @brief Gets the sound type of the stream information.
 * @since_tizen 3.0
 *
 * @remarks	In case of a @a stream_info made with #SOUND_STREAM_TYPE_EMERGENCY, it'll return #SOUND_MANAGER_ERROR_NO_DATA.\n
 *	The reason is that there is no #sound_type_e matched with this @a stream_info.
 *
 * @param[in]	stream_info	The handle of stream information
 * @param[out]	sound_type	The sound type
 * @return @c 0 on success,
 *         otherwise a negative error value
 * @retval #SOUND_MANAGER_ERROR_NONE Success
 * @retval #SOUND_MANAGER_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #SOUND_MANAGER_ERROR_NO_DATA No data
 * @pre Call sound_manager_create_stream_information() before calling this function.
 * @see sound_manager_create_stream_information()
 * @see sound_manager_set_volume()
 * @see sound_manager_get_max_volume()
 * @see sound_manager_get_volume()
 */
int sound_manager_get_sound_type(sound_stream_info_h stream_info, sound_type_e *sound_type);

/**
 * @brief Adds the device to the stream information for the stream routing.
 * @since_tizen 3.0
 *
 * @remarks	Use sound_manager_get_device_list() and sound_manager_get_next_device() to get the device.\n
 *	#SOUND_MANAGER_ERROR_POLICY could be returned according to the stream type of the @a stream_info.\n
 *	The available types of the @a stream_info for this function are #SOUND_STREAM_TYPE_VOIP and #SOUND_STREAM_TYPE_MEDIA_EXTERNAL_ONLY.
 *
 * @param[in]	stream_info	The handle of stream information
 * @param[in]	device		The device item from #sound_device_list_h
 * @return @c 0 on success,
 *         otherwise a negative error value
 * @retval #SOUND_MANAGER_ERROR_NONE Success
 * @retval #SOUND_MANAGER_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #SOUND_MANAGER_ERROR_POLICY Noncompliance with the sound system policy
 * @pre Call sound_manager_create_stream_information() before calling this function.
 * @post You can apply this setting by calling sound_manager_apply_stream_routing().
 * @see sound_manager_create_stream_information()
 * @see sound_manager_destroy_stream_information()
 * @see sound_manager_remove_device_for_stream_routing()
 * @see sound_manager_remove_all_devices_for_stream_routing()
 * @see sound_manager_apply_stream_routing()
 */
int sound_manager_add_device_for_stream_routing(sound_stream_info_h stream_info, sound_device_h device);

/**
 * @brief Removes the device from the stream information for the stream routing.
 * @since_tizen 3.0
 *
 * @remarks	Use sound_manager_get_device_list() and sound_manager_get_next_device() to get the device.\n
 *
 * @param[in]	stream_info	The handle of stream information
 * @param[in]	device		The device item from #sound_device_list_h
 * @return @c 0 on success,
 *         otherwise a negative error value
 * @retval #SOUND_MANAGER_ERROR_NONE Success
 * @retval #SOUND_MANAGER_ERROR_INVALID_PARAMETER Invalid parameter
 * @pre Call sound_manager_create_stream_information() and sound_manager_add_device_for_stream_routing() before calling this function.
 * @post You can apply this setting by calling sound_manager_apply_stream_routing().
 * @see sound_manager_create_stream_information()
 * @see sound_manager_destroy_stream_information()
 * @see sound_manager_add_device_for_stream_routing()
 * @see sound_manager_remove_all_devices_for_stream_routing()
 * @see sound_manager_apply_stream_routing()
 */
int sound_manager_remove_device_for_stream_routing(sound_stream_info_h stream_info, sound_device_h device);

/**
 * @brief Removes all devices from the stream information for the stream routing.
 * @since_tizen 4.0
 *
 * @param[in]	stream_info	The handle of stream information
 * @return @c 0 on success,
 *         otherwise a negative error value
 * @retval #SOUND_MANAGER_ERROR_NONE Success
 * @retval #SOUND_MANAGER_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #SOUND_MANAGER_ERROR_POLICY Noncompliance with the sound system policy
 * @pre Call sound_manager_create_stream_information() before calling this function.
 * @see sound_manager_create_stream_information()
 * @see sound_manager_destroy_stream_information()
 * @see sound_manager_add_device_for_stream_routing()
 * @see sound_manager_remove_device_for_stream_routing()
 * @see sound_manager_apply_stream_routing()
 */
int sound_manager_remove_all_devices_for_stream_routing(sound_stream_info_h stream_info);

/**
 * @brief Applies the stream routing.
 * @since_tizen 3.0
 *
 * @remarks	If the stream has not been made yet, this setting will be applied when the stream starts to play.\n
 *
 * @param[in]	stream_info	The handle of stream information
 * @return @c 0 on success,
 *         otherwise a negative error value
 * @retval #SOUND_MANAGER_ERROR_NONE Success
 * @retval #SOUND_MANAGER_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #SOUND_MANAGER_ERROR_INVALID_STATE Invalid state
 * @retval #SOUND_MANAGER_ERROR_INTERNAL Internal error inside the sound system
 * @pre Call sound_manager_create_stream_information() and sound_manager_add_device_for_stream_routing() before calling this function.
 * @see sound_manager_create_stream_information()
 * @see sound_manager_destroy_stream_information()
 * @see sound_manager_add_device_for_stream_routing()
 * @see sound_manager_remove_device_for_stream_routing()
 * @see sound_manager_remove_all_devices_for_stream_routing()
 */
int sound_manager_apply_stream_routing(sound_stream_info_h stream_info);

/**
 * @deprecated Deprecated since 11.0.
 * @brief Sets the preferred built-in device for the stream routing.
 * @since_tizen 5.5
 *
 * @remarks	This function is to set a specific built-in device when the system has multiple devices of the same built-in device type.
 *	When there's only one device for a built-in device type in the system, nothing will happen even if this function succeeds in operation.
 *	If @a device is not supported by @a stream_info, #SOUND_MANAGER_ERROR_POLICY will be returned.
 *	After this function is called, @a device is not needed for the function's work. It's okay to destroy it by destroying the list from which it was obtained.
 *
 * @param[in]	stream_info	The handle of stream information
 * @param[in]	io_direction	The IO direction of the device
 * @param[in]	device	The preferred device (this can be NULL to unset)
 * @return @c 0 on success,
 *         otherwise a negative error value
 * @retval #SOUND_MANAGER_ERROR_NONE Success
 * @retval #SOUND_MANAGER_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #SOUND_MANAGER_ERROR_POLICY Noncompliance with the sound system policy
 * @retval #SOUND_MANAGER_ERROR_INTERNAL Internal error inside the sound system
 * @pre Call sound_manager_create_stream_information() before calling this function.
 * @pre Get a device by calling sound_manager_get_device_list() and sound_manager_get_next_device().
 * @post Call sound_manager_free_device_list() to free the devices.
 * @see sound_manager_create_stream_information()
 * @see sound_manager_destroy_stream_information()
 * @see sound_manager_get_device_list()
 * @see sound_manager_get_next_device()
 * @see sound_manager_free_device_list()
 */
int sound_manager_set_stream_preferred_device(sound_stream_info_h stream_info, sound_device_io_direction_e io_direction, sound_device_h device) TIZEN_DEPRECATED_API;

/**
 * @deprecated Deprecated since 11.0.
 * @brief Gets the preferred device id of the stream info handle.
 * @since_tizen 5.5
 *
 * @remarks	This function is to return the preferred device id per each direction if it has been set.
 *	If there is no preferred device for a given direction, the corresponding output value will be set to
 *	#SOUND_MANAGER_STREAM_NO_PREFERRED_DEVICE.
 *	You can get a device handle of the id value by using sound_manager_get_device_list(),
 *	sound_manager_get_next_device() and sound_manager_get_device_id().
 *
 * @param[in]	stream_info	The handle of stream information
 * @param[out]	in_device_id	The preferred input device id
 * @param[out]	out_device_id	The preferred output device id
 * @return @c 0 on success,
 *         otherwise a negative error value
 * @retval #SOUND_MANAGER_ERROR_NONE Success
 * @retval #SOUND_MANAGER_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #SOUND_MANAGER_ERROR_INTERNAL Internal error inside the sound system
 * @see sound_manager_get_device_list()
 * @see sound_manager_get_next_device()
 * @see sound_manager_get_device_id()
 * @see sound_manager_free_device_list()
 */
int sound_manager_get_stream_preferred_device(sound_stream_info_h stream_info, int *in_device_id, int *out_device_id) TIZEN_DEPRECATED_API;

/**
 * @brief Acquires the stream focus.
 * @since_tizen 3.0
 *
 * @remarks	Do not call this function within sound_stream_focus_state_changed_cb() or sound_stream_focus_state_watch_cb(),\n
 *	otherwise #SOUND_MANAGER_ERROR_INVALID_OPERATION will be returned. Since 4.0, it is allowed and does not return the error.
 *
 * @param[in]	stream_info	The handle of stream information
 * @param[in]	focus_mask	The focus mask that user wants to acquire
 * @param[in]	sound_behavior	The requesting sound behavior, values of #sound_behavior_e combined with bitwise 'or'
 * @param[in]	extra_info	The extra information for this request (optional, this can be NULL)
 * @return @c 0 on success,
 *         otherwise a negative error value
 * @retval #SOUND_MANAGER_ERROR_NONE Success
 * @retval #SOUND_MANAGER_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #SOUND_MANAGER_ERROR_INVALID_OPERATION Invalid operation
 * @retval #SOUND_MANAGER_ERROR_INVALID_STATE Invalid state
 * @retval #SOUND_MANAGER_ERROR_POLICY Noncompliance with the sound system policy
 * @retval #SOUND_MANAGER_ERROR_INTERNAL Internal error inside the sound system
 * @pre Call sound_manager_create_stream_information() with its parameter for sound_stream_focus_state_changed_cb()\n
 *	before calling this function.
 * @see sound_manager_create_stream_information()
 * @see sound_manager_destroy_stream_information()
 * @see sound_manager_release_focus()
 * @see sound_manager_get_focus_state()
 */
int sound_manager_acquire_focus(sound_stream_info_h stream_info, sound_stream_focus_mask_e focus_mask, int sound_behavior, const char *extra_info);

/**
 * @brief Releases the acquired stream focus.
 * @since_tizen 3.0
 *
 * @remarks	Do not call this function within sound_stream_focus_state_changed_cb() or sound_stream_focus_state_watch_cb(),\n
 *	otherwise #SOUND_MANAGER_ERROR_INVALID_OPERATION will be returned. Since 4.0, it is allowed and does not return the error.
 *
 * @param[in]	stream_info	The handle of stream information
 * @param[in]	focus_mask	The focus mask that user wants to release
 * @param[in]	sound_behavior	The requesting sound behavior, values of #sound_behavior_e combined with bitwise 'or'
 * @param[in]	extra_info	The extra information for this request (optional, this can be NULL)
 * @return @c 0 on success,
 *         otherwise a negative error value
 * @retval #SOUND_MANAGER_ERROR_NONE Success
 * @retval #SOUND_MANAGER_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #SOUND_MANAGER_ERROR_INVALID_OPERATION Invalid operation
 * @retval #SOUND_MANAGER_ERROR_INVALID_STATE Invalid state
 * @retval #SOUND_MANAGER_ERROR_INTERNAL Internal error inside the sound system
 * @pre Call sound_manager_create_stream_information() and sound_manager_acquire_focus() before calling this function.
 * @see sound_manager_create_stream_information()
 * @see sound_manager_destroy_stream_information()
 * @see sound_manager_acquire_focus()
 * @see sound_manager_get_focus_state()
 */
int sound_manager_release_focus(sound_stream_info_h stream_info, sound_stream_focus_mask_e focus_mask, int sound_behavior, const char *extra_info);

/**
 * @deprecated Deprecated since 11.0.
 * @brief Acquires all the stream focuses.
 * @since_tizen 4.0
 *
 * @remarks	This function does not return error even though one or all of focuses have already been acquired.\n
 *	Use sound_manager_get_focus_state() to get acquired focus state if it returns #SOUND_MANAGER_ERROR_POLICY.
 *
 * @param[in]	stream_info	The handle of stream information
 * @param[in]	sound_behavior	The requesting sound behavior, values of #sound_behavior_e combined with bitwise 'or'
 * @param[in]	extra_info	The extra information for this request (optional, this can be NULL)
 * @return @c 0 on success,
 *         otherwise a negative error value
 * @retval #SOUND_MANAGER_ERROR_NONE Success
 * @retval #SOUND_MANAGER_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #SOUND_MANAGER_ERROR_POLICY Noncompliance with the sound system policy
 * @retval #SOUND_MANAGER_ERROR_INTERNAL Internal error inside the sound system
 * @pre Call sound_manager_create_stream_information() with its parameter for sound_stream_focus_state_changed_cb()\n
 *	before calling this function.
 * @see sound_manager_create_stream_information()
 * @see sound_manager_destroy_stream_information()
 * @see sound_manager_acquire_focus()
 * @see sound_manager_release_focus()
 * @see sound_manager_get_focus_state()
 */
int sound_manager_acquire_focus_all(sound_stream_info_h stream_info, int sound_behavior, const char *extra_info) TIZEN_DEPRECATED_API;

/**
 * @deprecated Deprecated since 11.0.
 * @brief Releases all the acquired stream focuses.
 * @since_tizen 4.0
 *
 * @remarks	This function does not return error even though one or all of focuses have already been released.
 *
 * @param[in]	stream_info	The handle of stream information
 * @param[in]	sound_behavior	The requesting sound behavior, values of #sound_behavior_e combined with bitwise 'or'
 * @param[in]	extra_info	The extra information for this request (optional, this can be NULL)
 * @return @c 0 on success,
 *         otherwise a negative error value
 * @retval #SOUND_MANAGER_ERROR_NONE Success
 * @retval #SOUND_MANAGER_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #SOUND_MANAGER_ERROR_POLICY Noncompliance with the sound system policy
 * @retval #SOUND_MANAGER_ERROR_INTERNAL Internal error inside the sound system
 * @pre Call sound_manager_create_stream_information(), sound_manager_acquire_focus()\n
 *	before calling this function.
 * @see sound_manager_create_stream_information()
 * @see sound_manager_destroy_stream_information()
 * @see sound_manager_acquire_focus()
 * @see sound_manager_release_focus()
 * @see sound_manager_get_focus_state()
 */
int sound_manager_release_focus_all(sound_stream_info_h stream_info, int sound_behavior, const char *extra_info) TIZEN_DEPRECATED_API;

/**
 * @brief Gets the state of focus.
 * @since_tizen 3.0
 * @param[in]	stream_info	The handle of stream information
 * @param[out]	state_for_playback	The state of playback focus
 * @param[out]	state_for_recording	The state of recording focus
 *
 * @return @c 0 on success,
 *         otherwise a negative error value
 * @retval #SOUND_MANAGER_ERROR_NONE Success
 * @retval #SOUND_MANAGER_ERROR_INVALID_PARAMETER Invalid parameter
 * @pre Call sound_manager_create_stream_information() before calling this function.
 * @see sound_manager_create_stream_information()
 * @see sound_manager_destroy_stream_information()
 * @see sound_manager_acquire_focus()
 * @see sound_manager_release_focus()
 */
int sound_manager_get_focus_state(sound_stream_info_h stream_info, sound_stream_focus_state_e *state_for_playback, sound_stream_focus_state_e *state_for_recording);

/**
 * @brief Sets auto focus reacquisition property.
 * @since_tizen 3.0
 *
 * @remarks	The focus reacquisition is set as default.
 *	If you don't want to reacquire the focus you've lost automatically, disable the focus reacquisition setting by using this function and vice versa.
 *
 * @param[in]	stream_info	The handle of stream information
 * @param[in]	enable	The auto focus reacquisition property value to set: (@c true = enable, @c false = disable)
 * @return @c 0 on success,
 *         otherwise a negative error value
 * @retval #SOUND_MANAGER_ERROR_NONE Success
 * @retval #SOUND_MANAGER_ERROR_INVALID_PARAMETER Invalid parameter
 * @pre Call sound_manager_create_stream_information() before calling this function.
 * @see sound_manager_create_stream_information()
 */
int sound_manager_set_focus_reacquisition(sound_stream_info_h stream_info, bool enable);

/**
 * @brief Gets auto focus reacquisition property.
 * @since_tizen 3.0
 *
 * @remarks	The default value is true.
 *
 * @param[in]	stream_info	The handle of stream information
 * @param[out]	enabled	The value of focus auto reacquisition property: (@c true = enabled, @c false = disabled)
 * @return @c 0 on success,
 *         otherwise a negative error value
 * @retval #SOUND_MANAGER_ERROR_NONE Success
 * @retval #SOUND_MANAGER_ERROR_INVALID_PARAMETER Invalid parameter
 * @pre Call sound_manager_create_stream_information() before calling this function.
 * @see sound_manager_create_stream_information()
 */
int sound_manager_get_focus_reacquisition(sound_stream_info_h stream_info, bool *enabled);

/**
 * @deprecated Deprecated since 11.0.
 * @brief Delivers focuses to another stream information.
 * @since_tizen 4.0
 *
 * @remarks	This function does not affect any invocation of sound_stream_focus_state_changed_cb() or\n
 *	sound_stream_focus_state_watch_cb(). Do not call this function within sound_stream_focus_state_changed_cb() or\n
 *	sound_stream_focus_state_watch_cb(), otherwise #SOUND_MANAGER_ERROR_INVALID_OPERATION will be returned.
 *
 * @param[in]	source	The source handle of stream information which has focuses
 * @param[in]	destination	The destination handle of stream information which will receive focuses
 * @param[in]	focus_mask	The focus mask to deliver
 * @return @c 0 on success,
 *         otherwise a negative error value
 * @retval #SOUND_MANAGER_ERROR_NONE Success
 * @retval #SOUND_MANAGER_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #SOUND_MANAGER_ERROR_INVALID_OPERATION Invalid operation
 * @retval #SOUND_MANAGER_ERROR_INVALID_STATE Invalid state
 * @retval #SOUND_MANAGER_ERROR_POLICY Noncompliance with the sound system policy
 * @retval #SOUND_MANAGER_ERROR_INTERNAL Internal error inside the sound system
 * @pre Call sound_manager_create_stream_information(), sound_manager_acquire_focus()\n
 *	before calling this function.
 * @see sound_manager_create_stream_information()
 * @see sound_manager_acquire_focus()
 */
int sound_manager_deliver_focus(sound_stream_info_h source, sound_stream_info_h destination, sound_stream_focus_mask_e focus_mask) TIZEN_DEPRECATED_API;

/**
 * @deprecated Deprecated since 11.0.
 * @brief Checks if the stream information is using the device.
 * @since_tizen 3.0
 *
 * @param[in]	stream_info	The handle of stream information
 * @param[in]	device	The device item
 * @param[out]	is_on	Whether the stream info is using the device or not: (@c true = use, @c false = not use)
 * @return @c 0 on success,
 *         otherwise a negative error value
 * @retval #SOUND_MANAGER_ERROR_NONE Success
 * @retval #SOUND_MANAGER_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #SOUND_MANAGER_ERROR_INTERNAL Internal error inside the sound system
 * @see sound_manager_get_device_list()
 * @see sound_manager_get_next_device()
 * @see sound_manager_get_prev_device()
 * @see sound_manager_get_device_type()
 * @see sound_manager_get_device_io_direction()
 * @see sound_manager_get_device_id()
 * @see sound_manager_get_device_name()
 * @see sound_manager_free_device_list()
 */
int sound_manager_is_stream_on_device(sound_stream_info_h stream_info, sound_device_h device, bool *is_on) TIZEN_DEPRECATED_API;

/**
 * @deprecated Deprecated since 11.0.
 * @brief Gets the current device type for media playback stream.
 * @since_tizen 3.0
 *
 * @param[out]	device_type	The output device type that a media playback stream can go out
 * @return @c 0 on success,
 *         otherwise a negative error value
 * @retval #SOUND_MANAGER_ERROR_NONE Success
 * @retval #SOUND_MANAGER_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #SOUND_MANAGER_ERROR_NO_DATA No data
 * @see sound_manager_get_device_type()
 * @see sound_manager_get_device_io_direction()
 * @see sound_manager_get_device_id()
 * @see sound_manager_get_device_name()
 */
int sound_manager_get_current_media_playback_device_type(sound_device_type_e *device_type) TIZEN_DEPRECATED_API;

/**
 * @brief Gets the reason for the current acquired playback focus.
 * @since_tizen 3.0
 *
 * @remarks	If there is no acquired playback focus in this system, it'll return #SOUND_MANAGER_ERROR_NO_DATA.\n
 *	Do not call this function within sound_stream_focus_state_changed_cb() or sound_stream_focus_state_watch_cb(),\n
 *	otherwise #SOUND_MANAGER_ERROR_INVALID_OPERATION will be returned.\n
 *	The @a extra_info should be released using free().
 *
 * @param[out]	acquired_by	The reason for the current acquired playback focus
 * @param[out]	sound_behavior	The requested sound behavior that should be followed, values of #sound_behavior_e combined with bitwise 'or'
 * @param[out]	extra_info The extra information of the acquired playback focus (optional, this can be NULL)
 * @return @c 0 on success,
 *         otherwise a negative error value
 * @retval #SOUND_MANAGER_ERROR_NONE Success
 * @retval #SOUND_MANAGER_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #SOUND_MANAGER_ERROR_INVALID_OPERATION Invalid operation
 * @retval #SOUND_MANAGER_ERROR_NO_DATA No data
 * @see sound_manager_get_current_recording_focus()
 */
int sound_manager_get_current_playback_focus(sound_stream_focus_change_reason_e *acquired_by, int *sound_behavior, char **extra_info);

/**
 * @brief Gets the reason for the current acquired recording focus.
 * @since_tizen 3.0
 *
 * @remarks	If there is no acquired recording focus in this system, it'll return #SOUND_MANAGER_ERROR_NO_DATA.\n
 *	Do not call this function within sound_stream_focus_state_changed_cb() or sound_stream_focus_state_watch_cb(),\n
 *	otherwise #SOUND_MANAGER_ERROR_INVALID_OPERATION will be returned.\n
 *	The @a extra_info should be released using free().
 *
 * @param[out]	acquired_by	The reason for the current acquired recording focus
 * @param[out]	sound_behavior	The requested sound behavior that should be followed, values of #sound_behavior_e combined with bitwise 'or'
 * @param[out]	extra_info The extra information of the acquired recording focus (optional, this can be NULL)
 * @return @c 0 on success,
 *         otherwise a negative error value
 * @retval #SOUND_MANAGER_ERROR_NONE Success
 * @retval #SOUND_MANAGER_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #SOUND_MANAGER_ERROR_INVALID_OPERATION Invalid operation
 * @retval #SOUND_MANAGER_ERROR_NO_DATA No data
 * @see sound_manager_get_current_playback_focus()
 */
int sound_manager_get_current_recording_focus(sound_stream_focus_change_reason_e *acquired_by, int *sound_behavior, char **extra_info);

/**
 * @brief Registers the watch callback function to be invoked when the focus state for each sound stream type is changed regardless of the process.
 * @since_tizen 3.0
 *
 * @remarks	The registered callback is invoked by the internal thread of the sound manager.\n
 *	Do not call this function within sound_stream_focus_state_changed_cb() or sound_stream_focus_state_watch_cb(),\n
 *	otherwise #SOUND_MANAGER_ERROR_INVALID_OPERATION will be returned.
 *
 * @param[in]	focus_mask	The focus mask that user wants to watch
 * @param[in]	callback	The focus state change watch callback function
 * @param[in]	user_data	The user data to be passed to the callback function
 * @param[out]	id		The focus state change watch callback id
 * @return @c 0 on success,
 *         otherwise a negative error value
 * @retval #SOUND_MANAGER_ERROR_NONE Success
 * @retval #SOUND_MANAGER_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #SOUND_MANAGER_ERROR_INVALID_OPERATION Invalid operation
 * @retval #SOUND_MANAGER_ERROR_INTERNAL Internal error inside the sound system
 * @see sound_manager_remove_focus_state_watch_cb()
 */
int sound_manager_add_focus_state_watch_cb(sound_stream_focus_mask_e focus_mask, sound_stream_focus_state_watch_cb callback, void *user_data, int *id);

/**
 * @brief Unregisters the focus state change watch callback.
 * @since_tizen 3.0
 *
 * @remarks	Do not call this function within sound_stream_focus_state_changed_cb() or sound_stream_focus_state_watch_cb(),\n
 *	otherwise #SOUND_MANAGER_ERROR_INVALID_OPERATION will be returned.
 *
 * @param[in]	id		The focus state change watch callback id
 * @return @c 0 on success,
 *         otherwise a negative error value
 * @retval #SOUND_MANAGER_ERROR_NONE Success
 * @retval #SOUND_MANAGER_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #SOUND_MANAGER_ERROR_INVALID_OPERATION Invalid operation
 * @retval #SOUND_MANAGER_ERROR_INTERNAL Internal error inside the sound system
 * @see sound_manager_add_focus_state_watch_cb()
 */
int sound_manager_remove_focus_state_watch_cb(int id);

/**
 * @}
 */

/**
 * @addtogroup CAPI_MEDIA_SOUND_MANAGER_DEVICE_MODULE
 * @{
 */

/**
 * @brief Gets the list consisting of connected devices.
 * @since_tizen 3.0
 *
 * @remarks	@a device_list should be freed using sound_manager_free_device_list().\n
 *	Use sound_manager_get_next_device() to get the first node of the list.
 *
 * @param[in]	device_mask	The mask values of #sound_device_mask_e combined with bitwise 'or'
 * @param[out]	device_list	The list of connected devices
 * @return @c 0 on success,
 *         otherwise a negative error value
 * @retval #SOUND_MANAGER_ERROR_NONE Success
 * @retval #SOUND_MANAGER_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #SOUND_MANAGER_ERROR_NO_DATA No data
 * @retval #SOUND_MANAGER_ERROR_INTERNAL Internal error inside the sound system
 * @see sound_manager_get_next_device()
 * @see sound_manager_get_prev_device()
 * @see sound_manager_get_device_type()
 * @see sound_manager_get_device_io_direction()
 * @see sound_manager_get_device_id()
 * @see sound_manager_get_device_name()
 * @see sound_manager_is_device_running()
 * @see sound_manager_free_device_list()
 */
int sound_manager_get_device_list(int device_mask, sound_device_list_h *device_list);

/**
 * @brief Frees device list and each item of list.
 * @since_tizen 3.0
 *
 * @remarks	This function will deallocate not only device list itself but also each item.
 *
 * @param[in]	device_list	The device list got with sound_manager_get_device_list().
 * @return @c 0 on success,
 *         otherwise a negative error value
 * @retval #SOUND_MANAGER_ERROR_NONE Success
 * @retval #SOUND_MANAGER_ERROR_INVALID_PARAMETER Invalid parameter
 *
 *
 * @see sound_manager_get_device_list()
 * @see sound_manager_get_next_device()
 * @see sound_manager_get_prev_device()
 * @see sound_manager_get_device_type()
 * @see sound_manager_get_device_io_direction()
 * @see sound_manager_get_device_id()
 * @see sound_manager_get_device_name()
 * @see sound_manager_is_device_running()
 */
int sound_manager_free_device_list(sound_device_list_h device_list);

/**
 * @brief Gets the next item of the device list.
 * @since_tizen 2.3
 * @param[in]	device_list	The list of connected devices
 * @param[out]	device	The device item
 * @return @c 0 on success,
 *         otherwise a negative error value
 * @retval #SOUND_MANAGER_ERROR_NONE Success
 * @retval #SOUND_MANAGER_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #SOUND_MANAGER_ERROR_NO_DATA No data
 * @see sound_manager_get_device_list()
 * @see sound_manager_get_prev_device()
 * @see sound_manager_get_device_type()
 * @see sound_manager_get_device_io_direction()
 * @see sound_manager_get_device_id()
 * @see sound_manager_get_device_name()
 * @see sound_manager_is_device_running()
 * @see sound_manager_free_device_list()
 */
int sound_manager_get_next_device(sound_device_list_h device_list, sound_device_h *device);

/**
 * @brief Gets the previous item of the device list.
 * @since_tizen 2.3
 * @param[in]	device_list	The list of connected devices
 * @param[out]	device	The device item
 * @return @c 0 on success,
 *         otherwise a negative error value
 * @retval #SOUND_MANAGER_ERROR_NONE Success
 * @retval #SOUND_MANAGER_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #SOUND_MANAGER_ERROR_NO_DATA No data
 * @see sound_manager_get_device_list()
 * @see sound_manager_get_next_device()
 * @see sound_manager_get_device_type()
 * @see sound_manager_get_device_io_direction()
 * @see sound_manager_get_device_id()
 * @see sound_manager_get_device_name()
 * @see sound_manager_is_device_running()
 * @see sound_manager_free_device_list()
 */
int sound_manager_get_prev_device(sound_device_list_h device_list, sound_device_h *device);

/**
 * @brief Gets the type of the device.
 * @since_tizen 2.3
 * @param[in]	device	The device item
 * @param[out]	type	The type of the device
 * @return @c 0 on success,
 *         otherwise a negative error value
 * @retval #SOUND_MANAGER_ERROR_NONE Success
 * @retval #SOUND_MANAGER_ERROR_INVALID_PARAMETER Invalid parameter
 * @see sound_manager_get_device_list()
 * @see sound_manager_get_next_device()
 * @see sound_manager_get_prev_device()
 * @see sound_manager_get_device_io_direction()
 * @see sound_manager_get_device_id()
 * @see sound_manager_get_device_name()
 * @see sound_manager_is_device_running()
 * @see sound_manager_free_device_list()
 */
int sound_manager_get_device_type(sound_device_h device, sound_device_type_e *type);

/**
 * @brief Gets the io direction of the device.
 * @since_tizen 2.3
 * @param[in]	device	The device item
 * @param[out]	io_direction	The io direction of the device
 * @return @c 0 on success,
 *         otherwise a negative error value
 * @retval #SOUND_MANAGER_ERROR_NONE Success
 * @retval #SOUND_MANAGER_ERROR_INVALID_PARAMETER Invalid parameter
 * @see sound_manager_get_device_list()
 * @see sound_manager_get_next_device()
 * @see sound_manager_get_prev_device()
 * @see sound_manager_get_device_type()
 * @see sound_manager_get_device_id()
 * @see sound_manager_get_device_name()
 * @see sound_manager_is_device_running()
 * @see sound_manager_free_device_list()
 */
int sound_manager_get_device_io_direction(sound_device_h device, sound_device_io_direction_e *io_direction);

/**
 * @brief Gets the id of the device.
 * @since_tizen 2.3
 * @param[in]	device	The device item
 * @param[out]	id	The id of the device
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
 * @see sound_manager_is_device_running()
 * @see sound_manager_free_device_list()
 */
int sound_manager_get_device_id(sound_device_h device, int *id);

/**
 * @brief Gets the name of the device.
 * @since_tizen 2.3
 * @param[in]	device	The device item
 * @param[out]	name	The name of the device
 * @return @c 0 on success,
 *         otherwise a negative error value
 * @retval #SOUND_MANAGER_ERROR_NONE Success
 * @retval #SOUND_MANAGER_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #SOUND_MANAGER_ERROR_INTERNAL Internal error inside the sound system
 * @see sound_manager_get_device_list()
 * @see sound_manager_get_next_device()
 * @see sound_manager_get_prev_device()
 * @see sound_manager_get_device_type()
 * @see sound_manager_get_device_io_direction()
 * @see sound_manager_get_device_id()
 * @see sound_manager_is_device_running()
 * @see sound_manager_free_device_list()
 */
int sound_manager_get_device_name(sound_device_h device, char **name);

/**
 * @brief Checks if the device is running.
 * @since_tizen 5.0
 * @param[in]	device	The device item
 * @param[out]	is_running	Whether the device is running or not: (@c true = running, @c false = not running)
 * @return @c 0 on success,
 *         otherwise a negative error value
 * @retval #SOUND_MANAGER_ERROR_NONE Success
 * @retval #SOUND_MANAGER_ERROR_INVALID_PARAMETER Invalid parameter
 * @see sound_manager_get_device_list()
 * @see sound_manager_get_next_device()
 * @see sound_manager_get_prev_device()
 * @see sound_manager_get_device_type()
 * @see sound_manager_get_device_io_direction()
 * @see sound_manager_get_device_id()
 * @see sound_manager_get_device_name()
 * @see sound_manager_free_device_list()
 */
int sound_manager_is_device_running(sound_device_h device, bool *is_running);

/**
 * @deprecated Deprecated since 11.0.
 * @brief Gets the device's supported sample formats.
 * @since_tizen 5.0
 *
 * @remarks	@a device should be #SOUND_DEVICE_USB_AUDIO type and an output device,\n
 *	otherwise #SOUND_MANAGER_ERROR_INVALID_OPERATION will be returned.\n
 *	Use sound_manager_get_device_io_direction() to check if it is an output device or not.\n
 *	The @a formats should be released using free().
 *
 * @param[in]	device	The device item
 * @param[out]	formats	The supported sample format list
 * @param[out]	num_of_elems	The number of elements in the sample format list
 * @return @c 0 on success,
 *         otherwise a negative error value
 * @retval #SOUND_MANAGER_ERROR_NONE Success
 * @retval #SOUND_MANAGER_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #SOUND_MANAGER_ERROR_INVALID_OPERATION Invalid operation
 * @retval #SOUND_MANAGER_ERROR_INTERNAL Internal error inside the sound system
 */
int sound_manager_get_supported_sample_formats(sound_device_h device, sound_sample_format_e **formats, unsigned int *num_of_elems) TIZEN_DEPRECATED_API;

/**
 * @deprecated Deprecated since 11.0.
 * @brief Sets the device's sample format.
 * @since_tizen 5.0
 *
 * @remarks	@a device should be #SOUND_DEVICE_USB_AUDIO type and an output device.\n
 *	@a format should be one of the values received from sound_manager_get_supported_sample_formats(),\n
 *	otherwise #SOUND_MANAGER_ERROR_INVALID_OPERATION will be returned.\n
 *	Use sound_manager_get_device_io_direction() to check if it is an output device or not.
 *
 * @param[in]	device	The device item
 * @param[in]	format	The sample format
 * @return @c 0 on success,
 *         otherwise a negative error value
 * @retval #SOUND_MANAGER_ERROR_NONE Success
 * @retval #SOUND_MANAGER_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #SOUND_MANAGER_ERROR_INVALID_OPERATION Invalid operation
 * @retval #SOUND_MANAGER_ERROR_INTERNAL Internal error inside the sound system
 * @see sound_manager_get_sample_format()
 */
int sound_manager_set_sample_format(sound_device_h device, sound_sample_format_e format) TIZEN_DEPRECATED_API;

/**
 * @deprecated Deprecated since 11.0.
 * @brief Gets the device's sample format.
 * @since_tizen 5.0
 *
 * @remarks	@a device should be #SOUND_DEVICE_USB_AUDIO type and an output device,\n
 *	otherwise #SOUND_MANAGER_ERROR_INVALID_OPERATION will be returned.\n
 *	Use sound_manager_get_device_io_direction() to check if it is an output device or not.
 *
 * @param[in]	device	The device item
 * @param[out]	format	The sample format
 * @return @c 0 on success,
 *         otherwise a negative error value
 * @retval #SOUND_MANAGER_ERROR_NONE Success
 * @retval #SOUND_MANAGER_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #SOUND_MANAGER_ERROR_INVALID_OPERATION Invalid operation
 * @retval #SOUND_MANAGER_ERROR_INTERNAL Internal error inside the sound system
 */
int sound_manager_get_sample_format(sound_device_h device, sound_sample_format_e *format) TIZEN_DEPRECATED_API;

/**
 * @deprecated Deprecated since 11.0.
 * @brief Gets the device's supported sample rates.
 * @since_tizen 5.0
 *
 * @remarks	@a device should be #SOUND_DEVICE_USB_AUDIO type and an output device,\n
 *	otherwise #SOUND_MANAGER_ERROR_INVALID_OPERATION will be returned.\n
 *	Use sound_manager_get_device_io_direction() to check if it is an output device or not.\n
 *	The @a rates should be released using free().
 *
 * @param[in]	device	The device item
 * @param[out]	rates	The supported sample rate list
 * @param[out]	num_of_elems	The number of elements in the sample rate list
 * @return @c 0 on success,
 *         otherwise a negative error value
 * @retval #SOUND_MANAGER_ERROR_NONE Success
 * @retval #SOUND_MANAGER_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #SOUND_MANAGER_ERROR_INVALID_OPERATION Invalid operation
 * @retval #SOUND_MANAGER_ERROR_INTERNAL Internal error inside the sound system
 */
int sound_manager_get_supported_sample_rates(sound_device_h device, sound_sample_rate_e **rates, unsigned int *num_of_elems) TIZEN_DEPRECATED_API;

/**
 * @deprecated Deprecated since 11.0.
 * @brief Sets the device's sample rate.
 * @since_tizen 5.0
 *
 * @remarks	@a device should be #SOUND_DEVICE_USB_AUDIO type and an output device.\n
 *	@a rate should be one of the values received from sound_manager_get_supported_sample_rates(),\n
 *	otherwise #SOUND_MANAGER_ERROR_INVALID_OPERATION will be returned.\n
 *	Use sound_manager_get_device_io_direction() to check if it is an output device or not.
 *
 * @param[in]	device	The device item
 * @param[in]	rate	The sample rate
 * @return @c 0 on success,
 *         otherwise a negative error value
 * @retval #SOUND_MANAGER_ERROR_NONE Success
 * @retval #SOUND_MANAGER_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #SOUND_MANAGER_ERROR_INVALID_OPERATION Invalid operation
 * @retval #SOUND_MANAGER_ERROR_INTERNAL Internal error inside the sound system
 */
int sound_manager_set_sample_rate(sound_device_h device, sound_sample_rate_e rate) TIZEN_DEPRECATED_API;

/**
 * @deprecated Deprecated since 11.0.
 * @brief Gets the device's sample rate.
 * @since_tizen 5.0
 *
 * @remarks	@a device should be #SOUND_DEVICE_USB_AUDIO type and an output device,\n
 *	otherwise #SOUND_MANAGER_ERROR_INVALID_OPERATION will be returned.\n
 *	Use sound_manager_get_device_io_direction() to check if it is an output device or not.
 *
 * @param[in]	device	The device item
 * @param[out]	rate	The sample rate
 * @return @c 0 on success,
 *         otherwise a negative error value
 * @retval #SOUND_MANAGER_ERROR_NONE Success
 * @retval #SOUND_MANAGER_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #SOUND_MANAGER_ERROR_INVALID_OPERATION Invalid operation
 * @retval #SOUND_MANAGER_ERROR_INTERNAL Internal error inside the sound system
 */
int sound_manager_get_sample_rate(sound_device_h device, sound_sample_rate_e *rate) TIZEN_DEPRECATED_API;

/**
 * @deprecated Deprecated since 11.0.
 * @brief Sets the device's 'avoid resampling' property.
 * @since_tizen 5.0
 *
 * @remarks	@a device should be #SOUND_DEVICE_USB_AUDIO type and an output device,\n
 *	otherwise #SOUND_MANAGER_ERROR_INVALID_OPERATION will be returned.\n
 *	Use sound_manager_get_device_io_direction() to check if it is an output device or not.\n
 *	The 'avoid resampling' property is not enabled as default. With this enabled, @a device will\n
 *	use the first stream's original sample format and rate without resampling if supported.
 *
 * @param[in]	device	The device item
 * @param[in]	enable	The 'avoid resampling' property value to set: (@c true = enable, @c false = disable)
 * @return @c 0 on success,
 *         otherwise a negative error value
 * @retval #SOUND_MANAGER_ERROR_NONE Success
 * @retval #SOUND_MANAGER_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #SOUND_MANAGER_ERROR_INVALID_OPERATION Invalid operation
 * @retval #SOUND_MANAGER_ERROR_INTERNAL Internal error inside the sound system
 */
int sound_manager_set_avoid_resampling(sound_device_h device, bool enable) TIZEN_DEPRECATED_API;

/**
 * @deprecated Deprecated since 11.0.
 * @brief Gets the device's 'avoid resampling' property.
 * @since_tizen 5.0
 *
 * @remarks	@a device should be #SOUND_DEVICE_USB_AUDIO type and an output device,\n
 *	otherwise #SOUND_MANAGER_ERROR_INVALID_OPERATION will be returned.\n
 *	Use sound_manager_get_device_io_direction() to check if it is an output device or not.\n
 *	The default value of 'avoid resampling' is false.
 *
 * @param[in]	device	The device item
 * @param[in]	enabled	The value of 'avoid resampling' property: (@c true = enabled, @c false = disabled)
 * @return @c 0 on success,
 *         otherwise a negative error value
 * @retval #SOUND_MANAGER_ERROR_NONE Success
 * @retval #SOUND_MANAGER_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #SOUND_MANAGER_ERROR_INVALID_OPERATION Invalid operation
 * @retval #SOUND_MANAGER_ERROR_INTERNAL Internal error inside the sound system
 */
int sound_manager_get_avoid_resampling(sound_device_h device, bool *enabled) TIZEN_DEPRECATED_API;

/**
 * @deprecated Deprecated since 11.0.
 * @brief Sets the restriction of stream type only for media.
 * @since_tizen 5.0
 *
 * @remarks	@a device should be #SOUND_DEVICE_USB_AUDIO type and an output device,\n
 *	otherwise #SOUND_MANAGER_ERROR_INVALID_OPERATION will be returned.\n
 *	Use sound_manager_get_device_io_direction() to check if it is an output device or not.\n
 *	This property is not enabled as default. With this enabled, no other stream type\n
 *	is allowed to @a device to play sound except for #SOUND_STREAM_TYPE_MEDIA.
 *
 * @param[in]	device	The device item
 * @param[in]	enable	The 'media stream only' property value to set: (@c true = enable, @c false = disable)
 * @return @c 0 on success,
 *         otherwise a negative error value
 * @retval #SOUND_MANAGER_ERROR_NONE Success
 * @retval #SOUND_MANAGER_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #SOUND_MANAGER_ERROR_INVALID_OPERATION Invalid operation
 * @retval #SOUND_MANAGER_ERROR_INTERNAL Internal error inside the sound system
 */
int sound_manager_set_media_stream_only(sound_device_h device, bool enable) TIZEN_DEPRECATED_API;

/**
 * @deprecated Deprecated since 11.0.
 * @brief Gets the restriction of stream type only for media.
 * @since_tizen 5.0
 *
 * @remarks	@a device should be #SOUND_DEVICE_USB_AUDIO type and an output device,\n
 *	otherwise #SOUND_MANAGER_ERROR_INVALID_OPERATION will be returned.\n
 *	Use sound_manager_get_device_io_direction() to check if it is an output device or not.\n
 *	The property is not enabled as default.
 *
 * @param[in]	device	The device item
 * @param[out]	enabled	The value of 'media stream only' property: (@c true = enabled, @c false = disabled)
 * @return @c 0 on success,
 *         otherwise a negative error value
 * @retval #SOUND_MANAGER_ERROR_NONE Success
 * @retval #SOUND_MANAGER_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #SOUND_MANAGER_ERROR_INVALID_OPERATION Invalid operation
 * @retval #SOUND_MANAGER_ERROR_INTERNAL Internal error inside the sound system
 */
int sound_manager_get_media_stream_only(sound_device_h device, bool *enabled) TIZEN_DEPRECATED_API;

/**
 * @brief Adds a callback function to be invoked when the connection state of a sound device was changed.
 * @since_tizen 3.0
 *
 * @param[in]	device_mask	Devices for which changes should be tracked, values of #sound_device_mask_e combined with bitwise 'or'
 * @param[in]	callback	The device connection state changed callback function
 * @param[in]	user_data	The user data to be passed to the callback function
 * @param[out]	id	The callback id
 * @return @c 0 on success,
 *         otherwise a negative error value
 * @retval #SOUND_MANAGER_ERROR_NONE Success
 * @retval #SOUND_MANAGER_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #SOUND_MANAGER_ERROR_INTERNAL Internal error inside the sound system
 * @post sound_device_connection_changed_cb() will be invoked.
 * @see sound_manager_remove_device_connection_changed_cb()
 * @see sound_device_connection_changed_cb()
 */
int sound_manager_add_device_connection_changed_cb(int device_mask, sound_device_connection_changed_cb callback, void *user_data, int *id);

/**
 * @brief Removes a callback function invoked when the connection of a sound device was changed.
 * @since_tizen 3.0
 *
 * @param[in]	id	The id of the callback to remove
 * @return @c 0 on success,
 *         otherwise a negative error value
 * @retval #SOUND_MANAGER_ERROR_NONE Success
 * @retval #SOUND_MANAGER_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #SOUND_MANAGER_ERROR_INTERNAL Internal error inside the sound system
 * @see sound_manager_add_device_connection_changed_cb()
 */
int sound_manager_remove_device_connection_changed_cb(int id);

/**
 * @brief Adds a callback function to be invoked when the state of a sound device was changed.
 * @since_tizen 5.0
 *
 * @param[in]	device_mask	Devices for which changes should be tracked, values of #sound_device_mask_e combined with bitwise 'or'
 * @param[in]	callback	The device state changed callback function
 * @param[in]	user_data	The user data to be passed to the callback function
 * @param[out]	id	The callback id
 * @return @c 0 on success,
 *         otherwise a negative error value
 * @retval #SOUND_MANAGER_ERROR_NONE Success
 * @retval #SOUND_MANAGER_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #SOUND_MANAGER_ERROR_INTERNAL Internal error inside the sound system
 * @post  sound_device_running_changed_cb() will be invoked.
 * @see sound_manager_remove_device_running_changed_cb()
 * @see sound_device_running_changed_cb()
 */
int sound_manager_add_device_running_changed_cb(int device_mask, sound_device_running_changed_cb callback, void *user_data, int *id);

/**
 * @brief Removes a callback function invoked when the state of a sound device was changed.
 * @since_tizen 5.0
 *
 * @param[in]	id	The id of the callback to remove
 * @return @c 0 on success,
 *         otherwise a negative error value
 * @retval #SOUND_MANAGER_ERROR_NONE Success
 * @retval #SOUND_MANAGER_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #SOUND_MANAGER_ERROR_INTERNAL Internal error inside the sound system
 * @see sound_manager_add_device_running_changed_cb()
 */
int sound_manager_remove_device_running_changed_cb(int id);

/**
 * @addtogroup CAPI_MEDIA_SOUND_MANAGER_STREAM_POLICY_MODULE
 * @{
 */

/**
 * @brief Sets sound effect methods that must need a reference device such as acoustic echo cancellation.
 * @since_tizen 8.0
 * @param[in]	stream_info	The handle of stream information
 * @param[in]	method	The method of #sound_effect_method_with_reference_e
 * @param[in]	device	The reference device
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
int sound_manager_set_effect_method_with_reference(sound_stream_info_h stream_info, sound_effect_method_with_reference_e method, sound_device_h device);

/**
 * @brief Gets sound effect methods that must need a reference device such as acoustic echo cancellation.
 * @since_tizen 8.0
 * @param[in]	stream_info	The handle of stream information
 * @param[out]	method	The method of #sound_effect_method_with_reference_e
 * @param[out]	device_id	The reference device id
 * @return @c 0 on success,
 *         otherwise a negative error value
 * @retval #SOUND_MANAGER_ERROR_NONE Success
 * @retval #SOUND_MANAGER_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #SOUND_MANAGER_ERROR_INVALID_OPERATION Invalid operation
 * @see sound_manager_create_stream_information()
 * @see sound_manager_set_effect_method_with_reference()
 */
int sound_manager_get_effect_method_with_reference(sound_stream_info_h stream_info, sound_effect_method_with_reference_e *method, int *device_id);

/**
 * @brief Sets sound effect methods.
 * @since_tizen 8.0
 * @param[in]	stream_info	The handle of stream information
 * @param[in]	method	The methods of #sound_effect_method_e, values of #sound_effect_method_e combined with bitwise 'or'
 * @return @c 0 on success,
 *         otherwise a negative error value
 * @retval #SOUND_MANAGER_ERROR_NONE Success
 * @retval #SOUND_MANAGER_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #SOUND_MANAGER_ERROR_POLICY Noncompliance with the sound system policy
 * @see sound_manager_create_stream_information()
 * @see sound_manager_set_effect_method_with_reference()
 */
int sound_manager_set_effect_method(sound_stream_info_h stream_info, int method);

/**
 * @brief Gets sound effect methods.
 * @since_tizen 8.0
 * @param[in]	stream_info	The handle of stream information
 * @param[out]	method	The methods of #sound_effect_method_e, values of #sound_effect_method_e combined with bitwise 'or'
 * @return @c 0 on success,
 *         otherwise a negative error value
 * @retval #SOUND_MANAGER_ERROR_NONE Success
 * @retval #SOUND_MANAGER_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #SOUND_MANAGER_ERROR_INVALID_OPERATION Invalid operation
 * @see sound_manager_create_stream_information()
 * @see sound_manager_set_effect_method()
 */
int sound_manager_get_effect_method(sound_stream_info_h stream_info, int *method);

/**
 * @}
 */

/**
 * @}
 */

/**
 * @}
 */

#ifdef __cplusplus
}
#endif

#endif /* __TIZEN_MEDIA_SOUND_MANAGER_H__ */
