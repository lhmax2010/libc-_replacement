/*
 * Copyright (c) 2019 - 2021 Samsung Electronics Co., Ltd All Rights Reserved
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

#ifndef __TIZEN_APPFW_COMPONENT_INFO_H__
#define __TIZEN_APPFW_COMPONENT_INFO_H__

#include <tizen.h>

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @file component_info.h
 */

/**
* @addtogroup CAPI_COMPONENT_INFO_MODULE
* @{
*/

/**
 * @brief The component handle having own properties such name, path, and icons.
 * @since_tizen 5.5
 */
typedef void *component_info_h;

/**
 * @brief Enumeration for component type.
 * @since_tizen 5.5
 * @see component_info_get_component_type()
 */
typedef enum {
	COMPONENT_INFO_COMPONENT_TYPE_FRAME,    /**< Frame component */
	COMPONENT_INFO_COMPONENT_TYPE_SERVICE,  /**< Service component */
	COMPONENT_INFO_COMPONENT_TYPE_WIDGET,   /**< Widget component (Since 6.5) */
} component_info_component_type_e;

/**
 * @brief Creates a component handle.
 * @since_tizen 5.5
 * @privlevel public
 * @privilege %http://tizen.org/privilege/packagemanager.info
 * @remarks The @a handle should be released using component_info_destroy().
 *
 * @param[in]   component_id    The component ID
 * @param[out]  handle          The component information handle
 * @return      @c 0 on success,
 *              otherwise a negative error value
 * @retval #COMPONENT_MANAGER_ERROR_NONE Successful
 * @retval #COMPONENT_MANAGER_ERROR_PERMISSION_DENIED Permission denied
 * @retval #COMPONENT_MANAGER_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #COMPONENT_MANAGER_ERROR_NO_SUCH_COMPONENT No such component
 * @retval #COMPONENT_MANAGER_ERROR_OUT_OF_MEMORY Out of memory
 */
int component_info_create(const char *component_id, component_info_h *handle);

/**
 * @brief Destroys the component handle.
 * @since_tizen 5.5
 *
 * @param[in]   handle          The component information handle
 * @return      @c 0 on success,
 *              otherwise a negative error value
 * @retval #COMPONENT_MANAGER_ERROR_NONE Successful
 * @retval #COMPONENT_MANAGER_ERROR_INVALID_PARAMETER Invalid parameter
 */
int component_info_destroy(component_info_h handle);

/**
 * @brief Gets the application ID of the component.
 * @since_tizen 5.5
 * @remarks You must release @a app_id using free().
 *
 * @param[in]   handle          The component information handle
 * @param[out]  app_id          The application ID
 * @return      @c 0 on success,
 *              otherwise a negative error value
 * @retval #COMPONENT_MANAGER_ERROR_NONE Successful
 * @retval #COMPONENT_MANAGER_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #COMPONENT_MANAGER_ERROR_IO_ERROR I/O error
 * @retval #COMPONENT_MANAGER_ERROR_OUT_OF_MEMORY Out of memory
 */
int component_info_get_app_id(component_info_h handle, char **app_id);

/**
 * @brief Gets the component ID.
 * @since_tizen 5.5
 * @remarks You must release @a component_id using free().
 *
 * @param[in]   handle          The component information handle
 * @param[out]  component_id    The component ID
 * @return      @c 0 on success,
 *              otherwise a negative error value
 * @retval #COMPONENT_MANAGER_ERROR_NONE Successful
 * @retval #COMPONENT_MANAGER_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #COMPONENT_MANAGER_ERROR_IO_ERROR I/O error
 * @retval #COMPONENT_MANAGER_ERROR_OUT_OF_MEMORY Out of memory
 */
int component_info_get_component_id(component_info_h handle, char **component_id);

/**
 * @brief Gets the type of component.
 * @since_tizen 5.5
 *
 * @param[in]   handle          The component information handle
 * @param[out]  type            The component type
 * @return      @c 0 on success,
 *              otherwise a negative error value
 * @retval #COMPONENT_MANAGER_ERROR_NONE Successful
 * @retval #COMPONENT_MANAGER_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #COMPONENT_MANAGER_ERROR_IO_ERROR I/O error
 * @retval #COMPONENT_MANAGER_ERROR_OUT_OF_MEMORY Out of memory
 * @see #component_info_component_type_e
 */
int component_info_get_component_type(component_info_h handle, component_info_component_type_e *type);

/**
 * @brief Gets whether the icon of the component is displayed on the home screen or not.
 * @since_tizen 5.5
 *
 * @param[in]   handle          The component information handle
 * @param[out]  icon_display    @c true if the icon should be displayed, \n
 *                              otherwise @c false
 * @return      @c 0 on success,
 *              otherwise a negative error value
 * @retval #COMPONENT_MANAGER_ERROR_NONE Successful
 * @retval #COMPONENT_MANAGER_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #COMPONENT_MANAGER_ERROR_IO_ERROR I/O error
 */
int component_info_is_icon_display(component_info_h handle, bool *icon_display);

/**
 * @brief Checks whether the component should be managed by task-manager or not.
 * @since_tizen 5.5
 *
 * @param[in]   handle          The component information handle
 * @param[out]  managed         @c if the component should be managed by task-manager, \n
 *                              otherwise @c false
 * @return      @c 0 on success,
 *              otherwise a negative error value
 * @retval #COMPONENT_MANAGER_ERROR_NONE Successful
 * @retval #COMPONENT_MANAGER_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #COMPONENT_MANAGER_ERROR_IO_ERROR I/O error
 */
int component_info_is_managed_by_task_manager(component_info_h handle, bool *managed);

/**
 * @brief Gets the icon path of the component.
 * @since_tizen 5.5
 * @remarks You must release @a path using free().
 *
 * @param[in]   handle          The component information handle
 * @param[out]  path            The icon path of the component
 * @return      @c 0 on success,
 *              otherwise a negative error value
 * @retval #COMPONENT_MANAGER_ERROR_NONE Successful
 * @retval #COMPONENT_MANAGER_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #COMPONENT_MANAGER_ERROR_IO_ERROR I/O error
 * @retval #COMPONENT_MANAGER_ERROR_OUT_OF_MEMORY Out of memory
 */
int component_info_get_icon(component_info_h handle, char **path);

/**
 * @brief Gets the label of the component.
 * @since_tizen 5.5
 * @remarks You must release @a label using free().
 *
 * @param[in]   handle          The component information handle
 * @param[out]  label           The label of the component
 * @return      @c 0 on success,
 *              otherwise a negative error value
 * @retval #COMPONENT_MANAGER_ERROR_NONE Successful
 * @retval #COMPONENT_MANAGER_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #COMPONENT_MANAGER_ERROR_IO_ERROR I/O error
 * @retval #COMPONENT_MANAGER_ERROR_OUT_OF_MEMORY Out of memory
 */
int component_info_get_label(component_info_h handle, char **label);

/**
 * @brief Gets the localized label of the component.
 * @since_tizen 5.5
 * @remarks You must release @a label using free().
 *
 * @param[in]   handle          The component information handle
 * @param[in]   locale          The locale information
 * @param[out]  label           The localized label of the component
 * @return      @c 0 on success,
 *              otherwise a negative error value
 * @retval #COMPONENT_MANAGER_ERROR_NONE Successful
 * @retval #COMPONENT_MANAGER_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #COMPONENT_MANAGER_ERROR_IO_ERROR I/O error
 * @retval #COMPONENT_MANAGER_ERROR_LABEL_NOT_FOUND The localized label does not exist
 * @retval #COMPONENT_MANAGER_ERROR_OUT_OF_MEMORY Out of memory
 */
int component_info_get_localized_label(component_info_h handle, const char *locale, char **label);

/**
 * @brief Clones the component information handle.
 * @since_tizen 5.5
 * @remarks You must release @a clone using component_info_destroy().
 *
 * @param[out]  clone           A newly created component information handle, if successfully cloned
 * @param[in]   handle          The component information
 * @return      @c 0 on success,
 *              otherwise a negative error value
 * @retval #COMPONENT_MANAGER_ERROR_NONE Successful
 * @retval #COMPONENT_MANAGER_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #COMPONENT_MANAGER_ERROR_OUT_OF_MEMORY Out of memory
 */
int component_info_clone(component_info_h *clone, component_info_h handle);

/**
* @}
*/

#ifdef __cplusplus
}
#endif

#endif /* __TIZEN_APPFW_COMPONENT_INFO_H__ */
