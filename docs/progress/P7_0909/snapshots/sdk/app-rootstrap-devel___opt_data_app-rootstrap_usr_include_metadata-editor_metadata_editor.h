/*
* Copyright (c) 2013 Samsung Electronics Co., Ltd All Rights Reserved
*
* Licensed under the Apache License, Version 2.0 (the "License");
* you may not use this file except in compliance with the License.
* You may obtain a copy of the License at
*
* http://www.apache.org/licenses/LICENSE-2.0
*
* Unless required by applicable law or agreed to in writing, software
* distributed under the License is distributed on an "AS IS" BASIS,
* WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
* See the License for the specific language governing permissions and
* limitations under the License.
*/

#ifndef __TIZEN_MULTIMEDIA_METADATA_EDITOR_H__
#define __TIZEN_MULTIMEDIA_METADATA_EDITOR_H__


#include <metadata_editor_type.h>


#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

/**
 * @addtogroup CAPI_MEDIA_METADATA_EDITOR_MODULE
 * @{
 *
 * @file metadata_editor.h
 * @brief This file contains functions for editing metadata of MP3 and MP4 files. \n
 *        Since 6.0, this module supports WAV, FLAC and OGG files as well.
 *        Description of metadata: title, album, artist, author, genre and description etc. \n
 */


/**
 * @deprecated Deprecated since 9.0.
 * @brief Creates metadata.
 * @since_tizen 2.4
 *
 * @remarks The @a metadata should be released using metadata_editor_destroy().
 *
 * @param[in] metadata The handle to metadata
 * @return 0 on success, otherwise a negative error value
 * @retval #METADATA_EDITOR_ERROR_NONE Successful
 * @retval #METADATA_EDITOR_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #METADATA_EDITOR_ERROR_OUT_OF_MEMORY Out of memory
 * @see metadata_editor_destroy()
 */
int metadata_editor_create(metadata_editor_h *metadata) TIZEN_DEPRECATED_API;


/**
 * @deprecated Deprecated since 9.0.
 * @brief Sets file path to read or write metadata.
 * @since_tizen 2.4
 *
 * @remarks In case of accessing specific path in internal storage or external storage, you may add the privilege for accessing the path. \n
 *                   For example, if you get the specific path by using storage_get_directory(). you should add privilege %http://tizen.org/privilege/mediastorage or %http://tizen.org/privilege/externalstorage.
 *
 * @param[in] metadata The handle to metadata
 * @param[in] path path to read or write metadata
 * @return 0 on success, otherwise a negative error value
 * @retval #METADATA_EDITOR_ERROR_NONE Successful
 * @retval #METADATA_EDITOR_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #METADATA_EDITOR_ERROR_FILE_EXISTS File not exist
 * @retval #METADATA_EDITOR_ERROR_NOT_SUPPORTED unsupported file type
 * @retval #METADATA_EDITOR_ERROR_PERMISSION_DENIED Permission denied
 * @retval #METADATA_EDITOR_ERROR_OPERATION_FAILED Internal Operation Fail
 * @pre Create metadata handle by calling metadata_editor_create()
 * @see metadata_editor_create()
 * @see metadata_editor_destroy()
 */
int metadata_editor_set_path(metadata_editor_h metadata, const char *path) TIZEN_DEPRECATED_API;


/**
 * @deprecated Deprecated since 9.0.
 * @brief Gets the metadata corresponding to the attribute.
 * @since_tizen 2.4
 *
 * @remarks The @a value should be released using free(). \n
 *                   If the attribute value of the metadata is empty, return value is NULL. \n
 *                   In case of accessing specific path in internal storage or external storage, you may add the privilege for accessing the path. \n
 *                   For example, if you get the specific path by using storage_get_directory(). you should add privilege %http://tizen.org/privilege/mediastorage or %http://tizen.org/privilege/externalstorage.
 *
 * @param[in] metadata The handle to metadata
 * @param[in] attribute key attribute name to get
 * @param[out] value The value of the attribute
 * @return 0 on success, otherwise a negative error value
 * @retval #METADATA_EDITOR_ERROR_NONE Successful
 * @retval #METADATA_EDITOR_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #METADATA_EDITOR_ERROR_OUT_OF_MEMORY Out of memory
 * @retval #METADATA_EDITOR_ERROR_PERMISSION_DENIED Permission denied
 * @retval #METADATA_EDITOR_ERROR_OPERATION_FAILED Internal Operation Fail
 * @pre Set path to read or write metadata by calling metadata_editor_set_path()
 * @see metadata_editor_create()
 * @see metadata_editor_set_path()
 * @see metadata_editor_destroy()
 */
int metadata_editor_get_metadata(metadata_editor_h metadata, metadata_editor_attr_e attribute, char **value) TIZEN_DEPRECATED_API;


/**
 * @deprecated Deprecated since 9.0.
 * @brief Sets the attribute of the metadata.
 * @details This function set the attribute of the metadata for updating the metadata. \n
 *            If @a attribute is #METADATA_EDITOR_ATTR_PICTURE_NUM, this function returns #METADATA_EDITOR_ERROR_INVALID_PARAMETER.
 * @since_tizen 2.4
 *
 * @remarks The @a value should be released using free(). \n
 *               You must call metadata_editor_update_metadata() to apply metadata changes to the media file. Otherwise, the previous metadata will be returned when you call metadata_editor_get_metadata().
 *               This function doesn't support updating metadata of an invalid media file.
 *               Before 6.0, this function returned #METADATA_EDITOR_ERROR_OPERATION_FAILED. Since 6.0, #METADATA_EDITOR_ERROR_METADATA_UPDATE_NOT_POSSIBLE is returned in such cases.
 *
 * @param[in] metadata The handle to metadata
 * @param[in] attribute key attribute name to get
 * @param[in] value The value of the attribute
 * @return 0 on success, otherwise a negative error value
 * @retval #METADATA_EDITOR_ERROR_NONE Successful
 * @retval #METADATA_EDITOR_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #METADATA_EDITOR_ERROR_OUT_OF_MEMORY Out of memory
 * @retval #METADATA_EDITOR_ERROR_OPERATION_FAILED Internal Operation Fail
 * @retval #METADATA_EDITOR_ERROR_METADATA_UPDATE_NOT_POSSIBLE Update not possible
 * @pre Set path to read or write metadata by calling metadata_editor_set_path()
 * @see metadata_editor_create()
 * @see metadata_editor_set_path()
 * @see metadata_editor_update_metadata()
 * @see metadata_editor_destroy()
 */
int metadata_editor_set_metadata(metadata_editor_h metadata, metadata_editor_attr_e attribute, const char *value) TIZEN_DEPRECATED_API;


/**
 * @deprecated Deprecated since 9.0.
 * @brief Updates the modified metadata.
 * @details This function update the metadata in the media file that is modified by metadata_editor_set_metadata().
 * @since_tizen 2.4
 *
 * @remarks In case of accessing specific path in internal storage or external storage, you may add the privilege for accessing the path. \n
 *                   For example, if you get the specific path by using storage_get_directory(). you should add privilege %http://tizen.org/privilege/mediastorage or %http://tizen.org/privilege/externalstorage.
 *
 * @param[in] metadata The handle to metadata
 * @return 0 on success, otherwise a negative error value
 * @retval #METADATA_EDITOR_ERROR_NONE Successful
 * @retval #METADATA_EDITOR_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #METADATA_EDITOR_ERROR_OUT_OF_MEMORY Out of memory
 * @retval #METADATA_EDITOR_ERROR_PERMISSION_DENIED Permission denied
 * @retval #METADATA_EDITOR_ERROR_OPERATION_FAILED Internal Operation Fail
 * @pre Set path to read or write metadata by calling metadata_editor_set_path()
 * @see metadata_editor_create()
 * @see metadata_editor_set_path()
 * @see metadata_editor_destroy()
 */
int metadata_editor_update_metadata(metadata_editor_h metadata) TIZEN_DEPRECATED_API;

/**
 * @deprecated Deprecated since 9.0.
 * @brief Gets the picture in the media file.
 * @details If there is no picture or if the @a index is invalid, this function returns #METADATA_EDITOR_ERROR_INVALID_PARAMETER.\n
 *            You can get picture number by using metadata_editor_get_metadata() and index starts with 0.
 * @since_tizen 2.4
 *
 * @remarks The @a picture and @a mime_type should be released using free(). \n
 *                   In case of accessing specific path in internal storage or external storage, you may add the privilege for accessing the path. \n
 *                   For example, if you get the specific path by using storage_get_directory(). you should add privilege %http://tizen.org/privilege/mediastorage or %http://tizen.org/privilege/externalstorage.
 *
 * @param[in] metadata The handle to metadata
 * @param[in] index picture order (starts with 0)
 * @param[out] picture encoded picture
 * @param[out] size encoded picture size
 * @param[out] mime_type the MIME of the picture
 * @return 0 on success, otherwise a negative error value
 * @retval #METADATA_EDITOR_ERROR_NONE Successful
 * @retval #METADATA_EDITOR_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #METADATA_EDITOR_ERROR_OUT_OF_MEMORY Out of memory
 * @retval #METADATA_EDITOR_ERROR_OPERATION_FAILED Internal Operation Fail
 * @retval #METADATA_EDITOR_ERROR_PERMISSION_DENIED Permission denied
 * @pre Set path to read or write metadata by calling metadata_editor_set_path()
 * @see metadata_editor_create()
 * @see metadata_editor_set_path()
 * @see metadata_editor_destroy()
 */
int metadata_editor_get_picture(metadata_editor_h metadata, int index, void **picture, int *size, char **mime_type) TIZEN_DEPRECATED_API;

/**
 * @deprecated Deprecated since 9.0.
 * @brief Appends the picture to the media file.
 * @since_tizen 2.4
 *
 * @remarks You must call metadata_editor_update_metadata() to apply metadata changes to the media file. Otherwise, the previous metadata will be returned when you call metadata_editor_get_metadata(). \n
 *                   Image type of the metadata supports jpeg and png. \n
 *                   In case of accessing specific path in internal storage or external storage, you may add the privilege for accessing the path. \n
 *                   For example, if you get the specific path by using storage_get_directory(). you should add privilege %http://tizen.org/privilege/mediastorage or %http://tizen.org/privilege/externalstorage.
 *                   This function doesn't support updating metadata of an invalid media file.
 *                   Before 6.0, this function returned #METADATA_EDITOR_ERROR_OPERATION_FAILED. Since 6.0, #METADATA_EDITOR_ERROR_METADATA_UPDATE_NOT_POSSIBLE is returned in such cases.
 *
 * @param[in] metadata The handle to metadata
 * @param[in] picture_path The path of picture for adding to the metadata
 * @return 0 on success, otherwise a negative error value
 * @retval #METADATA_EDITOR_ERROR_NONE Successful
 * @retval #METADATA_EDITOR_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #METADATA_EDITOR_ERROR_OUT_OF_MEMORY Out of memory
 * @retval #METADATA_EDITOR_ERROR_NOT_SUPPORTED unsupported file type
 * @retval #METADATA_EDITOR_ERROR_OPERATION_FAILED Internal Operation Fail
 * @retval #METADATA_EDITOR_ERROR_PERMISSION_DENIED Permission denied
 * @retval #METADATA_EDITOR_ERROR_METADATA_UPDATE_NOT_POSSIBLE Update not possible
 * @pre Set path to read or write metadata by calling metadata_editor_set_path()
 * @see metadata_editor_create()
 * @see metadata_editor_set_path()
 * @see metadata_editor_update_metadata()
 * @see metadata_editor_destroy()
 */
int metadata_editor_append_picture(metadata_editor_h metadata, const char *picture_path) TIZEN_DEPRECATED_API;

/**
 * @deprecated Deprecated since 9.0.
 * @brief Removes artwork image from media file.
 * @since_tizen 2.4
 *
 * @remarks You must call metadata_editor_update_metadata() to apply metadata changes to the media file. Otherwise, the previous metadata will be returned when you call metadata_editor_get_metadata(). \n
 *                   In case of accessing specific path in internal storage or external storage, you may add the privilege for accessing the path. \n
 *                   For example, if you get the specific path by using storage_get_directory(). you should add privilege %http://tizen.org/privilege/mediastorage or %http://tizen.org/privilege/externalstorage.
 *
 * @param[in] metadata The handle to metadata
 * @param[in] index artwork image order
 * @return 0 on success, otherwise a negative error value
 * @retval #METADATA_EDITOR_ERROR_NONE Successful
 * @retval #METADATA_EDITOR_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #METADATA_EDITOR_ERROR_OUT_OF_MEMORY Out of memory
 * @retval #METADATA_EDITOR_ERROR_OPERATION_FAILED Internal Operation Fail
 * @retval #METADATA_EDITOR_ERROR_PERMISSION_DENIED Permission denied
 * @pre Set path to read or write metadata by calling metadata_editor_set_path()
 * @see metadata_editor_create()
 * @see metadata_editor_set_path()
 * @see metadata_editor_update_metadata()
 * @see metadata_editor_destroy()
 */
int metadata_editor_remove_picture(metadata_editor_h metadata, int index) TIZEN_DEPRECATED_API;

/**
 * @deprecated Deprecated since 9.0.
 * @brief Destroys metadata.
 * @since_tizen 2.4
 *
 * @param[in] metadata The handle to metadata
 * @return 0 on success, otherwise a negative error value
 * @retval #METADATA_EDITOR_ERROR_NONE Successful
 * @retval #METADATA_EDITOR_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #METADATA_EDITOR_ERROR_OPERATION_FAILED Internal Operation Fail
 * @pre Create metadata handle by calling metadata_editor_create()
 * @see metadata_editor_create()
 */
int metadata_editor_destroy(metadata_editor_h metadata) TIZEN_DEPRECATED_API;

/**
 * @}
 */


#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* __TIZEN_MULTIMEDIA_METADATA_EDITOR_H__ */
