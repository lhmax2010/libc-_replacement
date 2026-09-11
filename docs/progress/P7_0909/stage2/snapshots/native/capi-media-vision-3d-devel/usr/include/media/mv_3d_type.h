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

#ifndef __TIZEN_MEDIAVISION_3D_TYPE_H__
#define __TIZEN_MEDIAVISION_3D_TYPE_H__

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

/**
 * @file   mv_3d_type.h
 * @brief  This file contains the mv3d type used.
 * @since_tizen 7.0
 */

/**
 * @addtogroup CAPI_MEDIA_VISION_3D_MODULE
 * @{
 */

/**
 * @brief Enumeration for mv3d depth mode.
 * @since_tizen 7.0
 */
typedef enum {
	MV_3D_DEPTH_MODE_NONE, /**< NONE */
	MV_3D_DEPTH_MODE_STEREO /**< Stereo images are used */
} mv_3d_depth_mode_e;

/**
 * @brief Enumeration for mv3d pointcloud file format type.
 * @since_tizen 7.0
 */
typedef enum {
	MV_3D_POINTCLOUD_TYPE_PCD_TXT, /**< PointCloudData format as text */
	MV_3D_POINTCLOUD_TYPE_PCD_BIN, /**< PointCloudData format as binary */
	MV_3D_POINTCLOUD_TYPE_PLY_TXT, /**< Polygon format as text */
	MV_3D_POINTCLOUD_TYPE_PLY_BIN /**< Polygon format as binary */
} mv_3d_pointcloud_type_e;

/**
 * @brief The mv3d handle.
 * @since_tizen 7.0
 */
typedef void *mv_3d_h;

/**
 * @brief The pointcloud result handle.
 * @since_tizen 7.0
 */
typedef void *mv_3d_pointcloud_h;

/**
 * @}
 */
#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* __TIZEN_MEDIAVISION_3D_TYPE_H__ */
