/*
* Copyright (c) 2019 Samsung Electronics Co., Ltd All Rights Reserved
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

#ifndef __TIZEN_MEDIA_SCMIRRORING_PRIMARY_SINK_H__
#define __TIZEN_MEDIA_SCMIRRORING_PRIMARY_SINK_H__

#include <scmirroring_type.h>
#include <scmirroring_internal.h>
#include <scmirroring_type_internal.h>

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

/**
 * @file scmirroring_primary_sink.h
 * @brief This file contains APIs and functions related with screen mirroring as primary sink device.
 */

/**
 * @addtogroup CAPI_MEDIA_SCREEN_MIRRORING_MODULE
 * @{
 */

/**
 * @brief Creates a new screen mirroring primary sink handle.
 * @since_tizen 5.5
 *
 * @remarks You must release @a scmirroring_primary_sink using scmirroring_primary_sink_destroy().
 *
 * @param[out] scmirroring_primary_sink	A newly returned handle to the screen mirroring primary sink
 * @return @c 0 on success,
 *         otherwise a negative error value
 * @retval #SCMIRRORING_ERROR_NONE Successful
 * @retval #SCMIRRORING_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #SCMIRRORING_ERROR_OUT_OF_MEMORY Out of memory
 * @retval #SCMIRRORING_ERROR_INVALID_OPERATION Invalid operation
 * @retval #SCMIRRORING_ERROR_NOT_SUPPORTED Not supported
 * @retval #SCMIRRORING_ERROR_UNKNOWN Unknown Error
 *
 * @post The screen mirroring state will be #SCMIRRORING_SINK_STATE_NULL
 *
 * @see scmirroring_primary_sink_destroy()
 */
int scmirroring_primary_sink_create(scmirroring_primary_sink_h *scmirroring_primary_sink);

/**
 * @brief Registers a callback function to be called when state change happens.
 * @details This function registers user callback and this callback is called when each status is changed.
 *
 * @since_tizen 5.5
 *
 * @param[in] scmirroring_primary_sink The handle to the screen mirroring primary sink
 * @param[in] callback The callback function to invoke
 * @param[in] user_data The user data passed to the callback registration function
 * @return @c 0 on success,
 *         otherwise a negative error value
 * @retval #SCMIRRORING_ERROR_NONE Successful
 * @retval #SCMIRRORING_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #SCMIRRORING_ERROR_OUT_OF_MEMORY Out of memory
 * @retval #SCMIRRORING_ERROR_INVALID_OPERATION Invalid operation
 * @retval #SCMIRRORING_ERROR_PERMISSION_DENIED Permission denied
 * @retval #SCMIRRORING_ERROR_NOT_SUPPORTED Not supported
 * @retval #SCMIRRORING_ERROR_UNKNOWN Unknown Error
 *
 * @pre Create a screen mirroring primary sink handle by calling scmirroring_primary_sink_create().
 *
 * @see scmirroring_primary_sink_create()
 */
int scmirroring_primary_sink_set_state_changed_cb(scmirroring_primary_sink_h scmirroring_primary_sink, scmirroring_sink_state_cb callback, void *user_data);

/**
 * @brief Sets server IP and port.
 *
 * @since_tizen 5.5
 *
 * @param[in] scmirroring_primary_sink The handle to the screen mirroring primary sink
 * @param[in] ip The server IP address to connect to
 * @param[in] port The server port to connect to
 * @return @c 0 on success,
 *         otherwise a negative error value
 * @retval #SCMIRRORING_ERROR_NONE Successful
 * @retval #SCMIRRORING_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #SCMIRRORING_ERROR_OUT_OF_MEMORY Out of memory
 * @retval #SCMIRRORING_ERROR_INVALID_OPERATION Invalid operation
 * @retval #SCMIRRORING_ERROR_PERMISSION_DENIED Permission denied
 * @retval #SCMIRRORING_ERROR_NOT_SUPPORTED Not supported
 * @retval #SCMIRRORING_ERROR_UNKNOWN Unknown Error
 *
 * @pre Create a screen mirroring primary sink handle by calling scmirroring_primary_sink_create().
 *
 * @see scmirroring_primary_sink_create()
 */
int scmirroring_primary_sink_set_ip_and_port(scmirroring_primary_sink_h scmirroring_primary_sink, const char *ip, const char *port);

/**
 * @brief Pass window handle created by application and surface type(x11/evas).
 * @details This function will use handle created by the application to set the overlay &
 *          display on the surface passed by the application
 *
 * @since_tizen 5.5
 *
 * @param[in] scmirroring_primary_sink The handle to the screen mirroring primary sink
 * @param[in] type Surface type(x11/evas)
 * @param[in] display_surface The display_surface created by application to force sink to display content over it
 * @return @c 0 on success,
 *         otherwise a negative error value
 * @retval #SCMIRRORING_ERROR_NONE Successful
 * @retval #SCMIRRORING_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #SCMIRRORING_ERROR_OUT_OF_MEMORY Out of memory
 * @retval #SCMIRRORING_ERROR_INVALID_OPERATION Invalid operation
 * @retval #SCMIRRORING_ERROR_PERMISSION_DENIED Permission denied
 * @retval #SCMIRRORING_ERROR_NOT_SUPPORTED Not supported
 * @retval #SCMIRRORING_ERROR_UNKNOWN Unknown Error
 *
 * @pre Create a screen mirroring primary sink handle by calling scmirroring_primary_sink_create().
 *
 * @see scmirroring_primary_sink_create()
 */
int scmirroring_primary_sink_set_display(scmirroring_primary_sink_h scmirroring_primary_sink, scmirroring_display_type_e type, void *display_surface);

/**
 * @brief Sets resolutions of screen mirroring primary sink.
 * @details This function sets resolutions of screen mirroring primary sink using scmirroring_resolution_e as following.
 *          (ex. SCMIRRORING_RESOLUTION_1920x1080_P30 | SCMIRRORING_RESOLUTION_1280x720_P30)
 *          Use it only when you want to set specific resolutions but if screen mirroring source dose not support
 *          the resolutions which you set, the screen mirroring primary sink will be disconnected.
 *
 * @since_tizen 5.5
 *
 * @param[in] scmirroring_primary_sink The handle to the screen mirroring primary sink
 * @param[in] resolution Resolution of screen mirroring primary sink
 * @return @c 0 on success,
 *         otherwise a negative error value
 * @retval #SCMIRRORING_ERROR_NONE Successful
 * @retval #SCMIRRORING_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #SCMIRRORING_ERROR_OUT_OF_MEMORY Out of memory
 * @retval #SCMIRRORING_ERROR_INVALID_OPERATION Invalid operation
 * @retval #SCMIRRORING_ERROR_PERMISSION_DENIED Permission denied
 * @retval #SCMIRRORING_ERROR_NOT_SUPPORTED Not supported
 * @retval #SCMIRRORING_ERROR_UNKNOWN Unknown Error
 *
 * @pre Create a screen mirroring primary sink handle by calling scmirroring_primary_sink_create().
 *
 * @see scmirroring_primary_sink_create()
 */
int scmirroring_primary_sink_set_resolution(scmirroring_primary_sink_h scmirroring_primary_sink, int resolution);

/**
 * @brief Prepares the screen mirroring primary sink handle and allocates specific resources.
 * @since_tizen 5.5
 *
 * @param[in] scmirroring_primary_sink The handle to the screen mirroring primary sink
 * @return @c 0 on success,
 *         otherwise a negative error value
 * @retval #SCMIRRORING_ERROR_NONE Successful
 * @retval #SCMIRRORING_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #SCMIRRORING_ERROR_OUT_OF_MEMORY Out of memory
 * @retval #SCMIRRORING_ERROR_INVALID_OPERATION Invalid operation
 * @retval #SCMIRRORING_ERROR_NOT_SUPPORTED Not supported
 * @retval #SCMIRRORING_ERROR_UNKNOWN Unknown Error
 *
 * @pre Create a screen mirroring primary sink handle by calling scmirroring_primary_sink_create().
 * @pre Register user callback by calling scmirroring_primary_sink_set_state_changed_cb().
 * @pre The screen mirroring state should be #SCMIRRORING_SINK_STATE_NULL
 * @post The screen mirroring state will be #SCMIRRORING_SINK_STATE_PREPARED
 *
 * @see scmirroring_primary_sink_create()
 * @see scmirroring_primary_sink_set_state_changed_cb()
 */
int scmirroring_primary_sink_prepare(scmirroring_primary_sink_h scmirroring_primary_sink);

/**
 * @brief Creates connection and prepare for receiving data from SCMIRRORING source.
 *
 * @since_tizen 5.5
 * @privlevel public
 * @privilege %http://tizen.org/privilege/internet
 *
 * @param[in] scmirroring_primary_sink The handle to the screen mirroring primary sink
 * @return @c 0 on success,
 *         otherwise a negative error value
 * @retval #SCMIRRORING_ERROR_NONE Successful
 * @retval #SCMIRRORING_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #SCMIRRORING_ERROR_OUT_OF_MEMORY Out of memory
 * @retval #SCMIRRORING_ERROR_INVALID_OPERATION Invalid operation
 * @retval #SCMIRRORING_ERROR_PERMISSION_DENIED Permission denied
 * @retval #SCMIRRORING_ERROR_NOT_SUPPORTED Not supported
 * @retval #SCMIRRORING_ERROR_UNKNOWN Unknown Error
 *
 * @pre Create a screen mirroring primary sink handle by calling scmirroring_primary_sink_create().
 * @pre Register user callback by calling scmirroring_primary_sink_set_state_changed_cb().
 * @pre Call scmirroring_primary_sink_prepare()
 * @pre The screen mirroring state should be #SCMIRRORING_SINK_STATE_PREPARED
 * @post The screen mirroring state will be #SCMIRRORING_SINK_STATE_CONNECTED
 *
 * @see scmirroring_primary_sink_create()
 * @see scmirroring_primary_sink_set_state_changed_cb()
 * @see scmirroring_primary_sink_prepare()
 */
int scmirroring_primary_sink_connect(scmirroring_primary_sink_h scmirroring_primary_sink);

/**
 * @brief Starts receiving data from the SCMIRRORING source and display it(mirror).
 *
 * @since_tizen 5.5
 * @privlevel public
 * @privilege %http://tizen.org/privilege/internet
 *
 * @param[in] scmirroring_primary_sink The handle to the screen mirroring primary sink
 * @return @c 0 on success,
 *         otherwise a negative error value
 * @retval #SCMIRRORING_ERROR_NONE Successful
 * @retval #SCMIRRORING_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #SCMIRRORING_ERROR_OUT_OF_MEMORY Out of memory
 * @retval #SCMIRRORING_ERROR_INVALID_OPERATION Invalid operation
 * @retval #SCMIRRORING_ERROR_PERMISSION_DENIED Permission denied
 * @retval #SCMIRRORING_ERROR_NOT_SUPPORTED Not supported
 * @retval #SCMIRRORING_ERROR_UNKNOWN Unknown Error
 *
 * @pre Create a screen mirroring primary sink handle by calling scmirroring_primary_sink_create().
 * @pre Register user callback by calling scmirroring_primary_sink_set_state_changed_cb().
 * @pre Call scmirroring_primary_sink_prepare()
 * @pre Call scmirroring_primary_sink_connect()
 * @pre The screen mirroring state should be #SCMIRRORING_SINK_STATE_CONNECTED
 * @post The screen mirroring state will be #SCMIRRORING_SINK_STATE_PLAYING
 *
 * @see scmirroring_primary_sink_create()
 * @see scmirroring_primary_sink_set_state_changed_cb()
 * @see scmirroring_primary_sink_prepare()
 * @see scmirroring_primary_sink_connect()
 */
int scmirroring_primary_sink_start(scmirroring_primary_sink_h scmirroring_primary_sink);

/**
 * @brief Pauses receiving data from the SCMIRRORING source.
 * @details This function pauses receiving data from the SCMIRRORING source,
 *	which means it sends RTSP PAUSE message to source.
 *
 * @since_tizen 5.5
 * @privlevel public
 * @privilege %http://tizen.org/privilege/internet
 *
 * @param[in] scmirroring_primary_sink The handle to the screen mirroring primary sink
 * @return @c 0 on success,
 *         otherwise a negative error value
 * @retval #SCMIRRORING_ERROR_NONE Successful
 * @retval #SCMIRRORING_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #SCMIRRORING_ERROR_OUT_OF_MEMORY Out of memory
 * @retval #SCMIRRORING_ERROR_INVALID_OPERATION Invalid operation
 * @retval #SCMIRRORING_ERROR_PERMISSION_DENIED Permission denied
 * @retval #SCMIRRORING_ERROR_NOT_SUPPORTED Not supported
 * @retval #SCMIRRORING_ERROR_UNKNOWN Unknown Error
 *
 * @pre The screen mirroring state should be #SCMIRRORING_SINK_STATE_PLAYING
 * @post The screen mirroring state will be #SCMIRRORING_SINK_STATE_PAUSED
 *
 * @see scmirroring_primary_sink_create()
 * @see scmirroring_primary_sink_set_state_changed_cb()
 * @see scmirroring_primary_sink_prepare()
 * @see scmirroring_primary_sink_connect()
 * @see scmirroring_primary_sink_start()
 */
int scmirroring_primary_sink_pause(scmirroring_primary_sink_h scmirroring_primary_sink);

/**
 * @brief Resumes receiving data from the SCMIRRORING source.
 * @details This function pauses receiving data from the SCMIRRORING source, which means it sends RTSP PLAY message to source.
 *
 * @since_tizen 5.5
 * @privlevel public
 * @privilege %http://tizen.org/privilege/internet
 *
 * @param[in] scmirroring_primary_sink The handle to the screen mirroring primary sink
 * @return @c 0 on success,
 *         otherwise a negative error value
 * @retval #SCMIRRORING_ERROR_NONE Successful
 * @retval #SCMIRRORING_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #SCMIRRORING_ERROR_OUT_OF_MEMORY Out of memory
 * @retval #SCMIRRORING_ERROR_INVALID_OPERATION Invalid operation
 * @retval #SCMIRRORING_ERROR_PERMISSION_DENIED Permission denied
 * @retval #SCMIRRORING_ERROR_NOT_SUPPORTED Not supported
 * @retval #SCMIRRORING_ERROR_UNKNOWN Unknown Error
 *
 * @pre The screen mirroring state should be #SCMIRRORING_SINK_STATE_PAUSED
 * @post The screen mirroring state will be #SCMIRRORING_SINK_STATE_PLAYING
 *
 * @see scmirroring_primary_sink_pause()
 */
int scmirroring_primary_sink_resume(scmirroring_primary_sink_h scmirroring_primary_sink);

/**
 * @brief Disconnects and stops receiving data from the SCMIRRORING source.
 *
 * @since_tizen 5.5
 * @privlevel public
 * @privilege %http://tizen.org/privilege/internet
 *
 * @param[in] scmirroring_primary_sink The handle to the screen mirroring primary sink
 * @return @c 0 on success,
 *         otherwise a negative error value
 * @retval #SCMIRRORING_ERROR_NONE Successful
 * @retval #SCMIRRORING_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #SCMIRRORING_ERROR_OUT_OF_MEMORY Out of memory
 * @retval #SCMIRRORING_ERROR_INVALID_OPERATION Invalid operation
 * @retval #SCMIRRORING_ERROR_PERMISSION_DENIED Permission denied
 * @retval #SCMIRRORING_ERROR_NOT_SUPPORTED Not supported
 * @retval #SCMIRRORING_ERROR_UNKNOWN Unknown Error
 *
 * @pre The screen mirroring state should be #SCMIRRORING_SINK_STATE_CONNECTED
 *  or #SCMIRRORING_SINK_STATE_PLAYING or #SCMIRRORING_SINK_STATE_PAUSED
 * @post The screen mirroring state will be #SCMIRRORING_SINK_STATE_DISCONNECTED
 *
 * @see scmirroring_primary_sink_create()
 * @see scmirroring_primary_sink_set_state_changed_cb()
 * @see scmirroring_primary_sink_prepare()
 * @see scmirroring_primary_sink_connect()
 * @see scmirroring_primary_sink_start()
 */
int scmirroring_primary_sink_disconnect(scmirroring_primary_sink_h scmirroring_primary_sink);

/**
 * @brief Unprepares screen mirroring.
 * @details This function unprepares screen mirroring, which closes specific resources.
 *
 * @since_tizen 5.5
 *
 * @param[in] scmirroring_primary_sink The handle to the screen mirroring primary sink
 * @return @c 0 on success,
 *         otherwise a negative error value
 * @retval #SCMIRRORING_ERROR_NONE Successful
 * @retval #SCMIRRORING_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #SCMIRRORING_ERROR_OUT_OF_MEMORY Out of memory
 * @retval #SCMIRRORING_ERROR_INVALID_OPERATION Invalid operation
 * @retval #SCMIRRORING_ERROR_NOT_SUPPORTED Not supported
 * @retval #SCMIRRORING_ERROR_UNKNOWN Unknown Error
 *
 * @pre Create a screen mirroring primary sink handle by calling scmirroring_primary_sink_create().
 * @pre Register user callback by calling scmirroring_primary_sink_set_state_changed_cb().
 * @pre Call scmirroring_primary_sink_prepare()
 * @post The screen mirroring state will be #SCMIRRORING_SINK_STATE_NULL
 *
 * @see scmirroring_primary_sink_create()
 * @see scmirroring_primary_sink_set_state_changed_cb()
 * @see scmirroring_primary_sink_prepare()
 */
int scmirroring_primary_sink_unprepare(scmirroring_primary_sink_h scmirroring_primary_sink);

/**
 * @brief Unregisters the callback function user registered.
 *
 * @since_tizen 5.5
 *
 * @param[in] scmirroring_primary_sink The handle to the screen mirroring primary sink
 * @return @c 0 on success,
 *         otherwise a negative error value
 * @retval #SCMIRRORING_ERROR_NONE Successful
 * @retval #SCMIRRORING_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #SCMIRRORING_ERROR_INVALID_OPERATION Invalid operation
 * @retval #SCMIRRORING_ERROR_PERMISSION_DENIED Permission denied
 * @retval #SCMIRRORING_ERROR_NOT_SUPPORTED Not supported
 * @retval #SCMIRRORING_ERROR_UNKNOWN Unknown Error
 *
 * @pre Create a screen mirroring primary sink handle by calling scmirroring_primary_sink_create().
 * @pre Register user callback by calling scmirroring_primary_sink_set_state_changed_cb().
 *
 * @see scmirroring_primary_sink_create()
 * @see scmirroring_primary_sink_set_state_changed_cb()
 */
int scmirroring_primary_sink_unset_state_changed_cb(scmirroring_primary_sink_h scmirroring_primary_sink);

/**
 * @brief Destroys screen mirroring primary sink handle.
 *
 * @since_tizen 5.5
 *
 * @param[in] scmirroring_primary_sink The handle to the screen mirroring primary sink
 * @return @c 0 on success,
 *         otherwise a negative error value
 * @retval #SCMIRRORING_ERROR_NONE Successful
 * @retval #SCMIRRORING_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #SCMIRRORING_ERROR_INVALID_OPERATION Invalid operation
 * @retval #SCMIRRORING_ERROR_PERMISSION_DENIED Permission denied
 * @retval #SCMIRRORING_ERROR_NOT_SUPPORTED Not supported
 * @retval #SCMIRRORING_ERROR_UNKNOWN Unknown Error
 *
 * @pre Create a screen mirroring primary sink handle by calling scmirroring_primary_sink_create().
 * @pre The screen mirroring state should be #SCMIRRORING_SINK_STATE_NULL
 *
 * @see scmirroring_primary_sink_create()
 */
int scmirroring_primary_sink_destroy(scmirroring_primary_sink_h scmirroring_primary_sink);

/**
 * @brief Gets negotiated video codec of screen mirroring primary sink.
 * @details The video codec is negotiated by screen mirroring source.
 *
 * @since_tizen 5.5
 *
 * @param[in] scmirroring_primary_sink The handle to the screen mirroring primary sink
 * @param[out] codec Codec of video
 * @return @c 0 on success,
 *         otherwise a negative error value
 * @retval #SCMIRRORING_ERROR_NONE Successful
 * @retval #SCMIRRORING_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #SCMIRRORING_ERROR_INVALID_OPERATION Invalid operation
 * @retval #SCMIRRORING_ERROR_NOT_SUPPORTED Not supported
 * @retval #SCMIRRORING_ERROR_UNKNOWN Unknown Error
 *
 * @pre Create a screen mirroring primary sink handle by calling scmirroring_primary_sink_create().
 * @pre Register user callback by calling scmirroring_primary_sink_set_state_changed_cb().
 * @pre Call scmirroring_primary_sink_prepare()
 * @pre Call scmirroring_primary_sink_connect()
 * @pre The screen mirroring state must be #SCMIRRORING_SINK_STATE_CONNECTED or #SCMIRRORING_SINK_STATE_PLAYING
 */
int scmirroring_primary_sink_get_negotiated_video_codec(scmirroring_primary_sink_h scmirroring_primary_sink, scmirroring_video_codec_e *codec);

/**
 * @brief Gets negotiated video resolution of screen mirroring primary sink.
 * @details The video resolution is negotiated by screen mirroring source.
 *
 * @since_tizen 5.5
 *
 * @param[in] scmirroring_primary_sink The handle to the screen mirroring primary sink
 * @param[out] width Width of video
 * @param[out] height Height of video
 * @return @c 0 on success,
 *         otherwise a negative error value
 * @retval #SCMIRRORING_ERROR_NONE Successful
 * @retval #SCMIRRORING_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #SCMIRRORING_ERROR_INVALID_OPERATION Invalid operation
 * @retval #SCMIRRORING_ERROR_NOT_SUPPORTED Not supported
 * @retval #SCMIRRORING_ERROR_UNKNOWN Unknown Error
 *
 * @pre Create a screen mirroring primary sink handle by calling scmirroring_primary_sink_create().
 * @pre Register user callback by calling scmirroring_primary_sink_set_state_changed_cb().
 * @pre Call scmirroring_primary_sink_prepare()
 * @pre Call scmirroring_primary_sink_connect()
 * @pre The screen mirroring state must be #SCMIRRORING_SINK_STATE_CONNECTED or #SCMIRRORING_SINK_STATE_PLAYING
 */
int scmirroring_primary_sink_get_negotiated_video_resolution(scmirroring_primary_sink_h scmirroring_primary_sink, int *width, int *height);

/**
 * @brief Gets negotiated frame rate of screen mirroring primary sink.
 * @details The video frame rate is negotiated by screen mirroring source.
 *
 * @since_tizen 5.5
 *
 * @param[in] scmirroring_primary_sink The handle to the screen mirroring primary sink
 * @param[out] frame_rate Frame rate of video
 * @return @c 0 on success,
 *         otherwise a negative error value
 * @retval #SCMIRRORING_ERROR_NONE Successful
 * @retval #SCMIRRORING_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #SCMIRRORING_ERROR_INVALID_OPERATION Invalid operation
 * @retval #SCMIRRORING_ERROR_NOT_SUPPORTED Not supported
 * @retval #SCMIRRORING_ERROR_UNKNOWN Unknown Error
 *
 * @pre Create a screen mirroring primary sink handle by calling scmirroring_primary_sink_create().
 * @pre Register user callback by calling scmirroring_primary_sink_set_state_changed_cb().
 * @pre Call scmirroring_primary_sink_prepare()
 * @pre Call scmirroring_primary_sink_connect()
 * @pre The screen mirroring state must be #SCMIRRORING_SINK_STATE_CONNECTED or #SCMIRRORING_SINK_STATE_PLAYING
 */
int scmirroring_primary_sink_get_negotiated_video_frame_rate(scmirroring_primary_sink_h scmirroring_primary_sink, int *frame_rate);

/**
 * @brief Gets negotiated audio codec of screen mirroring primary sink.
 * @details The audio codec is negotiated by screen mirroring source.
 *
 * @since_tizen 5.5
 *
 * @param[in] scmirroring_primary_sink The handle to the screen mirroring primary sink
 * @param[out] codec Codec of audio
 * @return @c 0 on success,
 *         otherwise a negative error value
 * @retval #SCMIRRORING_ERROR_NONE Successful
 * @retval #SCMIRRORING_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #SCMIRRORING_ERROR_INVALID_OPERATION Invalid operation
 * @retval #SCMIRRORING_ERROR_NOT_SUPPORTED Not supported
 * @retval #SCMIRRORING_ERROR_UNKNOWN Unknown Error
 *
 * @pre Create a screen mirroring primary sink handle by calling scmirroring_primary_sink_create().
 * @pre Register user callback by calling scmirroring_primary_sink_set_state_changed_cb().
 * @pre Call scmirroring_primary_sink_prepare()
 * @pre Call scmirroring_primary_sink_connect()
 * @pre The screen mirroring state must be #SCMIRRORING_SINK_STATE_CONNECTED or #SCMIRRORING_SINK_STATE_PLAYING
 */
int scmirroring_primary_sink_get_negotiated_audio_codec(scmirroring_primary_sink_h scmirroring_primary_sink, scmirroring_audio_codec_e *codec);

/**
 * @brief Gets negotiated audio channel of screen mirroring primary sink.
 * @details The audio channel is negotiated by screen mirroring source.
 *
 * @since_tizen 5.5
 *
 * @param[in] scmirroring_primary_sink The handle to the screen mirroring primary sink
 * @param[out] channel Channel of audio
 * @return @c 0 on success,
 *         otherwise a negative error value
 * @retval #SCMIRRORING_ERROR_NONE Successful
 * @retval #SCMIRRORING_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #SCMIRRORING_ERROR_INVALID_OPERATION Invalid operation
 * @retval #SCMIRRORING_ERROR_NOT_SUPPORTED Not supported
 * @retval #SCMIRRORING_ERROR_UNKNOWN Unknown Error
 *
 * @pre Create a screen mirroring primary sink handle by calling scmirroring_primary_sink_create().
 * @pre Register user callback by calling scmirroring_primary_sink_set_state_changed_cb().
 * @pre Call scmirroring_primary_sink_prepare()
 * @pre Call scmirroring_primary_sink_connect()
 * @pre The screen mirroring state must be #SCMIRRORING_SINK_STATE_CONNECTED or #SCMIRRORING_SINK_STATE_PLAYING
 */
int scmirroring_primary_sink_get_negotiated_audio_channel(scmirroring_primary_sink_h scmirroring_primary_sink, int *channel);

/**
 * @brief Gets negotiated audio sample rate of screen mirroring primary sink.
 * @details The audio sample rate is negotiated by screen mirroring source.
 *
 * @since_tizen 5.5
 *
 * @param[in] scmirroring_primary_sink The handle to the screen mirroring primary sink
 * @param[out] sample_rate Sample rate of audio
 * @return @c 0 on success,
 *         otherwise a negative error value
 * @retval #SCMIRRORING_ERROR_NONE Successful
 * @retval #SCMIRRORING_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #SCMIRRORING_ERROR_INVALID_OPERATION Invalid operation
 * @retval #SCMIRRORING_ERROR_NOT_SUPPORTED Not supported
 * @retval #SCMIRRORING_ERROR_UNKNOWN Unknown Error
 *
 * @pre Create a screen mirroring primary sink handle by calling scmirroring_primary_sink_create().
 * @pre Register user callback by calling scmirroring_primary_sink_set_state_changed_cb().
 * @pre Call scmirroring_primary_sink_prepare()
 * @pre Call scmirroring_primary_sink_connect()
 * @pre The screen mirroring state must be #SCMIRRORING_SINK_STATE_CONNECTED or #SCMIRRORING_SINK_STATE_PLAYING
 */
int scmirroring_primary_sink_get_negotiated_audio_sample_rate(scmirroring_primary_sink_h scmirroring_primary_sink, int *sample_rate);

/**
 * @brief Gets negotiated audio bitwidth of screen mirroring primary sink.
 * @details The audio bitwidth is negotiated by screen mirroring source.
 *
 * @since_tizen 5.5
 *
 * @param[in] scmirroring_primary_sink The handle to the screen mirroring primary sink
 * @param[out] bitwidth Bitwidth of audio
 * @return @c 0 on success,
 *         otherwise a negative error value
 * @retval #SCMIRRORING_ERROR_NONE Successful
 * @retval #SCMIRRORING_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #SCMIRRORING_ERROR_INVALID_OPERATION Invalid operation
 * @retval #SCMIRRORING_ERROR_NOT_SUPPORTED Not supported
 * @retval #SCMIRRORING_ERROR_UNKNOWN Unknown Error
 *
 * @pre Create a screen mirroring primary sink handle by calling scmirroring_primary_sink_create().
 * @pre Register user callback by calling scmirroring_primary_sink_set_state_changed_cb().
 * @pre Call scmirroring_primary_sink_prepare()
 * @pre Call scmirroring_primary_sink_connect()
 * @pre The screen mirroring state must be #SCMIRRORING_SINK_STATE_CONNECTED or #SCMIRRORING_SINK_STATE_PLAYING
 */
int scmirroring_primary_sink_get_negotiated_audio_bitwidth(scmirroring_primary_sink_h scmirroring_primary_sink, int *bitwidth);

/**
 * @brief Gets the current state of screen mirroring primary sink.
 * @details The current state of screen mirroring primary sink izzs changed by calling CAPIs. And it provides the state of screen mirroring primary sink the time this api is called.
 *
 * @since_tizen 5.5
 *
 * @param[in] scmirroring_primary_sink The handle to the screen mirroring primary sink
 * @param[out] state The current state of screen mirroring primary sink
 *
 * @return @c 0 on success,
 *         otherwise a negative error value
 * @retval #SCMIRRORING_ERROR_NONE Successful
 * @retval #SCMIRRORING_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #SCMIRRORING_ERROR_INVALID_OPERATION Invalid operation
 * @retval #SCMIRRORING_ERROR_NOT_SUPPORTED Not supported
 * @retval #SCMIRRORING_ERROR_UNKNOWN Unknown Error
 *
 * @pre Create a screen mirroring primary sink handle by calling scmirroring_primary_sink_create().
 */
int scmirroring_primary_sink_get_current_state(scmirroring_primary_sink_h scmirroring_primary_sink, scmirroring_sink_state_e *state);

/**
 * @brief Called when user wants to set 'address' of screen mirroring primary sink.
 *
 * @details It sets MAC address to screen mirroring primary sink's property.
 *
 * @since_tizen 5.5
 *
 * @param[in] scmirroring_primary_sink     Screen mirroring primary sink handle
 * @param[in] address              Mac address of coupled screen mirroring coupled sink.
 *
 * @pre scmirroring_primary_sink_create()
 *
 * @see scmirroring_primary_sink_create()
 */
int scmirroring_primary_sink_set_coupled_sink(scmirroring_primary_sink_h scmirroring_primary_sink, gchar* address);

/**
 * @brief Called when user wants to set 'status' of screen mirroring primary sink.
 *
 * @details It sets status to screen mirroring primary sink's property.
 *
 * @since_tizen 5.5
 *
 * @param[in] scmirroring_primary_sink     Screen mirroring primary sink handle
 * @param[in] address              Mac address of coupled screen mirroring coupled sink.
 *
 * @pre scmirroring_primary_sink_create()
 *
 * @see scmirroring_primary_sink_create()
 */
int scmirroring_primary_sink_set_coupled_sink_status(scmirroring_primary_sink_h scmirroring_primary_sink, int status);

/**
 * @brief Creates screen mirroring source handle.
 * @remarks You must release @a scmirroring_primary_sink using scmirroring_primary_src_destroy().
 *
 * @since_tizen 5.5
 *
 * @param[out] scmirroring_primary_sink The handle to screen mirroring source
 * @return @c 0 on success,
 *         otherwise a negative error value
 * @retval #SCMIRRORING_ERROR_NONE Successful
 * @retval #SCMIRRORING_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #SCMIRRORING_ERROR_OUT_OF_MEMORY Not enough memory is available
 * @retval #SCMIRRORING_ERROR_INVALID_OPERATION Invalid operation
 *
 * @see scmirroring_primary_src_destroy()
 */
int scmirroring_primary_src_create(scmirroring_primary_sink_h *scmirroring_primary_sink);

/**
 * @brief Registers user callback to get status of screen mirroring.
 * @details This function registers user callback and this callback is called when each status is changed.
 *
 * @since_tizen 5.5
 *
 * @param[in] scmirroring_primary_sink The handle to screen mirroring source
 * @param[in] callback The callback function to invoke
 * @param[in] user_data The user data passed to the callback registration function
 *
 * @return @c 0 on success,
 *         otherwise a negative error value
 *
 * @retval #SCMIRRORING_ERROR_NONE Successful
 * @retval #SCMIRRORING_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #SCMIRRORING_ERROR_OUT_OF_MEMORY Not enough memory is available
 * @retval #SCMIRRORING_ERROR_INVALID_OPERATION Invalid operation
 *
 * @pre Create a screen mirroring source handle by calling scmirroring_primary_src_create().
 *
 * @see scmirroring_primary_src_create()
 */
int scmirroring_primary_src_set_state_changed_cb(scmirroring_primary_sink_h scmirroring_primary_sink_src, scmirroring_state_cb callback, void *user_data);

/**
 * @brief Sets connection mode of screen mirroring.
 *
 * @since_tizen 5.5
 *
 * @param[in] scmirroring_primary_sink The handle to screen mirroring source
 * @param[in] connection_mode connection mode of screen mirroring
 *
 * @return @c 0 on success,
 *         otherwise a negative error value
 *
 * @retval #SCMIRRORING_ERROR_NONE Successful
 * @retval #SCMIRRORING_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #SCMIRRORING_ERROR_OUT_OF_MEMORY Not enough memory is available
 * @retval #SCMIRRORING_ERROR_INVALID_OPERATION Invalid operation
 *
 * @pre Create a screen mirroring source handle by calling scmirroring_primary_src_create().
 *
 * @see scmirroring_primary_src_create()
 */
int scmirroring_primary_src_set_connection_mode(scmirroring_primary_sink_h scmirroring_primary_sink_src, scmirroring_connection_mode_e connection_mode);

/**
 * @brief Sets IP address and port number of screen mirroring source.
 *
 * @since_tizen 5.5
 *
 * @param[in] scmirroring_primary_sink The handle to screen mirroring source
 * @param[in] ip Server IP address
 * @param[in] port Server port
 *
 * @return @c 0 on success,
 *         otherwise a negative error value
 *
 * @retval #SCMIRRORING_ERROR_NONE Successful
 * @retval #SCMIRRORING_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #SCMIRRORING_ERROR_OUT_OF_MEMORY Not enough memory is available
 * @retval #SCMIRRORING_ERROR_INVALID_OPERATION Invalid operation
 *
 * @pre Create a screen mirroring source handle by calling scmirroring_primary_src_create().
 *
 * @see scmirroring_primary_src_create()
 */
int scmirroring_primary_src_set_ip_and_port(scmirroring_primary_sink_h scmirroring_primary_sink_src, const char *ip, const char *port);

/**
 * @brief Sets resolution of screen mirroring source.
 *
 * @since_tizen 5.5
 *
 * @param[in] scmirroring_primary_sink The handle to screen mirroring source
 * @param[in] resolution Resolution of screen mirroring source
 *
 * @return @c 0 on success,
 *         otherwise a negative error value
 *
 * @retval #SCMIRRORING_ERROR_NONE Successful
 * @retval #SCMIRRORING_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #SCMIRRORING_ERROR_OUT_OF_MEMORY Not enough memory is available
 * @retval #SCMIRRORING_ERROR_INVALID_OPERATION Invalid operation
 *
 * @pre Create a screen mirroring source handle by calling scmirroring_primary_src_create().
 *
 * @see scmirroring_primary_src_create()
 */
int scmirroring_primary_src_set_resolution(scmirroring_primary_sink_h scmirroring_primary_sink_src, scmirroring_resolution_e resolution);

/**
 * @brief Sets name of screen mirroring source server.
 *
 * @since_tizen 5.5
 *
 * @param[in] scmirroring_primary_sink The handle to screen mirroring source
 * @param[in] name Name of screen mirroring source server
 *
 * @return @c 0 on success,
 *         otherwise a negative error value
 *
 * @retval #SCMIRRORING_ERROR_NONE Successful
 * @retval #SCMIRRORING_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #SCMIRRORING_ERROR_OUT_OF_MEMORY Not enough memory is available
 * @retval #SCMIRRORING_ERROR_INVALID_OPERATION Invalid operation
 *
 * @pre Create a screen mirroring source handle by calling scmirroring_primary_src_create().
 *
 * @see scmirroring_primary_src_create()
 */
int scmirroring_primary_src_set_server_name(scmirroring_primary_sink_h scmirroring_primary_sink_src, const char *name);

/**
 * @brief Enables/Disables screen mirroring multisink.
 *
 * @since_tizen 5.5
 *
 * @param[in] scmirroring_primary_sink The handle to screen mirroring source
 * @param[in] multisink Ability to send to multisink
 *
 * @return @c 0 on success,
 *         otherwise a negative error value
 *
 * @retval #SCMIRRORING_ERROR_NONE Successful
 * @retval #SCMIRRORING_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #SCMIRRORING_ERROR_OUT_OF_MEMORY Not enough memory is available
 * @retval #SCMIRRORING_ERROR_INVALID_OPERATION Invalid operation
 *
 * @pre Create a screen mirroring source handle by calling scmirroring_primary_src_create().
 *
 * @see scmirroring_primary_src_create()
 */
int scmirroring_primary_src_set_multisink_ability(scmirroring_primary_sink_h scmirroring_primary_sink_src, scmirroring_multisink_e multisink);

/**
 * @brief Connects to server for screen mirroring as source, asynchronously.
 * @details This function launches server and connects to the server for screen mirroring as source to command server to start/pause/resume/stop.
 *
 * @since_tizen 5.5
 *
 * @param[in] scmirroring_primary_sink The handle to screen mirroring source
 *
 * @return @c 0 on success,
 *         otherwise a negative error value
 *
 * @retval #SCMIRRORING_ERROR_NONE Successful
 * @retval #SCMIRRORING_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #SCMIRRORING_ERROR_OUT_OF_MEMORY Not enough memory is available
 * @retval #SCMIRRORING_ERROR_INVALID_OPERATION Invalid operation
 * @retval #SCMIRRORING_ERROR_CONNECTION_TIME_OUT Connection timeout
 *
 * @pre Create a screen mirroring source handle by calling scmirroring_primary_src_create().
 * @pre Register user callback by calling scmirroring_primary_src_set_state_changed_cb().
 * @post The screen mirroring state will be SCMIRRORING_STATE_READY
 *
 * @see scmirroring_primary_src_create()
 * @see scmirroring_primary_src_set_state_changed_cb()
 */
int scmirroring_primary_src_connect(scmirroring_primary_sink_h scmirroring_primary_sink);

/**
 * @brief Starts screen mirroring, asynchronously.
 * @details This function starts screen mirroring, which means it starts to negotiate and stream RTP multimedia data.
 *
 * @since_tizen 5.5
 *
 * @param[in] scmirroring_primary_sink The handle to screen mirroring source
 *
 * @return @c 0 on success,
 *         otherwise a negative error value
 *
 * @retval #SCMIRRORING_ERROR_NONE Successful
 * @retval #SCMIRRORING_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #SCMIRRORING_ERROR_OUT_OF_MEMORY Not enough memory is available
 * @retval #SCMIRRORING_ERROR_INVALID_OPERATION Invalid operation
 * @retval #SCMIRRORING_ERROR_CONNECTION_TIME_OUT Connection timeout
 * @retval #SCMIRRORING_ERROR_NOT_SUPPORTED Not supported
 *
 * @pre Create a screen mirroring source handle by calling scmirroring_primary_src_create().
 * @pre Register user callback by calling scmirroring_primary_src_set_state_changed_cb().
 * @pre Call scmirroring_primary_src_connect()
 * @post The screen mirroring state will be SCMIRRORING_STATE_CONNECTION_WAIT if server starts to listen
 * @post The screen mirroring state will be SCMIRRORING_STATE_CONNECTED if client connects to the server
 * @post The screen mirroring state will be SCMIRRORING_STATE_PLAYING if server starts to stream multimedia data
 *
 * @see scmirroring_primary_src_create()
 * @see scmirroring_primary_src_set_state_changed_cb()
 * @see scmirroring_primary_src_connect()
 */
int scmirroring_primary_src_start(scmirroring_primary_sink_h scmirroring_primary_sink);

/**
 * @brief Pauses screen mirroring, asynchronously.
 * @details This function pauses screen mirroring, which means it sends RTSP PAUSE trigger message to sink.
 *
 * @since_tizen 5.5
 *
 * @param[in] scmirroring_primary_sink The handle to screen mirroring source
 *
 * @return @c 0 on success,
 *         otherwise a negative error value
 *
 * @retval #SCMIRRORING_ERROR_NONE Successful
 * @retval #SCMIRRORING_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #SCMIRRORING_ERROR_OUT_OF_MEMORY Not enough memory is available
 * @retval #SCMIRRORING_ERROR_INVALID_OPERATION Invalid operation
 *
 * @pre The screen mirroring state should be SCMIRRORING_STATE_PLAYING
 * @post The screen mirroring state will be SCMIRRORING_STATE_PAUSED
 *
 * @see scmirroring_primary_src_create()
 * @see scmirroring_primary_src_set_state_changed_cb()
 * @see scmirroring_primary_src_connect()
 * @see scmirroring_primary_src_start()
 */
int scmirroring_primary_src_pause(scmirroring_primary_sink_h scmirroring_primary_sink);

/**
 * @brief Resumes screen mirroring, asynchronously.
 * @details This function resumes screen mirroring, which means it sends RTSP PLAY message to sink.
 *
 * @since_tizen 5.5
 *
 * @param[in] scmirroring_primary_sink The handle to screen mirroring source
 *
 * @return @c 0 on success,
 *         otherwise a negative error value
 *
 * @retval #SCMIRRORING_ERROR_NONE Successful
 * @retval #SCMIRRORING_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #SCMIRRORING_ERROR_OUT_OF_MEMORY Not enough memory is available
 * @retval #SCMIRRORING_ERROR_INVALID_OPERATION Invalid operation
 *
 * @pre The screen mirroring state should be SCMIRRORING_STATE_PAUSED
 * @post The screen mirroring state will be SCMIRRORING_STATE_PLAYING
 *
 * @see scmirroring_primary_src_pause()
 */
int scmirroring_primary_src_resume(scmirroring_primary_sink_h scmirroring_primary_sink);

/**
 * @brief Enable streaming without reencoding for screen mirroring.
 * @details This function enable streaming without reencoding for screen mirroring, which means files that encoded with supported formats will be streamed without decoding and reencoding.
 *
 * @since_tizen 5.5
 *
 * @param[in] scmirroring_primary_sink The handle to screen mirroring source
 * @param[in] enable Ability to stream file directly
 * @param[in] uri_srcname File name to stream directly
 *
 * @return @c 0 on success,
 *         otherwise a negative error value
 *
 * @retval #SCMIRRORING_ERROR_NONE Successful
 * @retval #SCMIRRORING_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #SCMIRRORING_ERROR_OUT_OF_MEMORY Not enough memory to allocate new object
 * @retval #SCMIRRORING_ERROR_INVALID_OPERATION External function not implemented
 *
 * @pre The screen mirroring state should be SCMIRRORING_STATE_PLAYING
 *
 * @see scmirroring_primary_src_create()
 * @see scmirroring_primary_src_set_state_changed_cb()
 * @see scmirroring_primary_src_connect()
 * @see scmirroring_primary_src_start()
 */
int scmirroring_primary_src_set_direct_streaming(scmirroring_primary_sink_h scmirroring_primary_sink_src, scmirroring_direct_streaming_e enable, const char *uri_srcname);

/**
 * @brief Make Miracast Server 'coupling' mode.
 * @details This function make Miracast Server 'coupling' mode.
 * If it is set, connected sink devices is considered as secondary sink and both are goint to try coupling.
 *
 * @since_tizen 5.5
 *
 * @param[in] scmirroring_primary_sink The handle to screen mirroring source
 * @param[in] coupling_mode value which set coupling mode (0 : disable, 1 : enable)
 *
 * @return @c 0 on success,
 *         otherwise a negative error value
 *
 * @retval #SCMIRRORING_ERROR_NONE Successful
 * @retval #SCMIRRORING_ERROR_INVALID_PARAMETER Invalid parameter
 *
 * @pre The screen mirroring state should be SCMIRRORING_STATE_PLAYING
 *
 * @see scmirroring_primary_src_create()
 * @see scmirroring_primary_src_set_state_changed_cb()
 * @see scmirroring_primary_src_connect()
 * @see scmirroring_primary_src_start()
 */
int scmirroring_primary_src_set_coupling_mode(scmirroring_primary_sink_h scmirroring, scmirroring_coupling_mode_e coupling_mode);

/**
 * @brief Change transport for AV streaming.
 * @details This function changes transport for AV streaming. Default transport is UDP.
 *
 * @since_tizen 5.5
 *
 * @param[in] scmirroring_primary_sink The handle to screen mirroring source
 * @param[in] transport Transport for audio/video streaming data
 *
 * @return @c 0 on success,
 *         otherwise a negative error value
 *
 * @retval #SCMIRRORING_ERROR_NONE Successful
 * @retval #SCMIRRORING_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #SCMIRRORING_ERROR_OUT_OF_MEMORY Not enough memory to allocate new object
 * @retval #SCMIRRORING_ERROR_INVALID_OPERATION External function not implemented
 *
 * @pre The screen mirroring state should be SCMIRRORING_STATE_PLAYING
 *
 * @see scmirroring_primary_src_create()
 * @see scmirroring_primary_src_set_state_changed_cb()
 * @see scmirroring_primary_src_connect()
 * @see scmirroring_primary_src_start()
 */
int scmirroring_primary_src_AV_transport_switch(scmirroring_primary_sink_h scmirroring_primary_sink_src, scmirroring_av_transport_e transport);

/**
 * @brief Stops screen mirroring, asynchronously.
 * @details This function stops screen mirroring, which means it sends RTSP TEARDOWN trigger message to sink.
 *
 * @since_tizen 5.5
 *
 * @param[in] scmirroring_primary_sink The handle to screen mirroring source
 *
 * @return @c 0 on success,
 *         otherwise a negative error value
 *
 * @retval #SCMIRRORING_ERROR_NONE Successful
 * @retval #SCMIRRORING_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #SCMIRRORING_ERROR_OUT_OF_MEMORY Not enough memory is available
 * @retval #SCMIRRORING_ERROR_INVALID_OPERATION Invalid operation
 * @retval #SCMIRRORING_ERROR_CONNECTION_TIME_OUT Connection timeout
 *
 * @pre The screen mirroring state should be SCMIRRORING_STATE_PAUSED or SCMIRRORING_STATE_PLAYING
 * @post The screen mirroring state will be SCMIRRORING_STATE_TEARDOWN
 *
 * @see scmirroring_primary_src_create()
 * @see scmirroring_primary_src_set_state_changed_cb()
 * @see scmirroring_primary_src_connect()
 * @see scmirroring_primary_src_start()
 */
int scmirroring_primary_src_stop(scmirroring_primary_sink_h scmirroring_primary_sink);

/**
 * @brief Disconnects server for screen mirroring.
 *
 * @since_tizen 5.5
 *
 * @param[in] scmirroring_primary_sink The handle to screen mirroring source
 *
 * @return @c 0 on success,
 *         otherwise a negative error value
 *
 * @retval #SCMIRRORING_ERROR_NONE Successful
 * @retval #SCMIRRORING_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #SCMIRRORING_ERROR_OUT_OF_MEMORY Not enough memory is available
 * @retval #SCMIRRORING_ERROR_INVALID_OPERATION Invalid operation
 * @retval #SCMIRRORING_ERROR_CONNECTION_TIME_OUT Connection timeout
 *
 * @pre Create a screen mirroring source handle by calling scmirroring_primary_src_create().
 * @pre Register user callback by calling scmirroring_primary_src_set_state_changed_cb().
 * @pre Connects to server for screen mirroring source by calling scmirroring_primary_src_connect().
 * @post The screen mirroring state will be SCMIRRORING_STATE_NULL
 *
 * @see scmirroring_primary_src_create()
 * @see scmirroring_primary_src_set_state_changed_cb()
 * @see scmirroring_primary_src_connect()
 */
int scmirroring_primary_src_disconnect(scmirroring_primary_sink_h scmirroring_primary_sink);

/**
 * @brief Unregisters the callback function user registered
 *
 * @since_tizen 5.5
 *
 * @param[in] scmirroring_primary_sink The handle to screen mirroring source
 *
 * @return @c 0 on success,
 *         otherwise a negative error value
 *
 * @retval #SCMIRRORING_ERROR_NONE Successful
 * @retval #SCMIRRORING_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #SCMIRRORING_ERROR_OUT_OF_MEMORY Not enough memory is available
 * @retval #SCMIRRORING_ERROR_INVALID_OPERATION Invalid operation
 *
 * @pre Create a screen mirroring source handle by calling scmirroring_primary_src_create().
 * @pre Register user callback by calling scmirroring_primary_src_set_state_changed_cb().
 *
 * @see scmirroring_primary_src_create()
 * @see scmirroring_primary_src_set_state_changed_cb()
 */
int scmirroring_primary_src_unset_state_changed_cb(scmirroring_primary_sink_h scmirroring_primary_sink);

/**
 * @brief Destroys server and screen mirroring source handle.
 *
 * @since_tizen 5.5
 *
 * @param[in] scmirroring_primary_sink The handle to screen mirroring source
 *
 * @return @c 0 on success,
 *         otherwise a negative error value
 *
 * @retval #SCMIRRORING_ERROR_NONE Successful
 * @retval #SCMIRRORING_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #SCMIRRORING_ERROR_OUT_OF_MEMORY Not enough memory is available
 * @retval #SCMIRRORING_ERROR_INVALID_OPERATION Invalid operation
 * @retval #SCMIRRORING_ERROR_CONNECTION_TIME_OUT Connection timeout
 *
 * @pre Create a screen mirroring source handle by calling scmirroring_primary_src_create().
 * @pre The screen mirroring state should be SCMIRRORING_STATE_NULL
 *
 * @see scmirroring_primary_src_create()
 */
int scmirroring_primary_src_destroy(scmirroring_primary_sink_h scmirroring_primary_sink);

#ifdef __cplusplus
}
#endif /* __cplusplus */

/**
 * @}
 */

#endif /* __TIZEN_MEDIA_SCMIRRORING_PRIMARY_SINK_H__ */
