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

#ifndef __TIZEN_MEDIAVISION_3D_PRIVATE_H__
#define __TIZEN_MEDIAVISION_3D_PRIVATE_H__

#include "mv_3d_type.h"
#include <cstdio>

#ifdef MV_3D_POINTCLOUD_IS_AVAILABLE
#include <open3d/Open3D.h>

using PointCloudPtr = std::shared_ptr<open3d::geometry::PointCloud>;

#endif

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

/**
 * @file   mv_3d_private.h
 * @brief  This file contains the internal structure for depth or pointcloud.
 * @since_tizen 7.0
 */

/**
  * @brief structure of pointcloud.
  *
  * @since_tizen 7.0
  *
  */
typedef struct {
	mv_3d_pointcloud_type_e type { MV_3D_POINTCLOUD_TYPE_PCD_BIN };
#ifdef MV_3D_POINTCLOUD_IS_AVAILABLE
	PointCloudPtr pointcloud;
#endif
} mv_3d_pointcloud_s;

/**
 * @}
 */
#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* __TIZEN_MEDIAVISION_3D_PRIVATE_H__ */
