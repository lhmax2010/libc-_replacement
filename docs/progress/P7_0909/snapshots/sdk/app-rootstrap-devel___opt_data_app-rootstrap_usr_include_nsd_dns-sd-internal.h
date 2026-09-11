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

#ifndef __TIZEN_NET_DNSSD_INTERNAL_H__
#define __TIZEN_NET_DNSSD_INTERNAL_H__

#include <tizen.h>
#include "dns-sd.h"

#ifdef __cplusplus
extern "C" {
#endif

#ifndef TIZEN_ERROR_DNSSD
#define TIZEN_ERROR_DNSSD -0x01CA0000
#endif

/**
 * @brief Starts browsing the DNSSD remote service on a specific network interface.
 * @details found_cb would be called only if there are any services available of
 *          service_type provided in the argument. Application will keep
 *          browsing for available/unavailable services until it calls
 *          dnssd_stop_browsing_service().
 * @since_tizen 4.0
 * @privlevel public
 * @privilege %http://tizen.org/privilege/internet
 * @param[in] service_type The DNSSD service type to browse. It is expressed
 *                         as type followed by protocol, separated by a dot(e.g. "_ftp._tcp").
 *                         It must begin with an underscore, followed by 1-15 characters
 *                         which may be letters, digits, or hyphens. The transport protocol
 *                         must be "_tcp" or "_udp". New service types should be registered
 *                         at http://www.dns-sd.org/ServiceTypes.html
 * @param[in] interface The interface name
 * @param[out] dnssd_service The DNSSD browse service handle
 * @param[in] found_cb The callback function to be called
 * @param[in] user_data The user data passed to the callback function
 * @return @c 0 on success,
 *         otherwise negative error value
 * @retval #DNSSD_ERROR_NONE Successful
 * @retval #DNSSD_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #DNSSD_ERROR_NOT_SUPPORTED Not Supported
 * @retval #DNSSD_ERROR_SERVICE_NOT_RUNNING Service Not Running
 * @retval #DNSSD_ERROR_NOT_INITIALIZED Not Initialized
 * @retval #DNSSD_ERROR_PERMISSION_DENIED Permission Denied
 * @pre This API needs dnssd_initialize() before use.
 */
int dnssd_start_browsing_service_on_interface(const char *service_type, const char *interface,
		dnssd_browser_h *dnssd_service, dnssd_found_cb found_cb,
		void *user_data);

#ifdef __cplusplus
}
#endif

#endif /* __TIZEN_NET_DNSSD_INTERNAL_H__ */
