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

#ifndef SCREEN_CONNECTOR_WATCHER_EVAS_INCLUDE_SCREEN_CONNECTOR_TOOLKIT_EVAS_H_
#define SCREEN_CONNECTOR_WATCHER_EVAS_INCLUDE_SCREEN_CONNECTOR_TOOLKIT_EVAS_H_

#include <Evas.h>
#include <tizen.h>
#include <tizen_type.h>
#include <screen_connector_toolkit.h>
#include <screen_connector_common_evas.h>

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
  VISIBILITY_TYPE_UNOBSCURED,
  VISIBILITY_TYPE_PARTIALLY_OBSCURED,
  VISIBILITY_TYPE_FULLY_OBSCURED,
  VISIBILITY_TYPE_UNKNOWN,
} visibility_type;

typedef void (*screen_connector_toolkit_evas_added_cb)(const char* appid,
    const char* instance_id, const int pid, Evas_Object* image, void* data);
typedef void (*screen_connector_toolkit_evas_removed_cb)(const char* appid,
    const char* instance_id, const int pid, Evas_Object* image, void* data);
typedef void (*screen_connector_toolkit_evas_updated_cb)(const char* appid,
    const char* instance_id, const int pid, Evas_Object* image, void* data);

typedef struct _screen_connector_toolkit_evas_ops {
  screen_connector_toolkit_evas_added_cb added_cb;
  screen_connector_toolkit_evas_removed_cb removed_cb;
  screen_connector_toolkit_evas_updated_cb updated_cb;
} screen_connector_toolkit_evas_ops;

typedef void* screen_connector_toolkit_evas_h;

int screen_connector_toolkit_evas_init(Evas_Object* win,
    screen_connector_screen_type_e type);
int screen_connector_toolkit_evas_fini(screen_connector_screen_type_e type);
screen_connector_toolkit_evas_h screen_connector_toolkit_evas_add(
    screen_connector_toolkit_evas_ops* ops, char* id,
    screen_connector_screen_type_e type, void* data);
int screen_connector_toolkit_evas_set_delayed_resuming_time(int ms);
int screen_connector_toolkit_evas_remove(
    screen_connector_toolkit_evas_h handle);
int screen_connector_toolkit_evas_start_visibility_notify(void);
int screen_connector_toolkit_evas_stop_visibility_notify(void);
int screen_connector_toolkit_evas_freeze_visibility(Evas_Object* obj,
    visibility_type type);
int screen_connector_toolkit_evas_thaw_visibility(Evas_Object* obj);
int screen_connector_toolkit_evas_send_visibility(Evas_Object* obj,
    visibility_type type);
int screen_connector_toolkit_evas_send_mouse_up(Evas_Object* obj);
int screen_connector_toolkit_evas_send_touch_cancel(Evas_Object* obj);
int screen_connector_toolkit_evas_bind(screen_connector_toolkit_evas_h h,
    Evas_Object* win);
int screen_connector_toolkit_evas_unbind(screen_connector_toolkit_evas_h h);

/**
 * @brief Add screen connector toolkit by rid.
 * @details Using this API, viewer can watch screen which already watched by other viewer.\n
 * @since_tizen 3.0
 * @param[in]	obj		screen image object
 * @param[out]	pid		screen provider pid
 * @return	0 if success, negative value(<0) if fail
 *
 * @remark
 *	This API is only for main viewer. \n
 *	Sub viewer can not get provider's information like provider's pid by screen image object.
 * @see screen_connector_toolkit_evas_add_by_rid()
*/
int screen_connector_toolkit_evas_get_pid(Evas_Object* obj, int* pid);

/**
 * @brief Add screen connector toolkit by rid.
 * @details Using this API, viewer can watch screen which already watched by other viewer.\n
 * @since_tizen 3.0
 * @param[in]	ops		screen toolkit evas callback handler function
 * @param[in]	res_id		screen provider rid
 * @param[in]	data		user-supplied data for toolkit handler
 * @return @c Not null on success
 *
 * @remark
 *	This API is only for sub viewer. \n
 *	Sub viewer can not get provider's information like provider's pid by screen image object.
*/
screen_connector_toolkit_evas_h screen_connector_toolkit_evas_add_by_rid(
    screen_connector_toolkit_evas_ops* ops, int res_id,
    screen_connector_screen_type_e type, void* data);
int screen_connector_toolkit_evas_get_rid(Evas_Object* obj, int* resource_id);
int screen_connector_toolkit_evas_is_visible(Evas_Object* obj,
    bool* is_visible);
visibility_type screen_connector_toolkit_evas_auto_visibility(
    screen_connector_toolkit_evas_h h);
int screen_connector_toolkit_evas_set_changed_event_filter(
    screen_connector_toolkit_evas_h toolkit_evas_h,
    screen_connector_evas_changed_event_filter_type type);

/**
 * @}
 */

#ifdef __cplusplus
}
#endif

#endif  // SCREEN_CONNECTOR_WATCHER_EVAS_INCLUDE_SCREEN_CONNECTOR_TOOLKIT_EVAS_H_
