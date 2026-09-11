/*
 * Copyright (c) 2017 Samsung Electronics Co., Ltd All Rights Reserved
 *
 * Licensed under the Apache License, Version 2.0 (the License);
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 * http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an AS IS BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#ifndef __TIZEN_APPFW_PACKAGE_MANAGER_EXTENSION_H
#define __TIZEN_APPFW_PACKAGE_MANAGER_EXTENSION_H

#include "package_manager.h"

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @file package_manager_extension.h
 */

/**
 * @addtogroup CAPI_PACKAGE_MANAGER_MODULE
 * @{
 */

/**
 * @platform
 * @brief Installs the package located at the given path as the runtime mountable package, asynchronously.
 * @since_tizen 4.0
 * @privlevel platform
 * @privilege %http://tizen.org/privilege/packagemanager.admin
 * @param[in]  request The request handle
 * @param[in]  path    The absolute path to the package to be installed
 * @param[out] id      The ID of the request to the package manager
 * @return @c 0 on success,
 *         otherwise a negative error value
 * @retval #PACKAGE_MANAGER_ERROR_NONE              Successful
 * @retval #PACKAGE_MANAGER_ERROR_PERMISSION_DENIED Permission denied
 * @retval #PACKAGE_MANAGER_ERROR_INVALID_PARAMETER Invalid parameter
 * @see package_manager_request_uninstall()
 */
int package_manager_request_mount_install(package_manager_request_h request, const char *path, int *id);


/**
 * @platform
 * @brief Installs the package located at the given path as the runtime mountable package, asynchronously.
 * @details The progress of the request is asynchronously received by the callback function.
 *          The @a callback is the individual callback only called for the current API call.
 *          The @a callback is the only callback called, even if another callback was set for this request
 *          with package_manager_request_set_event_cb().
 * @since_tizen 4.0
 * @privlevel platform
 * @privilege %http://tizen.org/privilege/packagemanager.admin
 * @param[in]  request   The request handle
 * @param[in]  path      The absolute path to the package to be installed
 * @param[in]  callback  The callback function to be invoked
 * @param[in]  user_data The user data to be passed to the callback function
 * @param[out] id        The ID of the request to the package manager
 * @return @c 0 on success,
 *         otherwise a negative error value
 * @retval #PACKAGE_MANAGER_ERROR_NONE              Successful
 * @retval #PACKAGE_MANAGER_ERROR_PERMISSION_DENIED Permission denied
 * @retval #PACKAGE_MANAGER_ERROR_INVALID_PARAMETER Invalid parameter
 */
int package_manager_request_mount_install_with_cb(package_manager_request_h request,
						const char *path,
						package_manager_request_event_cb callback,
						void *user_data,
						int *id);

/**
 * @platform
 * @brief Installs the packages located at the given paths as the runtime mountable package, asynchronously.
 * @since_tizen 6.0
 * @privlevel platform
 * @privilege %http://tizen.org/privilege/packagemanager.admin
 * @param[in]  request     The request handle
 * @param[in]  paths       The array of absolute paths to the packages to be installed
 * @param[in]  paths_count The number of paths in array
 * @param[out] id          The ID of the request to the package manager
 * @return @c 0 on success,
 *         otherwise a negative error value
 * @retval #PACKAGE_MANAGER_ERROR_NONE              Successful
 * @retval #PACKAGE_MANAGER_ERROR_PERMISSION_DENIED Permission denied
 * @retval #PACKAGE_MANAGER_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #PACKAGE_MANAGER_ERROR_NO_SUCH_PACKAGE   No such package
 * @retval #PACKAGE_MANAGER_ERROR_OUT_OF_MEMORY     Out of memory
 * @retval #PACKAGE_MANAGER_ERROR_IO_ERROR          I/O error
 * @retval #PACKAGE_MANAGER_ERROR_SYSTEM_ERROR      Severe system error
 */
int package_manager_request_mount_install_packages(package_manager_request_h request,
						const char **paths,
						int paths_count,
						int *id);

/**
 * @platform
 * @brief Installs the packages located at the given paths as the runtime mountable package, asynchronously.
 * @details The progress of the request is asynchronously received by the callback function.
 *          The @a callback is the individual callback only called for the current API call.
 *          The @a callback is the only callback called, even if another callback was set for this request
 *          with package_manager_request_set_event_cb().
 * @since_tizen 6.0
 * @privlevel platform
 * @privilege %http://tizen.org/privilege/packagemanager.admin
 * @param[in]  request       The request handle
 * @param[in]  paths         The array of absolute paths to the packages to be installed
 * @param[in]  paths_count   The number of paths in array
 * @param[in]  callback      The callback function to be invoked
 * @param[in]  user_data     The user data to be passed to the callback function
 * @param[out] id            The ID of the request to the package manager
 * @return @c 0 on success,
 *         otherwise a negative error value
 * @retval #PACKAGE_MANAGER_ERROR_NONE              Successful
 * @retval #PACKAGE_MANAGER_ERROR_PERMISSION_DENIED Permission denied
 * @retval #PACKAGE_MANAGER_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #PACKAGE_MANAGER_ERROR_NO_SUCH_PACKAGE   No such package
 * @retval #PACKAGE_MANAGER_ERROR_OUT_OF_MEMORY     Out of memory
 * @retval #PACKAGE_MANAGER_ERROR_IO_ERROR          I/O error
 * @retval #PACKAGE_MANAGER_ERROR_SYSTEM_ERROR      Severe system error
 */
int package_manager_request_mount_install_packages_with_cb(package_manager_request_h request,
						const char **paths,
						int paths_count,
						package_manager_request_event_cb callback,
						void *user_data,
						int *id);


/**
 * @platform
 * @brief Clears a data in given relative file path of user directories used by applications in the given package.
 * @details A file or directory specified with the @a file_path stored in data directories used by applications in the package specified with the @a package_id are removed.
 * @since_tizen 8.0
 * @privlevel platform
 * @privilege %http://tizen.org/privilege/packagemanager.admin
 * @param[in] package_id  The package ID
 * @param[in] file_path   The file path that is relative path from the given package user data directory.
 * @return @c 0 on success,
 *         otherwise a negative error value
 * @retval #PACKAGE_MANAGER_ERROR_NONE              Successful
 * @retval #PACKAGE_MANAGER_ERROR_PERMISSION_DENIED Permission denied
 * @retval #PACKAGE_MANAGER_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #PACKAGE_MANAGER_ERROR_NO_SUCH_PACKAGE   No such package
 * @retval #PACKAGE_MANAGER_ERROR_OUT_OF_MEMORY     Out of memory
 * @retval #PACKAGE_MANAGER_ERROR_IO_ERROR          I/O error
 * @retval #PACKAGE_MANAGER_ERROR_SYSTEM_ERROR      Severe system error
 */
int package_manager_clear_user_data_with_path(const char *package_id, const char *file_path);


/**
 * @platform
 * @brief Get the detailed error code from async result in the thread.
 * @details If the package_manager_error_e value is not PACKAGE_MANAGER_ERROR_NONE from the async result callback,
 *          the detailed error code is set internally and can be obtained with this api.
 *          The detailed error code is valid only inside result callback
 * @since_tizen 10.0
 * @return detailed error code
 * @see package_manager_event_cb()
 * @see package_manager_request_event_cb()
 * @see package_manager_res_event_cb()
 * @see package_manager_request_res_event_cb()
 */
int package_manager_get_last_detailed_error();


/**
 * @platform
 * @brief Get error message from detailed error code.
 * @since_tizen 10.0
 * @return detailed error code
 * @see package_manager_get_detailed_error()
 */
const char *package_manager_detailed_error_to_string(int error);

/**
* @}
*/

#ifdef __cplusplus
}
#endif

#endif /* __TIZEN_APPFW_PACKAGE_MANAGER_EXTENSION_H */
