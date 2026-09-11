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

#ifndef	__MM_SOUND_H__
#define	__MM_SOUND_H__

#include <mm_types.h>
#include <mm_error.h>
#include <mm_message.h>

#ifdef __cplusplus
	extern "C" {
#endif

/**
	@addtogroup SOUND
	@{
	@par
	This part is describes the sound module of multimedia framework. Sound
	module provides APIs to implement play wav file with simple api, to handle volume information,
	to handle audio route policy.

	@par
	There is six different volume type for normal usage. application should set proper volume type to multimedia playback APIs.
	<div> <table>
	<tr>
	<td><B>Type</B></td>
	<td><B>Description</B></td>
	</tr>
	<tr>
	<td>VOLUME_TYPE_SYSTEM</td>
	<td>volume for normal system sound (e.g. keysound, camera shutter)</td>
	</tr>
	<tr>
	<td>VOLUME_TYPE_NOTIFICATION</td>
	<td>volume for notification (e.g. message, email notification)</td>
	</tr>
	<tr>
	<td>VOLUME_TYPE_RINGTONE</td>
	<td>volume for incoming call ring</td>
	</tr>
	<tr>
	<td>VOLUME_TYPE_MEDIA</td>
	<td>volume for media playback (e.g. music, video playback)</td>
	</tr>
	<tr>
	<td>VOLUME_TYPE_CALL</td>
	<td>volume for call</td>
	</tr>
	</table> </div>

	@par
	application can change audio route policy with mm-sound API.
	Audio route is input and output of audio stream.

	@par
	@image html		audio_device.png	"Figure1. Audio Devices of mobile phone"	width=12cm
	@image latex	audio_device.png	"Figure1. Audio Devices of mobile phone"	width=12cm

	@par
	Default audio route policy is like follows
	@par
	for playback
	<div><table>
	<tr>
	<td><B>Bluetooth headset</B></td>
	<td><B>Wired headset</B></td>
	<td><B>Playback Device</B></td>
	</tr>
	<tr>
	<td>connected</td>
	<td>plugged</td>
	<td>Bluetooth headset</td>
	</tr>
	<tr>
	<td>connected</td>
	<td>unplugged</td>
	<td>Bluetooth headset</td>
	</tr>
	<tr>
	<td>disconnected</td>
	<td>plugged</td>
	<td>Wired headset</td>
	</tr>
	<tr>
	<td>disconnected</td>
	<td>unplugged</td>
	<td>Loud speaker</td>
	</tr>
	</table></div>

	@par
	for capture (bluetooth headset mic used only in call mode)
	<div><table>
	<tr>
	<td><B>Bluetooth headset mic</B></td>
	<td><B>Wired headset mic</B></td>
	<td><B>Capture Device</B></td>
	</tr>
	<tr>
	<td>connected</td>
	<td>plugged</td>
	<td>Wired headset mic</td>
	</tr>
	<tr>
	<td>connected</td>
	<td>unplugged</td>
	<td>microphone</td>
	</tr>
	<tr>
	<td>disconnected</td>
	<td>plugged</td>
	<td>Wired headset mic</td>
	</tr>
	<tr>
	<td>disconnected</td>
	<td>unplugged</td>
	<td>Wired headset mic</td>
	</tr>
	</table></div>

	@par
	If application changes routing policy to SYSTEM_AUDIO_ROUTE_POLICY_IGNORE_A2DP with mm_sound_route_set_system_policy
	audio routing policy has changed to ignore bluetooth headset connection.
	@par
	for playback
	<div><table>
	<tr>
	<td><B>Bluetooth headset</B></td>
	<td><B>Wired headset</B></td>
	<td><B>Playback Device</B></td>
	</tr>
	<tr>
	<td>connected</td>
	<td>plugged</td>
	<td>Wired headset</td>
	</tr>
	<tr>
	<td>connected</td>
	<td>unplugged</td>
	<td>Loud speaker</td>
	</tr>
	<tr>
	<td>disconnected</td>
	<td>plugged</td>
	<td>Wired headset</td>
	</tr>
	<tr>
	<td>disconnected</td>
	<td>unplugged</td>
	<td>Loud speaker</td>
	</tr>
	</table></div>

	@par
	for capture (bluetooth headset mic used only in call mode)
	<div><table>
	<tr>
	<td><B>Bluetooth headset mic</B></td>
	<td><B>Wired headset mic</B></td>
	<td><B>Capture Device</B></td>
	</tr>
	<tr>
	<td>connected</td>
	<td>plugged</td>
	<td>Wired headset mic</td>
	</tr>
	<tr>
	<td>connected</td>
	<td>unplugged</td>
	<td>microphone</td>
	</tr>
	<tr>
	<td>disconnected</td>
	<td>plugged</td>
	<td>Wired headset mic</td>
	</tr>
	<tr>
	<td>disconnected</td>
	<td>unplugged</td>
	<td>Wired headset mic</td>
	</tr>
	</table></div>

	@par
	If application changes routing policy to SYSTEM_AUDIO_ROUTE_POLICY_HANDSET_ONLY with mm_sound_route_set_system_policy
	audio routing policy has changed to use only loud speaker and microphone.
	@par
	for playback
	<div><table>
	<tr>
	<td><B>Bluetooth headset</B></td>
	<td><B>Wired headset</B></td>
	<td><B>Playback Device</B></td>
	</tr>
	<tr>
	<td>connected</td>
	<td>plugged</td>
	<td>Loud speaker</td>
	</tr>
	<tr>
	<td>connected</td>
	<td>unplugged</td>
	<td>Loud speaker</td>
	</tr>
	<tr>
	<td>disconnected</td>
	<td>plugged</td>
	<td>Loud speaker</td>
	</tr>
	<tr>
	<td>disconnected</td>
	<td>unplugged</td>
	<td>Loud speaker</td>
	</tr>
	</table></div>

	@par
	for capture (bluetooth headset mic used only in call mode)
	<div><table>
	<tr>
	<td><B>Bluetooth headset mic</B></td>
	<td><B>Wired headset mic</B></td>
	<td><B>Capture Device</B></td>
	</tr>
	<tr>
	<td>connected</td>
	<td>plugged</td>
	<td>microphone</td>
	</tr>
	<tr>
	<td>connected</td>
	<td>unplugged</td>
	<td>microphone</td>
	</tr>
	<tr>
	<td>disconnected</td>
	<td>plugged</td>
	<td>microphone</td>
	</tr>
	<tr>
	<td>disconnected</td>
	<td>unplugged</td>
	<td>microphone</td>
	</tr>
	</table></div>

 */

/*
 * MMSound Volume APIs
 */

/**
 * Enumerations of Volume type.
 */

typedef enum {
	VOLUME_TYPE_SYSTEM,				/**< System volume type */
	VOLUME_TYPE_NOTIFICATION,		/**< Notification volume type */
	VOLUME_TYPE_ALARM,				/**< Alarm volume type */
	VOLUME_TYPE_RINGTONE,			/**< Ringtone volume type */
	VOLUME_TYPE_MEDIA,				/**< Media volume type */
	VOLUME_TYPE_CALL,				/**< Call volume type */
	VOLUME_TYPE_VOIP,				/**< VOIP volume type */
	VOLUME_TYPE_VOICE,				/**< VOICE volume type */
	VOLUME_TYPE_FIXED,				/**< Volume type for fixed acoustic level */
	VOLUME_TYPE_MAX,				/**< Volume type count */
} volume_type_t;

typedef enum {
	VOLUME_TYPE_BIXBY,				/**< Bixby volume type */
} volume_type_internal_t;

typedef enum {
	VOLUME_GAIN_DEFAULT		= 0,
	VOLUME_GAIN_DIALER		= 1<<8,
	VOLUME_GAIN_TOUCH		= 2<<8,
	VOLUME_GAIN_AF			= 3<<8,
	VOLUME_GAIN_SHUTTER1	= 4<<8,
	VOLUME_GAIN_SHUTTER2	= 5<<8,
	VOLUME_GAIN_CAMCORDING	= 6<<8,
	VOLUME_GAIN_MIDI		= 7<<8,
	VOLUME_GAIN_BOOTING		= 8<<8,
	VOLUME_GAIN_VIDEO		= 9<<8,
	VOLUME_GAIN_TTS			= 10<<8,
} volume_gain_t;

/**
 * @brief Enumerations of supporting source_type
 */
typedef enum {
	SUPPORT_SOURCE_TYPE_DEFAULT,
	SUPPORT_SOURCE_TYPE_MIRRORING,
	SUPPORT_SOURCE_TYPE_VOICECONTROL,
	SUPPORT_SOURCE_TYPE_SVR,
	SUPPORT_SOURCE_TYPE_VIDEOCALL,
	SUPPORT_SOURCE_TYPE_VOICERECORDING,
	SUPPORT_SOURCE_TYPE_VOIP, /* Supporting VoIP source*/
	SUPPORT_SOURCE_TYPE_CALL_FORWARDING,
	SUPPORT_SOURCE_TYPE_FMRADIO,
	SUPPORT_SOURCE_TYPE_LOOPBACK,
} mm_sound_source_type_e;

/**
 * Volume change callback function prototype.
 *
 * @param	type			[in]	The type of changed volume
 * @param	volume			[in]	The new volume value
 * @param	user_data		[in]	Argument passed when callback has called
 *
 * @return	No return value
 * @remark	None.
 * @see		mm_sound_add_volume_changed_callback mm_sound_remove_volume_changed_callback
 */
typedef void (*mm_sound_volume_changed_cb) (volume_type_t type, unsigned int volume, void *user_data);

int mm_sound_add_volume_changed_callback(mm_sound_volume_changed_cb func, void* user_data, unsigned int *id);

/**
 * This function is to remove volume change callback.
 *
 * @return	This function returns MM_ERROR_NONE on success, or negative value
 *			with error code.
 **/
int mm_sound_remove_volume_changed_callback(unsigned int id);

/**
 * This function is to set volume level of certain volume type.
 *
 * @param	type			[in]	volume type to set value.
 * @param	value			[in]	volume value.
 *
 * @return	This function returns MM_ERROR_NONE on success, or negative value
 *			with error code.
 * @remark	value should be 0 ~ MAX
 * @see		mm_sound_volume_get_value volume_type_t
 * @pre		None.
 * @post	Volume value will be changed to given value for given volume type.
 * @par Example
 */
int mm_sound_volume_set_value(volume_type_t type, const unsigned int value);

/**
 * This function is to get volume level of certain volume type.
 *
 * @param	type			[in]	volume type to get value.
 * @param	value			[out]	volume value.
 *
 * @return	This function returns MM_ERROR_NONE on success, or negative value
 *			with error code.
 * @remark	None.
 * @pre		None.
 * @post	None.
 * @see		volume_type_t mm_sound_volume_set_value
 * @par Example
 * @code
int value = 0;
int ret = 0;

ret = mm_sound_volume_get_value(VOLUME_TYPE_SYSTEM, &value);
if(ret < 0)
{
	printf("Can not get volume\n");
}
else
{
	printf("System type volume is %d\n", value);
}
 * @endcode
 * @see		mm_sound_volume_set_value
 */
int mm_sound_volume_get_value(volume_type_t type, unsigned int *value);

int mm_sound_set_mute(volume_type_t type, bool mute);
int mm_sound_get_mute(volume_type_t type, bool *muted);


typedef void (*mm_sound_volume_changed_cb_internal) (volume_type_internal_t type, unsigned int volume, void *user_data);
int mm_sound_add_volume_changed_callback_internal(mm_sound_volume_changed_cb_internal func, void* user_data, unsigned int *id);
int mm_sound_remove_volume_changed_callback_internal(unsigned int id);
int mm_sound_volume_set_value_internal(volume_type_internal_t type, const unsigned int value);
int mm_sound_volume_get_value_internal(volume_type_internal_t type, unsigned int *value);

/**
 * This function is to set sound filter and apply to selected stream type.
 *
 * @param	stream_type			[in]	stream type to apply
 * @param	filter_name			[in]	name of filter module to apply
 * @param	filter_parameters	[in]	extra filter parameters(optional)
 * @param	filter_group		[in]	extra filter group(optional)
 *
 * @return	This function returns MM_ERROR_NONE on success, or negative value
 *			with error code.
 * @remark	None
 * @pre		None
 * @post	None
 * @see		None
 */
int mm_sound_set_filter(const char *stream_type, const char *filter_name, const char *filter_parameters, const char *filter_group);

/**
 * This function is to unset sound filter and remove from selected stream type.
 *
 * @param	stream_type			[in]	stream type to remove
 *
 * @return	This function returns MM_ERROR_NONE on success, or negative value
 *			with error code.
 * @remark	None
 * @pre		None
 * @post	None
 * @see		None
 */
int mm_sound_unset_filter(const char *stream_type);

/**
 * This function is to control filter configurations to selected stream type.
 *
 * @param	stream_type			[in]	stream type to apply
 * @param	filter_name			[in]	specify filter module to apply
 * @param	filter_controls		[in]	filter control parameters(e.g, "0.0, 0.1, 3.0")
 *
 * @return	This function returns MM_ERROR_NONE on success, or negative value
 *			with error code.
 * @remark	None
 * @pre		None
 * @post	None
 * @see		None
 */
int mm_sound_control_filter(const char *stream_type, const char *filter_name, const char *filter_controls);

/**
 * Terminate callback function type.
 *
 * @param	data		[in]	Argument passed when callback was set
 * @param	id			[in]	handle which has completed playing
 *
 * @return	No return value
 * @remark	It is not allowed to call MMSound API recursively or do time-consuming
 *			task in this callback because this callback is called synchronously.
 * @see		mm_sound_play_sound
 */
typedef void (*mm_sound_stop_callback_func) (void *data, int id);

/*
 * Enumerations of device & route
 */

typedef enum {
	MM_SOUND_DIRECTION_NONE,
	MM_SOUND_DIRECTION_IN,							/**< Capture */
	MM_SOUND_DIRECTION_OUT,							/**< Playback */
} mm_sound_direction;

typedef enum {
	MM_SOUND_DEVICE_IN_NONE				= 0x00,
	MM_SOUND_DEVICE_IN_MIC				= 0x01,		/**< Device builtin microphone */
	MM_SOUND_DEVICE_IN_WIRED_ACCESSORY	= 0x02,		/**< Wired input devices */
	MM_SOUND_DEVICE_IN_BT_SCO	= 0x08,		/**< Bluetooth SCO device */
} mm_sound_device_in;

typedef enum {
	MM_SOUND_DEVICE_OUT_NONE			= 0x000,
	MM_SOUND_DEVICE_OUT_SPEAKER		= 0x001<<8,	/**< Device builtin speaker */
	MM_SOUND_DEVICE_OUT_RECEIVER		= 0x002<<8,	/**< Device builtin receiver */
	MM_SOUND_DEVICE_OUT_WIRED_ACCESSORY	= 0x004<<8,	/**< Wired output devices such as headphone, headset, and so on. */
	MM_SOUND_DEVICE_OUT_BT_SCO			= 0x008<<8,	/**< Bluetooth SCO device */
	MM_SOUND_DEVICE_OUT_BT_A2DP		= 0x010<<8,	/**< Bluetooth A2DP device */
	MM_SOUND_DEVICE_OUT_DOCK			= 0x020<<8,	/**< DOCK device */
	MM_SOUND_DEVICE_OUT_HDMI			= 0x040<<8,	/**< HDMI device */
	MM_SOUND_DEVICE_OUT_MIRRORING		= 0x080<<8,	/**< MIRRORING device */
	MM_SOUND_DEVICE_OUT_USB_AUDIO		= 0x100<<8,	/**< USB Audio device */
	MM_SOUND_DEVICE_OUT_MULTIMEDIA_DOCK	= 0x200<<8,	/**< Multimedia DOCK device */
} mm_sound_device_out;

typedef enum {
	MM_SOUND_VOLUME_DEVICE_OUT_SPEAKER,				/**< Device builtin speaker */
	MM_SOUND_VOLUME_DEVICE_OUT_RECEIVER,			/**< Device builtin receiver */
	MM_SOUND_VOLUME_DEVICE_OUT_WIRED_ACCESSORY,		/**< Wired output devices such as headphone, headset, and so on. */
	MM_SOUND_VOLUME_DEVICE_OUT_BT_SCO,				/**< Bluetooth SCO device */
	MM_SOUND_VOLUME_DEVICE_OUT_BT_A2DP,				/**< Bluetooth A2DP device */
	MM_SOUND_VOLUME_DEVICE_OUT_DOCK,				/**< DOCK device */
	MM_SOUND_VOLUME_DEVICE_OUT_HDMI,				/**< HDMI device */
	MM_SOUND_VOLUME_DEVICE_OUT_MIRRORING,			/**< MIRRORING device */
	MM_SOUND_VOLUME_DEVICE_OUT_USB_AUDIO,			/**< USB Audio device */
	MM_SOUND_VOLUME_DEVICE_OUT_MULTIMEDIA_DOCK,		/**< Multimedia DOCK device */
} mm_sound_volume_device_out_t;

/*
 * MMSound Device APIs
 */

typedef enum {
	MM_SOUND_DEVICE_IO_DIRECTION_IN_FLAG      = 0x0001,  /**< Flag for input devices */
	MM_SOUND_DEVICE_IO_DIRECTION_OUT_FLAG     = 0x0002,  /**< Flag for output devices */
	MM_SOUND_DEVICE_IO_DIRECTION_BOTH_FLAG    = 0x0004,  /**< Flag for input/output devices (both directions are available) */
	MM_SOUND_DEVICE_TYPE_INTERNAL_FLAG        = 0x0010,  /**< Flag for built-in devices */
	MM_SOUND_DEVICE_TYPE_EXTERNAL_FLAG        = 0x0020,  /**< Flag for external devices */
	MM_SOUND_DEVICE_STATE_DEACTIVATED_FLAG    = 0x1000,  /**< Flag for deactivated devices */
	MM_SOUND_DEVICE_STATE_ACTIVATED_FLAG      = 0x2000,  /**< Flag for activated devices */
	MM_SOUND_DEVICE_ALL_FLAG                  = 0xFFFF,  /**< Flag for all devices */
} mm_sound_device_flags_e;

typedef enum {
	MM_SOUND_DEVICE_IO_DIRECTION_IN           = 0x1,
	MM_SOUND_DEVICE_IO_DIRECTION_OUT          = 0x2,
	MM_SOUND_DEVICE_IO_DIRECTION_BOTH         = MM_SOUND_DEVICE_IO_DIRECTION_IN | MM_SOUND_DEVICE_IO_DIRECTION_OUT,
} mm_sound_device_io_direction_e;

typedef enum {
	MM_SOUND_DEVICE_STATE_DEACTIVATED,
	MM_SOUND_DEVICE_STATE_ACTIVATED,
} mm_sound_device_state_e;

typedef enum {
	MM_SOUND_DEVICE_TYPE_BUILTIN_SPEAKER,   /**< Built-in speaker */
	MM_SOUND_DEVICE_TYPE_BUILTIN_RECEIVER,  /**< Built-in receiver */
	MM_SOUND_DEVICE_TYPE_BUILTIN_MIC,       /**< Built-in microphone */
	MM_SOUND_DEVICE_TYPE_AUDIOJACK,         /**< Audio jack such as headphone, headset, and so on */
	MM_SOUND_DEVICE_TYPE_BLUETOOTH,         /**< Bluetooth */
	MM_SOUND_DEVICE_TYPE_BLUETOOTH_A2DP = MM_SOUND_DEVICE_TYPE_BLUETOOTH,    /**< Bluetooth A2DP */
	MM_SOUND_DEVICE_TYPE_HDMI,              /**< HDMI */
	MM_SOUND_DEVICE_TYPE_MIRRORING,         /**< MIRRORING */
	MM_SOUND_DEVICE_TYPE_USB_AUDIO,         /**< USB Audio */
	MM_SOUND_DEVICE_TYPE_BLUETOOTH_SCO,     /**< Bluetooth SCO */
	MM_SOUND_DEVICE_TYPE_NETWORK            /**< Network */
} mm_sound_device_type_e;

typedef void *MMSoundDevice_t;          /**< MMsound Device handle */
typedef void *MMSoundDeviceList_t;      /**< MMsound Device list handle */
typedef void (*mm_sound_device_connected_cb) (MMSoundDevice_t device_h, bool is_connected, void *user_data);
typedef void (*mm_sound_device_info_changed_cb) (MMSoundDevice_t device_h, int changed_info_type, void *user_data);
typedef void (*mm_sound_device_state_changed_cb) (MMSoundDevice_t device_h, mm_sound_device_state_e state, void *user_data);
typedef void (*mm_sound_device_running_changed_cb) (MMSoundDevice_t device_h, bool is_running, void *user_data);

int mm_sound_add_device_connected_callback(int flags, mm_sound_device_connected_cb func, void *user_data, unsigned int *id);
int mm_sound_remove_device_connected_callback(unsigned int id);
int mm_sound_add_device_information_changed_callback(int flags, mm_sound_device_info_changed_cb func, void *user_data, unsigned int *id);
int mm_sound_remove_device_information_changed_callback(unsigned int id);
int mm_sound_add_device_state_changed_callback(int flags, mm_sound_device_state_changed_cb func, void *user_data, unsigned int *id);
int mm_sound_remove_device_state_changed_callback(unsigned int id);
int mm_sound_add_device_running_changed_callback(int flags, mm_sound_device_running_changed_cb func, void *user_data, unsigned int *id);
int mm_sound_remove_device_running_changed_callback(unsigned int id);

int mm_sound_get_current_device_list(mm_sound_device_flags_e device_mask, MMSoundDeviceList_t *device_list);
int mm_sound_get_device_list(int device_mask, MMSoundDeviceList_t *device_list);
/* Free this device handle with mm_sound_free_device */
int mm_sound_get_device_by_id(int device_id, MMSoundDevice_t *device);
int mm_sound_free_device_list(MMSoundDeviceList_t device_list);
/* Use this only for the device handle which got from mm_sound_get_device_by_id */
int mm_sound_free_device(MMSoundDevice_t device_h);
int mm_sound_get_next_device(MMSoundDeviceList_t device_list, MMSoundDevice_t *device);
int mm_sound_get_prev_device(MMSoundDeviceList_t device_list, MMSoundDevice_t *device);
int mm_sound_get_device_type(MMSoundDevice_t device_h, mm_sound_device_type_e *type);
int mm_sound_get_device_io_direction(MMSoundDevice_t device_h, mm_sound_device_io_direction_e *io_direction);
int mm_sound_get_device_id(MMSoundDevice_t device_h, int *id);
int mm_sound_get_device_state(MMSoundDevice_t device_h, mm_sound_device_state_e *state);
int mm_sound_get_device_name(MMSoundDevice_t device_h, char **name);
int mm_sound_get_device_vendor_id(MMSoundDevice_t device_h, int *vendor_id);
int mm_sound_get_device_product_id(MMSoundDevice_t device_h, int *product_id);

int mm_sound_get_device_sample_format(MMSoundDevice_t device_h, int *format);
int mm_sound_get_device_sample_rate(MMSoundDevice_t device_h, int *samplerate);
int mm_sound_get_device_channels(MMSoundDevice_t device_h, int *channels);

/* Ducking */
typedef void (*mm_sound_ducking_state_changed_cb) (int idx, bool is_ducked, void *user_data);
int mm_sound_add_ducking_state_changed_callback(mm_sound_ducking_state_changed_cb func, void *user_data, unsigned int *subs_id);
int mm_sound_remove_ducking_state_changed_callback(unsigned int subs_id);

/**
 * Active device changed callback function type.
 *
 * @param	user_data		[in]	Argument passed when callback has called
 *
 * @return	No return value
 * @remark	None.
 * @see		mm_sound_add_active_device_changed_callback mm_sound_remove_active_device_changed_callback
 */
typedef void (*mm_sound_active_device_changed_cb) (mm_sound_device_in device_in, mm_sound_device_out device_out, void *user_data);

typedef void (*mm_sound_test_cb) (int a, void *user_data);
int mm_sound_test(int a, int b, int* get);
int mm_sound_add_test_callback(mm_sound_test_cb func, void *user_data, unsigned int *id);
int mm_sound_remove_test_callback(unsigned int sub_id);

void mm_sound_convert_volume_type_to_stream_type(int volume_type, char *stream_type);

int mm_sound_is_stream_on_device(int stream_id, MMSoundDevice_t device_h, bool *is_on);
int mm_sound_is_stream_on_device_by_id(int stream_id, int device_id, bool *is_on);
int mm_sound_is_device_running(MMSoundDevice_t device_h, bool *is_running);

#ifdef TIZEN_TV
void mm_sound_dotnet_cleanup(int signo);
#endif

/**
	@}
 */

#ifdef __cplusplus
}
#endif

#endif	/* __MM_SOUND_H__ */

