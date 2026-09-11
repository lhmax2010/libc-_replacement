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

#ifndef __TIZEN_CAPI_NETWORK_MTP_PRIVATE_H__
#define __TIZEN_CAPI_NETWORK_MTP_PRIVATE_H__

#define CHECK_SUPPORTED() \
do { \
		{ \
			if (__is_mtp_supported() == false) { \
				return MTP_ERROR_NOT_SUPPORTED; \
		} \
	} \
} while (0);

#define CHECK_ACTIVATED() \
do { \
		{ \
			if (__is_mtp_activated() == false) { \
				return MTP_ERROR_NOT_ACTIVATED; \
		} \
	} \
} while (0);

typedef enum {
	MTP_PROPERTY_ASSOCIATION_DESC = 1,
	MTP_PROPERTY_ASSOCIATION_TYPE,
	MTP_PROPERTY_SIZE,
	MTP_PROPERTY_DATE_CREATED,
	MTP_PROPERTY_DATE_MODIFIED,
	MTP_PROPERTY_FILE_TYPE,
	MTP_PROPERTY_IMAGE_BIT_DEPTH,
	MTP_PROPERTY_IMAGE_PIX_WIDTH,
	MTP_PROPERTY_IMAGE_PIX_HEIGHT,
	MTP_PROPERTY_PARENT_OBJECT_HANDLE,
	MTP_PROPERTY_STORAGE,
	MTP_PROPERTY_THUMBNAIL_SIZE,
	MTP_PROPERTY_THUMBNAIL_FILE_TYPE,
	MTP_PROPERTY_THUMBNAIL_WIDTH,
	MTP_PROPERTY_THUMBNAIL_HEIGHT,
	MTP_PROPERTY_FILENAME,
	MTP_PROPERTY_KEYWORDS
} mtp_property_e;

#endif /* __TIZEN_CAPI_NETWORK_MTP_PRIVATE_H__ */