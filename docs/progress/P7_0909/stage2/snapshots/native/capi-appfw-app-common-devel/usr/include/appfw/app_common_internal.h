/*
 * Copyright (c) 2011 - 2025 Samsung Electronics Co., Ltd All Rights Reserved
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

#ifndef __TIZEN_APPFW_APP_COMMON_INTERNAL_H__
#define __TIZEN_APPFW_APP_COMMON_INTERNAL_H__

#include <app_common.h>
#include <app_types.h>

#ifdef __cplusplus
extern "C" {
#endif

#define TIZEN_PATH_MAX 1024

typedef void (*app_finalizer_cb) (void *data);

int app_error(app_error_e error, const char *function, const char *description);

int app_get_package_app_name(const char *package, char **name);

int app_finalizer_add(app_finalizer_cb callback, void *data);

int app_finalizer_remove(app_finalizer_cb callback);

void app_finalizer_execute(void);

int app_get_package(char **package);

/**
 * @brief Gets the absolute path to the application's common data directory which is used to store private data of the application.
 * @details An application can read and write its own files in the application's common data directory.
 * @since_tizen 10.0
 * @remarks The returned value should be released using free().
 * @return The absolute path to the application's commondata directory, @n
 *         otherwise a null pointer if the memory is insufficient
 */
char *app_get_common_data_path(void);

/**
 * @brief Gets the absolute path to the application's common cache directory which is used to store temporary data of the application.
 * @details An application can read and write its own files in the application's common cache directory.
 * @since_tizen 10.0
 * @remarks The returned value should be released using free(). @n
 *          The files stored in the application's common cache directory can be removed by setting application or platform while the application is running.
 * @return The absolute path to the application's common cache directory, @n
 *         otherwise a null pointer if the memory is insufficient
 */
char *app_get_common_cache_path(void);

/**
 * @brief Gets the absolute path to the application's common shared data directory which is used to share data with other applications.
 * @details An application can read and write its own files in the application's shared data directory and others can only read the files.
 * @since_tizen 10.0
 * @privlevel public
 * @privilege %http://tizen.org/privilege/appdir.shareddata
 * @remarks The returned value should be released using free(). @n
 *          An application that want to use shared/data directory must declare %http://tizen.org/privilege/appdir.shareddata privilege. If the application doesn't declare the privilege, the framework will not create shared/data directory for the application.
 *          Carefully consider the privacy implications when deciding whether to use the shared/data directory, since the application cannot control access to this directory by other applications.
 *          If you want to share files with other applications, consider passing path via @ref CAPI_APP_CONTROL_MODULE API.
 *          The @ref CAPI_APP_CONTROL_MODULE API supports giving permission to other applications by passing path via app_control. @n
 *          The specific error code can be obtained using the get_last_result(). Error codes are described in Exception section.
 *
 * @return The absolute path to the application's common shared data directory, @n
 *         otherwise a null pointer if the memory is insufficient. It will return NULL, and set #APP_ERROR_PERMISSION_DENIED if the application does not declare the shareddata privilege.
 * @exception #APP_ERROR_NONE Successful
 * @exception #APP_ERROR_PERMISSION_DENIED Permission denied
 * @exception #APP_ERROR_OUT_OF_MEMORY Out of memory
 */
char *app_get_common_shared_data_path(void);

/**
 * @brief Gets the absolute path to the application's common shared resource directory which is used to share resources with other applications.
 * @details An application can read its own files in the application's common shared resource directory, and others can only read the files.
 * @since_tizen 10.0
 * @remarks The returned value should be released using free().
 * @return The absolute path to the application's common shared resource directory, @n
 *         otherwise a null pointer if the memory is insufficient
 */
char *app_get_common_shared_trusted_path(void);

/**
 * @brief Callback function to add an event handler.
 * @param[in] event The event handle to be added
 * @param[in] user_data The user data passed from app_event_init()
 */
typedef void (*add_event_cb)(app_event_handler_h event, void *user_data);

/**
 * @brief Callback function to remove an event handler.
 * @param[in] event The event handle to be removed
 * @param[in] user_data The user data passed from app_event_init()
 */
typedef void (*remove_event_cb)(app_event_handler_h event, void *user_data);

/**
 * @brief Initializes the event module.
 * @since_tizen 10.0
 * @param[in] add_cb The callback function to add an event handler
 * @param[in] remove_cb The callback function to remove an event handler
 * @param[in] user_data The user data to be passed to the callback functions
 */
void app_event_init(add_event_cb add_cb, remove_event_cb remove_cb, void *user_data);

/**
 * @brief Finalizes the event module.
 * @since_tizen 10.0
 */
void app_event_fini(void);

/**
 * @brief Adds an event handler for the given event type.
 * @since_tizen 10.0
 * @param[out] handler The event handler handle
 * @param[in] event_type The type of the event to handle
 * @param[in] callback The callback function to be called when the event occurs
 * @param[in] user_data The user data to be passed to the callback function
 * @return @c 0 on success, otherwise a negative error value
 * @retval #APP_ERROR_NONE Successful
 * @retval #APP_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #APP_ERROR_OUT_OF_MEMORY Out of memory
 */
int app_event_add_handler(app_event_handler_h *handler,
                          app_event_type_e event_type, app_event_cb callback,
                          void *user_data);

/**
 * @brief Removes the event handler.
 * @since_tizen 10.0
 * @param[in] handler The event handler to be removed
 * @return @c 0 on success, otherwise a negative error value
 * @retval #APP_ERROR_NONE Successful
 * @retval #APP_ERROR_INVALID_PARAMETER Invalid parameter
 */
int app_event_remove_handler(app_event_handler_h handler);

/**
 * @brief Set application's language independent of system locale.
 * @since_tizen 10.0
 * @param[in] lang Locale conforming to POSIX SPEC
 * @return @c 0 on success, otherwise a negative error value
 * @retval #APP_ERROR_NONE Successful
 * @retval #APP_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #APP_ERROR_INVALID_CONTEXT Invalid application context
 * @see app_locale_manager_get_language()
 */
int app_locale_manager_set_language(const char *lang);

/**
 * @brief Get application's language
 * @since_tizen 10.0
 * @remarks The @a lang MUST be released using free().
 * @param[out] lang The application language is newly set upon success.
 * @return @c 0 on success, otherwise a negative error value
 * @retval #APP_ERROR_NONE Successful
 * @retval #APP_ERROR_INVALID_CONTEXT Invalid application context
 * @retval #APP_ERROR_OUT_OF_MEMORY Out of memory
 * @see app_locale_manager_set_language()
 */
int app_locale_manager_get_language(char **lang);

/**
 * @brief Get system's language
 * @since_tizen 10.0
 * @remarks The @a lang MUST be released using free().
 * @param[out] lang The system language is newly set on success
 * @return @c 0 on success, otherwise a negative error value
 * @retval #APP_ERROR_NONE Successful
 * @retval #APP_ERROR_INVALID_CONTEXT Invalid application context
 * @retval #APP_ERROR_OUT_OF_MEMORY Out of memory
 */
int app_locale_manager_get_system_language(char **lang);

#ifdef __cplusplus
}
#endif

#endif /* __TIZEN_APPFW_APP_COMMON_INTERNAL_H__ */
