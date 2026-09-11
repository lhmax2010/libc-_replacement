/*
* Copyright (c) 2011 Samsung Electronics Co., Ltd All Rights Reserved
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

#ifndef __TIZEN_MULTIMEDIA_MEDIA_CONTROLLER_METADATA_H__
#define __TIZEN_MULTIMEDIA_MEDIA_CONTROLLER_METADATA_H__

#include <media_controller_type.h>
#include <bundle.h>

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */



/**
 * @file media_controller_metadata.h
 * @brief This file contains the media controller metadata API and functions related with handling media control.
 */

/**
 * @addtogroup CAPI_MEDIA_CONTROLLER_METADATA_MODULE
 * @{
 */

/**
 * @brief Clones the metadata handle of media.
 * @details This function copies the metadata handle from one to another.
 *            The #mc_metadata_h is created internally and therefore available through the functions of media controller metadata.
 *
 * @since_tizen 4.0
 *
 * @remarks The @a dst should be released using mc_metadata_destroy().
 *
 * @param[in] src The source handle of the media controller metadata
 * @param[out] dst The destination handle of the media controller metadata
 *
 * @return @c 0 on success,
 *         otherwise a negative error value
 *
 * @retval #MEDIA_CONTROLLER_ERROR_NONE Successful
 * @retval #MEDIA_CONTROLLER_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #MEDIA_CONTROLLER_ERROR_OUT_OF_MEMORY Out of memory
 *
 * @see mc_metadata_destroy()
 */
int mc_metadata_clone(mc_metadata_h src, mc_metadata_h *dst);

/**
 * @brief Destroys a media controller metadata handle.
 * @details This function frees all resources related to the media controller metadata handle. This
 *          handle no longer can be used to perform any operations. A new handle has to
 *          be created before next usage.
 *
 * @since_tizen 4.0
 *
 * @param[in] metadata        The handle of the media controller metadata
 *
 * @return @c 0 on success,
 *         otherwise a negative error value
 *
 * @retval #MEDIA_CONTROLLER_ERROR_NONE Successful
 * @retval #MEDIA_CONTROLLER_ERROR_INVALID_PARAMETER Invalid parameter
 *
 * @see mc_metadata_clone()
 */
int mc_metadata_destroy(mc_metadata_h metadata);

/**
 * @brief Gets the metadata.
 * @since_tizen 4.0
 *
 * @remarks The @a value should be released using free(). \n
 *              If the attribute value of the metadata is empty, return value is NULL.
 *
 * @param[in] metadata    The handle of the media controller metadata
 * @param[in] attribute    The key attribute name to get
 * @param[out] value      The value of the attribute
 *
 * @return @c 0 on success,
 *         otherwise a negative error value
 *
 * @retval #MEDIA_CONTROLLER_ERROR_NONE Successful
 * @retval #MEDIA_CONTROLLER_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #MEDIA_CONTROLLER_ERROR_OUT_OF_MEMORY Out of memory
 *
 * @pre Call mc_client_set_metadata_updated_cb() or mc_client_get_server_metadata() function to get the metadata if you are media controller client. \n
 *         and also you can use this function when get the metadata when callback function invoked in mc_playlist_foreach_item().
 * @see mc_client_set_metadata_updated_cb()
 * @see mc_client_get_server_metadata()
 * @see mc_playlist_foreach_item()
 */
int mc_metadata_get(mc_metadata_h metadata, mc_meta_e attribute, char **value);

/**
 * @brief Encodes the season metadata of the media.
 * @details You can set various metadata with functions such as mc_server_set_metadata() and mc_server_add_item_to_playlist().
 *             If you want to set the season,\n
 *             you should encode the season with this function and then set the encoded data.\n
 *             Otherwise, the setting function returns an error. \n
 *             @a season_title can be NULL. If it is NULL, the decoded season title will also be NULL.\n
 * @since_tizen 5.5
 *
 * @remarks The @a encoded_season should be released using free(). \n
 *
 * @param[in] season_num    The season number
 * @param[in] season_title     The season title
 * @param[out] encoded_season         The encoded season information
 *
 * @return @c 0 on success,
 *         otherwise a negative error value
 *
 * @retval #MEDIA_CONTROLLER_ERROR_NONE Successful
 * @retval #MEDIA_CONTROLLER_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #MEDIA_CONTROLLER_ERROR_OUT_OF_MEMORY Out of memory
 *
 * @see mc_server_set_metadata()
 * @see mc_server_add_item_to_playlist()
 * @see mc_metadata_decode_season()
 */
int mc_metadata_encode_season(int season_num, const char *season_title, char **encoded_season);

/**
 * @brief Encodes the episode metadata of the media.
 * @details You can set various metadata with functions such as mc_server_set_metadata() and mc_server_add_item_to_playlist().
 *             If you want to set the episode,\n
 *             you should encode the episode with this function and then set the encoded data.\n
 *             Otherwise, the setting function returns an error. \n
 *             @a episode_title can be NULL. If it is NULL, the decoded episode title will also be NULL.\n
 * @since_tizen 5.5
 *
 * @remarks The @a encoded_episode should be released using free(). \n
 *
 * @param[in] episode_num    The episode number
 * @param[in] episode_title     The episode title
 * @param[out] encoded_episode         The encoded episode information
 *
 * @return @c 0 on success,
 *         otherwise a negative error value
 *
 * @retval #MEDIA_CONTROLLER_ERROR_NONE Successful
 * @retval #MEDIA_CONTROLLER_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #MEDIA_CONTROLLER_ERROR_OUT_OF_MEMORY Out of memory
 *
 * @see mc_server_set_metadata()
 * @see mc_server_add_item_to_playlist()
 * @see mc_metadata_decode_episode()
 */
int mc_metadata_encode_episode(int episode_num, const char *episode_title, char **encoded_episode);

/**
 * @brief Encodes the resolution metadata of the media.
 * @details You can set various metadata with functions such as mc_server_set_metadata() and mc_server_add_item_to_playlist().
 *             If you want to set the resolution,\n
 *             you should encode the resolution with this function and then set the encoded data.\n
 *             Otherwise, the setting function returns an error. \n
 * @since_tizen 5.5
 *
 * @remarks The @a encoded_resolution should be released using free(). \n
 *
 * @param[in] width                Content width
 * @param[in] height               Content height
 * @param[out] encoded_resolution         The encoded resolution information
 *
 * @return @c 0 on success,
 *         otherwise a negative error value
 *
 * @retval #MEDIA_CONTROLLER_ERROR_NONE Successful
 * @retval #MEDIA_CONTROLLER_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #MEDIA_CONTROLLER_ERROR_OUT_OF_MEMORY Out of memory
 *
 * @see mc_server_set_metadata()
 * @see mc_server_add_item_to_playlist()
 * @see mc_metadata_decode_resolution()
 */
int mc_metadata_encode_resolution(unsigned int width, unsigned int height, char **encoded_resolution);

/**
 * @brief Decodes the season of the media.
 * @details You can get various metadata using mc_metadata_get().
 *             If you want to get the proper season number and season title,\n
 *             you should decode the metadata values gotten by mc_metadata_get() with this function.\n
 *             Otherwise, the values are illegible. \n
 * @since_tizen 5.5
 *
 * @remarks The @a season_title should be released using free(). \n
 *
 * @param[in] encoded_season                The encoded season information
 * @param[out] season_num        The season number
 * @param[out] season_title         The season title
 *
 * @return @c 0 on success,
 *         otherwise a negative error value
 *
 * @retval #MEDIA_CONTROLLER_ERROR_NONE Successful
 * @retval #MEDIA_CONTROLLER_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #MEDIA_CONTROLLER_ERROR_OUT_OF_MEMORY Out of memory
 *
 * @pre Get the encoded season information by calling mc_metadata_get()
 *
 * @see mc_metadata_get()
 * @see mc_metadata_encode_season()
 */
int mc_metadata_decode_season(const char *encoded_season, int *season_num, char **season_title);

/**
 * @brief Decodes the episode of the media.
 * @details You can get various metadata using mc_metadata_get().
 *             If you want to get the proper episode number and episode title,\n
 *             you should decode the metadata values gotten by mc_metadata_get() with this function.\n
 *             Otherwise, the values are illegible. \n
 * @since_tizen 5.5
 *
 * @remarks The @a episode_title should be released using free(). \n
 *
 * @param[in] encoded_episode                The encoded episode information
 * @param[out] episode_num        The episode number
 * @param[out] episode_title         The episode title
 *
 * @return @c 0 on success,
 *         otherwise a negative error value
 *
 * @retval #MEDIA_CONTROLLER_ERROR_NONE Successful
 * @retval #MEDIA_CONTROLLER_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #MEDIA_CONTROLLER_ERROR_OUT_OF_MEMORY Out of memory
 *
 * @pre Get the encoded episode information by calling mc_metadata_get()
 *
 * @see mc_metadata_get()
 * @see mc_metadata_encode_episode()
 */
int mc_metadata_decode_episode(const char *encoded_episode, int *episode_num, char **episode_title);

/**
 * @brief Decodes the resolution of the media.
 * @details You can get various metadata using mc_metadata_get().
 *             If you want to get the proper resolution, \n
 *             you should decode the metadata values gotten by mc_metadata_get() with this function.\n
 *             Otherwise, the values are illegible. \n
 * @since_tizen 5.5
 *
 * @param[in] encoded_resolution    The encoded resolution information
 * @param[out] width        Content width
 * @param[out] height       Content height
 *
 * @return @c 0 on success,
 *         otherwise a negative error value
 *
 * @retval #MEDIA_CONTROLLER_ERROR_NONE Successful
 * @retval #MEDIA_CONTROLLER_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #MEDIA_CONTROLLER_ERROR_OUT_OF_MEMORY Out of memory
 *
 * @pre Get the encoded resolution information by calling mc_metadata_get()
 *
 * @see mc_metadata_get()
 * @see mc_metadata_encode_resolution()
 */
int mc_metadata_decode_resolution(const char *encoded_resolution, unsigned int *width, unsigned int *height);

/**
* @}
*/

/**
 * @addtogroup CAPI_MEDIA_CONTROLLER_ABILITY_MODULE
 * @{
 */

/**
 * @brief Clones a playback ability handle.
 * @details This function copies the playback ability handle from a source to
 *          destination. The #mc_playback_ability_h is created internally and available through playback ability functions.
 *
 * @since_tizen 5.0
 *
 * @remarks The @a dst should be released using mc_playback_ability_destroy().
 *
 * @param[in] src The source handle of the playback ability
 * @param[out] dst The destination handle of the playback ability
 *
 * @return @c 0 on success,
 *         otherwise a negative error value
 *
 * @retval #MEDIA_CONTROLLER_ERROR_NONE Successful
 * @retval #MEDIA_CONTROLLER_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #MEDIA_CONTROLLER_ERROR_OUT_OF_MEMORY Out of memory
 *
 * @pre Call the mc_client_set_playback_ability_updated_cb() or mc_client_get_server_playback_ability() function to get the ability if you are media controller client.
 *
 * @see mc_playback_ability_destroy()
 */
int mc_playback_ability_clone(mc_playback_ability_h src, mc_playback_ability_h *dst);

/**
 * @brief Destroys a playback ability handle.
 * @details This function frees all resources related to the playback ability handle. This
 *          handle no longer can be used to perform any operations. A new handle has to
 *          be created before next use.
 *
 * @since_tizen 5.0
 *
 * @param[in] ability        The handle of the playback ability
 *
 * @return @c 0 on success,
 *         otherwise a negative error value
 *
 * @retval #MEDIA_CONTROLLER_ERROR_NONE Successful
 * @retval #MEDIA_CONTROLLER_ERROR_INVALID_PARAMETER Invalid parameter
 *
 * @pre Call the mc_client_set_playback_ability_updated_cb() or mc_client_get_server_playback_ability() function to get the ability if you are media controller client.
 *
 * @see mc_playback_ability_clone()
 */
int mc_playback_ability_destroy(mc_playback_ability_h ability);

/**
 * @brief Gets the support value of the playback ability.
 * @since_tizen 5.0
 *
 * @remarks If the ability's support is not set, the result value is #MC_ABILITY_SUPPORTED_UNDECIDED.
 *
 * @param[in] ability    The handle of the playback ability
 * @param[in] action    The playback action to get
 * @param[out] supported      The support value of the ability
 *
 * @return @c 0 on success,
 *         otherwise a negative error value
 *
 * @retval #MEDIA_CONTROLLER_ERROR_NONE Successful
 * @retval #MEDIA_CONTROLLER_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #MEDIA_CONTROLLER_ERROR_OUT_OF_MEMORY Out of memory
 *
 * @pre Call the mc_client_set_playback_ability_updated_cb() or mc_client_get_server_playback_ability() function to get the ability if you are media controller client.
 *
 * @see mc_client_set_playback_ability_updated_cb()
 * @see mc_client_get_server_playback_ability()
 */
int mc_playback_action_is_supported(mc_playback_ability_h ability, mc_playback_action_e action, mc_ability_support_e *supported);

/**
* @}
*/

/**
 * @addtogroup CAPI_MEDIA_CONTROLLER_SEARCH_MODULE
 * @{
 */

/**
 * @brief Called for every search condition information in the obtained list of search.
 * @details Iterates over a search list.
 *
 * @since_tizen 5.0
 *
 * @remarks The callback is called in the main loop.
 *              The @a search_keyword and @a data should not be released by the application. \n
 *              The @a search_keyword and @a data can be used only in the callback. To use outside, make a copy.
 *
 * @param[in] content_type The content type to search
 * @param[in] category The category to search
 * @param[in] search_keyword The key word to search
 * @param[in] data The extra data
 * @param[in] user_data The user data passed from the foreach function
 *
 * @return @c true to continue with the next iteration of the loop,
 *         otherwise @c false to break out of the loop
 *
 * @pre mc_search_foreach_condition() will invoke this function.
 * @see mc_search_foreach_condition()
 */
typedef bool (*mc_search_condition_cb)(mc_content_type_e content_type, mc_search_category_e category, const char *search_keyword, bundle *data, void *user_data);

/**
 * @brief Creates a handle for searching media.
 * @since_tizen 5.0
 *
 * @remarks The @a search should be released using mc_search_destroy().
 *
 * @param[out] search The handle of the media controller search
 * @return @c 0 on success,
 *         otherwise a negative error value
 * @retval #MEDIA_CONTROLLER_ERROR_NONE Successful
 * @retval #MEDIA_CONTROLLER_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #MEDIA_CONTROLLER_ERROR_OUT_OF_MEMORY Out of memory
 *
 * @see mc_search_set_condition()
 * @see mc_search_foreach_condition()
 * @see mc_client_send_search_cmd()
 * @see mc_server_search_cmd_received_cb()
 * @see mc_server_set_search_cmd_received_cb()
 */
int mc_search_create(mc_search_h *search);

/**
 * @brief Sets the information to search.
 * @details Sets the information to search. You can set various search condition by calling this function for the same search handle. \n
 *            But it's not allowed to set more than 20 conditions. If you try to set more than 20 conditions, this function will return #MEDIA_CONTROLLER_ERROR_INVALID_PARAMETER.
 * @since_tizen 5.0
 *
 * @param[in] search The handle of the media controller search
 * @param[in] content_type The content type to search
 * @param[in] category The category to search
 * @param[in] search_keyword The key word to search
 * @param[in] data The extra data
 * @return @c 0 on success,
 *         otherwise a negative error value
 * @retval #MEDIA_CONTROLLER_ERROR_NONE Successful
 * @retval #MEDIA_CONTROLLER_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #MEDIA_CONTROLLER_ERROR_OUT_OF_MEMORY Out of memory
 *
 * @pre Call the mc_search_create() function to create handle if you are media controller client.
 * @see mc_search_foreach_condition()
 */
int mc_search_set_condition(mc_search_h search, mc_content_type_e content_type, mc_search_category_e category, const char *search_keyword, bundle *data);

/**
 * @brief Destroys search.
 * @since_tizen 5.0
 * @param[in] search The handle of the media controller search
 * @return @c 0 on success,
 *         otherwise a negative error value
 * @retval #MEDIA_CONTROLLER_ERROR_NONE Successful
 * @retval #MEDIA_CONTROLLER_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #MEDIA_CONTROLLER_ERROR_INVALID_OPERATION Invalid operation
 *
 * @pre Create a media controller search handle by calling mc_search_create().
 * @see mc_search_create()
 */
int mc_search_destroy(mc_search_h search);

/**
 * @brief Gets the number of conditions to search.
 * @since_tizen 5.5
 *
 * @param[in] search        The handle of the media controller search
 * @param[out] condition_count      The number of conditions.
 *
 * @return @c 0 on success,
 *         otherwise a negative error value
 *
 * @retval #MEDIA_CONTROLLER_ERROR_NONE Successful
 * @retval #MEDIA_CONTROLLER_ERROR_INVALID_PARAMETER Invalid parameter
 *
 * @see mc_search_create()
 * @see mc_search_set_condition()
 * @see mc_search_foreach_condition()
 */
int mc_search_get_condition_count(mc_search_h search, int *condition_count);

/**
 * @brief Retrieves all search conditions.
 * @details This function gets all search conditions set.
 *            The callback function will be invoked for every retrieved search condition.
 *            If there were no search conditions set with mc_search_set_condition(), this function will return #MEDIA_CONTROLLER_ERROR_NONE, and the callback will not be invoked.
 * @since_tizen 5.0
 *
 * @param[in] search    The handle of the media controller search
 * @param[in] callback      The callback function to be invoked.
 * @param[in] user_data   The user data to be passed to the callback function
 *
 * @return @c 0 on success,
 *         otherwise a negative error value
 *
 * @retval #MEDIA_CONTROLLER_ERROR_NONE Successful
 * @retval #MEDIA_CONTROLLER_ERROR_INVALID_PARAMETER Invalid parameter
 *
 * @pre Create a media controller search handle by calling mc_search_create() and set the search conditions by calling mc_search_set_condition() if you are media controller client.
 * @pre Receive search command from the media controller client by calling mc_server_search_cmd_received_cb() if you are media controller server.
 * @see mc_search_create()
 * @see mc_search_set_condition()
 * @see mc_server_search_cmd_received_cb()
 */
int mc_search_foreach_condition(mc_search_h search, mc_search_condition_cb callback, void *user_data);

/**
 * @brief Clones a media controller search handle.
 * @details This function copies the media controller search handle from a source to
 *          destination. The #mc_search_h is created internally and available through media controller search functions.
 *
 * @since_tizen 5.0
 *
 * @remarks The @a dst should be released using mc_search_destroy().
 *
 * @param[in] src The source handle of the media controller search
 * @param[out] dst The destination handle of the media controller search
 *
 * @return @c 0 on success,
 *         otherwise a negative error value
 *
 * @retval #MEDIA_CONTROLLER_ERROR_NONE Successful
 * @retval #MEDIA_CONTROLLER_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #MEDIA_CONTROLLER_ERROR_OUT_OF_MEMORY Out of memory
 *
 * @see mc_search_destroy()
 * @see mc_server_search_cmd_received_cb()
 */
int mc_search_clone(mc_search_h src, mc_search_h *dst);

/**
* @}
*/

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* __TIZEN_MULTIMEDIA_MEDIA_CONTROLLER_METADATA_H__ */
