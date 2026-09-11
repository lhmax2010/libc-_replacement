/*
 * vconf-internal-keys
 *
 * Copyright (c) 2000 - 2011 Samsung Electronics Co., Ltd. All rights reserved.
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

#ifndef __VCONF_INTERNAL_LOCKSCREEN_KEYS_H__
#define __VCONF_INTERNAL_LOCKSCREEN_KEYS_H__
#ifdef USE_NONCREATED_VCONF_KEY
#warning using non-created vconf-key ----- please make sure this key is in use actually
#endif
/**
 *  This key is not used anymore since Tizen 3.0
 */
#define VCONFKEY_LOCKSCREEN_CAMERA_QUICK_ACCESS  "db/lockscreen/camera_quick_access"

/**
 *  This key is used to set a app shortcut in lockscreen. The value must be a application ID NOT a pakcage ID.
 */
#define VCONFKEY_LOCKSCREEN_QUICK_ACCESS_APPID  "db/lockscreen/quick_access_appid"

/**
 *
 */
#define VCONFKEY_LOCKSCREEN_WALLPAPER_TYPE  "db/lockscreen/wallpaper_type"

enum {
	LOCKSCREEN_WALLPAPER_TYPE_DEFAULT  = 0,
		LOCKSCREEN_WALLPAPER_TYPE_GALLERY,
		LOCKSCREEN_WALLPAPER_TYPE_MULTIPLE,
		LOCKSCREEN_WALLPAPER_TYPE_MAX
};

/**
 *
 */
#define VCONFKEY_LOCKSCREEN_WALLPAPER_COUNT  "db/lockscreen/wallpaper_count"

#endif /*   __VCONF_INTERNAL_LOCKSCREEN_KEYS_H__    */
