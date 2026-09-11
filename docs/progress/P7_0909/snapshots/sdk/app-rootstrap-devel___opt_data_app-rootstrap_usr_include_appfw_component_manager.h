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

#ifndef __TIZEN_APPFW_COMPONENT_MANAGER_H__
#define __TIZEN_APPFW_COMPONENT_MANAGER_H__

#include <tizen.h>

#include <component_context.h>
#include <component_info.h>

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @file component_manager.h
 */

/**
 * @addtogroup CAPI_COMPONENT_MANAGER_MODULE
 * @{
 */

#ifndef TIZEN_ERROR_COMPONENT_MANAGER
#define TIZEN_ERROR_COMPONENT_MANAGER -0x03040000
#endif

/**
 * @brief Enumerations for Component Manager Error.
 * @since_tizen 5.5
 */
typedef enum {
	COMPONENT_MANAGER_ERROR_NONE = TIZEN_ERROR_NONE, /**< Successful */
	COMPONENT_MANAGER_ERROR_INVALID_PARAMETER = TIZEN_ERROR_INVALID_PARAMETER, /**< Invalid parameter */
	COMPONENT_MANAGER_ERROR_OUT_OF_MEMORY = TIZEN_ERROR_OUT_OF_MEMORY, /**< Out of memory */
	COMPONENT_MANAGER_ERROR_IO_ERROR = TIZEN_ERROR_IO_ERROR, /**< Internal I/O error */
	COMPONENT_MANAGER_ERROR_NO_SUCH_COMPONENT = TIZEN_ERROR_COMPONENT_MANAGER | 0x01, /**< No such component */
	COMPONENT_MANAGER_ERROR_DB_FAILED = TIZEN_ERROR_COMPONENT_MANAGER | 0x03, /**< Database error  */
	COMPONENT_MANAGER_ERROR_INVALID_APPLICATION = TIZEN_ERROR_COMPONENT_MANAGER | 0x04, /**< Invalid application ID */
	COMPONENT_MANAGER_ERROR_NOT_RUNNING = TIZEN_ERROR_COMPONENT_MANAGER | 0x05, /**< Component is not running */
	COMPONENT_MANAGER_ERROR_LABEL_NOT_FOUND = TIZEN_ERROR_COMPONENT_MANAGER | 0x06, /**< Label not found */
	COMPONENT_MANAGER_ERROR_PERMISSION_DENIED = TIZEN_ERROR_PERMISSION_DENIED, /**< Permission denied */
} component_manager_error_e;

/**
 * @brief Called whenever the application retrieves the context of running components.
 * @since_tizen 5.5
 * @remarks The @a handle should not be released.
 *          The @a handle can be used only in the callback. To use outside, make a copy using component_context_clone().
 *
 * @param[in]   handle          The component context of each running component
 * @param[in]   user_data       The user data passed from the foreach function
 * @return      @c true to continue with the next iteration of the loop, \n
 *              otherwise @c false to break out of the loop
 * @pre component_manager_foreach_component_context() will invoke this callback.
 * @see component_manager_foreach_component_context()
 */
typedef bool (*component_manager_component_context_cb)(component_context_h handle, void *user_data);

/**
 * @brief Called whenever the application retrieves the information of installed components.
 * @since_tizen 5.5
 * @remarks The @a handle should not be released.
 *          The @a handle can be used only in the callback. To use outside, make a copy using component_info_clone().
 *
 * @param[in]   handle          The component information of each installed component
 * @param[in]   user_data       The user data passed from the foreach function
 * @return      @c true to continue with the next iteration of the loop, \n
 *              otherwise @c false to break out of the loop
 * @pre component_manager_foreach_component_info() will invoke this callback.
 * @see component_manager_foreach_component_info()
 */
typedef bool (*component_manager_component_info_cb)(component_info_h handle, void *user_data);

/**
 * @brief Retrieves all component contexts of running components.
 * @since_tizen 5.5
 * @privlevel public
 * @privilege %http://tizen.org/privilege/packagemanager.info
 *
 * @param[in]   callback        The callback function to invoke
 * @param[in]   user_data       The user data to be passed to the callback function
 * @return      @c 0 on success,
 *              otherwise a negative error value
 * @retval #COMPONENT_MANAGER_ERROR_NONE Successful
 * @retval #COMPONENT_MANAGER_ERROR_PERMISSION_DENIED Permission denied
 * @retval #COMPONENT_MANAGER_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #COMPONENT_MANAGER_ERROR_NO_SUCH_COMPONENT No such component
 * @retval #COMPONENT_MANAGER_ERROR_OUT_OF_MEMORY Out of memory
 * @retval #COMPONENT_MANAGER_ERROR_IO_ERROR I/O error
 *
 * @see component_manager_component_context_cb()
 */
int component_manager_foreach_component_context(component_manager_component_context_cb callback, void *user_data);

/**
 * @brief Retrieves all installed components information.
 * @since_tizen 5.5
 * @privlevel public
 * @privilege %http://tizen.org/privilege/packagemanager.info
 *
 * @param[in]   callback        The callback function to invoke
 * @param[in]   user_data       The user data to be passed to the callback function
 * @return      @c 0 on success,
 *              otherwise a negative error value
 * @retval #COMPONENT_MANAGER_ERROR_NONE Successful
 * @retval #COMPONENT_MANAGER_ERROR_PERMISSION_DENIED Permission denied
 * @retval #COMPONENT_MANAGER_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #COMPONENT_MANAGER_ERROR_NO_SUCH_COMPONENT No such component
 * @retval #COMPONENT_MANAGER_ERROR_OUT_OF_MEMORY Out of memory
 * @retval #COMPONENT_MANAGER_ERROR_IO_ERROR I/O error
 *
 * @see component_manager_component_info_cb()
 */
int component_manager_foreach_component_info(component_manager_component_info_cb callback, void *user_data);

/**
 * @brief Gets the component context for the given component ID.
 * @since_tizen 5.5
 * @privlevel public
 * @privilege %http://tizen.org/privilege/packagemanager.info
 * @remarks This function returns #COMPONENT_MANAGER_ERROR_NO_SUCH_COMPONENT if the component with the given component ID is not running. \n
 *          You MUST release @c handle using component_context_destroy().
 *
 * @param[in]   comp_id         The component ID
 * @param[out]  handle          The component context of the given component ID
 * @return      @c 0 on success,
 *              otherwise a negative error value
 * @retval #COMPONENT_MANAGER_ERROR_NONE Successful
 * @retval #COMPONENT_MANAGER_ERROR_PERMISSION_DENIED Permission denied
 * @retval #COMPONENT_MANAGER_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #COMPONENT_MANAGER_ERROR_NO_SUCH_COMPONENT No such component
 * @retval #COMPONENT_MANAGER_ERROR_OUT_OF_MEMORY Out of memory
 * @retval #COMPONENT_MANAGER_ERROR_IO_ERROR I/O error
 */
int component_manager_get_component_context(const char *comp_id, component_context_h *handle);

/**
 * @brief Gets the component information for the given component ID.
 * @since_tizen 5.5
 * @privlevel public
 * @privilege %http://tizen.org/privilege/packagemanager.info
 * @remarks You MUST release @c handle using component_info_destroy().
 *
 * @param[in]   comp_id         The component ID
 * @param[out]  handle          The component information of the given component ID
 * @return      @c 0 on success,
 *              otherwise a negative error value
 * @retval #COMPONENT_MANAGER_ERROR_NONE Successful
 * @retval #COMPONENT_MANAGER_ERROR_PERMISSION_DENIED Permission denied
 * @retval #COMPONENT_MANAGER_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #COMPONENT_MANAGER_ERROR_NO_SUCH_COMPONENT No such component
 * @retval #COMPONENT_MANAGER_ERROR_OUT_OF_MEMORY Out of memory
 */
int component_manager_get_component_info(const char *comp_id, component_info_h *handle);

/**
 * @brief Checks whether the component is running or not.
 * @since_tizen 5.5
 * @privlevel public
 * @privilege %http://tizen.org/privilege/packagemanager.info
 *
 * @param[in]   comp_id         The component ID
 * @param[out]  running         @c true if the component is running, \n
 *                              otherwise @c false if not running
 * @return      @c 0 on success,
 *              otherwise a negative error value
 * @retval #COMPONENT_MANAGER_ERROR_NONE Successful
 * @retval #COMPONENT_MANAGER_ERROR_PERMISSION_DENIED Permission denied
 * @retval #COMPONENT_MANAGER_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #COMPONENT_MANAGER_ERROR_IO_ERROR I/O error
 * @retval #COMPONENT_MANAGER_ERROR_OUT_OF_MEMORY Out of memory
 */
int component_manager_is_running(const char *comp_id, bool *running);

/**
 * @brief Resumes the component.
 * @since_tizen 5.5
 * @privlevel public
 * @privilege %http://tizen.org/privilege/appmanager.launch
 *
 * @param[in]   handle          The component context handle
 * @return      @c 0 on success,
 *              otherwise a negative error value
 * @retval #COMPONENT_MANAGER_ERROR_NONE Successful
 * @retval #COMPONENT_MANAGER_ERROR_PERMISSION_DENIED Permission denied
 * @retval #COMPONENT_MANAGER_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #COMPONENT_MANAGER_ERROR_IO_ERROR I/O error
 * @retval #COMPONENT_MANAGER_ERROR_OUT_OF_MEMORY Out of memory
 * @retval #COMPONENT_MANAGER_ERROR_NO_SUCH_COMPONENT No such component
 */
int component_manager_resume_component(component_context_h handle);

/**
 * @brief Terminates the background component.
 * @since_tizen 5.5
 * @privlevel public
 * @privilege %http://tizen.org/privilege/appmanager.kill.bgapp
 * @remarks This function returns after it just sends a request for terminating a background component.
 *          Platform will decide if the target component could be terminated or not according to the state of the target component.
 *
 * @param[in]   handle          The component context handle
 * @return      @c 0 on success,
 *              otherwise a negative error value
 * @retval #COMPONENT_MANAGER_ERROR_NONE Successful
 * @retval #COMPONENT_MANAGER_ERROR_PERMISSION_DENIED Permission denied
 * @retval #COMPONENT_MANAGER_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #COMPONENT_MANAGER_ERROR_IO_ERROR I/O error
 * @retval #COMPONENT_MANAGER_ERROR_OUT_OF_MEMORY Out of memory
 * @retval #COMPONENT_MANAGER_ERROR_NO_SUCH_COMPONENT No such component
 */
int component_manager_terminate_bg_component(component_context_h handle);

/**
 * @}
 */

#ifdef __cplusplus
}
#endif

#endif /* __TIZEN_APPFW_COMPONENT_MANAGER_H__ */
