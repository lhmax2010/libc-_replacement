/*
 * Copyright (c) 2011 Samsung Electronics Co., Ltd All Rights Reserved
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


#ifndef __TIZEN_APPFW_PACKAGE_INFO_H
#define __TIZEN_APPFW_PACKAGE_INFO_H

#include <tizen.h>
#include <sys/types.h>

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @file package_info.h
 */

/**
 * @addtogroup CAPI_PACKAGE_INFO_MODULE
 * @{
 */


/**
 * @brief The package information handle.
 * @since_tizen 2.3
 */
typedef struct package_info_s *package_info_h;

/**
 * @brief The package update information handle.
 * @since_tizen 4.0
 */
typedef struct package_updateinfo_s *package_updateinfo_h;

/**
 * @brief The required privilege handle.
 * @since_tizen 6.5
 * @see package_info_foreach_required_privilege()
 */
typedef struct allowed_package_required_privilege_s *allowed_package_required_privilege_h;

/**
 * @brief Enumeration for storage type.
 * @since_tizen 2.3
 */
typedef enum {
	PACKAGE_INFO_INTERNAL_STORAGE = 0,    /**< Internal storage */
	PACKAGE_INFO_EXTERNAL_STORAGE = 1,    /**< External storage */
	PACKAGE_INFO_EXTENDED_STORAGE = 2,    /**< Extended storage (Since 5.0) */
} package_info_installed_storage_type_e;


/**
 * @brief Enumeration for app component type.
 * @since_tizen 2.3
 */
typedef enum {
	PACKAGE_INFO_ALLAPP = 0,                                /**< All applications
          (Deprecated since 5.5, use #PACKAGE_INFO_APP_COMPONENT_TYPE_ALL instead) */
	PACKAGE_INFO_UIAPP = 1,                                 /**< UI application
          (Deprecated since 5.5, use #PACKAGE_INFO_APP_COMPONENT_TYPE_UI instead) */
	PACKAGE_INFO_SERVICEAPP = 2,                            /**< Service application
          (Deprecated since 5.5, use #PACKAGE_INFO_APP_COMPONENT_TYPE_SERVICE instead) */
	PACKAGE_INFO_WIDGETAPP = 3,                             /**< Widget application (Since 4.0)
          (Deprecated since 5.5, use #PACKAGE_INFO_APP_COMPONENT_TYPE_WIDGET instead) */
	PACKAGE_INFO_WATCHAPP = 4,                              /**< Watch application (Since 4.0)
          (Deprecated since 5.5, use #PACKAGE_INFO_APP_COMPONENT_TYPE_WATCH instead) */

	PACKAGE_INFO_APP_COMPONENT_TYPE_ALL = 0,                /**< All applications (Since 5.5) */
	PACKAGE_INFO_APP_COMPONENT_TYPE_UI = 1,                 /**< UI application (Since 5.5) */
	PACKAGE_INFO_APP_COMPONENT_TYPE_SERVICE = 2,            /**< Service application (Since 5.5) */
	PACKAGE_INFO_APP_COMPONENT_TYPE_WIDGET = 3,             /**< Widget application (Since 5.5) */
	PACKAGE_INFO_APP_COMPONENT_TYPE_WATCH = 4,              /**< Watch application (Since 5.5) */
	PACKAGE_INFO_APP_COMPONENT_TYPE_COMPONENT_BASED = 5,    /**< Component-based application (Since 5.5) */
} package_info_app_component_type_e;


/**
 * @brief Enumeration for certification type.
 * @since_tizen 2.3
 */
typedef enum {
	PACKAGE_INFO_AUTHOR_ROOT_CERT = 0,               /**< Author Root Certificate*/
	PACKAGE_INFO_AUTHOR_INTERMEDIATE_CERT = 1,       /**< Author Intermediate Certificate*/
	PACKAGE_INFO_AUTHOR_SIGNER_CERT = 2,             /**< Author Signer Certificate*/
	PACKAGE_INFO_DISTRIBUTOR_ROOT_CERT = 3,          /**< Distributor Root Certificate*/
	PACKAGE_INFO_DISTRIBUTOR_INTERMEDIATE_CERT = 4,  /**< Distributor Intermediate Certificate*/
	PACKAGE_INFO_DISTRIBUTOR_SIGNER_CERT = 5,        /**< Distributor Signer Certificate*/
	PACKAGE_INFO_DISTRIBUTOR2_ROOT_CERT = 6,         /**< Distributor2 Root Certificate*/
	PACKAGE_INFO_DISTRIBUTOR2_INTERMEDIATE_CERT = 7, /**< Distributor2 Intermediate Certificate*/
	PACKAGE_INFO_DISTRIBUTOR2_SIGNER_CERT = 8,       /**< Distributor2 Signer Certificate*/
} package_cert_type_e;

/**
 * @brief Enumeration for package update info type.
 * @since_tizen 4.0
 */
typedef enum {
	PACKAGE_UPDATEINFO_TYPE_NONE = 0,   /**< None type */
	PACKAGE_UPDATEINFO_TYPE_FORCE,      /**< Force type */
	PACKAGE_UPDATEINFO_TYPE_OPTIONAL    /**< Optional type */
} package_updateinfo_type_e;

/**
 * @brief Called for each update information of all packages.
 * @since_tizen 4.0
 * @remarks @a info should not be freed and can be used only in the callback.
 * @param[in] info       The package update information handle
 * @param[in] user_data  The user data passed from the foreach function
 * @return @c true to continue with the next iteration of the loop,
 *         otherwise @c false to break out of the loop
 * @pre package_info_updateinfo_foreach_info() will invoke this callback.
 * @see package_info_updateinfo_foreach_info()
 */
typedef bool (*package_info_updateinfo_cb) (package_updateinfo_h info, void *user_data);

/**
 * @brief Called to get the application ID once for each installed package.
 * @since_tizen 2.3
 * @param[in] comp_type The application component type
 * @param[in] app_id    The application ID.
 * @param[in] user_data The user data passed from the foreach function
 * @return @c true to continue with the next iteration of the loop,
 *         otherwise @c false to break out of the loop
 * @pre package_info_foreach_app_from_package() will invoke this callback.
 * @see package_info_foreach_app_from_package()
 */
typedef bool (*package_info_app_cb) (package_info_app_component_type_e comp_type, const char *app_id, void *user_data);


/**
 * @brief Called to get the certification information.
 * @since_tizen 2.3
 * @param[in] handle       The package info handle
 * @param[in] cert_type    The certificate type
 * @param[in] cert_value   The certificate value of corresponding certificate key \n
 *                         This value is base64 encoded data.
 * @param[in] user_data    The user data passed from the foreach function
 * @return @c true to continue with the next iteration of the loop,
 *         otherwise @c false to break out of the loop
 * @pre package_info_foreach_cert_info() will invoke this callback.
 * @see package_info_foreach_cert_info()
 */
typedef bool (*package_info_cert_info_cb) (package_info_h handle, package_cert_type_e cert_type, const char *cert_value, void *user_data);


/**
 * @brief Called to get the privilege information.
 * @since_tizen 2.3
 * @param[in] privilege_name the name of the privilege
 * @param[in] user_data    The user data passed from the foreach function
 * @return  @c true to continue with the next iteration of the loop,
 *          otherwise @c false to break out of the loop
 * @pre package_info_foreach_privilege_info() will invoke this callback.
 * @see package_info_foreach_privilege_info()
 */
typedef bool (*package_info_privilege_info_cb) (const char *privilege_name, void *user_data);


/**
 * @brief Called when dependency information between packages is retrieved.
 * @since_tizen 5.5
 * @remarks @a from, @a to, @a type and @a required_version are managed by the platform and will be released after the callback exits.
 * @param[in] from             The ID of package that depends on another
 * @param[in] to               The ID of package that is required by another
 * @param[in] type             The type of dependency
 * @param[in] required_version The required version
 * @param[in] user_data        The user data passed from the foreach function
 * @return  @c true to continue with the next iteration of the loop,
 *          otherwise @c false to break out of the loop
 * @pre package_info_foreach_dependency_info() will invoke this callback.
 * @pre package_info_foreach_dependency_info_depends_on() will invoke this callback.
 * @see package_info_foreach_dependency_info()
 * @see package_info_foreach_dependency_info_depends_on()
 */
typedef bool (*package_info_dependency_info_cb) (const char *from, const char *to, const char *type, const char *required_version, void *user_data);


/**
 * @brief Called to get the allowed package information of resource package.
 * @since_tizen 6.5
 * @remarks @a allowed_package, @a privilege_handle are managed by the platform and will be released after the callback exits.
 * @param[in] allowed_package  The ID of allowed package
 * @param[in] privilege_handle The privilege handle required by resource package
 * @param[in] user_data        The user data passed from the foreach function
 * @return  @c true to continue with the next iteration of the loop,
 *          otherwise @c false to break out of the loop
 * @pre package_info_foreach_res_allowed_package() will invoke this callback.
 * @see package_info_foreach_res_allowed_package()
 */
typedef bool (*package_info_res_allowed_package_cb) (const char *allowed_package, allowed_package_required_privilege_h privilege_handle, void *user_data);


/**
 * @brief Retrieves all application IDs of each package.
 * @since_tizen 2.3
 * @param[in] package_info  The package info handle
 * @param[in] comp_type     The application component type
 * @param[in] callback      The callback function to invoke
 * @param[in] user_data     The user data to be passed to the callback function
 * @return @c 0 on success,
 *         otherwise a negative error value
 * @retval #PACKAGE_MANAGER_ERROR_NONE              Successful
 * @retval #PACKAGE_MANAGER_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #PACKAGE_MANAGER_ERROR_NO_SUCH_PACKAGE   No such package
 * @post This function invokes package_info_app_cb() repeatedly for each package.
 * @see package_info_app_cb()
 */
int package_info_foreach_app_from_package(package_info_h package_info, package_info_app_component_type_e comp_type, package_info_app_cb callback, void *user_data);


/**
 * @brief Destroys the package information handle and releases all its resources.
 * @since_tizen 2.3
 * @param[in] package_info The package information handle
 * @return @c 0 on success,
 *         otherwise a negative error value
 * @retval #PACKAGE_MANAGER_ERROR_NONE              Successful
 * @retval #PACKAGE_MANAGER_ERROR_INVALID_PARAMETER Invalid parameter
 * @see package_manager_foreach_package_info()
 * @see package_manager_get_package_info()
 */
int package_info_destroy(package_info_h package_info);


/**
 * @brief Gets the package name.
 * @since_tizen 2.3
 * @remarks You must release @a package using free().
 * @param[in]  package_info The package information
 * @param[out] package      The package name
 * @return @c 0 on success,
 *         otherwise a negative error value
 * @retval #PACKAGE_MANAGER_ERROR_NONE              Successful
 * @retval #PACKAGE_MANAGER_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #PACKAGE_MANAGER_ERROR_OUT_OF_MEMORY     Out of memory
 */
int package_info_get_package(package_info_h package_info, char **package);


/**
 * @brief Gets the main application ID of the package.
 * @since_tizen 4.0
 * @remarks You must release @a main_app_id using free().
 * @param[in]  package_info     The package information
 * @param[out] main_app_id      The main application ID of the package
 * @return @c 0 on success,
 *         otherwise a negative error value
 * @retval #PACKAGE_MANAGER_ERROR_NONE              Successful
 * @retval #PACKAGE_MANAGER_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #PACKAGE_MANAGER_ERROR_OUT_OF_MEMORY     Out of memory
 * @retval #PACKAGE_MANAGER_ERROR_IO_ERROR          I/O error
 */
int package_info_get_main_app_id(package_info_h package_info, char **main_app_id);


/**
 * @brief Gets the label of the package.
 * @since_tizen 2.3
 * @remarks You must release @a label using free().
 * @param[in]  package_info The package information
 * @param[out] label        The label of the package
 * @return @c 0 on success,
 *         otherwise a negative error value
 * @retval #PACKAGE_MANAGER_ERROR_NONE              Successful
 * @retval #PACKAGE_MANAGER_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #PACKAGE_MANAGER_ERROR_OUT_OF_MEMORY     Out of memory
 * @retval #PACKAGE_MANAGER_ERROR_IO_ERROR          I/O error
 */
int package_info_get_label(package_info_h package_info, char **label);


/**
 * @brief Gets the absolute path to the icon image.
 * @since_tizen 2.3
 * @remarks You must release @a path using free().
 * @param[in]  package_info The package information
 * @param[out] path         The path of the package
 * @return @c 0 on success,
 *         otherwise a negative error value
 * @retval #PACKAGE_MANAGER_ERROR_NONE              Successful
 * @retval #PACKAGE_MANAGER_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #PACKAGE_MANAGER_ERROR_OUT_OF_MEMORY     Out of memory
 * @retval #PACKAGE_MANAGER_ERROR_IO_ERROR          I/O error
 */
int package_info_get_icon(package_info_h package_info, char **path);


/**
 * @brief Gets the version of the package.
 * @since_tizen 2.3
 * @remarks You must release @a version using free().
 * @param[in]  package_info The package information
 * @param[out] version      The version of the package
 * @return @c 0 on success,
 *         otherwise a negative error value
 * @retval #PACKAGE_MANAGER_ERROR_NONE              Successful
 * @retval #PACKAGE_MANAGER_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #PACKAGE_MANAGER_ERROR_OUT_OF_MEMORY     Out of memory
 * @retval #PACKAGE_MANAGER_ERROR_IO_ERROR          I/O error
 */
int package_info_get_version(package_info_h package_info, char **version);


/**
 * @brief Gets the type of the package.
 * @since_tizen 2.3
 * @remarks You must release @a type using free().
 * @param[in]  package_info The package information
 * @param[out] type         The type of the package
 * @return @c 0 on success,
 *         otherwise a negative error value
 * @retval #PACKAGE_MANAGER_ERROR_NONE              Successful
 * @retval #PACKAGE_MANAGER_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #PACKAGE_MANAGER_ERROR_OUT_OF_MEMORY     Out of memory
 * @retval #PACKAGE_MANAGER_ERROR_IO_ERROR          I/O error
 */
int package_info_get_type(package_info_h package_info, char **type);


/**
 * @brief Gets the installed storage for the given package.
 * @since_tizen 2.3
 * @param[in]  package_info The package information
 * @param[out] storage      The installed storage
 * @return @c 0 on success,
 *         otherwise a negative error value
 * @retval #PACKAGE_MANAGER_ERROR_NONE              Successful
 * @retval #PACKAGE_MANAGER_ERROR_INVALID_PARAMETER Invalid parameter
 */
int package_info_get_installed_storage(package_info_h package_info, package_info_installed_storage_type_e *storage);


/**
 * @brief Gets the root path of the package.
 * @since_tizen 2.3
 * @remarks You must release @a path using free().
 * @param[in]  package_info The package information
 * @param[out] path         The root path of the package
 * @return @c 0 on success,
 *         otherwise a negative error value
 * @retval #PACKAGE_MANAGER_ERROR_NONE              Successful
 * @retval #PACKAGE_MANAGER_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #PACKAGE_MANAGER_ERROR_OUT_OF_MEMORY     Out of memory
 * @retval #PACKAGE_MANAGER_ERROR_IO_ERROR          I/O error
 */
int package_info_get_root_path(package_info_h package_info, char **path);


/**
 * @platform
 * @brief Gets the name of the TEP (Tizen Expansion Package).
 * @since_tizen 2.4
 * @privlevel platform
 * @privilege %http://tizen.org/privilege/packagemanager.admin
 * @remarks Name must be released using free().
 * @param[in] package_info The package information
 * @param[out] name The name of the tep
 * @return 0 on success,
 *         otherwise a negative error value
 * @retval #PACKAGE_MANAGER_ERROR_NONE Successful
 * @retval #PACKAGE_MANAGER_ERROR_PERMISSION_DENIED Permission denied
 * @retval #PACKAGE_MANAGER_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #PACKAGE_MANAGER_ERROR_OUT_OF_MEMORY Out of memory
 * @retval #PACKAGE_MANAGER_ERROR_SYSTEM_ERROR Severe system error
 */
int package_info_get_tep_name(package_info_h package_info, char **name);


/**
 * @brief Gets the resource type of the package.
 * @since_tizen 6.5
 * @remarks You must release @a res_type using free().
 * @param[in]  package_info The package information
 * @param[out] res_type     The resource type of the package
 * @return @c 0 on success,
 *         otherwise a negative error value
 * @retval #PACKAGE_MANAGER_ERROR_NONE              Successful
 * @retval #PACKAGE_MANAGER_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #PACKAGE_MANAGER_ERROR_OUT_OF_MEMORY     Out of memory
 * @retval #PACKAGE_MANAGER_ERROR_IO_ERROR          I/O error
 */
int package_info_get_res_type(package_info_h package_info, char **res_type);


/**
 * @brief Gets the resource version of the package.
 * @since_tizen 6.5
 * @remarks You must release @a res_version using free().
 * @param[in]  package_info The package information
 * @param[out] res_version  The resource version of the package
 * @return @c 0 on success,
 *         otherwise a negative error value
 * @retval #PACKAGE_MANAGER_ERROR_NONE              Successful
 * @retval #PACKAGE_MANAGER_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #PACKAGE_MANAGER_ERROR_OUT_OF_MEMORY     Out of memory
 * @retval #PACKAGE_MANAGER_ERROR_IO_ERROR          I/O error
 */
int package_info_get_res_version(package_info_h package_info, char **res_version);


/**
 * @brief Checks whether the package is system package.
 * @since_tizen 2.3
 * @param[in]  package_info The package information
 * @param[out] system       @c true if the package is system package,
 *                          otherwise @c false if the package is not system package
 * @return @c 0 on success,
 *         otherwise a negative error value
 * @retval #PACKAGE_MANAGER_ERROR_NONE              Successful
 * @retval #PACKAGE_MANAGER_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #PACKAGE_MANAGER_ERROR_OUT_OF_MEMORY     Out of memory
 * @retval #PACKAGE_MANAGER_ERROR_IO_ERROR          I/O error
 */
int package_info_is_system_package(package_info_h package_info, bool *system);


/**
 * @brief Checks whether the package is removable.
 * @since_tizen 2.3
 * @param[in]  package_info The package information
 * @param[out] removable    The removable info of the package
 * @return @c 0 on success,
 *         otherwise a negative error value
 * @retval #PACKAGE_MANAGER_ERROR_NONE              Successful
 * @retval #PACKAGE_MANAGER_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #PACKAGE_MANAGER_ERROR_OUT_OF_MEMORY Out of memory
 * @retval #PACKAGE_MANAGER_ERROR_IO_ERROR IO error
 */
int package_info_is_removable_package(package_info_h package_info, bool *removable);


/**
 * @brief Checks whether the package is preloaded.
 * @since_tizen 2.3
 * @param[in]  package_info The package information
 * @param[out] preload      The preload info of the package
 * @return @c 0 on success,
 *         otherwise a negative error value
 * @retval #PACKAGE_MANAGER_ERROR_NONE              Successful
 * @retval #PACKAGE_MANAGER_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #PACKAGE_MANAGER_ERROR_OUT_OF_MEMORY     Out of memory
 * @retval #PACKAGE_MANAGER_ERROR_IO_ERROR          I/O error
 */
int package_info_is_preload_package(package_info_h package_info, bool *preload);


/**
 * @brief Checks whether two package information is equal.
 * @since_tizen 2.3
 * @param[in]  lhs    The first package information to be compared
 * @param[in]  rhs    The second package information to be compared
 * @param[out] equal  @c true if the package information are equal,
 *                    otherwise @c false if package information are not equal
 * @return @c 0 on success,
 *         otherwise a negative error value
 * @retval #PACKAGE_MANAGER_ERROR_NONE              Successful
 * @retval #PACKAGE_MANAGER_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #PACKAGE_MANAGER_ERROR_OUT_OF_MEMORY     Out of memory
 * @retval #PACKAGE_MANAGER_ERROR_IO_ERROR          Database error occurred
 */
int package_info_is_equal(package_info_h lhs, package_info_h rhs, bool *equal);


/**
 * @brief Checks whether the package info is accessible for the given package.
 * @since_tizen 2.3
 * @param[in]  package_info The package information
 * @param[out] accessible   @c true if the package info is accessible,
 *                          otherwise @c false if the package info is not accessible
 * @return @c 0 on success,
 *         otherwise a negative error value
 * @retval #PACKAGE_MANAGER_ERROR_NONE              Successful
 * @retval #PACKAGE_MANAGER_ERROR_INVALID_PARAMETER Invalid parameter
 */
int package_info_is_accessible(package_info_h package_info, bool *accessible);

/**
 * @brief Gets the first installed time for the given package.
 * @details If the package was created, the @a first_installed_time represents created time.
 *          So, the meaning of 'first installed time' corresponds with 'created time'.
 *          Even though package is updated, first_installed_time isn't changed except you
 *          delete package.
 * @since_tizen 10.0
 * @param[in]  package_info   The package information
 * @param[out] first_installed_time The value of time_t type for first installed time
 * @return @c 0 on success,
 *         otherwise a negative error value
 * @retval #PACKAGE_MANAGER_ERROR_NONE              Successful
 * @retval #PACKAGE_MANAGER_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #PACKAGE_MANAGER_ERROR_IO_ERROR          Database error occurred
 */
int package_info_get_first_installed_time(package_info_h package_info, time_t *first_installed_time);

/**
 * @brief Gets the installed time for the given package.
 * @details If the package was updated, the @a installed_time represents updated time.
 *          So, the meaning of 'installed time' corresponds with 'last modified time'.
 * @since_tizen 4.0
 * @param[in]  package_info   The package information
 * @param[out] installed_time The integer value of time_t type for installed time
 * @return @c 0 on success,
 *         otherwise a negative error value
 * @retval #PACKAGE_MANAGER_ERROR_NONE              Successful
 * @retval #PACKAGE_MANAGER_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #PACKAGE_MANAGER_ERROR_IO_ERROR          Database error occurred
 */
int package_info_get_installed_time(package_info_h package_info, int *installed_time);

/**
 * @brief Clones the package information handle.
 * @since_tizen 2.3
 * @remarks The @a clone should be released using package_info_destroy().
 * @param[out] clone          The newly created package information handle
 * @param[in]  package_info   The package information
 * @return @c 0 on success,
 *         otherwise a negative error value
 * @retval #PACKAGE_MANAGER_ERROR_NONE              Successful
 * @retval #PACKAGE_MANAGER_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #PACKAGE_MANAGER_ERROR_NO_SUCH_PACKAGE   The package is not installed
 * @retval #PACKAGE_MANAGER_ERROR_OUT_OF_MEMORY     Out of memory
 */
int package_info_clone(package_info_h *clone, package_info_h package_info);

/**
 * @brief Gets the package information for the given package.
 * @since_tizen 2.3
 * @remarks You must release @a package_info using package_info_destroy().
 * @param[in]  package      The ID of the package
 * @param[out] package_info The package information for the given package ID
 * @return @c 0 on success,
 *         otherwise a negative error value
 * @retval #PACKAGE_MANAGER_ERROR_NONE              Successful
 * @retval #PACKAGE_MANAGER_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #PACKAGE_MANAGER_ERROR_NO_SUCH_PACKAGE   The package is not installed
 * @retval #PACKAGE_MANAGER_ERROR_OUT_OF_MEMORY     Out of memory
 */
int package_info_create(const char *package, package_info_h *package_info);


/**
 * @brief Retrieves certification information of the package.
 * @since_tizen 2.3
 * @param[in] package_info The package information
 * @param[in] callback     The iteration callback function
 * @param[in] user_data    The user data to be passed to the callback function
 * @return @c 0 on success,
 *         otherwise a negative error value
 * @retval #PACKAGE_MANAGER_ERROR_NONE              Successful
 * @retval #PACKAGE_MANAGER_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #PACKAGE_MANAGER_ERROR_OUT_OF_MEMORY     Out of memory
 * @retval #PACKAGE_MANAGER_ERROR_IO_ERROR          I/O error
 */
int package_info_foreach_cert_info(package_info_h package_info, package_info_cert_info_cb callback, void *user_data);


/**
 * @brief Retrieves privilege information of the package.
 * @since_tizen 2.3
 * @param[in] package_info The package information
 * @param[in] callback     The iteration callback function
 * @param[in] user_data    The user data to be passed to the callback function
 * @return @c 0 on success,
 *         otherwise a negative error value
 * @retval #PACKAGE_MANAGER_ERROR_NONE              Successful
 * @retval #PACKAGE_MANAGER_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #PACKAGE_MANAGER_ERROR_OUT_OF_MEMORY     Out of memory
 * @retval #PACKAGE_MANAGER_ERROR_IO_ERROR          I/O error
 */
int package_info_foreach_privilege_info(package_info_h package_info, package_info_privilege_info_cb callback, void *user_data);

/**
 * @brief Retrieves dependencies between packages.
 * @details @a callback is called whenever there is package dependency. The callback includes packages that are only directly required by the given package.
 *          For example, if there are packages having the following relationship:
 *
 *                B --> E
 *          A --> B --> C
 *                D --> C
 *
 *          A --> B means that A depends on B.
 *          When package_info_foreach_dependency_info(A) is called, the callback results in two parameters: @b from is A and @b to is B.
 * @since_tizen 5.5
 * @remarks The function provides the results synchronously. If there are no dependencies, this function will return #PACKAGE_MANAGER_ERROR_NONE immediately and the callback will not be invoked.
 * @param[in] package_info The package information
 * @param[in] callback     The iteration callback function
 * @param[in] user_data    The user data to be passed to the callback function
 * @return @c 0 on success,
 *         otherwise a negative error value
 * @retval #PACKAGE_MANAGER_ERROR_NONE              Successful
 * @retval #PACKAGE_MANAGER_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #PACKAGE_MANAGER_ERROR_SYSTEM_ERROR      Severe system error
 */
int package_info_foreach_dependency_info(package_info_h package_info, package_info_dependency_info_cb callback, void *user_data);

/**
 * @brief Retrieves which packages depend on the given package.
 * @details @a callback is called whenever there is package dependency. The callback includes packages that are both directly and indirectly depend on the given package.
 *          For example, if there are packages having the following relationship:
 *
 *                B --> E
 *          A --> B --> C
 *                D --> C
 *
 *          A --> B means that A depends on B.
 *          When package_info_foreach_dependency_info(C) is called, the callback results in two parameters: @b from is B and @b to is C, @b from is D and @b to is C, @b from is A and @b to is B.
 * @since_tizen 5.5
 * @remarks The function provides the results synchronously. If there are no dependencies, this function will return #PACKAGE_MANAGER_ERROR_NONE immediately and the callback will not be invoked.
 * @param[in] package_info The package information
 * @param[in] callback     The iteration callback function
 * @param[in] user_data    The user data to be passed to the callback function
 * @return @c 0 on success,
 *         otherwise a negative error value
 * @retval #PACKAGE_MANAGER_ERROR_NONE              Successful
 * @retval #PACKAGE_MANAGER_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #PACKAGE_MANAGER_ERROR_SYSTEM_ERROR      Severe system error
 */
int package_info_foreach_dependency_info_depends_on(package_info_h package_info, package_info_dependency_info_cb callback, void *user_data);


/**
 * @brief Retrieves allowed package information of the resource package.
 * @details Resource packages allow access to allowed data only for allowed packages that have specific privileges.
 *          If not, they just allow access to global data
 * @since_tizen 6.5
 * @param[in] package_info The package information
 * @param[in] callback     The iteration callback function
 * @param[in] user_data    The user data to be passed to the callback function
 * @return @c 0 on success,
 *         otherwise a negative error value
 * @retval #PACKAGE_MANAGER_ERROR_NONE              Successful
 * @retval #PACKAGE_MANAGER_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #PACKAGE_MANAGER_ERROR_IO_ERROR          I/O error
 * @retval #PACKAGE_MANAGER_ERROR_SYSTEM_ERROR      Severe system error
 */
int package_info_foreach_res_allowed_package(package_info_h package_info, package_info_res_allowed_package_cb callback, void *user_data);


/**
 * @brief Retrieves required privilege information of the allowed package.
 * @details The packages want to access in the allowed area of resource package must have all the privileges required by resource package.
 * @since_tizen 6.5
 * @param[in] priv_info    The privilege information handle
 * @param[in] callback     The iteration callback function
 * @param[in] user_data    The user data to be passed to the callback function
 * @return @c 0 on success,
 *         otherwise a negative error value
 * @retval #PACKAGE_MANAGER_ERROR_NONE              Successful
 * @retval #PACKAGE_MANAGER_ERROR_INVALID_PARAMETER Invalid parameter
 */
int package_info_foreach_required_privilege(allowed_package_required_privilege_h priv_info, package_info_privilege_info_cb callback, void *user_data);


/**
 * @brief Gets the package update information for the given package.
 * @since_tizen 4.0
 * @remarks You must release @a info using package_info_updateinfo_destroy().
 * @param[in]  pkgid       The ID of the package
 * @param[out] info        The package update information for the given package ID
 * @return @c 0 on success,
 *         otherwise a negative error value
 * @retval #PACKAGE_MANAGER_ERROR_NONE              Successful
 * @retval #PACKAGE_MANAGER_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #PACKAGE_MANAGER_ERROR_NO_SUCH_PACKAGE   The package is not installed
 * @retval #PACKAGE_MANAGER_ERROR_OUT_OF_MEMORY     Out of memory
 * @retval #PACKAGE_MANAGER_ERROR_IO_ERROR          Database error occurred
 * @see package_info_updateinfo_destroy()
 */
int package_info_updateinfo_create(const char *pkgid, package_updateinfo_h *info);

/**
 * @brief Gets the package name.
 * @since_tizen 4.0
 * @remarks You must release @a package using free().
 * @param[in]  info        The package update information
 * @param[out] package     The package name
 * @return @c 0 on success,
 *         otherwise a negative error value
 * @retval #PACKAGE_MANAGER_ERROR_NONE              Successful
 * @retval #PACKAGE_MANAGER_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #PACKAGE_MANAGER_ERROR_OUT_OF_MEMORY     Out of memory
 * @retval #PACKAGE_MANAGER_ERROR_IO_ERROR          Internal I/O error occurred
 */
int package_info_updateinfo_get_pkgid(package_updateinfo_h info, char **package);

/**
 * @brief Gets the update version of package.
 * @since_tizen 4.0
 * @remarks You must release @a version using free().
 * @param[in]  info        The package update information
 * @param[out] version     The package update version
 * @return @c 0 on success,
 *         otherwise a negative error value
 * @retval #PACKAGE_MANAGER_ERROR_NONE              Successful
 * @retval #PACKAGE_MANAGER_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #PACKAGE_MANAGER_ERROR_OUT_OF_MEMORY     Out of memory
 * @retval #PACKAGE_MANAGER_ERROR_IO_ERROR          Internal I/O error occurred
 */
int package_info_updateinfo_get_version(package_updateinfo_h info, char **version);

/**
 * @brief Gets the update type of package.
 * @since_tizen 4.0
 * @param[in]  info        The package update information
 * @param[out] type        The package update type
 * @return @c 0 on success,
 *         otherwise a negative error value
 * @retval #PACKAGE_MANAGER_ERROR_NONE              Successful
 * @retval #PACKAGE_MANAGER_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #PACKAGE_MANAGER_ERROR_IO_ERROR          Internal I/O error occurred
 */
int package_info_updateinfo_get_type(package_updateinfo_h info,
						package_updateinfo_type_e *type);

/**
 * @brief Destroys the package update information handle and releases all its resources.
 * @since_tizen 4.0
 * @param[in] info         The package update information handle
 * @return @c 0 on success,
 *         otherwise a negative error value
 * @retval #PACKAGE_MANAGER_ERROR_NONE              Successful
 * @retval #PACKAGE_MANAGER_ERROR_INVALID_PARAMETER Invalid parameter
 * @see package_info_updateinfo_create()
 */
int package_info_updateinfo_destroy(package_updateinfo_h info);

/**
 * @brief Retrieves update information of all packages and invoke callback for each of it.
 * @since_tizen 4.0
 * @param[in] callback     The iteration callback function
 * @param[in] user_data    The user data to be passed to the callback function
 * @return @c 0 on success,
 *         otherwise a negative error value
 * @retval #PACKAGE_MANAGER_ERROR_NONE              Successful
 * @retval #PACKAGE_MANAGER_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #PACKAGE_MANAGER_ERROR_IO_ERROR          I/O error
 */
int package_info_updateinfo_foreach_info(package_info_updateinfo_cb callback, void *user_data);


/**
* @}
*/

#ifdef __cplusplus
}
#endif

#endif /* __TIZEN_APPFW_PACKAGE_INFO_H */
