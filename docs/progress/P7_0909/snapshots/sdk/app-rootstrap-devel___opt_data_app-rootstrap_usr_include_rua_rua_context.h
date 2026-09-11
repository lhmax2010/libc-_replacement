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

#ifndef __RUA_CONTEXT_H__
#define __RUA_CONTEXT_H__

#include <rua_types.h>

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @brief The RUA context handle.
 * @since_tizen 5.5
 */
typedef struct rua_context_s *rua_context_h;

/**
 * @brief Destroyes the RUA context handle.
 * @since_tizen 5.5
 *
 * @param[in]   context         The RUA context handle
 * @return      @c 0 on success,
 *              otherwise a negative error value
 */
int rua_context_destroy(rua_context_h context);

/**
 * @brief Gets the application ID of the instance.
 * @since_tizen 5.5
 * @remarks You should release @a app_id using free().
 *
 * @param[in]   context         The RUA context handle
 * @param[out]  app_id          The application ID
 * @return      @c 0 on success,
 *              otherwise a negative error value
 */
int rua_context_get_app_id(rua_context_h context, char **app_id);

/**
 * @brief Gets the ID of the instance.
 * @details The function returns RUA_ERROR_NONE, event if @a instance_id is null.
 * @since_tizen 5.5
 * @remarks You should release @a instance_id using free().
 *
 * @param[in]   context         The RUA context handle
 * @param[out]  instance_id     The instance ID
 * @return      @c 0 on success,
 *              otherwise a negative error value
 */
int rua_context_get_instance_id(rua_context_h context, char **instance_id);

/**
 * @brief Gets the component ID of the instance.
 * @details The function returns RUA_ERROR_NONE, event if @a component_id is null.
 * @since_tizen 5.5
 * @remarks You should release @a component_id using free().
 *
 * @param[in]   context         The RUA context handle
 * @param[out]  component_id    The component ID
 * @return      @c 0 on success,
 *              otherwise a negative error value
 */
int rua_context_get_component_id(rua_context_h context, char **component_id);

#ifdef __cplusplus
}
#endif

#endif /*__RUA_CONTEXT_H__*/
