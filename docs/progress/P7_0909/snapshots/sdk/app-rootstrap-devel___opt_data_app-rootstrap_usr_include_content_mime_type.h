/*
 * Copyright (c) 2011 - 2021 Samsung Electronics Co., Ltd All Rights Reserved
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

#ifndef __TIZEN_CONTENT_MIME_TYPE_H__
#define __TIZEN_CONTENT_MIME_TYPE_H__

#include <sys/types.h>
#include <tizen.h>

#ifdef __cplusplus
extern "C"
{
#endif

/**
 * @addtogroup CAPI_CONTENT_MIME_TYPE_MODULE
 * @{
 */


/**
 * @brief Enumeration of error code.
 * @since_tizen 2.3
 */
typedef enum {
	MIME_TYPE_ERROR_NONE = TIZEN_ERROR_NONE, /**< Successful */
	MIME_TYPE_ERROR_INVALID_PARAMETER = TIZEN_ERROR_INVALID_PARAMETER, /**< Invalid parameter */
	MIME_TYPE_ERROR_OUT_OF_MEMORY = TIZEN_ERROR_OUT_OF_MEMORY, /**< Out of memory */
	MIME_TYPE_ERROR_IO_ERROR = TIZEN_ERROR_IO_ERROR, /**< Internal I/O error */
	MIME_TYPE_ERROR_PERMISSION_DENIED = TIZEN_ERROR_PERMISSION_DENIED, /**< No access for the file (Since 4.0) */
} mime_type_error_e;


/**
 * @brief Gets the MIME type for the given file extension.
 *
 * @since_tizen 2.3
 * @remarks The @a mime_type must be released with free() by you.
 * @param[in] file_extension The file extension without the leading dot ('.')
 * @param[out] mime_type The MIME type for the given file extension \n
 * The MIME type is 'application/octet-stream' if the given file extension is not associated with specific file formats
 * @return 0 on success, otherwise a negative error value.
 * @retval #MIME_TYPE_ERROR_NONE Successful
 * @retval #MIME_TYPE_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #MIME_TYPE_ERROR_OUT_OF_MEMORY Out of memory
 * @retval #MIME_TYPE_ERROR_IO_ERROR Internal I/O error
 * @see mime_type_get_file_extension()
 */
int mime_type_get_mime_type(const char *file_extension, char **mime_type);


/**
 * @brief Gets the file extensions for the given MIME type.
 *
 * @since_tizen 2.3
 * @remarks The @a file_extension must be released with free() by you.
 * @param[in] mime_type The MIME type
 * @param[out] file_extension The array of file extension without the leading dot ('.')
 * @param[out] length The length of the array of file extension or zero if there is none
 * @return 0 on success, otherwise a negative error value.
 * @retval #MIME_TYPE_ERROR_NONE Successful
 * @retval #MIME_TYPE_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #MIME_TYPE_ERROR_OUT_OF_MEMORY Out of memory
 * @retval #MIME_TYPE_ERROR_IO_ERROR Internal I/O error
 * @see mime_type_get_mime_type()
 */
int mime_type_get_file_extension(const char *mime_type, char ***file_extension, int *length);

/**
 * @brief Gets the MIME type for the given data.
 *
 * @since_tizen 4.0
 * @remarks The @a mime_type must be released with free() by you.
 * @param[in] data The stream of file data
 * @param[in] len The length of data
 * @param[out] mime_type The MIME type for the given file extension \n
 * The MIME type is 'application/octet-stream' if the given file extension is not associated with specific file formats
 * @return 0 on success, otherwise a negative error value.
 * @retval #MIME_TYPE_ERROR_NONE Successful
 * @retval #MIME_TYPE_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #MIME_TYPE_ERROR_OUT_OF_MEMORY Out of memory
 * @retval #MIME_TYPE_ERROR_IO_ERROR Internal I/O error
 */
int mime_type_get_mime_type_for_data(const void *data, size_t len, char **mime_type);

/**
 * @brief Gets the MIME type for the given file path.
 *
 * @since_tizen 4.0
 * @remarks The @a mime_type must be released with free() by you.
 * @remarks %http://tizen.org/privilege/mediastorage is needed if input or output path are relevant to media storage.
 * @remarks %http://tizen.org/privilege/externalstorage is needed if input or output path are relevant to external storage.
 * @param[in] file_path The path of target file
 * @param[out] mime_type The MIME type for the given file extension \n
 * The MIME type is 'application/octet-stream' if the given file extension is not associated with specific file formats
 * @return 0 on success, otherwise a negative error value.
 * @retval #MIME_TYPE_ERROR_NONE Successful
 * @retval #MIME_TYPE_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #MIME_TYPE_ERROR_OUT_OF_MEMORY Out of memory
 * @retval #MIME_TYPE_ERROR_IO_ERROR Internal I/O error
 * @retval #MIME_TYPE_ERROR_PERMISSION_DENIED Permission denied
 */
int mime_type_get_mime_type_for_file(const char *file_path, char **mime_type);

/**
 * @}
 */

#ifdef __cplusplus
}
#endif

#endif /* __TIZEN_CONTENT_MIME_TYPE_H__ */
