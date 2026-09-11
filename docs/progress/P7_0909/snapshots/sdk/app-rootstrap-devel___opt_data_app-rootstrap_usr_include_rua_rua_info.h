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

#ifndef __RUA_INFO_H__
#define __RUA_INFO_H__

#include <time.h>
#include <sys/types.h>

#include <rua_types.h>

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @brief The RUA information handle.
 * @since_tizen 5.5
 */
typedef struct rua_info_s *rua_info_h;

/**
 * @brief Gets the ID of the application.
 * @since_tizen 5.5
 * @remarks You should release @a app_id using free().
 *
 * @param[in]   info            The RUA information handle
 * @param[out]  app_id          The application ID
 * @return      @c 0 on success,
 *              otherwise a negative error value
 *
 * @see rua_info_foreach()
 * @see rua_info_usr_foreach()
 */
int rua_info_get_app_id(rua_info_h info, char **app_id);

/**
 * @brief Gets the path of the application.
 * @since_tizen 5.5
 * @remarks You should release @a app_path using free().
 *
 * @param[in]   info            The RUA information handle
 * @param[out]  app_path        The application path
 * @return      @c 0 on success,
 *              otherwise a negative error value
 *
 * @see rua_info_foreach()
 * @see rua_info_usr_foreach()
 */
int rua_info_get_app_path(rua_info_h info, char **app_path);

/**
 * @brief Gets the arguments of the application.
 * @since_tizen 5.5
 * @remarks You should release @a args using free().
 *
 * @param[in]   info            The RUA information handle
 * @param[out]  args            The arguments
 * @return      @c 0 on success,
 *              otherwise a negative error value
 *
 * @see rua_info_foreach()
 * @see rua_info_usr_foreach()
 */
int rua_info_get_args(rua_info_h info, char **args);

/**
 * @brief Gets the launch time of the application.
 * @since_tizen 5.5
 *
 * @param[in]   info            The RUA information handle
 * @param[out]  launch_time     The launch time
 * @return      @c 0 on success,
 *              otherwise a negative error value
 *
 * @see rua_info_foreach()
 * @see rua_info_usr_foreach()
 */
int rua_info_get_launch_time(rua_info_h info, time_t *launch_time);

/**
 * @brief Gets the instance ID of the application.
 * @details The function returns RUA_ERROR_NONE, event if @a instance_id is null.
 * @since_tizen 5.5
 * @remarks You should release @a instance_id using free().
 *
 * @param[in]   info            The RUA information handle
 * @param[out]  instance_id     The instance ID
 * @return      @c 0 on success,
 *              otherwise a negative error value
 *
 * @see rua_info_foreach()
 * @see rua_info_usr_foreach()
 */
int rua_info_get_instance_id(rua_info_h info, char **instance_id);

/**
 * @brief Gets the instance name of the application.
 * @details The function returns RUA_ERROR_NONE, event if @a instance_name is null.
 * @since_tizen 5.5
 * @remarks You should release @a instance_name using free().
 *
 * @param[in]   info            The RUA information handle
 * @param[out]  instance_name   The instance name
 * @return      @c 0 on success,
 *              otherwise a negative error value
 *
 * @see rua_info_foreach()
 * @see rua_info_usr_foreach()
 */
int rua_info_get_instance_name(rua_info_h info, char **instance_name);

/**
 * @brief Gets the icon path of the application.
 * @details The function returns RUA_ERROR_NONE, event if @a icon is null.
 * @since_tizen 5.5
 * @privlevel public
 * @privilege %http://tizen.org/privilege/packagemanager.info
 * @remarks You should release @a icon using free().
 *
 * @param[in]   info            The RUA information handle
 * @param[out]  icon            The icon path
 * @return      @c 0 on success,
 *              otherwise a negative error value
 *
 * @see rua_info_foreach()
 * @see rua_info_usr_foreach()
 */
int rua_info_get_icon(rua_info_h info, char **icon);

/**
 * @brief Gets the URI of the application.
 * @details The function returns RUA_ERROR_NONE, event if @a uri is null.
 * @since_tizen 5.5
 * @remarks You should release @a uri using free().
 *
 * @param[in]   info            The RUA information handle
 * @param[out]  uri             The URI
 * @return      @c 0 on success,
 *              otherwise a negative error value
 *
 * @see rua_info_foreach()
 * @see rua_info_usr_foreach()
 */
int rua_info_get_uri(rua_info_h info, char **uri);

/**
 * @brief Gets the image path of the application.
 * @details The function returns RUA_ERROR_NONE, event if @a image is null.
 * @since_tizen 5.5
 * @remarks You should release @a image using free().
 *
 * @param[in]   info            The RUA information handle
 * @param[out]  image           The image path
 * @return      @c 0 on success,
 *              otherwise a negative error value
 *
 * @see rua_info_foreach()
 * @see rua_info_usr_foreach()
 */
int rua_info_get_image(rua_info_h info, char **image);

/**
 * @brief Gets the component ID of the application.
 * @details The function returns RUA_ERROR_NONE, event if @a component_id is null.
 * @since_tizen 5.5
 * @remarks You should release @a component_id using free().
 *
 * @param[in]   info            The RUA information handle
 * @param[out]  component_id    The component ID
 * @return      @c 0 on success,
 *              otherwise a negative error value
 *
 * @see rua_info_foreach()
 * @see rua_info_usr_foreach()
 */
int rua_info_get_component_id(rua_info_h info, char **component_id);

/**
 * @brief Checks whether the application should be managed by task-manager or not.
 * @since_tizen 5.5
 *
 * @param[in]   info            The RUA information handle
 * @param[out]  managed         @c true if the application should be managed by task-manager, \n
 *                              otherwise @c false
 * @return      @c 0 on success,
 *              otherwise a negative error value
 *
 * @see rua_info_foreach()
 * @see rua_info_usr_forach()
 */
int rua_info_is_managed_by_task_manager(rua_info_h info, bool *managed);

/**
 * @brief Gets the label.
 * @details The function returns RUA_ERROR_NONE, event if @a label is null.
 * @since_tizen 5.5
 * @remarks You should release @a label using free().
 *
 * @param[in]   info            The RUA information handle
 * @param[out]  label           The label
 * @return      @c 0 on success,
 *              otherwise a negative error value
 *
 * @see rua_info_foreach()
 * @see rua_info_usr_foreach()
 */
int rua_info_get_label(rua_info_h info, char **label);

/**
 * @brief Destroyes the RUA information handle.
 * @since_tizen 5.5
 *
 * @param[in]   info            The RUA information handle
 * @return      @c 0 on success,
 *              otherwise a negative error value
 */
int rua_info_destroy(rua_info_h info);

/**
 * @brief Creates and returns a copy of the given RUA information handle.
 * @since_tizen 5.5
 * @remarks You should release @a clone using rua_info_destroy().
 *
 * @param[in]   info            The RUA information handle
 * @param[out]  clone           If successful, a newly created RUA information handle will be returned
 * @return      @c 0 on success,
 *              otherwise a negative error value
 *
 * @see rua_info_destroy()
 */
int rua_info_clone(rua_info_h info, rua_info_h *clone);

#ifdef __cplusplus
}
#endif

#endif /*__RUA_INFO_H__*/
