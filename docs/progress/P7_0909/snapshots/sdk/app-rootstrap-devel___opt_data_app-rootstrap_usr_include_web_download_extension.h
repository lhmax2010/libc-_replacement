/*
 * Copyright (c) 2023 Samsung Electronics Co., Ltd All Rights Reserved
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

#ifndef __TIZEN_WEB_DOWNLOAD_EXTENSION_H__
#define __TIZEN_WEB_DOWNLOAD_EXTENSION_H__

#ifdef __cplusplus
extern "C"
{
#endif

/**
 * @file download_extension.h
 */

 /**
 * @addtogroup CAPI_WEB_DOWNLOAD_MODULE
 * @{
 */


/**
 * @brief Sets the max size of the cache.
 *
 * @since_tizen 8.0
 * @privlevel public
 * @privilege %http://tizen.org/privilege/download
 *
 * @remarks The default size is 1000 MB.
 *
 * @param[in] size  The max size of the cache (MB)
 *
 * @return @c 0 on success,
 *         otherwise a negative error value
 *
 * @retval #DOWNLOAD_ERROR_NONE              Successful
 * @retval #DOWNLOAD_ERROR_NOT_SUPPORTED     Not supported
 * @retval #DOWNLOAD_ERROR_PERMISSION_DENIED Permission denied
 * @retval #DOWNLOAD_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #DOWNLOAD_ERROR_ID_NOT_FOUND      No download ID
 *
 * @see download_get_cache_max_size()
 */
int download_set_cache_max_size(unsigned int size);


/**
 * @brief Gets the max size of the cache.
 *
 * @since_tizen 8.0
 * @privlevel public
 * @privilege %http://tizen.org/privilege/download
 *
 * @param[out] size  The max size of the cache (MB)
 *
 * @return @c 0 on success,
 *         otherwise a negative error value
 *
 * @retval #DOWNLOAD_ERROR_NONE              Successful
 * @retval #DOWNLOAD_ERROR_NOT_SUPPORTED     Not supported
 * @retval #DOWNLOAD_ERROR_PERMISSION_DENIED Permission denied
 * @retval #DOWNLOAD_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #DOWNLOAD_ERROR_ID_NOT_FOUND      No download ID
 *
 * @see download_set_cache_max_size()
 */
int download_get_cache_max_size(unsigned int *size);


/**
 * @brief Clears all cache.
 *
 * @since_tizen 8.0
 * @privlevel public
 * @privilege %http://tizen.org/privilege/download
 *
 * @return 0 on success, otherwise a negative error value
 *
 * @retval #DOWNLOAD_ERROR_NONE              Successful
 * @retval #DOWNLOAD_ERROR_NOT_SUPPORTED     Not supported
 * @retval #DOWNLOAD_ERROR_PERMISSION_DENIED Permission denied
 * @retval #DOWNLOAD_ERROR_ID_NOT_FOUND      No download ID
 */
int download_reset_all_cache(void);


/**
 * @brief Sets the path of the cache stored.
 *
 * @since_tizen 8.0
 * @privlevel public
 * @privilege %http://tizen.org/privilege/download
 *
 * @param[in] path  the path of the cache stored
 *
 * @return @c 0 on success,
 *         otherwise a negative error value
 *
 * @retval #DOWNLOAD_ERROR_NONE              Successful
 * @retval #DOWNLOAD_ERROR_NOT_SUPPORTED     Not supported
 * @retval #DOWNLOAD_ERROR_PERMISSION_DENIED Permission denied
 * @retval #DOWNLOAD_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #DOWNLOAD_ERROR_ID_NOT_FOUND      No download ID
 *
 * @see download_get_cache_path()
 */
int download_set_cache_path(const char *path);


/**
 * @brief Gets the path of the cache stored.
 *
 * @since_tizen 8.0
 * @privlevel public
 * @privilege %http://tizen.org/privilege/download
 *
 * @param[out] path  the path of the cache stored
 *
 * @return @c 0 on success,
 *         otherwise a negative error value
 *
 * @retval #DOWNLOAD_ERROR_NONE              Successful
 * @retval #DOWNLOAD_ERROR_NOT_SUPPORTED     Not supported
 * @retval #DOWNLOAD_ERROR_PERMISSION_DENIED Permission denied
 * @retval #DOWNLOAD_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #DOWNLOAD_ERROR_ID_NOT_FOUND      No download ID
 *
 * @see download_set_cache_path()
 */
int download_get_cache_path(char **path);


/**
 * @brief Sets the life cycle of the cache.
 *
 * @since_tizen 8.0
 * @privlevel public
 * @privilege %http://tizen.org/privilege/download
 *
 * @remarks The default time is 48 * 3600 seconds.
 *
 * @param[in] time  The life cycle of the cache (seconds)
 *
 * @return @c 0 on success,
 *         otherwise a negative error value
 *
 * @retval #DOWNLOAD_ERROR_NONE              Successful
 * @retval #DOWNLOAD_ERROR_NOT_SUPPORTED     Not supported
 * @retval #DOWNLOAD_ERROR_PERMISSION_DENIED Permission denied
 * @retval #DOWNLOAD_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #DOWNLOAD_ERROR_ID_NOT_FOUND      No download ID
 *
 * @see download_get_cache_lifecycle()
 */
int download_set_cache_lifecycle(unsigned int time);


/**
 * @brief Gets the life cycle of the cache.
 *
 * @since_tizen 8.0
 * @privlevel public
 * @privilege %http://tizen.org/privilege/download
 *
 * @param[out] time  The life cycle of the cache (seconds)
 *
 * @return @c 0 on success,
 *         otherwise a negative error value
 *
 * @retval #DOWNLOAD_ERROR_NONE              Successful
 * @retval #DOWNLOAD_ERROR_NOT_SUPPORTED     Not supported
 * @retval #DOWNLOAD_ERROR_PERMISSION_DENIED Permission denied
 * @retval #DOWNLOAD_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #DOWNLOAD_ERROR_ID_NOT_FOUND      No download ID
 *
 * @see download_set_cache_lifecycle()
 */
int download_get_cache_lifecycle(unsigned int *time);

/**
 * @brief Sets the download rate limit in kilobytes per second.
 *
 * @since_tizen 10.0
 * @privlevel public
 * @privilege %http://tizen.org/privilege/download
 *
 * @param[in] download_id  The download ID
 * @param[in] rate_kbps    Maximum rate (in kilobytes per second) for download
 *
 * @return @c 0 on success,
 *         otherwise a negative error value
 *
 * @retval #DOWNLOAD_ERROR_NONE              Successful
 * @retval #DOWNLOAD_ERROR_PERMISSION_DENIED Permission denied
 * @retval #DOWNLOAD_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #DOWNLOAD_ERROR_ID_NOT_FOUND      No download ID
 *
 * @see download_get_limit_rate()
 */
int download_set_limit_rate(int download_id, long rate_kbps);

/**
 * @brief Gets the download rate limit in kilobytes per second.
 *
 * @since_tizen 10.0
 * @privlevel public
 * @privilege %http://tizen.org/privilege/download
 *
 * @param[in]  download_id  The download ID
 * @param[out] rate_kbps    Maximum rate (in kilobytes per second) for download
 *
 * @return @c 0 on success,
 *         otherwise a negative error value
 *
 * @retval #DOWNLOAD_ERROR_NONE              Successful
 * @retval #DOWNLOAD_ERROR_PERMISSION_DENIED Permission denied
 * @retval #DOWNLOAD_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #DOWNLOAD_ERROR_ID_NOT_FOUND      No download ID
 *
 * @see download_set_limit_rate()
 */
int download_get_limit_rate(int download_id, long *rate_kbps);
/**
 * @}
 */

#ifdef __cplusplus
}
#endif

#endif /* __TIZEN_WEB_DOWNLOAD_EXTENSION_H__ */
