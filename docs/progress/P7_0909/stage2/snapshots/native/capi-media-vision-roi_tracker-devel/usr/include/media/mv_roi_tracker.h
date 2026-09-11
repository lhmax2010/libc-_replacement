/*
 * Copyright (c) 2022 Samsung Electronics Co., Ltd All Rights Reserved
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

#ifndef __MEDIAVISION_ROI_TRACKER_H__
#define __MEDIAVISION_ROI_TRACKER_H__

#include <mv_common.h>
#include <mv_roi_tracker_type.h>

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

/**
 * @file   mv_roi_tracker.h
 * @brief  This file contains the ROI tracker based Media Vision API.
 */

/**
 * @addtogroup CAPI_MEDIA_VISION_ROI_TRACKER_MODULE
 * @{
 */

/**
 * @brief Defines #MV_ROI_TRACKER_TYPE to set the type used
 *        for tracker type attribute of the engine configuration.
 * @details Switches between SPEED, BALANCED, or ACCURACY\n
 *          #MV_ROI_TRACKER_TYPE_ACCURACY,\n
 *          #MV_ROI_TRACKER_TYPE_BALANCE,\n
 *          #MV_ROI_TRACKER_TYPE_SPEED.\n
 *
 *          The default type is #MV_ROI_TRACKER_TYPE_BALANCE.
 *
 * @since_tizen 7.0
 * @see mv_engine_config_set_int_attribute()
 * @see mv_engine_config_get_int_attribute()
 */
#define MV_ROI_TRACKER_TYPE "MV_ROI_TRACKER_TYPE"

/**
 * @brief Creates tracker handle.
 * @details Use this function to create a tracker handle.
 *
 * @since_tizen 7.0
 *
 * @remarks The @a handle should be released using mv_roi_tracker_destroy().
 *
 * @param[out] handle   The handle to the tracker to be created
 *
 * @return @c 0 on success, otherwise a negative error value
 * @retval #MEDIA_VISION_ERROR_NONE Successful
 * @retval #MEDIA_VISION_ERROR_NOT_SUPPORTED Not supported
 * @retval #MEDIA_VISION_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #MEDIA_VISION_ERROR_OUT_OF_MEMORY Out of memory
 *
 * @see mv_roi_tracker_destroy()
 */
int mv_roi_tracker_create(mv_roi_tracker_h *handle);

/**
 * @brief Destroys tracker handle and release all its resources.
 *
 * @since_tizen 7.0
 *
 * @param[in] handle    The handle to the tracker object to be destroyed
 *
 * @return @c 0 on success, otherwise a negative error value
 * @retval #MEDIA_VISION_ERROR_NONE Successful
 * @retval #MEDIA_VISION_ERROR_INVALID_PARAMETER Invalid parameter
 *
 * @pre Create tracker handle by using mv_roi_tracker_create()
 */
int mv_roi_tracker_destroy(mv_roi_tracker_h handle);

/**
 * @brief Configures the attributes of the roi tracker.
 * @details Use this function to configure the attributes of the roi tracker
 *          which is set to @a engine_config.
 *
 * @since_tizen 7.0
 *
 * @param[in] handle         The handle to the roi tracker
 * @param[in] engine_config The handle to the configuration of
 *                           engine.
 *
 * @return @c 0 on success, otherwise a negative error value
 * @retval #MEDIA_VISION_ERROR_NONE Successful
 * @retval #MEDIA_VISION_ERROR_NOT_SUPPORTED Not supported
 * @retval #MEDIA_VISION_ERROR_INVALID_PARAMETER Invalid parameter
 *                                               in @a engine_config
 */
int mv_roi_tracker_configure(mv_roi_tracker_h handle, mv_engine_config_h engine_config);

/**
 * @brief Prepares roi tracker.
 * @details Use this function to prepare roi tracker based on
 *          the configuration. ROI related variables are used when 'mv_roi_tracker_perform()' is executed.
 *
 * @since_tizen 7.0
 *
 * @param[in] handle    The handle to the roi tracker
 * @param[in] x         The x coordinate to set ROI to be tracked
 * @param[in] y         The y coordinate to set ROI to be tracked
 * @param[in] width     The width to set ROI to be tracked
 * @param[in] height    The height to set ROI to be tracked
 * @return @c 0 on success, otherwise a negative error value
 * @retval #MEDIA_VISION_ERROR_NONE Successful
 * @retval #MEDIA_VISION_ERROR_NOT_SUPPORTED Not supported
 * @retval #MEDIA_VISION_ERROR_INVALID_PARAMETER Invalid parameter
 */
int mv_roi_tracker_prepare(mv_roi_tracker_h handle, int x, int y, int width, int height);

/**
 * @brief Called when roi in @a source are detected.
 * @details This type callback is invoked each time when
 *          mv_roi_tracker_perform() is called to provide
 *          the results of the tracked roi.
 *
 * @since_tizen 7.0
 * @remarks The @a roi should not be released by app. They can be used only in the callback.
 *
 * @param[in] source     The handle to the source of the media where
 *                       roi tracker were performed. @a source is the same object
 *                       for which mv_roi_tracker_perform() was called.
 *                       It should be released by calling mv_destroy_source()
 *                       when it's not needed anymore.
 * @param[in] roi        Roi of the tracked result.
 * @param[in] user_data  The user data passed from callback invoking code
 *
 * @see mv_roi_tracker_perform()
 */
typedef void (*mv_roi_tracker_tracked_cb)(mv_source_h source, mv_rectangle_s roi, void *user_data);

/**
 * @brief Tracks with a given tracker on the @a source.
 * @details Use this function to track with a given source and ROI information which is set to 'mv_roi_tracker_prepare()'.
 *          This function returns a proper ROI coordinates of the tracked region inside given source.
 *
 * @since_tizen 7.0
 *
 * @param[in]  handle     The handle to the tracker object.
 * @param[in]  source     The handle to the source of the media.
 * @param[in]  tracked_cb The callback which will receive the tracked results.
 * @param[in]  user_data  The user data passed from the code where
 *                        mv_roi_tracker_perform() is invoked.
 *                        This data will be accessible in @a tracked_cb callback.
 * @return @c 0 on success, otherwise a negative error value
 * @retval #MEDIA_VISION_ERROR_NONE Successful
 * @retval #MEDIA_VISION_ERROR_NOT_SUPPORTED Not supported
 * @retval #MEDIA_VISION_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #MEDIA_VISION_ERROR_OUT_OF_MEMORY Out of memory
 * @retval #MEDIA_VISION_ERROR_INVALID_OPERATION Invalid operation
 * @retval #MEDIA_VISION_ERROR_NOT_SUPPORTED_FORMAT Source colorspace
 *                                                  isn't supported
 *
 * @pre Create a new tracker handle by calling mv_roi_tracker_create()
 */
int mv_roi_tracker_perform(mv_roi_tracker_h handle, mv_source_h source, mv_roi_tracker_tracked_cb tracked_cb,
						   void *user_data);

/**
 * @}
 */

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* __MEDIAVISION_ROI_TRACKER_H__ */