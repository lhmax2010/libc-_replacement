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

#ifndef __TIZEN_MEDIA_SCMIRRORING_INTERNAL_H__
#define __TIZEN_MEDIA_SCMIRRORING_INTERNAL_H__

#include <tizen.h>
#include <scmirroring_type.h>
#include <scmirroring_type_internal.h>

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

/**
 * @file scmirroring_internal.h
 * @brief This file contains API related to screen mirroring internal enumerations for classes of errors.
 */

/**
 * @brief	The handle to the screen mirroring source.
 */
typedef void *scmirroring_src_h;

/**
 * @brief Enumeration for screen mirroring connection mode.
 */
typedef enum {
	SCMIRRORING_CONNECTION_WIFI_DIRECT = 0,     /**< Wi-Fi Direct*/
	SCMIRRORING_CONNECTION_MAX,                 /* Number of Connection mode */
} scmirroring_connection_mode_e;

/**
 * @brief Enumeration for screen mirroring source state.
 */
typedef enum {
	SCMIRRORING_STATE_NULL = 0,         /**< Screen mirroring is not created yet */
	SCMIRRORING_STATE_CREATED,          /**< Screen mirroring is created, but not realized yet */
	SCMIRRORING_STATE_READY,            /**< Screen mirroring is ready to play media */
	SCMIRRORING_STATE_CONNECTION_WAIT,  /**< Screen mirroring is waiting for connection */
	SCMIRRORING_STATE_CONNECTED,        /**< Screen mirroring is connected */
	SCMIRRORING_STATE_PLAYING,          /**< Screen mirroring is now playing media */
	SCMIRRORING_STATE_PAUSED,           /**< Screen mirroring is paused while playing media */
	SCMIRRORING_STATE_TEARDOWN,         /**< Teardown Screen mirroring */
	SCMIRRORING_STATE_MAX               /* Number of screen mirroring states */
} scmirroring_state_e;

/**
 * @brief Called when each status is changed.
 *
 * @details This callback is called for state and error of screen mirroring.
 *
 * @param[in] error     The error code
 * @param[in] state     The screen mirroring state
 * @param[in] user_data The user data passed from the scmirroring_src_set_state_cb() function
 *
 * @pre scmirroring_src_create()
 *
 * @see scmirroring_src_create()
 */
typedef void(*scmirroring_state_cb)(scmirroring_error_e error, scmirroring_state_e state, void *user_data);

/**
 * @internal
 * @brief Sets the ecore wayland video display.
 *
 * @since_tizen 6.5
 *
 * @remarks This function must be called in main thread of the application.
 *          Otherwise, it will return #SCMIRRORING_ERROR_INVALID_OPERATION by internal restriction.
 *          To avoid #SCMIRRORING_ERROR_INVALID_OPERATION in sub thread, ecore_thread_main_loop_begin() and
 *          ecore_thread_main_loop_end() can be used, but deadlock can occur if the main thread is busy.
 *          So, it's not recommended to use them.
 *
 * @param[in] scmirroring_sink The handle to the screen mirroring sink
 * @param[in] display_surface The ecore wayland window handle
 * @return @c 0 on success,
 *         otherwise a negative error value
 * @retval #SCMIRRORING_ERROR_NONE Successful
 * @retval #SCMIRRORING_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #SCMIRRORING_ERROR_OUT_OF_MEMORY Out of memory
 * @retval #SCMIRRORING_ERROR_INVALID_OPERATION Invalid operation
 *
 * @pre Create a screen mirroring sink handle by calling scmirroring_sink_create().
 *
 * @see scmirroring_sink_create()
 */
int scmirroring_sink_set_ecore_wl_display(scmirroring_sink_h scmirroring_sink, void *display_surface);

/**
 * @internal
 * @brief Sets the window size for UIBC events within the specified region.
 * @details Configuration of the UIBC area for calculation internal coordinates of UIBC events.
 *
 * @since_tizen 10.0
 *
 * @remarks This function must be called before scmirroring_sink_enable_uibc().
 * @remarks This function is related to the following feature:\n
 *          %http://tizen.org/feature/network.wifi.direct.display\n
 * @param[in] scmirroring_sink The handle to the screen mirroring sink
 * @param[in] width The width of application window
 * @param[in] height The height of application window
 * @return @c 0 on success,
 *         otherwise a negative error value
 * @retval #SCMIRRORING_ERROR_NONE Successful
 * @retval #SCMIRRORING_ERROR_NOT_SUPPORTED Not supported
 * @retval #SCMIRRORING_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #SCMIRRORING_ERROR_INVALID_OPERATION Invalid operation
 * @retval #SCMIRRORING_ERROR_UNKNOWN Unknown Error
 * @pre Call scmirroring_sink_prepare() before calling this function.
 * @see scmirroring_sink_enable_uibc()
 *
 */
int scmirroring_sink_set_window_size(scmirroring_sink_h scmirroring_sink, int width, int height);

/**
 * @internal
 * @brief Enable UIBC.
 *
 *  @since_tizen 10.0
 *
 * @remarks This function is related to the following feature:\n
 *          %http://tizen.org/feature/network.wifi.direct.display\n
 * @remarks This function must be called before scmirroring_sink_connect().
 * @param[in] scmirroring_sink The handle to the screen mirroring sink
 * @param[in] mode UIBC capture mode
 * @return @c 0 on success,
 *         otherwise a negative error value
 * @retval #SCMIRRORING_ERROR_NONE Successful
 * @retval #SCMIRRORING_ERROR_NOT_SUPPORTED Not supported
 * @retval #SCMIRRORING_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #SCMIRRORING_ERROR_INVALID_OPERATION Invalid operation
 * @retval #SCMIRRORING_ERROR_UNKNOWN Unknown Error
 * @pre Call scmirroring_sink_set_window_size() before calling this function.
 * @pre Call scmirroring_sink_prepare() before calling this function.
 * @see scmirroring_sink_set_window_size()
 * @see scmirroring_sink_prepare()
 * @see scmirroring_sink_connect()
 */
int scmirroring_sink_enable_uibc(scmirroring_sink_h scmirroring_sink, scmirroring_uibc_capture_mode_e mode);

/**
 * @internal
 * @brief Sends UIBC generic mouse event to the screen mirroring source.
 *
 * @since_tizen 10.0
 *
 * @remarks This function is related to the following feature:\n
 *          %http://tizen.org/feature/network.wifi.direct.display\n
 * @remarks   This function support only mouse-left-down, mouse-left-move and mouse-left-up event.
 *
 * @param[in] scmirroring_sink The handle to the screen mirroring sink
 * @param[in] uibc_event UIBC mouse event
 * @return @c 0 on success,
 *         otherwise a negative error value
 * @retval #SCMIRRORING_ERROR_NONE Successful
 * @retval #SCMIRRORING_ERROR_NOT_SUPPORTED Not supported
 * @retval #SCMIRRORING_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #SCMIRRORING_ERROR_INVALID_OPERATION Invalid operation
 * @retval #SCMIRRORING_ERROR_UNKNOWN Unknown Error
 * @pre Call scmirroring_sink_connect() before calling this function.
 * @see scmirroring_sink_connect()
 * @see #scmirroring_uibc_mouse_s
 * @see #scmirroring_uibc_mouse_event_s
 */
int scmirroring_sink_send_generic_mouse_event(scmirroring_sink_h scmirroring_sink, scmirroring_uibc_mouse_event_s *uibc_event);

/**
 * @internal
 * @brief Sends UIBC generic key event to the screen mirroring source.
 *
 * @since_tizen 10.0
 *
 * @remarks This function is related to the following feature:\n
 *          %http://tizen.org/feature/network.wifi.direct.display\n
 * @param[in] scmirroring_sink The handle to the screen mirroring sink
 * @param[in] event_type Key event type
 * @param[in] key_code_1 Key code 1 (ASCII), The key code of the first key down/up event. The basic/extended ASCII code uses the lower one byte.
 *                 The higher one byte is reserved for future ASCII compatible key codes.
 * @param[in] key_code_2 Key code 2 (ASCII), The key code for the second key down/up event. The value is set to 0x0000(NULL), if the second key code is not present.
 *                 The basic/extended ASCII code uses the lower one byte. The higher one byte is reserved for future ASCII compatible key codes.
 * @return @c 0 on success,
 *         otherwise a negative error value
 * @retval #SCMIRRORING_ERROR_NONE Successful
 * @retval #SCMIRRORING_ERROR_NOT_SUPPORTED Not supported
 * @retval #SCMIRRORING_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #SCMIRRORING_ERROR_INVALID_OPERATION Invalid operation
 * @retval #SCMIRRORING_ERROR_UNKNOWN Unknown Error
 * @pre Call scmirroring_sink_connect() before calling this function.
 * @see scmirroring_sink_connect()
 */
int scmirroring_sink_send_generic_key_event(scmirroring_sink_h scmirroring_sink, scmirroring_key_event_type_e event_type, unsigned short key_code_1, unsigned short key_code_2);


/**
 * @internal
 * @brief Registers a callback function to be called when display orientation of screen mirroring source device is received.
 *
 * @since_tizen 10.0
 *
 * @param[in] scmirroring_sink The handle to the screen mirroring sink
 * @param[in] callback         The callback function to invoke
 * @param[in] user_data        The user data passed to the callback registration function
 * @return @c 0 on success,
 *         otherwise a negative error value
 *
 * @retval #SCMIRRORING_ERROR_NONE Successful
 * @retval #SCMIRRORING_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #SCMIRRORING_ERROR_OUT_OF_MEMORY Out of memory
 *
 * @pre Create a screen mirroring sink handle by calling scmirroring_sink_create().
 *
 * @see scmirroring_sink_create()
 */
int scmirroring_sink_set_src_display_orientation_notify_cb(scmirroring_sink_h scmirroring_sink, scmirroring_sink_src_display_orientation_notify_cb callback, void *user_data);

/**
 * @internal
 * @brief Registers a callback function to be called when UIBC information is received.
 *
 * @since_tizen 10.0
 *
 * @param[in] scmirroring_sink The handle to the screen mirroring sink
 * @param[in] callback         The callback function to invoke
 * @param[in] user_data        The user data passed to the callback registration function
 * @return @c 0 on success,
 *         otherwise a negative error value
 *
 * @retval #SCMIRRORING_ERROR_NONE Successful
 * @retval #SCMIRRORING_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #SCMIRRORING_ERROR_OUT_OF_MEMORY Out of memory
 *
 * @pre Create a screen mirroring sink handle by calling scmirroring_sink_create().
 *
 * @see scmirroring_sink_create()
 */
int scmirroring_sink_set_uibc_info_received_cb(scmirroring_sink_h scmirroring_sink, scmirroring_sink_uibc_info_received_cb callback, void *user_data);

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* __TIZEN_MEDIA_SCMIRRORING_INTERNAL_H__ */
