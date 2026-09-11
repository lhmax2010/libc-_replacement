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

#ifndef __TIZEN_MEDIA_PLAYER_H__
#define __TIZEN_MEDIA_PLAYER_H__

#include <tizen.h>
#include <sound_manager.h>
#include <media_packet.h>

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @brief The player error class define.
 * @since_tizen 2.3
 */
#define PLAYER_ERROR_CLASS          TIZEN_ERROR_PLAYER | 0x20

/**
 * @brief The player custom error class define.
 * @since_tizen 3.0
 */
#define PLAYER_CUSTOM_ERROR_CLASS   TIZEN_ERROR_PLAYER | 0x1000

/**
 * @file player.h
 * @brief This file contains the media player API.
 */

/**
 * @addtogroup CAPI_MEDIA_PLAYER_MODULE
 * @{
 */

/**
 * @brief The media player's type handle.
 * @since_tizen 2.3
 */
typedef struct player_s *player_h;

/**
 * @brief Enumeration for media player state.
 * @since_tizen 2.3
 */
typedef enum {
	PLAYER_STATE_NONE,          /**< Player is not created */
	PLAYER_STATE_IDLE,          /**< Player is created, but not prepared */
	PLAYER_STATE_READY,         /**< Player is ready to play media */
	PLAYER_STATE_PLAYING,       /**< Player is playing media */
	PLAYER_STATE_PAUSED,        /**< Player is paused while playing media */
} player_state_e;

/**
 * @brief Enumeration for media player's error codes.
 * @since_tizen 2.3
 */
typedef enum {
	PLAYER_ERROR_NONE               = TIZEN_ERROR_NONE,                             /**< Successful */
	PLAYER_ERROR_OUT_OF_MEMORY      = TIZEN_ERROR_OUT_OF_MEMORY,                    /**< Out of memory */
	PLAYER_ERROR_INVALID_PARAMETER  = TIZEN_ERROR_INVALID_PARAMETER,                /**< Invalid parameter */
	PLAYER_ERROR_NO_SUCH_FILE       = TIZEN_ERROR_NO_SUCH_FILE,                     /**< No such file or directory */
	PLAYER_ERROR_INVALID_OPERATION  = TIZEN_ERROR_INVALID_OPERATION,                /**< Invalid operation */
	PLAYER_ERROR_FILE_NO_SPACE_ON_DEVICE    = TIZEN_ERROR_FILE_NO_SPACE_ON_DEVICE,  /**< No space left on the device */
	PLAYER_ERROR_FEATURE_NOT_SUPPORTED_ON_DEVICE    = TIZEN_ERROR_NOT_SUPPORTED,    /**< Not supported Feature */
	PLAYER_ERROR_SEEK_FAILED        = PLAYER_ERROR_CLASS | 0x01,                    /**< Seek operation failure */
	PLAYER_ERROR_INVALID_STATE      = PLAYER_ERROR_CLASS | 0x02,                    /**< Invalid state */
	PLAYER_ERROR_NOT_SUPPORTED_FILE = PLAYER_ERROR_CLASS | 0x03,                    /**< File format not supported */
	PLAYER_ERROR_INVALID_URI        = PLAYER_ERROR_CLASS | 0x04,                    /**< Invalid URI */
	PLAYER_ERROR_SOUND_POLICY       = PLAYER_ERROR_CLASS | 0x05,                    /**< Sound policy error */
	PLAYER_ERROR_CONNECTION_FAILED  = PLAYER_ERROR_CLASS | 0x06,                    /**< Streaming connection failed */
	PLAYER_ERROR_VIDEO_CAPTURE_FAILED   = PLAYER_ERROR_CLASS | 0x07,                /**< Video capture failed */
	PLAYER_ERROR_DRM_EXPIRED        = PLAYER_ERROR_CLASS | 0x08,                    /**< Expired license */
	PLAYER_ERROR_DRM_NO_LICENSE     = PLAYER_ERROR_CLASS | 0x09,                    /**< No license */
	PLAYER_ERROR_DRM_FUTURE_USE     = PLAYER_ERROR_CLASS | 0x0a,                    /**< License for future use */
	PLAYER_ERROR_DRM_NOT_PERMITTED  = PLAYER_ERROR_CLASS | 0x0b,                    /**< Format not permitted */
	PLAYER_ERROR_RESOURCE_LIMIT     = PLAYER_ERROR_CLASS | 0x0c,                    /**< Resource limit */
	PLAYER_ERROR_PERMISSION_DENIED  = TIZEN_ERROR_PERMISSION_DENIED,                /**< Permission denied */
	PLAYER_ERROR_SERVICE_DISCONNECTED = PLAYER_ERROR_CLASS | 0x0d,                  /**< Socket connection lost (Since 3.0) */
	PLAYER_ERROR_BUFFER_SPACE         = TIZEN_ERROR_BUFFER_SPACE,                   /**< No buffer space available (Since 3.0) */
	PLAYER_ERROR_NOT_SUPPORTED_AUDIO_CODEC = PLAYER_ERROR_CLASS | 0x0e,             /**< Not supported audio codec but video can be played (Since 4.0) */
	PLAYER_ERROR_NOT_SUPPORTED_VIDEO_CODEC = PLAYER_ERROR_CLASS | 0x0f,             /**< Not supported video codec but audio can be played (Since 4.0) */
	PLAYER_ERROR_NOT_SUPPORTED_SUBTITLE = PLAYER_ERROR_CLASS | 0x10,                /**< Not supported subtitle format (Since 4.0) */
	PLAYER_ERROR_NOT_SUPPORTED_FORMAT = PLAYER_ERROR_CLASS | 0x11,                  /**< Not supported format (Since 5.5) */
	PLAYER_ERROR_NOT_AVAILABLE        = PLAYER_ERROR_CLASS | 0x12,                  /**< Not available operation (Since 5.5)*/
} player_error_e;

/**
 * @brief Enumeration for media player's interruption type.
 * @since_tizen 2.3
 */
typedef enum {
	PLAYER_INTERRUPTED_BY_RESOURCE_CONFLICT = 4,    /**< Interrupted by a resource conflict and the player handle will be paused or unprepared */
} player_interrupted_code_e;

/**
 * @brief Enumeration for display type.
 * @since_tizen 2.3
 * @details In case of using #PLAYER_DISPLAY_TYPE_OVERLAY_SYNC_UI, the video is rendering in full scale in the rendering area.\n
 *          To change the video rendering position, use UI API functions like ecore_wl2_subsurface_exported_surface_move(),
 *          ecore_wl2_subsurface_exported_surface_resize(), ecore_wl2_subsurface_exported_surface_transform_set(),
 *          ecore_wl2_subsurface_exported_surface_show() and ecore_wl2_subsurface_exported_surface_commit().\n
 *          player_set_display_mode(), player_set_display_rotation() and player_set_display_roi_area() cannot be used and
 *          #PLAYER_ERROR_NOT_AVAILABLE is returned if they are used.\n
 *          Exported shell handle must be obtained by ecore_wl2_subsurface_exported_surface_handle_get() from Ecore_Wl2_Subsurface.\n
 *          Then the exported shell handle should be set with player_set_display().\n
 *          To render the first video buffer, commit by ecore_wl2_window_commit() after player_prepare() of Ecore_Wl2_Subsurface.\n
 *          Always commit with ecore_wl2_window_commit() with parent surface when the UI changes and the video needs to be synchronized.
 */
typedef enum {
	PLAYER_DISPLAY_TYPE_OVERLAY = 0,        /**< Overlay surface display (@deprecated Deprecated since 10.1) */
	PLAYER_DISPLAY_TYPE_EVAS = 3,           /**< Evas image object surface display (Since 4.0) (@deprecated Deprecated since 10.1) */
	PLAYER_DISPLAY_TYPE_NONE = 4,           /**< This disposes of buffers (Since 4.0) */
	PLAYER_DISPLAY_TYPE_OVERLAY_SYNC_UI = 5 /**< Overlay surface display for synchronization between UI and video (Since 6.5) (@deprecated Deprecated since 10.1) */
} player_display_type_e;

/**
 * @brief Enumeration for audio latency mode.
 * @since_tizen 2.3
 */
typedef enum {
	AUDIO_LATENCY_MODE_LOW = 0,     /**< Low audio latency mode */
	AUDIO_LATENCY_MODE_MID,         /**< Middle audio latency mode */
	AUDIO_LATENCY_MODE_HIGH,        /**< High audio latency mode */
} audio_latency_mode_e;

/**
 * @brief Enumeration for stream type.
 * @since_tizen 2.4
 */
typedef enum {
	PLAYER_STREAM_TYPE_DEFAULT,	/**< Container type */
	PLAYER_STREAM_TYPE_AUDIO,	/**< Audio element stream type */
	PLAYER_STREAM_TYPE_VIDEO,	/**< Video element stream type */
	PLAYER_STREAM_TYPE_TEXT,	/**< Text type */
} player_stream_type_e;

/**
 * @brief Enumeration of media stream buffer status.
 * @since_tizen 2.4
 * @see player_set_media_stream_buffer_max_size()
 * @see player_set_media_stream_buffer_min_threshold()
 */
typedef enum {
	PLAYER_MEDIA_STREAM_BUFFER_UNDERRUN, /**< The buffer level is lower than the threshold */
	PLAYER_MEDIA_STREAM_BUFFER_OVERFLOW, /**< The buffer level is over than the threshold */
} player_media_stream_buffer_status_e;

/**
 * @brief Enumeration of audio extract option.
 * @since_tizen 5.5
 * @see player_set_media_packet_audio_frame_decoded_cb()
 */
typedef enum {
	PLAYER_AUDIO_EXTRACT_DEFAULT                        = 0x00,   /**< Synchronized multichannel audio stream with the playback clock */
	PLAYER_AUDIO_EXTRACT_NO_SYNC_WITH_CLOCK             = 0x01,   /**< No synchronized multichannel audio stream with the playback clock */
	PLAYER_AUDIO_EXTRACT_DEINTERLEAVE                   = 0x02,   /**< Synchronized mono audio stream with the playback clock */
	PLAYER_AUDIO_EXTRACT_NO_SYNC_AND_DEINTERLEAVE       = 0x03,   /**< No synchronized mono audio stream with playback clock */
} player_audio_extract_option_e;

/**
 * @brief Enumeration for codec type.
 * @since_tizen 5.5
 */
typedef enum {
	PLAYER_CODEC_TYPE_HW,           /**< This is an optional flag for using the H/W codec */
	PLAYER_CODEC_TYPE_SW,           /**< This is an optional flag for using the S/W codec */
} player_codec_type_e;

/**
 * @}
 */

/**
 * @addtogroup CAPI_MEDIA_PLAYER_DISPLAY_MODULE
 * @{
 */

/**
 * @brief The player display handle.
 * @since_tizen 2.3
 */
typedef void *player_display_h;

#ifndef GET_DISPLAY
/**
 * @brief Definition for a display handle from evas object.
 * @since_tizen 2.3
 */
#define GET_DISPLAY(x) (void *)(x)
#endif

/**
 * @brief Enumeration for display rotation type.
 * @since_tizen 2.3
 */
typedef enum {
	PLAYER_DISPLAY_ROTATION_NONE,   /**< Display is not rotated */
	PLAYER_DISPLAY_ROTATION_90,     /**< Display is rotated 90 degrees */
	PLAYER_DISPLAY_ROTATION_180,    /**< Display is rotated 180 degrees */
	PLAYER_DISPLAY_ROTATION_270,    /**< Display is rotated 270 degrees */
} player_display_rotation_e;

/**
 * @brief Enumeration for display mode.
 * @since_tizen 2.3
 */
typedef enum {
	PLAYER_DISPLAY_MODE_LETTER_BOX = 0,     /**< Letter box */
	PLAYER_DISPLAY_MODE_ORIGIN_SIZE,        /**< Origin size */
	PLAYER_DISPLAY_MODE_FULL_SCREEN,        /**< Full-screen */
	PLAYER_DISPLAY_MODE_CROPPED_FULL,       /**< Cropped full-screen */
	PLAYER_DISPLAY_MODE_ORIGIN_OR_LETTER,   /**< Origin size (if surface size is larger than video size(width/height)) or Letter box (if video size(width/height) is larger than surface size) */
	PLAYER_DISPLAY_MODE_DST_ROI,            /**< Dst ROI mode */
	PLAYER_DISPLAY_MODE_NUM
} player_display_mode_e;

/**
 * @}
 */

/**
 * @addtogroup CAPI_MEDIA_PLAYER_STREAM_INFO_MODULE
 * @{
 */

/**
 * @brief Enumeration for media stream content information.
 * @since_tizen 2.3
 */
typedef enum {
	PLAYER_CONTENT_INFO_ALBUM,      /**< Album */
	PLAYER_CONTENT_INFO_ARTIST,     /**< Artist */
	PLAYER_CONTENT_INFO_AUTHOR,     /**< Author */
	PLAYER_CONTENT_INFO_GENRE,      /**< Genre */
	PLAYER_CONTENT_INFO_TITLE,      /**< Title */
	PLAYER_CONTENT_INFO_YEAR,       /**< Year */
} player_content_info_e;

/**
 * @}
 */


/**
 * @addtogroup CAPI_MEDIA_PLAYER_SUBTITLE_MODULE
 * @{
 */

/**
 * @brief  Called when the subtitle is updated.
 * @since_tizen 2.3
 * @remarks The @a text should not be released and it can be used only in the callback.
 *          To use outside, make a copy.
 * @param[in] duration    The duration of the updated subtitle
 * @param[in] text        The text of the updated subtitle
 * @param[in] user_data   The user data passed from the callback registration function
 * @see player_set_subtitle_updated_cb()
 * @see player_unset_subtitle_updated_cb()
 */
typedef void (*player_subtitle_updated_cb)(unsigned long duration, char *text, void *user_data);

/**
 * @}
 */

/**
 * @addtogroup CAPI_MEDIA_PLAYER_MODULE
 * @{
 */

/**
 * @brief Called when the media player is prepared.
 * @details It will be invoked when player has reached the begin of stream.
 * @since_tizen 2.3
 * @param[in]   user_data  The user data passed from the callback registration function
 * @pre player_prepare_async() will cause this callback.
 * @post The player state will be #PLAYER_STATE_READY.
 * @see player_prepare_async()
 */
typedef void (*player_prepared_cb)(void *user_data);

/**
 * @brief Called when the media player is completed.
 * @details It will be invoked when player has reached the end of the stream.
 * @since_tizen 2.3
 * @param[in]   user_data  The user data passed from the callback registration function
 * @pre It will be invoked when the playback is completed if you register this callback using player_set_completed_cb().
 * @see player_set_completed_cb()
 * @see player_unset_completed_cb()
 */
typedef void (*player_completed_cb)(void *user_data);

/**
 * @brief Called when the seek operation is completed.
 * @since_tizen 2.3
 * @param[in]   user_data  The user data passed from the callback registration function
 * @see player_set_play_position()
 * @see player_set_play_position_nsec()
 */
typedef void (*player_seek_completed_cb)(void *user_data);

/**
 * @brief Called when the media player is interrupted.
 * @details If the code is #PLAYER_INTERRUPTED_BY_RESOURCE_CONFLICT,
 *          the player state will be one of #PLAYER_STATE_IDLE, #PLAYER_STATE_READY, or #PLAYER_STATE_PAUSED.
 *          Application should get exact state by calling player_get_state().
 * @since_tizen 2.3
 * @param[in] code        The interrupted error code
 * @param[in] user_data   The user data passed from the callback registration function
 * @see player_set_interrupted_cb()
 * @see player_unset_interrupted_cb()
 * @see player_get_state()
 */
typedef void (*player_interrupted_cb)(player_interrupted_code_e code, void *user_data);

/**
 * @brief Called when an error occurs in the media player.
 * @details Following error codes can be delivered.
 *          #PLAYER_ERROR_INVALID_OPERATION
 *          #PLAYER_ERROR_INVALID_STATE
 *          #PLAYER_ERROR_INVALID_URI
 *          #PLAYER_ERROR_CONNECTION_FAILED
 *          #PLAYER_ERROR_DRM_NOT_PERMITTED
 *          #PLAYER_ERROR_FILE_NO_SPACE_ON_DEVICE
 *          #PLAYER_ERROR_NOT_SUPPORTED_FILE
 *          #PLAYER_ERROR_SEEK_FAILED
 *          #PLAYER_ERROR_SERVICE_DISCONNECTED
 *          #PLAYER_ERROR_NOT_SUPPORTED_AUDIO_CODEC (Since 4.0)
 *          #PLAYER_ERROR_NOT_SUPPORTED_VIDEO_CODEC (Since 4.0)
 *          #PLAYER_ERROR_NOT_SUPPORTED_SUBTITLE (Since 4.0)
 * @since_tizen 2.3
 * @param[in] error_code  The error code
 * @param[in] user_data   The user data passed from the callback registration function
 * @see player_set_error_cb()
 * @see player_unset_error_cb()
 */
typedef void (*player_error_cb)(int error_code, void *user_data);

/**
 * @brief Called when the video is captured.
 * @since_tizen 2.3
 * @remarks The color space format of the captured image is #IMAGE_UTIL_COLORSPACE_RGB888.
 * @remarks The @a captured_data should not be released and it can be used only in the callback.
 *          To use outside, make a copy.
 * @param[in] captured_data     The captured image buffer
 * @param[in] width             The width of the captured image
 * @param[in] height            The height of the captured image
 * @param[in] size              The size of the captured image
 * @param[in] user_data         The user data passed from the callback registration function
 * @see player_capture_video()
 */
typedef void (*player_video_captured_cb)(unsigned char *captured_data, int width, int height, unsigned int size, void *user_data);

/**
 * @brief Called to register for notifications about delivering media packet when each video frame is decoded.
 * @since_tizen 2.3
 * @remarks The UI update code must not be directly invoked.\n
 * @remarks The @a packet should be released using media_packet_destroy(). \n
 *          If not, the decoder will fail due to having insufficient buffer space for the decoded frame.
 *
 * @param[in] packet      Reference pointer to the media packet
 * @param[in] user_data   The user data passed from the callback registration function
 */
typedef void (*player_media_packet_video_decoded_cb)(media_packet_h packet, void *user_data);

/**
 * @brief Called to register for notifications about delivering media packet when audio data is decoded.
 * @since_tizen 5.5
 * @remarks The @a packet should be released by calling media_packet_destroy(). \n
 *          It is recommended to release it as soon as it is rendered, to avoid memory exhaustion.
 *
 * @param[in] packet      Reference pointer to the media packet
 * @param[in] user_data   The user data passed from the callback registration function
 */
typedef void (*player_media_packet_audio_decoded_cb)(media_packet_h packet, void *user_data);

/**
 * @brief Called when the buffer level drops below the threshold of max size or no free space in buffer.
 * @since_tizen 2.4
 * @remarks This function is used for media stream playback only.
 * @param[in] status    The buffer status
 * @param[in] user_data The user data passed from the callback registration function
 * @see player_set_media_stream_buffer_status_cb()
 * @see player_set_media_stream_buffer_max_size()
 * @see player_set_media_stream_buffer_min_threshold()
 */
typedef void (*player_media_stream_buffer_status_cb)(player_media_stream_buffer_status_e status, void *user_data);

/**
 * @brief Called to notify the next push-buffer offset when seeking is occurred.
 * @details The next push-buffer should produce buffers from the new offset.
 * @since_tizen 2.4
 * @remarks This function is used for media stream playback only.
 * @param[in] offset    The new byte position to seek
 * @param[in] user_data The user data passed from the callback registration function
 */
typedef void (*player_media_stream_seek_cb)(unsigned long long offset, void *user_data);

/**
 * @brief Called to inform about the supported media format MIME type.
 * @details The supported media format can vary depending on the device capabilities.
 * @since_tizen 5.5
 * @param[in] format      The supportable media format MIME type
 * @param[in] user_data   The user data passed from the callback registration function
 * @return @c true to continue with the next iteration of the loop, otherwise @c false to break out of the loop
 * @see #media_format_mimetype_e
 * @see player_audio_offload_foreach_supported_format()
 */
typedef bool (*player_supported_media_format_cb)(media_format_mimetype_e format, void *user_data);

/**
 * @}
 */

/**
 * @addtogroup CAPI_MEDIA_PLAYER_STREAM_INFO_MODULE
 * @{
 */

/**
 * @brief Called to notify the video stream changed.
 * @details The video stream changing is detected just before rendering operation.
 * @since_tizen 2.4
 * @param[in] width       The width of the captured image
 * @param[in] height      The height of the captured image
 * @param[in] fps         The frame per second of the video \n
 *                        It can be @c 0 if there is no video stream information.
 * @param[in] bit_rate    The video bit rate [Hz] \n
 *                        It can be an invalid value if there is no video stream information.
 * @param[in] user_data   The user data passed from the callback registration function
 * @see player_set_video_stream_changed_cb()
 */
typedef void (*player_video_stream_changed_cb)(int width, int height, int fps, int bit_rate, void *user_data);

/**
 * @}
 */

/**
 * @addtogroup CAPI_MEDIA_PLAYER_STREAMING_MODULE
 * @{
 */

/**
 * @brief Called when the buffering percentage of the media playback is updated.
 * @details If the buffer is full, it will return 100%.
 * @since_tizen 2.3
 * @param[in] percent     The percentage of buffering completed (0~100)
 * @param[in] user_data   The user data passed from the callback registration function
 * @see player_set_buffering_cb()
 * @see player_unset_buffering_cb()
 */
typedef void (*player_buffering_cb)(int percent, void *user_data);

/**
 * @brief Called to notify the streaming variant information.
 * @details The adaptive streaming protocol(hls, mpeg dash) can support variant stream condition.
 *          All the streaming variant information can be shared by calling player_foreach_adaptive_variant().
 * @since_tizen 4.0
 * @param[in] bandwidth   The bandwidth of the stream can be supportable, this is mandatory parameter
 * @param[in] width       The width of the stream, this is optional parameter
 * @param[in] height      The height of the stream, this is optional parameter
 * @param[in] user_data   The user data passed from the callback registration function
 * @see player_foreach_adaptive_variant()
 */
typedef void (*player_adaptive_variant_cb)(int bandwidth, int width, int height, void *user_data);

/**
 * @}
 */

/**
 * @addtogroup CAPI_MEDIA_PLAYER_MODULE
 * @{
 */

/**
 * @brief Creates a player handle for playing multimedia content.
 * @since_tizen 2.3
 * @remarks You must release @a player by using player_destroy().\n
 *          Although you can create multiple player handles at the same time,
 *          the player cannot guarantee proper operation because of limited resources, such as
 *          audio or display device.
 *
 * @param[out] player   A new handle to the media player
 * @return @c 0 on success,
 *         otherwise a negative error value
 * @retval #PLAYER_ERROR_NONE Successful
 * @retval #PLAYER_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #PLAYER_ERROR_OUT_OF_MEMORY Out of memory
 * @retval #PLAYER_ERROR_INVALID_OPERATION Invalid operation
 * @retval #PLAYER_ERROR_RESOURCE_LIMIT Cannot create more instance due to resource(socket, thread, etc) limitation on system.
 * @post The player state will be #PLAYER_STATE_IDLE.
 * @see player_destroy()
 */
int player_create(player_h *player);

/**
 * @brief Destroys the media player handle and releases all its resources.
 * @since_tizen 2.3
 * @remarks To completely shutdown player operation, call this function with a valid player handle from any player state.
 * @param[in] player   The handle to the media player to be destroyed
 * @return @c 0 on success,
 *         otherwise a negative error value
 * @retval #PLAYER_ERROR_NONE Successful
 * @retval #PLAYER_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #PLAYER_ERROR_INVALID_OPERATION Invalid operation
 * @pre The player state must be one of #PLAYER_STATE_IDLE, #PLAYER_STATE_READY, #PLAYER_STATE_PLAYING, or #PLAYER_STATE_PAUSED.
 * @post The player state will be #PLAYER_STATE_NONE.
 * @see player_create()
 */
int player_destroy(player_h player);

/**
 * @brief Prepares the media player for playback.
 * @since_tizen 2.3
 * @remarks The mediastorage privilege(http://tizen.org/privilege/mediastorage) must be added if any video/audio files are used to play located in the internal storage.
 * @remarks The externalstorage privilege(http://tizen.org/privilege/externalstorage) must be added if any video/audio files are used to play located in the external storage.
 * @remarks The internet privilege(http://tizen.org/privilege/internet) must be added if any URLs are used to play from network.
 * @param[in] player   The handle to the media player
 * @return @c 0 on success,
 *         otherwise a negative error value
 * @retval #PLAYER_ERROR_NONE Successful
 * @retval #PLAYER_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #PLAYER_ERROR_INVALID_URI Invalid URI
 * @retval #PLAYER_ERROR_NO_SUCH_FILE File not found
 * @retval #PLAYER_ERROR_NOT_SUPPORTED_FILE File not supported
 * @retval #PLAYER_ERROR_INVALID_OPERATION Invalid operation
 * @retval #PLAYER_ERROR_INVALID_STATE Invalid player state
 * @retval #PLAYER_ERROR_PERMISSION_DENIED Permission denied
 * @retval #PLAYER_ERROR_NOT_SUPPORTED_AUDIO_CODEC Not support audio codec format (Since 4.0)
 * @retval #PLAYER_ERROR_NOT_SUPPORTED_VIDEO_CODEC Not support video codec format (Since 4.0)
 * @pre The player state must be set to #PLAYER_STATE_IDLE by calling player_create() or player_unprepare(). After that, call player_set_uri() to load the media content you want to play.
 * @post The player state will be #PLAYER_STATE_READY.
 * @see player_prepare_async()
 * @see player_unprepare()
 * @see player_set_uri()
 */
int player_prepare(player_h player);

/**
 * @brief Prepares the media player for playback, asynchronously.
 * @since_tizen 2.3
 * @remarks The mediastorage privilege(http://tizen.org/privilege/mediastorage) must be added if any video/audio files are used to play located in the internal storage.
 *          The externalstorage privilege(http://tizen.org/privilege/externalstorage) must be added if any video/audio files are used to play located in the external storage.
 *          The internet privilege(http://tizen.org/privilege/internet) must be added if any URLs are used to play from network. \n
 *          Since 5.0: To cancel the asynchronous preparing, call player_unprepare() even in #PLAYER_STATE_IDLE state.
 * @param[in] player      The handle to the media player
 * @param[in] callback    The callback function to register
 * @param[in] user_data   The user data to be passed to the callback function
 * @return @c 0 on success,
 *         otherwise a negative error value
 * @retval #PLAYER_ERROR_NONE Successful
 * @retval #PLAYER_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #PLAYER_ERROR_INVALID_URI Invalid URI
 * @retval #PLAYER_ERROR_NO_SUCH_FILE File not found
 * @retval #PLAYER_ERROR_NOT_SUPPORTED_FILE File not supported
 * @retval #PLAYER_ERROR_INVALID_OPERATION Invalid operation
 * @retval #PLAYER_ERROR_INVALID_STATE Invalid player state
 * @retval #PLAYER_ERROR_PERMISSION_DENIED Permission denied
 * @pre The player state must be set to #PLAYER_STATE_IDLE by calling player_create() or player_unprepare(). After that, call player_set_uri() to load the media content you want to play.
 * @post It invokes player_prepared_cb() when playback is prepared.
 * @see player_prepare()
 * @see player_prepared_cb()
 * @see player_unprepare()
 * @see player_set_uri()
 */
int player_prepare_async(player_h player, player_prepared_cb callback, void *user_data);

/**
 * @brief Resets the media player.
 * @details The most recently used media is reset and no longer associated with the player.
 *          Playback is no longer possible. If you want to use the player again, you must set the data URI and call
 *          player_prepare() again.
 * @since_tizen 2.3
 * @param[in] player   The handle to the media player
 * @return @c 0 on success,
 *         otherwise a negative error value
 * @retval #PLAYER_ERROR_NONE Successful
 * @retval #PLAYER_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #PLAYER_ERROR_INVALID_OPERATION Invalid operation
 * @retval #PLAYER_ERROR_INVALID_STATE Invalid player state
 * @pre Before 5.0: The player state must be one of: #PLAYER_STATE_READY, #PLAYER_STATE_PLAYING, #PLAYER_STATE_PAUSED. \n
 * @pre Since 5.0: The player state must be one of: #PLAYER_STATE_IDLE, #PLAYER_STATE_READY, #PLAYER_STATE_PLAYING, #PLAYER_STATE_PAUSED.
 *                 #PLAYER_STATE_IDLE is allowed only if player preparation was started with player_prepare_async().
 * @post The player state will be #PLAYER_STATE_IDLE.
 * @see player_prepare()
 */
int player_unprepare(player_h player);

/**
 * @brief Sets the data source (file-path, HTTP or RTSP URI) to use.
 *
 * @details Associates media contents, referred to by the URI, with the player.
 *          If the function call is successful, subsequent calls to player_prepare() and player_start() will start playing the media.
 * @since_tizen 2.3
 * @remarks If you use HTTP or RTSP, URI must start with "http://" or "rtsp://". The default protocol is "file://".
 *          If you provide an invalid URI, you won't receive an error message until you call player_start().
 * @remarks This function must be called before calling the player_prepare() or player_prepare_async() to build the player based on the URI information.
 * @remarks The mediastorage privilege(http://tizen.org/privilege/mediastorage) must be added if any video/audio files are used to play located in the internal storage.
 * @remarks The externalstorage privilege(http://tizen.org/privilege/externalstorage) must be added if any video/audio files are used to play located in the external storage.
 * @remarks The internet privilege(http://tizen.org/privilege/internet) must be added if any URLs are used to play from network.
 *
 * @param[in] player   The handle to the media player
 * @param[in] uri      The content location, such as the file path, the URI of the HTTP or RTSP stream you want to play
 *
 * @return @c 0 on success,
 *         otherwise a negative error value
 * @retval #PLAYER_ERROR_NONE Successful
 * @retval #PLAYER_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #PLAYER_ERROR_INVALID_OPERATION Invalid operation
 * @retval #PLAYER_ERROR_INVALID_STATE Invalid player state
 * @retval #PLAYER_ERROR_PERMISSION_DENIED Permission denied
 * @pre The player state must be set to #PLAYER_STATE_IDLE by calling player_create() or player_unprepare().
 * @see player_set_memory_buffer()
 */
int player_set_uri(player_h player, const char *uri);

/**
 * @brief Sets memory as the data source.
 *
 * @details Associates media content, cached in memory, with the player. Unlike the case of player_set_uri(), the media resides in memory.
 *          If the function call is successful, subsequent calls to player_prepare() and player_start() will start playing the media.
 * @since_tizen 2.3
 * @remarks If you provide an invalid data, you won't receive an error message until you call player_start().
 * @remarks This function must be called before calling the player_prepare() or player_prepare_async()
 *          to build the player based on the data.
 *
 * @param[in] player   The handle to the media player
 * @param[in] data     The memory pointer of media data
 * @param[in] size     The size of media data
 * @return @c 0 on success,
 *         otherwise a negative error value
 * @retval #PLAYER_ERROR_NONE Successful
 * @retval #PLAYER_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #PLAYER_ERROR_INVALID_OPERATION Invalid operation
 * @retval #PLAYER_ERROR_INVALID_STATE Invalid player state
 * @pre The player state must be set to #PLAYER_STATE_IDLE by calling player_create() or player_unprepare().
 * @see player_set_uri()
 */
int player_set_memory_buffer(player_h player, const void *data, int size);

/**
 * @brief Gets the player's current state.
 * @since_tizen 2.3
 * @param[in]  player   The handle to the media player
 * @param[out] state    The current state of the player
 * @return @c 0 on success,
 *         otherwise a negative error value
 * @retval #PLAYER_ERROR_NONE Successful
 * @retval #PLAYER_ERROR_INVALID_PARAMETER Invalid parameter
 * @see #player_state_e
 */
int player_get_state(player_h player, player_state_e *state);

/**
 * @brief Sets the player's volume.
 * @details  Setting this volume adjusts the player's instance volume, not the system volume.
 *           The valid range is from 0 to 1.0, inclusive (1.0 = 100%). Default value is 1.0.
 *           To change system volume, use the @ref CAPI_MEDIA_SOUND_MANAGER_MODULE API.
 *           Finally, it does not support to set other value into each channel currently.
 * @since_tizen 2.3
 * @param[in] player   The handle to the media player
 * @param[in] left     The left volume scalar
 * @param[in] right    The right volume scalar
 * @return @c 0 on success,
 *         otherwise a negative error value
 * @retval #PLAYER_ERROR_NONE Successful
 * @retval #PLAYER_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #PLAYER_ERROR_INVALID_OPERATION Invalid operation
 * @see player_get_volume()
 */
int player_set_volume(player_h player, float left, float right);

/**
 * @brief Gets the player's current volume factor.
 * @details The range of @a left and @a right is from @c 0 to @c 1.0, inclusive (1.0 = 100%).
 *          This function gets the player volume, not the system volume.
 *          To get the system volume, use the @ref CAPI_MEDIA_SOUND_MANAGER_MODULE API.
 * @since_tizen 2.3
 * @param[in]  player   The handle to the media player
 * @param[out] left     The current left volume scalar
 * @param[out] right    The current right volume scalar
 * @return @c 0 on success,
 *         otherwise a negative error value
 * @retval #PLAYER_ERROR_NONE Successful
 * @retval #PLAYER_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #PLAYER_ERROR_INVALID_OPERATION Invalid operation
 * @see player_set_volume()
 */
int player_get_volume(player_h player, float *left, float *right);

/**
 * @brief Sets the player's sound manager stream information.
 * @since_tizen 3.0
 * @remarks You can set sound stream information including audio routing and volume type.
 *          For more details, please refer to sound_manager.h
 * @remarks This function must be called before calling the player_prepare() or player_prepare_async()
 *          to reflect the sound stream information when the player is building.
 * @remarks This function is related to the following feature:\n
 *          %http://tizen.org/feature/multimedia.player.stream_info\n
 *          If this feature is not supported, the stream_type of the player is fixed to the #SOUND_STREAM_TYPE_MEDIA.
 * @param[in] player      The handle to the media player
 * @param[in] stream_info The sound manager info type
 * @return @c 0 on success,
 *         otherwise a negative error value
 * @retval #PLAYER_ERROR_NONE Successful
 * @retval #PLAYER_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #PLAYER_ERROR_INVALID_OPERATION Invalid operation
 * @retval #PLAYER_ERROR_INVALID_STATE Invalid player state
 * @retval #PLAYER_ERROR_FEATURE_NOT_SUPPORTED_ON_DEVICE Unsupported feature
 * @pre The player state must be set to #PLAYER_STATE_IDLE by calling player_create().
 * @see #sound_stream_info_h
 * @see sound_manager_create_stream_information()
 * @see sound_manager_destroy_stream_information()
 * @par Example
   @code
       #include <player.h>
       #include <sound_manager.h>
       ...
       player_h player = NULL;
       sound_stream_info_h stream_info = NULL;
       sound_stream_type_e stream_type = SOUND_STREAM_TYPE_MEDIA;
       ...
       sound_manager_create_stream_information (stream_type, _focus_callback, use_data, &stream_info);
       player_create (&player);
       player_set_sound_stream_info (player, stream_info);
       ...
       player_prepare_async (player, _prepared_cb, udata);
       ...
   @endcode
 */
int player_set_sound_stream_info(player_h player, sound_stream_info_h stream_info);

/**
 * @brief Sets the audio latency mode.
 * @since_tizen 2.3
 * @remarks The default audio latency mode of the player is #AUDIO_LATENCY_MODE_MID.
 *          To get the current audio latency mode, use player_get_audio_latency_mode().
 *          If it's high mode, audio output interval can be increased so, it can keep more audio data to play.
 *          But, state transition like pause or resume can be more slower than default(mid) mode.
 * @remarks If audio offload is enabled by calling player_audio_offload_set_enabled(),
 *          this function will return #PLAYER_ERROR_NOT_AVAILABLE and
 *          this will not work at all even if it was called before enabling offload. (Since 5.5)
 * @param[in] player       The handle to the media player
 * @param[in] latency_mode The latency mode to be applied to the audio
 * @return @c 0 on success,
 *         otherwise a negative error value
 * @retval #PLAYER_ERROR_NONE Successful
 * @retval #PLAYER_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #PLAYER_ERROR_INVALID_OPERATION Invalid operation
 * @retval #PLAYER_ERROR_NOT_AVAILABLE Not available (Since 5.5)
 * @see #audio_latency_mode_e
 * @see player_get_audio_latency_mode()
 * @see player_audio_offload_set_enabled()
 * @see player_audio_offload_is_enabled()
 */
int player_set_audio_latency_mode(player_h player, audio_latency_mode_e latency_mode);

/**
 * @brief Gets the current audio latency mode.
 * @since_tizen 2.3
 * @remarks If audio offload is enabled by calling player_audio_offload_set_enabled(),
 *          this function will return #PLAYER_ERROR_NOT_AVAILABLE. (Since 5.5)
 * @param[in] player        The handle to the media player
 * @param[out] latency_mode The latency mode to get from the audio
 * @return @c 0 on success,
 *         otherwise a negative error value
 * @retval #PLAYER_ERROR_NONE Successful
 * @retval #PLAYER_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #PLAYER_ERROR_INVALID_OPERATION Invalid operation
 * @retval #PLAYER_ERROR_NOT_AVAILABLE Not available (Since 5.5)
 * @see #audio_latency_mode_e
 * @see player_set_audio_latency_mode()
 * @see player_audio_offload_set_enabled()
 * @see player_audio_offload_is_enabled()
 */
int player_get_audio_latency_mode(player_h player, audio_latency_mode_e *latency_mode);

/**
 * @brief Starts or resumes playback.
 * @details Plays current media content, or resumes playback if the player is paused or buffering during HTTP streaming play.
 * @since_tizen 2.3
 * @remarks Even if you don't set visible to true by calling player_set_display_visible(), the video will be shown on #PLAYER_STATE_PLAYING state.
 * @remarks Since 3.0: \n
 *          Sound can be mixed with other sounds, if you don't control the stream focus in sound-manager module.\n
 *          You can refer to @ref CAPI_MEDIA_SOUND_MANAGER_MODULE.
 * @remarks Since 5.0: \n
 *          In case of HTTP streaming playback, the player could be internally paused for buffering.
 *          If the application calls this function during the buffering, the playback will be resumed by force
 *          and the buffering message posting by player_buffering_cb() will be stopped.
 * @param[in]   player The handle to the media player
 * @return @c 0 on success,
 *         otherwise a negative error value
 * @retval #PLAYER_ERROR_NONE Successful
 * @retval #PLAYER_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #PLAYER_ERROR_INVALID_OPERATION Invalid operation
 * @retval #PLAYER_ERROR_INVALID_STATE Invalid player state
 * @retval #PLAYER_ERROR_CONNECTION_FAILED Network connection failed
 * @retval #PLAYER_ERROR_SOUND_POLICY Sound policy error
 * @pre Before 5.0: The player state must be #PLAYER_STATE_READY or #PLAYER_STATE_PAUSED. \n
 * @pre Since 5.0: The player state must be one of #PLAYER_STATE_READY, #PLAYER_STATE_PLAYING, or #PLAYER_STATE_PAUSED.
 * @post The player state will be #PLAYER_STATE_PLAYING.
 * @post It invokes player_completed_cb() when playback completes, if you set a callback with player_set_completed_cb().
 * @see player_prepare()
 * @see player_prepare_async()
 * @see player_stop()
 * @see player_pause()
 * @see player_set_completed_cb()
 * @see player_completed_cb()
 * @see player_set_display_visible()
 * @see player_set_buffering_cb()
 * @see player_buffering_cb()
 */
int player_start(player_h player);

/**
 * @brief Stops playing media content.
 * @since_tizen 2.3
 * @param[in]   player The handle to the media player
 * @return @c 0 on success,
 *         otherwise a negative error value
 * @retval #PLAYER_ERROR_NONE Successful
 * @retval #PLAYER_ERROR_INVALID_PARAMETER Invalid state
 * @retval #PLAYER_ERROR_INVALID_OPERATION Invalid operation
 * @retval #PLAYER_ERROR_INVALID_STATE Invalid player state
 * @retval #PLAYER_ERROR_SOUND_POLICY Sound policy error
 * @pre The player state must be set to #PLAYER_STATE_PLAYING by calling player_start() or set to #PLAYER_STATE_PAUSED by calling player_pause().
 * @post The player state will be #PLAYER_STATE_READY.
 * @see player_start()
 * @see player_pause()
 */
int player_stop(player_h player);

/**
 * @brief Pauses the player.
 * @since_tizen 2.3
 * @remarks You can resume playback using player_start().
 *
 * @param[in]   player The handle to the media player
 * @return @c 0 on success,
 *         otherwise a negative error value
 * @retval #PLAYER_ERROR_NONE Successful
 * @retval #PLAYER_ERROR_INVALID_PARAMETER Invalid state
 * @retval #PLAYER_ERROR_INVALID_OPERATION Invalid operation
 * @retval #PLAYER_ERROR_INVALID_STATE Invalid player state
 * @retval #PLAYER_ERROR_SOUND_POLICY Sound policy error
 * @pre The player state must be set to #PLAYER_STATE_PLAYING.
 * @post The player state will be #PLAYER_STATE_PAUSED.
 * @see player_start()
 */
int player_pause(player_h player);

/**
 * @brief Sets the seek position for playback, asynchronously.
 * @since_tizen 2.3
 * @remarks This function will trigger the seeking operation on player instance like in the case of player_set_play_position_nsec().
 *          Normally application needs to wait for player_seek_completed_cb() before calling setting position function again.
 *          Otherwise it will return #PLAYER_ERROR_SEEK_FAILED.
 * @remarks Calling player_pause() or player_start() before player_seek_completed_cb() is invoked will cause #PLAYER_ERROR_INVALID_OPERATION to be returned.
 * @remarks Please note that if application is playing external media data via player_set_media_stream_info(),
 *          then consecutive calling of this function will always succeed and there is no need to wait for player_seek_completed_cb()
 *          before next calling of this function.(Since 3.0)
 * @remarks Even if you don't set visible to true by calling player_set_display_visible(),
 *          the video will be shown when the player_seek_completed_cb() is invoked.
 * @remarks In case of non-seekable content, the function will return #PLAYER_ERROR_INVALID_OPERATION
 *          and the player will keep playing without changing the play position.
 * @param[in] player        The handle to the media player
 * @param[in] milliseconds  The position in milliseconds from the start to the seek point
 * @param[in] accurate      If @c true the nearest frame position is returned, but this might be considerably slow,
 *                          if @c false the nearest key frame position is returned, this might be faster but less accurate.
 * @param[in] callback      The callback function to register
 * @param[in] user_data     The user data to be passed to the callback function
 * @return @c 0 on success,
 *         otherwise a negative error value
 * @retval #PLAYER_ERROR_NONE Successful
 * @retval #PLAYER_ERROR_INVALID_STATE Invalid player state
 * @retval #PLAYER_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #PLAYER_ERROR_INVALID_OPERATION Invalid operation
 * @retval #PLAYER_ERROR_SEEK_FAILED Seek operation failure
 * @pre The player state must be one of #PLAYER_STATE_READY, #PLAYER_STATE_PLAYING, or #PLAYER_STATE_PAUSED.
 * @post It invokes player_seek_completed_cb() when seek operation completes, if you set a callback.
 * @see player_get_play_position()
 * @see player_set_play_position_nsec()
 * @see player_get_play_position_nsec()
 */
int player_set_play_position(player_h player, int milliseconds, bool accurate, player_seek_completed_cb callback, void *user_data);

/**
 * @brief Sets the seek position in nanoseconds for playback, asynchronously.
 * @since_tizen 5.0
 * @remarks This function will trigger the seeking operation on player instance like in the case of player_set_play_position().
 *          Normally application needs to wait for player_seek_completed_cb() before calling setting position function again.
 *          Otherwise it will return #PLAYER_ERROR_SEEK_FAILED.
 * @remarks Calling player_pause() or player_start() before player_seek_completed_cb() is invoked will cause #PLAYER_ERROR_INVALID_OPERATION to be returned.
 * @remarks Please note that if application is playing external media data via player_set_media_stream_info(),
 *          then consecutive calling of this function will always succeed and there is no need to wait for player_seek_completed_cb()
 *          before next calling of this function.
 * @remarks Even if you don't set visible to true by calling player_set_display_visible(),
 *          the video will be shown when the player_seek_completed_cb() is invoked.
 * @remarks In case of non-seekable content, the function will return #PLAYER_ERROR_INVALID_OPERATION
 *          and the player will keep playing without changing the play position.
 * @param[in] player        The handle to the media player
 * @param[in] nanoseconds   The position in nanoseconds from the start to the seek point
 * @param[in] accurate      If @c true the nearest frame position is returned, but this might be considerably slow,
 *                          if @c false the nearest key frame position is returned, this might be faster but less accurate.
 * @param[in] callback      The callback function to register
 * @param[in] user_data     The user data to be passed to the callback function
 * @return @c 0 on success,
 *         otherwise a negative error value
 * @retval #PLAYER_ERROR_NONE Successful
 * @retval #PLAYER_ERROR_INVALID_STATE Invalid player state
 * @retval #PLAYER_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #PLAYER_ERROR_INVALID_OPERATION Invalid operation
 * @retval #PLAYER_ERROR_SEEK_FAILED Seek operation failure
 * @pre The player state must be one of #PLAYER_STATE_READY, #PLAYER_STATE_PLAYING, or #PLAYER_STATE_PAUSED.
 * @post It invokes player_seek_completed_cb() when seek operation completes, if you set a callback.
 * @see player_set_play_position()
 * @see player_get_play_position()
 * @see player_get_play_position_nsec()
 */
int player_set_play_position_nsec(player_h player, int64_t nanoseconds, bool accurate, player_seek_completed_cb callback, void *user_data);

/**
 * @brief Gets the current position in milliseconds.
 * @since_tizen 2.3
 * @param[in]   player        The handle to the media player
 * @param[out]  milliseconds  The current position in milliseconds
 * @return @c 0 on success,
 *         otherwise a negative error value
 * @retval #PLAYER_ERROR_NONE Successful
 * @retval #PLAYER_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #PLAYER_ERROR_INVALID_OPERATION Invalid operation
 * @retval #PLAYER_ERROR_SEEK_FAILED Seek operation failure
 * @see player_set_play_position()
 * @see player_set_play_position_nsec()
 * @see player_get_play_position_nsec()
 */
int player_get_play_position(player_h player, int *milliseconds);

/**
 * @brief Gets the current position in nanoseconds.
 * @since_tizen 5.0
 * @param[in]   player        The handle to the media player
 * @param[out]  nanoseconds   The current position in nanoseconds
 * @return @c 0 on success,
 *         otherwise a negative error value
 * @retval #PLAYER_ERROR_NONE Successful
 * @retval #PLAYER_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #PLAYER_ERROR_INVALID_OPERATION Invalid operation
 * @retval #PLAYER_ERROR_SEEK_FAILED Seek operation failure
 * @see player_set_play_position()
 * @see player_get_play_position()
 * @see player_set_play_position_nsec()
 */
int player_get_play_position_nsec(player_h player, int64_t *nanoseconds);

/**
 * @brief Sets the player's mute status.
 * @details If the mute status is @c true, no sounds are played.
 *          If it is @c false, sounds are played at the previously set volume level.
 *          Until this function is called, by default the player is not muted.
 * @since_tizen 2.3
 * @param[in]   player The handle to the media player
 * @param[in]   muted  The new mute status: (@c true = mute, @c false = not muted)
 * @return @c 0 on success,
 *         otherwise a negative error value
 * @retval #PLAYER_ERROR_NONE Successful
 * @retval #PLAYER_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #PLAYER_ERROR_INVALID_OPERATION Invalid operation
 * @see player_is_muted()
 */
int player_set_mute(player_h player, bool muted);

/**
 * @brief Gets the player's mute status.
 * @details If the mute status is @c true, no sounds are played.
 *          If it is @c false, sounds are played at the previously set volume level.
 * @since_tizen 2.3
 * @param[in]   player The handle to the media player
 * @param[out]  muted  The current mute status: (@c true = mute, @c false = not muted)
 * @return @c 0 on success,
 *         otherwise a negative error value
 * @retval #PLAYER_ERROR_NONE Successful
 * @retval #PLAYER_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #PLAYER_ERROR_INVALID_OPERATION Invalid operation
 * @see player_set_mute()
 */
int player_is_muted(player_h player, bool *muted);

/**
 * @brief Sets the player's looping status.
 * @details If the looping status is @c true, playback automatically restarts upon finishing.
 *          If it is @c false, it won't. The default value is @c false.
 * @since_tizen 2.3
 * @param[in]   player  The handle to the media player
 * @param[in]   looping The new looping status: (@c true = looping, @c false = non-looping )
 * @return @c 0 on success,
 *         otherwise a negative error value
 * @retval #PLAYER_ERROR_NONE Successful
 * @retval #PLAYER_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #PLAYER_ERROR_INVALID_OPERATION Invalid operation
 * @see player_is_looping()
 */
int player_set_looping(player_h player, bool looping);

/**
 * @brief Gets the player's looping status.
 * @details If the looping status is @c true, playback automatically restarts upon finishing.
 *          If it is @c false, it won't.
 * @since_tizen 2.3
 * @param[in]   player  The handle to the media player
 * @param[out]  looping The looping status: (@c true = looping, @c false = non-looping )
 * @return @c 0 on success,
 *         otherwise a negative error value
 * @retval #PLAYER_ERROR_NONE Successful
 * @retval #PLAYER_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #PLAYER_ERROR_INVALID_OPERATION Invalid operation
 * @see player_set_looping()
 */
int player_is_looping(player_h player, bool *looping);

/**
 * @brief Sets the video display.
 * @since_tizen 2.3
 * @remarks To get @a display to set, use #GET_DISPLAY().
 * @remarks We are not supporting changing display.
 * @remarks This function must be called before calling the player_prepare() or player_prepare_async() \n
 *          to reflect the display type.
 * @remarks This function must be called in main thread of application.
 *          Otherwise, it will return #PLAYER_ERROR_INVALID_OPERATION by internal restriction.
 *          To avoid #PLAYER_ERROR_INVALID_OPERATION in sub thread, ecore_thread_main_loop_begin() and
 *          ecore_thread_main_loop_end() can be used, but deadlock can be also occurred if main thread is busy.
 *          So, it's not recommended to use them. (since 5.0)
 * @param[in]   player  The handle to the media player
 * @param[in]   type    The display type
 * @param[in]   display The handle to display
 * @return @c 0 on success,
 *         otherwise a negative error value
 * @retval #PLAYER_ERROR_NONE Successful
 * @retval #PLAYER_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #PLAYER_ERROR_INVALID_OPERATION Invalid operation
 * @retval #PLAYER_ERROR_INVALID_STATE Invalid player state
 * @pre The player state must be set to #PLAYER_STATE_IDLE by calling player_create() or player_unprepare().
 * @see #player_display_type_e
 * @see player_set_display_mode()
 * @see player_set_display_roi_area()
 * @see player_set_display_visible()
 * @see player_set_display_rotation()
 * @see ecore_thread_main_loop_begin()
 * @see ecore_thread_main_loop_end()
 */
int player_set_display(player_h player, player_display_type_e type, player_display_h display);

/**
 * @brief Sets a callback function for getting the decoded video frame.
 * @since_tizen 2.3
 * @remarks This function must be called before calling player_prepare() or player_prepare_async().\n
 *          A registered callback is called in a separate thread (not in the main loop).\n
 *          A video frame can be retrieved using a registered callback as a media packet.\n
 *          The callback function holds the same buffer that will be drawn on the display device.\n
 *          So if you change the media packet in a registered callback, it will be displayed on the device\n
 *          and the media packet is available until it's destroyed by media_packet_destroy().\n
 *          The packet have to be destroyed as quickly as possible after rendering the packet\n
 *          and all the packets have to be destroyed before player_unprepare() is called.\n
 * @remarks If the content is encrypted or there are copyright issues with it,
 *          this function could be unsupported depending on the DRM policy.
 * @param[in] player    The handle to the media player
 * @param[in] callback  The callback function to be registered
 * @param[in] user_data The user data to be passed to the callback function
 * @return @c 0 on success,
 *         otherwise a negative error value
 * @retval #PLAYER_ERROR_NONE Successful
 * @retval #PLAYER_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #PLAYER_ERROR_INVALID_STATE Invalid state
 * @pre The player's state must be #PLAYER_STATE_IDLE. And, #PLAYER_DISPLAY_TYPE_NONE must be set by calling player_set_display().
 * @see player_unset_media_packet_video_frame_decoded_cb()
 */
int player_set_media_packet_video_frame_decoded_cb(player_h player, player_media_packet_video_decoded_cb callback, void *user_data);

/**
 * @brief Unsets the callback notifying the decoded video frame.
 * @since_tizen 2.3
 * @param[in] player The handle to the media player
 * @return @c 0 on success,
 *         otherwise a negative error value
 * @retval #PLAYER_ERROR_NONE Successful
 * @retval #PLAYER_ERROR_INVALID_PARAMETER Invalid parameter
 * @pre The player's state must be #PLAYER_STATE_READY or #PLAYER_STATE_IDLE
 * @see player_set_media_packet_video_frame_decoded_cb()
 */
int player_unset_media_packet_video_frame_decoded_cb(player_h player);

/**
 * @brief Sets a callback function for getting the decoded audio data.
 * @details This function is used to get audio PCM data of input media content via registered callback.\n
 *          An application can specify the output PCM format by @ref CAPI_MEDIA_TOOL_MEDIA_FORMAT_MODULE API.
 * @since_tizen 5.5
 * @remarks This function must be called before calling player_prepare() or player_prepare_async().\n
 *          A registered callback is called in a separate thread (not in the main loop).\n
 *          The audio PCM data can be retrieved using a registered callback as a media packet
 *          and it is available until it's destroyed by media_packet_destroy().\n
 *          The packet has to be destroyed as quickly as possible after rendering the data\n
 *          and all the packets have to be destroyed before player_unprepare() is called.\n
 * @remarks If audio offload is enabled by calling player_audio_offload_set_enabled(),
 *          this function will return #PLAYER_ERROR_NOT_AVAILABLE and
 *          this will not work at all even if this was called before enabling offload.
 * @remarks This function could be unavailable depending on the audio codec type and
 *          this will return #PLAYER_ERROR_NOT_AVAILABLE.
 * @remarks If the content is encrypted or there are copyright issues with it,
 *          this function could be unsupported depending on the DRM policy.
 * @param[in] player     The handle to the media player
 * @param[in] format     The media format handle about required audio PCM specification.
 *                       This format has to include PCM MIME type, audio channel and sampling rate.
 *                       If the format is NULL, the original PCM format or platform default PCM format will be applied.
 * @param[in] opt        The audio extract option
 * @param[in] callback   The callback function to be registered
 * @param[in] user_data  The user data to be passed to the callback function
 * @return @c 0 on success,
 *         otherwise a negative error value
 * @retval #PLAYER_ERROR_NONE Successful
 * @retval #PLAYER_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #PLAYER_ERROR_INVALID_STATE Invalid state
 * @retval #PLAYER_ERROR_INVALID_OPERATION Invalid operation
 * @retval #PLAYER_ERROR_NOT_AVAILABLE Not available
 * @pre The player's state must be #PLAYER_STATE_IDLE.
 * @see player_unset_media_packet_audio_frame_decoded_cb()
 * @see player_audio_offload_set_enabled()
 * @see player_audio_offload_is_enabled()
 * @see player_set_audio_codec_type()
 * @see player_get_audio_codec_type()
 * @par Example
   @code
      #include <player.h>
      #include <media_format.h>
       ...
       player_h player = NULL;
       media_format_h a_format = NULL;
       ...
       media_format_create(&a_format);
       media_format_set_audio_mime(a_format, MEDIA_FORMAT_PCM_F32LE);
       media_format_set_audio_channel(a_format, 2);
       media_format_set_audio_samplerate(a_format, 44100);

       player_set_media_packet_audio_frame_decoded_cb(player, a_format, PLAYER_AUDIO_EXTRACT_DEFAULT, _audio_pcm_cb, udata);

       media_format_unref(a_format);
       ...
   @endcode
 */
int player_set_media_packet_audio_frame_decoded_cb(player_h player, media_format_h format,
		player_audio_extract_option_e opt, player_media_packet_audio_decoded_cb callback, void *user_data);

/**
 * @brief Unsets the callback notifying the decoded audio data.
 * @since_tizen 5.5
 * @remarks Depending on the audio codec type or by enabling audio offload,
 *          this function could be unavailable and this will return #PLAYER_ERROR_NOT_AVAILABLE.
 * @param[in] player The handle to the media player
 * @return @c 0 on success,
 *         otherwise a negative error value
 * @retval #PLAYER_ERROR_NONE Successful
 * @retval #PLAYER_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #PLAYER_ERROR_INVALID_STATE Invalid state
 * @retval #PLAYER_ERROR_NOT_AVAILABLE Not available
 * @pre The player's state must be #PLAYER_STATE_READY or #PLAYER_STATE_IDLE
 * @see player_set_media_packet_audio_frame_decoded_cb()
 * @see player_audio_offload_set_enabled()
 * @see player_audio_offload_is_enabled()
 * @see player_set_audio_codec_type()
 * @see player_get_audio_codec_type()
 */
int player_unset_media_packet_audio_frame_decoded_cb(player_h player);

/**
 * @brief  Pushes elementary stream to decode audio or video.
 * @since_tizen 2.4
 * @remarks player_set_media_stream_info() must be called before using this function.
 * @remarks The available buffer size can be set by calling player_set_media_stream_buffer_max_size().
 *          If there is no available buffer space, this function will return error since 3.0.
 * @param[in]  player   The handle to media player
 * @param[in]  packet   The media packet to decode
 * @return @c 0 on success,
 *         otherwise a negative error value
 * @retval #PLAYER_ERROR_NONE Successful
 * @retval #PLAYER_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #PLAYER_ERROR_INVALID_STATE Invalid state
 * @retval #PLAYER_ERROR_NOT_SUPPORTED_FILE File not supported
 * @retval #PLAYER_ERROR_BUFFER_SPACE No buffer space available (since 3.0)
 * @retval #PLAYER_ERROR_INVALID_OPERATION Invalid operation (since 3.0)
 * @pre The player state must be one of #PLAYER_STATE_READY, #PLAYER_STATE_PLAYING or #PLAYER_STATE_PAUSED.
 * @see player_set_media_stream_info()
 * @see player_set_media_stream_buffer_max_size()
 */
int player_push_media_stream(player_h player, media_packet_h packet);

/**
 * @brief Retrieves all supported media formats for the playback of external media stream.
 * @details The supported media format can vary depending on the device capabilities.
 * @since_tizen 5.5
 * @param[in] player      The handle to the media player
 * @param[in] callback    The iteration callback function
 * @param[in] user_data   The user data to be passed to the callback function
 * @return @c 0 on success,
 *         otherwise a negative error value
 * @retval #PLAYER_ERROR_NONE Successful
 * @retval #PLAYER_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #PLAYER_ERROR_INVALID_OPERATION Invalid operation
 * @see player_supported_media_format_cb()
 * @see player_set_media_stream_info()
 * @see player_push_media_stream()
 */
int player_foreach_media_stream_supported_format(player_h player, player_supported_media_format_cb callback, void *user_data);

/**
 * @brief  Sets contents information for media stream.
 * @since_tizen 2.4
 * @remarks AV format must be set before pushing elementary stream with player_push_media_stream().
 * @remarks This function must be called before calling the player_prepare() or player_prepare_async()
 *          to reflect the media information.
 * @remarks The supported media format MIME type can be checked
 *          by calling player_foreach_media_stream_supported_format(). (Since 5.5)
 * @param[in] player The handle to media player
 * @param[in] type   The type of target stream
 * @param[in] format The media format to set media information
 * @return @c 0 on success,
 *         otherwise a negative error value
 * @retval #PLAYER_ERROR_NONE Successful
 * @retval #PLAYER_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #PLAYER_ERROR_INVALID_STATE Invalid state
 * @retval #PLAYER_ERROR_INVALID_OPERATION Invalid operation
 * @retval #PLAYER_ERROR_NOT_SUPPORTED_FORMAT Not supported format (Since 5.5)
 * @pre The player state must be set to #PLAYER_STATE_IDLE by calling player_create() or player_unprepare().
 * @see player_push_media_stream()
 * @see player_foreach_media_stream_supported_format()
 */
int player_set_media_stream_info(player_h player, player_stream_type_e type, media_format_h format);

/**
 * @brief Sets a callback function to be invoked when buffer underrun or overflow is occurred.
 * @since_tizen 2.4
 * @remarks This function is used for media stream playback only.
 * @remarks The callback is called in a separate thread (not in the main loop).
 * @param[in] player    The handle to the media player
 * @param[in] type      The type of target stream
 * @param[in] callback  The buffer status callback function to register
 * @param[in] user_data The user data to be passed to the callback function
 * @return @c 0 on success,
 *         otherwise a negative error value
 * @retval #PLAYER_ERROR_NONE Successful
 * @retval #PLAYER_ERROR_INVALID_STATE Invalid player state
 * @retval #PLAYER_ERROR_INVALID_PARAMETER Invalid parameter
 * @pre The player state must be set to #PLAYER_STATE_IDLE by calling player_create() or player_unprepare().
 * @post player_media_stream_buffer_status_cb() will be invoked.
 * @see player_unset_media_stream_buffer_status_cb()
 * @see player_media_stream_buffer_status_cb()
 */
int player_set_media_stream_buffer_status_cb(player_h player, player_stream_type_e type, player_media_stream_buffer_status_cb callback, void *user_data);

/**
 * @brief Unsets the buffer status callback function.
 * @since_tizen 2.4
 * @remarks This function is used for media stream playback only.
 * @param[in] player The handle to the media player
 * @param[in] type   The type of target stream
 * @return @c 0 on success,
 *         otherwise a negative error value
 * @retval #PLAYER_ERROR_NONE Successful
 * @retval #PLAYER_ERROR_INVALID_PARAMETER Invalid parameter
 * @see player_set_media_stream_buffer_status_cb()
 */
int player_unset_media_stream_buffer_status_cb(player_h player, player_stream_type_e type);

/**
 * @brief Sets a callback function to be invoked when seeking is occurred.
 * @since_tizen 2.4
 * @remarks This function is used for media stream playback only.
 * @remarks The callback is called in a separate thread (not in the main loop).
 * @param[in] player    The handle to the media player
 * @param[in] type      The type of target stream
 * @param[in] callback  The callback function to register
 * @param[in] user_data The user data to be passed to the callback function
 * @return @c 0 on success,
 *         otherwise a negative error value
 * @retval #PLAYER_ERROR_NONE Successful
 * @retval #PLAYER_ERROR_INVALID_STATE Invalid player state
 * @retval #PLAYER_ERROR_INVALID_PARAMETER Invalid parameter
 * @pre The player state must be set to #PLAYER_STATE_IDLE by calling player_create() or player_unprepare().
 * @post player_media_stream_seek_cb() will be invoked.
 * @see player_unset_media_stream_seek_cb()
 * @see player_media_stream_seek_cb()
 */
int player_set_media_stream_seek_cb(player_h player, player_stream_type_e type, player_media_stream_seek_cb callback, void *user_data);

/**
 * @brief Unsets the seek callback function.
 * @since_tizen 2.4
 * @param[in] player The handle to the media player
 * @param[in] type   The type of target stream
 * @return @c 0 on success,
 *         otherwise a negative error value
 * @retval #PLAYER_ERROR_NONE Successful
 * @retval #PLAYER_ERROR_INVALID_PARAMETER Invalid parameter
 * @see player_set_media_stream_seek_cb()
 */
int player_unset_media_stream_seek_cb(player_h player, player_stream_type_e type);

/**
 * @brief Sets the max size bytes of buffer.
 * @since_tizen 2.4
 * @remarks This function is used for media stream playback only.
 * @remarks If the buffer level over the max size, player_media_stream_buffer_status_cb() will be invoked with overflow status.
 * @param[in] player   The handle to the media player
 * @param[in] type     The type of target stream
 * @param[in] max_size The max bytes of buffer, it has to be bigger than zero. (default: 200000)
 * @return @c 0 on success,
 *         otherwise a negative error value
 * @retval #PLAYER_ERROR_NONE Successful
 * @retval #PLAYER_ERROR_INVALID_PARAMETER Invalid parameter
 * @see player_get_media_stream_buffer_max_size()
 * @see player_media_stream_buffer_status_cb()
 */
int player_set_media_stream_buffer_max_size(player_h player, player_stream_type_e type, unsigned long long max_size);

/**
 * @brief Gets the max size bytes of buffer.
 * @since_tizen 2.4
 * @remarks This function is used for media stream playback only.
 * @remarks If the buffer level over the max size, player_media_stream_buffer_status_cb() will be invoked with overflow status.
 * @param[in] player    The handle to the media player
 * @param[in] type      The type of target stream
 * @param[out] max_size The max bytes of buffer
 * @return @c 0 on success,
 *         otherwise a negative error value
 * @retval #PLAYER_ERROR_NONE Successful
 * @retval #PLAYER_ERROR_INVALID_PARAMETER Invalid parameter
 * @see player_set_media_stream_buffer_max_size()
 * @see player_media_stream_buffer_status_cb()
 */
int player_get_media_stream_buffer_max_size(player_h player, player_stream_type_e type, unsigned long long *max_size);

/**
 * @brief Sets the buffer threshold percent of buffer.
 * @since_tizen 2.4
 * @remarks This function is used for media stream playback only.
 * @remarks If the buffer level drops below the percent value, player_media_stream_buffer_status_cb() will be invoked with underrun status.
 * @param[in] player  The handle to the media player
 * @param[in] type    The type of target stream
 * @param[in] percent The minimum threshold(0~100) of buffer (default: 0)
 * @return @c 0 on success,
 *         otherwise a negative error value
 * @retval #PLAYER_ERROR_NONE Successful
 * @retval #PLAYER_ERROR_INVALID_PARAMETER Invalid parameter
 * @see player_get_media_stream_buffer_min_threshold()
 * @see player_media_stream_buffer_status_cb()
 */
int player_set_media_stream_buffer_min_threshold(player_h player, player_stream_type_e type, unsigned int percent);

/**
 * @brief Gets the buffer threshold percent of buffer.
 * @since_tizen 2.4
 * @remarks This function is used for media stream playback only.
 * @remarks If the buffer level drops below the percent value, player_media_stream_buffer_status_cb() will be invoked with underrun status.
 * @param[in] player   The handle to the media player
 * @param[in] type     The type of target stream
 * @param[out] percent The minimum threshold(0~100) of buffer
 * @return @c 0 on success,
 *         otherwise a negative error value
 * @retval #PLAYER_ERROR_NONE Successful
 * @retval #PLAYER_ERROR_INVALID_PARAMETER Invalid parameter
 * @see player_set_media_stream_buffer_min_threshold()
 * @see player_media_stream_buffer_status_cb()
 */
int player_get_media_stream_buffer_min_threshold(player_h player, player_stream_type_e type, unsigned int *percent);

/**
 * @}
 */

/**
 * @addtogroup CAPI_MEDIA_PLAYER_DISPLAY_MODULE
 * @{
 */

/**
 * @brief Sets the video display mode.
 * @since_tizen 2.3
 * @remarks If no display is set, no operation is performed.
 * @param[in] player   The handle to the media player
 * @param[in] mode     The display mode
 * @return @c 0 on success,
 *         otherwise a negative error value
 * @retval #PLAYER_ERROR_NONE Successful
 * @retval #PLAYER_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #PLAYER_ERROR_INVALID_OPERATION Invalid operation
 * @retval #PLAYER_ERROR_NOT_AVAILABLE Not available (Since 6.5)
 * @see #player_display_mode_e
 * @see player_set_display()
 * @see player_get_display_mode()
 */
int player_set_display_mode(player_h player, player_display_mode_e mode);

/**
 * @brief Gets the video display mode.
 * @since_tizen 2.3
 * @remarks If no display is set, no operation is performed.
 * @param[in] player The handle to the media player
 * @param[out] mode  The current display mode
 * @return @c 0 on success,
 *         otherwise a negative error value
 * @retval  #PLAYER_ERROR_NONE Successful
 * @retval  #PLAYER_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval  #PLAYER_ERROR_INVALID_OPERATION Invalid operation
 * @retval  #PLAYER_ERROR_NOT_AVAILABLE Not available (Since 6.5)
 * @see #player_display_mode_e
 * @see player_set_display_mode()
 */
int player_get_display_mode(player_h player, player_display_mode_e *mode);

/**
 * @brief Sets the ROI (Region Of Interest) area of display.
 * @since_tizen 3.0
 * @remarks If no display is set, no operation is performed and
 *          the ROI area is valid only in #PLAYER_DISPLAY_MODE_DST_ROI display mode.
 * @remarks The minimum value of width and height are 1.
 * @remarks ROI area can be set before setting ROI display mode. (since 4.0)
 * @param[in] player The handle to the media player
 * @param[in] x      X coordinate of area
 * @param[in] y      Y coordinate of area
 * @param[in] width  Width of area
 * @param[in] height Height of area
 * @return @c 0 on success,
 *         otherwise a negative error value
 * @retval  #PLAYER_ERROR_NONE Successful
 * @retval  #PLAYER_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval  #PLAYER_ERROR_INVALID_OPERATION Invalid operation
 * @retval  #PLAYER_ERROR_NOT_AVAILABLE Not available (Since 6.5)
 * @see player_set_display()
 * @see player_set_display_mode()
 */
int player_set_display_roi_area(player_h player, int x, int y, int width, int height);

/**
 * @brief Sets the visibility of the video display.
 * @since_tizen 2.3
 * @remarks If no display is set, no operation is performed.
 * @remarks If you set visible before calling player_set_display(), it will be applied on #PLAYER_STATE_READY state.
 * @param[in] player   The handle to the media player
 * @param[in] visible  The visibility of the display (@c true = visible, @c false = non-visible )
 * @return @c 0 on success,
 *         otherwise a negative error value
 * @retval #PLAYER_ERROR_NONE Successful
 * @retval #PLAYER_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #PLAYER_ERROR_INVALID_OPERATION Invalid operation
 * @see player_set_display()
 * @see player_is_display_visible()
 */
int player_set_display_visible(player_h player, bool visible);

/**
 * @brief Gets the visibility of the video display.
 * @since_tizen 2.3
 * @param[in] player   The handle to the media player
 * @param[out] visible The current visibility of the display (@c true = visible, @c false = non-visible )
 * @return @c 0 on success,
 *         otherwise a negative error value
 * @retval  #PLAYER_ERROR_NONE Successful
 * @retval  #PLAYER_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval  #PLAYER_ERROR_INVALID_OPERATION Invalid operation
 * @see player_set_display_visible()
 */
int player_is_display_visible(player_h player, bool *visible);

/**
 * @brief Sets the rotation settings of the video surface display.
 * @details Use this function to change the video orientation to portrait mode.
 *          The video out will be rotated in a counterclockwise direction.
 * @since_tizen 2.3
 * @remarks If no display is set, no operation is performed.
 * @param[in] player   The handle to the media player
 * @param[in] rotation The rotation of the display
 * @return @c 0 on success,
 *         otherwise a negative error value
 * @retval #PLAYER_ERROR_NONE Successful
 * @retval #PLAYER_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #PLAYER_ERROR_INVALID_OPERATION Invalid operation
 * @retval #PLAYER_ERROR_NOT_AVAILABLE Not available (Since 6.5)
 * @see #player_display_rotation_e
 * @see  player_set_display()
 * @see  player_get_display_rotation()
 */
int player_set_display_rotation(player_h player, player_display_rotation_e rotation);

/**
 * @brief Gets the rotation of the video surface display.
 * @since_tizen 2.3
 * @param[in] player    The handle to the media player
 * @param[out] rotation The current rotation of the display
 * @return @c 0 on success,
 *         otherwise a negative error value
 * @retval  #PLAYER_ERROR_NONE Successful
 * @retval  #PLAYER_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval  #PLAYER_ERROR_INVALID_OPERATION Invalid operation
 * @retval  #PLAYER_ERROR_NOT_AVAILABLE Not available (Since 6.5)
 * @see     #player_display_rotation_e
 * @see     player_set_display_rotation()
 */
int player_get_display_rotation(player_h player, player_display_rotation_e *rotation);

/**
 * @}
 */


/**
 * @addtogroup CAPI_MEDIA_PLAYER_STREAM_INFO_MODULE
 * @{
 */

 /**
 * @brief Gets the media content information.
 * @since_tizen 2.3
 * @remarks You must release @a value using free().
 * @remarks The playback type should be local playback or HTTP streaming playback.
 * @param[in] player  The handle to the media player
 * @param[in] key     The key attribute name to get
 * @param[out] value  The value of the key attribute \n
 *                   It can be an empty string if there is no content information.
 * @return @c 0 on success,
 *         otherwise a negative error value
 * @retval #PLAYER_ERROR_NONE Successful
 * @retval #PLAYER_ERROR_INVALID_PARAMETER  Invalid parameter
 * @retval #PLAYER_ERROR_OUT_OF_MEMORY Not enough memory is available
 * @retval #PLAYER_ERROR_INVALID_OPERATION Invalid operation
 * @retval #PLAYER_ERROR_INVALID_STATE Invalid player state
 * @pre The player state must be one of #PLAYER_STATE_READY, #PLAYER_STATE_PLAYING or #PLAYER_STATE_PAUSED.
 */
int player_get_content_info(player_h player, player_content_info_e key, char **value);

/**
 * @brief Gets the audio and video codec information.
 * @since_tizen 2.3
 * @remarks You must release @a audio_codec and @a video_codec using free().
 * @remarks The playback type should be local playback or HTTP streaming playback.
 * @param[in] player       The handle to the media player
 * @param[out] audio_codec The name of the audio codec \n
 *                         It can be @c NULL if there is no audio codec.
 * @param[out] video_codec The name of the video codec \n
 *                         It can be @c NULL if there is no video codec.
 * @return @c 0 on success,
 *         otherwise a negative error value
 * @retval #PLAYER_ERROR_NONE Successful
 * @retval #PLAYER_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #PLAYER_ERROR_INVALID_OPERATION Invalid operation
 * @retval #PLAYER_ERROR_INVALID_STATE Invalid player state
 * @pre The player state must be one of #PLAYER_STATE_READY, #PLAYER_STATE_PLAYING or #PLAYER_STATE_PAUSED.
 */
int player_get_codec_info(player_h player, char **audio_codec, char **video_codec);

/**
 * @brief Gets the audio stream information.
 * @since_tizen 2.3
 * @remarks The playback type should be local playback or HTTP streaming playback.
 * @param[in] player       The handle to the media player
 * @param[out] sample_rate The audio sample rate [Hz] \n
 *                         Value can be invalid if there is no audio stream information.
 * @param[out] channel     The audio channel (1: mono, 2: stereo) \n
 *                         Value can be invalid if there is no audio stream information.
 * @param[out] bit_rate    The audio bit rate [Hz] \n
 *                         Value can be invalid if there is no audio stream information.
 * @return @c 0 on success,
 *         otherwise a negative error value
 * @retval #PLAYER_ERROR_NONE Successful
 * @retval #PLAYER_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #PLAYER_ERROR_INVALID_OPERATION Invalid operation
 * @retval #PLAYER_ERROR_INVALID_STATE Invalid player state
 * @pre The player state must be one of #PLAYER_STATE_READY, #PLAYER_STATE_PLAYING or #PLAYER_STATE_PAUSED.
 */
int player_get_audio_stream_info(player_h player, int *sample_rate, int *channel, int *bit_rate);

/**
 * @brief Gets the video stream information.
 * @since_tizen 2.3
 * @remarks The playback type should be local playback or HTTP streaming playback.
 * @param[in] player     The handle to the media player
 * @param[out]  fps      The frame per second of the video \n
 *                       It can be @c 0 if there is no video stream information.
 * @param[out]  bit_rate The video bit rate [Hz] \n
 *                       It can be an invalid value if there is no video stream information.
 * @return @c 0 on success,
 *         otherwise a negative error value
 * @retval #PLAYER_ERROR_NONE Successful
 * @retval #PLAYER_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #PLAYER_ERROR_INVALID_OPERATION Invalid operation
 * @retval #PLAYER_ERROR_INVALID_STATE Invalid player state
 * @pre The player state must be one of #PLAYER_STATE_READY, #PLAYER_STATE_PLAYING or #PLAYER_STATE_PAUSED.
 */
int player_get_video_stream_info(player_h player, int *fps, int *bit_rate);

/**
 * @brief Gets the video display's height and width.
 * @since_tizen 2.3
 * @remarks The playback type should be local playback or HTTP streaming playback.
 * @param[in] player  The handle to the media player
 * @param[out] width  The width of the video \n
 *                    Value can be invalid if there is no video or no display is set.
 * @param[out] height The height of the video \n
 *                    Value can be invalid value if there is no video or no display is set.
 * @return @c 0 on success,
 *         otherwise a negative error value
 * @retval #PLAYER_ERROR_NONE Successful
 * @retval #PLAYER_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #PLAYER_ERROR_INVALID_OPERATION Invalid operation
 * @retval #PLAYER_ERROR_INVALID_STATE Invalid player state
 * @pre The player state must be one of #PLAYER_STATE_READY, #PLAYER_STATE_PLAYING or #PLAYER_STATE_PAUSED.
 */
int player_get_video_size(player_h player, int *width, int *height);

/**
 * @brief Gets the album art in the media resource.
 * @since_tizen 2.3
 * @remarks You must not release @a album_art.
 *          The @a album_art is managed by the platform and will be released
 *          when the player is unprepared or destroyed by calling player_unprepare() or player_destroy().
 * @param[in] player     The handle to the media player
 * @param[out] album_art The encoded artwork image
 * @param[out] size      The encoded artwork size
 * @return @c 0 on success,
 *         otherwise a negative error value
 * @retval #PLAYER_ERROR_NONE Successful
 * @retval #PLAYER_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #PLAYER_ERROR_INVALID_OPERATION Invalid operation
 * @retval #PLAYER_ERROR_INVALID_STATE Invalid player state
 * @pre The player state must be one of #PLAYER_STATE_READY, #PLAYER_STATE_PLAYING or #PLAYER_STATE_PAUSED.
 */
int player_get_album_art(player_h player, void **album_art, int *size);

/**
 * @brief Gets the total running time of the associated media.
 * @since_tizen 2.3
 * @remarks The media source is associated with the player, using either player_set_uri() or player_set_memory_buffer().
 * @remarks The playback type should be local playback or HTTP streaming playback.
 * @param[in]   player         The handle to the media player
 * @param[out]  milliseconds   The duration in milliseconds
 * @return @c 0 on success,
 *         otherwise a negative error value
 * @retval #PLAYER_ERROR_NONE Successful
 * @retval #PLAYER_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #PLAYER_ERROR_INVALID_OPERATION Invalid operation
 * @retval #PLAYER_ERROR_INVALID_STATE Invalid player state
 * @pre The player state must be one of #PLAYER_STATE_READY, #PLAYER_STATE_PLAYING or #PLAYER_STATE_PAUSED.
 * @see player_set_uri()
 * @see player_set_memory_buffer()
 * @see player_get_duration_nsec()
 */
int player_get_duration(player_h player, int *milliseconds);

/**
 * @brief Gets the total running time in nanoseconds of the associated media.
 * @since_tizen 5.0
 * @param[in]   player         The handle to the media player
 * @param[out]  nanoseconds    The duration time in nanoseconds
 * @return @c 0 on success,
 *         otherwise a negative error value
 * @retval #PLAYER_ERROR_NONE Successful
 * @retval #PLAYER_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #PLAYER_ERROR_INVALID_OPERATION Invalid operation
 * @retval #PLAYER_ERROR_INVALID_STATE Invalid player state
 * @pre The player state must be one of #PLAYER_STATE_READY, #PLAYER_STATE_PLAYING or #PLAYER_STATE_PAUSED.
 * @see player_set_uri()
 * @see player_set_memory_buffer()
 * @see player_get_duration()
 */
int player_get_duration_nsec(player_h player, int64_t *nanoseconds);

/**
 * @}
 */


/**
 * @addtogroup CAPI_MEDIA_PLAYER_AUDIO_EFFECT_MODULE
 * @{
 */

/**
 * @brief Gets the number of equalizer bands.
 * @since_tizen 2.3
 * @remarks If player_audio_effect_equalizer_is_available() returns @a available parameter as @c false,
 *          this function will return #PLAYER_ERROR_NOT_AVAILABLE. (Since 5.5)
 * @param[in] player The handle to the media player
 * @param[out] count The number of equalizer bands
 * @return @c 0 on success,
 *         otherwise a negative error value
 * @retval #PLAYER_ERROR_NONE Successful
 * @retval #PLAYER_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #PLAYER_ERROR_NOT_AVAILABLE Not available (Since 5.5)
 * @see player_audio_effect_equalizer_is_available()
 * @see player_audio_effect_set_equalizer_band_level()
 * @see player_audio_effect_set_equalizer_all_bands()
 */
int player_audio_effect_get_equalizer_bands_count(player_h player, int *count);

/**
 * @brief Sets the gain set for the given equalizer band.
 * @since_tizen 2.3
 * @remarks If player_audio_effect_equalizer_is_available() returns @a available parameter as @c false,
 *          this function will return #PLAYER_ERROR_NOT_AVAILABLE and
 *          this will not work at all even if it was called before enabling audio offload
 *          which makes audio effect function group unavailable. (Since 5.5)
 * @param[in] player The handle to the media player
 * @param[in] index  The index of the equalizer band to be set
 * @param[in] level  The new gain in decibel that is set to the given band [dB]
 * @return @c 0 on success,
 *         otherwise a negative error value
 * @retval #PLAYER_ERROR_NONE Successful
 * @retval #PLAYER_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #PLAYER_ERROR_INVALID_OPERATION Invalid operation
 * @retval #PLAYER_ERROR_NOT_AVAILABLE Not available (Since 5.5)
 * @see player_audio_effect_equalizer_is_available()
 * @see player_audio_effect_get_equalizer_bands_count()
 * @see player_audio_effect_get_equalizer_level_range()
 * @see player_audio_effect_get_equalizer_band_level()
 * @see player_audio_effect_set_equalizer_all_bands()
 * @see player_audio_offload_set_enabled()
 */
int player_audio_effect_set_equalizer_band_level(player_h player, int index, int level);

/**
 * @brief Gets the gain set for the given equalizer band.
 * @since_tizen 2.3
 * @remarks If player_audio_effect_equalizer_is_available() returns @a available parameter as @c false,
 *          this function will return #PLAYER_ERROR_NOT_AVAILABLE. (Since 5.5)
 * @param[in] player The handle to the media player
 * @param[in] index  The index of the requested equalizer band
 * @param[out] level The gain in decibel of the given band [dB]
 * @return @c 0 on success,
 *         otherwise a negative error value
 * @retval #PLAYER_ERROR_NONE Successful
 * @retval #PLAYER_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #PLAYER_ERROR_NOT_AVAILABLE Not available (Since 5.5)
 * @see player_audio_effect_equalizer_is_available()
 * @see player_audio_effect_set_equalizer_band_level()
 */
int player_audio_effect_get_equalizer_band_level(player_h player, int index, int *level);

/**
 * @brief Sets all bands of the equalizer.
 * @since_tizen 2.3
 * @remarks If player_audio_effect_equalizer_is_available() returns @a available parameter as @c false,
 *          this function will return #PLAYER_ERROR_NOT_AVAILABLE and
 *          this will not work at all even if it was called before enabling audio offload
 *          which makes audio effect function group unavailable. (Since 5.5)
 * @param[in] player      The handle to the media player
 * @param[in] band_levels The list of band levels to be set
 * @param[in] length      The length of the band level
 * @return @c 0 on success,
 *         otherwise a negative error value
 * @retval #PLAYER_ERROR_NONE Successful
 * @retval #PLAYER_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #PLAYER_ERROR_INVALID_OPERATION Invalid operation
 * @retval #PLAYER_ERROR_NOT_AVAILABLE Not available (Since 5.5)
 * @see player_audio_effect_equalizer_is_available()
 * @see player_audio_effect_get_equalizer_bands_count()
 * @see player_audio_effect_get_equalizer_level_range()
 * @see player_audio_effect_set_equalizer_band_level()
 * @see player_audio_offload_set_enabled()
 */
int player_audio_effect_set_equalizer_all_bands(player_h player, int *band_levels, int length);

/**
 * @brief Gets the valid band level range of the equalizer.
 * @since_tizen 2.3
 * @remarks If player_audio_effect_equalizer_is_available() returns @a available parameter as @c false,
 *          this function will return #PLAYER_ERROR_NOT_AVAILABLE. (Since 5.5)
 * @param[in] player The handle to the media player
 * @param[out] min   The minimum value to be set [dB]
 * @param[out] max   The maximum value to be set [dB]
 * @return @c 0 on success,
 *         otherwise a negative error value
 * @retval #PLAYER_ERROR_NONE Successful
 * @retval #PLAYER_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #PLAYER_ERROR_INVALID_OPERATION Invalid operation
 * @retval #PLAYER_ERROR_NOT_AVAILABLE Not available (Since 5.5)
 * @see player_audio_effect_equalizer_is_available()
 * @see player_audio_effect_set_equalizer_band_level()
 * @see player_audio_effect_set_equalizer_all_bands()
 */
int player_audio_effect_get_equalizer_level_range(player_h player, int *min, int *max);

/**
 * @brief Gets the band frequency of the equalizer.
 * @since_tizen 2.3
 * @remarks If player_audio_effect_equalizer_is_available() returns @a available parameter as @c false,
 *          this function will return #PLAYER_ERROR_NOT_AVAILABLE. (Since 5.5)
 * @param[in] player     The handle to the media player
 * @param[in]  index     The index of the requested equalizer band
 * @param[out] frequency The frequency of the given band [dB]
 * @return @c 0 on success,
 *         otherwise a negative error value
 * @retval #PLAYER_ERROR_NONE Successful
 * @retval #PLAYER_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #PLAYER_ERROR_NOT_AVAILABLE Not available (Since 5.5)
 * @see player_audio_effect_equalizer_is_available()
 */
int player_audio_effect_get_equalizer_band_frequency(player_h player, int index, int *frequency);

/**
 * @brief Gets the band frequency range of the equalizer.
 * @since_tizen 2.3
 * @remarks If player_audio_effect_equalizer_is_available() returns @a available parameter as @c false,
 *          this function will return #PLAYER_ERROR_NOT_AVAILABLE. (Since 5.5)
 * @param[in] player The handle to the media player
 * @param[in]  index The index of the requested equalizer band
 * @param[out] range The frequency range of the given band [dB]
 * @return @c 0 on success,
 *         otherwise a negative error value
 * @retval #PLAYER_ERROR_NONE Successful
 * @retval #PLAYER_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #PLAYER_ERROR_NOT_AVAILABLE Not available (Since 5.5)
 * @see player_audio_effect_equalizer_is_available()
 */
int player_audio_effect_get_equalizer_band_frequency_range(player_h player, int index, int *range);

/**
 * @brief Clears the equalizer effect.
 * @since_tizen 2.3
 * @remarks If player_audio_effect_equalizer_is_available() returns @a available parameter as @c false,
 *          this function will return #PLAYER_ERROR_NOT_AVAILABLE. (Since 5.5)
 * @param[in] player The handle to the media player
 * @return @c 0 on success,
 *         otherwise a negative error value
 * @retval #PLAYER_ERROR_NONE Successful
 * @retval #PLAYER_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #PLAYER_ERROR_INVALID_OPERATION Invalid operation
 * @retval #PLAYER_ERROR_NOT_AVAILABLE Not available (Since 5.5)
 * @see player_audio_effect_equalizer_is_available()
 * @see player_audio_effect_set_equalizer_band_level()
 * @see player_audio_effect_set_equalizer_all_bands()
 */
int player_audio_effect_equalizer_clear(player_h player);

/**
 * @brief Checks whether the custom equalizer effect is available.
 * @details This function returns the availability of the audio effect function group and
 *          it could be unavailable depending on the platform capabilities.
 * @since_tizen 2.3
 * @remarks If audio offload is enabled by calling player_audio_offload_set_enabled(),
 *          @a available will be @c false.
 * @param[in]  player    The handle to the media player
 * @param[out] available If @c true the specified audio effect is available,
 *                       otherwise @c false
 * @return @c 0 on success,
 *         otherwise a negative error value
 * @retval #PLAYER_ERROR_NONE Successful
 * @retval #PLAYER_ERROR_INVALID_PARAMETER Invalid parameter
 * @see player_audio_effect_set_equalizer_band_level()
 * @see player_audio_effect_set_equalizer_all_bands()
 * @see player_audio_offload_set_enabled()
 * @see player_audio_offload_is_enabled()
 * @see player_set_audio_codec_type()
 * @see player_get_audio_codec_type()
 */
int player_audio_effect_equalizer_is_available(player_h player, bool *available);

/**
 * @}
 */


/**
 * @addtogroup CAPI_MEDIA_PLAYER_MODULE
 * @{
 */

/**
 * @brief Captures the video frame, asynchronously.
 * @since_tizen 2.3
 * @remarks If the content is encrypted or there are copyright issues with it,
 *          this function could be unsupported depending on the DRM policy.
 *          In case the function is not supported by the DRM policy, it will return an error.
 * @param[in] player      The handle to the media player
 * @param[in] callback    The callback function to register
 * @param[in] user_data   The user data to be passed to the callback function
 * @return @c 0 on success,
 *         otherwise a negative error value
 * @retval #PLAYER_ERROR_NONE Successful
 * @retval #PLAYER_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #PLAYER_ERROR_INVALID_OPERATION Invalid operation
 * @retval #PLAYER_ERROR_INVALID_STATE Invalid player state
 * @pre The player state must be set to #PLAYER_STATE_PLAYING by calling player_start() or set to #PLAYER_STATE_PAUSED by calling player_pause().
 * @pre Video display type should be set by calling player_set_display() otherwise, audio stream is only processed even though video file is set.
 * @post It invokes player_video_captured_cb() when capture completes, if you set a callback.
 * @see player_video_captured_cb()
 */
int player_capture_video(player_h player, player_video_captured_cb callback, void *user_data);

/**
 * @}
 */

/**
 * @addtogroup CAPI_MEDIA_PLAYER_STREAMING_MODULE
 * @{
 */

/**
 * @brief Sets the cookie for streaming playback.
 * @since_tizen 2.3
 * @remarks This function must be called before calling the player_prepare() or player_prepare_async()
 *          to reflect the cookie information when the streaming connection is set up.
 * @param[in] player The handle to the media player
 * @param[in] cookie The cookie to set
 * @param[in] size   The size of the cookie
 * @return @c 0 on success,
 *         otherwise a negative error value
 * @retval #PLAYER_ERROR_NONE Successful
 * @retval #PLAYER_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #PLAYER_ERROR_INVALID_OPERATION Invalid operation
 * @retval #PLAYER_ERROR_INVALID_STATE Invalid player state
 * @pre The player state must be set to #PLAYER_STATE_IDLE by calling player_create() or player_unprepare().
 * @see player_set_uri()
 * @see player_set_streaming_user_agent()
 */
int player_set_streaming_cookie(player_h player, const char *cookie, int size);

/**
 * @brief Sets the streaming user agent for playback.
 * @since_tizen 2.3
 * @remarks This function must be called before calling the player_prepare() or player_prepare_async()
 *          to reflect the user agent information when the streaming connection is set up.
 * @param[in] player     The handle to the media player
 * @param[in] user_agent The user agent to set
 * @param[in] size       The size of the user agent
 * @return @c 0 on success,
 *         otherwise a negative error value
 * @retval #PLAYER_ERROR_NONE Successful
 * @retval #PLAYER_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #PLAYER_ERROR_INVALID_OPERATION Invalid operation
 * @retval #PLAYER_ERROR_INVALID_STATE Invalid player state
 * @pre The player state must be set to #PLAYER_STATE_IDLE by calling player_create() or player_unprepare().
 * @see player_set_uri()
 * @see player_set_streaming_cookie()
 */
int player_set_streaming_user_agent(player_h player, const char *user_agent, int size);

/**
 * @brief Gets the download progress for streaming playback.
 * @since_tizen 2.3
 * @param[in] player The handle to the media player
 * @param[out] start The starting position of received data in percentage [0, 100]
 * @param[out] end   The end position of received data in percentage [0, 100]
 * @return @c 0 on success,
 *         otherwise a negative error value
 * @retval #PLAYER_ERROR_NONE Successful
 * @retval #PLAYER_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #PLAYER_ERROR_INVALID_OPERATION Invalid operation
 * @retval #PLAYER_ERROR_INVALID_STATE Invalid player state
 * @pre The player state must be one of #PLAYER_STATE_READY, #PLAYER_STATE_PLAYING, or #PLAYER_STATE_PAUSED.
 */
int player_get_streaming_download_progress(player_h player, int *start, int *end);

/**
 * @}
 */

/**
 * @addtogroup CAPI_MEDIA_PLAYER_MODULE
 * @{
 */

/**
 * @brief Sets a callback function to be invoked when the playback is finished.
 * @since_tizen 2.3
 * @param[in] player      The handle to the media player
 * @param[in] callback    The callback function to register
 * @param[in] user_data   The user data to be passed to the callback function
 * @return @c 0 on success,
 *         otherwise a negative error value
 * @retval #PLAYER_ERROR_NONE Successful
 * @retval #PLAYER_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #PLAYER_ERROR_INVALID_OPERATION Invalid operation
 * @post  player_completed_cb() will be invoked.
 * @see player_unset_completed_cb()
 * @see player_completed_cb()
 * @see player_start()
 */
int player_set_completed_cb(player_h player, player_completed_cb callback, void *user_data);

/**
 * @brief Unsets the playback completed callback function.
 * @since_tizen 2.3
 * @param[in] player The handle to the media player
 * @return @c 0 on success,
 *         otherwise a negative error value
 * @retval #PLAYER_ERROR_NONE Successful
 * @retval #PLAYER_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #PLAYER_ERROR_INVALID_OPERATION Invalid operation
 * @see player_set_completed_cb()
 */
int player_unset_completed_cb(player_h player);

/**
 * @brief Sets a callback function to be invoked when the playback is interrupted or the interrupt is completed.
 * @since_tizen 2.3
 * @param[in] player      The handle to the media player
 * @param[in] callback    The callback function to register
 * @param[in] user_data   The user data to be passed to the callback function
 * @return @c 0 on success,
 *         otherwise a negative error value
 * @retval #PLAYER_ERROR_NONE Successful
 * @retval #PLAYER_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #PLAYER_ERROR_INVALID_OPERATION Invalid operation
 * @post  player_interrupted_cb() will be invoked.
 * @see player_unset_interrupted_cb()
 * @see #player_interrupted_code_e
 * @see player_interrupted_cb()
 */
int player_set_interrupted_cb(player_h player, player_interrupted_cb callback, void *user_data);

/**
 * @brief Unsets the interrupted callback function.
 * @since_tizen 2.3
 * @param[in] player The handle to the media player
 * @return @c 0 on success,
 *         otherwise a negative error value
 * @retval #PLAYER_ERROR_NONE Successful
 * @retval #PLAYER_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #PLAYER_ERROR_INVALID_OPERATION Invalid operation
 * @see player_set_interrupted_cb()
 */
int player_unset_interrupted_cb(player_h player);

/**
 * @brief Sets a callback function to be invoked when an error occurs.
 * @since_tizen 2.3
 * @param[in] player      The handle to the media player
 * @param[in] callback    The callback function to register
 * @param[in] user_data   The user data to be passed to the callback function
 * @return @c 0 on success,
 *         otherwise a negative error value
 * @retval #PLAYER_ERROR_NONE Successful
 * @retval #PLAYER_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #PLAYER_ERROR_INVALID_OPERATION Invalid operation
 * @post  player_error_cb() will be invoked.
 * @see player_unset_error_cb()
 * @see player_error_cb()
 */
int player_set_error_cb(player_h player, player_error_cb callback, void *user_data);

/**
 * @brief Unsets the error callback function.
 * @since_tizen 2.3
 * @param[in] player The handle to the media player
 * @return @c 0 on success,
 *         otherwise a negative error value
 * @retval #PLAYER_ERROR_NONE Successful
 * @retval #PLAYER_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #PLAYER_ERROR_INVALID_OPERATION Invalid operation
 * @see player_set_error_cb()
 */
int player_unset_error_cb(player_h player);

/**
 * @}
 */

/**
 * @addtogroup CAPI_MEDIA_PLAYER_STREAMING_MODULE
 * @{
 */

/**
 * @brief Sets a callback function to be invoked when there is a change in the buffering status of a media stream.
 * @since_tizen 2.3
 * @remarks The media resource should be streamed over the network.
 * @param[in] player      The handle to the media player
 * @param[in] callback    The callback function to register
 * @param[in] user_data   The user data to be passed to the callback function
 * @return @c 0 on success,
 *         otherwise a negative error value
 * @retval #PLAYER_ERROR_NONE Successful
 * @retval #PLAYER_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #PLAYER_ERROR_INVALID_OPERATION Invalid operation
 * @post  player_buffering_cb() will be invoked.
 * @see player_unset_buffering_cb()
 * @see player_set_uri()
 * @see player_buffering_cb()
 */
int player_set_buffering_cb(player_h player, player_buffering_cb callback, void *user_data);

/**
 * @brief Unsets the buffering callback function.
 * @since_tizen 2.3
 * @param[in] player The handle to the media player
 * @return @c 0 on success,
 *         otherwise a negative error value
 * @retval #PLAYER_ERROR_NONE Successful
 * @retval #PLAYER_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #PLAYER_ERROR_INVALID_OPERATION Invalid operation
 * @see player_set_buffering_cb()
 */
int player_unset_buffering_cb(player_h player);

/**
 * @}
 */

/**
 * @addtogroup CAPI_MEDIA_PLAYER_MODULE
 * @{
 */

/**
 * @brief Sets the playback rate.
 * @details The default value is @c 1.0.
 * @since_tizen 2.3
 * @remarks #PLAYER_ERROR_INVALID_OPERATION occurs when streaming playback.
 * @remarks No operation is performed, if @a rate is @c 0.
 * @remarks The sound is muted, when playback rate is under @c 0.0 and over @c 2.0.
 * @remarks If audio offload is enabled by calling player_audio_offload_set_enabled(),
 *          this function will return #PLAYER_ERROR_NOT_AVAILABLE. (Since 5.5)
 * @param[in] player   The handle to the media player
 * @param[in] rate     The playback rate (-5.0x ~ 5.0x)
 * @return @c 0 on success,
 *         otherwise a negative error value
 * @retval #PLAYER_ERROR_NONE Successful
 * @retval #PLAYER_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #PLAYER_ERROR_INVALID_OPERATION Invalid operation
 * @retval #PLAYER_ERROR_INVALID_STATE Invalid player state
 * @retval #PLAYER_ERROR_NOT_AVAILABLE Not available (Since 5.5)
 * @pre The player state must be one of #PLAYER_STATE_READY, #PLAYER_STATE_PLAYING, or #PLAYER_STATE_PAUSED.
 * @see player_audio_offload_set_enabled()
 */
int player_set_playback_rate(player_h player, float rate);

/**
 * @}
 */

/**
 * @addtogroup CAPI_MEDIA_PLAYER_SUBTITLE_MODULE
 * @{
 */

/**
 * @brief Sets a subtitle path.
 * @since_tizen 2.3
 * @remarks Only MicroDVD/SubViewer(*.sub), SAMI(*.smi), and SubRip(*.srt) subtitle formats are supported.
 * @remarks The mediastorage privilege(http://tizen.org/privilege/mediastorage) must be added if any files are used to play located in the internal storage.
 * @remarks The externalstorage privilege(http://tizen.org/privilege/externalstorage) must be added if any files are used to play located in the external storage.
 * @remarks The path value can be @c NULL for reset before calling player_prepare() or player_prepare_async().
 *
 * @param[in] player   The handle to the media player
 * @param[in] path     The absolute path of the subtitle file, it can be @c NULL in the #PLAYER_STATE_IDLE state.
 * @return @c 0 on success,
 *         otherwise a negative error value
 * @retval #PLAYER_ERROR_NONE Successful
 * @retval #PLAYER_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #PLAYER_ERROR_INVALID_OPERATION Invalid operation
 * @retval #PLAYER_ERROR_PERMISSION_DENIED Permission denied
 * @retval #PLAYER_ERROR_NO_SUCH_FILE File not found (Since 4.0)
 */
int player_set_subtitle_path(player_h player, const char *path);

/**
 * @brief Sets a callback function to be invoked when a subtitle updates.
 * @since_tizen 2.3
 * @remarks The callback is called in a separate thread (not in the main loop).
 * @param[in] player      The handle to the media player
 * @param[in] callback    The callback function to register
 * @param[in] user_data   The user data to be passed to the callback function
 * @return @c 0 on success,
 *         otherwise a negative error value
 * @retval #PLAYER_ERROR_NONE Successful
 * @retval #PLAYER_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #PLAYER_ERROR_INVALID_OPERATION Invalid operation
 * @pre The subtitle must be set by calling player_set_subtitle_path().
 * @post  player_subtitle_updated_cb() will be invoked.
 * @see player_unset_subtitle_updated_cb()
 * @see player_subtitle_updated_cb()
 * @see player_set_subtitle_path()
 */
int player_set_subtitle_updated_cb(player_h player, player_subtitle_updated_cb callback, void *user_data);

/**
 * @brief Unsets the subtitle updated callback function.
 * @since_tizen 2.3
 * @param[in] player The handle to the media player
 * @return @c 0 on success,
 *         otherwise a negative error value
 * @retval #PLAYER_ERROR_NONE Successful
 * @retval #PLAYER_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #PLAYER_ERROR_INVALID_OPERATION Invalid operation
 * @see player_set_subtitle_updated_cb()
 */
int player_unset_subtitle_updated_cb(player_h player);

/**
 * @brief Sets the seek position for the subtitle.
 * @since_tizen 2.3.1
 * @remarks Only MicroDVD/SubViewer(*.sub), SAMI(*.smi), and SubRip(*.srt) subtitle formats are supported.
 * @param[in]  player        The handle to the media player
 * @param[in]  milliseconds  The position in milliseconds from the start to the seek point
 * @return @c 0 on success,
 *         otherwise a negative error value
 * @retval #PLAYER_ERROR_NONE Successful
 * @retval #PLAYER_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #PLAYER_ERROR_INVALID_OPERATION Invalid operation
 * @retval #PLAYER_ERROR_INVALID_STATE Invalid player state
 * @pre The subtitle must be set by calling player_set_subtitle_path().
 * @pre The player state must be one of #PLAYER_STATE_PLAYING or #PLAYER_STATE_PAUSED.
 */
int player_set_subtitle_position_offset(player_h player, int milliseconds);

/**
 * @}
 */


/**
 * @addtogroup CAPI_MEDIA_PLAYER_STREAM_INFO_MODULE
 * @{
 */

/**
 * @brief Sets a callback function to be invoked when video stream is changed.
 * @since_tizen 2.4
 * @remarks The stream changing is detected just before rendering operation.
 * @remarks The callback is called in a separate thread (not in the main loop).
 * @remarks This function must be called before calling the player_prepare() or player_prepare_async()
 *          to reflect the requirement when the player is building.
 * @param[in] player      The handle to the media player
 * @param[in] callback    The stream changed callback function to register
 * @param[in] user_data   The user data to be passed to the callback function
 * @return @c 0 on success,
 *         otherwise a negative error value
 * @retval #PLAYER_ERROR_NONE Successful
 * @retval #PLAYER_ERROR_INVALID_STATE Invalid player state
 * @retval #PLAYER_ERROR_INVALID_PARAMETER Invalid parameter
 * @pre The player state must be set to #PLAYER_STATE_IDLE by calling player_create() or player_unprepare().
 * @post player_video_stream_changed_cb() will be invoked.
 * @see player_unset_video_stream_changed_cb()
 * @see player_video_stream_changed_cb()
 */
int player_set_video_stream_changed_cb(player_h player, player_video_stream_changed_cb callback, void *user_data);

/**
 * @brief Unsets the video stream changed callback function.
 * @since_tizen 2.4
 * @param[in] player   The handle to the media player
 * @return @c 0 on success,
 *         otherwise a negative error value
 * @retval #PLAYER_ERROR_NONE Successful
 * @retval #PLAYER_ERROR_INVALID_PARAMETER Invalid parameter
 * @see player_set_video_stream_changed_cb()
 */
int player_unset_video_stream_changed_cb(player_h player);

/**
 * @}
 */


/**
 * @addtogroup CAPI_MEDIA_PLAYER_MODULE
 * @{
 */

/**
 * @brief Gets current track index.
 * @details Index starts from 0.
 * @since_tizen 2.4
 * @remarks If audio offload is enabled by calling player_audio_offload_set_enabled(),
 *          this function will return #PLAYER_ERROR_NOT_AVAILABLE. (Since 5.5)
 * @param[in]  player   The handle to the media player
 * @param[in]  type     The type of target stream which is #PLAYER_STREAM_TYPE_AUDIO or #PLAYER_STREAM_TYPE_TEXT
 * @param[out] index    The index of track
 * @return @c 0 on success,
 *         otherwise a negative error value
 * @retval #PLAYER_ERROR_NONE Successful
 * @retval #PLAYER_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #PLAYER_ERROR_INVALID_OPERATION Invalid operation
 * @retval #PLAYER_ERROR_INVALID_STATE Invalid player state
 * @retval #PLAYER_ERROR_NOT_AVAILABLE Not available (Since 5.5)
 * @pre The player state must be one of #PLAYER_STATE_READY, #PLAYER_STATE_PLAYING, or #PLAYER_STATE_PAUSED
 * @see player_audio_offload_set_enabled()
 */
int player_get_current_track(player_h player, player_stream_type_e type, int *index);

/**
 * @brief Gets language code of a track.
 * @since_tizen 2.4
 * @remarks @a code must be released with free() by caller
 * @remarks If audio offload is enabled by calling player_audio_offload_set_enabled(),
 *          this function will return #PLAYER_ERROR_NOT_AVAILABLE. (Since 5.5)
 * @param[in] player   The handle to the media player
 * @param[in] type     The type of target stream which is #PLAYER_STREAM_TYPE_AUDIO or #PLAYER_STREAM_TYPE_TEXT
 * @param[in] index    The index of track
 * @param[out] code A language code in ISO 639-1. "und" will be returned if the language is undefined.
 * @return @c 0 on success,
 *         otherwise a negative error value
 * @retval #PLAYER_ERROR_NONE Successful
 * @retval #PLAYER_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #PLAYER_ERROR_INVALID_OPERATION Invalid operation
 * @retval #PLAYER_ERROR_INVALID_STATE Invalid player state
 * @retval #PLAYER_ERROR_NOT_AVAILABLE Not available (Since 5.5)
 * @pre The player state must be one of #PLAYER_STATE_READY, #PLAYER_STATE_PLAYING, or #PLAYER_STATE_PAUSED
 * @see player_audio_offload_set_enabled()
 */
int player_get_track_language_code(player_h player, player_stream_type_e type, int index, char **code);

/**
 * @brief Gets the track count.
 * @since_tizen 2.4
 * @remarks If audio offload is enabled by calling player_audio_offload_set_enabled(),
 *          this function will return #PLAYER_ERROR_NOT_AVAILABLE. (Since 5.5)
 * @param[in] player   The handle to the media player
 * @param[in] type     The type of target stream which is #PLAYER_STREAM_TYPE_AUDIO or #PLAYER_STREAM_TYPE_TEXT
 * @param[out] count   The number of track
 * @return @c 0 on success,
 *         otherwise a negative error value
 * @retval #PLAYER_ERROR_NONE Successful
 * @retval #PLAYER_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #PLAYER_ERROR_INVALID_OPERATION Invalid operation
 * @retval #PLAYER_ERROR_INVALID_STATE Invalid player state
 * @retval #PLAYER_ERROR_NOT_AVAILABLE Not available (Since 5.5)
 * @pre The player state must be one of #PLAYER_STATE_READY, #PLAYER_STATE_PLAYING, or #PLAYER_STATE_PAUSED
 * @see player_audio_offload_set_enabled()
 */
int player_get_track_count(player_h player, player_stream_type_e type, int *count);

/**
 * @brief Selects a track to play.
 * @since_tizen 2.4
 * @remarks If audio offload is enabled by calling player_audio_offload_set_enabled(),
 *          this function will return #PLAYER_ERROR_NOT_AVAILABLE. (Since 5.5)
 * @param[in] player   The handle to the media player
 * @param[in] type     The type of target stream which is #PLAYER_STREAM_TYPE_AUDIO or #PLAYER_STREAM_TYPE_TEXT
 * @param[in] index    The index of track
 * @return @c 0 on success,
 *         otherwise a negative error value
 * @retval #PLAYER_ERROR_NONE Successful
 * @retval #PLAYER_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #PLAYER_ERROR_INVALID_OPERATION Invalid operation
 * @retval #PLAYER_ERROR_INVALID_STATE Invalid player state
 * @retval #PLAYER_ERROR_NOT_AVAILABLE Not available (Since 5.5)
 * @pre The player state must be one of #PLAYER_STATE_READY, #PLAYER_STATE_PLAYING, or #PLAYER_STATE_PAUSED
 * @see player_get_current_track()
 * @see player_audio_offload_set_enabled()
 */
int player_select_track(player_h player, player_stream_type_e type, int index);

/**
 * @}
 */

/**
 * @addtogroup CAPI_MEDIA_PLAYER_STREAMING_MODULE
 * @{
 */

/**
 * @brief Retrieves all the streaming variant information.
 * @since_tizen 4.0
 * @remarks This function is used for adaptive streaming(hls/mpeg dash) only.
 * @param[in] player      The handle to the media player
 * @param[in] callback    The iteration callback function
 * @param[in] user_data   The user data to be passed to the callback function
 * @return @c 0 on success,
 *         otherwise a negative error value
 * @retval #PLAYER_ERROR_NONE Successful
 * @retval #PLAYER_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #PLAYER_ERROR_INVALID_OPERATION Invalid operation
 * @retval #PLAYER_ERROR_INVALID_STATE Invalid player state
 * @pre The player state must be one of #PLAYER_STATE_READY, #PLAYER_STATE_PLAYING, or #PLAYER_STATE_PAUSED
 * @see player_adaptive_variant_cb()
 */
int player_foreach_adaptive_variant(player_h player, player_adaptive_variant_cb callback, void *user_data);

/**
 * @brief Sets the maximum limit of the streaming variant.
 * @since_tizen 4.0
 * @remarks This function is used for adaptive streaming(hls/mpeg dash) only.
 * @remarks The bandwidth setting can only be applied if there is no width, height information at streaming variant header.
 *          Application can get all the variant information by calling player_foreach_adaptive_variant() function.
 * @remarks If there is no affordable stream for the condition, the minimum bandwidth stream will be selected.
 * @param[in] player      The handle to the media player
 * @param[in] bandwidth   The max bandwidth limit of the stream variant (default: -1)
 * @param[in] width       The max width limit of the stream variant (default: -1)
 * @param[in] height      The max height limit of the stream variant (default: -1)
 * @return @c 0 on success,
 *         otherwise a negative error value
 * @retval #PLAYER_ERROR_NONE Successful
 * @retval #PLAYER_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #PLAYER_ERROR_INVALID_OPERATION Invalid operation
 * @see player_foreach_adaptive_variant()
 * @see player_get_max_adaptive_variant_limit()
 */
int player_set_max_adaptive_variant_limit(player_h player, int bandwidth, int width, int height);

/**
 * @brief Gets the maximum limit of the streaming variant.
 * @since_tizen 4.0
 * @remarks This function is used for adaptive streaming(hls/mpeg dash) only.
 * @param[in]  player      The handle to the media player
 * @param[out] bandwidth   The max bandwidth limit of the stream variant (default: -1)
 * @param[out] width       The max width limit of the stream variant (default: -1)
 * @param[out] height      The max height limit of the stream variant (default: -1)
 * @return @c 0 on success,
 *         otherwise a negative error value
 * @retval #PLAYER_ERROR_NONE Successful
 * @retval #PLAYER_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #PLAYER_ERROR_INVALID_OPERATION Invalid operation
 * @see player_foreach_adaptive_variant()
 * @see player_set_max_adaptive_variant_limit()
 */
int player_get_max_adaptive_variant_limit(player_h player, int *bandwidth, int *width, int *height);

/**
 * @}
 */

/**
 * @addtogroup CAPI_MEDIA_PLAYER_MODULE
 * @{
 */

/**
 * @brief Sets the audio only mode.
 * @details This function is used to disable or enable video rendering during playback.
 * @since_tizen 4.0
 * @param[in] player       The handle to the media player
 * @param[in] audio_only   The new audio only status: (@c true = enable audio only, @c false = disable audio only)
 * @return @c 0 on success,
 *         otherwise a negative error value
 * @retval #PLAYER_ERROR_NONE                Successful
 * @retval #PLAYER_ERROR_INVALID_PARAMETER   Invalid parameter
 * @retval #PLAYER_ERROR_INVALID_OPERATION   Invalid operation
 * @retval #PLAYER_ERROR_INVALID_STATE       Invalid player state
 * @pre The player state must be one of: #PLAYER_STATE_READY, #PLAYER_STATE_PLAYING, or #PLAYER_STATE_PAUSED.
 * @see player_is_audio_only()
 */
int player_set_audio_only(player_h player, bool audio_only);

/**
 * @brief Gets the audio only mode status.
 * @since_tizen 4.0
 * @param[in]  player       The handle to the media player
 * @param[out] audio_only   The current audio only status: (@c true = audio only enabled, @c false = audio only disabled)
 * @return @c 0 on success,
 *         otherwise a negative error value
 * @retval #PLAYER_ERROR_NONE                Successful
 * @retval #PLAYER_ERROR_INVALID_PARAMETER   Invalid parameter
 * @retval #PLAYER_ERROR_INVALID_OPERATION   Invalid operation
 * @see player_set_audio_only()
 */
int player_is_audio_only(player_h player, bool *audio_only);

/**
 * @brief Sets the player's replaygain status.
 * @details If the replaygain status is @c true, replaygain is applied (if contents has a replaygain tag).
 *          If it is @c false, the replaygain isn't affected by tag and properties.
 * @since_tizen 5.0
 * @remarks If audio offload is enabled by calling player_audio_offload_set_enabled(),
 *          this function will return #PLAYER_ERROR_NOT_AVAILABLE and
 *          this will not work at all even if this was called before enabling offload. (Since 5.5)
 * @remarks This function could be unavailable depending on the audio codec type and
 *          this will return #PLAYER_ERROR_NOT_AVAILABLE. (Since 5.5)
 * @param[in]   player  The handle to the media player
 * @param[in]   enabled The new replaygain status: (@c true = enable, @c false = disable)
 * @return @c 0 on success,
 *         otherwise a negative error value
 * @retval #PLAYER_ERROR_NONE Successful
 * @retval #PLAYER_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #PLAYER_ERROR_INVALID_OPERATION Invalid operation
 * @retval #PLAYER_ERROR_NOT_AVAILABLE Not available (Since 5.5)
 * @see player_is_replaygain_enabled()
 * @see player_audio_offload_set_enabled()
 * @see player_audio_offload_is_enabled()
 * @see player_set_audio_codec_type()
 * @see player_get_audio_codec_type()
 */
int player_set_replaygain_enabled(player_h player, bool enabled);

/**
 * @brief Gets the player's replaygain status.
 * @since_tizen 5.0
 * @remarks Depending on audio codec type or by enabling audio offload,
 *          this function could be unavailable and this will return #PLAYER_ERROR_NOT_AVAILABLE. (Since 5.5)
 * @param[in]   player    The handle to the media player
 * @param[out]  enabled   Pointer to store current replaygain status:
 *                        (@c true = enabled replaygain,
                          @c false = disabled replaygain)
 * @return @c 0 on success,
 *         otherwise a negative error value
 * @retval #PLAYER_ERROR_NONE Successful
 * @retval #PLAYER_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #PLAYER_ERROR_INVALID_OPERATION Invalid operation
 * @retval #PLAYER_ERROR_NOT_AVAILABLE Not available (Since 5.5)
 * @see player_set_replaygain_enabled()
 * @see player_audio_offload_set_enabled()
 * @see player_audio_offload_is_enabled()
 * @see player_set_audio_codec_type()
 * @see player_get_audio_codec_type()
 */
int player_is_replaygain_enabled(player_h player, bool *enabled);

/**
 * @brief Sets the ROI (Region Of Interest) area of the content video source.
 * @details This function is to set the ROI area of video content to render it
 *          on #PLAYER_DISPLAY_TYPE_OVERLAY display with current display mode.
 *          It can be regarded as zooming operation because the selected video area will be rendered fit to the display.
 * @since_tizen 5.0
 * @remarks This function requires the ratio value of the each coordinate and size based on the video resolution size
 *          to consider the dynamic resolution video content.
 * @remarks This function have to be called after calling player_set_display() and the ROI area is valid only in #PLAYER_DISPLAY_TYPE_OVERLAY.
 * @param[in] player     The handle to the media player
 * @param[in] x_scale    X coordinate ratio value of the video source area based on the video width size \n
 *                       Valid range is 0.0~1.0.
 * @param[in] y_scale    Y coordinate ratio value of the video source area based on the video height size \n
 *                       Valid range is 0.0~1.0.
 * @param[in] w_scale    Width ratio value of the video source area based on the video width size \n
 *                       Valid range is from greater than 0.0 to 1.0.
 * @param[in] h_scale    Height ratio value of the video source area based on the video height size \n
 *                       Valid range is from greater than 0.0 to 1.0.
 * @return @c 0 on success,
 *         otherwise a negative error value
 * @retval  #PLAYER_ERROR_NONE Successful
 * @retval  #PLAYER_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval  #PLAYER_ERROR_INVALID_OPERATION Invalid operation
 * @see #player_display_type_e
 * @see player_set_display()
 * @see player_set_display_mode()
 * @see player_get_video_size()
 * @see player_get_video_roi_area()
 * @par Example
   @code
       #include <player.h>
       ...
       player_h player = NULL;
       player_display_h display = NULL;
       int width = 0, height = 0;
       ...
       player_set_display (player, PLAYER_DISPLAY_TYPE_OVERLAY, display);
       player_get_video_size (player, &width, &height);
       ...
       player_set_video_roi_area (player, 30/(double)width, 30/(double)height, 480/(double)width, 270/(double)height);
       ...
   @endcode
 */
int player_set_video_roi_area(player_h player, double x_scale, double y_scale, double w_scale, double h_scale);

/**
 * @brief Gets the ROI (Region Of Interest) area of the content video source.
 * @since_tizen 5.0
 * @remarks This function gets the ratio value of the each coordinate and size based on the video resolution size.
 * @remarks The ROI area is valid only in #PLAYER_DISPLAY_TYPE_OVERLAY.
 * @param[in]  player     The handle to the media player
 * @param[out] x_scale    The current X coordinate ratio value of the video source area based on the video width size
 * @param[out] y_scale    The current Y coordinate ratio value of the video source area based on the video height size
 * @param[out] w_scale    The current width ratio value of the video source area based on the video width size
 * @param[out] h_scale    The current height ratio value of the video source area based on the video height size
 * @return @c 0 on success,
 *         otherwise a negative error value
 * @retval  #PLAYER_ERROR_NONE Successful
 * @retval  #PLAYER_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval  #PLAYER_ERROR_INVALID_OPERATION Invalid operation
 * @see #player_display_type_e
 * @see player_set_display()
 * @see player_get_video_size()
 * @see player_set_video_roi_area()
 */
int player_get_video_roi_area(player_h player, double *x_scale, double *y_scale, double *w_scale, double *h_scale);

/**
 * @brief Enables or disables controlling the pitch of audio.
 * @since_tizen 5.5
 * @remarks This function is used for audio content only.
 * @remarks Enabling pitch control could increase the CPU usage on some devices.
 * @remarks If audio offload is enabled by calling player_audio_offload_set_enabled(),
 *          this function will return #PLAYER_ERROR_NOT_AVAILABLE and
 *          this will not work at all even if this was called before enabling offload.
 * @remarks This function could be unavailable depending on the audio codec type and
 *          this will return #PLAYER_ERROR_NOT_AVAILABLE.
 * @param[in] player   The handle to the media player
 * @param[in] enabled  The new audio pitch control status (default: false)
 * @return @c 0 on success,
 *         otherwise a negative error value
 * @retval #PLAYER_ERROR_NONE Successful
 * @retval #PLAYER_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #PLAYER_ERROR_INVALID_STATE Invalid player state
 * @retval #PLAYER_ERROR_INVALID_OPERATION Invalid operation
 * @retval #PLAYER_ERROR_NOT_AVAILABLE Not available
 * @pre The player state must be #PLAYER_STATE_IDLE.
 * @see player_audio_pitch_is_enabled()
 * @see player_audio_pitch_set_value()
 * @see player_audio_pitch_get_value()
 * @see player_audio_offload_set_enabled()
 * @see player_audio_offload_is_enabled()
 * @see player_set_audio_codec_type()
 * @see player_get_audio_codec_type()
 */
int player_audio_pitch_set_enabled(player_h player, bool enabled);

/**
 * @brief Gets the status of controlling the pitch of audio.
 * @since_tizen 5.5
 * @remarks This function is used for audio content only.
 * @remarks Depending on audio codec type or by enabling audio offload,
 *          this function could be unavailable and this will return #PLAYER_ERROR_NOT_AVAILABLE.
 * @param[in]   player   The handle to the media player
 * @param[out]  enabled  The audio pitch control status (default: false)
 * @return @c 0 on success,
 *         otherwise a negative error value
 * @retval #PLAYER_ERROR_NONE Successful
 * @retval #PLAYER_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #PLAYER_ERROR_INVALID_OPERATION Invalid operation
 * @retval #PLAYER_ERROR_NOT_AVAILABLE Not available
 * @see player_audio_pitch_set_enabled()
 * @see player_audio_pitch_set_value()
 * @see player_audio_pitch_get_value()
 * @see player_audio_offload_set_enabled()
 * @see player_audio_offload_is_enabled()
 * @see player_set_audio_codec_type()
 * @see player_get_audio_codec_type()
 */
int player_audio_pitch_is_enabled(player_h player, bool *enabled);

/**
 * @brief Sets the pitch of audio.
 * @since_tizen 5.5
 * @remarks This function is used for audio content only.
 * @remarks If audio offload is enabled by calling player_audio_offload_set_enabled(),
 *          this function will return #PLAYER_ERROR_NOT_AVAILABLE and
 *          this will not work at all even if this was called before enabling offload.
 * @remarks This function could be unavailable depending on the audio codec type and
 *          this will return #PLAYER_ERROR_NOT_AVAILABLE.
 * @param[in] player The handle to the media player
 * @param[in] value  The audio stream pitch value  \n
 *                   Valid range is 0.5~2. Default value is 1.
 * @return @c 0 on success,
 *         otherwise a negative error value
 * @retval #PLAYER_ERROR_NONE Successful
 * @retval #PLAYER_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #PLAYER_ERROR_INVALID_OPERATION Invalid operation
 * @retval #PLAYER_ERROR_NOT_AVAILABLE Not available
 * @pre The pitch control must be enabled by calling player_audio_pitch_set_enabled() function.
 * @see player_audio_pitch_set_enabled()
 * @see player_audio_pitch_is_enabled()
 * @see player_audio_pitch_get_value()
 * @see player_audio_offload_set_enabled()
 * @see player_audio_offload_is_enabled()
 * @see player_set_audio_codec_type()
 * @see player_get_audio_codec_type()
 */
int player_audio_pitch_set_value(player_h player, float value);

/**
 * @brief Gets the pitch of audio.
 * @since_tizen 5.5
 * @remarks This function is used for audio content only.
 * @remarks Depending on audio codec type or by enabling audio offload,
 *          this function could be unavailable and this will return #PLAYER_ERROR_NOT_AVAILABLE.
 * @param[in]  player The handle to the media player
 * @param[out] value  The audio stream pitch value \n
 *                    Valid range is 0.5~2. Default value is 1.
 * @return @c 0 on success,
 *         otherwise a negative error value
 * @retval #PLAYER_ERROR_NONE Successful
 * @retval #PLAYER_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #PLAYER_ERROR_INVALID_OPERATION Invalid operation
 * @retval #PLAYER_ERROR_NOT_AVAILABLE Not available
 * @see player_audio_pitch_set_enabled()
 * @see player_audio_pitch_is_enabled()
 * @see player_audio_pitch_set_value()
 * @see player_audio_offload_set_enabled()
 * @see player_audio_offload_is_enabled()
 * @see player_set_audio_codec_type()
 * @see player_get_audio_codec_type()
 */
int player_audio_pitch_get_value(player_h player, float *value);

/**
 * @brief Retrieves all supported media formats for audio offload.
 * @details The supported media format can vary depending on the device capabilities.
 * @since_tizen 5.5
 * @remarks This function is related to the following feature:\n
 *          %http://tizen.org/feature/multimedia.player.audio_offload\n
 * @param[in] player      The handle to the media player
 * @param[in] callback    The iteration callback function
 * @param[in] user_data   The user data to be passed to the callback function
 * @return @c 0 on success,
 *         otherwise a negative error value
 * @retval #PLAYER_ERROR_NONE Successful
 * @retval #PLAYER_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #PLAYER_ERROR_INVALID_OPERATION Invalid operation
 * @retval #PLAYER_ERROR_FEATURE_NOT_SUPPORTED_ON_DEVICE Unsupported feature
 * @see player_supported_media_format_cb()
 * @see player_audio_offload_set_enabled()
 * @see player_audio_offload_is_enabled()
 * @see player_audio_offload_is_activated()
 */
int player_audio_offload_foreach_supported_format(player_h player, player_supported_media_format_cb callback, void *user_data);

/**
 * @brief Enables or disables the audio offload.
 * @details The player lets the hardware decode and render the sound if the audio offload is enabled.
 *          This will reduce the power consumption, but will disable the ability to handle output PCM.
 *          Please check the remarks for a list of functions which will not work if offloading is enabled.
 * @since_tizen 5.5
 * @remarks This function is related to the following feature:\n
 *          %http://tizen.org/feature/multimedia.player.audio_offload\n
 * @remarks The sound stream type of the player should be #SOUND_STREAM_TYPE_MEDIA.\n
 * @remarks If audio offload is enabled, functions for audio effect are not available.\n
 * @remarks If audio offload is enabled, the following functions will return #PLAYER_ERROR_NOT_AVAILABLE
 *          and they will not work at all even if they were called before enabling offload. :\n
 *          player_set_media_packet_audio_frame_decoded_cb()\n
 *          player_unset_media_packet_audio_frame_decoded_cb()\n
 *          player_set_audio_latency_mode()\n
 *          player_get_audio_latency_mode()\n
 *          player_set_playback_rate()\n
 *          player_get_current_track()\n
 *          player_get_track_language_code()\n
 *          player_get_track_count()\n
 *          player_select_track()\n
 *          player_set_replaygain_enabled()\n
 *          player_is_replaygain_enabled()\n
 *          player_audio_pitch_set_enabled()\n
 *          player_audio_pitch_is_enabled()\n
 *          player_audio_pitch_set_value()\n
 *          player_audio_pitch_get_value()\n
 * @param[in] player   The handle to the media player
 * @param[in] enabled  The new audio offload status (default: false)
 * @return @c 0 on success,
 *         otherwise a negative error value
 * @retval #PLAYER_ERROR_NONE Successful
 * @retval #PLAYER_ERROR_INVALID_STATE Invalid player state
 * @retval #PLAYER_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #PLAYER_ERROR_INVALID_OPERATION Invalid operation
 * @retval #PLAYER_ERROR_FEATURE_NOT_SUPPORTED_ON_DEVICE Unsupported feature
 * @pre The player state must be #PLAYER_STATE_IDLE.
 * @see player_set_sound_stream_info()
 * @see player_audio_effect_equalizer_is_available()
 * @see player_set_interrupted_cb()
 * @see player_audio_offload_foreach_supported_format()
 * @see player_audio_offload_is_enabled()
 * @see player_audio_offload_is_activated()
 */
int player_audio_offload_set_enabled(player_h player, bool enabled);

/**
 * @brief Gets the enabling status of audio offload.
 * @since_tizen 5.5
 * @remarks This function is related to the following feature:\n
 *          %http://tizen.org/feature/multimedia.player.audio_offload\n
 * @param[in]  player   The handle to the media player
 * @param[out] enabled  The enabling status (default: false)
 * @return @c 0 on success,
 *         otherwise a negative error value
 * @retval #PLAYER_ERROR_NONE Successful
 * @retval #PLAYER_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #PLAYER_ERROR_INVALID_OPERATION Invalid operation
 * @retval #PLAYER_ERROR_FEATURE_NOT_SUPPORTED_ON_DEVICE Unsupported feature
 * @see player_audio_offload_foreach_supported_format()
 * @see player_audio_offload_set_enabled()
 * @see player_audio_offload_is_activated()
 */
int player_audio_offload_is_enabled(player_h player, bool *enabled);

/**
 * @brief Gets the activation status of audio offload.
 * @details Audio offload could be inactivated depending on the
 *          audio device capability even if the audio offload feature is supported.
 * @since_tizen 5.5
 * @remarks This function is related to the following feature:\n
 *          %http://tizen.org/feature/multimedia.player.audio_offload\n
 * @param[in]   player     The handle to the media player
 * @param[out]  activated  The activation status
 * @return @c 0 on success,
 *         otherwise a negative error value
 * @retval #PLAYER_ERROR_NONE Successful
 * @retval #PLAYER_ERROR_INVALID_STATE Invalid state
 * @retval #PLAYER_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #PLAYER_ERROR_INVALID_OPERATION Invalid operation
 * @retval #PLAYER_ERROR_FEATURE_NOT_SUPPORTED_ON_DEVICE Unsupported feature
 * @pre The player state must be one of #PLAYER_STATE_READY, #PLAYER_STATE_PLAYING, or #PLAYER_STATE_PAUSED
 * @see player_audio_offload_foreach_supported_format()
 * @see player_audio_offload_set_enabled()
 * @see player_audio_offload_is_enabled()
 */
int player_audio_offload_is_activated(player_h player, bool *activated);

/**
 * @brief Sets the default codec type of the audio decoder.
 * @since_tizen 5.5
 * @remarks The default codec type could be different depending on the device capability.
 *          S/W codec type is supported basically.
 * @remarks If selected H/W audio codec type does not support in some cases,
 *          S/W audio codec type could be used instead.\n
 * @remarks If application use the H/W audio codec type by default,
 *          following functions have to be called after setting codec type
 *          because the availability could be changed depending on the codec capability. :\n
 *          player_audio_effect_equalizer_is_available()\n
 *          player_set_media_packet_audio_frame_decoded_cb()\n
 *          player_unset_media_packet_audio_frame_decoded_cb()\n
 *          player_set_replaygain_enabled()\n
 *          player_is_replaygain_enabled()\n
 *          player_audio_pitch_set_enabled()\n
 *          player_audio_pitch_is_enabled()\n
 *          player_audio_pitch_set_value()\n
 *          player_audio_pitch_get_value()\n
 * @param[in] player       The handle to the media player
 * @param[in] codec_type   The default codec type
 * @return @c 0 on success,
 *         otherwise a negative error value
 * @retval #PLAYER_ERROR_NONE Successful
 * @retval #PLAYER_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #PLAYER_ERROR_INVALID_OPERATION Invalid operation
 * @retval #PLAYER_ERROR_INVALID_STATE Invalid player state
 * @retval #PLAYER_ERROR_NOT_SUPPORTED_AUDIO_CODEC Not support audio codec type
 * @pre The player state must be #PLAYER_STATE_IDLE by player_create() or player_unprepare().
 * @see player_get_audio_codec_type()
 */
int player_set_audio_codec_type(player_h player, player_codec_type_e codec_type);

/**
 * @brief Gets the default codec type of the audio decoder.
 * @since_tizen 5.5
 * @param[in]  player        The handle to the media player
 * @param[out] codec_type    The default codec type
 * @return @c 0 on success,
 *         otherwise a negative error value
 * @retval #PLAYER_ERROR_NONE Successful
 * @retval #PLAYER_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #PLAYER_ERROR_INVALID_OPERATION Invalid operation
 * @see player_set_audio_codec_type()
 */
int player_get_audio_codec_type(player_h player, player_codec_type_e *codec_type);

/**
 * @brief Sets the default codec type of the video decoder.
 * @since_tizen 8.0
 * @remarks The default codec type could be different depending on the device capability.
 *          Usually the H/W codec has higher priority than S/W codec if it exist.
 * @param[in] player       The handle to the media player
 * @param[in] codec_type   The codec type
 * @return @c 0 on success,
 *         otherwise a negative error value
 * @retval #PLAYER_ERROR_NONE Successful
 * @retval #PLAYER_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #PLAYER_ERROR_INVALID_OPERATION Invalid operation
 * @retval #PLAYER_ERROR_INVALID_STATE Invalid player state
 * @retval #PLAYER_ERROR_NOT_SUPPORTED_VIDEO_CODEC Not supported video codec type
 * @pre The player state must be #PLAYER_STATE_IDLE by player_create() or player_unprepare().
 * @see player_get_video_codec_type()
 */
int player_set_video_codec_type(player_h player, player_codec_type_e codec_type);

/**
 * @brief Gets the default codec type of the video decoder.
 * @since_tizen 8.0
 * @param[in]  player        The handle to the media player
 * @param[out] codec_type    The default codec type
 * @return @c 0 on success,
 *         otherwise a negative error value
 * @retval #PLAYER_ERROR_NONE Successful
 * @retval #PLAYER_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #PLAYER_ERROR_INVALID_OPERATION Invalid operation
 * @see player_set_video_codec_type()
 */
int player_get_video_codec_type(player_h player, player_codec_type_e *codec_type);

/**
 * @}
 */

/**
 * @addtogroup CAPI_MEDIA_PLAYER_STREAMING_MODULE
 * @{
 */

/**
 * @brief Sets the streaming buffering time.
 *  4.0
 * @param[in] player        The handle to the media player
 * @param[in] prebuffer_ms  The time duration of buffering data that must be prerolled to start playback. \n
 *                          The value should be more than 1000 milliseconds to ensure the normal buffering. \n
 *                          There are, however, two exceptions: \n
 *                           0: Indicate to use platform default value depending on the streaming type and network status (default) \n
 *                          -1: Indicate to use current value (since 5.5)
 * @param[in] rebuffer_ms   The time duration of buffering data that must be prerolled to resume playback
 *                          if player is paused for buffering internally. \n
 *                          The value should be more than 1000 milliseconds to ensure the normal buffering. \n
 *                          There are, however, two exceptions: \n
 *                           0: Indicate to use platform default value depending on the streaming type and network status (default) \n
 *                          -1: Indicate to use current value (since 5.5)
 * @return @c 0 on success,
 *         otherwise a negative error value
 * @retval #PLAYER_ERROR_NONE Successful
 * @retval #PLAYER_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #PLAYER_ERROR_INVALID_STATE Invalid state
 * @retval #PLAYER_ERROR_INVALID_OPERATION Invalid operation (since 5.5)
 * @pre The player state must be #PLAYER_STATE_IDLE.
 * @see player_get_streaming_buffering_time()
 * @par Example
 * @code
   #include <player.h>
   bool set_buffering_time(player_h p)
   {
       int err = PLAYER_ERROR_NONE;

       // sets the prebuffer_ms to 5000 milliseconds but does not change the rebuffer_ms
       err =  player_set_streaming_buffering_time(p, 5000, -1);
       if (err != PLAYER_ERROR_NONE) {
          printf("Fail to set buffering time = 0x%x\n", err);
          return false;
       }
       return true;
   }
 * @endcode
 */
int player_set_streaming_buffering_time(player_h player, int prebuffer_ms, int rebuffer_ms);

/**
 * @brief Gets the streaming buffering time.
 * @since_tizen 4.0
 * @param[in]  player        The handle to the media player
 * @param[out] prebuffer_ms  The time duration of buffering data that must be prerolled to start playback
 *                           If the user did not set any value by calling player_set_streaming_buffering_time() function (or if the value was set to 0),
 *                           the value is @c 0 which means platform default value depending on the streaming type and network status. \n
 *                           The value is set to time duration instead of @c 0
 *                           if the player state is one of: #PLAYER_STATE_READY, #PLAYER_STATE_PLAYING or #PLAYER_STATE_PAUSED. (since 5.5)
 * @param[out] rebuffer_ms   The time duration of buffering data that must be prerolled to resume playback
 *                           if player is paused for buffering internally. \n
 *                           If the user did not set any value by calling player_set_streaming_buffering_time() function (or if the value was set to 0),
 *                           the value is @c 0 which means platform default value depending on the streaming type and network status. \n
 *                           The value is set to time duration instead of @c 0
 *                           if the player state is one of: #PLAYER_STATE_READY, #PLAYER_STATE_PLAYING or #PLAYER_STATE_PAUSED. (since 5.5)
 * @return @c 0 on success,
 *         otherwise a negative error value
 * @retval #PLAYER_ERROR_NONE Successful
 * @retval #PLAYER_ERROR_INVALID_PARAMETER Invalid parameter
 * @see player_set_streaming_buffering_time()
 */
int player_get_streaming_buffering_time(player_h player, int *prebuffer_ms, int *rebuffer_ms);

/**
 * @}
 */

/**
 * @addtogroup CAPI_MEDIA_PLAYER_360_MODULE
 * @{
 */

/**
 * @brief Gets information whether the current content of the player is spherical.
 * @since_tizen 5.0
 * @remarks This function is related to the following features:\n
 *          %http://tizen.org/feature/multimedia.player.spherical_video\n
 *          %http://tizen.org/feature/opengles.version.2_0\n
 * @param[in]   player         The handle to the media player
 * @param[out]  is_spherical   The value indicating whether the content is spherical
 * @return @c 0 on success,
 *         otherwise a negative error value
 * @retval #PLAYER_ERROR_NONE              Successful
 * @retval #PLAYER_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #PLAYER_ERROR_INVALID_STATE     Invalid player state
 * @retval #PLAYER_ERROR_INVALID_OPERATION Invalid operation
 * @retval #PLAYER_ERROR_FEATURE_NOT_SUPPORTED_ON_DEVICE Unsupported feature
 * @pre The player state must be one of #PLAYER_STATE_READY, #PLAYER_STATE_PLAYING or #PLAYER_STATE_PAUSED.
 */
int player_360_is_content_spherical(player_h player, bool *is_spherical);

/**
 * @brief Sets the 360 video mode.
 * @details In case the media content is spherical, display mode can be selected by this function.
 * @since_tizen 5.0
 * @remarks This function is related to the following features:\n
 *          %http://tizen.org/feature/multimedia.player.spherical_video\n
 *          %http://tizen.org/feature/opengles.version.2_0\n
 * @param[in] player   The handle to the media player
 * @param[in] enabled  The 360 video display status: @c true = display with 360 video mode,
                       @c false = display with full panorama mode. The default value is @c true.
 * @return @c 0 on success,
 *         otherwise a negative error value
 * @retval #PLAYER_ERROR_NONE              Successful
 * @retval #PLAYER_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #PLAYER_ERROR_INVALID_OPERATION Invalid operation
 * @retval #PLAYER_ERROR_FEATURE_NOT_SUPPORTED_ON_DEVICE Unsupported feature
 * @see player_360_is_enabled()
 */
int player_360_set_enabled(player_h player, bool enabled);

/**
 * @brief Gets the 360 video display mode.
 * @since_tizen 5.0
 * @remarks This function is related to the following features:\n
 *          %http://tizen.org/feature/multimedia.player.spherical_video\n
 *          %http://tizen.org/feature/opengles.version.2_0\n
 * @param[in]   player    The handle to the media player
 * @param[out]  enabled   Pointer to store current 360 video display mode:
 *                        (@c true = display with 360 video mode,
                          @c false = display with full panorama mode)
 * @return @c 0 on success,
 *         otherwise a negative error value
 * @retval #PLAYER_ERROR_NONE              Successful
 * @retval #PLAYER_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #PLAYER_ERROR_INVALID_OPERATION Invalid operation
 * @retval #PLAYER_ERROR_FEATURE_NOT_SUPPORTED_ON_DEVICE Unsupported feature
 * @see player_360_set_enabled()
 */
int player_360_is_enabled(player_h player, bool *enabled);

/**
 * @brief Sets the 360 video direction of view.
 * @details This function is to set horizontal (yaw) and vertical (pitch) angles
 *          of current direction of view in radians. Default direction of view
 *          is taken from meta-data stored in the media. If meta-data omits
 *          these values, zeros are assumed to be equal to the center of the
 *          panorama image.
 * @since_tizen 5.0
 * @remarks This function is related to the following features:\n
 *          %http://tizen.org/feature/multimedia.player.spherical_video\n
 *          %http://tizen.org/feature/opengles.version.2_0\n
 * @param[in] player   The handle to the media player
 * @param[in] yaw      The angle value around vertical axis. Valid values are in
 *                     range [-PI, PI]. Default value is 0.
 * @param[in] pitch    The angle value around lateral axis. Valid values are in
 *                     range [-PI/2, PI/2]. Default value is 0.
 * @return @c 0 on success,
 *         otherwise a negative error value
 * @retval #PLAYER_ERROR_NONE              Successful
 * @retval #PLAYER_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #PLAYER_ERROR_INVALID_OPERATION Invalid operation
 * @retval #PLAYER_ERROR_FEATURE_NOT_SUPPORTED_ON_DEVICE Unsupported feature
 * @see player_360_get_direction_of_view()
 */
int player_360_set_direction_of_view(player_h player, float yaw, float pitch);

/**
 * @brief Gets the 360 video direction of view.
 * @details This function is to get horizontal (yaw) and vertical (pitch) angles
 *          of current direction of view in radians.
 * @since_tizen 5.0
 * @remarks This function is related to the following features:\n
 *          %http://tizen.org/feature/multimedia.player.spherical_video\n
 *          %http://tizen.org/feature/opengles.version.2_0\n
 * @param[in]  player   The handle to the media player
 * @param[out] yaw      Pointer to store current value of direction of view
 *                      angle around vertical axis
 * @param[out] pitch    Pointer to store current value of direction of view
 *                      angle around lateral axis
 * @return @c 0 on success,
 *         otherwise a negative error value
 * @retval #PLAYER_ERROR_NONE              Successful
 * @retval #PLAYER_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #PLAYER_ERROR_INVALID_OPERATION Invalid operation
 * @retval #PLAYER_ERROR_FEATURE_NOT_SUPPORTED_ON_DEVICE Unsupported feature
 * @see player_360_set_direction_of_view()
 */
int player_360_get_direction_of_view(player_h player, float *yaw, float *pitch);

/**
 * @brief Sets the zoom level of 360 video.
 * @details  The zoom means scaling of the flat image cut from the panorama.
 *           The valid range is from 1.0 to 10.0, where 1.0 is the actual image and
 *           values above are zoom-in factor. Default value is 1.0 - no zoom.
 * @since_tizen 5.0
 * @remarks This function is related to the following features:\n
 *          %http://tizen.org/feature/multimedia.player.spherical_video\n
 *          %http://tizen.org/feature/opengles.version.2_0\n
 * @param[in] player    The handle to the media player
 * @param[in] level     The zoom level\n
 *                      Valid range is 1.0~10.0. Default value is 1.0.
 * @return @c 0 on success,
 *         otherwise a negative error value
 * @retval #PLAYER_ERROR_NONE              Successful
 * @retval #PLAYER_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #PLAYER_ERROR_INVALID_OPERATION Invalid operation
 * @retval #PLAYER_ERROR_FEATURE_NOT_SUPPORTED_ON_DEVICE Unsupported feature
 * @see player_360_get_zoom()
 */
int player_360_set_zoom(player_h player, float level);

/**
 * @brief Gets the current zoom level of 360 video.
 * @details  The zoom means scaling of the flat image cut from the panorama.
 *           The valid range is from 1.0 to 10.0. Where 1.0 is actual image and
 *           values above are zoom-in factor. Default value is 1.0 - no zoom.
 * @since_tizen 5.0
 * @remarks This function is related to the following features:\n
 *          %http://tizen.org/feature/multimedia.player.spherical_video\n
 *          %http://tizen.org/feature/opengles.version.2_0\n
 * @param[in]  player    The handle to the media player
 * @param[out] level     Pointer to store current value of zoom level
 * @return @c 0 on success,
 *         otherwise a negative error value
 * @retval #PLAYER_ERROR_NONE              Successful
 * @retval #PLAYER_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #PLAYER_ERROR_INVALID_OPERATION Invalid operation
 * @retval #PLAYER_ERROR_FEATURE_NOT_SUPPORTED_ON_DEVICE Unsupported feature
 * @see player_360_set_zoom()
 */
int player_360_get_zoom(player_h player, float *level);

/**
 * @brief Sets the field of view information of 360 video.
 * @details This function is to set the field of view to decide the output frame size.
 * @since_tizen 5.0
 * @remarks values above the default ones extend the field of view to significantly
 *          distorted areas and will not be useful in most cases.
 * @remarks This function is related to the following features:\n
 *          %http://tizen.org/feature/multimedia.player.spherical_video\n
 *          %http://tizen.org/feature/opengles.version.2_0\n
 * @param[in] player              The handle to the media player
 * @param[in] horizontal_degrees  The horizontal field of view to display in degrees\n
 *                                Valid range is 1~360 degrees. Default value is 120 degrees.
 * @param[in] vertical_degrees    The vertical field of view to display in degrees\n
 *                                Valid range is 1~180 degrees. Default value is 67 degrees.
 * @return @c 0 on success,
 *         otherwise a negative error value
 * @retval #PLAYER_ERROR_NONE              Successful
 * @retval #PLAYER_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #PLAYER_ERROR_INVALID_OPERATION Invalid operation
 * @retval #PLAYER_ERROR_FEATURE_NOT_SUPPORTED_ON_DEVICE Unsupported feature
 * @see player_360_get_field_of_view()
 */
int player_360_set_field_of_view(player_h player, int horizontal_degrees, int vertical_degrees);

/**
 * @brief Gets the field of view information of 360 video.
 * @details This function is to get the field of view information.
 * @since_tizen 5.0
 * @remarks This function is related to the following features:\n
 *          %http://tizen.org/feature/multimedia.player.spherical_video\n
 *          %http://tizen.org/feature/opengles.version.2_0\n
 * @param[in]  player              The handle to the media player
 * @param[out] horizontal_degrees  Pointer to store current value of horizontal
 *                                 field of view to display in degrees.
 * @param[out] vertical_degrees    Pointer to store current value of vertical
 *                                 field of view to display in degrees.
 * @return @c 0 on success,
 *         otherwise a negative error value
 * @retval #PLAYER_ERROR_NONE              Successful
 * @retval #PLAYER_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #PLAYER_ERROR_INVALID_OPERATION Invalid operation
 * @retval #PLAYER_ERROR_FEATURE_NOT_SUPPORTED_ON_DEVICE Unsupported feature
 * @see player_360_set_field_of_view()
 */
int player_360_get_field_of_view(player_h player, int *horizontal_degrees, int *vertical_degrees);

/**
 * @brief Sets the zoom level with field of view information of 360 video.
 * @details This function is provided to reduce the distortion of zoom operation.
 *          The zoom means scaling of the flat image cut from the panorama
 *          which is decided by the field of view data.
 *          The zoom level valid range is from 1.0 to 10.0, where 1.0 is the actual image and
 *          values above are zoom-in factor. Default value is 1.0 - no zoom.
 * @since_tizen 5.0
 * @remarks The degree values above the default ones extend the field of view to significantly
 *          distorted areas and will not be useful in most cases.
 * @remarks This function is related to the following features:\n
 *          %http://tizen.org/feature/multimedia.player.spherical_video\n
 *          %http://tizen.org/feature/opengles.version.2_0\n
 * @param[in] player              The handle to the media player
 * @param[in] level               The zoom level\n
 *                                Valid range is 1.0~10.0. Default value is 1.0.
 * @param[in] horizontal_degrees  The horizontal field of view to display in degrees\n
 *                                Valid range is 1~360 degrees. Default value is 120 degrees.
 * @param[in] vertical_degrees    The vertical field of view to display in degrees\n
 *                                Valid range is 1~180 degrees. Default value is 67 degrees.
 * @return @c 0 on success,
 *         otherwise a negative error value
 * @retval #PLAYER_ERROR_NONE              Successful
 * @retval #PLAYER_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #PLAYER_ERROR_INVALID_OPERATION Invalid operation
 * @retval #PLAYER_ERROR_FEATURE_NOT_SUPPORTED_ON_DEVICE Unsupported feature
 * @see player_360_set_zoom()
 * @see player_360_get_zoom()
 * @see player_360_set_field_of_view()
 * @see player_360_get_field_of_view()
 */
int player_360_set_zoom_with_field_of_view(player_h player, float level, int horizontal_degrees, int vertical_degrees);

/**
 * @}
 */

#ifdef __cplusplus
}
#endif

#endif /* __TIZEN_MEDIA_PLAYER_H__ */
