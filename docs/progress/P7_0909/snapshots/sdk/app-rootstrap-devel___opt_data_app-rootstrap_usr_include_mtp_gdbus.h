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

#ifndef __TIZEN_CAPI_NETWORK_MTP_GDBUS_H__
#define __TIZEN_CAPI_NETWORK_MTP_GDBUS_H__

#include <glib.h>
#include <gio/gio.h>

#include "mtp.h"
#include "mtp_private.h"
#include "mtp_gdbuslib.h"

#define MTP_DBUS_SERVICE			"org.tizen.mtp"
#define MTP_DBUS_MANAGER_PATH		"/org/tizen/mtp/manager"
#define MTP_DBUS_DEVICEINFO_PATH	"/org/tizen/mtp/deviceinfo"
#define MTP_DBUS_STORAGEINFO_PATH	"/org/tizen/mtp/storageinfo"
#define MTP_DBUS_OBJECTINFO_PATH	"/org/tizen/mtp/objectinfo"

#endif /* __TIZEN_CAPI_NETWORK_MTP_GDBUS_H__ */