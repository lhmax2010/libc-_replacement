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

#ifndef __VCONF_INTERNAL_FILEMANAGER_KEYS_H__
#define __VCONF_INTERNAL_FILEMANAGER_KEYS_H__
/**
 *
 */
#define VCONFKEY_FILEMANAGER_MMC_STATUS  "memory/filemanager/Mmc"

enum {
	VCONFKEY_FILEMANAGER_MMC_REMOVED = 0,
		VCONFKEY_FILEMANAGER_MMC_LOADING,
		VCONFKEY_FILEMANAGER_MMC_LOADED,
		VCONFKEY_FILEMANAGER_MMC_FULL
};

/**
 *
 */
#define VCONFKEY_FILEMANAGER_DB_STATUS  "db/filemanager/dbupdate"

enum {
	VCONFKEY_FILEMANAGER_DB_UPDATING = 0,
		VCONFKEY_FILEMANAGER_DB_UPDATED
};

#endif /*   __VCONF_INTERNAL_FILEMANAGER_KEYS_H__    */
