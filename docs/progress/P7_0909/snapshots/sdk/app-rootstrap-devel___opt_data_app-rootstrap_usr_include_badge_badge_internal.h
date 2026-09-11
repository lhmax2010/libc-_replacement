/*
 * Copyright (c) 2000 - 2017 Samsung Electronics Co., Ltd. All rights reserved.
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
 *
 */

#ifndef __BADGE_INTERNAL_DEF_H__
#define __BADGE_INTERNAL_DEF_H__

#include <sys/types.h>

#include "badge.h"
#include "badge_error.h"

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

int badge_is_service_ready(void);

int badge_add_deferred_task(
		void (*badge_add_deferred_task)(void *data), void *user_data);

int badge_del_deferred_task(
		void (*badge_add_deferred_task)(void *data));

int badge_is_existing(const char *app_id, bool *existing);

int badge_create(const char *pkgname, const char *writable_pkg);

int badge_create_for_uid(const char *pkgname, const char *writable_pkg, uid_t uid);

int badge_new_for_uid(const char *writable_app_id, uid_t uid);

int badge_new_for(const char *badge_app_id, const char *writable_app_id);

int badge_new_for_for_uid(const char *badge_app_id, const char *writable_app_id, uid_t uid);

int badge_add_for_uid(const char *badge_app_id, uid_t uid);

int badge_remove_for_uid(const char *app_id, uid_t uid);

int badge_is_existing_for_uid(const char *app_id, bool *existing, uid_t uid);

int badge_foreach_for_uid(badge_foreach_cb callback, void *user_data, uid_t uid);

int badge_set_count_for_uid(const char *app_id, unsigned int count, uid_t uid);

int badge_get_count_for_uid(const char *app_id, unsigned int *count, uid_t uid);

int badge_set_display_for_uid(const char *app_id, unsigned int is_display, uid_t uid);

int badge_get_display_for_uid(const char *app_id, unsigned int *is_display, uid_t uid);

int badge_register_changed_cb_for_uid(badge_change_cb callback, void *data, uid_t uid);

int badge_unregister_changed_cb_for_uid(badge_change_cb callback, uid_t uid);


#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* __BADGE_INTERNAL_DEF_H__ */

