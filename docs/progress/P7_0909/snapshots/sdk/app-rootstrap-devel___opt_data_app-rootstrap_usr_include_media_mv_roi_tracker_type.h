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

#ifndef __MEDIAVISION_ROI_TRACKER_TYPE_H__
#define __MEDIAVISION_ROI_TRACKER_TYPE_H__

#include <mv_common.h>

#define MAX_LABEL_LEN 128
#define MAX_LABEL_CNT 100

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

/**
 * @file  mv_roi_tracker_type.h
 * @brief This file contains enumerations and a handle required by
 *        MediaVision ROI tracker API.
 */

/**
 * @addtogroup CAPI_MEDIA_VISION_ROI_TRACKER_MODULE
 * @{
 */

/**
 * @brief The ROI tracker result structure.
 * @details Contains roi tracker result such as coordinates.
 * @since_tizen 7.0
 */
typedef struct {
	int x; /**< Left-top x coordinate of tracked region */
	int y; /**< Left-top y coordinate of tracked region */
	int width; /**< Width of tracked region */
	int height; /**< Height of tracked region */
	bool initialized; /**< flag that struct is initialized or not */
} mv_roi_tracker_result_s;

/**
 * @brief The ROI tracker handle.
 * @since_tizen 7.0
 */
typedef void *mv_roi_tracker_h;

/**
 * @brief Enumeration for ROI tracker type.
 * @since_tizen 7.0
 */
typedef enum {
	MV_ROI_TRACKER_TYPE_NONE = 0, /**< None */
	MV_ROI_TRACKER_TYPE_ACCURACY, /**< Tracker type focused on accuracy */
	MV_ROI_TRACKER_TYPE_BALANCE, /**< Tracker type focused on balance */
	MV_ROI_TRACKER_TYPE_SPEED /**< Tracker type focused on speed */
} mv_roi_tracker_type_e;
/**
 * @}
 */

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* __MEDIAVISION_ROI_TRACKER_TYPE_H__ */
