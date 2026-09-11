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

#ifndef __VCONF_INTERNAL_WMS_KEYS_H__
#define __VCONF_INTERNAL_WMS_KEYS_H__
#ifdef USE_NONCREATED_VCONF_KEY
#warning using non-created vconf-key ----- please make sure this key is in use actually
#endif
/**
 *
 */
#define VCONFKEY_WMS_FAVORITE_ORDER  "memory/wms/favorites_order"

#ifdef USE_NONCREATED_VCONF_KEY
#warning using non-created vconf-key ----- please make sure this key is in use actually
#endif
/**
 *
 */
#define VCONFKEY_WMS_APPS_ORDER  "memory/wms/apps_order"

/**
 *
 */
#define VCONFKEY_WMS_WAKEUP_BY_GESTURE_SETTING  "db/wms/wakeup_by_gesture_setting"

/**
 *
 */
#define VCONFKEY_WMS_CLOCKS_SET_IDLE  "db/wms/clocks_set_idle"

/**
 *
 */
#define VCONFKEY_WMS_POWERKEY_DOUBLE_PRESSING  "db/wms/powerkey_double_pressing"

/**
 *
 */
#define VCONFKEY_WMS_SMART_RELAY  "db/wms/smart_relay"

/**
 *
 */
#define VCONFKEY_WMS_WMANAGER_CONNECTED  "memory/wms/wmanager_connected"

#endif /*   __VCONF_INTERNAL_WMS_KEYS_H__    */
