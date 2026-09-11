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

#ifndef __TIZEN_MEDIA_SCMIRRORING_TYPE_INTERNAL_H__
#define __TIZEN_MEDIA_SCMIRRORING_TYPE_INTERNAL_H__

#include <stdint.h>
#include <tizen.h>
#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

/**
 * @file scmirroring_type_internal.h
 * @brief This file contains API related to screen mirroring enumerations for classes of errors.
 */

/**
 * @brief	The handle to the screen mirroring primary sink.
 * @since_tizen 5.5
 */
typedef void *scmirroring_primary_sink_h;

/**
 * @brief	The handle to the screen mirroring secondary sink.
 * @since_tizen 5.5
 */
typedef void *scmirroring_secondary_sink_h;

/**
 * @brief	The handle to the screen mirroring UIBC mouse.
 * @since_tizen 10.0
 */
typedef void *scmirroring_sink_mouse_h;

/**
 * @brief	The handle to the screen mirroring UIBC mouse event.
 * @since_tizen 10.0
 */
typedef void *scmirroring_sink_mouse_event_h;

/**
 * @brief Enumeration for screen mirroring coupling mode.
 * @since_tizen 5.5
 */
typedef enum {
	SCMIRRORING_COUPLING_MODE_DISABLED = 0,    /**< Disable coupling mode of miracast server */
	SCMIRRORING_COUPLING_MODE_ENABLED          /**< Enable coupling mode of miracast server */
} scmirroring_coupling_mode_e;

/**
 * @brief Enumeration for screen mirroring coupling status.
 * @since_tizen 5.5
 */
typedef enum {
	SCMIRRORING_COUPLING_STATUS_NOT_COUPLED = 0,
	SCMIRRORING_COUPLING_STATUS_COUPLED,
	SCMIRRORING_COUPLING_STATUS_TEARDOWN_COUPLING,
	SCMIRRORING_COUPLING_STATUS_MAX
} scmirroring_coupled_sink_status_e;

/**
 * @brief Enumerations for key event type.
 * @since_tizen 10.0
 */
typedef enum
{
	SCMIRRORING_EVENT_KEY_DOWN,           /**< Key down event */
	SCMIRRORING_EVENT_KEY_UP              /**< Key up event */
} scmirroring_key_event_type_e;

/**
 * @brief Enumerations for mouse event type.
 * @since_tizen 10.0
 */
typedef enum
{
	SCMIRRORING_EVENT_MOUSE_LEFT_DOWN,    /**< Mouse left button down event */
	SCMIRRORING_EVENT_MOUSE_LEFT_UP,      /**< Mouse left button up event */
	SCMIRRORING_EVENT_MOUSE_MOVE          /**< Mouse move event */
} scmirroring_mouse_event_type_e;

/**
 * @brief Enumerations for UIBC capture mode.
 * @since_tizen 10.0
 */
typedef enum
{
	SCMIRRORING_CAPTURE_FROM_APPLICATION,   /**< Capture from application (Generic input) */
	SCMIRRORING_CAPTURE_FROM_RAW_INTERFACE  /**< Capture from raw interface (Human Interface Device) */
} scmirroring_uibc_capture_mode_e;

/**
 * @brief Structure for UIBC mouse information.
 * @since_tizen 10.0
 */
typedef struct {
	uint16_t id; /**< ID for pointer */
	uint16_t x;  /**< X-coordicate for mouse */
	uint16_t y;  /**< Y-coordicate for mouse */
} scmirroring_uibc_mouse_s;

/**
 * @brief Structure for UIBC event information.
 * @since_tizen 10.0
 */
typedef struct {
	int num_objects;                            /**< Number of uibc_object pointer */
	scmirroring_mouse_event_type_e event_type;  /**< Mouse event type */
	scmirroring_uibc_mouse_s *uibc_object;      /**< UIBC mouse */
} scmirroring_uibc_mouse_event_s;

/**
 * @internal
 * @brief Enumeration for screen mirroring display orientation.
 * @since_tizen 10.0
 */
typedef enum {
	SCMIRRORING_DISPLAY_ORIENTATION_NONE = 0,
	SCMIRRORING_DISPLAY_ORIENTATION_PORTRAIT_DISPLAY,
	SCMIRRORING_DISPLAY_ORIENTATION_PORTRAIT,
	SCMIRRORING_DISPLAY_ORIENTATION_LANDSCAPE
} scmirroring_display_orientation_e;

/**
 * @internal
 * @brief Enumeration for screen mirroring UIBC input type.
 * @since_tizen 10.0
 */
typedef enum {
	SCMIRRORING_UIBC_INPUT_TYPE_UNKNOWN       = 0,
	SCMIRRORING_UIBC_INPUT_TYPE_KEYBOARD      = (1 << 0),
	SCMIRRORING_UIBC_INPUT_TYPE_MOUSE         = (1 << 1),
	SCMIRRORING_UIBC_INPUT_TYPE_SINGLETOUCH   = (1 << 2),
	SCMIRRORING_UIBC_INPUT_TYPE_MULTITOUCH    = (1 << 3),
	SCMIRRORING_UIBC_INPUT_TYPE_JOYSTICK      = (1 << 4),
	SCMIRRORING_UIBC_INPUT_TYPE_CAMERA        = (1 << 5),
	SCMIRRORING_UIBC_INPUT_TYPE_GESTURE       = (1 << 6),
	SCMIRRORING_UIBC_INPUT_TYPE_REMOTECONTROL = (1 << 7)
} scmirroring_uibc_input_type_e;

/**
 * @internal
 * @brief Enumeration for screen mirroring UIBC input path.
 * @since_tizen 10.0
 */
typedef enum {
	SCMIRRORING_UIBC_INPUT_PATH_UNKNOWN  = 0,
	SCMIRRORING_UIBC_INPUT_PATH_INFRARED = (1 << 0),
	SCMIRRORING_UIBC_INPUT_PATH_USB      = (1 << 1),
	SCMIRRORING_UIBC_INPUT_PATH_BT       = (1 << 2),
	SCMIRRORING_UIBC_INPUT_PATH_ZIGBEE   = (1 << 3),
	SCMIRRORING_UIBC_INPUT_PATH_WIFI     = (1 << 4),
	SCMIRRORING_UIBC_INPUT_PATH_NOSP     = (1 << 5)
} scmirroring_uibc_input_path_e;

/**
 * @internal
 * @brief Screen mirroring UIBC input.
 * @since_tizen 10.0
 */
typedef struct {
	scmirroring_uibc_input_type_e type;
	scmirroring_uibc_input_path_e path;
} scmirroring_uibc_input_s;

/**
 * @internal
 * @brief Screen mirroring UIBC information.
 * @since_tizen 10.0
 */
typedef struct {
	char *ip;
	uint32_t port;
	uint32_t gen_capability;
	int32_t width;
	int32_t height;
	scmirroring_uibc_input_s *hidc_caps_list;
	uint32_t hidc_caps_count;
} scmirroring_uibc_info_s;

/**
 * @internal
 * @brief Called when display orientation of screen mirroring source is received.
 * @since_tizen 10.0
 *
 * @details This callback is called for display orientation of screen mirroring source will be received.
 *
 * @param[in] orientation The display orientation of screen mirroring source device
 * @param[in] user_data   The user data passed from the scmirroring_sink_set_src_display_orientation_notify_cb() function
 *
 * @see scmirroring_sink_set_src_display_orientation_notify_cb()
 */
typedef void(*scmirroring_sink_src_display_orientation_notify_cb)(scmirroring_display_orientation_e orientation, void *user_data);

/**
 * @internal
 * @brief Called when UIBC information is received.
 * @since_tizen 10.0
 *
 * @details This callback is called for UIBC information will be received.
 *
 * @param[in] error     The error code
 * @param[in] uibc_info The UIBC information
 * @param[in] user_data The user data passed from the scmirroring_sink_set_uibc_info_received_cb() function
 *
 * @see scmirroring_sink_set_uibc_info_received_cb()
 */
typedef void(*scmirroring_sink_uibc_info_received_cb)(scmirroring_error_e error, scmirroring_uibc_info_s *uibc_info, void *user_data);

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* __TIZEN_MEDIA_SCMIRRORING_TYPE_INTERNAL_H__ */
