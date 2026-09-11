/*
 * Copyright (c) 2019 - 2021 Samsung Electronics Co., Ltd.
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

#ifndef __TIZEN_COMPONENT_BASED_APPLICATION_H__
#define __TIZEN_COMPONENT_BASED_APPLICATION_H__

#include <app_common.h>
#include <component_common.h>
#include <frame_component.h>
#include <service_component.h>

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @addtogroup COMPONENT_BASED_APPLICATION_MODULE
 * @{
 */


/**
 * @deprecated Deprecated since 10.0.
 * @brief Called when the application starts.
 * @details The callback function is called before the main loop of the application starts.
 *          In this callback, you can initialize resources which can be shared among component instances.
 *          This function should return the handle for component class so that it will be used for making instances of component.
 * @since_tizen 5.5
 * @remarks The returned object is managed by platform. You should not release the object.
 *
 * @param[in]   user_data       The user data to be passed from the callback registration function
 * @return      The object of component class
 * @see component_based_app_main()
 * @see component_based_app_add_frame_component()
 * @see component_based_app_add_service_component()
 */
typedef component_class_h (*component_based_app_create_cb)(void *user_data);

/**
 * @deprecated Deprecated since 10.0.
 * @brief Called when the main loop of the application exits.
 * @details This callback function is called once after the main loop of the application exits.
 *          You should release the resources of the application in this function.
 * @since_tizen 5.5
 *
 * @param[in]   user_data       The user data to be passed from the callback registration function
 * @see component_based_app_main()
 */
typedef void (*component_based_app_terminate_cb)(void *user_data);

/**
 * @deprecated Deprecated since 10.0.
 * @brief The structure for lifecycle of a component based application.
 * @since_tizen 5.5
 */
typedef struct {
	component_based_app_create_cb create;           /**< The callback function is called before the main loop of the application starts. */
	component_based_app_terminate_cb terminate;     /**< The callback function is called once after the main loop of the application exits. */
} component_based_app_lifecycle_callback_s;

/**
 * @deprecated Deprecated since 10.0.
 * @brief Runs the main loop of the application until component_based_app_exit() is called.
 * @since_tizen 5.5
 *
 * @param[in]   argc            The argument count
 * @param[in]   argv            The argument vector
 * @param[in]   callback        The set of callback functions to handle application events
 * @param[in]   user_data       The user data to be passed to the callback function
 * @return      @c 0 on success,
 *              otherwise a negative error value
 * @retval #APP_ERROR_NONE Successful
 * @retval #APP_ERROR_INVALID_PARAMETER Invalid parameter
 * @see component_based_app_exit()
 */
int component_based_app_main(int argc, char **argv,
		component_based_app_lifecycle_callback_s *callback,
		void *user_data) TIZEN_DEPRECATED_API;

/**
 * @deprecated Deprecated since 10.0.
 * @brief Exits the main loop of the application.
 * @since_tizen 5.5
 *
 * @return      @c 0 on success,
 *              otherwise a negative error value
 * @retval #APP_ERROR_NONE Successful
 */
int component_based_app_exit(void) TIZEN_DEPRECATED_API;

/**
 * @deprecated Deprecated since 10.0.
 * @brief Adds a new frame component to @a comp_class.
 * @details Adds a new frame component with @a comp_id to @a comp_class.
 *          If @a comp_class is null, the platform creates a #component_class_h and adds a new frame component with @a comp_id and then return the #component_class_h.
 * @since_tizen 5.5
 * @remarks The specific error code can be obtained using the get_last_result() function.
 *          Error codes are described in Exception section.
 * @remarks The returned value is managed by the platform and will be released when the component_based_app_create_cb() function returns it.
 *
 * @param[in]   comp_class      The component class
 * @param[in]   comp_id         The ID of the frame component
 * @param[in]   callback        The set of lifecycle callbacks of the frame component
 * @param[in]   user_data       The user data to be passed to the callback functions
 * @return      @a comp_class on success, otherwise NULL
 * @exception #APP_ERROR_NONE Successful
 * @exception #APP_ERROR_INVALID_PARAMETER Invalid parameter
 * @exception #APP_ERROR_OUT_OF_MEMORY Out of memory
 * @see get_last_result()
 */
component_class_h component_based_app_add_frame_component(
		component_class_h comp_class,
		const char *comp_id,
		frame_component_lifecycle_callback_s *callback,
		void *user_data) TIZEN_DEPRECATED_API;

/**
 * @deprecated Deprecated since 10.0.
 * @brief Adds a service component to @a comp_class.
 * @details Adds a new service component with @a comp_id to @a comp_class.
 *          If @a comp_class is null, the platform creates a #component_class_h and adds a new service component with @a comp_id and then return the #component_class_h.
 * @since_tizen 5.5
 * @remarks The specific error code can be obtained using the get_last_result() function.
 *          Error codes are described in Exception section.
 * @remarks The returned value is managed by the platform and will be released when the component_based_app_create_cb() function returns it.
 *
 * @param[in]   comp_class      The component class
 * @param[in]   comp_id         The ID of the service component
 * @param[in]   callback        The set of lifecycle callbacks of the service component
 * @param[in]   user_data       The user data to be passed to the callback functions
 * @return      @a comp_class on success, otherwise NULL
 * @exception #APP_ERROR_NONE Successful
 * @exception #APP_ERROR_INVALID_PARAMETER Invalid parameter
 * @exception #APP_ERROR_OUT_OF_MEMORY Out of memory
 * @see get_last_result()
 */
component_class_h component_based_app_add_service_component(
		component_class_h comp_class,
		const char *comp_id,
		service_component_lifecycle_callback_s *callback,
		void *user_data) TIZEN_DEPRECATED_API;

/**
 * @}
 */

#ifdef __cplusplus
}
#endif
#endif /* __TIZEN_COMPONENT_BASED_APPLICATION_H__ */

