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

#ifndef __TIZEN_MEDIA_PLAYER_INTERNAL_H__
#define	__TIZEN_MEDIA_PLAYER_INTERNAL_H__
#include <player.h>
#include <glib.h>
#ifdef __cplusplus
extern "C" {
#endif

/**
 * @file player_internal.h
 */

/**
 * @brief Enumeration for wayland window type.
 * @since_tizen 3.0
 */
typedef enum {
	ELM_WAYLAND_WIN = 1,
	ECORE_WAYLAND_WIN
} wl_window_type;

/**
 * @brief Enumeration for video codec type.
 * @since_tizen 4.0
 */
typedef enum {
	PLAYER_VIDEO_CODEC_TYPE_EX_HW,           /**< This is an optional flag for using the h/w codec */
	PLAYER_VIDEO_CODEC_TYPE_EX_SW,           /**< This is an optional flag for using the s/w codec */
	PLAYER_VIDEO_CODEC_TYPE_EX_DEFAULT,      /**< This is an optional flag for using the codec which has higher priority */
} player_video_codec_type_ex_e;

/**
 * @brief Video stream info in external demux case
 * @since_tizen @if TV 3.0 @endif
 */
typedef struct _video_stream_info_s {
	const char *mime;
	unsigned int framerate_num;
	unsigned int framerate_den;
	int format3D;
	unsigned int width;
	unsigned int height;
	unsigned int pixelAspectRatioX;
	unsigned int pixelAspectRatioY;
	unsigned char *codec_extradata;
	unsigned int extradata_size;
	unsigned int version;
	unsigned int colordepth;
	unsigned int max_width;
	unsigned int max_height;
	unsigned int drm_type;
	unsigned int buffer_type;
} player_video_stream_info_s;

/**
 * @brief Audio stream info in external demux case
 * @since_tizen @if TV 3.0 @endif
 */
typedef struct _audio_stream_info_s {
	const char *mime;
	unsigned int channels;
	unsigned int sample_rate;
	unsigned int bit_rate;
	unsigned int block_align;
	unsigned char *codec_extradata;
	unsigned int extradata_size;
	unsigned int version;
	unsigned int user_info;

	/* for pcm, eg. CODEC_ID_PCM_S16LE, width=16,depth=16,endianness=1234,signedness=true */
	unsigned int width;
	unsigned int depth;
	unsigned int endianness;	/* LE:1234, BE:4321 */
	bool signedness;			/* S:true, U:false */
	unsigned int drm_type;
	unsigned int buffer_type;
} player_audio_stream_info_s;

/**
 * @brief Called when the buffer level drops below the min size or exceeds the max size.
 * @since_tizen 3.0
 * @remarks This API is used for media stream playback only.
 * @param[in] status The buffer status
 * @param[in] bytes The current buffer level bytes
 * @param[in] user_data The user data passed from the callback registration function
 * @see player_set_media_stream_buffer_status_cb_ex()
 * @see player_set_media_stream_buffer_max_size()
 * @see player_set_media_stream_buffer_min_threshold()
 */
typedef void (*player_media_stream_buffer_status_cb_ex)(player_media_stream_buffer_status_e status, unsigned long long bytes, void *user_data);

/**
 * @brief Sets the playback rate include streaming mode.
 * @since_tizen 3.0
 * @details The default value is @c 1.0.
 * @remarks No operation is performed, if @a rate is @c 0.
 * @remarks The sound is muted, when playback rate is under @c 0.0 and over @c 2.0.
 * @param[in]   player The handle to the media player
 * @param[in]   rate The playback rate
 * @retval #PLAYER_ERROR_INVALID_OPERATION Invalid operation
 * @retval #PLAYER_ERROR_INVALID_STATE Invalid player state
 * @pre The player state must be set to #PLAYER_STATE_READY by calling player_prepare() or set to #PLAYER_STATE_PLAYING by calling player_start() or set to #PLAYER_STATE_PAUSED by calling player_pause().
 */
int player_set_streaming_playback_rate(player_h player, float rate);

/**
 * @brief Registers a callback function to be invoked when buffer underrun or overflow is occurred.
 * @since_tizen 3.0
 * @remarks This API is used for media stream playback only.
 * @remarks The registered callback is issued in the internal thread of the player.
 * @param[in] player   The handle to the media player
 * @param[in] type     The type of target stream
 * @param[in] callback The buffer status callback function to register
 * @param[in] user_data The user data to be passed to the callback function
 * @return @c 0 on success,
 *         otherwise a negative error value
 * @retval #PLAYER_ERROR_NONE Successful
 * @retval #PLAYER_ERROR_INVALID_STATE Invalid player state
 * @retval #PLAYER_ERROR_INVALID_PARAMETER Invalid parameter
 * @pre The player state must be set to #PLAYER_STATE_IDLE by calling player_create() or player_unprepare().
 * @post player_media_stream_buffer_status_cb_ex() will be invoked.
 * @see player_unset_media_stream_buffer_status_cb_ex()
 * @see player_media_stream_buffer_status_cb_ex()
 */
int player_set_media_stream_buffer_status_cb_ex(player_h player, player_stream_type_e type, player_media_stream_buffer_status_cb_ex callback, void *user_data);

/**
 * @brief Unregisters the buffer status callback function.
 * @since_tizen 3.0
 * @remarks This API is used for media stream playback only.
 * @param[in] player The handle to the media player
 * @param[in] type   The type of target stream
 * @return @c 0 on success,
 *         otherwise a negative error value
 * @retval #PLAYER_ERROR_NONE Successful
 * @retval #PLAYER_ERROR_INVALID_STATE Invalid player state
 * @retval #PLAYER_ERROR_INVALID_PARAMETER Invalid parameter
 * @see player_set_media_stream_buffer_status_cb()
 */
int player_unset_media_stream_buffer_status_cb_ex(player_h player, player_stream_type_e type);

/**
 * @brief Sets DRC(dynamic resolution change) information of media stream data.
 * @since_tizen 3.0
 * @remarks This API is used for media stream playback only.
 *          If the hw codec can not support drc, app have to set DRC information before calling player_prepare().
 * @param[in] player The handle to the media player
 * @param[in] drc The drc status: (@c true = drc, @c false = non drc)
 * @return @c 0 on success,
 *         otherwise a negative error value
 * @retval #PLAYER_ERROR_NONE Successful
 * @retval #PLAYER_ERROR_INVALID_STATE Invalid player state
 * @retval #PLAYER_ERROR_INVALID_PARAMETER Invalid parameter
 * @pre The player state must be set to #PLAYER_STATE_IDLE by calling player_create() or player_unprepare().
 */
int player_set_media_stream_dynamic_resolution(player_h player, bool drc);

/**
 * @brief Sets the ecore wayland video display.
 * @since_tizen 3.0
 * @remarks This API support PLAYER_DISPLAY_TYPE_OVERLAY type only.
 * @param[in]   player The handle to the media player
 * @param[in]   type The display type
 * @param[in]   ecore_wl_window The ecore wayland window handle
 * @param[in]   x the x coordinate of window
 * @param[in]   y the y coordinate of window
 * @param[in]   width the width of window
 * @param[in]   height the height of window
 * @return @c 0 on success,
 *         otherwise a negative error value
 * @retval #PLAYER_ERROR_NONE Successful
 * @retval #PLAYER_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #PLAYER_ERROR_INVALID_OPERATION Invalid operation
 * @retval #PLAYER_ERROR_INVALID_STATE Invalid player state
 * @pre The player state must be one of #PLAYER_STATE_IDLE, #PLAYER_STATE_READY, #PLAYER_STATE_PLAYING, or #PLAYER_STATE_PAUSED.
 * @see player_set_display_rotation
 */
int player_set_ecore_wl_display(player_h player, player_display_type_e type, void *ecore_wl_window, int x, int y, int width, int height);

/**
 * @brief Enumeration for tizen-core-wl display type.
 */
typedef enum {
	PLAYER_DISPLAY_TYPE_TCORE_OVERLAY,           /**< Video overlay using tizen_core_wl_window_h */
	PLAYER_DISPLAY_TYPE_TCORE_OVERLAY_SYNC_UI,   /**< Sync UI video overlay using handle string from tizen_core_wl_video_shell_surface_get_handle() */
} player_display_type_tcore_e;

/**
 * @brief Sets the tizen-core-wl display for video overlay.
 * @details Sets the display for video rendering using tizen-core-wl.
 *          For #PLAYER_DISPLAY_TYPE_TCORE_OVERLAY, @a display must be a tizen_core_wl_window_h handle.
 *          For #PLAYER_DISPLAY_TYPE_TCORE_OVERLAY_SYNC_UI, @a display must be the handle string
 *          obtained from tizen_core_wl_video_shell_surface_get_handle().
 * @param[in] player The player handle
 * @param[in] type The tizen-core-wl display type
 * @param[in] display The display handle (type depends on @a type)
 * @return @c 0 on success, otherwise a negative error value
 * @retval #PLAYER_ERROR_NONE Successful
 * @retval #PLAYER_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #PLAYER_ERROR_INVALID_OPERATION Invalid operation
 * @pre The player state must be one of #PLAYER_STATE_IDLE, #PLAYER_STATE_READY, #PLAYER_STATE_PLAYING, or #PLAYER_STATE_PAUSED.
 */
int player_set_tcore_display(player_h player, player_display_type_tcore_e type, void *display);

/**
 * @brief Called when no free space in buffer.
 * @since_tizen @if TV 3.0 @endif
 * @param[in] user_data    The user data passed from the callback registration function
 * @see player_set_buffer_enough_video_data_cb()
 * @see player_set_buffer_enough_audio_data_cb()
 */
typedef void (*player_buffer_enough_data_cb)(void *user_data);

/**
 * @brief Called when the buffer level drops below the threshold.
 * @since_tizen @if TV 3.0 @endif
 * @param[in] size         Required data size
 * @param[in] user_data    The user data passed from the callback registration function
 * @see player_set_buffer_need_video_data_cb()
 * @see player_set_buffer_need_audio_data_cb()
 */
typedef void (*player_buffer_need_data_cb)(unsigned int size, void *user_data);

/**
 * @brief Submits packet to appsrc, used in external demux mode
 * @since_tizen @if TV 3.0 @endif
 * @param[in]  player   The handle to media player
 * @param[in]  buf      the buf want to submit to appsrc, usually one buffer one frame
 * @param[in]  len      the size of buf
 * @param[in]  pts      pts of the submit buf(frame)
 * @param[in]  streamtype   true for video, false for audio
 * @return 0 on success, otherwise a negative error value.
 */
int player_submit_packet(player_h player, guint8 *buf, guint32 len, guint64 pts, player_stream_type_e streamtype);

/**
 * @brief Sets video stream info, especially used in external demux mode
 * @since_tizen @if TV 3.0 @endif
 * @param[in]  player   The handle to media player
 * @param[in]  info     the info of video stream set by external demux
 * @return 0 on success, otherwise a negative error value.
 */
int player_set_video_stream_info(player_h player, player_video_stream_info_s *info);

/**
 * @brief Sets audio stream info, especially used in external demux mode
 * @since_tizen @if TV 3.0 @endif
 * @param[in]  player   The handle to media player
 * @param[in]  info     the info of audio stream set by external demux
 * @return 0 on success, otherwise a negative error value.
 */
int player_set_audio_stream_info(player_h player, player_audio_stream_info_s *info);

/**
 * @brief Registers the callback function.
 * @since_tizen @if TV 3.0 @endif
 * @remarks The registered callback is issued in the internal thread of the player.
 * @param[in] player The handle to media player
 * @param[in] callback  The callback function to register
 * @param[in] user_data The user data to be passed to the callback function
 * @return 0 on success, otherwise a negative error value.
 * @retval #PLAYER_ERROR_NONE Successful
 * @retval #PLAYER_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #PLAYER_ERROR_INVALID_OPERATION Invalid operation
 */
int player_set_buffer_enough_video_data_cb(player_h player, player_buffer_enough_data_cb callback, void *user_data);

/**
 * @brief Registers the callback function.
 * @since_tizen @if TV 3.0 @endif
 * @remarks The registered callback is issued in the internal thread of the player.
 * @param[in] player The handle to media player
 * @param[in] callback  The callback function to register
 * @param[in] user_data The user data to be passed to the callback function
 * @return 0 on success, otherwise a negative error value.
 * @retval #PLAYER_ERROR_NONE Successful
 * @retval #PLAYER_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #PLAYER_ERROR_INVALID_OPERATION Invalid operation
 */
int player_set_buffer_enough_audio_data_cb(player_h player, player_buffer_enough_data_cb callback, void *user_data);

/**
 * @brief Registers the callback function.
 * @since_tizen @if TV 3.0 @endif
 * @remarks The registered callback is issued in the internal thread of the player.
 * @param[in] player The handle to media player
 * @param[in] callback  The callback function to register
 * @param[in] user_data The user data to be passed to the callback function
 * @return 0 on success, otherwise a negative error value.
 * @retval #PLAYER_ERROR_NONE Successful
 * @retval #PLAYER_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #PLAYER_ERROR_INVALID_OPERATION Invalid operation
 */
int player_set_buffer_need_video_data_cb(player_h player, player_buffer_need_data_cb callback, void *user_data);

/**
 * @brief Registers the callback function.
 * @since_tizen @if TV 3.0 @endif
 * @remarks The registered callback is issued in the internal thread of the player.
 * @param[in] player The handle to media player
 * @param[in] callback  The callback function to register
 * @param[in] user_data The user data to be passed to the callback function
 * @return 0 on success, otherwise a negative error value.
 * @retval #PLAYER_ERROR_NONE Successful
 * @retval #PLAYER_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #PLAYER_ERROR_INVALID_OPERATION Invalid operation
 */
int player_set_buffer_need_audio_data_cb(player_h player, player_buffer_need_data_cb callback, void *user_data);

/**
 * @brief Display the first video frame at the player_prepare() function to improve the playback performance.
 * @since_tizen @if TV 3.0 @endif
 * @details This is only available when you want to start playback from 0 position.
 *	        1. Normal play : player_create() -> player_display_video_at_paused_state() -> player_prepare()[unmute] -> player_start()
 *			2. Resume play : player_create() -> player_prepare() -> player_set_position() -> player_start()[unmute]
 * @param[in] player    The handle to media player
 * @param[in] visible   The visibility of the display (@c true = visible, @c false = non-visible )
 * @pre The player state must be : #PLAYER_STATE_IDLE
*/
int player_display_video_at_paused_state(player_h player, bool visible);

/**
 * @brief Sets the win_ind to player
 * @since_tizen @if TV 3.0 @endif
 * @param[in] player    The handle to media player
 * @param[in] win_id    windows handle
 * @pre The player state must be : #PLAYER_STATE_IDLE
 */
int player_set_display_parent_win_id(player_h player, int win_id);

/**
 * @brief Sets the data source (http or local file) to transit.
 * @since_tizen @if TV @else 3.0 @endif
 *
 * @details Player will transit the media source as seamlessly as possible after finishing current playback.
 *          If the function call is successful, the uri will be added at the end of uri list.
 *          You can add several uris and the set of uris will be played repeatedly if you call the player_set_looping().
 *          The first uri in the list is the original uri which is set by player_set_uri().
 *
 * @remarks Uri list will be cleared except first one when the player_unprepare() is called.
 *          If you call the player_set_uri(), you can change the first uri too.
 *
 * @param[in]   player The handle to the media player
 * @param[in]   uri Specifies the data location.
 *
 * @return @c 0 on success,
 *         otherwise a negative error value
 * @retval #PLAYER_ERROR_NONE Successful
 * @retval #PLAYER_ERROR_INVALID_OPERATION Invalid operation
 * @retval #PLAYER_ERROR_INVALID_STATE Invalid player state
 * @pre The original uri must be set first by player_set_uri().
 * @pre The player state must be one of #PLAYER_STATE_IDLE, #PLAYER_STATE_READY, #PLAYER_STATE_PLAYING, or #PLAYER_STATE_PAUSED.
 * @see player_set_uri(), player_set_looping()
 */
int player_set_next_uri(player_h player, const char *uri);

/**
 * @brief Gets the data source path.
 * @since_tizen @if TV @else 3.0 @endif
 *
 * @param[in]   player The handle to the media player
 * @param[out]  uri Specifies the next data location.
 *
 * @return @c 0 on success,
 *         otherwise a negative error value
 * @retval #PLAYER_ERROR_NONE Successful
 * @retval #PLAYER_ERROR_INVALID_OPERATION Invalid operation
 * @retval #PLAYER_ERROR_INVALID_STATE Invalid player state
 * @pre The player state must be one of #PLAYER_STATE_IDLE, #PLAYER_STATE_READY, #PLAYER_STATE_PLAYING, or #PLAYER_STATE_PAUSED.
 * @see player_set_uri()
 */
int player_get_next_uri(player_h player, char **uri);

/**
 * @brief Sets the gapless playback status
 * @since_tizen @if TV @else 3.0 @endif
 * @details If it is @c true, player will play next content without(or with minimized) gap.
 *          If it is @c false, it won't. The default value is @c false.
 * @param[in] player   The handle to the media player
 * @param[in] gapless  The new gapless status : (@c true = gapless, @c false = non-gapless )
 * @return @c 0 on success,
 *         otherwise a negative error value
 * @retval #PLAYER_ERROR_NONE Successful
 * @retval #PLAYER_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #PLAYER_ERROR_INVALID_STATE Invalid state
 * @pre The player state must be one of #PLAYER_STATE_IDLE, #PLAYER_STATE_READY, #PLAYER_STATE_PLAYING, or #PLAYER_STATE_PAUSED.
 * @see	player_is_gapless()
 */
int player_set_gapless(player_h player, bool gapless);

/**
 * @brief Gets the player's gapless status.
 * @since_tizen @if TV @else 3.0 @endif
 * @details If it is @c true, player will play next content without(or with minimized) gap.
 *          If it is @c false, it won't. The default value is @c false.
 * @param[in]   player The handle to the media player
 * @param[out]  gapless The gapless status: (@c true = gapless, @c false = non-gapless )
 * @return @c 0 on success,
 *         otherwise a negative error value
 * @retval #PLAYER_ERROR_NONE Successful
 * @retval #PLAYER_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #PLAYER_ERROR_INVALID_OPERATION Invalid operation
 * @retval #PLAYER_ERROR_INVALID_STATE Invalid player state
 * @pre The player state must be one of #PLAYER_STATE_IDLE, #PLAYER_STATE_READY, #PLAYER_STATE_PLAYING, or #PLAYER_STATE_PAUSED.
 * @see player_set_gapless()
 */
int player_is_gapless(player_h player, bool *gapless);

/**
 * @brief Enables the tbm surface pool.
 * @since_tizen @if TV @else 3.0 @endif
 * @details If it is @c true, tbm surface will be enabled to share the video frame.
 *          If it is @c false, it won't. The default value is @c false.
 * @param[in] player  The handle to the media player
 * @param[in] enable  The new tbm surface status
 * @return @c 0 on success,
 *         otherwise a negative error value
 * @retval #PLAYER_ERROR_NONE Successful
 * @retval #PLAYER_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #PLAYER_ERROR_INVALID_STATE Invalid state
 * @pre The player state must be #PLAYER_STATE_IDLE.
 * @see	player_is_enabled_tsurf_pool()
 */
int player_enable_tsurf_pool(player_h player, bool enable);

/**
 * @brief Gets the player's tbm surface pool enable status.
 * @since_tizen @if TV @else 3.0 @endif
 * @details If it is @c true, tbm surface pool is enabled to share the video frame.
 *          If it is @c false, it won't. The default value is @c false.
 * @param[in]   player The handle to the media player
 * @param[out]  enabled The tbm surface enable status
 * @return @c 0 on success,
 *         otherwise a negative error value
 * @retval #PLAYER_ERROR_NONE Successful
 * @retval #PLAYER_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #PLAYER_ERROR_INVALID_OPERATION Invalid operation
 * @retval #PLAYER_ERROR_INVALID_STATE Invalid player state
 * @pre The player state must be one of #PLAYER_STATE_IDLE, #PLAYER_STATE_READY, #PLAYER_STATE_PLAYING, or #PLAYER_STATE_PAUSED.
 * @see player_enable_tsurf_pool()
 */
int player_is_enabled_tsurf_pool(player_h player, bool *enabled);

/**
 * @brief Gets the size of video frame pool.
 * @since_tizen @if TV @else 3.0 @endif
 * @details App gets the video frame pool size which will be reused during playback.
 * @param[in] player The handle to the media player
 * @param[out] size The size of surface pool
 * @return @c 0 on success,
 *         otherwise a negative error value
 * @retval #PLAYER_ERROR_NONE Successful
 * @retval #PLAYER_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #PLAYER_ERROR_INVALID_OPERATION Invalid operation
 * @retval #PLAYER_ERROR_INVALID_STATE Invalid player state
 * @pre The player state must be one of #PLAYER_STATE_READY, #PLAYER_STATE_PLAYING, or #PLAYER_STATE_PAUSED
 * @see player_set_media_packet_video_frame_decoded_cb()
 */
int player_get_media_packet_video_frame_pool_size(player_h player, int *size);

 /**
 * @brief Enables media_packet callback
 * @since_tizen @if TV @else 3.0 @endif
 * @details If it is @c true, media_packet callback will be activated.
 *          If it is @c false, it will be deactivated. The default value is @c false.
 * @param[in] player   The handle to the media player
 * @param[in] enable   The active status : (@c true = send media_packet, @c false = render through overlay surface)
 * @return @c 0 on success,
 *         otherwise a negative error value
 * @retval #PLAYER_ERROR_NONE Successful
 * @retval #PLAYER_ERROR_INVALID_PARAMETER Invalid parameter
 * @pre The player state must be one of #PLAYER_STATE_IDLE, #PLAYER_STATE_READY, #PLAYER_STATE_PLAYING, or #PLAYER_STATE_PAUSED.
 * @pre display_type must be set to PLAYER_DISPLAY_TYPE_OVERLAY by player_set_display.
 * @pre player_media_packet_video_decoded_cb must be set.
 * @see player_set_display()
 * @see player_set_media_packet_video_frame_decoded_cb()
 */
int player_enable_media_packet_video_frame_decoded_cb(player_h player, bool enable);

/**
 * @brief Set video codec type as h/w codec or s/w codec.
 * @since_tizen 5.5
 * @details The default codec type of the player is #PLAYER_CODEC_TYPE_DEFAULT.
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
 * @see player_get_video_codec_type_ex()
 */
int player_set_video_codec_type_ex(player_h player, player_video_codec_type_ex_e codec_type);

/**
 * @brief Get video codec type.
 * @since_tizen 5.5
 * @param[in]  player      The handle to the media player
 * @param[out] codec_type  The codec type
 * @return @c 0 on success,
 *         otherwise a negative error value
 * @retval #PLAYER_ERROR_NONE Successful
 * @retval #PLAYER_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #PLAYER_ERROR_INVALID_OPERATION Invalid operation
 * @retval #PLAYER_ERROR_INVALID_STATE Invalid player state
 * @pre The player state must be one of these: #PLAYER_STATE_IDLE, #PLAYER_STATE_READY, #PLAYER_STATE_PLAYING, or #PLAYER_STATE_PAUSED.
 * @see player_set_video_codec_type_ex()
 */
int player_get_video_codec_type_ex(player_h player, player_video_codec_type_ex_e *codec_type);

#ifdef __cplusplus
}
#endif

#endif /*__TIZEN_MEDIA_PLAYER_INTERNAL_H__*/
