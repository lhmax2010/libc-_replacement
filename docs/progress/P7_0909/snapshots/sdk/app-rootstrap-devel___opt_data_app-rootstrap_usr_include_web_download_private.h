/*
 * Copyright (c) 2011 Samsung Electronics Co., Ltd All Rights Reserved
 *
 * Licensed under the Apache License, Version 2.0 (the License);
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 * http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an AS IS BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#ifndef __TIZEN_WEB_DOWNLOAD_PRIVATE_H__
#define __TIZEN_WEB_DOWNLOAD_PRIVATE_H__

#include "download.h"

#ifdef __cplusplus
extern "C"
{
#endif

#define DEBUG_MSG
#ifdef DEBUG_MSG
#include <dlog.h>
#ifdef LOG_TAG
#undef LOG_TAG
#endif
#define LOG_TAG "TIZEN_N_URL_DOWNLOAD"
#define TRACE_ERROR(format, ARG...)  \
	{ \
		LOGE(format, ##ARG); \
	}
#define TRACE_STRERROR(format, ARG...)  \
	{ \
		LOGE(format" [%s]", ##ARG, strerror(errno)); \
	}
#define TRACE_INFO(format, ARG...)  \
	{ \
		LOGI(format, ##ARG); \
	}
#else
#define TRACE_DEBUG_MSG(format, ARG...) ;
#endif

#define DOWNLOAD_FEATURE    "tizen.org/feature/download"
#define TELEPHONY_FEATURE   "tizen.org/feature/network.telephony"
#define WIFI_FEATURE        "tizen.org/feature/network.wifi"
#define WIFI_DIRECT_FEATURE "tizen.org/feature/network.wifi.direct"

typedef enum {
	DOWNLOAD_SUPPORTED_FEATURE_DOWNLOAD = 0,
	DOWNLOAD_SUPPORTED_FEATURE_TELEPHONY,
	DOWNLOAD_SUPPORTED_FEATURE_WIFI,
	DOWNLOAD_SUPPORTED_FEATURE_WIFI_DIRECT,
	DOWNLOAD_SUPPORTED_FEATURE_MAX
} download_supported_feature_e;

#define CHECK_FEATURE_SUPPORTED(...) \
	do { \
		int rv = _download_check_feature_supported(__VA_ARGS__, NULL); \
		if (rv != DOWNLOAD_ERROR_NONE) \
			return rv; \
	} while (0)

int _download_check_feature_supported(const char *feature, ...);


#ifdef __cplusplus
}
#endif

#endif /* __TIZEN_WEB_DOWNLOAD_PRIVATE_H__ */
