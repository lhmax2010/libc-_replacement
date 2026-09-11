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

#ifndef __TIZEN_MULTIMEDIA_CAMERA_INTERNAL_H__
#define __TIZEN_MULTIMEDIA_CAMERA_INTERNAL_H__

#include <camera.h>


#ifdef __cplusplus
extern "C" {
#endif

/**
 * @file camera_internal.h
 * @brief This file contains the internal Camera API, related structures and enumerations.
 * @since_tizen 3.0
 */

/**
 * @addtogroup CAPI_MEDIA_CAMERA_INTERNAL_MODULE
 * @{
 */

/**
 * @internal
 * @brief Enumeration for the auto exposure status of preview frame.
 * @since_tizen 7.0
 */
typedef enum {
	CAMERA_STATUS_AUTO_EXPOSURE_NONE,       /**< None */
	CAMERA_STATUS_AUTO_EXPOSURE_UNSTABLE,   /**< Unstable */
	CAMERA_STATUS_AUTO_EXPOSURE_STABLE      /**< Stable */
} camera_status_auto_exposure_e;

/**
 * @internal
 * @brief Enumeration for the auto white balance status of preview frame.
 * @since_tizen 7.0
 */
typedef enum {
	CAMERA_STATUS_AUTO_WHITE_BALANCE_NONE,      /**< None */
	CAMERA_STATUS_AUTO_WHITE_BALANCE_UNSTABLE,  /**< Unstable */
	CAMERA_STATUS_AUTO_WHITE_BALANCE_STABLE     /**< Stable */
} camera_status_auto_white_balance_e;

/**
 * @internal
 * @brief The structure type of the frame meta.
 * @since_tizen 7.0
 * @remarks This is available for specific device only.
 */
typedef struct _camera_frame_meta_s {
	unsigned long long ts_soe;
	unsigned long long ts_eoe;
	unsigned long long ts_sof;
	unsigned long long ts_eof;
	unsigned long long ts_hal;
	unsigned long long ts_qmf;
	unsigned long long ts_gst;
	unsigned long long td_exp;
	unsigned long long ts_aux;
	unsigned long long td_aux;
	unsigned long long seqnum;
	unsigned long long flags;
	unsigned long long lux_index;
} camera_frame_meta_s;


/**
 * @internal
 * @brief Start the evas rendering.
 * @since_tizen 3.0
 * @param[in] camera The handle to the camera
 * @return @c 0 on success, otherwise a negative error value
 * @retval #CAMERA_ERROR_NONE Successful
 * @retval #CAMERA_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #CAMERA_ERROR_INVALID_STATE Invalid state
 * @retval #CAMERA_ERROR_INVALID_OPERATION Invalid operation
 * @retval #CAMERA_ERROR_NOT_SUPPORTED The feature is not supported
 */
int camera_start_evas_rendering(camera_h camera);

/**
 * @internal
 * @brief Stop the evas rendering.
 * @since_tizen 3.0
 * @param[in] camera The handle to the camera
 * @param[in] keep_screen If @c true keep last frame on display, otherwise @c false
 * @return @c 0 on success, otherwise a negative error value
 * @retval #CAMERA_ERROR_NONE Successful
 * @retval #CAMERA_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #CAMERA_ERROR_INVALID_STATE Invalid state
 * @retval #CAMERA_ERROR_INVALID_OPERATION Invalid operation
 * @retval #CAMERA_ERROR_NOT_SUPPORTED The feature is not supported
 */
int camera_stop_evas_rendering(camera_h camera, bool keep_screen);

/**
 * @internal
 * @brief Sets the ecore wayland video display.
 * @since_tizen 6.0
 * @remarks This function must be called in main thread of the application.
 *          Otherwise, it will return #CAMERA_ERROR_INVALID_OPERATION by internal restriction.
 *          To avoid #CAMERA_ERROR_INVALID_OPERATION in sub thread, ecore_thread_main_loop_begin() and
 *          ecore_thread_main_loop_end() can be used, but deadlock can occur if the main thread is busy.
 *          So, it's not recommended to use them.
 * @param[in] camera The handle to the camera
 * @param[in] ecore_wl_window The ecore wayland window handle
 * @return @c 0 on success,
 *         otherwise a negative error value
 * @retval #CAMERA_ERROR_NONE Successful
 * @retval #CAMERA_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #CAMERA_ERROR_INVALID_STATE Invalid state
 * @retval #CAMERA_ERROR_INVALID_OPERATION Invalid operation
 * @retval #CAMERA_ERROR_SERVICE_DISCONNECTED The socket to multimedia server is disconnected
 * @pre The camera state must be set to #CAMERA_STATE_CREATED.
 * @see camera_start_preview()
 * @see ecore_thread_main_loop_begin()
 * @see ecore_thread_main_loop_end()
 */
int camera_set_ecore_wl_display(camera_h camera, void *ecore_wl_window);

/**
 * @internal
 * @brief Creates preview frame from stream data.
 * @since_tizen 6.0
 * @param[in] stream The stream from internal pipeline
 * @param[in] num_buffer_fd The number of buffer fd
 * @param[in] buffer_bo_handle The bo handle of buffer
 * @param[in] data_bo_handle The bo handle of data
 * @param[out] frame The frame which will be filled
 */
void camera_create_preview_frame(void *stream, int num_buffer_fd,
	void *buffer_bo_handle, void *data_bo_handle, camera_preview_data_s *frame);

/**
 * @internal
 * @brief Sets the brightness level of flash.
 * @since_tizen 6.5
 * @remarks If the min value is greater than the max value from camera_attr_get_flash_brightness_range(), \n
 *          it means that this feature is not supported.
 * @param[in] camera The handle to the camera
 * @param[in] level The brightness level of flash
 * @return @c 0 on success, otherwise a negative error value
 * @retval #CAMERA_ERROR_NONE Successful
 * @retval #CAMERA_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #CAMERA_ERROR_INVALID_STATE Invalid state
 * @retval #CAMERA_ERROR_NOT_SUPPORTED The feature is not supported
 * @retval #CAMERA_ERROR_SERVICE_DISCONNECTED The socket to multimedia server is disconnected
 * @pre The camera state must be set to #CAMERA_STATE_CREATED or #CAMERA_STATE_PREVIEW.
 * @see camera_attr_get_flash_brightness()
 * @see camera_attr_get_flash_brightness_range()
 */
int camera_attr_set_flash_brightness(camera_h camera, int level);

/**
 * @internal
 * @brief Gets the brightness level of flash.
 * @since_tizen 6.5
 * @param[in]  camera The handle to the camera
 * @param[out] level  The brightness level of flash
 * @return @c 0 on success, otherwise a negative error value
 * @retval #CAMERA_ERROR_NONE Successful
 * @retval #CAMERA_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #CAMERA_ERROR_NOT_SUPPORTED The feature is not supported
 * @retval #CAMERA_ERROR_SERVICE_DISCONNECTED The socket to multimedia server is disconnected
 * @see camera_attr_set_flash_brightness()
 * @see camera_attr_get_flash_brightness_range()
 */
int camera_attr_get_flash_brightness(camera_h camera, int *level);

/**
 * @internal
 * @brief Gets the available brightness level of flash.
 * @since_tizen 6.5
 * @remarks If the min value is greater than the max value, it means that this feature is not supported.
 * @param[in]  camera The handle to the camera
 * @param[out] min    The minimum brightness level of flash
 * @param[out] max    The maximum brightness level of flash
 * @return @c 0 on success, otherwise a negative error value
 * @retval #CAMERA_ERROR_NONE Successful
 * @retval #CAMERA_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #CAMERA_ERROR_NOT_SUPPORTED The feature is not supported
 * @retval #CAMERA_ERROR_SERVICE_DISCONNECTED The socket to multimedia server is disconnected
 * @see camera_attr_set_flash_brightness()
 * @see camera_attr_get_flash_brightness()
 */
int camera_attr_get_flash_brightness_range(camera_h camera, int *min, int *max);

/**
 * @internal
 * @brief Sets device for the extra preview stream.
 * @since_tizen 7.0
 * @param[in] camera       The handle to the camera
 * @param[in] stream_id    The id of extra preview stream
 * @param[in] device       The camera type
 * @return @c 0 on success, otherwise a negative error value
 * @retval #CAMERA_ERROR_NONE Successful
 * @retval #CAMERA_ERROR_NOT_SUPPORTED The feature is not supported
 * @retval #CAMERA_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #CAMERA_ERROR_INVALID_STATE Invalid state
 * @retval #CAMERA_ERROR_SERVICE_DISCONNECTED The socket to multimedia server is disconnected
 * @pre The camera state must be set to #CAMERA_STATE_CREATED.
 * @see camera_set_extra_preview_cb()
 * @see camera_unset_extra_preview_cb()
 */
int camera_set_extra_preview_device(camera_h camera, int stream_id, camera_device_e device);

/**
 * @internal
 * @brief Requests codec config data for encoded format.
 * @since_tizen 7.0
 * @param[in] camera       The handle to the camera
 * @return @c 0 on success, otherwise a negative error value
 * @retval #CAMERA_ERROR_NONE Successful
 * @retval #CAMERA_ERROR_NOT_SUPPORTED The feature is not supported
 * @retval #CAMERA_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #CAMERA_ERROR_INVALID_STATE Invalid state
 * @retval #CAMERA_ERROR_SERVICE_DISCONNECTED The socket to multimedia server is disconnected
 * @pre The camera state must be set to #CAMERA_STATE_PREVIEW.
 * @see camera_start_preview()
 */
int camera_request_codec_config(camera_h camera);

/**
 * @internal
 * @brief Gets the timestamp of preview frame in nano second.
 * @since_tizen 7.0
 * @remarks The function should be called in camera_preview_cb() or camera_media_packet_preview_cb(),\n
 *          otherwise, it will return #CAMERA_ERROR_INVALID_OPERATION.
 * @param[in] camera     The handle to the camera
 * @param[out] timestamp The timestamp of preview frame (in nsec)
 * @return @c 0 on success, otherwise a negative error value
 * @retval #CAMERA_ERROR_NONE Successful
 * @retval #CAMERA_ERROR_INVALID_OPERATION Internal error
 * @retval #CAMERA_ERROR_INVALID_PARAMETER Invalid parameter
 */
int camera_attr_get_preview_frame_timestamp(camera_h camera, unsigned long long *timestamp);

/**
 * @internal
 * @brief Gets the frame meta of preview frame.
 * @since_tizen 7.0
 * @remarks The function should be called in camera_preview_cb() or camera_media_packet_preview_cb(),\n
 *          otherwise, it will return #CAMERA_ERROR_INVALID_OPERATION.
 * @param[in] camera      The handle to the camera
 * @param[out] frame_meta The frame meta of preview buffer (in nsec for timestamp)
 * @return @c 0 on success, otherwise a negative error value
 * @retval #CAMERA_ERROR_NONE Successful
 * @retval #CAMERA_ERROR_INVALID_OPERATION Internal error
 * @retval #CAMERA_ERROR_INVALID_PARAMETER Invalid parameter
 */
int camera_attr_get_preview_frame_meta(camera_h camera, camera_frame_meta_s *frame_meta);

/**
 * @internal
 * @brief Gets the auto exposure status of preview frame.
 * @since_tizen 7.0
 * @remarks The function should be called in camera_preview_cb() or camera_media_packet_preview_cb(),\n
 *          otherwise, it will return #CAMERA_ERROR_INVALID_OPERATION.
 * @param[in] camera  The handle to the camera
 * @param[out] status The auto exposure status of preview frame
 * @return @c 0 on success, otherwise a negative error value
 * @retval #CAMERA_ERROR_NONE Successful
 * @retval #CAMERA_ERROR_INVALID_OPERATION Internal error
 * @retval #CAMERA_ERROR_INVALID_PARAMETER Invalid parameter
 */
int camera_attr_get_preview_frame_status_auto_exposure(camera_h camera, camera_status_auto_exposure_e *status);

/**
 * @internal
 * @brief Gets the auto white balance status of preview frame.
 * @since_tizen 7.0
 * @remarks The function should be called in camera_preview_cb() or camera_media_packet_preview_cb(),\n
 *          otherwise, it will return #CAMERA_ERROR_INVALID_OPERATION.
 * @param[in] camera  The handle to the camera
 * @param[out] status The auto white balance status of preview frame
 * @return @c 0 on success, otherwise a negative error value
 * @retval #CAMERA_ERROR_NONE Successful
 * @retval #CAMERA_ERROR_INVALID_OPERATION Internal error
 * @retval #CAMERA_ERROR_INVALID_PARAMETER Invalid parameter
 */
int camera_attr_get_preview_frame_status_auto_white_balance(camera_h camera, camera_status_auto_white_balance_e *status);

/**
 * @internal
 * @brief Gets the media packet preview internal callback feature's supported state.
 * @since_tizen 7.0
 * @remarks The specific error code can be obtained using the get_last_result() method. Error codes are described in Exception section.
 * @param[in] camera The handle to the camera
 * @return @c true if supported, otherwise @c false
 * @exception #CAMERA_ERROR_NONE Successful
 * @exception #CAMERA_ERROR_NOT_SUPPORTED The feature is not supported
 * @exception #CAMERA_ERROR_PERMISSION_DENIED The access to the resources can not be granted
 * @exception #CAMERA_ERROR_INVALID_PARAMETER Invalid parameter
 */
bool camera_is_supported_media_packet_preview_internal_cb(camera_h camera);

/**
 * @internal
 * @brief Sets a media packet callback function to be invoked once per frame when previewing.
 * @since_tizen 7.0
 * @remarks A @a callback is invoked on the internal thread of the camera.\n
 *          A video frame can be retrieved using a @a callback as a media packet.\n
 *          The callback function holds the same buffer that will be drawn on the display device.\n
 *          So if you change the media packet in a callback, it will be displayed on the device \n
 *          and the media packet is available until it's released by media_packet_unref().
 * @param[in] camera    The handle to the camera
 * @param[in] callback  The callback function to be invoked
 * @param[in] user_data The user data to be passed to the callback function
 * @return 0 on success, otherwise a negative error value
 * @retval #CAMERA_ERROR_NONE Successful
 * @retval #CAMERA_ERROR_NOT_SUPPORTED The feature is not supported
 * @retval #CAMERA_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #CAMERA_ERROR_SERVICE_DISCONNECTED The socket to multimedia server is disconnected
 * @see camera_start_preview()
 * @see camera_unset_media_packet_preview_internal_cb()
 * @see camera_media_packet_preview_cb()
 */
int camera_set_media_packet_preview_internal_cb(camera_h camera, camera_media_packet_preview_cb callback, void *user_data);

/**
 * @internal
 * @brief Unsets the media packet internal callback function.
 * @since_tizen 7.0
 * @param[in] camera The handle to the camera
 * @return @c 0 on success, otherwise a negative error value
 * @retval #CAMERA_ERROR_NONE Successful
 * @retval #CAMERA_ERROR_NOT_SUPPORTED The feature is not supported
 * @retval #CAMERA_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #CAMERA_ERROR_SERVICE_DISCONNECTED The socket to multimedia server is disconnected
 * @see camera_set_media_packet_preview_internal_cb()
 */
int camera_unset_media_packet_preview_internal_cb(camera_h camera);

/**
 * @internal
 * @brief Sets the tizen-core-wl display for video overlay.
 * @details Sets the display for video rendering using tizen-core-wl.
 *          @a display must be a tizen_core_wl_window_h handle.
 * @param[in] camera The handle to the camera
 * @param[in] display The display handle
 * @return @c 0 on success, otherwise a negative error value
 * @retval #CAMERA_ERROR_NONE Successful
 * @retval #CAMERA_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #CAMERA_ERROR_INVALID_OPERATION Invalid operation
 * @pre The camera state must be set to #CAMERA_STATE_CREATED.
 */
int camera_set_tcore_display(camera_h camera, camera_display_h display);


/**
 * @}
 */
#ifdef __cplusplus
}
#endif

#endif /* __TIZEN_MULTIMEDIA_CAMERA_INTERNAL_H__ */
