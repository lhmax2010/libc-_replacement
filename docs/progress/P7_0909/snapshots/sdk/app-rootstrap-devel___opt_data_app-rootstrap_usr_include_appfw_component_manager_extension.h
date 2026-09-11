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

#ifndef __TIZEN_APPFW_COMPONENT_MANAGER_EXTENSION_H__
#define __TIZEN_APPFW_COMPONENT_MANAGER_EXTENSION_H__

#include <tizen.h>

#include <component_manager.h>

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @file component_manager_extension.h
 */

/**
 * @addtogroup CAPI_COMPONENT_MANAGER_MODULE
 * @{
 */

/**
 * @brief Pauses the component.
 * @since_tizen 5.5
 * @privlevel public
 * @privilege %http://tizen.org/privilege/appmanager.launch
 *
 * @param[in]   handle          The component context handle
 * @return      @c 0 on success,
 *              otherwise a negative error value
 * @retval #COMPONENT_MANAGER_ERROR_NONE Successful
 * @retval #COMPONENT_MANAGER_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #COMPONENT_MANAGER_ERROR_IO_ERROR I/O error
 * @retval #COMPONENT_MANAGER_ERROR_NO_SUCH_COMPONENT No such component
 * @retval #COMPONENT_MANAGER_ERROR_PERMISSION_DENIED Permission denied
 */
int component_manager_pause_component(component_context_h handle);

/**
 * @brief Terminates the running component.
 * @since_tizen 5.5
 * @privlevel public
 * @privilege %http://tizen.org/privilege/appmanager.kill
 *
 * @param[in]   handle          The component context handle
 * @return      @c 0 on success,
 *              otherwise a negative error value
 * @retval #COMPONENT_MANAGER_ERROR_NONE Successful
 * @retval #COMPONENT_MANAGER_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #COMPONENT_MANAGER_ERROR_IO_ERROR I/O error
 * @retval #COMPONENT_MANAGER_ERROR_NO_SUCH_COMPONENT No such component
 * @retval #COMPONENT_MANAGER_ERROR_PERMISSION_DENIED Permission denied
 */
int component_manager_terminate_component(component_context_h handle);

/**
 * @}
 */

#ifdef __cplusplus
}
#endif

#endif /* __TIZEN_APPFW_COMPONENT_MANAGER_EXTENSION_H__ */
