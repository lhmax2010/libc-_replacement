/*
 * Copyright (c) 2000 - 2017 Samsung Electronics Co., Ltd. All rights reserved.
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


#ifndef __SYSPOPUP_CALLER_H__
#define __SYSPOPUP_CALLER_H__

#include <unistd.h>
#include <bundle.h>

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @file        syspopup_caller.h
 * @brief       Library to launch/destroy System Popup Application
 *
 * Patched by   Knhoon Baik <knhoon.baik@samsung.com>
 * Patched by   Noha Park <noha.park@samsung.com>
 */

/**
 * @defgroup syspopup_caller    Helper Library to launch/destroy system popup application
 * @ingroup syspopup
 * @brief                       Helper Library to launch/destroy system popup application
 */

/**
 * @addtogroup syspopup_caller
 * @{
 */

/**
 * @brief Called to get the system popup information.
 * @since_tizen 4.0
 * @param[in] popup_name   The name of the system popup
 * @param[in] appid        The application ID of the system popup
 * @param[in] user_data    The user data passed from the foreach function
 * @return @c 0 to continue with the next iteration of the loop,
 *         otherwise a negative error value to break out of the loop
 * @pre syspopup_foreach_info() will invoke this callback.
 * @see syspopup_foreach_info()
 */
typedef int (*syspopup_info_cb)(const char *popup_name, const char *appid, void *user_data);

/**
 * @brief       This API launch the system popup application with given popup name.
 *
 *              This API launch the system popup application.
 *              This API find system popup application package name with given popup name from system popup infomation DB.\n
 *              And then, launch found application package
 *
 * @privilege   %http://tizen.org/privilege/appmanager.launch
 * @param[in]   popup_name      system popup application name to launch (not package name)
 * @param[in]   b               extra bundle(arguement) to toss the popup application
 * @param[in]   uid             User ID
 * @return      0 if success, negative value(<0) if fail
 * @retval      0               - success
 * @retval      -1              - generic error
 *
 * @sample code
 * @code
 *
 * ...
 *	bundle *b = NULL;
 *
 *	b = bundle_create();
 *	bundle_add(b, "_SYSPOPUP_TITLE_", "System Popup Title");
 *	bundle_add(b, "_SYSPOPUP_CONTENT_", "System Popup Content");
 *
 *	ret = syspopup_launch_for_uid("syspopup-app", b, 5001);
 *
 *	bundle_free(b);
 * ...
 *
 * @endcode
 * @remark
 *      This API is only available to System users.
 */
int syspopup_launch_for_uid(char *popup_name, bundle *b, uid_t uid);

/**
 * @brief       This API launch the system popup application with given popup name.
 *
 *              This API launch the system popup application.
 *              This API find system popup application package name with given popup name from system popup infomation DB.\n
 *              And then, launch found application package
 *
 * @privilege   %http://tizen.org/privilege/appmanager.launch
 * @param[in]   popup_name      system popup application name to launch (not package name)
 * @param[in]   b               extra bundle(arguement) to toss the popup application
 * @return      0 if success, negative value(<0) if fail
 * @retval      0               - success
 * @retval      -1              - generic error
 *
 * @sample code
 * @code
 *
 * ...
 *	bundle *b = NULL;
 *
 *	b = bundle_create();
 *	bundle_add(b, "_SYSPOPUP_TITLE_", "System Popup Title");
 *	bundle_add(b, "_SYSPOPUP_CONTENT_", "System Popup Content");
 *
 *	ret = syspopup_launch("syspopup-app", b);
 *
 *	bundle_free(b);
 * ...
 *
 * @endcode
 */
int syspopup_launch(char *popup_name, bundle *b);

/**
 * @brief       This API generate destroy event to all system popup application.
 *		This API can call with proper permission (root or inhouse)
 *
 * @return      0 if success, negative value(<0) if fail
 * @retval      0               - success
 * @retval      -1              - generic error
 */
int syspopup_destroy_all(void);

/**
 * @brief Retrieves the system popup information.
 * @since_tizen 4.0
 * @param[in] callback     The iteration callback function
 * @param[in] user_data    The user data to be passed to the callback function
 * @return @c 0 on success,
 *         otherwise a negative error value
 */
int syspopup_foreach_info(syspopup_info_cb callback, void *user_data);

/** @} */

#ifdef __cplusplus
}
#endif

#endif
