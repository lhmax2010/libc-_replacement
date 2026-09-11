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

#ifndef SCREEN_CONNECTOR_WATCHER_EVAS_INCLUDE_SCREEN_CONNECTOR_WATCHER_EVAS_H_
#define SCREEN_CONNECTOR_WATCHER_EVAS_INCLUDE_SCREEN_CONNECTOR_WATCHER_EVAS_H_

#include <Evas.h>
#include <tizen.h>
#include <tizen_type.h>
#include <screen_connector_common_evas.h>

#ifdef __cplusplus
extern "C" {
#endif
/**
 * @file screen_connector_watcher_evas.h
 * @brief  This file declares API of libscreen_connector library
 * @since_tizen 3.0
 */

/**
 * @addtogroup CAPI_SCREEN_CONNECTOR_WATCHER_EVAS_MODULE
 * @{
 */

typedef void (*screen_connector_watcher_evas_added_cb)(const char* appid,
    const char* instance_id, const int pid, void* data);
typedef void (*screen_connector_watcher_evas_removed_cb)(const char* appid,
    const char* instance_id, const int pid, void* data);
typedef void (*screen_connector_watcher_evas_updated_cb)(const char* appid,
    const char* instance_id, const int pid, Evas_Object* image, void* data);

typedef struct _screen_connector_watcher_evas_ops {
  screen_connector_watcher_evas_added_cb added_cb;
  screen_connector_watcher_evas_removed_cb removed_cb;
  screen_connector_watcher_evas_updated_cb updated_cb;
} screen_connector_watcher_evas_ops;

typedef void* screen_connector_watcher_evas_h;

int screen_connector_watcher_evas_init(Evas_Object* win);
int screen_connector_watcher_evas_fini(void);
screen_connector_watcher_evas_h screen_connector_watcher_evas_add(
    screen_connector_watcher_evas_ops* ops, void* data);
int screen_connector_watcher_evas_remove(screen_connector_watcher_evas_h h);
int screen_connector_watcher_evas_update(
    screen_connector_watcher_evas_h watcher_evas_h, const char* instance_id);
int screen_connector_watcher_evas_set_changed_event_filter(
    screen_connector_watcher_evas_h watcher_evas_h,
    screen_connector_evas_changed_event_filter_type type);

/**
 * @}
 */

#ifdef __cplusplus
}
#endif

#endif  // SCREEN_CONNECTOR_WATCHER_EVAS_INCLUDE_SCREEN_CONNECTOR_WATCHER_EVAS_H_
