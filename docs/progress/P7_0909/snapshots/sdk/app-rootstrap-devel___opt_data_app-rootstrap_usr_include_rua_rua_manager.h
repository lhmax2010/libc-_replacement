/*
 * Copyright (c) 2019 Samsung Electronics Co., Ltd. All rights reserved.
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

#ifndef __RUA_MANAGER_H__
#define __RUA_MANAGER_H__

#include <app_control.h>
#include <rua_context.h>
#include <rua_info.h>
#include <rua_types.h>

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @brief Called to get the RUA information once for each launched application.
 * @since_tizen 5.5
 * @remarks After this callback returns, the handle of the RUA information handle is freed.
 *          Therefore, if you want to use the handle after returning this callback, you MUST copy it by using rua_info_clone() function.
 *
 * @param[in]   info            The RUA information handle
 * @param[in]   user_data       The user data passed from the foreach function
 * @return      @c true to continue with the next iteration of the loop, \n
 *              otherwise @c false to break out of the loop
 *
 * @see rua_manager_foreach_rua_info()
 * @see rua_manager_foreach_all_rua_info()
 */
typedef bool (*rua_manager_rua_info_cb)(rua_info_h info, void *user_data);

/**
 * @brief Retrieves all launched applications information.
 * @since_tizen 5.5
 * @privlevel public
 * @privilege %http://tizen.org/privilege/packagemanager.info
 *
 * @param[in]   callback        The callback function to invoke
 * @param[in]   user_data       The user data to be passed to the callback function
 * @return      @c 0 on success,
 *              otherwise a negative error value
 *
 * @see rua_info_foreach_cb()
 */
int rua_manager_foreach_rua_info(rua_manager_rua_info_cb callback, void *user_data);

/**
 * @brief Deletes the RUA information.
 * @since_tizen 5.5
 *
 * @param[in]   info            The RUA information handle
 * @return      @c 0 on success,
 *              otherwise a negative error value
 *
 * @remarks This function is only available for platform level signed applications.
 */
int rua_manager_delete_rua_info(rua_info_h info);

/**
 * @brief Deletes all RUA informations.
 * @since_tizen 5.5
 *
 * @return      @c 0 on success,
 *              otherwise a negative error value
 *
 * @remarks This function is only available for platform level signed applications.
 */
int rua_manager_delete_all_rua_info(void);

/**
 * @brief Gets the app_control handle from the given RUA information handle.
 * @since_tizen 5.5
 * @remarks You should release @a app_control using app_control_destroy().
 *
 * @param[in]   info            The RUA information handle
 * @param[out]  app_control     The app_control handle
 * @return      @c 0 on success,
 *              otherwise a negative error value
 *
 * @see rua_info_foreach()
 * @see rua_info_usr_foreach()
 * @see app_control_send_launch_request()
 * @see app_control_send_launch_request_async()
 * @see app_control_destroy()
 */
int rua_manager_get_app_control_from_rua_info(rua_info_h info, app_control_h *app_control);

/**
 * @brief Gets the RUA context handle from the given RUA information handle.
 * @since_tizen 5.5
 * @remarks You should release @a context using rua_context_destroy().
 *
 * @param[in]   info            The RUA information handle
 * @param[out]  context         The RUA context handle
 * @return      @c 0 on success,
 *              otherwise a negative error value
 *
 * @see rua_info_foreach()
 * @see rua_info_usr_foreach()
 * @see rua_context_destroy()
 */
int rua_manager_get_rua_context_from_rua_info(rua_info_h info, rua_context_h *context);

/**
 * @brief Checks whether the instance with the given RUA context handle is running or not.
 * @since_tizen 5.5
 *
 * @param[in]   context         The RUA context handle
 * @param[out]  running         @c true if the instance is running, \n
 *                              otherwise @c false if not running
 * @return      @c 0 on success,
 *              otherwise a negative error value
 */
int rua_manager_is_running(rua_context_h context, bool *running);

/**
 * @brief Resumes the instance.
 * @since_tizen 5.5
 * @privlevel public
 * @privilege %http://tizen.org/privilege/appmanager.launch
 *
 * @param[in]   context         The RUA context handle
 * @return      @c 0 on success,
 *              otherwise a negative error value
 */
int rua_manager_resume(rua_context_h context);

/**
 * @brief Terminates the instance.
 * @since_tizen 5.5
 * @privlevel platform
 * @privilege %http://tizen.org/privilege/appmanager.kill
 *
 * @param[in]   context         The RUA context handle
 * @return      @c 0 on succes,
 *              otherwise a negative error value
 */
int rua_manager_terminate(rua_context_h context);

#ifdef __cplusplus
}
#endif

#endif /*__RUA_MANAGER_H__*/
