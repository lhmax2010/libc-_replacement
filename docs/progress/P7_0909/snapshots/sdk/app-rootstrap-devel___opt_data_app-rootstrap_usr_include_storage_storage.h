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


#ifndef __STORAGE_H__
#define __STORAGE_H__

#ifdef __cplusplus
extern "C" {
#endif


/**
 * @file storage.h
 * @ingroup CAPI_SYSTEM_STORAGE_MODULE
 * @brief This file contains the API for the status of devices.
 * @author TIZEN
 * @date 2013-02-15
 * @version 0.1
 */


 /**
 * @addtogroup CAPI_SYSTEM_STORAGE_MODULE
 * @{
 */


#include <sys/statvfs.h>
#include <storage-expand.h>


/**
 * @brief Gets the size of the internal storage located in the TZ_SYS_USER path.
 * @details Retrieves of the internal storage information using statvfs() syscall \n
 *          and returns the information stored in the struct statvfs structure.
 * @since_tizen 2.3
 * @remarks Does not require internal storage path as input. \n
 *          It always uses the path stored in the TZ_SYS_USER environment variable.
 * @param[out] buf A pointer to a statvfs structure
 * @return @c 0 on success,
 *         otherwise a negative error value on failure
 * @retval #STORAGE_ERROR_NONE Successful
 * @retval #STORAGE_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #STORAGE_ERROR_OPERATION_FAILED Operation failed
 * @par Example:
 * @code
 *	...
 *  struct statvfs s;
 *	if (storage_get_internal_memory_size(&s) < 0)
 *	dlog_print(DLOG_DEBUG, LOG_TAG, "Fail to get internal memory size");
 *	else
 *		dlog_print(DLOG_DEBUG, LOG_TAG, "Total mem : %lf, Avail mem : %lf",
 *				(double)s.f_frsize*s.f_blocks, (double)s.f_bsize*s.f_bavail);
 *	...
 * @endcode
 */
#ifdef __USE_FILE_OFFSET64
#define storage_get_internal_memory_size storage_get_internal_memory_size64
extern int storage_get_internal_memory_size64(struct statvfs *buf);
#else
extern int storage_get_internal_memory_size(struct statvfs *buf);
#endif


/**
 * @brief Gets the size of the primary external storage if external storage is supported.
 * @details Retrieves of the external storage information \n
 *          and returns the information stored in the struct statvfs structure.
 * @since_tizen 2.3
 * @remarks Returns #STORAGE_ERROR_NOT_SUPPORTED
 *          if device does not support external storage feature.
 * @param[out] buf A pointer to a statvfs structure
 * @return @c 0 on success,
 *         otherwise a negative error value on failure
 * @retval #STORAGE_ERROR_NONE Successful
 * @retval #STORAGE_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #STORAGE_ERROR_OPERATION_FAILED Operation failed
 * @retval #STORAGE_ERROR_NOT_SUPPORTED Storage not supported
 * @par Example:
 * @code
 *	...
 *  struct statvfs s;
 *	if (storage_get_external_memory_size(&s) < 0)
 *		dlog_print(DLOG_DEBUG, LOG_TAG, "Fail to get external memory size");
 *	else
 *		dlog_print(DLOG_DEBUG, LOG_TAG, "Total mem : %lf, Avail mem : %lf",
 *				(double)s.f_frsize*s.f_blocks, (double)s.f_bsize*s.f_bavail);
 *	...
 * @endcode
 */
#ifdef __USE_FILE_OFFSET64
#ifdef __REDIRECT_NTH
extern int __REDIRECT_NTH(storage_get_external_memory_size,
				(struct statvfs *buf), storage_get_external_memory_size64)
	__nonnull((1));
#else
#define storage_get_external_memory_size storage_get_external_memory_size64
#endif
extern int storage_get_external_memory_size64(struct statvfs *buf);
#else
extern int storage_get_external_memory_size(struct statvfs *buf);
#endif


/**
 * @}
 */

#ifdef __cplusplus
}
#endif
#endif
