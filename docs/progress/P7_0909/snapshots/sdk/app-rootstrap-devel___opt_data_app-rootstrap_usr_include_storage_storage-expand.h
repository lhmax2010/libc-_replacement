/*
 * storage
 * Copyright (c) 2012 Samsung Electronics Co., Ltd.
 *
 * Licensed under the Apache License, Version 2.0 (the License);
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */


#ifndef __STORAGE_EXPAND_H__
#define __STORAGE_EXPAND_H__

#ifdef __cplusplus
extern "C" {
#endif


 /**
 * @addtogroup CAPI_SYSTEM_STORAGE_MODULE
 * @{
 */

#include <tizen.h>


/**
 * @brief Enumeration for Storage of error codes.
 * @since_tizen 2.3
 */
typedef enum {
	STORAGE_ERROR_NONE = TIZEN_ERROR_NONE, /**< Successful */
	STORAGE_ERROR_INVALID_PARAMETER = TIZEN_ERROR_INVALID_PARAMETER, /**< Invalid parameter */
	STORAGE_ERROR_OUT_OF_MEMORY = TIZEN_ERROR_OUT_OF_MEMORY, /**< Out of memory */
	STORAGE_ERROR_NOT_SUPPORTED = TIZEN_ERROR_NO_SUCH_DEVICE, /**< Storage not supported */
	STORAGE_ERROR_OPERATION_FAILED  = TIZEN_ERROR_SYSTEM_CLASS | 0x12, /**< Operation failed */
} storage_error_e;


/**
 * @brief Enumeration for the storage types.
 * @since_tizen 2.3
 */
typedef enum {
	STORAGE_TYPE_INTERNAL,          /**< Internal device storage (built-in storage in a device, non-removable) */
	STORAGE_TYPE_EXTERNAL,          /**< External storage */
	STORAGE_TYPE_EXTENDED_INTERNAL, /**< Extended internal storage (External storage used as internal storage) (Since 4.0) */
} storage_type_e;


/**
 * @brief Enumeration for storage devices state.
 * @since_tizen 2.3
 */
typedef enum {
	STORAGE_STATE_UNMOUNTABLE = -2, /**< Storage is present but cannot be mounted. Typically it happens if the file system of the storage is corrupted */
	STORAGE_STATE_REMOVED = -1, /**< Storage is not present */
	STORAGE_STATE_MOUNTED = 0, /**< Storage is present and mounted with read/write access */
	STORAGE_STATE_MOUNTED_READ_ONLY = 1, /**< Storage is present and mounted with read only access */
} storage_state_e;


/**
 * @brief Called to get information once for each supported storage.
 * @details Called up to the maximum number of supported storages to get information.
 * @since_tizen 2.3
 * @remarks Defined by the storage_foreach_device_supported() caller.
 * @param[in] storage_id The unique storage ID
 * @param[in] type The type of the storage
 * @param[in] state The current state of the storage
 * @param[in] path The absolute path to the root directory of the storage
 * @param[in] user_data The user data passed from the foreach function
 * @return @c true to continue with the next iteration of the loop, \n
 *         otherwise @c false to break out of the loop
 * @pre	storage_foreach_device_supported() will invoke this callback function.
 * @see	storage_foreach_device_supported()
 * @code
 *      my_storage_device_supported_cb(int storage_id, storage_type_e type,
 *              storage_state_e state, const char *path, void *user_data) {
 *              return true;
 *      }
 *
 *      storage_foreach_device_supported(my_storage_device_supported_cb, NULL);
 * @endcode
 */
typedef bool (*storage_device_supported_cb)(int storage_id, storage_type_e type,
		storage_state_e state, const char *path, void *user_data);


/**
 * @brief Retrieves all the storage in a device using a callback function.
 * @details Invokes the callback function once for each storage in a device. \n
 *          If storage_device_supported_cb() returns @c false, then the iteration will be finished.
 * @since_tizen 2.3
 * @remarks A callback function should be defined by this function caller.
 * @param[in] callback The iteration callback function
 * @param[in] user_data The user data to be passed to the callback function
 * @return @c 0 on success,
 *         otherwise a negative error value
 * @retval #STORAGE_ERROR_NONE Successful
 * @retval #STORAGE_ERROR_INVALID_PARAMETER Invalid parameter
 * @post Invokes storage_device_supported_cb() repeatedly for each supported device.
 * @see storage_device_supported_cb()
 * @code
 *      my_storage_device_supported_cb(int storage_id, storage_type_e type,
 *              storage_state_e state, const char *path, void *user_data) {
 *              return true;
 *      }
 *
 *      storage_foreach_device_supported(my_storage_device_supported_cb, NULL);
 * @endcode
 */
int storage_foreach_device_supported(storage_device_supported_cb callback, void *user_data);


/**
 * @brief Gets the absolute path to the root directory of the given storage.
 * @details Determines root directory of internal or external storage using storage_id.
 * @since_tizen 2.3
 * @remarks Files saved on the internal/external storage are readable or writable by all applications.\n
 *          When an application is uninstalled, the files written by that application are not removed from the internal/external storage.\n
 *          If you want to access files or directories in internal storage, you must declare %http://tizen.org/privilege/mediastorage.\n
 *          If you want to access files or directories in external storage, you must declare %http://tizen.org/privilege/externalstorage.\n
 *          Refer to <a href="https://docs.tizen.org/application/native/guides/security/privacy-related-permissions"><b>Privacy-related Permissions</b></a>.\n
 *          You must release @a path using free().
 * @param[in] storage_id The storage device
 * @param[out] path The absolute path to the storage directory
 * @return @c 0 on success,
 *         otherwise a negative error value
 * @retval #STORAGE_ERROR_NONE Successful
 * @retval #STORAGE_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #STORAGE_ERROR_OUT_OF_MEMORY Out of memory
 * @retval #STORAGE_ERROR_NOT_SUPPORTED Storage not supported
 * @retval #STORAGE_ERROR_OPERATION_FAILED Operation failed
 * @see storage_get_state()
 * @code
 *      int storage_id = 0;
 *      char path[100];
 *
 *      storage_get_root_directory(storage_id, &path)
 * @endcode
 */
int storage_get_root_directory(int storage_id, char **path);


/**
 * @brief Enumeration for the storage directory types.
 * @since_tizen 2.3
 */
typedef enum {
	STORAGE_DIRECTORY_IMAGES, /**< Image directory */
	STORAGE_DIRECTORY_SOUNDS, /**< Sounds directory */
	STORAGE_DIRECTORY_VIDEOS, /**< Videos directory */
	STORAGE_DIRECTORY_CAMERA, /**< Camera directory */
	STORAGE_DIRECTORY_DOWNLOADS, /**< Downloads directory */
	STORAGE_DIRECTORY_MUSIC, /**< Music directory */
	STORAGE_DIRECTORY_DOCUMENTS, /**< Documents directory */
	STORAGE_DIRECTORY_OTHERS, /**< Others directory */
	STORAGE_DIRECTORY_SYSTEM_RINGTONES, /**< System ringtones directory. Only available for internal storage. */
	STORAGE_DIRECTORY_MAX
} storage_directory_e;


/**
 * @brief Gets the absolute path to the each directory of the given storage.
 * @details Gets absolute path among several directories defined in the #storage_directory_e.
 * @since_tizen 2.3
 * @remarks Files saved on the internal/external storage are readable or writable by all applications.\n
 *          When an application is uninstalled, the files written by that application are not removed from the internal/external storage.\n
 *          The directory path may not exist, so you must make sure that it exists before using it.\n
 *          If you want to access files or directories in internal storage, you must declare %http://tizen.org/privilege/mediastorage.\n
 *          If you want to access files or directories in external storage, you must declare %http://tizen.org/privilege/externalstorage.\n
 *          Refer to <a href="https://docs.tizen.org/application/native/guides/security/privacy-related-permissions"><b>Privacy-related Permissions</b></a>.\n
 *          You must release @a path using free().
 * @param[in] storage_id The storage device
 * @param[in] type The directory type
 * @param[out] path The absolute path to the directory type
 * @return @c 0 on success,
 *         otherwise a negative error value
 * @retval #STORAGE_ERROR_NONE Successful
 * @retval #STORAGE_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #STORAGE_ERROR_OUT_OF_MEMORY Out of memory
 * @retval #STORAGE_ERROR_NOT_SUPPORTED Storage not supported
 * @retval #STORAGE_ERROR_OPERATION_FAILED Operation failed
 * @see storage_get_state()
 * @code
 *      int storage_id = 0;
 *      char path[100];
 *
 *      storage_get_directory(storage_id, STORAGE_DIRECTORY_IMAGES, &path)
 * @endcode
 */
int storage_get_directory(int storage_id, storage_directory_e type, char **path);


/**
 * @brief Gets the type (such as internal or external) of the given storage.
 * @details Gets the storage type among defined in the #storage_type_e using storage_id.
 * @since_tizen 2.3
 * @remarks Determines whether it is an internal or external storage using the input storage_id.
 * @param[in] storage_id The storage device
 * @param[out] type The type of the storage
 * @return @c 0 on success,
 *         otherwise a negative error value
 * @retval #STORAGE_ERROR_NONE Successful
 * @retval #STORAGE_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #STORAGE_ERROR_OUT_OF_MEMORY Out of memory
 * @retval #STORAGE_ERROR_NOT_SUPPORTED Storage not supported
 * @retval #STORAGE_ERROR_OPERATION_FAILED Operation failed
 *
 * @code
 *      int storage_id = 0;
 *      storage_type_e type;
 *
 *      storage_get_type(storage_id, &type);
 * @endcode
 */
int storage_get_type(int storage_id, storage_type_e *type);


/**
 * @brief Gets the current mounted state of storage corresponding to the specified id.
 * @details Gets the storage mounted state among defined in the #storage_state_e using storage_id.
 * @since_tizen 2.3
 * @remarks Determines whether it is an internal or external storage using the input storage_id.
 * @param[in] storage_id The storage device
 * @param[out] state The current state of the storage
 * @return @c 0 on success,
 *         otherwise a negative error value
 * @retval #STORAGE_ERROR_NONE Successful
 * @retval #STORAGE_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #STORAGE_ERROR_OUT_OF_MEMORY Out of memory
 * @retval #STORAGE_ERROR_NOT_SUPPORTED Storage not supported
 * @retval #STORAGE_ERROR_OPERATION_FAILED Operation failed
 * @see storage_get_root_directory()
 * @see storage_get_total_space()
 * @see storage_get_available_space()
 * @code
 *      int storage_id = 0;
 *      storage_state_e state;
 *
 *      storage_get_state(storage_id, &state);
 * @endcode
 */
int storage_get_state(int storage_id, storage_state_e *state);


/**
 * @brief Called every time when the state of specific storage changes.
 * @details Called every time the state of a specific storage changes,
 *          and when it is called, it retrieves the id and state of the storage.
 * @since_tizen 2.3
 * @remarks Defined by the storage_set_state_changed_cb() and
 *          storage_unset_state_changed_cb() caller.
 * @param[in] storage_id The unique storage ID
 * @param[in] state The current state of the storage
 * @param[in] user_data The user data passed from the foreach function
 * @pre	storage_set_state_changed_cb() will invoke this callback function.
 * @see	storage_set_state_changed_cb()
 * @see	storage_unset_state_changed_cb()
 * @code
 *      my_storage_state_changed_cb(int storage_id, storage_state_e state, void *user_data) {
 *      }
 *
 *      storage_set_state_changed_cb(0, my_storage_state_changed_cb, NULL);
 * @endcode
 */
typedef void (*storage_state_changed_cb)(int storage_id, storage_state_e state, void *user_data);


/**
 * @brief Registers a callback function to be invoked when the state of the specific storage changes.
 * @details Invokes the callback function each time for a specific storage whose state changes.
 * @since_tizen 2.3
 * @remarks A callback function should be defined by this function caller.
 * @param[in] storage_id The storage device
 * @param[in] callback The callback function to register
 * @param[in] user_data The user data to be passed to the callback function
 * @return @c 0 on success,
 *         otherwise a negative error value
 * @retval #STORAGE_ERROR_NONE Successful
 * @retval #STORAGE_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #STORAGE_ERROR_NOT_SUPPORTED Storage not supported
 * @retval #STORAGE_ERROR_OPERATION_FAILED Operation failed
 * @post storage_state_changed_cb() will be invoked if the state of the registered storage changes.
 * @see storage_state_changed_cb()
 * @see storage_unset_state_changed_cb()
 * @code
 *      my_storage_state_changed_cb(int storage_id, storage_state_e state, void *user_data) {
 *      }
 *
 *      storage_set_state_changed_cb(0, my_storage_state_changed_cb, NULL);
 * @endcode
 */
int storage_set_state_changed_cb(int storage_id, storage_state_changed_cb callback, void *user_data);


/**
 * @brief Unregisters the callback function to stop calling the callback.
 * @details Prevents the callback function from being called any further.
 * @since_tizen 2.3
 * @remarks A callback function should be defined by this function caller.
 * @param[in] storage_id The storage device to monitor
 * @param[in] callback The callback function to register
 * @return @c 0 on success,
 *         otherwise a negative error value
 * @retval #STORAGE_ERROR_NONE Successful
 * @retval #STORAGE_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #STORAGE_ERROR_NOT_SUPPORTED Storage not supported
 * @retval #STORAGE_ERROR_OPERATION_FAILED Operation failed
 * @see storage_state_changed_cb()
 * @see storage_set_state_changed_cb()
 * @code
 *      my_storage_state_changed_cb(int storage_id, storage_state_e state, void *user_data) {
 *      }
 *
 *      storage_set_state_changed_cb(0, my_storage_state_changed_cb, NULL);
 *      storage_unset_state_changed_cb(0, my_storage_state_changed_cb);
 * @endcode
 */
int storage_unset_state_changed_cb(int storage_id, storage_state_changed_cb callback);


/**
 * @brief Enumeration for storage device types.
 * @since_tizen 3.0
 */
typedef enum {
	STORAGE_DEV_EXT_SDCARD = 1001,     /**< SD card device (external storage) */
	STORAGE_DEV_EXT_USB_MASS_STORAGE,  /**< USB storage device (external storage) */
	STORAGE_DEV_EXTENDED_INTERNAL,     /**< Extended internal storage device (External storage used as internal storage) (Since 4.0) */
} storage_dev_e;


/**
 * @brief Called each time when the state of a specific storage type changes.
 * @details Called every time the state of specific storage type changes.
 * @since_tizen 3.0
 * @remarks Defined by the storage_set_changed_cb() and
 *          storage_unset_changed_cb() caller.
 * @param[in] storage_id The unique storage ID
 * @param[in] dev The type of the external storage device
 * @param[in] state The state of the storage
 * @param[in] fstype The type of the file system
 * @param[in] fsuuid The uuid of the file system
 * @param[in] mountpath The mount path of the file system
 * @param[in] primary The primary partition
 * @param[in] flags The flags for the storage status
 * @param[in] user_data The user data
 * @pre	storage_set_changed_cb() will invoke this callback function.
 * @see	storage_set_changed_cb()
 * @see	storage_unset_changed_cb()
 * @code
 *      my_storage_changed_cb(int storage_id, storage_dev_e dev, storage_state_e state,
 *                const char *fstype, const char *fsuuid, const char *mountpath,
 *                bool primary, int flags, void *user_data) {
 *      }
 *
 *      storage_set_changed_cb(STORAGE_TYPE_EXTERNAL, my_storage_changed_cb, NULL);
 * @endcode
 */
typedef void (*storage_changed_cb)(int storage_id,
		storage_dev_e dev, storage_state_e state,
		const char *fstype, const char *fsuuid, const char *mountpath,
		bool primary, int flags, void *user_data);


/**
 * @brief Registers a callback function to be invoked when the state of the specified storage device type changes.
 * @details Invokes the callback function each time for specific storage type whose state changes.
 * @since_tizen 3.0
 * @remarks A callback function should be defined by this function caller.
 * @param[in] type The type of the storage device
 * @param[in] callback The callback function to register
 * @param[in] user_data The user data to be passed to the callback function
 * @return @c 0 on success,
 *         otherwise a negative error value
 * @retval #STORAGE_ERROR_NONE Successful
 * @retval #STORAGE_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #STORAGE_ERROR_NOT_SUPPORTED Storage not supported
 * @retval #STORAGE_ERROR_OPERATION_FAILED Operation failed
 * @post storage_changed_cb() will be invoked if the state of the registered storage type changes.
 * @see storage_changed_cb()
 * @see storage_unset_changed_cb()
 * @code
 *      my_storage_changed_cb(int storage_id, storage_dev_e dev, storage_state_e state,
 *                const char *fstype, const char *fsuuid, const char *mountpath,
 *                bool primary, int flags, void *user_data) {
 *      }
 *
 *      storage_set_changed_cb(STORAGE_TYPE_EXTERNAL, my_storage_changed_cb, NULL);
 * @endcode
 */
int storage_set_changed_cb(storage_type_e type, storage_changed_cb callback, void *user_data);


/**
 * @brief Unregisters the callback function for storage type state changes.
 * @details Prevents the callback function from being called any further.
 * @since_tizen 3.0
 * @remarks A callback function should be defined by this function caller.
 * @param[in] type The type of the the storage device
 * @param[in] callback The callback function to unregister
 * @return @c 0 on success,
 *         otherwise a negative error value
 * @retval #STORAGE_ERROR_NONE Successful
 * @retval #STORAGE_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #STORAGE_ERROR_NOT_SUPPORTED Storage not supported
 * @retval #STORAGE_ERROR_OPERATION_FAILED Operation failed
 * @see storage_changed_cb()
 * @see storage_set_changed_cb()
 * @code
 *      my_storage_changed_cb(int storage_id, storage_dev_e dev, storage_state_e state,
 *                const char *fstype, const char *fsuuid, const char *mountpath,
 *                bool primary, int flags, void *user_data) {
 *      }
 *
 *      storage_set_changed_cb(STORAGE_TYPE_EXTERNAL, my_storage_changed_cb, NULL);
 *      storage_unset_changed_cb(STORAGE_TYPE_EXTERNAL, my_storage_changed_cb);
 * @endcode
 */
int storage_unset_changed_cb(storage_type_e type, storage_changed_cb callback);


/**
 * @brief Gets the total space of the given storage in bytes using id.
 * @details Finds target storage using storage_id and gets the total space of a storage in bytes.
 * @since_tizen 2.3
 * @remarks Determines whether it is an internal or external storage using the input storage_id.
 * @param[in] storage_id The storage device
 * @param[out] bytes The total space size of the storage (bytes)
 * @return @c 0 on success,
 *         otherwise a negative error value
 * @retval #STORAGE_ERROR_NONE Successful
 * @retval #STORAGE_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #STORAGE_ERROR_OUT_OF_MEMORY Out of memory
 * @retval #STORAGE_ERROR_NOT_SUPPORTED Storage not supported
 * @retval #STORAGE_ERROR_OPERATION_FAILED Operation failed
 * @see storage_get_state()
 * @see storage_get_available_space()
 * @code
 *      int storage_id = 0;
 *      unsigned long long bytes;
 *
 *      storage_get_total_space(storage_id, &bytes);
 * @endcode
 */
int storage_get_total_space(int storage_id, unsigned long long *bytes);


/**
 * @brief Gets the available space size of the given storage in bytes.
 * @details Gets the available space of a storage in bytes using storage_id.
 * @since_tizen 2.3
 * @remarks Determines whether it is an internal or external storage using the input storage_id.
 * @param[in] storage_id The storage device
 * @param[out] bytes The available space size of the storage (bytes)
 * @return @c 0 on success,
 *         otherwise a negative error value
 * @retval #STORAGE_ERROR_NONE Successful
 * @retval #STORAGE_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #STORAGE_ERROR_OUT_OF_MEMORY Out of memory
 * @retval #STORAGE_ERROR_NOT_SUPPORTED Storage not supported
 * @retval #STORAGE_ERROR_OPERATION_FAILED Operation failed
 * @see storage_get_state()
 * @see storage_get_total_space()
 * @code
 *      int storage_id = 0;
 *      unsigned long long bytes;
 *
 *      storage_get_available_space(storage_id, &bytes);
 * @endcode
 */
int storage_get_available_space(int storage_id, unsigned long long *bytes);


/**
 * @brief Gets the type and the kind of external device for the given storage id.
 * @details Gets the external storage type defined in #storage_type_e and kind
 *          defined in #storage_dev_e using storage_id.
 * @since_tizen 5.0
 * @remarks Works solely on external storages, not internal storages.
 *          If @a type is #STORAGE_TYPE_INTERNAL, this function returns #STORAGE_ERROR_INVALID_PARAMETER and @a dev is unchanged.
 *
 * @param[in] storage_id The storage id
 * @param[out] type The storage @a type (internal or external). If @a type is #STORAGE_TYPE_INTERNAL, this function returns #STORAGE_ERROR_INVALID_PARAMETER and @a dev is unchanged.
 * @param[out] dev The storage device for external storage.
 *
 * @return @c 0 on success,
 *         otherwise a negative error value
 *
 * @retval #STORAGE_ERROR_NONE               Successful
 * @retval #STORAGE_ERROR_INVALID_PARAMETER  Invalid parameter
 * @retval #STORAGE_ERROR_OUT_OF_MEMORY      Out of memory
 * @retval #STORAGE_ERROR_NOT_SUPPORTED      Storage not supported
 * @retval #STORAGE_ERROR_OPERATION_FAILED   Operation failed
 *
 * @code
 *      int storage_id = 0;
 *      storage_type_e type;
 *      storage_dev_e dev;
 *
 *      storage_get_type_dev(storage_id, &type, &dev);
 * @endcode
 */
int storage_get_type_dev(int storage_id, storage_type_e *type, storage_dev_e *dev);


/**
 * @}
 */

#ifdef __cplusplus
}
#endif
#endif
