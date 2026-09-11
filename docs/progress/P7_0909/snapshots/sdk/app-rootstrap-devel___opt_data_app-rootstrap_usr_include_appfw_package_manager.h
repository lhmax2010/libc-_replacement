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

#ifndef __TIZEN_APPFW_PACKAGE_MANAGER_H
#define __TIZEN_APPFW_PACKAGE_MANAGER_H

#include <sys/types.h>

#include <tizen.h>

#include <package_info.h>

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @file package_manager.h
 */

/**
 * @addtogroup CAPI_PACKAGE_MANAGER_MODULE
 * @{
 */

/**
 * @brief Definition for value to be used when filtering based on install location: Boolean property for filtering whether the package is removable or not based on package info.
 * @since_tizen 2.3
 */
#define PACKAGE_MANAGER_PKGINFO_PROP_REMOVABLE       "PMINFO_PKGINFO_PROP_PACKAGE_REMOVABLE"


/**
 * @brief Definition for value to be used when filtering based on install location: Boolean property for filtering whether the package is readonly or not based on package info.
 * @since_tizen 2.3
 */
#define PACKAGE_MANAGER_PKGINFO_PROP_READONLY        "PMINFO_PKGINFO_PROP_PACKAGE_READONLY"


/**
 * @brief Definition for value to be used when filtering based on install location: Boolean property for filtering whether the package supports disable or not based on package info.
 * @since_tizen 2.3
 */
#define PACKAGE_MANAGER_PKGINFO_PROP_SUPPORT_DISABLE     "PMINFO_PKGINFO_PROP_PACKAGE_SUPPORT_DISABLE"


/**
 * @brief Definition for value to be used when filtering based on install location: Boolean property for filtering whether the package is disabled based on package info.
 * @since_tizen 2.3
 */
#define PACKAGE_MANAGER_PKGINFO_PROP_DISABLE             "PMINFO_PKGINFO_PROP_PACKAGE_DISABLE"


/**
 * @brief Definition for value to be used when filtering based on install location: Boolean property for filtering whether the package is preload or not based on package info.
 * @since_tizen 2.3
 */
#define PACKAGE_MANAGER_PKGINFO_PROP_PRELOAD		"PMINFO_PKGINFO_PROP_PACKAGE_PRELOAD"


/**
 * @brief Definition for value to be used when filtering based on resource type: String property for filtering packages with specific resource types.
 * @since_tizen 6.5
 */
#define PACKAGE_MANAGER_PKGINFO_PROP_RES_TYPE		"PMINFO_PKGINFO_PROP_PACKAGE_RES_TYPE"


/**
 * @brief Definition for value to be used when filtering based on package type: String property for filtering packages with specific package types.
 * @since_tizen 6.5
 */
#define PACKAGE_MANAGER_PKGINFO_PROP_TYPE		"PMINFO_PKGINFO_PROP_PACKAGE_TYPE"


/**
 * @brief Enumeration for error code.
 * @since_tizen 2.3
 */
typedef enum {
	PACKAGE_MANAGER_ERROR_NONE = TIZEN_ERROR_NONE,                                /**< Successful */
	PACKAGE_MANAGER_ERROR_INVALID_PARAMETER = TIZEN_ERROR_INVALID_PARAMETER,      /**< Invalid parameter */
	PACKAGE_MANAGER_ERROR_OUT_OF_MEMORY = TIZEN_ERROR_OUT_OF_MEMORY,              /**< Out of memory */
	PACKAGE_MANAGER_ERROR_IO_ERROR = TIZEN_ERROR_IO_ERROR,                        /**< Internal I/O error */
	PACKAGE_MANAGER_ERROR_NO_SUCH_PACKAGE = TIZEN_ERROR_PACKAGE_MANAGER | 0x71,   /**< No such package */
	PACKAGE_MANAGER_ERROR_SYSTEM_ERROR = TIZEN_ERROR_PACKAGE_MANAGER | 0x72,      /**< Severe system error */
	PACKAGE_MANAGER_ERROR_PERMISSION_DENIED = TIZEN_ERROR_PERMISSION_DENIED       /**< Permission denied */
} package_manager_error_e;


/**
 * @brief Enumeration for event type.
 * @since_tizen 2.3
 */
typedef enum {
	PACKAGE_MANAGER_EVENT_TYPE_INSTALL = 0,    /**< Install event type */
	PACKAGE_MANAGER_EVENT_TYPE_UNINSTALL,      /**< Uninstall event type */
	PACKAGE_MANAGER_EVENT_TYPE_UPDATE,         /**< Update event type */
	PACKAGE_MANAGER_EVENT_TYPE_MOVE,           /**< Move event type (Since 3.0) */
	PACKAGE_MANAGER_EVENT_TYPE_CLEAR,          /**< Clear event type (Since 3.0) */
	PACKAGE_MANAGER_EVENT_TYPE_RES_COPY,       /**< Resource copy event type (Since 6.5) */
	PACKAGE_MANAGER_EVENT_TYPE_RES_CREATE_DIR, /**< Create directory for resource event type (Since 6.5) */
	PACKAGE_MANAGER_EVENT_TYPE_RES_REMOVE,     /**< Remove resource event type (Since 6.5) */
	PACKAGE_MANAGER_EVENT_TYPE_RES_UNINSTALL,     /**< Uninstall resource event type (Since 6.5) */
} package_manager_event_type_e;


/**
 * @brief Enumeration for event state.
 * @since_tizen 2.3
 */
typedef enum {
	PACKAGE_MANAGER_EVENT_STATE_STARTED = 0,   /**< Started event state */
	PACKAGE_MANAGER_EVENT_STATE_PROCESSING,    /**< Processing event state */
	PACKAGE_MANAGER_EVENT_STATE_COMPLETED,     /**< Completed event state */
	PACKAGE_MANAGER_EVENT_STATE_FAILED,        /**< Failed event state */
} package_manager_event_state_e;


/**
 * @brief Enumeration for move type.
 * @since_tizen 2.3
 */
typedef enum {
	PACKAGE_MANAGER_REQUEST_MOVE_TO_INTERNAL = 0,    /**< Internal type */
	PACKAGE_MANAGER_REQUEST_MOVE_TO_EXTERNAL,        /**< External type */
	PACKAGE_MANAGER_REQUEST_MOVE_TO_EXTENDED,        /**< Extended type (Since 5.0) */
} package_manager_move_type_e;


/**
 * @brief Enumeration for certification compare type.
 * @since_tizen 2.3
 */
typedef enum {
	PACKAGE_MANAGER_COMPARE_MATCH = 0,      /**< Matching certification */
	PACKAGE_MANAGER_COMPARE_MISMATCH,       /**< Mismatching certification */
	PACKAGE_MANAGER_COMPARE_LHS_NO_CERT,    /**< First package has no certification */
	PACKAGE_MANAGER_COMPARE_RHS_NO_CERT,    /**< Second package has no certification */
	PACKAGE_MANAGER_COMPARE_BOTH_NO_CERT,   /**< Both have no certification */
} package_manager_compare_result_type_e;


/**
 * @brief Enumeration for permission type.
 * @since_tizen 2.3
 */
typedef enum {
	PACKAGE_MANAGER_PERMISSION_NORMAL = 0,    /**< Normal permission */
	PACKAGE_MANAGER_PERMISSION_SIGNATURE,     /**< Signature permission */
	PACKAGE_MANAGER_PERMISSION_PRIVILEGE,     /**< Privilege permission */
} package_manager_permission_type_e;


/**
 * @brief Enumeration for status type.
 * @since_tizen 2.3
 */
typedef enum {
	PACKAGE_MANAGER_STATUS_TYPE_ALL = 0x00,               /**< All status */
	PACKAGE_MANAGER_STATUS_TYPE_INSTALL = 0x01,           /**< Install package status */
	PACKAGE_MANAGER_STATUS_TYPE_UNINSTALL = 0x02,         /**< Uninstall package status */
	PACKAGE_MANAGER_STATUS_TYPE_UPGRADE = 0x04,           /**< Upgrade package status */
	PACKAGE_MANAGER_STATUS_TYPE_MOVE = 0x08,              /**< Move package status */
	PACKAGE_MANAGER_STATUS_TYPE_CLEAR_DATA = 0x10,        /**< Clear data status */
	PACKAGE_MANAGER_STATUS_TYPE_INSTALL_PROGRESS = 0x20,  /**< Install progress status */
	PACKAGE_MANAGER_STATUS_TYPE_GET_SIZE = 0x40,          /**< Get size status */
	PACKAGE_MANAGER_STATUS_TYPE_RES_COPY = 0x80,          /**< Resource copy status (Since 6.5) */
	PACKAGE_MANAGER_STATUS_TYPE_RES_CREATE_DIR = 0x100,   /**< Resource create directory status (Since 6.5) */
	PACKAGE_MANAGER_STATUS_TYPE_RES_REMOVE = 0x200,       /**< Resource remove status (Since 6.5) */
	PACKAGE_MANAGER_STATUS_TYPE_RES_UNINSTALL = 0x400,    /**< Resource uninstall status (Since 6.5) */
} package_manager_status_type_e;


/**
 * @brief Enumeration for resource event path state.
 * @since_tizen 6.5
 */
typedef enum {
	PACKAGE_MANAGER_RES_EVENT_PATH_STATE_NONE = 0,   /**< State that operation do nothing about the path */
	PACKAGE_MANAGER_RES_EVENT_PATH_STATE_OK,         /**< State that operation success about the path */
	PACKAGE_MANAGER_RES_EVENT_PATH_STATE_FAILED,     /**< State that operation fail about the path */
} package_manager_res_event_path_state_e;

/**
 * @brief The Package manager update info request handle.
 * @since_tizen 4.0
 */
typedef struct package_updateinfo_request_s *package_updateinfo_request_h;

/**
 * @brief The Package manager handle.
 * @since_tizen 2.3
 */
typedef struct package_manager_s *package_manager_h;


/**
 * @brief Package manager filter handle.
 * @since_tizen 2.3
 */
typedef struct package_manager_filter_s *package_manager_filter_h;

/**
 * @brief Resource share event handle.
 * @since_tizen 6.5
 */
typedef struct package_manager_res_event_info_s *package_manager_res_event_info_h;

/**
 * @brief Called when the package is installed, uninstalled, or updated, and the progress of the request to the package manager changes.
 * @since_tizen 2.3
 * @param[in] type The type of the package to be installed, uninstalled, or updated
 * @param[in] package The name of the package to be installed, uninstalled, or updated
 * @param[in] event_type The type of the request to the package manager
 * @param[in] event_state The current state of the request to the package manager
 * @param[in] progress    The progress for the request that is being processed by the package manager \n
 *                        The range of progress is from @c 0 to @c 100
 * @param[in] error       The error code when the package manager failed to process the request
 * @param[in] user_data   The user data passed from package_manager_set_event_cb()
 * @see package_manager_set_event_cb()
 * @see package_manager_unset_event_cb()
 */
typedef void (*package_manager_event_cb) (const char *type, const char *package, package_manager_event_type_e event_type, package_manager_event_state_e event_state, int progress, package_manager_error_e error, void *user_data);

/**
 * @brief Called when the progress of the request to the package resource share changes.
 * @since_tizen 6.5
 * @remarks The @a pkgid should not be released. The @a pkgid can be used only in the callback.
 * @remarks The @a handle should not be released. The @a handle can be used only in the callback.
 * @param[in] pkgid The package ID of resource owner
 * @param[in] event_type The type of resource event
 * @param[in] event_state The state of resource event
 * @param[in] handle    The handle which contains additional information of event
 * @param[in] user_data   The user data passed from package_manager_set_res_event_cb()
 * @see package_manager_set_res_event_cb()
 * @see package_manager_unset_event_cb()
 */
typedef void (*package_manager_res_event_cb) (const char *pkgid,
		package_manager_event_type_e event_type,
		package_manager_event_state_e event_state,
		package_manager_res_event_info_h handle, void *user_data);


/**
 * @brief Creates a package manager handle.
 * @since_tizen 2.3
 * @privlevel public
 * @privilege %http://tizen.org/privilege/packagemanager.info
 * @remarks You must release @a manager using package_manager_destroy().
 * @param[out] manager The package manager handle that is newly created on success
 * @return @c 0 on success,
 *         otherwise a negative error value
 * @retval #PACKAGE_MANAGER_ERROR_NONE              Successful
 * @retval #PACKAGE_MANAGER_ERROR_PERMISSION_DENIED Permission denied
 * @retval #PACKAGE_MANAGER_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #PACKAGE_MANAGER_ERROR_OUT_OF_MEMORY     Out of memory
 * @retval #PACKAGE_MANAGER_ERROR_IO_ERROR          Internal I/O error
 * @see package_manager_destroy()
 */
int package_manager_create(package_manager_h *manager);


/**
 * @brief Destroys the package manager handle.
 * @since_tizen 2.3
 * @param[in] manager The package manager handle
 * @return @c 0 on success,
 *         otherwise a negative error value
 * @retval #PACKAGE_MANAGER_ERROR_NONE              Successful
 * @retval #PACKAGE_MANAGER_ERROR_INVALID_PARAMETER Invalid parameter
 * @see package_manager_create()
 */
int package_manager_destroy(package_manager_h manager);


/**
 * @brief Sets the event status of the package when the package is installed, uninstalled, or updated.
 * @details You can combine multiple status using OR operation which you want to listen.
 * @since_tizen 2.3
 * @param[in] manager     The package manager handle
 * @param[in] status_type The status of the package
 * @return @c 0 on success,
 *         otherwise a negative error value
 * @retval #PACKAGE_MANAGER_ERROR_NONE              Successful
 * @retval #PACKAGE_MANAGER_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #PACKAGE_MANAGER_ERROR_IO_ERROR          Internal I/O error
 * @see package_manager_status_type_e
 * @see package_manager_set_event_cb()
 */
int package_manager_set_event_status(package_manager_h manager, int status_type);


/**
 * @brief Registers a callback function to be invoked when the package is installed, uninstalled, or updated.
 * @since_tizen 2.3
 * @privlevel public
 * @privilege %http://tizen.org/privilege/packagemanager.info
 * @param[in] manager    The package manager handle
 * @param[in] callback   The callback function to be registered
 * @param[in] user_data  The user data to be passed to the callback function
 * @return @c 0 on success,
 *         otherwise a negative error value
 * @retval #PACKAGE_MANAGER_ERROR_NONE              Successful
 * @retval #PACKAGE_MANAGER_ERROR_PERMISSION_DENIED Permission denied
 * @retval #PACKAGE_MANAGER_ERROR_INVALID_PARAMETER Invalid parameter
 * @post package_manager_event_cb() will be invoked.
 * @see package_manager_set_event_status()
 * @see package_manager_event_cb()
 * @see package_manager_unset_event_cb()
 */
int package_manager_set_event_cb(package_manager_h manager, package_manager_event_cb callback, void *user_data);


/**
 * @platform
 * @brief Registers a callback function to be invoked when the progress of the request to the package resource share changes.
 * @since_tizen 6.5
 * @privlevel platform
 * @privilege %http://tizen.org/privilege/packagemanager.admin
 * @param[in] manager    The package manager handle
 * @param[in] callback   The callback function to be registered
 * @param[in] user_data  The user data to be passed to the callback function
 * @return @c 0 on success,
 *         otherwise a negative error value
 * @retval #PACKAGE_MANAGER_ERROR_NONE              Successful
 * @retval #PACKAGE_MANAGER_ERROR_PERMISSION_DENIED Permission denied
 * @retval #PACKAGE_MANAGER_ERROR_INVALID_PARAMETER Invalid parameter
 * @post package_manager_res_event_cb() will be invoked.
 * @see package_manager_set_event_status()
 * @see package_manager_res_event_cb()
 * @see package_manager_unset_event_cb()
 */
int package_manager_set_res_event_cb(package_manager_h manager, package_manager_res_event_cb callback, void *user_data);


/**
 * @brief Unregisters the callback function.
 * @since_tizen 2.3
 * @param[in] manager The package manager handle
 * @return @c 0 on success,
 *         otherwise a negative error value
 * @retval #PACKAGE_MANAGER_ERROR_NONE              Successful
 * @retval #PACKAGE_MANAGER_ERROR_INVALID_PARAMETER Invalid parameter
 * @see package_manager_event_cb()
 * @see package_manager_set_event_cb()
 */
int package_manager_unset_event_cb(package_manager_h manager);


/**
 * @brief Called to retrieve all packages.
 * @since_tizen 2.3
 * @param[in] package_info The package information
 * @param[in] user_data    The user data passed from the foreach function
 * @return  @c true to continue with the next iteration of the loop,
 *          otherwise @c false to break out of the loop
 * @see package_manager_foreach_package_info()
 */
typedef bool (*package_manager_package_info_cb) (package_info_h package_info, void *user_data);


/**
 * @brief Retrieves all package information of installed packages.
 * @since_tizen 2.3
 * @privlevel public
 * @privilege %http://tizen.org/privilege/packagemanager.info
 * @param[in] callback  The callback function to be invoked
 * @param[in] user_data The user data to be passed to the callback function
 * @return @c 0 on success,
 *         otherwise a negative error value
 * @retval #PACKAGE_MANAGER_ERROR_NONE              Successful
 * @retval #PACKAGE_MANAGER_ERROR_PERMISSION_DENIED Permission denied
 * @retval #PACKAGE_MANAGER_ERROR_INVALID_PARAMETER Invalid parameter
 * @post This function invokes package_manager_package_info_cb() repeatedly for each package information.
 * @see package_manager_package_info_cb()
 */
int package_manager_foreach_package_info(package_manager_package_info_cb callback, void *user_data);


/**
 * @brief Gets the package ID for the given app ID.
 * @since_tizen 2.3
 * @privlevel public
 * @privilege %http://tizen.org/privilege/packagemanager.info
 * @remarks The @a package_id should be released using free().
 * @param[in]  app_id     The ID of the application
 * @param[out] package_id The ID of the package
 * @return @c 0 on success,
 *         otherwise a negative error value
 * @retval #PACKAGE_MANAGER_ERROR_NONE              Successful
 * @retval #PACKAGE_MANAGER_ERROR_PERMISSION_DENIED Permission denied
 * @retval #PACKAGE_MANAGER_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #PACKAGE_MANAGER_ERROR_OUT_OF_MEMORY     Out of memory
 */
int package_manager_get_package_id_by_app_id(const char *app_id, char **package_id);


/**
 * @brief Gets the package information for the given package.
 * @since_tizen 2.3
 * @privlevel public
 * @privilege %http://tizen.org/privilege/packagemanager.info
 * @remarks You must release @a package_info using package_info_destroy().
 * @param[in]  package_id   The ID of the package
 * @param[out] package_info The package information for the given package ID
 * @return @c 0 on success,
 *         otherwise a negative error value
 * @retval #PACKAGE_MANAGER_ERROR_NONE              Successful
 * @retval #PACKAGE_MANAGER_ERROR_PERMISSION_DENIED Permission denied
 * @retval #PACKAGE_MANAGER_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #PACKAGE_MANAGER_ERROR_OUT_OF_MEMORY     Out of memory
 * @retval #PACKAGE_MANAGER_ERROR_IO_ERROR Database error occurred
 */
int package_manager_get_package_info(const char *package_id, package_info_h *package_info);


/**
 * @brief Compares whether two package certifications are matched.
 * @since_tizen 2.3
 * @param[in]  lhs_package_id The first package ID to compare
 * @param[in]  rhs_package_id The second package ID to compare
 * @param[out] compare_result @c 0 if the certification information are matched,
 *                            otherwise the compared result
 * @return @c 0 on success,
 *         otherwise a negative error value
 * @retval #PACKAGE_MANAGER_ERROR_NONE              Successful
 * @retval #PACKAGE_MANAGER_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #PACKAGE_MANAGER_ERROR_OUT_OF_MEMORY     Out of memory
 * @retval #PACKAGE_MANAGER_ERROR_IO_ERROR          Database error occurred
 */
int package_manager_compare_package_cert_info(const char *lhs_package_id, const char *rhs_package_id, package_manager_compare_result_type_e *compare_result);


/**
 * @brief Compares whether two app certifications are matched.
 * @since_tizen 2.3
 * @param[in]  lhs_app_id     The first app ID to compare
 * @param[in]  rhs_app_id     The second app ID to compare
 * @param[out] compare_result @c 0 if the certification information are matched,
 *                            otherwise the compared result
 * @return @c 0 on success,
 *         otherwise a negative error value
 * @retval #PACKAGE_MANAGER_ERROR_NONE              Successful
 * @retval #PACKAGE_MANAGER_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #PACKAGE_MANAGER_ERROR_OUT_OF_MEMORY     Out of memory
 * @retval #PACKAGE_MANAGER_ERROR_IO_ERROR          Database error occurred
 */
int package_manager_compare_app_cert_info(const char *lhs_app_id, const char *rhs_app_id, package_manager_compare_result_type_e *compare_result);


/**
 * @brief Checks whether the package is preloaded by @a app_id.
 * @since_tizen 2.3
 * @privlevel public
 * @privilege %http://tizen.org/privilege/packagemanager.info
 * @param[in]  app_id   The ID of the application
 * @param[out] preload  The preload info of the package
 * @return @c 0 on success,
 *         otherwise a negative error value
 * @retval #PACKAGE_MANAGER_ERROR_NONE              Successful
 * @retval #PACKAGE_MANAGER_ERROR_PERMISSION_DENIED Permission denied
 * @retval #PACKAGE_MANAGER_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #PACKAGE_MANAGER_ERROR_OUT_OF_MEMORY     Out of memory
 * @retval #PACKAGE_MANAGER_ERROR_IO_ERROR          I/O error
 */
int package_manager_is_preload_package_by_app_id(const char *app_id, bool *preload);


/**
 * @brief Gets the package permission type by @a app_id.
 * @since_tizen 2.3
 * @privlevel public
 * @privilege %http://tizen.org/privilege/packagemanager.info
 * @param[in]  app_id          The ID of the application
 * @param[out] permission_type The package permission type
 * @return @c 0 on success,
 *         otherwise a negative error value
 * @retval #PACKAGE_MANAGER_ERROR_NONE              Successful
 * @retval #PACKAGE_MANAGER_ERROR_PERMISSION_DENIED Permission denied
 * @retval #PACKAGE_MANAGER_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #PACKAGE_MANAGER_ERROR_OUT_OF_MEMORY     Out of memory
 * @retval #PACKAGE_MANAGER_ERROR_IO_ERROR          I/O error
 */
int package_manager_get_permission_type(const char *app_id, package_manager_permission_type_e *permission_type);


/**
 * @brief Clears the application's internal and external cache directory, asynchronously.
 * @details All files stored in the cache directory of the application specified with the package ID are removed.
 * @since_tizen 2.4
 * @privlevel public
 * @privilege %http://tizen.org/privilege/packagemanager.clearcache
 * @param[in] package_id  The package ID
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
int package_manager_clear_cache_dir(const char *package_id);

/**
 * @platform
 * @brief  Clears all applications' internal and external cache directory, asynchronously.
 * @details All files stored in the cache directory of each application are removed.
 * @since_tizen 2.3
 * @privlevel platform
 * @privilege %http://tizen.org/privilege/packagemanager.admin
 * @return @c 0 on success,
 *         otherwise a negative error value
 * @retval #PACKAGE_MANAGER_ERROR_NONE              Successful
 * @retval #PACKAGE_MANAGER_ERROR_PERMISSION_DENIED Permission denied
 * @retval #PACKAGE_MANAGER_ERROR_NO_SUCH_PACKAGE   No such package
 * @retval #PACKAGE_MANAGER_ERROR_OUT_OF_MEMORY     Out of memory
 * @retval #PACKAGE_MANAGER_ERROR_IO_ERROR          I/O error
 * @retval #PACKAGE_MANAGER_ERROR_SYSTEM_ERROR      Severe system error
 */
int package_manager_clear_all_cache_dir(void);

/**
 * @platform
 * @brief Clears internal and external data directories used by applications in the given package.
 * @details All files stored in data directories used by applications in the package specified with the @a package_id are removed.
 * @since_tizen 4.0
 * @privlevel platform
 * @privilege %http://tizen.org/privilege/packagemanager.admin
 * @param[in] package_id  The package ID
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
int package_manager_clear_data_dir(const char *package_id);


/**
 * @brief The package size information handle.
 * @since_tizen 2.4
 */
typedef struct package_size_info *package_size_info_h;


/**
 * @brief Called when the package size information is obtained.
 * @since_tizen 2.4
 * @param[in] package_id  The package ID
 * @param[in] size_info   The pointer to the structure including the package size information
 * @param[in] user_data   The user data to be passed to the callback function
 */
typedef void (*package_manager_size_info_receive_cb)(const char *package_id, const package_size_info_h size_info, void *user_data);


/**
 * @platform
 * @brief Called when the total package size information is obtained.
 * @since_tizen 2.3
 * @param[in] size_info  The pointer to the structure including the package size information
 * @param[in] user_data  The user data to be passed to the callback function
 */
typedef void (*package_manager_total_size_info_receive_cb)(const package_size_info_h size_info, void *user_data);


/**
 * @brief Gets the package size information, asynchronously.
 * @details The package size info is asynchronously obtained by the callback function.
 * @since_tizen 2.4
 * @privlevel public
 * @privilege %http://tizen.org/privilege/packagemanager.info
 * @param[in] package_id  The package ID
 * @param[in] callback    The asynchronous callback function to get the package size information
 * @param[in] user_data   The user data to be passed to the callback function
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
int package_manager_get_package_size_info(const char *package_id, package_manager_size_info_receive_cb callback, void *user_data);


/**
 * @brief Gets the total package size information, asynchronously.
 * @details The total package size info is asynchronously obtained by the callback function.
 * @since_tizen 2.3
 * @privlevel public
 * @privilege %http://tizen.org/privilege/packagemanager.info
 * @param[in] callback  The asynchronous callback function to get the total package size information
 * @param[in] user_data The user data to be passed to the callback function
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
int package_manager_get_total_package_size_info(package_manager_total_size_info_receive_cb callback, void *user_data);


/**
 * @brief Creates the package information filter handle from db.
 * @details The filtered result will be based on AND operation of all added filter properties.
 * @since_tizen 2.3
 * @privlevel public
 * @privilege %http://tizen.org/privilege/packagemanager.info
 * @remarks The @a handle should be released using package_manager_filter_destroy().
 * @param[out] handle Pointer to the package info filter handle
 * @return @c 0 on success,
 *         otherwise a negative error value
 * @retval #PACKAGE_MANAGER_ERROR_NONE Successful
 * @retval #PACKAGE_MANAGER_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #PACKAGE_MANAGER_ERROR_IO_ERROR I/O error
 * @post package_manager_filter_destroy()
 * @see package_manager_filter_add_bool()
 * @see package_manager_filter_foreach_package_info()
 */
int package_manager_filter_create(package_manager_filter_h *handle);


/**
 * @brief Destroys the package information filter handle freeing up all the resources.
 * @since_tizen 2.3
 * @param[in] handle Pointer to the package info filter handle
 * @return @c 0 on success,
 *         otherwise a negative error value
 * @retval #PACKAGE_MANAGER_ERROR_NONE Successful
 * @retval #PACKAGE_MANAGER_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #PACKAGE_MANAGER_ERROR_IO_ERROR I/O error
 * @pre package_manager_filter_create()
 * @see package_manager_filter_count()
 * @see package_manager_filter_foreach_package_info()
 */
int package_manager_filter_destroy(package_manager_filter_h handle);


/**
 * @brief Adds a boolean filter property to the filter handle.
 * @since_tizen 2.3
 * @param[in] handle Pointer to the package info filter handle
 * @param[in] property boolean property name
 * @param[in] value value corresponding to the property
 * @return @c 0 on success,
 *         otherwise a negative error value
 * @retval #PACKAGE_MANAGER_ERROR_NONE Successful
 * @retval #PACKAGE_MANAGER_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #PACKAGE_MANAGER_ERROR_IO_ERROR I/O error
 * @pre package_manager_filter_create()
 * @post package_manager_filter_destroy()
 * @see package_manager_filter_count()
 * @see package_manager_filter_foreach_package_info()
 */
int package_manager_filter_add_bool(package_manager_filter_h handle, const char *property, const bool value);


/**
 * @brief Adds a string filter property to the filter handle.
 * @since_tizen 6.5
 * @param[in] handle pointer to the package info filter handle
 * @param[in] property string property name
 * @param[in] value value corresponding to the property
 * @return @c 0 on success,
 *         otherwise a negative error value
 * @retval #PACKAGE_MANAGER_ERROR_NONE Successful
 * @retval #PACKAGE_MANAGER_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #PACKAGE_MANAGER_ERROR_IO_ERROR I/O error
 * @pre package_manager_filter_create()
 * @post package_manager_filter_destroy()
 * @see package_manager_filter_count()
 * @see package_manager_filter_foreach_package_info()
 */
int package_manager_filter_add_string(package_manager_filter_h handle, const char *property, const char *value);


/**
 * @brief Counts the package that satisfy the filter conditions.
 * @since_tizen 2.3
 * @privlevel public
 * @privilege %http://tizen.org/privilege/packagemanager.info
 * @param[in] handle Pointer to the package info filter handle
 * @param[out] count Pointer to store the count value
 * @return @c 0 on success,
 *         otherwise a negative error value
 * @retval #PACKAGE_MANAGER_ERROR_NONE Successful
 * @retval #PACKAGE_MANAGER_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #PACKAGE_MANAGER_ERROR_IO_ERROR I/O error
 * @pre package_manager_filter_create()
 * @post package_manager_filter_destroy()
 * @see package_manager_filter_foreach_package_info()
 */
int package_manager_filter_count(package_manager_filter_h handle, int *count);


/**
 * @brief Executes the user supplied callback function for each package that satisfy the filter conditions.
 * @since_tizen 2.3
 * @privlevel public
 * @privilege %http://tizen.org/privilege/packagemanager.info
 * @param[in] handle Pointer to the package info filter handle
 * @param[in] callback callback function
 * @param[in] user_data user data to be passed to the callback function
 * @return @c 0 on success,
 *         otherwise a negative error value
 * @retval #PACKAGE_MANAGER_ERROR_NONE Successful
 * @retval #PACKAGE_MANAGER_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #PACKAGE_MANAGER_ERROR_IO_ERROR I/O error
 * @pre package_manager_filter_create()
 * @post package_manager_filter_destroy()
 * @post This function invokes package_manager_package_info_cb() repeatedly for each package information.
 * @see package_manager_package_info_cb()
 * @see package_manager_filter_count()
 */
int package_manager_filter_foreach_package_info(package_manager_filter_h handle, package_manager_package_info_cb callback, void *user_data);


/**
 * @platform
 * @brief Generates request for getting License, asynchronously.
 * @since_tizen 2.4
 * @privlevel platform
 * @privilege %http://tizen.org/privilege/packagemanager.admin
 * @remarks You must release @a req_data and @a license_url by yourself.
 * @param[in] resp_data The response data string of the purchase request
 * @param[out] req_data	License request data
 * @param[out] license_url License acquisition url data
 * @return @c 0 on success,
 *         otherwise a negative error value
 * @retval #PACKAGE_MANAGER_ERROR_NONE Successful
 * @retval #PACKAGE_MANAGER_ERROR_PERMISSION_DENIED Permission denied
 * @retval #PACKAGE_MANAGER_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #PACKAGE_MANAGER_ERROR_OUT_OF_MEMORY Out of memory
 * @retval #PACKAGE_MANAGER_ERROR_IO_ERROR Internal I/O error
 * @post package_manager_drm_register_license()
 */
int package_manager_drm_generate_license_request(const char *resp_data, char **req_data, char **license_url);


/**
 * @platform
 * @brief Registers encrypted license, asynchronously.
 * @since_tizen 2.4
 * @privlevel platform
 * @privilege %http://tizen.org/privilege/packagemanager.admin
 * @param[in] resp_data The response data string of the rights request
 * @return @c 0 on success,
 *         otherwise a negative error value
 * @retval #PACKAGE_MANAGER_ERROR_NONE Successful
 * @retval #PACKAGE_MANAGER_ERROR_PERMISSION_DENIED Permission denied
 * @retval #PACKAGE_MANAGER_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #PACKAGE_MANAGER_ERROR_IO_ERROR Internal I/O error
 * @pre package_manager_drm_generate_license_request()
 */
int package_manager_drm_register_license(const char *resp_data);

/**
 * @platform
 * @brief Decrypts contents which is encrypted, asynchronously.
 * @since_tizen 2.4
 * @privlevel platform
 * @privilege %http://tizen.org/privilege/packagemanager.admin
 * @param[in] drm_file_path DRM file path
 * @param[in] decrypted_file_path Decrypted file path
 * @return @c 0 on success,
 *         otherwise a negative error value
 * @retval #PACKAGE_MANAGER_ERROR_NONE Successful
 * @retval #PACKAGE_MANAGER_ERROR_PERMISSION_DENIED Permission denied
 * @retval #PACKAGE_MANAGER_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #PACKAGE_MANAGER_ERROR_IO_ERROR Internal I/O error
 */
int package_manager_drm_decrypt_package(const char *drm_file_path, const char *decrypted_file_path);


/**
 * @brief Retrieves data size from given handle.
 * @since_tizen 2.4
 * @param[in] handle Package size info handle
 * @param[out] data_size Data size will be returned
 * @return @c 0 on success,
 *         otherwise a negative error value
 * @retval #PACKAGE_MANAGER_ERROR_NONE              Successful
 * @retval #PACKAGE_MANAGER_ERROR_INVALID_PARAMETER Invalid parameter
 */
int package_size_info_get_data_size(package_size_info_h handle, long long *data_size);


/**
 * @brief Retrieves cache size from given handle.
 * @since_tizen 2.4
 * @param[in] handle  Package size info handle
 * @param[out] cache_size Cache size will be returned
 * @return @c 0 on success,
 *         otherwise a negative error value
 * @retval #PACKAGE_MANAGER_ERROR_NONE              Successful
 * @retval #PACKAGE_MANAGER_ERROR_INVALID_PARAMETER Invalid parameter
 */
int package_size_info_get_cache_size(package_size_info_h handle, long long *cache_size);


/**
 * @brief Retrieves application size from given handle.
 * @since_tizen 2.4
 * @param[in] handle Package size info handle
 * @param[out] app_size App size will be returned
 * @return @c 0 on success,
 *         otherwise a negative error value
 * @retval #PACKAGE_MANAGER_ERROR_NONE              Successful
 * @retval #PACKAGE_MANAGER_ERROR_INVALID_PARAMETER Invalid parameter
 */
int package_size_info_get_app_size(package_size_info_h handle, long long *app_size);


/**
 * @brief Retrieves external data size from given handle.
 * @since_tizen 2.4
 * @param[in] handle  Package size info handle
 * @param[out] ext_data_size External data size will be returned
 * @return @c 0 on success,
 *         otherwise a negative error value
 * @retval #PACKAGE_MANAGER_ERROR_NONE              Successful
 * @retval #PACKAGE_MANAGER_ERROR_INVALID_PARAMETER Invalid parameter
 */
int package_size_info_get_external_data_size(package_size_info_h handle, long long *ext_data_size);


/**
 * @brief Retrieves external cache size from given handle.
 * @since_tizen 2.4
 * @param[in] handle Package size info handle
 * @param[out] ext_cache_size External cache size will be returned
 * @return @c 0 on success,
 *         otherwise a negative error value
 * @retval #PACKAGE_MANAGER_ERROR_NONE              Successful
 * @retval #PACKAGE_MANAGER_ERROR_INVALID_PARAMETER Invalid parameter
 */
int package_size_info_get_external_cache_size(package_size_info_h handle, long long *ext_cache_size);


/**
 * @brief Retrieves external application size from given handle.
 * @since_tizen 2.4
 * @param[in] handle Package size info handle
 * @param[out] ext_app_size External app size will be returned
 * @return @c 0 on success,
 *         otherwise a negative error value
 * @retval #PACKAGE_MANAGER_ERROR_NONE              Successful
 * @retval #PACKAGE_MANAGER_ERROR_INVALID_PARAMETER Invalid parameter
 */
int package_size_info_get_external_app_size(package_size_info_h handle, long long *ext_app_size);


/**
* @}
*/

/**
 * @addtogroup CAPI_PACKAGE_REQUEST_MODULE
 * @{
 */


/**
 * @platform
 * @brief Enumeration for request mode.
 * @since_tizen 2.3
 */
typedef enum {
	PACKAGE_MANAGER_REQUEST_MODE_DEFAULT = 0,    /**< @platform Default request mode */
	PACKAGE_MANAGER_REQUEST_MODE_QUIET,          /**< @platform Quiet request mode */
} package_manager_request_mode_e;


/**
 * @platform
 * @brief The Package manager request handle.
 * @since_tizen 2.3
 */
typedef struct package_manager_request_s *package_manager_request_h;


/**
 * @platform
 * @brief Called when the progress of the request to the package manager changes.
 * @since_tizen 2.3
 * @param[in] id          The ID of the request to the package manager
 * @param[in] type        The type of the package to install, uninstall or update
 * @param[in] package     The name of the package to install, uninstall or update
 * @param[in] event_type  The type of the request to the package manager
 * @param[in] event_state The current state of the request to the package manager
 * @param[in] progress    The progress for the request that is being processed by the package manager \n
 *                        The range of progress is from @c 0 to @c 100.
 * @param[in] error       The error code when the package manager failed to process the request
 * @param[in] user_data   The user data passed from package_manager_request_set_event_cb()
 * @see package_manager_request_set_event_cb()
 * @see package_manager_request_unset_event_cb()
 */
typedef void (*package_manager_request_event_cb) (int id, const char *type, const char *package, package_manager_event_type_e event_type, package_manager_event_state_e event_state, int progress, package_manager_error_e error, void *user_data);


/**
 * @platform
 * @brief Called when the progress of the request to the package resource share changes.
 * @since_tizen 6.5
 * @remarks The @a pkgid should not be released. The @a pkgid can be used only in the callback.
 * @remarks The @a handle should not be released. The @a handle can be used only in the callback.
 * @param[in] req_id      The ID of the request to the package manager
 * @param[in] pkgid       The package ID of resource owner
 * @param[in] event_type  The type of resource event
 * @param[in] event_state The state of resource event
 * @param[in] handle      The handle which contains additional information of event
 * @param[in] user_data   The user data passed from package_manager_request_res_copy_with_cb(), package_manager_request_res_create_dir_with_cb(), package_manager_request_res_remove_with_cb()
 * @see package_manager_request_res_copy_with_cb()
 * @see package_manager_request_res_create_dir_with_cb()
 * @see package_manager_request_res_remove_with_cb()
 */
typedef void (*package_manager_request_res_event_cb) (int req_id,
		const char *pkgid, package_manager_event_type_e event_type, package_manager_event_state_e event_state,
		package_manager_res_event_info_h handle, void *user_data);


/**
 * @platform
 * @brief Creates a request handle to the package manager.
 * @since_tizen 2.3
 * @remarks You must release @a request using package_manager_request_destroy().
 * @param[out] request The request handle that is newly created on success
 * @return @c 0 on success,
 *         otherwise a negative error value
 * @retval #PACKAGE_MANAGER_ERROR_NONE              Successful
 * @retval #PACKAGE_MANAGER_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #PACKAGE_MANAGER_ERROR_OUT_OF_MEMORY     Out of memory
 * @retval #PACKAGE_MANAGER_ERROR_IO_ERROR          Internal I/O error
 * @see package_manager_request_destroy()
 */
int package_manager_request_create(package_manager_request_h *request);


/**
 * @platform
 * @brief Destroys the request handle to the package manager.
 * @since_tizen 2.3
 * @param[in] request The request handle to the package manager
 * @return @c 0 on success,
 *         otherwise a negative error value
 * @retval #PACKAGE_MANAGER_ERROR_NONE              Successful
 * @retval #PACKAGE_MANAGER_ERROR_INVALID_PARAMETER Invalid parameter
 * @see package_manager_request_create()
 */
int package_manager_request_destroy(package_manager_request_h request);


/**
 * @platform
 * @brief Registers a callback function to be invoked when the progress of the request changes.
 * @since_tizen 2.3
 * @privlevel public
 * @privilege %http://tizen.org/privilege/packagemanager.info
 * @param[in] request The request handle
 * @param[in] callback The callback function to be registered
 * @param[in] user_data The user data to be passed to the callback function
 * @return @c 0 on success,
 *         otherwise a negative error value
 * @retval #PACKAGE_MANAGER_ERROR_NONE              Successful
 * @retval #PACKAGE_MANAGER_ERROR_PERMISSION_DENIED Permission denied
 * @retval #PACKAGE_MANAGER_ERROR_INVALID_PARAMETER Invalid parameter
 * @post package_manager_request_event_cb() will be invoked.
 * @see package_manager_request_event_cb()
 * @see package_manager_request_unset_event_cb()
 */
int package_manager_request_set_event_cb(package_manager_request_h request, package_manager_request_event_cb callback, void *user_data);


/**
 * @platform
 * @brief Unregisters the callback function.
 * @since_tizen 2.3
 * @param[in] request The request handle
 * @return @c 0 on success,
 *         otherwise a negative error value
 * @retval #PACKAGE_MANAGER_ERROR_NONE              Successful
 * @retval #PACKAGE_MANAGER_ERROR_INVALID_PARAMETER Invalid parameter
 * @see package_manager_request_event_cb()
 * @see package_manager_request_set_event_cb()
 */
int package_manager_request_unset_event_cb(package_manager_request_h request);


/**
 * @platform
 * @brief Sets the type of the package to install, uninstall, or update.
 * @since_tizen 2.3
 * @param[in] request The request handle
 * @param[in] type    The type of the package
 * @return @c 0 on success,
 *         otherwise a negative error value
 * @retval #PACKAGE_MANAGER_ERROR_NONE              Successful
 * @retval #PACKAGE_MANAGER_ERROR_INVALID_PARAMETER Invalid parameter
 */
int package_manager_request_set_type(package_manager_request_h request, const char *type);


/**
 * @platform
 * @brief Sets the mode of the request.
 * @since_tizen 2.3
 * @param[in] request The request handle
 * @param[in] mode    The mode of the request
 * @return @c 0 on success,
 *         otherwise a negative error value
 * @retval #PACKAGE_MANAGER_ERROR_NONE              Successful
 * @retval #PACKAGE_MANAGER_ERROR_INVALID_PARAMETER Invalid parameter
 */
int package_manager_request_set_mode(package_manager_request_h request, package_manager_request_mode_e mode);


/**
 * @platform
 * @brief Sets the path of TEP file to the request. The TEP file that is set will be installed when the package is installed.
 * @since_tizen 2.4
 * @privlevel platform
 * @privilege %http://tizen.org/privilege/packagemanager.admin
 * @param[in] request The request handle
 * @param[in] tep_path The TEP path to set. If this is NULL on update, installed TEP will be removed.
 * @return @c 0 on success,
 *         otherwise a negative error value
 * @retval #PACKAGE_MANAGER_ERROR_NONE Successful
 * @retval #PACKAGE_MANAGER_ERROR_PERMISSION_DENIED Permission denied
 * @retval #PACKAGE_MANAGER_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #PACKAGE_MANAGER_ERROR_SYSTEM_ERROR		 Severe system error
 */
int package_manager_request_set_tep(package_manager_request_h request, const char *tep_path);


/**
 * @platform
 * @brief Installs the package located at the given path, asynchronously.
 * @since_tizen 2.3
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
int package_manager_request_install(package_manager_request_h request, const char *path, int *id);


/**
 * @platform
 * @brief Installs the package located at the given path, asynchronously.
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
int package_manager_request_install_with_cb(package_manager_request_h request,
						const char *path,
						package_manager_request_event_cb callback,
						void *user_data,
						int *id);

/**
 * @platform
 * @brief Installs the packages located at the given paths, asynchronously.
 * @since_tizen 6.0
 * @privlevel platform
 * @privilege %http://tizen.org/privilege/packagemanager.admin
 * @param[in]  request The request handle
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
int package_manager_request_install_packages(package_manager_request_h request,
						const char **paths,
						int paths_count,
						int *id);

/**
 * @platform
 * @brief Installs the packages located at the given paths, asynchronously.
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
int package_manager_request_install_packages_with_cb(package_manager_request_h request,
						const char **paths,
						int paths_count,
						package_manager_request_event_cb callback,
						void *user_data,
						int *id);

/**
 * @platform
 * @brief Uninstalls the package with the given name, asynchronously.
 * @since_tizen 2.3
 * @privlevel platform
 * @privilege %http://tizen.org/privilege/packagemanager.admin
 * @param[in]  request The request handle
 * @param[in]  name    The name of the package to be uninstalled
 * @param[out] id      The ID of the request to the package manager
 * @return @c 0 on success,
 *         otherwise a negative error value
 * @retval #PACKAGE_MANAGER_ERROR_NONE              Successful
 * @retval #PACKAGE_MANAGER_ERROR_PERMISSION_DENIED Permission denied
 * @retval #PACKAGE_MANAGER_ERROR_INVALID_PARAMETER Invalid parameter
 */
int package_manager_request_uninstall(package_manager_request_h request, const char *name, int *id);


/**
 * @platform
 * @brief Uninstalls the package with the given name, asynchronously.
 * @details The progress of the request is asynchronously received by the callback function.
 *          The @a callback is the individual callback only called for the current API call.
 *          The @a callback is the only callback called, even if another callback was set for this request
 *          with package_manager_request_set_event_cb().
 * @since_tizen 4.0
 * @privlevel platform
 * @privilege %http://tizen.org/privilege/packagemanager.admin
 * @param[in]  request   The request handle
 * @param[in]  name      The name of the package to be uninstalled
 * @param[in]  callback  The callback function to be invoked
 * @param[in]  user_data The user data to be passed to the callback function
 * @param[out] id        The ID of the request to the package manager
 * @return @c 0 on success,
 *         otherwise a negative error value
 * @retval #PACKAGE_MANAGER_ERROR_NONE              Successful
 * @retval #PACKAGE_MANAGER_ERROR_PERMISSION_DENIED Permission denied
 * @retval #PACKAGE_MANAGER_ERROR_INVALID_PARAMETER Invalid parameter
 */
int package_manager_request_uninstall_with_cb(package_manager_request_h request,
						const char *name,
						package_manager_request_event_cb callback,
						void *user_data,
						int *id);

/**
 * @platform
 * @brief Moves the package from SD card to the internal memory and vice versa, asynchronously.
 * @since_tizen 2.3
 * @privlevel platform
 * @privilege %http://tizen.org/privilege/packagemanager.admin
 * @param[in] request   The request handle
 * @param[in] name      The name of the package to be moved
 * @param[in] move_type The move type [#package_manager_move_type_e], [external to internal/internal to external]
 * @return @c 0 on success,
 *         otherwise a negative error value
 * @retval #PACKAGE_MANAGER_ERROR_NONE              Successful
 * @retval #PACKAGE_MANAGER_ERROR_PERMISSION_DENIED Permission denied
 * @retval #PACKAGE_MANAGER_ERROR_INVALID_PARAMETER Invalid parameter
 */
int package_manager_request_move(package_manager_request_h request, const char *name, package_manager_move_type_e move_type);


/**
 * @platform
 * @brief Moves the package from SD card to the internal memory and vice versa, asynchronously.
 * @details The progress of the request is asynchronously received by the callback function.
 *          The @a callback is the individual callback only called for the current API call.
 *          The @a callback is the only callback called, even if another callback was set for this request
 *          with package_manager_request_set_event_cb().
 * @since_tizen 4.0
 * @privlevel platform
 * @privilege %http://tizen.org/privilege/packagemanager.admin
 * @param[in]  request   The request handle
 * @param[in]  name      The name of the package to be moved
 * @param[in]  move_type The move type [#package_manager_move_type_e], [external to internal/internal to external]
 * @param[in]  callback  The callback function to be invoked
 * @param[in]  user_data The user data to be passed to the callback function
 * @param[out] id        The ID of the request to the package manager
 * @return @c 0 on success,
 *         otherwise a negative error value
 * @retval #PACKAGE_MANAGER_ERROR_NONE              Successful
 * @retval #PACKAGE_MANAGER_ERROR_PERMISSION_DENIED Permission denied
 * @retval #PACKAGE_MANAGER_ERROR_INVALID_PARAMETER Invalid parameter
 */
int package_manager_request_move_with_cb(package_manager_request_h request,
						const char *name,
						package_manager_move_type_e move_type,
						package_manager_request_event_cb callback,
						void *user_data,
						int *id);

/**
 * @platform
 * @brief Creates a package update info request.
 * @since_tizen 4.0
 * @privlevel platform
 * @privilege %http://tizen.org/privilege/packagemanager.admin
 * @remarks You must release @a pkg_updateinfo_req using package_manager_updateinfo_request_destroy().
 * @param[out] pkg_updateinfo_req The package update info request handle that is newly created on success
 * @return @c 0 on success,
 *         otherwise a negative error value
 * @retval #PACKAGE_MANAGER_ERROR_NONE              Successful
 * @retval #PACKAGE_MANAGER_ERROR_PERMISSION_DENIED Permission denied
 * @retval #PACKAGE_MANAGER_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #PACKAGE_MANAGER_ERROR_OUT_OF_MEMORY     Out of memory
 * @see package_manager_updateinfo_request_destroy()
 */
int package_manager_updateinfo_request_create(package_updateinfo_request_h *pkg_updateinfo_req);

/**
 * @brief Sets the package ID to the package update info request.
 * @since_tizen 4.0
 * @param[in] pkg_updateinfo_req   The update info request handle
 * @param[in] pkgid                The package ID
 * @return @c 0 on success,
 *         otherwise a negative error value
 * @retval #PACKAGE_MANAGER_ERROR_NONE              Successful
 * @retval #PACKAGE_MANAGER_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #PACKAGE_MANAGER_ERROR_OUT_OF_MEMORY     Out of memory
 * @see package_manager_updateinfo_request_create()
 */
int package_manager_updateinfo_set_pkgid(package_updateinfo_request_h pkg_updateinfo_req, const char *pkgid);

/**
 * @brief Sets the package version to the package update info request.
 * @since_tizen 4.0
 * @param[in] pkg_updateinfo_req   The update info request handle
 * @param[in] version              The package version
 * @return @c 0 on success,
 *         otherwise a negative error value
 * @retval #PACKAGE_MANAGER_ERROR_NONE              Successful
 * @retval #PACKAGE_MANAGER_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #PACKAGE_MANAGER_ERROR_OUT_OF_MEMORY     Out of memory
 * @see package_manager_updateinfo_request_create()
 */
int package_manager_updateinfo_set_version(package_updateinfo_request_h pkg_updateinfo_req, const char *version);

/**
 * @brief Sets the package update type to the package update info request.
 * @since_tizen 4.0
 * @param[in] pkg_updateinfo_req   The update info request handle
 * @param[in] type                 The package update type
 * @return @c 0 on success,
 *         otherwise a negative error value
 * @retval #PACKAGE_MANAGER_ERROR_NONE              Successful
 * @retval #PACKAGE_MANAGER_ERROR_INVALID_PARAMETER Invalid parameter
 * @see package_manager_updateinfo_request_create()
 * @see package_updateinfo_type_e
 */
int package_manager_updateinfo_set_type(package_updateinfo_request_h pkg_updateinfo_req, package_updateinfo_type_e type);

/**
 * @brief Requests to register package update info in platform.
 * @since_tizen 4.0
 * @param[in] pkg_updateinfo_req    The update info request handle
 * @return @c 0 on success,
 *         otherwise a negative error value
 * @retval #PACKAGE_MANAGER_ERROR_NONE              Successful
 * @retval #PACKAGE_MANAGER_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #PACKAGE_MANAGER_ERROR_IO_ERROR          I/O error
 * @retval #PACKAGE_MANAGER_ERROR_SYSTEM_ERROR      Severe system error
 * @see package_manager_updateinfo_request_create()
 * @see package_manager_updateinfo_request_destroy()
 */
int package_manager_updateinfo_request_register(package_updateinfo_request_h pkg_updateinfo_req);

/**
 * @brief Destroys the package update info request.
 * @since_tizen 4.0
 * @param[in] pkg_updateinfo_req    The update info request handle
 * @return @c 0 on success,
 *         otherwise a negative error value
 * @retval #PACKAGE_MANAGER_ERROR_NONE              Successful
 * @retval #PACKAGE_MANAGER_ERROR_INVALID_PARAMETER Invalid parameter
 * @see package_manager_updateinfo_request_create()
 */
int package_manager_updateinfo_request_destroy(package_updateinfo_request_h pkg_updateinfo_req);

/**
 * @brief Requests to unregister update info in platform for given package id.
 * @since_tizen 4.0
 * @param[in] pkg_updateinfo_req   The update info request handle
 * @param[in] pkgid                The package id to request unregister update info
 * @return @c 0 on success,
 *         otherwise a negative error value
 * @retval #PACKAGE_MANAGER_ERROR_NONE              Successful
 * @retval #PACKAGE_MANAGER_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #PACKAGE_MANAGER_ERROR_IO_ERROR          I/O error
 * @retval #PACKAGE_MANAGER_ERROR_SYSTEM_ERROR      Severe system error
 */
int package_manager_updateinfo_request_unregister(package_updateinfo_request_h pkg_updateinfo_req, const char *pkgid);

/**
 * @brief Request to unregister update info of all packages in platform.
 * @since_tizen 4.0
 * @param[in] pkg_updateinfo_req    The update info request handle
 * @return @c 0 on success,
 *         otherwise a negative error value
 * @retval #PACKAGE_MANAGER_ERROR_NONE              Successful
 * @retval #PACKAGE_MANAGER_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #PACKAGE_MANAGER_ERROR_IO_ERROR          I/O error
 * @retval #PACKAGE_MANAGER_ERROR_SYSTEM_ERROR      Severe system error
 */
int package_manager_updateinfo_request_unregister_all(package_updateinfo_request_h pkg_updateinfo_req);

/**
 * @platform
 * @brief  Adds resource source and destination path into handle.
 * @details Adds resource source path and destination path to be copied into handle
 * @since_tizen 6.5
 * @privlevel platform
 * @privilege %http://tizen.org/privilege/packagemanager.admin
 * @param[in] request      The package manager request handle
 * @param[in] src_path     The relative path of resource file or directory to be copied
 * @param[in] dest_path    The relative path of destination
 * @return @c 0 on success,
 *         otherwise a negative error value
 * @retval #PACKAGE_MANAGER_ERROR_NONE              Successful
 * @retval #PACKAGE_MANAGER_ERROR_PERMISSION_DENIED Permission denied
 * @retval #PACKAGE_MANAGER_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #PACKAGE_MANAGER_ERROR_SYSTEM_ERROR      Severe system error
 * @see package_manager_request_res_copy_with_cb()
 */
int package_manager_request_add_res_copy_path(package_manager_request_h request, const char *src_path, const char *dest_path);

/**
 * @platform
 * @brief  Copies resources into target directory, asynchronously.
 * @details Copies resources into directory, which could be access via privileged applications only.
 * @since_tizen 6.5
 * @privlevel platform
 * @privilege %http://tizen.org/privilege/packagemanager.admin
 * @param[in] request      The package manager request handle
 * @param[in] callback     The callback function to be invoked
 * @param[in] user_data    The user data to be passed to the callback function
 * @param[out] id        The ID of the request to the package manager
 * @return @c 0 on success,
 *         otherwise a negative error value
 * @retval #PACKAGE_MANAGER_ERROR_NONE              Successful
 * @retval #PACKAGE_MANAGER_ERROR_PERMISSION_DENIED Permission denied
 * @retval #PACKAGE_MANAGER_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #PACKAGE_MANAGER_ERROR_OUT_OF_MEMORY     Out of memory
 * @retval #PACKAGE_MANAGER_ERROR_IO_ERROR          I/O error
 * @retval #PACKAGE_MANAGER_ERROR_SYSTEM_ERROR      Severe system error
 * @see package_manager_request_add_res_copy_path()
 */
int package_manager_request_res_copy_with_cb(package_manager_request_h request, package_manager_request_res_event_cb callback, void *user_data, int *id);

/**
 * @platform
 * @brief  Add directory path to be created at privileged shared directory into handle.
 * @details Added directory path will be created into directory existed for sharing resources via privileged applications
 * @since_tizen 6.5
 * @privlevel platform
 * @privilege %http://tizen.org/privilege/packagemanager.admin
 * @param[in] request      The package manager request handle
 * @param[in] dir_path     The relative path of directories to be created under shared resource path
 * @return @c 0 on success,
 *         otherwise a negative error value
 * @retval #PACKAGE_MANAGER_ERROR_NONE              Successful
 * @retval #PACKAGE_MANAGER_ERROR_PERMISSION_DENIED Permission denied
 * @retval #PACKAGE_MANAGER_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #PACKAGE_MANAGER_ERROR_SYSTEM_ERROR      Severe system error
 * @see package_manager_request_res_create_dir_with_cb()
 */
int package_manager_request_add_res_create_dir_path(package_manager_request_h request, const char *dir_path);

/**
 * @platform
 * @brief  Create directories into directory for sharing resources via privileged application, asynchronously.
 * @details Create directories into certain directory which could be access via privileged applications only.
 * @since_tizen 6.5
 * @privlevel platform
 * @privilege %http://tizen.org/privilege/packagemanager.admin
 * @param[in] request      The package manager request handle
 * @param[in] callback     The callback function to be invoked
 * @param[in] user_data    The user data to be passed to the callback function
 * @param[out] id        The ID of the request to the package manager
 * @return @c 0 on success,
 *         otherwise a negative error value
 * @retval #PACKAGE_MANAGER_ERROR_NONE              Successful
 * @retval #PACKAGE_MANAGER_ERROR_PERMISSION_DENIED Permission denied
 * @retval #PACKAGE_MANAGER_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #PACKAGE_MANAGER_ERROR_OUT_OF_MEMORY     Out of memory
 * @see package_manager_request_add_res_create_dir_path()
 */
int package_manager_request_res_create_dir_with_cb(package_manager_request_h request, package_manager_request_res_event_cb callback, void *user_data, int *id);

/**
 * @platform
 * @brief  Adds file or directory path to be removed located at privileged shared directory into handle.
 * @details Added path will be removed from directory existed for sharing resources via privileged applications
 * @since_tizen 6.5
 * @privlevel platform
 * @privilege %http://tizen.org/privilege/packagemanager.admin
 * @param[in] request      The package manager request handle
 * @param[in] res_path     The relative path of resources to be removed from privileged shared resource directory
 * @return @c 0 on success,
 *         otherwise a negative error value
 * @retval #PACKAGE_MANAGER_ERROR_NONE              Successful
 * @retval #PACKAGE_MANAGER_ERROR_PERMISSION_DENIED Permission denied
 * @retval #PACKAGE_MANAGER_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #PACKAGE_MANAGER_ERROR_SYSTEM_ERROR      Severe system error
 * @see package_manager_request_res_remove_with_cb()
 */
int package_manager_request_add_res_remove_path(package_manager_request_h request, const char *res_path);

/**
 * @platform
 * @brief  Removes resources from for sharing resources via privileged application, asynchronously.
 * @details Removes resources added at handle from certain directory which could be access via privileged applications only.
 * @since_tizen 6.5
 * @privlevel platform
 * @privilege %http://tizen.org/privilege/packagemanager.admin
 * @param[in] request      The package manager request handle
 * @param[in] callback     The callback function to be invoked
 * @param[in] user_data    The user data to be passed to the callback function
 * @param[out] id        The ID of the request to the package manager
 * @return @c 0 on success,
 *         otherwise a negative error value
 * @retval #PACKAGE_MANAGER_ERROR_NONE              Successful
 * @retval #PACKAGE_MANAGER_ERROR_PERMISSION_DENIED Permission denied
 * @retval #PACKAGE_MANAGER_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #PACKAGE_MANAGER_ERROR_OUT_OF_MEMORY     Out of memory
 * @see package_manager_request_add_res_remove_path()
 */
int package_manager_request_res_remove_with_cb(package_manager_request_h request, package_manager_request_res_event_cb callback, void *user_data, int *id);

/**
 * @platform
 * @brief Gets the error code from given resource event handle.
 * @since_tizen 6.5
 * @privlevel platform
 * @privilege %http://tizen.org/privilege/packagemanager.admin
 * @param[in] handle       Resource event info handle
 * @param[out] error       Error will be returned
 * @return @c 0 on success,
 *         otherwise a negative error value
 * @retval #PACKAGE_MANAGER_ERROR_NONE              Successful
 * @retval #PACKAGE_MANAGER_ERROR_PERMISSION_DENIED Permission denied
 * @retval #PACKAGE_MANAGER_ERROR_INVALID_PARAMETER Invalid parameter
 */
int package_manager_res_event_info_get_error_code(package_manager_res_event_info_h handle, package_manager_error_e *error);


/**
 * @brief Called to retrieve all path state about resource event.
 * @since_tizen 6.5
 * @remarks The @a path should not be released. The @a path can be used only in the callback.
 * @param[in] path         The path handled by a resource event
 * @param[in] state        The state of the path
 * @param[in] user_data    The user data passed from the foreach function
 * @return  @c true to continue with the next iteration of the loop,
 *          otherwise @c false to break out of the loop
 * @see package_manager_res_event_info_foreach_path()
 */
typedef bool (*package_manager_res_event_path_cb) (const char *path, package_manager_res_event_path_state_e state, void *user_data);

/**
 * @platform
 * @brief Retrieves all package information of installed packages.
 * @since_tizen 6.5
 * @privlevel platform
 * @privilege %http://tizen.org/privilege/packagemanager.admin
 * @param[in] handle    Resource event info handle
 * @param[in] callback  The callback function to be invoked
 * @param[in] user_data The user data to be passed to the callback function
 * @return @c 0 on success,
 *         otherwise a negative error value
 * @retval #PACKAGE_MANAGER_ERROR_NONE              Successful
 * @retval #PACKAGE_MANAGER_ERROR_PERMISSION_DENIED Permission denied
 * @retval #PACKAGE_MANAGER_ERROR_INVALID_PARAMETER Invalid parameter
 * @post This function invokes package_manager_res_event_path_cb() repeatedly for each path state handled by resource event.
 * @see package_manager_res_event_path_cb()
 */
int package_manager_res_event_info_foreach_path(package_manager_res_event_info_h handle, package_manager_res_event_path_cb callback, void *user_data);

/**
 * @platform
 * @brief Gets the privileged shared resource path for the given package ID.
 * @since_tizen 6.5
 * @privlevel platform
 * @privilege %http://tizen.org/privilege/packagemanager.admin
 * @remarks You must release @a path using free().
 * @param[in]  package_id	The ID of the package
 * @param[out] path			The path of privileged shared resource path of the package
 * @return @c 0 on success,
 *         otherwise a negative error value
 * @retval #PACKAGE_MANAGER_ERROR_NONE              Successful
 * @retval #PACKAGE_MANAGER_ERROR_PERMISSION_DENIED Permission denied
 * @retval #PACKAGE_MANAGER_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #PACKAGE_MANAGER_ERROR_OUT_OF_MEMORY     Out of memory
 */
int package_manager_get_priv_shared_res_path(const char *package_id, char **path);


/**
* @}
*/

#ifdef __cplusplus
}
#endif

#endif /* __TIZEN_APPFW_PACKAGE_MANAGER_H */
