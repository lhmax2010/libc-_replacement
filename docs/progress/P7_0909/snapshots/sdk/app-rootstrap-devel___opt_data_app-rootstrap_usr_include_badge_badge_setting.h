/*
 * Copyright (c) 2016 - 2017 Samsung Electronics Co., Ltd. All rights reserved.
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

#ifndef __BADGE_SETTING_H__
#define __BADGE_SETTING_H__

#ifdef __cplusplus
extern "C" {
#endif /*  __cplusplus */

typedef struct badge_setting *badge_setting_h;

struct badge_setting {
	char *pkgname;
	char *appid;
	int allow_to_display;
};

int badge_setting_get_pkgname(badge_setting_h setting, char **pkgname);
int badge_setting_get_appid(badge_setting_h setting, char **appid);
int badge_setting_set_allow_to_display(badge_setting_h setting, bool value);
int badge_setting_get_allow_to_display(badge_setting_h setting, bool *value);
int badge_setting_update_setting(badge_setting_h setting);
int badge_setting_update_setting_for_uid(badge_setting_h setting, uid_t uid);
int badge_setting_get_setting_by_appid(badge_setting_h *setting, const char *appid);
int badge_setting_get_setting_by_appid_for_uid(badge_setting_h *setting, const char *appid, uid_t uid);
int badge_setting_free_setting(badge_setting_h setting);

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* __BADGE_SETTING_H__ */
