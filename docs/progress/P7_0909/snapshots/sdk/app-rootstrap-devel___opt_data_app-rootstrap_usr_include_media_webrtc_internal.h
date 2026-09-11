/*
 * Copyright (c) 2020 Samsung Electronics Co., Ltd All Rights Reserved
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

#ifndef __TIZEN_MEDIA_WEBRTC_INTERNAL_H__
#define __TIZEN_MEDIA_WEBRTC_INTERNAL_H__

#include <webrtc.h>

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

/**
* @file webrtc_internal.h
* @brief This file contains the WebRTC internal API.
*/

/**
* @addtogroup CAPI_MEDIA_WEBRTC_MODULE
* @{
*/

/**
 * @internal
 * @brief WebRTC signaling server handle type.
 * @since_tizen 6.5
 */
typedef void *webrtc_signaling_server_h;

/**
 * @internal
 * @brief WebRTC signaling client handle type.
 * @since_tizen 6.5
 */
typedef void *webrtc_signaling_client_h;

/**
 * @internal
 * @brief Enumeration for WebRTC media source type for internal use.
 * @since_tizen 6.5
 */
typedef enum {
	WEBRTC_MEDIA_SOURCE_TYPE_CUSTOM_AUDIO = 8,  /**<  Custom audio */
	WEBRTC_MEDIA_SOURCE_TYPE_CUSTOM_VIDEO,      /**<  Custom video */
} webrtc_media_source_type_internal_e;

/**
 * @internal
 * @brief Enumeration for WebRTC snapshot format.
 * @since_tizen 7.0
 */
typedef enum {
	WEBRTC_SNAPSHOT_FORMAT_RAW_RGB24 = 0x01,    /**< Raw 24-bit RGB */
	WEBRTC_SNAPSHOT_FORMAT_JPEG      = 0x11,    /**< JPEG */
	WEBRTC_SNAPSHOT_FORMAT_PNG       = 0x12,    /**< PNG */
} webrtc_snapshot_format_e;

/**
 * @internal
 * @brief Definition for snapshot option.
 * @since_tizen 7.0
 * @see webrtc_take_snapshot()
 */
#define WEBRTC_SNAPSHOT_OPTION_NONE    -1

/**
 * @internal
 * @brief Enumeration for WebRTC signaling message type.
 * @since_tizen 6.5
 */
typedef enum {
	SIGNALING_MESSAGE_TYPE_CONNECTED,           /**< Connected */
	SIGNALING_MESSAGE_TYPE_DISCONNECTED,        /**< Disconnected */
	SIGNALING_MESSAGE_TYPE_SESSION_ESTABLISHED, /**< Session established */
	SIGNALING_MESSAGE_TYPE_SESSION_CLOSED,      /**< Session closed */
	SIGNALING_MESSAGE_TYPE_SDP,                 /**< SDP */
	SIGNALING_MESSAGE_TYPE_ICE_CANDIDATE,       /**< ICE candidate */
	SIGNALING_MESSAGE_TYPE_ERROR,               /**< Error */
} webrtc_signaling_message_type_e;


/**
 * @internal
 * @brief Called when an image is captured by webrtc_take_snapshot().
 * @since_tizen 7.0
 * @remarks The @a webrtc is the same object for which the callback was set.\n
 *          The @a webrtc should not be released.\n
 *          The @a data should not be released. The @a data can be used only in the callback. To use outside, make a copy.
 * @param[in] webrtc     WebRTC handle
 * @param[in] track_id   The track id
 * @param[in] format     The snapshot image format
 * @param[in] data       The snapshot image data captured from @a track_id
 * @param[in] width      The snapshot image width
 * @param[in] height     The snapshot image height
 * @param[in] size       The size of @a data
 * @param[in] user_data  The user data passed from the callback registration function
 * @see webrtc_take_snapshot()
 */
typedef void (*webrtc_snapshot_cb)(webrtc_h webrtc, unsigned int track_id, webrtc_snapshot_format_e format, const char *data, int width, int height, unsigned int size, void *user_data);

/**
 * @internal
 * @brief Called when a message to be handled is sent from the remote peer or the signaling server.
 * @since_tizen 6.5
 * @param[in] type       The signaling message type
 * @param[in] message    The message
 * @param[in] user_data  The user data passed from the callback registration function
 * @see webrtc_signaling_connect_server()
 */
typedef void (*webrtc_signaling_message_cb)(webrtc_signaling_message_type_e type, const char *message, void *user_data);

/**
 * @internal
 * @brief Sets an ecore wayland display to the media track.
 * @since_tizen 6.5
 * @remarks Call this function within webrtc_track_added_cb(), otherwise #WEBRTC_ERROR_INVALID_OPERATION will be returned.
 * @param[in] webrtc           WebRTC handle
 * @param[in] track_id         The track id
 * @param[in] ecore_wl_window  The ecore wayland window handle
 * @return @c 0 on success,
 *         otherwise a negative error value
 * @retval #WEBRTC_ERROR_NONE    Successful
 * @retval #WEBRTC_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #WEBRTC_ERROR_INVALID_OPERATION Invalid operation
 * @pre webrtc_track_added_cb() must be set by calling webrtc_set_track_added_cb().
 * @see webrtc_set_track_added_cb()
 * @see webrtc_unset_track_added_cb()
 */
int webrtc_set_ecore_wl_display(webrtc_h webrtc, unsigned int track_id, void *ecore_wl_window);

/**
 * @internal
 * @brief Sets a display surface id to the video track to be rendered.
 * @since_tizen 8.0
 * @remarks Call this function within webrtc_track_added_cb(), otherwise #WEBRTC_ERROR_INVALID_OPERATION will be returned.\n
 *          If webrtc_set_encoded_video_frame_cb() has been called, it will return #WEBRTC_ERROR_INVALID_OPERATION.
 * @param[in] webrtc      WebRTC handle
 * @param[in] track_id    The track id
 * @param[in] surface_id  The display surface id
 * @param[in] x           The start X coordinate of the display surface
 * @param[in] y           The start Y coordinate of the display surface
 * @param[in] width       Width of the display surface
 * @param[in] height      Height of the display surface
 * @return @c 0 on success,
 *         otherwise a negative error value
 * @retval #WEBRTC_ERROR_NONE    Successful
 * @retval #WEBRTC_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #WEBRTC_ERROR_INVALID_OPERATION Invalid operation
 * @pre webrtc_track_added_cb() must be set by calling webrtc_set_track_added_cb().
 * @see webrtc_set_track_added_cb()
 * @see webrtc_unset_track_added_cb()
 * @see webrtc_set_display_mode()
 * @see webrtc_get_display_mode()
 * @see webrtc_set_display_visible()
 * @see webrtc_get_display_visible()
 */
int webrtc_set_display_surface_id(webrtc_h webrtc, unsigned int track_id, int surface_id, int x, int y, int width, int height);

/**
 * @internal
 * @brief Sets a display quality of service.
 * @since_tizen 7.0
 * @remarks Call this function within webrtc_track_added_cb(), otherwise #WEBRTC_ERROR_INVALID_OPERATION will be returned.\n
 *          If webrtc_set_encoded_video_frame_cb() has been called, it will return #WEBRTC_ERROR_INVALID_OPERATION.\n
 *          It'll set qos property of video sink that generates Quality-of-Service events upstream in some cases.\n
 *          The default value is @c true.
 * @param[in] webrtc      WebRTC handle
 * @param[in] track_id    The track id
 * @param[in] enable      Enable or not (@c true = enable, @c false = disable)
 * @return @c 0 on success,
 *         otherwise a negative error value
 * @retval #WEBRTC_ERROR_NONE    Successful
 * @retval #WEBRTC_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #WEBRTC_ERROR_INVALID_OPERATION Invalid operation
 * @pre webrtc_track_added_cb() must be set by calling webrtc_set_track_added_cb().
 * @pre webrtc_set_display() or webrtc_set_display_surface_id() must be called before calling this function.
 * @see webrtc_set_track_added_cb()
 * @see webrtc_unset_track_added_cb()
 */
int webrtc_set_display_qos(webrtc_h webrtc, unsigned int track_id, bool enable);

/**
 * @brief Sets sw video decoder forcedly regardless of system configuration.
 * @since_tizen 7.0
 * @remarks Call this function within webrtc_track_added_cb(), otherwise #WEBRTC_ERROR_INVALID_OPERATION will be returned.\n
 *          If webrtc_set_encoded_video_frame_cb() has been called, it will return #WEBRTC_ERROR_INVALID_OPERATION.
 * @param[in] webrtc      WebRTC handle
 * @param[in] track_id    The track id
 * @param[in] enable      Enable or not (@c true = use sw video decoder, @c false = follow system configuration)
 * @return @c 0 on success,
 *         otherwise a negative error value
 * @retval #WEBRTC_ERROR_NONE    Successful
 * @retval #WEBRTC_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #WEBRTC_ERROR_INVALID_OPERATION Invalid operation
 * @pre webrtc_track_added_cb() must be set by calling webrtc_set_track_added_cb().
 * @see webrtc_set_track_added_cb()
 * @see webrtc_unset_track_added_cb()
 */
int webrtc_set_sw_video_decoder(webrtc_h webrtc, unsigned int track_id, bool enable);

/**
 * @internal
 * @brief Gets the video resolution of the video track.
 * @since_tizen 7.0
 * @param[in] webrtc      WebRTC handle
 * @param[in] track_id    The track id
 * @param[out] width      The video width
 * @param[out] height     The video height
 * @return @c 0 on success,
 *         otherwise a negative error value
 * @retval #WEBRTC_ERROR_NONE    Successful
 * @retval #WEBRTC_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #WEBRTC_ERROR_INVALID_OPERATION Invalid operation
 * @pre webrtc_track_added_cb() must be set by calling webrtc_set_track_added_cb().
 */
int webrtc_get_video_resolution(webrtc_h webrtc, unsigned int track_id, int *width, int *height);

/**
 * @internal
 * @brief Takes a snapshot from the video track asynchronously.
 * @since_tizen 7.0
 * @remarks The registered callback will be invoked in an internal thread of the webrtc.\n
 *          If webrtc_set_encoded_video_frame_cb() has been called, it will return #WEBRTC_ERROR_INVALID_OPERATION.\n
 *          @a option only affects when @a format is #WEBRTC_SNAPSHOT_FORMAT_JPEG or #WEBRTC_SNAPSHOT_FORMAT_PNG,
 *          otherwise #WEBRTC_SNAPSHOT_OPTION_NONE can be used.\n
 *          If @a format is #WEBRTC_SNAPSHOT_FORMAT_JPEG, @a option is for quality level which range from 1 to 100.\n
 *          If @a format is #WEBRTC_SNAPSHOT_FORMAT_PNG, @a option is for compression level which range from 0 to 9.\n
 * @param[in] webrtc      WebRTC handle
 * @param[in] track_id    The track id
 * @param[in] format      The snapshot image format
 * @param[in] option      The option of @a format
 * @param[in] callback    Callback function pointer
 * @param[in] user_data   The user data to be passed to the callback function
 * @return @c 0 on success,
 *         otherwise a negative error value
 * @retval #WEBRTC_ERROR_NONE    Successful
 * @retval #WEBRTC_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #WEBRTC_ERROR_INVALID_OPERATION Invalid operation
 * @pre webrtc_track_added_cb() must be set by calling webrtc_set_track_added_cb().
 * @post webrtc_snapshot_cb() will be invoked.
 */
int webrtc_take_snapshot(webrtc_h webrtc, unsigned int track_id, webrtc_snapshot_format_e format, int option, webrtc_snapshot_cb callback, void *user_data);

/**
 * @internal
 * @brief Sets a video loopback to render the video frames of the media source to an ecore wayland display.
 * @details The following media source types are available for this function:\n
 *          #WEBRTC_MEDIA_SOURCE_TYPE_VIDEOTEST\n
 *          #WEBRTC_MEDIA_SOURCE_TYPE_CAMERA\n
 *          #WEBRTC_MEDIA_SOURCE_TYPE_SCREEN\n
 *          #WEBRTC_MEDIA_SOURCE_TYPE_FILE
 * @since_tizen 6.5
 * @param[in] webrtc           WebRTC handle
 * @param[in] source_id        The video source id
 * @param[in] ecore_wl_window  The ecore wayland window handle
 * @param[out] track_id        The track id (optional, this can be NULL)
 * @return @c 0 on success,
 *         otherwise a negative error value
 * @retval #WEBRTC_ERROR_NONE    Successful
 * @retval #WEBRTC_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #WEBRTC_ERROR_INVALID_OPERATION Invalid operation
 * @pre Add media source to @a webrtc to get @a source_id by calling webrtc_add_media_source().
 */
int webrtc_media_source_set_video_loopback_to_ecore_wl(webrtc_h webrtc, unsigned int source_id, void *ecore_wl_window, unsigned int *track_id);

/**
 * @internal
 * @brief Adds an internal media source.
 * @since_tizen 6.5
 * @remarks #WEBRTC_ERROR_INVALID_STATE will no longer occur. (Since 9.0)
 * @param[in] webrtc      WebRTC handle
 * @param[in] type        The media source type to be added
 * @param[out] source_id  The media source id
 * @return @c 0 on success,
 *         otherwise a negative error value
 * @retval #WEBRTC_ERROR_NONE    Successful
 * @retval #WEBRTC_ERROR_NOT_SUPPORTED Not supported
 * @retval #WEBRTC_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #WEBRTC_ERROR_INVALID_OPERATION Invalid operation
 * @see webrtc_remove_media_source()
 * @see webrtc_media_source_set_transceiver_direction()
 * @see webrtc_media_source_get_transceiver_direction()
 * @see webrtc_media_source_set_pause()
 * @see webrtc_media_source_get_pause()
 * @see webrtc_media_source_set_mute()
 * @see webrtc_media_source_get_mute()
 * @see webrtc_media_source_set_video_resolution()
 * @see webrtc_media_source_get_video_resolution()
 */
int webrtc_add_media_source_internal(webrtc_h webrtc, webrtc_media_source_type_internal_e type, unsigned int *source_id);

/**
 * @internal
 * @brief Adds the transceiver encoding option to the media source.
 * @since_tizen 8.0
 * @remarks If @a source_id should be a media source of #WEBRTC_MEDIA_SOURCE_TYPE_AUDIOTEST or #WEBRTC_MEDIA_SOURCE_TYPE_MIC or #WEBRTC_MEDIA_SOURCE_TYPE_VIDEOTEST or #WEBRTC_MEDIA_SOURCE_TYPE_NULL,
 *          otherwise this function will return #WEBRTC_ERROR_INVALID_PARAMETER.\n
 *          If @a source_id is a media source of #WEBRTC_MEDIA_SOURCE_TYPE_NULL, @a target_bitrate, @a width, and @a height will be ignored.\n
 *          If @a source_id is a media source of #WEBRTC_MEDIA_SOURCE_TYPE_NULL, webrtc_media_source_set_transceiver_codec() must be called before calling this function.\n
 *          If @a media_type is for #WEBRTC_MEDIA_TYPE_AUDIO, @a width and @a height will be ignored.
 * @param[in] webrtc      WebRTC handle
 * @param[in] source_id   The media source id
 * @param[in] media_type  The media type
 * @param[in] rid         The RID (RTP Stream Identifier)
 * @param[in] target_bitrate  The target bitrate of the encoder
 * @param[in] width       The video width (video only)
 * @param[in] height      The video height (video only)
 * @param[out] ssrc       The SSRC (optional, this can be NULL)
 * @return @c 0 on success,
 *         otherwise a negative error value
 * @retval #WEBRTC_ERROR_NONE    Successful
 * @retval #WEBRTC_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #WEBRTC_ERROR_INVALID_OPERATION Invalid operation
 * @pre Add media source to @a webrtc to get @a source_id by calling webrtc_add_media_source().
 * @see webrtc_media_source_remove_transceiver_encoding()
 */
int webrtc_media_source_add_transceiver_encoding(webrtc_h webrtc, unsigned int source_id, webrtc_media_type_e media_type, const char *rid, int target_bitrate, int width, int height, int *ssrc);

/**
 * @internal
 * @brief Removes the transceiver encoding option from the media source.
 * @since_tizen 8.0
 * @param[in] webrtc      WebRTC handle
 * @param[in] source_id   The media source id
 * @param[in] media_type  The media type
 * @param[in] rid         The RID (RTP Stream Identifier)
 * @return @c 0 on success,
 *         otherwise a negative error value
 * @retval #WEBRTC_ERROR_NONE    Successful
 * @retval #WEBRTC_ERROR_INVALID_PARAMETER Invalid parameter
 * @pre Add media source to @a webrtc to get @a source_id by calling webrtc_add_media_source().
 * @see webrtc_media_source_add_transceiver_encoding()
 */
int webrtc_media_source_remove_transceiver_encoding(webrtc_h webrtc, unsigned int source_id, webrtc_media_type_e media_type, const char *rid);

/**
 * @internal
 * @brief Actives the transceiver encoding option of the media source.
 * @since_tizen 8.0
 * @remarks The default value is @c true.
 * @param[in] webrtc      WebRTC handle
 * @param[in] source_id   The media source id
 * @param[in] media_type  The media type
 * @param[in] rid         The RID (RTP Stream Identifier)
 * @param[in] active      Active or not (@c true = active, @c false = inactive)
 * @return @c 0 on success,
 *         otherwise a negative error value
 * @retval #WEBRTC_ERROR_NONE    Successful
 * @retval #WEBRTC_ERROR_INVALID_PARAMETER Invalid parameter
 * @pre Add media source to @a webrtc to get @a source_id by calling webrtc_add_media_source().
 * @pre webrtc_media_source_add_transceiver_encoding() must be called before calling this function.
 * @see webrtc_media_source_add_transceiver_encoding()
 * @see webrtc_media_source_remove_transceiver_encoding()
 */
int webrtc_media_source_active_transceiver_encoding(webrtc_h webrtc, unsigned int source_id, webrtc_media_type_e media_type, const char *rid, bool active);

/**
 * @brief Gets the transceiver media id of the media source.
 * @since_tizen 8.0
 * @remarks The @a mid should be released using free() if the value is not NULL.
 * @param[in] webrtc      WebRTC handle
 * @param[in] source_id   The media source id
 * @param[in] media_type  The media type
 * @param[out] mid        The transceiver media id
 * @return @c 0 on success,
 *         otherwise a negative error value
 * @retval #WEBRTC_ERROR_NONE    Successful
 * @retval #WEBRTC_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #WEBRTC_ERROR_INVALID_OPERATION Invalid operation
 * @pre Add media source to @a webrtc to get @a source_id by calling webrtc_add_media_source().
 * @pre Call webrtc_start() before calling this function, otherwise NULL can be set to @a mid.
 */
int webrtc_media_source_get_transceiver_mid(webrtc_h webrtc, unsigned int source_id, webrtc_media_type_e media_type, char **mid);

/**
 * @internal
 * @brief Sets drop of the receiving packets from the transceiver of the media source.
 * @since_tizen 8.0
 * @remarks If @a drop is set to @c true, playback of media received from a remote peer will be dropped without decoding.\n
 *          If the transceiver of @a source_id has the #WEBRTC_TRANSCEIVER_DIRECTION_SENDONLY direction, #WEBRTC_ERROR_INVALID_OPERATION will be returned.
 * @param[in] webrtc      WebRTC handle
 * @param[in] source_id   The source id
 * @param[in] media_type  The media type
 * @param[in] drop        Drop or not (@c true = drop, @c false = not drop)
 * @return @c 0 on success,
 *         otherwise a negative error value
 * @retval #WEBRTC_ERROR_NONE    Successful
 * @retval #WEBRTC_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #WEBRTC_ERROR_INVALID_OPERATION Invalid operation
 * @see webrtc_media_source_get_transceiver_recv_drop()
 */
int webrtc_media_source_set_transceiver_recv_drop(webrtc_h webrtc, unsigned int source_id, webrtc_media_type_e media_type, bool drop);

/**
 * @internal
 * @brief Gets the drop state of the receiving packets from the transceiver of the media source.
 * @since_tizen 8.0
 * @remarks The default value is @c false.
 * @param[in] webrtc      WebRTC handle
 * @param[in] source_id   The track id
 * @param[in] media_type  The media type
 * @param[out] dropped    Dropped or not (@c true = dropped, @c false = not dropped)
 * @return @c 0 on success,
 *         otherwise a negative error value
 * @retval #WEBRTC_ERROR_NONE    Successful
 * @retval #WEBRTC_ERROR_INVALID_PARAMETER Invalid parameter
 * @see webrtc_media_source_set_transceiver_recv_drop()
 */
int webrtc_media_source_get_transceiver_recv_drop(webrtc_h webrtc, unsigned int source_id, webrtc_media_type_e media_type, bool *dropped);

/**
 * @internal
 * @brief Gets the media type of the media source.
 * @since_tizen 8.0
 * @remarks If @a source_id is a media source of #WEBRTC_MEDIA_SOURCE_TYPE_FILE, this function will return #WEBRTC_ERROR_INVALID_PARAMETER.
 * @param[in] webrtc      WebRTC handle
 * @param[in] source_id   The media source id
 * @param[out] media_type The media type
 * @return @c 0 on success,
 *         otherwise a negative error value
 * @retval #WEBRTC_ERROR_NONE    Successful
 * @retval #WEBRTC_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #WEBRTC_ERROR_INVALID_OPERATION Invalid operation
 * @pre Add media source to @a webrtc to get @a source_id by calling webrtc_add_media_source().
 */
int webrtc_media_source_get_type(webrtc_h webrtc, unsigned int source_id, webrtc_media_type_e *media_type);

/**
 * @internal
 * @brief Sets the media type to the null source.
 * @since_tizen 9.0
 * @remarks If @a source_id is not a media source of #WEBRTC_MEDIA_SOURCE_TYPE_NULL, this function will return #WEBRTC_ERROR_INVALID_PARAMETER.
 * @param[in] webrtc      WebRTC handle
 * @param[in] source_id   The null source id
 * @param[in] media_type  The media type
 * @return @c 0 on success,
 *         otherwise a negative error value
 * @retval #WEBRTC_ERROR_NONE    Successful
 * @retval #WEBRTC_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #WEBRTC_ERROR_INVALID_OPERATION Invalid operation
 * @pre Add media source of #WEBRTC_MEDIA_SOURCE_TYPE_NULL to @a webrtc to get @a source_id by calling webrtc_add_media_source().
 */
int webrtc_null_source_set_media_type(webrtc_h webrtc, unsigned int source_id, webrtc_media_type_e media_type);

/**
 * @internal
 * @brief Enables all the supported codecs on the media source so they can be represented in the SDP description.
 * @since_tizen 11.0
 * @remarks If @a source_id is a media source of #WEBRTC_MEDIA_SOURCE_TYPE_FILE or #WEBRTC_MEDIA_SOURCE_TYPE_NULL,
 *          this function will return #WEBRTC_ERROR_INVALID_PARAMETER.\n
 * @param[in] webrtc      WebRTC handle
 * @param[in] source_id   The source id
 * @param[in] media_type  The media type
 * @return @c 0 on success,
 *         otherwise a negative error value
 * @retval #WEBRTC_ERROR_NONE    Successful
 * @retval #WEBRTC_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #WEBRTC_ERROR_INVALID_OPERATION Invalid operation
 * @pre Add media source to @a webrtc to get @a source_id by calling webrtc_add_media_source().
 * @see webrtc_media_source_foreach_supported_transceiver_codec()
 * @see webrtc_media_source_set_payload_type()
 */
int webrtc_media_source_enable_all_supported_codecs(webrtc_h webrtc, unsigned int source_id, webrtc_media_type_e media_type);

/**
 * @internal
 * @brief Sets the probability of RTP packet dropping.
 * @since_tizen 7.0
 * @param[in] webrtc       WebRTC handle
 * @param[in] sender       Sender or receiver
 * @param[in] probability  The probability to be dropped (from @c 0 to @c 1.0 = 100%)
 * @return @c 0 on success,
 *         otherwise a negative error value
 * @retval #WEBRTC_ERROR_NONE    Successful
 * @retval #WEBRTC_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #WEBRTC_ERROR_INVALID_OPERATION Invalid operation
 * @see webrtc_get_rtp_packet_drop_probability()
 */
int webrtc_set_rtp_packet_drop_probability(webrtc_h webrtc, bool sender, float probability);

/**
 * @internal
 * @brief Gets the probability of RTP packet dropping.
 * @since_tizen 7.0
 * @remarks The default value is 0.
 * @param[in] webrtc        WebRTC handle
 * @param[in] sender        Sender or receiver
 * @param[out] probability  The probability to be dropped (from @c 0 to @c 1.0 = 100%)
 * @return @c 0 on success,
 *         otherwise a negative error value
 * @retval #WEBRTC_ERROR_NONE    Successful
 * @retval #WEBRTC_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #WEBRTC_ERROR_INVALID_OPERATION Invalid operation
 * @see webrtc_set_rtp_packet_drop_probability()
 */
int webrtc_get_rtp_packet_drop_probability(webrtc_h webrtc, bool sender, float *probability);

/**
 * @internal
 * @brief Strips the session description.
 * @since_tizen 8.0
 * @remarks @a origin_description is a JSON string. It should be {"sdp":{"type":"offer or answer","sdp":"..."}}.\n
 *          @a description will be the string member of the "sdp" object in @a origin_description.\n
 *          The @a description should be released using free().
 * @param[in] origin_description The original session description
 * @param[out] description       The stripped session description
 * @return @c 0 on success,
 *         otherwise a negative error value
 * @retval #WEBRTC_ERROR_NONE    Successful
 * @retval #WEBRTC_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #WEBRTC_ERROR_INVALID_OPERATION Invalid operation
 * @see webrtc_util_create_description()
 */
int webrtc_util_strip_description(const char *origin_description, char **description);

/**
 * @internal
 * @brief Create the session description from sdp string.
 * @since_tizen 8.0
 * @remarks @a sdp should be a string that follows the sdp format like "v=0\r\no=- 1832017678083131571 0 IN IP4 0.0.0.0\r\ns=-\r\n...".\n
 *          @a description is a JSON string. It will be {"sdp":{"type":"offer or answer","sdp":"..."}}.\n
 *          The @a description should be released using free().
 * @param[in] offer         SDP type (@c true = offer, @c false = answer)
 * @param[in] sdp           SDP string
 * @param[out] description  The session description
 * @return @c 0 on success,
 *         otherwise a negative error value
 * @retval #WEBRTC_ERROR_NONE    Successful
 * @retval #WEBRTC_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #WEBRTC_ERROR_INVALID_OPERATION Invalid operation
 * @see webrtc_util_strip_description()
 */
int webrtc_util_create_description(bool offer, const char *sdp, char **description);

/**
 * @internal
 * @brief Creates a signaling server for private network.
 * @since_tizen 6.5
 * @remarks The @a server should be released using webrtc_signaling_server_destroy().
 * @param[in] port           The port number
 * @param[out] server        Signaling server handle
 * @return @c 0 on success,
 *         otherwise a negative error value
 * @retval #WEBRTC_ERROR_NONE    Successful
 * @retval #WEBRTC_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #WEBRTC_ERROR_INVALID_OPERATION Invalid operation
 * @see webrtc_signaling_server_start()
 * @see webrtc_signaling_server_stop()
 * @see webrtc_signaling_server_destroy()
 */
int webrtc_signaling_server_create(int port, webrtc_signaling_server_h *server);

/**
 * @internal
 * @brief Starts the signaling server.
 * @since_tizen 6.5
 * @param[in] server        Signaling server handle
 * @return @c 0 on success,
 *         otherwise a negative error value
 * @retval #WEBRTC_ERROR_NONE    Successful
 * @retval #WEBRTC_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #WEBRTC_ERROR_INVALID_OPERATION Invalid operation
 * @pre Call webrtc_signaling_server_create() before calling this function.
 * @see webrtc_signaling_server_stop()
 * @see webrtc_signaling_server_destroy()
 */
int webrtc_signaling_server_start(webrtc_signaling_server_h server);

/**
 * @internal
 * @brief Stops the signaling server.
 * @since_tizen 6.5
 * @param[in] server        Signaling server handle
 * @return @c 0 on success,
 *         otherwise a negative error value
 * @retval #WEBRTC_ERROR_NONE    Successful
 * @retval #WEBRTC_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #WEBRTC_ERROR_INVALID_OPERATION Invalid operation
 * @pre Call webrtc_signaling_server_start() before calling this function.
 * @see webrtc_signaling_server_start()
 * @see webrtc_signaling_server_destroy()
 */
int webrtc_signaling_server_stop(webrtc_signaling_server_h server);

/**
 * @internal
 * @brief Destroys the signaling server.
 * @since_tizen 6.5
 * @param[in] server        Signaling server handle
 * @return @c 0 on success,
 *         otherwise a negative error value
 * @retval #WEBRTC_ERROR_NONE    Successful
 * @retval #WEBRTC_ERROR_INVALID_PARAMETER Invalid parameter
 * @pre Call webrtc_signaling_server_create() before calling this function.
 * @see webrtc_signaling_server_start()
 * @see webrtc_signaling_server_stop()
 */
int webrtc_signaling_server_destroy(webrtc_signaling_server_h server);

/**
 * @internal
 * @brief Connects to a signaling server for private network.
 * @since_tizen 6.5
 * @remarks The @a client should be released using webrtc_signaling_disconnect().
 * @param[in] server_ip      The signaling server IP address
 * @param[in] port           The port number
 * @param[in] callback       Callback function pointer
 * @param[in] user_data      The user data to be passed to the callback function
 * @param[out] client        Signaling client handle
 * @return @c 0 on success,
 *         otherwise a negative error value
 * @retval #WEBRTC_ERROR_NONE    Successful
 * @retval #WEBRTC_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #WEBRTC_ERROR_INVALID_OPERATION Invalid operation
 * @see webrtc_signaling_get_id()
 * @see webrtc_signaling_request_session()
 * @see webrtc_signaling_send_message()
 * @see webrtc_signaling_disconnect()
 */
int webrtc_signaling_connect(const char *server_ip, int port, webrtc_signaling_message_cb callback, void *user_data, webrtc_signaling_client_h *client);

/**
 * @internal
 * @brief Requests session to the peer id.
 * @since_tizen 6.5
 * @param[in] client         Signaling client handle
 * @param[in] peer_id        The peer client handle id
 * @return @c 0 on success,
 *         otherwise a negative error value
 * @retval #WEBRTC_ERROR_NONE    Successful
 * @retval #WEBRTC_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #WEBRTC_ERROR_INVALID_OPERATION Invalid operation
 * @pre Call webrtc_signaling_connect_server() before calling this function.
 * @see webrtc_signaling_get_id()
 * @see webrtc_signaling_send_message()
 */
int webrtc_signaling_request_session(webrtc_signaling_client_h client, int peer_id);

/**
 * @internal
 * @brief Sends message to the peer client.
 * @since_tizen 6.5
 * @param[in] client         Signaling client handle
 * @param[in] message        The message
 * @return @c 0 on success,
 *         otherwise a negative error value
 * @retval #WEBRTC_ERROR_NONE    Successful
 * @retval #WEBRTC_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #WEBRTC_ERROR_INVALID_OPERATION Invalid operation
 * @retval #WEBRTC_ERROR_INVALID_STATE Invalid state
 * @pre Call webrtc_signaling_connect_server() before calling this function.
 * @pre Call webrtc_signaling_request_session() before calling this function.
 * @see webrtc_signaling_get_id()
 * @see webrtc_signaling_request_session()
 */
int webrtc_signaling_send_message(webrtc_signaling_client_h client, const char *message);

/**
 * @internal
 * @brief Gets the id of the signaling client handle.
 * @since_tizen 6.5
 * @param[in] client         Signaling client handle
 * @param[out] id            Signaling client handle id
 * @return @c 0 on success,
 *         otherwise a negative error value
 * @retval #WEBRTC_ERROR_NONE    Successful
 * @retval #WEBRTC_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #WEBRTC_ERROR_INVALID_OPERATION Invalid operation
 * @pre Call webrtc_signaling_connect_server() before calling this function.
 * @see webrtc_signaling_request_session()
 * @see webrtc_signaling_send_message()
 */
int webrtc_signaling_get_id(webrtc_signaling_client_h client, int *id);

/**
 * @internal
 * @brief Disconnects from the signaling server.
 * @since_tizen 6.5
 * @param[in] client         Signaling client handle
 * @return @c 0 on success,
 *         otherwise a negative error value
 * @retval #WEBRTC_ERROR_NONE    Successful
 * @retval #WEBRTC_ERROR_INVALID_PARAMETER Invalid parameter
 * @pre Call webrtc_signaling_connect_server() before calling this function.
 * @see webrtc_signaling_get_id()
 * @see webrtc_signaling_request_session()
 * @see webrtc_signaling_send_message()
 */
int webrtc_signaling_disconnect(webrtc_signaling_client_h client);

/**
 * @}
 */

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* __TIZEN_MEDIA_WEBRTC_INTERNAL_H__ */
