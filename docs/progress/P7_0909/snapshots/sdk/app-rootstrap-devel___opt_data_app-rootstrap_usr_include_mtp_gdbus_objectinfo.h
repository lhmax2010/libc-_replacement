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

#ifndef __TIZEN_CAPI_NETWORK_MTP_GDBUS_OBJECTINFO_H__
#define __TIZEN_CAPI_NETWORK_MTP_GDBUS_OBJECTINFO_H__

#include "mtp_gdbus.h"

void mtp_gdbus_objectinfo_proxy_init(void);
void mtp_gdbus_objectinfo_proxy_deinit(void);

int mtp_gdbus_objectinfo_get_property(int mtp_device,
	int object_handle, int property, int *property_value);
int mtp_gdbus_objectinfo_get_property_string(int mtp_device,
	int object_handle, int property, char **property_value);

#endif /* __TIZEN_CAPI_NETWORK_MTP_GDBUS_OBJECTINFO_H__ */