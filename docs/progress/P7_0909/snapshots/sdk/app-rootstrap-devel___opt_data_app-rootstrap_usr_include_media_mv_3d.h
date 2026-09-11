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

#ifndef __TIZEN_MEDIAVISION_3D_H__
#define __TIZEN_MEDIAVISION_3D_H__

#include <mv_3d_type.h>
#include <mv_common.h>
#include <stdbool.h>

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

/**
 * @file   mv_3d.h
 * @brief  This file contains the 3D API
 *         supporting depth and point cloud data process.
 */

/**
 * @addtogroup CAPI_MEDIA_VISION_3D_MODULE
 * @{
 */

/**
 * @brief Defines #MV_3D_DEPTH_MODE to set the engine configuration.
 *        Use #mv_3d_depth_mode_e for a value.
 *
 * @since_tizen 7.0
 */
#define MV_3D_DEPTH_MODE "MV_3D_DEPTH_MODE"

/**
 * @brief Defines #MV_3D_DEPTH_WIDTH to set the engine configuration.
 *
 * @since_tizen 7.0
 */
#define MV_3D_DEPTH_WIDTH "MV_3D_DEPTH_WIDTH"

/**
 * @brief Defines #MV_3D_DEPTH_HEIGHT to set the engine configuration.
 *
 * @since_tizen 7.0
 */
#define MV_3D_DEPTH_HEIGHT "MV_3D_DEPTH_HEIGHT"

/**
 * @brief Defines #MV_3D_DEPTH_MIN_DISPARITY to set the engine configuration.
 *
 * @since_tizen 7.0
 */
#define MV_3D_DEPTH_MIN_DISPARITY "MV_3D_DEPTH_MIN_DISPARITY"

/**
 * @brief Defines #MV_3D_DEPTH_MAX_DISPARITY to set the engine configuration.
 *
 * @since_tizen 7.0
 */
#define MV_3D_DEPTH_MAX_DISPARITY "MV_3D_DEPTH_MAX_DISPARITY"

/**
 * @brief Defines #MV_3D_DEPTH_STEREO_CONFIG_FILE_PATH to set the stereo configuration
 *       file path to the engine configuration.
 * @since_tizen 7.0
 */
#define MV_3D_DEPTH_STEREO_CONFIG_FILE_PATH "MV_3D_DEPTH_STEREO_CONFIG_FILE_PATH"

/**
 * @brief Defines #MV_3D_POINTCLOUD_OUTPUT_FILE_PATH to set the output file path
 *        to the engine configuration.
 * @since_tizen 7.0
 */
#define MV_3D_POINTCLOUD_OUTPUT_FILE_PATH "MV_3D_POINTCLOUD_OUTPUT_FILE_PATH"

/**
 * @brief Defines #MV_3D_POINTCLOUD_SAMPLING_RATIO to set the downsampling ratio
 *       to the engine configuration.
 * @since_tizen 7.0
 */
#define MV_3D_POINTCLOUD_SAMPLING_RATIO "MV_3D_POINTCLOUD_SAMPLING_RATIO"

/**
 * @brief Defines #MV_3D_POINTCLOUD_OUTLIER_REMOVAL_POINTS to set the criteria number of outlier
 *        removal points to the engine configuration.
 * @since_tizen 7.0
 */
#define MV_3D_POINTCLOUD_OUTLIER_REMOVAL_POINTS "MV_3D_POINTCLOUD_OUTLIER_REMOVAL_POINTS"

/**
 * @brief Defines #MV_3D_POINTCLOUD_OUTLIER_REMOVAL_RADIUS to set the outlier
 *        removal radius to the engine configuration.
 * @since_tizen 7.0
 */
#define MV_3D_POINTCLOUD_OUTLIER_REMOVAL_RADIUS "MV_3D_POINTCLOUD_OUTLIER_REMOVAL_RADIUS"

/**
 * @brief Creates mv3d handle.
 *
 * @since_tizen 7.0
 * @remarks The @a mv3d should be released using mv_3d_destroy()
 *          if there is no more usage of @a mv3d.
 * @param[out] mv3d    The created mv3d handle
 *
 * @return @c 0 on success, otherwise a negative error value
 * @retval #MEDIA_VISION_ERROR_NONE Successful
 * @retval #MEDIA_VISION_ERROR_NOT_SUPPORTED Not supported
 * @retval #MEDIA_VISION_ERROR_INVALID_PARAMETER Invalid parameter
 *
 * @see mv_3d_destroy()
 */
int mv_3d_create(mv_3d_h *mv3d);

/**
 * @brief Destroys mv3d handle and release all its resources.
 *
 * @since_tizen 7.0
 *
 * @param[in] mv3d    The handle to the mv3d to be destroyed
 *
 * @return @c 0 on success, otherwise a negative error value
 * @retval #MEDIA_VISION_ERROR_NONE Successful
 * @retval #MEDIA_VISION_ERROR_NOT_SUPPORTED Not supported
 * @retval #MEDIA_VISION_ERROR_INVALID_PARAMETER Invalid parameter
 *
 * @pre Create mv3d handle by using mv_3d_create()
 *
 * @see mv_3d_create()
 */
int mv_3d_destroy(mv_3d_h mv3d);

/**
 * @brief Configures handle.
 * @details Use this function to configure parameters of the mv3d
 *          which is set to @a engine_config.
 *
 * @since_tizen 7.0
 *
 * @param[in] mv3d          The handle to the mv3d
 * @param[in] engine_config The handle to the configuration of engine
 *
 * @return @c 0 on success, otherwise a negative error value
 * @retval #MEDIA_VISION_ERROR_NONE Successful
 * @retval #MEDIA_VISION_ERROR_NOT_SUPPORTED Not supported
 * @retval #MEDIA_VISION_ERROR_INVALID_PARAMETER Invalid parameter
 *
 * @pre Create mv3d handle by using mv_3d_create()
 * @pre Create engine_config handle by using mv_create_engine_config()
 *
 * @see mv_3d_create()
 * @see mv_create_engine_config()
 */
int mv_3d_configure(mv_3d_h mv3d, mv_engine_config_h engine_config);

/**
 * @brief Prepares handle.
 *
 * @since_tizen 7.0
 *
 * @param[in] mv3d    The handle to the mv3d
 *
 * @return @c 0 on success, otherwise a negative error value
 * @retval #MEDIA_VISION_ERROR_NONE Successful
 * @retval #MEDIA_VISION_ERROR_NOT_SUPPORTED Not supported
 * @retval #MEDIA_VISION_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #MEDIA_VISION_ERROR_INVALID_OPERATION Invalid operation
 *
 * @pre Configure mv3d handle by using mv_3d_configure()
 *
 * @see mv_3d_configure()
 */
int mv_3d_prepare(mv_3d_h mv3d);

/**
 * @brief Invoked when @a depth is ready.
 * @details This callback is invoked each time when
 *          mv_3d_run() or mv3d_run_async() is called
 *          to report estimated depth from given source(s).
 *
 * @since_tizen 7.0
 * @remarks The @a source is available until it is released by mv_destroy_source().
 *          The @a depth can be used only in the callback.
 *          To use outside, make a copy. The @a depth is managed by the platform
 *          and should not be freed.
 * @param[in] source    The handle to the source of the media where
 *                      the depth data comes from
 * @param[in] depth     The pointer of the depth data
 * @param[in] width     The width of @a depth
 * @param[in] height    The height of @a depth
 * @param[in] user_data The user data passed from callback invoking code
 *
 * @pre Call mv_3d_run() or mv_3d_run_async() function to get depth data
 *      and to invoke this callback as a result
 *
 * @see mv_3d_run()
 * @see mv_3d_run_async()
 */
typedef void (*mv_3d_depth_cb)(mv_source_h source, unsigned short *depth, unsigned int width, unsigned int height,
							   void *user_data);

/**
 * @brief Sets mv_3d_depth_cb() callback.
 * @details Use this function to set mv_3d_depth_cb() callback.
 *
 * @since_tizen 7.0
 *
 * @param[in] mv3d       The handle to the mv3d
 * @param[in] depth_cb   The callback which will be invoked for
 *                       getting depth data
 * @param[in] user_data  The user data passed from the code where
 *                       mv_3d_run() or mv_3d_run_async() is invoked. This data will be
 *                       accessible in @a depth_cb callback
 *
 * @return @c 0 on success, otherwise a negative error value
 * @retval #MEDIA_VISION_ERROR_NONE Successful
 * @retval #MEDIA_VISION_ERROR_NOT_SUPPORTED Not supported
 * @retval #MEDIA_VISION_ERROR_INVALID_PARAMETER Invalid parameter
 *
 * @pre Create mv3d handle by using mv_3d_create()
 * @pre Create engine_config handle by using mv_create_engine_config()
 * @pre Configure mv3d handle by using mv_3d_configure()
 * @see mv_3d_create()
 * @see mv_create_engine_config()
 * @see mv_3d_configure()
 */
int mv_3d_set_depth_cb(mv_3d_h mv3d, mv_3d_depth_cb depth_cb, void *user_data);

/**
 * @brief Invoked when @a pointcloud is ready.
 * @details This callback is invoked each time when
 *          mv_3d_run() or mv3d_run_async() is called
 *          to report estimated pointcloud from given source(s).
 *
 * @since_tizen 7.0
 * @remarks The @a source is available until it is released by mv_destroy_source().
 *          The @a pointcloud can be used only in the callback.
 *          To use outside, make a copy. The @a pointcloud is managed by the platform
 *          and should not be freed.
 * @param[in] source     The handle to the source of the media where
 *                       the pointcloud data comes from
 * @param[in] pointcloud The pointer of the pointcloud result
 * @param[in] user_data  The user data passed from callback invoking code
 *
 * @pre Call mv_3d_run() or mv_3d_run_async() function to get pointcloud data
 *      and to invoke this callback as a result
 *
 * @see mv_3d_run()
 * @see mv_3d_run_async()
 */
typedef void (*mv_3d_pointcloud_cb)(mv_source_h source, mv_3d_pointcloud_h pointcloud, void *user_data);

/**
 * @brief Sets mv_3d_pointcloud_cb() callback.
 * @details Use this function to set mv_3d_pointcloud_cb() callback.
 *
 * @since_tizen 7.0
 *
 * @param[in] mv3d              The handle to the mv3d
 * @param[in] pointcloud_cb     The callback which will be invoked for
 *                              getting pointcloud data
 * @param[in] user_data         The user data passed from the code where
 *                              mv_3d_run() or mv_3d_run_async() is invoked. This data will be
 *                              accessible in @a pointcloud_cb callback
 *
 * @return @c 0 on success, otherwise a negative error value
 * @retval #MEDIA_VISION_ERROR_NONE Successful
 * @retval #MEDIA_VISION_ERROR_NOT_SUPPORTED Not supported
 * @retval #MEDIA_VISION_ERROR_INVALID_PARAMETER Invalid parameter
 *
 * @pre Create mv3d handle by using mv_3d_create()
 * @pre Create engine_config handle by using mv_create_engine_config()
 * @pre Configure mv3d handle by using mv_3d_configure()
 * @see mv_3d_create()
 * @see mv_create_engine_config()
 * @see mv_3d_configure()
 */
int mv_3d_set_pointcloud_cb(mv_3d_h mv3d, mv_3d_pointcloud_cb pointcloud_cb, void *user_data);

/**
 * @brief Gets depth or pointcloud synchronously from given @a source
 *        or @a source_extra.
 * @details Use this function to get depth data.
 *          @a source_extra can be null if @a source is a stereoscopic format
 *          media, for example a left and a right media are concatenated
 *          as a side-by-side format and then it should be given to @a source
 *          but @a source_extra should be null.
 *          @a source_extra should not be null if @a source is a mono format media or
 *          a single side/channel of a stereoscopic format, for example a left
 *          and a right media are separated and then they should be given to @a source
 *          and @a source_extra, respectively.
 *          @a color may not be null if pointcloud data includes color.
 *
 * @since_tizen 7.0
 *
 * @param[in] mv3d            The handle to the mv3d
 * @param[in] source          The handle to the source of the media
 * @param[in] source_extra    The handle to the extra source of the media
 * @param[in] color           The handle to the color of the media
 *
 * @return @c 0 on success, otherwise a negative error value
 * @retval #MEDIA_VISION_ERROR_NONE Successful
 * @retval #MEDIA_VISION_ERROR_NOT_SUPPORTED Not supported
 * @retval #MEDIA_VISION_ERROR_INVALID_PARAMETER Invalid parameter
 *
 * @pre Create a source handle by using mv_create_source()
 * @pre Create a source_extra handle by using mv_create_source()
 * @pre Create a mv3d handle by using mv_3d_create()
 * @pre Configure a mv3d handle by using mv_3d_configure()
 * @pre Set depth callback to a depth handle by using mv_3d_set_depth_cb()
 * @pre Prepare a mv3d handle by using mv_3d_prepare()
 * @post Callback which is set by mv_3d_set_depth_cb() will be invoked
 *       to provide depth data
 *
 * @see mv_3d_set_depth_cb()
 * @see mv_3d_depth_cb()
 */
int mv_3d_run(mv_3d_h mv3d, mv_source_h source, mv_source_h source_extra, mv_source_h color);

/**
 * @brief Gets depth or pointcloud asynchronously from given @a source
 *        or @a source_extra.
 * @details Use this function to get depth data.
 *          @a source_extra can be null if @a source is a stereoscopic format
 *          media, for example a left and a right media are concatenated
 *          as a side-by-side format and then it should be given to @a source
 *          but @a source_extra should be null.
 *          @a source_extra should not be null if @a source is a mono format media or
 *          a single side/channel of a stereoscopic format, for example a left
 *          and a right media are separated and then they should be given to @a source
 *          and @a source_extra, respectively.
 *          @a color may not be null if pointcloud data includes color.
 *
 * @since_tizen 7.0
 *
 * @param[in] mv3d            The handle to the mv3d
 * @param[in] source          The handle to the source of the media
 * @param[in] source_extra    The handle to the extra source of the media
 * @param[in] color           The handle to the color of the media
 *
 * @return @c 0 on success, otherwise a negative error value
 * @retval #MEDIA_VISION_ERROR_NONE Successful
 * @retval #MEDIA_VISION_ERROR_NOT_SUPPORTED Not supported
 * @retval #MEDIA_VISION_ERROR_INVALID_PARAMETER Invalid parameter
 *
 * @pre Create a source handle by using mv_create_source()
 * @pre Create a source_extra handle by using mv_create_source()
 * @pre Create a mv3d handle by using mv_3d_create()
 * @pre Configure a mv3d handle by using mv_3d_configure()
 * @pre Set depth callback to a depth handle by using mv_3d_set_depth_cb()
 * @pre Prepare a mv3d handle by using mv_3d_prepare()
 * @post Callback which is set by mv_3d_set_depth_cb() will be invoked
 *       to provide depth data
 *
 * @see mv_3d_set_depth_cb()
 * @see mv_3d_depth_cb()
 */
int mv_3d_run_async(mv_3d_h mv3d, mv_source_h source, mv_source_h source_extra, mv_source_h color);

/**
 * @brief Writes pointcloud data to a file.
 * @details Use this function to write pointcloud data to a file.
 *
 * @since_tizen 7.0
 * @remarks The mediastorage privilege %http://tizen.org/privilege/mediastorage
 *          is needed if @a filename is relevant to media storage.\n
 *          The externalstorage privilege %http://tizen.org/privilege/externalstorage
 *          is needed if @a filename is relevant to external storage.
 * @param[in] mv3d         The handle to the mv3d
 * @param[in] pointcloud   The handle to the pointcloud
 * @param[in] type         The file format type to be written
 * @param[in] filename     The filename to save @a pointcloud
 *
 * @return @c 0 on success, otherwise a negative error value
 * @retval #MEDIA_VISION_ERROR_NONE Successful
 * @retval #MEDIA_VISION_ERROR_NOT_SUPPORTED Not supported
 * @retval #MEDIA_VISION_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #MEDIA_VISION_ERROR_PERMISSION_DENIED Permission denied
 * @retval #MEDIA_VISION_ERROR_INVALID_PATH Invalid path
 *
 * @pre Create mv3d handle by using mv_3d_create()
 * @pre Create engine_config handle by using mv_create_engine_config()
 * @pre Configure mv3d handle by using mv_3d_configure()
 * @see mv_3d_create()
 * @see mv_create_engine_config()
 * @see mv_3d_configure()
 */
int mv_3d_pointcloud_write_file(mv_3d_h mv3d, mv_3d_pointcloud_h pointcloud, mv_3d_pointcloud_type_e type,
								char *filename);

/**
 * @}
 */
#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* __TIZEN_MEDIAVISION_3D_H__ */
