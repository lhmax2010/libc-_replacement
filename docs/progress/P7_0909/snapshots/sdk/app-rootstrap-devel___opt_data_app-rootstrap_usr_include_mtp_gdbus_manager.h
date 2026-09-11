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

#ifndef __TIZEN_CAPI_NETWORK_MTP_GDBUS_MANAGER_H__
#define __TIZEN_CAPI_NETWORK_MTP_GDBUS_MANAGER_H__

#include "mtp_gdbus.h"

mtp_error_e mtp_gdbus_manager_initialize(void);
mtp_error_e mtp_gdbus_manager_get_devices(int **mtp_devices, int *device_num);
mtp_error_e mtp_gdbus_manager_get_storages(int mtp_device,
	int **mtp_storages, int *storage_num);
mtp_error_e mtp_gdbus_manager_get_object_handles(int mtp_device,
	int mtp_storage, int format, int parent_object_handle, int **object_handles, int *object_num);
mtp_error_e mtp_gdbus_manager_delete_object(int mtp_device, int object_handle);
mtp_error_e mtp_gdbus_manager_get_object(int mtp_device,
	int object_handle, char *dest_path);
mtp_error_e mtp_gdbus_manager_get_partial_object(int mtp_device,
	int object_handle, char *dest_path, int offset, int max_bytes, int *actual_written);
mtp_error_e mtp_gdbus_manager_get_thumbnail(int mtp_device,
	int object_handle, char *dest_path);
mtp_error_e mtp_gdbus_manager_add_event_cb(mtp_event_cb callback, void *user_data);
mtp_error_e mtp_gdbus_manager_remove_event_cb(mtp_event_cb callback);
mtp_error_e mtp_gdbus_manager_remove_all_event_cb(void);
mtp_error_e mtp_gdbus_manager_deinitialize(void);

#endif /* __TIZEN_CAPI_NETWORK_MTP_GDBUS_MANAGER_H__ */