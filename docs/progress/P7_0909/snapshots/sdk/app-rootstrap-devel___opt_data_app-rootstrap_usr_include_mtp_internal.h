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

#ifndef __TIZEN_CAPI_NETWORK_MTP_INTERNAL_H__
#define __TIZEN_CAPI_NETWORK_MTP_INTERNAL_H__

#include <mtp.h>

#ifdef __cplusplus
extern "C" {
#endif

#define MTP_ERROR_DB -980

/* internal struct */
typedef struct _mtp_object_info {
	int StorageID;
	int ObjectFormat;
	int ProtectionStatus;
	int ObjectCompressedSize;
	int ThumbFormat;
	int ThumbCompressedSize;
	int ThumbPixWidth;
	int ThumbPixHeight;
	int ImagePixWidth;
	int ImagePixHeight;
	int ImageBitDepth;
	int ParentObject;
	int AssociationType;
	int AssociationDesc;
	int SequenceNumber;
	char *Filename;
	int	CaptureDate;
	int	ModificationDate;
	char *Keywords;
} mtp_object_info;

/* internal api */
int mtp_objectinfo_get_object_info(int mtp_device, int object_handle, mtp_object_info **object_info);
int mtp_delete_object(mtp_device_h mtp_device, mtp_object_h object_handle);
int mtp_deviceinfo_get_bus_location(mtp_device_h mtp_device, int *bus_locaton);
int mtp_deviceinfo_get_device_number(mtp_device_h mtp_device, int *device_number);
int mtp_deviceinfo_get_stitching_engine_version(mtp_device_h mtp_device, char **stitching_engine_version);
int mtp_get_partial_object(int mtp_device, int object_handle, char *dest_path, int offset, int max_bytes, int *actual_written);

#ifdef __cplusplus
}
#endif

#endif /* __TIZEN_CAPI_NETWORK_MTP_INTERNAL_H__ */