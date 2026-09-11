/*
 * Copyright (c) 2014-2023 Samsung Electronics Co., Ltd All Rights Reserved
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


#ifndef __TIZEN_SECURITY_PRIVILEGE_INFORMATION_H__
#define __TIZEN_SECURITY_PRIVILEGE_INFORMATION_H__


#include <tizen.h>
#include <glib.h>

#ifndef TIZEN_DEPRECATED_API
#define TIZEN_DEPRECATED_API __attribute__ ((__visibility__("default"), deprecated))
#endif

#ifdef __cplusplus
extern "C" {
#endif


/**
 * @addtogroup CAPI_SECURITY_FRAMEWORK_PRIVILEGE_INFO_MODULE
 * @{
 */


/**
 * @brief Enumeration for Privilege Information errors.
 *
 * @since_tizen 2.3
 */
typedef enum {
	PRVINFO_ERROR_NONE = TIZEN_ERROR_NONE, /**< Successful */
	PRVINFO_ERROR_INVALID_PARAMETER = TIZEN_ERROR_INVALID_PARAMETER, /**< Invalid function parameter */
	PRVINFO_ERROR_OUT_OF_MEMORY = TIZEN_ERROR_OUT_OF_MEMORY, /**< Out of memory */
	PRVINFO_ERROR_INTERNAL_ERROR = TIZEN_ERROR_UNKNOWN, /**< Unknown error */
	PRVINFO_ERROR_NOT_SUPPORTED = TIZEN_ERROR_NOT_SUPPORTED, /**< @deprecated Not supported (Deprecated since 8.0) */
	PRVINFO_ERROR_NO_MATCHING_PRIVILEGE = TIZEN_ERROR_PRIVILEGE_INFORMATION | 0x01 /**< No matching privilege found (Since 6.0) */
} privilege_info_error_e;


/**
 * @brief Gets the display name of the given privilege.
 *
 * @since_tizen 2.3
 *
 * @remarks @a display_name must be released using free().
 * @remarks Since 6.0, this function returns #PRVINFO_ERROR_NO_MATCHING_PRIVILEGE if the given
 *          @a privilege doesn't exist.
 *
 * @param[in] api_version API version of the application to get privilege information
 * @param[in] privilege Given privilege
 * @param[out] display_name Display name of the privilege
 *
 * @return @c 0 on success, otherwise a negative error value
 * @retval #PRVINFO_ERROR_NONE Successful
 * @retval #PRVINFO_ERROR_INVALID_PARAMETER Invalid function parameter
 * @retval #PRVINFO_ERROR_OUT_OF_MEMORY Out of memory
 * @retval #PRVINFO_ERROR_INTERNAL_ERROR Unknown error
 * @retval #PRVINFO_ERROR_NO_MATCHING_PRIVILEGE No matching privilege found
 */
int privilege_info_get_display_name(const char *api_version, const char *privilege, char **display_name);


/**
 * @brief Gets the display name of the given privilege by type of application package.
 *
 * @since_tizen 2.3
 *
 * @remarks @a display_name must be released using free().
 * @remarks @a package_type must be one of following: "PRVINFO_PACKAGE_TYPE_NATIVE",
 *          "PRVINFO_PACKAGE_TYPE_WEB".
 * @remarks Since 6.0, this function returns #PRVINFO_ERROR_NO_MATCHING_PRIVILEGE if the given
 *          @a privilege doesn't exist.
 *
 * @param[in] package_type Type of application package
 * @param[in] api_version API version of the application to get privilege information
 * @param[in] privilege Given privilege
 * @param[out] display_name Display name of the privilege
 *
 * @return @c 0 on success, otherwise a negative error value
 * @retval #PRVINFO_ERROR_NONE Successful
 * @retval #PRVINFO_ERROR_INVALID_PARAMETER Invalid function parameter
 * @retval #PRVINFO_ERROR_OUT_OF_MEMORY Out of memory
 * @retval #PRVINFO_ERROR_INTERNAL_ERROR Unknown error
 * @retval #PRVINFO_ERROR_NO_MATCHING_PRIVILEGE No matching privilege found
 */
int privilege_info_get_display_name_by_pkgtype(const char *package_type, const char *api_version, const char *privilege, char **display_name);


/**
 * @brief Gets the description of the given privilege.
 *
 * @since_tizen 2.3
 *
 * @remarks @a description must be released using free().
 * @remarks Since 6.0, this function returns #PRVINFO_ERROR_NO_MATCHING_PRIVILEGE if the given
 *          @a privilege doesn't exist.
 *
 * @param[in] api_version API version of the application to get privilege information
 * @param[in] privilege Given privilege
 * @param[out] description Description of the privilege
 *
 * @return @c 0 on success, otherwise a negative error value
 * @retval #PRVINFO_ERROR_NONE Successful
 * @retval #PRVINFO_ERROR_INVALID_PARAMETER Invalid function parameter
 * @retval #PRVINFO_ERROR_OUT_OF_MEMORY Out of memory
 * @retval #PRVINFO_ERROR_INTERNAL_ERROR Unknown error
 * @retval #PRVINFO_ERROR_NO_MATCHING_PRIVILEGE No matching privilege found
 */
int privilege_info_get_description(const char *api_version, const char *privilege, char **description);


/**
 * @brief Gets the description of the given privilege by type of application package.
 *
 * @since_tizen 2.3
 *
 * @remarks @a description must be released using free().
 * @remarks @a package_type must be one of following: "PRVINFO_PACKAGE_TYPE_NATIVE",
 *          "PRVINFO_PACKAGE_TYPE_WEB".
 * @remarks Since 6.0, this function returns #PRVINFO_ERROR_NO_MATCHING_PRIVILEGE if the given
 *          @a privilege doesn't exist.
 *
 * @param[in] package_type Type of application package
 * @param[in] api_version API version of the application to get privilege information
 * @param[in] privilege Given privilege
 * @param[out] description Description of the privilege
 *
 * @return @c 0 on success, otherwise a negative error value
 * @retval #PRVINFO_ERROR_NONE Successful
 * @retval #PRVINFO_ERROR_INVALID_PARAMETER Invalid function parameter
 * @retval #PRVINFO_ERROR_OUT_OF_MEMORY Out of memory
 * @retval #PRVINFO_ERROR_INTERNAL_ERROR Unknown error
 * @retval #PRVINFO_ERROR_NO_MATCHING_PRIVILEGE No matching privilege found
 */
int privilege_info_get_description_by_pkgtype(const char *package_type, const char *api_version, const char *privilege, char **description);


/**
 * @deprecated Deprecated since 8.0.
 *
 * @brief Gets the display name of the privacy group in which the given privilege is included.
 *
 * @since_tizen 3.0
 *
 * @remarks @a privacy_name must be released using free().
 * @remarks @a privilege must be privacy related, otherwise
 *          #PRVINFO_ERROR_INVALID_PARAMETER is returned.
 *
 * @param[in] privilege Given privilege
 * @param[out] privacy_name Privacy group's display name in which the given privilege is included
 *
 * @return @c 0 on success, otherwise a negative error value
 * @retval #PRVINFO_ERROR_NONE Successful
 * @retval #PRVINFO_ERROR_NOT_SUPPORTED Not supported
 * @retval #PRVINFO_ERROR_INVALID_PARAMETER Invalid function parameter
 * @retval #PRVINFO_ERROR_OUT_OF_MEMORY Out of memory
 * @retval #PRVINFO_ERROR_INTERNAL_ERROR Unknown error
 */
int privilege_info_get_privacy_display_name(const char *privilege, char **privacy_name) TIZEN_DEPRECATED_API;


/**
 * @brief Structure for privilege information.
 *
 * @since_tizen 5.5
 */
typedef struct _privilege_info_s {
	char* privilege_name; /**< Privilege name */
	char* display_name; /**< Display name of the privilege */
	char* description; /**< Description of the privilege */
} privilege_info_s;


/**
 * @brief Enumeration for return codes sent to consumer.
 *
 * @since_tizen 5.5
 */
typedef enum {
	PRIVILEGE_CONSUMER_RETURN_CODE_SUCCESS = 0, /**< Successful */
	PRIVILEGE_CONSUMER_RETURN_CODE_UNKNOWN_LOCALE_CODE, /**< Unknown locale code */
	PRIVILEGE_CONSUMER_RETURN_CODE_INVALID_PARAMETER, /**< Invalid parameter */
} privilege_consumer_return_code_e;


/**
 * @brief Gets the privilege information list of the given locale and privileges.
 * @details If the given @a locale is not recognized, @a return_result is set to
 *          #PRIVILEGE_CONSUMER_RETURN_CODE_UNKNOWN_LOCALE_CODE and the result in the
 *          @a privilege_info_list is provided according to the current locale.
 *          If all of the privileges in @a privilege_name_list are invalid then @a return_result
 *          is set to #PRIVILEGE_CONSUMER_RETURN_CODE_INVALID_PARAMETER. Otherwise, @a return_result
 *          is set to #PRIVILEGE_CONSUMER_RETURN_CODE_SUCCESS. If some of the privileges in
 *          @a privilege_name_list are invalid then corresponding elements in the
 *          @a privilege_info_list will have the display_name and the description set
 *          to an empty string.
 *
 * @since_tizen 5.5
 *
 * @remarks You must destroy the newly created privilege_info_list by calling
 *          privilege_info_free_privilege_info_list() if it is no longer needed.
 *
 * @param[in] locale Locale information
 * @param[in] privilege_name_list Privilege name list
 * @param[out] privilege_info_list Privilege information list of the given locale and privileges
 * @param[out] return_result Return code to be sent to consumer
 *
 * @return @c 0 on success, otherwise a negative error value
 * @retval #PRVINFO_ERROR_NONE Successful
 * @retval #PRVINFO_ERROR_INVALID_PARAMETER Invalid function parameter
 * @retval #PRVINFO_ERROR_OUT_OF_MEMORY Out of memory
 * @retval #PRVINFO_ERROR_INTERNAL_ERROR Unknown error
 *
 * @see privilege_info_free_privilege_info_list()
 * @see privilege_consumer_return_code_e
 */
int privilege_info_get_privilege_info_list(const char* locale, GList* privilege_name_list, GList** privilege_info_list, privilege_consumer_return_code_e* return_result);


/**
 * @brief Destroys the privilege information list.
 *
 * @since_tizen 5.5
 *
 * @remarks If @a privilege_info_list is NULL, no operation is performed and
 *          #PRVINFO_ERROR_INVALID_PARAMETER will be returned.
 *
 * @param[in] privilege_info_list Privilege information list to destroy
 *
 * @return @c 0 on success, otherwise a negative error value
 * @retval #PRVINFO_ERROR_NONE Successful
 * @retval #PRVINFO_ERROR_INVALID_PARAMETER Invalid function parameter
 */
int privilege_info_free_privilege_info_list(GList* privilege_info_list);


/**
 * @}
 */


#ifdef __cplusplus
}
#endif


#endif /* __TIZEN_SECURITY_PRIVILEGE_INFORMATION_H__ */
