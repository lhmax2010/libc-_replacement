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

#ifndef __TIZEN_APPFW_PACKAGE_ARCHIVE_INFO_H
#define __TIZEN_APPFW_PACKAGE_ARCHIVE_INFO_H

#include <package_info.h>

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @file package_archive_info.h
 */

/**
 * @addtogroup CAPI_PACKAGE_ARCHIVE_INFO_MODULE
 * @{
 */


/**
 * @brief The package archive information handle.
 * @since_tizen 4.0
 */
typedef struct package_archive_info_s *package_archive_info_h;

/**
 * @brief Creates the package archive information for the given package.
 * @since_tizen 4.0
 * @remarks If the given path is relevant to media storage, the privilege
 *          %http://tizen.org/privilege/mediastorage is needed.
 * @remarks If the given path is relevant to external storage, the privilege
 *          %http://tizen.org/privilege/externalstorage is needed.
 * @remarks You must release @a archive_info
 *          using package_archive_info_destroy().
 * @param[in]  path         The path of the package
 * @param[out] archive_info The package archive information for
 *                          the given package
 * @return @c 0 on success,
 *         otherwise a negative error value
 * @retval #PACKAGE_MANAGER_ERROR_NONE              Successful
 * @retval #PACKAGE_MANAGER_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #PACKAGE_MANAGER_ERROR_OUT_OF_MEMORY     Out of memory
 * @retval #PACKAGE_MANAGER_ERROR_IO_ERROR          Internal I/O error
 * @see package_archive_info_destroy()
 */
int package_archive_info_create(const char *path,
		package_archive_info_h *archive_info);

/**
 * @brief Destroys the package archive information handle.
 * @since_tizen 4.0
 * @param[in] archive_info The package archive information handle
 * @return @c 0 on success,
 *         otherwise a negative error value
 * @retval #PACKAGE_MANAGER_ERROR_NONE              Successful
 * @retval #PACKAGE_MANAGER_ERROR_INVALID_PARAMETER Invalid parameter
 * @see package_archive_info_create()
 */
int package_archive_info_destroy(package_archive_info_h archive_info);

/**
 * @brief Gets the package name.
 * @since_tizen 4.0
 * @remarks You must release @a package using free().
 * @param[in]  archive_info The package archive information
 * @param[out] package      The package name
 * @return @c 0 on success,
 *         otherwise a negative error value
 * @retval #PACKAGE_MANAGER_ERROR_NONE              Successful
 * @retval #PACKAGE_MANAGER_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #PACKAGE_MANAGER_ERROR_OUT_OF_MEMORY     Out of memory
 */
int package_archive_info_get_package(package_archive_info_h archive_info,
		char **package);

/**
 * @brief Gets the type of the package.
 * @since_tizen 4.0
 * @remarks You must release @a type using free().
 * @param[in]  archive_info The package archive information
 * @param[out] type         The type of the package
 * @return @c 0 on success,
 *         otherwise a negative error value
 * @retval #PACKAGE_MANAGER_ERROR_NONE              Successful
 * @retval #PACKAGE_MANAGER_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #PACKAGE_MANAGER_ERROR_OUT_OF_MEMORY     Out of memory
 */
int package_archive_info_get_type(package_archive_info_h archive_info,
		char **type);

/**
 * @brief Gets the version of the package.
 * @since_tizen 4.0
 * @remarks You must release @a version using free().
 * @param[in]  archive_info The package archive information
 * @param[out] version      The version of the package
 * @return @c 0 on success,
 *         otherwise a negative error value
 * @retval #PACKAGE_MANAGER_ERROR_NONE              Successful
 * @retval #PACKAGE_MANAGER_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #PACKAGE_MANAGER_ERROR_OUT_OF_MEMORY     Out of memory
 */
int package_archive_info_get_version(package_archive_info_h archive_info,
		char **version);

/**
 * @brief Gets the API version of the package.
 * @since_tizen 4.0
 * @remarks You must release @a api_version using free().
 * @param[in]  archive_info The package archive information
 * @param[out] api_version  The API version of the package
 * @return @c 0 on success,
 *         otherwise a negative error value
 * @retval #PACKAGE_MANAGER_ERROR_NONE              Successful
 * @retval #PACKAGE_MANAGER_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #PACKAGE_MANAGER_ERROR_OUT_OF_MEMORY     Out of memory
 */
int package_archive_info_get_api_version(package_archive_info_h archive_info,
		char **api_version);

/**
 * @brief Gets the description of the package.
 * @since_tizen 4.0
 * @remarks You must release @a description using free().
 * @param[in]  archive_info The package archive information
 * @param[out] description  The description of the package
 * @return @c 0 on success,
 *         otherwise a negative error value
 * @retval #PACKAGE_MANAGER_ERROR_NONE              Successful
 * @retval #PACKAGE_MANAGER_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #PACKAGE_MANAGER_ERROR_OUT_OF_MEMORY     Out of memory
 * @retval #PACKAGE_MANAGER_ERROR_IO_ERROR          I/O error
 */
int package_archive_info_get_description(package_archive_info_h archive_info,
		char **description);

/**
 * @brief Gets the label of the package.
 * @since_tizen 4.0
 * @remarks You must release @a label using free().
 * @param[in]  archive_info The package archive information
 * @param[out] label        The label of the package
 * @return @c 0 on success,
 *         otherwise a negative error value
 * @retval #PACKAGE_MANAGER_ERROR_NONE              Successful
 * @retval #PACKAGE_MANAGER_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #PACKAGE_MANAGER_ERROR_OUT_OF_MEMORY     Out of memory
 * @retval #PACKAGE_MANAGER_ERROR_IO_ERROR          I/O error
 */
int package_archive_info_get_label(package_archive_info_h archive_info,
		char **label);

/**
 * @brief Gets the author of the package.
 * @since_tizen 4.0
 * @remarks You must release @a author using free().
 * @param[in]  archive_info The package archive information
 * @param[out] author       The author of the package
 * @return @c 0 on success,
 *         otherwise a negative error value
 * @retval #PACKAGE_MANAGER_ERROR_NONE              Successful
 * @retval #PACKAGE_MANAGER_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #PACKAGE_MANAGER_ERROR_OUT_OF_MEMORY     Out of memory
 * @retval #PACKAGE_MANAGER_ERROR_IO_ERROR          I/O error
 */
int package_archive_info_get_author(package_archive_info_h archive_info,
		char **author);

/**
 * @brief Gets the raw icon of the package.
 * @since_tizen 4.0
 * @remarks You must release @a icon using free().
 * @param[in]  archive_info The package archive information
 * @param[out] icon         The raw icon of the package
 * @param[out] icon_size    The size of the icon
 * @return @c 0 on success,
 *         otherwise a negative error value
 * @retval #PACKAGE_MANAGER_ERROR_NONE              Successful
 * @retval #PACKAGE_MANAGER_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #PACKAGE_MANAGER_ERROR_OUT_OF_MEMORY     Out of memory
 * @retval #PACKAGE_MANAGER_ERROR_IO_ERROR          I/O error
 */
int package_archive_info_get_icon(package_archive_info_h archive_info,
		unsigned char **icon, size_t *icon_size);

/**
 * @brief Retrieves direct dependencies of the given package.
 * @details package_info_dependency_info_cb() is called for each direct dependency of the given package.
 *          For example, if there are packages having the following relationship:
 *
 * <pre>
 *                B --> E
 *          A --> B --> C
 *                D --> C
 * </pre>
 * A --> B means that A depends on B.
 *
 * If package_archive_info_foreach_direct_dependency(A) is called, the callback will be called once with the following argument values: @e from = A, @e to = B.
 * @since_tizen 5.5
 * @remarks The function provides the results synchronously. If there are no dependencies, this function will return #PACKAGE_MANAGER_ERROR_NONE immediately and the callback will not be invoked.
 * @param[in] archive_info The package archive information
 * @param[in] callback     The iteration callback function
 * @param[in] user_data    The user data to be passed to the callback function
 * @return @c 0 on success,
 *         otherwise a negative error value
 * @retval #PACKAGE_MANAGER_ERROR_NONE              Successful
 * @retval #PACKAGE_MANAGER_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #PACKAGE_MANAGER_ERROR_SYSTEM_ERROR      Severe system error
 */
int package_archive_info_foreach_direct_dependency(
		package_archive_info_h archive_info,
		package_info_dependency_info_cb callback, void *user_data);


/**
* @}
*/

#ifdef __cplusplus
}
#endif

#endif /* __TIZEN_APPFW_PACKAGE_ARCHIVE_INFO_H */
