/*
 * Copyright (c) 2011 - 2016 Samsung Electronics Co., Ltd All Rights Reserved
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


#ifndef __TIZEN_APPFW_APP_INFO_H
#define __TIZEN_APPFW_APP_INFO_H

#include <tizen.h>

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @file app_info.h
 */

/**
* @addtogroup CAPI_APP_INFO_MODULE
* @{
*/

/**
 * @brief  Definition for string property for filtering based on app info: String property for filtering packages with app id.
 * @since_tizen 2.3
 */
#define  PACKAGE_INFO_PROP_APP_ID            "PACKAGE_INFO_PROP_APP_ID"

/**
 * @brief  Definition for string property for filtering based on app info: String property for filtering packages with app type.
 * @since_tizen 2.3
 */
#define  PACKAGE_INFO_PROP_APP_TYPE          "PACKAGE_INFO_PROP_APP_TYPE"

/**
 * @brief  Definition for string property for filtering based on app info: String property for filtering packages with app category.
 * @since_tizen 2.3
 */
#define  PACKAGE_INFO_PROP_APP_CATEGORY      "PACKAGE_INFO_PROP_APP_CATEGORY"

/**
 * @brief  Definition for string property for filtering based on app info: String property for filtering packages with installed storage of app.
 *         Value related with this property should be "installed_internal" or "installed_external".
 * @since_tizen 3.0
 */
#define  PACKAGE_INFO_PROP_APP_INSTALLED_STORAGE      "PACKAGE_INFO_PROP_APP_INSTALLED_STORAGE"

/**
 * @brief Definition for boolean property for filtering based on app info: Boolean property for filtering whether the package is nodisplay or not based on package info.
 * @since_tizen 2.3
 */
#define	PACKAGE_INFO_PROP_APP_NODISPLAY     "PACKAGE_INFO_PROP_APP_NODISPLAY"

/**
 * @brief Definition for boolean property for filtering based on app info: Boolean property for filtering whether the package has the authority to manage task or not based on package info.
 * @since_tizen 2.3
 */
#define	PACKAGE_INFO_PROP_APP_TASKMANAGE    "PACKAGE_INFO_PROP_APP_TASKMANAGE"

/**
 * @brief Definition for boolean property for filtering based on app info: Boolean property for filtering whether the application has been disabled.
 * @since_tizen 4.0
 */
#define   PACKAGE_INFO_PROP_APP_DISABLED    "PACKAGE_INFO_PROP_APP_DISABLED"

/**
 * @brief Definition for string property for filtering based on app info: String property for
 *        filtering with the application component type.
 *        Value related with this property should be one of "uiapp", "svcapp", "widgetapp",
 *        "watchapp" and "componentbasedapp" (Since 5.5).
 * @since_tizen 4.0
 * @see app_info_app_component_type_e
 * @see app_info_get_app_component_type()
 */
#define   PACKAGE_INFO_PROP_APP_COMPONENT_TYPE    "PACKAGE_INFO_PROP_APP_COMPONENT_TYPE"

/**
 * @brief Enumeration for application component type.
 * @details A component is an application considered as a part of a package.
 *          The application component type indicates what type of
 *          a component an application is in a package.
 * @since_tizen 4.0
 * @see PACKAGE_INFO_PROP_APP_COMPONENT_TYPE
 * @see app_info_get_app_component_type()
 */
typedef enum {
	APP_INFO_APP_COMPONENT_TYPE_UI_APP,                     /**< UI application */
	APP_INFO_APP_COMPONENT_TYPE_SERVICE_APP,                /**< Service application */
	APP_INFO_APP_COMPONENT_TYPE_WIDGET_APP,                 /**< Widget application */
	APP_INFO_APP_COMPONENT_TYPE_WATCH_APP,                  /**< Watch application */
	APP_INFO_APP_COMPONENT_TYPE_COMPONENT_BASED_APP,        /**< Component-based application (Since 5.5) */
} app_info_app_component_type_e;

/**
 * @brief Application information handle.
 * @since_tizen 2.3
 */
typedef struct app_info_s *app_info_h;

/**
 * @brief Application filter handle.
 * @since_tizen 2.3
 */
typedef struct app_info_filter_s *app_info_filter_h;

/**
 * @brief Application metadata filter handle.
 * @since_tizen 2.3
 */
typedef struct app_info_metadata_filter_s *app_info_metadata_filter_h;

/**
 * @brief  Called to get the application information once for each installed application.
 * @since_tizen 2.3
 * @param[in]   app_info   The application information of each installed application
 * @param[in]   user_data  The user data passed from the foreach function
 * @return      @c true to continue with the next iteration of the loop, \n
 *              otherwise @c false to break out of the loop
 * @pre app_manager_foreach_app_info() will invoke this callback.
 * @see app_manager_foreach_app_info()
 */
typedef bool (*app_info_filter_cb) (app_info_h app_info, void *user_data);

/**
 * @brief  Called to get application metadata in app_info_foreach_metadata().
 * @since_tizen 2.3
 * @param[in]   metadata_key   The key of the metadata
 * @param[in]   metadata_value  The value of the metadata
 * @param[in]   user_data       The user data passed to app_info_foreach_metadata()
 * @return      @c true to continue with the next iteration of the loop, \n
 *              otherwise @c false to break out of the loop
 * @pre app_info_foreach_metadata() will invoke this callback.
 * @see app_info_foreach_metadata()
 */
typedef bool (*app_info_metadata_cb) (const char *metadata_key, const char *metadata_value, void *user_data);

/**
 * @brief  Called for each application category in app_info_foreach_category().
 * @since_tizen 4.0
 * @remarks     @a category will be freed when the application information handle is destroyed
 *              using app_info_destroy()
 * @param[in]   category        The name of the category
 * @param[in]   user_data       The user data passed to app_info_foreach_category()
 * @return      @c true to continue with the next iteration of the loop, \n
 *              otherwise @c false to break out of the loop
 * @pre app_info_foreach_category() will invoke this callback.
 * @see app_info_foreach_category()
 */
typedef bool (*app_info_category_cb) (const char *category, void *user_data);

/**
 * @brief  Called for each application resource control in app_info_foreach_res_control().
 * @since_tizen 6.5
 * @remarks @a res_type, @a min_res_version, @a max_res_version, and @a auto_close are managed by the platform and will be released after the callback exits.
 * @param[in]   res_type         The resource type
 * @param[in]   min_res_version  The minimum version of the resource package to use
 * @param[in]   max_res_version  The maximum version of the resource package to use
 * @param[in]   auto_close       The value of auto close
 * @param[in]   user_data        The user data passed to app_info_foreach_res_control()
 * @return      @c true to continue with the next iteration of the loop, \n
 *              otherwise @c false to break out of the loop
 * @pre app_info_foreach_res_control() will invoke this callback.
 * @see app_info_foreach_res_control()
 */
typedef bool (*app_info_res_control_cb) (const char *res_type,
		const char *min_res_version, const char *max_res_version,
		const char *auto_close, void *user_data);

/**
 * @brief  Creates the application information handle.
 * @since_tizen 2.3
 * @remarks The @a app_info should be released using app_info_destroy().
 * @param[in]   app_id    The application ID
 * @param[out]  app_info  The application information handle
 * @return      @c 0 on success,
 *              otherwise a negative error value
 * @retval  #APP_MANAGER_ERROR_NONE               Successful
 * @retval  #APP_MANAGER_ERROR_INVALID_PARAMETER  Invalid parameter
 * @retval  #APP_MANAGER_ERROR_NO_SUCH_APP        The app is not installed
 * @retval  #APP_MANAGER_ERROR_OUT_OF_MEMORY      Out of memory
 * @see app_manager_foreach_app_info()
 * @see app_manager_get_app_info()
 */
int app_info_create(const char *app_id, app_info_h *app_info);

/**
 * @brief  Destroys the application information handle and releases all its resources.
 * @since_tizen 2.3
 * @param[in]   app_info  The application information handle
 * @return      @c 0 on success,
 *              otherwise a negative error value
 * @retval  #APP_MANAGER_ERROR_NONE               Successful
 * @retval  #APP_MANAGER_ERROR_INVALID_PARAMETER  Invalid parameter
 * @see app_manager_foreach_app_info()
 * @see app_manager_get_app_info()
 */
int app_info_destroy(app_info_h app_info);

/**
 * @brief  Gets the application ID with the given application context.
 * @since_tizen 2.3
 * @remarks     You must release @a app_id using free().
 * @param[in]   app_info  The application information
 * @param[out]  app_id    The application ID of the given application context
 * @return      @c 0 on success,
 *              otherwise a negative error value
 * @retval  #APP_MANAGER_ERROR_NONE               Successful
 * @retval  #APP_MANAGER_ERROR_INVALID_PARAMETER  Invalid parameter
 * @retval  #APP_MANAGER_ERROR_OUT_OF_MEMORY      Out of memory
 */
int app_info_get_app_id(app_info_h app_info, char **app_id);

/**
 * @brief  Gets the executable path of the application.
 * @since_tizen 2.3
 * @remarks     You must release @a exec path using free().
 * @param[in]   app_info The application information
 * @param[out]  exec     The executable path of the application
 * @return      @c 0 on success,
 *              otherwise a negative error value
 * @retval  #APP_MANAGER_ERROR_NONE               Successful
 * @retval  #APP_MANAGER_ERROR_INVALID_PARAMETER  Invalid parameter
 * @retval  #APP_MANAGER_ERROR_OUT_OF_MEMORY      Out of memory
 */
int app_info_get_exec(app_info_h app_info, char **exec);

/**
 * @brief  Gets the label of the application.
 * @since_tizen 2.3
 * @remarks     You must release @a label using free().
 * @param[in]   app_info  The application information
 * @param[out]  label     The label of the application
 * @return      @c 0 on success,
 *              otherwise a negative error value
 * @retval  #APP_MANAGER_ERROR_NONE               Successful
 * @retval  #APP_MANAGER_ERROR_INVALID_PARAMETER  Invalid parameter
 * @retval  #APP_MANAGER_ERROR_OUT_OF_MEMORY      Out of memory
 */
int app_info_get_label(app_info_h app_info, char **label);

/**
 * @brief  Gets the localed name of the application.
 * @since_tizen 2.3
 * @remarks     You must release localed name using free().
 * @param[in]   app_id  The application ID
 * @param[in]   locale  The locale information
 * @param[out]  label   The localed name of the application
 * @return      @c 0 on success,
 *              otherwise a negative error value
 * @retval  #APP_MANAGER_ERROR_NONE               Successful
 * @retval  #APP_MANAGER_ERROR_INVALID_PARAMETER  Invalid parameter
 * @retval  #APP_MANAGER_ERROR_OUT_OF_MEMORY      Out of memory
 */
int app_info_get_localed_label(const char *app_id, const char *locale, char **label);

/**
 * @brief  Gets the absolute path to the icon image.
 * @since_tizen 2.3
 * @remarks     You must release @a path using free().
 * @param[in]   app_info  The application information
 * @param[out]  path      The absolute path to the icon
 * @return      @c 0 on success,
 *              otherwise a negative error value
 * @retval  #APP_MANAGER_ERROR_NONE               Successful
 * @retval  #APP_MANAGER_ERROR_INVALID_PARAMETER  Invalid parameter
 * @retval  #APP_MANAGER_ERROR_OUT_OF_MEMORY      Out of memory
 */
int app_info_get_icon(app_info_h app_info, char **path);

/**
 * @brief  Gets the package name.
 * @since_tizen 2.3
 * @remarks     You must release @a package name using free().
 * @param[in]   app_info  The application information
 * @param[out]  package   The package name
 * @return      @c 0 on success,
 *              otherwise a negative error value
 * @retval  #APP_MANAGER_ERROR_NONE               Successful
 * @retval  #APP_MANAGER_ERROR_INVALID_PARAMETER  Invalid parameter
 * @retval  #APP_MANAGER_ERROR_OUT_OF_MEMORY      Out of memory
 */
int app_info_get_package(app_info_h app_info, char **package);

/**
 * @brief  Gets the package type name.
 * @since_tizen 2.3
 * @remarks     You must release package @a type name using free().
 * @param[in]   app_info The application information
 * @param[out]  type     The package type
 * @return      @c 0 on success,
 *              otherwise a negative error value
 * @retval  #APP_MANAGER_ERROR_NONE               Successful
 * @retval  #APP_MANAGER_ERROR_INVALID_PARAMETER  Invalid parameter
 * @retval  #APP_MANAGER_ERROR_OUT_OF_MEMORY      Out of memory
 */
int app_info_get_type(app_info_h app_info, char **type);

/**
 * @brief  Gets the application component type.
 * @since_tizen 4.0
 * @param[in]   app_info  The application information
 * @param[out]  type      The application component type
 * @return      @c 0 on success,
 *              otherwise a negative error value
 * @retval  #APP_MANAGER_ERROR_NONE               Successful
 * @retval  #APP_MANAGER_ERROR_INVALID_PARAMETER  Invalid parameter
 * @retval  #APP_MANAGER_ERROR_IO_ERROR           I/O error
 * @see #PACKAGE_INFO_PROP_APP_COMPONENT_TYPE
 * @see #app_info_app_component_type_e
 */
int app_info_get_app_component_type(app_info_h app_info, app_info_app_component_type_e *type);

/**
 * @brief  Gets the list of metadata for a particular application.
 * @since_tizen 2.3
 * @param[in]  app_info   The application information
 * @param[in]  callback   The callback function for list
 * @param[in]  user_data  The user data to be passed to callback function
 * @return      @c 0 on success,
 *              otherwise a negative error value
 * @retval  #APP_MANAGER_ERROR_NONE               Successful
 * @retval  #APP_MANAGER_ERROR_INVALID_PARAMETER  Invalid parameter
 * @retval  #APP_MANAGER_ERROR_IO_ERROR           I/O error
 */
int app_info_foreach_metadata(app_info_h app_info, app_info_metadata_cb callback, void *user_data);

/**
 * @brief  Checks whether application information is nodisplay.
 * @since_tizen 2.3
 * @param[in]   app_info   The application information
 * @param[out]  nodisplay  @c true if the application is nodisplay, \n
 *                         otherwise @c false
 * @return      @c 0 on success,
 *              otherwise a negative error value
 * @retval  #APP_MANAGER_ERROR_NONE               Successful
 * @retval  #APP_MANAGER_ERROR_INVALID_PARAMETER  Invalid parameter
 */
int app_info_is_nodisplay(app_info_h app_info, bool *nodisplay);

/**
 * @brief  Checks whether two application information are equal.
 * @since_tizen 2.3
 * @param[in]   lhs    The first application information to compare
 * @param[in]   rhs    The second application information to compare
 * @param[out]  equal  @c true if the application information are equal, \n
 *                     otherwise @c false
 * @return      @c 0 on success,
 *              otherwise a negative error value
 * @retval  #APP_MANAGER_ERROR_NONE               Successful
 * @retval  #APP_MANAGER_ERROR_INVALID_PARAMETER  Invalid parameter
 */
int app_info_is_equal(app_info_h lhs, app_info_h rhs, bool *equal);

/**
 * @brief  Checks whether application is enabled.
 * @since_tizen 2.3
 * @param[in]   app_info  The application information
 * @param[out]  enabled   @c true if the application is enabled, \n
 *                        otherwise @c false
 * @return      @c 0 on success,
 *              otherwise a negative error value
 * @retval  #APP_MANAGER_ERROR_NONE               Successful
 * @retval  #APP_MANAGER_ERROR_INVALID_PARAMETER  Invalid parameter
 */
int app_info_is_enabled(app_info_h app_info, bool *enabled);

/**
 * @brief  Checks whether application is launched on booting time.
 * @since_tizen 2.3
 * @param[in]   app_info  The application information
 * @param[out]  onboot    @c true if the application is launched on booting time, \n
 *                        otherwise @c false
 * @return      @c 0 on success,
 *              otherwise a negative error value
 * @retval  #APP_MANAGER_ERROR_NONE               Successful
 * @retval  #APP_MANAGER_ERROR_INVALID_PARAMETER  Invalid parameter
 */
int app_info_is_onboot(app_info_h app_info, bool *onboot);

/**
 * @brief  Checks whether application is preloaded.
 * @since_tizen 2.3
 * @param[in]   app_info  The application information
 * @param[out]  preload   @c true if the application is preloaded, \n
 *                        otherwise @c false
 * @return        @c 0 on success,
 *                otherwise a negative error value
 * @retval  #APP_MANAGER_ERROR_NONE               Successful
 * @retval  #APP_MANAGER_ERROR_INVALID_PARAMETER  Invalid parameter
 */
int app_info_is_preload(app_info_h app_info, bool *preload);

/**
 * @brief  Checks whether the application supports ambient mode.
 * @since_tizen 5.5
 * @param[in]   app_info         The application information
 * @param[out]  ambient_supported  @c true if the application supports ambient mode, \n
 *              otherwise @c false
 * @return      @c 0 on success,
 *              otherwise a negative error value
 * @retval  #APP_MANAGER_ERROR_NONE               Successful
 * @retval  #APP_MANAGER_ERROR_INVALID_PARAMETER  Invalid parameter
 */
int app_info_is_support_ambient(app_info_h app_info, bool *ambient_supported);

/**
 * @brief  Clones the application information handle.
 * @since_tizen 2.3
 * @remarks The @a clone should be released using app_info_destroy().
 * @param[out]  clone     A newly created application information handle, if successfully cloned
 * @param[in]   app_info  The application information
 * @return      @c 0 on success,
 *              otherwise a negative error value
 * @retval  #APP_MANAGER_ERROR_NONE               Successful
 * @retval  #APP_MANAGER_ERROR_INVALID_PARAMETER  Invalid parameter
 * @retval  #APP_MANAGER_ERROR_OUT_OF_MEMORY      Out of memory
 * @retval  #APP_MANAGER_ERROR_NO_SUCH_APP        The app is not installed
 */
int app_info_clone(app_info_h *clone, app_info_h app_info);

/**
 * @platform
 * @brief  Runs a callback for each category which the given application belongs to.
 * @since_tizen 4.0
 * @privlevel platform
 * @privilege %http://tizen.org/privilege/packagemanager.admin
 * @param[in]  app_info   The application information
 * @param[in]  callback   The callback function
 * @param[in]  user_data  The user data to be passed to the callback function
 * @return      @c 0 on success,
 *              otherwise a negative error value
 * @retval  #APP_MANAGER_ERROR_NONE               Successful
 * @retval  #APP_MANAGER_ERROR_PERMISSION_DENIED  Permission denied
 * @retval  #APP_MANAGER_ERROR_INVALID_PARAMETER  Invalid parameter
 * @retval  #APP_MANAGER_ERROR_IO_ERROR           I/O error
 * @see app_info_create()
 */
int app_info_foreach_category(app_info_h app_info, app_info_category_cb callback, void *user_data);

/**
 * @brief  Creates the application information filter handle from DB.
 *         All filter properties will be ANDed.
 * @since_tizen 2.3
 * @remarks The @a handle should be released using app_info_filter_destroy().
 * @param[out]  handle  The pointer to the application info filter handle
 * @return      @c 0 on success,
 *              otherwise a negative error value
 * @retval  #APP_MANAGER_ERROR_NONE               Successful
 * @retval  #APP_MANAGER_ERROR_INVALID_PARAMETER  Invalid parameter
 * @retval  #APP_MANAGER_ERROR_OUT_OF_MEMORY      Out of memory
 * @retval  #APP_MANAGER_ERROR_IO_ERROR           I/O error
 */
int app_info_filter_create(app_info_filter_h *handle);

/**
 * @brief  Destroys the application information filter handle, freeing up all the resources.
 * @since_tizen 2.3
 * @param[in]  handle  The pointer to the application info filter handle
 * @return     @c 0 on success,
 *             otherwise a negative error value
 * @retval  #APP_MANAGER_ERROR_NONE               Successful
 * @retval  #APP_MANAGER_ERROR_INVALID_PARAMETER  Invalid parameter
 * @retval  #APP_MANAGER_ERROR_IO_ERROR           I/O error
 */
int app_info_filter_destroy(app_info_filter_h handle);

/**
 * @brief  Adds a boolean filter property to the filter handle.
 * @since_tizen 2.3
 * @param[in]  handle    The pointer to the application info filter handle
 * @param[in]  property  The integer property name
 * @param[in]  value     The value corresponding to the property
 * @return     @c 0 on success,
 *             otherwise a negative error value
 * @retval  #APP_MANAGER_ERROR_NONE               Successful
 * @retval  #APP_MANAGER_ERROR_INVALID_PARAMETER  Invalid parameter
 * @retval  #APP_MANAGER_ERROR_IO_ERROR           I/O error
 */
int app_info_filter_add_bool(app_info_filter_h handle, const char *property, const bool value);

/**
 * @brief  Adds a string filter property to the filter handle.
 * @since_tizen 2.3
 * @param[in]  handle    The pointer to the application info filter handle
 * @param[in]  property  The integer property name
 * @param[in]  value     The value corresponding to the property
 * @return     @c 0 on success,
 *             otherwise a negative error value
 * @retval  #APP_MANAGER_ERROR_NONE               Successful
 * @retval  #APP_MANAGER_ERROR_INVALID_PARAMETER  Invalid parameter
 * @retval  #APP_MANAGER_ERROR_IO_ERROR           I/O error
 */
int app_info_filter_add_string(app_info_filter_h handle, const char *property, const char *value);

/**
 * @brief  Gets the count of filtered apps.
 * @since_tizen 2.3
 * @param[in]   handle  The pointer to the application info filter handle
 * @param[out]  count   The pointer to the variable for count
 * @return      @c 0 on success,
 *              otherwise a negative error value
 * @retval  #APP_MANAGER_ERROR_NONE               Successful
 * @retval  #APP_MANAGER_ERROR_INVALID_PARAMETER  Invalid parameter
 * @retval  #APP_MANAGER_ERROR_IO_ERROR           I/O error
 */
int app_info_filter_count_appinfo(app_info_filter_h handle, int *count);

/**
 * @brief  Executes the user supplied callback function for each application that satisfy the filter conditions.
 * @since_tizen 2.3
 * @param[in]  handle     The pointer to the application info filter handle
 * @param[in]  callback   The callback function
 * @param[in]  user_data  The user data to be passed to the callback function
 * @return     @c 0 on success,
 *             otherwise a negative error value
 * @retval  #APP_MANAGER_ERROR_NONE               Successful
 * @retval  #APP_MANAGER_ERROR_INVALID_PARAMETER  Invalid parameter
 * @retval  #APP_MANAGER_ERROR_IO_ERROR           I/O error
 */
int app_info_filter_foreach_appinfo(app_info_filter_h handle, app_info_filter_cb callback, void *user_data);

/**
 * @brief  Creates the application's metadata information filter handle from DB.
 * @since_tizen 2.3
 * @remarks The @a handle should be released using app_info_metadata_filter_destroy().
 * @param[out]  handle  The pointer to the application metadata info filter handle
 * @return      @c 0 on success,
 *              otherwise a negative error value
 * @retval  #APP_MANAGER_ERROR_NONE               Successful
 * @retval  #APP_MANAGER_ERROR_INVALID_PARAMETER  Invalid parameter
 * @retval  #APP_MANAGER_ERROR_OUT_OF_MEMORY      Out of memory
 * @retval  #APP_MANAGER_ERROR_IO_ERROR           I/O error
 */
int app_info_metadata_filter_create(app_info_metadata_filter_h *handle);

/**
 * @brief  Destroys the application's metadata information filter handle.
 * @since_tizen 2.3
 * @param[in]  handle  The pointer to the application info filter handle
 * @return  @c 0 on success,
 *          otherwise a negative error value
 * @retval  #APP_MANAGER_ERROR_NONE               Successful
 * @retval  #APP_MANAGER_ERROR_INVALID_PARAMETER  Invalid parameter
 * @retval  #APP_MANAGER_ERROR_IO_ERROR           I/O error
 */
int app_info_metadata_filter_destroy(app_info_metadata_filter_h handle);

/**
 * @brief  Adds filter condition for the query API.
 * @details  The query will search the entire application metadata information collected from
 *           the manifest file of all the installed packages. You can specify value as @c NULL to search based on key only.
 * @since_tizen 2.3
 * @param[in]  handle  The pointer to the application metadata info filter handle
 * @param[in]  key     The pointer to metadata key
 * @param[in]  value   The pointer to metadata value
 * @return     @c 0 on success,
 *             otherwise a negative error value
 * @retval  #APP_MANAGER_ERROR_NONE               Successful
 * @retval  #APP_MANAGER_ERROR_INVALID_PARAMETER  Invalid parameter
 * @retval  #APP_MANAGER_ERROR_IO_ERROR           I/O error
 */
int app_info_metadata_filter_add(app_info_metadata_filter_h handle, const char *key, const char *value);

/**
 * @brief  Executes the filter query.
 * @details  The query will search the entire application metadata information collected from
 *           the manifest file of all the installed packages. For each application returned by the query, the callback will be called. If callback returns
 *           negative value, no more callbacks will be called and API will return.
 * @since_tizen 2.3
 * @param[in]  handle     The pointer to the application metadata info filter handle
 * @param[in]  callback   The function pointer to callback
 * @param[in]  user_data  The pointer to user data
 * @return     @c 0 on success,
 *             otherwise a negative error value
 * @retval  #APP_MANAGER_ERROR_NONE               Successful
 * @retval  #APP_MANAGER_ERROR_INVALID_PARAMETER  Invalid parameter
 * @retval  #APP_MANAGER_ERROR_IO_ERROR           I/O error
 */
int app_info_metadata_filter_foreach(app_info_metadata_filter_h handle, app_info_filter_cb callback, void *user_data);

/**
 * @brief  Gets the list of resource controls for a particular application.
 * @details If a application has declared @b res_control and there is an available resource allowed by the resource package, the application uses the allowed resource of highest @b res_version of resource package among them.
 *          If a application has declared @b res_control and there is an available resource package, the application uses the global resource of highest @b res_version of resource package among them.
 *          The allowed resource can be accessed at @b {rootpath}/mount/allowed/{res_type} and the global resource can be accessed at @b {rootpath}/mount/global/{res_type}
 *          If the application declared @b res_control with @b auto_close 'true' and the resource package which the application is using is updated, the application will be terminated.
 *
 *          If an application 'appA' declared @b res_control like below *
 *            <table>
 *            <tr>
 *             <th> res_type </th>
 *             <th> min_res_version </th>
 *             <th> max_res_version </th>
 *            </tr>
 *            <tr>
 *             <td> ai_model </td>
 *             <td> 1.0.0 </td>
 *             <td> 2.0.0 </td>
 *            </tr>
 *            </table>
 *          and there are resource packages with res_type 'ai_model' like below
 *            <table>
 *            <tr>
 *             <th> package </th>
 *             <th> res_type </th>
 *             <th> res_version </th>
 *             <th> allows 'appA'? </th>
 *            </tr>
 *            <tr>
 *             <td> ai_1 </td>
 *             <td> ai_model </td>
 *             <td> 0.0.1 </td>
 *             <td> yes </td>
 *            </tr>
 *            <tr>
 *             <td> ai_2 </td>
 *             <td> ai_model </td>
 *             <td> 1.0.0 </td>
 *             <td> yes </td>
 *            </tr>
 *            <tr>
 *             <td> ai_3 </td>
 *             <td> ai_model </td>
 *             <td> 2.0.0 </td>
 *             <td> no </td>
 *            </tr>
 *            </table>
 *          'ai_1' package's @b res_version is lower than 'appA's @b min_res_version in @b res_control so 'ai_1' is not available.
 *          'ai_2' is available and allows 'appA' and 'ai_3' is available but doesn't allow 'appA'
 *          In this situation 'appA' can access the allowed resource of 'ai_2' and the global resource of 'ai_3'
 *
 * @since_tizen 6.5
 * @param[in]  app_info   The application information
 * @param[in]  callback   The callback function
 * @param[in]  user_data  The user data to be passed to the callback function
 * @return      @c 0 on success,
 *              otherwise a negative error value
 * @retval  #APP_MANAGER_ERROR_NONE               Successful
 * @retval  #APP_MANAGER_ERROR_INVALID_PARAMETER  Invalid parameter
 * @retval  #APP_MANAGER_ERROR_IO_ERROR           I/O error
 */
int app_info_foreach_res_control(app_info_h app_info, app_info_res_control_cb callback, void *user_data);


/**
* @}
*/

#ifdef __cplusplus
}
#endif

#endif /* __TIZEN_APPFW_APP_INFO_H */

