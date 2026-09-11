/*
 * Copyright (c) 2019 Samsung Electronics Co., Ltd All Rights Reserved
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

#ifndef __TIZEN_APPFW_COMPONENT_CONTEXT_H__
#define __TIZEN_APPFW_COMPONENT_CONTEXT_H__

#include <stdbool.h>
#include <sys/types.h>
#include <tizen.h>

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @file component_context.h
 */

/**
 * @addtogroup CAPI_COMPONENT_CONTEXT_MODULE
 * @{
 */


/**
 * @brief Component context handle.
 * @since_tizen 5.5
 */
typedef void *component_context_h;

/**
 * @brief Enumeration for the component state.
 * @since_tizen 5.5
 */
typedef enum {
	COMPONENT_STATE_INITIALIZED,    /**< The initialized state. The state that the component is constructed but the callback for creating a component is not called yet. */
	COMPONENT_STATE_CREATED,        /**< The created state. The state that the creating callback is called.  */
	COMPONENT_STATE_STARTED,        /**< The started state. The state that the starting callback is called. */
	COMPONENT_STATE_RESUMED,        /**< The resumed state. The state that the resuming callback is called. */
	COMPONENT_STATE_PAUSED,         /**< The paused state. The state that the pausing callback is called. */
	COMPONENT_STATE_DESTROYED,      /**< The destroyed state. The state that right before the destroying callback is called. */
} component_state_e;

/**
 * @brief Destroys the component context handle and releases all its resources.
 * @since_tizen 5.5
 *
 * @param[in]   handle          The component context handle
 * @return      @c 0 on success,
 *              otherwise a negative error value
 * @retval #COMPONENT_MANAGER_ERROR_NONE Successful
 * @retval #COMPONENT_MANAGER_ERROR_INVALID_PARAMETER Invalid parameter
 */
int component_context_destroy(component_context_h handle);

/**
 * @brief Gets the application ID of the component.
 * @since_tizen 5.5
 * @remarks You MUST release @c app_id using free().
 *
 * @param[in]   handle          The component context handle
 * @param[out]  app_id          The application ID
 * @return      @c 0 on success,
 *              otherwise a negative error value
 * @retval #COMPONENT_MANAGER_ERROR_NONE Successful
 * @retval #COMPONENT_MANAGER_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #COMPONENT_MANAGER_ERROR_OUT_OF_MEMORY Out of memory
 */
int component_context_get_app_id(component_context_h handle, char **app_id);

/**
 * @brief Gets the ID of the component.
 * @since_tizen 5.5
 * @remarks You MUST release @c comp_id using free().
 *
 * @param[in]   handle          The component context handle
 * @param[out]  component_id    The component ID
 * @return      @c 0 on success,
 *              otherwise a negative error value
 * @retval #COMPONENT_MANAGER_ERROR_NONE Successful
 * @retval #COMPONENT_MANAGER_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #COMPONENT_MANAGER_ERROR_OUT_OF_MEMORY Out of memory
 */
int component_context_get_component_id(component_context_h handle, char **component_id);

/**
 * @brief Gets the instance ID of the component.
 * @since_tizen 5.5
 * @remarks You MUST release @c instance_id using free().
 *
 * @param[in]   handle          The component context handle
 * @param[out]  instance_id     The instance ID
 * @return      @c 0 on success,
 *              otherwise a negative error value
 * @retval #COMPONENT_MANAGER_ERROR_NONE Successful
 * @retval #COMPONENT_MANAGER_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #COMPONENT_MANAGER_ERROR_OUT_OF_MEMORY Out of memory
 */
int component_context_get_instance_id(component_context_h handle, char **instance_id);

/**
 * @brief Gets the state of the component.
 * @since_tizen 5.5
 *
 * @param[in]   handle          The component context handle
 * @param[out]  state           The component state
 * @return      @c 0 on success,
 *              otherwise a negative error value
 * @retval #COMPONENT_MANAGER_ERROR_NONE Successful
 * @retval #COMPONENT_MANAGER_ERROR_INVALID_PARAMETER Invalid parameter
 */
int component_context_get_component_state(component_context_h handle, component_state_e *state);

/**
 * @brief Checks whether the component is terminated or not.
 * @since_tizen 5.5
 *
 * @param[in]   handle          The component context handle
 * @param[out]  terminated      @c true if the component is terminated, \n
 *                              otherwise @c false if the component is running
 * @return      @c 0 on success,
 *              otherwise a negative error value
 * @retval #COMPONENT_MANAGER_ERROR_NONE Successful
 * @retval #COMPONENT_MANAGER_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #COMPONENT_MANAGER_ERROR_OUT_OF_MEMORY Out of memory
 * @retval #COMPONENT_MANAGER_ERROR_IO_ERROR I/O error
 */
int component_context_is_terminated(component_context_h handle, bool *terminated);

/**
 * @brief Checks whether the component is running as sub component of the group.
 * @since_tizen 5.5
 *
 * @param[in]   handle          The component context handle
 * @param[out]  is_subcomponent @c true if the sub component of the group, \n
 *                              otherwise @c false if the main component of the group
 * @return      @c 0 on success,
 *              otherwise a negative error value
 * @retval #COMPONENT_MANAGER_ERROR_NONE Successful
 * @retval #COMPONENT_MANAGER_ERROR_INVALID_PARAMETER Invalid parameter
 */
int component_context_is_subcomponent(component_context_h handle, bool *is_subcomponent);

/**
 * @brief Clones the component context handle.
 * @since_tizen 5.5
 * @remarks The @a clone should be released using component_context_destroy().
 *
 * @param[out]  clone           A newly created component context handle, if cloning is successful
 * @param[in]   handle          The component context handle
 * @return      @c 0 on success,
 *              otherwise a negative error value
 * @retval #COMPONENT_MANAGER_ERROR_NONE Successful
 * @retval #COMPONENT_MANAGER_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #COMPONENT_MANAGER_ERROR_OUT_OF_MEMORY Out of memory
 */
int component_context_clone(component_context_h *clone, component_context_h handle);

/**
 * @}
 */

#ifdef __cplusplus
}
#endif

#endif /* __TIZEN_APPFW_COMPONENT_CONTEXT_H__ */
