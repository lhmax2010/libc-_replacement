/*
 * Copyright (c) 2018 Samsung Electronics Co., Ltd All Rights Reserved
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

#ifndef __TIZEN_UIX_STICKER_PROVIDER_H__
#define __TIZEN_UIX_STICKER_PROVIDER_H__

#include <sticker_error.h>
#include <sticker_data.h>

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @file sticker_provider.h
 * @brief This file contains sticker provider's APIs.
 */

/**
 * @addtogroup CAPI_UIX_STICKER_PROVIDER_MODULE
 * @{
 */

/**
 * @brief The sticker provider handle.
 * @since_tizen 5.5
 */
typedef struct sticker_provider_s *sticker_provider_h;

/**
 * @brief Called to retrieve all sticker data in the sticker database.
 * @details The sticker_provider_data_foreach_all() must be called to invoke this callback function, synchronously.
 * @since_tizen 5.5
 * @remarks @a data_handle should not be freed and can be used only in the callback.
 *          If you want to use it outside of the callback, you need to use a clone which can be obtained sticker_data_clone().
 * @param[in] data_handle The sticker data handle
 * @param[in] user_data The user data passed from the foreach function
 * @pre sticker_provider_data_foreach_all() will invoke this callback.
 * @see sticker_provider_data_foreach_all()
 */
typedef void (*sticker_provider_data_foreach_cb)(sticker_data_h data_handle, void *user_data);

/**
 * @brief Called when inserting sticker data is finished.
 * @details The following error codes can be received: \n
 *          #STICKER_ERROR_NONE                 Successful \n
 *          #STICKER_ERROR_OPERATION_FAILED     Operation failed \n
 * @since_tizen 5.5
 * @param[in] error The sticker error code
 * @param[in] user_data The user data passed from the foreach function
 * @pre sticker_provider_insert_data_by_json_file() will invoke this callback.
 * @see sticker_provider_insert_data_by_json_file()
 */
typedef void (*sticker_provider_insert_finished_cb)(sticker_error_e error, void *user_data);

/**
 * @brief Creates a sticker provider handle.
 * @since_tizen 5.5
 * @remarks If the function succeeds, @a provider_handle must be released with sticker_provider_destroy().
 * @param[out] provider_handle The sticker provider handle
 * @return 0 on success, otherwise a negative error value
 * @retval #STICKER_ERROR_NONE Successful
 * @retval #STICKER_ERROR_NOT_SUPPORTED Not supported
 * @retval #STICKER_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #STICKER_ERROR_OUT_OF_MEMORY Out of memory
 * @retval #STICKER_ERROR_OPERATION_FAILED Operation failed
 * @see sticker_provider_destroy()
 */
int sticker_provider_create(sticker_provider_h *provider_handle);

/**
 * @brief Destroys a sticker provider handle.
 * @since_tizen 5.5
 * @param[in] provider_handle The sticker provider handle
 * @return 0 on success, otherwise a negative error value
 * @retval #STICKER_ERROR_NONE Successful
 * @retval #STICKER_ERROR_NOT_SUPPORTED Not supported
 * @retval #STICKER_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #STICKER_ERROR_OPERATION_FAILED Operation failed
 * @see sticker_provider_create()
 */
int sticker_provider_destroy(sticker_provider_h provider_handle);

/**
 * @brief Inserts a sticker data to the sticker database.
 * @since_tizen 5.5
 * @remarks All data except thumbnail, description, and display_type must be set in the @a data_handle to insert the sticker data.
 *          If the URI type is #STICKER_DATA_URI_LOCAL_PATH, the sticker file is copied to a sticker directory.
 *          It is recommended to delete your sticker file after inserting a sticker data.
 * @param[in] provider_handle The sticker provider handle
 * @param[in] data_handle The sticker data handle to be saved
 * @return 0 on success, otherwise a negative error value
 * @retval #STICKER_ERROR_NONE Successful
 * @retval #STICKER_ERROR_NOT_SUPPORTED Not supported
 * @retval #STICKER_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #STICKER_ERROR_OPERATION_FAILED Operation failed
 * @retval #STICKER_ERROR_FILE_EXISTS File exists
 * @retval #STICKER_ERROR_NO_SUCH_FILE No such file
 * @see sticker_provider_update_data()
 * @see sticker_provider_delete_data()
 */
int sticker_provider_insert_data(sticker_provider_h provider_handle, sticker_data_h data_handle);

/**
 * @brief Inserts a sticker data using JSON file.
 * @details @a json_path must be a relative path like '/data/message_sticker.json'.
 * @since_tizen 5.5
 * @remarks All data except thumbnail, description, and display_type must be set in the JSON file to insert the sticker data.
 *          @a json_path must have a non-null value and must be an existing file. If not, the error as invalid parameter will be returned.
 *          If the URI type is #STICKER_DATA_URI_LOCAL_PATH, the sticker file is copied to a sticker directory.
 *          It is recommended to delete your sticker files after inserting a sticker data.
 * @param[in] provider_handle The sticker provider handle
 * @param[in] json_path The path of JSON file containing sticker information to be saved
 * @param[in] callback The callback function to invoke
 * @param[in] user_data The user data to be passed to the callback function
 * @return 0 on success, otherwise a negative error value
 * @retval #STICKER_ERROR_NONE Successful
 * @retval #STICKER_ERROR_NOT_SUPPORTED Not supported
 * @retval #STICKER_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #STICKER_ERROR_OPERATION_FAILED Operation failed
 * @post This function invokes sticker_provider_insert_finished_cb().
 * @see sticker_provider_insert_finished_cb()
 *
 * @code
 //JSON file format
 {
    "sticker":
    [
        {
            "type" : 1,
            "uri" : "/res/face/heart_eyes.png",
            "keyword" : ["heart eyes", "love", "cute"],
            "group" : "face",
            "thumbnail" : "/res/face/thumbnail/heart_eyes.png",
            "description" : "Smiling face with heart eyes emoji.",
            "display_type" : 1
        },
        {
            "type" : 2,
            "uri" : "https://samsung.com/example/01/high_five.png",
            "keyword" : ["smile", "high five"],
            "group" : "face",
            "thumbnail" : "",
            "description" : "Smiling face with high five emoji.",
            "display_type" : null
        },
            .....
        {
            .....
        }
    ]
 }
 * @endcode
 */
int sticker_provider_insert_data_by_json_file(sticker_provider_h provider_handle, const char *json_path, sticker_provider_insert_finished_cb callback, void *user_data);

/**
 * @brief Updates a sticker data in the sticker database.
 * @since_tizen 5.5
 * @param[in] provider_handle The sticker provider handle
 * @param[in] data_handle The sticker data handle to be updated
 * @return 0 on success, otherwise a negative error value
 * @retval #STICKER_ERROR_NONE Successful
 * @retval #STICKER_ERROR_NOT_SUPPORTED Not supported
 * @retval #STICKER_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #STICKER_ERROR_OPERATION_FAILED Operation failed
 * @retval #STICKER_ERROR_FILE_EXISTS File exists
 * @retval #STICKER_ERROR_NO_SUCH_FILE No such file
 * @see sticker_provider_insert_data()
 * @see sticker_provider_delete_data()
 */
int sticker_provider_update_data(sticker_provider_h provider_handle, sticker_data_h data_handle);

/**
 * @brief Deletes a sticker data in the sticker database.
 * @since_tizen 5.5
 * @remarks The @a data_handle must be the handle of the sticker stored in the sticker database.
 * @param[in] provider_handle The sticker provider handle
 * @param[in] data_handle The sticker data handle to be deleted
 * @return 0 on success, otherwise a negative error value
 * @retval #STICKER_ERROR_NONE Successful
 * @retval #STICKER_ERROR_NOT_SUPPORTED Not supported
 * @retval #STICKER_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #STICKER_ERROR_OPERATION_FAILED Operation failed
 * @see sticker_provider_insert_data()
 * @see sticker_provider_update_data()
 */
int sticker_provider_delete_data(sticker_provider_h provider_handle, sticker_data_h data_handle);

/**
 * @brief Deletes a sticker with the given @a uri from the sticker database.
 * @details If the database does not have a sticker that matches the given @a uri, the #STICKER_ERROR_NO_SUCH_FILE error is returned.
 * @since_tizen 5.5
 * @param[in] provider_handle The sticker provider handle
 * @param[in] uri The URI of the sticker data to be deleted
 * @return 0 on success, otherwise a negative error value
 * @retval #STICKER_ERROR_NONE Successful
 * @retval #STICKER_ERROR_NOT_SUPPORTED Not supported
 * @retval #STICKER_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #STICKER_ERROR_OPERATION_FAILED Operation failed
 * @retval #STICKER_ERROR_NO_SUCH_FILE No such file
 * @see sticker_provider_insert_data()
 * @see sticker_provider_delete_data()
 */
int sticker_provider_delete_data_by_uri(sticker_provider_h provider_handle, const char *uri);

/**
 * @brief Gets the count of stickers stored by the provider application.
 * @since_tizen 5.5
 * @param[in] provider_handle The sticker provider handle
 * @param[out] count The number of stickers
 * @return 0 on success, otherwise a negative error value
 * @retval #STICKER_ERROR_NONE Successful
 * @retval #STICKER_ERROR_NOT_SUPPORTED Not supported
 * @retval #STICKER_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #STICKER_ERROR_OPERATION_FAILED Operation failed
 */
int sticker_provider_get_sticker_count(sticker_provider_h provider_handle, int *count);

/**
 * @brief Retrieves all sticker data in the sticker database.
 * @details If you set the @a offset as @c 10 and @a count as @c 10, then only searched data from @c 10 to @c 19 will be invoked.
 * @since_tizen 5.5
 * @remarks It is not an error if @a result is smaller than @a count.
 * @param[in] provider_handle The sticker provider handle
 * @param[in] offset The start position (Starting from zero)
 * @param[in] count The number of stickers to be searched with respect to the offset
 * @param[out] result The number of stickers retrieved (zero indicates that no data was found)
 * @param[in] callback The callback function to invoke
 * @param[in] user_data The user data to be passed to the callback function
 * @return 0 on success, otherwise a negative error value
 * @retval #STICKER_ERROR_NONE Successful
 * @retval #STICKER_ERROR_NOT_SUPPORTED Not supported
 * @retval #STICKER_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #STICKER_ERROR_OUT_OF_MEMORY Out of memory
 * @retval #STICKER_ERROR_OPERATION_FAILED Operation failed
 * @post This function invokes sticker_provider_data_foreach_cb() repeatedly for getting data.
 * @see sticker_provider_data_foreach_cb()
 */
int sticker_provider_data_foreach_all(sticker_provider_h provider_handle, int offset, int count, int *result, sticker_provider_data_foreach_cb callback, void *user_data);

/**
 * @brief Sets the image of the sticker group.
 * @details @a uri must be a relative path like '/res/smile.png' when @a type is #STICKER_DATA_URI_LOCAL_PATH.
 * @since_tizen 6.5
 * @param[in] provider_handle The sticker provider handle
 * @param[in] group The group name for setting group image
 * @param[in] type The URI type of the image file
 * @param[in] uri The URI of the image file
 * @return 0 on success, otherwise a negative error value
 * @retval #STICKER_ERROR_NONE Successful
 * @retval #STICKER_ERROR_NOT_SUPPORTED Not supported
 * @retval #STICKER_ERROR_INVALID_PARAMETER Invalid parameter, e.g. @a group is @c NULL or the group name does not exist in the database
 * @retval #STICKER_ERROR_OPERATION_FAILED Operation failed
 * @retval #STICKER_ERROR_NO_SUCH_FILE No such file, e.g. @a uri is @c NULL or the specified file does not exist
 * @see sticker_provider_insert_data()
 */
int sticker_provider_set_group_image(sticker_provider_h provider_handle, const char *group, sticker_data_uri_type_e type, const char *uri);

/**
 * @}
 */

#ifdef __cplusplus
}
#endif

#endif /* __TIZEN_UIX_STICKER_PROVIDER_H__ */
