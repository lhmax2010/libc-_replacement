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

#ifndef __TIZEN_CAPI_NETWORK_MTP_GDBUS_DEVICEINFO_H__
#define __TIZEN_CAPI_NETWORK_MTP_GDBUS_DEVICEINFO_H__

#include "mtp_gdbus.h"

void mtp_gdbus_deviceinfo_proxy_init(void);
void mtp_gdbus_deviceinfo_proxy_deinit(void);

mtp_error_e mtp_gdbus_deviceinfo_get_manufacturer_name(int mtp_device, char **manufacturername);
mtp_error_e mtp_gdbus_deviceinfo_get_model_name(int mtp_device, char **modelname);
mtp_error_e mtp_gdbus_deviceinfo_get_serial_number(int mtp_device, char **serialnumber);
mtp_error_e mtp_gdbus_deviceinfo_get_device_version(int mtp_device, char **deviceversion);
mtp_error_e mtp_gdbus_deviceinfo_get_bus_location(int mtp_device, int *bus_location);
mtp_error_e mtp_gdbus_deviceinfo_get_device_number(int mtp_device, int *device_number);
mtp_error_e mtp_gdbus_deviceinfo_get_stitching_engine_version(int mtp_device, char **stitching_engine_version);

#endif /* __TIZEN_CAPI_NETWORK_MTP_GDBUS_DEVICEINFO_H__ */