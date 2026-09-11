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

#ifndef __VCONF_INTERNAL_CONTACTS_SVC_KEYS_H__
#define __VCONF_INTERNAL_CONTACTS_SVC_KEYS_H__
/**
 *  Do not remove, it is in use (jiwon44.park@samsung.com)
 */
#define VCONFKEY_CONTACTS_SVC_NAME_DISPLAY_ORDER  "db/contacts-svc/name_display_order"

enum {
	VCONFKEY_CONTACTS_SVC_NAME_DISPLAY_ORDER_FIRSTLAST = 0,
		VCONFKEY_CONTACTS_SVC_NAME_DISPLAY_ORDER_LASTFIRST = 1
};

/**
 *  Do not remove, it is in use (jiwon44.park@samsung.com)
 */
#define VCONFKEY_CONTACTS_SVC_NAME_SORTING_ORDER  "db/contacts-svc/name_sorting_order"

enum {
	VCONFKEY_CONTACTS_SVC_NAME_SORTING_ORDER_FIRSTLAST = 0,
		VCONFKEY_CONTACTS_SVC_NAME_SORTING_ORDER_LASTFIRST = 1
};

/**
 *  Do not remove, it is in use (jiwon44.park@samsung.com)
 */
#define VCONFKEY_CONTACTS_SVC_PHONENUMBER_MIN_MATCH_DIGIT  "db/contacts-svc/phonenumber_min_match_digit"

#endif /*   __VCONF_INTERNAL_CONTACTS_SVC_KEYS_H__    */
