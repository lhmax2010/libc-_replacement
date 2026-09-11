/*
 * storage
 * Copyright (c) 2016 Samsung Electronics Co., Ltd.
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


#ifndef __STORAGE_INTERNAL_H__
#define __STORAGE_INTERNAL_H__

#ifdef __cplusplus
extern "C" {
#endif


/**
 * @addtogroup CAPI_SYSTEM_STORAGE_INTERNAL_MODULE
 * @{
 */

#include <tizen.h>
#include "storage.h"

#define STORAGE_ERROR_NO_DEVICE TIZEN_ERROR_NO_SUCH_DEVICE

/**
 * @brief Gets the mount path for the primary partition of the sdcard.
 * @details Allocates memory dynamically and return the path of the mounted primary sdcard partition to this memory.
 * @since_tizen 3.0
 * @remarks Releases the @a path using free() after completion of usage.
 *
 * @param[out] storage_id The storage id
 * @param[out] path The mount path of sdcard primary partition.
 *
 * @return @c 0 on success,
 *         otherwise a negative error value
 *
 * @retval #STORAGE_ERROR_NONE               Successful
 * @retval #STORAGE_ERROR_INVALID_PARAMETER  Invalid parameter
 * @retval #STORAGE_ERROR_NOT_SUPPORTED      Storage not supported
 * @retval #STORAGE_ERROR_OPERATION_FAILED   Operation failed
 * @retval #STORAGE_ERROR_OUT_OF_MEMORY      Out of memory
 * @code
 *      int storage_id;
 *      char *path;
 *
 *      storage_get_primary_sdcard(&storage_id, &path);
 *      free(path);
 * @endcode
 */
int storage_get_primary_sdcard(int *storage_id, char **path);

/**
 * @brief Retrieves the compat path from origin multi user path.
 * @details Gets compat path of origin multi user path from TZ_USER_CONTENT/.. to /opt/usr/media/.. path.
 * @remarks Only apps and user session daemons are allowed to use. Otherwise, return error.
 *
 * @param[in] origin original path
 * @param[in] len the maximun length of compat path
 * @param[out] compat compat path
 *
 * @return @c 0 on success,
 *         @c -1 on fail
 *
 * @see storage_get_origin_internal_path()
 * @code
 *      const char *src = "/opt/usr/home/owner/media";
 *      char dest[100];
 *      if(storage_get_compat_internal_path(src, sizeof(dest), dest) < 0)
 *              // cannot convert. use src path
 *      else
 *              // can convert. use dest path
 * @endcode
 */
int storage_get_compat_internal_path(const char* origin, int len, char* compat);

/**
 * @brief Retrieves the original path from compat multi user path.
 * @details Gets original path of compat multi user path from /opt/usr/media/.. to TZ_USER_CONTENT/.. path.
 * @remarks Only apps and user session daemons are allowed to use. Otherwise, return error.
 *
 * @param[in] compat compat path
 * @param[in] len the maximun length of origin path
 * @param[out] origin origin path
 *
 * @return @c 0 on success,
 *         @c -1 on fail
 *
 * @see storage_get_compat_internal_path()
 * @code
 *      const char *src = "/opt/usr/media";
 *      char dest[100];
 *      if(storage_get_origin_internal_path(src, sizeof(dest), dest) < 0)
 *              // cannot convert. use src path
 *      else
 *              // can convert. use dest path
 * @endcode
 */
int storage_get_origin_internal_path(const char* compat, int len, char* origin);

/**
 * @brief Retrieves the storage level (such as Warning or Critical) of the path.
 * @details Allocates memory dynamically and return the storage level of the specified path in string format.
 * @remarks Releases the @a level using free() after completion of usage.
 *
 * @param[in] path path for the storage level
 * @param[out] level the storage level of the path
 *
 * @return @c 0 on success,
 *         otherwise a negative error value
 *
 * @retval #STORAGE_ERROR_NONE               Successful
 * @retval #STORAGE_ERROR_INVALID_PARAMETER  Invalid parameter
 * @retval #STORAGE_ERROR_OPERATION_FAILED   Operation failed
 * @retval #STORAGE_ERROR_OUT_OF_MEMORY      Out of memory
 * @code
 *      const char *path = "/opt";
 *      char *level;
 *
 *      storage_get_storage_level(path, &level);
 *      free(level);
 * @endcode
 */
int storage_get_storage_level(const char *path, char **level);

/**
 * @brief This structure defines the data for receive result of mmc operations(mount/unmount/format)
 */
struct mmc_contents {
	void (*mmc_cb) (int result, void* data); /**< user callback function for receive result of mmc operations */
	void* user_data; /**< input data for callback function's second-param(data) */
};

/**
 * @brief Used to mount mmc of the primary partition of the sdcard.
 * @details Gets a callback to obtain mmc mount result of the sdcard.
 * @remarks Inserts a callback to know the completion of mount operation.
 *
 * @param[in] mmc_data for receive result of mount operation
 *
 * @return @c 0 on success,
 *         otherwise a negative error value
 *
 * @retval #STORAGE_ERROR_NONE               Successful
 * @retval #STORAGE_ERROR_INVALID_PARAMETER  Invalid parameter
 * @retval #STORAGE_ERROR_NO_DEVICE          No such device
 * @retval #STORAGE_ERROR_OPERATION_FAILED   Operation failed
 * @retval #STORAGE_ERROR_OUT_OF_MEMORY      Out of memory
 * @see storage_request_unmount_mmc()
 * @code
 *      static void storage_sd_finish_cb(int result, void *data)
 *      {
 *      }
 *
 *      struct mmc_contents mmc_data;
 *	mmc_data.mmc_cb = storage_sd_finish_cb;
 *      mmc_data.user_data = NULL;
 *
 *      storage_request_mount_mmc(&mmc_data);
 * @endcode
 */
int storage_request_mount_mmc(struct mmc_contents *mmc_data);

/**
 * @brief Used to unmount mmc of the primary partition of the sdcard.
 * @details Gets a callback to obtain mmc unmount result of the sdcard.
 * @remarks Inserts a callback to know the completion of unmount operation.
 *
 * @param[in] mmc_data for receive result of unmount operation
 * @param[in] option type of unmount option \n
 *		0 : Normal unmount \n
 *			(if other process still access a sdcard, \n
 *			 unmount will be failed.) \n
 *		1 : Force unmount \n
 *			(if other process still access a sdcard, \n
 *			this process will be received SIGTERM or SIGKILL.)
 *
 * @return @c 0 on success,
 *         otherwise a negative error value
 *
 * @retval #STORAGE_ERROR_NONE               Successful
 * @retval #STORAGE_ERROR_INVALID_PARAMETER  Invalid parameter
 * @retval #STORAGE_ERROR_NO_DEVICE          No such device
 * @retval #STORAGE_ERROR_OPERATION_FAILED   Operation failed
 * @retval #STORAGE_ERROR_OUT_OF_MEMORY      Out of memory
 * @see storage_request_mount_mmc()
 * @code
 *      static void storage_sd_finish_cb(int result, void *data)
 *      {
 *      }
 *
 *      struct mmc_contents mmc_data;
 *	mmc_data.mmc_cb = storage_sd_finish_cb;
 *      mmc_data.user_data = NULL;
 *
 *      storage_request_unmount_mmc(&mmc_data, 0);
 * @endcode
 */
int storage_request_unmount_mmc(struct mmc_contents *mmc_data, int option);

/**
 * @brief Used to format mmc of the primary partition of the sdcard.
 * @details Gets a callback to obtain mmc format result of the sdcard.
 * @remarks Insert a callback to know the completion of format operation.
 *
 * @param[in] mmc_data for receive result of format operation
 *
 * @return @c 0 on success,
 *         otherwise a negative error value
 *
 * @retval #STORAGE_ERROR_NONE               Successful
 * @retval #STORAGE_ERROR_INVALID_PARAMETER  Invalid parameter
 * @retval #STORAGE_ERROR_NO_DEVICE          No such device
 * @retval #STORAGE_ERROR_OPERATION_FAILED   Operation failed
 * @retval #STORAGE_ERROR_OUT_OF_MEMORY      Out of memory
 * @see storage_format_mmc()
 * @code
 *      static void storage_sd_finish_cb(int result, void *data)
 *      {
 *      }
 *
 *      struct mmc_contents mmc_data;
 *	mmc_data.mmc_cb = storage_sd_finish_cb;
 *      mmc_data.user_data = NULL;
 *
 *      storage_request_format_mmc(&mmc_data);
 * @endcode
 */
int storage_request_format_mmc(struct mmc_contents *mmc_data);

/**
 * @brief Used to format mmc of the sdcard with a specific option.
 * @details Gets a callback to obtain mmc format result of the sdcard.
 * @remarks Insert a callback to know the completion of format operation.
 *
 * @param[in] mmc_data for receive result of format operation
 * @param[in] option FMT_NORMAL is 0, FMT_FORCE is 1
 *
 * @return @c 0 on success,
 *         otherwise a negative error value
 *
 * @retval #STORAGE_ERROR_NONE               Successful
 * @retval #STORAGE_ERROR_INVALID_PARAMETER  Invalid parameter
 * @retval #STORAGE_ERROR_NO_DEVICE          No such device
 * @retval #STORAGE_ERROR_OPERATION_FAILED   Operation failed
 * @retval #STORAGE_ERROR_OUT_OF_MEMORY      Out of memory
 * @see storage_request_format_mmc()
 * @code
 *      static void storage_sd_finish_cb(int result, void *data)
 *      {
 *      }
 *
 *      struct mmc_contents mmc_data;
 *	mmc_data.mmc_cb = storage_sd_finish_cb;
 *      mmc_data.user_data = NULL;
 *
 *      storage_format_mmc(&mmc_data, 1);
 * @endcode
 */
int storage_format_mmc(struct mmc_contents *mmc_data, int option);

typedef enum {
	STORAGE_PART_ERROR = -1, /**< Checking partition is failed */
	STORAGE_PART_NOT_MOUNTED = 0, /**< Partition is not mounted */
	STORAGE_PART_MOUNTED = 1, /**< Partition is mounted */
	STORAGE_PART_NOT_SUPPORTED = 2, /**< Partition is not supported */
} storage_part_mount_e;

/**
 * @brief Used to check user data partition is mounted.
 * @details Gets whether or not the user data partition is mounted as storage_part_mount_e.
 * @remarks Confirms success through @a mounted without checking the return value.
 *
 * @param[out] mounted 1 when user partition is mounted, 0 when user partition is not mounted, 2 when user partition is not supported(2 partitions), and Less then 0 when error return.
 *
 * @return @c 0 on success,
 *         otherwise a negative error value
 *
 * @retval #STORAGE_ERROR_NONE               Successful
 * @retval #STORAGE_ERROR_INVALID_PARAMETER  Invalid parameter
 * @retval #STORAGE_ERROR_OPERATION_FAILED   Operation failed
 * @retval #STORAGE_ERROR_OUT_OF_MEMORY      Out of memory
 * @code
 *      storage_part_mount_e mount;
 *
 *      storage_is_mounted_opt_usr(&mount);
 *      switch (mount) {
 *      case STORAGE_PART_ERROR:
 *              // Handling error case
 *              break;
 *      case STORAGE_PART_NOT_MOUNTED:
 *              // Handling not mounted case
 *              break;
 *      case STORAGE_PART_MOUNTED:
 *              // Handling mounted case
 *              break;
 *      case STORAGE_PART_NOT_SUPPORTED:
 *              // Handling not supported case
 *              break;
 *      }
 * @endcode
 */
int storage_is_mounted_opt_usr(storage_part_mount_e *mounted);

/**
 * @}
 */

#ifdef __cplusplus
}
#endif
#endif /* __STORAGE_INTERNAL_H__ */
