/*
 * Copyright (c) 2019 Samsung Electronics Co., Ltd All Rights Reserved
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

#ifndef __TIZEN_UIX_STICKER_DATA_H__
#define __TIZEN_UIX_STICKER_DATA_H__

#include <sticker_error.h>

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @file sticker_data.h
 * @brief This file contains sticker data APIs and related enumeration.
 */

/**
 * @addtogroup CAPI_UIX_STICKER_DATA_MODULE
 * @{
 */

/**
 * @brief Enumeration for sticker URI type.
 *
 * @since_tizen 5.5
 */
typedef enum {
    STICKER_DATA_URI_LOCAL_PATH = 1, /**< Local path URI */
    STICKER_DATA_URI_WEB_RESOURCE, /**< Web resource URI */
} sticker_data_uri_type_e;

/**
 * @brief Enumeration for sticker display type.
 *
 * @since_tizen 5.5
 */
typedef enum {
    STICKER_DATA_DISP_EMOJI = 1, /**< Emoji type */
    /**
     * @brief Wallpaper type
     * @details The stickers of the wallpaper type are intended to be used by
     * a wallpaper application and usually have larger size.
     */
    STICKER_DATA_DISP_WALLPAPER,
} sticker_data_display_type_e;

/**
 * @brief The sticker data handle.
 * @since_tizen 5.5
 */
typedef struct sticker_data_s *sticker_data_h;

/**
 * @brief Called to retrieve the keyword of the sticker.
 * @details The sticker_data_foreach_keyword() must be called to invoke this callback function, synchronously.
 * @since_tizen 5.5
 * @remarks @a keyword should not be freed and can be used only in the callback.
 * @param[in] keyword The sticker keyword
 * @param[in] user_data The user data passed from the foreach function
 * @pre sticker_data_foreach_keyword() will invoke this callback.
 * @see sticker_data_foreach_keyword()
 */
typedef void (*sticker_data_keyword_foreach_cb)(const char *keyword, void *user_data);

/**
 * @brief Creates a sticker data handle.
 * @since_tizen 5.5
 * @remarks If the function succeeds, @a data_handle must be released with sticker_data_destroy().
 * @param[out] data_handle The sticker data handle
 * @return 0 on success, otherwise a negative error value
 * @retval #STICKER_ERROR_NONE Successful
 * @retval #STICKER_ERROR_NOT_SUPPORTED Not supported
 * @retval #STICKER_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #STICKER_ERROR_OUT_OF_MEMORY Out of memory
 * @retval #STICKER_ERROR_OPERATION_FAILED Operation failed
 * @see sticker_data_destroy()
 */
int sticker_data_create(sticker_data_h *data_handle);

/**
 * @brief Destroys a sticker data handle.
 * @since_tizen 5.5
 * @param[in] data_handle The sticker data handle
 * @return 0 on success, otherwise a negative error value
 * @retval #STICKER_ERROR_NONE Successful
 * @retval #STICKER_ERROR_NOT_SUPPORTED Not supported
 * @retval #STICKER_ERROR_INVALID_PARAMETER Invalid parameter
 * @see sticker_data_create()
 */
int sticker_data_destroy(sticker_data_h data_handle);

/**
 * @brief Clones a sticker data handle.
 * @since_tizen 5.5
 * @remarks If the function succeeds, @a target_handle must be released with sticker_data_destroy().
 * @param[in] origin_handle The sticker data handle
 * @param[out] target_handle The sticker data handle to be cloned
 * @return 0 on success, otherwise a negative error value
 * @retval #STICKER_ERROR_NONE Successful
 * @retval #STICKER_ERROR_NOT_SUPPORTED Not supported
 * @retval #STICKER_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #STICKER_ERROR_OUT_OF_MEMORY Out of memory
 * @see sticker_data_destroy()
 */
int sticker_data_clone(sticker_data_h origin_handle, sticker_data_h *target_handle);

/**
 * @brief Gets the sticker data handle for the given URI.
 * @since_tizen 5.5
 * @remarks If the function succeeds, @a data_handle must be released with sticker_data_destroy().
 * @param[in] uri The URI of the sticker data handle
 * @param[out] data_handle The sticker data handle for the given sticker URI
 * @return 0 on success, otherwise a negative error value
 * @retval #STICKER_ERROR_NONE Successful
 * @retval #STICKER_ERROR_NOT_SUPPORTED Not supported
 * @retval #STICKER_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #STICKER_ERROR_OUT_OF_MEMORY Out of memory
 * @retval #STICKER_ERROR_OPERATION_FAILED Operation failed
 * @retval #STICKER_ERROR_NO_SUCH_FILE A sticker with given @a uri does not exist
 * @see sticker_data_destroy()
 */
int sticker_data_get_handle(const char* uri, sticker_data_h *data_handle);

/**
 * @brief Gets the name of the sticker provider application from sticker data handle.
 * @since_tizen 5.5
 * @remarks @a app_id must be released using free().
 * @param[in] data_handle The sticker data handle
 * @param[out] app_id The name of the application that provides sticker information
 * @return 0 on success, otherwise a negative error value
 * @retval #STICKER_ERROR_NONE Successful
 * @retval #STICKER_ERROR_NOT_SUPPORTED Not supported
 * @retval #STICKER_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #STICKER_ERROR_NO_DATA No data available
 */
int sticker_data_get_app_id(sticker_data_h data_handle, char **app_id);

/**
 * @brief Sets the URI and URI type of the sticker.
 * @details @a uri must be a relative path like '/res/smile.png' when the type of URI is local path.
 * @since_tizen 5.5
 * @remarks @a uri must have a non-null value and must be an existing file. If not, the error as invalid parameter will be returned.
 * @param[in] data_handle The sticker data handle
 * @param[in] type The URI type to be saved
 * @param[in] uri The URI to be saved
 * @return 0 on success, otherwise a negative error value
 * @retval #STICKER_ERROR_NONE Successful
 * @retval #STICKER_ERROR_NOT_SUPPORTED Not supported
 * @retval #STICKER_ERROR_INVALID_PARAMETER Invalid parameter
 * @see sticker_data_get_uri()
 */
int sticker_data_set_uri(sticker_data_h data_handle, sticker_data_uri_type_e type, const char *uri);

/**
 * @brief Gets the URI and URI type from sticker data handle.
 * @since_tizen 5.5
 * @remarks @a uri must be released using free().
 * @param[in] data_handle The sticker data handle
 * @param[out] type The URI type
 * @param[out] uri The URI
 * @return 0 on success, otherwise a negative error value
 * @retval #STICKER_ERROR_NONE Successful
 * @retval #STICKER_ERROR_NOT_SUPPORTED Not supported
 * @retval #STICKER_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #STICKER_ERROR_NO_DATA No data available
 * @see sticker_data_set_uri()
 */
int sticker_data_get_uri(sticker_data_h data_handle, sticker_data_uri_type_e *type, char **uri);

/**
 * @brief Retrieves all keywords of the sticker using callback function.
 * @since_tizen 5.5
 * @param[in] data_handle The sticker data handle
 * @param[in] callback The callback function to invoke
 * @param[in] user_data The user data to be passed to the callback function
 * @return 0 on success, otherwise a negative error value
 * @retval #STICKER_ERROR_NONE Successful
 * @retval #STICKER_ERROR_NOT_SUPPORTED Not supported
 * @retval #STICKER_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #STICKER_ERROR_NO_DATA No data available
 * @post This function invokes sticker_data_keyword_foreach_cb() repeatedly for getting keywords.
 * @see sticker_data_keyword_foreach_cb()
 * @see sticker_data_add_keyword()
 * @see sticker_data_remove_keyword()
 */
int sticker_data_foreach_keyword(sticker_data_h data_handle, sticker_data_keyword_foreach_cb callback, void *user_data);

/**
 * @brief Adds a keyword of the sticker to the list.
 * @since_tizen 5.5
 * @remarks @a keyword must have a non-null value and can not have duplicate value. If not, the error as invalid parameter will be returned.
 * @param[in] data_handle The sticker data handle
 * @param[in] keyword The keyword to be saved
 * @return 0 on success, otherwise a negative error value
 * @retval #STICKER_ERROR_NONE Successful
 * @retval #STICKER_ERROR_NOT_SUPPORTED Not supported
 * @retval #STICKER_ERROR_INVALID_PARAMETER Invalid parameter
 * @see sticker_data_keyword_foreach_cb()
 * @see sticker_data_foreach_keyword()
 * @see sticker_data_remove_keyword()
 */
int sticker_data_add_keyword(sticker_data_h data_handle, const char *keyword);

/**
 * @brief Removes a keyword of the sticker from the list.
 * @since_tizen 5.5
 * @remarks @a keyword must exist value in the list. If not, the error as invalid parameter will be returned.
 * @param[in] data_handle The sticker data handle
 * @param[in] keyword The keyword to be removed
 * @return 0 on success, otherwise a negative error value
 * @retval #STICKER_ERROR_NONE Successful
 * @retval #STICKER_ERROR_NOT_SUPPORTED Not supported
 * @retval #STICKER_ERROR_INVALID_PARAMETER Invalid parameter
 * @see sticker_data_keyword_foreach_cb()
 * @see sticker_data_foreach_keyword()
 * @see sticker_data_add_keyword()
 */
int sticker_data_remove_keyword(sticker_data_h data_handle, const char *keyword);

/**
 * @brief Sets the group name of the sticker.
 * @since_tizen 5.5
 * @remarks @a group must have a non-null value. If not, the error as invalid parameter will be returned.
 * @param[in] data_handle The sticker data handle
 * @param[in] group The group name to be saved
 * @return 0 on success, otherwise a negative error value
 * @retval #STICKER_ERROR_NONE Successful
 * @retval #STICKER_ERROR_NOT_SUPPORTED Not supported
 * @retval #STICKER_ERROR_INVALID_PARAMETER Invalid parameter
 * @see sticker_data_get_group_name()
 */
int sticker_data_set_group_name(sticker_data_h data_handle, const char *group);

/**
 * @brief Gets the group name from sticker data handle.
 * @since_tizen 5.5
 * @remarks @a group must be released using free().
 * @param[in] data_handle The sticker data handle
 * @param[out] group The group name
 * @return 0 on success, otherwise a negative error value
 * @retval #STICKER_ERROR_NONE Successful
 * @retval #STICKER_ERROR_NOT_SUPPORTED Not supported
 * @retval #STICKER_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #STICKER_ERROR_NO_DATA No data available
 * @see sticker_data_set_group_name()
 */
int sticker_data_get_group_name(sticker_data_h data_handle, char **group);

/**
 * @brief Sets the thumbnail local path of the sticker.
 * @details @a thumbnail must be a relative path like '/res/smile_thumbnail.png'.
 * @since_tizen 5.5
 * @remarks @a thumbnail must have a non-null value and must be an existing file. If not, the error as invalid parameter will be returned.
 * @param[in] data_handle The sticker data handle
 * @param[in] thumbnail The thumbnail path to be saved
 * @return 0 on success, otherwise a negative error value
 * @retval #STICKER_ERROR_NONE Successful
 * @retval #STICKER_ERROR_NOT_SUPPORTED Not supported
 * @retval #STICKER_ERROR_INVALID_PARAMETER Invalid parameter
 * @see sticker_data_get_thumbnail()
 */
int sticker_data_set_thumbnail(sticker_data_h data_handle, const char *thumbnail);

/**
 * @brief Gets the thumbnail local path from sticker data handle.
 * @details If the thumbnail is empty, the result will be an empty string.
 * @since_tizen 5.5
 * @remarks @a thumbnail must be released using free().
 * @param[in] data_handle The sticker data handle
 * @param[out] thumbnail The thumbnail path
 * @return 0 on success, otherwise a negative error value
 * @retval #STICKER_ERROR_NONE Successful
 * @retval #STICKER_ERROR_NOT_SUPPORTED Not supported
 * @retval #STICKER_ERROR_INVALID_PARAMETER Invalid parameter
 * @see sticker_data_set_thumbnail()
 */
int sticker_data_get_thumbnail(sticker_data_h data_handle, char **thumbnail);

/**
 * @brief Sets the description of the sticker.
 * @since_tizen 5.5
 * @remarks @a description must have a non-null value. If not, the error as invalid parameter will be returned.
 * @param[in] data_handle The sticker data handle
 * @param[in] description The description to be saved
 * @return 0 on success, otherwise a negative error value
 * @retval #STICKER_ERROR_NONE Successful
 * @retval #STICKER_ERROR_NOT_SUPPORTED Not supported
 * @retval #STICKER_ERROR_INVALID_PARAMETER Invalid parameter
 * @see sticker_data_get_description()
 */
int sticker_data_set_description(sticker_data_h data_handle, const char *description);

/**
 * @brief Gets the description from sticker data handle.
 * @details If the description is empty, the result will be an empty string.
 * @since_tizen 5.5
 * @remarks @a description must be released using free().
 * @param[in] data_handle The sticker data handle
 * @param[out] description The description of the sticker
 * @return 0 on success, otherwise a negative error value
 * @retval #STICKER_ERROR_NONE Successful
 * @retval #STICKER_ERROR_NOT_SUPPORTED Not supported
 * @retval #STICKER_ERROR_INVALID_PARAMETER Invalid parameter
 * @see sticker_data_set_description()
 */
int sticker_data_get_description(sticker_data_h data_handle, char **description);

/**
 * @brief Gets the last updated date from sticker data handle.
 * @details The format of @a date is YYYY-MM-DD HH:MM:SS.
 * @since_tizen 5.5
 * @remarks @a date must be released using free().
 * @param[in] data_handle The sticker data handle
 * @param[out] date The last updated date
 * @return 0 on success, otherwise a negative error value
 * @retval #STICKER_ERROR_NONE Successful
 * @retval #STICKER_ERROR_NOT_SUPPORTED Not supported
 * @retval #STICKER_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #STICKER_ERROR_NO_DATA No data available
 */
int sticker_data_get_date(sticker_data_h data_handle, char **date);

/**
 * @brief Sets the display type of the sticker.
 * @since_tizen 5.5
 * @param[in] data_handle The sticker data handle
 * @param[in] type The display type to be saved
 * @return 0 on success, otherwise a negative error value
 * @retval #STICKER_ERROR_NONE Successful
 * @retval #STICKER_ERROR_NOT_SUPPORTED Not supported
 * @retval #STICKER_ERROR_INVALID_PARAMETER Invalid parameter
 * @see sticker_data_get_display_type()
 */
int sticker_data_set_display_type(sticker_data_h data_handle, sticker_data_display_type_e type);

/**
 * @brief Gets the display type from sticker data handle.
 * @details If the display type is empty, the result will be a zero.
 * @since_tizen 5.5
 * @param[in] data_handle The sticker data handle
 * @param[out] type The display type of the sticker
 * @return 0 on success, otherwise a negative error value
 * @retval #STICKER_ERROR_NONE Successful
 * @retval #STICKER_ERROR_NOT_SUPPORTED Not supported
 * @retval #STICKER_ERROR_INVALID_PARAMETER Invalid parameter
 * @see sticker_data_set_display_type()
 */
int sticker_data_get_display_type(sticker_data_h data_handle, sticker_data_display_type_e *type);

/**
 * @}
 */

#ifdef __cplusplus
}
#endif

#endif /* __TIZEN_UIX_STICKER_DATA_H__ */