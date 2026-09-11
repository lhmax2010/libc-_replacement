/*
 * Copyright (c) 2021 Samsung Electronics Co., Ltd. All rights reserved.
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

#ifndef __TIZEN_CAPI_NETWORK_USER_AWARENESS_INTERNAL_TYPE_H__
#define __TIZEN_CAPI_NETWORK_USER_AWARENESS_INTERNAL_TYPE_H__

#include <tizen.h>

#include <user-awareness-type.h>

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @file user-awareness-internal-type.h
 */

/**
 * @ingroup CAPI_NETWORK_UA_MODULE
 * @brief Enumerations of logical conjunction operation in between
 *         AND and OR user detection conditions.
 * @since_tizen 6.5
 */
typedef enum {
	UA_OR_OPERATION = 0x00, /**< AND | OR */
	UA_AND_OPERATION = 0x01 /**< AND & OR */
} ua_condition_conjunction_e;

/**
 * @ingroup CAPI_NETWORK_UA_MODULE
 * @brief Callback to get all user handles.
 * @since_tizen 6.5
 *
 * @remarks The @a user_handle should not be released.
 * @remarks The @a user_handle can be used only in the callback.
 *
 * @param[in] user_handle The user handle.
 * @param[in] user_data User data passed in ua_foreach_users().
 *
 * @exception
 * @pre
 * @post
 *
 * @see ua_foreach_users()
 */
typedef bool (*ua_registered_user_cb)(
		ua_user_h user_handle,
		void *user_data);

/**
 * @}
 */

#ifdef __cplusplus
}
#endif
#endif /* __TIZEN_CAPI_NETWORK_USER_AWARENESS_INTERNAL_TYPE_H__ */
