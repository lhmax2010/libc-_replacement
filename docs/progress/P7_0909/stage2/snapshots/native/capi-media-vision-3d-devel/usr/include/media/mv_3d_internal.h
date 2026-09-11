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

#ifndef __TIZEN_MEDIAVISION_3D_INTERNAL_H__
#define __TIZEN_MEDIAVISION_3D_INTERNAL_H__

#include "mv_3d.h"

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

/**
 * @file   mv_3d_internal.h
 * @brief  This file contains the mv3d internal structure and function for plane segmentation.
 * @since_tizen 7.0
 */

/**
 * @internal
 * @brief The plane model handle.
 * @since_tizen 7.0
 */
typedef void *mv_3d_pointcloud_plane_model_h;

/**
 * @internal
 * @brief The plane inlier handle.
 * @since_tizen 7.0
 */
typedef void *mv_3d_pointcloud_plane_inlier_h;

/**
 * @internal
 * @brief Creates plane model handle.
 * @details Use this function to create a plane model handle.
 *
 * @since_tizen 7.0
 */
int mv_3d_pointcloud_plane_model_create(mv_3d_pointcloud_plane_model_h *handle);

/**
 * @internal
 * @brief Destroys plane model handle and release all its resources.
 *
 * @since_tizen 7.0
 */
int mv_3d_pointcloud_plane_model_destroy(mv_3d_pointcloud_plane_model_h handle);

/**
 * @internal
 * @brief Creates plane inlier handle.
 * @details Use this function to create a plane model handle.
 *
 * @since_tizen 7.0
 */
int mv_3d_pointcloud_plane_inlier_create(mv_3d_pointcloud_plane_inlier_h *handle);

/**
 * @internal
 * @brief Destroys plane inlier handle and release all its resources.
 *
 * @since_tizen 7.0
 */
int mv_3d_pointcloud_plane_inlier_destroy(mv_3d_pointcloud_plane_inlier_h handle);

/**
 * @internal
 * @brief Segment PointCloud plane.
 * @details Use this function to segment pointcloud plane using the RANSAC algorithm.
 *
 * @since_tizen 7.0
 */
int mv_3d_pointcloud_segment_plane(mv_3d_h mv3d, mv_3d_pointcloud_h pointcloud,
								   mv_3d_pointcloud_plane_model_h *plane_model,
								   mv_3d_pointcloud_plane_inlier_h *plane_inlier);

/**
 * @internal
 * @brief Writes pointcloud plane data to a file.
 * @details Use this function to write pointcloud plane data to a file.
 *
 * @since_tizen 7.0
 */

int mv_3d_pointcloud_plane_write_file(mv_3d_h mv3d, mv_3d_pointcloud_plane_model_h model,
									  mv_3d_pointcloud_plane_inlier_h inlier, mv_3d_pointcloud_h pointcloud,
									  mv_3d_pointcloud_type_e type, char *filename);

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* __TIZEN_MEDIAVISION_3D_INTERNAL_H__ */
