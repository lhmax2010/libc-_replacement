/*
 * Copyright (c) 2012, 2013 Samsung Electronics Co., Ltd.
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

#ifndef __TIZEN_CAPI_NETWORK_MTP_DB_H__
#define __TIZEN_CAPI_NETWORK_MTP_DB_H__

#include <glib.h>
#include <gio/gio.h>
#include <stdlib.h>
#include <string.h>
#include <sqlite3.h>
#include <tzplatform_config.h>

#include "mtp.h"
#include "mtp_internal.h"

#define MTP_DB_FILE tzplatform_mkpath(TZ_SYS_DB, "mtp/.mtp.db")
#define MTP_DB_TABLE "mtp_object_info"

mtp_error_e mtp_db_init(void);
mtp_error_e mtp_db_get_object_info(int mtp_device, int object_handle, mtp_object_info** object_info);
mtp_error_e mtp_db_deinit(void);

#endif /* __TIZEN_CAPI_NETWORK_MTP_DB_H__ */

