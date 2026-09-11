/*
 * Copyright (c) 2016 Samsung Electronics Co., Ltd All Rights Reserved
 *
 * Licensed under the Apache License, Version 2.0 (the License);
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 * http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an AS IS BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#ifndef SCREEN_CONNECTOR_WATCHER_EVAS_INCLUDE_SCREEN_CONNECTOR_COMMON_EVAS_H_
#define SCREEN_CONNECTOR_WATCHER_EVAS_INCLUDE_SCREEN_CONNECTOR_COMMON_EVAS_H_

#include <tizen.h>

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @file screen_connector_toolkit_evas.h
 * @brief  This file declares API of libscreen_connector library
 * @since_tizen 3.0
 */

/**
 * @addtogroup CAPI_SCREEN_CONNECTOR_WATCHER_EVAS_MODULE
 * @{
 */
typedef enum {
  SCREEN_CONNECTOR_EVAS_CHANGED_EVENT_FILTER_NONE,
  SCREEN_CONNECTOR_EVAS_CHANGED_EVENT_FILTER_TBM,
  SCREEN_CONNECTOR_EVAS_CHANGED_EVENT_FILTER_IMAGE_FILE,
  SCREEN_CONNECTOR_EVAS_CHANGED_EVENT_FILTER_ALL
} screen_connector_evas_changed_event_filter_type;

/**
 * @}
 */

#ifdef __cplusplus
}
#endif

#endif  // SCREEN_CONNECTOR_WATCHER_EVAS_INCLUDE_SCREEN_CONNECTOR_COMMON_EVAS_H_
